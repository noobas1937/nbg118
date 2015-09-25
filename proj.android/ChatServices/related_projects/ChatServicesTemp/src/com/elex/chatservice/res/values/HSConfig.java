package com.elex.chatservice.res.values;

import android.util.Log;
import java.util.HashMap;
import java.util.Map;
import org.json.JSONException;
import org.json.JSONObject;

public final class HSConfig
{
  public static final String TAG = "HelpShiftDebug";
  public static Map<String, String> themeData = new HashMap();
  public static Map<String, Object> configData;

  public static void updateThemeData(JSONObject theme)
    throws JSONException
  {
    themeData.put("title", theme.getString("title"));
    themeData.put("sp", theme.getString("sp"));
    themeData.put("hc", theme.getString("hc"));
    themeData.put("tc", theme.getString("tc"));
    themeData.put("hl", theme.getString("hl"));
  }

  public static void updateConfig(JSONObject config) {
    configData.put("rurl", config.optString("rurl", ""));
    configData.put("bcl", Integer.valueOf(config.optInt("bcl", 10)));
    configData.put("dbgl", Integer.valueOf(config.optInt("dbgl", 25)));
    configData.put("pr", config.optJSONObject("pr"));
    configData.put("pfe", Boolean.valueOf(config.optBoolean("pfe", true)));
    configData.put("rne", Boolean.valueOf(config.optBoolean("rne", false)));
    configData.put("dia", Boolean.valueOf(config.optBoolean("dia", false)));
    try {
      updateThemeData(config.getJSONObject("t"));
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", e.toString(), e);
    }
  }

  static
  {
    themeData.put("title", "Help");
    themeData.put("sp", "Describe your problem");
    themeData.put("hc", "516B90");
    themeData.put("tc", "535353");
    themeData.put("hl", "true");

    configData = new HashMap();
    configData.put("bcl", Integer.valueOf(10));
    configData.put("dbgl", Integer.valueOf(0));
    configData.put("rurl", "");
    configData.put("t", themeData);
    configData.put("pfe", Boolean.valueOf(true));
    configData.put("pr", null);
    configData.put("rne", Boolean.valueOf(false));
    configData.put("dia", Boolean.valueOf(false));
  }
}