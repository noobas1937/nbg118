package com.elex.chatservice.view.dialog;

import net.londatiga.android.PopupWindows;
import net.londatiga.android.QuickAction;
import android.app.Activity;
import android.content.Context;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.widget.PopupWindow.OnDismissListener;

import com.elex.chatservice.R;

public class JoinAlliancePopup extends PopupWindows implements OnDismissListener
{
	private LayoutInflater		mInflater;
	private OnDismissListener	mDismissListener;
	private boolean				mDidAction;
	private Context				mContext;

	public JoinAlliancePopup(Context context)
	{
		super(context);

		mContext = context;
		mInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

		mRootView = (ViewGroup) mInflater.inflate(R.layout.cs__first_alliance_popup, null);
		mRootView.setLayoutParams(new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));

		setContentView(mRootView);
	}

	public void show(View anchor)
	{
		preShow();

		mDidAction = false;

		int[] location = new int[2];

		anchor.getLocationOnScreen(location);

		mRootView.measure(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);

		int screenWidth = mWindowManager.getDefaultDisplay().getWidth();
		int screenHeight = mWindowManager.getDefaultDisplay().getWidth();

		mWindow.showAtLocation(anchor, Gravity.NO_GRAVITY, screenWidth / 2, screenHeight / 2);
	}

	public void setOnDismissListener(OnDismissListener listener)
	{
		setOnDismissListener(this);

		mDismissListener = listener;

		WindowManager.LayoutParams lp = ((Activity) mContext).getWindow().getAttributes();
		lp.alpha = 1f;
		((Activity) mContext).getWindow().setAttributes(lp);
	}

	@Override
	public void onDismiss()
	{
		if (!mDidAction && mDismissListener != null)
		{
			mDismissListener.onDismiss();
		}
	}

	public interface OnActionItemClickListener
	{
		public abstract void onItemClick(QuickAction source, int pos, int actionId);
	}

	public interface OnDismissListener
	{
		public abstract void onDismiss();
	}

}
