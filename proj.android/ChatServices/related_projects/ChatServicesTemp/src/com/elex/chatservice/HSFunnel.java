package com.elex.chatservice;

import android.util.Log;

import com.elex.chatservice.util.HSFormat;

import java.text.DecimalFormat;
import java.util.Date;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

final class HSFunnel
{
  public static final String TAG = "HelpShiftDebug";
  public static final String LIBRARY_OPENED = "o";
  public static final String LIBRARY_OPENED_DECOMP = "d";
  public static final String SUPPORT_LAUNCH = "l";
  public static final String PERFORMED_SEARCH = "s";
  public static final String BROWSED_FAQ_LIST = "b";
  public static final String READ_FAQ = "f";
  public static final String MARKED_HELPFUL = "h";
  public static final String MARKED_UNHELPFUL = "u";
  public static final String REPORTED_ISSUE = "i";
  public static final String CONVERSATION_POSTED = "p";
  public static final String REVIEWED_APP = "r";
  public static final String OPEN_ISSUE = "c";
  public static final String OPEN_INBOX = "x";
  public static final String LIBRARY_QUIT = "q";
  public static final String MESSAGE_ADDED = "m";
  public static final String RESOLUTION_ACCEPTED = "y";
  public static final String RESOLUTION_REJECTED = "n";
  private static JSONArray actionTrail = new JSONArray();
  public static String libraryStartedTs;

  public static void pushEvent(String event)
  {
    pushEvent(event, null);
  }

  public static void pushEvent(String event, JSONObject data) {
    JSONObject eventObj = new JSONObject();
    try {
      eventObj.put("ts", HSFormat.tsSecFormatter.format(System.currentTimeMillis() / 1000.0D));
      eventObj.put("t", event);
      eventObj.put("d", data);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSONException", e);
    }
    actionTrail.put(eventObj);
  }

  public static JSONArray getActions()
  {
    JSONArray actions = actionTrail;
    flush();
    return actions;
  }

  public static void flush()
  {
    actionTrail = new JSONArray();
    libraryStartedTs = Long.toString(new Date().getTime());
  }

  public static void pushAppReviewedEvent(String userAction) {
    try {
      JSONObject eventData = new JSONObject();
      eventData.put("type", "periodic");
      eventData.put("response", userAction);
      pushEvent("r", eventData);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSONException", e);
    }
  }
}