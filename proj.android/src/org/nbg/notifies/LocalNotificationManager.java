/*************************************************************************
 *
 * ADOBE CONFIDENTIAL
 * ___________________
 *
 *  Copyright 2011 Adobe Systems Incorporated
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Adobe Systems Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Adobe Systems Incorporated and its
 * suppliers and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Adobe Systems Incorporated.
 **************************************************************************/

package org.nbg.notifies;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.lang.reflect.Field;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.commons.lang.StringUtils;
import org.cocos2dx.ext.Device;
import org.cocos2dx.ext.Native;
import org.nbg.IF.R;
import org.nbg.IF.IF;
import org.nbg.stac.empire.common.manager.NotificationReceiver;
import org.nbg.util.GameContext;
import com.elex.chatservice.controller.ChatServiceController;
import com.facebook.appevents.AppEventsConstants;
import com.facebook.appevents.AppEventsLogger;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.ActivityManager.RunningTaskInfo;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.app.NotificationCompat;
import android.text.TextUtils;
import android.util.Log;

public class LocalNotificationManager {
	final static String TAG = "HFLocalNotification";
	final static String CURRENT_NOTIFICATION_CONTENT = "HFLocalNotificationContent";
	final static String CURRENT_NOTIFICATION_CONTENT_KEY = "content";
	public final static String CACHE_NOTIF_RECORD_KEY = "COK_PUSH_RECORD";
	public final static String CACHE_GAME_UID_KEY = "COK_GAMEUID";
	
	public final static String NOTIFICATION_CONTENT_SEPERATOR = "\n";
	
	Context androidActivity;
	Context androidContext;
	NotificationManager notificationManager;
	// the max lines show in inbox style notification
	private int mMaxNotificationShow = 5;

	public static boolean wasNotificationSelected = false;
	public static String selectedNotificationCode = "";
	public static byte[] selectedNotificationData = {};

	/**
	 * Represents a second in milliseconds
	 */
	private final static long SECOND_CALENDAR_UNIT_MS = 1000;
	/**
	 * Represents a minute in milliseconds
	 */
	private final static long MINUTE_CALENDAR_UNIT_MS = SECOND_CALENDAR_UNIT_MS * 60;
	/**
	 * Represents a week in milliseconds
	 */
	private final static long HOUR_CALENDAR_UNIT_MS = MINUTE_CALENDAR_UNIT_MS * 60;
	/**
	 * Represents a day in milliseconds
	 */
	private final static long DAY_CALENDAR_UNIT_MS = HOUR_CALENDAR_UNIT_MS * 24;
	/**
	 * Represents a month in milliseconds
	 * Not exact
	 */
	private final static long MONTH_CALENDAR_UNIT_MS = DAY_CALENDAR_UNIT_MS * 30;
	/**
	 * Represents a year in milliseconds
	 * Not exact
	 */
	private final static long YEAR_CALENDAR_UNIT_MS = DAY_CALENDAR_UNIT_MS * 365;
	/**
	 * Represents a year in milliseconds
	 * Not supported
	 */
	private final static long ERA_CALENDAR_UNIT_MS = YEAR_CALENDAR_UNIT_MS;
	/**
	 * Represents an hour in milliseconds
	 */
	private final static long WEEK_CALENDAR_UNIT_MS = DAY_CALENDAR_UNIT_MS * 7;
	/**
	 * Represents a week day (e.g. each Monday) in milliseconds
	 */
	private final static long WEEKDAY_CALENDAR_UNIT_MS = DAY_CALENDAR_UNIT_MS * 7;
	/**
	 * Represents a week day ordinal in milliseconds
	 * Not supported
	 */
	private final static long WEEKDAY_ORDINAL_CALENDAR_UNIT_MS = MONTH_CALENDAR_UNIT_MS;
	/**
	 * Represents a quarter year in milliseconds
	 * Not exact
	 */
	private final static long QUARTER_CALENDAR_UNIT_MS = YEAR_CALENDAR_UNIT_MS / 4;

