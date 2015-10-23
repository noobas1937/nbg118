package org.nbg.util;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.text.format.Formatter;

public class AOEUtil {

	/**
	 * 判断是否连网
	 * 
	 * @param act
	 * @return true:联网 false:不联网
	 */
	public static boolean isNetworkAvailable(Context act) {
		ConnectivityManager manager = (ConnectivityManager) act
				.getApplicationContext().getSystemService(
						Context.CONNECTIVITY_SERVICE);
		if (manager == null) {
			return false;
		}
		NetworkInfo networkinfo = manager.getActiveNetworkInfo();

		if (networkinfo == null || !networkinfo.isAvailable()) {
			return false;
		}

		return true;
	}

	public static String getDeviceTotalMemory(Context context) {
		String memTag = "/proc/meminfo";// 系统内存信息文件
		String memInfo;
		String[] arrayOfString;
		long initial_memory = 0;
		try {
			FileReader localFileReader = new FileReader(memTag);
			BufferedReader localBufferedReader = new BufferedReader(
					localFileReader, 8192);
			memInfo = localBufferedReader.readLine();// 读取meminfo第一行，系统总内存大小

			arrayOfString = memInfo.split("\\s+");
			// for (String num : arrayOfString) {
			// Log.d(memInfo, num + "\t");
			// }
			initial_memory = Integer.valueOf(arrayOfString[1]).intValue() * 1024;// 获得系统总内存，单位是KB，乘以1024转换为Byte
			localBufferedReader.close();

			// String[] args = { "/system/bin/cat", "/proc/meminfo" };
			// String memoryResult = CMDExecute.run(args, "/system/bin/");
			// DebugLog.d(
			// TAG,
			// "mem=" + memoryResult + " size="
			// + Formatter.formatFileSize(context, initial_memory));
		} catch (Exception e) {
		}
		return Formatter.formatFileSize(context, initial_memory);// Byte转换为KB或者MB，内存大小规格化
	}

	// 获得系统可用内存信息
	public static String getDeviceAvaialbeMemorySize(Context context) {
		long memSize = getDeviceAvaiableMemory(context);
		// 字符类型转换
		String availMemStr = AOEUtil.formateFileSize(context, memSize);
		return availMemStr;
	}

	private static long getDeviceAvaiableMemory(Context context) {
		ActivityManager am = (ActivityManager) context
				.getSystemService(Activity.ACTIVITY_SERVICE);
		// 获得MemoryInfo对象
		MemoryInfo memoryInfo = new MemoryInfo();
		// 获得系统可用内存，保存在MemoryInfo对象上
		am.getMemoryInfo(memoryInfo);
		return memoryInfo.availMem;
	}

	/**
	 * 调用系统函数，字符串转换 long -String KB/MB
	 * 
	 * @param size
	 * @return
	 */
	private static String formateFileSize(Context context, long size) {
		return Formatter.formatFileSize(context, size);
	}

	/**
	 * MD5
	 * 
	 * @author Administrator
	 * 
	 */
	public static class MD5 {

		public static String getMD5Str(String str) {
			MessageDigest messageDigest = null;

			try {
				messageDigest = MessageDigest.getInstance("MD5");

				messageDigest.reset();

				messageDigest.update(str.getBytes("UTF-8"));
			} catch (NoSuchAlgorithmException e) {
				throw new RuntimeException(e);
			} catch (UnsupportedEncodingException e) {
				throw new RuntimeException(e);
			}

			byte[] byteArray = messageDigest.digest();

			StringBuffer md5StrBuff = new StringBuffer();

			for (int i = 0; i < byteArray.length; i++) {
				if (Integer.toHexString(0xFF & byteArray[i]).length() == 1)
					md5StrBuff.append("0").append(
							Integer.toHexString(0xFF & byteArray[i]));
				else
					md5StrBuff.append(Integer.toHexString(0xFF & byteArray[i]));
			}

			return md5StrBuff.toString();
		}
	}
	/**
	 * 将Unix时间戳转换成指定格式日期
	 * 
	 * @param timestampString
	 * @param formats
	 * @return
	 */
	public static String TimeStampDate(String timestampString, String formats) {
		Long timestamp = Long.parseLong(timestampString) * 1000;
		String date = new java.text.SimpleDateFormat(formats)
				.format(new java.util.Date(timestamp));
		return date;
	}
	/**
	 * 将Unix时间戳转换成指定格式日期
	 * 
	 * @param timestampString
	 * @param formats
	 * @return
	 */
	public static String TimeStampDate(Long timestamp, String formats) {
		return TimeStampDate(timestamp, formats, true);
	}
	
	public static String TimeStampDate(Long timestamp, String formats, boolean useServerTimeZone) {
		SimpleDateFormat sdf = new SimpleDateFormat(formats);
		if(useServerTimeZone) {
			// todo by lxl
			//sdf.setTimeZone(TimeZone.getTimeZone("GMT" + AppData.getInstance().getCityInfo().getTimeZoneOffSet() + ":00"));
		}
		return sdf.format(new Date(timestamp));
	}
}
