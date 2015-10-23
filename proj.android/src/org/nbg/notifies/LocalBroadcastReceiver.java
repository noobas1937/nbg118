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

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class LocalBroadcastReceiver extends BroadcastReceiver {

	
	@Override
	public void onReceive(Context context, Intent intent) {
		boolean isAppInForeground = LocalNotificationManager.isAppInForeground(context);

		if (isAppInForeground){
			Log.d("LocalBroadcastReceiver::onReceive", "App is running in foreground");
			return;
		} else {
			Log.d("LocalBroadcastReceiver::onReceive", "App is not running");
			
		}
		try{
			LocalNotificationManager manager = new LocalNotificationManager(context);
			manager. fireNotificationNew(context,intent);
		}catch(Throwable e){
			e.printStackTrace();
		}
		
		Log.d("LocalBroadcastReceiver::onReceive", "Local notification Intent: " + intent.toString());
	}

}
