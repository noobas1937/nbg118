package com.elex.tech.nbg;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.UUID;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Context;
import android.content.SharedPreferences;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.text.TextUtils;

public class Udid {
	// 新安装的设备--用户
	public static boolean isNewInstallDevice;
	private static final String KEY_UUID = "uuid";
	
	public static Cocos2dxActivity m_activity;

	/**
	 * 读取UUID
	 * 
	 * @return
	 */
	private static String getUuid() {
		String uuid = "";
		uuid = Settings.System.getString(m_activity
				.getContentResolver(), KEY_UUID);
		return uuid;
	}

	/**
	 * 存储UUID
	 * 
	 * @param uuid
	 */
	public static void saveUid(String uuid) {
		Settings.System.putString(m_activity.getContentResolver(),
				KEY_UUID, uuid);
	}

	/**
	 * 取得手机唯一IMEI号
	 * 
	 * @return String
	 */
	public static String getUid() {
		// 取得存储设备的UUID
		String cardUid = getUuid();
		if (TextUtils.isEmpty(cardUid)) {
			isNewInstallDevice = true;
			// 取得UUID
			cardUid = generateUUID(m_activity);
			// 保存到本地文件
			saveUid(cardUid);
		}
		return cardUid;
	}

	/**
	 * 取得手机UUID
	 * 
	 * @param context
	 * @return
	 */
	public static String generateUUID(Context context) {
		String deviceUUID;
		SharedPreferences pref = context.getSharedPreferences("xcuuid", 0);
		if ((pref != null) && (pref.getString("uuid", "") != null)
				&& (pref.getString("uuid", "").trim().length() > 0)) {
			return pref.getString("uuid", "");
		}

		TelephonyManager tm = (TelephonyManager) context
				.getSystemService("phone");
		String deviceId = tm.getDeviceId();

		UUID uuid = UUID.randomUUID();
		String uuidStr = uuid.toString().replaceAll("-", "").substring(0, 15);
		String wifiMAC = getMacAddr(context);
		if (wifiMAC != null) {
			wifiMAC = wifiMAC.replaceAll("\\.|:", "");
		}
		if ((deviceId == null) || (TextUtils.isEmpty(deviceId.trim()))) {
			if ((wifiMAC != null) && (!(TextUtils.isEmpty(wifiMAC)))) {
				deviceUUID = "-" + wifiMAC;
			} else {
				deviceUUID = uuidStr;
			}

		} else if ((wifiMAC != null) && (!(TextUtils.isEmpty(wifiMAC)))) {
			deviceUUID = deviceId + "-" + wifiMAC;
		} else {
			deviceUUID = deviceId + "-" + uuidStr;
		}

		return deviceUUID;
	}

	/**
	 * 取得mac地址
	 * 
	 * @param context
	 * @return
	 */
	public static String getMacAddr(Context context) {
		WifiManager wifi = (WifiManager) context.getSystemService("wifi");
		WifiInfo info = wifi.getConnectionInfo();
		return info.getMacAddress();
	}
	
	/**
	 * 交叉推广需要用到的方法
	 * 
	 * @return
	 */
	public static String getUidForCpb() {
		return "uuid";
//		TelephonyManager tm = (TelephonyManager) GameContext
//				.getActivityInstance().getSystemService(
//						Context.TELEPHONY_SERVICE);
//		// gets the imei (GSM) or MEID/ESN (CDMA)
//		String uid = tm.getDeviceId();
//		if (TextUtils.isEmpty(uid)) {
//			// 设备不能打电话？好吧，来看看wifi有不
//			// requires ACCESS_WIFI_STATE
//			WifiManager wm = (WifiManager) GameContext.getActivityInstance()
//					.getSystemService(Context.WIFI_SERVICE);
//			// gets the MAC address
//			if (wm.isWifiEnabled()) {
//				// 能打电话就肯定有 TelephonyId，否则只能wifi上网玩游戏，那就肯定有wifi的uid
//				uid = wm.getConnectionInfo().getMacAddress();
//			}
//		}
//		// 保存下来
//
//		return SHA1(uid + "uuid");

	}

	public static String SHA1(String s) {
		try {
			MessageDigest digest = MessageDigest.getInstance("SHA-1");
			digest.update(s.getBytes());
			byte messageDigest[] = digest.digest();
			return toHexString(messageDigest);
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}
		return "";
	}

	public static String toHexString(byte[] keyData) {
		if (keyData == null) {
			return null;
		}
		int expectedStringLen = keyData.length * 2;
		StringBuilder sb = new StringBuilder(expectedStringLen);
		for (int i = 0; i < keyData.length; i++) {
			String hexStr = Integer.toString(keyData[i] & 0x00FF, 16);
			if (hexStr.length() == 1) {
				hexStr = "0" + hexStr;
			}
			sb.append(hexStr);
		}
		return sb.toString();
	}

}
