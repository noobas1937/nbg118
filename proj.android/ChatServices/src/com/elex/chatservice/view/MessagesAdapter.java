package com.elex.chatservice.view;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import net.londatiga.android.QuickAction;
import org.apache.commons.lang.StringUtils;
import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
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
import android.widget.BaseAdapter;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.R;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.JniController;
import com.elex.chatservice.controller.MenuController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChannelView;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.StickManager;
import com.elex.chatservice.model.TranslateManager;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.net.WebSocketManager;
import com.elex.chatservice.model.viewholder.MessageViewHolder;
import com.elex.chatservice.model.viewholder.ViewHolderHelper;
import com.elex.chatservice.util.AllianceTaskInfo;
import com.elex.chatservice.util.CompatibleApiUtil;
import com.elex.chatservice.util.ImageUtil;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.util.ScaleUtil;
import com.elex.chatservice.util.TranslateListener;
import com.elex.chatservice.util.gif.GifMovieView;

public final class MessagesAdapter extends BaseAdapter
{
	private Context				c;
	private List<MsgItem>		items;
	private LayoutInflater		inflater;
	private QuickAction			quickAction;

	private static final int	ITEM_MESSAGE_SEND		= 0;
	private static final int	ITEM_MESSAGE_RECEIVE	= 1;
	private static final int	ITEM_GIF_SEND			= 2;
	private static final int	ITEM_GIF_RECEIVE		= 3;
	private static final int	ITEM_PIC_SEND			= 4;
	private static final int	ITEM_PIC_RECEIVE		= 5;
	private static final int	ITEM_REDPACKAGE_SEND	= 6;
	private static final int	ITEM_REDPACKAGE_RECEIVE	= 7;
	private static final int	ITEM_CHATROOM_TIP		= 8;
	private static final int	ITEM_NEWMESSAGE_TIP		= 9;
	private static final int	ITEM_TYPE_TOTAL_COUNT	= 10;

	public MessagesAdapter(Fragment f, int textViewResourceId, List<MsgItem> objects)
	{
		this.c = f.getActivity();
		this.items = objects;
		this.inflater = ((LayoutInflater) this.c.getSystemService("layout_inflater"));
	}

	private QuickAction.OnActionItemClickListener	actionClickListener	= new QuickAction.OnActionItemClickListener()
																		{

																			@Override
																			public void onItemClick(QuickAction source, int pos,
																					int actionId)
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
		// return ((MessageHolder)
		// quickAction.currentTextView.getTag()).msgItem;
		if (quickAction.currentTextView != null && quickAction.currentTextView.getTag() != null)
			return ((MessageViewHolder) quickAction.currentTextView.getTag()).currentMsgItem;
		return null;
	}

	public void showTranslatedLanguage(final TextView textView, final MsgItem item)
	{
		if (TranslateManager.getInstance().isTranslateMsgValid(item))
		{
			item.hasTranslated = true;
			item.isTranslatedByForce = true;
			item.hasTranslatedByForce = true;
			item.isOriginalLangByForce = false;
			setText(textView, item.translateMsg, item, true);
		}
		else
		{
			TranslateManager.getInstance().loadTranslation(item, new TranslateListener()
			{
				@Override
				public void onTranslateFinish(final String translateMsg)
				{
					if (!item.canShowTranslateMsg() || StringUtils.isEmpty(translateMsg) || translateMsg.startsWith("{\"code\":{"))
						return;
					item.isOriginalLangByForce = false;
					setTextOnUIThread(textView, translateMsg, item);
				}
			});
		}
	}

	private void setTextOnUIThread(final TextView textView, final String translateMsg, final MsgItem item)
	{
		if (c != null && c instanceof Activity)
		{
			((Activity) c).runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						setText(textView, translateMsg, item, true);
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
		}
	}

	public void showOriginalLanguage(TextView textView, MsgItem item)
	{
		item.hasTranslated = false;
		item.isTranslatedByForce = false;
		item.isOriginalLangByForce = true;
		setText(textView, item.msg, item, false);
	}

	// TODO 返回值是否正确？
	public boolean isEnabled(int position)
	{
		return false;
	}

	@Override
	public int getViewTypeCount()
	{
		return ITEM_TYPE_TOTAL_COUNT;
	}

	@Override
	public int getItemViewType(int position)
	{
		if (position >= 0 && position < items.size())
		{
			MsgItem item = items.get(position);
			if (item != null)
			{
				int type = item.getMsgItemType(c);
				boolean isSelfMsg = item.isSelfMsg();
				if (type == MsgItem.MSGITEM_TYPE_MESSAGE)
				{
					return isSelfMsg ? ITEM_MESSAGE_SEND : ITEM_MESSAGE_RECEIVE;
				}
				else if (type == MsgItem.MSGITEM_TYPE_GIF)
				{
					return isSelfMsg ? ITEM_GIF_SEND : ITEM_GIF_RECEIVE;
				}
				else if (type == MsgItem.MSGITEM_TYPE_PIC)
				{
					return isSelfMsg ? ITEM_PIC_SEND : ITEM_PIC_RECEIVE;
				}
				else if (type == MsgItem.MSGITEM_TYPE_REDPACKAGE)
				{
					return isSelfMsg ? ITEM_REDPACKAGE_SEND : ITEM_REDPACKAGE_RECEIVE;
				}
				else if (type == MsgItem.MSGITEM_TYPE_CHATROM_TIP)
				{
					return ITEM_CHATROOM_TIP;
				}
				else if (type == MsgItem.MSGITEM_TYPE_NEW_MESSAGE_TIP)
				{
					return ITEM_NEWMESSAGE_TIP;
				}
			}
		}
		return -1;
	}

	private boolean needShowTime(int position)
	{
		int itemType = getItemViewType(position);
		if (itemType == ITEM_NEWMESSAGE_TIP)
			return false;
		return true;
	}