	/**
	 * Represents an era repeat interval
	 */
	public final static int ERA_CALENDAR_UNIT = 1 << 1;
	/**
	 * Represents a year repeat interval
	 */
	public final static int YEAR_CALENDAR_UNIT = 1 << 2;
	/**
	 * Represents a month repeat interval
	 */
	public final static int MONTH_CALENDAR_UNIT = 1 << 3;
	/**
	 * Represents a day repeat interval
	 */
	public final static int DAY_CALENDAR_UNIT = 1 << 4;
	/**
	 * Represents an hour repeat interval
	 */
	public final static int HOUR_CALENDAR_UNIT = 1 << 5;
	/**
	 * Represents a minute repeat interval
	 */
	public final static int MINUTE_CALENDAR_UNIT = 1 << 6;
	/**
	 * Represents a second repeat interval
	 */
	public final static int SECOND_CALENDAR_UNIT = 1 << 7;
	/**
	 * Represents a week repeat interval
	 */
	public final static int WEEK_CALENDAR_UNIT = 1 << 8;
	/**
	 * Represents a week day (e.g. each Monday) repeat interval
	 */
	public final static int WEEKDAY_CALENDAR_UNIT = 1 << 9;
	/**
	 * Represents a week day ordinal repeat interval
	 */
	public final static int WEEKDAY_ORDINAL_CALENDAR_UNIT = 1 << 10;
	/**
	 * Represents a quarter year repeat interval
	 */
	public final static int QUARTER_CALENDAR_UNIT = 1 << 11;

	final static int STANDARD_NOTIFICATION_ID = 0;

	final static String NOTIFICATION_UNIQUE_KEY = "com.clash.of.notify";
	
	public final static String MAIN_ACTIVITY_CLASS_NAME = "com.clash.of.kings.EmpireActivity";
	public final static String MAIN_ACTIVITY_CLASS_NAME_KEY = "com.clash.of.mainActivityClassNameKey";

	public final static String NOTIFICATION_CODE_KEY = "com.clash.of.notificationCodeKey";
	public final static String ACTION_DATA_KEY = "com.clash.of.actionDataKey";

	public static final String ICON_RESOURCE = "NOTIF_ICON_RESOURCE";
	public static final String TITLE = "NOTIF_TITLE";
	public static final String BODY = "NOTIF_BODY";
	public static final String NUMBER_ANNOTATION = "NOTIF_NUM_ANNOT";
	public static final String TICKER_TEXT = "NOTIF_TICKER";
	public static final String PLAY_SOUND = "NOTIF_PLAY_SOUND";
	public static final String SOUND_NAME = "NOTIF_SOUND_NAME";
	public static final String VIBRATE = "NOTIF_VIBRATE";
	public static final String CANCEL_ON_SELECT = "NOTIF_CANCEL_OS";
	public static final String REPEAT_UNTIL_ACKNOWLEDGE = "NOTIF_RUA";
	public static final String ON_GOING = "NOTIF_ONGOING";
	public static final String ALERT_POLICY = "NOTIF_POLICY";
	public static final String HAS_ACTION = "NOTIF_HAS_ACTION";
	public static final String SOUND_KEY = "NOTIF_SOUND_KEY";
	public static final String PUSH_MSG = "cok_push";
	public static final String PUSH_TYPE = "cok_push_type";
	public static final String PUSH_TIME = "cok_push_time";
	public static final String PUSH_RECORD = "cok_push_record";
	public static final String PUSH_CLICK_DATA = "cok_push_click_data";

