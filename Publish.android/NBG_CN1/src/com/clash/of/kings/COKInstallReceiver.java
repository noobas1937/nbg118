package com.clash.of.kings;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.google.android.apps.analytics.MyAnalyticsReceiver;

public class COKInstallReceiver extends BroadcastReceiver{

	@Override
	public void onReceive(Context context, Intent intent) {
		
		Log.d("COK", "cok install receiver");

		MyAnalyticsReceiver myReceiver = new MyAnalyticsReceiver();
		myReceiver.onReceive(context, intent);
		
	}

}