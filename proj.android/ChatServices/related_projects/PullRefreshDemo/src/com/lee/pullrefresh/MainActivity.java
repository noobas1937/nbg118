package com.lee.pullrefresh;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public class MainActivity extends Activity implements View.OnClickListener {
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_pull_refresh_demo);
        
        findViewById(R.id.listview).setOnClickListener(this);
        findViewById(R.id.webview).setOnClickListener(this);
        findViewById(R.id.scrollview).setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.listview) {
			startActivity(new Intent(this, PullRefreshListViewActivity.class));
		} else if (v.getId() == R.id.scrollview) {
			startActivity(new Intent(this, PullRefreshScrollViewActivity.class));
		} else if (v.getId() == R.id.webview) {
			startActivity(new Intent(this, PullRefreshWebViewActivity.class));
		}
    }
}
