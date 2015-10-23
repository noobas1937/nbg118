package com.clash.of.util;

import com.clash.of.kings.NBG_CN1;
import android.app.Application;
import android.util.Log;
import android.content.Intent;

import org.nbg.IF.NotifyUtilBase;

import com.parse.ParseAnalytics;
import com.parse.ParseInstallation;
import com.parse.PushService;
import com.parse.ParsePush;
import org.nbg.util.GameContext;

public class NotifyUtil extends NotifyUtilBase {

	private static String _parseNotifyToken = null;
	public static String getParseNotifyToken(){
		if(_parseNotifyToken == null){
			initParsePushNotification();
		}
		
		return _parseNotifyToken;
	}
	
	
	private static void initParsePushNotification(){
		try{
			Application app = GameContext.getActivityInstance().getApplication();
			// parse should init at application onCreate
//			ApplicationInfo appInfo = app.getPackageManager().getApplicationInfo(
//					app.getPackageName(),
//					PackageManager.GET_META_DATA);
//			Bundle metaData = appInfo.metaData;
//			String strAppId = metaData.getString("PARSE_ID");
//			String strAppKey = metaData.getString("PARSE_KEY");
//	    	Parse.initialize(app, strAppId, strAppKey);
//	    	ParseUser.enableAutomaticUser();
//	    	ParseACL defaultACL = new ParseACL();
//	    	ParseACL.setDefaultACL(defaultACL, true);
	    	ParseInstallation data = ParseInstallation.getCurrentInstallation();
	    	_parseNotifyToken = ((String)data.get("installationId"));
//	    	PushService.subscribe(app, "", NBG_CN1.class);
//	    	PushService.setDefaultPushCallback(app, NBG_CN1.class);
	    	data.saveInBackground();
		}catch(Exception e){
			e.printStackTrace();
		}catch(Throwable e){
			e.printStackTrace();
		}
    }
	
	public static void sendParseLang(String lang)
	{
		try{
			Log.d("APP", "sendParseLang");
	    	ParseInstallation data = ParseInstallation.getCurrentInstallation();
	    	data.put("lang", lang);
	    	data.saveInBackground();
		}catch(Exception e){
			e.printStackTrace();
		}catch(Throwable e){
			e.printStackTrace();
		}
	}
	
	public static void sendParseChannel(String Channel)
	{
		try{
			Log.d("APP", "sendParseChannel");
	    	ParseInstallation data = ParseInstallation.getCurrentInstallation();
	    	data.put("Channels", Channel);
	    	data.saveInBackground();
		}catch(Exception e){
			e.printStackTrace();
		}catch(Throwable e){
			e.printStackTrace();
		}
	}
	
	public static void sendTest(){
		long weekInterval = 60*60*24*7; // 1 week
		 
		// Send push notification with expiration interval
		ParsePush push = new ParsePush();
		push.setExpirationTimeInterval(60);
		//push.setQuery(everyoneQuery);
		push.setChannel("happyfarm");
		push.setMessage("Season tickets on sale until December 9th");
		push.sendInBackground();
		
	}
	public static void trackAppOpened(Intent intent){
		ParseAnalytics.trackAppOpened(intent);
	}
	
}
