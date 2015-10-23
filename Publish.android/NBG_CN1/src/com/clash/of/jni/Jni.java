package com.clash.of.jni;

import org.nbg.IF.IF;
import org.nbg.util.Constants;
import org.nbg.stac.empire.pay.PayItemData;

import android.util.Log;

import com.adjust.sdk.Adjust;
import com.clash.of.kings.NBG_CN1;
import com.clash.of.weibo.WBUtil;
import com.stac.empire.pay.platform.PlatformPay;

public class Jni {
	private static String TAG = "NBG_JNI";
	
	public static void doPlatformLogin(){
		NBG_CN1.doLogin();
	}
	
	public static void payClicked(final String pay_id , final String gold_num, final String get_gold_count, final String userId) {
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				try {
					PayItemData payItem = new PayItemData();
					payItem.setItemType(PayItemData.ITEM_TYPE_INAPP);
					payItem.setItemId(pay_id);
					payItem.setGoldNum(Float.parseFloat(gold_num));//购买所需钱数
					payItem.setCoin(Integer.parseInt(get_gold_count));//购买金币数量，用于显示名字
					payItem.setUserId(userId);
					new PlatformPay().pay(payItem);
				} catch (Exception e) {
					// tracker.reportException(e);
				}
			}
		});
	}
	
	public static void sendPayLog(String cost,String itemId){
		double price = Math.ceil(Float.parseFloat(cost)*100);
		Adjust.trackRevenue(price);
	}
	
	public static void trackEvent(String event){
	}
	
	public static void onConsumeCallback(String orderId, int state){
	}

	public static void queryHistoryPurchase(){
		Log.d(TAG, "queryPurchaseOrder start");
	}
	
	public static void loginPlatform(String pf){
		Log.d(TAG, "loginPlatform "+pf);
		if(Constants.Platform.WEIBO.equals(pf))
			WBUtil.login();
	}
	
	public static void logoutPlatform(String pf){
		Log.d(TAG, "logoutPlatform "+pf);
		if(Constants.Platform.WEIBO.equals(pf))
			WBUtil.logout();		
	}
	
}
