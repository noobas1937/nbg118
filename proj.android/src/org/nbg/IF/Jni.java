package org.nbg.IF;

import io.branch.referral.Branch;
import io.branch.referral.Branch.BranchLinkCreateListener;
import io.branch.referral.BranchError;

import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

import org.cocos2dx.ext.Native;
import org.nbg.IF.R;

import com.appsflyer.AFInAppEventParameterName;
import com.appsflyer.AFInAppEventType;
import com.appsflyer.AppsFlyerLib;
import com.helpshift.HelpshiftBridge;
//import com.testin.agent.TestinAgent;





import org.json.JSONObject;
import org.nbg.notifies.LocalNotification;
import org.nbg.notifies.LocalNotificationManager;
import org.nbg.stac.empire.common.manager.NotificationReceiver;
import org.nbg.stac.empire.sns.FBUtil;
import org.nbg.util.AOEUtil;
import org.nbg.util.GameContext;
import org.nbg.util.MiAnalyticsUtil;
import org.nbg.util.AOEUtil.MD5;

import android.R.string;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.util.Log;

public class Jni {
	public static native String nativeGetAppId();
	private static int notifyCount = 0;
	private static LocalNotificationManager manager = null;
	private static LocalNotificationManager getManager(){
		if(manager == null){
			manager = new LocalNotificationManager(GameContext.getActivityInstance());
		}
		return manager;
	}
	
	public static void cancelNotification(int type){
		if(type == 987 && disconnectFuture != null){
			disconnectFuture.cancel(false);
			return;
		}
		cancel(Integer.toString(type));
//		Intent intent = new Intent(IF.getInstance(), NotificationReceiver.class);
//		intent.putExtra("type", type);
//		PendingIntent sender = PendingIntent.getBroadcast(IF.getInstance(), type, intent, 0);	
//		AlarmManager am = (AlarmManager) IF.getInstance().getSystemService(Context.ALARM_SERVICE);
//		am.cancel(sender);		
	}
	
	private static ScheduledExecutorService disconnectService = Executors.newSingleThreadScheduledExecutor();
	private static ScheduledFuture<?> disconnectFuture;
	public static void pushNotification(int type,int time, String text, String soundKey, String pushType){
		if((type == 987 || type == 988 || type == 989) && (disconnectFuture == null || disconnectFuture.isCancelled() || disconnectFuture.isDone())){
			disconnectFuture = disconnectService.schedule(new Runnable() {
				@Override
				public void run() {
					Log.d("TimerTask", "Net disconnect");
					//SFS disconnect
					Net.logout();
					Net.disconnect();					
				}
			} , 300000, TimeUnit.MILLISECONDS);
			return;
		}
		sendNotify(text,text,time,Integer.toString(type),soundKey,pushType);
		// 包装延时通知intent, 目标为游戏统一接收器NotificationReceiver
//		Intent intent = new Intent(IF.getInstance(), NotificationReceiver.class);
//		intent.putExtra("type", type);
//		intent.putExtra("text", text);
//		PendingIntent sender = PendingIntent.getBroadcast(IF.getInstance(),type, intent, 0);
//		// 设定时间，我们想要在data.getTime()秒之后进行通知
//		Calendar calendar = Calendar.getInstance();
//		calendar.setTimeInMillis(System.currentTimeMillis());
//		calendar.add(Calendar.SECOND, time);
//		// 启动一个定时器
//		AlarmManager am = (AlarmManager)IF.getInstance().getSystemService(Context.ALARM_SERVICE);
//		am.set(AlarmManager.RTC_WAKEUP, System.currentTimeMillis()+1000*time, sender);		
	}

//	public static void sendNotify(String title,String body,String identify, String soundName, String playSound, int seconds){
//		sendNotify(title,body,seconds,identify,null, soundName, playSound);
//	}

