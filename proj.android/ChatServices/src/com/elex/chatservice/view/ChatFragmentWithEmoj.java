package com.elex.chatservice.view;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

import android.R.color;
import android.content.Context;
import android.graphics.Rect;
import android.os.Bundle;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
import android.text.InputType;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnFocusChangeListener;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.HorizontalScrollView;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

import com.elex.chatservice.R;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.util.ScaleUtil;
import com.elex.chatservice.util.emoji.EmojiNoMoveViewPager;
import com.elex.chatservice.util.emoji.EmojiUnicode;
import com.elex.chatservice.util.emoji.EmojiUtils;
import com.elex.chatservice.view.adapter.EmojAdapter;

public class ChatFragmentWithEmoj extends ChatFragment
{
	private ArrayList<ArrayList<String>>	emojiPages;
	private Button							emojiSwitch;											// 表情添加按钮
	private EmojiNoMoveViewPager			emojiViewPager;										// 定义Emoji表情的ViewPager
	private LinearLayout					emojiView;
	private ArrayList<GridView>				emojiList;
	// private ArrayList<ImageButton> emojiImageButtons;
	private boolean							isEmojiShowed			= false;
	private boolean							isEmojiCreated			= false;
	private boolean							emojiOn					= false;
	private boolean							isEmojiExisted			= false;
	private boolean							isSoftKeyBoardSetted	= false;
	private ArrayList<String>				emojiUnicodes;
	private boolean							waitToOpenEmojiMethod;
	private int								emojiHight				= 705;
	private LinearLayout					replyLinearLayout;
	private View							activityRootView;
	private ImageButton						emojiPage1;
	private ImageButton						emojiPage2;
	private ImageButton						emojiPage3;
	private ImageButton						emojiPage4;
	private ImageButton						emojiPage5;
	private LinearLayout					emojiDelete;
	private int								emojiPagenumber			= 5;
	View									mYourView;
	int										mVisibleHeight;
	boolean									mIsKeyboardShow;
	private LinearLayout					bottomLinearLayout;
	private int								titleBarHeight;
	private int								statusBarHeight;
	private int								contentViewTop;
	private int								softKeyBoardHeight;
	ArrayList<ImageButton>					emojiImageButtons		= new ArrayList<ImageButton>();

