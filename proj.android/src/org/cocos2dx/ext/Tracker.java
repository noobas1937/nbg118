package org.cocos2dx.ext;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;

//import com.google.analytics.tracking.android.EasyTracker;
import com.umeng.analytics.MobclickAgent;

/** For events & error tracking */
public class Tracker {

	public static void init(Cocos2dxActivity ctx) {
		// Get the intent that started this Activity.
		Intent intent = ctx.getIntent();
		Uri uri = intent.getData();
	
		// Call setContext() here so that we can access EasyTracker
		// to update campaign information before calling activityStart().
//		EasyTracker.getInstance().setContext(ctx);
	
		if (intent.getData() != null) {
//		  EasyTracker.getTracker().setCampaign(uri.getPath());
		}
	}
	
	public static void sendEvent(String event) {
		sendEvent(Jni.getGameActivity(), event);
	}

	public static void sendEvent(String event, String param) {
		sendEvent(Jni.getGameActivity(), event, param);
	}

	public static void sendEvent(String event, int param) {
		sendEvent(Jni.getGameActivity(), event, param);
	}

	public static void sendEvent(Context ctx, String event) {
		try {
			MobclickAgent.onEvent(ctx, event);
//			EasyTracker.getInstance().setContext(ctx);
//			EasyTracker.getTracker().sendEvent("umeng", event, "", 1L);
		} catch (Exception e2) {
			Log.e(Jni.LOG_TAG, Log.getStackTraceString(e2));
		}
		Log.d(Jni.LOG_TAG, "event:" + event);
	}

	public static void sendEvent(Context ctx, String event, String param) {
		try {
			MobclickAgent.onEvent(ctx, event, param);
//			EasyTracker.getInstance().setContext(ctx);
//			EasyTracker.getTracker().sendEvent("umeng", event, param, 1L);
		} catch (Exception e2) {
			Log.e(Jni.LOG_TAG, Log.getStackTraceString(e2));
		}
		Log.d(Jni.LOG_TAG, "event:" + event + " " + param);
	}

	public static void sendEvent(Context ctx, String event, int param) {
		try {
			MobclickAgent.onEvent(ctx, event, param);
//			EasyTracker.getInstance().setContext(ctx);
//			EasyTracker.getTracker()
//					.sendEvent("umeng", event, "", (long) param);
		} catch (Exception e2) {
			Log.e(Jni.LOG_TAG, Log.getStackTraceString(e2));
		}
		Log.d(Jni.LOG_TAG, "event:" + event + " " + param);
	}

	public void reportException(Exception e) {
		reportException(Jni.getGameActivity(), e);
	}

	public static void reportException(Context c, Exception e) {
		reportException(c, Log.getStackTraceString(e));
	}

	public static void reportException(String error) {
		reportException(Jni.getGameActivity(), error);
	}

	public static void reportException(Context ctx, String error) {
		try {
			MobclickAgent.reportError(ctx, error);
		} catch (Exception e2) {
			Log.e(Jni.LOG_TAG, Log.getStackTraceString(e2));
		}
		try {
//			EasyTracker.getInstance().setContext(ctx);
//			EasyTracker.getTracker().sendException(error, false);
		} catch (Exception e2) {
			Log.e(Jni.LOG_TAG, Log.getStackTraceString(e2));
		}
		Log.w(Jni.LOG_TAG, "exception:" + error);
	}
}
