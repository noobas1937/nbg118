package com.elex.chatservice;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.annotation.TargetApi;
import android.app.NotificationManager;
import android.content.BroadcastReceiver;
import android.content.ClipData;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.provider.MediaStore;
import android.support.v4.app.Fragment;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.util.Log;
import android.view.ContextMenu;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.inputmethod.InputMethodManager;
import android.widget.AbsListView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.elex.chatservice.customadapters.MessagesAdapter;
import com.elex.chatservice.util.AttachmentUtil;
import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSErrors;
import com.elex.chatservice.util.HSIcons;
import com.elex.chatservice.util.HSJSONUtils;
import com.elex.chatservice.util.HSRes;
import com.elex.chatservice.viewstructs.HSMsg;
import com.lee.pullrefresh.ui.PullToRefreshBase;
import com.lee.pullrefresh.ui.PullToRefreshListView;
import com.lee.pullrefresh.ui.PullToRefreshBase.OnRefreshListener;

public class HSMessagesFragment extends Fragment
{
  private final BroadcastReceiver failedMessageRequestChecker;
  private final BroadcastReceiver connChecker;
  public static final String TAG = "HelpShiftDebug";
  private HSActivity activity;
  private Bundle extras;
  private String issueId;
  private MessagesAdapter adapter;
  private ArrayList<HSMsg> messagesList;
  private FrameLayout messagesListFrameLayout;
  private PullToRefreshListView countryPullListView;
  private ListView messagesListView;
  private ListView messagesListView2;
  private HSStorage hsStorage;
  private HSApiClient hsClient;
  private HSApiData hsApiData;
  private String ssMsgPos;
  private String chatLaunchSource;
  private HashSet<String> messageIdsSet;
  private HashSet<String> scReferIdsSet;
  private final int MESSAGE_POLL_DURATION = 3;
  private Thread pollerThread;
  private Handler pollerThreadHandler;
  private Boolean newIssue;
  private Boolean decomp;
  private Boolean showConvOnReportIssue;
  private boolean selectImage;
  private EditText replyField;
  private LinearLayout confirmationBox;
  private LinearLayout newConversationBox;
  private RelativeLayout messageBox;
  private LinearLayout header;
  private ImageButton solvedBtn;
  private ImageButton unsolvedBtn;
  private Button newConversationBtn;
  private boolean persistMessageBox;
  private boolean enableNCRMessage;
  private boolean newActivity;
  private MenuItem attachScreenshotMenu;
  private Handler fetchMessagesSuccess;
  private Handler fetchMessagesFailure;
  private Handler replyHandler;
  private Handler replyFailHandler;
  private TextView wordCount;
  private Button buttonCountry;
  private Button buttonAllie;
  private Button backButton;
  
  public HSMessagesFragment()
  {
	System.out.println("HSMessagesFragment");
    this.failedMessageRequestChecker = new BroadcastReceiver()
    {
      public void onReceive(Context context, Intent intent) {
    	  System.out.println("HSMessagesFragment failedMessageRequestChecker onReceive");
        refreshMessages();
      }
    };
    this.connChecker = new BroadcastReceiver()
    {
      public void onReceive(Context context, Intent intent) {
    	  System.out.println("HSMessagesFragment connChecker onReceive");
        startPoller();
      }
    };
    this.messagesList = new ArrayList();

    this.messageIdsSet = new HashSet();
    this.scReferIdsSet = new HashSet();

    this.selectImage = false;

    this.persistMessageBox = false;
    this.enableNCRMessage = true;
    this.newActivity = true;

    this.fetchMessagesSuccess = new Handler() {
      public void handleMessage(Message msg) {
        super.handleMessage(msg);
        JSONArray messages = (JSONArray)msg.obj;
        if (messages.length() > 0) {
        	System.out.println("fetchMessagesSuccess handleMessage");
          refreshMessages();
          refreshStatus();
          messagesListView.setSelection(adapter.getCount() - 1);
        }
      }
    };
    this.fetchMessagesFailure = new Handler() {
      public void handleMessage(Message msg) {
        super.handleMessage(msg);
        HashMap result = (HashMap)msg.obj;
        Integer status = (Integer)result.get("status");
        if ((status.intValue() != -1) && (pollerThreadHandler != null))
        {
          pollerThreadHandler.getLooper().quit();
        }
        HSErrors.showFailToast(status.intValue(), null, activity);
      }
    };
    this.replyHandler = new Handler() {
      public void handleMessage(Message msg) {
    	  System.out.println("replyHandler handleMessage");
        renderReplyMsg(msg);
      }
    };
    this.replyFailHandler = new Handler() {
      public void handleMessage(Message msg) {
        super.handleMessage(msg);
        if (pollerThreadHandler != null) {
          pollerThreadHandler.getLooper().quit();
        }
        System.out.println("replyFailHandler handleMessage");
        refreshMessages();
      }
    };
  }

  private void gotoApp(String marketUrl)
  {
	  System.out.println("gotoApp");
    Intent intent = new Intent("android.intent.action.VIEW");
    intent.setData(Uri.parse(marketUrl));
    if (isResumed())
      startActivity(intent);
  }

  private void renderReplyMsg(Message msg)
  {
	  System.out.println("renderReplyMsg");
    HashMap result = (HashMap)msg.obj;
    JSONObject message = (JSONObject)result.get("response");
    JSONArray messages = new JSONArray();
    messages.put(message);
    try
    {
      this.hsStorage.storeMessages(this.issueId, messages);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSONException", e);
    }

    refreshMessages();
  }

