package com.elex.chatservice;

import java.util.HashMap;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

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
import android.text.TextUtils;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.elex.chatservice.customadapters.MessagesAdapter;
import com.elex.chatservice.util.HSRes;

public class ChatInputFragment extends Fragment
{
  private final BroadcastReceiver failedMessageRequestChecker;
  private final BroadcastReceiver connChecker;
  private Activity activity;
  private Bundle extras;
  private String issueId;
  private MessagesAdapter adapter;
  private HSStorage hsStorage;
  private HSApiData hsApiData;
  private String chatLaunchSource;
  private Thread pollerThread;
  private Handler pollerThreadHandler;
  
  private EditText replyField;
  private RelativeLayout messageBox;
  private TextView wordCount;
  
  private boolean persistMessageBox;
  private boolean newActivity;
  private Handler fetchMessagesSuccess;
  private Handler fetchMessagesFailure;
  private Handler replyHandler;
  private Handler replyFailHandler;

  public ChatInputFragment()
  {
	System.out.println("ChatInputFragment()");
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
        if (pollerThreadHandler != null) {
          pollerThreadHandler.getLooper().quit();
        }
        refreshMessages();
      }
    };
  }

  public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
  {
	System.out.println("ChatInputFragment onCreateView");
    this.activity = ((Activity)getActivity());

//    this.activity.setSupportProgressBarIndeterminateVisibility(false);
    this.extras = getArguments();
    this.extras.remove("message");

    this.hsApiData = new HSApiData(this.activity);
    this.hsStorage = this.hsApiData.storage;

//    this.newIssue = Boolean.valueOf(this.extras.getBoolean("newIssue", false));
//    this.decomp = Boolean.valueOf(this.extras.getBoolean("decomp", false));
    this.chatLaunchSource = this.extras.getString("chatLaunchSource");
//    this.showConvOnReportIssue = Boolean.valueOf(this.extras.getBoolean("showConvOnReportIssue"));

//    setHasOptionsMenu(true);

    return inflater.inflate(HSRes.getId(this, "layout", "cs__chat_input_fragment"), container, false);
  }

  public void onViewCreated(View view, Bundle savedInstanceState)
  {
	System.out.println("ChatInputFragment onViewCreated");
    super.onViewCreated(view, savedInstanceState);

    this.messageBox = ((RelativeLayout)view.findViewById(HSRes.getId(this.activity, "id", "cs__chat_input_relativeLayout")));
//	System.out.println("1");
    this.replyField = ((EditText)view.findViewById(HSRes.getId(this.activity, "id", "cs__messageText")));
//	System.out.println("2");
    this.wordCount = ((TextView)view.findViewById(HSRes.getId(this.activity, "id", "wordCountTextView")));
//	System.out.println("3");
    final Button addReply = (Button)view.findViewById(HSRes.getId(this.activity, "id", "hs__sendMessageBtn"));
//	System.out.println("4");

    refreshStatus();

    if (this.replyField.getText().length() == 0) {
      addReply.setEnabled(false);
      changeAlpha(addReply, false);
    } else {
      addReply.setEnabled(true);
      changeAlpha(addReply, true);
    }

    addReply.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View view) {
        String replyText = replyField.getText().toString().trim();
        String identity = hsStorage.getIdentity();

        if (!TextUtils.isEmpty(replyText)) {
          replyField.setText("");
          try
          {
        	  sendMessage(replyText);
          } catch (Exception e) {
    		  System.out.println(e.getMessage());
          }
          
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
              	refreshWordCount();
              }
          });
      }

      public void beforeTextChanged(CharSequence s, int start, int count, int after) {
      }

      @TargetApi(Build.VERSION_CODES.HONEYCOMB)
      public void onTextChanged(CharSequence s, int start, int before, int count) {
        persistMessageBox = true;
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
  
  private void sendMessage(String messageText)
  {
	  System.out.println("ChatInputFragment sendMessage");
	  ChatServiceBridge.sendMessage(messageText);
  }
  
  private void renderReplyMsg(Message msg)
  {
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
	  showMessageBox();
  }

  private void showMessageBox()
  {
		System.out.println("ChatInputFragment showMessageBox");
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

  @TargetApi(Build.VERSION_CODES.HONEYCOMB)
  private void changeAlpha(Button button, boolean enabled)
  {
		if (Build.VERSION.SDK_INT >= 11) {
			if(enabled){
				button.setAlpha(1.0f);
//				button.getBackground().setAlpha(255);
			}else{
				button.setAlpha(0.4f);
			}
		} else {
			
		}
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
    this.hsStorage.setScreenShotDraft(Boolean.valueOf(false));
    this.hsApiData.addMessage(success, failure, issueId, messageText, type, refers);
  }

  private void addMessage(Handler success, Handler failure, String issueId, String messageText, String type, String refers, int failedState)
  {
    this.hsStorage.setScreenShotDraft(Boolean.valueOf(false));
    this.hsApiData.addMessage(success, failure, issueId, messageText, type, refers, failedState);
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