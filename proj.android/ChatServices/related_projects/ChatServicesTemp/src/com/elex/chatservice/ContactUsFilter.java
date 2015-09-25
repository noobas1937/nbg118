package com.elex.chatservice;

import android.content.Context;
import java.util.HashMap;

public final class ContactUsFilter
{
  private static HSStorage storage = null;
  private static Helpshift.ENABLE_CONTACT_US enableContactUs = Helpshift.ENABLE_CONTACT_US.ALWAYS;

  public static void init(Context context)
  {
    if (storage == null)
      storage = new HSStorage(context);
  }

  protected static void setConfig(HashMap configMap)
  {
    Object enableContactUsObj = configMap.get("enableContactUs");

    if ((enableContactUsObj instanceof Helpshift.ENABLE_CONTACT_US)) {
      enableContactUs = (Helpshift.ENABLE_CONTACT_US)configMap.get("enableContactUs");
    } else if ((enableContactUsObj instanceof Boolean)) {
      if (((Boolean)enableContactUsObj).booleanValue())
        enableContactUs = Helpshift.ENABLE_CONTACT_US.ALWAYS;
      else
        enableContactUs = Helpshift.ENABLE_CONTACT_US.NEVER;
    }
    else if ((enableContactUsObj instanceof String)) {
      String option = (String)enableContactUsObj;
      if (option.equals("always"))
        enableContactUs = Helpshift.ENABLE_CONTACT_US.ALWAYS;
      else if (option.equals("after_viewing_faqs"))
        enableContactUs = Helpshift.ENABLE_CONTACT_US.AFTER_VIEWING_FAQS;
      else
        enableContactUs = Helpshift.ENABLE_CONTACT_US.NEVER;
    }
  }

  protected static boolean showContactUs(LOCATION location)
  {
//    switch (1.$SwitchMap$com$helpshift$Helpshift$ENABLE_CONTACT_US[enableContactUs.ordinal()]) {
    switch (enableContactUs.ordinal()) {
    case 1:
      return true;
    case 2:
      return false;
    case 3:
//      switch (1.$SwitchMap$com$helpshift$ContactUsFilter$LOCATION[location.ordinal()]) {
      switch (location.ordinal()) {
      case 1:
        return true;
      case 2:
        return true;
      case 3:
        return true;
      case 4:
        String activeConversation = storage.getActiveConversation();
        String archivedConversation = storage.getArchivedConversation();
        if ((!activeConversation.equals("")) || (!archivedConversation.equals("")))
        {
          return true;
        }
        return false;
      }

      return true;
    }

    return true;
  }

  protected static enum LOCATION
  {
    ACTION_BAR, 
    SEARCH_FOOTER, 
    QUESTION_FOOTER, 
    QUESTION_ACTION_BAR;
  }
}