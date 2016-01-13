package com.codebutler.android_websockets;

import java.io.BufferedInputStream;
import java.io.EOFException;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.URI;
import java.net.URL;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import javax.net.SocketFactory;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLException;
import javax.net.ssl.SSLSocketFactory;
import javax.net.ssl.TrustManager;
import javax.net.ssl.TrustManagerFactory;

import org.apache.commons.lang.StringUtils;
import org.apache.http.Header;
import org.apache.http.HttpException;
import org.apache.http.HttpStatus;
import org.apache.http.NameValuePair;
import org.apache.http.StatusLine;
import org.apache.http.client.HttpResponseException;
import org.apache.http.message.BasicLineParser;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONObject;

import android.content.res.AssetManager;
import android.os.Handler;
import android.os.HandlerThread;
import android.text.TextUtils;
import android.util.Base64;
import android.util.Log;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.util.LogUtil;

public class WebSocketClient {
	private static final String			TAG			= "WebSocketClient";

	private URI							mURI;
	private Listener					mListener;
	private Socket						mSocket;
	private Thread						mThread;
	private HandlerThread				mHandlerThread;
	private Handler						mHandler;
	private List<BasicNameValuePair>	mExtraHeaders;
	private HybiParser					mParser;
	private boolean						mConnected;
	private ScheduledExecutorService	heartbeatService;
	private String						clientID;

	private final Object				mSendLock	= new Object();

	private static TrustManager[]		sTrustManagers;

    public static void setTrustManagers(TrustManager[] tm) {
        sTrustManagers = tm;
    }