	public View getView(int position, View convertView, ViewGroup parent)
	{
		if (position < 0 || position >= items.size())
			return null;

		final MsgItem item = (MsgItem) this.items.get(position);
		if (item == null)
		{
			System.out.println("item == null");
			return null;
		}
		int type = item.getMsgItemType(c);

		if (convertView == null)
		{
			System.out.println("convertView == null");
			convertView = createViewByMessage(item, position, type);
			adjustSize(convertView, type);
		}

		MessageViewHolder holder = ViewHolderHelper.getViewHolder(convertView);
		if (holder != null)
			holder.currentMsgItem = item;

//		System.out.println("getView position:" + position + "  msg:" + item.msg + "   items.size():" + items.size() + "  item.uid:"+item.uid);

		if (type == MsgItem.MSGITEM_TYPE_NEW_MESSAGE_TIP)
		{
			ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
			if (channel != null && WebSocketManager.isRecieveFromWebSocket(channel.channelType) && channel.wsNewMsgCount > 0)
			{
				channel.wsNewMsgCount = 0;
				if (getChatFragment() != null)
				{
					getChatFragment().refreshToolTip();
				}
			}
			setNewMsgTipData(convertView, item);
		}
		else
		{
			if (needShowTime(position))
				setSendTimeData(convertView, item);

			if (type == MsgItem.MSGITEM_TYPE_CHATROM_TIP)
			{
				setChatRoomTipData(convertView, item);
			}
			else
			{
				adjustHeadImageContainerSize(convertView, item);
				setPlayerData(convertView, item);
				addHeadImageOnClickAndTouchListener(convertView, item);
				addSendStatusTimerAndRefresh(convertView, item);
				addOnClickSendStateListener(convertView, item);

				int sdk = android.os.Build.VERSION.SDK_INT;
				setHeadImageBackground(convertView, item, sdk);

				if (type == MsgItem.MSGITEM_TYPE_MESSAGE)
				{
					setMessageTextBackground(convertView, item, sdk);
					setMessageData(convertView, item);
				}
				else if (type == MsgItem.MSGITEM_TYPE_REDPACKAGE)
				{
					setRedPackageBackground(convertView, item, sdk);
					setRedPackageData(convertView, item);
				}
				else if (type == MsgItem.MSGITEM_TYPE_GIF)
				{
					setGifData(convertView, item);
				}
				else if (type == MsgItem.MSGITEM_TYPE_PIC)
				{
					setPicData(convertView, item);
				}
			}
		}
		return convertView;
	}

	private void addItemOnClickListener(final View convertView)
	{
		MessageViewHolder holder = ViewHolderHelper.getViewHolder(convertView);
		if (holder == null)
			return;
		final MsgItem item = holder.currentMsgItem;
		if (item == null)
			return;
		OnClickListener onClickListener = new View.OnClickListener()
		{
			public void onClick(View view)
			{
				if (c == null || ((ChatActivity) c).fragment == null || ((ChatFragment) ((ChatActivity) c).fragment).isKeyBoradShowing)
				{
					return;
				}
				if (ignoreClick)
				{
					ignoreClick = false;
					return;
				}

				if (item.isRedPackageMessage())
				{
					if (item.sendState == MsgItem.HANDLED || item.isSelfMsg())
						ChatServiceController.doHostAction("viewRedPackage", "", "", item.attachmentId, true);
					else
					{
						if (ChatServiceController.getChatFragment() != null)
							ChatServiceController.getChatFragment().showRedPackageConfirm(item);

					}
				}
				else
				{
					if (quickAction != null)
					{
						quickAction.setOnActionItemClickListener(null);
					}

					System.out.println("click item msg:" + item.msg);
					quickAction = ChatQuickActionFactory.createQuickAction((Activity) c, item);
					quickAction.setOnActionItemClickListener(actionClickListener);

					quickAction.currentTextView = (TextView) view;
					quickAction.show(view);
				}
			}
		};

		if (item.isRedPackageMessage())
		{
			LinearLayout red_package_top_layout = ViewHolderHelper.get(convertView, R.id.red_package_top_layout);
			if (red_package_top_layout != null)
				red_package_top_layout.setOnClickListener(onClickListener);

			LinearLayout red_package_bottom_layout = ViewHolderHelper.get(convertView, R.id.red_package_bottom_layout);
			if (red_package_bottom_layout != null)
				red_package_bottom_layout.setOnClickListener(onClickListener);
		}
		else
		{
			TextView messageText = ViewHolderHelper.get(convertView, R.id.messageText);
			if (messageText != null)
			{
				messageText.setTag(holder);
				messageText.setOnClickListener(onClickListener);
			}
		}
	}

