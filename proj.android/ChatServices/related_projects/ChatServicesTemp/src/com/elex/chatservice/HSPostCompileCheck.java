package com.elex.chatservice;

import java.io.File;
import java.io.PrintStream;
import java.lang.reflect.Field;
import java.net.URI;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;

public final class HSPostCompileCheck
{
  private static HashMap<String, List<String>> map = new HashMap();
  private static final String[] drawableList = { "hs__chat_bubble_admin", "hs__chat_bubble_user", "hs__notification_badge", "hs__action_search" };

  private static final String[] idList = { "hs__root", "hs__helpshiftActivityFooter", "hs__username", "hs__email", "hs__messageText", "hs__messagesList", "hs__sendMessageBtn", "hs__questionContent", "hs__customViewContainer", "hs__sectionFooter" };

  private static final String[] layoutList = { "hs__questions_list", "hs__question", "hs__section" };

  private static final String[] pluralsList = new String[0];
  private static final String[] stringList = new String[0];
  private static final String[] attrList = new String[0];
  private static final String[] colorList = new String[0];

  public static void main(String[] args)
    throws Exception
  {
    boolean error = false;
    File root = new File("bin/classes/");
    URLClassLoader classLoader = URLClassLoader.newInstance(new URL[] { root.toURI().toURL() });

    Class r = Class.forName(args[0] + ".R", true, classLoader);

    for (Class cls : r.getClasses())
    {
      String key = cls.getName().split("\\$")[1];
      if (map.keySet().contains(key))
      {
        for (Field field : cls.getFields()) {
          ((List)map.get(key)).remove(field.getName());
        }
      }
    }

    for (String key : map.keySet()) {
      if (((List)map.get(key)).size() != 0) {
        alert(key);
        error = true;
      }
    }

    if (error)
      System.exit(1);
    else
      System.out.println("Helpshift resources check successful.");
  }

  private static void alert(String key)
  {
    if (key == "drawable")
      System.out.println("Following drawable resources not found. Check the \"drawable/\" folder");
    else if (key == "id")
      System.out.println("View/ViewGroups with the following ids not found");
    else if (key == "layout")
      System.out.println("Following layout resources not found. Check the \"layout/\" folder.");
    else if (key == "plurals")
      System.out.println("Following plural resources not found. Check hs__strings.xml file.");
    else if (key == "string")
      System.out.println("Following string resources not found. Check hs__strings.xml file.");
    else if (key == "attr")
      System.out.println("Following style resources not found. Check hs__styles.xml file.");
    else if (key == "color") {
      System.out.println("Following color resources not found. Check hs__resources.xml file.");
    }
    System.out.println(map.get(key));
  }

  static
  {
    map.put("drawable", new LinkedList(Arrays.asList(drawableList)));
    map.put("id", new LinkedList(Arrays.asList(idList)));
    map.put("layout", new LinkedList(Arrays.asList(layoutList)));
    map.put("plurals", new LinkedList(Arrays.asList(pluralsList)));
    map.put("string", new LinkedList(Arrays.asList(stringList)));
    map.put("attr", new LinkedList(Arrays.asList(attrList)));
    map.put("color", new LinkedList(Arrays.asList(colorList)));
  }
}