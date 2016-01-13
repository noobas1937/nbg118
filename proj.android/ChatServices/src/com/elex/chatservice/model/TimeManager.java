package com.elex.chatservice.model;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

/**
 * Date是以默认时区创建的，已经包含了本机的时区 2dx中的时区也是从本机获取的，所以不用考虑时区因素
 */
public class TimeManager
{
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

	public void setServerBaseTime(int serverTime)
	{
		serverBaseTime = (long) serverTime * 1000;
		localBaseTime = System.currentTimeMillis();
	}

	public int getCurrentTime()
	{
		long ms = getCurrentTimeMS();
		double s = ms / 1000;
		return (int) Math.round(s);
	}

	public long getCurrentTimeMS()
	{
		return serverBaseTime + System.currentTimeMillis() - localBaseTime;
	}

	/**
	 * 获取当前时区与0时区的时间差，单位为s
	 */
	public int getTimeOffset()
	{
		Calendar cal = Calendar.getInstance(Locale.getDefault());
		return (cal.get(Calendar.ZONE_OFFSET) + cal.get(Calendar.DST_OFFSET)) / 1000;
	}

	/**
	 * 获取当前时区的gmt时间，单位为s
	 */
	public int getCurrentLocalTime()
	{
		return getCurrentTime() + getTimeOffset();
	}

	public boolean isToday(int time)
	{
		Calendar cal = Calendar.getInstance();
		cal.setTime(getDate(time));
		int date = cal.get(Calendar.DATE);
		cal.setTime(new Date(getCurrentTimeMS()));
		int currentDate = cal.get(Calendar.DATE);
		if (date == currentDate)
			return true;
		return false;
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
		if (year < currentYear)
			return true;
		return false;
	}

	public static boolean isInValidTime(long time)
	{
		return isInValidTime(getTimeInS(time));
	}

	public static boolean isInValidTime(int time)
	{
		boolean isInValid = false;
		Calendar cal = Calendar.getInstance();
		cal.setTime(getDate(time));
		int year = cal.get(Calendar.YEAR);
		isInValid = year < 2010;
		return isInValid;
	}

	public static String getReadableTime(long time)
	{
		return getReadableTime(getTimeInS(time));
	}
	
	public static int getTimeInS(long time)
	{
		if(time > 9999999999L){
			return Double.valueOf(Math.floor(time / 1000)).intValue();
		}else{
			return Long.valueOf(time).intValue();
		}
	}
	
	public static long getTimeInMS(long time)
	{
		if(time < 10000000000L){
			return time * 1000;
		}else{
			return time;
		}
	}

	public static String getReadableTime(int time)
	{
		String timeText = "";
		int dt = TimeManager.getInstance().getCurrentTime() - time;
		int timedt = 0;
		if (dt >= 24 * 3600 * 2)
		{
			if (isLastYear(time))
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
