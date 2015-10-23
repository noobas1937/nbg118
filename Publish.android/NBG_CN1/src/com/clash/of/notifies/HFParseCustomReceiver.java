package com.clash.of.notifies;


import java.util.Iterator;

import org.json.JSONException;
import org.json.JSONObject;

import org.nbg.notifies.*;
import com.hcg.cok.cn1.R;
import android.app.NotificationManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class HFParseCustomReceiver extends BroadcastReceiver {

	private static final String TAG = "HFParseCustomReceiver";
	
	@Override
	public void onReceive(Context context, Intent intent) {
		Intent notifyIntent = new Intent();
		try {
		      String action = intent.getAction();
		      String channel = intent.getExtras().getString("com.parse.Channel");
		      JSONObject json = new JSONObject(intent.getExtras().getString("com.parse.Data"));
		 
		      String message = "";
		      if(json.has(LocalNotificationManager.PUSH_MSG))
		    	  message = json.getString(LocalNotificationManager.PUSH_MSG);
		      else
		    	  message = json.getString("alert");
		      String notifType = "";
		      if(json.has(LocalNotificationManager.PUSH_TYPE))
		    	  notifType = json.getString(LocalNotificationManager.PUSH_TYPE);
		      String soundKey = "";
		      if(json.has(LocalNotificationManager.SOUND_KEY))
		    	  soundKey = json.getString(LocalNotificationManager.SOUND_KEY);
		      String title = context.getString(R.string.app_name);
		      
		      notifyIntent.putExtra(LocalNotificationManager.BODY, message);
		      notifyIntent.putExtra(LocalNotificationManager.PUSH_TYPE, notifType);
		      notifyIntent.putExtra(LocalNotificationManager.PUSH_TIME, String.valueOf(System.currentTimeMillis()));
		      notifyIntent.putExtra(LocalNotificationManager.TITLE, title);
		      notifyIntent.putExtra(LocalNotificationManager.TICKER_TEXT, message);
		      notifyIntent.putExtra(LocalNotificationManager.ICON_RESOURCE, R.drawable.notification_icon);
		      notifyIntent.putExtra(LocalNotificationManager.HAS_ACTION, true);
		      notifyIntent.putExtra(LocalNotificationManager.SOUND_KEY, soundKey);
		      notifyIntent.putExtra(LocalNotificationManager.NUMBER_ANNOTATION, 1);
		      notifyIntent.putExtra(LocalNotificationManager.MAIN_ACTIVITY_CLASS_NAME_KEY,
		    		  "com.clash.of.kings.NBG_CN1");
		      notifyIntent.setAction("org.nbg.stac.empire.intent.action.IF");
		      Log.d(TAG, "got action " + action + " on channel " + channel + " with message:" + message + " all keys:");
		      Iterator<String> itr = json.keys();
		      while (itr.hasNext()) {
		        String key = itr.next();
		        Log.d(TAG, "..." + key + " => " + json.getString(key));
		      }
		    } catch (JSONException e) {
		      Log.d(TAG, "JSONException: " + e.getMessage());
		    }
		NotificationManager manager = (NotificationManager) context.getSystemService(android.content.Context.NOTIFICATION_SERVICE);
		manager.cancelAll();
		
		LocalNotificationManager notifyManager = new LocalNotificationManager(context);
	    notifyManager.fireNotificationNew(context, notifyIntent);
		
	}

}