	private void setupSound(Context context,Intent intent, NotificationCompat.Builder builder){
//		Uri path = Uri.parse("android.resource://" + context.getPackageName() + R.raw.parsesound);// + R.raw.sheepbaa
		AudioManager manager = (AudioManager)context.getSystemService(Context.AUDIO_SERVICE);
		final Bundle bundle = intent.getExtras();
		String soundKey = bundle.getString(SOUND_KEY);
//		boolean soundEnable = "".equals(Native.nativeGetConfig(soundKey));
//		boolean playSound = bundle.getBoolean(PLAY_SOUND);
//		if(manager.getStreamVolume(AudioManager.STREAM_NOTIFICATION) > 0){
//			Log.d(TAG, "volume of IF");
//			int soundVolume = AudioManager.STREAM_NOTIFICATION;
//			if (!playSound) 
//			{
//				Log.d(TAG, "volume of close sound");
//				path = null;
//				soundVolume = 0;
//			}
//			builder.setSound(path, soundVolume);
//		}else{
		boolean soundEnable="1".equals(soundKey) || "".equals(soundKey);
		Log.d(TAG, "volume soundKey " + soundKey);
		if(soundEnable){
			Map<String, Integer> soundMap = new HashMap<String, Integer>(){
				{
//					put("0",R.raw.parsesound);//未知
//					put("1",R.raw.parsesound);//1天金币
//					put("2",R.raw.parsesound);//3天金币
//					put("3",R.raw.parsesound);//7天金币
//					put("4",R.raw.parsesound);//24小时不上线推送
					put("5",R.raw.push_building);//建筑升级完成
//					put("6",R.raw.parsesound);//部队训练完成
//					put("7",R.raw.parsesound);//陷阱建造完成
//					put("8",R.raw.parsesound);//科技研究完成
//					put("9",R.raw.push_under_attack);//被侦查
//					put("10",R.raw.push_under_attack);//被攻击
//					put("11",R.raw.parsesound);//部队回城
//					put("12",R.raw.push_mail);//个人邮件
//					put("13",R.raw.push_mail);//联盟群体邮件
//					put("14",R.raw.push_resouce_full);//资源满
//					put("15",R.raw.parsesound);//联盟礼物
//					put("16",R.raw.push_harbor);//港口物资
//					put("17",R.raw.parsesound);//组队战
//					put("18",R.raw.push_mail);//全服邮件推送
//					put("19",R.raw.push_chat);//联盟聊天
//					put("20",R.raw.parsesound);//GM推送
				}
			};
			if(bundle.containsKey(PUSH_TYPE) 
					&& soundMap.containsKey(bundle.getString(PUSH_TYPE))){
				Log.d(TAG, "volume sound type "+bundle.getString(PUSH_TYPE));
				builder.setSound(Uri.parse("android.resource://"+ context.getPackageName() + "/" + soundMap.get(bundle.getString(PUSH_TYPE))));
			}else{
				Log.d(TAG, "volume default");
				builder.setDefaults(Notification.DEFAULT_ALL);
			}
		}else{
			Log.d(TAG, "volume no sound");
			builder.setSound(null, 0);
		}
		Log.d(TAG, "volume of STREAM_ALARM:" + manager.getStreamVolume(AudioManager.STREAM_ALARM));
		Log.d(TAG, "volume of STREAM_MUSIC:" + manager.getStreamVolume(AudioManager.STREAM_MUSIC));
		Log.d(TAG, "volume of STREAM_NOTIFICATION:" + manager.getStreamVolume(AudioManager.STREAM_NOTIFICATION));
		Log.d(TAG, "volume of STREAM_RING:" + manager.getStreamVolume(AudioManager.STREAM_RING));
		Log.d(TAG, "volume of STREAM_SYSTEM:" + manager.getStreamVolume(AudioManager.STREAM_SYSTEM));
		
	}
	private void setupMiscellaneous(Intent intent, NotificationCompat.Builder builder) {
		final Bundle bundle = intent.getExtras();
		boolean ongoing = bundle.getBoolean(ON_GOING);
		if(ongoing){
			builder.setOngoing(true);
		}
	}
	public void setPushClickData(String pushClickData){
		Log.d("COK", "COK setNotifClickData" + pushClickData);
		final SharedPreferences pushSharedPreferences = androidContext.getSharedPreferences(CACHE_NOTIF_RECORD_KEY, Context.MODE_PRIVATE);
		Editor pushRecordEditor = pushSharedPreferences.edit();
		pushRecordEditor.putString(PUSH_CLICK_DATA, pushClickData);
		pushRecordEditor.commit();
//		sendDataToXingcloud(pushClickData,"pushopen");
	}
	public void sendDataToXingcloud(final String pushData,final String action){
		new Thread(new Runnable() {
			@Override
			public void run() {
				String[] pushStr = pushData.split("\\[,\\]");
				final SharedPreferences sharedPreferences = androidContext.getSharedPreferences(LocalNotificationManager.CACHE_GAME_UID_KEY, Context.MODE_PRIVATE);
				String gameUid = sharedPreferences.getString("GAMEUID", "");
				Log.d("COK", "COK sendDataToXingcloud gameUid "+gameUid);
				if(pushStr[2] != null && !"".equals(pushStr[2]) && !"".equals(gameUid)){
					try{
						String url = "http://xa.xingcloud.com/v4/cok/"+gameUid+"?action="+action+"&type="+pushStr[2]+"&device=android";
						Log.d("COK", "COK sendDataToXingcloud "+url);
			            URL obj = new URL(url);
			            HttpURLConnection con = (HttpURLConnection) obj.openConnection();
			            con.setConnectTimeout(10000);
			            con.setReadTimeout(10000);
			            int responseCode = con.getResponseCode();
						Log.d("COK", "COK sendDataToXingcloud "+String.valueOf(responseCode));
					}catch(Exception e){
						e.printStackTrace();
					}
				}
			}
		}).start();  
	}
	private String getPushRecordString(Intent intent){
		Bundle bundle = intent.getExtras();
		String notif_type = "0";
		if(bundle.containsKey(PUSH_TYPE) && !StringUtils.isBlank(bundle.getString(PUSH_TYPE))){
			notif_type = bundle.getString(PUSH_TYPE);
		}
		String[] newRecord = new String[]{
				bundle.getString(PUSH_TIME),
				bundle.getString(BODY),
				notif_type,
		};
		return TextUtils.join("[,]", newRecord);
	}
	private void recordPush(Intent intent){
		final SharedPreferences pushSharedPreferences = androidContext.getSharedPreferences(CACHE_NOTIF_RECORD_KEY, Context.MODE_PRIVATE);
		String cacheRecord = pushSharedPreferences.getString(PUSH_RECORD, "");
		List<String> records = new ArrayList<String>();
		if(!"".equals(cacheRecord))
			records = new ArrayList<String>(Arrays.asList(cacheRecord.split("\\[;\\]")));
		records.add(getPushRecordString(intent));
		Editor pushRecordEditor = pushSharedPreferences.edit();
		pushRecordEditor.putString(PUSH_RECORD, TextUtils.join("[;]", records));
		pushRecordEditor.commit();
		sendDataToXingcloud(getPushRecordString(intent),"pushreceive");
		Log.d("COK", "COK recordPush "+pushSharedPreferences.getString(PUSH_RECORD, ""));
	}
	public void fireNotificationNew(Context context, Intent intent){
		boolean isAppInForeground = isAppInForeground(context);
		if (isAppInForeground){
			Log.d("COK", "COK fireNotificationNew return due to App is running in foreground");
			return;
		} else {
			Log.d("COK", "COK fireNotificationNew App is not running");
		}
		Bundle bundle = intent.getExtras();
		boolean hasAction = bundle.getBoolean(HAS_ACTION);
		Log.d("LocalNotificationManager::fireNotificationNew", "Activity Class Name: " + hasAction);
		
		int iconResourceId = bundle.getInt(ICON_RESOURCE);
		int numberAnnotation = bundle.getInt(NUMBER_ANNOTATION);
		String tickerText = bundle.getString(TICKER_TEXT);
		String title = bundle.getString(TITLE);
		String body = bundle.getString(BODY);
		String code = NOTIFICATION_UNIQUE_KEY;
		String contentText = addCurrentNotificationContent(body);
		NotificationCompat.InboxStyle inboxStyle = new NotificationCompat.InboxStyle();
		String[] textLines = contentText.split(NOTIFICATION_CONTENT_SEPERATOR);
		for(String line: textLines){
//			Log.d("LocalNotifyReceiver","local notify add line to inboxstyle:" + line);
			if(line.trim().length() > 0){
				inboxStyle.addLine(line);
			}
		}
	
		inboxStyle.setBigContentTitle(title);
		
		NotificationCompat.Builder mBuilder = new NotificationCompat.Builder(context);
		
		mBuilder.setSmallIcon(iconResourceId)
		.setContentTitle(title)
		.setContentText(body)
		.setDefaults(Notification.DEFAULT_LIGHTS | Notification.DEFAULT_VIBRATE)
		.setAutoCancel(true)
		.setTicker(tickerText)
		.setPriority(Notification.PRIORITY_HIGH)
		;
		
		mBuilder.setNumber(textLines.length);
		
		setupSound(context,intent,mBuilder);
		setupMiscellaneous(intent,mBuilder);
		
		final Intent notificationIntent = new Intent();

		if (hasAction) {
			String activityClassName = bundle.getString(MAIN_ACTIVITY_CLASS_NAME_KEY);
			byte actionData[] = bundle.getByteArray(ACTION_DATA_KEY);

			notificationIntent.setClassName(context, "org.nbg.notifies.LocalNotificationIntentService");
			notificationIntent.putExtra(MAIN_ACTIVITY_CLASS_NAME_KEY, activityClassName);

			Log.d("LocalBroadcastReceiver::onReceive", "Activity Class Name: " + activityClassName);

			// Add the notification code of the notification to the intent so we can retrieve it later if the notification is selected by a user.
			notificationIntent.putExtra(NOTIFICATION_CODE_KEY, code);

			// Add the action data of the notification to the intent as well.
			notificationIntent.putExtra(ACTION_DATA_KEY, actionData);
			
			//set push intent so we can know which notification player pressed
			notificationIntent.putExtra(PUSH_CLICK_DATA, getPushRecordString(intent));
		}

		notificationIntent.setAction(Intent.ACTION_MAIN);

		notificationIntent.addCategory(Intent.CATEGORY_LAUNCHER);
		final PendingIntent pendingIntent = PendingIntent.getService(context, code.hashCode(), notificationIntent, PendingIntent.FLAG_UPDATE_CURRENT);
		
		mBuilder.setContentIntent(pendingIntent);
		mBuilder.setStyle(inboxStyle);
		// Fire the notification.
		final NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
		notificationManager.cancelAll();
		notificationManager.notify(code.hashCode(), mBuilder.build());
		//record for analytics
		recordPush(intent);
	}
	
