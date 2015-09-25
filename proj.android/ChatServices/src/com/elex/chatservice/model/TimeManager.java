package com.elex.chatservice.model;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

public class TimeManager
{
	// Date是以默认时区创建的，已经包含了本机的时区
	// 2dx中的时区也是从本机获取的，所以不用考虑时区因素
	private long				serverBaseTime;
	private long				localBaseTime;

	private static TimeManager	instance;

	private TimeManager()
	{
	}

	public static TimeManager getInstance()
	{
		if (instance == null)
		{
			instance = new TimeManager();
		}
		return instance;
	}

	/**
	 * @param serverTime utc时间，s为单位为
	 */
	public void setServerBaseTime(int serverTime)
	{
		serverBaseTime = (long) serverTime * 1000;
		localBaseTime = System.currentTimeMillis();
		System.out.println("setServerBaseTime() serverBaseTime:" + serverBaseTime + " localBaseTime:" + localBaseTime);
	}

	/**
	 * @return utc时间，单位为s
	 */
	public int getCurrentTime()
	{
		long ms = getCurrentTimeMS();
		double s = ms / 1000;	// 12位数转换过来只能保留到小数点后9位
//		System.out.println("getCurrentTime() ms:" + ms + " s:" + (int) Math.round(s));

		return (int) Math.round(s);
	}
	
	public long getCurrentTimeMS()
	{
//		System.out.println("getCurrentTimeMS() ms:" + (serverBaseTime + System.currentTimeMillis() - localBaseTime));
		return serverBaseTime + System.currentTimeMillis() - localBaseTime;
	}
	
	/**
	 * @return 当前时区与0时区的时间差，单位为s
	 */
	public int getTimeOffset()
	{
		Calendar cal = Calendar.getInstance(Locale.getDefault());
		return (cal.get(Calendar.ZONE_OFFSET) + cal.get(Calendar.DST_OFFSET)) / 1000;
	}

	/**
	 * @return 当前时区的gmt时间，单位为s
	 */
	public int getCurrentLocalTime()
	{
		return getCurrentTime() + getTimeOffset();
	}

	private static Date getDate(int time)
	{
		Date date = new Date((long) time * 1000);
		return date;
	}
	
	public static String getTimeYMDHM(int time)
	{
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm", Locale.getDefault());
		return formatter.format(getDate(time));
	}
	
	public static String getTimeMDHM(int time)
	{
		SimpleDateFormat formatter = new SimpleDateFormat("MM-dd HH:mm", Locale.getDefault());
		return formatter.format(getDate(time));
	}

	public static String getSendTimeYMD(int time)
	{
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
		return formatter.format(getDate(time));
	}

	public static String getSendTimeHM(int time)
	{
		SimpleDateFormat formatter = new SimpleDateFormat("HH:mm", Locale.getDefault());
		return formatter.format(getDate(time));
	}
	
	public static boolean isLastYear(int time)
	{
		Calendar cal = Calendar.getInstance();
		cal.setTime(getDate(time));  
		int year = cal.get(Calendar.YEAR);
		cal.setTime(new Date(System.currentTimeMillis()));
		int currentYear = cal.get(Calendar.YEAR);
		if(year < currentYear)
			return true;
		return false;
	}
	
	public static boolean isInValidTime(int time)
	{
		boolean isInValid = false;
		Calendar cal = Calendar.getInstance();
		cal.setTime(getDate(time));  
		int year = cal.get(Calendar.YEAR);
		isInValid = year<2010;
		return isInValid;
	}
	
	public static String getReadableTime(int time)
	{
		String timeText = "";
		int dt = TimeManager.getInstance().getCurrentTime() - time;
		int timedt = 0;
//		System.out.println("getReadableTime dt:" + dt);
		if (dt >= 24 * 3600 * 2)
		{
			if(isLastYear(time))
				timeText = getTimeYMDHM(time);
			else
				timeText = getTimeMDHM(time);
		}
		else
		{
			if (dt >= 24 * 60 * 60)
			{
				timedt = dt / (24 * 60 * 60);
				timeText = "" + timedt + LanguageManager.getLangByKey(LanguageKeys.TIME_DAY);
			}
			else if (dt >= 60 * 60)
			{
				timedt = dt / (60 * 60);
				timeText = "" + timedt + LanguageManager.getLangByKey(LanguageKeys.TIME_HOUR);
			}
			else if (dt >= 60)
			{
				timedt = dt / 60;
				timeText = "" + timedt + LanguageManager.getLangByKey(LanguageKeys.TIME_MIN);
			}
			else
			{
				timeText = "1" + LanguageManager.getLangByKey(LanguageKeys.TIME_MIN);
			}
			timeText = timeText + " " + LanguageManager.getLangByKey(LanguageKeys.TIME_BEFORE);
		}
		return timeText;
	}
}
