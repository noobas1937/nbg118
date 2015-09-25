package com.elex.chatservice.util;

import java.util.ArrayList;
import java.util.Iterator;
import org.json.JSONObject;

public class HSJSONUtils
{
  public static String[] getJSONObjectKeys(JSONObject inputObject)
  {
    Iterator keys = inputObject.keys();
    ArrayList objectKeys = new ArrayList();

    while ((keys != null) && (keys.hasNext())) {
      objectKeys.add((String)keys.next());
    }
    String[] returnArray = new String[objectKeys.size()];
    return (String[])objectKeys.toArray(returnArray);
  }
}