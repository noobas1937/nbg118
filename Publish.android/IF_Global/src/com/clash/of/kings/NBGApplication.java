package com.clash.of.kings;

/*import android.app.Application;

import com.nbg.gp.R;
import com.parse.Parse;
import com.parse.ParseInstallation;
import com.parse.PushService;


public class EmpireApplication extends Application {
	@Override
	public void onCreate() {
		super.onCreate();
		try {
			// Add your initialization code here
		    Parse.initialize(this, "mOzkRMxp0CwRal3O1rOXH9VJK4pIgJApzcY3Vmsj", "vyXy2kQHrwmCv0sLCxIRFDwpZSZk2qyTirjKgmwh");
			
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

public class EmpireApplication extends MultiDexApplication {

	
	@Override
	public void onCreate() {
		Log.d("EmpireApplication","enter onCreate");
		super.onCreate();
		try{
			String strAppId = "mOzkRMxp0CwRal3O1rOXH9VJK4pIgJApzcY3Vmsj";
			String strAppKey = "vyXy2kQHrwmCv0sLCxIRFDwpZSZk2qyTirjKgmwh";
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