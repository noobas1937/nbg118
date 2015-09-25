package com.elex.chatservice;

import android.content.Intent;
import android.content.res.Resources.Theme;
import android.content.res.TypedArray;
import android.os.Build;
import android.os.Build.VERSION;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.text.TextUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.webkit.WebSettings;
import android.webkit.WebSettings.PluginState;
import android.webkit.WebSettings.TextSize;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSErrors;
import com.elex.chatservice.util.HSHTML5WebView;
import com.elex.chatservice.util.HSIcons;
import com.elex.chatservice.util.HSRes;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import org.json.JSONException;
import org.json.JSONObject;

public final class HSQuestionFragment extends Fragment
{
  public static final String TAG = "HelpShiftDebug";
  private HSApiData data;
  private HSStorage storage;
  private String faqId = "";
  private LinearLayout contactUsContainer;
  private Button contactUsBtn;
  private String bodyText;
  private String titleText;
  private Boolean isRtl = Boolean.valueOf(false);
  private int isHelpful = 0;
  private HSActivity activity;
  private HSHTML5WebView mWebView;
  private Boolean enableContactUs;
  private Boolean eventSent = Boolean.valueOf(false);
  private Boolean likeClicked = Boolean.valueOf(false);
  private Boolean dislikeClicked = Boolean.valueOf(false);
  private Button likeButton;
  private Button dislikeButton;
  private TextView questionText;
  private TextView helpfulText;
  private TextView unhelpfulText;
  private JSONObject eventData;
  public Handler markFailHandler = new Handler() {
    public void handleMessage(Message msg) {
      super.handleMessage(msg);
      HashMap result = (HashMap)msg.obj;
      Integer status = (Integer)result.get("status");
      HSErrors.showFailToast(status.intValue(), null, activity);
    }
  };

  public Handler questionHandler = new Handler() {
    public void handleMessage(Message msg) {
      Faq question = (Faq)msg.obj;
      if (isResumed()) {
        updateQuestionUI(question);
      }

      if (!eventSent.booleanValue())
        try {
          faqId = question.getId();
          JSONObject eventData = new JSONObject();
          eventData.put("id", faqId);
          HSFunnel.pushEvent("f", eventData);
          eventSent = Boolean.valueOf(true);
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", "JSONException", e);
        }
    }
  };

  private Handler questionFailHandler = new Handler() {
    public void handleMessage(Message msg) {
      super.handleMessage(msg);
      HashMap result = (HashMap)msg.obj;
      Integer status = (Integer)result.get("status");
      if (status.intValue() == 404) {
        Log.d("HelpShiftDebug", "Specified question does not exist");
      }

      HSErrors.showFailToast(status.intValue(), null, activity);
    }
  };

  private void showMarkedToast(Boolean isHelpful)
  {
    CharSequence toastString = "";
    if (isHelpful.booleanValue())
      toastString = HSRes.getString(this.activity, "hs__mark_helpful_toast");
    else
      toastString = HSRes.getString(this.activity, "hs__mark_unhelpful_toast");
    int duration = 0;
    Toast toast = Toast.makeText(this.activity, toastString, duration);
    toast.setGravity(17, 0, 0);
    toast.show();
  }

  public void showMenuOptions() {
    if (this.isHelpful == 1)
      hideDislikeItem();
    else if (this.isHelpful == -1)
      hideLikeItem();
    else if (this.isHelpful == 0)
      showQuestionItem();
  }

  private void showQuestionItem()
  {
    if ((!this.likeClicked.booleanValue()) && (!this.dislikeClicked.booleanValue())) {
      this.questionText.setVisibility(0);
      this.likeButton.setVisibility(0);
      this.dislikeButton.setVisibility(0);
      this.unhelpfulText.setVisibility(8);
      this.contactUsBtn.setVisibility(8);
      this.helpfulText.setVisibility(8);
    }
  }

  private void hideLikeItem() {
    if (this.enableContactUs.booleanValue()) {
      this.contactUsBtn.setVisibility(0);
    }
    this.unhelpfulText.setVisibility(0);
    this.questionText.setVisibility(8);
    this.likeButton.setVisibility(8);
    this.dislikeButton.setVisibility(8);
  }

  private void hideDislikeItem() {
    this.questionText.setVisibility(8);
    this.likeButton.setVisibility(8);
    this.dislikeButton.setVisibility(8);
    this.helpfulText.setVisibility(0);
  }