	public static void sendNotify(String title,String body,int seconds,String identify,String soundKey,String pushType){
		Object actionParam = null;
		++notifyCount;
		Log.d("LocalNotify", "send notify after " + seconds + " seconds with body: " + body);
//		seconds = adjustTime(seconds);
		Log.d("LocalNotify", "local notification seconds after adjust: " + seconds);
		LocalNotification notification = new LocalNotification(GameContext.getActivityInstance().getClass().getName());
		notification.code = identify;
		// 设定时间，我们想要在data.getTime()秒之后进行通知
		Calendar calendar = Calendar.getInstance();
		calendar.setTimeInMillis(System.currentTimeMillis());
		calendar.add(Calendar.SECOND, seconds);

		notification.fireDate = new Date(System.currentTimeMillis()+1000*seconds);
		notification.repeatInterval = 0;
		notification.body = body;
		notification.pushType = pushType;
		notification.title = IF.getContext().getString(R.string.app_name);
		notification.numberAnnotation = notifyCount;
		notification.iconResourceId = R.drawable.notification_icon;
		notification.soundKey = soundKey;
		notification.vibrate = true;
		
//		if(soundName != null && soundName.length() > 0){
//			notification.soundName = soundName;
//		}
		if(actionParam == null){
			notification.actionData = new byte[0];
		}else{
//			TODO: support for actionParam
//			notification.actionData = actionParam;
			notification.actionData = new byte[0];
		}
		
		getManager().notify( notification);
		getManager().persistNotification(notification);
	}
	
	public static String getPushRecordData(){
		final SharedPreferences pushRecordSharedPreferences = GameContext.getActivityInstance().getSharedPreferences(LocalNotificationManager.CACHE_NOTIF_RECORD_KEY, Context.MODE_PRIVATE);
		Log.d("COK", "COK getPushRecordData"+pushRecordSharedPreferences.getString(LocalNotificationManager.PUSH_RECORD, ""));
		return pushRecordSharedPreferences.getString(LocalNotificationManager.PUSH_RECORD, "");
	}
	public static String getPushClickData(){
		final SharedPreferences pushRecordSharedPreferences = GameContext.getActivityInstance().getSharedPreferences(LocalNotificationManager.CACHE_NOTIF_RECORD_KEY, Context.MODE_PRIVATE);
		Log.d("COK", "COK getPushClickData"+pushRecordSharedPreferences.getString(LocalNotificationManager.PUSH_CLICK_DATA, ""));
		return pushRecordSharedPreferences.getString(LocalNotificationManager.PUSH_CLICK_DATA, "");
	}
	public static void clearPushCache(){
		final SharedPreferences pushRecordSharedPreferences = GameContext.getActivityInstance().getSharedPreferences(LocalNotificationManager.CACHE_NOTIF_RECORD_KEY, Context.MODE_PRIVATE);
		Editor pushRecordEditor = pushRecordSharedPreferences.edit();
		pushRecordEditor.clear();
		pushRecordEditor.commit();
	}
	
	//第三方统计事件触发start
	public static void triggerEventLoginComplete(String userId,String userName,String userLevel){
		AppsFlyerLib.setAppUserId(userId);

		IF.getInstance().collectUserInfo(userId, userName, userLevel);

		MiAnalyticsUtil.triggerEventLoginComplete(userId, userLevel);
		
		final SharedPreferences sharedPreferences = GameContext.getActivityInstance().getSharedPreferences(LocalNotificationManager.CACHE_GAME_UID_KEY, Context.MODE_PRIVATE);
		Editor editor = sharedPreferences.edit();
		editor.putString("GAMEUID", userId);
		editor.commit();
	}
	
	public static void triggerEventCompletedRegistration(){
		FBUtil.fbEventCompletedRegistration();

		AppsFlyerLib.trackEvent(IF.getInstance(),AFInAppEventType.COMPLETE_REGISTRATION,null);
		
		IF.getInstance().triggerEventCompletedRegistration();
	}
	
	public static void triggerEventCompletedTutorial(){
		FBUtil.fbEventCompletedTutorial();

		AppsFlyerLib.trackEvent(IF.getInstance(),AFInAppEventType.TUTORIAL_COMPLETION,null);
	}
	
