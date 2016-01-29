package com.elex.chatservice.view.actionbar;

import java.util.List;

import org.apache.commons.lang.StringUtils;

import android.annotation.TargetApi;
import android.content.Context;
import android.os.Build;
import android.os.Bundle;
import android.os.PowerManager;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentTransaction;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewTreeObserver;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.AnticipateInterpolator;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.RotateAnimation;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.elex.chatservice.R;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.JniController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.FlyMutiRewardInfo;
import com.elex.chatservice.model.FlyRewardInfo;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.util.ImageUtil;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.util.ScaleUtil;
import com.elex.chatservice.view.ChatActivity;
import com.elex.chatservice.view.ChatFragment;
import com.elex.chatservice.view.ICocos2dxScreenLockListener;
import com.nineoldandroids.animation.Animator;
import com.nineoldandroids.animation.Animator.AnimatorListener;
import com.nineoldandroids.animation.AnimatorSet;
import com.nineoldandroids.animation.ObjectAnimator;
import com.nineoldandroids.view.ViewHelper;

public abstract class MyActionBarActivity extends FragmentActivity
{
	public Button				backButton;
	public TextView				titleLabel;
	public Button				optionButton;
	public Button				editButton;
	public Button				writeButton;
	public Button				returnButton;
	public Button				showFriend;
	public RelativeLayout		actionbarLayout;
	protected int				fragmentHolderId;
	public ActionBarFragment	fragment;
	protected Bundle			bundle;
	protected RelativeLayout	fragmentLayout;
	public FrameLayout			fragment_holder;
	public ProgressBar			activityProgressBar;
	public LinearLayout			reward_loading_layout;
	public TextView				loading_textview;
	public FrameLayout			reward_fly_layout;
	private LayoutInflater		inflater;
	// 红包面板
	private FrameLayout			red_package_root_layout;
	private TextView			red_package_sendername;
	private TextView			red_package_sendertip;
	// private TextView red_package_msg;
	private ImageView			red_package_senderHeaderPic;
	private ImageView			red_package_HeaderPicContainer;
	private TextView			red_package_detail;
	private TextView			red_package_warning;
	private LinearLayout		red_package_warning_layout;
	private ImageView			red_package_open_btn;
	private LinearLayout		red_package_unhandlelayout;
	private View				red_package_background_layout;
	private MsgItem				redPackageItem;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		ChatServiceController.setCurrentActivity(this);
		ServiceInterface.pushActivity(this);
		inflater = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		// actionBarHelper.onCreate(savedInstanceState);
		// actionBarHelper.setDisplayHomeAsUpEnabled(true);
		if (ConfigManager.getInstance().scaleFontandUI)
		{
			ConfigManager.calcScale(this);
		}

		setContentView(R.layout.cs__chat_activity);

		backButton = (Button) findViewById(R.id.cs__actionbar_backButton);
		titleLabel = (TextView) findViewById(R.id.cs__actionbar_titleLabel);
		optionButton = (Button) findViewById(R.id.cs__actionbar_optionButton);
		editButton = (Button) findViewById(R.id.cs__actionbar_editButton);
		writeButton = (Button) findViewById(R.id.cs__actionbar_writeButton);
		returnButton = (Button) findViewById(R.id.cs__actionbar_returnButton);
		showFriend = (Button) findViewById(R.id.cs__actionbar_showFriendButton);
		actionbarLayout = (RelativeLayout) findViewById(R.id.cs__actionbar_layout);
		fragmentLayout = (RelativeLayout) findViewById(R.id.cs__activity_fragment_layout);