	public void fireNotification(Context context, Intent intent){
		Bundle bundle = intent.getExtras();
		boolean hasAction = bundle.getBoolean(HAS_ACTION);

		int iconResourceId = bundle.getInt(ICON_RESOURCE);
		int numberAnnotation = bundle.getInt(NUMBER_ANNOTATION);
		String tickerText = bundle.getString(TICKER_TEXT);
		String title = bundle.getString(TITLE);
		String body = bundle.getString(BODY);
		String code = bundle.getString(NOTIFICATION_CODE_KEY);
		
		// Create the notification, passing in the icon, ticker text and time.
		Notification notification = new Notification(iconResourceId, tickerText, System.currentTimeMillis());
		notification.flags |= Notification.FLAG_AUTO_CANCEL;

		// Set up the various categories of properties of the notification.
		setupSound(context, intent, notification);
		setupVibrate(intent, notification);
		setupLight(notification);
		setupMiscellaneous(intent, notification);

		notification.number = numberAnnotation;

		final Intent notificationIntent = new Intent();

		if (hasAction) {
			String activityClassName = bundle.getString(MAIN_ACTIVITY_CLASS_NAME_KEY);
			byte actionData[] = bundle.getByteArray(ACTION_DATA_KEY);

			notificationIntent.setClassName(context, "org.nbg.notifies.LocalNotificationIntentService");
			notificationIntent.putExtra(MAIN_ACTIVITY_CLASS_NAME_KEY, activityClassName);

			Log.d("LocalBroadcastReceiver::onReceive", "Activity Class Name: " + activityClassName);

			// Add the notification code of the notification to the intent so we can retrieve it later if the notification is selected by a user.
			notificationIntent.putExtra(NOTIFICATION_CODE_KEY, code);

			// Add the action data of the notification to the intent as well.
			notificationIntent.putExtra(ACTION_DATA_KEY, actionData);
		}

//		final PendingIntent pendingIntent = PendingIntent.getService(context, code.hashCode(), notificationIntent, PendingIntent.FLAG_CANCEL_CURRENT);
//		notification.setLatestEventInfo(context, title, body, pendingIntent);

		// Fire the notification.
		final NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
		notificationManager.notify(code, STANDARD_NOTIFICATION_ID, notification);
	}