	public static void triggerEventAchievedLevel(int level){
		FBUtil.fbEventAchievedLevel(level);

		Map<String, Object> eventValue = new HashMap<String, Object>();
		eventValue.put(AFInAppEventParameterName.LEVEL,level);
		AppsFlyerLib.trackEvent(IF.getInstance(),AFInAppEventType.LEVEL_ACHIEVED,eventValue);
		
		MiAnalyticsUtil.triggerEventAchievedLevel(level);
		
		IF.getInstance().triggerEventAchievedLevel(level);
	}
	
	public static void triggerEventPurchaseInit(String cost,String itemId){
		MiAnalyticsUtil.triggerEventPurchaseInit(cost);
	}
	
	public static void triggerEventPurchase(String cost,String itemId){
//		FBUtil.fbEventPurchase(cost, itemId);
		
		Map<String, Object> eventValue = new HashMap<String, Object>();
		eventValue.put(AFInAppEventParameterName.REVENUE,cost);
		eventValue.put(AFInAppEventParameterName.CONTENT_ID,itemId);
		eventValue.put(AFInAppEventParameterName.CURRENCY,"USD");
		AppsFlyerLib.trackEvent(IF.getInstance(),AFInAppEventType.PURCHASE,eventValue);
		
		MiAnalyticsUtil.triggerEventPurchase(cost);
	}
	
	//第三方统计事件触发end


	private static int lastSeconds = 0;
	private static int adjustTime(int seconds)
	{
//		if(Jni.isDebugMode){
//			return seconds;
//		}
		
		if(lastSeconds > 0 && (seconds - lastSeconds) < 3600){
			seconds = seconds + 3600;
		}
		
		if(seconds < 1200){
			Log.d("LocalNotify","seconds less then 1200, reset to 1200");
			seconds = 1200;
		}
		
		lastSeconds = seconds;
		Date date = new Date();
		long tmpTime = date.getTime() + seconds*1000;
		date.setTime(tmpTime);
		int h = date.getHours();
		Log.d("LocalNotify", "adjust time for hours: " + h + " date:" + date);
		int ret = seconds;
		if(h<9 || h>=22)
		{
			if(h<9)
			{
				ret += (9-h) * 3600;
			}
			else
			{
				ret += (24+9-h)* 3600;
			}
		}
		return ret;
	}
	
	public static void cancel(String identify){
		if("-1".equals(identify)){
			getManager().cancelAll();
			getManager().unpersistAllNotifications();
		}else{
			getManager().cancel(identify);
			getManager().unpersistNotification(identify);
		}
	}
	
	public static void cancelAll(){
		Log.d("LocalNotify", "start cancel all");
		getManager().cancelAll();
		getManager().unpersistAllNotifications();
		getManager().clearCurrentNotificationContent();
		Log.d("LocalNotify", "end cancel all");
	}
	
	public static String getMD5String(String str){
		return MD5.getMD5Str(str);
	}
	
	public static void recordCmd(String cmd){
		System.out.print("setCmd " + cmd);
		IF.getInstance().recordCmd(cmd);
	}
	
	public static void doPlatformLoading(){
		IF.getInstance().doPlatformLoading();
	}

