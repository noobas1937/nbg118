package org.hcg.IF;

import org.hcg.stac.empire.sns.FBUtil;

import android.os.AsyncTask;

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