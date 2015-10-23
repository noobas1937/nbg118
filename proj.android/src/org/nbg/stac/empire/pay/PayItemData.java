package org.nbg.stac.empire.pay;

import java.io.Serializable;

import org.cocos2dx.ext.Native;
import org.nbg.IF.R;
import org.nbg.IF.IF;
import org.nbg.stac.empire.pay.platform.PayPlatformConst;
import org.nbg.stac.empire.publish.IPublishChannel;
import org.nbg.util.CommonUtil;
import org.nbg.util.GameContext;

public class PayItemData implements Serializable {

	private static final long serialVersionUID = -598181875871093995L;
	public static final int GET_REWARD_RATE_APP = 108;
	public static final int BIG_PAY_AB_TEST_INDEX = 7;// 129.99
	public static final int JP_PAY_3_99_INDEX = 1;
	/**
	 * {@link com.stac.empire.pay.market.Const#ITEM_TYPE_INAPP }
	 */
	public static final String ITEM_TYPE_INAPP = "inapp";// 应用内产品
	/**
	 * {@link com.stac.empire.pay.market.Const#ITEM_TYPE_SUBSCRIPTION }
	 */
	public static final String ITEM_TYPE_SUBSCRIPTION = "subs";// 订阅产品【包月，包年】

	// --end-------------------subs[订阅-包月/年]--product id---------------------
	/**
	 * @see ITEM_TYPE_INAPP,ITEM_TYPE_SUBSCRIPTION
	 */
	private String userId;//用户
	
	public String getUserId(){
		return userId;
	}
	public void setUserId(String userId){
		this.userId = userId;
	}
	
	private String toUserID = ""; //赠送UID
	
	public String getToUserID(){
		return toUserID;
	}
	public void setToUserID(String toUid){
		this.toUserID = toUid;
	}
	
	private String itemType;// 购买类型
	private String itemId;

	private int coin;//获得金币数，发货名字用
	
	private float goldNum;//花费数目,由cocos-2dx传入
	
	public float getGoldNum(){
		return goldNum;
	}
	public void setGoldNum(float goldNum){
		this.goldNum = goldNum;
	}
	/**
	 * @return the coin
	 */
	public int getCoin() {
		return coin;
	}

	/**
	 * @param coin
	 *            the coin to set
	 */
	public void setCoin(int coin) {
		this.coin = coin;
	}

	/**
	 * @return the itemId
	 */
	public String getItemId() {
		return itemId;
	}

	/**
	 * @param itemId
	 *            the itemId to set
	 */
	public void setItemId(String itemId) {
		this.itemId = itemId;
	}
	
	public String getProductName(){
		String productName = String.format("%d金币礼包", getItemCoin());
		if("9006".equals(getItemId()))
			productName = "月卡";
		return productName;
	}
	
	public String getProductNameEn(){
		String productName = String.format("%dGoldPackage", getItemCoin());
		if("9006".equals(getItemId()))
			productName = "MonthlyCard";
		return productName;
	}
	
	public String getOrderId(){
		//目前最大位数为40
		String orderId = String.format("%s_%d_%s_%d", 
				getUserId(), // UID   目前最大16位
				Native.nativeGetServerId(), // 服务器ID 4+1
				getItemId(), // xmlId 4+1
				System.currentTimeMillis() // 当前时间 13+1
				);
		return orderId;
	}

	/**
	 * @return the description
	 */
	public String getDescription() {

		String description = String.format(Native.nativeGetMultiLangText("shop_coin_desc"),
				String.valueOf(getItemPrice()), String.valueOf(getItemCoin()));
		return description;
	}

	/**
	 * GooglePlay 应用内支付ID
	 * 
	 * @return
	 */
	public String getProductId_GooglePlay() {
		if (this.itemType == ITEM_TYPE_INAPP) {
			return "gold_" + this.getItemId();
		} else {
			return this.getItemId();
		}
	}

	/**
	 * 当前支付货币类型
	 * 
	 * @return
	 */
	public String getPayCurrency() {
		String region = IF.getInstance().getPublishImpl().getPublish_Region();
		if (IPublishChannel.PUBLISH_JAPANESE.equals(region)) {
			return PayPlatformConst.CURRENCY_JP_MARKET;
		} else if (IPublishChannel.PUBLISH_360.equals(region) ||
				"samsung".equals(region) ||
				"jiyou".equals(region) ||
				"moji".equals(region)) {
			return PayPlatformConst.CURRENCY_CHINA;
		} else {
			return PayPlatformConst.CURRENCY_MARKET;
		}
	}

	public String getItemType() {
		return itemType;
	}

	public void setItemType(String itemType) {
		this.itemType = itemType;
	}

