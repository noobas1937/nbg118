package org.nbg.stac.empire.common.activity;

import org.nbg.IF.IF;
import org.nbg.stac.empire.publish.IPublishChannel;

import android.content.Intent;
import android.os.Bundle;

import com.facebook.CallbackManager;
import com.facebook.FacebookSdk;

public abstract class CommonActivity extends IF{

	// XXX do nothing; 用来适配原有AOE Android 工程结构
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
	}

	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
	}
	
	@Override
	public void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		// TODO Auto-generated method stub
		super.onSaveInstanceState(outState);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
	}

	@Override
	public IPublishChannel getPublishImpl() {
		// TODO Auto-generated method stub
		return null;
	}
}
