package com.elex.chatservice.util;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.List;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.UserManager;

public class TranslateUtil {
	
	public static void loadTranslate(final String text,final TranslateListener translateListener)
	{
		final Handler handler = new Handler(){  
		    @Override  
		    public void handleMessage(Message msg) {  
		        super.handleMessage(msg);  
		        Bundle data = msg.getData();  
		        String translateMsg = data.getString("translateMsg");
		        String originalLang = data.getString("originalLang");
//		        System.out.println("translateMsg:"+translateMsg+"  originalLang:"+originalLang);
		        if(translateListener!=null)
		        {
		        	translateListener.onTranslateFinish(translateMsg,originalLang);
		        }
		        
		    }  
		};  
		  
		Runnable runnable = new Runnable(){  
		    @Override  
		    public void run() {  
//		    	System.out.println("loadTranslate 1");
		        String ret= translate(text);
//		        System.out.println("ret:"+ret);
		        try {
					TranslateParam param=JSON.parseObject(ret,TranslateParam.class);
//					System.out.println(param);
					
					String translateMsg=param.getTranslatedMsg();
					String originalLang=param.getSrc();
					Message msg = new Message();  
					Bundle data = new Bundle();  
					data.putString("translateMsg",translateMsg);  
					data.putString("originalLang",originalLang); 
					msg.setData(data);  
					handler.sendMessage(msg);
				} catch (Exception e) {
					LogUtil.trackMessage("JSON.parseObject exception on server" + UserManager.getInstance().getCurrentUser().serverId, "", "");
				}  
		    }  
		}; 
		new Thread(runnable).start();
	}
	
