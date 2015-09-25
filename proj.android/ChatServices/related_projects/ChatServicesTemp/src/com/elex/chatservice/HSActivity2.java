package com.elex.chatservice;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.v4.app.FragmentActivity;
import android.text.TextUtils;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.elex.chatservice.app.ActionBarActivity;
import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSIcons;
import com.elex.chatservice.util.HSRes;
import com.elex.chatservice.view.SimpleMenuItemCompat;

import org.json.JSONException;

class HSActivity2 extends FragmentActivity
{
  protected static final String TAG = "HelpShiftDebug";
  private final int ISSUE_POLL_DURATION = 3;
  protected static final int CALL_FINISH_REQ_CODE = 1;
  private Bundle extras;
  private boolean enableContactUs;
  private boolean showConvOnReportIssue;
  private MenuItem reportIssueAction;
  private TextView notifCount;
  private TextView convIcon;
  private Thread pollerThread;
  private Handler pollerThreadHandler;
  private HSStorage storage;
  private HSApiData data;
  private Menu conversationMenu = null;

  private void killPoller() {
    if (this.pollerThreadHandler != null)
      this.pollerThreadHandler.getLooper().quit();
  }

  protected void startPoller()
  {
    killPoller();

    String conversation = this.storage.getActiveConversation();
    if (TextUtils.isEmpty(conversation)) {
      return;
    }

    this.pollerThread = new Thread(new Runnable()
    {
      public void run() {
        Looper.prepare();
        pollerThreadHandler = new Handler() {
          public void handleMessage(Message msg) {
            runOnUiThread(new Runnable() {
              public void run() {
                updateCount();
              }
            });
          }
        };
        Runnable m_handlerTask = new Runnable()
        {
          public void run()
          {
            try {
              data.getLatestIssues(pollerThreadHandler, new Handler());
            } catch (JSONException e) {
              Log.d("HelpShiftDebug", "get issues", e);
            }
            pollerThreadHandler.postDelayed(this, 3000L);
          }
        };
        m_handlerTask.run();
        Looper.loop();
      }
    });
    this.pollerThread.start();
  }

  private void updateCount() {
    int count = this.storage.getActiveNotifCnt().intValue();
    if (this.notifCount != null)
      if (count > 0) {
        this.convIcon.setVisibility(8);
        this.notifCount.setVisibility(0);
        this.notifCount.setText("" + count);
      } else {
        this.convIcon.setVisibility(0);
        this.notifCount.setVisibility(8);
      }
  }

  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    this.extras = getIntent().getExtras();
    this.showConvOnReportIssue = this.extras.getBoolean("showConvOnReportIssue", false);
    this.data = new HSApiData(this);
    this.storage = this.data.storage;

//    if ((this instanceof HSQuestion))
//      this.enableContactUs = ContactUsFilter.showContactUs(ContactUsFilter.LOCATION.QUESTION_ACTION_BAR);
//    else
      this.enableContactUs = ContactUsFilter.showContactUs(ContactUsFilter.LOCATION.ACTION_BAR);
  }

  protected void onResume()
  {
    super.onResume();

//    if ((this instanceof HSQuestion))
//      this.enableContactUs = ContactUsFilter.showContactUs(ContactUsFilter.LOCATION.QUESTION_ACTION_BAR);
//    else {
      this.enableContactUs = ContactUsFilter.showContactUs(ContactUsFilter.LOCATION.ACTION_BAR);
//    }

    if (!this.enableContactUs) {
      showConversationMenu(false);
    } else if ((this.enableContactUs))// && (!(this instanceof HSConversation))) 
    {
      showConversationMenu(true);
      if (!TextUtils.isEmpty(this.storage.getIdentity())) {
        updateCount();
        startPoller();
      }
    }
  }

  protected void onPause()
  {
    super.onPause();
    killPoller();
  }

  public boolean onCreateOptionsMenu(Menu menu)
  {
    super.onCreateOptionsMenu(menu);

    this.conversationMenu = menu;
    if ((this.enableContactUs))// && (!(this instanceof HSConversation)))
    {
      showConversationMenu(menu);
    }

    return true;
  }

  private void startConversation() {
    Intent i = new Intent(this, HSConversation.class);
    i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(this));
    i.putExtra("chatLaunchSource", "support");
    i.putExtras(this.extras);
    i.removeExtra("isRoot");
    startActivityForResult(i, 1);
  }

  public void onStart()
  {
    super.onStart();
    HSAnalytics.onActivityStarted(this);
  }

  public void onStop()
  {
    super.onStop();
    HSAnalytics.onActivityStopped(this);
  }

  protected void showConversationMenu(boolean show) {
    if (this.reportIssueAction != null)
      this.reportIssueAction.setVisible(show);
    else if ((show) && (this.conversationMenu != null))
      showConversationMenu(this.conversationMenu);
  }

  private void showConversationMenu(Menu menu)
  {
    int convMenuId = HSRes.getId(this, "menu", "hs__show_conversation");
    int reportIssueActionId = HSRes.getId(this, "id", "hs__action_report_issue");
    int notifCountId = HSRes.getId(this, "id", "hs__notifcation_badge");
    int convIconId = HSRes.getId(this, "id", "hs__conversation_icon");

    MenuInflater menuInflater = getMenuInflater();
    menuInflater.inflate(convMenuId, menu);

    this.reportIssueAction = menu.findItem(reportIssueActionId);
    LinearLayout badgeLayout = (LinearLayout)SimpleMenuItemCompat.getActionView(this.reportIssueAction);

    if (badgeLayout == null) {
      return;
    }

    this.notifCount = ((TextView)badgeLayout.findViewById(notifCountId));
    this.convIcon = ((TextView)badgeLayout.findViewById(convIconId));

    HSIcons.applyMenuItemSelectedFilter(this, this.convIcon.getBackground());
    HSIcons.applyNotificationFilter(this, this.notifCount.getBackground());

    badgeLayout.setOnClickListener(new View.OnClickListener() {
      public void onClick(View v) {
        startConversation();
      }
    });
    updateCount();
  }

  protected void onActivityResult(int requestCode, int resultCode, Intent data)
  {
    super.onActivityResult(requestCode, resultCode, data);
    if (data != null) {
      Boolean callFinish = Boolean.valueOf(data.getBooleanExtra("callFinish", false));
      if ((requestCode == 1) && (resultCode == -1))
      {
//        if ((this instanceof HSConversation))
//          onBackPressed();
//        else 
        	if (callFinish.booleanValue())
          callFinish();
      }
    }
  }

  private void callFinish() {
    Intent returnIntent = new Intent();
    returnIntent.putExtra("callFinish", true);
    setResult(-1, returnIntent);
    finish();
  }
}