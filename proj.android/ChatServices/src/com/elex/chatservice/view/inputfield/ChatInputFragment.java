package com.elex.chatservice.view.inputfield;

import org.json.JSONArray;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.util.CompatibleApiUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.view.MessagesAdapter;

public class ChatInputFragment extends Fragment
{
  private final BroadcastReceiver failedMessageRequestChecker;
  private final BroadcastReceiver connChecker;
  private Activity activity;
  private Bundle extras;
  private String issueId;
  private MessagesAdapter adapter;
//  private HSStorage hsStorage;
//  private HSApiData hsApiData;
  private String chatLaunchSource;
  private Thread pollerThread;
//  private Handler pollerThreadHandler;

  private EditText replyField;
  private LinearLayout messageBox;
  private TextView wordCount;

  private boolean persistMessageBox;
  private boolean newActivity;
  private Handler fetchMessagesSuccess;
  private Handler fetchMessagesFailure;
  private Handler replyHandler;
  private Handler replyFailHandler;

  public ChatInputFragment()
  {
    this.failedMessageRequestChecker = new BroadcastReceiver()
    {
      public void onReceive(Context context, Intent intent) {
        refreshMessages();
      }
    };
    this.connChecker = new BroadcastReceiver()
    {
      public void onReceive(Context context, Intent intent) {
        startPoller();
      }
    };

    this.persistMessageBox = false;
    this.newActivity = true;

    this.replyHandler = new Handler() {
      public void handleMessage(Message msg) {
        renderReplyMsg(msg);
      }
    };
    this.replyFailHandler = new Handler() {
      public void handleMessage(Message msg) {
        super.handleMessage(msg);
//        if (pollerThreadHandler != null) {
//          pollerThreadHandler.getLooper().quit();
//        }
        refreshMessages();
      }
    };
  }

  public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
  {
    this.activity = ((Activity)getActivity());

    this.extras = getArguments();
    this.extras.remove("message");

    this.chatLaunchSource = this.extras.getString("chatLaunchSource");

    return inflater.inflate(ResUtil.getId(this, "layout", "cs__chat_input_fragment"), container, false);
  }