	/*
	// JNI methods
	public static void showWebView(String url, int titleId) {
		final String address = url;
		final String titleString = Main.getInstance().getResources()
				.getString(titleId);
		Main.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				final Dialog dlgh = new Dialog(Main.getInstance());
				dlgh.requestWindowFeature(Window.FEATURE_NO_TITLE);
				LayoutInflater inflater = Main.getInstance()
						.getLayoutInflater();

				dlgh.setContentView(inflater.inflate(R.layout.webdlg2, null));
				((Button) dlgh.findViewById(R.id.btnClose))
						.setOnClickListener(new OnClickListener() {
							@Override
							public void onClick(View v) {
								dlgh.dismiss();
							}
						});

				WebView webview = (WebView) dlgh.findViewById(R.id.webView);
				webview.setBackgroundColor(0);
				webview.setScrollBarStyle(View.SCROLLBARS_INSIDE_OVERLAY);
				webview.getSettings().setJavaScriptEnabled(true);

				webview.setWebViewClient(new WebViewClient() {
					@Override
					public boolean shouldOverrideUrlLoading(WebView view,
							String url) {
						view.loadUrl(url);
						return true;
					}
				});
				final ProgressBar progress = (ProgressBar) dlgh
						.findViewById(R.id.progress);
				webview.setWebChromeClient(new WebChromeClient() {
					@Override
					public void onProgressChanged(WebView view, int nProgress) {
						progress.setVisibility(View.VISIBLE);
						progress.setProgress(nProgress);
						if (nProgress == 100)
							progress.setVisibility(View.INVISIBLE);
					}
				});
				webview.loadUrl(address);
				TextView title = (TextView) dlgh.findViewById(R.id.title);
				title.setText(titleString);
				WindowManager.LayoutParams lp = new WindowManager.LayoutParams();
				lp.copyFrom(dlgh.getWindow().getAttributes());
				DisplayMetrics dm = new DisplayMetrics();
				dlgh.getWindow().getWindowManager().getDefaultDisplay()
						.getMetrics(dm);
				if (dm.widthPixels <= 480)
					lp.width = WindowManager.LayoutParams.FILL_PARENT;
				else
					lp.width = (int) (dm.widthPixels * 0.85);
				lp.height = (int) (dm.heightPixels * 0.85);// WindowManager.LayoutParams.FILL_PARENT;
				dlgh.show();
				dlgh.getWindow().setAttributes(lp);
			}
		});
	}

	public static void showHelpView() {
		// todo::getLanguage
		String lan = Device.getLanguage();
		String dir = "";
		if (!lan.contains("zh")) {
			dir = "-" + lan;
		}
		final String faq = "file:///android_asset/help" + dir + "/faq.html";
		final String pay = "file:///android_asset/help" + dir + "/pay.html";
		final String newbie = "file:///android_asset/help" + dir
				+ "/newbie.html";
		final String walkthrough = "file:///android_asset/help" + dir
				+ "/walkthrough.html";
		Main.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				final Dialog dlgh = new Dialog(Main.getInstance());
				dlgh.requestWindowFeature(Window.FEATURE_NO_TITLE);

				LayoutInflater inflater = Main.getInstance()
						.getLayoutInflater();
				dlgh.setContentView(inflater.inflate(R.layout.webdlg, null));

				((Button) dlgh.findViewById(R.id.btnClose))
						.setOnClickListener(new OnClickListener() {
							@Override
							public void onClick(View v) {
								dlgh.dismiss();
							}
						});

				Button btnFAQ = (Button) dlgh.findViewById(R.id.btnFAQ);
				btnFAQ.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {
						WebView webview = (WebView) dlgh
								.findViewById(R.id.webView);
						webview.loadUrl(faq);
					}
				});
				Button btnPay = (Button) dlgh.findViewById(R.id.btnPay);
				btnPay.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {
						WebView webview = (WebView) dlgh
								.findViewById(R.id.webView);
						webview.loadUrl(pay);
					}
				});
				Button btnNewbie = (Button) dlgh.findViewById(R.id.btnNewbie);
				btnNewbie.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {
						WebView webview = (WebView) dlgh
								.findViewById(R.id.webView);
						webview.loadUrl(newbie);
					}
				});
				Button btnZishen = (Button) dlgh.findViewById(R.id.btnZishen);
				btnZishen.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {
						WebView webview = (WebView) dlgh
								.findViewById(R.id.webView);
						webview.loadUrl(walkthrough);
					}
				});

				WebView webview = (WebView) dlgh.findViewById(R.id.webView);
				webview.setScrollBarStyle(View.SCROLLBARS_INSIDE_OVERLAY);
				webview.getSettings().setJavaScriptEnabled(true);
				webview.setWebViewClient(new WebViewClient() {
					@Override
					public boolean shouldOverrideUrlLoading(WebView view,
							String url) {
						view.loadUrl(url);
						return true;
					}
				});
				final ProgressBar progress = (ProgressBar) dlgh
						.findViewById(R.id.progress);
				webview.setWebChromeClient(new WebChromeClient() {
					@Override
					public void onProgressChanged(WebView view, int nProgress) {
						progress.setVisibility(View.VISIBLE);
						progress.setProgress(nProgress);
						if (nProgress == 100)
							progress.setVisibility(View.INVISIBLE);
					}
				});
				webview.loadUrl(faq);

				WindowManager.LayoutParams lp = new WindowManager.LayoutParams();
				lp.copyFrom(dlgh.getWindow().getAttributes());
				DisplayMetrics dm = new DisplayMetrics();
				dlgh.getWindow().getWindowManager().getDefaultDisplay()
						.getMetrics(dm);
				if (dm.widthPixels <= 480)
					lp.width = WindowManager.LayoutParams.FILL_PARENT;
				else
					lp.width = (int) (dm.widthPixels * 0.85);
				lp.height = (int) (dm.heightPixels * 0.85);// WindowManager.LayoutParams.FILL_PARENT;
				webview.setBackgroundColor(0);
				dlgh.show();
				dlgh.getWindow().setAttributes(lp);
			}
		});
	}
	*/
	
