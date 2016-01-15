package com.clash.of.kings;

import io.fabric.sdk.android.Fabric;
import it.partytrack.sdk.Track;

import java.util.Locale;

import org.cocos2dx.ext.Native;
import org.nbg.notifies.LocalNotificationManager;
import org.nbg.stac.empire.common.activity.CommonActivity;
import org.nbg.stac.empire.publish.IPublishChannel;

import android.content.SharedPreferences.Editor;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;

import com.chartboost.sdk.Chartboost;
import com.chartboost.sdk.Libraries.CBLogging.Level;
import com.clash.of.PayGoogle;
import com.clash.of.publish.GlobalPublishImpl;
import com.clash.of.util.IabBroadcastReceiver;
import com.clash.of.util.IabBroadcastReceiver.IabBroadcastListener;
import com.clash.of.util.NotifyUtil;
import com.crashlytics.android.Crashlytics;

import org.nbg.IF.IF;
import org.nbg.IF.Jni;
import org.json.JSONException;
import org.json.JSONObject;

import com.facebook.appevents.AppEventsLogger;
//import com.clash.of.util.MyGooglePlusUtil;
import com.google.android.apps.analytics.AdInstallRefMointor;
import com.nbg.innertest.R;
import com.inmobi.commons.InMobi;
import com.parse.ParseAnalytics;
import com.umeng.analytics.MobclickAgent;
import com.google.android.gms.GoogleGameServiceHelper;
import com.google.android.gms.ads.identifier.AdvertisingIdClient;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.common.GooglePlayServicesRepairableException;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.vk.sdk.VKAccessToken;
import com.vk.sdk.VKCallback;
import com.vk.sdk.VKSdk;
import com.vk.sdk.api.VKError;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import com.tencent.bugly.crashreport.CrashReport;

public class EmpireActivity extends CommonActivity {
	
	private IabBroadcastReceiver mBroadcastReceiver;
	private IabBroadcastListener mBroadcastListener;
	private GoogleGameServiceHelper mGameServiceHelper;
	
	@Override
	protected void recordCmd(String cmd){
        SharedPreferences sharedPreferences = this.getSharedPreferences("crashCommand", Context.MODE_PRIVATE);
        Editor editor = sharedPreferences.edit();//获取编辑器
        editor.putString("name", cmd);
        editor.commit();//提交修改
	}
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//GOOGLE相关
		try{
			mGameServiceHelper = new GoogleGameServiceHelper(this);
		}catch(Exception e){
			e.printStackTrace();
		}

		//InMobi
		InMobi.initialize(IF.getInstance(), "25471e0e24b84596a50258413b4c9bf5");
		//Party Track
		Track.start(IF.getInstance(), 3733, "d3ca92f1cdb78a9740d4caaed1fbb024");

		Native.nativeSetFromCountry(Locale.getDefault().getCountry());
		Native.nativeSetParseRegisterId(NotifyUtil.getParseNotifyToken());
		NotifyUtil.trackAppOpened(getIntent());
		try{
			Chartboost.startWithAppId(this, "53b24eda1873da027d86cbfe", "bafbb34d4d5197a974d4554d2d83e992169434ca");
			Chartboost.setLoggingLevel(Level.NONE);
			Chartboost.onCreate(this);
		}catch(Exception e)
		{
		}
		
		VKSdk.initialize(IF.getInstance());
		
//		try {
//			// 接入FaceBook广告
//			String FACEBOOK_APP_ID = IF.getInstance().getResources().getString(
//					R.string.fb_app_id);
////			com.facebook.Settings.publishInstallAsync(this, FACEBOOK_APP_ID);
//			AppEventsLogger.activateApp(IF.getInstance(), FACEBOOK_APP_ID);
//		} catch (Exception e) {
//
//		}
		
        // google play payment init
        m_payment = new PayGoogle();
        mBroadcastListener = new IabBroadcastListener() {
			
			@Override
			public void receivedBroadcast() {
				//是否需要调用
				if(m_payment != null)
					m_payment.queryPurchaseOrder();
				Log.d("COK","COK IabBroadcastListener");
			}
		};
		mBroadcastReceiver = new IabBroadcastReceiver(mBroadcastListener);
		IntentFilter broadcastFilter = new IntentFilter("com.android.vending.billing.PURCHASES_UPDATED");
		registerReceiver(mBroadcastReceiver, broadcastFilter);
        m_payment.init(this);
        // delay do init this in payment view
        Log.d("crash", "test crash start init....");
        Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());
		final SharedPreferences sharedPreferences = this.getSharedPreferences(LocalNotificationManager.CACHE_GAME_UID_KEY, Context.MODE_PRIVATE);
		String gameUid = sharedPreferences.getString("GAMEUID", "");
        
        final SharedPreferences nameSharedPreferences = this.getSharedPreferences("crashCommand", Context.MODE_PRIVATE);
        String name = nameSharedPreferences.getString("name", "");
        
        Log.d("COK UID", gameUid);
        Crashlytics.setUserIdentifier(gameUid + "_" + name);
//        Fabric.with(this, new CrashlyticsNdk());
        Log.d("crash", "test crash end init....");