	public ChatFragmentWithEmoj()
	{
		try
		{

			emojiPages = new ArrayList<ArrayList<String>>();
			for (int i = 0; i < emojiPagenumber; i++)
			{
				emojiUnicodes = EmojiUnicode.getEmojiUnicode(i);
				emojiPages.add(emojiUnicodes);
			}
			// ArrayList<String> emojiCharacters = new ArrayList<String>();
			//
			// emojiCharacters.addAll(temp);
			//
			// emojiCharacters.add(BACKSPACE_CHARACTER);
			//
			// emojiPages.add(emojiCharacters);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
	{
		try
		{
			this.activity = ((ChatActivity) getActivity());
			return inflater.inflate(ResUtil.getId(this, "layout", "cs__messages_fragment_emoj"), container, false);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		return inflater.inflate(ResUtil.getId(this, "layout", "cs__messages_fragment_emoj"), container, false);
	}

	public void checkFirstGlobalLayout()
	{
		if (isJustCreated)
		{
			isJustCreated = false;
			refreshTab();
		}
		if (oldChatFragmentHeight == -1 && computeUsableHeight() > 0)
		{
			oldChatFragmentHeight = computeUsableHeight();
		}
		else if (oldChatFragmentHeight > computeUsableHeight())
		{
			oldChatFragmentHeight = computeUsableHeight();
			if (isKeyBoardFirstShowed)
			{
				isKeyBoradShowing = true;
				onKeyboradShow(oldChatFragmentHeight);
			}
			else
			{
				isKeyBoardFirstShowed = true;
			}
			if (!rememberPosition)
			{
				gotoLastLine();
			}
			else
			{
				rememberPosition = false;
			}
		}
		else if (oldChatFragmentHeight == computeUsableHeight())
		{
			if (isKeyBoradChange)
			{
				keyBoardChangeCount++;
			}
			if (keyBoardChangeCount == 2)
			{
				isKeyBoradChange = false;
			}
		}
		else if (oldChatFragmentHeight < computeUsableHeight())
		{
			keyBoardChangeCount = 0;
			isKeyBoradChange = true;
			oldChatFragmentHeight = computeUsableHeight();
			isKeyBoradShowing = false;
			isKeyBoardFirstShowed = true;
			if (!isEmojiExisted)
			{
				onKeyboradHide();
			}
			if (waitToOpenEmojiMethod)
			{
				if (emojiOn)
				{
					openEmojiMethod();
				}
			}
		}
		int usableHeightNow = computeUsableHeight();
		if (usableHeight == -1 && usableHeightNow > 0)
		{
			usableHeight = usableHeightNow;
		}
		if (usableHeight != -1 && usableHeight > usableHeightNow)
		{
			if (!isSystemBarResized)
			{
				isSystemBarResized = true;
				return;
			}
			for (int i = 0; i < getChannelViewCount(); i++)
			{
				if (getChannelView(i).chatChannel != null && getChannelView(i).chatChannel.lastPosition.x == -1
						&& getChannelView(i).messagesListView != null && getChannelView(i).getMessagesAdapter() != null)
				{
					getChannelView(i).messagesListView.setSelection(getChannelView(i).getMessagesAdapter().getCount() - 1);
				}
			}
			usableHeight = usableHeightNow;
		}
	}

	public void onViewCreated(View view, Bundle savedInstanceState)
	{
		super.onViewCreated(view, savedInstanceState);
		try
		{

			emojiSwitch = (Button) view.findViewById(ResUtil.getId(this.activity, "id", "hs__emojiSwitch"));// 添加的表情按钮
			emojiViewPager = (EmojiNoMoveViewPager) view.findViewById(ResUtil.getId(this.activity, "id", "EmojiViewPager"));
			emojiView = (LinearLayout) view.findViewById(ResUtil.getId(this.activity, "id", "EmojiView"));
			replyLinearLayout = (LinearLayout) view.findViewById(R.id.hs__sendMessageLinearLayout);
			emojiPage1 = (ImageButton) view.findViewById(R.id.emojiImage1);
			emojiPage2 = (ImageButton) view.findViewById(R.id.emojiImage2);
			emojiPage3 = (ImageButton) view.findViewById(R.id.emojiImage3);
			emojiPage4 = (ImageButton) view.findViewById(R.id.emojiImage4);
			emojiPage5 = (ImageButton) view.findViewById(R.id.emojiImage5);
			emojiDelete = (LinearLayout) view.findViewById(R.id.emoji_delete);
			emojiImageButtons.add(emojiPage1);
			emojiImageButtons.add(emojiPage2);
			emojiImageButtons.add(emojiPage3);
			emojiImageButtons.add(emojiPage4);
			emojiImageButtons.add(emojiPage5);

			// 必须用这种方法获得。不用得到的数据为0
			replyField.post(new Runnable()
			{
				public void run()
				{
					init();
				}
			});
			activityRootView = view.findViewById(R.id.emoji_frameLayout);
			bottomLinearLayout = (LinearLayout) view.findViewById(R.id.bottom_layout);
			hideSoftInputMethod(replyField);
			this.activity.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);
			// 点击输入框后，如果键盘未显示则弹出hanleClick方法弹出键盘设定表情高度；如果键盘弹出则
			replyField.setOnFocusChangeListener(new OnFocusChangeListener()
			{
				@Override
				public void onFocusChange(View v, boolean hasFocus)
				{
					if (hasFocus)
					{
						try
						{
							emojiSwitch.setVisibility(View.VISIBLE);
							if (!isKeyBoradShowing && !isEmojiExisted)
							{
								handleClick();

							}
							else if (!isKeyBoradShowing && isEmojiExisted)
							{
								showSoftKeyBoard();
							}
							isKeyBoradShowing = true;
							isEmojiShowed = false;
						}
						catch (Exception e)
						{
							e.printStackTrace();
						}
					}
				}
			});
			this.activity.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);
			replyField.setOnClickListener(new View.OnClickListener()
			{
				@Override
				public void onClick(View v)
				{
					try
					{
						emojiSwitch.setVisibility(View.VISIBLE);
						if (!isKeyBoradShowing && !isEmojiExisted)
						{
							handleClick();

						}
						else if (!isKeyBoradShowing && isEmojiExisted)
						{
							showSoftKeyBoard();
						}
						isKeyBoradShowing = true;
						isEmojiShowed = false;
					}
					catch (Exception e)
					{
						e.printStackTrace();
					}
				}
			});
			emojiSwitch.setOnClickListener(new View.OnClickListener()
			{
				@Override
				public void onClick(View v)
				{
					emojiSwitch();
					emojiOn = true;
				}
			});
			emojiDelete.setOnClickListener(new View.OnClickListener()
			{
				@Override
				public void onClick(View v)
				{
					enterBackspace();
				}
			});
			emojiPage1.setOnClickListener(new ButtonListener());
			emojiPage2.setOnClickListener(new ButtonListener());
			emojiPage3.setOnClickListener(new ButtonListener());
			emojiPage4.setOnClickListener(new ButtonListener());
			emojiPage5.setOnClickListener(new ButtonListener());
			EmojiUtils.setTypeface(replyField);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		onGlobalLayoutListener = new ViewTreeObserver.OnGlobalLayoutListener()
		{
			@Override
			public void onGlobalLayout()
			{
				checkFirstGlobalLayout();
			}
		};
		activityRootView.getViewTreeObserver().addOnGlobalLayoutListener(onGlobalLayoutListener);
		this.activity.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);
	}

	private void initGridView(int index)
	{
		if (emojiList.get(index).getAdapter() != null)
		{
			return;
		}
		EmojAdapter simpleAdapter = new EmojAdapter(this.activity, emojiCode(index), R.layout.emoji_singleexpression,
				new String[] { "emoji_image" }, new int[] { R.id.emoji_image });
		final int m = index;
		emojiList.get(index).setAdapter(simpleAdapter);
		emojiList.get(index).setOnItemClickListener(new OnItemClickListener()
		{
			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3)
			{
				emojiClick(arg0, arg1, arg2, arg3, m);
			}
		});
	}

