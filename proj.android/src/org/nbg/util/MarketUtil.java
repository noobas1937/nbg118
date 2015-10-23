package org.nbg.util;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;

public class MarketUtil {

	public static void rateAppInMarket(Activity mainActivity, int requestCode) {
		openAppsInMarket(mainActivity, requestCode);
	}

	public static void downLoadFromMarket(Activity mainActivity) {
		openAppsInMarket(mainActivity, -1);
//		GameContext.release();
		GameContext.getActivityInstance().finish();
		android.os.Process.killProcess(android.os.Process.myPid());// 杀掉进程彻底退出
	}

	public static void openAppsInMarket(Activity mainActivity, int requestCode) {
		String httpUrl = "https://play.google.com/store/apps/details?id=";
		String markUrl = "market://details?id=";

		try {
			StringBuilder marketUrl = new StringBuilder(markUrl)
					.append(mainActivity.getPackageName());
			Uri localUri = Uri.parse(marketUrl.toString());
			Intent localIntent = new Intent("android.intent.action.VIEW",
					localUri);
			if (requestCode != -1) {
				mainActivity.startActivityForResult(localIntent, requestCode);
			} else {
				mainActivity.startActivity(localIntent);
			}

		} catch (Exception e) {// google play app is not installed
			StringBuilder marketUrl = new StringBuilder(httpUrl)
					.append(mainActivity.getPackageName());
			Uri localUri = Uri.parse(marketUrl.toString());
			Intent localIntent = new Intent("android.intent.action.VIEW",
					localUri);
			if (requestCode != -1) {
				mainActivity.startActivityForResult(localIntent, requestCode);
			} else {
				mainActivity.startActivity(localIntent);
			}
		}
	}
}