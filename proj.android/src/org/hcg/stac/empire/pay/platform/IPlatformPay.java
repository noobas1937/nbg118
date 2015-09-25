package org.hcg.stac.empire.pay.platform;

import org.hcg.stac.empire.pay.PayItemData;

public interface IPlatformPay {
	public final static String TAG = "IPlatformPay";
    
    /*
	 * 支付接口
	 * 
	 * @param goodsOrder
	 */
	public void pay(PayItemData payItem);

	/**
	 * 初始化支付平台参数
	 * @param publishKey [目前只有Google支付需要此key]
	 */
	public void initPlatformInfo(String publishKey);
}
