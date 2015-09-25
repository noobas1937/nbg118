package com.elex.chatservice;

import android.content.Context;
import android.database.SQLException;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentStatePagerAdapter;
import java.util.ArrayList;
import java.util.HashMap;

public final class HSSectionPagerAdapter extends FragmentStatePagerAdapter
{
  public static final String TAG = "HelpShiftDebug";
  private static ArrayList<Section> sectionsList = new ArrayList();
  private HSApiData data;
  private HSStorage storage;
  private int currentPosition;

  private void appendHashMap(ArrayList list, Object type, Object obj)
  {
    HashMap map = new HashMap();
    map.put("type", type);
    map.put("obj", obj);
    list.add(map);
  }

  public void onPageSelected(int position)
  {
    this.currentPosition = position;
  }

  public HSSectionPagerAdapter(FragmentManager fm) {
    super(fm);
  }

  public HSSectionPagerAdapter(FragmentManager fm, Context c, String sectionPubId)
  {
    super(fm);

    this.data = new HSApiData(c);
    this.storage = this.data.storage;
    sectionsList.clear();
    this.currentPosition = -1;
    try {
      sectionsList = this.data.getPopulatedSections();
      int i = 0;
      for (i = 0; i < sectionsList.size(); i++) {
        if (((Section)sectionsList.get(i)).getPublishId().equals(sectionPubId)) {
          this.currentPosition = i;
          break;
        }
      }
      notifyDataSetChanged();
    } catch (SQLException e) {
      Log.d("HelpShiftDebug", e.toString(), e);
    }
  }

  public int getCurrentPosition() {
    return this.currentPosition;
  }

  public CharSequence getPageTitle(int position) {
    Section item = (Section)sectionsList.get(position);
    return item.getTitle();
  }

  public int getCount()
  {
    return sectionsList.size();
  }

  public Fragment getItem(int position)
  {
    Section item = (Section)sectionsList.get(position);

    HSSectionFragment newFragment = new HSSectionFragment();

    String publishId = item.getPublishId();

    Bundle fragmentData = new Bundle();
    fragmentData.putString("sectionPublishId", publishId);
    newFragment.setArguments(fragmentData);
    return newFragment;
  }
}