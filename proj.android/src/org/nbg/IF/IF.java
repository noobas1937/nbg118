/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.nbg.IF;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHandler;
import org.cocos2dx.ext.Device;
import org.cocos2dx.ext.Jni;
import org.cocos2dx.ext.Native;

import android.net.Uri;
import android.net.http.SslError;
import android.os.Bundle;
import android.os.Environment;
import android.os.Message;
import android.provider.MediaStore;
import android.text.ClipboardManager;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.webkit.SslErrorHandler;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.content.Intent;
import android.app.AlarmManager;
import android.app.PendingIntent;
import io.branch.referral.Branch;
import io.branch.referral.BranchError;
import io.branch.referral.Branch.BranchReferralInitListener;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.math.BigDecimal;
import java.util.Calendar;
import java.util.Currency;
import java.util.Iterator;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import org.nbg.IF.R;
import org.json.JSONObject;
import org.nbg.IF.sns.SNSHelper;
import org.nbg.stac.empire.common.manager.GCMAsyncTask;
import org.nbg.stac.empire.common.manager.NotificationReceiver;
import org.nbg.stac.empire.publish.IPublishChannel;
import org.nbg.stac.empire.sns.FBUtil;
import org.nbg.util.AOEUtil;
import org.nbg.util.MiAnalyticsUtil;
import org.apache.commons.lang.StringUtils;

import com.adjust.sdk.Adjust;
import com.appsflyer.AppsFlyerLib;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.JniController;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.util.IAnalyticTracker;
import com.elex.chatservice.util.IJniCallHelper;
import com.elex.chatservice.util.LogUtil;
import com.facebook.AccessToken;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.Profile;
import com.facebook.ProfileTracker;
import com.facebook.appevents.AppEventsLogger;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.helpshift.HelpshiftBridge;
import com.nanigans.android.sdk.NanigansEventManager;
//import com.testin.agent.TestinAgent;
import com.umeng.analytics.MobclickAgent;
import com.xiaomi.migamechannel.MiGameChannel;


//import com.xingcloud.analytic.CloudAnalytic;
import org.cocos2dx.ext.Tracker;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.ResolveInfo;
import android.graphics.Bitmap;

import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.db.DBManager;
import android.os.Build; 


public class IF extends Cocos2dxActivity implements IAnalyticTracker ,IJniCallHelper{
	private static IF instance = null;
	private Tracker tracker;
	private LinearLayout m_webLayout;
	private WebView m_webView=null;
	private LinearLayout.LayoutParams linearParams;
	public ClipboardManager clipboard = null;
	private native void nativeGameStart();
	private SNSHelper m_snsHelper;

    public Payment m_payment;
    
	protected AppEventsLogger appEventsLogger;
	public AppEventsLogger getAppEventsLogger(){
		if(appEventsLogger == null){
			String FACEBOOK_APP_ID = this.getResources().getString(
					R.string.fb_app_id);
			appEventsLogger = AppEventsLogger.newLogger(instance,FACEBOOK_APP_ID,null);	
		}
		return appEventsLogger;
	}
	protected CallbackManager callbackManager;
	public CallbackManager getCallbackManager(){
		if(callbackManager == null){
			callbackManager = CallbackManager.Factory.create();
			LoginManager.getInstance().registerCallback(callbackManager,
		            new FacebookCallback<LoginResult>() {
		                @Override
		                public void onSuccess(LoginResult loginResult) {
		                	//已测试无法在这里获得getCurrentProfile
//	                		Log.d("fb", "fb login success");
//	                		Profile.fetchProfileForCurrentAccessToken();
//		                	Profile currentProfile = Profile.getCurrentProfile();
//		                	if(currentProfile != null){
//		                		Log.d("fb", "fb get profile success");
//		                		FBUtil.nativeSetIsLogin(true,currentProfile.getId(),currentProfile.getName());
//		                	}
		                }

		                @Override
		                public void onCancel() {
		                }

		                @Override
		                public void onError(FacebookException exception) {
		                }
		    });
		}
		return callbackManager;
	}
	protected ProfileTracker profileTracker;
	public ProfileTracker getProfileTracker(){
		if(profileTracker == null){
			profileTracker = new ProfileTracker() {
		        @Override
		        protected void onCurrentProfileChanged(
		                Profile oldProfile,
		                Profile currentProfile) {
		        	if(currentProfile != null){
		        		Log.d("fb", "fb onCurrentProfileChanged id " + currentProfile.getId());
		        		Log.d("fb", "fb onCurrentProfileChanged name " + currentProfile.getName());
		        		Log.d("fb", "fb change FBUtil.preRequest="+FBUtil.preRequest);
		        		FBUtil.nativeSetIsLogin(true,currentProfile.getId(),currentProfile.getName());
		        		Log.d("fb", "fb onCurrentProfileChanged FBUtil.preRequest="+FBUtil.preRequest);
		        		int requestType = FBUtil.preRequest;
		        		Log.d("fb", "fb onCurrentProfileChanged requestType="+requestType);
		        		if(requestType==5){
		        			Log.d("fb", "fb onCurrentProfileChanged execute feed");
		        	    	if(FBUtil.feedType==1 && !AccessToken.getCurrentAccessToken().getPermissions().contains("publish_actions")){
		        	    		FBUtil.feedType = 0;
		        	    	}
		        			FBUtil.rePublishFeedDialog();
		        		}
		        	}
		        	else
		        		Log.d("fb", "fb onCurrentProfileChanged profile is null");
		        }
		    };
		    profileTracker.startTracking();
		}
		FBUtil.preRequest = 0;
		return profileTracker;
	}
	
