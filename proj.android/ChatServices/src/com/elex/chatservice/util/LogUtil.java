package com.elex.chatservice.util;

import java.io.UnsupportedEncodingException;
import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.commons.lang.StringUtils;

public class LogUtil
{
	public static IAnalyticTracker tracker;
	
	public static void printStack()
	{
		try{
			String a = null;
			a.trim();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}
	
	public static void printException(Exception e)
	{
		String function = "";
		String cause = "";
		String message = "";
		if (e.getCause() != null){
			System.out.println("cause:" + e.getCause().getMessage());
			cause = e.getCause().getMessage();
		}
		if (e.getMessage() != null){
			System.out.println("message:" + e.getMessage());
			message = e.getMessage();
		}

		e.printStackTrace();

		StackTraceElement[] stack = e.getStackTrace();
		if (stack != null) System.out.println("stack:");
		for (int i = 0; i < stack.length; i++)
		{
			StackTraceElement stackTraceElement = stack[i];
			System.out.println(stackTraceElement.toString());
			if(i == 0){
				function = stackTraceElement.toString(); 
			}
		}
		
		if(tracker != null){
			tracker.trackException("JavaCatched", function, cause, message);
		}
	}
	
	public static void trackMessage(String function, String cause, String message)
	{
		if(tracker != null){
			tracker.trackException("JavaCatched", function, cause, message);
		}
	}

	public static String typeToString(Object obj)
	{
		return typeToString("", obj, false);
	}
	
	/**
	 * 用法：System.out.println(LogUtil.typeToString("变量名", object));
	 * 
	 * The typeToString() method is used to dump the contents of a passed object of any type (or collection) to a String. This can be very
	 * useful for debugging code that manipulates complex structures.
	 * 
	 * @param scope
	 * @param obj
	 * 
	 * @return String
	 * 
	 */
	@SuppressWarnings("rawtypes")
	private static String typeToString(String scope, Object obj, boolean isCompare)
	{
		if (obj == null)
		{
			return (scope + ": null\n");
		}
		else if (isCollectionType(obj))
		{
			// 集合类型
			return collectionTypeToString(scope, obj, new ArrayList(), isCompare);
		}
		else if (isComplexType(obj))
		{
			// 自定义类型
			return "[" + getCompactObjectId(obj) + "] " + "\n" + complexTypeToString(scope, obj, new ArrayList(), isCompare);
		}
		else
		{
			// 基本类型
			return (scope + ": " + obj.toString() + "\n\r");
		}
	}
	
	public static String getCompactObjectId(Object obj)
	{
		int index = obj.toString().lastIndexOf(".");
		if(index < 0){
			return obj.toString();
		}else{
			return obj.toString().substring(index + 1);
		}
	}

	public static String compareObjects(Object[] objs)
	{
		int objCount = objs.length;
		if(objCount == 0) return "";
		if(objCount == 1) return typeToString("", objs[0], true);
		if(!isSameType(objs)) return "compareObjects: objects are not the same type";
		
		String result = "[compareObjects]" + "\n";
		String objectLine = String.format("%-5s%s", "", "Field Name");
		String[] objectStrings = new String[objCount];
		
		for (int i = 0; i < objCount; i++)
		{
			objectStrings[i] = typeToString("", objs[i], true);
			objectLine = String.format("%-"+40*(i+1)+"s%s", objectLine, getCompactObjectId(objs[i]));
		}
		result += objectLine + "\n";
		
		int lineCount = objectStrings[0].split("\n").length;
		
		for (int i = 1; i < lineCount; i++)
		{
			String line = "";
			for (int j = 0; j < objCount; j++)
			{
				if(j == 0){
					String columnName = parseLine(getFieldStringAt(objectStrings[j], i))[0];
					line = String.format("%-5s%s", "", columnName + ":");
				}
				String columnValue = parseLine(getFieldStringAt(objectStrings[j], i))[1];
//				line = String.format("%-"+40*(j+1)+"s%s", line, columnValue);
				
				// 针对中文对齐问题的处理
				try
				{
			        byte[] buff = columnValue.getBytes("GBK");
			        // System.out.println(buff.length);//4,汉字在GBK里面刚好是4个字节。
			        String tmp = new String(buff,"ISO-8859-1");//把4个字节的字节数组按ISO方式转化成字符串
					line = String.format("%-"+40*(j+1)+"s%s", line, tmp);
				}
				catch (UnsupportedEncodingException e)
				{
					printException(e);
				}
			}
			
			try
			{
				line = new String(line.getBytes("ISO-8859-1"),"GBK");//先按ISO的方式还原出字节数组来，然后再按GBK编码成字符串。
			}
			catch (UnsupportedEncodingException e)
			{
				printException(e);
			}
			
			result += line + "\n";
		}

		return result;
	}
	
	private static boolean isSameType(Object[] objs)
	{
		String type = objs[0].getClass().getName();
		for (int i = 1; i < objs.length; i++)
		{
			if(!type.equals(objs[i].getClass().getName()))
				return false;
		}
		return true;
	}
	
	private static String getFieldStringAt(String total, int i)
	{
		String[] lines = total.split("\n");
		if(i >= lines.length){
			return "";
		}
		return lines[i];
	}

	private static final String	LINE_INDENT	= "		";
	private static String[] parseLine(String str)
	{
		String temp = str.replaceAll(LINE_INDENT, "");
		// 可能value为空
		if(temp.split(": ").length == 1){
			return new String[]{temp.split(": ")[0], ""};
		}else{
			return temp.split(": ");
		}
	}

	/**
	 * Returns a string holding the contents of the passed object,
	 * 
	 * @param scope
	 *            String
	 * @param parentObject
	 *            Object
	 * @param visitedObjs
	 *            List
	 * @return String
	 */
	@SuppressWarnings({ "rawtypes" })
	private static String complexTypeToString(String scope, Object parentObject, List visitedObjs, boolean isCompare)
	{
		StringBuffer buffer = new StringBuffer("");
		try
		{
			//
			// Ok, now we need to reflect into the object and add its child
			// nodes...
			//
			Class cl = parentObject.getClass();
			while (cl != null)
			{

				processFields(cl.getDeclaredFields(), scope, parentObject, buffer, visitedObjs, isCompare);

				cl = cl.getSuperclass();
			}
		}
		catch (IllegalAccessException iae)
		{
			buffer.append(iae.toString());
		}

		return (buffer.toString());
	}

	/**
	 * Method processFields
	 * 
	 * @param fields
	 *            Field[]
	 * @param scope
	 *            String
	 * @param parentObject
	 *            Object
	 * @param buffer
	 *            StringBuffer
	 * @param visitedObjs
	 *            List
	 * @throws IllegalAccessException
	 */
	@SuppressWarnings({ "rawtypes" })
	private static void processFields(Field[] fields, String scope, Object parentObject, StringBuffer buffer, List visitedObjs, boolean isCompare)
			throws IllegalAccessException
	{
		for (int i = 0; i < fields.length; i++)
		{
			//
			// Disregard certain fields for IDL structures
			//
			if (fields[i].getName().equals("__discriminator") || fields[i].getName().equals("__uninitialized"))
			{
				continue;
			}

			//
			// This allows us to see non-public fields. We might need to deal
			// with some
			// SecurityManager issues here once it is outside of VAJ...
			//
			fields[i].setAccessible(true);
			if (Modifier.isStatic(fields[i].getModifiers()))
			{
				//
				// Ignore all static members. The classes that this dehydrator
				// is
				// meant to handle are simple data objects, so static members
				// have no
				// bearing....
				//
			}
			else
			{
				if(StringUtils.isEmpty(scope)){
					if(isCompare){
						buffer.append(typeToString(LINE_INDENT + fields[i].getName(), fields[i].get(parentObject), visitedObjs, isCompare));
					}else{
						String line = typeToString(fields[i].getName(), fields[i].get(parentObject), visitedObjs, isCompare);
						line = line.replaceAll("\n", "");
						String objectLine = String.format("%-5s%s", "", parseLine(line)[0] + ":");
						objectLine = String.format("%-"+40+"s%s", objectLine, parseLine(line)[1] + "\n");
						
						buffer.append(objectLine);
					}
				}else{
					buffer.append(typeToString("	" + scope + "." + fields[i].getName(), fields[i].get(parentObject), visitedObjs, isCompare));
				}
			}
		}
	}

	/**
	 * Method isCollectionType
	 * 
	 * @param obj
	 *            Object
	 * @return boolean
	 */
	private static boolean isCollectionType(Object obj)
	{
		return (obj.getClass().isArray() || (obj instanceof Collection) || (obj instanceof Hashtable) || (obj instanceof HashMap)
				|| (obj instanceof HashSet) || (obj instanceof List) || (obj instanceof AbstractMap));
	}

	/**
	 * Method isComplexType
	 * 
	 * @param obj
	 *            Object
	 * @return boolean
	 */
	@SuppressWarnings("rawtypes")
	private static boolean isComplexType(Object obj)
	{
		if (obj instanceof Boolean || obj instanceof Short || obj instanceof Byte || obj instanceof Integer
				|| obj instanceof Long || obj instanceof Float || obj instanceof Character || obj instanceof Double
				|| obj instanceof String)
		{

			return false;
		}
		else
		{
			Class objectClass = obj.getClass();

			if (objectClass == boolean.class || objectClass == Boolean.class || objectClass == short.class
					|| objectClass == Short.class || objectClass == byte.class || objectClass == Byte.class
					|| objectClass == int.class || objectClass == Integer.class || objectClass == long.class
					|| objectClass == Long.class || objectClass == float.class || objectClass == Float.class
					|| objectClass == char.class || objectClass == Character.class || objectClass == double.class
					|| objectClass == Double.class || objectClass == String.class)
			{
				return false;
			}

			else
			{
				return true;
			}
		}
	}

	/**
	 * Returns a string holding the contents of the passed object,
	 * 
	 * @param scope
	 *            String
	 * @param obj
	 *            Object
	 * @param visitedObjs
	 *            List
	 * @return String
	 */
	@SuppressWarnings({ "rawtypes", "unused" })
	private static String collectionTypeToString(String scope, Object obj, List visitedObjs, boolean isCompare)
	{
		if(obj == null) return "";
		
		StringBuffer buffer = new StringBuffer("");
		if (obj.getClass().isArray())
		{
			if (Array.getLength(obj) > 0)
			{
				for (int j = 0; j < Array.getLength(obj); j++)
				{
					Object x = Array.get(obj, j);
					buffer.append(typeToString(scope + "[" + j + "]", x, visitedObjs, isCompare));
				}
			}
			else
			{
				buffer.append(scope + "[]: empty\n");
			}
		}
		else
		{
			boolean isCollection = (obj instanceof Collection);
			boolean isHashTable = (obj instanceof Hashtable);
			boolean isHashMap = (obj instanceof HashMap);
			boolean isHashSet = (obj instanceof HashSet);
			boolean isAbstractMap = (obj instanceof AbstractMap);
			boolean isMap = isAbstractMap || isHashMap || isHashTable;
			if (isMap)
			{
				Set keySet = ((Map) obj).keySet();
				Iterator iterator = keySet.iterator();
				int size = keySet.size();
				if (size > 0)
				{
					for (int j = 0; iterator.hasNext(); j++)
					{
						Object key = iterator.next();
						Object x = ((Map) obj).get(key);
						buffer.append(typeToString(scope + "[" + key + "]", x, visitedObjs, isCompare));
					}
				}
				else
				{
					buffer.append(scope + "[]: empty\n");
				}
			}
			else if (/* isHashTable || */
			isCollection || isHashSet /* || isHashMap */
			)
			{
				Iterator iterator = null;
				int size = 0;
				if (obj != null)
				{
					if (isCollection)
					{
						iterator = ((Collection) obj).iterator();
						size = ((Collection) obj).size();
					}
					else if (isHashTable)
					{
						iterator = ((Hashtable) obj).values().iterator();
						size = ((Hashtable) obj).size();
					}
					else if (isHashSet)
					{
						iterator = ((HashSet) obj).iterator();
						size = ((HashSet) obj).size();
					}
					else if (isHashMap)
					{
						iterator = ((HashMap) obj).values().iterator();
						size = ((HashMap) obj).size();
					}
					if (size > 0)
					{
						for (int j = 0; iterator.hasNext(); j++)
						{
							Object x = iterator.next();
							buffer.append(typeToString(scope + "[" + j + "]", x, visitedObjs, isCompare));
						}
					}
					else
					{
						buffer.append(scope + "[]: empty\n");
					}
				}
				else
				{
					//
					// theObject is null
					buffer.append(scope + "[]: null\n");
				}
			}
		}
		return (buffer.toString());
	}

	/**
	 * Method typeToString
	 * 
	 * @param scope
	 *            String
	 * @param obj
	 *            Object
	 * @param visitedObjs
	 *            List
	 * @return String
	 */
	@SuppressWarnings({ "rawtypes", "unchecked" })
	private static String typeToString(String scope, Object obj, List visitedObjs, boolean isCompare)
	{
		if (obj == null)
		{
			return (scope + ": null\n");
		}
		else if (isCollectionType(obj))
		{
			return collectionTypeToString(scope, obj, visitedObjs, isCompare);
		}
		else if (isComplexType(obj))
		{
//			if (!visitedObjs.contains(obj))
//			{
//				visitedObjs.add(obj);
//				return complexTypeToString(scope, obj, visitedObjs, isCompare);
//			}
//			else
//			{
//				return (scope + ": <already visited>\n");
//			}
			
			// 复合类中的子复合类对象只输出简单信息，以免比较不方便（一个为null，一个有内容时，格式错乱）
			return (scope + ": " + getCompactObjectId(obj) + "\n");
		}
		else
		{
//			String objectLine = String.format("%-5s%s", "", scope + ":");
//			objectLine = String.format("%-"+40+"s%s", objectLine, obj.toString() + "\n");
//			return objectLine;
			
			return (scope + ": " + obj.toString() + "\n");
		}
	}
}
