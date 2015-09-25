package com.elex.chatservice.util;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.text.SimpleDateFormat;
import java.util.Locale;
import java.util.TimeZone;

public final class HSFormat
{
  private static final String TAG = "HelpShiftDebug";
  public static final SimpleDateFormat issueTsFormat = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSS'Z'");
  public static final SimpleDateFormat breadCrumbTsFormat = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSS'Z'");
  public static final SimpleDateFormat deviceInfoTsFormat = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSS'Z'");
  public static final SimpleDateFormat inputMsgFormatter = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSS'Z'");
  public static final SimpleDateFormat outputMsgFormatter = new SimpleDateFormat("dd-MMM-yyyy hh:mm a");

  public static final DecimalFormat tsSecFormatter = new DecimalFormat("0.000", new DecimalFormatSymbols(Locale.US));

  static {
    issueTsFormat.setTimeZone(TimeZone.getTimeZone("GMT"));
    inputMsgFormatter.setTimeZone(TimeZone.getTimeZone("GMT"));
  }
}