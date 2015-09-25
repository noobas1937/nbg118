package com.elex.chatservice.viewstructs;

import android.util.Log;

import com.elex.chatservice.util.HSFormat;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

public final class HSMsg
{
  public static String TAG = "HelpShiftDebug";
  public String type;
  public String origin;
  public String body;
  public String date;
  public int state;
  public String id;
  public String screenshot;
  public Boolean clickable = Boolean.valueOf(true);
  public Boolean invisible = Boolean.valueOf(false);
  public Boolean inProgress = Boolean.valueOf(false);

  public HSMsg(String id, String type, String origin, String body, String date, Boolean invisible, String screenshot, int state, Boolean inProgress)
  {
    this.id = id;
    this.type = type;
    this.origin = origin;
    this.body = linkText(body);
    this.invisible = invisible;
    this.screenshot = screenshot;
    this.state = state;
    this.inProgress = inProgress;
    String inputDate = date;
    Date outputDate = new Date();
    try
    {
      outputDate = HSFormat.inputMsgFormatter.parse(inputDate);
    } catch (ParseException e) {
      Log.d(TAG, e.toString(), e);
    }

    this.date = HSFormat.outputMsgFormatter.format(outputDate);
  }

  private String linkText(String input) {
    String urlRegex = "(?i)(\\n|\\s|^)((?:https?://|market://|www\\d{0,3}[.]|[a-z0-9.\\-]+[.][a-z]{2,4}/)(?:[^\\s()<>]+|\\(([^\\s()<>]+|(\\([^\\s()<>]+\\)))*\\))+(?:\\(([^\\s()<>]+|(\\([^\\s()<>]+\\)))*\\)|[^\\s`!()\\[\\]{};:'\\\".,<>?«»“”‘’]))";
    String phoneRegex = "(?:[0+]?\\d[\\s-.]?\\d{0,3})?[\\s-.]?\\(?\\d{3}\\)?[\\s-.]?\\d{3}[\\s-.]?\\d{4}";

    return input.replaceAll(urlRegex, "$1<a href=\"$2\">$2</a>").replace("\n", "<br/>").replaceAll("<a href=\"(?!\\w*?://)(.*?)\">", "<a href=\"http://$1\">").replaceAll("<a href=\"(.*?)\">(https?://)?(www\\.)?([^<]{30}).*?</a>", "<a href=\"$1\">$4&#8230;</a>").replaceAll(phoneRegex, "<a href=\"tel:$0\">$0</a>");
  }
}