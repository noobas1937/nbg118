/*************************************************************************
 *
 * ADOBE CONFIDENTIAL
 * ___________________
 *
 *  Copyright 2011 Adobe Systems Incorporated
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Adobe Systems Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Adobe Systems Incorporated and its
 * suppliers and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Adobe Systems Incorporated.
 **************************************************************************/

package org.nbg.notifies;

import java.util.List;

import org.cocos2dx.ext.Native;
import org.nbg.IF.Jni;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.IntentService;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class LocalNotificationIntentService extends IntentService {
	public LocalNotificationIntentService() {
		super("org.nbg.notifies.LocalNotificationIntentService");
	}

	@Override
	public final void onHandleIntent(Intent intent) {
		LocalNotificationManager.wasNotificationSelected = true;
		LocalNotificationManager.selectedNotificationCode = intent.getStringExtra(LocalNotificationManager.NOTIFICATION_CODE_KEY);
		LocalNotificationManager.selectedNotificationData = intent.getByteArrayExtra(LocalNotificationManager.ACTION_DATA_KEY);

		Context context = getApplicationContext();
		boolean isAppInForeground = isAppInForeground(context);

		if (isAppInForeground) {
				Log.d("COK", "COK LocalNotificationIntentService::onHandleIntent App is running in foreground");
			} else {
				Log.d("COK", "COK LocalNotificationIntentService::onHandleIntent App is running in background");
			}

		//set push click data 
		if(intent.getExtras() != null && intent.getExtras().getString(LocalNotificationManager.PUSH_CLICK_DATA) != null){
			LocalNotificationManager notifyManager = new LocalNotificationManager(context);
		    notifyManager.setPushClickData(intent.getExtras().getString(LocalNotificationManager.PUSH_CLICK_DATA));
		}
		if (!isAppInForeground) {
			// If the app is not running, or it's running, but in the background, start it by bringing it to the foreground or launching it.
			// The OS will handle what happens correctly.
			Intent newIntent = getBaseContext().getPackageManager()
					.getLaunchIntentForPackage( getBaseContext().getPackageName() );
			newIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			newIntent.setAction(Intent.ACTION_MAIN);  
		    newIntent.addCategory(Intent.CATEGORY_LAUNCHER);  
			context.startActivity(newIntent);
//			Intent newIntent = new Intent();
//			newIntent.setClassName(context, intent.getStringExtra(LocalNotificationManager.MAIN_ACTIVITY_CLASS_NAME_KEY));
//			newIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//			context.startActivity(newIntent);
		}

		Log.d("LocalNotificationIntentService::onHandleIntent", "Intent: " + intent.toString());
	}

	private boolean isAppInForeground(Context context) {
		ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
		List<RunningAppProcessInfo> appProcesses = activityManager.getRunningAppProcesses();
		if (appProcesses != null) {
			final String packageName = context.getPackageName();
			for (RunningAppProcessInfo appProcess : appProcesses) {
				if (appProcess.importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND && appProcess.processName.equals(packageName)) {
					return true;
				}
			}
		}

		return false;
	}
}
