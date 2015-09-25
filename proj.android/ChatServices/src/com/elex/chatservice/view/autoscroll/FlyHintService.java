package com.elex.chatservice.view.autoscroll;

import com.elex.chatservice.R;
import com.elex.chatservice.util.ResUtil;

import android.app.Service;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.os.IBinder;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.widget.ImageView;

public class FlyHintService extends Service {

	private String flyHintText;
	private String flyHintIcon;
	private View view;
	private boolean viewAdded = false;
	private WindowManager windowManager;
	private WindowManager.LayoutParams layoutParams;
	private AutoScroll horn_text_tip;

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	@Override
	public void onCreate() {
		super.onCreate();

		view = LayoutInflater.from(this).inflate(R.layout.cs__messages_fragment, null);

		windowManager = (WindowManager) this.getSystemService(WINDOW_SERVICE);

		layoutParams = new LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.WRAP_CONTENT, LayoutParams.TYPE_SYSTEM_ERROR,
				LayoutParams.FLAG_NOT_FOCUSABLE, PixelFormat.TRANSPARENT);
		layoutParams.gravity = Gravity.RIGHT | Gravity.TOP;
		horn_text_tip = (AutoScroll) view.findViewById(R.id.horn_text_tip);
		horn_text_tip.setText(flyHintText);
	}

	private void refresh() {
		if (viewAdded) {
			windowManager.updateViewLayout(view, layoutParams);
		} else {
			windowManager.addView(view, layoutParams);
			viewAdded = true;
			AutoScroll autoScrollTextView = (AutoScroll) view
					.findViewById(R.id.horn_text_tip);
			autoScrollTextView.setText(flyHintText);
			ImageView icon=(ImageView)view.findViewById(R.id.flyhint_icon);
			if(flyHintIcon!=null && !flyHintIcon.equals(""))
			{
				if(ResUtil.getId(this, "drawable", flyHintIcon)!=0)
					icon.setImageResource(ResUtil.getId(this, "drawable", flyHintIcon));
			}
			autoScrollTextView.init(windowManager);
			autoScrollTextView.startScroll();
		}
	}

	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		flyHintText = intent.getStringExtra("flyhinttext");
		flyHintIcon = intent.getStringExtra("flyhinticon");
		refresh();
	}

	public void removeView() {
		if (viewAdded) {
			windowManager.removeView(view);
			viewAdded = false;
		}
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		removeView();
	}
}