		red_package_root_layout = (FrameLayout) findViewById(R.id.red_package_root_layout);
		red_package_background_layout = findViewById(R.id.red_package_background_layout);
		red_package_sendername = (TextView) findViewById(R.id.red_package_sendername);
		red_package_sendertip = (TextView) findViewById(R.id.red_package_sendertip);
		// red_package_msg = (TextView) findViewById(R.id.red_package_msg);
		red_package_senderHeaderPic = (ImageView) findViewById(R.id.red_package_senderHeaderPic);
		red_package_HeaderPicContainer = (ImageView) findViewById(R.id.red_package_HeaderPicContainer);
		red_package_detail = (TextView) findViewById(R.id.red_package_detail);
		red_package_warning = (TextView) findViewById(R.id.red_package_warning);
		red_package_warning_layout = (LinearLayout) findViewById(R.id.red_package_warning_layout);
		red_package_open_btn = (ImageView) findViewById(R.id.red_package_open_btn);
		red_package_unhandlelayout = (LinearLayout) findViewById(R.id.red_package_unhandlelayout);
		red_package_root_layout.setVisibility(View.GONE);

		redPackageItem = null;

		red_package_background_layout.setOnClickListener(new OnClickListener()
		{

			@Override
			public void onClick(View v)
			{
				hideRedPackagePopup();
			}
		});

