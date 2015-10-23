package org.nbg.util;

import org.cocos2dx.ext.Native;
import org.nbg.IF.IF;

import android.util.Log;

import com.xiaomi.migamechannel.MiGameChannel;

public class MiAnalyticsUtil {
	
	private static String TAG = "COK_MiAnalyticsUtil";
	
	public static void triggerEventLoginComplete(String userId,String userLevel){
		if(!IF.getInstance().miAnalyticsEnabled)
			return;
        MiGameChannel.Login(userId, userLevel);
	}
	
	public static void triggerEventPurchaseInit(String cost){
		if(!IF.getInstance().miAnalyticsEnabled)
			return;
		String amount = String.valueOf(CommonUtil.getPaidAmountInCents("CNY", Float.parseFloat(cost)));
		String level = String.valueOf(Native.nativeGetLevel());
        MiGameChannel.BeforeRecharge(Native.nativeGetUID(), level, amount);
	}
	
	public static void triggerEventPurchase(String cost){
		if(!IF.getInstance().miAnalyticsEnabled)
			return;
		String amount = String.valueOf(CommonUtil.getPaidAmountInCents("CNY", Float.parseFloat(cost)));
		String level = String.valueOf(Native.nativeGetLevel());
        MiGameChannel.AfterRecharge(Native.nativeGetUID(), level, amount);
	}

	public static void triggerEventAchievedLevel(int level){
		if(!IF.getInstance().miAnalyticsEnabled)
			return;
		MiGameChannel.UserLevelUpgrade(Native.nativeGetUID(), String.valueOf(level));
	}
	
	public static void triggerEventAppForground(){
		if(!IF.getInstance().miAnalyticsEnabled)
			return;
		MiGameChannel.setForegroundTime();
	}

	public static void triggerEventAppBackground(){
		if(!IF.getInstance().miAnalyticsEnabled)
			return;
		MiGameChannel.UploadDuration();
	}

}