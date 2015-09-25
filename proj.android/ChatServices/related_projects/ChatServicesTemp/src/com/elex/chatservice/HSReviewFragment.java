package com.elex.chatservice;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.FragmentActivity;
import android.text.TextUtils;

import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSRes;

import org.json.JSONException;
import org.json.JSONObject;

public final class HSReviewFragment extends DialogFragment
{
  private final String TAG = "HelpShiftDebug";
  private HSApiData data;
  private HSStorage storage;
  private static HSAlertToRateAppListener alertToRateAppListener;
  private boolean disableReview = true;
  private String rurl = "";

  protected static void setAlertToRateAppListener(HSAlertToRateAppListener listener) {
    alertToRateAppListener = listener;
  }

  public Dialog onCreateDialog(Bundle savedInstanceState)
  {
    FragmentActivity activity = getActivity();
    Bundle extras = activity.getIntent().getExtras();
    if (extras != null) {
      this.disableReview = extras.getBoolean("disableReview", true);
      this.rurl = extras.getString("rurl");
    }
    this.data = new HSApiData(activity);
    this.storage = this.data.storage;
    return initAlertDialog(activity);
  }

  private void gotoApp(String marketUrl) {
    Intent intent = new Intent("android.intent.action.VIEW");
    intent.setData(Uri.parse(marketUrl));
    startActivity(intent);
  }

  public void onCancel(DialogInterface dialog)
  {
    HSFunnel.pushAppReviewedEvent("later");
    sendAlertToRateAppAction(Helpshift.HS_RATE_ALERT.CLOSE);
  }

  public void onDestroyView()
  {
    super.onDestroyView();
    if (this.disableReview) {
      this.data.disableReview();
    }
    getActivity().finish();
  }

  private void sendAlertToRateAppAction(Helpshift.HS_RATE_ALERT action) {
    if (alertToRateAppListener != null) {
      alertToRateAppListener.onAction(action);
    }
    alertToRateAppListener = null;
  }

  private Dialog initAlertDialog(FragmentActivity activity) {
    AlertDialog.Builder builder = new AlertDialog.Builder(activity);
    builder.setMessage(HSRes.getString(activity, "hs__review_message"));
    AlertDialog dialog = builder.create();
    dialog.setTitle(HSRes.getString(activity, "hs__review_title"));
    dialog.setIcon(activity.getApplicationInfo().icon);
    dialog.setCanceledOnTouchOutside(false);

    dialog.setButton(-1, HSRes.getString(activity, "hs__rate_button"), new DialogInterface.OnClickListener()
    {
      public void onClick(DialogInterface dialog, int which)
      {
        try
        {
          if (TextUtils.isEmpty(rurl)) {
            rurl = storage.getConfig().optString("rurl", "");
          }
          rurl = rurl.trim();
          if (!TextUtils.isEmpty(rurl))
            gotoApp(rurl);
        }
        catch (JSONException e) {
          Log.d("HelpShiftDebug", e.getMessage());
        }
        HSFunnel.pushAppReviewedEvent("reviewed");
        sendAlertToRateAppAction(Helpshift.HS_RATE_ALERT.SUCCESS);
      }
    });
    dialog.setButton(-3, HSRes.getString(activity, "hs__feedback_button"), new DialogInterface.OnClickListener()
    {
      public void onClick(DialogInterface dialog, int which)
      {
        HSFunnel.pushAppReviewedEvent("feedback");
        sendAlertToRateAppAction(Helpshift.HS_RATE_ALERT.FEEDBACK);
        if (!storage.getIsConversationShowing().booleanValue()) {
          Intent i = new Intent(getActivity(), HSConversation.class);
          i.putExtra("decomp", true);
          i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(getActivity()));

          i.putExtra("chatLaunchSource", "support");
          i.putExtra("isRoot", true);
          getActivity().startActivity(i);
        }
      }
    });
    dialog.setButton(-2, HSRes.getString(activity, "hs__review_close_button"), new DialogInterface.OnClickListener()
    {
      public void onClick(DialogInterface dialog, int which)
      {
        HSFunnel.pushAppReviewedEvent("later");
        sendAlertToRateAppAction(Helpshift.HS_RATE_ALERT.CLOSE);
      }
    });
    return dialog;
  }
}