package org.nbg.IF;

import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Vector;

import sfs2x.client.SmartFox;
import sfs2x.client.core.BaseEvent;
import sfs2x.client.core.IEventListener;
import sfs2x.client.core.SFSEvent;
import sfs2x.client.core.sockets.SocketEvent;
import sfs2x.client.requests.ExtensionRequest;
import sfs2x.client.requests.JoinRoomRequest;
import sfs2x.client.requests.LoginRequest;
import sfs2x.client.requests.LogoutRequest;
import android.text.TextUtils;
import android.util.Log;
import org.cocos2dx.ext.Jni;
import org.cocos2dx.ext.Native;
import org.cocos2dx.ext.Tracker;
import org.nbg.IF.NotifyUtilBase;
import org.nbg.IF.jsondeserializer.ChatServiceJsonDeSerializer;
import org.nbg.stac.empire.publish.IPublishChannel;
import org.nbg.util.AOEUtil;
import org.nbg.util.DeviceUtil;
import org.nbg.util.GameContext;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.JniController;
import com.google.android.apps.analytics.AdInstallRefMointor;
import com.smartfoxserver.v2.entities.data.ISFSArray;
import com.smartfoxserver.v2.entities.data.ISFSObject;
import com.smartfoxserver.v2.entities.data.SFSArray;
import com.smartfoxserver.v2.entities.data.SFSDataWrapper;
import com.smartfoxserver.v2.entities.data.SFSObject;
import com.smartfoxserver.v2.exceptions.SFSException;

public class Net implements IEventListener {
	private static String mIp = "s1.dc.elexapp.com"; // S1_ACCOUNT_IP
	private static String mZone = "COK1";
	private static int mPort = 80;
	private static int mPort2 = 8088;// 备用登陆端口

	public static SmartFox sfsClient;
	public static boolean debug = false;
	private static Net instance;

	public synchronized static Net getInstance() {
		if (instance == null) {
			instance = new Net();
		}
		return instance;
	}

    public static String getIP() {
        return mIp;
    }

    public static int getPort() {
        return mPort;
    }
	
	@Override
	public void dispatch(BaseEvent event) throws SFSException {
		Log.d(Jni.LOG_TAG, "dispatch got event " + event.getType());
		if (event.getType().equalsIgnoreCase(SFSEvent.EXTENSION_RESPONSE)) {
			// 各种网络命令
			onResponse(event.getArguments());
		} else if (event.getType().equalsIgnoreCase(SFSEvent.CONNECTION_LOST)
//				|| event.getType().equalsIgnoreCase(SFSEvent.SOCKET_ERROR)
//				|| event.getType().equalsIgnoreCase(SocketEvent.OnError)
				|| event.getType().equalsIgnoreCase(SocketEvent.OnDisconnect)) {
			// 连接丢失
			if("kick".equals(event.getArguments().get("reason"))){
				Native.postNotification("LoginKick");
			}
			Log.d("Net", "COK Net dispatch >>>>>> CONNECTION_LOST");
			releaseSfsClient();
			Native.postNotification("NConnectionLost");
		} else if (event.getType().equalsIgnoreCase(SFSEvent.SOCKET_ERROR)
				|| event.getType().equalsIgnoreCase(SocketEvent.OnError)) {
			// 连接丢失
			Log.d("Net", "COK Net dispatch >>>>>> SOCKET_ERROR");
			releaseSfsClient();
			Log.e("Net.java", "type = " + event.getType());
			Native.postNotification("sfs_connect_error");

		} else if (event.getType().equalsIgnoreCase(SFSEvent.CONNECTION)) {
			nativeConnectMode(sfsClient.getConnectionMode());
			// 连接成功
			if (event.getArguments().get("success").equals(true)){
				newConnection = false;
				Native.postNotification("NConnected");
			}
			else{
				// 连接丢失
				Log.d("Net", "COK Net dispatch >>>>>> CONNECTION not success");
	    		releaseSfsClient();
				Native.postNotification("NConnectionLost");
			}
		}else if (event.getType().equalsIgnoreCase(SFSEvent.LOGIN_ERROR)) {
			// 登录失败
			Map<String, Object> result = new HashMap<String, Object>();
			result.put("cmd", "login");
			ISFSObject params = new SFSObject();
			params.putUtfString("errorMessage", (String) event.getArguments()
					.get("errorMessage"));
			 
			if (event.getArguments().containsKey("errorCode")) {
				short errorcode = (Short) event.getArguments().get("errorCode");
				params.putShort("errorCode",errorcode);
			}
			
			result.put("params", params);
			
			onResponse(result);
		} else if (event.getType().equalsIgnoreCase(SFSEvent.LOGIN)) {
			// 登录成功
			Map<String, Object> result = new HashMap<String, Object>();
			result.put("cmd", "login");
			ISFSObject params = (ISFSObject) event.getArguments().get("data");
			result.put("params", params);
			onResponse(result);
		} else if (event.getType().equalsIgnoreCase(SFSEvent.LOGOUT)) {
			// 登出成功
			Map<String, Object> result = new HashMap<String, Object>();
			result.put("cmd", "logout");
			ISFSObject params = new SFSObject();
			params.putUtfString("zoneName", "");
			result.put("params", params);
			onResponse(result);
		} else if(event.getType().equalsIgnoreCase(SFSEvent.PUBLIC_MESSAGE)) {
            //接收国家聊天信息
//			setStatusText("SFS EVENT: " + event.getType() + "; Param:"
//                          + event.getArguments().toString());
            
            Map<String, Object> result = new HashMap<String, Object>();
			result.put("cmd", "push.chat");
//			ISFSObject params = new SFSObject();
            ISFSObject params = (ISFSObject) event.getArguments().get("data");
            params.putUtfString("msg", (String) event.getArguments().get("message"));
            result.put("params", params);
			onResponse(result);
            
        }else {
		}
	}

