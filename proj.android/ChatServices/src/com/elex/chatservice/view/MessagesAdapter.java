package com.elex.chatservice.view;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.commons.lang.StringUtils;

import net.londatiga.android.QuickAction;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.support.v4.app.Fragment;
import android.text.Html;
import android.text.Html.ImageGetter;
import android.text.Layout.Alignment;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.SpannableStringBuilder;
import android.text.Spanned;
import android.text.TextUtils;
import android.text.method.LinkMovementMethod;
import android.text.style.AlignmentSpan;
import android.text.style.ClickableSpan;
import android.text.style.ForegroundColorSpan;
import android.text.style.URLSpan;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.ArrayAdapter;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.elex.chatservice.R;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.MenuController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChannelView;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.TranslateManager;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.util.CompatibleApiUtil;
import com.elex.chatservice.util.ImageUtil;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.util.ScaleUtil;
import com.elex.chatservice.util.TranslateListener;
import com.elex.chatservice.util.TranslateUtil;

public final class MessagesAdapter extends ArrayAdapter<MsgItem>
{
	// private Fragment f;
	private Context c;
	private List<MsgItem> items;
	private LayoutInflater inflater;
	private QuickAction quickAction;

	public MessagesAdapter(Fragment f, int textViewResourceId, List<MsgItem> objects)
	{
		super(f.getActivity(), textViewResourceId, objects);
		// this.f = ((Fragment) f);
		this.c = f.getActivity();
		this.items = objects;
		this.inflater = ((LayoutInflater) this.c.getSystemService("layout_inflater"));
	}

	private QuickAction.OnActionItemClickListener actionClickListener = new QuickAction.OnActionItemClickListener()
	{

		@Override
		public void onItemClick(QuickAction source, int pos, int actionId)
		{
			handleItemClick(source, pos, actionId);
		}
	};

