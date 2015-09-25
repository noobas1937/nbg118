package com.google.android.apps.analytics;

import java.util.Locale;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.util.Log;
import org.cocos2dx.ext.Tracker;

import org.hcg.util.GameContext;
//import com.xingcloud.analytic.CloudAnalytic;

/**
 * 推广渠道跟踪
 */
public class AdInstallRefMointor {
	public static final String AD_FROM_TAPJOY = "tapjoy";
	public static final String AD_FROM_LEADBOLT = "leadbolt";
	public static final String AD_FROM_APPLOVIN = "applovin";
	public static final String AD_FROM_ALEADPAY = "aleadpay_";
	public static final String AD_FROM_SPONSORPAY = "sponsorpay";

	public static final String AD_FROM_CHARTBOOST="chartboost";
	public static final String AD_FROM_GLISPA = "glispa";
	public static final String AD_FROM_FACEBOOK="facebook";
	
	//hasoffers 可以使用两种链接：1跳转链接无法跟踪渠道数据，2 使用直接链接可以跟踪渠道数据
	//1跳转链接：http://hastrk1.com/serve?action=click&publisher_id=23508&site_id=28826&offer_id=255398
	//转换后的URL:
	//https://play.google.com/store/apps/details?id=com.stac.empire.main&referrer=tracking_id%3Dd341a8280c41906d498c721bad1c4517
	//特别说明 hasoffers无法跟踪渠道参数，只能通过 tracking_id来识别
	//D/AdInstall(24219): decode referrer=tracking_id=de76924dd6c12f8e08d0ccd95617b0e7
	//2 直接链接：market://details?id=com.stac.empire.main&referrer=sub_publisher%3Dhasoffers%26sub_adgroup%3Dus%253Bage%2540androidmarket%253Bad_channel%253Bregion_ad%26publisher_id%3D23508%26offer_id%3D255398
	//referrer=sub_publisher=hasoffers&sub_adgroup=us;age@androidmarket;ad_channel;region_ad 
	
	//结论：直接链接会加大推广难度【运营方面】，确定使用跳转链接方式
	public static final String AD_FROM_HASOFFERS="tracking_id";//us;age@androidmarket;ad_channel;region_ad
	
	private static final String TAG = "AdInstall";
	public static String adInstalledInfo;

	/**
	 * 读取本地存储的广告跟踪信息
	 */
	private static String readLocalAdReferrer() {
		String referrer = MyAnalyticsReceiver.getAdReferrer(GameContext
				.getActivityInstance());
		Log.d(TAG, "LocaleStore-referrer:" + referrer);
		if (referrer != null && !referrer.equals("")) {
			return wrapRegionAdRef(referrer);
		}
		return null;
	}

	/**
	 * 
	 * @param activity
	 */
	public static String checkAdInstalledRef(Activity activity) {
		adInstalledInfo = null;
		String configAdRef = getConfigAdRef(activity);
		// 优先读取配置文件中SRC
		if (configAdRef == null) {
			adInstalledInfo = readLocalAdReferrer();
			Log.i("AdInstall", "AdInstall from url--" + adInstalledInfo);
		}

		if (adInstalledInfo != null) {// 安装通知非空时
//			CloudAnalytic.instance().setChannelReference(adInstalledInfo,
//					activity);
			return adInstalledInfo;
		}
		return null;
	}
	public static boolean isAdFromHasoffers(Activity activity) {
		return isAdFrom(activity, AD_FROM_HASOFFERS);
	}
	public static boolean isAdFromChartboost(Activity activity) {
		return isAdFrom(activity, AD_FROM_CHARTBOOST);
	}
	public static boolean isAdFromSponsorpay(Activity activity) {
		return isAdFrom(activity, AD_FROM_SPONSORPAY);
	}

	public static boolean isAdFromTapjoy(Activity activity) {
		return isAdFrom(activity, AD_FROM_TAPJOY);
	}

	public static boolean isAdFromLeadbolt(Activity activity) {
		return isAdFrom(activity, AD_FROM_LEADBOLT);
	}

