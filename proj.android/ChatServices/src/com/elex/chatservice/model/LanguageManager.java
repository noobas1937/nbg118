package com.elex.chatservice.model;

import java.util.HashMap;
import java.util.Map;

public class LanguageManager
{
	private static LanguagePack chatText;

	public static String getLangByKey(String key)
	{
		return chatText != null ? chatText.getTextByKey(key) : key;
	}

	public static String getLangByKey(String key, String replaceStr)
	{
		return chatText != null ? chatText.getTextByKey(key, replaceStr) : key;
	}
	
	public static String getLangByKey(String key,String replaceStr1,String replaceStr2)
	{
		return chatText != null ? chatText.getTextByKey(key, replaceStr1,replaceStr2) : key;
	}
	
	public static String getLangByKey(String key,String replaceStr1,String replaceStr2,String replaceStr3)
	{
		return chatText != null ? chatText.getTextByKey(key, replaceStr1,replaceStr2,replaceStr3) : key;
	}

	public static void initChatLanguage(LanguageItem[] chatLangArray)
	{
		chatText = new LanguagePack(chatLangArray);
	}

	private static class LanguagePack
	{
		private Map<String, String> textMap;

		public LanguagePack(LanguageItem[] chatLangArray)
		{
			System.out.println("LanguagePack start");
			textMap = new HashMap<String, String>();
			for (int i = 0; i < chatLangArray.length; i++)
			{
				textMap.put(chatLangArray[i].key, chatLangArray[i].langValue);
			}
		}

		public String getTextByKey(String key)
		{
			if (textMap == null || textMap.size() == 0)
			{
				return "";
			}
			else if (textMap.get(key) == null || textMap.get(key).equals(""))
			{
				return "";
			}
			else
			{
				return textMap.get(key);
			}
		}

		public String getTextByKey(String key, String replaceStr)
		{
			String s = getTextByKey(key);
			if (s.contains("{0}"))
			{
				s = s.replace("{0}", replaceStr);
			}
//			return "{" + replaceStr + "}";
			return s;
		}
		
		public String getTextByKey(String key, String replaceStr1,String replaceStr2)
		{
			String s = getTextByKey(key);
			if (s.contains("{0}"))
			{
				s = s.replace("{0}", replaceStr1);
			}
			if (s.contains("{1}"))
			{
				s = s.replace("{1}", replaceStr2);
			}
			return s;
		}
		
		public String getTextByKey(String key, String replaceStr1,String replaceStr2,String replaceStr3)
		{
			String s = getTextByKey(key);
			if (s.contains("{0}"))
			{
				s = s.replace("{0}", replaceStr1);
			}
			if (s.contains("{1}"))
			{
				s = s.replace("{1}", replaceStr2);
			}
			if (s.contains("{2}"))
			{
				s = s.replace("{2}", replaceStr3);
			}
			return s;
		}
	}
}
