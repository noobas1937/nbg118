package org.hcg.IF;

import android.app.Activity;
import android.os.AsyncTask;
import com.umeng.analytics.MobclickAgent;

import java.io.*;

/**
 * Created by long on 14-1-6.
 */
public class CrashLogTask extends AsyncTask<Activity,Void,Void> {
    @Override
    protected Void doInBackground(Activity... params) {
        /** init and send last crash log*/
        String crashLogPath = params[0].getFilesDir().getAbsolutePath() + "/if_crash.log";
        File tmp = new File(crashLogPath);
        if (tmp.exists()) {
            try {
                FileInputStream fis = new FileInputStream(tmp);
                InputStreamReader isr = new InputStreamReader(fis);
                BufferedReader bufferedReader = new BufferedReader(isr);
                StringBuilder sb = new StringBuilder();
                String line;
                try {
                    while ((line = bufferedReader.readLine()) != null) {
                        sb.append(line);
                    }
                    MobclickAgent.reportError(params[0], sb.toString());
                } catch (IOException e) {
                    e.printStackTrace();
                }
                fis.close();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
            tmp.delete();
        }
        return null;
    }
}
