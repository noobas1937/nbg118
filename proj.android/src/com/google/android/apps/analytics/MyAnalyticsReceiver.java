package com.google.android.apps.analytics;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

import org.cocos2dx.ext.Tracker;


/**
 * 参照自Google Analytics lib:AnalyticsReceiver 测试发现使用自定义别名类难以接收到此事件 src定义：
 * http://wiki.hcg-tech.com/pages/viewpage.action?pageId=9339693 数据抓取：
 * 注意测试发现不能使用String格式化"%1$s"-- 1直接去掉国家代码即可;age@androidmarket;OneMobile;en_ad
 * 2country;age@androidmarket;OneMobile;en_ad---代码里直接将country替换为实际国家代码 09-27
 * 15:34:40.159: I/AdInstall(11450):
 * referrer=utm_source=admob&utm_medium=banner&
 * utm_content=%3Bage%40androidmarket%3Badmob%3Ben_ad&utm_campaign=age 09-27
 * 15:34:40.169: I/AdInstall(11450): decode
 * referrer=utm_source=admob&utm_medium=
 * banner&utm_content=;age@androidmarket;admob;en_ad&utm_campaign=age 09-27
 * 15:34:40.179: D/AdInstall(11450): hcg-ref:;age@androidmarket;admob;en_ad
 * 09-27 15:34:52.962: I/AdInstall(11450): AdInstall from
 * url--cn;age@androidmarket;admob;en_ad
 * 
 */
public class MyAnalyticsReceiver extends BroadcastReceiver {
	private static String currentAdReffer = "unknown";

	static final String INSTALL_ACTION = "com.android.vending.INSTALL_REFERRER";
	private static final String REFERRER_KEY = "referrer";

	/**
	 * 安装完成后：电子市场会发送广播并启动此方法
	 */
	@Override
	public void onReceive(Context context, Intent intent) {
		// Log.d(TAG, "onReceive context=" + context);
		// 自己存储一份reffer，到本地，以用来跟踪公司内部广告分析
		try {
			saveAdRefferToLocal(context, intent);
		} catch (Exception e) {
			Tracker.reportException(context, e);
		}
	}

	private void saveAdRefferToLocal(Context context, Intent intent) throws Exception {
		if (!INSTALL_ACTION.equals(intent.getAction()))
			throw new Exception("wrong intent " + intent.getAction());
		// 接到安装通知 并进行分割提取hcg_ref后面的参数
		String refStr = intent.getStringExtra(REFERRER_KEY);
		// Log.d(TAG, "referrer=" + refStr);
		
		String referrer = getUrlString(refStr);
		// Log.d(TAG, "decode referrer=" + referrer);
		if ((!INSTALL_ACTION.equals(intent.getAction())) || (referrer == null))
			return;
		referrer = referrer.toLowerCase();
		// referre参考格式：
		// ;---> %3B
		// =---> %3D
		// &--->%26
		// 国家;redmine项目名@项目所在平台;广告渠道;广告渠道代理商;自定义字段1;...自定义字段N
		// 注意：项目所在平台与Manifest文件中：
		// <meta-data android:name="XINGCLOUD_CHANNEL"
		// android:value="androidmarket" />
		// 保持一致
		// --------------------------
		// [小镇]referrer=us;snsnations@androidmarket;admob;image_10
		// [AOE]referrer=pl;age@androidmarket;onenet
		// 05-22 13:59:32.879: I/ref(30271):
		// referrer=utm_source=google&utm_medium=cpc&utm_content=pl%3Bage%40androidmarket%3Bonenet&utm_campaign=onenet
		// decode
		// referrer=utm_source=google&utm_medium=cpc&utm_content=pl;age@androidmarket;onenet&utm_campaign=onenet
		final String utm_content = "utm_content";
		if (isFromAdLeadbolt(context, referrer))
			return;
		String refc[] = referrer.split("&");
		for (int i = 0; i < refc.length; i++) {
			if (refc[i].startsWith(utm_content)) {
				currentAdReffer = refc[i].substring(utm_content.length() + 1,
						refc[i].length());
				saveAdReferrer(context, currentAdReffer);// 保存安装通知到本地
				break;
			}
		}
		if (currentAdReffer.equals("unknown")) {
            for (int i = 0; i < refc.length; i++) {
                if (refc[i].equals("utm_source=cpb")) {
                    currentAdReffer = "us;age@androidmarket;cpb_unknown;banner";
                    saveAdReferrer(context, currentAdReffer);// 保存安装通知到本地
                    break;
                }
            }
            if (currentAdReffer.equals("unknown")) {
//                Tracker.reportException(context, new Exception("unknown install channel "+referrer));
//                Tracker.sendEvent(context, "install", "bad referrer:"+referrer);
                return;
            }
        }
    
//		Tracker.sendEvent(context, "install", currentAdReffer);
    }

	private static boolean isFromAdLeadbolt(Context context, String referrer) {
		if (referrer.contains(AdInstallRefMointor.AD_FROM_LEADBOLT)) {
			//market://details?id=com.stac.empire.main&
			//referrer=utm_source=leadbolt&utm_medium=[TRACK_ID]&utm_content=[CLK_ID]&utm_campaign=age
			// 特殊格式再次转换下
			currentAdReffer="us;age@androidmarket;"+ AdInstallRefMointor.AD_FROM_LEADBOLT;
			saveAdReferrer(context, currentAdReffer);// 保存安装通知到本地
			return true;
		}
		return false;
	}

	private static String getUrlString(String url) {
		try {
			return URLDecoder.decode(url, "utf-8");
		} catch (UnsupportedEncodingException e) {
			return null;
		}

	}

	public static final String AD_INFO = "AdReferrerStore"; // 广告信息
	public static final String AD_REF = "referrer";

	/**
	 * 保存广告信息数据 ad_info
	 * 
	 */
	private static void saveAdReferrer(Context context, String referrer) {
		SharedPreferences sharedPreferences = context.getSharedPreferences(
				AD_INFO, Context.MODE_PRIVATE);
		Editor editor = sharedPreferences.edit();// 获取编辑器
		editor.putString(AD_REF, referrer);
		editor.commit();// 提交
	}

	public static String getAdReferrer(Context context) {
		SharedPreferences sharedPreferences = context.getSharedPreferences(
				AD_INFO, Context.MODE_PRIVATE);
		String referrer = sharedPreferences.getString(AD_REF, null);
		return referrer;
	}
}