	public static String getItemId_GooglePlay(String marketId) {
		if (isSubsProductId(marketId)) {
			return marketId;
		}
		return marketId.substring("gold_".length());
	}

	private static int getItemIndex(String productId) {
		int pId = Integer.parseInt(productId);
		for (int index = 0; index < getConfig_ProducdId().length; index++) {
			if (pId == getConfig_ProducdId()[index]) {
				return index;
			}
		}
		return -1;
	}
	
	private String channel = "";
	public String getChannel(){
		return channel;
	}
	public void setChannel(String itemType) {
		this.channel = itemType;
	}

//	public float getItemPrice() {
//		String region = GameContext.getGamePublisher().getPublish_Region();
//		int index = getItemIndex(this.itemId);
//		if (index == -1)
//			return 0;
//		if (IPublishChannel.PUBLISH_JAPANESE.equals(region)) {
//			return getConfig_PriceJPY()[index];
//		} else if (IPublishChannel.PUBLISH_360.equals(region)
//				|| IPublishChannel.PUBLISH_WANDOUJIA.equals(region)
//				|| IPublishChannel.PUBLISH_XIAOMI.equals(region)
//				|| IPublishChannel.PUBLISH_UC.equals(region)
//				|| IPublishChannel.PUBLISH_ANZHI.equals(region)
//				|| "samsung".equals(region)
//				|| "jiyou".equals(region)
//				|| "moji".equals(region)) {
//			return getConfig_PriceCNY()[index];
//		} else {
//			return getConfig_PriceUSD()[index];
//		}
//	}
	
	public float getItemPrice() {
		return goldNum;
	}
	/**
	 * 
	 * @param productId
	 * @return
	 */
	public float getItemPrice_Market() {
		return getProductPrice_Market(this.itemId);
	}
//	public float getItemCoin() {
//		//国服用传入的金币数（金币打折活动），否则用Java配置的
//		String region = GameContext.getGamePublisher().getPublish_Region();
//		if (IPublishChannel.PUBLISH_360.equals(region)
//				|| IPublishChannel.PUBLISH_WANDOUJIA.equals(region)
//				|| IPublishChannel.PUBLISH_XIAOMI.equals(region)
//				|| IPublishChannel.PUBLISH_UC.equals(region)
//				|| IPublishChannel.PUBLISH_ANZHI.equals(region)
//				|| "samsung".equals(region)
//				|| "jiyou".equals(region)
//				|| "moji".equals(region)) 
//		{
//			return this.goldNum;
//		}
//		else
//		{
//			int index = getItemIndex(this.itemId);
//			if (index != -1) {
//				return getConfig_BUY_COIN_NUMBER()[index];
//			} else {
//				return 0;
//			}	
//		}
//
//	}
	
	public int getItemCoin() {
		return coin;
	}


	public static float getProductPrice_Market(String productId) {
		int index = getItemIndex(productId);
		if (index != -1) {
			String region = GameContext.getGamePublisher().getPublish_Region();
			if (IPublishChannel.PUBLISH_JAPANESE.equals(region)) {
				return getConfig_PriceJPY()[index];
			} else {
				return getConfig_PriceUSD()[index];
			}
		} else {
			return 0;
		}

	}
	/**
	 * 
	 * @param productId
	 * @return
	 */
	public static int getProductVAmount(String productId) {
		if (isSubsProductId(productId)) {// 订阅类，全部为1
			return 1;
		}
		int index = getItemIndex(productId);
		if (index != -1) {
			return getConfig_BUY_COIN_NUMBER()[index];
		} else {
			return 0;
		}
	}

	/**
	 * 订阅产品总数
	 * 
	 * @return
	 */
	public static int getSubsSize_Google() {
		return ConfigSubs.SUBS_GOOGLE_ID.length;
	}

	/**
	 * 订阅ID
	 * 
	 * @param index
	 * @return
	 */
	public static String getSubsID_Google(int index) {
		return ConfigSubs.SUBS_GOOGLE_ID[index];
	}

	/**
	 * 订阅价格
	 * 
	 * @param index
	 * @return
	 */
	public static float getSubsPrice_Google(int index) {
		String region = GameContext.getGamePublisher().getPublish_Region();
		if (IPublishChannel.PUBLISH_JAPANESE.equals(region)) {
			return ConfigSubs.SUBS_PRICE_JPY[index];
		} else {
			return ConfigSubs.SUBS_PRICE_USD[index];
		}
	}

	public static float getSubsPrice_Google(String id) {
		int index = 0;
		for (String sid : ConfigSubs.SUBS_GOOGLE_ID) {
			if (sid.equals(id)) {
				return getSubsPrice_Google(index);
			}
			index++;
		}
		return 0;
	}

