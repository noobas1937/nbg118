package com.elex.chatservice.model.mail.resouce;

import java.util.ArrayList;
import java.util.List;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.model.mail.battle.RewardParams;
import com.elex.chatservice.util.LogUtil;

public class ResourceMailData extends MailData {
	private int unread;
	private int totalNum;
	private List<ResourceMailContents> collect;

	public int getUnread()
	{
		return unread;
	}

	public void setUnread(int unread)
	{
		this.unread = unread;
	}

	public int getTotalNum()
	{
		return totalNum;
	}

	public void setTotalNum(int totalNum)
	{
		this.totalNum = totalNum;
	}

	public List<ResourceMailContents> getCollect()
	{
		return collect;
	}

	public void setCollect(List<ResourceMailContents> collect)
	{
		this.collect = collect;
	}

	public void parseContents() {
		super.parseContents();
		if(!getContents().equals(""))
		{
			try {
				collect=new ArrayList<ResourceMailContents>();
				ResourceMailContents detail=JSON.parseObject(getContents(),ResourceMailContents.class);
				detail.setUid(getUid());
				long time=((long)getCreateTime())*1000;
				detail.setCreateTime(""+time);
				collect.add(detail);
				if(collect!=null)
					hasParseLocal = true;
				
				if(hasParseCompex)
					return;
				if(detail == null)
				{
					contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_108896);
				}
				else
				{
					boolean resourceResult = false;
					if(detail.getReward()==null || detail.getReward().size()<=0)
					{
						resourceResult = false;
					}
					else
					{
						resourceResult = true;
					}
					
					if(resourceResult)
					{
						RewardParams reward = detail.getReward().get(0);
						if(reward!=null)
						{
							int type = reward.getT();
			                int value = reward.getV();
			                String icon = ChatServiceController.getInstance().host.getPicByType(type, value);
			                contentText = "[" + icon + "]" + " + " + value;
						}
						else
						{
							contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_108896);
						}
					}
					else
						contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_108896);
				}
				
				hasParseCompex = true;
				
			} catch (Exception e) {
				LogUtil.trackMessage("[ResourceMailContents parseContents error]: contents:"+getContents(), "", "");
			}
		}
	}
}
