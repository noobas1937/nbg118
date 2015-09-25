package com.stac.empire.pay.platform;

import android.util.Log;

import org.hcg.stac.empire.pay.PayItemData;
import org.hcg.stac.empire.pay.platform.IPlatformPay;

import com.clash.of.kings.COK_CN1;

public class PlatformPay implements IPlatformPay {
    private static final String TAG = "PlatformPay";
    
	@Override
	public void pay(PayItemData payItemData) {
		COK_CN1.doPay(payItemData);
	}

	@Override
	public void initPlatformInfo(String publishKey) {
	}
}
