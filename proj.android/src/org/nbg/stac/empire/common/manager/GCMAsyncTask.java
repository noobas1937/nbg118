package org.nbg.stac.empire.common.manager;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.text.TextUtils;
import android.util.Log;
import org.cocos2dx.ext.Native;
import org.nbg.IF.IF;

import com.google.android.gcm.GCMBaseIntentService;
import com.google.android.gcm.GCMRegistrar;
import com.helpshift.HelpshiftBridge;
import com.umeng.analytics.MobclickAgent;
/**
 * 根据不同的发布版本，从友盟在线参数中获取GCM通信的SenderId,进行GCM注册
 * @author Jimin
 *
 */
public class GCMAsyncTask extends AsyncTask<Activity,Void,Void>{

	@Override
	protected Void doInBackground(Activity... params) {
		try {
			Context context = IF.getInstance();

			String senderId = GCMBaseIntentService.GCM_DEFAULT_SENDER_ID;
			GCMIntentService.safeSetSenderId(senderId);
			GCMRegistrar.checkDevice(context);
//			GCMRegistrar.checkManifest(context); //发布阶段此行代码可注释掉,否则有的机型报异常
			// 获取此设备的GCM注册ID
			final String regId = GCMRegistrar.getRegistrationId(context);
			if (TextUtils.isEmpty(regId)) {
				// 若取不到GCM注册ID，则立即发起注册请求
				Log.d("COK", "COK GCMAsyncTask waiting for Helpshift register");
//				GCMRegistrar.register(context, senderId);
			} else {
				// 设备已经注册到了GCM
				Log.d("COK", "COK GCMAsyncTask already has regId " + regId);
				HelpshiftBridge.registerDeviceToken(regId);
				Native.nativeSetGcmRegisterId(regId);
			}
		} catch (UnsupportedOperationException e) {
			Log.e("CommonActivity", e.getMessage());
		} catch (Exception e) {
			Log.e("CommonActivity", e.getMessage());
		}
		return null;
	}

	
}