	private void setupSound(Context context, Intent intent, Notification notification) {
        Uri path = Uri.parse("android.resource://" + context.getPackageName()  + R.raw.parsesound);// + R.raw.sheepbaa
        notification.sound = path;
        // notification.defaults |= Notification.DEFAULT_SOUND;

        
//		final Bundle bundle = intent.getExtras();
//		boolean playSound = bundle.getBoolean(PLAY_SOUND);
//		String soundName = bundle.getString(SOUND_NAME);
//		soundName = "PigCall.mp3";
//		if (playSound) {
//			if(soundName != null && soundName.length() > 0)
//			{
//				int pointIndex = soundName.indexOf('.');
//				if(pointIndex != -1)
//				{
//					soundName = soundName.substring(0, pointIndex);
//				}
//				
//				Uri path = Uri.parse("android.resource://" + context.getPackageName() + "/raw/" + soundName);
//				notification.sound = path;
//			}
//			else
//			{
//				notification.defaults = Notification.DEFAULT_SOUND;
//			}
//		} else {
//			notification.sound = null;
//		}
	}

	private void setupVibrate(Intent intent, Notification notification) {
		final Bundle bundle = intent.getExtras();
		boolean vibrate = bundle.getBoolean(VIBRATE);

		if (vibrate) {
			notification.defaults |= Notification.DEFAULT_VIBRATE;
		} else {
			long[] emptyVibrationPattern = { 0 };
			notification.vibrate = emptyVibrationPattern;
		}
	}