	// public void initEmojiHorizontalScrollView(){
	// SimpleAdapter simpleAdapter2 = new SimpleAdapter(this.activity,
	// emojiHorizontalScrollView(), R.layout.emoji_scrollview, new String[] {
	// "image" },
	// new int[] { R.id.scrollview_image });
	// emojiScrollView.setAdapter(simpleAdapter2);
	// gView2.setOnItemClickListener(new OnItemClickListener()
	// {
	// @Override
	// public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long
	// arg3)
	// {
	// Bitmap bitmap = null;
	// bitmap = BitmapFactory.decodeResource(getResources(),
	// expressionImages1[arg2 % expressionImages1.length]);
	// ImageSpan imageSpan = new ImageSpan(mCon, bitmap);
	// SpannableString spannableString = new
	// SpannableString(expressionImageNames1[arg2].substring(1,
	// expressionImageNames1[arg2].length() - 1));
	// spannableString.setSpan(imageSpan, 0,
	// expressionImageNames1[arg2].length() - 2,
	// Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
	// // 编辑框设置数据
	// mEditTextContent.append(spannableString);
	// System.out.println("edit的内容 = " + spannableString);
	// }
	// });
	// }
	public void initEmojiViewPager()
	{
		emojiList = new ArrayList<GridView>();
		LayoutInflater inflater = LayoutInflater.from(activity);
		for (int i = 0; i < emojiPagenumber; i++)
		{
			GridView gView = (GridView) inflater.inflate(R.layout.emoji_grid, null);
			emojiList.add(gView);
		}
		initGridView(0);
		// 填充ViewPager的数据适配器
		PagerAdapter mPagerAdapter = new PagerAdapter()
		{
			@Override
			public boolean isViewFromObject(View arg0, Object arg1)
			{
				System.out.println("isView From");
				return arg0 == arg1;
			}

			@Override
			public int getCount()
			{
				System.out.println("getcount");
				return emojiList.size();
			}

			@Override
			public void destroyItem(View container, int position, Object object)
			{
				System.out.println("destory");
				((ViewPager) container).removeView(emojiList.get(position));
			}

			@Override
			public Object instantiateItem(View container, int position)
			{
				System.out.println("instantiateItem");
				((ViewPager) container).addView(emojiList.get(position));
				return emojiList.get(position);
			}
		};
		// emojiViewPager.onInterceptTouchEvent( );
		emojiViewPager.setAdapter(mPagerAdapter);
		// viewPager.setAdapter();
		emojiViewPager.setOnPageChangeListener(new GuidePageChangeListener());
	}

