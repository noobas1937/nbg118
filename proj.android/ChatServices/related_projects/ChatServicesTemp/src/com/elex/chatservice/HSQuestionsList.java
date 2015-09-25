package com.elex.chatservice;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources.Theme;
import android.content.res.TypedArray;
import android.os.Bundle;
import android.support.v4.view.PagerTabStrip;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.SimpleOnPageChangeListener;
import android.text.TextUtils;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.ImageView;
import android.widget.TextView;

import com.elex.chatservice.app.ActionBarHelper;
import com.elex.chatservice.res.drawable.HSDraw;
import com.elex.chatservice.res.drawable.HSImages;
import com.elex.chatservice.res.values.HSConfig;
import com.elex.chatservice.util.HSRes;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public final class HSQuestionsList extends HSActivity
{
  public static final String TAG = "HelpShiftDebug";
  private HSSectionPagerAdapter hsAdapter;
  private ViewPager viewPager;
  private ImageView hsFooter;

  private void appendHashMap(ArrayList list, Object type, Object obj)
  {
    HashMap map = new HashMap();
    map.put("type", type);
    map.put("obj", obj);
    list.add(map);
  }

  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    Boolean isFullScreen = Boolean.valueOf(getIntent().getExtras().getBoolean("showInFullScreen"));

    if (isFullScreen.booleanValue()) {
      getWindow().setFlags(1024, 1024);
    }

    setContentView(HSRes.getId(this, "layout", "hs__questions_list"));

    String sectionPubId = getIntent().getExtras().getString("sectionPublishId");

    this.hsAdapter = new HSSectionPagerAdapter(getSupportFragmentManager(), this, sectionPubId);
    this.viewPager = ((ViewPager)findViewById(HSRes.getId(this, "id", "hs__sections_pager")));

    this.viewPager.setAdapter(this.hsAdapter);
    this.viewPager.setOnPageChangeListener(new ViewPager.SimpleOnPageChangeListener()
    {
      public void onPageSelected(int position) {
        hsAdapter.onPageSelected(position);
      }
    });
    this.viewPager.setCurrentItem(this.hsAdapter.getCurrentPosition());

    PagerTabStrip tabStrip = (PagerTabStrip)findViewById(HSRes.getId(this, "id", "hs__pager_tab_strip"));

    for (int i = 0; i < tabStrip.getChildCount(); i++) {
      View nextChild = tabStrip.getChildAt(i);
      if ((nextChild instanceof TextView)) {
        TextView textViewToConvert = (TextView)nextChild;
        textViewToConvert.setSingleLine();
        textViewToConvert.setEllipsize(TextUtils.TruncateAt.END);
        TypedArray array = getActionBarHelper().getThemedContext().getTheme().obtainStyledAttributes(new int[] { 16842806 });

        int textColor = array.getColor(0, 16777215);
        textViewToConvert.setTextColor(textColor);
        array.recycle();
      }
    }

    int tabIndicatorColor = HSRes.getColor(this, "hs__actionBarTabIndicatorColor");
    tabStrip.setTabIndicatorColor(tabIndicatorColor);

    ActionBarHelper actionBarHelper = getActionBarHelper();
    actionBarHelper.setDisplayHomeAsUpEnabled(true);
    actionBarHelper.setTitle(HSRes.getString(this, "hs__faq_header"));

    if (((String)HSConfig.themeData.get("hl")).equals("true")) {
      this.hsFooter = ((ImageView)findViewById(HSRes.getId(this, "id", "hs__helpshiftActivityFooter")));

      this.hsFooter.setImageDrawable(HSDraw.getBitmapDrawable(this, (String)HSImages.imagesMap.get("newHSLogo")));

      this.hsFooter.setBackgroundResource(17170444);
    }
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

  public void onResume()
  {
    super.onResume();
  }

  public void onBackPressed()
  {
    super.onBackPressed();
  }
}