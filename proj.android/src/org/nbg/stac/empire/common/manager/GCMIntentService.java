package org.nbg.stac.empire.common.manager;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.text.TextUtils;
import android.util.Log;

import org.cocos2dx.ext.Native;
import com.google.android.gcm.GCMBaseIntentService;
import com.helpshift.Helpshift;
import com.helpshift.HelpshiftBridge;
import org.nbg.IF.R;
import org.nbg.util.FormulaUtil;

import com.umeng.analytics.MobclickAgent;
/**
 * GCM消息服务
 * @author Jimin
 *
 */
public class GCMIntentService extends GCMBaseIntentService{
	  /** 资源满了 */
    public final static int WAREHOUSEFULL = 0;
    /** 粮食为0 */
    public final static int CROPNULL = 1;
    /** 主城被攻击了 */
    public final static int MAINCITYBEATTACKED = 2;
    /** 野地被攻击了 */
    public final static int OASISBEATTACKED = 3;
    /** 竞技场被挑战 */
    public final static int BECHALLIANGED =4;
	private static final String TAG = "GCMIntentService";
	private static String sSenderId;
	
	public static void safeSetSenderId(String senderId) {
		if(TextUtils.isEmpty(senderId))
			sSenderId = GCM_DEFAULT_SENDER_ID;
		else
			sSenderId = senderId;
	}
	
	public static String getSenderId() {
		return sSenderId;
	}
	
	public GCMIntentService() {
		super(TextUtils.isEmpty(sSenderId) ? GCM_DEFAULT_SENDER_ID
				: sSenderId);
		Log.d(TAG, "GCMIntentService construct called");
	}
	
	@Override
	protected void onError(Context context, String regId) {
		Log.e(TAG, "onError called, regId=" + regId);
	}

	@Override
	protected void onMessage(Context context, Intent intent) {
		Log.d(TAG, "onMessage called , intent = " + intent);
		// 如果程序在运行，则直接返回
//		if(FormulaUtil.isAppOnForeground(context))
//			return;
        if(intent.getExtras().containsKey("origin") && intent.getExtras().getString("origin").equals("helpshift")) {
        	Log.d(TAG, "COK GCM received and push to helpshift");
        	Helpshift.handlePush(context, intent);
			return;
        }
		
		String message = intent.getStringExtra("gcmmessage");
//		Log.d(TAG, "onMessage called , gcmmessage = " + message);
		String gcmType = intent.getStringExtra("gcm_type");
//		Log.d(TAG, "onMessage called , gcm_type = " + gcmType);
		
		NotificationManager nm = (NotificationManager) context
				.getSystemService(Context.NOTIFICATION_SERVICE);
		
		Intent notificationIntent = new Intent();
		notificationIntent.setAction("org.nbg.stac.empire.intent.action.IF");
		notificationIntent.putExtra("notification_type", "gcm");
		notificationIntent.putExtra("value", gcmType);
		notificationIntent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
		PendingIntent contentIntent = PendingIntent.getActivity(context, 0,
				notificationIntent, 0);
		
//		Notification notification = new Notification(R.drawable.ic_launcher, message,
//				System.currentTimeMillis());
//		// 设定Notification出现时的声音，一般不建议自定义
//		notification.defaults |= Notification.DEFAULT_SOUND;
//		notification.flags |= Notification.FLAG_AUTO_CANCEL;
//		notification.setLatestEventInfo(context, context.getString(R.string.app_name), message,
//				contentIntent);
//		// 显示这个notification
//		Long currentTime = System.currentTimeMillis();
//		nm.notify(currentTime.intValue(), notification);
		
		if (!TextUtils.isEmpty(gcmType)) {
			// 初始化友盟
			MobclickAgent.onResume(context);
//			MobclickAgent.onError(context);
			MobclickAgent.updateOnlineConfig(context);// 开启在线参数配置功能
			// 行云统计
//			CloudAnalysisUitl.trackClickEvent("UI_Other","Notification","GCM", "Show","" + gcmType);
		}
	}

	@Override
	protected void onRegistered(Context context, String regId) {
		Log.d(TAG, "COK GCM onRegistered called, regId=" + regId);
		// 如果程序没在运行，则直接返回
		if(!FormulaUtil.isAppOnForeground(context)){
			Log.d(TAG, "COK GCM onRegistered return due to not isAppOnForeground");
			return;
		}
//		AppData.getInstance().setGcmRegisterId(regId);
		try {
//			Native.nativeSetGcmRegisterId(regId);
			HelpshiftBridge.registerDeviceToken(regId);
		} catch (Exception e) {
			Log.e("GCMIntentService.onRegistered", e.getMessage());
		}
	}

	@Override
	protected void onUnregistered(Context context, String regId) {
		Log.d(TAG, "onUnregistered called, regId=" + regId);
	}

	@Override
	protected boolean onRecoverableError(Context context, String errorId) {
		Log.e(TAG, "onRecoverableError called, errorId=" + errorId);
		return super.onRecoverableError(context, errorId);
	}
	
	@Override
	protected void onDeletedMessages(Context context, int total) {
		Log.e(TAG, "onDeletedMessages called, total=" + total);
		super.onDeletedMessages(context, total);
	}
}
