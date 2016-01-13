package com.elex.chatservice.util;

import org.apache.commons.lang.StringUtils;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Shader;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;

import com.elex.chatservice.R;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.image.AsyncImageLoader;
import com.elex.chatservice.image.ImageLoaderListener;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.UserInfo;

public class ImageUtil
{
	public static int getHeadResId(Context c, String headPic)
	{
		int defaultId = ResUtil.getId(c, "drawable", "g026");
		if (StringUtils.isEmpty(headPic))
		{
			return defaultId;
		}
		else
		{
			int idFlag = ResUtil.getId(c, "drawable", headPic);

			return idFlag != 0 ? idFlag : defaultId;
		}
	}

	/**
	 * 如果默认的g026图片不存在，则不会设置imageView
	 * 
	 * @param headPic
	 *            可为空，使用默认头像g026
	 */
	public static void setPredefinedHeadImage(final Context c, String headPic, final ImageView imageView)
	{
		final int resId = getHeadResId(c, headPic);
		try
		{
			if (resId != 0)
			{
				if (c != null && c instanceof Activity)
				{
					((Activity) c).runOnUiThread(new Runnable()
					{
						@Override
						public void run()
						{
							try
							{
								// 极少情况可能发生 Fatal Exception:
								// java.lang.OutOfMemoryError ，且没有被try捕获
								// 应该是因为有自定义头像，超出了堆内存上限
								imageView.setImageDrawable(c.getResources().getDrawable(resId));
							}
							catch (Exception e)
							{
								LogUtil.printException(e);
							}
						}
					});
				}
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	public static void setImageOnUiThread(final Context c, final ImageView imageView, final Bitmap bitmap)
	{
		if (c != null && c instanceof Activity)
		{
			((Activity) c).runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						imageView.setImageBitmap(bitmap);
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
		}
	}

	public static void setCustomHeadImage(final Context c, final ImageView imageView, final UserInfo user)
	{
		getCustomHeadImage(user, new ImageLoaderListener()
		{
			@Override
			public void onImageLoaded(final Bitmap bitmap)
			{
				String uid = (String)imageView.getTag();
				if((StringUtils.isNotEmpty(uid) && !uid.equals(user.uid)) || bitmap == null)
					return;
				setImageOnUiThread(c, imageView, bitmap);
			}
		});
	}

	public static void getCustomHeadImage(final UserInfo user, final ImageLoaderListener listener)
	{
		if (ConfigManager.enableCustomHeadImg && user != null && user.isCustomHeadImage() && listener != null)
		{
			String locaPath = user.getCustomHeadPic();
			if (AsyncImageLoader.getInstance().isCacheExistForKey(locaPath))
			{
				Bitmap bitmap = AsyncImageLoader.getInstance().loadBitmapFromCache(locaPath);
				listener.onImageLoaded(bitmap);
//				LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "bitmap from cache is not null", bitmap != null,
//						"isListFilling", ChatServiceController.isListViewFling, "user", user.userName);
			}
			else /*if (!ChatServiceController.isListViewFling)*/
			{
				if (user.isCustomHeadPicExist())
				{
					AsyncImageLoader.getInstance().loadBitmapFromStore(user.getCustomHeadPic(), new ImageLoaderListener()
					{
						@Override
						public void onImageLoaded(Bitmap bitmap)
						{
							listener.onImageLoaded(bitmap);
//							LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "bitmap from sdcard is not null",
//									bitmap != null, "isListFilling", ChatServiceController.isListViewFling, "user", user.userName);
						}
					});
				}
				else
				{
					AsyncImageLoader.getInstance().loadBitmapFromUrl(user.getCustomHeadPicUrl(), user.getCustomHeadPic(),
							new ImageLoaderListener()
							{
								@Override
								public void onImageLoaded(Bitmap bitmap)
								{
									listener.onImageLoaded(bitmap);
//									LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "bitmap from http is not null",
//											bitmap != null, "isListFilling", ChatServiceController.isListViewFling, "user", user.userName);
								}
							});
				}
			}
		}
	}

	public static void setHeadImage(Context c, String headPic, final ImageView imageView, UserInfo user)
	{
		setPredefinedHeadImage(c, headPic, imageView);

		setCustomHeadImage(c, imageView, user);
	}

	/**
	 * x匹配宽度，y按tile重复
	 */
	@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
	public static void setYRepeatingBG(Activity activity, View view, int id)
	{
		Drawable d = ImageUtil.getRepeatingBG(activity, R.drawable.mail_list_bg);
		if (d == null)
			return;
		if (Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN)
		{
			view.setBackgroundDrawable(d);
		}
		else
		{
			view.setBackground(d);
		}
	}

	public static Drawable getRepeatingBG(Activity activity, int center)
	{
		DisplayMetrics dm = new DisplayMetrics();
		activity.getWindowManager().getDefaultDisplay().getMetrics(dm);

		BitmapFactory.Options options = new BitmapFactory.Options();
		options.inScaled = true;

		Bitmap center_bmp = BitmapFactory.decodeResource(activity.getResources(), center, options);
		if (center_bmp == null)
			return null;
		center_bmp.setDensity(Bitmap.DENSITY_NONE);
		center_bmp = Bitmap.createScaledBitmap(center_bmp, dm.widthPixels, center_bmp.getHeight(), true);

		BitmapDrawable center_drawable = new BitmapDrawable(activity.getResources(), center_bmp);
		// change here setTileModeY to setTileModeX if you want to repear in X
		center_drawable.setTileModeY(Shader.TileMode.REPEAT);

		return center_drawable;
	}
}