  public void onResume()
  {
    if (this.mWebView != null) {
      this.mWebView.onResume();
    }

    if ((!TextUtils.isEmpty(this.faqId)) && (!this.eventSent.booleanValue())) {
      try {
        JSONObject eventData = new JSONObject();
        eventData.put("id", this.faqId);
        HSFunnel.pushEvent("f", eventData);
        this.eventSent = Boolean.valueOf(true);
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", "JSONException", e);
      }
    }
    super.onResume();
  }

  public void onPause()
  {
    try {
      if (this.mWebView != null) {
        Class.forName("android.webkit.WebView").getMethod("onPause", (Class[])null).invoke(this.mWebView, (Object[])null);
      }
    }
    catch (ClassNotFoundException cnfe)
    {
      Log.d("HelpShiftDebug", "ClassNotFoundException : ", cnfe);
    } catch (NoSuchMethodException nsme) {
      Log.d("HelpShiftDebug", "NoSuchMethodException : ", nsme);
    } catch (InvocationTargetException ite) {
      Log.d("HelpShiftDebug", "InvocationTargetException : ", ite);
    } catch (IllegalAccessException iae) {
      Log.d("HelpShiftDebug", "IllegalAccessException : ", iae);
    }
    super.onPause();
  }

  private void initWebView() {
    LinearLayout webViewParent = (LinearLayout)this.activity.findViewById(HSRes.getId(this, "id", "hs__webViewParent"));

    TypedArray array = this.activity.getTheme().obtainStyledAttributes(new int[] { 16842801, 16842806 });

    if (this.mWebView == null) {
      this.mWebView = new HSHTML5WebView(getActivity(), this);
      webViewParent.addView(this.mWebView.getLayout(), new FrameLayout.LayoutParams(-1, -1));

      int activityBackgroundColor = array.getColor(0, 16777215);
      array.recycle();
      this.mWebView.setBackgroundColor(activityBackgroundColor);

      WebSettings s = this.mWebView.getSettings();
      s.setJavaScriptEnabled(true);
      if (Build.VERSION.SDK_INT <= 11) {
        s.setPluginState(WebSettings.PluginState.ON);
      }

      s.setTextSize(WebSettings.TextSize.NORMAL);
    }

    if (this.bodyText.contains("<iframe")) {
      try {
        this.bodyText = this.bodyText.replace("https", "http");
      } catch (NullPointerException e) {
        Log.d("HelpShiftDebug", e.toString(), e);
      }

    }

    String textColor = String.format("#%06X", new Object[] { Integer.valueOf(0xFFFFFF & array.getColor(1, 16777215)) });
    String webBodyText;
    if (this.isRtl.booleanValue())
      webBodyText = "<html dir=\"rtl\">";
    else {
      webBodyText = "<html>";
    }

    webBodyText = webBodyText + "<head>" + "<style type=\"text/css\">img, object, embed { max-width: 100%; }" + "body { margin: 0px 10px 10px 0px; padding: 0; line-height: 1.5; white-space: normal; word-wrap: break-word; color: " + textColor + "; }" + ".title { display:block; margin: -12px 0 6px 0; padding: 0; font-size: 1.3125em; line-height: 1.25 }" + "</style>" + "<script language=\"javascript\">var iframe = document.getElementsByTagName (\"iframe\") [0]; if (iframe) { iframe.width = \"100%\"; iframe.style.width = \"100%\"; }" + "document.addEventListener('click',function(event) {" + "if (event.target instanceof HTMLImageElement) { event.preventDefault(); event.stopPropagation(); }" + "}, false);" + "</script>​" + "</head>" + "<body>" + "<strong class='title'>" + this.titleText + "</strong>" + this.bodyText + "</body>" + "</html>";

    this.mWebView.loadDataWithBaseURL(null, webBodyText, "text/html", "utf-8", null);
  }

  private void updateQuestionUI(Faq question) {
    this.titleText = question.getTitle();
    this.bodyText = question.getBody();
    this.faqId = question.getId();
    this.isRtl = question.getIsRtl();
    this.isHelpful = question.getIsHelpful();
    initWebView();
  }

