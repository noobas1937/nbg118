package org.cocos2dx.ext;

import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.text.TextUtils;
import android.util.Log;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.net.HttpURLConnection;
import java.util.List;
import java.util.Map;

import org.json.JSONObject;
import org.nbg.IF.IF;
import org.nbg.stac.empire.publish.IPublishChannel;
import org.nbg.stac.empire.sns.FBUtil;
import org.nbg.util.CommonUtil;
import org.nbg.util.GameContext;

import com.adjust.sdk.Reflection;

public class CppCallJava {
	
	public static String getGaid(){
		String gaid = Reflection.getPlayAdId(IF.getContext());
		Log.d("COK","COK read gaid "+ gaid);
		if(gaid == null || "".equals(gaid)){
			gaid = Reflection.getAndroidId(IF.getContext());
			Log.d("COK","COK read androidid "+ gaid);
		}
		if(gaid == null || "".equals(gaid))
			return null;
		return gaid;
	}

	public static String getServerIp() {
		return GameContext.getGamePublisher().getPublish_ServerIp();
	}
	
	public static String getServerZone() {
		return GameContext.getGamePublisher().getPublish_ServerZone();
	}
	
	public static int getServerPort() {
		return GameContext.getGamePublisher().getPublish_ServerPort();
	}
	
	public static void gotoMarket(String url) {
//		return GameContext.getGamePublisher().getPublish_ServerPort();
		try{
			Uri localUri = Uri.parse(url);
			Intent intent = new Intent(Intent.ACTION_VIEW, localUri);
			GameContext.getActivityInstance().startActivity(intent);
		}catch(Exception e)
		{
			
		}
//		GameContext.release();
//		GameContext.getActivityInstance().finish();
//		android.os.Process.killProcess(android.os.Process.myPid());// 杀掉进程彻底退出
		
//		intent.setAction(Intent.ACTION_VIEW);
		
	}
	
	public static String getNewsUrlVerCode(String url)
	{
//		boolean isNeedShow = true;
//		String localVersion = AccountStorage.getInstance()
//				.getNewsCacheName();// 1353144047NewsCache.html
//		if (localVersion == null) {
//			isNeedShow = true;
//		} else {
//		}
//		String locVerCode = localVersion.substring(
//				0,
//				localVersion.length()
//						- "NewsCache.html".length());
		String strLocalUrl = null;
		HttpURLConnection httpConnector = null;
		try {
			URL myurl = new URL(url);
			httpConnector = (HttpURLConnection) myurl
					.openConnection();
			httpConnector.setInstanceFollowRedirects(false);
			httpConnector.setConnectTimeout(3000);
			httpConnector.connect();
			int nResponseCode = httpConnector.getResponseCode();
			if (nResponseCode == HttpURLConnection.HTTP_MOVED_TEMP) {// 重定向
																		// 302
				// http://aoe91.hcgapp.com:8081/news/android_cn/news_cn.html?version=1352887242
				Map<String, List<String>> mapHead = httpConnector
						.getHeaderFields();
				if (mapHead != null
						&& mapHead.containsKey("Location")) {
					List<String> value = mapHead.get("Location");
					strLocalUrl = value.get(0);// [news_cn.html?version=1353144047]
//					if (strLocalUrl.contains(locVerCode)) {
//						isNeedShow = false;
//					}
				}
			}
		} catch (IOException e) {
			return strLocalUrl;
		} finally {
			
		}
		
		return strLocalUrl;
	}
	
	public static String getNetworkStatus(){
		String ret = "no";
		ConnectivityManager manager = (ConnectivityManager) GameContext.getActivityInstance().getApplicationContext().getSystemService(
				Context.CONNECTIVITY_SERVICE);
		if (manager == null) {
			
		}else {
			NetworkInfo networkinfo = manager.getActiveNetworkInfo();
			if (networkinfo != null) {
				switch (networkinfo.getType()) {
				case 1:
				case 6:
					ret = "wifi";
					break;
				case 0:
				case 2:
				case 3:
				case 4:
				case 5:
					ret = "mobile";
				default:
					break;
				}
			}
		}
		return ret;
	}
	
	public static void sendMail(String address,String title,String content){
		
		final String faddress = address;
		final String ftitle = title;
		final String fcontent = content;
		
		GameContext.getActivityInstance().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				try {
					Intent email = new Intent(Intent.ACTION_SENDTO);
					email.setType("plain/text");
					email.setData(Uri.parse("mailto:"
							+ faddress));

					email.putExtra(Intent.EXTRA_SUBJECT, ftitle);
					// 设置要默认发送的内容
					email.putExtra(Intent.EXTRA_TEXT, fcontent);
					// 调用系统的邮件系统
					GameContext.getActivityInstance().startActivity(email);
				} catch (ActivityNotFoundException e) {
					// TODO: handle exception
				}
			}
		});
	}
	
