package com.elex.chatservice.util;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;
import android.os.Build.VERSION;
import android.os.Environment;
import android.os.StatFs;
import android.telephony.TelephonyManager;
import android.text.TextUtils;

import com.elex.chatservice.HSCallable;
import com.elex.chatservice.HSStorage;
import com.elex.chatservice.res.values.HSConfig;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Locale;
import java.util.Map;
import java.util.MissingResourceException;
import java.util.Set;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public final class Meta
{
  public static final String TAG = "HelpShiftDebug";
  private static HSCallable metaFn;

  public static JSONObject getMetaInfo(Context c, Boolean attachDeviceInfo, String customIdentifier)
  {
    JSONObject meta = new JSONObject();
    try
    {
      meta.put("breadcrumbs", getBreadCrumbs(c));
    } catch (JSONException e) {
      android.util.Log.d("HelpShiftDebug", "Error while getting device info", e);
    }

    try
    {
      if (attachDeviceInfo.booleanValue()) {
        meta.put("device_info", getDeviceInfo(c));
      }
      else
        meta.put("device_info", new JSONObject());
    }
    catch (JSONException e) {
      android.util.Log.d("HelpShiftDebug", "Error while getting device info", e);
    }

    try
    {
      meta.put("extra", getExtra(customIdentifier));
    } catch (JSONException e) {
      android.util.Log.d("HelpShiftDebug", "Error while getting extra info", e);
    }

    try
    {
      Integer logLimit = (Integer)HSConfig.configData.get("dbgl");
      meta.put("logs", formatLogList(com.elex.chatservice.Log.getLogs(logLimit.intValue())));
    }
    catch (JSONException e) {
      android.util.Log.d("HelpShiftDebug", "Error while getting debug logs", e);
    }

    try
    {
      HSStorage storage = new HSStorage(c);
      meta.put("device_token", storage.getDeviceToken());
    } catch (JSONException e) {
      android.util.Log.d("HelpShiftDebug", "Error while getting device token", e);
    }

    if (metaFn != null) {
      try {
        JSONObject customMeta = getCustomMeta();
        if (customMeta != null)
          meta.put("custom_meta", customMeta);
      }
      catch (JSONException e) {
        android.util.Log.d("HelpShiftDebug", "Error while getting extra meta", e);
      }
    } else {
      HSStorage storage = new HSStorage(c);
      JSONObject customMeta = storage.getMetaData();
      if (meta != null) {
        try {
          meta.put("custom_meta", customMeta);
        } catch (JSONException e) {
          android.util.Log.d("HelpShiftDebug", "Error while getting extra meta ", e);
        }
      }

    }

    return meta;
  }

  private static JSONArray getBreadCrumbs(Context c)
    throws JSONException
  {
    HSStorage storage = new HSStorage(c);
    return storage.getBreadCrumbs();
  }

  private static JSONObject getDeviceInfo(Context c)
    throws JSONException
  {
    JSONObject deviceInfo = new JSONObject();

    deviceInfo.put("platform", "android");
    deviceInfo.put("library-version", "3.4.1");
    deviceInfo.put("device-model", Build.MODEL);
    deviceInfo.put("os-version", Build.VERSION.RELEASE);
    try
    {
      deviceInfo.put("language-code", Locale.getDefault().getISO3Language());
    } catch (MissingResourceException e) {
      android.util.Log.d("HelpShiftDebug", "Device Info - MissingResourceException", e);
    }

    String ts = HSFormat.deviceInfoTsFormat.format(new Date());
    deviceInfo.put("timestamp", ts);

    String packageName = c.getPackageName();
    deviceInfo.put("application-identifier", packageName);

    deviceInfo.put("application-name", getAppName(c));

    deviceInfo.put("application-version", getApplicationVersion(c));

    deviceInfo.put("disk-space", getDiskSpace(c));

    TelephonyManager tm = (TelephonyManager)c.getSystemService("phone");
    deviceInfo.put("country-code", tm.getSimCountryIso());

    deviceInfo.put("carrier-name", tm.getNetworkOperatorName());
    try
    {
      deviceInfo.put("network-type", getNetworkType(c));
    } catch (SecurityException e) {
      android.util.Log.d("HelpShiftDebug", "No permission for Network Access", e);
    }

    IntentFilter ifilter = new IntentFilter("android.intent.action.BATTERY_CHANGED");
    Intent batteryStatus = c.registerReceiver(null, ifilter);
    deviceInfo.put("battery-level", getBatteryLevel(batteryStatus));
    deviceInfo.put("battery-status", getBatteryStatus(batteryStatus));

    return deviceInfo;
  }

  private static JSONObject getExtra(String customIdentifier)
    throws JSONException
  {
    JSONObject extra = new JSONObject();
    extra.put("api-version", "1");
    extra.put("library-version", "3.4.1");
    if (customIdentifier != null) {
      extra.put("user-id", customIdentifier);
    }

    return extra;
  }

  private static String getAppName(Context c)
  {
    PackageManager pm = c.getPackageManager();
    ApplicationInfo ai;
    try
    {
      ai = pm.getApplicationInfo(c.getPackageName(), 0);
    } catch (PackageManager.NameNotFoundException e) {
      ai = null;
    }
    String applicationName = (String)(ai != null ? pm.getApplicationLabel(ai) : "(unknown)");

    return applicationName;
  }

  private static JSONObject getDiskSpace(Context c)
  {
    JSONObject diskSpace = new JSONObject();

    StatFs phoneStat = new StatFs(Environment.getDataDirectory().getPath());
    StatFs sdStat = new StatFs(Environment.getExternalStorageDirectory().getPath());

    double free_phone_memory = phoneStat.getAvailableBlocks() * phoneStat.getBlockSize() / 1073741824.0D;
    free_phone_memory = Math.round(free_phone_memory * 100.0D) / 100.0D;
    double total_phone_memory = phoneStat.getBlockCount() * phoneStat.getBlockSize() / 1073741824.0D;
    total_phone_memory = Math.round(total_phone_memory * 100.0D) / 100.0D;

    double free_sd_memory = sdStat.getAvailableBlocks() * sdStat.getBlockSize() / 1073741824.0D;
    free_sd_memory = Math.round(free_sd_memory * 100.0D) / 100.0D;
    double total_sd_memory = sdStat.getBlockCount() * sdStat.getBlockSize() / 1073741824.0D;
    total_sd_memory = Math.round(total_sd_memory * 100.0D) / 100.0D;
    try
    {
      diskSpace.put("free-space-phone", free_phone_memory + " GB");
      diskSpace.put("total-space-phone", total_phone_memory + " GB");
      diskSpace.put("free-space-sd", free_sd_memory + " GB");
      diskSpace.put("total-space-sd", total_sd_memory + " GB");
    } catch (JSONException e) {
      android.util.Log.d("HelpShiftDebug", e.toString(), e);
    }
    return diskSpace;
  }

  private static String getNetworkType(Context c)
  {
    ConnectivityManager cm = (ConnectivityManager)c.getSystemService("connectivity");
    NetworkInfo ani = cm.getActiveNetworkInfo();
    String type = "Unknown";
    if (ani != null) {
      type = ani.getTypeName();
    }
    return type;
  }

  private static String getBatteryLevel(Intent batteryStatus)
  {
    int level = batteryStatus.getIntExtra("level", -1);
    int scale = batteryStatus.getIntExtra("scale", -1);

    int batteryPct = (int)(level / scale * 100.0F);
    return batteryPct + "%";
  }

  private static String getBatteryStatus(Intent batteryStatus)
  {
    int status = batteryStatus.getIntExtra("status", -1);
    boolean isCharging = (status == 2) || (status == 5);

    return isCharging ? "Charging" : "Not charging";
  }

  public static String getApplicationVersion(Context c) {
    String appVersion = null;
    try {
      String packageName = c.getPackageName();
      PackageInfo p = c.getPackageManager().getPackageInfo(packageName, 0);
      appVersion = p.versionName;
    } catch (PackageManager.NameNotFoundException e) {
      android.util.Log.d("HelpShiftDebug", "Package not found exception", e);
    }
    return appVersion;
  }

  private static JSONObject formatLog(HashMap log) {
    JSONObject outputLog = new JSONObject();
    try {
      outputLog.put("message", log.get("message"));
      outputLog.put("level", log.get("level"));
      outputLog.put("tag", log.get("tag"));
      outputLog.put("exception", log.get("exception"));
    } catch (JSONException e) {
      android.util.Log.d("HelpShiftDebug", "Format debug lgos", e);
    }

    return outputLog;
  }

  private static JSONArray formatLogList(ArrayList<HashMap> logs) {
    JSONArray outputList = new JSONArray();
    for (int i = 0; i < logs.size(); i++) {
      outputList.put(formatLog((HashMap)logs.get(i)));
    }
    return outputList;
  }

  public static void setMetadataCallback(HSCallable f) {
    metaFn = f;
  }

  public static JSONObject getCustomMeta() throws JSONException {
    HashMap meta = metaFn.call();
    if (meta != null) {
      meta = removeEmptyKeyOrValue(meta);
      return new JSONObject(cleanMetaForTags(meta));
    }
    return null;
  }

  private static HashMap removeEmptyKeyOrValue(HashMap metadata)
  {
    HashMap newMetaData = (HashMap)metadata.clone();

    for (Iterator i$ = metadata.keySet().iterator(); i$.hasNext(); ) { Object key = i$.next();
      Object value = metadata.get(key);
      if (((key instanceof String)) && 
        (((String)key).trim().equalsIgnoreCase(""))) {
        newMetaData.remove(key);
      }
      if (((value instanceof String)) && 
        (((String)value).trim().equalsIgnoreCase(""))) {
        newMetaData.remove(key);
      }
    }
    return newMetaData;
  }

  private static String[] cleanTags(String[] input)
  {
    String[] v = input;
    int w;
    int r;
    int n = r = w = v.length;
    while (r > 0) {
      String s = v[(--r)];
      if ((s != null) && (!TextUtils.isEmpty(s.trim())))
      {
        v[(--w)] = s.trim();
      }
    }

    v = ArraysCompat.copyOfRange(v, w, n);

    v = (String[])new HashSet(Arrays.asList(v)).toArray(new String[0]);

    return v;
  }

  private static HashMap cleanMetaForTags(HashMap meta) {
    Object tags = meta.get("hs-tags");
    meta.remove("hs-tags");
    if ((tags instanceof String[])) {
      JSONArray tagsJSON = new JSONArray(Arrays.asList(cleanTags((String[])tags)));
      meta.put("hs-tags", tagsJSON);
    }
    return meta;
  }
}