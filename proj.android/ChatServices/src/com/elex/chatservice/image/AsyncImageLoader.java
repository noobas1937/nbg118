package com.elex.chatservice.image;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;


public class AsyncImageLoader
{
	private final static int HTTP_STATE_OK = 200;
	private final static int BUFFER_SIZE = 1024 * 4;
	private final static int DEFAULT_TIMEOUT = 30 * 1000;

	private static ImageStoreCache mImageCache = new ImageStoreCache(16);

	private AsyncImageLoader()
	{
	}

	public static Bitmap loadBitmapFromStoreSync(String key)
	{
		return mImageCache.getFromPath(key);
	}

	public static void removeMemoryCache(String key)
	{
		mImageCache.removeMemoryCache(key);
	}

	public static void loadBitmapFromStore(final String key,
			final ImageLoaderListener listener,Context context)
	{
		final Handler handler = new Handler()
		{
			public void handleMessage(Message message)
			{
				if (listener != null)
				{
					listener.onImageLoaded(key, (Bitmap) message.obj);
				}
			}
		};

		new Thread(new Runnable()
		{

			@Override
			public void run()
			{
				// TODO Auto-generated method stub
				Bitmap ret = mImageCache.get(key);
				if (ret != null)
				{
					Message message = handler.obtainMessage(0, ret);
					message.sendToTarget();
				}
			}
		}).start();
	}

	public static void loadBitmapFromUrl(final String url,final String localUrl,
			final ImageLoaderListener listener)
	{
		final Handler handler = new Handler()
		{
			public void handleMessage(Message message)
			{
				if (listener != null)
				{
//					System.out.println("loadBitmapFromUrl localUrl:"+localUrl);
					listener.onImageLoaded(url, (Bitmap) message.obj);
				}
			}
		};
		
		new Thread(new Runnable()
		{
			@Override
			public void run()
			{
				
				// TODO Auto-generated method stub
				Bitmap ret = mImageCache.get(localUrl);
				if (ret == null)
				{
					ret = mImageCache.cache(localUrl,
							getHttpBitmap(url, DEFAULT_TIMEOUT));
					Message message = handler.obtainMessage(0, ret);
					message.sendToTarget();
				}
			}
		}).start();
	}

	protected static byte[] getHttpBitmap(String bitmapPath, int timeout)
	{
		InputStream is = null;
		HttpURLConnection conn = null;
		BufferedInputStream bis = null;
		ByteArrayOutputStream out = null;
		try
		{
			out = new ByteArrayOutputStream();
			URL url = new URL(bitmapPath);
			conn = (HttpURLConnection) url.openConnection();
			if (timeout > 0)
			{
				conn.setConnectTimeout(timeout);
				conn.setReadTimeout(timeout);
			}
			conn.setRequestProperty("Connection", "close");
			conn.connect();
			is = conn.getInputStream();
			url = null;
			if (conn.getResponseCode() == HTTP_STATE_OK)
			{
				bis = new BufferedInputStream(is, BUFFER_SIZE);
				int i = -1;
				byte buf[] = new byte[4 * 1024];
				while ((i = bis.read(buf)) != -1)
				{
					out.write(buf, 0, i);
				}
				byte imgData[] = out.toByteArray();

				return imgData;
			}
		}
		catch (Exception e)
		{
			Log.e("getHttpBitmap", "load image from url failed");
		}
		finally
		{
			try
			{
				if (is != null)
					is.close();
				if (bis != null)
					bis.close();
				if (out != null)
					out.close();
				if (conn != null)
					conn.disconnect();
				is = null;
				bis = null;
				out = null;
				conn = null;
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
		}
		return null;
	}
}
