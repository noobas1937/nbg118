package com.elex.chatservice.view.adapter;

import java.util.List;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

public class EmojiFragmentPagerAdapter extends FragmentPagerAdapter
{

	private List<Fragment> mFragments;

	public EmojiFragmentPagerAdapter(FragmentManager fm, List<Fragment> fragments)
	{
		super(fm);
		// TODO Auto-generated constructor stub
		mFragments = fragments;
	}

	@Override
	public Fragment getItem(int arg0)
	{
		// TODO Auto-generated method stub
		return mFragments.get(arg0);
	}

	@Override
	public int getCount()
	{
		// TODO Auto-generated method stub
		return mFragments.size();
	}

}