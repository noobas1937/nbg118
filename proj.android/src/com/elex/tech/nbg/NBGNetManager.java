package com.elex.tech.nbg;

import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import org.cocos2dx.lib.Cocos2dxActivity;

import sfs2x.client.SmartFox;
import sfs2x.client.core.BaseEvent;
import sfs2x.client.core.IEventListener;
import sfs2x.client.core.SFSEvent;
import sfs2x.client.requests.LoginRequest;
import android.util.Log;

import com.smartfoxserver.v2.entities.data.ISFSArray;
import com.smartfoxserver.v2.entities.data.ISFSObject;
import com.smartfoxserver.v2.entities.data.SFSArray;
import com.smartfoxserver.v2.entities.data.SFSDataWrapper;
import com.smartfoxserver.v2.entities.data.SFSObject;
import com.smartfoxserver.v2.exceptions.SFSException;

public class NBGNetManager implements IEventListener {
	private static NBGNetManager m_instance = null;
	
	private static Cocos2dxActivity m_activity;
	private static SmartFox m_sfsClient = null;
	private static boolean m_debug = true;
	
	public static NBGNetManager getInstance() {
		if (m_instance == null) {
			m_instance = new NBGNetManager();
		}
		return m_instance;
	}
	
	public static void init(Cocos2dxActivity activity) {
		m_activity = activity;
		
		if (m_sfsClient == null) {
			m_sfsClient = new SmartFox(nativeIsDebugMode());
		}
		m_sfsClient.setUseBlueBox(true);
		
		// Add event listeners
		m_sfsClient.addEventListener(SFSEvent.CONNECTION, m_instance);
		m_sfsClient.addEventListener(SFSEvent.CONNECTION_LOST, m_instance);
		m_sfsClient.addEventListener(SFSEvent.LOGIN, m_instance);
		m_sfsClient.addEventListener(SFSEvent.LOGIN_ERROR, m_instance);
		m_sfsClient.addEventListener(SFSEvent.ROOM_JOIN, m_instance);
		m_sfsClient.addEventListener(SFSEvent.HANDSHAKE, m_instance);
		m_sfsClient.addEventListener(SFSEvent.SOCKET_ERROR, m_instance);
		
		if (m_debug) Log.v("NBG", "SmartFox created:" + m_sfsClient.isConnected() + " BlueBox enabled=" + m_sfsClient.useBlueBox());
	}
	
	public static void connect(String ip_addr, int port) {
		try {
			Class.forName("sfs2x.client.controllers.system.ResHandshake", true, Thread.currentThread().getContextClassLoader());
		} catch (ClassNotFoundException e) {
			if (m_debug) Log.d("NBG", "reset class loader");
			Thread.currentThread().setContextClassLoader(m_sfsClient.getClass().getClassLoader());
		}
		if (m_sfsClient != null && m_sfsClient.isConnected() == false) {
			m_sfsClient.connect(ip_addr, port);
		}
	}
	
	public static void login(ISFSObject requestData) {
		String zone = requestData.getUtfString("zone");
		String username = requestData.getUtfString("username");
		String password = requestData.getUtfString("password");
		m_sfsClient.send(new LoginRequest(username, password, zone, requestData));
	}
	
	@Override
	public void dispatch(BaseEvent event) throws SFSException {
		if (m_debug) Log.v("NBG", "Dispatching " + event.getType() + " (arguments=" + event.getArguments() + ")");
		
		if (event.getType().equalsIgnoreCase(SFSEvent.CONNECTION)) {
			nativeLuaHandlerOnConnection(event.getArguments().get("success").equals(true) ? 1 : 0);
		} else if (event.getType().equalsIgnoreCase(SFSEvent.CONNECTION_LOST)) {
				
		} else if (event.getType().equalsIgnoreCase(SFSEvent.LOGIN)) {
			Map<String, Object> result = new HashMap<String, Object>();
			result.put("cmd", "login");
			ISFSObject params = (ISFSObject) event.getArguments().get("data");
			result.put("params", params);
			onResponse(result);
		} else if (event.getType().equalsIgnoreCase(SFSEvent.LOGIN_ERROR)) {
			Map<String, Object> result = new HashMap<String, Object>();
			result.put("cmd", "login");
			result.put("errorMessage", (String) event.getArguments().get("errorMessage"));
			result.put("errorCode", event.getArguments().get("errorCode").toString());
			onResponse(result);
		} else if (event.getType().equalsIgnoreCase(SFSEvent.EXTENSION_RESPONSE)) {
			onResponse(event.getArguments());
		} else if (event.getType().equalsIgnoreCase(SFSEvent.PUBLIC_MESSAGE)) {
			Map<String, Object> result = new HashMap<String, Object>();
			result.put("cmd", "push.chat");
            ISFSObject params = (ISFSObject) event.getArguments().get("data");
            params.putUtfString("msg", (String) event.getArguments().get("message"));
            result.put("params", params);
			onResponse(result);
		}
	}
	
	private static void onResponse(final Map<String, Object> response) {
		
		m_activity.runOnGLThread(new Runnable() {
			
			@Override
			public void run() {
				long luaValueDict = nativeNewLuaValueDict();
				
				String cmd = (String) response.get("cmd");
				nativeAddString2LuaValueDict(luaValueDict, "cmd", cmd);

				Object params = response.get("params");
				if (params != null) {
					long sub_luaValueDict = SFSObject2LuaValueDict((ISFSObject) params);
					nativeAddLuaValueDict2LuaValueDict(luaValueDict, "params", sub_luaValueDict);
				}
				
				Object errorMessage = response.get("errorMessage");
				if (errorMessage != null) {
					nativeAddString2LuaValueDict(luaValueDict, "errorMessage", (String) errorMessage);
				}
				
				Object errorCode = response.get("errorCode");
				if (errorCode != null) {
					nativeAddString2LuaValueDict(luaValueDict, "errorCode", (String) errorCode);
				}

				nativeOnResponse(luaValueDict);
			}
			
		});
	}
	
