package com.elex.chatservice.customadapters;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.List;

import net.londatiga.android.ActionItem;
import net.londatiga.android.QuickAction;
import net.londatiga.android.QuickActionFactory;
import android.R;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.Build;
import android.support.v4.app.Fragment;
import android.text.Html;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.elex.chatservice.HSMessagesFragment;
import com.elex.chatservice.util.AttachmentUtil;
import com.elex.chatservice.util.HSIcons;
import com.elex.chatservice.util.HSRes;
import com.elex.chatservice.viewstructs.HSMsg;

public final class MessagesAdapter extends ArrayAdapter
{
  private HSMessagesFragment f;
  private Context c;
  private List<HSMsg> items;
  private static final int TYPE_TXT_ADMIN = 1;
  private static final int TYPE_TXT_MOBILE = 2;
  private static final int TYPE_CB = 5;
  private static final int TYPE_CA_MOBILE = 6;
  private static final int TYPE_CR_MOBILE = 7;
  private static final int TYPE_SC_MOBILE = 8;
  private static final int TYPE_RAR = 11;
  private static final int TYPE_AR_MOBILE = 12;
  private static final int TYPE_RSC = 13;
  private static final int TYPE_LOCAL_RSC = 14;
  private final LayoutInflater inflater;
  private final int txtAdminId;
  private final int txtUserId;
  private final int confBoxId;
  private final int confStatusId;
  private final int rscId;
  private final int localRscId;
  private final int scId;
  private final int rarId;
  private final int arId;
  private boolean enableBtn;
  private QuickAction quickAction;

  public MessagesAdapter(Fragment f, int textViewResourceId, List<HSMsg> objects)
  {
    super(f.getActivity(), textViewResourceId, objects);
    this.f = ((HSMessagesFragment)f);
    this.c = f.getActivity();
    this.items = objects;
    this.inflater = ((LayoutInflater)this.c.getSystemService("layout_inflater"));
    this.txtAdminId = HSRes.getId(this.c, "layout", "hs__msg_txt_admin");
    this.txtUserId = HSRes.getId(this.c, "layout", "hs__msg_txt_user");
    this.confBoxId = HSRes.getId(this.c, "layout", "hs__msg_confirmation_box");
    this.confStatusId = HSRes.getId(this.c, "layout", "hs__msg_confirmation_status");
    this.rscId = HSRes.getId(this.c, "layout", "hs__msg_request_screenshot");
    this.localRscId = HSRes.getId(this.c, "layout", "hs__local_msg_request_screenshot");
    this.scId = HSRes.getId(this.c, "layout", "hs__msg_screenshot_status");
    this.rarId = HSRes.getId(this.c, "layout", "hs__msg_review_request");
    this.arId = HSRes.getId(this.c, "layout", "hs__msg_review_accepted");
    quickAction = QuickActionFactory.createQuickAction((Activity)c);

    //Set listener for action item clicked
	quickAction.setOnActionItemClickListener(new QuickAction.OnActionItemClickListener() {			
		@Override
		public void onItemClick(QuickAction source, int pos, int actionId) {				
			ActionItem actionItem = quickAction.getActionItem(pos);
            
			if (actionId == QuickActionFactory.ID_UP) {
				copyToClipboard();
			}
		}
	});
  }

  public void enableButtons(boolean enable) {
    this.enableBtn = enable;
  }

  public boolean isEnabled(int position)
  {
    return false;
  }

  public int getViewTypeCount()
  {
    return 20;
  }

  public int getItemViewType(int position)
  {
    HSMsg item = (HSMsg)this.items.get(position);
    if (((item.type.equals("txt")) && ((item.state == -1) || (item.state == 1))) || ((item.type.equals("txt")) && (item.state <= -2)) || ((item.type.equals("txt")) && (item.origin.equals("mobile"))))
    {
      return 2;
    }if ((item.type.equals("txt")) && (item.origin.equals("admin")))
      return 1;
    if ((item.type.equals("cb")) && (item.origin.equals("admin")))
      return 5;
    if ((item.type.equals("rsc")) && (item.origin.equals("admin"))) {
      if (item.id.startsWith("localRscMessage_")) {
        return 14;
      }
      return 13;
    }
    if ((item.type.equals("ca")) && (item.origin.equals("mobile")))
      return 6;
    if ((item.type.equals("ncr")) && (item.origin.equals("mobile")))
      return 7;
    if ((item.type.equals("sc")) && (item.origin.equals("mobile")))
      return 8;
    if ((item.type.equals("rar")) && (item.origin.equals("admin")))
      return 11;
    if ((item.type.equals("ar")) && (item.origin.equals("mobile"))) {
      return 12;
    }
    return 0;
  }