		backButton.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				onBackButtonClick();
			}
		});
		onGlobalLayoutListener = new ViewTreeObserver.OnGlobalLayoutListener()
		{
			@Override
			public void onGlobalLayout()
			{
				adjustSize();
			}
		};
		actionbarLayout.getViewTreeObserver().addOnGlobalLayoutListener(onGlobalLayoutListener);

		this.fragmentHolderId = R.id.cs__activity_fragment_holder;
		fragment_holder = (FrameLayout) findViewById(fragmentHolderId);

		activityProgressBar = (ProgressBar) findViewById(R.id.cs__activity_progress_bar);
		hideProgressBar();

		reward_loading_layout = (LinearLayout) findViewById(R.id.reward_loading_layout);
		ViewHelper.setAlpha(reward_loading_layout, 0.4f);
		hideRewardLoadingPopup();

		loading_textview = (TextView) findViewById(R.id.loading_textview);
		// loading_textview.setText(LanguageManager.getLangByKey(LanguageKeys.));

		reward_fly_layout = (FrameLayout) findViewById(R.id.reward_fly_layout);

		if (fragmentClass != null)
			showFragment(fragmentClass.getName());

		showBackground();
		ScaleUtil.initialize(this);
	}

	/**
	 * 在加载fragment前先在fragment容器上显示背景
	 * <p>
	 * 以防止因为fragment初始化较慢，内容区出现黑屏
	 */
	protected void showBackground()
	{
	}

	public void showRewardLoadingPopup()
	{
		reward_loading_layout.setVisibility(View.VISIBLE);
	}

	public void hideRewardLoadingPopup()
	{
		reward_loading_layout.setVisibility(View.GONE);
	}

	private OnClickListener createOnClickLinstener(final String actionName, final MsgItem msgItem)
	{
		return new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				if (actionName.equals("pickRedPackage"))
				{
					msgItem.handleRedPackageFinishState();
					if (msgItem.sendState == MsgItem.FINISH)
					{
						if( ChatServiceController.getChatFragment() != null )
							ChatServiceController.getChatFragment().showRedPackageConfirm(msgItem);
//						ServiceInterface.notifyDataSetChangedChatFragment();
					}
					else
					{
						System.out.println("getRedPackageStatus");
						JniController.getInstance().excuteJNIVoidMethod("getRedPackageStatus", new Object[] { msgItem.attachmentId });
					}
				}
				else
				{
					hideRedPackagePopup();
					ChatServiceController.doHostAction(actionName, "", "", msgItem.attachmentId, true);
				}
			}
		};
	}

	public MsgItem getRedPackagePopItem()
	{
		return redPackageItem;
	}

	private void showRedPackageBtnAnimation()
	{
		final RotateAnimation rotate = new RotateAnimation(-15, 15, Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF, 0.6f);
		rotate.setRepeatCount(3);
		rotate.setDuration(100);
		rotate.setRepeatMode(Animation.RESTART);

		final RotateAnimation rotate2 = new RotateAnimation(0, 0, Animation.RELATIVE_TO_SELF, 0.43f, Animation.RELATIVE_TO_SELF, 0.5f);
		rotate2.setDuration(2000);
		rotate2.setAnimationListener(new AnimationListener()
		{

			@Override
			public void onAnimationStart(Animation animation)
			{
			}

			@Override
			public void onAnimationRepeat(Animation animation)
			{
			}

			@Override
			public void onAnimationEnd(Animation animation)
			{
				System.out.println("scale onAnimationEnd");
				rotate.reset();
				red_package_open_btn.setAnimation(rotate);
				rotate.startNow();
				rotate2.cancel();
			}
		});

		rotate.setAnimationListener(new AnimationListener()
		{

			@Override
			public void onAnimationStart(Animation animation)
			{
			}

			@Override
			public void onAnimationRepeat(Animation animation)
			{
			}

			@Override
			public void onAnimationEnd(Animation animation)
			{
				System.out.println("rotate onAnimationEnd");
				rotate2.reset();
				red_package_open_btn.setAnimation(rotate2);
				rotate2.startNow();
				rotate.cancel();
			}
		});

		red_package_open_btn.setAnimation(rotate);
		rotate.startNow();
	}
	
	private void setHeadImageLayoutParams(ImageView headImageContainer,ImageView headImage)
	{
		int headWidthDP = 42;
		int headWidthDP2 = 39;
		int length1 = (int) (ScaleUtil.dip2px(this, headWidthDP) * ConfigManager.scaleRatio * getScreenCorrectionFactor());
		int length2 = (int) (ScaleUtil.dip2px(this, headWidthDP2) * ConfigManager.scaleRatio * getScreenCorrectionFactor());
		FrameLayout.LayoutParams layoutParams2 = (FrameLayout.LayoutParams)red_package_senderHeaderPic.getLayoutParams();
		layoutParams2.width = length2;
		layoutParams2.height = length2;
		
		FrameLayout.LayoutParams layoutParams1 = (FrameLayout.LayoutParams)red_package_HeaderPicContainer.getLayoutParams();
		layoutParams1.width = length1;
		layoutParams1.height = length1;
		red_package_HeaderPicContainer.setLayoutParams(layoutParams1);
		red_package_senderHeaderPic.setLayoutParams(layoutParams2);
	}

	public void showRedPackagePopup(MsgItem msgItem)
	{
		red_package_background_layout.setVisibility(View.VISIBLE);

		if (red_package_root_layout.getVisibility() != View.VISIBLE)
			red_package_root_layout.setVisibility(View.VISIBLE);
		ViewHelper.setAlpha(red_package_root_layout, 1.0f);
		ViewHelper.setScaleX(red_package_root_layout, 1.0f);
		ViewHelper.setScaleY(red_package_root_layout, 1.0f);

		showRedPackageBtnAnimation();
		redPackageItem = msgItem;
		red_package_open_btn.setOnClickListener(createOnClickLinstener("pickRedPackage", msgItem));
		red_package_detail.setOnClickListener(createOnClickLinstener("viewRedPackage", msgItem));

		ImageUtil.setHeadImage(this, msgItem.getHeadPic(), red_package_senderHeaderPic, msgItem.getUser());
		
		setHeadImageLayoutParams(red_package_HeaderPicContainer,red_package_senderHeaderPic);
		
		red_package_sendername.setText(msgItem.getName());
		red_package_sendertip.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_SEND_RED_PACKAGE,
				LanguageManager.getLangByKey(LanguageKeys.ITEM_RED_PACKAGE)));

		red_package_detail.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_RED_PACKAGE_DETAIL));

		if (msgItem.sendState == MsgItem.UNHANDLE)
		{
			red_package_unhandlelayout.setVisibility(View.VISIBLE);
			red_package_warning_layout.setVisibility(View.GONE);
		}
		else if (msgItem.sendState == MsgItem.NONE_MONEY)
		{
			red_package_unhandlelayout.setVisibility(View.GONE);
			red_package_warning_layout.setVisibility(View.VISIBLE);
			red_package_warning.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_RED_PACKAGE_NO_MONEY));
		}
		else if (msgItem.sendState == MsgItem.FINISH)
		{
			red_package_unhandlelayout.setVisibility(View.GONE);
			red_package_warning_layout.setVisibility(View.VISIBLE);
			red_package_warning.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_RED_PACKAGE_FINISH,
					LanguageManager.getLangByKey(LanguageKeys.ITEM_RED_PACKAGE)));
		}
	}

	public void hideRedPackagePopup()
	{
		Animator scaleX = ObjectAnimator.ofFloat(red_package_root_layout, "scaleX", 0);
		Animator scaleY = ObjectAnimator.ofFloat(red_package_root_layout, "scaleY", 0);
		Animator alpha = ObjectAnimator.ofFloat(red_package_root_layout, "alpha", 0);
		red_package_background_layout.setVisibility(View.GONE);
		AnimatorSet animator = new AnimatorSet();
		animator.playTogether(scaleX, scaleY,alpha);
		animator.setInterpolator(new AnticipateInterpolator(0.8f));
		animator.setDuration(300);
		animator.start();
		animator.addListener(new AnimatorListener()
		{

			@Override
			public void onAnimationStart(Animator animation)
			{
			}

			@Override
			public void onAnimationRepeat(Animator animation)
			{
			}

			@Override
			public void onAnimationEnd(Animator animation)
			{
				red_package_root_layout.setVisibility(View.GONE);
				redPackageItem = null;
			}

			@Override
			public void onAnimationCancel(Animator animation)
			{
			}
		});

	}

	public void showProgressBar()
	{
		activityProgressBar.setVisibility(View.VISIBLE);
	}

	public void hideProgressBar()
	{
		activityProgressBar.setVisibility(View.GONE);
	}

	// ---------------------------------------------------
	// Chat Activity相关
	// ---------------------------------------------------

	protected Class<?>	fragmentClass;

	public void showFragment(String className)
	{
		FragmentTransaction ft = getSupportFragmentManager().beginTransaction();
		ft.setTransition(FragmentTransaction.TRANSIT_FRAGMENT_OPEN);
		ft.replace(this.fragmentHolderId, Fragment.instantiate(this, className, this.bundle));
		ft.commitAllowingStateLoss();
	}

	/**
	 * 会先于fragment的onDestroy调用
	 */
	@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
	protected void onDestroy()
	{
		System.out.println("MyActionBarActivity.onDestroy()");
		beforeExit();
		super.onDestroy();

		int sdk = android.os.Build.VERSION.SDK_INT;
		if (sdk >= android.os.Build.VERSION_CODES.JELLY_BEAN)
		{
			actionbarLayout.getViewTreeObserver().removeOnGlobalLayoutListener(onGlobalLayoutListener);
		}
		onGlobalLayoutListener = null;
		backButton.setOnClickListener(null);

		hideRewardLoadingPopup();
		backButton = null;
		titleLabel = null;
		optionButton = null;
		editButton = null;
		writeButton = null;
		returnButton = null;
		showFriend = null;
		actionbarLayout = null;
		fragment_holder = null;
		activityProgressBar = null;
		reward_loading_layout = null;
		loading_textview = null;
	}

	private ViewTreeObserver.OnGlobalLayoutListener	onGlobalLayoutListener;
	private boolean									adjustSizeCompleted	= false;

	protected void adjustSize()
	{
		if (!ConfigManager.getInstance().scaleFontandUI)
		{
			if (backButton.getWidth() != 0 && !adjustSizeCompleted)
			{
				adjustSizeCompleted = true;
			}
			return;
		}

		if (backButton.getWidth() != 0 && !adjustSizeCompleted)
		{
			actionbarLayout.setLayoutParams(new RelativeLayout.LayoutParams((int) actionbarLayout.getWidth(),
					(int) (88 * ConfigManager.scaleRatioButton)));
			RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams((int) (88 * ConfigManager.scaleRatioButton),
					(int) (88 * ConfigManager.scaleRatioButton));
			param.setMargins(ScaleUtil.dip2px(this, -4), 0, 0, 0);
			backButton.setLayoutParams(param);

			RelativeLayout.LayoutParams param2 = new RelativeLayout.LayoutParams((int) (88 * ConfigManager.scaleRatioButton),
					(int) (88 * ConfigManager.scaleRatioButton));
			param2.setMargins(0, 0, ScaleUtil.dip2px(this, -4), 0);
			param2.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
//			optionButton.setLayoutParams(param2);

			RelativeLayout.LayoutParams param3 = new RelativeLayout.LayoutParams((int) (124 * ConfigManager.scaleRatioButton),
					(int) (48 * ConfigManager.scaleRatioButton));
			param3.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
			param3.addRule(RelativeLayout.CENTER_VERTICAL);
//			editButton.setLayoutParams(param3);
//			returnButton.setLayoutParams(param3);

			RelativeLayout.LayoutParams param4 = new RelativeLayout.LayoutParams((int) (124 * ConfigManager.scaleRatioButton),
					(int) (48 * ConfigManager.scaleRatioButton));
			param4.setMargins(0, 0, 0, 0);
			param4.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
			param4.addRule(RelativeLayout.CENTER_VERTICAL);
//			writeButton.setLayoutParams(param4);

			RelativeLayout.LayoutParams param5 = new RelativeLayout.LayoutParams((int) (88 * ConfigManager.scaleRatioButton),
					(int) (88 * ConfigManager.scaleRatioButton));
			param5.setMargins(0, 0, 0, 0);
			param5.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
			param5.addRule(RelativeLayout.CENTER_VERTICAL);
			showFriend.setLayoutParams(param5);
            showFriend.setVisibility(View.INVISIBLE);

			ScaleUtil.adjustTextSize(titleLabel, ConfigManager.scaleRatio);

			ScaleUtil.adjustTextSize(red_package_sendername, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(red_package_sendertip, ConfigManager.scaleRatio);
			// ScaleUtil.adjustTextSize(red_package_msg,
			// ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(red_package_detail, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(red_package_warning, ConfigManager.scaleRatio);

			adjustSizeCompleted = true;
		}
	}

	/**
	 * 高ppi手机的缩放修正因子
	 */
	public double getScreenCorrectionFactor()
	{
		int density = getResources().getDisplayMetrics().densityDpi;

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

	public int getToastPosY()
	{
		int[] location = { 0, 0 };
		fragment_holder.getLocationOnScreen(location);
		return location[1] + ScaleUtil.dip2px(this, 5);
	}

	// ---------------------------------------------------
	// 锁屏超过1分钟，返回后自动退出
	// ---------------------------------------------------

	public static ICocos2dxScreenLockListener	previousActivity;
	public static boolean						isReturnFromScreenLock	= false;
	private long								screenLockTime;
	/**
	 * TODO 无用变量 是否是主动触发的退出（否则可能是锁屏）
	 */
	protected boolean							isExiting				= false;
	private boolean								isScreenLocked			= false;

	@Override
	protected void onResume()
	{
		System.out.println("MyActionBarActivity onResume: " + this);

		super.onResume();
		ChatServiceController.setCurrentActivity(this);
		if (isScreenLocked)
		{
			isScreenLocked = false;
			// 锁屏返回，超时，退出聊天界面
			// 仅调用2dx的onResume
			if ((System.currentTimeMillis() - screenLockTime) > (1000 * 60))
			{
				isReturnFromScreenLock = true;
				ChatServiceController.showGameActivity(ChatServiceController.getCurrentActivity());
			}
			// 锁屏返回，未超时，不退出聊天界面
			else
			{
				if (previousActivity != null && (previousActivity instanceof ICocos2dxScreenLockListener))
				{
					previousActivity.handle2dxResume();
				}
			}
		}
		else if (!isJumpToInnerActivity())
		{
			if (previousActivity != null && (previousActivity instanceof ICocos2dxScreenLockListener))
			{
				System.out.println("MyActionBarActivity handle2dxResume() ");
				previousActivity.handle2dxResume();
			}
		}

		ChatServiceController.isNativeStarting = false;
		System.out.println("ChatServiceController.isNativeStarting:" + ChatServiceController.isNativeStarting);
	}

	private boolean isJumpToInnerActivity()
	{
		System.out.println("isJumpToInnerActivity: " + ChatServiceController.isNativeStarting + " || "
				+ ChatServiceController.isReturningToGame + " = "
				+ (ChatServiceController.isNativeStarting || ChatServiceController.isReturningToGame));
		return ChatServiceController.isNativeStarting || ChatServiceController.isReturningToGame;
	}

	@Override
	protected void onPause()
	{
		System.out.println("MyActionBarActivity onPause: " + this);

		// 当打开其它原生activity时，会出现并非锁屏的onPause，需要直接判断是否锁屏
		PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
		boolean isScreenOn = pm.isScreenOn();
		System.out.println("isScreenOn: " + isScreenOn);

		super.onPause();
		if (!isScreenOn) // !isExiting &&
		{
			// 聊天界面锁屏
			isScreenLocked = true;
			screenLockTime = System.currentTimeMillis();
		}

		// 仅调用2dx的onPause
		if (!isJumpToInnerActivity())
		{
			System.out.println("previousActivity: " + previousActivity);
			if (previousActivity != null && (previousActivity instanceof ICocos2dxScreenLockListener))
			{
				System.out.println("MyActionBarActivity handle2dxPause() ");
				previousActivity.handle2dxPause();
			}
		}
	}

	private void beforeExit()
	{
		if (fragment != null)
		{
			fragment.saveState();
		}

		isExiting = true;
		ServiceInterface.stopFlyHintTimer();
		// 极少情况fragment为null
		if (this instanceof ChatActivity && fragment != null && fragment instanceof ChatFragment)
		{
			((ChatFragment) fragment).resetChannelView();
		}
	}

	public void onBackButtonClick()
	{
		exitActivity();
	}

	public void onBackPressed()
	{
		exitActivity();
	}

	public void exitActivity()
	{
		System.out.println("MyActionBarActivity.exitActivity(): " + this);
		ServiceInterface.popActivity(this);
		if (ServiceInterface.getNativeActivityCount() == 0)
		{
			ChatServiceController.isReturningToGame = true;
		}
		else
		{
			ChatServiceController.isNativeStarting = true;
		}

		try
		{
			// 从onResume()调用时，可能在FragmentManagerImpl.checkStateLoss()出异常
			// java.lang.RuntimeException Unable to resume activity
			// {com.hcg.cok.gp/com.elex.chatservice.view.ChatActivity}:
			// java.lang.IllegalStateException: Can not perform this action
			// after onSaveInstanceState
			super.onBackPressed();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finish();
	}

	/**
	 * 因退栈被销毁的话，不会被调用
	 */
	public void finish()
	{
		super.finish();
		overridePendingTransition(R.anim.in_from_left, R.anim.out_to_right);
	}

	public void showFlyMutiReward(FlyMutiRewardInfo mutiRewardInfo)
	{
		reward_fly_layout.removeAllViews();
		reward_fly_layout.setVisibility(View.VISIBLE);
		if (mutiRewardInfo != null)
		{
			List<FlyRewardInfo> flyToolRewardArray = mutiRewardInfo.getFlyToolReward();
			createFlyNodeByType(flyToolRewardArray, 1);
			List<FlyRewardInfo> flyRewardArray = mutiRewardInfo.getFlyReward();
			createFlyNodeByType(flyRewardArray, 2);
			hideRewardLoadingPopup();
			createFlyAnimationForAllNode();
		}
	}

	public void createFlyAnimationForAllNode()
	{
		if (reward_fly_layout.getChildCount() <= 0)
			return;
		int flyToolRewardIndex = 0;
		int flyRewardIndex = 0;
		int totalFlyToolRewardNum = getChildNumByType("1");
		int totalFlyRewardNum = getChildNumByType("2");
		for (int i = 0; i < reward_fly_layout.getChildCount(); i++)
		{
			View view = reward_fly_layout.getChildAt(i);
			String tag = view.getTag().toString();
			if (tag.equals("1"))
			{
				createFlyToolAnimationForSingleNode(view, totalFlyToolRewardNum, flyToolRewardIndex, 0);
				flyToolRewardIndex++;
			}
			else if (tag.equals("2"))
			{
				createFlyToolAnimationForSingleNode(view, totalFlyRewardNum, flyRewardIndex, 1600);
				flyRewardIndex++;
			}
		}
	}

	private int getChildNumByType(String type)
	{
		int num = 0;
		for (int i = 0; i < reward_fly_layout.getChildCount(); i++)
		{
			View view = reward_fly_layout.getChildAt(i);
			String tag = view.getTag().toString();
			if (tag.equals(type))
				num++;
		}
		return num;
	}

	private Animator createflyOutAnimation(View view, int totalNum, int index, float centerY, long durationTime, long delayTime)
	{
		float scaleFactor = ((ScaleUtil.getScreenWidth() - ScaleUtil.dip2px(70)) / 4.0f - ScaleUtil.dip2px(20)) / ScaleUtil.dip2px(45);
		scaleFactor = scaleFactor > 2 ? 2 : scaleFactor;
		float cellW = ScaleUtil.dip2px(45) * scaleFactor + ScaleUtil.dip2px(20);
		float lineH = ScaleUtil.dip2px(60) * scaleFactor + ScaleUtil.dip2px(20);
		int lineCount = totalNum / 4 + 1;
		float delta = lineCount * lineH - ScaleUtil.getScreenHeight() / 2.0f;

		// LogUtil.printVariablesWithFuctionName("lineCount",lineCount,"scaleFactor",scaleFactor,"delta",delta,"ScaleUtil.getScreenHeight()",ScaleUtil.getScreenHeight());
		if (delta > 0)
		{
			centerY = -delta;
		}
		else
		{
			centerY = -ScaleUtil.dip2px(50);
		}

		int line = totalNum / 4 - index / 4;
		float endX = 0;
		if (line > 0)
		{
			endX = (index % 4 - 2) * cellW + cellW / 2;
		}
		else
		{
			if (totalNum % 4 == 1)
			{
				endX = 0;
			}
			else if (totalNum % 4 == 2)
			{
				endX = (index % 2 - 1) * cellW + cellW / 2;
			}
			else if (totalNum % 4 == 3)
			{
				endX = ((index % 3 == 0 ? 3 : index % 3) - 2) * cellW;
			}
			else
			{
				endX = (index % 4 - 2) * cellW + cellW / 2;
			}
		}
		AnimatorSet animSet = new AnimatorSet();
		Animator transAnimatorX = ObjectAnimator.ofFloat(view, "translationX", 0, endX);
		Animator transAnimatorY = ObjectAnimator.ofFloat(view, "translationY", centerY, -(line * lineH + centerY));

		if (delayTime > 0)
		{
			ViewHelper.setScaleX(view, 0);
			ViewHelper.setScaleY(view, 0);
			animSet.setStartDelay(delayTime);
		}

		Animator scaleX = ObjectAnimator.ofFloat(view, "scaleX", 0, scaleFactor);
		Animator scaleY = ObjectAnimator.ofFloat(view, "scaleY", 0, scaleFactor);
		animSet.playTogether(transAnimatorX, transAnimatorY, scaleX, scaleY);
		animSet.setDuration(durationTime);
		animSet.setInterpolator(new DecelerateInterpolator());
		return animSet;
	}

	private Animator createFlyToolAnimationForSingleNode(final View view, int totalNum, int index, int delayTime)
	{
		AnimatorSet animSet2 = new AnimatorSet();
		Animator transAnimatorX2 = ObjectAnimator.ofFloat(view, "translationX", 0);
		Animator transAnimatorY2 = ObjectAnimator.ofFloat(view, "translationY", ScaleUtil.getScreenHeight() / 2.0f);
		Animator scaleX2 = ObjectAnimator.ofFloat(view, "scaleX", 0);
		Animator scaleY2 = ObjectAnimator.ofFloat(view, "scaleY", 0);
		animSet2.playTogether(transAnimatorX2, transAnimatorY2, scaleX2, scaleY2);
		animSet2.setInterpolator(new AnticipateInterpolator(0.5f));
		animSet2.setDuration(600);
		animSet2.setStartDelay(400);

		AnimatorSet animator = new AnimatorSet();
		animator.playSequentially(createflyOutAnimation(view, totalNum, index, 0, 500, delayTime), animSet2);
		animator.start();
		animator.addListener(new AnimatorListener()
		{

			@Override
			public void onAnimationStart(Animator animation)
			{
			}

			@Override
			public void onAnimationRepeat(Animator animation)
			{
			}

			@Override
			public void onAnimationEnd(Animator animation)
			{
				reward_fly_layout.removeView(view);
				if (reward_fly_layout.getChildCount() <= 0)
					hideFlyReward();
			}

			@Override
			public void onAnimationCancel(Animator animation)
			{
			}
		});
		return animator;
	}

	private void createFlyAnimationForSingleNode(final View view, int totalNum, int index)
	{
		Animator animator = createflyOutAnimation(view, totalNum, index, 100, 1200, 0);
		animator.start();
		animator.addListener(new AnimatorListener()
		{

			@Override
			public void onAnimationStart(Animator animation)
			{
			}

			@Override
			public void onAnimationRepeat(Animator animation)
			{
			}

			@Override
			public void onAnimationEnd(Animator animation)
			{
				reward_fly_layout.removeView(view);
				if (reward_fly_layout.getChildCount() <= 0)
					hideFlyReward();
			}

			@Override
			public void onAnimationCancel(Animator animation)
			{
			}
		});
	}

	public void createFlyNodeByType(List<FlyRewardInfo> flyRewardArray, int type)
	{
		if (flyRewardArray != null)
		{
			for (int i = 0; i < flyRewardArray.size(); i++)
			{
				FlyRewardInfo rewardInfo = flyRewardArray.get(i);
				createSingleRewardNode(rewardInfo, "" + type);
			}
		}
	}

	public void hideFlyReward()
	{
		reward_fly_layout.removeAllViews();
		reward_fly_layout.setVisibility(View.GONE);
	}

	private void createSingleRewardNode(FlyRewardInfo rewardInfo, String type)
	{
		if (rewardInfo == null)
			return;
		String pic = rewardInfo.getItemPic();
		pic = pic.toLowerCase();
		if (StringUtils.isNotEmpty(pic) && pic.endsWith(".png"))
		{
			pic = pic.substring(0, pic.indexOf(".png"));
		}
		int num = rewardInfo.getItemNum();
		int picId = ResUtil.getId(this, "drawable", pic);
		if (picId == 0)
		{
			picId = R.drawable.no_iconflag;
			if (picId == 0)
				return;
		}
		View view = inflater.inflate(R.layout.item_reward, null);
		if (view != null)
		{
			ImageView rewardImage = (ImageView) view.findViewById(R.id.reward_img);
			TextView rewardNum = (TextView) view.findViewById(R.id.reward_num);
			if (rewardImage != null)
			{
				rewardImage.setImageDrawable(getResources().getDrawable(picId));
			}
			if (rewardNum != null)
			{
				rewardNum.setText("+" + num);
			}
			view.setTag(type);
			addViewOnParent(view);
		}
	}

	public void addViewOnParent(View view)
	{
		android.widget.FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(LayoutParams.WRAP_CONTENT,
				LayoutParams.WRAP_CONTENT, Gravity.CENTER);
		view.setLayoutParams(layoutParams);
		reward_fly_layout.addView(view);
	}
}