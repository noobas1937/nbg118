package com.elex.chatservice;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.elex.chatservice.app.ActionBarHelper;
import com.elex.chatservice.res.drawable.HSDraw;
import com.elex.chatservice.res.drawable.HSImages;
import com.elex.chatservice.res.values.HSConfig;
import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSRes;

import java.util.Map;
import org.json.JSONException;
import org.json.JSONObject;

public final class HSSection extends HSActivity
  implements HSFaqSyncStatusEvents
{
  public static final String TAG = "HelpShiftDebug";
  private HSApiData data;
  private HSStorage storage;
  private HSSectionFragment mainListFragment;
  private String sectionPublishId;
  private ActionBarHelper actionBarHelper;
  private MenuItem mSearchItem;
  private View mSearchView;
  private TextView faqSyncStatus;
  private ImageView faqsUpdatedIcon;
  private ProgressBar faqsUpdatingIcon;

  private void setTextViewAlpha(TextView tv, float alpha)
  {
    int color = tv.getCurrentTextColor();
    int newColor = Color.argb((int)Math.floor(alpha * 255.0F), Color.red(color), Color.green(color), Color.blue(color));

    tv.setTextColor(newColor);
  }

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

    HSAnalytics.decomp = true;
    Bundle extras = getIntent().getExtras();
    if (extras != null) {
      this.sectionPublishId = ((String)extras.get("sectionPublishId"));
    }

    this.data = new HSApiData(this);
    this.storage = this.data.storage;

    Boolean isFullScreen = Boolean.valueOf(extras.getBoolean("showInFullScreen"));

    if (isFullScreen.booleanValue()) {
      getWindow().setFlags(1024, 1024);
    }

    setContentView(HSRes.getId(this, "layout", "hs__section"));

    this.actionBarHelper = getActionBarHelper();
    this.actionBarHelper.setDisplayHomeAsUpEnabled(true);

    if (((String)HSConfig.themeData.get("hl")).equals("true")) {
      int sectionFooterId = HSRes.getId(this, "id", "hs__sectionFooter");

      ImageView iv = (ImageView)findViewById(sectionFooterId);
      iv.setImageDrawable(HSDraw.getBitmapDrawable(this, (String)HSImages.imagesMap.get("newHSLogo")));

      iv.setBackgroundResource(17170444);
    }

    FragmentManager fm = getSupportFragmentManager();
    FragmentTransaction ft = fm.beginTransaction();
    this.mainListFragment = new HSSectionFragment();
    Bundle fragmentData = new Bundle();
    fragmentData.putString("sectionPublishId", this.sectionPublishId);
    fragmentData.putBoolean("decomp", true);
    fragmentData.putAll(extras);
    this.mainListFragment.setArguments(fragmentData);

    ft.add(HSRes.getId(this, "id", "hs__sectionContainer"), this.mainListFragment);

    ft.commit();

    HSApiData.addFaqSyncStatusObserver(this);
    this.faqSyncStatus = ((TextView)findViewById(HSRes.getId(this, "id", "hs__faq_section_sync_status_text")));

    this.faqsUpdatedIcon = ((ImageView)findViewById(HSRes.getId(this, "id", "hs__faq_section_sync_status_icon")));

    this.faqsUpdatingIcon = ((ProgressBar)findViewById(HSRes.getId(this, "id", "hs__faq_section_sync_status_spinner")));

    setTextViewAlpha(this.faqSyncStatus, 0.5F);
    faqsUpdating();
  }

  public void onDestroy()
  {
    super.onDestroy();
    HSApiData.removeFaqSyncStatusObserver(this);
  }

  public void faqsUpdating()
  {
    this.faqsUpdatedIcon.setVisibility(8);
    this.faqsUpdatingIcon.setVisibility(0);
    this.faqSyncStatus.setText(HSRes.getString(this, "hs__faqs_updating"));
  }

  public void faqsUpdated()
  {
    this.faqsUpdatedIcon.setVisibility(0);
    this.faqsUpdatingIcon.setVisibility(8);
    this.faqSyncStatus.setText(HSRes.getString(this, "hs__faqs_uptodate"));
  }

  public void faqsNotUpdated()
  {
    this.faqsUpdatedIcon.setVisibility(8);
    this.faqsUpdatingIcon.setVisibility(8);
    this.faqSyncStatus.setText(HSRes.getString(this, "hs__faqs_updation_failure"));
  }

  public void searchIndexesUpdated()
  {
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