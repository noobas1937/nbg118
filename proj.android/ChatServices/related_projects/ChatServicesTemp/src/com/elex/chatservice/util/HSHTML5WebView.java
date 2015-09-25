package com.elex.chatservice.util;

import android.annotation.TargetApi;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.webkit.ConsoleMessage;
import android.webkit.GeolocationPermissions;
import android.webkit.GeolocationPermissions.Callback;
import android.webkit.WebChromeClient;
import android.webkit.WebChromeClient.CustomViewCallback;
import android.webkit.WebResourceResponse;
import android.webkit.WebSettings;
import android.webkit.WebSettings.LayoutAlgorithm;
import android.webkit.WebSettings.PluginState;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;

import com.elex.chatservice.HSQuestionFragment;
import com.elex.chatservice.app.ActionBarActivity;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

public final class HSHTML5WebView extends WebView
{
  public static final String TAG = "HelpShiftDebug";
  private Context mContext;
  private HSWebChromeClient mWebChromeClient;
  private View mCustomView;
  private FrameLayout mCustomViewContainer;
  private WebChromeClient.CustomViewCallback mCustomViewCallback;
  private FrameLayout mContentView;
  private FrameLayout mBrowserFrameLayout;
  private HSQuestionFragment mFragment;
  private ActionBarActivity mActivity;

  private void init(Context context, HSQuestionFragment fragment)
  {
    this.mFragment = fragment;
    this.mContext = context;
    this.mActivity = ((ActionBarActivity)this.mContext);

    this.mBrowserFrameLayout = ((FrameLayout)LayoutInflater.from(this.mActivity).inflate(HSRes.getId(this.mContext, "layout", "hs__webview_custom_content"), null));

    this.mContentView = ((FrameLayout)this.mBrowserFrameLayout.findViewById(HSRes.getId(this.mContext, "id", "hs__webview_main_content")));

    this.mCustomViewContainer = ((FrameLayout)this.mActivity.findViewById(HSRes.getId(this.mContext, "id", "hs__customViewContainer")));

    this.mWebChromeClient = new HSWebChromeClient();
    setWebChromeClient(this.mWebChromeClient);

    setWebViewClient(new MyWebViewClient());

    WebSettings s = getSettings();
    s.setLayoutAlgorithm(WebSettings.LayoutAlgorithm.NORMAL);
    s.setLoadWithOverviewMode(true);
    s.setSavePassword(true);
    s.setSaveFormData(true);
    s.setJavaScriptEnabled(true);

    s.setDomStorageEnabled(true);

    s.setJavaScriptEnabled(true);
    s.setAllowFileAccess(true);
    s.setAppCacheEnabled(true);
    s.setDomStorageEnabled(true);
    s.setPluginState(WebSettings.PluginState.OFF);
    s.setAllowFileAccess(true);

    this.mContentView.addView(this);
  }

  public HSHTML5WebView(Context context, HSQuestionFragment fragment) {
    super(context);
    init(context, fragment);
  }

  public FrameLayout getLayout() {
    return this.mBrowserFrameLayout;
  }

  public boolean inCustomView() {
    return this.mCustomView != null;
  }

  public void hideCustomView() {
    this.mWebChromeClient.onHideCustomView();
  }

  public boolean onKeyDown(int keyCode, KeyEvent event)
  {
    if ((keyCode == 4) && 
      (this.mCustomView == null) && (canGoBack())) {
      goBack();
      return true;
    }

    return super.onKeyDown(keyCode, event);
  }

  private boolean isImage(URL url)
  {
    Set allowedMimeTypes = new HashSet(Arrays.asList(new String[] { "image/jpeg", "image/png", "image/gif", "image/x-png", "image/x-citrix-pjpeg", "image/x-citrix-gif", "image/pjpeg" }));
    try
    {
      return allowedMimeTypes.contains(url.openConnection().getContentType());
    } catch (Exception e) {
      Log.d("HelpShiftDebug", "openConnection() Exception :", e);
    }
    return false;
  }

  private void saveFile(URL url, File saveFile) {
    try {
      InputStream input = url.openStream();
      OutputStream output = new FileOutputStream(saveFile);
      try
      {
        byte[] buffer = new byte[500];
        int bytesRead = 0;
        while ((bytesRead = input.read(buffer, 0, buffer.length)) >= 0)
          output.write(buffer, 0, bytesRead);
      }
      finally {
        output.close();
        input.close();
      }
    } catch (Exception e) {
      Log.d("HelpShiftDebug", "saveFile Exception :", e);
    }
  }