	// 将文本进行URL编码
	private static String encodeText(String text){
		String str=text;
		try {
			str = URLEncoder.encode(text,"UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}  
		return str;
	}
	
	public static String translate(String text) {
		InputStream inputStream=null;
		ByteArrayOutputStream output=new ByteArrayOutputStream();
		BufferedInputStream bufferInputStream=null;
//		String engineUrl ="https://translate.google.com/translate_a/single?client=t&sl=auto&tl="+ConfigManager.getInstance().gameLang+"&dt=t&ie=UTF-8&oe=UTF-8&source=btn&q=";
		String engineUrl2 ="http://translate.google.com/translate_a/t?client=te&text=";
		String langParam="&sl=auto&tl="+ConfigManager.getInstance().gameLang+"&ie=UTF-8&oe=UTF-8";
		
//		System.out.println("translate engineUrl:"+engineUrl);
//		System.out.println("translate engineUrl2:"+engineUrl2);
		String urlstr=engineUrl2 + encodeText(text)+langParam;
		URL url = null;
		String res="";
		HttpURLConnection connection = null;
		try {
			url = new URL(urlstr);
//			System.out.println(url);
			connection = (HttpURLConnection)url.openConnection();
			String userAgent="Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/534.24 (KHTML, like Gecko) Chrome/11.0.696.71 Safari/534.24";
			long randomType=Math.round(Math.random()*3+1);
			switch((int)randomType)
			{
			case 1:
				userAgent="Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/534.24 (KHTML, like Gecko) Chrome/11.0.696.71 Safari/534.24";
				break;
			case 2:
				userAgent="Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2) Gecko/20100115 Firefox/3.6";
				break;
			case 3:
				userAgent="Mozilla/5.0 (Linux; U; Android 4.1.1; fr-fr; MB525 Build/JRO03H; CyanogenMod-10) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30";
				break;
			case 4:
				userAgent="Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_4) AppleWebKit/534.30 (KHTML, like Gecko) Chrome/12.0.742.100 Safari/534.30";
				break;
			default:
				userAgent="Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2) Gecko/20100115 Firefox/3.6";
				break;
					
			}
//			System.out.println("userAgent:"+userAgent);
			connection.setRequestProperty("User-Agent", userAgent);
			connection.connect();
			inputStream = connection.getInputStream();
			bufferInputStream = new BufferedInputStream(inputStream, 4096);
			int i = -1;
			byte buf[] = new byte[4 * 1024];
			while ((i = bufferInputStream.read(buf)) != -1)
			{
				output.write(buf, 0, i);
			}
			res =new String(output.toByteArray(),"UTF-8");
			inputStream.close();
			output.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
//		System.out.println(res);
		return res;
	}
	
	public static String getOriginalLang(String text)
	{
		if(text.lastIndexOf(",,,,")<4|| text.lastIndexOf("\"]],,")<0 || text.lastIndexOf(",,[[\"")<0 || text.lastIndexOf("\"],,")<0 || text.lastIndexOf(",,[[\"")+5>text.lastIndexOf("\"],,"))
			return "";
		String ret=text.substring(text.lastIndexOf(",,[[\"")+5, text.lastIndexOf("\"],,"));
		if(ret.equals("zh-CN"))
			ret="zh_CN";
		else if(ret.equals("zh-TW"))
			ret="zh_TW";
		return ret;
	}
	
	public static String getTranslateText(String text)
	{
		if(text.lastIndexOf(",,,,")<4)
			return text;
		String temp=text.substring(4, text.lastIndexOf(",,,,"));
//		System.out.println("temp:"+temp);
		if(temp.lastIndexOf("\"]],,")<0)
			return text;
		temp=temp.substring(0, temp.lastIndexOf("\"]],,"));
//		System.out.println("test2:"+temp);
		String replaceStr=TranslateUtil.class.getSimpleName()+System.currentTimeMillis();
//		System.out.println("replaceStr:"+replaceStr);
		String temp2=temp.replace("\"],[\"",replaceStr);
//		System.out.println("temp2:"+temp2);
		String[] testArr=temp2.split(replaceStr);
		String result="";
		for(int i=0;i<testArr.length;i++)
		{
			if(i<testArr.length-1 && testArr[i].endsWith("\\"))
				testArr[i]+="\"],[\"";
			else
			{
				if(testArr[i].indexOf("\",\"")<0)
					return text;
				testArr[i]=testArr[i].substring(0, testArr[i].indexOf("\",\""));
			}
			testArr[i]=testArr[i].replace("\\\"", "\"");
//			System.out.println("testArr["+i+"]"+testArr[i]);
			
			result+=testArr[i];
		}
//		System.out.println("result:"+result);
		return decodeUnicode(result).toString();
	}
	
	public static StringBuffer decodeUnicode(final String dataStr) {
        final StringBuffer buffer = new StringBuffer();
        String tempStr = "";
        String operStr = dataStr;
       
        if (operStr != null && operStr.indexOf("\\u") == -1)
                return buffer.append(operStr); //
        if (operStr != null && !operStr.equals("") && !operStr.startsWith("\\u")) { //
                tempStr = operStr.substring(0, operStr.indexOf("\\u")); //  
                operStr = operStr.substring(operStr.indexOf("\\u"),    operStr.length());// operStr字符一定是以unicode编码字符打头的字符串
        }
        buffer.append(tempStr);
        while (operStr != null && !operStr.equals("") && operStr.startsWith("\\u")) { // 循环处理,处理对象一定是以unicode编码字符打头的字符串
                tempStr = operStr.substring(0, 6);
                operStr = operStr.substring(6, operStr.length());
                String charStr = "";
                charStr = tempStr.substring(2, tempStr.length());
                char letter = (char) Integer.parseInt(charStr, 16); // 16进制parse整形字符串。
                buffer.append(new Character(letter).toString());
                if (operStr.indexOf("\\u") == -1) { //  
                        buffer.append(operStr);
                } else { // 处理operStr使其打头字符为unicode字符
                        tempStr = operStr.substring(0, operStr.indexOf("\\u"));
                        operStr = operStr.substring(operStr.indexOf("\\u"), operStr.length());
                        buffer.append(tempStr);
                }
        }
        return buffer;
}
}
