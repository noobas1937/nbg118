package com.example.sample1;

import android.app.Activity;
import android.os.Bundle;

import com.elex.chatservice.Helpshift;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
//		this.requestWindowFeature(Window.FEATURE_NO_TITLE);//去掉标题栏
//		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);//去掉信息栏
		
		setContentView(R.layout.activity_main);
		
	    Helpshift.install(getApplication(), "d364a0d2a0b3856ba1aa8d4cc0371855",
	        "elex.helpshift.com",
	        "elex_platform_20151106082553299-dfa2768467c76a9");
	    
	    Helpshift.showConversation(MainActivity.this);
//		Helpshift.showFAQs(MainActivity.this);
	    
	    //会出错 Binary XML file line #1: Error inflating class <unknown>
	    //android.content.res.Resources$NotFoundException: Resource is not a Drawable (color or path): TypedValue{t=0x2/d=0x7f010071 a=-1}
//		Helpshift.showFAQSection(MainActivity.this, "392");
//		Helpshift.showSingleFAQ(MainActivity.this, "1637");
//		Helpshift.showFAQs(MainActivity.this);
	}
}