    static {
        System.loadLibrary("game");
    }
    
    public static IF getInstance()
    {
    	return instance;
    }

	public static SNSHelper getSNSHelper() {
		return instance.m_snsHelper;
	}
	
	private static void setDefaultUncaughtExceptionHandler() {
	    try {
	        Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
	            @Override
	            public void uncaughtException(Thread t, Throwable e) {
	            	if(e instanceof Exception)
	            		LogUtil.printException((Exception)e);
	            }
	        });
	    } catch (SecurityException e) {
	    	System.out.println("Could not set the Default Uncaught Exception Handler");
	    }
	}
	
	protected void recordCmd(String cmd){
		
	}
	
	public void showToast(String showMsg){
		Message message = Message.obtain();  
        message.obj = showMsg;
        message.what = Cocos2dxHandler.HANDLER_SHOW_TOAST;
        handleMessage(message);
	}
	
	public void collectUserInfo(String userId,String userName,String userLevel){
	}
	
	public void doPlatformLoading(){
	}
	
	public boolean isGoogleSignedIn(){
		return false;
	}
	
	public void doGoogleSignIn(){
		
	}
	
	public String getGoogleLoginInfo(){
		return "";
	}
	
	public void doGoogleSignOut(){
		
	}
	
	public boolean isGooglePlayServicesAvailable(){
		return false;
	}
	
	public void openLeaderBoards(){
	}

	public void openAchievements(){
	}

	public void submitScore(int score){
	}

	public void unlockAchievements(String id){
	}

	public void triggerEventAchievedLevel(int level){
	}
	
	public void triggerEventCompletedRegistration(){
	}
	
	public void changeAccount(){
		Native.nativeClearGuideData();
    	Net.logout();
    	Net.disconnect();
    	Intent i = getBaseContext().getPackageManager()
                .getLaunchIntentForPackage( getBaseContext().getPackageName() );
    	i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
    	finish();
   		startActivity(i);
    	System.exit(0);
	}
	
	//国内版本控制，否则会造成其余asynctask阻塞如qq登陆pingpp支付
	public boolean facebookEnabled = true;
	//gp目前未发现阻塞，保险加个开关
	public boolean googleplayEnabled = true;
	//小米统计
	public boolean miAnalyticsEnabled = false;
	
	protected void onCreate(Bundle savedInstanceState) {
//		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);
//		requestWindowFeature(Window.FEATURE_NO_TITLE);
		super.onCreate(savedInstanceState);
		instance = this;
		setDefaultUncaughtExceptionHandler();
		Native.nativeSetPlatformUID("");
		// facebook 初始化
		if(facebookEnabled){
			FacebookSdk.sdkInitialize(this);
			FacebookSdk.setApplicationName("Dragon Clans");
			getProfileTracker();
			getCallbackManager();
		}
		if(miAnalyticsEnabled)
	        MiGameChannel.Initialize(this, "com.nbg.mi", getPublishImpl().getPublish_Region());
		
		Native.nativeSetFromCountry(Locale.getDefault().getCountry());
		
		AppsFlyerLib.setAppsFlyerKey("2zyZ7Ubw9shuEjnRQZKiZX");
		AppsFlyerLib.sendTracking(getApplicationContext());
		
		try {
			clipboard = (ClipboardManager) getSystemService(CLIPBOARD_SERVICE);
		} catch (Exception e) {
			tracker.reportException(e);
		}
		
		Jni.init(this);
		try {
			HelpshiftBridge.install(this,
                    "d364a0d2a0b3856ba1aa8d4cc0371855",
                    "elex.helpshift.com",
                    "elex_platform_20151106082553299-dfa2768467c76a9");
		} catch (Exception e) {
			e.printStackTrace();
		}
		/** FBLikeButton */
		if(facebookEnabled)
			new HelpShiftTask().execute(this);
		/** GCM for HelpShift  */
		new GCMAsyncTask().execute(this);
        /** Crash log */
        new CrashLogTask().execute(this);
		//webView
		m_webLayout = new LinearLayout(this);
		this.addContentView(m_webLayout, 
				new LayoutParams(LayoutParams.FILL_PARENT,LayoutParams.FILL_PARENT));
		m_snsHelper = new SNSHelper(this);
		try{
			NanigansEventManager.getInstance().setDebug(false);
			NanigansEventManager.getInstance().setTrackingEnabled(true);
			NanigansEventManager.getInstance().onActivityCreate(instance, this.getResources().getString(
					R.string.fb_app_id), 217314);
			NanigansEventManager.getInstance().setUserId(Device.getUid());
			NanigansEventManager.getInstance().trackAppLaunch();
		} catch (Exception e) {

		}

		LogUtil.tracker = this;
		ChatServiceController.init(this, false);
		MailManager.tracker=this;
		JniController.jniHelper=this;
//		if(ChatServiceBridge.enableChatInputField){
//			ChatInputInterface.initChatInputView(this);
//			try{
//				ChatInputView chatInputView = getChatInputView(this);
//				this.addContentView(chatInputView, new LayoutParams(LayoutParams.FILL_PARENT,LayoutParams.FILL_PARENT));
//	//			AndroidBug5497Workaround.assistActivity(chatInputView);
//			} catch (Exception e) {
//				printException(e);
//			}
//		}
		
		// create game helper with all APIs (Games, Plus, AppState):
		
		PackageInfo info;  
		try {  
		    info = this.getPackageManager().getPackageInfo(this.getPackageName(), 0);  
		    // 当前应用的版本名称  
		    String versionName = info.versionName;  
		    // 当前版本的版本号  
		    int versionCode = info.versionCode;  
		    // 当前版本的包名  
		    String packageNames = info.packageName;  
		    Native.nativeAPKpackageName(packageNames);
		    Log.d("fb", "fb packageNames="+packageNames);
		} catch (NameNotFoundException e) {  
		    e.printStackTrace();  
		}  
		
		//作一个appLink 入口的记录
		Log.d("fb", "fb new appLink record start");
		Bundle appLinkData = getIntent().getBundleExtra("al_applink_data");
        if (appLinkData != null) {
            String targetURLString = appLinkData.getString("target_url");
            Log.d("fb", "fb targetURLString"+targetURLString);
            Uri targetURL = Uri.parse(targetURLString);
            Log.d("fb", "fb targetURL"+targetURL);
            String key = getUrlParam(targetURLString,"feed_key");
            Log.d("fb", "fb targetURL key="+key);
            String ids = getUrlParam(targetURLString,"request_ids");
            Log.d("fb", "fb targetURL request_ids="+ids);
            String nativeKey = "nativeKey";
            if(ids!=null &&!ids.equals("")){
            	nativeKey = ids;
            }
            if(key!=null &&!key.equals("")){
            	nativeKey = key;
            }
            //Native.nativeAPPLink(nativeKey);
            Native.nativeAPPLink(targetURLString);
            Log.d("fb", "fb targetURL request_ids end");
        }else{
        	Log.d("fb", "fb appLink appLinkData NULL");
        }
        
        if (!checkSDCardAvailable()) 
        {
        	FBUtil.appEventSdCardUnAvable(android.os.Environment.getExternalStorageState());
		}
        
		if (Jni.isDebugMode) {
			// 打开这个看sfs的debug信息
			// Net.debug=true;
			// 打开这个看umeng的debug信息
			// MobclickAgent.setDebugMode(true);
			// Log显示内存占用信息
			new Timer().schedule(new TimerTask() {
				@Override
				public void run() {
					Log.i("memory",
							AOEUtil.getDeviceAvaialbeMemorySize(IF.getInstance())
									+ "/"
									+ AOEUtil.getDeviceTotalMemory(IF
											.getInstance()));
				}
			}, 0, 10000);
		}
		savePhoneDevice();
	}

	// 若isAccountChanged，则强制重新初始化db
    public static void initDatabase(boolean isAccountChanged)
    {
		System.out.println("IF.initDatabase() : " + isAccountChanged);
		if(isAccountChanged && DBManager.getInstance().isDBAvailable()){
			DBManager.getInstance().closeDB();
		}
    	if(!DBManager.getInstance().isDBAvailable() && instance != null){
    		DBManager.getInstance().initDB(instance);
    	}
		if(isAccountChanged){
			ChatServiceController.getInstance().reset();
		}
    }
	
	private static String getUrlParam(String url, String paramName) {
		String paramValue = "";
		if (StringUtils.isBlank(url) || StringUtils.isBlank(paramName)) {
			return paramValue;
		}
		String urlArray[] = StringUtils.split(url, '?');
		if (urlArray.length > 1) {
			String paramsArray[] = StringUtils.split(urlArray[1], '&');
			for (int index = 0; index < paramsArray.length; index++) {
				String param[] = StringUtils.split(paramsArray[index], '=');
				if (paramName.equals(param[0])) {
					paramValue = param[1];
					break;
				}
			}
		}
		return paramValue;
	}
	
	private boolean isGameStart = false;
	
	@Override
	protected void onStart() {
		super.onStart();
//		try{
//			TestinAgent.onStart(this);
//		}catch(Exception e)
//		{
//			
//		}
		
		// 通知c代码启动游戏
		if (!isGameStart) {
			isGameStart = true;
		}
		
		try {
			Intent intent = new Intent(IF.getInstance(),
					NotificationReceiver.class);
			PendingIntent sender = PendingIntent.getBroadcast(
					IF.getInstance(), 1, intent, 0);
	
			// 当登入游戏后，取消掉AFK的Alarm通知
			AlarmManager am = (AlarmManager) getSystemService(ALARM_SERVICE);
			am.cancel(sender);
			
			org.nbg.IF.Jni.cancelAll();
			
//			CloudAnalytic.instance().onStart(this);
//			CloudAnalysisManager.onStart();
		} catch (Exception e) {
			tracker.reportException(e);
		}
		
		//branch
	    Branch branch = Branch.getInstance(getApplicationContext());
	    BranchReferralInitListener listener = new BranchReferralInitListener(){
	        @Override
	        public void onInitFinished(JSONObject referringParams, BranchError error) {
	            if (error == null) {
	                // params are the deep linked params associated with the link that the user clicked before showing up
	                // params will be empty if no data found

	            	Iterator<?> it = referringParams.keys();  
	                String key = "";  

	                while(it.hasNext()){//遍历JSONObject  
	                	key = (String) it.next().toString();  
	                	if(null!=key && !"".equals(key)){
	                		try{
		                		String value = referringParams.getString(key);
		                		Native.nativeSetBranchValue(key, value);
	                		}catch(Exception e){
	                			e.printStackTrace();
	                		}
	                	}
	                }  
	            } else {
	                Log.d("MyApp", error.getMessage());
	            }
	        }
	    };
	    branch.initSession(listener, this.getIntent().getData(), this);
	}

	@Override
	public void onStop() {
	    super.onStop();
//	    try{
//	    	TestinAgent.onStop(this);
//	    }catch(Exception e)
//	    {
//	    	
//	    }
	}
	
	//@Override
	public void finishGame() {
		try {
			/*
			 * 当玩家离开游戏时，即时启动一个Alarm，在之后的1天内如果玩家仍未启动游戏，则Alarm通知用户
			 */
			// 包装延时通知intent, 目标为游戏统一接收器NotificationReceiver
			Intent intent = new Intent(IF.getInstance(),
					NotificationReceiver.class);
			intent.putExtra("afk", true);
			intent.putExtra("afk_day", 1);
			PendingIntent sender = PendingIntent.getBroadcast(
					IF.getInstance(), 9999, intent, 0);
			Calendar calendar = Calendar.getInstance();
			calendar.setTimeInMillis(System.currentTimeMillis());
			int needHour;
			int hour = calendar.get(Calendar.HOUR_OF_DAY);
			
			if (hour >= 22 && hour <= 24) {
				// [22, 24] 区间
				needHour = 24 - (hour - 22);
			} else {
				// [0, 22] 区间
				needHour = (22 - hour) + 24;
			}			
			/*
			 * 离线后、在晚上22点，客户端提示玩家去抽奖
			 */
			if (needHour < 24) {
				calendar.add(Calendar.HOUR_OF_DAY, needHour);
			} else {
				calendar.add(Calendar.DAY_OF_MONTH, 1);
				calendar.add(Calendar.HOUR_OF_DAY, needHour - 24);
			}
			
			// 启动一个定时器
			AlarmManager am = (AlarmManager) IF.getInstance()
					.getSystemService(Context.ALARM_SERVICE);
			am.set(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), sender);
			this.finish();
//			CloudAnalytic.instance().onFinish(this);
		} catch (Exception e) {
			tracker.reportException(e);
		}
	}
	
	@Override
	public void onResume() {
		super.onResume();
		Adjust.onResume(this);
		//AppEventsLogger.activateApp(this);
		try {
			// 接入FaceBook广告
			String FACEBOOK_APP_ID = this.getResources().getString(
					R.string.fb_app_id);
//			com.facebook.Settings.publishInstallAsync(this, FACEBOOK_APP_ID);
			if(facebookEnabled)
				AppEventsLogger.activateApp(this, FACEBOOK_APP_ID);
		} catch (Exception e) {

		}
		if(miAnalyticsEnabled)
			MiAnalyticsUtil.triggerEventAppForground();
		try {
			MobclickAgent.onResume(this);
//			CloudAnalytic.instance().onResume(this);
		} catch (Exception e) {
			tracker.reportException(e);
		}
//		HelpshiftBridge.init(this);
	}

	@Override
	protected void onPause() {
		super.onPause();
		Adjust.onPause();
		if(facebookEnabled)
			AppEventsLogger.deactivateApp(this);
		if(miAnalyticsEnabled)
			MiAnalyticsUtil.triggerEventAppBackground();
		try {
			MobclickAgent.onPause(this);
//			CloudAnalytic.instance().onPause(this);
		} catch (Exception e) {
			tracker.reportException(e);
		}
	}
	
	@Override
	protected void onDestroy(){
		super.onDestroy();
		if(profileTracker != null)
			profileTracker.stopTracking();
		NanigansEventManager.getInstance().onDestroy();
		// 关闭同app内其它task
		android.os.Process.killProcess(android.os.Process.myPid());
	}
	
	//WebView
    public void displayWebView(final int x,final int y,final int width,final int height) {
    	this.runOnUiThread(new Runnable() {
            public void run() {
            	removeWebView();
            	m_webView = new WebView(IF.getInstance());
            	m_webLayout.addView(m_webView);
            	m_webView.requestFocus();
            	m_webView.setBackgroundColor(0);
            	m_webView.getSettings().setCacheMode(WebSettings.LOAD_NO_CACHE);
            	m_webView.getSettings().setAppCacheEnabled(false);
            	m_webView.getSettings().setJavaScriptEnabled(true);
            	m_webView.setBackgroundColor(0x99ffffff);
            	
            	LinearLayout.LayoutParams linearParams = (LinearLayout.LayoutParams) m_webView.getLayoutParams();
            	linearParams.leftMargin = x;
            	linearParams.topMargin = y;
            	linearParams.width = width;
            	linearParams.height = height;
            	m_webView.setLayoutParams(linearParams);
            	m_webView.setWebViewClient(new WebViewClient(){
                    @Override
                    public boolean shouldOverrideUrlLoading(WebView view, String url){
                    	view.loadUrl(url);// //重写此方法表明点击网页里面的链接还是在当前的webview里跳转，不跳到浏览器
                        return true;
                    }
                    @Override
                    public void onPageFinished(WebView view, String url) {
                    	view.setBackgroundColor(0xffffffff);
                    }
                    @Override
                    public void onReceivedError(WebView view, int errorCode, String description, String failingUrl){
                    	Log.d("COK", "onReceivedError "+String.valueOf(errorCode));
                    	m_webView.loadData(description, "text/html", "UTF-8");
                    }
                    @Override
                    public void onReceivedSslError(WebView view, SslErrorHandler handler,SslError error){
//						handler.cancel();//默认的处理方式，WebView变成空白页
                    	Log.d("COK", "onReceivedSslError");
                    	//20150724兼容gash支付
                    	handler.proceed();//接受证书
//						handleMessage(Message msg);//其他处理， 比如弹出对话框 让用户 选择继续还是取消
                    }
                });
            	
            }
        });
    }
    
    public void updateURL(final String url) {
    	this.runOnUiThread(new Runnable() {
            public void run() {
            	if(m_webView!=null){
            		m_webView.loadUrl(url);
            	}
            }
        });
    }
    @Override
    public void onBackPressed() {
        if(m_webView != null && m_webView.isShown())
    	{
    		removeWebView();
    	}
    	else
    	{
//    		super.onBackPressed();
    	}
    }
    
    public void removeWebView() {
    	this.runOnUiThread(new Runnable() {
            public void run() {
            	if(m_webView!=null){
            		m_webLayout.removeView(m_webView);
            		m_webView.destroy();
            		m_webView = null;
            		Native.nativeRemoveWaitInterface();
            		Native.nativeAndroidPaymentFail();
            	}
            }
        });
    }
    
    protected boolean showCpb() {
		return false;
	}
    
    public IPublishChannel getPublishImpl() {
        return null;
    }
    
    public static void ensureGameLibLoad() {
    	// do nothing. Just ensure game lib so file been loaded.
    }
    
    public static String getServerCV(){
    	return IF.getInstance().getPublishImpl().getPublish_ServerCV();
    }

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		//fblogin
		if(callbackManager != null)
			callbackManager.onActivityResult(requestCode, resultCode, data);
		// photo taker
		onPhotoActivityResult(requestCode, resultCode, data);
	}
    
    private final static int PLAY_SERVICES_RESOLUTION_REQUEST = 9000;

    /**
     * Check the device to make sure it has the Google Play Services APK. If
     * it doesn't, display a dialog that allows users to download the APK from
     * the Google Play Store or enable it in the device's system settings.
     */