	public static void showNewsView(String url) {
		//showWebView(url, R.string.news_title);
	}

	public static void showContactUsView(String url) {
		//showWebView(url, R.string.btn_email_us);
	}
	
	//clipBoard
	
	public static String clipboardGetText() {
		try {
			return IF.getInstance().clipboard.getText().toString();
		} catch (Exception e) {
//			reportException(Main.getInstance(), e);
		}
		return "";
	}
	
	public static void clipboardSetText(final String text) {
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				try {
					IF.getInstance().clipboard.setText(text);
				} catch (Exception e) {
//					reportException(Main.getInstance(), e);
				}
			}
		});
	}
	
	public static boolean isNetworkAvailable()
	{
		return AOEUtil.isNetworkAvailable(IF.getInstance());
	}
	
	public static void finishGame() {
		IF.getInstance().finishGame();
	}	
	
	public static void setTestinUser(String _user) {
//		try{
//			TestinAgent.setUserInfo(_user);
//		}catch(Exception e){
//			
//		}
	}
	
	public static void writeLog(String str){
		Log.d("call_by_2dx", str);
	}
	
	public static void doSetHelpShiftUserInfo(String userId,String userName,String userLevel){
		HelpshiftBridge.setUserIdentifier(userId);
		HelpshiftBridge.setNameAndEmail(userName, "");
	}
	public static boolean needInit = true;
	public static void doHelpShiftInit(){
		if(!needInit)
			return;
		HelpshiftBridge.install(IF.getInstance(),
	      "d364a0d2a0b3856ba1aa8d4cc0371855",
	      "elex.helpshift.com",
	      "elex_platform_20151106082553299-dfa2768467c76a9");
		needInit = false;
	}
	
	public static void doSetBranch(String key, String value){
		Log.d("branch::", "doSetBranch");
		JSONObject obj = new JSONObject();
		try{
			obj.put(key, value);
		}catch(Exception e){
			
		}
		
		Branch branch = Branch.getInstance(IF.getContext());
	    branch.getShortUrl(obj, new BranchLinkCreateListener() {
			@Override
			public void onLinkCreate(String url, BranchError error) {
				Log.d("branch::", url);
				Native.postNotification("branch", url);
			}
		});
	}
	
	public static void redeemCredit(int credit){
		Branch branch = Branch.getInstance(IF.getContext());
		branch.redeemRewards(credit);
	}
	
	public static int getCredit(){
		Branch branch = Branch.getInstance(IF.getContext());
		return branch.getCredits();
	}
	
	public static void saveCrashPID(){
		IF.getInstance().saveCrashPID();
	}
    
    public static void relogin(){
        Net.logout();
        Net.disconnect();
        Intent i = IF.getInstance().getBaseContext().getPackageManager()
        .getLaunchIntentForPackage( IF.getInstance().getBaseContext().getPackageName() );
        i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        IF.getInstance().finish();
        IF.getInstance().startActivity(i);
        System.exit(0);
    }
}
