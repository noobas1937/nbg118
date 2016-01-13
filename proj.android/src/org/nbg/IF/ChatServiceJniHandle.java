package org.nbg.IF;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.concurrent.Callable;
import java.util.concurrent.FutureTask;

public class ChatServiceJniHandle
{
	private static Object	mJniObj	= null;

	private static Object getJniInstance()
	{
		if (mJniObj == null)
		{
			synchronized (ChatServiceJniHandle.class)
			{
				if (mJniObj == null)
				{
					try
					{
						Class cls = Class.forName("com.elex.chatservice.host.GameHost");
						if (cls != null)
						{
							Constructor constructor = cls.getConstructor(null);
							mJniObj = constructor.newInstance(null);
						}
					}
					catch (Exception e)
					{
						e.printStackTrace();
					}
				}
			}
		}
		return mJniObj;
	}

	public static <T> T excuteJNIMethod(final String methodName, final Object[] params)
	{
		// System.out.println("excuteJNIMethod methodName:"+methodName);
		try
		{
			Class cls = Class.forName("com.elex.chatservice.host.GameHost");
			if (cls != null)
			{
				Class[] methodCls = getMethodTypeClass(params);
				final Method method = cls.getMethod(methodName, methodCls);
				if (method != null)
				{
					// System.out.println("method!=null");
					FutureTask<T> futureTask = new FutureTask<T>(new Callable<T>()
					{

						@Override
						public T call() throws Exception
						{
							// System.out.println("excuteJNIMethod futureTask:");
							try
							{
								return (T) method.invoke(getJniInstance(), params);
							}
							catch (Exception e)
							{
								e.printStackTrace();
							}
							return null;
						}
					});

					IF.getInstance().runOnGLThread(futureTask);

					return (T) method.invoke(getJniInstance(), params);
				}
			}

		}
		catch (Exception e)
		{
			e.printStackTrace();
		}

		return null;
	}

	public static void excuteJNIVoidMethod(final String methodName, final Object[] params)
	{
		// System.out.println("excuteJNIVoidMethod methodName:"+methodName);
		IF.getInstance().runOnGLThread(new Runnable()
		{

			@Override
			public void run()
			{
				// System.out.println("excuteJNIVoidMethod run");
				try
				{
					Class cls = Class.forName("com.elex.chatservice.host.GameHost");
					if (cls != null)
					{
						Class[] methodCls = getMethodTypeClass(params);
						final Method method = cls.getMethod(methodName, methodCls);
						if (method != null)
						{
							// System.out.println("method!=null");
							method.invoke(getJniInstance(), params);
						}
					}
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}
			}
		});

	}

	private static Class[] getMethodTypeClass(String[] types)
	{
		if (types == null || types.length == 0)
			return null;
		Class[] classes = new Class[types.length];
		for (int i = 0; i < classes.length; i++)
		{
			String type = types[i];
			if (type != null && !type.trim().equals(""))
			{
				if (type.equals("int") || type.equals("Integer"))
					classes[i] = Integer.TYPE;
				else if (type.equals("long") || type.equals("Long"))
					classes[i] = Long.TYPE;
				else if (type.equals("float") || type.equals("Float"))
					classes[i] = Float.TYPE;
				else if (type.equals("double") || type.equals("Double"))
					classes[i] = Double.TYPE;
				else if (type.equals("boolean") || type.equals("Boolean"))
					classes[i] = Boolean.TYPE;
				else
					classes[i] = String.class;
			}
		}
		return classes;
	}

	private static Class[] getMethodTypeClass(Object[] params)
	{
		if (params == null || params.length == 0)
			return null;
		Class[] classes = new Class[params.length];
		for (int i = 0; i < classes.length; i++)
		{
			Object param = params[i];
			if (param != null)
			{
				if(param instanceof String)
					classes[i] = String.class;
				else if(param instanceof Long)
					classes[i] = long.class;
				else if(param instanceof Float)
					classes[i] = float.class;
				else if(param instanceof Integer)
					classes[i] = int.class;
				else if(param instanceof Boolean)
					classes[i] = boolean.class;
				else if(param instanceof Double)
					classes[i] = double.class;
			}
		}
		return classes;
	}

	private static Object[] getMethodParams(String[] types, String[] params)
	{
		if (types == null || params == null || params.length == 0)
			return null;
		Object[] paramsObj = new Object[params.length];
		for (int i = 0; i < paramsObj.length; i++)
		{
			String param = params[i];
			String type = types[i];
			if (param != null && type != null)
			{
				if (type.equals("int") || type.equals("Integer"))
					paramsObj[i] = Integer.parseInt(param);
				else if (type.equals("long") || type.equals("Long"))
					paramsObj[i] = Long.parseLong(param);
				else if (type.equals("float") || type.equals("Float"))
					paramsObj[i] = Float.parseFloat(param);
				else if (type.equals("double") || type.equals("Double"))
					paramsObj[i] = Double.parseDouble(param);
				else if (type.equals("boolean") || type.equals("Boolean"))
					paramsObj[i] = Boolean.parseBoolean(param);
				else
					paramsObj[i] = param;
			}
		}
		return paramsObj;

	}
}
