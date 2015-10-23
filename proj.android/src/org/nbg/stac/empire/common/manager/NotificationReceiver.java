package org.nbg.stac.empire.common.manager;

import java.util.Calendar;

import org.cocos2dx.ext.Native;
import org.nbg.IF.R;
import org.nbg.IF.IF;
import org.nbg.notifies.LocalNotificationManager;
import org.nbg.util.FormulaUtil;

import com.umeng.analytics.MobclickAgent;

import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.text.TextUtils;
import android.util.Log;

public class NotificationReceiver extends BroadcastReceiver{

	@Override
	public void onReceive(Context context, Intent intent) {
		Log.d("LocalBroadcastReceiver::onReceive", "Local notification Intent: " + intent.toString());
		
		if(FormulaUtil.isAppOnForeground(context))
			return;
		
		try{
			LocalNotificationManager manager = new LocalNotificationManager(context);
			manager. fireNotificationNew(context,intent);
		}catch(Throwable e){
			e.printStackTrace();
		}
		
	}

	private void setNextAFKAlarm(Context context, int days) {
		if (days >= 30) return;
		
		// 包装延时通知intent, 目标为游戏统一接收器NotificationReceiver
		//Intent intent = new Intent();
		/*
		 * FIXME 需要使用action来初始化intent，若用context以及class名的方式，
		 * 则会与当前receiver内部intent混淆状态,造成intent信息错误
		 */
		//intent.setAction("com.hcgtech.aoe.intent.action.NOTIFICATION");
		Intent intent = new Intent(context,
				NotificationReceiver.class);
		intent.putExtra("afk", true);
		intent.putExtra("afk_day", FormulaUtil.getNextAFKDay(days));
		
		PendingIntent sender = PendingIntent.getBroadcast(
				context, 1, intent, PendingIntent.FLAG_UPDATE_CURRENT);
		Calendar calendar = Calendar.getInstance();
		calendar.setTimeInMillis(System.currentTimeMillis());
		calendar.add(Calendar.DAY_OF_MONTH, FormulaUtil.getNextAFKDay(days) - days);
		// 启动一个定时器
		AlarmManager am = (AlarmManager) context
				.getSystemService(Context.ALARM_SERVICE);
		am.set(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), sender);
	}
	
}