	private void setupLight(Notification notification) {
		notification.defaults |= Notification.DEFAULT_LIGHTS;
	}

	private void setupMiscellaneous(Intent intent, Notification notification) {
		final Bundle bundle = intent.getExtras();
		boolean cancelOnSelect = bundle.getBoolean(CANCEL_ON_SELECT);
		boolean repeatAlertUntilAcknowledged = bundle.getBoolean(REPEAT_UNTIL_ACKNOWLEDGE);
		boolean ongoing = bundle.getBoolean(ON_GOING);
		String alertPolicy = bundle.getString(ALERT_POLICY);

		if (cancelOnSelect) {
			notification.flags |= Notification.FLAG_AUTO_CANCEL;
		}
		if (repeatAlertUntilAcknowledged) {
			notification.flags |= Notification.FLAG_INSISTENT;
		}
		// IMPORTANT: The alertPolicy string values map directly to the constants in the NotificationAlertPolicy class in the ActionScript interface.
		if (alertPolicy.compareTo("firstNotification") == 0) {
			notification.flags |= Notification.FLAG_ONLY_ALERT_ONCE;
		}
		if (ongoing) {
			notification.flags |= Notification.FLAG_ONGOING_EVENT;
		}
	}

	public static boolean isAppInForeground(Context context) 
	{
		if(GameContext.getGameInstance() != null){
			if(!GameContext.getGameInstance().appRunning)
				return ChatServiceController.isAppInForeGround();
			else
				return true;
		}
		return false;
//	    ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
//		if(Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP){    
//		    List<RunningTaskInfo> tasks = am.getRunningTasks(1);
//		    if (!tasks.isEmpty()) {
//		        ComponentName topActivity = tasks.get(0).topActivity;
//		        if (topActivity.getPackageName().equals(context.getPackageName())) {
//		            return true;
//		        }
//		    }
//		}else{
//			final int PROCESS_STATE_TOP = 2;
//			List<RunningAppProcessInfo> appList = am.getRunningAppProcesses();
//			try {
//				Field field = RunningAppProcessInfo.class.getDeclaredField("processState");
//			    for (RunningAppProcessInfo app : appList) {
//			        if (app.importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND &&
//			            app.importanceReasonCode == 0 ) {
//						try {
//							Integer state = field.getInt( app );
//				            if (state != null && state == PROCESS_STATE_TOP && app.processName.equals(context.getPackageName())) {
//				            	return true;
//				            }
//						} catch (Exception e) {
//							e.printStackTrace();
//						}
//			        }
//			    }
//			} catch (Exception e) {
//				e.printStackTrace();
//			}
//		}
	}
	
	
	static public long mapIntervalToMilliseconds(int interval) {
		switch (interval) {
		case ERA_CALENDAR_UNIT:
			return ERA_CALENDAR_UNIT_MS;
		case YEAR_CALENDAR_UNIT:
			return YEAR_CALENDAR_UNIT_MS;
		case MONTH_CALENDAR_UNIT:
			return MONTH_CALENDAR_UNIT_MS;
		case DAY_CALENDAR_UNIT:
			return DAY_CALENDAR_UNIT_MS;
		case HOUR_CALENDAR_UNIT:
			return HOUR_CALENDAR_UNIT_MS;
		case MINUTE_CALENDAR_UNIT:
			return MINUTE_CALENDAR_UNIT_MS;
		case SECOND_CALENDAR_UNIT:
			return SECOND_CALENDAR_UNIT_MS;
		case WEEK_CALENDAR_UNIT:
			return WEEK_CALENDAR_UNIT_MS;
		case WEEKDAY_CALENDAR_UNIT:
			return WEEKDAY_CALENDAR_UNIT_MS;
		case WEEKDAY_ORDINAL_CALENDAR_UNIT:
			return WEEKDAY_ORDINAL_CALENDAR_UNIT_MS;
		case QUARTER_CALENDAR_UNIT:
			return QUARTER_CALENDAR_UNIT_MS;
		default:
			return 0;
		}
	}

