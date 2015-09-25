package com.elex.chatservice;

import com.elex.chatservice.res.values.HSConfig;

import java.util.Map;

public final class IdentityFilter
{
  private static final String TAG = "HelpShiftDebug";

  protected static boolean showNameEmailForm()
  {
    if (((Boolean)HSConfig.configData.get("rne")).booleanValue())
      return true;
    if (!((Boolean)HSConfig.configData.get("pfe")).booleanValue()) {
      return false;
    }
    return true;
  }

  protected static boolean requireEmail(HSStorage hsStorage)
  {
    if (((Boolean)HSConfig.configData.get("rne")).booleanValue())
      return true;
    if (!((Boolean)HSConfig.configData.get("pfe")).booleanValue())
      return false;
    if (hsStorage.getRequireEmail().booleanValue()) {
      return true;
    }
    return false;
  }
}