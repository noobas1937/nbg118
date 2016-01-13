package com.elex.chatservice.view;

import java.util.Locale;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.lang.StringUtils;

import android.accounts.Account;
import android.accounts.AccountManager;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.ViewTreeObserver;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.util.CompatibleApiUtil;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.MathUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.util.ScaleUtil;
import com.elex.chatservice.view.actionbar.ActionBarFragment;

public class ForumFragment extends ActionBarFragment
{
	private WebView				webView;
	private ProgressBar			progressBar;
	private int					webLoadingProgress						= 0;
	private RelativeLayout		bottomButtonBar;
	private LinearLayout		navButtonLinearLayout;
	private RelativeLayout		webViewContainer;

	private ImageButton			backButton;
	private ImageButton			forwardButton;
	private ImageButton			refreshButton;
	private Button				translateButton;
	private Button				closeButton;
	private MyWebChromeClient	myWebChromeClient;

	private static int			lastScrollX								= -1;
	private static int			lastScrollY								= -1;
	public static String		targetUrl								= null;
	public static String		gmailAccount							= "";

	private boolean				webPageTranslated						= false;
	private String				currentWebUrl;
	private boolean				isLoadingTranslationPage				= true;
	public static int			webViewType								= 0;
	private String				loginUrl								= "";

	public static final int		WEBVIEW_TYPE_FORFUM						= 0;
	public static final int		WEBVIEW_TYPE_TRANSLATION_OPTIMIZATION	= 1;

