package com.elex.chatservice.util;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.elex.chatservice.HSApiData;
import com.elex.chatservice.HSConversation;
import com.elex.chatservice.HSStorage;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public final class HSNotification
{
  private static String TAG = "HelpShiftDebug";

  public static void showNotif(Context c, JSONObject issue, int messCnt, String chatLaunchSource)
  {
    try {
      int ts = (int)HSFormat.issueTsFormat.parse(issue.getString("created_at")).getTime();
      showNotif(c, issue.getString("id"), ts, issue.getString("title") + "  ", messCnt, chatLaunchSource);
    }
    catch (ParseException e)
    {
      Log.d(TAG, "showNotif ParseException", e);
    } catch (JSONException e) {
      Log.d(TAG, "showNotif JSONException", e);
    }
  }

  public static void showNotif(Context c, String issueId, int ts, String issueTitle, int newMessCnt, String chatLaunchSource)
  {
    NotificationManager notificationManager = (NotificationManager)c.getSystemService("notification");

    CharSequence notifText = HSRes.getString(c, "hs__notification_content_text") + " " + issueTitle;

    HSStorage storage = new HSStorage(c);
    int notificationIcon;
    try {
      JSONObject config = storage.getAppConfig();
      notificationIcon = config.getInt("notificationIcon");
    } catch (JSONException e) {
      notificationIcon = c.getApplicationInfo().icon;
    }

    CharSequence contentTitle = HSRes.getQuantityString(c, "hs__notification_content_title", newMessCnt, new Object[] { Integer.valueOf(newMessCnt) });

    Notification notification = new Notification(notificationIcon, contentTitle, System.currentTimeMillis());

    notification.flags = 16;

    Intent notificationIntent = new Intent(c, HSConversation.class);
    notificationIntent.setFlags(268435456);
    notificationIntent.putExtra("issueId", issueId);
    notificationIntent.putExtra("chatLaunchSource", chatLaunchSource);
    notificationIntent.putExtra("isRoot", true);
    PendingIntent contentIntent = PendingIntent.getActivity(c, ts, notificationIntent, 0);

    notification.setLatestEventInfo(c, contentTitle, notifText, contentIntent);

    notificationManager.notify(issueId, 1, notification);
  }

  public static Handler getNotifHandler(final Context c, final HSPolling notifCountPoller)
  {
    final HSApiData data = new HSApiData(c);

    Handler notifHandler = new Handler() {
      public void handleMessage(Message msg) {
        JSONArray issues = (JSONArray)msg.obj;
        try {
          if (notifCountPoller != null) {
            notifCountPoller.resetInterval();
          }
          for (int i = 0; i < issues.length(); i++) {
            JSONObject issue = issues.getJSONObject(i);
            String issueId = issue.getString("id");
            if (!data.storage.getForegroundIssue().equals(issueId)) {
              JSONObject storedIssue = data.storage.getIssue(issueId);
              JSONArray messages = storedIssue.getJSONArray("messages");
              int messCnt = storedIssue.optInt("newMessagesCnt", 0);
              if (messCnt != 0)
                try {
                  int ts = (int)HSFormat.issueTsFormat.parse(issue.getString("created_at")).getTime();
                  HSNotification.showNotif(c, issue.getString("id"), ts, issue.getString("title") + "  ", messCnt, "inapp");
                }
                catch (ParseException e)
                {
                  Log.d(HSNotification.TAG, e.toString());
                }
            }
          }
        }
        catch (JSONException e) {
          Log.d(HSNotification.TAG, e.getMessage());
        }
      }
    };
    return notifHandler;
  }
}