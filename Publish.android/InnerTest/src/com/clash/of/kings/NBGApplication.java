package com.clash.of.kings;

/*import android.app.Application;

import com.nbg.innertest.R;
import com.parse.Parse;
import com.parse.ParseInstallation;
import com.parse.PushService;


public class EmpireApplication extends Application {
	@Override
	public void onCreate() {
		super.onCreate();
		try {
			// Add your initialization code here
		    Parse.initialize(this, "id4b1N0wIOfYKMJjFzIrMOS7Mx7ifvuShwU7E44g", "OS1yie0SmvHnGHOv7JSzF0ryLIv3K1hgYcnpIZD4");
			
			PushService.setDefaultPushCallback(this, EmpireActivity.class);
			ParseInstallation.getCurrentInstallation().saveInBackground();
//			Native.nativeSetParseRegisterId(ParseInstallation.getCurrentInstallation().getInstallationId());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}*/

import com.parse.Parse;
import com.parse.ParseACL;
import com.parse.ParseUser;

import android.support.multidex.MultiDexApplication;
import android.util.Log;

public class NBGApplication extends MultiDexApplication {

	
	@Override
	public void onCreate() {
		Log.d("EmpireApplication","enter onCreate");
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