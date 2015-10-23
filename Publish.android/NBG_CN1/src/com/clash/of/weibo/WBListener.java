package com.clash.of.weibo;

import org.cocos2dx.ext.Native;
import org.nbg.IF.IF;
import org.json.JSONException;
import org.json.JSONObject;

import android.os.Bundle;
import android.text.TextUtils;
import android.widget.Toast;

import com.helpshift.Log;
import com.sina.weibo.sdk.auth.Oauth2AccessToken;
import com.sina.weibo.sdk.auth.WeiboAuthListener;
import com.sina.weibo.sdk.constant.WBConstants;
import com.sina.weibo.sdk.exception.WeiboException;
import com.sina.weibo.sdk.net.RequestListener;

public class WBListener {
	
	private static String TAG = "COK_WBListener";
	
    /** 登陆认证对应的listener */
    public static class AuthListener implements WeiboAuthListener {
        
        @Override
        public void onComplete(Bundle values) {
            Log.d(TAG, "AuthListener onComplete");
            // 从 Bundle 中解析 Token
        	Oauth2AccessToken mAccessToken = Oauth2AccessToken.parseAccessToken(values);
            //从这里获取用户输入的 电话号码信息 
            String  phoneNum =  mAccessToken.getPhoneNum();
            if (mAccessToken.isSessionValid()) {
                Log.d(TAG, "AuthListener "+mAccessToken.getUid());
                AccessTokenKeeper.writeAccessToken(WBUtil.mContext, mAccessToken);
    			try {
    				JSONObject obj = new JSONObject();
    				obj.put("msgId", "login_sucess_weibo");
					obj.put("userName", values.getString("userName"));
					obj.put("platform", "weibo");
					obj.put("userId", mAccessToken.getUid());
        			Native.postNotification("onResponsed3rdPlatform", obj.toString());
    			} catch (JSONException e) {
    				e.printStackTrace();
    			}
            } else {
                // 以下几种情况，您会收到 Code：
                // 1. 当您未在平台上注册的应用程序的包名与签名时；
                // 2. 当您注册的应用程序包名与签名不正确时；
                // 3. 当您在平台上注册的包名和签名与您当前测试的应用的包名和签名不匹配时。
                String code = values.getString("code");
                Toast.makeText(WBUtil.mContext, code, Toast.LENGTH_LONG).show();
            }
        }

        @Override
        public void onCancel() {
            Log.d(TAG, "AuthListener onCancel");
        }

        @Override
        public void onWeiboException(WeiboException e) {
            Toast.makeText(WBUtil.mContext, 
                    "Auth exception : " + e.getMessage(), Toast.LENGTH_LONG).show();
        }
    }
    /** 登出操作对应的listener */
    public static RequestListener mLogoutListener = new RequestListener() {
        @Override
        public void onComplete(String response) {
            if (!TextUtils.isEmpty(response)) {
                try {
                    JSONObject obj = new JSONObject(response);
                    String value = obj.getString("result");
                    if ("true".equalsIgnoreCase(value)) {
                        AccessTokenKeeper.clear(IF.getInstance());
                    }
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
        }     

        @Override
        public void onWeiboException(WeiboException e) {
        }
    };

}
