package com.stac.empire.pay.platform;

import android.util.Log;

import org.nbg.stac.empire.pay.PayItemData;
import org.nbg.stac.empire.pay.platform.IPlatformPay;

import com.clash.of.kings.NBG_CN1;

public class PlatformPay implements IPlatformPay {
    private static final String TAG = "PlatformPay";
    
	@Override
	public void pay(PayItemData payItemData) {
		NBG_CN1.doPay(payItemData);
	}

	@Override
	public void initPlatformInfo(String publishKey) {
	}
}
