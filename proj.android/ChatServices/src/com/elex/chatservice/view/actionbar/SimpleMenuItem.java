package com.elex.chatservice.view.actionbar;

import android.content.Intent;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.support.v4.view.MenuItemCompat;
import android.support.v4.view.MenuItemCompat.OnActionExpandListener;
import android.view.ActionProvider;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.MenuItem;
import android.view.SubMenu;
import android.view.View;

public class SimpleMenuItem implements MenuItem
{
	private SimpleMenu										mMenu;
	private final int										mItemId;
	private final int										mOrder;
	private CharSequence									mTitle;
	private CharSequence									mTitleCondensed;
	private Drawable										mIconDrawable;
	private int												mIconResId;
	private boolean											mVisible;
	private View											mActionView					= null;
	private MenuItemCompat.OnActionExpandListener			mOnActionExpandListener		= null;
	private boolean											mActionViewExpanded;
	private SimpleMenuItemCompat.MenuItemChangedListener	mMenuItemChangedListener	= null;

	public SimpleMenuItem(SimpleMenu menu, int itemId, int order, CharSequence title)
	{
		this.mMenu = menu;
		this.mItemId = itemId;
		this.mOrder = order;
		this.mTitle = title;
	}

	public int getItemId()
	{
		return this.mItemId;
	}

	public int getGroupId()
	{
		return 0;
	}

	public int getOrder()
	{
		return this.mOrder;
	}

	public MenuItem setTitle(CharSequence title)
	{
		this.mTitle = title;
		return this;
	}

	public MenuItem setTitle(int titleResId)
	{
		this.mTitle = this.mMenu.getResources().getString(titleResId);
		return this;
	}

	public CharSequence getTitle()
	{
		return this.mTitle;
	}

	public MenuItem setTitleCondensed(CharSequence titleCondensed)
	{
		this.mTitleCondensed = titleCondensed;
		return this;
	}

	public CharSequence getTitleCondensed()
	{
		return this.mTitleCondensed;
	}

	public MenuItem setIcon(Drawable iconDrawable)
	{
		this.mIconResId = 0;
		this.mIconDrawable = iconDrawable;
		return this;
	}

	public MenuItem setIcon(int iconResId)
	{
		this.mIconDrawable = null;
		this.mIconResId = iconResId;
		return this;
	}

	public Drawable getIcon()
	{
		if (this.mIconDrawable != null)
		{
			return this.mIconDrawable;
		}

		if (this.mIconResId != 0)
		{
			return this.mMenu.getResources().getDrawable(this.mIconResId);
		}

		return null;
	}

	public MenuItem setIntent(Intent intent)
	{
		return this;
	}

	public Intent getIntent()
	{
		return null;
	}

	public MenuItem setShortcut(char c, char c2)
	{
		return this;
	}

	public MenuItem setNumericShortcut(char c)
	{
		return this;
	}

	public char getNumericShortcut()
	{
		return '\000';
	}

	public MenuItem setAlphabeticShortcut(char c)
	{
		return this;
	}

	public char getAlphabeticShortcut()
	{
		return '\000';
	}

	public MenuItem setCheckable(boolean b)
	{
		return this;
	}

	public boolean isCheckable()
	{
		return false;
	}

	public MenuItem setChecked(boolean b)
	{
		return this;
	}

	public boolean isChecked()
	{
		return false;
	}

	public MenuItem setVisible(boolean visible)
	{
		this.mVisible = visible;

		if (this.mMenuItemChangedListener != null)
		{
			this.mMenuItemChangedListener.visibilityChanged(visible);
		}

		return this;
	}

	public boolean isVisible()
	{
		return this.mVisible;
	}

	public MenuItem setEnabled(boolean b)
	{
		return this;
	}

	public boolean isEnabled()
	{
		return false;
	}

	public boolean hasSubMenu()
	{
		return false;
	}

	public SubMenu getSubMenu()
	{
		return null;
	}

	public MenuItem setOnMenuItemClickListener(MenuItem.OnMenuItemClickListener onMenuItemClickListener)
	{
		return this;
	}

	public ContextMenu.ContextMenuInfo getMenuInfo()
	{
		return null;
	}

	public void setShowAsAction(int i)
	{
	}

	public MenuItem setShowAsActionFlags(int i)
	{
		return this;
	}

	public MenuItem setActionView(View actionView)
	{
		this.mActionView = actionView;
		return this;
	}

	public MenuItem setActionView(int actionViewResId)
	{
		return this;
	}

	public View getActionView()
	{
		return this.mActionView;
	}

	public MenuItem setActionProvider(ActionProvider actionProvider)
	{
		return this;
	}

	public ActionProvider getActionProvider()
	{
		return null;
	}

	public boolean expandActionView()
	{
		this.mActionViewExpanded = true;

		return true;
	}

	public boolean collapseActionView()
	{
		this.mActionViewExpanded = false;

		return true;
	}

	public boolean isActionViewExpanded()
	{
		return this.mActionViewExpanded;
	}

	public MenuItem setOnActionExpandListener(MenuItem.OnActionExpandListener onActionExpandListener)
	{
		return this;
	}

	public MenuItem setOnActionExpandListener(MenuItemCompat.OnActionExpandListener onActionExpandListener)
	{
		this.mOnActionExpandListener = onActionExpandListener;
		return this;
	}

	public MenuItem setOnMenuItemChangedListener(SimpleMenuItemCompat.MenuItemChangedListener menuItemChangedListener)
	{
		this.mMenuItemChangedListener = menuItemChangedListener;
		return this;
	}
}