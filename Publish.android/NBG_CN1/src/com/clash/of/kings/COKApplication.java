package com.clash.of.kings;

import android.support.multidex.MultiDexApplication;
import android.util.Log;
import com.parse.Parse;
import com.parse.ParseACL;
import com.parse.ParseInstallation;
import com.parse.ParseUser;
import com.parse.PushService;

public class COKApplication extends MultiDexApplication {
	@Override
	public void onCreate() {
		super.onCreate();
		try{
			String strAppId = "id4b1N0wIOfYKMJjFzIrMOS7Mx7ifvuShwU7E44g";
			String strAppKey = "OS1yie0SmvHnGHOv7JSzF0ryLIv3K1hgYcnpIZD4";
	    	Parse.initialize(this, strAppId, strAppKey);
	    	Log.d("EmpireApplication","Init Parse");
	    	ParseUser.enableAutomaticUser();
	    	ParseACL defaultACL = new ParseACL();
	    	ParseACL.setDefaultACL(defaultACL, true);
		}catch(Throwable e){
			e.printStackTrace();
		}
	}
}