  public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
  {
    this.activity = ((HSActivity)getActivity());
    this.data = new HSApiData(this.activity);
    this.storage = this.data.storage;

    return inflater.inflate(HSRes.getId(this, "layout", "hs__question_fragment"), container, false);
  }

  public void onViewCreated(View view, Bundle savedInstanceState)
  {
    super.onViewCreated(view, savedInstanceState);
    final Bundle extras = this.activity.getIntent().getExtras();
    if (extras != null) {
      if (extras.get("questionPublishId") != null) {
        if (extras.getBoolean("isDecomp") == true) {
          HSAnalytics.decomp = true;
        }
        String questionPublishId = (String)extras.get("questionPublishId");
        this.data.getQuestion(questionPublishId, this.questionHandler, this.questionFailHandler);
      }
      this.enableContactUs = Boolean.valueOf(ContactUsFilter.showContactUs(ContactUsFilter.LOCATION.QUESTION_FOOTER));
    }

    TypedArray array = this.activity.getTheme().obtainStyledAttributes(new int[] { 16842801 });

    int activityBackgroundColor = array.getColor(0, 16777215);
    array.recycle();
    getView().setBackgroundColor(activityBackgroundColor);

    this.contactUsContainer = ((LinearLayout)view.findViewById(HSRes.getId(this.activity, "id", "hs__contactUsContainer")));

    this.questionText = ((TextView)view.findViewById(HSRes.getId(this.activity, "id", "hs__question")));
    this.helpfulText = ((TextView)view.findViewById(HSRes.getId(this.activity, "id", "hs__helpful_text")));
    this.unhelpfulText = ((TextView)view.findViewById(HSRes.getId(this.activity, "id", "hs__unhelpful_text")));
    this.contactUsBtn = ((Button)view.findViewById(HSRes.getId(this.activity, "id", "hs__contact_us_btn")));

    HSIcons.applyTextColorPrimaryFilter(this.activity, this.contactUsBtn.getCompoundDrawables()[0]);

    this.likeButton = ((Button)view.findViewById(HSRes.getId(this.activity, "id", "hs__action_faq_helpful")));
    this.likeButton.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View view) {
        likeClicked = Boolean.valueOf(true);
        markQuestion(new Handler(), markFailHandler, faqId, Boolean.valueOf(true));
        HSFunnel.pushEvent("h", eventData);
        hideDislikeItem();
        showMarkedToast(Boolean.valueOf(true));
      }
    });
    this.dislikeButton = ((Button)view.findViewById(HSRes.getId(this.activity, "id", "hs__action_faq_unhelpful")));
    this.dislikeButton.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View view) {
        dislikeClicked = Boolean.valueOf(true);
        markQuestion(new Handler(), markFailHandler, faqId, Boolean.valueOf(false));
        HSFunnel.pushEvent("u", eventData);
        hideLikeItem();
        showMarkedToast(Boolean.valueOf(false));
      }
    });
    this.contactUsBtn.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View view) {
        Intent i = new Intent(activity, HSConversation.class);
        i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(activity));
        i.putExtra("chatLaunchSource", "support");
        i.putExtras(extras);
        i.removeExtra("isRoot");
        getActivity().startActivityForResult(i, 1);
      }
    });
    setHasOptionsMenu(true);
  }

  public void onDestroy()
  {
    if (this.mWebView != null) {
      this.mWebView.freeMemory();
      this.mWebView.removeAllViews();
      ((ViewGroup)this.mWebView.getParent()).removeView(this.mWebView);
      this.mWebView.destroy();
    }
    super.onDestroy();
  }

  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
  }

  private void markQuestion(Handler success, Handler failure, String faqId, Boolean helpful)
  {
    JSONObject params = new JSONObject();
    this.eventData = new JSONObject();
    try
    {
      this.eventData.put("id", faqId);
      params.put("f", faqId);
      params.put("h", helpful);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSONException", e);
    }

    Handler apiFailHandler = this.data.getApiFailHandler(failure, faqId, 0, params);

    this.data.markQuestion(success, apiFailHandler, faqId, helpful);
  }

  public boolean onOptionsItemSelected(MenuItem item)
  {
    int id = item.getItemId();
    if (id == 16908332) {
      if ((this.mWebView != null) && (this.mWebView.inCustomView()))
        this.mWebView.hideCustomView();
      else {
        getActivity().finish();
      }
      return true;
    }
    return super.onOptionsItemSelected(item);
  }
}