package org.cocos2dx.ext;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.pm.ApplicationInfo;
import android.util.Log;

public class Jni {
	public static final String LOG_TAG = "cocos2d-x java";

	private static Cocos2dxActivity instance;
	public static boolean isDebugMode = false;

	public static void init(Cocos2dxActivity ctx) {
		instance = ctx;

		try {
			isDebugMode = (0 != (ctx.getApplicationInfo().flags & ApplicationInfo.FLAG_DEBUGGABLE));
			Device.init(ctx);
			Tracker.init(ctx);
		} catch (Exception e) {
			Log.e(LOG_TAG, Log.getStackTraceString(e));
		}
	}

	public static Cocos2dxActivity getGameActivity() {
		return instance;
	}
}
