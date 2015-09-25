package com.elex.chatservice.view.listview;

import android.content.Context;
import android.graphics.Color;
import android.os.Handler;
import android.util.AttributeSet;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.MotionEvent;
import android.view.View;
import android.view.animation.AccelerateInterpolator;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;
import android.widget.Scroller;

import com.elex.chatservice.R;
import com.elex.chatservice.util.LogUtil;

public class PullDownToLoadMoreView extends FrameLayout
{
	private static int timeInterval = 400;
	private GestureDetector mGestureDetector;
	private Scroller mScroller;
	private int scrollType;
	private int bottomViewInitializeVisibility = View.INVISIBLE;
	private int topViewInitializeVisibility = View.INVISIBLE;
	private boolean hasAddTopAndBottom = false;
	private int scrollDeltaY=0;
	private int topViewHeight=0;
	private int bottomViewHeight;
	private boolean isScrollToTop = false;
	private boolean isScrollFarTop = false;
	private boolean isMoveDown = false;
	private boolean isScrollStoped = false;
	private boolean isFristTouch = true;
	private boolean isHideTopView = false;
	private boolean isCloseTopAllowRefersh = true;
	private boolean isBottomWithOutScroll = true;
	private ListViewLoadListener mListViewLoadListener;
	private View topView;
	private View bottomView;
	private Context context;
	private boolean isProgressBarShowed=false;

	private Handler mHandler = new Handler()
	{
		public void handleMessage(android.os.Message msg)
		{
			super.handleMessage(msg);
			switch (scrollType)
			{
				case 0:
					if (topView.getVisibility() != View.VISIBLE)
					{
						break;
					}
					isProgressBarShowed=true;
					
					if (mListViewLoadListener != null)
					{
						scrollDeltaY=Math.abs(getScrollY());
						mListViewLoadListener.refreshData();
					}
					System.out.println("handleMessage 1");
					
					break;
				case 1:
					if (bottomView.getVisibility() != View.VISIBLE)
					{
						break;
					}
					scrollTo(0, bottomViewHeight);
					break;
			}
			
		};
	};
	
	public void hideProgressBar()
	{
		if(isProgressBarShowed)
		{
			scrollTo(0, getPullDownHeight());
			startScroll();
			isProgressBarShowed=false;
		}
	}
	
	public PullDownToLoadMoreView(Context context)
	{
		this(context,null);
	}

	public PullDownToLoadMoreView(Context context, AttributeSet attrs)
	{
		this(context, attrs,0);
	}

	public PullDownToLoadMoreView(Context context, AttributeSet attrs, int defStyle)
	{
		super(context, attrs);
		this.mScroller = new Scroller(context, new AccelerateInterpolator());
		this.mGestureDetector = new GestureDetector(context, onGestureListener);
		this.context = context;
	}
	
	private OnGestureListener onGestureListener=new OnGestureListener() {
		
		@Override
		public boolean onSingleTapUp(MotionEvent e) {
			return false;
		}
		
		@Override
		public void onShowPress(MotionEvent e) {
			
		}
		
		@Override
		public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
			int i = -1;
			int j = 1;
			if (distanceY <= 0.0F)
			{
				isMoveDown = false;
			}
			else
			{
				isMoveDown = true;
			}
			int k;

			if (((!isMoveDown) || (!isScrollFarTop))
					&& ((isMoveDown) || (getScrollY() - topViewHeight <= 0) || (!isScrollFarTop)))
			{

				if (((isMoveDown) || (!isScrollToTop))
						&& ((!isMoveDown) || (getScrollY() - topViewHeight >= 0) || (!isScrollToTop)))
				{
					j = 0;
				}
				else
				{
					k = (int) (0.5D * distanceY);
					if (k != 0)
					{
						i = k;
					}
					else if (distanceY > 0.0F)
					{
						i = j;
					}
					if (i + getScrollY() > topViewHeight) 
					{
						i = topViewHeight - getScrollY();
					}
					
					if(getScrollY()+topViewHeight<=0 || getScrollY()>0)
						scrollBy(0, i);
					return true;
				}
			}
			else
			{
				k = (int) (0.5D * distanceY);
				if (k != 0)
				{
					i = k;
				}
				else if (distanceY > 0.0F)
				{
					i = j;
				}
				
				if ((i + getScrollY() < topViewHeight) && (!isMoveDown)) 
				{
					i = topViewHeight - getScrollY();
				}
				scrollBy(0, i);
				return true;
			}
			return false;
		}
		
		@Override
		public void onLongPress(MotionEvent e) {
			
		}
		