	public static boolean isSubsProductId(String id) {
		for (String sid : ConfigSubs.SUBS_GOOGLE_ID) {
			if (sid.equals(id)) {
				return true;
			}
		}
		return false;
	}

	public static int getChargeItemSize() {
		return getConfig_ProducdId().length;
	}

	public static String getProductId_InAppPurchase(int index) {
		return String.valueOf(getConfig_ProducdId()[index]);
	}

	public static boolean isIAP_AB_KOC() {
		return false;
//		return TypeConst.ABEntries.ABTest_IAP_KOC_PRICE_A.equals(Native
//				.nativeGetAbFlag());
	}

	public static boolean isGameAOE() {
		return true;
	}

	private static int[] getConfig_ProducdId() {
		if (isGameAOE()) {
			String region = GameContext.getGamePublisher().getPublish_Region();

			if (IPublishChannel.PUBLISH_UC.equals(region)) {
				return ConfigInAPP_AOE.PRODUCT_ID_UC;
			}
			if (isIAP_AB_KOC()) {
				return ConfigInAPP_AOE_KOC.PRODUCT_ID;
			} else {
				return ConfigInAPP_AOE.PRODUCT_ID;
			}
		} else {
			return ConfigInAPP_AOW.PRODUCT_ID;
		}
	}

	private static float[] getConfig_PriceUSD() {
		if (isGameAOE()) {
			if (isIAP_AB_KOC()) {
				return ConfigInAPP_AOE_KOC.PRICE_USD;
			} else {
				return ConfigInAPP_AOE.PRICE_USD;
			}
		} else {
			return ConfigInAPP_AOW.PRICE_USD;
		}
	}

	private static float[] getConfig_PriceCNY() {
		if (isGameAOE()) {
			String region = GameContext.getGamePublisher().getPublish_Region();

			if (IPublishChannel.PUBLISH_UC.equals(region)) {
				return ConfigInAPP_AOE.PRICE_CNY_UC;
			}
			if (isIAP_AB_KOC()) {
				return ConfigInAPP_AOE_KOC.PRICE_CNY;
			} else {
				return ConfigInAPP_AOE.PRICE_CNY;
			}

		} else {
			return ConfigInAPP_AOW.PRICE_CNY;
		}
	}

	private static int[] getConfig_PriceJPY() {
		if (isGameAOE()) {
			if (isIAP_AB_KOC()) {
				return ConfigInAPP_AOE_KOC.PRICE_JPY;
			} else {
				return ConfigInAPP_AOE.PRICE_JPY;
			}
		} else {
			return ConfigInAPP_AOW.PRICE_JPY;
		}
	}

	private static int[] getConfig_BUY_COIN_NUMBER() {
		if (isGameAOE()) {
			String region = GameContext.getGamePublisher().getPublish_Region();

			if (IPublishChannel.PUBLISH_UC.equals(region)) {
				return ConfigInAPP_AOE.BUY_COIN_NUMBER_UC;
			}
			if (isIAP_AB_KOC()) {
				return ConfigInAPP_AOE_KOC.BUY_COIN_NUMBER;
			} else {
				return ConfigInAPP_AOE.BUY_COIN_NUMBER;
			}
		} else {
			return ConfigInAPP_AOW.BUY_COIN_NUMBER;
		}
	}

	/**
	 * 
	 * 支付配置
	 * 
	 */
	static class ConfigInAPP_AOE {
		// http://developer.android.com/intl/zh-CN/guide/google/play/billing/billing_testing.html
		// google测试ID:reserved product IDs
		// 1 android.test.purchased
		// 2 android.test.canceled,
		// 3 android.test.refunded
		// 4 android.test.item_unavailable
		// id--------------------------------
		// 支付对应的金币和ID
		// coin: 9,50, 120,270,750,1600,2500
		// -id-:104,103,101,102,105,106, 109
		private static final int[] PRODUCT_ID = { 104, 107, 103, 101, 102, 105,
				106, 109 };
		// 美元-计价单位
		private static final float[] PRICE_USD = new float[] { 0.99f, 3.99f,
				4.99f, 9.99f, 19.99f, 49.99f, 99.99f, 129.99f };
		// RMB-计价单位
		private static final float[] PRICE_CNY = new float[] { 6.0F, 23.0f,
				29.0F, 59.0f, 119.0f, 299.0f, 499.0f, 649.0f };
		// 日元-计价单位
		private static final int[] PRICE_JPY = new int[] { 100, 300, 400, 800,
				1600, 4000, 8000, 10400 };
		private static final int[] BUY_COIN_NUMBER = new int[] { 9, 41, 50,
				120, 270, 750, 1600, 2500 };
		// RMB-计价单位
		private static final float[] PRICE_CNY_UC = new float[] { 10.0f, 20.0f, 30.0f, 50.0f, 100.0f, 300.0f, 500.0f, 649.0f };
		private static final int[] BUY_COIN_NUMBER_UC = new int[] { 17, 35, 55, 100, 225, 750, 1600, 2500 };
		private static final int[] PRODUCT_ID_UC = { 401, 402, 403, 404, 405, 406, 407, 408 };
	}