  public View getView(int position, View convertView, ViewGroup parent)
  {
    HSMsg item = (HSMsg)this.items.get(position);

    if (item != null)
      switch (getItemViewType(position)) {
      case 1:
        return setConvertView(convertView, item, new TxtAdminHolder());
      case 2:
        return setConvertView(convertView, item, new TxtUserHolder());
      case 5:
        return setConvertView(convertView, item, position, new CBViewHolder());
      case 6:
        return setConvertView(convertView, item, true, new CSViewHolder());
      case 7:
        return setConvertView(convertView, item, false, new CSViewHolder());
      case 13:
        return setConvertView(convertView, item, position, new RSCViewHolder());
      case 14:
        return setConvertView(convertView, item, position, new LocalRSCViewHolder());
      case 8:
        return setConvertView(convertView, item, new SCViewHolder());
      case 11:
        return setConvertView(convertView, item, position, new RARViewHolder());
      case 12:
        return setConvertView(convertView, item, new ARViewHolder());
      case 3:
      case 4:
      case 9:
      case 10: }  return null;
  }

  private View setConvertView(View convertView, HSMsg item, TxtAdminHolder holder)
  {
    if (convertView == null) {
      convertView = this.inflater.inflate(this.txtAdminId, null);

      holder.text1 = ((TextView)convertView.findViewById(R.id.text1));
      holder.text2 = ((TextView)convertView.findViewById(16908309));
      convertView.setTag(holder);
    } else {
      holder = (TxtAdminHolder)convertView.getTag();
    }

    holder.text1.setText(getText(item.body));
    holder.text2.setText(item.date);
    
    addClickListener(holder.text1);
    
    return convertView;
  }

  private View setConvertView(View convertView, final HSMsg item, TxtUserHolder holder)
  {
    if (convertView == null) {
      convertView = this.inflater.inflate(this.txtUserId, null);

      holder.text1 = ((TextView)convertView.findViewById(R.id.text1));
      holder.text2 = ((TextView)convertView.findViewById(16908309));
      holder.errorImage = ((ImageView)convertView.findViewById(16908294));
      convertView.setTag(holder);
    } else {
      holder = (TxtUserHolder)convertView.getTag();
    }

    if ((item.type.equals("txt")) && ((item.state == -1) || (item.state == 1))) {
      holder.text1.setText(getText(item.body));
      holder.text2.setText(HSRes.getString(this.c, "hs__sending_msg"));
      holder.errorImage.setVisibility(8);
    } else if ((item.type.equals("txt")) && (item.state <= -2)) {
      holder.text1.setText(getText(item.body));
      holder.text1.setOnClickListener(new View.OnClickListener()
      {
        public void onClick(View v) {
          f.retryMessage(item.id);
        }
      });
      holder.text2.setText(HSRes.getString(this.c, "hs__sending_fail_msg"));
      holder.errorImage.setVisibility(8);
    } else {
      holder.text1.setText(getText(item.body));
      holder.text2.setText(item.date);
      holder.errorImage.setVisibility(8);
    }
    
    addClickListener(holder.text1);
    
    return convertView;
  }

  private View setConvertView(View convertView, final HSMsg item, final int position, CBViewHolder holder)
  {
    if (convertView == null) {
      convertView = this.inflater.inflate(this.confBoxId, null);

      holder.text1 = ((TextView)convertView.findViewById(R.id.text1));
      holder.progress = ((ProgressBar)convertView.findViewById(16908301));
      holder.btnContainer = ((LinearLayout)convertView.findViewById(16908312));
      holder.button1 = ((ImageButton)convertView.findViewById(16908313));
      holder.button2 = ((ImageButton)convertView.findViewById(16908314));
      HSIcons.applyConfirmAcceptFilter(this.c, holder.button1.getDrawable());
      HSIcons.applyConfirmRejectFilter(this.c, holder.button2.getDrawable());
      convertView.setTag(holder);
    } else {
      holder = (CBViewHolder)convertView.getTag();
    }

    holder.text1.setText(getText(item.body));
    
    addClickListener(holder.text1);
    
    if (item.inProgress.booleanValue()) {
      holder.progress.setVisibility(0);
      holder.btnContainer.setVisibility(8);
    } else if (!item.invisible.booleanValue()) {
      holder.btnContainer.setVisibility(0);
      holder.progress.setVisibility(8);

      holder.button1.setOnClickListener(new View.OnClickListener()
      {
        public void onClick(View v) {
          if (item.clickable.booleanValue())
            f.replyConfirmation(item.id, Boolean.valueOf(true), position);
        }
      });
      holder.button2.setOnClickListener(new View.OnClickListener()
      {
        public void onClick(View v) {
          if (item.clickable.booleanValue())
            f.replyConfirmation(item.id, Boolean.valueOf(false), position);
        }
      });
      holder.button1.setEnabled(this.enableBtn);
      holder.button2.setEnabled(this.enableBtn);
    }
    else {
      holder.progress.setVisibility(8);
      holder.btnContainer.setVisibility(8);
    }
    
    addClickListener(holder.text1);
    
    return convertView;
  }