	// --------------------------------------------------------------------------------------------
	
	private static long SFSObject2LuaValueDict(ISFSObject obj) {
		long luaValueDict = nativeNewLuaValueDict();
		if (obj == null) return luaValueDict;
		for (Iterator<Entry<String, SFSDataWrapper>> it = obj.iterator(); it.hasNext();) {
            Entry<String, SFSDataWrapper> entry = it.next();

            String key = entry.getKey();
            SFSDataWrapper value = entry.getValue();

            switch (value.getTypeId()) {
            case BOOL:
            	nativeAddString2LuaValueDict(luaValueDict, key, (Boolean) value.getObject() ? "1" : "0");
                break;
            case BYTE:
            case FLOAT:
            case INT:
            case LONG:
            case SHORT:
            case UTF_STRING:
            	nativeAddString2LuaValueDict(luaValueDict, key, (String) value.getObject());
                break;
            case DOUBLE:
                    java.text.NumberFormat nf = java.text.NumberFormat.getInstance();   
                    nf.setGroupingUsed(false);  
                    nativeAddString2LuaValueDict(luaValueDict, key, nf.format(value.getObject()));
                break;
            case SFS_OBJECT:
                nativeAddLuaValueDict2LuaValueDict(luaValueDict, key, SFSObject2LuaValueDict((ISFSObject) value.getObject()));
                break;
            case SFS_ARRAY:
                nativeAddLuaValueArray2LuaValueDict(luaValueDict, key, SFSArray2LuaValueArray((ISFSArray) value.getObject()));
                break;
            case BOOL_ARRAY:
            case BYTE_ARRAY:
            case DOUBLE_ARRAY:
            case FLOAT_ARRAY:
            case INT_ARRAY:
            case LONG_ARRAY:
            case SHORT_ARRAY: 
            case UTF_STRING_ARRAY: {
                long lua_arr = nativeNewLuaValueArray();
                for (Object v : (Collection<?>) value.getObject()) {
                	nativeAddString2LuaValueArray(lua_arr, v.toString());
                }
                nativeAddLuaValueArray2LuaValueDict(luaValueDict, key, lua_arr);
            }
                break;
                
            case CLASS:
            case NULL:
            default:

                break;
            }
		}
		
		return luaValueDict;
	}
	
	private static long SFSArray2LuaValueArray(ISFSArray sfs_arr) {
		long lua_arr = nativeNewLuaValueArray();
		if (sfs_arr == null) return lua_arr;
		for (Iterator<SFSDataWrapper> it = sfs_arr.iterator(); it.hasNext();) {
            SFSDataWrapper value = it.next();
            switch (value.getTypeId()) {
            case BOOL:
            	nativeAddString2LuaValueArray(lua_arr, (Boolean) value.getObject() ? "1" : "0");
                break;
            case BYTE:
            case DOUBLE:
            case FLOAT:
            case INT:
            case LONG:
            case SHORT:
            case UTF_STRING:
            	nativeAddString2LuaValueArray(lua_arr, value.getObject().toString());
                break;
            case SFS_OBJECT:
            	nativeAddLuaValueDict2LuaValueArray(lua_arr, SFSObject2LuaValueDict((ISFSObject) value.getObject()));
                break;
            case SFS_ARRAY:
            	nativeAddLuaValueArray2LuaValueArray(lua_arr, SFSArray2LuaValueArray((ISFSArray) value.getObject()));
                break;
            case BOOL_ARRAY:
            case BYTE_ARRAY:
            case DOUBLE_ARRAY:
            case FLOAT_ARRAY:
            case INT_ARRAY:
            case LONG_ARRAY:
            case SHORT_ARRAY: 
            case UTF_STRING_ARRAY: {
                long arrayInC = nativeNewLuaValueArray();
                for (Object v : (Collection<?>) value.getObject()) {
                	nativeAddString2LuaValueArray(arrayInC, v.toString());
                }
                nativeAddLuaValueArray2LuaValueArray(lua_arr, arrayInC);
            }
                break;
            
            case CLASS:
            case NULL:
            default:
                break;
            }
        }
		return lua_arr;
	}
	
	// --------------------------------------------------------------------------------------------
	
	public static ISFSObject getSFSObject() {
		return new SFSObject();
	}

	public static ISFSArray getSFSArray() {
		return new SFSArray();
	}

	public static void addToIntArray(Collection<Integer> arr, int value) {
		arr.add(value);
	}
	
	// --------------------------------------------------------------------------------------------
	
	private static native boolean nativeIsDebugMode();
	private static native void nativeLuaHandlerOnConnection(int success);
	private static native void nativeOnResponse(long pLuaValueDict);
	
	private static native long nativeNewLuaValueDict();
	private static native long nativeNewLuaValueArray();
	
	private static native void nativeAddString2LuaValueDict(long pLuaValueDict, String key, String value);
	private static native void nativeAddLuaValueDict2LuaValueDict(long pLuaValueDict, String key, long value);
	private static native void nativeAddLuaValueArray2LuaValueDict(long pLuaValueDict, String key, long value);
	
	private static native void nativeAddString2LuaValueArray(long pLuaValueArray, String value);
	private static native void nativeAddLuaValueDict2LuaValueArray(long pLuaValueArray, long value);
	private static native void nativeAddLuaValueArray2LuaValueArray(long pLuaValueArray, long value);
}
