package com.elex.chatservice;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.LinearLayout;

import com.elex.chatservice.app.ActionBarHelper;
import com.elex.chatservice.res.drawable.HSDraw;
import com.elex.chatservice.res.drawable.HSImages;
import com.elex.chatservice.res.values.HSConfig;
import com.elex.chatservice.util.AttachmentUtil;
import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSRes;

import java.util.Map;

public final class HSConversation extends HSActivity
{
  public static final String TAG = "HelpShiftDebug";
  private HSApiData data;
  private HSStorage storage;
  private Bundle bundle;
  private int fragmentHolderId;
  private FragmentTransaction ft;

  public void onCreate(Bundle savedInstanceState)
  {
	  System.out.println("HSConversation onCreate");
    super.onCreate(savedInstanceState);
    this.data = new HSApiData(this);
    this.storage = this.data.storage;

    Bundle extras = getIntent().getExtras();
    String activeConversation = this.storage.getActiveConversation();

//    ActionBarHelper actionBarHelper = getActionBarHelper();
//    actionBarHelper.supportRequestWindowFeature(5);
//    actionBarHelper.setDisplayHomeAsUpEnabled(true);

    String chatLaunchSource = extras.getString("chatLaunchSource");
    String archivedConversation = this.storage.getArchivedConversation();
    if ((chatLaunchSource.equals("push")) || (chatLaunchSource.equals("inapp")) || !archivedConversation.equals("") || !activeConversation.equals(""))
    {
    	requestWindowFeature(Window.FEATURE_NO_TITLE);
    }
    
    //等效于getWindow().addFlags(1024);
    //会导致输入法把整个屏幕挤上去，helpshift的会话界面也不是全屏的（FAQ是全屏）
//    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
    
    setContentView(HSRes.getId(this, "layout", "cs__conversation"));

//	AndroidBug5497Workaround.assistActivity(this);
    
    //foot logo
//    if (((String)HSConfig.themeData.get("hl")).equals("true")) {
//      int addIssueFooterId = HSRes.getId(this, "id", "hs__newConversationFooter");
//
//      LinearLayout addIssueFooter = (LinearLayout)findViewById(addIssueFooterId);
//
//      ImageView iv = new ImageView(this);
//      iv.setImageDrawable(HSDraw.getBitmapDrawable(this, (String)HSImages.imagesMap.get("newHSLogo")));
//
//      iv.setBackgroundResource(17170444);
//      addIssueFooter.addView(iv);
//    }

    this.bundle = new Bundle(extras);

    HSAnalytics.decomp = extras.getBoolean("decomp", false);

    this.fragmentHolderId = HSRes.getId(this, "id", "hs__fragment_holder");
    this.ft = getSupportFragmentManager().beginTransaction();
    
//    showMessagesFragment();
    
    if (extras.getBoolean("newConversation"))
    {
		//新会话
      showNewConversationFragment();
    }
    else if ((chatLaunchSource.equals("push")) || (chatLaunchSource.equals("inapp")))
    {
		//出错
      showMessagesFragment();
    }
    else{
    	//新会话
    	showFragment();
    }
  }

  private void showFragment()
  {
    String activeConversation = this.storage.getActiveConversation();
    String archivedConversation = this.storage.getArchivedConversation();

    if (!archivedConversation.equals("")) {
      this.bundle.putString("issueId", archivedConversation);
      showMessagesFragment();
    } else if (!activeConversation.equals("")) {
      this.bundle.putString("issueId", activeConversation);
      showMessagesFragment();
    } else {
      showNewConversationFragment();
    }
  }

  private void showNewConversationFragment() {
    this.ft.add(this.fragmentHolderId, Fragment.instantiate(this, HSAddIssueFragment.class.getName(), this.bundle));

    this.ft.commit();
    super.startPoller();
  }

  private void showMessagesFragment() {
    this.ft.add(this.fragmentHolderId, Fragment.instantiate(this, HSMessagesFragment.class.getName(), this.bundle));

    this.ft.commit();
  }

  protected void onResume()
  {
    Bundle extras = getIntent().getExtras();
    String activeConversation = this.storage.getActiveConversation();
    String archivedConversation = this.storage.getArchivedConversation();
    
//    HSActivityUtil.restoreFullscreen(this);
//    if ((extras.getBoolean("newConversation")) || ((activeConversation.equals("")) && (archivedConversation.equals(""))))
//    {
//      HSActivityUtil.restoreFullscreen(this);
//    }
//    else{
//    	HSActivityUtil.forceNotFullscreen(this);
//    }

    this.storage.setIsConversationShowing(Boolean.valueOf(true));
    super.onResume();
  }

  protected void onPause()
  {
    Bundle extras = getIntent().getExtras();
    String activeConversation = this.storage.getActiveConversation();
    String archivedConversation = this.storage.getArchivedConversation();

    if ((extras.getBoolean("newConversation")) || ((activeConversation.equals("")) && (archivedConversation.equals(""))))
    {
      if (!AttachmentUtil.isSelectingAttachment()) {
        Intent returnIntent = new Intent();
        setResult(-1, returnIntent);
        finish();
      }
    }
//    HSActivityUtil.restoreFullscreen(this);
    if (extras != null) {
      Boolean isRoot = Boolean.valueOf(extras.getBoolean("isRoot"));
      if ((isRoot.booleanValue() == true) && (isFinishing() == true)) {
        HSActivityUtil.sessionEnding();
      }
    }
    super.onPause();
  }

  public void onBackPressed()
  {
    Intent returnIntent = new Intent();
    setResult(-1, returnIntent);
    super.onBackPressed();
  }

  public void onDestroy()
  {
    this.storage.setIsConversationShowing(Boolean.valueOf(false));
    super.onDestroy();
  }
}