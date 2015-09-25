package com.elex.chatservice;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.view.View;

public final class HSReview extends FragmentActivity
{
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(new View(this));

    FragmentManager fm = getSupportFragmentManager();
    HSReviewFragment reviewDialog = new HSReviewFragment();
    reviewDialog.show(fm, "hs__review_dialog");
  }
}