  private void refreshStatus()
  {
	  System.out.println("refreshStatus");
    try
    {
      List openConversations = this.hsStorage.getOpenConversations();
      JSONObject issue = this.hsStorage.getIssue(this.issueId);
      String status = issue.getString("status");
      int replyLength = this.hsStorage.getReply().trim().length();

      System.out.println("status:"+status+"this.issueId:"+this.issueId);
      if ((status.equals("0")) || (status.equals("1")))
      {
        showMessageBox();
        this.persistMessageBox = false;
      } else if (status.equals("2")) {
        if (openConversations.contains(this.issueId))
          showConfirmationBox();
        else if ((this.persistMessageBox) || (replyLength != 0) || (this.hsStorage.getScreenShotDraft().booleanValue()))
        {
          showMessageBox();
        }
        else showNewConversationBox(); 
      }
    }
    catch (JSONException e)
    {
      Log.d("HelpShiftDebug", "refresh status", e);
    }
  }

  private void showConfirmationBox() {
	  System.out.println("showConfirmationBox");
    this.adapter.enableButtons(false);
    RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams)this.confirmationBox.getLayoutParams();

    if ((!this.persistMessageBox) || (this.replyField.getText().toString().trim().length() == 0))
    {
      this.messageBox.setVisibility(8);
      hideKeyboard(this.replyField);
      params.addRule(12);
    } else {
      params.addRule(12, 0);
    }

    this.confirmationBox.setLayoutParams(params);
    this.confirmationBox.setVisibility(0);
    this.newConversationBox.setVisibility(8);
    this.header.setVisibility(View.VISIBLE);