	public LocalNotificationManager(Context activity) {
		androidActivity = activity;

		androidContext = activity.getApplicationContext();

		notificationManager = (NotificationManager) androidContext.getSystemService(android.content.Context.NOTIFICATION_SERVICE);

		Log.d("LocalNotificationManager::initialize", "Called with activity: " + activity.toString());
	}

	public void notify(LocalNotification localNotification) {
		// Time.
		long notificationTime = localNotification.fireDate.getTime();

		final Intent intent = new Intent(IF.getInstance(), NotificationReceiver.class);

//		intent.setAction(localNotification.code);
		intent.putExtra(TITLE, localNotification.title);
		intent.putExtra(BODY, localNotification.body);
		intent.putExtra(PUSH_TYPE, localNotification.pushType);
		intent.putExtra(PUSH_TIME, String.valueOf(System.currentTimeMillis()));
		intent.putExtra(TICKER_TEXT, localNotification.tickerText);
		intent.putExtra(NOTIFICATION_CODE_KEY, localNotification.code);
		intent.putExtra(ICON_RESOURCE, localNotification.iconResourceId);
		intent.putExtra(NUMBER_ANNOTATION, localNotification.numberAnnotation);
		intent.putExtra(PLAY_SOUND, localNotification.playSound);
		intent.putExtra(SOUND_KEY, localNotification.soundKey);
		intent.putExtra(SOUND_NAME, localNotification.soundName);
		intent.putExtra(VIBRATE, localNotification.vibrate);
		intent.putExtra(CANCEL_ON_SELECT, localNotification.cancelOnSelect);
		intent.putExtra(REPEAT_UNTIL_ACKNOWLEDGE, localNotification.repeatAlertUntilAcknowledged);
		intent.putExtra(ON_GOING, localNotification.ongoing);
		intent.putExtra(ALERT_POLICY, localNotification.alertPolicy);
		intent.putExtra(HAS_ACTION, localNotification.hasAction);
		intent.putExtra(ACTION_DATA_KEY, localNotification.actionData);

		Log.d("LocalNotificationManager", "when:" + String.valueOf(notificationTime) + ", current:" + System.currentTimeMillis());

		// If a notification is specified to have an action, set up the intent to launch the app when the notification is selected by a user.
		if (localNotification.hasAction) {
			intent.putExtra(MAIN_ACTIVITY_CLASS_NAME_KEY, localNotification.activityClassName);
			Log.d("LocalNotificationManager::notify", "Activity Class Name: " + localNotification.activityClassName);
		}

		final PendingIntent pendingIntent = PendingIntent.getBroadcast(IF.getInstance(), localNotification.code.hashCode(), intent, 0);
//		final AlarmManager am = getAlarmManager();
		AlarmManager am = (AlarmManager)IF.getInstance().getSystemService(Context.ALARM_SERVICE);
		long repeatInterval = mapIntervalToMilliseconds(localNotification.repeatInterval);

//		if (repeatInterval != 0) {
//			am.setRepeating(AlarmManager.RTC_WAKEUP, notificationTime, repeatInterval, pendingIntent);
//		} else {
			am.set(AlarmManager.RTC_WAKEUP, notificationTime, pendingIntent);
//		}

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
		
		Log.d("LocalNotificationManager::notify", "Called with notification code: " + localNotification.code);
	}

	public void cancel(String notificationCode) {
		/*
		 * Create an intent that looks similar, to the one that was registered
		 * using add. Making sure the notification id in the action is the same.
		 * Now we can search for such an intent using the 'getService' method
		 * and cancel it.
		 */
		final Intent intent = new Intent(androidContext, LocalBroadcastReceiver.class);
		intent.setAction(notificationCode);

		final PendingIntent pi = PendingIntent.getBroadcast(androidContext, notificationCode.hashCode(), intent, PendingIntent.FLAG_CANCEL_CURRENT);
		final AlarmManager am = getAlarmManager();

		try {
			am.cancel(pi);
			Log.d("LocalNotificationManager::cancel", "Called with notification code: " + notificationCode);
		} catch (Exception e) {
			Log.d("LocalNotificationManager::cancel", "Exception: " + e.getMessage());
		}

		notificationManager.cancel(notificationCode, STANDARD_NOTIFICATION_ID);
	}

	private AlarmManager getAlarmManager() {
		final AlarmManager am = (AlarmManager) androidContext.getSystemService(Context.ALARM_SERVICE);
		return am;
	}

