package com.example.sample1;

import java.io.File;

import android.util.Log;
import android.webkit.CookieManager;
import android.webkit.CookieSyncManager;
import android.webkit.WebView;

public class WebManager
{
    private static final String TAG = WrapperChatServiceMainActivity.class.getSimpleName();
    private static final String APP_CACAHE_DIRNAME = "/webcache";
    
    WrapperChatServiceMainActivity context;
    
    /**
     * 清除WebView缓存
     */
	public void clearWebViewCache()
	{
		WebView webView = new WebView(context);
		webView.clearCache(true);
		webView.clearHistory();
		CookieSyncManager.createInstance(context);
		CookieSyncManager.getInstance().startSync();
		CookieManager.getInstance().removeSessionCookie();

		// 清理Webview缓存数据库
		try
		{
			context.deleteDatabase("webview.db");
			context.deleteDatabase("webviewCache.db");
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}

		// WebView 缓存文件
		File appCacheDir = new File(context.getFilesDir().getAbsolutePath() + APP_CACAHE_DIRNAME);
		Log.e(TAG, "appCacheDir path=" + appCacheDir.getAbsolutePath());

		File webviewCacheDir = new File(context.getCacheDir().getAbsolutePath() + "/webviewCache");
		Log.e(TAG, "webviewCacheDir path=" + webviewCacheDir.getAbsolutePath());

		// 删除webview 缓存目录
		if (webviewCacheDir.exists())
		{
			deleteFile(webviewCacheDir);
		}
		// 删除webview 缓存 缓存目录
		if (appCacheDir.exists())
		{
			deleteFile(appCacheDir);
		}
	}

	/**
	 * 递归删除 文件/文件夹
	 *
	 * @param file
	 */
	public void deleteFile(File file)
	{
		Log.i(TAG, "delete file path=" + file.getAbsolutePath());
		if (file.exists())
		{
			if (file.isFile())
			{
				file.delete();
			}
			else if (file.isDirectory())
			{
				File files[] = file.listFiles();
				for (int i = 0; i < files.length; i++)
				{
					deleteFile(files[i]);
				}
			}
			file.delete();
		}
		else
		{
			Log.e(TAG, "delete file no exists " + file.getAbsolutePath());
		}
	}
}