  private View setConvertView(View convertView, HSMsg item, boolean accepted, CSViewHolder holder)
  {
    if (convertView == null) {
      convertView = this.inflater.inflate(this.confStatusId, null);

      holder.text1 = ((TextView)convertView.findViewById(R.id.text1));
      holder.text2 = ((TextView)convertView.findViewById(16908309));
      convertView.setTag(holder);
    } else {
      holder = (CSViewHolder)convertView.getTag();
    }

    if (accepted)
      holder.text1.setText(HSRes.getString(this.c, "hs__ca_msg"));
    else {
      holder.text1.setText(HSRes.getString(this.c, "hs__cr_msg"));
    }
    holder.text2.setText(item.date);
    
    addClickListener(holder.text1);
    
    return convertView;
  }

  private View setConvertView(View convertView, final HSMsg item, final int position, RSCViewHolder holder)
  {
    if (convertView == null) {
      convertView = this.inflater.inflate(this.rscId, null);

      holder.text1 = ((TextView)convertView.findViewById(R.id.text1));
      holder.attachBtn = ((ImageButton)convertView.findViewById(16908313));
      HSIcons.applyAttachFilter(this.c, holder.attachBtn.getDrawable());
      holder.progress = ((ProgressBar)convertView.findViewById(16908301));
      holder.imagePreview = ((LinearLayout)convertView.findViewById(16908291));
      holder.image = ((ImageView)convertView.findViewById(16908304));
      holder.changeBtn = ((ImageButton)convertView.findViewById(16908314));
      holder.doneBtn = ((ImageButton)convertView.findViewById(16908315));
      HSIcons.applyConfirmAcceptFilter(this.c, holder.doneBtn.getDrawable());
      HSIcons.applyConfirmRejectFilter(this.c, holder.changeBtn.getDrawable());
      holder.separatorLine = convertView.findViewById(16908331);
      convertView.setTag(holder);
    } else {
      holder = (RSCViewHolder)convertView.getTag();
    }

    holder.text1.setText(getText(item.body));

    if (item.inProgress.booleanValue()) {
      holder.progress.setVisibility(0);
      holder.attachBtn.setVisibility(8);
      holder.imagePreview.setVisibility(8);
      holder.image.setImageBitmap(null);
    } else if ((item.screenshot != null) && (!TextUtils.isEmpty(item.screenshot))) {
      holder.progress.setVisibility(8);
      holder.attachBtn.setVisibility(8);
      holder.separatorLine.setVisibility(8);
      holder.imagePreview.setVisibility(0);

      holder.changeBtn.setOnClickListener(new View.OnClickListener()
      {
        public void onClick(View v) {
          if (item.clickable.booleanValue())
            f.removeScreenshot(position);
        }
      });
      holder.doneBtn.setOnClickListener(new View.OnClickListener()
      {
        public void onClick(View v) {
          if (item.clickable.booleanValue())
            f.attachImage(position);
        }
      });
      Bitmap screenshotImage = AttachmentUtil.getBitmap(item.screenshot, 250);
      holder.image.setImageBitmap(screenshotImage);
    } else if (!item.invisible.booleanValue()) {
      holder.progress.setVisibility(8);
      holder.attachBtn.setVisibility(0);
      holder.separatorLine.setVisibility(0);
      holder.attachBtn.setOnClickListener(new View.OnClickListener()
      {
        public void onClick(View v) {
          if (item.clickable.booleanValue())
            f.selectImagePopup(position);
        }
      });
      holder.imagePreview.setVisibility(8);
      holder.image.setImageBitmap(null);
    } else {
      holder.progress.setVisibility(8);
      holder.attachBtn.setVisibility(8);
      holder.separatorLine.setVisibility(8);
      holder.imagePreview.setVisibility(8);
      holder.image.setImageBitmap(null);
    }

    holder.changeBtn.setEnabled(this.enableBtn);
    holder.doneBtn.setEnabled(this.enableBtn);
    holder.attachBtn.setEnabled(this.enableBtn);
    
    addClickListener(holder.text1);
    
    return convertView;
  }

