package org.nbg.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.util.Locale;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;
import android.telephony.TelephonyManager;
import android.text.format.Formatter;
import android.util.DisplayMetrics;
import android.util.Log;

import org.cocos2dx.ext.Device;

import com.smartfoxserver.v2.entities.data.ISFSObject;
import com.smartfoxserver.v2.entities.data.SFSObject;

public class DeviceUtil {
	public static String TAG = "DeviceInfo";
	public static float dmDensity = 0;

	static class CMDExecute {

		public static synchronized String run(String[] cmd, String workdirectory)
				throws IOException {
			String result = "";
			try {
				ProcessBuilder builder = new ProcessBuilder(cmd);
				InputStream in = null;
				// 设置一个路径
				if (workdirectory != null) {
					builder.directory(new File(workdirectory));
					builder.redirectErrorStream(true);
					Process process = builder.start();
					in = process.getInputStream();
					byte[] re = new byte[1024];
					while (in.read(re) != -1)
						result = result + new String(re);
				}
				if (in != null) {
					in.close();
				}
			} catch (Exception ex) {
				ex.printStackTrace();
			}
			return result;
		}
	}

	public static int getDevicePerAppHeapMemory(Context context) {
		ActivityManager activityManager = (ActivityManager) context
				.getSystemService(Context.ACTIVITY_SERVICE);
		if (activityManager != null) {
			int nAppMaxMemory = activityManager.getMemoryClass();
			return nAppMaxMemory;
		}
		return -1;
	}

	// 获得系统可用内存信息
	public static String getDeviceAvaialbeMemorySize(Context context) {
		long memSize = getDeviceAvaiableMemory(context);
		// 字符类型转换
		String availMemStr = formateFileSize(context, memSize);
		return availMemStr;
	}

	public static long getDeviceAvaiableMemory(Context context) {
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
	public static String formateFileSize(Context context, long size) {
		return Formatter.formatFileSize(context, size);
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
			for (String num : arrayOfString) {
				Log.i(memInfo, num + "\t");
			}
			initial_memory = Integer.valueOf(arrayOfString[1]).intValue() * 1024;// 获得系统总内存，单位是KB，乘以1024转换为Byte
			localBufferedReader.close();

			// String[] args = { "/system/bin/cat", "/proc/meminfo" };
			// String memoryResult = CMDExecute.run(args, "/system/bin/");
			// DebugLog.d(
			// TAG,
			// "mem=" + memoryResult + " size="
			// + Formatter.formatFileSize(context, initial_memory));
		} catch (IOException e) {
		}
		return Formatter.formatFileSize(context, initial_memory);// Byte转换为KB或者MB，内存大小规格化
	}