    if (this.attachScreenshotMenu != null)
      this.attachScreenshotMenu.setVisible(false);
  }

  private void showNewConversationBox()
  {
	  System.out.println("showNewConversationBox");
    this.enableNCRMessage = false;
    this.adapter.enableButtons(false);
    hideKeyboard(this.replyField);
    this.confirmationBox.setVisibility(8);
    this.newConversationBox.setVisibility(0);
    this.messageBox.setVisibility(8);
    this.header.setVisibility(View.VISIBLE);
    
    if (this.attachScreenshotMenu != null)
      this.attachScreenshotMenu.setVisible(false);
  }

  private void showMessageBox()
  {
	  System.out.println("showMessageBox");
    this.adapter.enableButtons(true);
    this.confirmationBox.setVisibility(8);
    this.newConversationBox.setVisibility(8);
    this.messageBox.setVisibility(0);
    this.header.setVisibility(View.VISIBLE);
    refreshWordCount();
    
    if (this.attachScreenshotMenu != null)
      this.attachScreenshotMenu.setVisible(true);
  }

  private void showKeyboard(View v)
  {
	  System.out.println("showKeyboard(View v)");
    v.requestFocus();
    InputMethodManager imm = (InputMethodManager)this.activity.getSystemService("input_method");
    imm.showSoftInput(v, 0);
  }

  private void hideKeyboard(View v) {
	  System.out.println("hideKeyboard(View v)");
    InputMethodManager imm = (InputMethodManager)this.activity.getSystemService("input_method");
    imm.hideSoftInputFromWindow(v.getWindowToken(), 0);
  }

  private void refreshMessages() {
	  System.out.println("refreshMessages");
    JSONArray messages = this.hsApiData.getMessagesWithFails(this.issueId);
    refreshMessages(messages);
  }

  private void refreshMessages(JSONArray messages) {
	  
	  System.out.println("refreshMessages(JSONArray messages)");
    try {
      this.messagesList.clear();
      this.messageIdsSet.clear();
      this.scReferIdsSet.clear();
      int messagesLength = messages.length();

      for (int i = 0; i < messagesLength; i++)
      {
        JSONObject message = messages.getJSONObject(i);
        String id = message.getString("id");
        String type = message.getString("type");
        String origin = message.getString("origin");
        String body = message.getString("body");
        String date = message.getString("created_at");
        int state = message.optInt("state", 0);
        Boolean inProgress = Boolean.valueOf(message.optBoolean("inProgress", false));

        
        
        if ((!origin.equals("mobile")) || (!type.equals("ncr")) || (this.enableNCRMessage) || (i != messagesLength - 1))
        {
          String screenshot = "";
          if (type.equals("rsc")) {
            screenshot = message.optString("screenshot", "");
            if ((id.startsWith("localRscMessage_")) && (this.scReferIdsSet.contains(id)))
            {
              this.hsStorage.deleteMessage(this.issueId, id);
              continue;
            }
          }

          Boolean metaResponse = Boolean.valueOf(false);
          JSONObject meta = message.optJSONObject("meta");

          if (meta != null) {
            JSONObject messageMeta = meta.optJSONObject("response");
            if (messageMeta != null) {
              metaResponse = Boolean.valueOf(messageMeta.optBoolean("state"));
            }
          }

          Boolean invisible = Boolean.valueOf((message.optBoolean("invisible")) || (metaResponse.booleanValue()));

          if (((!origin.equals("mobile")) || (!type.equals("ca"))) && (!this.messageIdsSet.contains(id)))
          {
        	  System.out.println("this.messagesList.add");
            this.messageIdsSet.add(id);
            System.out.println("id:"+id+" type:"+type+" origin:"+origin+" body:"+body+" date:"+date+" invisible:"+invisible+" screenshot:"+screenshot+" state:"+state+" inProgress:"+inProgress);
            this.messagesList.add(new HSMsg(id, type, origin, body, date, invisible, screenshot, state, inProgress));

            if (type.equals("sc")) {
              meta = null;
              meta = message.optJSONObject("meta");
              if (meta != null) {
                JSONArray attachments = meta.optJSONArray("attachments");

                String refers = meta.optString("refers", id);

                System.out.println("refers:"+refers);
                if (refers.startsWith("localRscMessage_")) {
                  if (this.messageIdsSet.contains(refers)) {
                    removeMessage(refers);
                    this.hsStorage.deleteMessage(this.issueId, refers);
                  } else {
                    this.scReferIdsSet.add(refers);
                  }
                }

                if ((attachments != null) && (attachments.length() > 0) && (refers != null))
                {
                  JSONObject attachment = attachments.optJSONObject(0);
                  if (attachment != null) {
                    String url = attachment.optString("url", "");
                    HashMap imgData = new HashMap();
                    imgData.put("url", url);
                    imgData.put("messageId", refers);
                    imgData.put("attachId", Integer.valueOf(0));
                    imgData.put("position", Integer.valueOf(this.messagesList.size() - 1));
                    new DownloadImagesTask().execute(new HashMap[] { imgData });
                  }
                } else if (attachments == null) {
                  removeMessage(id);
                }
              }
            }
          }
        }
      }
    } catch (JSONException e) { Log.d("HelpShiftDebug", "Slug in get(\"slug\") no found", e); }


    if (messages.length() > 0)
      this.adapter.notifyDataSetChanged();
  }

  private void removeMessage(String messageId)
  {
	  System.out.println("removeMessage(String messageId)");
    Iterator it = this.messagesList.iterator();
    while (it.hasNext()) {
      HSMsg msg = (HSMsg)it.next();
      if (msg.id.equals(messageId)) {
        it.remove();
      }
    }
    this.messageIdsSet.remove(messageId);
  }

  public void onPause()
  {
	  System.out.println("onPause");
    super.onPause();
    if (this.pollerThreadHandler != null)
      this.pollerThreadHandler.getLooper().quit();
    try
    {
      this.hsStorage.resetIssueCount(this.issueId);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", e.getMessage(), e);
    }
    this.hsStorage.setForeground(Boolean.valueOf(false));
    this.hsStorage.setForegroundIssue("");
    this.hsApiData.resetServiceInterval();
    this.activity.unregisterReceiver(this.connChecker);
    this.activity.unregisterReceiver(this.failedMessageRequestChecker);
    this.hsStorage.storeReply(this.replyField.getText().toString().trim());
  }

  public void startPoller()
  {
	  System.out.println("startPoller");
    if (this.pollerThreadHandler != null) {
      this.pollerThreadHandler.getLooper().quit();
      this.pollerThread = null;
    }

    String conversation = this.hsStorage.getActiveConversation();
    String archivedConversation = this.hsStorage.getArchivedConversation();
    if ((TextUtils.isEmpty(conversation)) && (TextUtils.isEmpty(archivedConversation))) {
      return;
    }

    this.pollerThread = new Thread(new Runnable()
    {
      public void run() {
        Looper.prepare();
        pollerThreadHandler = new Handler() {
          public void handleMessage(Message msg) {
            Message newMsg = fetchMessagesSuccess.obtainMessage();
            System.out.println("pollerThreadHandler handleMessage");
            newMsg.obj = msg.obj;
            fetchMessagesSuccess.sendMessage(newMsg);
          }
        };
        Runnable m_handlerTask = new Runnable()
        {
          public void run() {
            try {
            	System.out.println("m_handlerTask getLatestIssues");
              hsApiData.getLatestIssues(pollerThreadHandler, fetchMessagesFailure, chatLaunchSource);
            }
            catch (JSONException e)
            {
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

  public void onResume()
  {
	  System.out.println("onResume");
    super.onResume();
    NotificationManager notificationManager = (NotificationManager)getActivity().getSystemService("notification");

    notificationManager.cancel(this.issueId, 1);

    IntentFilter connFilter = new IntentFilter();
    connFilter.addAction("android.net.conn.CONNECTIVITY_CHANGE");
    this.activity.registerReceiver(this.connChecker, connFilter);

    IntentFilter failFilter = new IntentFilter();
    failFilter.addAction("com.elex.chatservice.failedMessageRequest");
    this.activity.registerReceiver(this.failedMessageRequestChecker, failFilter);

    startPoller();
    try
    {
      this.hsStorage.resetIssueCount(this.issueId);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", e.toString(), e);
    }

    this.hsStorage.setForeground(Boolean.valueOf(true));
    this.hsStorage.setForegroundIssue(this.issueId);
    this.hsApiData.updateMessageSeenState(this.issueId, this.chatLaunchSource);
    String replyText = this.hsStorage.getReply();
    this.replyField.setText(replyText);
    
    if ((this.newActivity) && (TextUtils.isEmpty(replyText)))
    {
      this.persistMessageBox = false;
    }
    this.newActivity = false;

    if (Build.VERSION.SDK_INT < 11)
      if ((this.messageBox != null) && (this.messageBox.getVisibility() == 0))
      {
        this.attachScreenshotMenu.setVisible(true);
      }
      else this.attachScreenshotMenu.setVisible(false);
  }

  public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
  {
	System.out.println("HSMessagesFragment onCreateView");
    this.activity = ((HSActivity)getActivity());

//    this.activity.setSupportProgressBarIndeterminateVisibility(false);
    this.extras = getArguments();
    this.extras.remove("message");

    this.hsApiData = new HSApiData(this.activity);
    this.hsStorage = this.hsApiData.storage;
    this.hsClient = this.hsApiData.client;

    this.newIssue = Boolean.valueOf(this.extras.getBoolean("newIssue", false));
    this.decomp = Boolean.valueOf(this.extras.getBoolean("decomp", false));
    this.chatLaunchSource = this.extras.getString("chatLaunchSource");
    this.showConvOnReportIssue = Boolean.valueOf(this.extras.getBoolean("showConvOnReportIssue"));

    setHasOptionsMenu(true);

    return inflater.inflate(HSRes.getId(this, "layout", "cs__messages_fragment"), container, false);
  }
  
  private FrameLayout.LayoutParams getLayoutParams()
  {
	  FrameLayout.LayoutParams param = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
	  param.gravity = Gravity.CENTER;
	  return param;
  }

  private SimpleDateFormat mDateFormat = new SimpleDateFormat("MM-dd HH:mm");
  private boolean mIsStart = true;
  private int mCurIndex = 0;
  private static final int mLoadDataCount = 100;
  
  private String formatDateTime(long time) {
      if (0 == time) {
          return "";
      }
      
      return mDateFormat.format(new Date(time));
  }

  private void setLastUpdateTime() {
      String text = formatDateTime(System.currentTimeMillis());
      countryPullListView.setLastUpdatedLabel(text);
  }
  public static final String[] mStrings = {
      "Abbaye de Belloc", "Abbaye du Mont des Cats", "Abertam"};
  private class GetDataTask extends AsyncTask<Void, Void, String[]> {

      @Override
      protected String[] doInBackground(Void... params) {
          // Simulates a background job.
          try {
              Thread.sleep(3000);
          } catch (InterruptedException e) {
          }
          return mStrings;
      }

      @Override
      protected void onPostExecute(String[] result) {
          boolean hasMoreData = true;
//          if (mIsStart) {
//              mListItems.addFirst("Added after refresh...");
//          } else {
//              int start = mCurIndex;
//              int end = mCurIndex + mLoadDataCount;
//              if (end >= mStrings.length) {
//                  end = mStrings.length;
//                  hasMoreData = false;
//              }
//              
//              for (int i = start; i < end; ++i) {
//                  mListItems.add(mStrings[i]);
//              }
//              
//              mCurIndex = end;
//          }
          
//          adapter.notifyDataSetChanged();
          countryPullListView.onPullDownRefreshComplete();
//          countryPullListView.onPullUpRefreshComplete();
          countryPullListView.setHasMoreData(hasMoreData);
          setLastUpdateTime();

          super.onPostExecute(result);
      }
  }
  
  public void onViewCreated(View view, Bundle savedInstanceState)
  {
	System.out.println("HSMessagesFragment onViewCreated");
    super.onViewCreated(view, savedInstanceState);
    
    this.messagesListFrameLayout = (FrameLayout)view.findViewById(HSRes.getId(this.activity, "id", "hs__messagesListLayout"));
    
    countryPullListView = new PullToRefreshListView(activity);
    countryPullListView.setLayoutParams(getLayoutParams());
    messagesListFrameLayout.addView(countryPullListView);
    countryPullListView.setPullLoadEnabled(false);
    countryPullListView.setScrollLoadEnabled(false);
    messagesListView = countryPullListView.getRefreshableView();
    messagesListView.setLayoutParams(getLayoutParams());
    messagesListView.setScrollingCacheEnabled(true);
    messagesListView.setTranscriptMode(AbsListView.TRANSCRIPT_MODE_ALWAYS_SCROLL);

    countryPullListView.setOnRefreshListener(new OnRefreshListener<ListView>() {
        @Override
        public void onPullDownToRefresh(PullToRefreshBase<ListView> refreshView) {
            mIsStart = true;
            new GetDataTask().execute();
        }

        @Override
        public void onPullUpToRefresh(PullToRefreshBase<ListView> refreshView) {
            mIsStart = false;
            new GetDataTask().execute();
        }
    });
    setLastUpdateTime();
    
//    this.messagesListView = ((ListView)view.findViewById(HSRes.getId(this.activity, "id", "hs__messagesList")));
    this.messagesListView2 = ((ListView)view.findViewById(HSRes.getId(this.activity, "id", "hs__messagesList2")));
    this.replyField = ((EditText)view.findViewById(HSRes.getId(this.activity, "id", "hs__messageText")));
    this.wordCount = ((TextView)view.findViewById(HSRes.getId(this.activity, "id", "wordCountTextView")));
    
    final Button addReply = (Button)view.findViewById(HSRes.getId(this.activity, "id", "hs__sendMessageBtn"));

    this.confirmationBox = ((LinearLayout)view.findViewById(HSRes.getId(this.activity, "id", "hs__confirmation")));

    this.newConversationBox = ((LinearLayout)view.findViewById(HSRes.getId(this.activity, "id", "hs__new_conversation")));

    this.messageBox = ((RelativeLayout)view.findViewById(HSRes.getId(this.activity, "id", "relativeLayout1")));
    header = ((LinearLayout)view.findViewById(HSRes.getId(this.activity, "id", "headerRelativeLayout")));
    
    this.solvedBtn = ((ImageButton)view.findViewById(16908313));
    this.unsolvedBtn = ((ImageButton)view.findViewById(16908314));
    this.newConversationBtn = ((Button)view.findViewById(HSRes.getId(this.activity, "id", "hs__new_conversation_btn")));
    buttonCountry = (Button)view.findViewById(HSRes.getId(this.activity, "id", "buttonCountry"));
    buttonAllie = (Button)view.findViewById(HSRes.getId(this.activity, "id", "buttonAllie"));
    changeAlpha(buttonCountry, true);
    changeAlpha(buttonAllie, false);
    backButton = (Button)view.findViewById(HSRes.getId(this.activity, "id", "backButton"));
    
    HSIcons.applyTextColorPrimaryFilter(this.activity, this.newConversationBtn.getCompoundDrawables()[0]);

    HSIcons.applyConfirmAcceptFilter(this.activity, this.solvedBtn.getDrawable());
    HSIcons.applyConfirmRejectFilter(this.activity, this.unsolvedBtn.getDrawable());

    this.solvedBtn.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View view) {
    	  System.out.println("solvedBtn onClick");
        replyField.setText("");
        hsStorage.storeReply("");
        sendResolutionEvent(Boolean.valueOf(true));
        persistMessageBox = false;
        refreshStatus();
      }
    });
    this.unsolvedBtn.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View view) {
    	  System.out.println("unsolvedBtn onClick");
        refreshMessages();
        persistMessageBox = true;
        showMessageBox();
        if (replyField.getText().toString().trim().length() == 0) {
          showKeyboard(replyField);
        }
        sendResolutionEvent(Boolean.valueOf(false));
      }
    });
    this.newConversationBtn.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View view) {
    	  System.out.println("newConversationBtn onClick");
        Intent i = new Intent(activity, HSConversation.class);
        i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(activity));
        i.putExtra("newConversation", true);
        i.putExtras(extras);
        i.removeExtra("isRoot");
        getActivity().startActivityForResult(i, 1);
        hsStorage.setArchivedConversation("");
      }
    });
    this.messagesListView.setDivider(null);
    this.adapter = new MessagesAdapter(this, 17367043, this.messagesList);
    this.messagesListView.setAdapter(this.adapter);
    try
    {
      this.issueId = this.extras.getString("issueId");
      refreshStatus();
      refreshMessages();
      this.messagesListView.setSelection(this.adapter.getCount() - 1);

      JSONObject issue = this.hsStorage.getIssue(this.issueId);

//      ActionBarHelper actionBarHelper = this.activity.getActionBarHelper();
//      actionBarHelper.setDisplayHomeAsUpEnabled(true);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", e.toString(), e);
    }
    
    messagesListView2.setDivider(null);
    messagesListView2.setVisibility(View.GONE);

    if (this.replyField.getText().length() == 0) {
      addReply.setEnabled(false);
      changeAlpha(addReply, false);
//      HSIcons.applyTextColorPrimaryFilter(this.activity, addReply.getDrawable());
    } else {
      addReply.setEnabled(true);
      changeAlpha(addReply, true);
//      HSIcons.applyAttachFilter(this.activity, addReply.getDrawable());
    }

    buttonCountry.setOnClickListener(new View.OnClickListener()
    {
        public void onClick(View view) {
        	System.out.println("buttonCountry onClick");
//        	buttonCountry.setEnabled(true);
            changeAlpha(buttonCountry, true);
//        	buttonAllie.setEnabled(false);
            changeAlpha(buttonAllie, false);
            countryPullListView.setVisibility(View.VISIBLE);
            messagesListView2.setVisibility(View.GONE);
        }
    });
    buttonAllie.setOnClickListener(new View.OnClickListener()
    {
        public void onClick(View view) {
        	System.out.println("buttonAllie onClick");
//        	buttonCountry.setEnabled(false);
            changeAlpha(buttonCountry, false);
//        	buttonAllie.setEnabled(true);
            changeAlpha(buttonAllie, true);
            countryPullListView.setVisibility(View.GONE);
            messagesListView2.setVisibility(View.VISIBLE);
        }
    });
    backButton.setOnClickListener(new View.OnClickListener()
    {
        public void onClick(View view) {
        	System.out.println("backButton onClick");
        }
    });
    
    addReply.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View view) {
        String replyText = replyField.getText().toString().trim();
        String identity = hsStorage.getIdentity();

        System.out.println("addReply onClick");
        if (!TextUtils.isEmpty(replyText)) {
          replyField.setText("");
          addMessage(replyHandler, replyFailHandler, issueId, replyText, "txt", "");
          try
          {
            JSONObject eventData = new JSONObject();
            eventData.put("type", "txt");
            eventData.put("body", replyText);
            eventData.put("id", issueId);
            HSFunnel.pushEvent("m", eventData);
          } catch (JSONException e) {
            Log.d("HelpShiftDebug", "JSONException", e);
          }
        }
      }
    });
    this.replyField.setOnEditorActionListener(new TextView.OnEditorActionListener() {
      public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
        if (actionId == 4) {
          addReply.performClick();
        }
        return false;
      }
    });
    this.replyField.addTextChangedListener(new TextWatcher()
    {
      public void afterTextChanged(Editable s) {
          replyField.post(new Runnable() {
              @Override
              public void run() {
            	  System.out.println("afterTextChanged");
              	refreshWordCount();
              }
          });
      }

      public void beforeTextChanged(CharSequence s, int start, int count, int after) {
      }

      @TargetApi(Build.VERSION_CODES.HONEYCOMB)
      public void onTextChanged(CharSequence s, int start, int before, int count) {
        persistMessageBox = true;
        System.out.println("onTextChanged");
        if (s.length() == 0) {
          addReply.setEnabled(false);
          changeAlpha(addReply, false);
//          addReply.setAlpha(64);
//          HSIcons.applyTextColorPrimaryFilter(activity, addReply.getDrawable());
        } else {
          addReply.setEnabled(true);
          changeAlpha(addReply, true);
//          addReply.setAlpha(255);
//          HSIcons.applyAttachFilter(activity, addReply.getDrawable());
        }
      }
    });
  }
  
  private void refreshWordCount()
  {
	  System.out.println("refreshWordCount");
      if(replyField.getLineCount()>2)
      {
      	wordCount.setVisibility(View.VISIBLE);
      }else{
      	wordCount.setVisibility(View.GONE);
      }
      wordCount.setText(replyField.getText().length() + "/500");
  }

  @TargetApi(Build.VERSION_CODES.HONEYCOMB)
  private void changeAlpha(Button button, boolean enabled)
  {
	  System.out.println("changeAlpha");
		if (Build.VERSION.SDK_INT >= 11) {
			if(enabled){
				System.out.println("changeAlpha enabled");
				button.setAlpha(1.0f);
//				button.getBackground().setAlpha(255);
			}else{
				button.setAlpha(0.4f);
			}
		} else {
			
		}
  }
  
  private void sendResolutionEvent(Boolean accepted) {
	  System.out.println("sendResolutionEvent");
    this.hsStorage.clearAndUpdateActiveConversation(this.issueId);
    try
    {
      JSONObject eventData = new JSONObject();
      eventData.put("id", this.issueId);
      String messageType = "";
      if (accepted.booleanValue()) {
        HSFunnel.pushEvent("y", eventData);
        messageType = "ca";
      } else {
        HSFunnel.pushEvent("n", eventData);
        messageType = "ncr";
      }
      addMessage(this.replyHandler, this.replyFailHandler, this.issueId, "", messageType, "");
    }
    catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSONException", e);
    }
  }

  public boolean onOptionsItemSelected(MenuItem item)
  {
	  System.out.println("onOptionsItemSelected");
    int id = item.getItemId();
    if (id == 16908332) {
      getActivity().onBackPressed();
      return true;
    }if (id == HSRes.getId(this, "id", "hs__attach_screenshot")) {
      selectImagePopup(0);
      return true;
    }
    return super.onOptionsItemSelected(item);
  }

  public void onStart()
  {
	  System.out.println("onStart");
    super.onStart();
    if (!this.selectImage) {
      HSAnalytics.onActivityStarted(this.activity);
      try {
        JSONObject eventData = new JSONObject();
        eventData.put("id", this.issueId);
        HSFunnel.pushEvent("c", eventData);
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", "JSONException", e);
      }
    }
    this.selectImage = false;
  }

  public void onStop()
  {
	  System.out.println("onStop");
    super.onStop();
    if (!this.selectImage)
      HSAnalytics.onActivityStopped(this.activity);
  }

  public void replyConfirmation(String messageId, Boolean accepted, final int position)
  {
	  System.out.println("replyConfirmation");
    HSMsg msgData = (HSMsg)this.messagesList.get(position);
    msgData.clickable = Boolean.valueOf(false);
    msgData.inProgress = Boolean.valueOf(true);
    try {
      this.hsApiData.setInProgress(msgData.id, this.issueId, Boolean.valueOf(true));
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", e.getMessage(), e);
    }

    this.adapter.notifyDataSetChanged();

    Handler replySysHandler = new Handler() {
      public void handleMessage(Message msg) {
    	  System.out.println("replyConfirmation replySysHandler handleMessage");
        HSMsg msgData = (HSMsg)messagesList.get(position);
        msgData.clickable = Boolean.valueOf(false);
        msgData.invisible = Boolean.valueOf(true);
        msgData.inProgress = Boolean.valueOf(false);
        adapter.notifyDataSetChanged();
        try {
          hsApiData.setInvisible(msgData.id, issueId);
          hsApiData.setInProgress(msgData.id, issueId, Boolean.valueOf(false));
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", e.getMessage(), e);
        }
        renderReplyMsg(msg);
      }
    };
    Handler replySysFailHandler = new Handler() {
      public void handleMessage(Message msg) {
    	  System.out.println("replyConfirmation replySysFailHandler handleMessage");
        HSMsg msgData = (HSMsg)messagesList.get(position);
        msgData.clickable = Boolean.valueOf(true);
        msgData.inProgress = Boolean.valueOf(false);
        try {
          hsApiData.setInProgress(msgData.id, issueId, Boolean.valueOf(false));
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", e.getMessage(), e);
        }

        adapter.notifyDataSetChanged();

        HashMap result = (HashMap)msg.obj;
        Integer status = (Integer)result.get("status");
        HSErrors.showFailToast(status.intValue(), null, activity);
      }
    };
    if (accepted.booleanValue()) {
      addMessage(replySysHandler, replySysFailHandler, this.issueId, "", "ca", messageId);
    }
    else {
      addMessage(replySysHandler, replySysFailHandler, this.issueId, "", "ncr", messageId);
    }

    try
    {
      JSONObject eventData = new JSONObject();
      eventData.put("id", this.issueId);
      if (accepted.booleanValue())
        HSFunnel.pushEvent("y", eventData);
      else
        HSFunnel.pushEvent("n", eventData);
    }
    catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSONException", e);
    }
  }

  public void replyReview(String messageId, final int position) {
	  System.out.println("replyReview");
    HSMsg msgData = (HSMsg)this.messagesList.get(position);
    msgData.clickable = Boolean.valueOf(false);
    msgData.inProgress = Boolean.valueOf(true);
    try {
      this.hsApiData.setInProgress(msgData.id, this.issueId, Boolean.valueOf(true));
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", e.getMessage(), e);
    }

    this.adapter.notifyDataSetChanged();

    Handler replySysHandler = new Handler() {
      public void handleMessage(Message msg) {
    	  System.out.println("replyReview replySysHandler handleMessage");
        HSMsg msgData = (HSMsg)messagesList.get(position);
        msgData.clickable = Boolean.valueOf(false);
        msgData.invisible = Boolean.valueOf(true);
        msgData.inProgress = Boolean.valueOf(false);
        adapter.notifyDataSetChanged();
        try {
          hsApiData.setInProgress(msgData.id, issueId, Boolean.valueOf(false));
          hsApiData.setInvisible(msgData.id, issueId);
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", e.getMessage(), e);
        }
        renderReplyMsg(msg);
        try {
          JSONObject eventData = new JSONObject();
          eventData.put("type", "conversation");
          HSFunnel.pushEvent("r", eventData);

          String rurl = hsStorage.getConfig().optString("rurl", "");
          rurl = rurl.trim();
          if (!TextUtils.isEmpty(rurl)) {
            hsApiData.disableReview();
            gotoApp(rurl);
          }
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", e.getMessage(), e);
        }
      }
    };
    Handler replySysFailHandler = new Handler() {
      public void handleMessage(Message msg) {
    	  System.out.println("replyReview replySysFailHandler handleMessage");
        HSMsg msgData = (HSMsg)messagesList.get(position);
        msgData.clickable = Boolean.valueOf(true);
        msgData.inProgress = Boolean.valueOf(false);
        try {
          hsApiData.setInProgress(msgData.id, issueId, Boolean.valueOf(false));
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", e.getMessage(), e);
        }

        adapter.notifyDataSetChanged();
        HashMap result = (HashMap)msg.obj;
        Integer status = (Integer)result.get("status");
        HSErrors.showFailToast(status.intValue(), null, activity);
      }
    };
    addMessage(replySysHandler, replySysFailHandler, this.issueId, "", "ar", messageId);
  }

  public void onActivityResult(int requestCode, int resultCode, Intent imageReturnedIntent)
  {
	  System.out.println("onActivityResult");
    super.onActivityResult(requestCode, resultCode, imageReturnedIntent);

    if (-1 == resultCode) {
      String screenshotPath = AttachmentUtil.getPath(getActivity(), imageReturnedIntent);
      if (!TextUtils.isEmpty(screenshotPath))
      {
        try
        {
          HSMsg msgData;
          if (requestCode == 0) {
            msgData = AttachmentUtil.addAndGetLocalRscMsg(this.hsStorage, this.issueId, screenshotPath);
            this.messagesList.add(msgData);
          } else {
            msgData = (HSMsg)this.messagesList.get(requestCode);
            msgData.screenshot = screenshotPath;
          }

          this.hsApiData.setScreenshot(msgData.id, this.issueId, screenshotPath);
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", e.getMessage(), e);
        }
        this.adapter.notifyDataSetChanged();
      }
    }
  }

  public void removeScreenshot(int position) {
	  System.out.println("removeScreenshot");
    HSMsg msgData = (HSMsg)this.messagesList.get(position);
    try
    {
      if (msgData.id.startsWith("localRscMessage_")) {
        this.hsStorage.deleteMessage(this.issueId, msgData.id);
        this.messagesList.remove(position);
      } else {
        this.hsApiData.setScreenshot(msgData.id, this.issueId, "");
        msgData.screenshot = "";
      }
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "remove screenshot", e);
    }

    this.adapter.notifyDataSetChanged();
  }

  public void selectImagePopup(int position) {
	  System.out.println("selectImagePopup");
    this.hsStorage.setScreenShotDraft(Boolean.valueOf(true));
    this.selectImage = true;
    Intent i = new Intent("android.intent.action.PICK", MediaStore.Images.Media.EXTERNAL_CONTENT_URI);

    startActivityForResult(i, position);
  }

  public void attachImage(final int position) {
	  System.out.println("attachImage");
    String identity = this.hsStorage.getIdentity();
    HSMsg msgData = (HSMsg)this.messagesList.get(position);
    msgData.clickable = Boolean.valueOf(false);
    msgData.inProgress = Boolean.valueOf(true);
    try {
      this.hsApiData.setInProgress(msgData.id, this.issueId, Boolean.valueOf(true));
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", e.getMessage(), e);
    }

    this.adapter.notifyDataSetChanged();

    Handler replySysHandler = new Handler() {
      public void handleMessage(Message msg) {
    	  
    	  System.out.println("attachImage replySysHandler handleMessage");
        HSMsg msgData = (HSMsg)messagesList.get(position);
        HashMap result = (HashMap)msg.obj;
        JSONObject message = (JSONObject)result.get("response");
        try
        {
          JSONObject eventData = new JSONObject();
          eventData.put("type", "url");
          eventData.put("body", message.getJSONObject("meta").getJSONArray("attachments").getJSONObject(0).getString("url"));
          eventData.put("id", issueId);
          HSFunnel.pushEvent("m", eventData);
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", "Error while getting screenshot url", e);
        }

        try
        {
          AttachmentUtil.copyAttachment(getActivity(), hsApiData, msgData.screenshot, msgData.id, 0);
        }
        catch (IOException e) {
          Log.d("HelpShiftDebug", "Saving uploaded screenshot", e);
        }

        try
        {
          if (msgData.id.startsWith("localRscMessage_")) {
            hsStorage.deleteMessage(issueId, msgData.id);
            messagesList.remove(position);
          } else {
            msgData.inProgress = Boolean.valueOf(false);
            msgData.invisible = Boolean.valueOf(true);
            msgData.screenshot = "";
            msgData.clickable = Boolean.valueOf(false);

            hsApiData.setInProgress(msgData.id, issueId, Boolean.valueOf(false));
            hsApiData.setInvisible(msgData.id, issueId);
            hsApiData.setScreenshot(msgData.id, issueId, "");
          }
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", e.getMessage(), e);
        }

        adapter.notifyDataSetChanged();
        renderReplyMsg(msg);
      }
    };
    Handler replySysFailHandler = new Handler() {
      public void handleMessage(Message msg) {
    	  
    	  System.out.println("attachImage replySysFailHandler handleMessage");
        HSMsg msgData = (HSMsg)messagesList.get(position);
        msgData.clickable = Boolean.valueOf(true);
        msgData.inProgress = Boolean.valueOf(false);
        try {
          hsApiData.setInProgress(msgData.id, issueId, Boolean.valueOf(false));
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", e.getMessage(), e);
        }

        adapter.notifyDataSetChanged();
        messagesList.set(position, msgData);
        HashMap result = (HashMap)msg.obj;
        Integer status = (Integer)result.get("status");
        HSErrors.showFailToast(status.intValue(), null, activity);
      }
    };
    this.hsClient.addScMessage(replySysHandler, replySysFailHandler, identity, this.issueId, "", "sc", msgData.id, msgData.screenshot);
  }

  public String downloadAttachment(String urlStr, String messageId, int attachId)
    throws IOException
  {
	  System.out.println("downloadAttachment");
    URL url = new URL(urlStr);
    InputStream input = null;
    FileOutputStream output = null;
    try {
      String outputName = messageId + attachId + "-thumbnail";
      File outputFile = new File(this.activity.getFilesDir(), outputName);
      String fname = outputFile.getAbsolutePath();
      int read;
      if (!outputFile.exists()) {
        this.hsApiData.storeFile(outputName);
        input = url.openConnection().getInputStream();
        output = this.activity.openFileOutput(outputName, 0);

        byte[] data = new byte[1024];
        while ((read = input.read(data)) != -1) {
          output.write(data, 0, read);
        }
      }
      return fname;
    } finally {
      if (output != null)
        output.close();
      if (input != null)
        input.close();
    }
  }

  @TargetApi(Build.VERSION_CODES.HONEYCOMB)
  public void onCreateContextMenu(ContextMenu menu, View v, ContextMenu.ContextMenuInfo menuInfo)
  {
	  System.out.println("onCreateContextMenu");
    menu.add(0, v.getId(), 0, "Copy");

    TextView copyTextView = (TextView)v;
    String copyText = copyTextView.getText().toString();

    if (Build.VERSION.SDK_INT >= 13) {
      android.content.ClipboardManager clipboard = (android.content.ClipboardManager)this.activity.getSystemService("clipboard");
      ClipData clip = ClipData.newPlainText("Copy Text", copyText);
      clipboard.setPrimaryClip(clip);
    } else {
      android.text.ClipboardManager clipboard = (android.text.ClipboardManager)this.activity.getSystemService("clipboard");
      clipboard.setText(copyText);
    }
  }

  public void onCreateOptionsMenu(Menu menu, MenuInflater inflater)
  {
	  System.out.println("onCreateOptionsMenu");
    inflater.inflate(HSRes.getId(this, "menu", "hs__messages_menu"), menu);

    this.attachScreenshotMenu = menu.findItem(HSRes.getId(this, "id", "hs__attach_screenshot"));

    if ((this.messageBox != null) && (this.messageBox.getVisibility() == 0))
    {
      this.attachScreenshotMenu.setVisible(true);
    }
    else this.attachScreenshotMenu.setVisible(false);
  }

  public void retryMessage(final String id)
  {
	  System.out.println("retryMessage");
    try
    {
      JSONObject failedMessage = this.hsApiData.storage.popFailedMessage(id);
      if (failedMessage == null) {
        return;
      }

      JSONObject tempMess = new JSONObject(failedMessage, HSJSONUtils.getJSONObjectKeys(failedMessage));

      tempMess.put("state", 1);

      JSONArray messages = this.hsApiData.getMessagesWithFails(this.issueId);
      messages.put(tempMess);
      refreshMessages(messages);

      this.hsApiData.storage.storeFailedMessage(tempMess);

      Handler addMessageHandler = new Handler() {
        public void handleMessage(Message msg) {
        	
        	System.out.println("retryMessage addMessageHandler handleMessage");
          super.handleMessage(msg);
          try {
            hsApiData.storage.popFailedMessage(id);
          } catch (JSONException e) {
            Log.d("HelpShiftDebug", "addMessageHandler", e);
          }
          refreshMessages();
        }
      };
      addMessage(addMessageHandler, addMessageHandler, failedMessage.getString("issue_id"), failedMessage.getString("body"), failedMessage.getString("type"), failedMessage.getString("refers"), failedMessage.optInt("state", 0) - 1);
    }
    catch (JSONException e)
    {
      Log.d("HelpShiftDebug", "retryMessage", e);
    }
  }

  private void addMessage(Handler success, Handler failure, String issueId, String messageText, String type, String refers)
  {
	  System.out.println("addMessage");
    this.hsStorage.setScreenShotDraft(Boolean.valueOf(false));
    this.hsApiData.addMessage(success, failure, issueId, messageText, type, refers);
  }

  private void addMessage(Handler success, Handler failure, String issueId, String messageText, String type, String refers, int failedState)
  {
	  System.out.println("addMessage2");
    this.hsStorage.setScreenShotDraft(Boolean.valueOf(false));
    this.hsApiData.addMessage(success, failure, issueId, messageText, type, refers, failedState);
  }

  private class DownloadImagesTask extends AsyncTask<HashMap, Void, HashMap>
  {
    private DownloadImagesTask()
    {
    }

    protected HashMap doInBackground(HashMap[] imagesData)
    {
    	System.out.println("doInBackground");
      HashMap imageData = imagesData[0];
      String url = (String)imageData.get("url");
      String messageId = (String)imageData.get("messageId");
      int attachId = ((Integer)imageData.get("attachId")).intValue();
      int position = ((Integer)imageData.get("position")).intValue();
      HashMap result = new HashMap();
      try {
        String filePath = downloadAttachment(url, messageId, attachId);
        result.put("success", Boolean.valueOf(true));
        result.put("filepath", filePath);
        result.put("position", Integer.valueOf(position));

        return result;
      } catch (IOException e) {
        Log.d("HelpShiftDebug", "Downloading image", e);
        result.put("success", Boolean.valueOf(false));
      }return result;
    }

    protected void onPostExecute(HashMap result)
    {
    	System.out.println("onPostExecute");
      if (((Boolean)result.get("success")).booleanValue()) {
        int position = ((Integer)result.get("position")).intValue();
        HSMsg msgData = (HSMsg)messagesList.get(position);
        msgData.screenshot = ((String)result.get("filepath"));
        adapter.notifyDataSetChanged();
      }
    }
  }
}