		@Override
		public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
			return false;
		}
		
		@Override
		public boolean onDown(MotionEvent e) {
			if (!mScroller.isFinished())
			{
				mScroller.abortAnimation();
			}
			return false;
		}
	};
	

	@Override
	protected void onLayout(boolean changed, int left, int top, int right, int bottom)
	{
		this.setBackgroundColor(Color.TRANSPARENT);
		if (!this.hasAddTopAndBottom)
		{
			View topView = inflate(this.context, R.layout.loading_view, null);
			View bottomView = inflate(this.context, R.layout.loading_view, null);
			addView(topView, 0, new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
			addView(bottomView, new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
			this.hasAddTopAndBottom = true;
		}
		int childrenCount = getChildCount();
		int index = 0;
		int topValue = 0;
		while (true)
		{
			if (index >= childrenCount)
			{
				this.topView = getChildAt(0);
				this.bottomView = getChildAt(-1 + getChildCount());
				this.topView.setVisibility(View.INVISIBLE);
				this.bottomView.setVisibility(View.INVISIBLE);
				this.topViewHeight = this.topView.getHeight();
				this.bottomViewHeight = this.bottomView.getHeight();
				if ((!this.isHideTopView) && (this.topViewHeight != 0))
				{
					this.isHideTopView = true;
					scrollTo(0, this.topViewHeight);
				}
				return;
			}
			View view = getChildAt(index);
			try
			{
				int height = view.getMeasuredHeight();
				if (view.getVisibility() != 8)
				{
					// 可能在进一步调用obtainView时出异常 NullPointerException 或 IllegalStateException 
					view.layout(0, topValue, view.getMeasuredWidth(), topValue + height);
					topValue += height;
				}
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
			index++;
		}
	}

	public final int getTopViewHeight()
	{
		return this.topViewHeight;
	}
	
	public int getPullDownHeight()
	{
		return scrollDeltaY>topViewHeight?scrollDeltaY:topViewHeight;
	}

	public final void startTopScroll()
	{
//		System.out.println("startTopScroll 0");
		if (!this.isCloseTopAllowRefersh)
		{
//			System.out.println("startTopScroll 1");
			if (this.topView.getVisibility() == View.INVISIBLE)
			{
//				System.out.println("startTopScroll 2");
				this.mScroller.startScroll(0, getScrollY(), 0, -getScrollY() + this.topViewHeight, 200);
			}
			else if (this.topView.getVisibility() == View.VISIBLE)
			{
				System.out.println("startTopScroll 3");
				this.mScroller.startScroll(0, getScrollY(), 0, -getScrollY(), 200);
			}
			this.scrollType = 0;
			this.isScrollStoped = true;
			this.isFristTouch = false;
		}
		else
		{
			this.mScroller.startScroll(0, getScrollY(), 0, -getScrollY() + this.topViewHeight, 200);
		}
		postInvalidate();
	}

	public void setListViewLoadListener(ListViewLoadListener listener)
	{
		this.mListViewLoadListener=listener;
	}

	public final void setAllowPullDownRefersh(boolean paramBoolean)
	{
		this.isCloseTopAllowRefersh = paramBoolean;
	}

	public final void setBottomViewWithoutScroll(boolean isBottomWithOutScroll)
	{
		this.isBottomWithOutScroll = isBottomWithOutScroll;
	}

	public final void setTopViewInitialize(boolean isInitialize)
	{
		this.topViewInitializeVisibility = isInitialize?View.VISIBLE:View.INVISIBLE;
		if (this.topView != null) 
		{
			this.topView.setVisibility(this.topViewInitializeVisibility);
		}
	}

	@Override
	protected void onScrollChanged(int paramInt1, int paramInt2, int paramInt3, int paramInt4)
	{
		super.onScrollChanged(paramInt1, paramInt2, paramInt3, paramInt4);
	}

	private void startScroll()
	{
//		System.out.println("startScroll 0");
		if (getScrollY() - this.topViewHeight < 0)
		{
//			System.out.println("startScroll 1");
			if (!this.isCloseTopAllowRefersh)
			{
//				System.out.println("startScroll 2");
				if (this.topView.getVisibility() == View.INVISIBLE)
				{
					System.out.println("startScroll 23");
					this.mScroller.startScroll(0, getScrollY(), 0, -getScrollY() + this.topViewHeight, 200);
				}
//				if (this.topView.getVisibility() == View.VISIBLE)
//				{
//					System.out.println("startScroll 24");
//					this.mScroller.startScroll(0, getScrollY(), 0, -getScrollY(), 200);
//				}
				this.scrollType = 0;
				this.isScrollStoped = true;
				this.isFristTouch = false;
			}
			else
			{
//				System.out.println("this.isCloseTopAllowRefersh");
				this.mScroller.startScroll(0, getScrollY(), 0, -getScrollY() + this.topViewHeight, 200);
			}
			postInvalidate();
		}
		else if (getScrollY() > this.bottomViewHeight)
		{
			if (!this.isBottomWithOutScroll)
			{
				if (this.bottomView.getVisibility() == View.INVISIBLE)
					this.mScroller.startScroll(0, getScrollY(), 0, this.bottomViewHeight - getScrollY(), 200);
				if (this.bottomView.getVisibility() == View.VISIBLE)
					this.mScroller.startScroll(0, getScrollY(), 0, this.bottomViewHeight - getScrollY() + this.bottomViewHeight, 200);
				this.scrollType = 1;
				this.isScrollStoped = true;
				this.isFristTouch = false;
			}
			else
			{
				this.mScroller.startScroll(0, getScrollY(), 0, this.bottomViewHeight - getScrollY(), 200);
			}
			postInvalidate();
		}
		this.isMoveDown = false;
	}

	@Override
	public boolean dispatchTouchEvent(MotionEvent paramMotionEvent)
	{
		boolean bool = true;
		if (this.isFristTouch)
		{
			if (this.mListViewLoadListener != null)
			{
				this.isScrollToTop = this.mListViewLoadListener.getIsListViewToTop();
				this.isScrollFarTop = this.mListViewLoadListener.getIsListViewToBottom();
			}
			else
			{
				this.isScrollToTop = false;
				this.isScrollFarTop = false;
			}
			
			if (this.topViewInitializeVisibility == View.VISIBLE)
			{
				if (!this.isCloseTopAllowRefersh)
				{
//					System.out.println("dispatchTouchEvent 00");
					this.topView.setVisibility(View.VISIBLE);
				}
				else
				{
//					System.out.println("dispatchTouchEvent 01");
					this.topView.setVisibility(View.INVISIBLE);
				}
			}
			
			if (this.bottomViewInitializeVisibility == View.VISIBLE)
			{
				if (!this.isBottomWithOutScroll)
				{
					this.bottomView.setVisibility(View.VISIBLE);
				}
				else
				{
					this.bottomView.setVisibility(View.INVISIBLE);
				}
			}
			
			if (paramMotionEvent.getAction() != MotionEvent.ACTION_UP)
			{
				if (paramMotionEvent.getAction() != MotionEvent.ACTION_CANCEL)
				{
					if (!this.mGestureDetector.onTouchEvent(paramMotionEvent))
					{
						// 可能在进一步调到obtainView后出异常
						try{
							bool = super.dispatchTouchEvent(paramMotionEvent);
						}catch(Exception e){
							LogUtil.printException(e);
						}
					}
					else
					{
						paramMotionEvent.setAction(MotionEvent.ACTION_CANCEL);
						try{
							bool = super.dispatchTouchEvent(paramMotionEvent);
						}catch(Exception e){
							LogUtil.printException(e);
						}
					}
				}
				else
				{
//					System.out.println("dispatchTouchEvent 0");
					startScroll();
				}
			}
			else
			{
//				System.out.println("dispatchTouchEvent 1");
				InputMethodManager inputManager = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE);  
				inputManager.hideSoftInputFromWindow(getWindowToken(), 0); 
				startScroll();
				try{
					bool = super.dispatchTouchEvent(paramMotionEvent);
				}catch(Exception e){
					LogUtil.printException(e);
				}
			}
		}
		return bool;

	}

	@Override
	public void computeScroll()
	{
		super.computeScroll();
		if (!this.mScroller.computeScrollOffset())
		{
			if (this.isScrollStoped)
			{
//				System.out.println("computeScroll 33");
				this.isScrollStoped = false;
				this.mHandler.sendEmptyMessageDelayed(0, timeInterval);
			}
		}
		else
		{
			scrollTo(this.mScroller.getCurrX(), this.mScroller.getCurrY());
			postInvalidate();
		}
		isFristTouch = this.mScroller.isFinished();
	}

	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		switch (event.getAction())
		{
			case MotionEvent.ACTION_UP:
				if (getScrollY() - this.topViewHeight < 0)
				{
					this.isScrollToTop = true;
				}
				if (getScrollY() > this.bottomViewHeight)
				{
					this.isScrollFarTop = true;
				}
				startScroll();
		}
		return true;
	}
}
