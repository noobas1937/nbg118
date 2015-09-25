package com.elex.chatservice;

import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore;
import android.provider.MediaStore.Images.Media;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.Window;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.elex.chatservice.app.ActionBarHelper;
import com.elex.chatservice.exceptions.IdentityException;
import com.elex.chatservice.res.values.HSConfig;
import com.elex.chatservice.util.AttachmentUtil;
import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSErrors;
import com.elex.chatservice.util.HSIcons;
import com.elex.chatservice.util.HSPattern;
import com.elex.chatservice.util.HSRes;
import com.elex.chatservice.viewstructs.HSMsg;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class HSAddIssueFragment extends Fragment
{
  public static final String TAG = "HelpShiftDebug";
  private HSActivity activity;
  private Bundle extras;
  private HSStorage hsStorage;
  private HSApiData hsApiData;
  private HSApiClient hsApiClient;
  private Boolean decomp;
  private Boolean showConvOnReportIssue;
  private int callFinishRequestCode = 1;
  private MenuItem addIssueMenuItem;
  private MenuItem attachScreenshotMenu;
  private TextView desc;
  private EditText userNameField;
  private EditText emailField;
  private String userName;
  private String email;
  private Boolean requireEmail;
  private ImageView screenshot;
  private String screenshotPath = null;
  private String issueId;
  private ImageButton clearBtn;
  private HSMsg msgData;
  private Handler failureHandler = new Handler() {
    public void handleMessage(Message msg) {
      super.handleMessage(msg);
      HashMap result = (HashMap)msg.obj;
      Integer status = (Integer)result.get("status");
      HSErrors.showFailToast(status.intValue(), null, activity);
      setIsReportingIssue(false);
    }
  };

  public Handler reportHandler = new Handler() {
    public void handleMessage(Message msg) {
      try {
        HashMap result = (HashMap)msg.obj;
        JSONObject issue = (JSONObject)result.get("response");
        issueId = issue.getString("id");
        JSONArray issues = new JSONArray();
        issues.put(issue);
        hsStorage.setIssuesTs(issue.getString("created_at"));
        hsStorage.storeIssues(issues);
        hsStorage.setUsername(userName);
        hsStorage.setEmail(email);
        hsStorage.storeReply("");
        hsStorage.storeConversationDetail("");
        desc.setText("");

        HSFunnel.pushEvent("p");

        System.out.println("reportHandler");
        if (!TextUtils.isEmpty(screenshotPath)) {
          hsStorage.setForegroundIssue(issueId);
          Log.d("HelpShiftDebug", "Conversation started. Uploading screenshot");
          msgData = AttachmentUtil.addAndGetLocalRscMsg(hsStorage, issueId, screenshotPath, true);
          hsApiClient.addScMessage(uploadSuccessHandler, uploadFailHandler, hsStorage.getIdentity(), issueId, "", "sc", msgData.id, msgData.screenshot);
        }
        else
        {
        	System.out.println("reportHandler handleExit...");
          handleExit();
        }
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", e.toString(), e);
      }
    }
  };

  private Handler getLatestIssuesHandler = new Handler()
  {
    public void handleMessage(Message msg) {
      clearScreenshot();
      System.out.println("getLatestIssuesHandler");
      handleExit();
    }
  };

  private Handler uploadSuccessHandler = new Handler()
  {
    public void handleMessage(Message msg) {
      HashMap result = (HashMap)msg.obj;
      JSONObject message = (JSONObject)result.get("response");
      try {
        JSONObject eventData = new JSONObject();
        eventData.put("type", "url");
        eventData.put("body", message.getJSONObject("meta").getJSONArray("attachments").getJSONObject(0).getString("url"));
        eventData.put("id", issueId);
        HSFunnel.pushEvent("m", eventData);

        AttachmentUtil.copyAttachment(getActivity(), hsApiData, screenshotPath, message.getString("id"), 0);
      }
      catch (IOException e) {
        Log.d("HelpShiftDebug", "Saving uploaded screenshot", e);
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", "uploadSuccessHandler", e);
      }
      try
      {
        String refers = message.getJSONObject("meta").getString("refers");
        if (!TextUtils.isEmpty(refers)) {
          hsStorage.deleteMessage(issueId, refers);
        }
        hsApiData.getLatestIssues(getLatestIssuesHandler, getLatestIssuesHandler);
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", "uploadSuccessHandler", e);
      }
    }
  };

  private Handler uploadFailHandler = new Handler()
  {
    public void handleMessage(Message msg) {
      try {
        hsApiData.setInProgress(msgData.id, issueId, Boolean.valueOf(false));
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", "uploadFailHanlder", e);
      }
      clearScreenshot();
      System.out.println("uploadFailHandler");
      handleExit();
    }
  };

  public Handler existsHandler = new Handler() {
    public void handleMessage(Message msg) {
      try {
        HashMap result = (HashMap)msg.obj;
        JSONObject profileResult = (JSONObject)result.get("response");
        hsStorage.setIdentity(profileResult.get("id").toString());
        hsApiData.getIssues(new Handler() {
          public void handleMessage(Message msg) {
            try {
              hsApiData.createIssue(reportHandler, failureHandler, getIssueText(), getUserInfo());
            }
            catch (IdentityException e) {
              Log.d("HelpShiftDebug", "Something really foul has happened", e);
            }
          }
        }
        , failureHandler);

        hsApiData.updateUAToken();
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", e.getMessage(), e);
      }
    }
  };

  private void showIssueFiledToast()
  {
	  System.out.println("showIssueFiledToast");
    Toast toast = Toast.makeText(this.activity, HSRes.getString(this, "hs__conversation_started_message"), 0);

    toast.setGravity(16, 0, 0);
    toast.show();
  }

  private void handleDecomp() {
	  System.out.println("handleDecomp");
    Intent returnIntent = new Intent();
    returnIntent.putExtra("callFinish", true);
    getActivity().setResult(-1, returnIntent);
    getActivity().finish();
    HSActivityUtil.sessionEnding();
  }

  private void handleExit() {
    Boolean dia = (Boolean)HSConfig.configData.get("dia");
    if ((this.showConvOnReportIssue.booleanValue()) && (!dia.booleanValue()))
    {
    	System.out.println("handleExit this.showConvOnReportIssue");
      Intent i = new Intent(this.activity, HSConversation.class);
      i.putExtra("newIssue", true);
      i.putExtra("issueId", this.issueId);
      i.putExtra("decomp", this.decomp);
      i.putExtra("showConvOnReportIssue", this.showConvOnReportIssue);
      i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(this.activity));
      i.putExtra("chatLaunchSource", "support");
      if (isResumed()) {
        getActivity().startActivityForResult(i, 1);
      }
    }
    else if (isResumed()) {
    	System.out.println("handleExit isResumed");
      showIssueFiledToast();
      handleDecomp();
      
      //接着显示Conversation（为了方便，客服经常关会话，发新消息后得重启app才能再一次打开Conversation界面）
      Helpshift.showConversation(activity);
    }
  }

  private void clearScreenshot()
  {
    this.screenshot.setVisibility(8);
    this.clearBtn.setVisibility(8);
    this.screenshotPath = "";
    this.hsStorage.setConversationScreenshot("");
    this.attachScreenshotMenu.setVisible(true);
  }

  private String getIssueText()
  {
    return this.desc.getText().toString().trim();
  }

  private HashMap getUserInfo() {
    HashMap data = null;

    if (IdentityFilter.showNameEmailForm()) {
      data = new HashMap();
      data.put("name", this.userName);
      if (this.email.trim().length() > 0) {
        data.put("email", this.email);
      }
    }
    return data;
  }

  public void onPause()
  {
    super.onPause();
    setIsReportingIssue(false);

    if (this.hsStorage.getActiveConversation().equals("")) {
      this.hsStorage.storeConversationDetail(getIssueText());
    }
    saveScreenshot(this.screenshotPath);
    this.hsStorage.setForegroundIssue("");
  }

  public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
  {
    this.activity = ((HSActivity)getActivity());
    this.extras = getArguments();

    Boolean isFullScreen = Boolean.valueOf(this.extras.getBoolean("showInFullScreen"));

    if (isFullScreen.booleanValue()) {
      this.activity.getWindow().setFlags(1024, 1024);
    }

    this.hsApiData = new HSApiData(this.activity);
    this.hsStorage = this.hsApiData.storage;
    this.hsApiClient = this.hsApiData.client;

    this.requireEmail = Boolean.valueOf(IdentityFilter.requireEmail(this.hsStorage));

    this.decomp = Boolean.valueOf(this.extras.getBoolean("decomp", false));
    this.showConvOnReportIssue = Boolean.valueOf(this.extras.getBoolean("showConvOnReportIssue"));

    if (this.decomp.booleanValue()) {
      HSAnalytics.decomp = true;
    }

    setHasOptionsMenu(true);
    return inflater.inflate(HSRes.getId(this, "layout", "hs__new_conversation_fragment"), container, false);
  }

  public void onViewCreated(View view, Bundle savedInstanceState)
  {
    super.onViewCreated(view, savedInstanceState);

    this.desc = ((TextView)view.findViewById(HSRes.getId(this.activity, "id", "hs__conversationDetail")));

    this.desc.addTextChangedListener(new TextWatcher()
    {
      public void afterTextChanged(Editable s) {
      }

      public void beforeTextChanged(CharSequence s, int start, int count, int after) {
      }

      public void onTextChanged(CharSequence s, int start, int before, int count) {
        desc.setError(null);
      }
    });
    this.userNameField = ((EditText)view.findViewById(HSRes.getId(this.activity, "id", "hs__username")));

    this.userNameField.addTextChangedListener(new TextWatcher()
    {
      public void afterTextChanged(Editable s) {
      }

      public void beforeTextChanged(CharSequence s, int start, int count, int after) {
      }

      public void onTextChanged(CharSequence s, int start, int before, int count) {
        userNameField.setError(null);
      }
    });
    this.emailField = ((EditText)view.findViewById(HSRes.getId(this.activity, "id", "hs__email")));

    this.emailField.addTextChangedListener(new TextWatcher()
    {
      public void afterTextChanged(Editable s) {
      }

      public void beforeTextChanged(CharSequence s, int start, int count, int after) {
      }

      public void onTextChanged(CharSequence s, int start, int before, int count) {
        emailField.setError(null);
      }
    });
    if (this.requireEmail.booleanValue()) {
      this.emailField.setHint(HSRes.getString(this, "hs__email_required_hint"));
    }

    if (!IdentityFilter.showNameEmailForm()) {
      this.userNameField.setText("Anonymous");
      this.userNameField.setVisibility(8);
      this.emailField.setVisibility(8);
    } else {
      this.userNameField.setText(this.hsStorage.getUsername());
      this.emailField.setText(this.hsStorage.getEmail());
    }

    this.activity.getWindow().setSoftInputMode(4);

    ActionBarHelper actionBarHelper = this.activity.getActionBarHelper();
    actionBarHelper.setDisplayHomeAsUpEnabled(true);
    actionBarHelper.setTitle(HSRes.getString(this, "hs__new_conversation_header"));

    this.screenshot = ((ImageView)view.findViewById(HSRes.getId(this.activity, "id", "hs__screenshot")));

    this.screenshot.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View view) {
        pickImage();
      }
    });
    this.clearBtn = ((ImageButton)view.findViewById(16908314));
    this.clearBtn.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View view) {
        clearScreenshot();
      }
    });
  }

  private void pickImage() {
    Intent i = new Intent("android.intent.action.PICK", MediaStore.Images.Media.EXTERNAL_CONTENT_URI);

    AttachmentUtil.setSelectingAttachment(true);
    startActivityForResult(i, 0);
  }

  public boolean onOptionsItemSelected(MenuItem item)
  {
    int id = item.getItemId();
    if (id == HSRes.getId(this, "id", "hs__action_add_conversation"))
    {
      if (isFormValid()) {
        hideKeyboard(this.desc);
        try {
          setIsReportingIssue(true);
          this.hsApiData.createIssue(this.reportHandler, this.failureHandler, getIssueText(), getUserInfo());
          System.out.println("onOptionsItemSelected createIssue");
        }
        catch (IdentityException e) {
        	System.out.println("onOptionsItemSelected IdentityException");
          this.hsApiData.registerProfile(this.existsHandler, this.failureHandler, this.userName, this.email, this.hsApiData.getHSId());
        }

      }

      return true;
    }if (id == 16908332) {
      getActivity().onBackPressed();
      return true;
    }if (id == HSRes.getId(this, "id", "hs__attach_screenshot")) {
      pickImage();
      return true;
    }
    return super.onOptionsItemSelected(item);
  }

  public void onActivityResult(int requestCode, int resultCode, Intent data)
  {
    super.onActivityResult(requestCode, resultCode, data);
    if ((resultCode == -1) && (requestCode == 0))
    {
      AttachmentUtil.setSelectingAttachment(false);
      this.screenshotPath = AttachmentUtil.getPath(getActivity(), data);
      saveScreenshot(this.screenshotPath);
      setScreenshot(this.screenshotPath);
    }
  }

  private void saveScreenshot(String screenshotPath) {
    if (!TextUtils.isEmpty(screenshotPath))
      this.hsStorage.setConversationScreenshot(screenshotPath);
  }

  private void setScreenshot(String screenshotPath)
  {
    Bitmap screenshotBitmap = AttachmentUtil.getBitmap(screenshotPath, -1);
    if (screenshotBitmap != null) {
      this.screenshot.setImageBitmap(screenshotBitmap);
      this.screenshot.setVisibility(0);
      this.clearBtn.setVisibility(0);
      this.screenshotPath = screenshotPath;
      if (this.attachScreenshotMenu != null) {
        this.attachScreenshotMenu.setVisible(false);
      }
      this.desc.measure(0, 0);
      int height = this.desc.getMeasuredHeight();
      this.screenshot.getLayoutParams().height = height;
      this.screenshot.getLayoutParams().width = ((int)Math.round(height * 0.6666666666666666D));
      this.screenshot.requestLayout();
    }
  }

  private boolean isFormValid() {
    Boolean validForm = Boolean.valueOf(true);

    String issueText = this.desc.getText().toString();
    this.userName = this.userNameField.getText().toString();
    this.email = this.emailField.getText().toString();

    if (issueText.trim().length() == 0) {
      this.desc.setError(HSRes.getString(this.activity, "hs__conversation_detail_error"));
      validForm = Boolean.valueOf(false);
    } else if (HSPattern.checkSpecialCharacters(issueText)) {
      this.desc.setError(HSRes.getString(this.activity, "hs__invalid_description_error"));
      validForm = Boolean.valueOf(false);
    }

    if ((this.userName.trim().length() == 0) || (HSPattern.checkSpecialCharacters(this.userName)))
    {
      this.userNameField.setError(HSRes.getString(this.activity, "hs__username_blank_error"));

      validForm = Boolean.valueOf(false);
    }

    if ((this.requireEmail.booleanValue()) && (this.email.trim().length() == 0) && (!HSPattern.checkEmail(this.email)))
    {
      this.emailField.setError(HSRes.getString(this.activity, "hs__invalid_email_error"));
      validForm = Boolean.valueOf(false);
    } else if ((this.email.trim().length() > 0) && (!HSPattern.checkEmail(this.email)))
    {
      this.emailField.setError(HSRes.getString(this.activity, "hs__invalid_email_error"));
      validForm = Boolean.valueOf(false);
    }

    return validForm.booleanValue();
  }

  public void onResume()
  {
    super.onResume();
    HSFunnel.pushEvent("i");
    String initText = "";
    String storedText = this.hsStorage.getConversationDetail();
    if (this.extras != null) {
      String input = this.extras.getString("message");
      if ((input != null) && (!input.trim().equals(""))) {
        initText = input.substring(0, 1).toUpperCase() + input.substring(1);
      }
    }

    if (!TextUtils.isEmpty(initText))
      this.desc.setText(initText);
    else {
      this.desc.setText(storedText);
    }
    this.desc.requestFocus();
    setScreenshot(this.hsStorage.getConversationScreenshot());
  }

  private void hideKeyboard(View view) {
    InputMethodManager in = (InputMethodManager)this.activity.getSystemService("input_method");
    in.hideSoftInputFromWindow(view.getWindowToken(), 2);
  }

  private void setIsReportingIssue(boolean isReportingIssue) {
    this.activity.setSupportProgressBarIndeterminateVisibility(isReportingIssue);
    if (this.addIssueMenuItem != null) {
      this.addIssueMenuItem.setVisible(!isReportingIssue);
    }
    if (this.clearBtn != null) {
      this.clearBtn.setEnabled(!isReportingIssue);
    }
    if (this.screenshot != null) {
      this.screenshot.setEnabled(!isReportingIssue);
    }

    if (this.attachScreenshotMenu != null)
      if ((isReportingIssue) || ((this.clearBtn != null) && (this.clearBtn.getVisibility() == 0)))
      {
        this.attachScreenshotMenu.setVisible(false);
      }
      else this.attachScreenshotMenu.setVisible(true);
  }

  public void onCreateOptionsMenu(Menu menu, MenuInflater inflater)
  {
    inflater.inflate(HSRes.getId(this, "menu", "hs__add_conversation_menu"), menu);

    this.addIssueMenuItem = menu.findItem(HSRes.getId(this, "id", "hs__action_add_conversation"));

    this.attachScreenshotMenu = menu.findItem(HSRes.getId(this, "id", "hs__attach_screenshot"));
    HSIcons.applyMenuItemSelectedFilter(this.activity, this.addIssueMenuItem.getIcon());
    setIsReportingIssue(false);
  }
}