	public static String getDeviceInfos(Context context) {
		// 三星GT-i9100|OS|SDK-VERSION|WxH|memory|CPU|LANG|App-ver
		StringBuffer deviceInfo = new StringBuffer();
		String appVer = Device.getVersionName();
		DisplayMetrics dm = new DisplayMetrics();
		((Activity) context).getWindowManager().getDefaultDisplay()
				.getMetrics(dm);
		String WxH = dm.widthPixels + "x" + dm.heightPixels;
		// android.os.Build.MODEL -->厂商--HTC Desire
		// android.os.Build.DISPLAY-->MIUI.2.3.7a
		String osInfo = android.os.Build.MODEL + ",os:"
				+ android.os.Build.DISPLAY + ",sdk-ver:"
				+ android.os.Build.VERSION.SDK_INT;
		deviceInfo.append(osInfo);
		deviceInfo.append("|").append(WxH).append(",dpi:").append(dm.density);
		// 内存信息
		// 获得ActivityManager服务的对象
		String totalMemory = getDeviceTotalMemory(context);
		// 获得可用内存信息
		String availMemory = getDeviceAvaialbeMemorySize(context);
		deviceInfo.append("|memory:" + availMemory + "/" + totalMemory);
		String cpuInfo = getDeviceCPUInfo();
		deviceInfo.append("|" + cpuInfo);
		deviceInfo.append("|App-ver:" + appVer);
		deviceInfo.append("|lang:" + Locale.getDefault().getLanguage());
		deviceInfo.append("_" + Locale.getDefault().getCountry());
		return deviceInfo.toString();
		// TelephonyManager tm = (TelephonyManager) context
		// .getSystemService(context.TELEPHONY_SERVICE);
		// StringBuilder sb = new StringBuilder();
		// sb.append("DeviceId(IMEI) = " + tm.getDeviceId());
		// sb.append("|DeviceSoftwareVersion = " +
		// tm.getDeviceSoftwareVersion());
		// sb.append("|Line1Number = " + tm.getLine1Number());
		// sb.append("|NetworkCountryIso = " + tm.getNetworkCountryIso());
		// sb.append("|NetworkOperator = " + tm.getNetworkOperator());
		// sb.append("|NetworkOperatorName = " + tm.getNetworkOperatorName());
		// sb.append("|NetworkType = " + tm.getNetworkType());
		// sb.append("|PhoneType = " + tm.getPhoneType());
		// sb.append("|SimCountryIso = " + tm.getSimCountryIso());
		// sb.append("|SimOperator = " + tm.getSimOperator());
		// sb.append("|SimOperatorName = " + tm.getSimOperatorName());
		// sb.append("|SimSerialNumber = " + tm.getSimSerialNumber());
		// sb.append("|SimState = " + tm.getSimState());
		// sb.append("|SubscriberId(IMSI) = " + tm.getSubscriberId());
		// sb.append("|VoiceMailNumber = " + tm.getVoiceMailNumber());
		// DebugLog.d(TAG, "Tel=" + sb.toString());

	}

