package com.elex.chatservice.util;

import java.io.UnsupportedEncodingException;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.db.DBHelper;

public class HeadPicUtil
{
	/**
	 * 形如http://cok.eleximg.com/cok/img/710001/bd65ca6d7d40cf95a3fe6b7fbb5cef7c.jpg
	 */
	public static String getCustomPicUrl(String uid, int picVer)
	{
		if (uid.length() == 0)
		{
			return "";
		}
		String url = "http://cok.eleximg.com/cok/img/";

		String md5Str = uid + "_" + picVer;

		String tempStr = uid;
		url += tempStr.substring(tempStr.length() - 6, tempStr.length()) + "/" + MD5.getMD5Str(md5Str) + ".jpg";

		return url;
	}

	public static String getCustomPic(String url)
	{
		String path = DBHelper.getHeadDirectoryPath(ChatServiceController.hostActivity);
		path += "cache_" + MD5.getMD5Str(url);
		return path;
	}

	public static class MD5
	{
		public static String getMD5Str(String str)
		{
			MessageDigest messageDigest = null;

			try
			{
				messageDigest = MessageDigest.getInstance("MD5");

				messageDigest.reset();

				messageDigest.update(str.getBytes("UTF-8"));
			}
			catch (NoSuchAlgorithmException e)
			{
				throw new RuntimeException(e);
			}
			catch (UnsupportedEncodingException e)
			{
				throw new RuntimeException(e);
			}

			byte[] byteArray = messageDigest.digest();

			StringBuffer md5StrBuff = new StringBuffer();

			for (int i = 0; i < byteArray.length; i++)
			{
				if (Integer.toHexString(0xFF & byteArray[i]).length() == 1)
					md5StrBuff.append("0").append(Integer.toHexString(0xFF & byteArray[i]));
				else
					md5StrBuff.append(Integer.toHexString(0xFF & byteArray[i]));
			}

			return md5StrBuff.toString();
		}

		public static String getMD5Str2(String str)
		{
			String hashtext = "";
			try
			{
				MessageDigest m = MessageDigest.getInstance("MD5");
				m.reset();
				m.update(str.getBytes());
				byte[] digest = m.digest();
				BigInteger bigInt = new BigInteger(1, digest);
				hashtext = bigInt.toString(16);
			}
			catch (NoSuchAlgorithmException e)
			{
				e.printStackTrace();
			}
			return hashtext;
		}
	}
}
