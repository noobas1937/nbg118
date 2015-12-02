package com.elex.tech.nbg;

import java.util.Locale;

import android.util.Log;

public class NBGDevice {
	public static String getHandSetInfo() {
		try {
			String handSetInfo = "Model:" + android.os.Build.MODEL + ",SDKVersion:" + android.os.Build.VERSION.SDK + ",SYSVersion:" + android.os.Build.VERSION.RELEASE;
		    Log.i("NBGDevice", "-------HandSetInfo:" + handSetInfo);
		    return handSetInfo; 
		} catch (Exception e) {
			Log.i("NBGDevice", "-------HandSetInfo:" + e.toString());
		}
		return "";
	}
	
	public static String getLanguage() {
		String sLang = Locale.getDefault().getLanguage();
		if (sLang.compareTo("zh") == 0) return Locale.getDefault().toString().equals("zh_CN") ? "zh-Hans" : "zh-Hant";
		return sLang;
	}
}
