package com.elex.chatservice.image;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class AsyncImageLoader
{
	private final static int		HTTP_STATE_OK	= 200;
	private final static int		BUFFER_SIZE		= 1024 * 4;
	private final static int		DEFAULT_TIMEOUT	= 30 * 1000;

	private static ImageStoreCache	mImageCache		= null;
	private static ExecutorService	executorService	= null;

	private static AsyncImageLoader	mInstance		= null;

	private AsyncImageLoader()
	{
		mImageCache = new ImageStoreCache(16);
		executorService = Executors.newFixedThreadPool(5);
	}

	public static AsyncImageLoader getInstance()
	{
		if (mInstance == null)
		{
			synchronized (AsyncImageLoader.class)
			{
				if (mInstance == null)
					mInstance = new AsyncImageLoader();
			}
		}
		return mInstance;
	}
	
	public boolean isCacheExistForKey(String key)
	{
		return mImageCache.containsKey(key);
	}

	public Bitmap loadBitmapFromCache(String key)
	{
		return mImageCache.get(key);
	}

	public static void removeMemoryCache(String key)
	{
		mImageCache.removeMemoryCache(key);
	}
	
	class ImageLoaderRunnable implements Runnable
	{
		private String	url;
		private String	localUrl;
		private Handler	handler;
		private boolean isFromHttp;

		ImageLoaderRunnable(String localUrl, Handler handler)
		{
			this.url = "";
			this.localUrl = localUrl;
			this.handler = handler;
			this.isFromHttp = false;
		}
		
		ImageLoaderRunnable(String url, String localUrl, Handler handler)
		{
			this.url = url;
			this.localUrl = localUrl;
			this.handler = handler;
			this.isFromHttp = true;
		}

		@Override
		public void run()
		{
			Bitmap ret = mImageCache.getFromLocalPath(localUrl);
			if(isFromHttp && ret == null)
			{
				ret = mImageCache.cache(localUrl, getHttpBitmap(url, DEFAULT_TIMEOUT));
			}
			Message message = handler.obtainMessage(0, ret);
			message.sendToTarget();
		}

	}

	public void loadBitmapFromStore(final String localUrl, final ImageLoaderListener listener)
	{
		Handler handler = new Handler()
		{
			public void handleMessage(Message message)
			{
				if (listener != null)
				{
					listener.onImageLoaded((Bitmap) message.obj);
				}
			}
		};

		if (executorService != null)
			executorService.submit(new ImageLoaderRunnable(localUrl, handler));
		
//		final Handler handler = new Handler()
//		{
//			public void handleMessage(Message message)
//			{
//				if (listener != null)
//				{
//					listener.onImageLoaded(key, (Bitmap) message.obj);
//				}
//			}
//		};
//
//		new Thread(new Runnable()
//		{
//			@Override
//			public void run()
//			{
//				Bitmap ret = mImageCache.getFromLocalPath(key);
//				if (ret != null)
//				{
//					Message message = handler.obtainMessage(0, ret);
//					message.sendToTarget();
//				}
//			}
//		}).start();
	}

	public void loadBitmapFromUrl(final String url, final String localUrl, final ImageLoaderListener listener)
	{
		Handler handler = new Handler()
		{
			public void handleMessage(Message message)
			{
				if (listener != null)
				{
					listener.onImageLoaded((Bitmap) message.obj);
				}
			}
		};

		if (executorService != null)
			executorService.submit(new ImageLoaderRunnable(url, localUrl, handler));
		// new Thread(new Runnable()
		// {
		// @Override
		// public void run()
		// {
		//
		// Bitmap ret = mImageCache.getFromPath(localUrl);
		// if (ret == null)
		// ret = mImageCache.cache(localUrl, getHttpBitmap(url,
		// DEFAULT_TIMEOUT));
		// Message message = handler.obtainMessage(0, ret);
		// message.sendToTarget();
		// }
		// }).start();
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
