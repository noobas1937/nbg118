package org.cocos2dx.ext;

import java.util.Locale;


import android.accounts.Account;
import android.accounts.AccountManager;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.text.ClipboardManager;
import android.util.Log;
import android.os.*;

public class Device {

	private static ClipboardManager clipboard = null;
	private static Context _context = null;
	
	public static void init(Context ctx) {
		_context = ctx;
		clipboard = (ClipboardManager) ctx
				.getSystemService(Context.CLIPBOARD_SERVICE);
	}

	public static String getLanguage() {
		String sLang = Locale.getDefault().getLanguage();
		if (sLang.compareTo("zh") == 0)
			return Locale.getDefault().toString().equals("zh_CN") ? "zh-Hans"
					: "zh-Hant";
		return sLang;
	}

	public static String clipboardGetText() {
		try {
			return clipboard.getText().toString();
		} catch (Exception e) {
			Tracker.reportException(Jni.getGameActivity(), e);
		}
		return "";
	}

	public static void clipboardSetText(final String text) {
		Jni.getGameActivity().runOnUiThread(new Runnable() {
			public void run() {
				try {
					clipboard.setText(text);
				} catch (Exception e) {
					Tracker.reportException(Jni.getGameActivity(), e);
				}
			}
		});
	}
	
	public static String getHandSetInfo() {
		try {
		    String handSetInfo=  
		    	      "Model:" + android.os.Build.MODEL +   
		    	      ",SDKVersion:" + android.os.Build.VERSION.SDK +   
		    	      ",SYSVersion:" + android.os.Build.VERSION.RELEASE;
		    Log.i(Jni.LOG_TAG, "-------HandSetInfo:" + handSetInfo);
		    return handSetInfo; 
		} catch (Exception e) {
			Tracker.reportException(Jni.getGameActivity(), e);
		}
		return "";
	}
	
	public static String getVersionName() {
		try {
			String pName = Jni.getGameActivity().getPackageName();
			PackageInfo pinfo = null;
			pinfo = Jni.getGameActivity().getPackageManager()
					.getPackageInfo(pName, PackageManager.GET_CONFIGURATIONS);
			String versionName = "1.1.8"; // pinfo.versionName;
			Log.i(Jni.LOG_TAG, "-------versionName:" + versionName);
			return versionName;
		} catch (Exception e) {
			Tracker.reportException(Jni.getGameActivity(), e);
		}
		return "";
	}

	public static String getVersionCode() {
		try {
			String pName = Jni.getGameActivity().getPackageName();
			PackageInfo pinfo = null;
			pinfo = Jni.getGameActivity().getPackageManager()
					.getPackageInfo(pName, PackageManager.GET_CONFIGURATIONS);
			int versionCode = pinfo.versionCode;
			Log.i(Jni.LOG_TAG, "-------versionCode:" + versionCode);
			return String.valueOf(versionCode);
		} catch (Exception e) {
			Tracker.reportException(Jni.getGameActivity(), e);
		}
		return "";
	}
	
	public static String getAccountInfo()
	{
		AccountManager accountManager = AccountManager.get(_context);
		Account[] accounts = accountManager.getAccountsByType("com.google");
		for(Account account : accounts)
		{
			return account.name;
		}
		return "";
	}

	public static String getUid() {
		return Udid.getUid();
	}
	
	public static String getChannel(){
		return "Android";
	}
	public static boolean hasEnoughSpace(int needSize) {
		Log.i(Jni.LOG_TAG, "-------need size:" + needSize);
		StatFs sf = new StatFs(_context.getCacheDir().getAbsolutePath());
		long freesize = sf.getAvailableBlocks() / 1024 ;
		freesize = freesize * sf.getBlockSize();
		Log.i(Jni.LOG_TAG, "-------free size:" + freesize);
		if(needSize > freesize){
			return false;
		}
		return true;
	}
}
