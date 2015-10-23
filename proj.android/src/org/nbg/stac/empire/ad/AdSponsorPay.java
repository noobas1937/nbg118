package org.nbg.stac.empire.ad;

import android.util.Log;

import org.cocos2dx.ext.Udid;
import org.nbg.util.CommonUtil;
import org.nbg.util.GameContext;

import com.sponsorpay.sdk.android.SponsorPay;

public class AdSponsorPay {
	private static final String APP_ID_ADVERTISER="A62648";
	private static final String App_ID_GoolgePaly = "9737";
	private static final String Security_Token_GoolgePaly = "e22af8544a07414e386ea112b5dac141";

	private static final String App_ID_MDD = "9734";
	private static final String Security_Token_MDD = "b4329e17d192702b06bebf52dcd55d38";
	/**
	 * 完成某个动作
	 * 
	 * @param id
	 */
//	public static void onRegisterAction() {
//		try {//注意当无reffer时 有异常
//			SponsorPayAdvertiser.register(GameContext.getActivityInstance()
//					.getApplicationContext());
//		} catch (Exception e) {
//			DebugLog.d("AdSponsorPay", e);
//		}
//	}
	public static void onStartAction() {
		String securityToken = null;
		String overridingAppId = null;
		if (CommonUtil.isVersion_Ad_Mdd()) {
			securityToken = Security_Token_MDD;
			overridingAppId = App_ID_MDD;
		} else {
			overridingAppId = App_ID_GoolgePaly;
			securityToken = Security_Token_GoolgePaly;
		}
		try {//publisher接入必须同时执行advertise 与publisher回调才有效
			SponsorPay.start(APP_ID_ADVERTISER, null, null, GameContext.getActivityInstance()
				     .getApplicationContext());
			SponsorPay.start(overridingAppId, Udid.getUid(), securityToken,
					GameContext.getActivityInstance());
		} catch (RuntimeException e) {
			Log.d("SponsorPay", "Err form sponsorpay", e);
		}
	}
}