	private void adjustSize(View convertView, int type)
	{
		if (convertView != null && ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
		{
			adjustTextSize(convertView);

			if (type != MsgItem.MSGITEM_TYPE_CHATROM_TIP && type != MsgItem.MSGITEM_TYPE_NEW_MESSAGE_TIP)
			{
				int length = (int) (ScaleUtil.dip2px(c, 50) * ConfigManager.scaleRatio * getScreenCorrectionFactor());
				ImageView headImage = ViewHolderHelper.get(convertView, R.id.headImage);
				if (headImage != null)
				{
					FrameLayout.LayoutParams headImageLayoutParams = (FrameLayout.LayoutParams) headImage.getLayoutParams();
					headImageLayoutParams.width = length;
					headImageLayoutParams.height = length;
					headImage.setLayoutParams(headImageLayoutParams);
				}
			}

			if (type == MsgItem.MSGITEM_TYPE_REDPACKAGE)
			{
				int headImageContainerWidth = (int) (ScaleUtil.dip2px(c, 60) * ConfigManager.scaleRatio * getScreenCorrectionFactor());

				LinearLayout redpackage_layout = ViewHolderHelper.get(convertView, R.id.redpackage_layout);
				if (redpackage_layout != null)
				{
					int redPackageWidth = (int) (ScaleUtil.getScreenWidth() * 0.8f) - headImageContainerWidth;
					LinearLayout.LayoutParams linearParams = (LinearLayout.LayoutParams) redpackage_layout.getLayoutParams();
					int targetWidht = redPackageWidth * 3 / 4;
					if (redPackageWidth > 600)
						targetWidht = 600;
					linearParams.width = targetWidht;
					redpackage_layout.setLayoutParams(linearParams);
				}

				ImageView red_package_image = ViewHolderHelper.get(convertView, R.id.red_package_image);
				if (red_package_image != null)
				{
					int originalHeight = 35;
					int targetRedPackageHeight = (int) (ScaleUtil.dip2px(c, originalHeight) * ConfigManager.scaleRatio * getScreenCorrectionFactor());
					LinearLayout.LayoutParams redPackageLayout = (LinearLayout.LayoutParams) red_package_image.getLayoutParams();
					redPackageLayout.width = targetRedPackageHeight;
					redPackageLayout.height = targetRedPackageHeight;
					red_package_image.setLayoutParams(redPackageLayout);
				}
			}
		}
	}

	private void adjustHeadImageContainerSize(View convertView, MsgItem item)
	{
		if (convertView != null && ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
		{
			FrameLayout headImageContainer = ViewHolderHelper.get(convertView, R.id.headImageContainer);
			if (headImageContainer != null)
			{
				int width = (int) (ScaleUtil.dip2px(c, 60) * ConfigManager.scaleRatio * getScreenCorrectionFactor());
				int height = (int) (ScaleUtil.dip2px(c, 60) * ConfigManager.scaleRatio * getScreenCorrectionFactor());
				LinearLayout.LayoutParams headImageContainerLayoutParams = (LinearLayout.LayoutParams) headImageContainer.getLayoutParams();
				headImageContainerLayoutParams.width = width;
				headImageContainerLayoutParams.height = height;
				headImageContainer.setLayoutParams(headImageContainerLayoutParams);
			}
			
			ImageView headImageKingIcon = ViewHolderHelper.get(convertView, R.id.headImageKingIcon);
			if(headImageKingIcon!=null)
			{  
				int width = (int) (ScaleUtil.dip2px(c, 40) * ConfigManager.scaleRatio * getScreenCorrectionFactor());
				int height = (int) (ScaleUtil.dip2px(c, 22) * ConfigManager.scaleRatio * getScreenCorrectionFactor());
				LinearLayout.LayoutParams headImageKingIconLayoutParams = (LinearLayout.LayoutParams) headImageKingIcon.getLayoutParams();
				headImageKingIconLayoutParams.width = width;
				headImageKingIconLayoutParams.height = height;
				headImageKingIcon.setLayoutParams(headImageKingIconLayoutParams);
			}
		}
	}

	private void adjustTextSize(View convertView)
	{
		TextView newMsgLabel = ViewHolderHelper.get(convertView, R.id.newMsgLabel);
		if (newMsgLabel != null)
			ScaleUtil.adjustTextSize(newMsgLabel, ConfigManager.scaleRatio);

		TextView sendDateLabel = ViewHolderHelper.get(convertView, R.id.sendDateLabel);
		if (sendDateLabel != null)
			ScaleUtil.adjustTextSize(sendDateLabel, ConfigManager.scaleRatio);

		TextView messageText_center = ViewHolderHelper.get(convertView, R.id.messageText_center);
		if (messageText_center != null)
			ScaleUtil.adjustTextSize(messageText_center, ConfigManager.scaleRatio);

		TextView messageText = ViewHolderHelper.get(convertView, R.id.messageText);
		if (messageText != null)
			ScaleUtil.adjustTextSize(messageText, ConfigManager.scaleRatio);

		TextView vipLabel = ViewHolderHelper.get(convertView, R.id.vipLabel);
		if (vipLabel != null)
			ScaleUtil.adjustTextSize(vipLabel, ConfigManager.scaleRatio);

		TextView allianceLabel = ViewHolderHelper.get(convertView, R.id.allianceLabel);
		if (allianceLabel != null)
			ScaleUtil.adjustTextSize(allianceLabel, ConfigManager.scaleRatio);

		TextView nameLabel = ViewHolderHelper.get(convertView, R.id.nameLabel);
		if (nameLabel != null)
			ScaleUtil.adjustTextSize(nameLabel, ConfigManager.scaleRatio);

		TextView red_package_title = ViewHolderHelper.get(convertView, R.id.red_package_title);
		if (red_package_title != null)
			ScaleUtil.adjustTextSize(red_package_title, ConfigManager.scaleRatio);

		TextView red_package_content = ViewHolderHelper.get(convertView, R.id.red_package_content);
		if (red_package_content != null)
			ScaleUtil.adjustTextSize(red_package_content, ConfigManager.scaleRatio);

		TextView redpackage_time = ViewHolderHelper.get(convertView, R.id.redpackage_time);
		if (redpackage_time != null)
			ScaleUtil.adjustTextSize(redpackage_time, ConfigManager.scaleRatio);
	}

	public void refreshSendState(View convertView, final MsgItem msgItem)
	{
		final ProgressBar send_progressbar = ViewHolderHelper.get(convertView, R.id.send_progressbar);
		final ImageView sendFail_image = ViewHolderHelper.get(convertView, R.id.sendFail_image);
		if (send_progressbar == null || sendFail_image == null)
			return;

		if (msgItem.sendState == MsgItem.SENDING)
		{
			if (send_progressbar.getVisibility() != View.VISIBLE)
				send_progressbar.setVisibility(View.VISIBLE);
			if (sendFail_image.getVisibility() != View.GONE)
				sendFail_image.setVisibility(View.GONE);
		}
		else if (msgItem.sendState == MsgItem.SEND_FAILED)
		{
			if (send_progressbar.getVisibility() != View.GONE)
				send_progressbar.setVisibility(View.GONE);
			if (send_progressbar.getVisibility() != View.VISIBLE)
				sendFail_image.setVisibility(View.VISIBLE);
		}
		else if (msgItem.sendState == MsgItem.SEND_SUCCESS)
		{
			if (send_progressbar.getVisibility() != View.GONE)
				send_progressbar.setVisibility(View.GONE);
			if (sendFail_image.getVisibility() != View.GONE)
				sendFail_image.setVisibility(View.GONE);
		}
	}

	private void addSendStatusTimerAndRefresh(final View convertView, final MsgItem item)
	{
		if (!item.isSelfMsg())
			return;
		if (!item.isRedPackageMessage())
			refreshSendState(convertView, item);
		else
		{
			final ProgressBar send_progressbar = ViewHolderHelper.get(convertView, R.id.send_progressbar);
			final ImageView sendFail_image = ViewHolderHelper.get(convertView, R.id.sendFail_image);
			if (send_progressbar != null && send_progressbar.getVisibility() != View.GONE)
				send_progressbar.setVisibility(View.GONE);
			if (sendFail_image != null && sendFail_image.getVisibility() != View.GONE)
				sendFail_image.setVisibility(View.GONE);
		}

		final MessageViewHolder holder = ViewHolderHelper.getViewHolder(convertView);
		if (holder == null)
			return;
		if ((!item.isSystemMessage() || item.isHornMessage()))
		{
			if (item.sendState == MsgItem.SENDING)
			{
				if (holder.sendTimer == null)
				{
					holder.sendTimer = new Timer();
				}
				if (holder.sendTimerTask != null)
					return;
				holder.sendTimerTask = new TimerTask()
				{
					@Override
					public void run()
					{
						if (item.sendState == MsgItem.SENDING)
							item.sendState = MsgItem.SEND_FAILED;

						if (c == null)
							return;

						((Activity) c).runOnUiThread(new Runnable()
						{
							@Override
							public void run()
							{
								refreshSendState(convertView, item);
								holder.removeSendTimer();
							}
						});
					}
				};

				holder.sendTimer.schedule(holder.sendTimerTask, 10000);
			}
			else
			{
				if (holder.sendTimer == null)
					return;
				holder.removeSendTimer();
			}
		}
	}

	private void setGifData(View convertView, MsgItem item)
	{
		GifMovieView gifMovieView = ViewHolderHelper.get(convertView, R.id.gifMovieView);
		if (gifMovieView == null)
			return;
		String replacedEmoj = StickManager.getPredefinedEmoj(item.msg);
		if (replacedEmoj != null)
		{
			int resId = ResUtil.getId(c, "drawable", replacedEmoj);
			if (resId != 0 && c.getString(resId).endsWith(".gif"))
				gifMovieView.setMovieResource(resId);
		}
	}

	private void setPicData(View convertView, MsgItem item)
	{
		ImageView picImageView = ViewHolderHelper.get(convertView, R.id.picImageView);
		if (picImageView == null)
			return;
		String replacedEmoj = StickManager.getPredefinedEmoj(item.msg);
		if (replacedEmoj != null)
		{
			int resId = ResUtil.getId(c, "drawable", replacedEmoj);
			if (resId != 0)
				picImageView.setImageResource(resId);
		}
	}

	private void setMessageData(final View convertView, final MsgItem item)
	{
		final TextView messageText = ViewHolderHelper.get(convertView, R.id.messageText);
		if (messageText == null)
			return;
		if (item.isSelfMsg())
		{
			setText(messageText, item.msg, item, false);
		}
		else
		{
			if (item.canShowTranslateMsg())
			{
				if (!item.isTipMsg())
					setText(messageText, item.translateMsg, item, true);
				else
					messageText.setText(item.translateMsg);
				TranslateManager.getInstance().enterTranlateQueue(item);
			}
			else
			{
				if (!item.isTipMsg())
					setText(messageText, item.msg, item, false);
				else
					messageText.setText(item.msg);
				if (ConfigManager.autoTranlateMode > 0)
				{
					TranslateManager.getInstance().loadTranslation(item, new TranslateListener()
					{
						@Override
						public void onTranslateFinish(final String translateMsg)
						{
							MessageViewHolder holder = ViewHolderHelper.getViewHolder(convertView);
							if (holder != null)
							{
								MsgItem msgItem = holder.currentMsgItem;
								if ((msgItem != null && !msgItem.equals(item)) || !item.canShowTranslateMsg()
										|| StringUtils.isEmpty(translateMsg) || translateMsg.startsWith("{\"code\":{"))
									return;
								setTextOnUIThread(messageText, translateMsg, item);
							}
						}
					});
				}
			}
		}

		addItemOnClickListener(convertView);
		// addOnClickItemListener(convertView, item);
	}

	private void setRedPackageData(View convertView, MsgItem item)
	{
		item.handleRedPackageFinishState();

		TextView red_package_title = ViewHolderHelper.get(convertView, R.id.red_package_title);
		if (red_package_title != null)
		{
			if (StringUtils.isNotEmpty(item.translateMsg))
				red_package_title.setText(item.translateMsg);
			else
				red_package_title.setText(item.msg);
		}

		TextView redpackage_time = ViewHolderHelper.get(convertView, R.id.redpackage_time);
		if (redpackage_time != null)
		{
			redpackage_time.setText(item.getSendTimeHM());

		}

		TextView red_package_content = ViewHolderHelper.get(convertView, R.id.red_package_content);
		if (red_package_content != null)
		{
			if (item.isSelfMsg())
				red_package_content.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_RED_PACKAGE_CONTENT_2));
			else
				red_package_content.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_RED_PACKAGE_CONTENT));
		}
		addItemOnClickListener(convertView);
		// addOnClickItemListener(convertView, item);
	}

	private void setPlayerData(View convertView, MsgItem item)
	{
		TextView vipLabel = ViewHolderHelper.get(convertView, R.id.vipLabel);
		if (vipLabel != null)
			vipLabel.setText(item.getVipLabel());

		TextView allianceLabel = ViewHolderHelper.get(convertView, R.id.allianceLabel);
		if (allianceLabel != null)
		{
			if (ChatServiceController.getCurrentChannelType() != DBDefinition.CHANNEL_TYPE_ALLIANCE)
				allianceLabel.setText(item.getAllianceLabel());
			else
				allianceLabel.setText("");
		}

		TextView nameLabel = ViewHolderHelper.get(convertView, R.id.nameLabel);
		if (nameLabel != null)
		{
			nameLabel.setText(item.getName() + (item.getSrcServerId() > 0 ? "#" + item.getSrcServerId() : ""));
		}

		ImageView headImage = ViewHolderHelper.get(convertView, R.id.headImage);
		if (headImage != null)
		{
			headImage.setTag(item.uid);
			ImageUtil.setHeadImage(c, item.getHeadPic(), headImage, item.getUser());
		}
		
		
		ImageView headImageKingIcon = ViewHolderHelper.get(convertView, R.id.headImageKingIcon);
		if(headImageKingIcon!=null)
		{
			if(item.isKingMsg() && headImageKingIcon.getVisibility()!=View.VISIBLE)
				headImageKingIcon.setVisibility(View.VISIBLE);
			else if(!item.isKingMsg() && headImageKingIcon.getVisibility()!=View.GONE)
				headImageKingIcon.setVisibility(View.GONE);
		}

		ImageView privilegeImage = ViewHolderHelper.get(convertView, R.id.privilegeImage);
		if (privilegeImage != null)
		{
			int idPrivilegeImage = getGmodResourceId(item.getGmod());
			if (idPrivilegeImage != 0)
			{
				privilegeImage.setImageResource(idPrivilegeImage);
			}
			else
			{
				privilegeImage.setImageDrawable(null);
			}
		}

	}

	@SuppressLint("ClickableViewAccessibility")
	private void addHeadImageOnClickAndTouchListener(View convertView, final MsgItem item)
	{
		FrameLayout headImageContainer = ViewHolderHelper.get(convertView, R.id.headImageContainer);
		if (headImageContainer == null)
			return;
		headImageContainer.setOnTouchListener(new View.OnTouchListener()
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

		headImageContainer.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(final View view)
			{
				if (!item.isSystemHornMsg())
				{
					if (ChatServiceController.isContactMod)
						ChatServiceController.doHostAction("showPlayerInfo@mod", item.uid, item.getName(), "", true);
					else
						ChatServiceController.doHostAction("showPlayerInfo", item.uid, item.getName(), "", true);
				}
			}
		});
	}

	private void addOnClickItemListener(View convertView, final MsgItem item)
	{
		OnClickListener onClickListener = new View.OnClickListener()
		{
			public void onClick(View view)
			{
				if (c == null || ((ChatActivity) c).fragment == null || ((ChatFragment) ((ChatActivity) c).fragment).isKeyBoradShowing)
				{
					return;
				}
				if (ignoreClick)
				{
					ignoreClick = false;
					return;
				}

				if (item.isRedPackageMessage())
				{
					if (item.sendState == MsgItem.HANDLED || item.isSelfMsg())
						ChatServiceController.doHostAction("viewRedPackage", "", "", item.attachmentId, true);
					else
					{
						if (ChatServiceController.getChatFragment() != null)
							ChatServiceController.getChatFragment().showRedPackageConfirm(item);

					}
				}
				else
				{
					if (quickAction != null)
					{
						quickAction.setOnActionItemClickListener(null);
					}

					quickAction = ChatQuickActionFactory.createQuickAction((Activity) c, item);
					quickAction.setOnActionItemClickListener(actionClickListener);

					quickAction.currentTextView = (TextView) view;
					quickAction.show(view);
				}
			}
		};

		if (item.isRedPackageMessage())
		{
			LinearLayout red_package_top_layout = ViewHolderHelper.get(convertView, R.id.red_package_top_layout);
			if (red_package_top_layout != null)
				red_package_top_layout.setOnClickListener(onClickListener);
			LinearLayout red_package_bottom_layout = ViewHolderHelper.get(convertView, R.id.red_package_bottom_layout);
			if (red_package_bottom_layout != null)
				red_package_bottom_layout.setOnClickListener(onClickListener);
		}
		else
		{
			TextView messageText = ViewHolderHelper.get(convertView, R.id.messageText);
			if (messageText != null)
			{
				messageText.setOnClickListener(onClickListener);
			}
		}
	}

	@SuppressWarnings("deprecation")
	@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
	private void setHeadImageBackground(View convertView, MsgItem msgItem, int sdk)
	{
		String headImageBg = "icon_kuang";
//		if (msgItem.isKingMsg())
//		{
//			headImageBg = "king_head_container";
//		}

		FrameLayout headImageContainer = ViewHolderHelper.get(convertView, R.id.headImageContainer);
		if (headImageContainer == null)
			return;
		if (sdk < android.os.Build.VERSION_CODES.JELLY_BEAN)
		{
			headImageContainer.setBackgroundDrawable(c.getResources().getDrawable(ResUtil.getId(c, "drawable", headImageBg)));
		}
		else
		{
			headImageContainer.setBackground(c.getResources().getDrawable(ResUtil.getId(c, "drawable", headImageBg)));
		}

	}

	@SuppressWarnings("deprecation")
	@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
	private void setRedPackageBackground(View convertView, MsgItem msgItem, int sdk)
	{
		LinearLayout red_package_top_layout = ViewHolderHelper.get(convertView, R.id.red_package_top_layout);
		LinearLayout red_package_bottom_layout = ViewHolderHelper.get(convertView, R.id.red_package_bottom_layout);
		String topbackground = "redpackage_left_bg";
		String bottombackground = "redpackage_left_time_bg";
		if (msgItem.isSelfMsg())
		{
			topbackground = "redpackage_right_bg";
			bottombackground = "redpackage_right_time_bg";
		}
		else
		{
			topbackground = "redpackage_left_bg";
			bottombackground = "redpackage_left_time_bg";
		}

		if (sdk < android.os.Build.VERSION_CODES.JELLY_BEAN)
		{
			red_package_top_layout.setBackgroundDrawable(c.getResources().getDrawable(ResUtil.getId(c, "drawable", topbackground)));
			red_package_bottom_layout.setBackgroundDrawable(c.getResources().getDrawable(ResUtil.getId(c, "drawable", bottombackground)));
		}
		else
		{
			red_package_top_layout.setBackground(c.getResources().getDrawable(ResUtil.getId(c, "drawable", topbackground)));
			red_package_bottom_layout.setBackground(c.getResources().getDrawable(ResUtil.getId(c, "drawable", bottombackground)));
		}
	}

	@SuppressWarnings("deprecation")
	@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
	private void setMessageTextBackground(View convertView, MsgItem msgItem, int sdk)
	{
		TextView messageText = ViewHolderHelper.get(convertView, R.id.messageText);
		if (messageText == null)
			return;
		String background = "chatfrom_bg";
		if (msgItem.isSystemMessage())
		{
			if (msgItem.isSelfMsg())
			{
				if (msgItem.isHornMessage())
					background = "horn_bg_right";
				else
					background = "chatsystem_right_bg";
			}
			else
			{
				if (msgItem.isHornMessage())
					background = "horn_bg_left";
				else
					background = "chatsystem_left_bg";
			}
		}
		else
		{
			if (msgItem.isSelfMsg())
			{
				if (msgItem.isKingMsg())
					background = "king_msg_right_bg";
				else
					background = "chatfrom_bg";
			}
			else
			{
				if (msgItem.isKingMsg())
					background = "king_msg_left_bg";
				else
					background = "chatto_bg";
			}
		}

		if (sdk < android.os.Build.VERSION_CODES.JELLY_BEAN)
		{
			messageText.setBackgroundDrawable(c.getResources().getDrawable(ResUtil.getId(c, "drawable", background)));
		}
		else
		{
			messageText.setBackground(c.getResources().getDrawable(ResUtil.getId(c, "drawable", background)));
		}
	}

	private void setSendTimeData(View convertView, MsgItem item)
	{
		TextView sendDateLabel = ViewHolderHelper.get(convertView, R.id.sendDateLabel);
		if (sendDateLabel == null)
			return;
		if (ChatServiceController.getInstance().isDifferentDate(item,items))
		{
			if (sendDateLabel.getVisibility() == View.GONE)
				sendDateLabel.setVisibility(View.VISIBLE);
			sendDateLabel.setText(item.getSendTimeToShow());
			if(ChatServiceController.sendTimeTextHeight==0)
				ChatServiceController.sendTimeTextHeight = sendDateLabel.getHeight();
		}
		else
		{
			if (sendDateLabel.getVisibility() == View.VISIBLE)
				sendDateLabel.setVisibility(View.GONE);
		}
	}

	private void setNewMsgTipData(View convertView, MsgItem item)
	{
		TextView newMsgLabel = ViewHolderHelper.get(convertView, R.id.newMsgLabel);
		if (newMsgLabel == null)
			return;
		if (item.firstNewMsgState == 1)
		{
			newMsgLabel.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_NEW_MESSAGE_BELOW));
		}
		else
		{
			newMsgLabel.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_N_NEW_MESSAGE_BELOW, ChannelManager.LOAD_ALL_MORE_MAX_COUNT
					+ ""));
		}
	}

	private void setChatRoomTipData(View convertView, MsgItem item)
	{
		TextView messageText_center = ViewHolderHelper.get(convertView, R.id.messageText_center);
		if (messageText_center == null)
			return;
		if (item.canShowTranslateMsg())
		{
			messageText_center.setText(item.translateMsg);
			TranslateManager.getInstance().enterTranlateQueue(item);
		}
		else
		{
			messageText_center.setText(item.msg);
			TranslateManager.getInstance().enterTranlateQueue(item);
		}
	}

	private void addOnClickSendStateListener(View convertView, final MsgItem item)
	{
		if (!item.isSelfMsg())
			return;
		ImageView sendFail_image = ViewHolderHelper.get(convertView, R.id.sendFail_image);
		if (sendFail_image == null)
			return;
		sendFail_image.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				if (item.sendState != MsgItem.SEND_SUCCESS)
					MenuController.showReSendConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_RESEND), item);
			}
		});
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
			case 11:
				idPrivilegeImage = R.drawable.vip_certification;
				break;
			default:
				break;
		}
		return idPrivilegeImage;
	}

	@SuppressLint("InflateParams")
	private View createViewByMessage(MsgItem msgItem, int position, int type)
	{
		int itemType = getItemViewType(position);
		System.out.println("createViewByMessage position:" + position + "  itemType:" + itemType);
		if (type == MsgItem.MSGITEM_TYPE_MESSAGE)
		{
			if (itemType == ITEM_MESSAGE_SEND)
				return inflater.inflate(R.layout.msgitem_message_send, null);
			else if (itemType == ITEM_MESSAGE_RECEIVE)
				return inflater.inflate(R.layout.msgitem_message_receive, null);
		}
		else if (type == MsgItem.MSGITEM_TYPE_GIF)
		{
			if (itemType == ITEM_GIF_SEND)
				return inflater.inflate(R.layout.msgitem_gif_send, null);
			else if (itemType == ITEM_GIF_RECEIVE)
				return inflater.inflate(R.layout.msgitem_gif_receive, null);
		}
		else if (type == MsgItem.MSGITEM_TYPE_PIC)
		{
			if (itemType == ITEM_PIC_SEND)
				return inflater.inflate(R.layout.msgitem_pic_send, null);
			else if (itemType == ITEM_PIC_RECEIVE)
				return inflater.inflate(R.layout.msgitem_pic_receive, null);
		}
		else if (type == MsgItem.MSGITEM_TYPE_REDPACKAGE)
		{
			if (itemType == ITEM_REDPACKAGE_SEND)
				return inflater.inflate(R.layout.msgitem_redpackage_send, null);
			else if (itemType == ITEM_REDPACKAGE_RECEIVE)
				return inflater.inflate(R.layout.msgitem_redpackage_receive, null);
		}
		else if (type == MsgItem.MSGITEM_TYPE_CHATROM_TIP)
		{
			if (itemType == ITEM_CHATROOM_TIP)
				return inflater.inflate(R.layout.msgitem_chatroom_tip, null);
		}
		else if (type == MsgItem.MSGITEM_TYPE_NEW_MESSAGE_TIP)
		{
			return itemType == ITEM_NEWMESSAGE_TIP ? inflater.inflate(R.layout.msgitem_newmsg_tip, null) : null;
		}
		return null;
	}

	private ChatFragment getChatFragment()
	{
		if (c != null && c instanceof ChatActivity && ((ChatActivity) c).fragment != null
				&& ((ChatActivity) c).fragment instanceof ChatFragment)
		{
			return ((ChatFragment) ((ChatActivity) c).fragment);
		}
		else
		{
			return null;
		}
	}

	private static final String	JOIN_NOW_URL	= "JoinNow";
	private static final String	SHOW_EQUIP_URL	= "ShowEquip";

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
				color = 0xffc7beb3;
				break;
			case 1:
				color = 0xff56e578;
				break;
			case 2:
				color = 0xff4599f8;
				break;
			case 3:
				color = 0xffaf49ea;
				break;
			case 4:
				color = 0xffe8771f;
				break;
			case 5:
				color = 0xffedd538;
				break;
			case 6:
				color = 0xffff0000;
				break;
			default:
				color = 0xffc7beb3;
				break;
		}
		return color;
	}

	private void setText(TextView textView, String str, MsgItem item, boolean isTranslated)
	{
		String equipName = "";
		String taskName = "";
		int colorIndex = -1;
		if (item.isEquipMessage())
		{
			String msgStr = item.msg;
			if (StringUtils.isNotEmpty(msgStr))
			{
				String[] equipInfo = msgStr.split("\\|");
				if (equipInfo.length == 2)
				{
					equipName = LanguageManager.getLangByKey(equipInfo[1]);
					if (StringUtils.isNumeric(equipInfo[0]))
						colorIndex = Integer.parseInt(equipInfo[0]);
				}
			}
			str = LanguageManager.getLangByKey(LanguageKeys.TIP_EQUIP_SHARE, equipName);
		}
		else if (item.isAllianceTaskMessage())
		{
			String msgStr = item.msg;
			if (StringUtils.isNotEmpty(msgStr))
			{
				String[] taskInfo = msgStr.split("\\|");
				if (taskInfo.length >= 4)
				{
					taskName = LanguageManager.getLangByKey(taskInfo[2]);
					if (StringUtils.isNumeric(taskInfo[0]))
						colorIndex = Integer.parseInt(taskInfo[0]);
					String taskPlayerName = taskInfo[3];
					if (taskInfo.length > 4)
					{
						for (int i = 4; i < taskInfo.length; i++)
						{
							taskPlayerName += "|" + taskInfo[i];
						}
					}
					if (StringUtils.isNotEmpty(taskPlayerName))
					{
						try
						{
							List<AllianceTaskInfo> taskInfoArr = JSON.parseArray(taskPlayerName, AllianceTaskInfo.class);
							if (taskInfoArr != null && taskInfoArr.size() >= 1 && taskInfoArr.get(0) != null)
							{
								String publisher = taskInfoArr.get(0).getName();
								if (taskInfoArr.size() == 1 && taskInfo[1].equals(LanguageKeys.TIP_ALLIANCE_TASK_SHARE_1))
								{
									str = LanguageManager.getLangByKey(LanguageKeys.TIP_ALLIANCE_TASK_SHARE_1, publisher, taskName);
								}
								else if (taskInfoArr.size() == 2 && taskInfo[1].equals(LanguageKeys.TIP_ALLIANCE_TASK_SHARE_2))
								{
									AllianceTaskInfo taskInfo2 = taskInfoArr.get(1);
									if (taskInfo2 != null)
									{
										str = LanguageManager.getLangByKey(LanguageKeys.TIP_ALLIANCE_TASK_SHARE_2, publisher, taskName,
												taskInfo2.getName());
									}
								}
							}
						}
						catch (Exception e)
						{
							e.printStackTrace();
						}
					}
				}
			}

		}

		if (item.isVersionInvalid())
		{
			str = LanguageManager.getLangByKey(LanguageKeys.MSG_VERSION_NO_SUPPORT);
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

			SpannableStringBuilder style = new SpannableStringBuilder(text2);
			style.clearSpans();

			if (ChatServiceController.getCurrentChannelType() < DBDefinition.CHANNEL_TYPE_USER && item.isSystemMessage()
					&& !item.isHornMessage())
			{
				ImageGetter imageGetter = new ImageGetter()
				{
					@Override
					public Drawable getDrawable(String source)
					{
						if (c == null)
							return null;

						Drawable d = c.getResources().getDrawable(R.drawable.sys);
						if (ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
						{
							d.setBounds(0, -10,
									(int) (d.getIntrinsicWidth() * 0.8f * ConfigManager.scaleRatio * getScreenCorrectionFactor()),
									(int) (d.getIntrinsicHeight() * 0.9f * ConfigManager.scaleRatio * getScreenCorrectionFactor()) - 10);
						}
						else
						{
							d.setBounds(0, -10, (int) (d.getIntrinsicWidth() * 0.8f), (int) (d.getIntrinsicHeight() * 0.9f) - 10);
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
						if (c == null)
							return null;

						Drawable d = c.getResources().getDrawable(R.drawable.mail_battlereport);
						if (ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
						{
							d.setBounds(0, 0, (int) (d.getIntrinsicWidth() * 0.5 * ConfigManager.scaleRatio * getScreenCorrectionFactor()),
									(int) (d.getIntrinsicHeight() * 0.5 * ConfigManager.scaleRatio * getScreenCorrectionFactor()));
						}
						else
						{
							d.setBounds(0, 0, (int) (d.getIntrinsicWidth() * 0.5), (int) (d.getIntrinsicHeight() * 0.5));
						}
						return d;
					}
				};

				style.append(Html.fromHtml("<img src='" + R.drawable.mail_battlereport + "'/>", imageGetter, null));
			}

			// 添加末尾的装备分享
			if (item.isEquipMessage())
			{

				int color = getColorByIndex(colorIndex);
				String txt = text.toString();
				int start = txt.indexOf(equipName) + 1;
				style.setSpan(new ForegroundColorSpan(color), start, start + equipName.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);

				ImageGetter imageGetter = new ImageGetter()
				{
					@Override
					public Drawable getDrawable(String source)
					{
						if (c == null)
							return null;

						Drawable d = c.getResources().getDrawable(R.drawable.equip_share);
						if (ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
						{
							d.setBounds(0, 0, (int) (d.getIntrinsicWidth() * 0.8 * ConfigManager.scaleRatio * getScreenCorrectionFactor()),
									(int) (d.getIntrinsicHeight() * 0.8 * ConfigManager.scaleRatio * getScreenCorrectionFactor()));
						}
						else
						{
							d.setBounds(0, 0, (int) (d.getIntrinsicWidth() * 0.8), (int) (d.getIntrinsicHeight() * 0.8));
						}
						return d;
					}
				};

				style.append(Html.fromHtml("<img src='" + R.drawable.equip_share + "'/>", imageGetter, null));
			}

			// 添加末尾的联盟任务分享
			if (item.isAllianceTaskMessage())
			{

				int color = getColorByIndex(colorIndex);
				String txt = text.toString();
				int start = txt.indexOf(taskName) + 1;
				style.setSpan(new ForegroundColorSpan(color), start, start + taskName.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
			}

			// 添加时间、翻译信息
			String time = "";
			if (!item.isSelfMsg())
			{
				if (isTranslated)
				{
					String originalLang = item.originalLang;
					String[] originalLangArr = originalLang.split(",");
					String lang = originalLang;
					for (int i = 0; i < originalLangArr.length; i++)
					{
						if (StringUtils.isNotEmpty(originalLangArr[i]))
						{
							lang = LanguageManager.getOriginalLangByKey(originalLangArr[i]);
							if (!lang.startsWith("lang."))
								break;
						}
					}
					time += " " + LanguageManager.getLangByKey(LanguageKeys.TIP_TRANSLATED_BY, lang);
				}
				else if (!isTranslated
						&& TranslateManager.getInstance().isTranslateMsgValid(item)
						&& !item.isTranlateDisable()
						&& !item.isOriginalSameAsTargetLang()
						&& (ChatServiceController.isDefaultTranslateEnable || (!ChatServiceController.isDefaultTranslateEnable && item.hasTranslatedByForce)))
				{
					time += " " + LanguageManager.getLangByKey(LanguageKeys.MENU_ORIGINALLAN);
				}
			}

			String brStr = "";
			if (StringUtils.isNotEmpty(time))
				brStr = "<br/>";
			SpannableString styledResultText = new SpannableString(Html.fromHtml(brStr + "<small><font color='#808080'>" + time
					+ "</font></small>"));
			if (!item.isSystemMessage() && item.isKingMsg())
				styledResultText = new SpannableString(Html.fromHtml(brStr + "<small><font color='#808080'>" + time + "</font></small>"));
			styledResultText.setSpan(new AlignmentSpan.Standard(Alignment.ALIGN_OPPOSITE), 0, styledResultText.length(),
					Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
			style.append(styledResultText);

			Spannable sp = (Spannable) textView.getText();
			URLSpan[] urls = sp.getSpans(0, end, URLSpan.class);
			for (URLSpan url : urls)
			{
				MyURLSpan myURLSpan = new MyURLSpan(url.getURL());
				if (item.isSystemMessage())
				{
					int endPos = sp.getSpanEnd(url) + 1 <= end - 1 ? sp.getSpanEnd(url) + 1 : end - 1;
					style.setSpan(myURLSpan, sp.getSpanStart(url) + 1, endPos, Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
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

	private TextView	joinAnnounceTextView;
	private MsgItem		joinAnnounceItem;

	public void onJoinAnnounceInvitationSuccess()
	{
		if (joinAnnounceTextView != null && joinAnnounceItem != null)
		{
			if (joinAnnounceItem.hasTranslated)
				setText(joinAnnounceTextView, joinAnnounceItem.currentText, joinAnnounceItem, true);
			else
				setText(joinAnnounceTextView, joinAnnounceItem.currentText, joinAnnounceItem, false);
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
				MessageViewHolder holder = (MessageViewHolder) widget.getTag();
				if (holder != null)
				{
					joinAnnounceItem = holder.currentMsgItem;

					if (coords.length == 2)
					{
						ServiceInterface.allianceIdJoining = coords[1];
						JniController.getInstance().excuteJNIVoidMethod("joinAnnounceInvitation", new Object[] { coords[1] });
					}
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
				System.out.println("showequip:" + coords[0]);
				if (coords.length == 2 && StringUtils.isNotEmpty(coords[1]))
				{
					System.out.println("showequip:" + coords[1]);
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
		private String	mUrl;

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

	/**
	 * 高ppi手机的缩放修正因子
	 */
	public double getScreenCorrectionFactor()
	{
		int density = c.getResources().getDisplayMetrics().densityDpi;

		if (density >= DisplayMetrics.DENSITY_XXHIGH)
		{
			// 小米note3是640，大于DENSITY_XXHIGH
			return 0.8;
		}
		else
		{
			return 1.0;
		}
	}

	private boolean	ignoreClick	= false;

	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	public void copyToClipboard(QuickAction source)
	{
		if (source.currentTextView == null || !(source.currentTextView instanceof TextView))
			return;

		MsgItem item = getMsgItemFromQuickAction(source);
		if (item == null)
			return;
		String text = item.canShowTranslateMsg() ? item.translateMsg : item.msg;

		int sdk = android.os.Build.VERSION.SDK_INT;
		if (sdk < android.os.Build.VERSION_CODES.HONEYCOMB)
		{
			android.text.ClipboardManager clipboard = (android.text.ClipboardManager) ((Activity) c)
					.getSystemService(Context.CLIPBOARD_SERVICE);
			clipboard.setText(text);
		}
		else
		{
			// 一个label对应一个clipboard slot
			android.content.ClipboardManager clipboard = (android.content.ClipboardManager) ((Activity) c)
					.getSystemService(Context.CLIPBOARD_SERVICE);
			android.content.ClipData clip = android.content.ClipData.newPlainText("cok_" + item.getName() + "_" + item.getSendTime(), text);
			clipboard.setPrimaryClip(clip);
		}
	}

	// 去掉复制后文本中的[obj]块（图片导致）
	// http://stackoverflow.com/questions/8560045/android-getting-obj-using-textview-settextcharactersequence
	private CharSequence stripHtml(String s)
	{
		String result = s.substring(0, s.lastIndexOf("\n"));
		result = result.replaceAll("\n", "#linebreak#");
		result = Html.fromHtml(result).toString().replace((char) 65532, (char) 32);
		result = result.replaceAll("#linebreak#", "\n");
		result = result.trim();
		return result;
	}

	public void destroy()
	{
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

	@Override
	public int getCount()
	{
		if (items != null)
			return items.size();
		return 0;
	}

	@Override
	public Object getItem(int position)
	{
		if (position >= 0 && position < items.size())
			return items.get(position);
		return null;
	}

	@Override
	public long getItemId(int position)
	{
		return position;
	}
}