  private View setConvertView(View convertView, final HSMsg item, final int position, LocalRSCViewHolder holder)
  {
    if (convertView == null) {
      convertView = this.inflater.inflate(this.localRscId, null);

      holder.imagePreview = ((LinearLayout)convertView.findViewById(16908299));
      holder.progress = ((ProgressBar)convertView.findViewById(16908301));
      holder.image = ((ImageView)convertView.findViewById(16908304));
      holder.changeBtn = ((ImageButton)convertView.findViewById(16908314));
      holder.doneBtn = ((ImageButton)convertView.findViewById(16908315));
      holder.separatorLine = convertView.findViewById(16908331);

      HSIcons.applyConfirmAcceptFilter(this.c, holder.doneBtn.getDrawable());
      HSIcons.applyConfirmRejectFilter(this.c, holder.changeBtn.getDrawable());

      convertView.setTag(holder);
    } else {
      holder = (LocalRSCViewHolder)convertView.getTag();
    }

    Bitmap screenshotImage = AttachmentUtil.getBitmap(item.screenshot, 250);
    holder.image.setImageBitmap(screenshotImage);

    if (item.inProgress.booleanValue()) {
      holder.imagePreview.setVisibility(0);
      holder.progress.setVisibility(0);
      holder.separatorLine.setVisibility(8);
      holder.changeBtn.setVisibility(8);
      holder.doneBtn.setVisibility(8);
    } else if (!TextUtils.isEmpty(item.screenshot)) {
      holder.imagePreview.setVisibility(0);
      holder.progress.setVisibility(8);
      holder.separatorLine.setVisibility(0);
      holder.changeBtn.setVisibility(0);
      holder.doneBtn.setVisibility(0);
      holder.changeBtn.setOnClickListener(new View.OnClickListener()
      {
        public void onClick(View v) {
          if (item.clickable.booleanValue())
            f.removeScreenshot(position);
        }
      });
      holder.doneBtn.setOnClickListener(new View.OnClickListener()
      {
        public void onClick(View v) {
          if (item.clickable.booleanValue())
            f.attachImage(position);
        }
      });
    }
    else if (item.invisible.booleanValue()) {
      holder.imagePreview.setVisibility(8);
    }

    holder.changeBtn.setEnabled(this.enableBtn);
    holder.doneBtn.setEnabled(this.enableBtn);

    return convertView;
  }

  private View setConvertView(View convertView, HSMsg item, SCViewHolder holder)
  {
    if (convertView == null) {
      convertView = this.inflater.inflate(this.scId, null);

      holder.text1 = ((TextView)convertView.findViewById(R.id.text1));
      holder.progress = ((ProgressBar)convertView.findViewById(16908301));
      holder.image = ((ImageView)convertView.findViewById(16908304));
      convertView.setTag(holder);
    } else {
      holder = (SCViewHolder)convertView.getTag();
    }

    holder.text1.setText(HSRes.getString(this.c, "hs__screenshot_sent_msg"));

    if (TextUtils.isEmpty(item.screenshot)) {
      holder.progress.setVisibility(0);
      holder.image.setVisibility(8);
      holder.image.setImageBitmap(null);
    } else {
      holder.progress.setVisibility(8);
      holder.image.setVisibility(0);
      Bitmap screenshotImage = AttachmentUtil.getBitmap(item.screenshot, -1);
      holder.image.setImageBitmap(screenshotImage);
    }
    
    addClickListener(holder.text1);
    
    return convertView;
  }