    public WebSocketClient(URI uri, Listener listener, List<BasicNameValuePair> extraHeaders) {
		mURI = uri;
		mListener = listener;
		if (extraHeaders != null)
			mExtraHeaders = extraHeaders;
		mConnected = false;
		mParser = new HybiParser(this);

		mHandlerThread = new HandlerThread("websocket-thread");
		mHandlerThread.start();
		mHandler = new Handler(mHandlerThread.getLooper());
        
        heartbeatService = Executors.newSingleThreadScheduledExecutor();
        TimerTask timerTask = new TimerTask()
		{
			@Override
			public void run()
			{
				try
				{
					sendKeepAlive();
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		};
		
		heartbeatService.scheduleWithFixedDelay(timerTask, 10000, 15 * 1000, TimeUnit.MILLISECONDS);
    }
	
	private void sendKeepAlive()
	{
		if(isConnected()){
			LogUtil.printVariablesWithFuctionName(Log.VERBOSE, LogUtil.TAG_WS_SEND);
			send("heartbeat");
		}
	}
    
    public void setExtraHeaders(List<BasicNameValuePair> extraHeaders)
    {
        mExtraHeaders = extraHeaders;
    }

    public Listener getListener() {
        return mListener;
    }
    
    private static boolean isWSS(URI uri)
    {
    	return uri != null && uri.getScheme().equals("wss");
    }
    
    private static boolean isHTTPS(URI uri)
    {
    	return uri != null && uri.getScheme().equals("https");
    }

    public synchronized void connect() {
        if (mThread != null && mThread.isAlive()) {
            return;
        }

        mThread = new Thread(new Runnable() {
            @Override
            public void run() {
            	doConnect();
            }
        });
        mThread.start();
    }

    private static int connectionTime = 0;
	private void doConnect()
	{
		try
		{
			connectionTime++;
			forceDisconnect = false;
			LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_WS_STATUS, "connectionTime", connectionTime);
			
			int port = (mURI.getPort() != -1) ? mURI.getPort() : ((isWSS(mURI) || isHTTPS(mURI)) ? 443 : 80);

			String path = TextUtils.isEmpty(mURI.getPath()) ? "/" : mURI.getPath();
			if (!TextUtils.isEmpty(mURI.getQuery()))
			{
				path += "?" + mURI.getQuery();
			}

			String originScheme = isWSS(mURI) ? "https" : "http";
			URI origin = new URI(originScheme, "//" + mURI.getHost(), null);

			SocketFactory factory = (isWSS(mURI) || isHTTPS(mURI)) ? getSSLSocketFactory() : SocketFactory.getDefault();
			mSocket = factory.createSocket(mURI.getHost(), port);

			PrintWriter out = new PrintWriter(mSocket.getOutputStream());
			String secretKey = createSecret();
			out.print("GET " + path + " HTTP/1.1\r\n");
			out.print("Upgrade: websocket\r\n");
			out.print("Connection: Upgrade\r\n");
			out.print("Host: " + mURI.getHost() + "\r\n");
			out.print("Origin: " + origin.toString() + "\r\n");
			out.print("Sec-WebSocket-Key: " + secretKey + "\r\n");
			out.print("Sec-WebSocket-Version: 13\r\n");
			if (mExtraHeaders != null)
			{
				for (NameValuePair pair : mExtraHeaders)
				{
					String extra = String.format("%s: %s\r\n", pair.getName(), pair.getValue());
					out.print(extra);
				}
			}
			out.print("\r\n");
			out.flush();

			HybiParser.HappyDataInputStream stream = new HybiParser.HappyDataInputStream(mSocket.getInputStream());

			// Read HTTP response status line.
			StatusLine statusLine = parseStatusLine(readLine(stream));
			if (statusLine == null)
			{
				throw new HttpException("Received no reply from server.");
			}
			else if (statusLine.getStatusCode() != HttpStatus.SC_SWITCHING_PROTOCOLS)
			{
				throw new HttpResponseException(statusLine.getStatusCode(), statusLine.getReasonPhrase());
			}

			// Read HTTP response headers.
			String line;
			while (!TextUtils.isEmpty(line = readLine(stream)))
			{
				Header header = parseHeader(line);
				if (header.getName().equals("Sec-WebSocket-Accept"))
				{
					String expected = expectedKey(secretKey);
					if (expected == null)
					{
						throw new Exception("SHA-1 algorithm not found");
					}
					else if (!expected.equals(header.getValue()))
					{
						throw new Exception("Invalid Sec-WebSocket-Accept, expected: " + expected + ", got: " + header.getValue());
					}
				}
			}

			reconnectRetryCount = RECONNECT_MAX_RETRY;

			mConnected = true;

			mListener.onConnect();

			// Now decode websocket frames.
			mParser.start(stream);
		}
		catch (EOFException ex)
		{
			Log.d(TAG, "WebSocket EOF!", ex); // 后台重启会触发
			mListener.onDisconnect(0, "EOF");
			mConnected = false;
			LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_WS_STATUS, "onConnectError: WebSocket EOF error");
			onConnectError();
		}
		catch (SSLException ex)
		{
			// Connection reset by peer
			Log.d(TAG, "Websocket SSL error!", ex);
//			LogUtil.printException(ex);
			mListener.onDisconnect(0, "SSL");
			mConnected = false;
			LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_WS_STATUS, "onConnectError: WebSocket SSL error");
			onConnectError();
		}
		catch (Exception ex)
		{
			mListener.onError(ex);
			LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_WS_STATUS, "onConnectError: WebSocket Exception error");
			onConnectError();
		}
	}
	
	public synchronized void onConnectError()
	{
		synchronized (this)
		{
			if (reconnectCountDown == 0 && reconnectRetryCount >= 0 && !forceDisconnect)
			{
				reconnectCountDown = RECONNECT_INTERVAL + 1;
				resetClientID();
				startReconnect();
			}
		}
	}
	
	private boolean forceDisconnect = false;
	private int	reconnectRetryCount = RECONNECT_MAX_RETRY;
	private final static int RECONNECT_INTERVAL = 5;
	private final static int RECONNECT_MAX_RETRY = 99;
	private int reconnectCountDown = 0;

	private ScheduledExecutorService reconnectService;
	private TimerTask reconnectTimerTask;
	
	/**
	 * 只会执行一次
	 */
	private synchronized void startReconnect()
	{
		if (reconnectService != null)
		{
			return;
		}

		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_WS_STATUS);

		reconnectService = Executors.newSingleThreadScheduledExecutor();
		reconnectTimerTask = new TimerTask()
		{
			@Override
			public void run()
			{
				try
				{
					checkReconnect();
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		};

		reconnectService.scheduleWithFixedDelay(reconnectTimerTask, 1000, 1000, TimeUnit.MILLISECONDS);
	}
	
	private synchronized void checkReconnect()
	{
		synchronized (this)
		{
			if(reconnectCountDown <= 0) return;
			
			reconnectCountDown--;
			if(reconnectCountDown <= 0){
				mListener.onStatus(LanguageManager.getLangByKey(LanguageKeys.WEB_SOCKET_RECONNECTING));
				reconnectRetryCount--;
				connect();
			}else{
				mListener.onStatus(LanguageManager.getLangByKey(LanguageKeys.WEB_SOCKET_CONNECT_FAIL, reconnectCountDown + ""));
			}
		}
	}

	/**
	 * 会使得mParser.start(stream)循环发生java.net.SocketException: Socket closed
	 */
    public void disconnect() {
        if (mSocket != null) {
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    if (mSocket != null) {
                    	forceDisconnect = true;
                        try {
                            mSocket.close();
                        } catch (IOException ex) {
                            Log.d(TAG, "Error while disconnecting", ex);
                            mListener.onError(ex);
                        }
                        mSocket = null;
                    }
                    mConnected = false;
                }
            });
        }
    }

    public void send(String data) {
        sendFrame(mParser.frame(data));
    }

    public void send(byte[] data) {
        sendFrame(mParser.frame(data));
    }

    public boolean isConnected() {
        return mConnected;
    }

    private StatusLine parseStatusLine(String line) {
        if (TextUtils.isEmpty(line)) {
            return null;
        }
        return BasicLineParser.parseStatusLine(line, new BasicLineParser());
    }

    private Header parseHeader(String line) {
        return BasicLineParser.parseHeader(line, new BasicLineParser());
    }

    // Can't use BufferedReader because it buffers past the HTTP data.
    private String readLine(HybiParser.HappyDataInputStream reader) throws IOException {
        int readChar = reader.read();
        if (readChar == -1) {
            return null;
        }
        StringBuilder string = new StringBuilder("");
        while (readChar != '\n') {
            if (readChar != '\r') {
                string.append((char) readChar);
            }

            readChar = reader.read();
            if (readChar == -1) {
                return null;
            }
        }
        return string.toString();
    }

    private String expectedKey(String secret) {
        //concatenate, SHA1-hash, base64-encode
        try {
            final String GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
            final String secretGUID = secret + GUID;
            MessageDigest md = MessageDigest.getInstance("SHA-1");
            byte[] digest = md.digest(secretGUID.getBytes());
            return Base64.encodeToString(digest, Base64.DEFAULT).trim();
        } catch (NoSuchAlgorithmException e) {
            return null;
        }
    }

    private String createSecret() {
        byte[] nonce = new byte[16];
        for (int i = 0; i < 16; i++) {
            nonce[i] = (byte) (Math.random() * 256);
        }
        return Base64.encodeToString(nonce, Base64.DEFAULT).trim();
    }

    void sendFrame(final byte[] frame) {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                try {
                    synchronized (mSendLock) {
                    	// 可能有Fatal Exception: java.lang.NullPointerException 
                        OutputStream outputStream = mSocket.getOutputStream();
                        outputStream.write(frame);
                        outputStream.flush();
                    }
                } catch (IOException e) {
                    mListener.onError(e);
        			LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_WS_STATUS, "onConnectError: sendFrame IOException error");
        			onConnectError();
                } catch (Exception e) {
                    mListener.onError(e);
        			LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_WS_STATUS, "onConnectError: sendFrame Exception error");
        			onConnectError();
                }
            }
        });
    }

    public interface Listener {
        public void onConnect();
        public void onMessage(String message);
        public void onMessage(byte[] data);
        public void onDisconnect(int code, String reason);
        public void onError(Exception error);
    	public void onStreamInput();
        public void onStatus(String status);
    }

    private SSLSocketFactory getSSLSocketFactory() throws NoSuchAlgorithmException, KeyManagementException {
        SSLContext context = SSLContext.getInstance("TLS");
        context.init(null, sTrustManagers, null);
        return context.getSocketFactory();
    }
	
	private String getClientID(JSONObject json)
	{
		try
		{
			return json.getString("server") + json.getString("clientid");
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		
		return null;
	}
    
    public boolean isMyMessage(JSONObject json)
    {
    	return StringUtils.isNotEmpty(clientID) && !clientID.equals(getClientID(json));
    }
	
	public void setClientID(JSONObject json)
	{
		clientID = getClientID(json);
	}
	
	private void resetClientID()
	{
		clientID = null;
	}
    
    public void destroy()
    {
    	heartbeatService.shutdown();
    	heartbeatService = null;
    	
    	reconnectService.shutdown();
    	reconnectService = null;
    }
}