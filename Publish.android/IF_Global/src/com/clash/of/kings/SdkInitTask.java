package com.clash.of.kings;

import it.partytrack.sdk.Track;

import org.nbg.IF.IF;

import android.os.AsyncTask;

import com.nbg.gp.R;
import com.inmobi.commons.InMobi;

/**
 * Created by long on 14-2-21.
 */
public class SdkInitTask extends AsyncTask {
    @Override
    protected Void doInBackground(Object... params) {
//		//InMobi
//		InMobi.initialize(IF.getInstance(), "25471e0e24b84596a50258413b4c9bf5");
//		//Party Track
//		Track.start(IF.getInstance(), 3733, "d3ca92f1cdb78a9740d4caaed1fbb024");
		
//		if (AdInstallRefMointor.isAdFromTapjoy(this)) {
			// Tapjoy广告推广
//			TapjoyConnect.requestTapjoyConnect(this,
//					CommonConst.GOLBAL_TAPJOY_APPID,
//					CommonConst.GOLBAL_TAPJOY_SECRET_KEY);
//		}
		
//		if (AdInstallRefMointor.isAdFromChartboost(this))
//		{
//			 Configure Chartboost
//			this.cb = Chartboost.sharedChartboost();
//			String appId = "53b24eda1873da027d86cbfe";
//			String appSignature = "bafbb34d4d5197a974d4554d2d83e992169434ca";
//			this.cb.onCreate(this, appId, appSignature, null);
//			this.cb.setImpressionsUseActivities(true);
//		}
		
//		try {
//			// 接入FaceBook广告
//			String FACEBOOK_APP_ID = IF.getInstance().getResources().getString(
//					R.string.fb_app_id);
////			com.facebook.Settings.publishInstallAsync(this, FACEBOOK_APP_ID);
//			com.facebook.AppEventsLogger.activateApp(IF.getInstance(), FACEBOOK_APP_ID);
//		} catch (Exception e) {
//
//		}
		return null;
    }
}
