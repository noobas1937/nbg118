package org.hcg.util;

import android.app.Dialog;
import android.content.Context;
import android.webkit.WebView;

public class HtmlDialog extends Dialog {

	private WebView mWebView;
	private String htmlUrl;
	 

	public HtmlDialog(Context context,int title,String url) {
		super(context);
		this.htmlUrl=url;
		mWebView = new WebView(this.getContext());
		mWebView.setBackgroundColor(0xFFDAC292);
		this.setContentView(mWebView);
		this.setTitle(title);
		mWebView.loadUrl(htmlUrl);
	}

	 
}
