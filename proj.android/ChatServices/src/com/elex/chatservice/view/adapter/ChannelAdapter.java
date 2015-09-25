package com.elex.chatservice.view.adapter;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.lang.StringUtils;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.image.AsyncImageLoader;
import com.elex.chatservice.image.ImageLoaderListener;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.UserInfo;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.util.BitmapUtil;
import com.elex.chatservice.util.CombineBitmapManager;
import com.elex.chatservice.util.ImageUtil;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.view.ChannelListActivity;
import com.elex.chatservice.view.ChannelListFragment;

public class ChannelAdapter extends AbstractMailListAdapter
{
	public ChannelAdapter(ChannelListActivity context, ChannelListFragment fragment) {
		super(context, fragment);
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent)
	{
		ChatChannel channel = (ChatChannel)getItem(position);
		if(channel==null)
			return null;
		
		channel.refreshRenderData();
		convertView = super.getView(position, convertView, parent);

		CategoryViewHolder holder = (CategoryViewHolder) convertView.getTag();
		int bgColor = 0;
		if(ChatServiceController.isNewMailUIEnable && this instanceof MsgChannelAdapter) {
			bgColor = MailManager.getColorByChannelId(((MsgChannelAdapter)this).mChannelId);
		}
		holder.setContent(context, channel, true, null, channel.nameText, channel.contentText, channel.timeText, fragment.isInEditMode(), position, bgColor);

		setIcon(channel, holder.item_icon);
		refreshMenu();
		
		return convertView;
	}

	private ConcurrentHashMap<String, Bitmap> chatroomHeadImages;
	private int customPicLoadingCnt;
	private boolean chatroomHeadImagesLoading = false;
	
	private synchronized void setIcon(final ChatChannel channel, final ImageView imageView)
	{
		if(channel.channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			if(chatroomHeadImagesLoading) return;
			
			if(isChatroomHeadPicExist(channel.channelID) && !channel.isMemberUidChanged)
			{
				String fileName = getChatroomHeadPicPath() + getChatroomHeadPicFile(channel.channelID);
				Bitmap bitmap=AsyncImageLoader.loadBitmapFromStoreSync(fileName);

				ImageUtil.setImageOnUiThread(context, imageView, bitmap);
				return;
			}
			
			chatroomHeadImages = new ConcurrentHashMap<String, Bitmap>();
			customPicLoadingCnt = 0;
			chatroomHeadImagesLoading = true;
			
			ArrayList<UserInfo> users = new ArrayList<UserInfo>();
			for (int i = 0; i < channel.memberUidArray.size(); i++)
			{
				UserInfo user = UserManager.getInstance().getUser(channel.memberUidArray.get(i));
				if(user != null){
					users.add(user);
				}
				if(users.size() >= 9) break;
			}
			
			for (int i = 0; i < users.size(); i++)
			{
				UserInfo user = users.get(i);

				chatroomHeadImages.put(user.uid,
						BitmapFactory.decodeResource(context.getResources(), ImageUtil.getHeadResId(context, user.headPic)));
				
				if(user.isCustomHeadImage())
				{
					customPicLoadingCnt++;
					System.out.println("customPicLoadingCnt++ : " + customPicLoadingCnt);
					ImageUtil.getCustomHeadImage(user, new ImageLoaderListener()
					{
						@Override
						public void onImageLoaded(String uid, final Bitmap bitmap)
						{
							onCustomImageLoaded(channel, uid, bitmap, imageView);
						}
					});
				}
			}
			if(customPicLoadingCnt == 0)
			{
				generateCombinePic(channel, imageView);
			}
		}else{
			UserInfo user = null;
			if(channel.channelType == DBDefinition.CHANNEL_TYPE_USER)
			{
				user = channel.channelShowUserInfo;
			}
			else
			{
				if(channel.showItem!=null)
					user = channel.showItem.getUser();
			}
			ImageUtil.setHeadImage(context, channel.channelIcon, imageView, user);
		}
	}
	
	private synchronized void onCustomImageLoaded(ChatChannel channel, String uid, final Bitmap bitmap, ImageView imageView)
	{
		if (bitmap != null)
		{
			chatroomHeadImages.put(uid, bitmap);
		}
		System.out.println("customPicLoadingCnt-- : " + customPicLoadingCnt);
		customPicLoadingCnt--;
		if(customPicLoadingCnt == 0)
		{
			generateCombinePic(channel, imageView);
		}
	}
	
	private void generateCombinePic(ChatChannel channel, ImageView imageView)
	{
		chatroomHeadImagesLoading = false;

		ArrayList<Bitmap> bitmaps = new ArrayList<Bitmap>();
		Set<String> keySet = chatroomHeadImages.keySet();
		for (String key : keySet)
		{
			if (StringUtils.isNotEmpty(key) && chatroomHeadImages.get(key) != null)
			{
				bitmaps.add(chatroomHeadImages.get(key));
			}
		}
		
		Bitmap bitmap = CombineBitmapManager.getInstance().getCombinedBitmap(bitmaps);
		try
		{
			if (bitmap != null && StringUtils.isNotEmpty(channel.channelID) && getChatroomHeadPicPath() != null)
			{
				BitmapUtil.saveMyBitmap(bitmap, getChatroomHeadPicPath(), getChatroomHeadPicFile(channel.channelID));

				if (channel.isMemberUidChanged)
				{
					channel.isMemberUidChanged = false;
					String fileName = getChatroomHeadPicPath() + getChatroomHeadPicFile(channel.channelID);
					AsyncImageLoader.removeMemoryCache(fileName);
				}
			}
		}
		catch (IOException e)
		{
			LogUtil.printException(e);
		}
		
		ImageUtil.setImageOnUiThread(context, imageView, bitmap);
	}
	
	public String getChatroomHeadPicPath()
	{
		if(context == null) return null;
		
		if (!Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
			String path = context.getFilesDir().getAbsolutePath();
			return path + "/head/chatroom/";
		}else{
			// 会发生NullPointerException 
			return Environment.getExternalStorageDirectory() + "/data/data/" + context.getPackageName() + "/head/chatroom/";
		}
	}
	
	public String getChatroomHeadPicFile(String channelId)
	{
		return channelId + ".png";
	}
	
	public boolean isChatroomHeadPicExist(String channelId)
	{
		String fileName = getChatroomHeadPicPath() + getChatroomHeadPicFile(channelId);
		File file = new File(fileName);
		if(file.exists()){
			return true;
		}
		return false;
	}
}
