package com.elex.chatservice;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.Window;
import android.widget.ImageView;

import com.elex.chatservice.app.ActionBarHelper;
import com.elex.chatservice.res.drawable.HSDraw;
import com.elex.chatservice.res.drawable.HSImages;
import com.elex.chatservice.res.values.HSConfig;
import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSRes;

import java.util.Map;
import org.json.JSONException;
import org.json.JSONObject;

public final class HSFaqs extends HSActivity
{
  public static final String TAG = "HelpShiftDebug";
  private HSApiData data;
  private HSStorage storage;
  private int callFinishRequestCode = 1;
  private Boolean showConvOnReportIssue;
  private ImageView hsFooter;

  public void onResume()
  {
    try
    {
      JSONObject configData = this.storage.getConfig();
      if (configData.length() != 0)
        HSConfig.updateConfig(configData);
    }
    catch (JSONException e) {
      Log.d("HelpShiftDebug", e.toString(), e);
    }

    super.onResume();
    HSFunnel.pushEvent("l");
  }

  public void onPause()
  {
    Bundle extras = getIntent().getExtras();
    if (extras != null) {
      Boolean isRoot = Boolean.valueOf(extras.getBoolean("isRoot"));
      if ((isRoot.booleanValue() == true) && (isFinishing() == true)) {
        HSActivityUtil.sessionEnding();
      }
    }

    super.onPause();
  }

  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    HSAnalytics.decomp = false;
    this.showConvOnReportIssue = Boolean.valueOf(getIntent().getExtras().getBoolean("showConvOnReportIssue"));
    Boolean isFullScreen = Boolean.valueOf(getIntent().getExtras().getBoolean("showInFullScreen"));

    if (isFullScreen.booleanValue()) {
      getWindow().setFlags(1024, 1024);
    }

    this.data = new HSApiData(this);
    this.storage = this.data.storage;

    setContentView(HSRes.getId(this, "layout", "hs__faqs"));

    ActionBarHelper actionBarHelper = getActionBarHelper();
    actionBarHelper.setDisplayHomeAsUpEnabled(true);
    this.hsFooter = ((ImageView)findViewById(HSRes.getId(this, "id", "hs__helpshiftActivityFooter")));

    if (((String)HSConfig.themeData.get("hl")).equals("true")) {
      this.hsFooter.setImageDrawable(HSDraw.getBitmapDrawable(this, (String)HSImages.imagesMap.get("newHSLogo")));

      this.hsFooter.setBackgroundResource(17170444);
    }
  }

  public boolean onCreateOptionsMenu(Menu menu)
  {
    getMenuInflater().inflate(HSRes.getId(this, "menu", "hs__faqs_fragment"), menu);
    return super.onCreateOptionsMenu(menu);
  }

  public boolean onOptionsItemSelected(MenuItem item)
  {
    int id = item.getItemId();
    if (id == 16908332) {
      finish();
      return true;
    }
    return super.onOptionsItemSelected(item);
  }
}