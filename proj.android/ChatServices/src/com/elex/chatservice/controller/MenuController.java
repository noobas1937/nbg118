package com.elex.chatservice.controller;

import java.util.ArrayList;
import java.util.List;

import net.londatiga.android.QuickAction;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.drawable.ColorDrawable;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.elex.chatservice.R;
import com.elex.chatservice.model.ChannelListItem;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ScaleUtil;
import com.elex.chatservice.view.ChatFragment;
import com.elex.chatservice.view.ChatQuickActionFactory;
import com.elex.chatservice.view.MemberSelectorFragment;
import com.elex.chatservice.view.MessagesAdapter;
import com.elex.chatservice.view.MessagesAdapter.MessageHolder;
import com.elex.chatservice.view.actionbar.MyActionBarActivity;

public class MenuController
{
	public static void handleItemClick(final MessagesAdapter adapter, QuickAction source, int pos, int actionId)
	{
		final MsgItem item = MessagesAdapter.getMsgItemFromQuickAction(source);

		switch (actionId)
		{
			case ChatQuickActionFactory.ID_INVITE:
				ChatServiceController.doHostAction("inviteJoinAlliance", item.uid, item.getName(), "", true);
				break;
			case ChatQuickActionFactory.ID_JOIN_ALLIANCE:
				ChatServiceController.doHostAction("joinAlliance", item.uid, item.getName(), "", true);
				break;
			case ChatQuickActionFactory.ID_COPY:
				adapter.copyToClipboard(source);
				break;
			case ChatQuickActionFactory.ID_SEND_MAIL:
				if(ChatServiceController.isNewMailListEnable){
					ChatServiceController.isCreateChatRoom = false;
					ServiceInterface.setMailInfo(item.uid,"",item.getName(),MailManager.MAIL_USER);
					ServiceInterface.showChatActivity(ChatServiceController.getCurrentActivity(), DBDefinition.CHANNEL_TYPE_USER, false);
				}else{
					ChatServiceController.doHostAction("sendMail", item.uid, item.getName(), "", true);
				}
				break;
			case ChatQuickActionFactory.ID_VIEW_PROFILE:
				System.out.println("ChatServiceController.isContactMod:"+ChatServiceController.isContactMod);
				if(ChatServiceController.isContactMod)
					ChatServiceController.doHostAction("showPlayerInfo@mod", item.uid, item.getName(), "", true);
				else
					ChatServiceController.doHostAction("showPlayerInfo", item.uid, item.getName(), "", true);
				break;
			case ChatQuickActionFactory.ID_BLOCK:
				showConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_SHIELD_PLAYER, item.getName()), item, UserManager.BLOCK_LIST);
				break;
			case ChatQuickActionFactory.ID_UNBLOCK:
				ChatServiceController.getInstance().host.unShieldPlayer(item.uid, item.getName());
				UserManager.getInstance().removeRestrictUser(item.getName(), UserManager.BLOCK_LIST);
				break;
			case ChatQuickActionFactory.ID_BAN:
				showBanConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_BAN, item.getName()), item);
				break;
			case ChatQuickActionFactory.ID_UNBAN:
				ChatServiceController.getInstance().host.unBanPlayer(item.uid);
				UserManager.getInstance().removeRestrictUser(item.getName(), UserManager.BAN_LIST);
				break;
			case ChatQuickActionFactory.ID_TRANSLATE:
				final MessageHolder holder = (MessageHolder) source.currentTextView.getTag();
				adapter.showTranslatedLanguage(holder, item);
				break;
			case ChatQuickActionFactory.ID_ORIGINAL_LANGUAGE:
				final MessageHolder holder2 = (MessageHolder) source.currentTextView.getTag();
				adapter.showOriginalLanguage(holder2, item);
				break;
			case ChatQuickActionFactory.ID_VIEW_BATTLE_REPORT:
				ChatServiceController.doHostAction("viewBattleReport", item.uid, "", item.attachmentId, true);
				break;
			case ChatQuickActionFactory.ID_VIEW_DETECT_REPORT:
				ChatServiceController.doHostAction("viewDetectReport", item.uid, "", item.attachmentId, true);
				break;
			case ChatQuickActionFactory.ID_VIEW_EQUIPMENT:
				ChatServiceController.doHostAction("showEquipment", "", "", item.attachmentId, true);
				break;
			case ChatQuickActionFactory.ID_REPORT_HEAD_IMG:
				if(UserManager.getInstance().isInRestrictList(item.uid, UserManager.REPORT_LIST))
				{
					showContentConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_HEADIMG_REPORTED));
				}
				else
				{
					showConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_REPORT_HEADIMG,item.getName()),item,UserManager.REPORT_LIST);
				}
				break;
			default:
				break;
		}
	}

	private static Window initAlertDialog(AlertDialog dlg, int id)
	{
		dlg.show();
		Window window = dlg.getWindow();
		window.setContentView(id);
		window.setBackgroundDrawable(new ColorDrawable());
		window.setLayout(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
		return window;
	}
	private static void setDismissListener(FrameLayout frame, final Dialog dlg)
	{
		frame.setOnTouchListener(new OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				dlg.dismiss();
				return true;
			}
		});
	}
	
	private static void showBanConfirm(String content, final MsgItem item)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		
		Window window = initAlertDialog(dlg, R.layout.cs__ban_confirm_dialog);
		setDismissListener((FrameLayout) window.findViewById(R.id.banConfirmFrameLayout), dlg);
		
		TextView alertTextView = (TextView) window.findViewById(R.id.textView1);
		alertTextView.setText(content);

		final ArrayList<CheckBox> checkBoxs = new ArrayList<CheckBox>();
		int[] checkBoxIds = { R.id.checkBox1, R.id.checkBox2, R.id.checkBox3, R.id.checkBox4 };
		String[] timeValues = { " 1", " 2", " 3", "4" };
		String timeStr = LanguageManager.getLangByKey(LanguageKeys.TIP_TIME);

		OnClickListener checkOnClickListener = new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				for (int i = 0; i < checkBoxs.size(); i++)
				{
					checkBoxs.get(i).setChecked((i + 1) == ((Integer) (v.getTag())).intValue());
				}
			}
		};

		for (int i = 0; i < checkBoxIds.length; i++)
		{
			CheckBox checkBox = (CheckBox) window.findViewById(checkBoxIds[i]);
			checkBox.setText(timeValues[i] + timeStr);
			checkBox.setTag(Integer.valueOf(i + 1));
			checkBox.setOnClickListener(checkOnClickListener);
			checkBoxs.add(checkBox);
		}

		// 为确认按钮添加事件,执行退出应用操作
		Button ok = (Button) window.findViewById(R.id.okBanBtn);
		ok.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_CONFIRM));
		ok.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();

				int selectIndex = 0;
				for (int i = 0; i < checkBoxs.size(); i++)
				{
					if (checkBoxs.get(i).isChecked())
					{
						selectIndex = i;
						break;
					}
				}

				ChatServiceController.getInstance().host.banPlayerByIndex(item.uid, selectIndex);
				UserManager.getInstance().addRestrictUser(item.uid, UserManager.BAN_LIST);
			}
		});
		// 关闭alert对话框架
		Button cancel = (Button) window.findViewById(R.id.cancelBanBtn);
		cancel.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_CANCEL));
		cancel.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
			}
		});

		adjustBanDialog(alertTextView, ok, cancel, checkBoxs);
	}

	private static void adjustBanDialog(TextView alertTextView, Button ok, Button cancel, final ArrayList<CheckBox> checkBoxs)
	{
		if (ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
		{
			ScaleUtil.adjustTextSize(alertTextView, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(ok, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(cancel, ConfigManager.scaleRatio);
			for (int i = 0; i < checkBoxs.size(); i++)
			{
				ScaleUtil.adjustTextSize(checkBoxs.get(i), ConfigManager.scaleRatio);
			}
		}
	}

	private static void showConfirm(String content, final MsgItem item, final int type)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				if (type == UserManager.BLOCK_LIST)
				{
					ChatServiceController.getInstance().host.shieldPlayer(item.uid);
				}
				else if(type == UserManager.REPORT_LIST)
				{
					ChatServiceController.getInstance().host.reportCustomHeadImg(item.uid);
					Toast toast = Toast.makeText(ChatServiceController.getCurrentActivity(),LanguageManager.getLangByKey(LanguageKeys.TIP_REPORT_HEADIMG_SUCCESS),Toast.LENGTH_SHORT);
					toast.setGravity(Gravity.TOP, 0, ChatServiceController.getCurrentActivity().getToastPosY());
					toast.show();
				}
				UserManager.getInstance().addRestrictUser(item.uid, type);
			}
		};
		setDialogView(dlg,content,okOnlickListener,0,true);
	}

	private static void adjustConfirmDialog(TextView alertTextView, Button ok, Button cancel)
	{
		if (ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
		{
			ScaleUtil.adjustTextSize(alertTextView, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(ok, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(cancel, ConfigManager.scaleRatio);
		}
	}
	
	private static void adjustConfirmCoinDialog(TextView alertTextView, TextView okTextView,TextView coinTextView, Button cancel)
	{
		if (ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
		{
			ScaleUtil.adjustTextSize(alertTextView, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(okTextView, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(coinTextView, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(cancel, ConfigManager.scaleRatio);
		}
	}

	public static void showReSendConfirm(String content, final MsgItem msgItem)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				if(msgItem.isHornMessage())
				{
					if(ChatServiceController.getInstance().host.getHornBanedTime()==0)
					{
						int price=ChatServiceController.getInstance().host.isHornEnough();
						String horn=LanguageManager.getLangByKey(LanguageKeys.TIP_HORN);
						if(price==0)
						{
							showResendHornMessageConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_USEITEM, horn), msgItem);
						}
						else if(price>0)
						{
							showResendHornWithCornConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_ITEM_NOT_ENOUGH, horn), msgItem, price);
						}
					}
					else
					{
						Toast.makeText(ChatServiceController.getCurrentActivity(), "you have been baned!", Toast.LENGTH_SHORT).show();
					}
				}
				else
				{
					ChatServiceController.getInstance().resendMsg(msgItem, false, false);
				}
				
			}
		};
		setDialogView(dlg,content,okOnlickListener,0,true);
	}
	
	public static void showResendHornMessageConfirm(String content, final MsgItem msgItem)
	{
		final int price=ChatServiceController.getInstance().host.isHornEnough();
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				ChatServiceController.resendMsg(msgItem, true,false);
			}
		};
		setDialogView(dlg,content,okOnlickListener,price,true);
	}
	
	public static void showResendHornWithCornConfirm(String content, final MsgItem msgItem,final int price)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				System.out.println("showResendHornWithCornConfirm price:"+price);
				if(ChatServiceController.getInstance().host.isCornEnough(price))
				{
					ChatServiceController.resendMsg(msgItem, true,true);
				}
				else
				{
					showCornNotEnoughConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_CORN_NOT_ENOUGH));
				}
			}
		};
		
		setDialogViewWithCoin(dlg,content,okOnlickListener,price,true);
	}
	
	
	public static void showSendHornMessageConfirm(String content, final String message)
	{
		final int price=ChatServiceController.getInstance().host.isHornEnough();
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				ChatServiceController.sendMsg(message, true,false);
			}
		};
		setDialogView(dlg,content,okOnlickListener,price,true);
	}
	
	public static void showSendHornWithCornConfirm(String content, final String message,final int price)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				System.out.println("showSendHornWithCornConfirm price:"+price);
				if(ChatServiceController.getInstance().host.isCornEnough(price))
				{
					ChatServiceController.sendMsg(message, true,true);
				}
				else
				{
					showCornNotEnoughConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_CORN_NOT_ENOUGH));
				}
			}
		};
		setDialogViewWithCoin(dlg,content,okOnlickListener,price,true);
	}
	
	public static void showCornNotEnoughConfirm(String content)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
			}
		};
		setDialogView(dlg,content,okOnlickListener,0,false);
	}
	
	public static void showCreateChatRoomConfirm(final MyActionBarActivity activity,String content,final ArrayList<String> memberUidAdd)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				
				ChatServiceController.getInstance().host.createChatRoom(UserManager.getInstance().createNameStr(memberUidAdd),
						UserManager.getInstance().createUidStr(memberUidAdd),
        				MemberSelectorFragment.roomName,"");
				activity.exitActivity();
			}
		};
		setDialogView(dlg,content,okOnlickListener,0,true);
	}
	
	public static void showChatRoomManagerConfirm(final MyActionBarActivity activity,String content,final ArrayList<String> memberUidAdd,final ArrayList<String> memberUidRemoved,final String roomName)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				
				if(ChatServiceController.isCreateChatRoom)
				{
					if(ChatServiceController.isNewMailListEnable)
					{
						ChatServiceController.isCreateChatRoom=true;
						Intent intent = new Intent();  
		                intent.putExtra("roomName", MemberSelectorFragment.roomName);
		                intent.putExtra("uidStr", UserManager.getInstance().createUidStr(memberUidAdd));  
		                intent.putExtra("nameStr", UserManager.getInstance().createNameStr(memberUidAdd));  
		                activity.setResult(Activity.RESULT_OK, intent);  
		                activity.exitActivity();
					}
					else
					{
						ChatServiceController.getInstance().host.selectChatRoomMember(MemberSelectorFragment.roomName, 
		        				UserManager.getInstance().createNameStr(memberUidAdd),
		        				UserManager.getInstance().createUidStr(memberUidAdd));
						activity.exitActivity();
					}
				}
            	else
            	{
            		if(memberUidAdd!=null && memberUidAdd.size()>0)
            		{	
            			ChatServiceController.getInstance().host.inviteChatRoomMember(UserManager.getInstance().getCurrentMail().opponentUid, 
            				UserManager.getInstance().createNameStr(memberUidAdd),
            				UserManager.getInstance().createUidStr(memberUidAdd));
            		}
            		if(memberUidRemoved!=null && memberUidRemoved.size()>0)
            		{
            			ChatServiceController.getInstance().host.kickChatRoomMember(UserManager.getInstance().getCurrentMail().opponentUid, 
                				UserManager.getInstance().createNameStr(memberUidRemoved),
                				UserManager.getInstance().createUidStr(memberUidRemoved));
            		}
            		if(roomName!=null && !roomName.equals(""))
            		{
            			ChatServiceController.getInstance().host.modifyChatRoomName(UserManager.getInstance().getCurrentMail().opponentUid, roomName);
            		}
            		
//            		activity.showFragment(ChatFragment.class.getName());
            		activity.exitActivity();
            	}
			}
		};
		setDialogView(dlg,content,okOnlickListener,0,true);
	}
	
	public static void showInviteChatRoomMemberConfirm(final MyActionBarActivity activity,String content,final ArrayList<String> memberUidAdd)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				
				if(ChatServiceController.isCreateChatRoom)
				{
					if(ChatServiceController.isNewMailListEnable)
					{
						ChatServiceController.isCreateChatRoom=true;
						Intent intent = new Intent();  
		                intent.putExtra("roomName", MemberSelectorFragment.roomName);
		                intent.putExtra("uidStr", UserManager.getInstance().createUidStr(memberUidAdd));  
		                intent.putExtra("nameStr", UserManager.getInstance().createNameStr(memberUidAdd));  
		                activity.setResult(Activity.RESULT_OK, intent);  
		                activity.exitActivity();
					}
					else
					{
						ChatServiceController.getInstance().host.selectChatRoomMember(MemberSelectorFragment.roomName, 
		        				UserManager.getInstance().createNameStr(memberUidAdd),
		        				UserManager.getInstance().createUidStr(memberUidAdd));
						activity.exitActivity();
					}
				}
            	else
            	{
            		ChatServiceController.getInstance().host.inviteChatRoomMember(UserManager.getInstance().getCurrentMail().opponentUid, 
            				UserManager.getInstance().createNameStr(memberUidAdd),
            				UserManager.getInstance().createUidStr(memberUidAdd));
//            		activity.showFragment(ChatFragment.class.getName());
            		activity.exitActivity();
            	}
			}
		};
		setDialogView(dlg,content,okOnlickListener,0,true);
	}
	
	public static void showKickChatRoomMemberConfirm(final MyActionBarActivity activity,String content,final ArrayList<String> memberUidRemoved)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				if(ChatServiceController.isCreateChatRoom)
            		activity.exitActivity();
            	else
            	{
            		ChatServiceController.getInstance().host.kickChatRoomMember(UserManager.getInstance().getCurrentMail().opponentUid, 
            				UserManager.getInstance().createNameStr(memberUidRemoved),
            				UserManager.getInstance().createUidStr(memberUidRemoved));
//            		activity.showFragment(ChatFragment.class.getName());
            		activity.exitActivity();
            	}
			}
		};
		setDialogView(dlg,content,okOnlickListener,0,true);
	}
	
	public static void modifyChatRoomNameConfirm(final MyActionBarActivity activity,String content,final String roomName)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				ChatServiceController.getInstance().host.modifyChatRoomName(UserManager.getInstance().getCurrentMail().opponentUid, roomName);
				activity.exitActivity();
			}
		};
		setDialogView(dlg,content,okOnlickListener,0,true);
	}
	
	public static void quitChatRoomConfirm(final MyActionBarActivity activity,String content)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				ChatServiceController.getInstance().host.quitChatRoom(UserManager.getInstance().getCurrentMail().opponentUid);
