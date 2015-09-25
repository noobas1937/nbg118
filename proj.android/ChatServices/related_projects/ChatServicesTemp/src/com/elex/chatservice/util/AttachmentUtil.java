package com.elex.chatservice.util;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.BitmapFactory.Options;
import android.net.Uri;
import android.text.TextUtils;
import android.widget.Toast;

import com.elex.chatservice.HSApiData;
import com.elex.chatservice.HSStorage;
import com.elex.chatservice.Log;
import com.elex.chatservice.viewstructs.HSMsg;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.UUID;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public final class AttachmentUtil
{
  private static final String TAG = "HelpShiftDebug";
  public static final String LOCAL_RSC_MSG_ID_PREFIX = "localRscMessage_";
  public static final int SCREENSHOT_ATTACH_REQ_CODE = 0;
  private static boolean selectingAttachment = false;

  public static void setSelectingAttachment(boolean selectingAttachment) {
    selectingAttachment = selectingAttachment;
  }

  public static boolean isSelectingAttachment() {
    return selectingAttachment;
  }

  private static void showScreenshotErrorToast(Activity activity) {
    Toast.makeText(activity.getApplicationContext(), HSRes.getString(activity, "hs__screenshot_cloud_attach_error"), 1).show();
  }

  private static String getPath(Activity activity, Uri selectedImageUri)
  {
    String[] projection = { "_data" };
    Cursor cursor = activity.managedQuery(selectedImageUri, projection, null, null, null);
    int column_index = cursor.getColumnIndexOrThrow("_data");
    cursor.moveToFirst();
    return cursor.getString(column_index);
  }
  public static String getPath(Activity activity, Intent dataIntent) {
    Uri ssUri = dataIntent.getData();
    String screenshot;
    File screenshotFile;
    try {
      screenshot = getPath(activity, ssUri);
      screenshotFile = new File(screenshot);
    } catch (NullPointerException e) {
      showScreenshotErrorToast(activity);
      return null;
    }

    if (!screenshotFile.exists()) {
      showScreenshotErrorToast(activity);
      return null;
    }

    Long screenshotLimit = new Long(5242880L);
    if (screenshotFile.length() > screenshotLimit.longValue()) {
      String errorString = String.format(HSRes.getString(activity, "hs__screenshot_limit_error"), new Object[] { Float.valueOf((float)screenshotLimit.longValue() / 1048576.0F) });

      Toast.makeText(activity.getApplicationContext(), errorString, 1).show();

      return null;
    }

    return screenshot;
  }

  public static Bitmap getBitmap(String path, int scale) {
    if (TextUtils.isEmpty(path)) {
      return null;
    }

    BitmapFactory.Options options = new BitmapFactory.Options();
    options.inSampleSize = 4;

    Bitmap bitmap = BitmapFactory.decodeFile(path, options);
    if (scale > 0) {
      float ratio = options.outHeight / options.outWidth;
      bitmap = Bitmap.createScaledBitmap(bitmap, 250, (int)(ratio * 250.0F), false);
    }

    return bitmap;
  }

  public static Bitmap getUnscaledBitmap(String path) {
    if (TextUtils.isEmpty(path)) {
      return null;
    }

    BitmapFactory.Options options = new BitmapFactory.Options();
    options.inScaled = false;

    return BitmapFactory.decodeFile(path, options);
  }

  public static String copyAttachment(Activity activity, HSApiData hsApiData, String filename, String messageId, int attachId)
    throws IOException
  {
    InputStream input = null;
    FileOutputStream output = null;
    try {
      String outputName = messageId + attachId + "-thumbnail";
      File outputFile = new File(activity.getFilesDir(), outputName);
      String fname = outputFile.getAbsolutePath();
      int read;
      if (!outputFile.exists()) {
        hsApiData.storeFile(outputName);
        input = new FileInputStream(new File(filename));
        output = activity.openFileOutput(outputName, 0);

        Log.d("HelpShiftDebug", fname);

        byte[] data = new byte[1024];
        while ((read = input.read(data)) != -1) {
          output.write(data, 0, read);
        }
      }
      return fname;
    }
    catch (NullPointerException e)
    {
      File outputFile;
      Log.d("HelpShiftDebug", "NPE", e);
      return null;
    } finally {
      if (output != null)
        output.close();
      if (input != null)
        input.close();
    }
  }

  public static HSMsg addAndGetLocalRscMsg(HSStorage storage, String issueId, String screenshot)
  {
    return addAndGetLocalRscMsg(storage, issueId, screenshot, false);
  }

  public static HSMsg addAndGetLocalRscMsg(HSStorage storage, String issueId, String screenshot, boolean inProgress)
  {
    HSMsg msg = null;
    try {
      String messageId = "localRscMessage_" + UUID.randomUUID().toString();
      String date = resolveTimestamp(storage.getIssue(issueId));

      String body = "Attaching Screenshot...";

      JSONObject message = new JSONObject();
      message.put("id", messageId);
      message.put("issue_id", issueId);
      message.put("type", "rsc");
      message.put("origin", "admin");
      message.put("body", body);
      message.put("invisible", false);
      message.put("screenshot", screenshot);
      message.put("state", 0);
      message.put("inProgress", inProgress);
      message.put("created_at", date);
      message.put("seen", true);

      JSONArray messages = new JSONArray();
      messages.put(message);
      storage.storeMessages(issueId, messages);

      msg = new HSMsg(messageId, "rsc", "admin", body, date, Boolean.valueOf(false), screenshot, 0, Boolean.valueOf(false));
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "addAndGetLocalRscMessage", e);
    }

    return msg;
  }

  private static String resolveTimestamp(JSONObject issue) {
    Date localTs = new Date();
    try
    {
      JSONArray messagesArray = issue.getJSONArray("messages");
      JSONObject lastMessage = messagesArray.getJSONObject(messagesArray.length() - 1);
      Date lastMessageTs = HSFormat.issueTsFormat.parse(lastMessage.getString("created_at"));

      if (localTs.before(lastMessageTs))
      {
        localTs.setTime(lastMessageTs.getTime() + 3000L);
      }
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "resolveDate", e);
    } catch (ParseException e) {
      Log.d("HelpShiftDebug", "resolveDate", e);
    }

    return HSFormat.issueTsFormat.format(localTs);
  }
}