	/**
	 * 
	 * 支付ABTest配置参数
	 * 
	 */
	static class ConfigInAPP_AOE_AB {
		// 观测数据： 单次平均付费金额是否能得到提升
		// 将所有金币提价30%，且只显示这些项，隐藏原有金币项
		// 9金币 1.99美元
		// 41金币 5.99美元
		// 50金币 6.99美元
		// 120金币 12.99美元
		// 270金币 25.99美元
		// 750金币 64.99美元
		// 1600金币 129.99美元
		private static final int[] PRODUCT_ID = { 200, 201, 202, 203, 204, 205,
				206 };
		// 美元-计价单位
		private static final float[] PRICE_USD = new float[] { 1.99f, 5.99f,
				6.99F, 12.99f, 25.99f, 64.99f, 129.99f };
		// RMB-计价单位
		private static final float[] PRICE_CNY = new float[] { 8.0F, 29.0f,
				39.0F, 79.0f, 149.0f, 399.0f, 649.0f };
		// 日元-计价单位
		private static final int[] PRICE_JPY = new int[] { 130, 390, 520, 1040,
				2080, 5200, 10400 };
		private static final int[] BUY_COIN_NUMBER = new int[] { 9, 41, 50,
				120, 270, 750, 1600 };
	}

	/**
	 * 
	 * 支付ABTest_koc配置参数<br>
	 * $0.99（9金币）<br>
	 * $4.99（50金币）<br>
	 * $9.99（100金币）<br>
	 * $19.99（240金币）<br>
	 * $49.99（665金币）<br>
	 * $99.99（1600金币）<br>
	 */
	static class ConfigInAPP_AOE_KOC {
		private static final int[] PRODUCT_ID = { 104, 302, 303, 304, 306, 307 };
		// 美元-计价单位
		private static final float[] PRICE_USD = new float[] { 0.99f, 4.99f,
				9.99f, 19.99f, 49.99f, 99.99f };
		// RMB-计价单位
		private static final float[] PRICE_CNY = new float[] { 6.0F, 29.0f,
				59.0F, 119.0f, 299.0f, 599.0f };
		// 日元-计价单位
		private static final int[] PRICE_JPY = new int[] { 100, 500, 1000,
				2000, 5000, 10000 };
		private static final int[] BUY_COIN_NUMBER = new int[] { 9, 50, 100,
				240, 665, 1600 };
	}

	/*
	 * AOW支付参数
	 */
	static class ConfigInAPP_AOW {
		private static final int[] PRODUCT_ID = { 101, 102, 103, 104, 105, 106,
				107, 109 };
		// 美元-计价单位
		private static final float[] PRICE_USD = new float[] { 2.99f, 4.99f,
				9.99f, 19.99f, 29.99f, 49.99f, 99.99f, 129.99f };
		// RMB-计价单位
		private static final float[] PRICE_CNY = new float[] { 19.0F, 29.0f,
				59.0F, 119.0f, 179.0f, 299.0f, 599.0f, 699.0f };
		// 日元-计价单位
		private static final int[] PRICE_JPY = new int[] { 300, 500, 1000,
				2000, 3000, 5000, 10000, 13000 };
		private static final int[] BUY_COIN_NUMBER = new int[] { 30, 50, 100,
				240, 375, 665, 1600, 2500 };
	}

	/**
	 * subs[订阅-包月/年]--product 通过服务端配置，客户端不再区分
	 */
	public static class ConfigSubs {

		// http://developer.android.com/intl/zh-CN/guide/google/play/billing/billing_subscriptions.html
		// --support the In-app Billing v2 API (Google Play 3.5 and higher)
		private static String[] SUBS_GOOGLE_ID = new String[] {};
		private static float[] SUBS_PRICE_USD = new float[] {};
		private static int[] SUBS_PRICE_JPY = new int[] {};

		// static{
		// initConfigSubs(new String[]{"test_2"},new float[]{4.99f},new
		// int[]{500});
		// }
		public static void initConfigSubs(String[] subsId, float[] subs_USD,
				int[] subs_JPY) {
			SUBS_GOOGLE_ID = subsId;
			SUBS_PRICE_USD = subs_USD;
			SUBS_PRICE_JPY = subs_JPY;
		}
	}
}
