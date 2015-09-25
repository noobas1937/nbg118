package com.clash.of.weibo;

import android.app.Activity;
import android.content.Context;

import com.clash.of.weibo.WBListener.AuthListener;
import com.sina.weibo.sdk.auth.AuthInfo;
import com.sina.weibo.sdk.auth.sso.SsoHandler;
import com.sina.weibo.sdk.utils.LogUtil;

public class WBUtil {
	
	public static Context mContext;
	public static AuthInfo mAuthInfo;
	public static SsoHandler mSsoHandler;
	public static AuthListener mAuthListener;
	private static String TAG = "COK_WBUtil";

	
	public static void init(Context context){
		mContext = context;
        LogUtil.sIsLogEnable = true;
	}
	
	public static void login(){
        mAuthInfo = new AuthInfo(mContext, Constants.APP_KEY, Constants.REDIRECT_URL, Constants.SCOPE);
        mAuthListener = new AuthListener();
		mSsoHandler = new SsoHandler((Activity)mContext, mAuthInfo);
        mSsoHandler.authorize(mAuthListener);
	}
	
	public static void logout(){
		AccessTokenKeeper.clear(mContext);
//      new LogoutAPI(context, Constants.APP_KEY, AccessTokenKeeper.readAccessToken(context)).logout(WBListener.mLogoutListener);
	}
	
	

}
