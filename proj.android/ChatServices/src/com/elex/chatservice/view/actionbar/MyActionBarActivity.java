package com.elex.chatservice.view.actionbar;

import android.annotation.TargetApi;
import android.app.ActionBar;
import android.content.Context;
import android.os.Build;
import android.os.Bundle;
import android.os.PowerManager;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentTransaction;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewTreeObserver;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.elex.chatservice.R;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.util.ScaleUtil;
import com.elex.chatservice.view.ChatActivity;
import com.elex.chatservice.view.ChatFragment;
import com.elex.chatservice.view.ICocos2dxScreenLockListener;

public abstract class MyActionBarActivity extends FragmentActivity
{
//	private ActionBarHelper	actionBarHelper	= ActionBarHelper.createInstance(this);
	public Button				backButton;
	public TextView				titleLabel;
	public Button				optionButton;
	public Button				editButton;
	public Button				writeButton;
	public Button				returnButton;
	public RelativeLayout		backLayout;
	protected int				fragmentHolderId;
	public ActionBarFragment	fragment;
	protected Bundle			bundle;
	public FrameLayout			fragment_holder;
	public ProgressBar			activityProgressBar;
	
//	public ActionBarHelper getActionBarHelper()
//	{
//		return actionBarHelper;
//	}

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		ChatServiceController.setCurrentActivity(this);
		ServiceInterface.pushActivity(this);
		
//		actionBarHelper.onCreate(savedInstanceState);
//	    actionBarHelper.setDisplayHomeAsUpEnabled(true);
		
		if (ConfigManager.getInstance().scaleFontandUI)
		{
			ConfigManager.calcScale(this);
		}

		// 会导致退出全屏
//		initActionBar();

//		ChatServiceController.toggleFullScreen(false, true, this);

		setContentView(ResUtil.getId(this, "layout", "cs__chat_activity"));
		
		backButton = (Button) findViewById(R.id.actionbar_backButton2);
		titleLabel = (TextView) findViewById(R.id.actionbar_titleLabel2);
		optionButton = (Button) findViewById(R.id.actionbar_optionButton2);
		editButton = (Button) findViewById(R.id.actionbar_editButton);
		writeButton = (Button) findViewById(R.id.actionbar_writeButton);
		returnButton = (Button) findViewById(R.id.actionbar_returnButton);
		backLayout = (RelativeLayout) findViewById(R.id.actionbar_backLayout2);
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
		backLayout.getViewTreeObserver().addOnGlobalLayoutListener(onGlobalLayoutListener);
		
		this.fragmentHolderId = ResUtil.getId(this, "id", "cs__chat_activity_fragment_holder");
		fragment_holder = (FrameLayout) findViewById(R.id.cs__chat_activity_fragment_holder);
		
		activityProgressBar = (ProgressBar) findViewById(R.id.activityProgressBar);
		hideProgressBar();
		
		if(fragmentClass != null) showFragment(fragmentClass.getName());
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
	//				Chat Activity相关
	// ---------------------------------------------------

	protected Class <?> fragmentClass;
	
	public void showFragment(String className)
	{
		FragmentTransaction ft=getSupportFragmentManager().beginTransaction();
		ft.setTransition(FragmentTransaction.TRANSIT_FRAGMENT_OPEN);
		ft.replace(this.fragmentHolderId, Fragment.instantiate(this, className, this.bundle));
		ft.commitAllowingStateLoss();
	}

	/**
	 * 会先于fragment的onDestroy调用
	 */
    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    protected void onDestroy() {
    	System.out.println("MyActionBarActivity.onDestroy()");
		beforeExit();
    	super.onDestroy();
//    	ChatServiceController.setCurrentActivity(null);

		int sdk = android.os.Build.VERSION.SDK_INT;
		if (sdk >= android.os.Build.VERSION_CODES.JELLY_BEAN)
		{
	        backLayout.getViewTreeObserver().removeOnGlobalLayoutListener(onGlobalLayoutListener);
		}
		onGlobalLayoutListener = null;
		backButton.setOnClickListener(null);
		
		backButton = null;
		titleLabel = null;
		optionButton = null;
		editButton = null;
		writeButton = null;
		returnButton = null;
		backLayout = null;
		fragment_holder = null;
		activityProgressBar = null;
    }
    private ViewTreeObserver.OnGlobalLayoutListener onGlobalLayoutListener;
	
