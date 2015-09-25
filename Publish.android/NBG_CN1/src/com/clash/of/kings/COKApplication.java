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