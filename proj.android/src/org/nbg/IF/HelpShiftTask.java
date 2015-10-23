package org.nbg.IF;

import android.os.AsyncTask;

import org.nbg.stac.empire.sns.FBUtil;

import com.helpshift.HelpshiftBridge;
/**
 * Created by long on 14-1-7.
 */
public class HelpShiftTask extends AsyncTask<IF,Void,Void> {
    @Override
    protected Void doInBackground(IF... params) {
		try {
			FBUtil.createLikeButton(IF.getInstance());
		} catch (Exception e) {
			e.printStackTrace();
		}
        return null;
    }
}