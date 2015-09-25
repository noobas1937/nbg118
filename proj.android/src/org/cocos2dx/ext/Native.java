package org.cocos2dx.ext;

public class Native {
	public static void postNotification(final String msg) {
		Jni.getGameActivity().runOnGLThread(new Runnable() {
			@Override
			public void run() {
				nativePostNotification(msg);
			}
		});
	}
	public static void postNotification(final String msgId,final String jsonMsg) {
		Jni.getGameActivity().runOnGLThread(new Runnable() {
			@Override
			public void run() {
				nativePostNotificationArgs(msgId,jsonMsg);
			}
		});
	}
	/**
	 * 发送一个带消息参数的通知
	 * @param msgId
	 * @param jsonStr
	 */
	public native static void nativePostNotificationArgs(String msgId,String jsonMsg);
	public native static void nativePostNotification(String msg);
	public native static String nativeGetAbFlag();
	public native static void nativeSetConnectConfig(String ip, String zone, int port);
	public native static void nativeSetABTestConfig(String ab_test_value);
	public native static boolean nativeIsGECLogEnable();

    // 根据玩家离开的天数，获得通知文本
	public native static String nativeGetAFKText(int days);
    // 获得多语言文本
	public native static String nativeGetMultiLangText(String key);
	
	public native static void nativeAPKpackageName(String linkStr);
	public native static void nativeSetBranchValue(String key, String value);


	
	public native static void nativeSetAdReferrer(String referrer);
	public native static void nativeShowWaitInterface();
	public native static void nativeRemoveWaitInterface();
	
	//账号相关
	public native static void nativeSetFromCountry(String country);
	public native static void nativeSetGcmRegisterId(String regId);
	public native static void nativeSetParseRegisterId(String regId);
	public native static void nativeSetGaid(String gaid);
	
	public native static void nativeSetPlatformToken(String platformToken);
	public native static void nativeSetPlatformUID(String platformUID);
	public native static void quitGame();
	public native static void logOut();
	public native static void nativeClearGuideData();
	
	public native static String nativeGetUID();
	public native static int nativeGetLevel();
	public native static String nativeGetNickname();
	public native static int nativeGetServerId();
	//缓存
    public native static void nativeSetConfig(String key,String value);
    public native static String nativeGetConfig(String key);
	//支付相关
	public native static String nativeGetGoogleEncodedPublickey();
	public native static String nativeGetAccountUrl();
	public native static String nativeGetPaymentCallbackUrl();
	public native static String nativeGetPaymentCallbackUrlTest();
	
	public native static void nativeTstorePaymentSuccess(String txid,String receipt,String itemId);
	public native static void nativeAndroidPaymentFail();
	public native static void nativeAmazonPaymentSuccess(String productType,String purchaseTime,String sku,String amazonUserId,String orderId,String itemId);
	public native static void nativeNstorePaymentSuccess(String paymentSeq,String nonce,String itemId);
	public native static void nativeCafebazaarPaymentSuccess(String orderId,String purchaseToken,String sku,String itemId);
	public native static void nativeTencentPaymentSuccess(String orderId,String openid,String itemId);
	public native static void nativeEnableQueryHistoryPurchase();
	public native static void nativeAPPLink(String linkStr);
	public native static void nativeShowLoginPanel();
	public native static void nativeCloseLoginPanel();
}