	class GuidePageChangeListener implements OnPageChangeListener
	{
		@Override
		public void onPageScrollStateChanged(int arg0)
		{
			System.out.println("页面滚动" + arg0);
		}

		@Override
		public void onPageScrolled(int arg0, float arg1, int arg2)
		{
			System.out.println("换页了" + arg0);
		}

		@Override
		public void onPageSelected(int arg0)
		{
			try
			{
				switchPage(arg0);
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}

		private void switchPage(int index)
		{
			// changePageHintStatus(index);
			initGridView(index);
		}

	}

	private void init()
	{
		try
		{
			Rect rect = new Rect();
			Window window = activity.getWindow();
			replyField.getWindowVisibleDisplayFrame(rect);
			// 状态栏的高度
			statusBarHeight = rect.top;
			// 标题栏跟状态栏的总体高度
			contentViewTop = window.findViewById(Window.ID_ANDROID_CONTENT).getTop();
			// 标题栏的高度：用上面的值减去状态栏的高度及为标题栏高度
			titleBarHeight = contentViewTop - statusBarHeight;
			System.out.println(statusBarHeight + "..." + contentViewTop + "..." + titleBarHeight);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	private List<Map<String, Object>> emojiCode(int page)
	{
		try
		{
			List<Map<String, Object>> listItems = new ArrayList<Map<String, Object>>();
			ArrayList<String> emojiCharacters = emojiPages.get(page);
			for (int i = 0; i < emojiCharacters.size(); i++)
			{

				Map<String, Object> listItem = new HashMap<String, Object>();
				listItem.put("emoji_image", EmojiUtils.unicodeToString(emojiCharacters.get(i)));
				listItems.add(listItem);

			}
			System.out.println("将emoji存入list中");
			return listItems;
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		return null;
	}

	public void keyBoardEmoji()
	{
		if (isKeyBoradShowing)
		{
			openEmojiMethod();
		}
	}

	public void emojiSwitch()
	{
		isEmojiExisted = true;
		if (!isEmojiShowed && !isKeyBoradShowing)
		{
			changeBottomMargin(emojiHight);
			setEmojiHeight(emojiHight);
			openEmojiMethod();
			setImageButton(0);
			isEmojiShowed = true;
		}
		else if (isEmojiShowed && !isKeyBoradShowing)
		{
			showSoftKeyBoard();
			isEmojiShowed = false;
			isKeyBoradShowing = true;
		}
		else if (!isEmojiShowed && isKeyBoradShowing)
		{
			openEmojiMethod();
			hideSoftKeyBoard();
			isEmojiShowed = true;
			isKeyBoradShowing = false;
		}
		else
		{
			closeEmojiMethod();
			hideSoftKeyBoard();
			onKeyboradHide();
		}
		waitToOpenEmojiMethod = !waitToOpenEmojiMethod;
		// if (isKeyBoradShowing)
		// {
		// hideSoftKeyBoard();
		// waitToOpenEmojiMethod = true;
		// isKeyBoradShowing=false;
		// }
		// else
		// {
		// showSoftKeyBoard();
		// }
		replyLinearLayout.setVisibility(View.VISIBLE);
	}

	public void openEmojiMethod()
	{
		try
		{
			waitToOpenEmojiMethod = false;
			if (isEmojiCreated == false)
			{
				initEmojiViewPager();
				isEmojiCreated = true;
			}
			// hideSoftKeyBoard();
			emojiViewPager.setVisibility(View.VISIBLE);
			emojiView.setVisibility(View.VISIBLE);
			// emojiPageSelect.setVisibility(View.VISIBLE);
			emojiSwitch.setBackgroundResource(R.drawable.emoji_switch_pressed);
			isEmojiShowed = true;
		}
		catch (Exception e)
		{
			System.out.println(e);
		}
	}

	public void closeEmojiMethod()
	{
		waitToOpenEmojiMethod = true;
		// emojiPageSelect.setVisibility(View.GONE);
		emojiViewPager.setVisibility(View.GONE);
		emojiView.setVisibility(View.GONE);
		isEmojiShowed = false;
		emojiSwitch.setBackgroundResource(R.drawable.emoji_switch_normal);
	}

	protected int computeUsableHeight()
	{
		Rect r = new Rect();
		// In effect, this tells you the available area where content can be
		// placed and remain visible to users.
		activity.getWindow().getDecorView().getWindowVisibleDisplayFrame(r);
		// fragmentLayout.getWindowVisibleDisplayFrame(r);
		return (r.bottom - r.top);
	}

	private void handleClick()
	{
		changeBottomMargin(ScaleUtil.getScreenHeight() - titleBarHeight - statusBarHeight - header.getHeight()
				- activity.actionbarLayout.getHeight() * 2 - imageView2.getHeight());
		bottomLinearLayout.setVisibility(View.INVISIBLE);
		messagesListFrameLayout.setVisibility(View.INVISIBLE);
		this.activity.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);
		showSoftKeyBoard();
	}

	private void onKeyboradHide()
	{
		if (ScaleUtil.getScreenHeight() == 0)
			return;
		changeBottomMargin(0);
	}

	private void onKeyboradShow(int oldChatFragmentHeight)
	{
		int bottom = ScaleUtil.getScreenHeight() - oldChatFragmentHeight - titleBarHeight - statusBarHeight;
		if (!isSoftKeyBoardSetted)
		{
			softKeyBoardHeight = bottom;
			isSoftKeyBoardSetted = true;
		}
		changeBottomMargin(softKeyBoardHeight);
		try
		{
			setEmojiHeight(softKeyBoardHeight);
		}
		catch (Exception e)
		{
			System.out.println(e);
		}
		bottomLinearLayout.post(new Runnable()
		{
			public void run()
			{
				bottomLinearLayout.setVisibility(View.VISIBLE);
				messagesListFrameLayout.setVisibility(View.VISIBLE);
			}
		});
	}

	private void changeBottomMargin(int margin)
	{
		RelativeLayout.LayoutParams layout = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT,
				RelativeLayout.LayoutParams.WRAP_CONTENT);
		layout.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
		layout.bottomMargin = margin;
		bottomLinearLayout.setLayoutParams(layout);
	}

	private void changeBottomMargin()
	{
		RelativeLayout.LayoutParams layout = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT,
				RelativeLayout.LayoutParams.WRAP_CONTENT);
		layout.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
		bottomLinearLayout.setLayoutParams(layout);
	}