	public void releaseSfsClient(){
		Log.d("Net", "COK Net releaseSfsClient >>>>>> sfs release");
		if(sfsClient!=null){
			this.removeListener();
			sfsClient=null;
		}
	}
	
	private void addListener() {
		// 连接相关
		sfsClient.addEventListener(SFSEvent.CONNECTION, this);
		sfsClient.addEventListener(SFSEvent.CONNECTION_LOST, this);
		sfsClient.addEventListener(SFSEvent.SOCKET_ERROR, this);
		sfsClient.getSocketEngine().getSocket()
				.addEventListener(SocketEvent.OnError, this);
		sfsClient.getSocketEngine().getSocket()
				.addEventListener(SocketEvent.OnDisconnect, this);
		// 登录相关
		sfsClient.addEventListener(SFSEvent.EXTENSION_RESPONSE, this);
		sfsClient.addEventListener(SFSEvent.LOGIN, this);
		sfsClient.addEventListener(SFSEvent.LOGIN_ERROR, this);
		sfsClient.addEventListener(SFSEvent.LOGOUT, this);
        
        sfsClient.addEventListener(SFSEvent.PUBLIC_MESSAGE, this);

		// blue box
		sfsClient.addEventListener(SFSEvent.CONNECTION_ATTEMPT_HTTP, this);
	}

	private void removeListener() {
		sfsClient.removeEventListener(SFSEvent.CONNECTION, this);
		sfsClient.removeEventListener(SFSEvent.CONNECTION_LOST, this);
		sfsClient.removeEventListener(SFSEvent.SOCKET_ERROR, this);
		sfsClient.removeEventListener(SFSEvent.EXTENSION_RESPONSE, this);
		sfsClient.removeEventListener(SFSEvent.LOGIN, this);
		sfsClient.removeEventListener(SFSEvent.LOGIN_ERROR, this);
		sfsClient.removeEventListener(SFSEvent.LOGOUT, this);
        
        sfsClient.removeEventListener(SFSEvent.PUBLIC_MESSAGE, this);
		// bluebox
//		sfsClient.removeEventListener(SFSEvent.CONNECTION_ATTEMPT_HTTP, this);
	}

	private static void onResponse(final Map<String, Object> response) {
		IF.getInstance().runOnGLThread(new Runnable() {
			@Override
			public void run() {
				long responseForC = nativeCCDictionary();
				nativeAddStringToCCDictionary(responseForC, "cmd",
						(String) response.get("cmd"));
				nativeAddCCObjectToCCDictionary(responseForC, "params",
						sfsObjectToCCDictionary((ISFSObject) response
								.get("params")));


				nativeOnResponse(responseForC);
			}
		});
	}
	
//	public static void transportMailInfo(final String jsonStr)
//	{
//		try
//		{
//			long responseForC = nativeCCDictionary();
//			SFSObject obj = ChatServiceJsonDeSerializer.getInstance().newFromJsonData(jsonStr);
//			nativeAddCCObjectToCCDictionary(responseForC, "mailInfo", sfsObjectToCCDictionary(obj));
//			ChatServiceController.getInstance().host.transportMailInfo(responseForC);
//		}
//		catch (Exception e)
//		{
//			System.out.println("Net transportMailInfo json error:"+jsonStr);
//		}
//	}
	
