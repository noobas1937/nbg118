package com.elex.chatservice.model.mail.resourcehelp;

import java.util.ArrayList;
import java.util.List;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.model.mail.battle.RewardParams;
import com.elex.chatservice.util.LogUtil;

public class ResourceHelpMailData extends MailData {
	private int unread;
	private int totalNum;
	private List<ResourceHelpMailContents> collect;

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

	public List<ResourceHelpMailContents> getCollect()
	{
		return collect;
	}

	public void setCollect(List<ResourceHelpMailContents> collect)
	{
		this.collect = collect;
	}

	public void parseContents() {
		super.parseContents();
		if(!getContents().equals(""))
		{
			try {
				collect=new ArrayList<ResourceHelpMailContents>();
				ResourceHelpMailContents detail = JSON.parseObject(getContents(),ResourceHelpMailContents.class);
				detail.setUid(getUid());
				this.totalNum = 1;
				if(getStatus()==0)
					this.unread = 1;
				else
					this.unread = 0;
				detail.setFromName(getFromName());
				long time=((long)getCreateTime())*1000;
				detail.setCreateTime(""+time);
				collect.add(detail);
				if(collect!=null)
					hasParseLocal = true;
				
				if(detail == null || hasParseCompex)
					return;
				String tempContent = "";
				if(detail.getReward()!=null && detail.getReward().size()>0)
				{
					RewardParams helpReward =  detail.getReward().get(0);
					if(helpReward!=null)
					{
						int type = helpReward.getT();
                        int value = helpReward.getV();
                        String icon = ChatServiceController.getInstance().host.getPicByType(type, value);
                        tempContent = "["+icon+"]"+" + "+value;
					}
					
					if(detail.getReward().size()>1)
					{
						RewardParams helpReward1 =  detail.getReward().get(1);
						if(helpReward1!=null)
						{
							int type = helpReward1.getT();
	                        int value = helpReward1.getV();
	                        String icon = ChatServiceController.getInstance().host.getPicByType(type, value);
	                        if(value>0)
	                        	tempContent += (" ["+icon+"]"+" + "+value);
						}
					}
					
					if(detail.getReward().size()>2)
					{
						RewardParams helpReward2 =  detail.getReward().get(2);
						if(helpReward2!=null)
						{
							int type = helpReward2.getT();
	                        int value = helpReward2.getV();
	                        String icon = ChatServiceController.getInstance().host.getPicByType(type, value);
	                        if(value>0)
	                        	tempContent += (" ["+icon+"]"+" + "+value);
						}
					}
				}
				
				contentText = tempContent;
				hasParseCompex = true;
			} catch (Exception e) {
				LogUtil.trackMessage("[ResourceHelpMailContents parseContents error]: contents:"+getContents(), "", "");
			}
		}
	}
}