	public void cancelAll() {
		final SharedPreferences alarmSettings = androidContext.getSharedPreferences(TAG, Context.MODE_PRIVATE);

		final Map<String, ?> allAlarms = alarmSettings.getAll();
		final Set<String> alarmIds = allAlarms.keySet();

		for (String alarmId : alarmIds) {
			Log.d(TAG, "Canceling notification with id: " + alarmId);
			String alarmCode = alarmId;
			cancel(alarmCode);
		}

		notificationManager.cancelAll();

		Log.d("LocalNotificationManager::cancelAll", "Called");
	}
	
	public int getMaxNotificationShow() {
		return mMaxNotificationShow;
	}

	public void setMaxNotificationShow(int maxNotificationShow) {
		if(maxNotificationShow > 0){
			this.mMaxNotificationShow = maxNotificationShow;
		}
	}
	
	public String addCurrentNotificationContent(String content){
		Log.d(TAG, "local notify add content: "+ content);
		final SharedPreferences contentSettings = getNotificationContentSP();
		String currentContent = contentSettings.getString(CURRENT_NOTIFICATION_CONTENT_KEY, "");
		Log.d(TAG, "local notify get current content: "+ currentContent);
		StringBuilder sb = new StringBuilder(content);
		int maxOldLines = mMaxNotificationShow - 1;
		if(maxOldLines > 4){
			// inbox style only support max 5 lines
			maxOldLines = 4;
		}
		if(currentContent != null && currentContent.length() > 0){
			String[] contentArr = currentContent.split(NOTIFICATION_CONTENT_SEPERATOR);
			for(int i =0; i < contentArr.length; ++i){
				if(i >= maxOldLines){
					break;
				}
				String line = contentArr[i];
				if(content.equals(line)){
					continue;
				}
				sb.append(NOTIFICATION_CONTENT_SEPERATOR).append(line);
			}
		}
		Editor editor = contentSettings.edit();
		String newContent = sb.toString();
		editor.putString(CURRENT_NOTIFICATION_CONTENT_KEY, newContent);
		editor.commit();
		Log.d(TAG, "local notify new content: "+ newContent);
		return newContent;
	}
	
	public String getCurrentNotificationContent(){
		final SharedPreferences contentSettings = getNotificationContentSP();
		String currentContent = contentSettings.getString(CURRENT_NOTIFICATION_CONTENT_KEY, "");
		return currentContent;
	}
	
	public void clearCurrentNotificationContent(){
		final SharedPreferences contentSettings = getNotificationContentSP();
		Editor editor = contentSettings.edit();
		editor.clear();
		editor.commit();
	}
	
	private SharedPreferences getNotificationContentSP(){
		return androidContext.getSharedPreferences(CURRENT_NOTIFICATION_CONTENT, Context.MODE_PRIVATE);
	}

	/**
	 * Persist the information of this notification to the Android Shared Preferences.
	 * This will allow the application to restore the alarm upon device reboot.
	 * Also this is used by the cancelAllNotifications method.
	 * 
	 * @see #cancelAllNotifications()
	 * 
	 * @param notification
	 *            The notification to persist.
	 */
	public void persistNotification(LocalNotification notification) {
		Log.d("LocalNotificationManager::persistNotification", "Notification: " + notification.code);
		final SharedPreferences alarmSettings = androidContext.getSharedPreferences(TAG, Context.MODE_PRIVATE);
		notification.serialize(alarmSettings);
	}

	/**
	 * Remove a specific notification from the Android shared Preferences
	 * 
	 * @param notification
	 *            The notification to persist.
	 */
	public void unpersistNotification(String notificationCode) {
		Log.d("LocalNotificationManager::unpersistNotification", "Notification: " + notificationCode);
		final Editor alarmSettingsEditor = androidContext.getSharedPreferences(TAG, Context.MODE_PRIVATE).edit();
		alarmSettingsEditor.remove(notificationCode);
		alarmSettingsEditor.commit();
	}

	/**
	 * Clear all notifications from the Android shared Preferences
	 */
	public void unpersistAllNotifications() {
		Log.d("LocalNotificationManager::unpersistAllNotifications", "Called");
		final Editor alarmSettingsEditor = androidContext.getSharedPreferences(TAG, Context.MODE_PRIVATE).edit();
		alarmSettingsEditor.clear();
		alarmSettingsEditor.commit();
	}
}