  public void onViewCreated(View view, Bundle savedInstanceState)
  {
    super.onViewCreated(view, savedInstanceState);

    this.messageBox = ((LinearLayout)view.findViewById(ResUtil.getId(this.activity, "id", "cs__chat_input_relativeLayout")));
    this.replyField = ((EditText)view.findViewById(ResUtil.getId(this.activity, "id", "cs__messageText")));
    this.wordCount = ((TextView)view.findViewById(ResUtil.getId(this.activity, "id", "wordCountTextView")));
    final Button addReply = (Button)view.findViewById(ResUtil.getId(this.activity, "id", "hs__sendMessageBtn"));

    refreshStatus();

    if (this.replyField.getText().length() == 0) {
      addReply.setEnabled(false);
      CompatibleApiUtil.getInstance().setButtonAlpha(addReply, false);
    } else {
      addReply.setEnabled(true);
      CompatibleApiUtil.getInstance().setButtonAlpha(addReply, true);
    }

    addReply.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View view) {
//        String replyText = replyField.getText().toString().trim();
//        String identity = hsStorage.getIdentity();
//
//        if (!TextUtils.isEmpty(replyText)) {
//          replyField.setText("");
//          try
//          {
//        	  sendMessage(replyText);
//          } catch (Exception e) {
//    		  System.out.println(e.getMessage());
//          }
//
//          addMessage(replyHandler, replyFailHandler, issueId, replyText, "txt", "");
//          try
//          {
//            JSONObject eventData = new JSONObject();
//            eventData.put("type", "txt");
//            eventData.put("body", replyText);
//            eventData.put("id", issueId);
//            HSFunnel.pushEvent("m", eventData);
//          } catch (JSONException e) {
//            Log.d("HelpShiftDebug", "JSONException", e);
//          }
//        }
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
              	refreshWordCount();
              }
          });
      }

      public void beforeTextChanged(CharSequence s, int start, int count, int after) {
      }

      public void onTextChanged(CharSequence s, int start, int before, int count) {
        persistMessageBox = true;
        if (s.length() == 0) {
          addReply.setEnabled(false);
          CompatibleApiUtil.getInstance().setButtonAlpha(addReply, false);
        } else {
          addReply.setEnabled(true);
          CompatibleApiUtil.getInstance().setButtonAlpha(addReply, true);
        }
      }
    });
  }

  private void sendMessage(String messageText)
  {
	  ChatServiceController.getInstance().host.sendMessage(messageText);
  }

  private void renderReplyMsg(Message msg)
  {
//    HashMap result = (HashMap)msg.obj;
//    JSONObject message = (JSONObject)result.get("response");
//    JSONArray messages = new JSONArray();
//    messages.put(message);
//    try
//    {
//      this.hsStorage.storeMessages(this.issueId, messages);
//    } catch (JSONException e) {
//      Log.d("HelpShiftDebug", "JSONException", e);
//    }
//
//    refreshMessages();
  }

  private void refreshStatus()
  {
	  showMessageBox();
  }

  private void showMessageBox()
  {
//    this.adapter.enableButtons(true);
    this.messageBox.setVisibility(0);
    refreshWordCount();
  }

  private void showKeyboard(View v)
  {
    v.requestFocus();
    InputMethodManager imm = (InputMethodManager)this.activity.getSystemService("input_method");
    imm.showSoftInput(v, 0);
  }

  private void hideKeyboard(View v) {
    InputMethodManager imm = (InputMethodManager)this.activity.getSystemService("input_method");
    imm.hideSoftInputFromWindow(v.getWindowToken(), 0);
  }

  private void refreshMessages() {
  }

  private void refreshMessages(JSONArray messages) {
  }

  public void onPause()
  {
    super.onPause();
//    if (this.pollerThreadHandler != null)
//      this.pollerThreadHandler.getLooper().quit();
//    try
//    {
//      this.hsStorage.resetIssueCount(this.issueId);
//    } catch (JSONException e) {
//      Log.d("HelpShiftDebug", e.getMessage(), e);
//    }
//    this.hsStorage.setForeground(Boolean.valueOf(false));
//    this.hsStorage.setForegroundIssue("");
//    this.hsApiData.resetServiceInterval();
//    this.activity.unregisterReceiver(this.connChecker);
//    this.activity.unregisterReceiver(this.failedMessageRequestChecker);
//    this.hsStorage.storeReply(this.replyField.getText().toString().trim());
  }

  public void startPoller()
  {
//    if (this.pollerThreadHandler != null) {
//      this.pollerThreadHandler.getLooper().quit();
//      this.pollerThread = null;
//    }
//
//    String conversation = this.hsStorage.getActiveConversation();
//    String archivedConversation = this.hsStorage.getArchivedConversation();
//    if ((TextUtils.isEmpty(conversation)) && (TextUtils.isEmpty(archivedConversation))) {
//      return;
//    }
//
//    this.pollerThread = new Thread(new Runnable()
//    {
//      public void run() {
//        Looper.prepare();
//        pollerThreadHandler = new Handler() {
//          public void handleMessage(Message msg) {
//            Message newMsg = fetchMessagesSuccess.obtainMessage();
//            newMsg.obj = msg.obj;
//            fetchMessagesSuccess.sendMessage(newMsg);
//          }
//        };
//        Runnable m_handlerTask = new Runnable()
//        {
//          public void run() {
//            try {
//              hsApiData.getLatestIssues(pollerThreadHandler, fetchMessagesFailure, chatLaunchSource);
//            }
//            catch (JSONException e)
//            {
//              Log.d("HelpShiftDebug", "get issues", e);
//            }
//            pollerThreadHandler.postDelayed(this, 3000L);
//          }
//        };
//        m_handlerTask.run();
//        Looper.loop();
//      }
//    });
//    this.pollerThread.start();
  }

  public void onResume()
  {
    super.onResume();
//    NotificationManager notificationManager = (NotificationManager)getActivity().getSystemService("notification");
//
//    notificationManager.cancel(this.issueId, 1);
//
//    IntentFilter connFilter = new IntentFilter();
//    connFilter.addAction("android.net.conn.CONNECTIVITY_CHANGE");
//    this.activity.registerReceiver(this.connChecker, connFilter);
//
//    IntentFilter failFilter = new IntentFilter();
//    failFilter.addAction("com.elex.chatservice.failedMessageRequest");
//    this.activity.registerReceiver(this.failedMessageRequestChecker, failFilter);
//
//    startPoller();
//    try
//    {
//      this.hsStorage.resetIssueCount(this.issueId);
//    } catch (JSONException e) {
//      Log.d("HelpShiftDebug", e.toString(), e);
//    }
//
//    this.hsStorage.setForeground(Boolean.valueOf(true));
//    this.hsStorage.setForegroundIssue(this.issueId);
//    this.hsApiData.updateMessageSeenState(this.issueId, this.chatLaunchSource);
//    String replyText = this.hsStorage.getReply();
//    this.replyField.setText(replyText);
//
//    if ((this.newActivity) && (TextUtils.isEmpty(replyText)))
//    {
//      this.persistMessageBox = false;
//    }
//    this.newActivity = false;
  }

  private void refreshWordCount()
  {
      if(replyField.getLineCount()>2)
      {
      	wordCount.setVisibility(View.VISIBLE);
      }else{
      	wordCount.setVisibility(View.GONE);
      }
      wordCount.setText(replyField.getText().length() + "/500");
  }

  public void onStart()
  {
    super.onStart();
  }

  public void onStop()
  {
    super.onStop();
  }

  private void addMessage(Handler success, Handler failure, String issueId, String messageText, String type, String refers)
  {
//    this.hsStorage.setScreenShotDraft(Boolean.valueOf(false));
//    this.hsApiData.addMessage(success, failure, issueId, messageText, type, refers);
  }

  private void addMessage(Handler success, Handler failure, String issueId, String messageText, String type, String refers, int failedState)
  {
//    this.hsStorage.setScreenShotDraft(Boolean.valueOf(false));
//    this.hsApiData.addMessage(success, failure, issueId, messageText, type, refers, failedState);
  }

  private void gotoApp(String marketUrl)
  {
    Intent intent = new Intent("android.intent.action.VIEW");
    intent.setData(Uri.parse(marketUrl));
    if (isResumed())
      startActivity(intent);
  }

  public void onActivityResult(int requestCode, int resultCode, Intent imageReturnedIntent)
  {
    super.onActivityResult(requestCode, resultCode, imageReturnedIntent);

//    if (-1 == resultCode) {
//      String screenshotPath = AttachmentUtil.getPath(getActivity(), imageReturnedIntent);
//      if (!TextUtils.isEmpty(screenshotPath))
//      {
//        try
//        {
//          HSMsg msgData;
//          if (requestCode == 0) {
//            msgData = AttachmentUtil.addAndGetLocalRscMsg(this.hsStorage, this.issueId, screenshotPath);
//            this.messagesList.add(msgData);
//          } else {
//            msgData = (HSMsg)this.messagesList.get(requestCode);
//            msgData.screenshot = screenshotPath;
//          }
//
//          this.hsApiData.setScreenshot(msgData.id, this.issueId, screenshotPath);
//        } catch (JSONException e) {
//          Log.d("HelpShiftDebug", e.getMessage(), e);
//        }
//        this.adapter.notifyDataSetChanged();
//      }
//    }
  }

//  @TargetApi(Build.VERSION_CODES.HONEYCOMB)
//  public void onCreateContextMenu(ContextMenu menu, View v, ContextMenu.ContextMenuInfo menuInfo)
//  {
//    menu.add(0, v.getId(), 0, "Copy");
//
//    TextView copyTextView = (TextView)v;
//    String copyText = copyTextView.getText().toString();
//
//    if (Build.VERSION.SDK_INT >= 13) {
//      android.content.ClipboardManager clipboard = (android.content.ClipboardManager)this.activity.getSystemService("clipboard");
//      ClipData clip = ClipData.newPlainText("Copy Text", copyText);
//      clipboard.setPrimaryClip(clip);
//    } else {
//      android.text.ClipboardManager clipboard = (android.text.ClipboardManager)this.activity.getSystemService("clipboard");
//      clipboard.setText(copyText);
//    }
//  }
}