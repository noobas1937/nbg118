package com.elex.chatservice.view.dialog;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.elex.chatservice.R;
import com.elex.chatservice.util.ResUtil;

/**
 * TODO 删除无用类
 */
public class YesOrNoDialog extends Activity
{
	private LinearLayout	layout;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.cs__confirm_dialog);
		layout = ((LinearLayout) findViewById(ResUtil.getId(this, "id", "exit_layout")));

		layout.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				Toast.makeText(getApplicationContext(), "提示：点击窗口外部关闭窗口！", Toast.LENGTH_SHORT).show();
			}
		});
	}

	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		finish();
		return true;
	}

	public void exitbutton1(View v)
	{
		this.finish();
	}

	public void exitbutton0(View v)
	{
		this.finish();
	}
}