	public static void transportMailInfo(final String jsonStr, final boolean isShowDetect)
	{
		IF.getInstance().runOnGLThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					long responseForC = nativeCCDictionary();
					SFSObject obj = ChatServiceJsonDeSerializer.getInstance().newFromJsonData(jsonStr);
					nativeAddCCObjectToCCDictionary(responseForC, "mailInfo", sfsObjectToCCDictionary(obj));
					// ChatServiceController.getInstance().host.transportMailInfo(responseForC);
					JniController.getInstance().excuteJNIVoidMethod("transportMailInfo",
							new Object[] { Long.valueOf(responseForC), Boolean.valueOf(isShowDetect) });
				}
				catch (Exception e)
				{
					System.out.println("Net transportMailInfo json error:" + jsonStr);
				}
			}
		});
	}
	
	public static String getParseFromCocos2dx(final String jsonStr)
	{
		String ret = "";
		try
		{
			long responseForC = nativeCCDictionary();
			SFSObject obj = ChatServiceJsonDeSerializer.getInstance().newFromJsonData(jsonStr);
			nativeAddCCObjectToCCDictionary(responseForC, "mailInfo", sfsObjectToCCDictionary(obj));
			ret = ChatServiceController.getInstance().host.getParseNameAndContent(responseForC);
		}
		catch (Exception e)
		{
			System.out.println("Net getParseFromCocos2dx json error:"+jsonStr);
		}
		return ret;
	}

	// ******** JNI methods ********

	private native static void nativeOnResponse(long response);

	private native static long nativeCCDictionary();

	private native static void nativeAddCCObjectToCCDictionary(long dict,
			String key, long value);

	private native static void nativeAddStringToCCDictionary(long dict,
			String key, String value);

	private native static long nativeCCArray();

	private native static void nativeConnectMode(String value);
	
	private native static void nativeAddStringToCCArray(long array, String value);

	private native static void nativeAddCCObjectToCCArray(long dict, long value);
	
	private static long sfsArrayToCCArray(ISFSArray obj) {
		long responseForC = nativeCCArray();
		for (Iterator<SFSDataWrapper> it = obj.iterator(); it.hasNext();) {
			SFSDataWrapper value = it.next();
			switch (value.getTypeId()) {
			case BOOL:
				nativeAddStringToCCArray(responseForC,
						(Boolean) value.getObject() ? "1" : "0");
				break;
			case BYTE:
			case DOUBLE:
			case FLOAT:
			case INT:
			case LONG:
			case SHORT:
				nativeAddStringToCCArray(responseForC, value.getObject()
						.toString());
				break;
			case UTF_STRING:
				nativeAddStringToCCArray(responseForC,
						(String) value.getObject());
				break;
			case SFS_OBJECT:
				nativeAddCCObjectToCCArray(responseForC,
						sfsObjectToCCDictionary((ISFSObject) value.getObject()));
				break;
			case SFS_ARRAY:
				nativeAddCCObjectToCCArray(responseForC,
						sfsArrayToCCArray((ISFSArray) value.getObject()));
				break;
			case BOOL_ARRAY:
			case BYTE_ARRAY:
			case DOUBLE_ARRAY:
			case FLOAT_ARRAY:
			case INT_ARRAY:
			case LONG_ARRAY:
			case SHORT_ARRAY: {
				long arrayInC = nativeCCArray();
				for (Object v : (Collection<?>) value.getObject()) {
					nativeAddStringToCCArray(arrayInC, v.toString());
				}
				nativeAddCCObjectToCCArray(responseForC, arrayInC);
			}
				break;
			case UTF_STRING_ARRAY: {
				long arrayInC = nativeCCArray();
				for (Object v : (Collection<?>) value.getObject()) {
					nativeAddStringToCCArray(arrayInC, (String) v);
				}
				nativeAddCCObjectToCCArray(responseForC, arrayInC);
			}
				break;
			case CLASS:
				Log.d("Net sfsArrayToCCArray"," CLASS");
			case NULL:
				Log.d("Net sfsArrayToCCArray"," NULL");
			default:
//				nativeAddStringToCCArray(responseForC, value.getObject()
//						.toString());
				break;
			}
		}
		return responseForC;
	}

	private static long sfsObjectToCCDictionary(ISFSObject obj) {
		long responseForC = nativeCCDictionary();
		if( obj == null)
		{
			return responseForC;
		}
		for (Iterator<Entry<String, SFSDataWrapper>> it = obj.iterator(); it
				.hasNext();) {
			Entry<String, SFSDataWrapper> entry = it.next();

			String key = entry.getKey();
			SFSDataWrapper value = entry.getValue();

			switch (value.getTypeId()) {
			case BOOL:
				nativeAddStringToCCDictionary(responseForC, key,
						(Boolean) value.getObject() ? "1" : "0");
				break;
			case BYTE:
			case FLOAT:
			case INT:
			case LONG:
			case SHORT:
				nativeAddStringToCCDictionary(responseForC, key, value
						.getObject().toString());
				break;
			case DOUBLE:
					java.text.NumberFormat nf = java.text.NumberFormat.getInstance();   
					nf.setGroupingUsed(false);  
					nativeAddStringToCCDictionary(responseForC, key, nf.format(value.getObject()));
				break;
			case UTF_STRING:
				nativeAddStringToCCDictionary(responseForC, key,
						(String) value.getObject());
				break;
			case SFS_OBJECT:
				nativeAddCCObjectToCCDictionary(responseForC, key,
						sfsObjectToCCDictionary((ISFSObject) value.getObject()));
				break;
			case SFS_ARRAY:
				nativeAddCCObjectToCCDictionary(responseForC, key,
						sfsArrayToCCArray((ISFSArray) value.getObject()));
				break;
			case BOOL_ARRAY:
			case BYTE_ARRAY:
			case DOUBLE_ARRAY:
			case FLOAT_ARRAY:
			case INT_ARRAY:
			case LONG_ARRAY:
			case SHORT_ARRAY: {
				long arrayInC = nativeCCArray();
				for (Object v : (Collection<?>) value.getObject()) {
					nativeAddStringToCCArray(arrayInC, v.toString());
				}
				nativeAddCCObjectToCCDictionary(responseForC, key, arrayInC);
			}
				break;
			case UTF_STRING_ARRAY: {
				long arrayInC = nativeCCArray();
				for (Object v : (Collection<?>) value.getObject()) {
					nativeAddStringToCCArray(arrayInC, (String) v);
				}
				nativeAddCCObjectToCCDictionary(responseForC, key, arrayInC);
			}
				break;
			case CLASS:
				Log.d("Net sfsArrayToCCArray"," CLASS");
			case NULL:
				Log.d("Net sfsArrayToCCArray"," NULL");
			default:
//				nativeAddStringToCCDictionary(responseForC, key, value
//						.getObject().toString());
				break;
			}
		}
		return responseForC;
	}

	public static boolean isConnected() {
//		Log.d("Net", "COK Net isConnected >>>>>> sfs exist:" + (sfsClient != null?"true":"false"));
//		Log.d("Net", "COK Net isConnected >>>>>> connected:" + ((sfsClient != null && sfsClient.isConnected())?"true":"false"));
//		Log.d("Net", "COK Net isConnected >>>>>> connecting:" + ((sfsClient != null && sfsClient.isConnecting())?"true":"false"));
		boolean isconnected = false;
		//初始化连接握手时间可能比较长，一直维持connecting状态
		if(newConnection)
			isconnected = sfsClient != null && sfsClient.isConnected() && !sfsClient.isConnecting();
		else
			isconnected	= sfsClient != null && sfsClient.isConnected();
		return isconnected;
	}
	
	public static boolean isNetWorkOK() {
		return AOEUtil.isNetworkAvailable(IF.getInstance());
	}
	
	private static boolean newConnection = false;

	public static void connect(String ip, int port, String zone) {
		if (!TextUtils.isEmpty(ip))
			mIp = ip;
		if (port > 0)
			mPort = port;
		if (!TextUtils.isEmpty(zone))
			mZone = zone;
		if (AOEUtil.isNetworkAvailable(IF.getInstance())) {
			if (sfsClient == null) {
				sfsClient = new SmartFox(debug);
				getInstance().addListener();
//				if(!IPublishChannel.PUBLISH_GLOBAL.equals(IF.getInstance().getPublishImpl().getPublish_Region()))
					newConnection = true;
				Log.d("Net", "COK Net Connect >>>>>> start sfsClient");
			}
			sfsClient.setUseBlueBox(false);
			Log.d("Net", "COK Net Connect >>>>>> ip:" + mIp + ", zone:" + mZone + ", port:" + mPort);
//			sfsClient.connect(mIp, mPort);
            // async method
			try {
				// 提前检验是否能找到 class
				Class handlerClass = Class.forName("sfs2x.client.controllers.system.ResHandshake", true, Thread.currentThread().getContextClassLoader());
			} catch (ClassNotFoundException e) {
				Log.d("aowe", "reset class loader");
				Thread.currentThread().setContextClassLoader(sfsClient.getClass().getClassLoader());
			}
			sfsClient.connect(mIp, mPort);
//            new NetTask().execute(instance);
		} else {
			Log.d("Net", "COK Net Connect >>>>>> Network Unavailable");
			Native.postNotification("NConnectionLost");
		}
	}
	
	private static void stopConnecting(){
		if(sfsClient != null && sfsClient.getSocketEngine() != null && sfsClient.getSocketEngine().getSocket() != null){
			sfsClient.getSocketEngine().getSocket().disconnect();
		}
	}

	public static void disconnect() {
		Log.d("Net", "COK Net disconnect >>>>>> " + (sfsClient == null ? "null" : ""));
		if(sfsClient != null)
		{
			sfsClient.disconnect();
			getInstance().releaseSfsClient();
		}
	}

	public static void logout() {
		Log.d("Net", "COK Net logout >>>>>> " + sfsClient == null ? "null" : "");
		if(sfsClient != null && sfsClient.isConnected() && !sfsClient.getSocketEngine().getUseBlueBox())
		{
			Log.d("Net", "COK Net logout2 >>>>>> " + sfsClient == null ? "null" : "");
			sfsClient.send(new LogoutRequest());
		}
	}

	public static void login(final ISFSObject sfso, final String username,
			final String passwd) {
		System.out.println("[Net.login] username:" + username + " passwd:" + passwd + ", ip:" + getIP());
		IF.getInstance().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				/** GCM 注册信息 */
				// if (AppData.getInstance().getGcmRegisterId() != null)
				// sfso.putUtfString("gcm_regid", AppData.getInstance()
				// .getGcmRegisterId());
				NotifyUtilBase.sendParseChannel(sfso.getUtfString("serverId"));
				NotifyUtilBase.sendParseLang(sfso.getUtfString("lang"));
				// 广告跟踪referrer
				String referrer = AdInstallRefMointor
						.checkAdInstalledRef(GameContext.getActivityInstance());
				if (referrer != null) {
					sfso.putUtfString("referrer", referrer);
				}

				// 地区，app_version,服务端改用IP方式判断
				String region = GameContext.getActivityInstance()
						.getResources().getConfiguration().locale.getCountry();// Locale.getDefault().getCountry();//eg:CN
				sfso.putUtfString("region", region);
				sfso.putSFSObject("device_info", DeviceUtil
						.getDeviceInfo(GameContext.getActivityInstance()));

				if (sfsClient != null){
					sfsClient.send(new LoginRequest(username, passwd, mZone, sfso));
				}else{
					return;
				}
			}
		});
	}

	public static void send(String cmd, ISFSObject obj) {
		System.out.println("[Net.send] cmd:"+cmd);
//        new NetSendTask(cmd,obj).execute(instance);
		if(sfsClient != null)
		{
			try{
				sfsClient.send(new ExtensionRequest(cmd, obj));
			}catch(java.lang.NullPointerException e){
				//网络中断等内部原因，导致
				// 连接丢失
				Log.d("Net", "COK Net Send >>>>>> NullPointerException");
				getInstance().releaseSfsClient();
				Native.postNotification("NConnectionLost");
				Runtime.getRuntime().gc();
			}
		}
	}
    
    public static void joinRoom(final String name) {
    	if(sfsClient != null)
    	{
    		sfsClient.send(new JoinRoomRequest(name));
    	}
	}

	public static ISFSObject getSFSObject() {
		return new SFSObject();
	}

	public static ISFSArray getSFSArray() {
		return new SFSArray();
	}

	public static Collection<Integer> getIntArray() {
		return new Vector<Integer>();
	}

	public static void addToIntArray(Collection<Integer> arr, int value) {
		arr.add(value);
	}
}