	private boolean	adjustSizeCompleted	= false;

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
			// parent链： ActionBarView ActionBarContainer ActionBarOverlayLayout PhoneWindow$DecorView android.view.ViewRootImpl
			// ActionBarView不能设置高度：java.lang.IllegalStateException: ActionBarView can only be used with android:layout_height="wrap_content"
			
			// 小米上backLayout.getWidth()只有一半，800多
//			WindowManager windowManager = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
//			double screenWidth = windowManager.getDefaultDisplay().getWidth();
//			int baseBeight = backLayout.getHeight();
			
			backLayout.setLayoutParams(new RelativeLayout.LayoutParams((int) backLayout.getWidth(), 
			                                                           (int) (88 * ConfigManager.scaleRatioButton)));
			RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams((int) (88 * ConfigManager.scaleRatioButton),
			                                                                    (int) (88 * ConfigManager.scaleRatioButton));
			param.setMargins(ScaleUtil.dip2px(this, -4), 0, 0, 0);
			backButton.setLayoutParams(param);
			
			RelativeLayout.LayoutParams param2 = new RelativeLayout.LayoutParams((int) (88 * ConfigManager.scaleRatioButton),
			                                                 					(int) (88 * ConfigManager.scaleRatioButton));
			param2.setMargins(0, 0, ScaleUtil.dip2px(this, -4), 0);
			param2.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
			optionButton.setLayoutParams(param2);

			RelativeLayout.LayoutParams param3 = new RelativeLayout.LayoutParams((int) (117 * ConfigManager.scaleRatioButton),
			                                                 					(int) (65 * ConfigManager.scaleRatioButton));
			param3.setMargins(0, 0, ScaleUtil.dip2px(this, -4), 0);
			param3.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
			param3.addRule(RelativeLayout.CENTER_VERTICAL);
			editButton.setLayoutParams(param3);
			returnButton.setLayoutParams(param3);

			RelativeLayout.LayoutParams param4 = new RelativeLayout.LayoutParams((int) (124 * ConfigManager.scaleRatioButton),
			                                                 					(int) (48 * ConfigManager.scaleRatioButton));
			param4.setMargins(0, 0, 0, 0);
			param4.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
			param4.addRule(RelativeLayout.CENTER_VERTICAL);
			writeButton.setLayoutParams(param4);
			
			ScaleUtil.adjustTextSize(titleLabel, ConfigManager.scaleRatio);

			adjustSizeCompleted = true;
		}
	}

	public int getToastPosY()
	{
		int[] location = { 0, 0 };
		fragment_holder.getLocationOnScreen(location);
		return location[1] + ScaleUtil.dip2px(this, 5);
	}
	
	// ---------------------------------------------------
	//				锁屏超过1分钟，返回后自动退出
	// ---------------------------------------------------

	public static ICocos2dxScreenLockListener previousActivity;
	public static boolean isReturnFromScreenLock=false;
	private long screenLockTime;
	protected boolean isExiting = false; // 是否是主动触发的退出（否则可能是锁屏）
	private boolean isScreenLocked = false;
	
	// 主动打开或锁屏返回两种情况
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
		else if(!isJumpToInnerActivity()){
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
		System.out.println("isJumpToInnerActivity: " + ChatServiceController.isNativeStarting + " || " + ChatServiceController.isReturningToGame + " = " + (ChatServiceController.isNativeStarting || ChatServiceController.isReturningToGame));
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
		if(!isJumpToInnerActivity()){
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
		if(this instanceof ChatActivity && fragment != null && fragment instanceof ChatFragment){
			((ChatFragment)fragment).resetChannelView();
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
		if(ServiceInterface.getNativeActivityCount() == 0)
		{
			ChatServiceController.isReturningToGame = true;
		}else{
			ChatServiceController.isNativeStarting = true;
		}
		
		try{
			// 从onResume()调用时，可能在FragmentManagerImpl.checkStateLoss()出异常
			// java.lang.RuntimeException Unable to resume activity {com.nbg.gp/com.elex.chatservice.view.ChatActivity}:
			// java.lang.IllegalStateException: Can not perform this action after onSaveInstanceState
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
}