  private View setConvertView(View convertView, final HSMsg item, final int position, RARViewHolder holder)
  {
    if (convertView == null) {
      convertView = this.inflater.inflate(this.rarId, null);

      holder.message = ((TextView)convertView.findViewById(R.id.text1));
      holder.progress = ((ProgressBar)convertView.findViewById(16908301));
      holder.reviewBtn = ((ImageButton)convertView.findViewById(16908313));
      HSIcons.applyReviewFilter(this.c, holder.reviewBtn.getDrawable());
      holder.separatorLine = convertView.findViewById(16908331);
      convertView.setTag(holder);
    } else {
      holder = (RARViewHolder)convertView.getTag();
    }

    holder.message.setText(HSRes.getString(this.c, "hs__review_request_message"));

    if (item.inProgress.booleanValue()) {
      holder.progress.setVisibility(0);
      holder.reviewBtn.setVisibility(8);
      holder.separatorLine.setVisibility(8);
    } else if (!item.invisible.booleanValue()) {
      holder.progress.setVisibility(8);
      holder.reviewBtn.setVisibility(0);
      holder.separatorLine.setVisibility(0);
      holder.reviewBtn.setOnClickListener(new View.OnClickListener()
      {
        public void onClick(View v) {
          if (item.clickable.booleanValue())
            f.replyReview(item.id, position);
        }
      });
    }
    else {
      holder.progress.setVisibility(8);
      holder.reviewBtn.setVisibility(8);
      holder.separatorLine.setVisibility(8);
    }

    return convertView;
  }

  private View setConvertView(View convertView, HSMsg item, ARViewHolder holder)
  {
    if (convertView == null) {
      convertView = this.inflater.inflate(this.arId, null);

      holder.text1 = ((TextView)convertView.findViewById(R.id.text1));
      convertView.setTag(holder);
    } else {
      holder = (ARViewHolder)convertView.getTag();
    }

    holder.text1.setText(HSRes.getString(this.c, "hs__review_accepted_message"));
    addClickListener(holder.text1);

    return convertView;
  }
  
  private void addClickListener(TextView textView)
  {
	textView.setOnClickListener(new View.OnClickListener()
    {
        public void onClick(View view) {
        	System.out.println("holder.text1 onClick");
        	quickAction.currentTextView = (TextView)view;
			quickAction.show(view);
//			quickAction.setAnimStyle(QuickAction.ANIM_REFLECT);
        }
    });
  }
  
	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	public void copyToClipboard()
	{
		if(quickAction.currentTextView == null) return;
		
		DateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		String date = df.format(Calendar.getInstance().getTime());
		
		int sdk = android.os.Build.VERSION.SDK_INT;
		if(sdk < android.os.Build.VERSION_CODES.HONEYCOMB) {
		    android.text.ClipboardManager clipboard = (android.text.ClipboardManager) ((Activity)c).getSystemService(Context.CLIPBOARD_SERVICE);
		    clipboard.setText(quickAction.currentTextView.getText());
		} else {
		    android.content.ClipboardManager clipboard = (android.content.ClipboardManager) ((Activity)c).getSystemService(Context.CLIPBOARD_SERVICE); 
		    android.content.ClipData clip = android.content.ClipData.newPlainText("cok "+date, quickAction.currentTextView.getText());
		    clipboard.setPrimaryClip(clip);
		}
	}

  private String getText(String input)
  {
    return Html.fromHtml(input.replace("\n", "<br/>")).toString();
  }

  private static class ARViewHolder
  {
    public TextView text1;
  }

  private static class RARViewHolder
  {
    public TextView message;
    public ProgressBar progress;
    public ImageButton reviewBtn;
    public View separatorLine;
  }

  private static class SCViewHolder
  {
    public TextView text1;
    public ProgressBar progress;
    public ImageView image;
  }

  private static class LocalRSCViewHolder
  {
    public LinearLayout imagePreview;
    public ProgressBar progress;
    public ImageView image;
    public ImageButton changeBtn;
    public ImageButton doneBtn;
    public View separatorLine;
  }

  private static class RSCViewHolder
  {
    public TextView text1;
    public ImageButton attachBtn;
    public ProgressBar progress;
    public LinearLayout imagePreview;
    public ImageView image;
    public ImageButton changeBtn;
    public ImageButton doneBtn;
    public View separatorLine;
  }

  private static class CSViewHolder
  {
    public TextView message;
    public TextView text1;
    public TextView text2;
  }

  private static class CBViewHolder
  {
    public TextView text1;
    public ProgressBar progress;
    public LinearLayout btnContainer;
    public ImageButton button1;
    public ImageButton button2;
  }

  private static class TxtUserHolder
  {
    public TextView text1;
    public TextView text2;
    public ImageView errorImage;
  }

  private static class TxtAdminHolder
  {
    public TextView text1;
    public TextView text2;
  }
}