//    private boolean checkPlayServices() {
//        int resultCode = GooglePlayServicesUtil.isGooglePlayServicesAvailable(this);
//        if (resultCode != ConnectionResult.SUCCESS) {
////            if (GooglePlayServicesUtil.isUserRecoverableError(resultCode)) {
////                GooglePlayServicesUtil.getErrorDialog(resultCode, this,
////                        PLAY_SERVICES_RESOLUTION_REQUEST).show();
////            } else {
//                Log.i("Google Play Services", "This device is not supported.");
//////                finish();
////            }
//            return false;
//        }
//        return true;
//    }
    
  /// photo taker
  	protected void onPhotoActivityResult(int requestCode, int resultCode, Intent data) {
//  		Log.d("onPhotoActivityResult", "resultCode: "+resultCode+",requestCode:"+requestCode);
		if (resultCode == RESULT_OK) {
			switch (requestCode) {
			case Cocos2dxHandler.TAKE_PICTURE:
				// 设置文件保存路径这里放在跟目录下
				File picture = new File(Environment.getExternalStorageDirectory() + "/image.jpg");
				startPhotoZoom(Uri.fromFile(picture), Cocos2dxHandler.CLIP_TAKEN_PICTURE);
				break;
			case Cocos2dxHandler.CHOOSE_PICTURE:
				if(data != null){
					//照片的原始资源地址
					Uri originalUri = data.getData();
//					Log.d("onPhotoActivityResult", "Uri: "+originalUri);
					if(originalUri != null){
						startPhotoZoom(originalUri, Cocos2dxHandler.CLIP_CHOOSED_PICTURE);
					}
				} 
				break;
			case Cocos2dxHandler.CLIP_TAKEN_PICTURE:
			case Cocos2dxHandler.CLIP_CHOOSED_PICTURE:
				if(data != null)
				{
					saveImg(data);
				}
				break;
			default:
				break;
			}
		}
	}
  	
    private static int gameUid = -1;
	private static int index = -1;
	// 点击“拍照”按钮时调用
	public void showPicturePicker1(int uid, int idx){
		if(checkCameraHardware())
		{
			gameUid = uid;
			index = idx;
			Intent openCameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
			Uri imageUri = Uri.fromFile(new File(Environment.getExternalStorageDirectory(),"image.jpg"));
			//指定照片保存路径（SD卡），image.jpg为一个临时文件，每次拍照后这个图片都会被替换
			//把返回数据存入Intent
			openCameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
			startActivityForResult(openCameraIntent, Cocos2dxHandler.TAKE_PICTURE);	
		}
		else
		{
			showPicturePicker2(uid, idx);
		}
	}
	// 点击“相册”按钮时调用
	public void showPicturePicker2(int uid, int idx){
		gameUid = uid;
		index = idx;
		Intent openAlbumIntent;
		if(android.os.Build.VERSION.SDK_INT>=android.os.Build.VERSION_CODES.KITKAT){
//			Log.d("showPicturePicker2", "SDK_INT0:"+android.os.Build.VERSION.SDK_INT);
			openAlbumIntent = new Intent(Intent.ACTION_PICK, android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI); 
		}else{
//			Log.d("showPicturePicker2", "SDK_INT1:"+android.os.Build.VERSION.SDK_INT);
			openAlbumIntent = new Intent(Intent.ACTION_GET_CONTENT);
		} 
		openAlbumIntent.setType("image/*");
		startActivityForResult(openAlbumIntent, Cocos2dxHandler.CHOOSE_PICTURE);
	}
	public static final String IMAGE_UNSPECIFIED = "image/*";	
	// 图片的剪裁
	public void startPhotoZoom(Uri uri, int resValue) {
		System.out.println(uri);
		Intent intent = new Intent("com.android.camera.action.CROP");
		intent.setDataAndType(uri, IMAGE_UNSPECIFIED);
		intent.putExtra("crop", "true");
		// aspectX aspectY 是宽高的比例
		intent.putExtra("aspectX", 1);
		intent.putExtra("aspectY", 1);
		// outputX outputY 是裁剪图片宽高
		intent.putExtra("outputX", 90);
		intent.putExtra("outputY", 90);
		intent.putExtra("return-data", true);
		
		try 
		{
			startActivityForResult(intent, resValue);
		}
		catch(Throwable thw){
			//ActivityNotFoundException 
		}
	}
	// 保存图片
	public void saveImg(Intent data){
		Bundle extras = data.getExtras();
		if (extras != null) {
    	    Bitmap bmp = data.getParcelableExtra("data");
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			bmp.compress(Bitmap.CompressFormat.JPEG, 75, stream);//
			
			// save to local
			String absolutePath = savePhotoToSDCard(bmp, Environment.getExternalStorageDirectory().getAbsolutePath(), String.valueOf(gameUid +"_" + index));
			File dir = new File(absolutePath);
			if(dir.exists()){
				FBUtil.nativeSendHeadImgUrl(absolutePath);
			}
		}
	}
	
	/** 检测设备是否存在Camera硬件 */
	private boolean checkCameraHardware() {
		if (getApplicationContext().getPackageManager().hasSystemFeature(
			PackageManager.FEATURE_CAMERA)) {
	          // 存在
	        return true;
	    } else {
	          // 不存在
	        return false;
	    }
	}
	
	/**
	 * Save image to the SD card 
	 * @param photoBitmap
	 * @param photoName
	 * @param path
	 */
	public String savePhotoToSDCard(Bitmap photoBitmap,String path,String photoName){
		String absolutePath = "";
		if (checkSDCardAvailable()) {
			File dir = new File(path);
			if (!dir.exists()){
				dir.mkdirs();
			}
			
			File photoFile = new File(path , photoName + ".jpg");
			absolutePath = photoFile.getAbsolutePath();
			
			FileOutputStream fileOutputStream = null;
			try {
				fileOutputStream = new FileOutputStream(photoFile);
				if (photoBitmap != null) {
					if (photoBitmap.compress(Bitmap.CompressFormat.JPEG, 100, fileOutputStream)) {
						fileOutputStream.flush();
					}
				}
			} catch (FileNotFoundException e) {
				photoFile.delete();
				e.printStackTrace();
			} catch (IOException e) {
				photoFile.delete();
				e.printStackTrace();
			} finally{
				try {
					fileOutputStream.close();
				} catch (Throwable e) {
					//e.printStackTrace();
				}
			}
		} 
		
		return absolutePath;
	}
	
	/**
	 * Check the SD card 
	 * @return
	 */
	public boolean checkSDCardAvailable(){
		return android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED);
	}

	@Override
	public void trackException(String exceptionType, String funcInfo, String cause, String message)
	{
		FBUtil.appEventException(exceptionType, funcInfo, cause, message);
	}
	
	public void saveCrashPID(){
		doSaveCrashPID();
	}
	
	public void doSaveCrashPID(){
		
	}

