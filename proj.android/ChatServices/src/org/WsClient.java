package org;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.Map;
import java.util.TimerTask;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.apache.commons.lang.StringUtils;
import org.java_websocket.client.WebSocketClient;
import org.java_websocket.drafts.Draft_10;
import org.java_websocket.handshake.ServerHandshake;
import org.json.JSONObject;

import android.util.Log;

import com.elex.chatservice.net.IWebSocketStatusListener;
import com.elex.chatservice.net.WebSocketManager;
import com.elex.chatservice.util.LogUtil;

public class WsClient extends WebSocketClient {
    public boolean isOpen = false;
    public boolean isClose = false;
    private IWebSocketStatusListener statusListener;
    private WebSocketManager webSocketManager;
	private ScheduledExecutorService	heartbeatService;
	private String						clientID;

    public WsClient(String serverURI, Map<String, String> header, WebSocketManager webSocketManager, IWebSocketStatusListener statusListener) throws URISyntaxException {
        super(new URI(serverURI), new Draft_10(), header, 1000);
        
        this.webSocketManager = webSocketManager;
        this.statusListener = statusListener;
        
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
		if(isOpen()){
			LogUtil.printVariablesWithFuctionName(Log.VERBOSE, LogUtil.TAG_WS_SEND);
			send("heartbeat");
		}
	}

    @Override
    public void onOpen(ServerHandshake serverHandshake) {
        isOpen = true;
        
    	LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_WS_STATUS, "Connected");
    	statusListener.onConsoleOutput("Connected");
		statusListener.onStatus("");
		webSocketManager.handleConnect();
    }

    @Override
    public void onMessage(String s) {
    	webSocketManager.handleMessage(s);
    }

    @Override
    public void onClose(int code, String reason, boolean b) {
        isClose = true;
        
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_WS_STATUS, String.format("Disconnected! Code: %d Reason: %s", code, reason));
		statusListener.onConsoleOutput(String.format("Disconnected! Code: %d Reason: %s", code, reason));
		webSocketManager.handleDisconnect();
		webSocketManager.onConnectError();
    }

    @Override
    public void onError(Exception e) {
    	LogUtil.printException(e);
		LogUtil.printVariablesWithFuctionName(Log.WARN, LogUtil.TAG_WS_STATUS, "error", e.getMessage());
		statusListener.onConsoleOutput("Error:" + e.getMessage());
		webSocketManager.onConnectError();
//		ServiceInterface.flyHint(null, "", "连接错误", 0, 0, false);
    }

//	@Override
//	public void onStreamInput()
//	{
//		statusListener.onStremInput();
//	}
//
//	@Override
//	public void onStatus(String status)
//	{
//		statusListener.onStatus(status);
//	}
	
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
    	
//    	reconnectService.shutdown();
//    	reconnectService = null;
    }
}
