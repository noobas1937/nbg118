package com.clash.of.kings;

import org.cocos2dx.ext.Tracker;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.google.android.apps.analytics.MyAnalyticsReceiver;
import com.google.android.gms.analytics.CampaignTrackingReceiver;
import com.inmobi.commons.analytics.androidsdk.IMAdTrackerReceiver;

public class COKInstallReceiver extends BroadcastReceiver{

	@Override
	public void onReceive(Context context, Intent intent) {
		
		Log.d("COK", "cok install receiver");
		
		try {
			CampaignTrackingReceiver googleReceiver = new CampaignTrackingReceiver();
			googleReceiver.onReceive(context, intent);
		} catch (Exception e) {
			Tracker.reportException(context, e);
		}

		IMAdTrackerReceiver inmobiReceiver = new IMAdTrackerReceiver();
		inmobiReceiver.onReceive(context, intent);
		
		MyAnalyticsReceiver myReceiver = new MyAnalyticsReceiver();
		myReceiver.onReceive(context, intent);
		
	}

}