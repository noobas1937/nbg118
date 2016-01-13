package org;

import java.net.URISyntaxException;
import java.util.HashMap;
import java.util.Map;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {

    WsClient client = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        String appid = "100001";
        String time = "" + System.currentTimeMillis();
        String uid = "1380131787000001";

        Map<String, String> header = new HashMap<String, String>();
        header.put("APPID", appid);
        header.put("TIME", time);
        header.put("UID", uid);
        header.put("SIGN", Utils.stringMD5(Utils.stringMD5(appid+uid)+time));

//        try {
//            client = new WsClient("ws://cokchat-s2.elexapp.com:8088", header);
//        } catch (URISyntaxException e) {
//            e.printStackTrace();
//        }

        final Button connect = null;//(Button) this.findViewById(R.id.connect);
        Button close = null;//(Button) this.findViewById(R.id.close);

        connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                connect();
            }
        });

        close.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(client != null){
                    client.close();
                }
            }
        });
    }

	private void connect()
	{
		if(client != null){
		    client.connect();
		    new Thread(new Runnable() {
		        @Override
		        public void run() {
		        	runWS();
		        }
		    }).start();
		}
	}

	private void runWS()
	{
		while (true){
		    try {
		        Thread.sleep(1000 * 8);
		    } catch (InterruptedException e) {
		        e.printStackTrace();
		    }
		    if(client.isOpen){
		        client.send("heartbeat");
		        if(client.isClose){
		            break;
		        }
		    }
		}
	}

//    @Override
//    public boolean onCreateOptionsMenu(Menu menu) {
//        // Inflate the menu; this adds items to the action bar if it is present.
//        getMenuInflater().inflate(R.menu.menu_main, menu);
//        return true;
//    }
//
//    @Override
//    public boolean onOptionsItemSelected(MenuItem item) {
//        // Handle action bar item clicks here. The action bar will
//        // automatically handle clicks on the Home/Up button, so long
//        // as you specify a parent activity in AndroidManifest.xml.
//        int id = item.getItemId();
//
//        //noinspection SimplifiableIfStatement
//        if (id == R.id.action_settings) {
//            return true;
//        }
//
//        return super.onOptionsItemSelected(item);
//    }
}