//	@Override
//	public String getParseFromCocos2dx(String jsonStr)
//	{
//		return Net.getParseFromCocos2dx(jsonStr);
//	}
	
	public static void savePhoneDevice(){
        final boolean result = false;
        try {
            String cmd = "getprop";
            System.out.println("start adb");
            Process process = Runtime.getRuntime().exec(cmd); // adb shell
            final BufferedReader inputStream = new BufferedReader(
                                                                  new InputStreamReader(process.getInputStream()));
            new Thread(new Runnable() {
                String line;
                public void run() {
                    try {
                    	String kernelVersion = System.getProperty("os.version");
           				System.out.println("kernelVersion = " + kernelVersion);
           				Native.nativeSetConfig("phoneDevice", kernelVersion);
                        /*while ((line = inputStream.readLine()) != null) {
                            System.out.println("line = " + line);
                            if(line.contains("ro.product.device")){
                            	String[] strs = line.split(":");
                            	if(strs.length == 2){
                            		Native.nativeSetConfig("phoneDevice", strs[1]);
                            	}
                            	break;
                            }
                        }*/
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }).start();
            
        } catch (Exception e) {
            e.printStackTrace();
        }
	}


	@Override
	public void transportMail(String jsonStr,boolean isShowDetectMail)
	{
		System.out.println("IF transportMail isShowDetectMail:"+isShowDetectMail);
		Net.transportMailInfo(jsonStr,isShowDetectMail);
	}
	
	@Override
	public <T> T excuteJNIMethod(String methodName, Object[] params)
	{
		return ChatServiceJniHandle.excuteJNIMethod(methodName, params);
	}
	
	@Override
	public void excuteJNIVoidMethod(String methodName, Object[] params)
	{
		ChatServiceJniHandle.excuteJNIVoidMethod(methodName, params);
	}

	@Override
	public String getPublishChannelName() {
		// TODO Auto-generated method stub
//		return IF.getInstance().getPublishImpl().getPublishChannel()
		return "debug" ;
	}

	@Override
	public void trackMessage(String messageType, String... args) {
		// TODO Auto-generated method stub
		FBUtil.appEventMessage(messageType, args);
	}
	
	public static void initDatabase(boolean isAccountChanged, boolean isNewUser)
    {
		DBManager.initDatabase(isAccountChanged, isNewUser);
    }
	
	@Override
	public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults)
	{
		DBManager.onRequestPermissionsResult(requestCode, permissions, grantResults);
	}

}