	public static boolean isAdFromAppLovin(Activity activity) {
		return isAdFrom(activity, AD_FROM_APPLOVIN);
	}

	public static boolean isAdFromGLISPA(Activity activity) {
		return isAdFrom(activity, AD_FROM_GLISPA);
	}
	public static boolean isAdFromFaceBook(Activity activity) {
		return isAdFrom(activity, AD_FROM_FACEBOOK);
	}
	public static boolean isAdFrom(Activity activity, String adName) {
		if (adInstalledInfo == null) {
			checkAdInstalledRef(activity);
		}
		if (adInstalledInfo != null
				&& adInstalledInfo.toLowerCase().contains(adName)) {
			return true;
		}
		return false;
	}

	/**
	 * 动态监测方式：<br>
	 * 
	 * @see http://wiki.hcg-tech.com/pages/viewpage.action?pageId=9339693<br>
	 *      国家;游戏@平台;admob;广告组名<br>
	 *      注意：国家必须使用在redmine上的发布的国家，不能动态写死，目前游戏中全部使用"us"--from<br>
	 *      1 使用：us;cok@androidmarket;OneMobile;en_ad<br>
	 *      2 使用：us;cok@androidmarket;OneMobile;region_ad<br>
	 *      3 使用：us;cok@androidmarket;OneMobile;<br>
	 */
	private static String wrapRegionAdRef(String ad_src) {
		String region = Locale.getDefault().getCountry().toLowerCase();
		// 兼容老版本参数
		if (ad_src.startsWith(";cok@")) {
			// 1兼容：[;age@androidmarket;OneMobile;en_ad]<br>
			ad_src = "us" + ad_src;
		} else {// 2兼容: [country;age@androidmarket;OneMobile;en_ad]<br>
			ad_src = ad_src.replaceFirst("country", "us");
		}

		// 添加国家代码【广告组名】
		String regionAD = region + "_ad";
		if (ad_src.contains("region_ad")) {
			ad_src = ad_src.replaceFirst("region_ad", regionAD);
		}
		return ad_src;
	}

	/**
	 * 读取AndroidManifest.xml中配置的渠道来源
	 * 
	 * @param ctx
	 * @return
	 */
	private static String getConfigAdRef(Activity ctx) {
		ApplicationInfo info;
		try {
			info = ctx.getPackageManager().getApplicationInfo(
					ctx.getPackageName(), PackageManager.GET_META_DATA);
			String adInstalledInfo = null;
			String ad_src = info.metaData.getString("PUBLISH_AD_SRC");
			if (ad_src != null && !ad_src.equals("")) {
				adInstalledInfo = wrapRegionAdRef(ad_src);
			}
			return adInstalledInfo;
		} catch (NameNotFoundException e) {
			Tracker.reportException(ctx, e);
			return null;
		}
	}

	/**
	 * 根据配置的src生成GoogleAnalytics跟踪所用的reffer
	 * 
	 * @param src
	 * @return
	 */
	// private static String createAdReffer_GoogleAnalytics(String src) {
	// // String
	// //
	// originRef="utm_source=test_hcg&utm_medium=banner&utm_content=%3Bage%40androidmarket%3Btest_hcg%3Ben_ad&utm_campaign=age";";
	// String[] refs = src.split(";");
	// String projectName = refs[1].substring(0, refs[1].indexOf("@"));
	// StringBuffer refBuf = new StringBuffer();
	// refBuf.append("utm_source=").append(refs[2])
	// .append("&utm_medium=banner").append("&utm_content=")
	// .append(src).append("&utm_campaign=").append(projectName);
	// // Log.i("AdInstall", "GoogleAnalyticsTracker from--" +
	// // refBuf.toString());
	// // GoogleAnalyticsTracker
	// //
	// from--:utm_source=other&utm_medium=config&utm_content=us;age@androidmarket;other;cn_ad&utm_campaign=age
	// return refBuf.toString();
	// }
}