//        Map<String, Object> result = null;//new HashMap<String, Object>();
//		result.put("test", "crashlytics");
        Context appContext = this.getApplicationContext();
		CrashReport.initCrashReport(appContext, "900004984", true);
        
	}
	
	@Override
	public boolean isGoogleSignedIn(){
		if(mGameServiceHelper != null)
			return mGameServiceHelper.isSignedIn();
		return false;
	}
	
	@Override
	public void doGoogleSignIn(){
		if(mGameServiceHelper != null)
			mGameServiceHelper.signIn();
	}
	
	@Override
	public String getGoogleLoginInfo(){
		if(mGameServiceHelper != null)
			return mGameServiceHelper.getLoginInfo_GoogleGameService();
		return "";
	}
		
	@Override
	public void doGoogleSignOut(){
		if(mGameServiceHelper != null)
			mGameServiceHelper.signOut();
	}
	
	@Override
	public boolean isGooglePlayServicesAvailable(){
		if(mGameServiceHelper != null)
			return mGameServiceHelper.isGooglePlayServicesAvailable();
		return false;
	}
	
	@Override
	public void openLeaderBoards(){
		if(mGameServiceHelper != null)
			mGameServiceHelper.openLeaderBoards();
	}

	@Override
	public void openAchievements(){
		if(mGameServiceHelper != null)
			mGameServiceHelper.openAchievements();
	}

	@Override
	public void submitScore(int score){
		if(mGameServiceHelper != null)
			mGameServiceHelper.submitScore(score);
	}

	@Override
	public void unlockAchievements(String id){
		if(mGameServiceHelper != null)
			mGameServiceHelper.unlockAchievements(id);
	}

	@Override
	protected boolean showCpb() {
		String flag = MobclickAgent.getConfigParams(this, "cpb_global");
		return "1".equals(flag);
	}

	@Override
	protected void onStart() {
		super.onStart();
		if(mGameServiceHelper != null)
			mGameServiceHelper.onStart(this);
	    Chartboost.onStart(this);
//		MyGooglePlusUtil.gpConnect();
	}

	@Override
	public void onStop() {
//		MyGooglePlusUtil.gpDisconnect();
//		Jni.cancelAll();
		super.onStop();
		if(mGameServiceHelper != null)
			mGameServiceHelper.onStop();
		Chartboost.onStop(this);
	}

	/**
	 * 游戏停止
	 */
	@Override
	protected void onPause() {
		super.onPause();
		Chartboost.onPause(this);
	}
	
	@Override
	public void onResume() {
		super.onResume();
		Chartboost.onResume(this);
	}
	
	/**
	 * 销毁退出
	 */
	@Override
	protected void onDestroy() {
//		AppFlood.destroy();
        if (getPayment().m_helper != null) {
            try {
                getPayment().m_helper.dispose();
            } catch (Exception e) {
                Log.d("Google Payment dispose",e.getMessage());
            }
        }
        if(mBroadcastReceiver != null)
        	unregisterReceiver(mBroadcastReceiver);
		super.onDestroy();
		Chartboost.onDestroy(this);
	}

    public PayGoogle getPayment() {
        return (PayGoogle)m_payment;
    }
	
	@Override
	public void onBackPressed() {
	    // If an interstitial is on screen, close it. Otherwise continue as normal.
	    if (Chartboost.onBackPressed())
	        return;
	    else
	        super.onBackPressed();
	}
	
	@Override
	public IPublishChannel getPublishImpl() {
		return new GlobalPublishImpl();
	}

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    	VKCallback<VKAccessToken> vkCallback = new VKCallback<VKAccessToken>() {
            @Override
            public void onResult(VKAccessToken res) {
            	Log.d("COK", "vkCallback onResult"+res.userId);
    			try {
					JSONObject obj = new JSONObject();
					obj.put("msgId", "login_sucess_other");
					obj.put("userName", res.userId);
					obj.put("platform", "vk");
					obj.put("userId", res.userId);
	    			Native.postNotification("onResponsed3rdPlatform", obj.toString());
    			} catch (JSONException e) {
    				e.printStackTrace();
    			}
            }
            @Override
            public void onError(VKError error) {
                // User didn't pass Authorization
            	Log.d("COK", "vkCallback onError");
            }
        };
    	if (!VKSdk.onActivityResult(requestCode, resultCode, data, vkCallback)) {
//        	MyGooglePlusUtil.onActivityResult(requestCode, resultCode, data);
	        // Pass on the activity result to the helper for handling
	        if (getPayment().m_helper == null || !getPayment().m_helper.handleActivityResult(requestCode, resultCode, data)) {
	            // not handled, so handle it ourselves (here's where you'd
	            // perform any handling of activity results not related to in-app
	            // billing...
	            super.onActivityResult(requestCode, resultCode, data);
	        }
	        else {
	            getPayment().debugLog("onActivityResult handled by IABUtil.");
	        }
    	}
		//Google play GameServices
		if(mGameServiceHelper != null)
			mGameServiceHelper.onActivityResult(requestCode, resultCode, data);
    }
}