//				activity.showFragment(ChatFragment.class.getName());
        		activity.exitActivity();
			}
		};
		setDialogView(dlg,content,okOnlickListener,0,true);
	}
	
	private static AlertDialog createAlertDialog()
	{
		if(ChatServiceController.getCurrentActivity() == null) return null;
		try
		{
			return new AlertDialog.Builder(ChatServiceController.getCurrentActivity()).create();
		}
		catch (Exception e)
		{
			return null;
		}
	}
	
	private static void setDialogView(final AlertDialog dlg,String content, OnClickListener onOKClickListener,int corn,boolean cancelBtnShow)
	{
		try{
			System.out.println("setDialogView content:"+content);

			Window window = initAlertDialog(dlg, R.layout.cs__confirm_dialog);
			setDismissListener((FrameLayout) window.findViewById(R.id.confirmFrameLayout), dlg);
			
			TextView alertTextView = (TextView) window.findViewById(R.id.alertTextView);
			Button ok = (Button) window.findViewById(R.id.exitBtn0);
			Button cancel = (Button) window.findViewById(R.id.exitBtn1);
			cancel.setVisibility(cancelBtnShow?View.VISIBLE:View.GONE);
	
			adjustConfirmDialog(alertTextView, ok, cancel);
	
			alertTextView.setText(content);
	
			ok.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_CONFIRM));
			
			ok.setOnClickListener(onOKClickListener);
	
			cancel.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_CANCEL));
			cancel.setOnClickListener(new View.OnClickListener()
			{
				@Override
				public void onClick(View v)
				{
					dlg.cancel();
				}
			});
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}
	
	private static void setDialogViewWithCoin(final AlertDialog dlg,String content, OnClickListener onOKClickListener,int coin,boolean cancelBtnShow)
	{
		Window window = initAlertDialog(dlg, R.layout.cs__gold_confirm_dialog);
		setDismissListener((FrameLayout) window.findViewById(R.id.goldConfirmFrameLayout), dlg);
		
		TextView alertTextView = (TextView) window.findViewById(R.id.alertTextView);
		LinearLayout confirm_layout = (LinearLayout) window.findViewById(R.id.confirm_layout);
		TextView coin_count_text=(TextView)window.findViewById(R.id.confirm_coin_count);
		TextView ok_btn_text=(TextView)window.findViewById(R.id.ok_btn_text);
		Button cancel = (Button) window.findViewById(R.id.exitBtn1);
		cancel.setVisibility(cancelBtnShow?View.VISIBLE:View.GONE);

		adjustConfirmCoinDialog(alertTextView, ok_btn_text,coin_count_text, cancel);

		alertTextView.setText(content);
		coin_count_text.setText(""+coin);

		ok_btn_text.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_CONFIRM));
		
		confirm_layout.setOnClickListener(onOKClickListener);

		cancel.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_CANCEL));
		cancel.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
			}
		});
	}
	
	public static void showContentConfirm(String content)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
			}
		};
		
		try{
			Window window = initAlertDialog(dlg, R.layout.cs__confirm_dialog);

			setDismissListener((FrameLayout) window.findViewById(R.id.confirmFrameLayout), dlg);
			
			TextView alertTextView = (TextView) window.findViewById(R.id.alertTextView);
			Button ok = (Button) window.findViewById(R.id.exitBtn0);
			Button cancel = (Button) window.findViewById(R.id.exitBtn1);
			cancel.setVisibility(View.GONE);
			adjustConfirmDialog(alertTextView, ok, cancel);
			alertTextView.setText(content);
			ok.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_CONFIRM));
			ok.setOnClickListener(okOnlickListener);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}
	
	public static void showDeleteSysMailConfirm(String content,final MailData mailData)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				if(ChatServiceController.getChannelListFragment()!=null)
				{	
					ChatServiceController.getChannelListFragment().actualDeleteSingleSysMail(mailData);
				}
				dlg.cancel();
			}
		};
		
		setDialogView(dlg, content, okOnlickListener, 0, true);
	}
	
	public static void showDeleteChannelConfirm(String content,final ChatChannel channel)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				ChatServiceController.getChannelListFragment().actualDeleteSingleChannel(channel);
				dlg.cancel();
			}
		};
		
		setDialogView(dlg,content,okOnlickListener,0,true);
	}
	
	public static void showDeleteMutiMail(String content,final List<ChannelListItem> list)
	{
		final AlertDialog dlg = createAlertDialog();
		if(dlg == null) return;
		OnClickListener okOnlickListener=new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				ChatServiceController.getChannelListFragment().comfirmDeleteMutiMail(list);
				dlg.cancel();
			}
		};
		
		setDialogView(dlg,content,okOnlickListener,0,true);
	}
}