	private void handleItemClick(final QuickAction source, final int pos, final int actionId)
	{
		final MessagesAdapter adapter = this;
		try
		{
			MenuController.handleItemClick(adapter, source, pos, actionId);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	public static MsgItem getMsgItemFromQuickAction(QuickAction quickAction)
	{
		return ((MessageHolder) quickAction.currentTextView.getTag()).msgItem;
	}

	public void showTranslatedLanguage(MessageHolder holder,final MsgItem item)
	{
//		System.out.println("translateMsg:"+item.translateMsg+"  item.translatedLang:"+item.translatedLang+"  ConfigManager.getInstance().gameLang:"+ConfigManager.getInstance().gameLang);
		if(item.hasTranslation() && (item.translatedLang==null || item.translatedLang.equals("") || item.translatedLang.equals(ConfigManager.getInstance().gameLang)))
		{
			item.hasTranslated = true;
			item.isTranslatedByForce=true;
			setText(holder.getMessageText(), item.translateMsg, item,true);
			if(item.translatedLang==null || item.translatedLang.equals("")){
				item.translatedLang=ConfigManager.getInstance().gameLang;
				ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
				// 极少情况下会发生
				if(channel != null){
					DBManager.getInstance().updateMessage(item, channel.getChatTable());
				}
			}
		}
		else
		{
			if(ConfigManager.autoTranlateMode <= 0)
				return;
//			ChatServiceController.getInstance().host.translateMsgByLua(item.msg, ConfigManager.getInstance().gameLang);
//			System.out.println("使用google翻译");
			final TextView textView=holder.getMessageText();
			TranslateUtil.loadTranslate(item.msg,new TranslateListener() {
				
				@Override
				public void onTranslateFinish(String translateMsg,String originalLang) {
					if(!originalLang.equals("")&&!translateMsg.equals(""))
					{
						item.hasTranslated = true;
						item.translateMsg=translateMsg;
						item.originalLang=originalLang;
						item.isTranslatedByForce=true;
						setText(textView,translateMsg, item,true);
						ChatServiceController.getInstance().host.callXCApi();
						
						item.translatedLang=ConfigManager.getInstance().gameLang;
						ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
						// 极少情况为null
						if(channel != null){
							DBManager.getInstance().updateMessage(item, channel.getChatTable());
						}
					}
				 }
			});
		}
	}
	
	public void showOriginalLanguage(MessageHolder holder, MsgItem item)
	{
		item.hasTranslated = false;
		item.isTranslatedByForce=false;
		setText(holder.getMessageText(), item.msg, item,false);
	}

	// TODO 返回值是否正确？
	public boolean isEnabled(int position)
	{
		return false;
	}

	// TODO 返回值是否正确？
	public int getViewTypeCount()
	{
		return 20;
	}

	public int getItemViewType(int position)
	{
		// 统计到过几次异常 IndexOutOfBoundsException: Invalid index 4, size is 0
		if (!(position >= 0 && position < items.size())) return 1;

		MsgItem item = (MsgItem) this.items.get(position);

		if (item.isSelfMsg)
		{
			return 2;
		}
		else
		{
			return 1;
		}
	}

	public View getView(int position, View convertView, ViewGroup parent)
	{
		MsgItem item = null;

		if (position < 0 || position >= items.size())
		{
			return null;
		}
		else
		{
			item = (MsgItem) this.items.get(position);
			if (item == null) 
			{
				System.out.println("item == null");
				return null;
			}
		}
		return setConvertView(convertView, item, R.layout.cs__msg_txt, new MessageHolder());
	}

	private boolean isDifferentDate(MsgItem item)
	{
		boolean result = true;
		if (items.indexOf(item) == 0)
		{
			result = true;
		}
		else
		{
			try
			{
				// item有可能不在items中，items.indexOf(item)得-1
				result = !item.getSendTimeYMD().equals(items.get(items.indexOf(item) - 1).getSendTimeYMD());
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
		return result;
	}

	public void setProgressBarState(boolean showProgressBar, boolean showSendFailImage)
	{

	}
	
	private int getGmodResourceId(int gmod)
	{
		int idPrivilegeImage = 0;
		switch (gmod)
		{
			case 2:
				idPrivilegeImage = R.drawable.mod;
				break;
			case 4:
				idPrivilegeImage = R.drawable.smod;
				break;
			case 5:
				idPrivilegeImage = R.drawable.tmod;
				break;
			case 3:
				idPrivilegeImage = R.drawable.gm;
				break;
			default:
				break;
		}
		return idPrivilegeImage;
	}
	
	private static void setMsgTextViewPadding(TextView textView)
	{
		int paddingLeft = textView.getPaddingLeft();
		int paddingRight = textView.getPaddingRight();
		int paddingTop = textView.getPaddingTop();
		int paddingBottom = textView.getPaddingBottom();
		textView.setPadding(paddingLeft, paddingTop, paddingRight, paddingBottom);

	}

	@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
	private View setConvertView(View convertView, final MsgItem item, int layoutID, MessageHolder holder)
	{
		int idPrivilegeImage = getGmodResourceId(item.getGmod());
		if (convertView == null)
		{
//			System.out.println("convertView == null");
			try
			{
				convertView = this.inflater.inflate(layoutID, null);
			}
			catch (Exception ex)
			{
				LogUtil.printException(ex);
			}

			holder.findView(convertView);
			adjustSize(holder, idPrivilegeImage);
		}
		else
		{
//			System.out.println("convertView != null");
			holder = (MessageHolder) convertView.getTag();
		}
		
		holder.setCurrentHolderStyle(item.isTipMsg(),item.isSelfMsg);
		
		int sdk = android.os.Build.VERSION.SDK_INT;
		holder.setMsgTextBackground(c, item.isSystemMessage(),item.isHornMessage(), sdk);
		holder.getName_layout().setVisibility((ChatServiceController.getCurrentChannelType()==DBDefinition.CHANNEL_TYPE_USER)?View.GONE:View.VISIBLE);
		
		if(item.firstNewMsgState == 1 || item.firstNewMsgState == 2){
			holder.getNewMsgLayout().setVisibility(View.VISIBLE);
			if(item.firstNewMsgState == 1){
				holder.getNewMsgLabel().setText(LanguageManager.getLangByKey(LanguageKeys.TIP_NEW_MESSAGE_BELOW));
			}else{
//				holder.getNewMsgLabel().setText("Below are latest 200 new messages");
				holder.getNewMsgLabel().setText(
						LanguageManager.getLangByKey(LanguageKeys.TIP_N_NEW_MESSAGE_BELOW, ChannelManager.LOAD_ALL_MORE_MAX_COUNT + ""));
			}
		}
		else
		{
			holder.getNewMsgLayout().setVisibility(View.GONE);
		}
		
		if (isDifferentDate(item) || item.isSendDataShowed)
		{
			if(!item.isSendDataShowed)
				item.isSendDataShowed=true;
			holder.getSendDateLayout().setVisibility(View.VISIBLE);
			holder.getSendDateLabel().setText(item.getSendTimeYMD());
		}
		else
		{
			holder.getSendDateLayout().setVisibility(View.GONE);
		}

		holder.getHeadImageContainer().setOnTouchListener(new View.OnTouchListener()
		{
			@Override
			public boolean onTouch(View v, MotionEvent event)
			{
				int iAction = event.getAction();
				if (iAction == MotionEvent.ACTION_DOWN || iAction == MotionEvent.ACTION_MOVE)
				{
					CompatibleApiUtil.getInstance().setButtonAlpha(v, false);
				}
				else
				{
					CompatibleApiUtil.getInstance().setButtonAlpha(v, true);
				}
				return false;
			}
		});
		
		holder.getHeadImageContainer().setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(final View view)
			{
				if(!item.isSystemHornMsg())
				{
					System.out.println("getHeadImageContainer onClick ChatServiceController.isContactMod:"+ChatServiceController.isContactMod);
					if(ChatServiceController.isContactMod)
						ChatServiceController.doHostAction("showPlayerInfo@mod", item.uid, item.getName(), "", true);
					else
						ChatServiceController.doHostAction("showPlayerInfo", item.uid, item.getName(), "", true);
				}
			}
		});

		if (item.canShowTranslateMsg())
		{
			if(!item.isTipMsg())
				setText(holder.getMessageText(), item.translateMsg, item,true);
			else
				holder.getMessageText().setText(item.translateMsg);
			TranslateManager.getInstance().enterTranlateQueue(item);
		}
		else
		{
			if(!item.isTipMsg())
				setText(holder.getMessageText(), item.msg, item,false);
			else
				holder.getMessageText().setText(item.msg);
			TranslateManager.getInstance().enterTranlateQueue(item);
		}
		
		
		holder.msgItem = item;
		holder.getVipLabel().setText(item.getVipLabel());
		if(ChatServiceController.getCurrentChannelType() != DBDefinition.CHANNEL_TYPE_ALLIANCE){
			holder.getAllianceLabel().setText(item.getAllianceLabel());
		}else{
			holder.getAllianceLabel().setText("");
		}
//		String name=UserManager.getInstance().getNameById(item.uid);
//		System.out.println("getNameLabel name:"+name);
		holder.getNameLabel().setText(item.getName() + (item.getSrcServerId() > 0 ? "#" + item.getSrcServerId() : ""));
		
		ImageUtil.setHeadImage(c, item.getHeadPic(), holder.getHeadImage(), item.getUser());

		if (idPrivilegeImage != 0)
		{
			holder.getPrivilegeImage().setImageResource(idPrivilegeImage);
		}
		else
		{
			holder.getPrivilegeImage().setImageDrawable(null);
		}
		
		holder.getSendFail_image().setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				if (item.sendState != MsgItem.SEND_SUCCESS)
					MenuController.showReSendConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_RESEND), item);
			}
		});

		refreshSendingStatus(item, holder);
		if(!item.isTipMsg())
			addClickListener(holder.getMessageText());
		return convertView;
	}

	// notify之后，holder会重新创建，这也导致转圈都同步转，重发的逻辑应确保正确复用这里（sendTimerTask是否要重新创建）
	// √1.让各个进度圈独立停止（因为refreshSendState后没有刷新UI）
	// 2.把进度圈的progress记录下来
	// 3.切换时会卡，如何提高性能
	private void refreshSendingStatus(final MsgItem item, final MessageHolder holder)
	{
		refreshSendState(holder);
		if (item.isSelfMsg && (!item.isSystemMessage() || item.isHornMessage()))
		{
			if(item.sendState == MsgItem.SENDING){
				// holder.stopSendTimer();
				if (holder.sendTimer == null)
				{
					holder.sendTimer = new Timer();
				}
				// 只需创建一次？
				if (holder.sendTimerTask == null)
				{
					holder.sendTimerTask = new TimerTask()
					{
						@Override
						public void run()
						{
							if (holder.msgItem.sendState == MsgItem.SENDING)
							{
								holder.msgItem.sendState = MsgItem.SEND_FAILED;
							}

							refreshSendState(holder);
							refreshUI(item.channelType);

							holder.removeSendTimer();
						}
					};

					holder.sendTimer.schedule(holder.sendTimerTask, 10000);
				}
			}else{
				refreshUI(item.channelType);
				if (holder.sendTimer != null)
				{
					holder.removeSendTimer();
				}
			}
		}
	}
	
	private ChatFragment getChatFragment()
	{
		if(c != null && c instanceof ChatActivity && ((ChatActivity) c).fragment != null
				&& ((ChatActivity) c).fragment instanceof ChatFragment){
			return ((ChatFragment)((ChatActivity) c).fragment);
		}else{
			return null;
		}
	}
	
	public void refreshUI(final int channelType)
	{
		if(c == null) return;

		((Activity) c).runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					
					ChannelView channelView = getChatFragment().getChannelView(ChannelManager.channelType2tab(channelType));
					if (channelView != null && channelView.getMessagesAdapter() != null)
						channelView.getMessagesAdapter().notifyDataSetChanged();
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	public void refreshSendState(final MessageHolder holder)
	{
		if(c == null) return;

		((Activity) c).runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (holder.msgItem.sendState == MsgItem.SENDING)
					{
						holder.getSend_progressbar().setVisibility(View.VISIBLE);
						holder.getSendFail_image().setVisibility(View.GONE);
					}
					else if (holder.msgItem.sendState == MsgItem.SEND_FAILED)
					{
						holder.getSend_progressbar().setVisibility(View.GONE);
						holder.getSendFail_image().setVisibility(View.VISIBLE);
					}
					else if (holder.msgItem.sendState == MsgItem.SEND_SUCCESS)
					{
						holder.getSend_progressbar().setVisibility(View.GONE);
						holder.getSendFail_image().setVisibility(View.GONE);
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	private static final String JOIN_NOW_URL = "JoinNow";
	private static final String SHOW_EQUIP_URL = "ShowEquip";

	private String convertLineBreak(String input)
	{
		return input.replace("\n", "<br/>");
	}
	
	private int getColorByIndex(int index)
	{
		int color = 0;
		switch (index)
		{
			case 0:
				color = 0xffadafac;
				break;
			case 1:
				color = 0xff539721;
				break;
			case 2:
				color = 0xff4784e1;
				break;
			case 3:
				color = 0xff7048c2;
				break;
			case 4:
				color = 0xffc18148;
				break;
			case 5:
				color = 0xffcdb73f;
				break;

			default:
				color = 0xffadafac;
				break;
		}
		return color;
	}
	
	class EquipURLSpan extends ClickableSpan
	{

		private String mURL = "";
		public EquipURLSpan(String url)
		{
			mURL = url;
		}
		
		@Override
		public void onClick(View widget)
		{
			System.out.println("url:"+mURL);
		}
	}

	private void setText(TextView textView, String str, MsgItem item,boolean isTranslated)
	{
		String equipName = "";
		int colorIndex = -1;
		if(item.isEquipMessage())
		{
			String msgStr = item.msg;
			if(StringUtils.isNotEmpty(msgStr))
			{
				String[] equipInfo = msgStr.split("\\|");
				if(equipInfo.length==2)
				{
					equipName = equipInfo[1];
					colorIndex = Integer.parseInt(equipInfo[0]);
				}
			}
			str = LanguageManager.getLangByKey(LanguageKeys.TIP_EQUIP_SHARE,equipName);
		}
		
		item.currentText = str;
		// 将html特殊符号进行转义，否则"<"后面的内容会被Html.fromHtml吞掉
		str = TextUtils.htmlEncode(str);

		// 转化坐标为链接
		String htmlLinkText = str;
		htmlLinkText = insertCoordinateLink(convertLineBreak(str));
		// annouce invite的链接，玩家不在联盟中才可见
		if (item.isAnnounceInvite() && UserManager.getInstance().getCurrentUser().allianceId.equals(""))
		{
			htmlLinkText += "<a href='" + JOIN_NOW_URL + "," + item.attachmentId + "'><u> "
					+ LanguageManager.getLangByKey(LanguageKeys.BTN_JOIN_NOW) + " </u></a>";
		}

		Spanned spannedText = Html.fromHtml(htmlLinkText);
		textView.setText(spannedText);
		
		textView.setMovementMethod(LinkMovementMethod.getInstance());
		
		CharSequence text = textView.getText();
		if (text instanceof Spannable)
		{
			int end = text.length();

			CharSequence text2 = textView.getText();
			if (item.isSystemMessage())
			{
				// text2=" "+textView.getText();
			}
			
			SpannableStringBuilder style = new SpannableStringBuilder(text2);
			style.clearSpans();// should clear old spans

			if (ChatServiceController.getCurrentChannelType() < DBDefinition.CHANNEL_TYPE_USER && item.isSystemMessage() && !item.isHornMessage())
			{
				ImageGetter imageGetter = new ImageGetter()
				{
					@Override
					public Drawable getDrawable(String source)
					{
						if(c == null) return null;
						
						Drawable d = c.getResources().getDrawable(R.drawable.sys);
						if (ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
						{
							d.setBounds(0, 0, (int) (d.getIntrinsicWidth() * ConfigManager.scaleRatio * getScreenCorrectionFactor()),
									(int) (d.getIntrinsicHeight() * ConfigManager.scaleRatio * getScreenCorrectionFactor()) + 6);
						}
						else
						{
							d.setBounds(0, 0, (int) (d.getIntrinsicWidth()), (int) (d.getIntrinsicHeight()) + 6);
						}
						// ((BitmapDrawable) d).setGravity(Gravity.TOP);
						return d;
					}
				};

				style.insert(0, Html.fromHtml("<img src='" + R.drawable.sys + "'/>", imageGetter, null));
			}

			// 添加末尾的战报图标
			boolean canViewBattleReport = ((item.isBattleReport() || item.isDetectReport()) && !UserManager.getInstance().getCurrentUser().allianceId
					.equals(""));
			if (canViewBattleReport)
			{
				ImageGetter imageGetter = new ImageGetter()
				{
					@Override
					public Drawable getDrawable(String source)
					{
						if(c == null) return null;
						
						Drawable d = c.getResources().getDrawable(R.drawable.mail_battlereport);
						if (ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
						{
							d.setBounds(0, 0, (int) (d.getIntrinsicWidth() * 0.5 * ConfigManager.scaleRatio * getScreenCorrectionFactor()),
									(int) (d.getIntrinsicHeight() * 0.5 * ConfigManager.scaleRatio * getScreenCorrectionFactor()) + 5);
						}
						else
						{
							d.setBounds(0, 0, (int) (d.getIntrinsicWidth() * 0.5), (int) (d.getIntrinsicHeight() * 0.5) + 5);
						}
						return d;
					}
				};

				style.append(Html.fromHtml("<img src='" + R.drawable.mail_battlereport + "'/>", imageGetter, null));
			}
			
			// 添加末尾的装备分享
			if (item.isEquipMessage())
			{
				
				int color =getColorByIndex(colorIndex);
				String txt = text.toString();
				int start = txt.indexOf(equipName)+1;
				style.setSpan(new ForegroundColorSpan(color), start, start+equipName.length(),
						Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
				
				ImageGetter imageGetter = new ImageGetter()
				{
					@Override
					public Drawable getDrawable(String source)
					{
						if(c == null) return null;
						
						Drawable d = c.getResources().getDrawable(R.drawable.equip_share);
						if (ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
						{
							d.setBounds(0, 0, (int) (d.getIntrinsicWidth() * 0.8 * ConfigManager.scaleRatio * getScreenCorrectionFactor()),
									(int) (d.getIntrinsicHeight() * 0.8 * ConfigManager.scaleRatio * getScreenCorrectionFactor()));
						}
						else
						{
							d.setBounds(0, 0, (int) (d.getIntrinsicWidth() * 0.5), (int) (d.getIntrinsicHeight() * 0.5) + 5);
						}
						return d;
					}
				};

				style.append(Html.fromHtml("<img src='" + R.drawable.equip_share + "'/>", imageGetter, null));
			}			

			// 添加时间、翻译信息
			String time = item.getSendTimeHM();
			if (isTranslated)
			{
				time += " " + LanguageManager.getLangByKey(LanguageKeys.TIP_TRANSLATED_BY,item.originalLang);
			}
			else if (!isTranslated && item.hasTranslation() && !item.isTranlateDisable() && !item.isOriginalSameAsTargetLang())
			{
				time += " " + LanguageManager.getLangByKey(LanguageKeys.MENU_ORIGINALLAN);
			}
			
			SpannableString styledResultText = new SpannableString(Html.fromHtml("<br/>" + "<small><font color='#808080'>" + time
					+ "</font></small>"));
			// if(item.isMyself())
			styledResultText.setSpan(new AlignmentSpan.Standard(Alignment.ALIGN_OPPOSITE), 0, styledResultText.length(),
					Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
			style.append(styledResultText);

			Spannable sp = (Spannable) textView.getText();
			URLSpan[] urls = sp.getSpans(0, end, URLSpan.class);
			for (URLSpan url : urls)
			{
				MyURLSpan myURLSpan = new MyURLSpan(url.getURL());
				if (item.isSystemMessage())
				{// item.gmod==2 || item.gmod==3 || item.gmod==4||item.gmod==5
					
					style.setSpan(myURLSpan, sp.getSpanStart(url) + 1, sp.getSpanEnd(url)+1,
							Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
				}
				else
				{
					style.setSpan(myURLSpan, sp.getSpanStart(url), sp.getSpanEnd(url) - 1, Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
				}
			}
			
			textView.setText(style);
		}
	}

	private String insertCoordinateLink(String htmlLinkText)
	{
		// (1200|[1][0-1][0-9]{2}|[1-9][0-9]{2}|[1-9][0-9]|[0-9])
		// 逆序可贪婪匹配，遇到不合法数字时可能只匹配合法部分
		// htmlLinkText = htmlLinkText.replaceAll("\\(([0-9]+),([0-9]+)\\)",
		// "<a href='$1,$2'><u>($1,$2)</u></a>");
		htmlLinkText = htmlLinkText.replaceAll(
				"(1200|[1][0-1][0-9]{2}|[1-9][0-9]{2}|[1-9][0-9]|[0-9])(:|：|: |： )(1200|[1][0-1][0-9]{2}|[1-9][0-9]{2}|[1-9][0-9]|[0-9])",
				"<a href='$1,$3'><u> $1:$3 </u></a>");
		return htmlLinkText;
	}

	private TextView joinAnnounceTextView;
	private MsgItem joinAnnounceItem;

	public void onJoinAnnounceInvitationSuccess()
	{
		if (joinAnnounceTextView != null && joinAnnounceItem != null)
		{
			if(joinAnnounceItem.hasTranslated)
				setText(joinAnnounceTextView, joinAnnounceItem.currentText, joinAnnounceItem,true);
			else
				setText(joinAnnounceTextView, joinAnnounceItem.currentText, joinAnnounceItem,false);
		}
		joinAnnounceItem = null;
		joinAnnounceTextView = null;
	}

	private void onURLClick(View widget, String url)
	{
		ignoreClick = true;
		final String[] coords = url.split(",");

		if (coords[0].equals(JOIN_NOW_URL))
		{
			try
			{
				joinAnnounceTextView = (TextView) widget;
				MessageHolder holder = (MessageHolder) widget.getTag();
				joinAnnounceItem = holder.msgItem;

				if (coords.length == 2)
				{
					ServiceInterface.allianceIdJoining = coords[1];
					ChatServiceController.getInstance().host.joinAnnounceInvitation(coords[1]);
				}
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
			return;
		}
		else if (coords[0].equals(SHOW_EQUIP_URL))
		{
			try
			{
				System.out.println("showequip:"+coords[0]);
				if (coords.length == 2 && StringUtils.isNotEmpty(coords[1]))
				{
					System.out.println("showequip:"+coords[1]);
					ChatServiceController.doHostAction("showEquipment", "", "", coords[1], true);
				}
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
			return;
		}
		else
		{
			if (!isCoordinateValid(coords[0]) || !isCoordinateValid(coords[1]))
			{
				Toast.makeText(c, "coordinate (" + coords[0] + "," + coords[1] + ") is invalid!", Toast.LENGTH_LONG).show();
				return;
			}
			ChatServiceController.doHostAction("gotoCoordinate", coords[0], coords[1], "", false);
		}
	}
	
	private class MyURLSpan extends ClickableSpan
	{
		private String mUrl;

		MyURLSpan(String url)
		{
			mUrl = url;
		}

		@Override
		public void onClick(View widget)
		{
			onURLClick(widget, mUrl);
		}
	}

	// 合法坐标[0,1200]
	public boolean isCoordinateValid(String coord)
	{
		return Integer.parseInt(coord) >= 0 && Integer.parseInt(coord) <= 1200;
	}

//	protected SpannableStringBuilder getClickableHTML(String html)
//	{
//		CharSequence sequence = Html.fromHtml(html);
//		SpannableStringBuilder strBuilder = new SpannableStringBuilder(sequence);
//		URLSpan[] urls = strBuilder.getSpans(0, sequence.length(), URLSpan.class);
//		for (URLSpan span : urls)
//		{
//			makeLinkClickable(strBuilder, span);
//		}
//
//		// text.setText(strBuilder);
//		return strBuilder;
//	}
//
//	protected void makeLinkClickable(SpannableStringBuilder strBuilder, final URLSpan span)
//	{
//		int start = strBuilder.getSpanStart(span);
//		int end = strBuilder.getSpanEnd(span);
//		int flags = strBuilder.getSpanFlags(span);
//		ClickableSpan clickable = new ClickableSpan()
//		{
//			public void onClick(View view)
//			{
//				Toast.makeText(c, "dfsdffdsf", Toast.LENGTH_LONG).show();
//			}
//		};
//		strBuilder.setSpan(clickable, start, end, flags);
//		strBuilder.removeSpan(span);
//	}

	/**
	 * 高ppi手机的缩放修正因子
	 */
	public double getScreenCorrectionFactor()
	{
		int density = c.getResources().getDisplayMetrics().densityDpi;

		return density == DisplayMetrics.DENSITY_XXHIGH ? 0.8 : 1.0;
	}

	private void adjustSize(MessageHolder holder, int idPrivilegeImage)
	{
		if (ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
		{// && holder.headImageContainer.getWidth() > 0
			int headWidthDP = 60;
			holder.adjustTextSize();
			
			holder.setHeadImageContainerLayoutParams(new LinearLayout.LayoutParams((int) (ScaleUtil.dip2px(c, headWidthDP)
					* ConfigManager.scaleRatio * getScreenCorrectionFactor()), (int) (ScaleUtil.dip2px(c, headWidthDP)
					* ConfigManager.scaleRatio * getScreenCorrectionFactor())));

			if (idPrivilegeImage != 0)
			{
				int originalPrivilegeWidth = 57;
				int originalPrivilegeHeadHeight = 27;
				try
				{
					BitmapDrawable bd = (BitmapDrawable) c.getResources().getDrawable(idPrivilegeImage);
					originalPrivilegeWidth = bd.getBitmap().getWidth();
					originalPrivilegeHeadHeight = bd.getBitmap().getHeight();
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
				int targetHeight = (int) (ScaleUtil.dip2px(c, headWidthDP) * ConfigManager.scaleRatio * 0.2 * getScreenCorrectionFactor());
				int privilegeWidth = targetHeight * originalPrivilegeWidth / originalPrivilegeHeadHeight;
				int privilegeHeadHeight = targetHeight;
				holder.setPrivilegeImageLayoutParams(new FrameLayout.LayoutParams(privilegeWidth, privilegeHeadHeight, Gravity.BOTTOM
						| Gravity.RIGHT));
			}
		}

		holder.adjustSizeCompleted = true;
	}

	private boolean ignoreClick = false;

	private void addClickListener(TextView textView)
	{
		textView.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View view)
			{
				if (c == null || ((ChatActivity) c).fragment == null || ((ChatFragment)((ChatActivity) c).fragment).isKeyBoradShowing)
				{
					return;
				}
				if (ignoreClick)
				{
					ignoreClick = false;
					return;
				}
				MsgItem item = ((MessageHolder) view.getTag()).msgItem;
				// System.out.println("alliance:"+item.asn+" my-alliance:"+UserManager.getInstance().getCurrentUser().allianceId);

				if (quickAction != null)
				{
					quickAction.setOnActionItemClickListener(null);
				}

				quickAction = ChatQuickActionFactory.createQuickAction((Activity) c, item);
				quickAction.setOnActionItemClickListener(actionClickListener);

				quickAction.currentTextView = (TextView) view;
				quickAction.show(view);
				// quickAction.setAnimStyle(QuickAction.ANIM_REFLECT);
			}
		});
	}

	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	public void copyToClipboard(QuickAction source)
	{
		if (source.currentTextView == null || !(source.currentTextView instanceof TextView)) return;

		MsgItem item = getMsgItemFromQuickAction(source);
		TextView messageText = (TextView) source.currentTextView;
		String text = messageText.getText().toString();

		int sdk = android.os.Build.VERSION.SDK_INT;
		if (sdk < android.os.Build.VERSION_CODES.HONEYCOMB)
		{
			android.text.ClipboardManager clipboard = (android.text.ClipboardManager) ((Activity) c)
					.getSystemService(Context.CLIPBOARD_SERVICE);
			clipboard.setText(stripHtml(text));
		}
		else
		{
			// 一个label对应一个clipboard slot
			android.content.ClipboardManager clipboard = (android.content.ClipboardManager) ((Activity) c)
					.getSystemService(Context.CLIPBOARD_SERVICE);
			android.content.ClipData clip = android.content.ClipData.newPlainText("cok_" + item.getName() + "_" + item.getSendTime(), stripHtml(text));
			clipboard.setPrimaryClip(clip);
		}
	}

	// 去掉复制后文本中的[obj]块（图片导致）
	// http://stackoverflow.com/questions/8560045/android-getting-obj-using-textview-settextcharactersequence
	public CharSequence stripHtml(String s)
	{
		String result = s.substring(0, s.lastIndexOf("\n"));
		result = result.replaceAll("\n", "#linebreak#");
		result = Html.fromHtml(result).toString().replace((char) 65532, (char) 32);
		result = result.replaceAll("#linebreak#", "\n");
		result = result.trim();
		return result;
	}
	
	public static class MessageHolder
	{
		public MsgItem msgItem;
		
		private LinearLayout msg_layout_center;
		private TextView messageText_center;
		
		private LinearLayout msg_layout_left;
		private LinearLayout msg_layout_right;
		private TextView messageText_left;
		private TextView vipLabel_left;
		private LinearLayout name_layout_left;
		private TextView allianceLabel_left;
		private TextView nameLabel_left;
		private ImageView headImage_left;
		private ImageView privilegeImage_left;
		private FrameLayout headImageContainer_left;
		private ProgressBar send_progressbar_left;
		private ImageView sendFail_image_left;
		
		private TextView messageText_right;
		private TextView vipLabel_right;
		private LinearLayout name_layout_right;
		public TextView allianceLabel_right;
		private TextView nameLabel_right;
		private ImageView headImage_right;
		private ImageView privilegeImage_right;
		private FrameLayout headImageContainer_right;
		private ProgressBar send_progressbar_right;
		private ImageView sendFail_image_right;
		private LinearLayout newMsgLayout;
		private TextView newMsgLabel;
		private LinearLayout sendDateLayout;
		private TextView sendDateLabel;
		
		public boolean adjustSizeCompleted = false;
		public int privilegeImageOriginalWidth = -1;
		public int privilegeImageOriginalHeight = -1;
		private Timer sendTimer;
		private TimerTask sendTimerTask;
		public boolean inited = false;
		private boolean isSelfMsg;
		private boolean isTipMsg;

		
		public void findView(View convertView)
		{
			newMsgLabel = ((TextView) convertView.findViewById(R.id.newMsgLabel));
			newMsgLayout = ((LinearLayout) convertView.findViewById(R.id.newMsgLayout));
			
			sendDateLabel = ((TextView) convertView.findViewById(R.id.sendDateLabel));
			sendDateLayout = ((LinearLayout) convertView.findViewById(R.id.sendDateLayout));
			
			msg_layout_center=((LinearLayout) convertView.findViewById(R.id.msg_layout_center));
			messageText_center=((TextView) convertView.findViewById(R.id.messageText_center));
			
			msg_layout_left = ((LinearLayout) convertView.findViewById(R.id.msg_layout_left));
			name_layout_left = ((LinearLayout) convertView.findViewById(R.id.name_layout_left));
			messageText_left = ((TextView) convertView.findViewById(R.id.messageText_left));
			vipLabel_left = ((TextView) convertView.findViewById(R.id.vipLabel_left));
			allianceLabel_left = ((TextView) convertView.findViewById(R.id.allianceLabel_left));
			nameLabel_left = ((TextView) convertView.findViewById(R.id.nameLabel_left));
			headImage_left = ((ImageView) convertView.findViewById(R.id.headImage_left));
			privilegeImage_left = ((ImageView) convertView.findViewById(R.id.privilegeImage_left));
			headImageContainer_left = ((FrameLayout) convertView.findViewById(R.id.headImageContainer_left));
			send_progressbar_left = (ProgressBar) convertView.findViewById(R.id.send_progressbar_left);
			sendFail_image_left = (ImageView) convertView.findViewById(R.id.sendFail_image_left);
			
			msg_layout_right = ((LinearLayout) convertView.findViewById(R.id.msg_layout_right));
			name_layout_right = ((LinearLayout) convertView.findViewById(R.id.name_layout_right));
			messageText_right = ((TextView) convertView.findViewById(R.id.messageText_right));
			vipLabel_right = ((TextView) convertView.findViewById(R.id.vipLabel_right));
			allianceLabel_right = ((TextView) convertView.findViewById(R.id.allianceLabel_right));
			nameLabel_right = ((TextView) convertView.findViewById(R.id.nameLabel_right));
			headImage_right = ((ImageView) convertView.findViewById(R.id.headImage_right));
			privilegeImage_right = ((ImageView) convertView.findViewById(R.id.privilegeImage_right));
			headImageContainer_right = ((FrameLayout) convertView.findViewById(R.id.headImageContainer_right));
			send_progressbar_right = (ProgressBar) convertView.findViewById(R.id.send_progressbar_right);
			sendFail_image_right = (ImageView) convertView.findViewById(R.id.sendFail_image_right);
			
			msg_layout_left.setVisibility(View.GONE);
			msg_layout_right.setVisibility(View.GONE);
			
			convertView.setTag(this);
			messageText_center.setTag(this);
			messageText_left.setTag(this);
			messageText_right.setTag(this);
			setMsgTextViewPadding(messageText_center);
			setMsgTextViewPadding(messageText_left);
			setMsgTextViewPadding(messageText_right);
		}
		
		public void setCurrentHolderStyle(boolean isTip,boolean isSelfMsg)
		{
			this.isTipMsg=isTip;
			this.isSelfMsg=isSelfMsg;
			
			if(isTip)
			{
				msg_layout_center.setVisibility(View.VISIBLE);
				msg_layout_left.setVisibility(View.GONE);
				msg_layout_right.setVisibility(View.GONE);
			}
			else
			{
				msg_layout_center.setVisibility(View.GONE);
				if(!this.isSelfMsg)
				{
					msg_layout_left.setVisibility(View.VISIBLE);
					msg_layout_right.setVisibility(View.GONE);
				}
				else
				{
					msg_layout_left.setVisibility(View.GONE);
					msg_layout_right.setVisibility(View.VISIBLE);
				}
			}
		}

		public void stopSendTimer()
		{
			if (sendTimer != null){
				if (sendTimerTask != null){
					sendTimerTask.cancel();
				}
				sendTimer.cancel();
				sendTimer.purge();
			}
		}
		public void removeSendTimer()
		{
			stopSendTimer();
			sendTimer = null;
			sendTimerTask = null;
		}
		
		public void setPrivilegeImageLayoutParams(LayoutParams layoutParams)
		{
			privilegeImage_left.setLayoutParams(layoutParams);
			privilegeImage_right.setLayoutParams(layoutParams);
		}
		
		public void setHeadImageContainerLayoutParams(LayoutParams layoutParams)
		{
			headImageContainer_left.setLayoutParams(layoutParams);
			headImageContainer_right.setLayoutParams(layoutParams);
		}
		
		public void adjustTextSize()
		{
			ScaleUtil.adjustTextSize(newMsgLabel, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(sendDateLabel, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(messageText_center, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(messageText_left, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(vipLabel_left, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(allianceLabel_left, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(nameLabel_left, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(messageText_right, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(vipLabel_right, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(allianceLabel_right, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(nameLabel_right, ConfigManager.scaleRatio);
		}
		
		@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
		public void setMsgTextBackground(Context c,boolean isSystemMessage,boolean isHornMessage,int sdk)
		{
			if (isSystemMessage)
			{
				String leftBg=isHornMessage?"horn_bg_left":"chatsystem_left_bg";
				String rightBg=isHornMessage?"horn_bg_right":"chatsystem_right_bg";
				
				if (sdk < android.os.Build.VERSION_CODES.JELLY_BEAN)
				{
					messageText_left.setBackgroundDrawable(c.getResources().getDrawable(ResUtil.getId(c, "drawable", leftBg)));
					messageText_right.setBackgroundDrawable(c.getResources().getDrawable(ResUtil.getId(c, "drawable", rightBg)));
				}
				else
				{
					messageText_left.setBackground(c.getResources().getDrawable(ResUtil.getId(c, "drawable", leftBg)));
					messageText_right.setBackground(c.getResources().getDrawable(ResUtil.getId(c, "drawable", rightBg)));
				}
				
			}
			else
			{
				if (sdk < android.os.Build.VERSION_CODES.JELLY_BEAN)
				{
					messageText_left.setBackgroundDrawable(c.getResources().getDrawable(R.drawable.chatto_bg));
					messageText_right.setBackgroundDrawable(c.getResources().getDrawable(R.drawable.chatfrom_bg));
				}
				else
				{
					messageText_left.setBackground(c.getResources().getDrawable(R.drawable.chatto_bg));
					messageText_right.setBackground(c.getResources().getDrawable(R.drawable.chatfrom_bg));
				}
			}
		}

		public LinearLayout getNewMsgLayout() {
			return newMsgLayout;
		}
		public TextView getNewMsgLabel() {
			return newMsgLabel;
		}
		
		public LinearLayout getSendDateLayout() {
			return sendDateLayout;
		}
		public TextView getSendDateLabel() {
			return sendDateLabel;
		}
		public LinearLayout getMsg_layout() {
			if(this.isTipMsg)
				return msg_layout_center;
			else if(this.isSelfMsg)
				return msg_layout_right;
			return msg_layout_left;
		}
		
		public TextView getMessageText() {
			if(!this.isTipMsg)
			{	
				if(this.isSelfMsg)
					return messageText_right;
				else
					return messageText_left;
			}
			return messageText_center;
		}
		public TextView getVipLabel() {
			return this.isSelfMsg?vipLabel_right:vipLabel_left;
		}
		public LinearLayout getName_layout() {
			return this.isSelfMsg?name_layout_right:name_layout_left;
		}
		public TextView getAllianceLabel() {
			return this.isSelfMsg?allianceLabel_right:allianceLabel_left;
		}
		public TextView getNameLabel() {
			return this.isSelfMsg?nameLabel_right:nameLabel_left;
		}
		public ImageView getHeadImage() {
			return this.isSelfMsg?headImage_right:headImage_left;
		}
		public ImageView getPrivilegeImage() {
			return this.isSelfMsg?privilegeImage_right:privilegeImage_left;
		}
		public FrameLayout getHeadImageContainer() {
			return this.isSelfMsg?headImageContainer_right:headImageContainer_left;
		}
		public ProgressBar getSend_progressbar() {
			return this.isSelfMsg?send_progressbar_right:send_progressbar_left;
		}
		public ImageView getSendFail_image() {
			return this.isSelfMsg?sendFail_image_right:sendFail_image_left;
		}
	}
	
	public void destroy()
	{
//		items.clear();
//		notifyDataSetChanged();
		c = null;
		inflater = null;
		items = null;

		if (quickAction != null)
		{
			quickAction.currentTextView = null;
			quickAction.setOnActionItemClickListener(null);
		}
		actionClickListener = null;
	}
}