	public ForumFragment()
	{
	}

	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		((ForumActivity) getActivity()).fragment = this;
	}

	private static Bundle	instanceState;

	public void saveState()
	{
		lastScrollX = webView.getScrollX();
		lastScrollY = webView.getScrollY();
		instanceState = new Bundle();
		webView.saveState(instanceState);
	}

	/**
	 * 打开软键盘时隐藏按钮栏
	 */
	private void checkGlobalLayoutChange()
	{
		int usableHeightNow = computeUsableHeight();

		if (usableHeight == -1 && usableHeightNow > 0)
		{
			usableHeight = usableHeightNow;
			return;
		}

		if (usableHeight - usableHeightNow > 200)
		{
			showBottomBar(false);
		}
		else
		{
			showBottomBar(true);
			usableHeight = usableHeightNow;
		}
	}

	private boolean	isPopupWebviewShowing	= false;

	private void showBottomBar(boolean show)
	{
		bottomButtonBar.setVisibility(show ? View.VISIBLE : View.GONE);
	}

	private void showCloseButton(boolean show)
	{
		navButtonLinearLayout.setVisibility(show ? View.GONE : View.VISIBLE);
		translateButton.setVisibility(show ? View.GONE : View.VISIBLE);
		closeButton.setVisibility(show ? View.VISIBLE : View.GONE);
	}

	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
	{
		this.activity = ((ForumActivity) getActivity());

		return inflater.inflate(ResUtil.getId(this, "layout", "cs__forum_fragment"), container, false);
	}

	private void generateWebviewUrl()
	{
		if (webViewType == WEBVIEW_TYPE_FORFUM)
		{
			getGmailAccount(activity);
			String accessToken = MathUtil.md5(UserManager.getInstance().getCurrentUser().uid + "accesstoken");

			if (!UserManager.getInstance().getCurrentUser().userName.equals("")
					&& !UserManager.getInstance().getCurrentUser().uid.equals(""))
			{
				// gmailAccount可以为空
				loginUrl = "http://f.elex.com/game_login.php?username=" + UserManager.getInstance().getCurrentUser().userName + "&userid="
						+ UserManager.getInstance().getCurrentUser().uid + "&useremail=" + gmailAccount + "&accesstoken=" + accessToken
						+ "&lang=" + ConfigManager.getInstance().gameLang;
			}
			else
			{
				if (ChatServiceController.getInstance().isInDummyHost())
				{
					loginUrl = "http://f.elex.com/game_login.php?username=ceshi&userid=222&useremail=test@qq.com&accesstoken=b9656cbedd14b59e139bdb14379312f2&lang="
							+ ConfigManager.getInstance().gameLang;
				}
				else
				{
					loginUrl = "http://f.elex.com/game_login.php?lang=" + ConfigManager.getInstance().gameLang;
				}
			}
		}
		else if (webViewType == WEBVIEW_TYPE_TRANSLATION_OPTIMIZATION)
		{
			if (ChatServiceController.getInstance().isInDummyHost())
			{
				loginUrl = "http://184.173.82.76/translate/add?sign=f33b1689f9a6532115c201aed977b450&uid=1381424418000001&userLang=en&fromLang=zh-Hans";
			}
			else
				loginUrl = targetUrl;
		}
	}

	/**
	 * 判断是否是邮箱
	 */
	public static boolean matchEmail(String account)
	{
		Pattern pattern = Pattern.compile("\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*");
		Matcher matcher = pattern.matcher(account);
		return matcher.matches();
	}

	public void getGmailAccount(Context context)
	{
		AccountManager accountManager = AccountManager.get(context);
		Account[] accounts = accountManager.getAccountsByType("com.google");
		Account account;
		int len = accounts.length;
		for (int i = 0; i < len; i++)
		{
			account = accounts[i];
			String accountName = account.name;
			if (matchEmail(accountName))
			{
				gmailAccount = accountName;
				break;
			}
		}
	}

	@SuppressLint("SetJavaScriptEnabled")
	public void onViewCreated(View view, Bundle savedInstanceState)
	{
		generateWebviewUrl();
		super.onViewCreated(view, savedInstanceState);
		bottomButtonBar = (RelativeLayout) view.findViewById(ResUtil.getId(this.activity, "id", "bottomButtonBar"));
		navButtonLinearLayout = (LinearLayout) view.findViewById(ResUtil.getId(this.activity, "id", "cs__NavButtonLinearLayout"));
		if (webViewType == WEBVIEW_TYPE_FORFUM)
			getTitleLabel().setText(LanguageManager.getLangByKey(LanguageKeys.TITLE_FORUM));
		else if (webViewType == WEBVIEW_TYPE_TRANSLATION_OPTIMIZATION)
			getTitleLabel().setText(LanguageManager.getLangByKey(LanguageKeys.TITLE_TRANSLATION_OPTIMIZATION));

		webViewContainer = (RelativeLayout) view.findViewById(ResUtil.getId(this.activity, "id", "messagesListLayout"));
		webView = (WebView) view.findViewById(ResUtil.getId(this.activity, "id", "cs__webView"));
		myWebChromeClient = new MyWebChromeClient();
		webView.setWebChromeClient(myWebChromeClient);
		// 界面正确显示需要开启JS
		webView.getSettings().setJavaScriptEnabled(true);
		webView.getSettings().setSupportMultipleWindows(true);
		webView.getSettings().setJavaScriptCanOpenWindowsAutomatically(true);
		webView.setScrollBarStyle(WebView.SCROLLBARS_OUTSIDE_OVERLAY);

		// 解决android2.3以下机器点击论坛webview的输入框不显示软键盘的问题
		webView.setOnTouchListener(new View.OnTouchListener()
		{
			@SuppressLint("ClickableViewAccessibility")
			@Override
			public boolean onTouch(View v, MotionEvent event)
			{
				switch (event.getAction())
				{
					case MotionEvent.ACTION_UP:
						if (!v.hasFocus())
						{
							v.requestFocus();
						}
						break;
				}
				return false;
			}
		});

		backButton = (ImageButton) view.findViewById(ResUtil.getId(this.activity, "id", "cs__backButton"));
		forwardButton = (ImageButton) view.findViewById(ResUtil.getId(this.activity, "id", "cs__forwardButton"));
		refreshButton = (ImageButton) view.findViewById(ResUtil.getId(this.activity, "id", "cs__refreshButton"));
		translateButton = (Button) view.findViewById(ResUtil.getId(this.activity, "id", "cs__translateButton"));
		closeButton = (Button) view.findViewById(ResUtil.getId(this.activity, "id", "cs__closeButton"));
		translateButton.setText(LanguageManager.getLangByKey(LanguageKeys.MENU_TRANSLATE));
		backButton.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View view)
			{
				webView.goBack();
			}
		});
		forwardButton.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View view)
			{
				webView.goForward();
			}
		});
		refreshButton.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View view)
			{
				webView.reload();
			}
		});

		translateButton.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View view)
			{
				onTranslateButtonClick();
			}
		});
		closeButton.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View view)
			{
				onCloseButtonClick();
			}
		});

		showCloseButton(false);

		refreshButtonState();

		progressBar = (ProgressBar) view.findViewById(ResUtil.getId(this.activity, "id", "progressBar"));
		progressBar.setVisibility(View.GONE);

		webView.setWebViewClient(new WebViewClient()
		{
			public boolean shouldOverrideUrlLoading(WebView view, String url)
			{
				return false;
			}

			public void onPageStarted(WebView view, String url, Bitmap favicon)
			{
				handlePageStarted();
			}

			/**
			 * WebView 在Android4.4的手机上onPageFinished()回调会多调用一次(具体原因待追查)
			 * 需要尽量避免在onPageFinished()中做业务操作，否则会导致重复调用，还有可能会引起逻辑上的错误.
			 */
			public void onPageFinished(WebView view, String url)
			{
			}
		});

		ConfigManager.calcScale(activity);

		FrameLayout content = (FrameLayout) activity.findViewById(android.R.id.content);
		content.getChildAt(0).getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener()
		{
			public void onGlobalLayout()
			{
				checkGlobalLayoutChange();
				adjustHeight();
			}
		});

		loadFirstPage();
	}

	private void loadFirstPage()
	{
		boolean hasLogined = true;
		if (isFirstLogin)
		{
			isFirstLogin = false;
			hasLogined = false;
		}

		if (instanceState != null)
		{
			webView.restoreState(instanceState);
			needToRecoverLastPosition = true;
		}

		if (!hasLogined)
		{
			if (webViewType == WEBVIEW_TYPE_FORFUM)
			{
				if (StringUtils.isNotEmpty(targetUrl))
				{
					loadUrl(loginUrl + "&userurl=" + targetUrl);
				}
				else
				{
					loadUrl(loginUrl + "&userurl=" + homePage);
				}
				targetUrl = "";
			}
			else if (webViewType == WEBVIEW_TYPE_TRANSLATION_OPTIMIZATION)
			{
				loadUrl(loginUrl);
			}
			needToRecoverLastPosition = false;
		}
		else
		{
			if (webViewType == WEBVIEW_TYPE_FORFUM)
			{
				if (StringUtils.isNotEmpty(targetUrl))
				{
					loadUrl(targetUrl);
					needToRecoverLastPosition = false;
				}
				else
				{
					if (instanceState == null)
					{
						loadUrl(homePageHttp);
					}
				}
				targetUrl = "";
			}
			else if (webViewType == WEBVIEW_TYPE_TRANSLATION_OPTIMIZATION)
			{
				loadUrl(loginUrl);
			}
		}
	}

	private void handlePageStarted()
	{
		refreshButtonState();
		webLoadingProgress = 5;
		new Thread()
		{
			@Override
			public void run()
			{
				while (webLoadingProgress < 100)
				{
					try
					{
						// 正在加载时，退出论坛界面，需停止此线程，否则会反复调用
						if(activity == null){
							webLoadingProgress = 100;
							continue;
						}
						activity.runOnUiThread(new Runnable()
						{
							@Override
							public void run()
							{
								try
								{
									webLoadingProgress = webView.getProgress();
									progressBar.setMax(100);
									progressBar.setIndeterminate(false);
									progressBar.setVisibility(View.VISIBLE);
									if (webLoadingProgress > 5)
									{
										progressBar.setProgress(webLoadingProgress);
									}
								}
								catch (Exception e)
								{
									LogUtil.printException(e);
								}
							}
						});
						Thread.sleep(100);
					}
					catch (Exception e)
					{
						e.printStackTrace();
					}
				}
				onLoadingFinished();
			}
		}.start();
	}

	private void onLoadingFinished()
	{
		if (activity == null)
			return;
		activity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				progressBar.setVisibility(View.GONE);

				if (isLoadingTranslationPage)
				{
					isLoadingTranslationPage = false;
				}

				refreshButtonState();

				if (needToRecoverLastPosition)
				{
					needToRecoverLastPosition = false;
					if (lastScrollX != -1 && lastScrollY != -1)
					{
						webView.scrollTo(lastScrollX, lastScrollY);
					}
					lastScrollX = -1;
					lastScrollY = -1;
				}

				calcCurrentWebUrl();

				if (webView.getUrl().contains("translate.google"))
				{
					webPageTranslated = true;
					translateButton.setText(LanguageManager.getLangByKey(LanguageKeys.MENU_ORIGINALLAN));
				}
				else
				{
					webPageTranslated = false;
					translateButton.setText(LanguageManager.getLangByKey(LanguageKeys.MENU_TRANSLATE));
				}
			}
		});
	}

	private void calcCurrentWebUrl()
	{
		// 获取源语言网页URL
		String url = webView.getUrl();
		if (url.contains("=http:") && !url.contains("=http://translate.google"))
		{
			int langpairIndex = url.indexOf("&langpair=");
			int httpIndex = url.indexOf("=http:");
			int sandboxIndex = url.indexOf("&sandbox=");
			int depthIndex = url.indexOf("&depth=");
			int usgIndex = url.indexOf("&usg=");
			int netErrorIndex = url.indexOf("%3Fs%3D");

			int minIndex = url.length();
			if (depthIndex > httpIndex)
			{
				minIndex = minIndex < depthIndex ? minIndex : depthIndex;
			}

			if (usgIndex > httpIndex)
			{
				minIndex = minIndex < usgIndex ? minIndex : usgIndex;
			}

			if (sandboxIndex > httpIndex)
			{
				minIndex = minIndex < sandboxIndex ? minIndex : sandboxIndex;
			}

			if (langpairIndex > httpIndex)
			{
				minIndex = minIndex < langpairIndex ? minIndex : langpairIndex;
			}

			if (netErrorIndex > httpIndex)
			{
				minIndex = minIndex < netErrorIndex ? minIndex : netErrorIndex;
			}

			if (minIndex > httpIndex)
			{
				currentWebUrl = url.substring(httpIndex + 1, minIndex);
			}
			else
			{
				currentWebUrl = url.substring(httpIndex + 1);
			}
		}
		else if (url.contains("http://") && url.contains("?s="))
		{
			currentWebUrl = url.substring(url.indexOf("http://"), url.indexOf("?s="));
		}
	}

	private void onTranslateButtonClick()
	{
		if (!isLoadingTranslationPage)
		{
			if (!webPageTranslated)
			{
				isLoadingTranslationPage = true;
				String localLang = Locale.getDefault().getLanguage();
				String newUrl = "http://translate.google.com/translate?u=" + webView.getUrl() + "&langpair=auto|" + localLang
						+ "&complete=1&hl=auto&newwindow=1&ie=UTF-8&oe=UTF-8&prev=/language_tools";
				webView.loadUrl(newUrl);
			}
			else
			{
				isLoadingTranslationPage = true;
				webView.loadUrl(currentWebUrl);
			}
		}
	}

	public static boolean		isFirstLogin				= true;

	private static final String	homePage					= "f.elex.com/forums/1-Clash-of-Kings";
	private static final String	homePageHttp				= "http://f.elex.com/forums/1-Clash-of-Kings";
	private boolean				needToRecoverLastPosition	= false;

	private void loadUrl(String url)
	{
		webView.loadUrl(url);
	}

	private void refreshButtonState()
	{
		if (activity == null)
			return;
		activity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					CompatibleApiUtil.getInstance().setButtonAlpha(backButton, webView.canGoBack() ? true : false);
					CompatibleApiUtil.getInstance().setButtonAlpha(forwardButton, webView.canGoForward() ? true : false);
					backButton.setEnabled(webView.canGoBack() ? true : false);
					forwardButton.setEnabled(webView.canGoForward() ? true : false);
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	public void adjustHeight()
	{
		if (!ConfigManager.getInstance().scaleFontandUI)
		{
			adjustSizeCompleted = true;
			return;
		}

		if (backButton.getWidth() != 0 && !adjustSizeCompleted)
		{
			backButton.setLayoutParams(new LinearLayout.LayoutParams((int) (backButton.getWidth() * ConfigManager.scaleRatio),
					(int) (backButton.getHeight() * ConfigManager.scaleRatio)));
			forwardButton.setLayoutParams(new LinearLayout.LayoutParams((int) (forwardButton.getWidth() * ConfigManager.scaleRatio),
					(int) (forwardButton.getHeight() * ConfigManager.scaleRatio)));
			refreshButton.setLayoutParams(new LinearLayout.LayoutParams((int) (refreshButton.getWidth() * ConfigManager.scaleRatio),
					(int) (refreshButton.getHeight() * ConfigManager.scaleRatio)));

			ScaleUtil.adjustTextSize(translateButton, ConfigManager.scaleRatio);

			adjustSizeCompleted = true;
		}
	}

	private void onCloseButtonClick()
	{
		if (!isPopupWebviewShowing)
			return;

		myWebChromeClient.onCloseWindow(myWebChromeClient.popupWebView);
	}

	public class MyWebChromeClient extends WebChromeClient
	{
		private WebView	popupWebView;

		@SuppressLint("SetJavaScriptEnabled")
		@Override
		public boolean onCreateWindow(WebView view, boolean dialog, boolean userGesture, android.os.Message resultMsg)
		{
			popupWebView = new WebView(view.getContext());
			// 需要开启，否则登录后页面渲染报错
			popupWebView.getSettings().setJavaScriptEnabled(true);
			popupWebView.setWebChromeClient(this);
			popupWebView.setWebViewClient(new WebViewClient());
			popupWebView.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
			webViewContainer.addView(popupWebView);
			WebView.WebViewTransport transport = (WebView.WebViewTransport) resultMsg.obj;
			transport.setWebView(popupWebView);
			resultMsg.sendToTarget();
			isPopupWebviewShowing = true;
			showCloseButton(true);
			return true;
		}

		@Override
		public void onCloseWindow(WebView window)
		{
			webViewContainer.removeView(window);
			isPopupWebviewShowing = false;
			showCloseButton(false);
		}
	}
}