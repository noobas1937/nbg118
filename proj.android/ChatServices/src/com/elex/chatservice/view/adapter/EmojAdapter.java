package com.elex.chatservice.view.adapter;

import java.util.List;
import java.util.Map;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.SimpleAdapter;
import android.widget.TextView;

import com.elex.chatservice.R;
import com.elex.chatservice.util.emoji.EmojiUtils;

public class EmojAdapter extends SimpleAdapter
{

	public EmojAdapter(Context context, List<? extends Map<String, ?>> data, int resource, String[] from, int[] to)
	{
		super(context, data, resource, from, to);
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent)
	{
		try
		{
			convertView = super.getView(position, convertView, parent);

			if (convertView instanceof LinearLayout)
			{
				setEmojiFont((LinearLayout) convertView);
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}

		return convertView;
	}

	public void setEmojiFont(LinearLayout emojLayout)
	{
		TextView textView = (TextView) emojLayout.findViewById(R.id.emoji_image);
		EmojiUtils.setTypeface(textView);
	}
}
