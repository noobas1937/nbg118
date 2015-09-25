package com.elex.chatservice.util;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public final class HSPattern
{
  private static Pattern specialCharPattern = Pattern.compile("\\W+");
  private static Pattern emailPattern = Pattern.compile("[a-zA-Z0-9\\+\\.\\_\\%\\-\\+]{1,256}\\@[a-zA-Z0-9][a-zA-Z0-9\\-]{0,64}(\\.[a-zA-Z0-9][a-zA-Z0-9\\-]{0,25})+");

  public static boolean checkEmail(String email)
  {
    Matcher m = emailPattern.matcher(email.trim());
    return m.matches();
  }

  public static boolean checkSpecialCharacters(String issueText) {
    Matcher m = specialCharPattern.matcher(issueText.trim());

    return m.matches();
  }
}