	/**
	 * Processor\t : ARMv7 Processor rev 2 (v7l)\t<br>
	 * BogoMIPS : 662.40<br>
	 * Features : swp half thumb fastmult vfp edsp thumbee neon <br>
	 * CPU implementer : 0x51<br>
	 * CPU architecture: 7<br>
	 * CPU variant : 0x0<br>
	 * CPU part : 0x00f<br>
	 * CPU revision : 2<br>
	 * Hardware : bravo<br>
	 * Revision : 0084<br>
	 * Serial : 0000000000000000
	 * 
	 * @return
	 */
	public static String getDeviceCPUInfo() {
		StringBuffer cpuBuf = new StringBuffer();
		try {
			String[] args = { "/system/bin/cat", "/proc/cpuinfo" };
			String result = CMDExecute.run(args, "/system/bin/");
			// DebugLog.d(TAG, "CPU=" + result);
			String[] parmArry = result.split("\n");
			for (String parm : parmArry) {
				if (parm.startsWith("Processor")) {
					cpuBuf.append(" ").append(parm);
				} else if (parm.startsWith("BogoMIPS")) {
					cpuBuf.append(",").append(parm);
				} else if (parm.startsWith("Hardware")) {
					cpuBuf.append(",").append(parm);
				}
			}
			return cpuBuf.toString();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
		return "";
	}

	public static boolean isEmulator() {
		return (Build.MODEL.equals("sdk"))
				|| (Build.MODEL.equals("google_sdk"));
	}

	public static String getSimpleDeviceInfo(Context ctx) {
		DisplayMetrics dm = new DisplayMetrics();
		((Activity) ctx).getWindowManager().getDefaultDisplay().getMetrics(dm);
		StringBuilder deviceInfo = new StringBuilder();
		deviceInfo.append("screen:" + dm.widthPixels + "x" + dm.heightPixels);
		deviceInfo.append(" density:" + dm.densityDpi).append("\n");
		deviceInfo.append("isNetworkAvailable:" + AOEUtil.isNetworkAvailable(ctx))
				.append("\n");
		int networkType = getNetWorkType(ctx);
		deviceInfo.append("networkType:" + networkType).append("\n");
		deviceInfo.append("networkName:" + getNetWorkTypeName(ctx))
				.append("\n");
		if (networkType != ConnectivityManager.TYPE_WIFI
				&& networkType != ConnectivityManager.TYPE_WIMAX) {
			// 移动网络
			try {
				TelephonyManager tm = (TelephonyManager) ctx
						.getSystemService(Context.TELEPHONY_SERVICE);
				int network_type = tm.getNetworkType();
				// 返回移动网络运营商的名字(SPN)
				deviceInfo.append("operator_name:")
						.append(tm.getNetworkOperatorName()).append("\n");// G7---SoftBank
				deviceInfo.append("network_type:").append(network_type)
						.append("\n");
				deviceInfo.append("network_name:")
						.append(getMobileNetworkType(network_type))
						.append("\n");
			} catch (Exception e) {
				deviceInfo.append("operators:").append("UNKNOWN").append("\n");// G7---SoftBank
				deviceInfo.append("network_type:").append(0).append("\n");
			}
		}
		return deviceInfo.toString();
	}

	/**
	 * 获得当前联网类型wifi or mobile
	 * 
	 */
	public static String getNetWorkTypeName(Context context) {
		ConnectivityManager manager = (ConnectivityManager) context
				.getApplicationContext().getSystemService(
						Context.CONNECTIVITY_SERVICE);
		if (manager == null) {
			return "UNKNOWN";
		}
		NetworkInfo networkinfo = manager.getActiveNetworkInfo();
		if (networkinfo != null) {
			return networkinfo.getTypeName();
		} else {
			return "UNKNOWN";
		}
	}

	/**
	 * / Field descriptor #26 I public static final int TYPE_MOBILE = 0;
	 * 
	 * // Field descriptor #26 I public static final int TYPE_WIFI = 1;
	 * 
	 * // Field descriptor #26 I public static final int TYPE_MOBILE_MMS = 2;
	 * 
	 * // Field descriptor #26 I public static final int TYPE_MOBILE_SUPL = 3;
	 * 
	 * // Field descriptor #26 I public static final int TYPE_MOBILE_DUN = 4;
	 * 
	 * // Field descriptor #26 I public static final int TYPE_MOBILE_HIPRI = 5;
	 * 
	 * // Field descriptor #26 I public static final int TYPE_WIMAX = 6;
	 * 
	 * @param context
	 * @return -1: 无连接类型; ConnectivityManager.TYPE_WIFI: wifi;
	 *         ConnectivityManager.TYPE_MOBILE:mobile
	 */
	public static int getNetWorkType(Context context) {
		ConnectivityManager manager = (ConnectivityManager) context
				.getApplicationContext().getSystemService(
						Context.CONNECTIVITY_SERVICE);
		if (manager == null) {
			return -1;
		}
		NetworkInfo networkinfo = manager.getActiveNetworkInfo();
		if (networkinfo != null) {
			return networkinfo.getType();
		} else {
			return -1;
		}
	}

	public static String getMobileNetworkType(int type) {
		String typeString = "UNKNOWN";
		if (type == TelephonyManager.NETWORK_TYPE_CDMA) {
			typeString = "CDMA";
		}
		if (type == TelephonyManager.NETWORK_TYPE_EDGE) {
			typeString = "EDGE";
		}
		if (type == TelephonyManager.NETWORK_TYPE_EVDO_0) {
			typeString = "EVDO_0";
		}
		if (type == TelephonyManager.NETWORK_TYPE_EVDO_A) {
			typeString = "EVDO_A";
		}
		if (type == TelephonyManager.NETWORK_TYPE_GPRS) {
			typeString = "GPRS";
		}
		if (type == TelephonyManager.NETWORK_TYPE_HSDPA) {
			typeString = "HSDPA";
		}
		if (type == TelephonyManager.NETWORK_TYPE_HSPA) {
			typeString = "HSPA";
		}
		if (type == TelephonyManager.NETWORK_TYPE_HSUPA) {
			typeString = "HSUPA";
		}
		if (type == TelephonyManager.NETWORK_TYPE_UMTS) {
			typeString = "UMTS";
		}
		if (type == TelephonyManager.NETWORK_TYPE_UNKNOWN) {
			typeString = "UNKNOWN";
		}
		return typeString;
	}

	public static ISFSObject getDeviceInfo(Context ctx) {
		ISFSObject deviceInfo = new SFSObject();
		deviceInfo.putUtfString("model", android.os.Build.MODEL);
		deviceInfo.putUtfString("os_version", android.os.Build.VERSION.RELEASE);
		deviceInfo.putUtfString("os_build", android.os.Build.DISPLAY);
		DisplayMetrics dm = new DisplayMetrics();
		((Activity) ctx).getWindowManager().getDefaultDisplay().getMetrics(dm);
		deviceInfo.putInt("screen_w", dm.widthPixels);
		deviceInfo.putInt("screen_h", dm.heightPixels);
		deviceInfo.putInt("screen_density", dm.densityDpi);
		try {
			TelephonyManager tm = (TelephonyManager) ctx
					.getSystemService(Context.TELEPHONY_SERVICE);
			// 返回移动网络运营商的名字(SPN)
			deviceInfo.putUtfString("operator_name",
					tm.getNetworkOperatorName());// G7---SoftBank
			deviceInfo.putInt("network_type", tm.getNetworkType());
		} catch (Exception e) {
			deviceInfo.putUtfString("operators", "UNKNOWN");// G7---SoftBank
			deviceInfo.putInt("network_type", 0);
		}
		// StringBuffer deviceLog = new StringBuffer();
		// deviceLog.append("model:"+android.os.Build.MODEL);
		// deviceLog.append(" os_version:"+android.os.Build.VERSION.RELEASE);
		// deviceLog.append(" os_build:"+ android.os.Build.DISPLAY);
		//
		// deviceLog.append(" screen_w:"+ dm.widthPixels);
		// deviceLog.append(" screen_h:"+dm.heightPixels);
		// deviceLog.append(" screen_density:"+dm.densityDpi);
		// try {
		// TelephonyManager tm = (TelephonyManager) ctx
		// .getSystemService(Context.TELEPHONY_SERVICE);
		// // 返回移动网络运营商的名字(SPN)
		// deviceLog.append(" operator_name:"+ tm.getNetworkOperatorName());//
		// G7---SoftBank
		// deviceLog.append(" network_type:"+tm.getNetworkType());
		// } catch (Exception e) {
		//
		// }
		// Log.d("device", deviceLog.toString());
		return deviceInfo;
	}

	public static float getScreenDensity() {
		if (dmDensity == 0) {
			try {
				DisplayMetrics dm = new DisplayMetrics();
				((Activity) GameContext.getActivityInstance())
						.getWindowManager().getDefaultDisplay().getMetrics(dm);
				dmDensity = dm.density;
			} catch (Exception e) {
				dmDensity = DisplayMetrics.DENSITY_DEFAULT;
			}
		}
		return dmDensity;
	}

	public static int hdpiPixelsToScreenDpiPixels(int hdpiPixels) {
		float density = getScreenDensity();// hdpi 1.5
		return (int) (hdpiPixels / 1.5f * density);

	}

	public static String getDeviceBoard() {
		String id = android.os.Build.MODEL;
		if (id.contains("GT-I9300") || id.contains("GT-I9100")
				|| id.contains("GT-S5360") || id.contains("GT-N7000")
				|| id.contains("Nexus 7") || id.contains("GT-N7100")
				|| id.contains("GT-S5830i") || id.contains("GT-S5360L")
				|| id.contains("GT-S5830") || id.contains("GT-P5110"))
			return id;
		return "";
	}

	public static boolean isAvailableShortCutSystem() {
		String id = android.os.Build.BOARD;
		// String str =
		// android.os.Build.FINGERPRINT;//samsung/GT-I9000/GT-I9000/GT-I9000:2.2.1/FROYO/ZSJPG:user/release-keys

		if (id.contains("MI") || id.contains("mx"))
			return false;
		return true;
	}
}
