package com.elex.chatservice;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.Window;
import android.widget.ImageView;

import com.elex.chatservice.app.ActionBarHelper;
import com.elex.chatservice.res.drawable.HSDraw;
import com.elex.chatservice.res.drawable.HSImages;
import com.elex.chatservice.res.values.HSConfig;
import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSRes;

import java.util.Map;

public final class HSQuestion extends HSActivity
{
  public static final String TAG = "HelpShiftDebug";
  private HSQuestionFragment questionFragment = null;
  private ImageView hsFooter;

  public void onResume()
  {
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
    supportRequestWindowFeature(5);

    Boolean isFullScreen = Boolean.valueOf(getIntent().getExtras().getBoolean("showInFullScreen"));

    if (isFullScreen.booleanValue()) {
      getWindow().setFlags(1024, 1024);
    }

    setContentView(HSRes.getId(this, "layout", "hs__question"));

    ActionBarHelper actionBarHelper = getActionBarHelper();
    actionBarHelper.setDisplayHomeAsUpEnabled(true);

    if (((String)HSConfig.themeData.get("hl")).equals("true")) {
      this.hsFooter = ((ImageView)findViewById(HSRes.getId(this, "id", "hs__helpshiftActivityFooter")));

      this.hsFooter.setImageDrawable(HSDraw.getBitmapDrawable(this, (String)HSImages.imagesMap.get("newHSLogo")));

      this.hsFooter.setBackgroundResource(17170444);
    }
    setSupportProgressBarIndeterminateVisibility(true);
  }

  public void onAttachFragment(Fragment f)
  {
    super.onAttachFragment(f);
    if ((f instanceof HSQuestionFragment))
      this.questionFragment = ((HSQuestionFragment)f);
  }
}