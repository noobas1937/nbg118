package org.hcg.IF;

import android.os.AsyncTask;
import android.util.Log;

/**
 * Created by long on 14-1-7.
 */
public class NetTask extends AsyncTask<Net,Void,Void> {
    @Override
    protected Void doInBackground(Net... params) {
    	if(params[0] == null || params[0].sfsClient == null)
    	{
    		Log.d("NetTask", "Connect >>>>>> NULL");
    		return null;
    	}
        params[0].sfsClient.connect(params[0].getIP(), params[0].getPort());
        return null;
    }
}