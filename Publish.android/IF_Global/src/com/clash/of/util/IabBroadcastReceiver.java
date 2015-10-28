package com.clash.of.util;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class IabBroadcastReceiver extends BroadcastReceiver{

	/** Listener interface for received broadcast messages. */ 
	public interface IabBroadcastListener {
		void receivedBroadcast();
	}
	
	private final IabBroadcastListener mListener;
	
	public IabBroadcastReceiver(IabBroadcastListener listener) {
		mListener = listener;
	}
	
	public void onReceive(Context context, Intent intent) { 
		if (mListener != null) {
			mListener.receivedBroadcast(); 
		}
	}

}