  private class MyWebViewClient extends WebViewClient
  {
    private MyWebViewClient()
    {
    }

    public void onPageFinished(WebView view, String url)
    {
      mActivity.setSupportProgressBarIndeterminateVisibility(false);
      mFragment.showMenuOptions();
    }

    public boolean shouldOverrideUrlLoading(WebView view, String url)
    {
      if (url != null) {
        view.getContext().startActivity(new Intent("android.intent.action.VIEW", Uri.parse(url)));
        return true;
      }
      return false;
    }

    @TargetApi(11)
    public WebResourceResponse shouldInterceptRequest(WebView view, String inputUrl)
    {
      URL url = null;
      File storagePath = mActivity.getExternalCacheDir();
      try
      {
        url = new URL(inputUrl);
      } catch (MalformedURLException e) {
        Log.d("HelpShiftDebug", "MalformedURLException", e);
      }

      if (url != null) {
        File saveFile = new File(storagePath, inputUrl.replace("/", "_"));

        if (saveFile.exists()) {
          try {
            return new WebResourceResponse("", "", new FileInputStream(saveFile));
          } catch (FileNotFoundException e) {
            Log.d("HelpShiftDebug", "FileNotFoundException", e);
          }
        }
        else if (isImage(url)) {
          saveFile(url, saveFile);
        }
      }

      return super.shouldInterceptRequest(view, inputUrl);
    }
  }

  private class HSWebChromeClient extends WebChromeClient
  {
    private Bitmap mDefaultVideoPoster;
    private View mVideoProgressView;

    private HSWebChromeClient()
    {
    }

    public void onShowCustomView(View view, WebChromeClient.CustomViewCallback callback)
    {
      Log.i("HelpShiftDebug", "here in on ShowCustomView");
      Log.d("HelpShiftDebug", "View's class: " + view.getClass());

      setVisibility(8);
      Log.d("HelpShiftDebug", "HSHTML5WebView child count: " + getChildCount());

      if (mCustomView != null) {
        callback.onCustomViewHidden();
        return;
      }

      mCustomViewContainer.addView(view);
      mCustomView = view;
      mCustomViewCallback = callback;
      mCustomViewContainer.setVisibility(0);
    }

    public void onHideCustomView()
    {
      Log.d("HelpShiftDebug", "onHideCustomView");
      if (mCustomView == null) {
        return;
      }

      mCustomView.setVisibility(8);

      mCustomViewContainer.removeView(mCustomView);
      mCustomView = null;
      mCustomViewContainer.setVisibility(8);
      mCustomViewCallback.onCustomViewHidden();

      setVisibility(0);

      Log.i("HelpShiftDebug", "set it to webVew");
    }

    public boolean onConsoleMessage(ConsoleMessage consoleMessage)
    {
      Log.i("HelpShiftDebug", consoleMessage.lineNumber() + " > " + consoleMessage.toString());
      return true;
    }

    public Bitmap getDefaultVideoPoster()
    {
      Log.i("HelpShiftDebug", "here in on getDefaultVideoPoster");
      if (this.mDefaultVideoPoster == null) {
        this.mDefaultVideoPoster = BitmapFactory.decodeResource(getResources(), HSRes.getId(mContext, "drawable", "default_video_poster"));
      }

      return this.mDefaultVideoPoster;
    }

    public View getVideoLoadingProgressView()
    {
      Log.i("HelpShiftDebug", "here in on getVideoLoadingPregressView");

      if (this.mVideoProgressView == null) {
        LayoutInflater inflater = LayoutInflater.from(mContext);
        this.mVideoProgressView = inflater.inflate(HSRes.getId(mContext, "layout", "hs__video_loading_progress"), null);
      }
      return this.mVideoProgressView;
    }

    public void onReceivedTitle(WebView view, String title)
    {
      Log.d("HelpShiftDebug", "onReceivedTitle");
      ((ActionBarActivity)mContext).setTitle(title);
    }

    public void onProgressChanged(WebView view, int newProgress)
    {
      Log.d("HelpShiftDebug", "onProgressChanged. progress: " + newProgress);
      ((ActionBarActivity)mContext).getWindow().setFeatureInt(2, newProgress * 100);
    }

    public void onGeolocationPermissionsShowPrompt(String origin, GeolocationPermissions.Callback callback)
    {
      callback.invoke(origin, true, false);
    }
  }
}