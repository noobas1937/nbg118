package com.clash.of.jni;

import java.util.Locale;

import it.partytrack.sdk.Track;

import org.nbg.IF.IF;
import org.nbg.IF.Payment;
import org.nbg.util.Constants;

import com.adjust.sdk.Adjust;
import com.google.ads.conversiontracking.AdWordsConversionReporter;
import com.helpshift.Log;
import com.nanigans.android.sdk.NanigansEventManager;
import com.vk.sdk.VKScope;
import com.vk.sdk.VKSdk;


public class Jni {
	
	public static void sendPayLog(String cost,String itemId){
		double price = Math.ceil(Float.parseFloat(cost)*100);
		Adjust.trackRevenue(price);
		
		float itemPrice = Float.parseFloat(cost);
		Track.payment(itemId, itemPrice, "USD", 1);
		
		NanigansEventManager.getInstance().trackPurchase(Float.parseFloat(cost)*100, String.valueOf(cost), 1D);
		
		AdWordsConversionReporter.reportWithConversionId(IF.getInstance().getApplicationContext(),
				"950451480", "VYeNCKfCtF4QmPqaxQM", cost, true);
	}
	
	public static void trackEvent(String event){
		String eventToken = "";
		if("two_days_login".equals(event))
			eventToken = "9f47vg";
		if("tutorial_over".equals(event))
			eventToken = "hw2r00";
		if("first_pay".equals(event))
			eventToken = "eyxm0i";
	    if("reach_level_4".equals(event))
	    	eventToken = "qumz9u";
	    if("reach_level_6".equals(event))
	    	eventToken = "biwo24";
		if(!"".equals(eventToken)){
			Adjust.trackEvent(eventToken);
		}
	}
	
	public static void queryHistoryPurchase(){
		Log.d("COK", "COK queryPurchaseOrder start");
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				try {
					if(IF.getInstance().m_payment != null)
						IF.getInstance().m_payment.queryPurchaseOrder();
					Log.d("COK", "COK queryPurchaseOrder finish");
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	
	public static void loginPlatform(String pf){
		Log.d("COK", "loginPlatform "+pf);
		if(Constants.Platform.VK.equals(pf)){
			final String[] sMyScope = new String[]{
	            VKScope.FRIENDS,
	            VKScope.WALL,
	            VKScope.PHOTOS,
	            VKScope.NOHTTPS
			};
			VKSdk.login(IF.getInstance(), sMyScope);
		}
	}
	
	public static void logoutPlatform(String pf){
		Log.d("COK", "logoutPlatform "+pf);
		if(Constants.Platform.VK.equals(pf))
			VKSdk.logout();
	}

}