	public void hideSoftInputMethod(EditText ed)
	{
		activity.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
		int currentVersion = android.os.Build.VERSION.SDK_INT;
		String methodName = null;
		if (currentVersion >= 16)
		{
			// 4.2
			methodName = "setShowSoftInputOnFocus";
		}
		else if (currentVersion >= 14)
		{
			// 4.0
			methodName = "setSoftInputShownOnFocus";
		}
		if (methodName == null)
		{
			ed.setInputType(InputType.TYPE_NULL);
		}
		else
		{
			Class<EditText> cls = EditText.class;
			Method setShowSoftInputOnFocus;
			try
			{
				setShowSoftInputOnFocus = cls.getMethod(methodName, boolean.class);
				setShowSoftInputOnFocus.setAccessible(true);
				setShowSoftInputOnFocus.invoke(ed, false);
			}
			catch (NoSuchMethodException e)
			{
				ed.setInputType(InputType.TYPE_NULL);
				e.printStackTrace();
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
		}
	}

	public void getViewLocation(View View, int[] a)
	{
		View.getLocationOnScreen(a);
	}

	public void setEmojiHeight(int height)
	{
		RelativeLayout.LayoutParams linearParams = (RelativeLayout.LayoutParams) emojiView.getLayoutParams(); // 取控件mGrid当前的布局参数
		linearParams.height = height;// 当控件的高强制设成50象素
		emojiView.setLayoutParams(linearParams);
		try
		{
			// LinearLayout.LayoutParams linearParams2 =
			// (LinearLayout.LayoutParams) emojiViewPager.getLayoutParams(); //
			// 取控件mGrid当前的布局参数
			// linearParams2.height = height - 50;// 当控件的高强制设成50象素
			// emojiViewPager.setLayoutParams(linearParams);
		}
		catch (Exception e)
		{
			System.out.println(e);
		}
	}

	// private void moveBottomLayout()
	// {
	// try
	// {
	// LayoutParams layoutParams = (LayoutParams)
	// bottomLinearLayout.getLayoutParams();
	// layoutParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, 0);
	// layoutParams.addRule(RelativeLayout.ABOVE, R.id.emoji_page_select);
	// bottomLinearLayout.setLayoutParams(layoutParams);
	// }
	// catch (Exception e)
	// {
	// System.out.println(e);
	// }
	// }
	// }
	// private void setEmojiViewPager()
	// {
	// FileOutputStream out = null;
	// BufferedWriter writer = null;
	// try
	// {
	// out = activity.openFileOutput("emojiViewPagerHight",
	// Context.MODE_PRIVATE);
	// writer = new BufferedWriter(new OutputStreamWriter(out));
	// writer.write(emojiHight);
	// }
	// catch (IOException e)
	// {
	// e.printStackTrace();
	// }
	// finally
	// {
	// try
	// {
	// if (writer != null)
	// {
	// writer.close();
	// }
	// }
	// catch (IOException e)
	// {
	// e.printStackTrace();
	// }
	//
	// }
	// }
	// private String getEmojiHight()
	// {
	// FileInputStream in = null;
	// BufferedReader reader = null;
	// StringBuilder content = new StringBuilder();
	// try
	// {
	// in = activity.openFileInput("emojiViewPagerHight");
	// reader = new BufferedReader(new InputStreamReader(in));
	// String line = "";
	// while ((line = reader.readLine()) != null)
	// {
	// content.append(line);
	// }
	// }
	// catch (IOException e)
	// {
	// e.printStackTrace();
	// }
	// finally
	// {
	// if (reader != null)
	// {
	// try
	// {
	// reader.close();
	// }
	// catch (IOException e)
	// {
	// e.printStackTrace();
	// }
	// }
	// else
	// {
	// return null;
	// }
	// }
	// return content.toString();
	// }
	public void emojiClick(AdapterView<?> arg0, View view, int index, long arg3, int pageIndex)
	{
		ArrayList<String> emojiCharacters = emojiPages.get(pageIndex);
		if (index == (emojiCharacters.size() - 1))
		{
			enterBackspace();
		}
		else
		{
			String str = EmojiUtils.unicodeToString(emojiCharacters.get(index));
			replyField.append(str);
			System.out.println("edit的内容 = " + str);
		}
	}

	protected void showSoftKeyBoard()
	{
		InputMethodManager imm = (InputMethodManager) activity.getSystemService(Context.INPUT_METHOD_SERVICE);
		imm.toggleSoftInput(0, InputMethodManager.HIDE_NOT_ALWAYS);
	}

	protected void onContentAreaTouched()
	{
		try
		{emojiSwitch.setVisibility(View.GONE);
			closeEmojiMethod();
			TimerTask task = new TimerTask()
			{
				public void run()
				{
					// execute the task
				}
			};
			Timer timer = new Timer();
			timer.schedule(task, 500);
			emojiOn = false;
			changeBottomMargin(0);
			hideSoftKeyBoard();
			isKeyBoradShowing = false;
			isEmojiExisted = false;
			isEmojiShowed = false;
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	private void enterBackspace()
	{
		replyField.dispatchKeyEvent(new KeyEvent(KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_DEL));
	}

	private class ButtonListener implements OnClickListener
	{
		public void onClick(View v)
		{
			int id = v.getId();
			if (id == R.id.emojiImage1)
			{
				emojiViewPager.setCurrentItem(0);
				setImageButton(0);
			}
			else if (id == R.id.emojiImage2)
			{
				emojiViewPager.setCurrentItem(1);
				setImageButton(1);
			}
			else if (id == R.id.emojiImage3)
			{
				emojiViewPager.setCurrentItem(2);
				setImageButton(2);
			}
			else if (id == R.id.emojiImage4)
			{
				emojiViewPager.setCurrentItem(3);
				setImageButton(3);
			}
			else if (id == R.id.emojiImage5)
			{
				emojiViewPager.setCurrentItem(4);
				setImageButton(4);
			}
			else
			{
			}
		}
	}

	private void setImageButton(int index)
	{
		for (int i = 0; i < emojiPagenumber; i++)
		{
			if (i != index)
			{
				emojiImageButtons.get(i).setBackgroundColor(getResources().getColor(R.color.emoji_normal));
			}
			else
			{
				emojiImageButtons.get(i).setBackgroundColor(getResources().getColor(R.color.emoji_pressed));
			}
		}
	}
}