//	public static void xingCloudTrackSender(ISFSArray actions,int isCheckFirst){
//		String[] actsStrings = new String[actions.size()];
//		for (int i = 0; i < actions.size(); i++) {
//			actsStrings[i] = actions.getUtfString(i);
//		}
//		if (isCheckFirst == 1) {
//			CloudAnalysisUitl.trackClickEventCheckFirst(actsStrings);
//		} else {
//			CloudAnalysisUitl.trackClickEvent(actsStrings);
//		}
//	}
	
	public static boolean isPublish_360(){
		return TextUtils.equals(GameContext.getGamePublisher().getPublish_Region(),IPublishChannel.PUBLISH_360);
//		AppData.getInstance().getLoginData().getChannel().equals(IPublishChannel.PUBLISH_360);
	}
	
	public static String getPublishRegion(){
		return GameContext.getGamePublisher().getPublish_Region();
	}
	
	public static boolean isNewInstallDevice(){
		return CommonUtil.isNewInstallDevice();
	}
	

	/**
	 * 
	 * @param platform
	 *            :0-Google-plus,1其他
	 * @return
	 */
	public static String getLoginedInfo_Platform(int platform) {
		try {
			if (platform == 0) {
//				return GooglePlusHelper.getInstance().getLoginInfo_GooglePlus();
				return IF.getInstance().getGoogleLoginInfo();
			} else if (platform == 1) {
				return "";//getLoginedInfo_3rd();
			}

		} catch (Exception e) {
		}
		JSONObject obj = new JSONObject();
		return obj.toString();
	}
	
	/**
	 * 登入第三方平台
	 * @param platform
	 */
	public static void login_Platform(int platform){
		if (platform == 0) {
			// GooglePlusHelper.getInstance().login();
			try {
				IF.getInstance().doGoogleSignIn();
			} catch (java.lang.IllegalStateException e) {
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * 登出第三方平台
	 * @param platform
	 */
	public static void logout_Platform(int platform){
		if (platform == 0){
//			GooglePlusHelper.getInstance().logout();
			try{
				IF.getInstance().doGoogleSignOut();
			} catch (java.lang.IllegalStateException e) {
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * 是否可使用第三方平台
	 * @param platform
	 * @return
	 */
	public static boolean isAvailable_Platform(int platform){
		if (platform == 0) {
			// return
			// GooglePlusHelper.getInstance().isGooglePlayServicesAvailable();
			try {
				return IF.getInstance().isGooglePlayServicesAvailable();
			} catch (java.lang.IllegalStateException e) {
				e.printStackTrace();
			}
		}
		return false;
	}
	
	/**
	 * 是否已经连接到第三方平台
	 * @param platform
	 * @return
	 */
	public static boolean isConnected_Platform(int platform){
		if (platform == 0){
//			return GooglePlusHelper.getInstance().isConnected();
			try{
				return IF.getInstance().isGoogleSignedIn();
			} catch (java.lang.IllegalStateException e) {
				e.printStackTrace();
			}
		}
		return false;
	}
	
	public static void onUploadPhoto(int uid, int srcCode, int idx){
//		Log.d(TAG, "onUploadPhoto2: " +srcCode);
		//Log.e(TAG, "onUploadPhoto2: " +srcCode);
		try
		{
			if(srcCode == 0)
			{
				// 拍照
				IF.getInstance().showPicturePicker1(uid, idx);
			}
			else if(srcCode == 1)
			{
				// 从本地选取照片
				 IF.getInstance().showPicturePicker2(uid, idx);
			}
		}
		catch(Exception ext){
			//
		}
	}
	
	public static String getSDcardPath(){
		String path = "";
		if (CommonUtil.isAvaiableSDcard())//check SDcard available
		{
			path = CommonUtil.getSDcardPath();
			path = path + File.separator + "data"+File.separator+"data"+File.separator+GameContext.getActivityInstance().getPackageName() + File.separator + "head";

			File dir = new File(path);
			if(!dir.exists() || !dir.isDirectory())
			{
				if(dir.mkdirs())
				{
					path = dir.getAbsolutePath() + File.separator;
				}
				else
				{
					path = CommonUtil.getSDcardPath() + File.separator;
				}
			}
			else
			{
				path = dir.getAbsolutePath() + File.separator;
			}
			File nomedia = new File(path + File.separator + ".nomedia");
			if(!nomedia.exists() || !nomedia.isDirectory())
			{
				nomedia.mkdirs();
			}
		}
//		Log.d("getSDcardPath", "getSDcardPath="+path);
		return path;
	}
}
