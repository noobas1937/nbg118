package com.elex.chatservice.model.mail.worldexplore;

import org.apache.commons.lang.StringUtils;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.LogUtil;

public class WorldExploreMailData extends MailData{
	private ExploreMailContents detail;
	
	public ExploreMailContents getDetail()
	{
		return detail;
	}

	public void setDetail(ExploreMailContents detail)
	{
		this.detail = detail;
	}

	public void parseContents() {
		super.parseContents();
		if(!getContents().equals(""))
		{
			try {
				detail=JSON.parseObject(getContents(),ExploreMailContents.class);
				
				if(detail == null || hasParseCompex)
					return;
				hasParseLocal = true;
				nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TABNAME_WORLDEXPLORE);
				boolean isExp = false;
				boolean isloss = false;
		        String exptem = "+";
		        if(detail.getReward()!=null)
		        {
		        	for(int i=0;i<detail.getReward().size();i++)
		        	{
		        		ExporeRewardParams reward = detail.getReward().get(i);
		        		if(reward!=null)
		        		{
		        			int value = reward.getValue();
		        			int type = reward.getType();
		        			if(type == 6)
		        			{
		        				isExp = true;
		        				exptem += value;
		        			}
		
		        		}
		        	}
		        }
		        
		        int numLoss = 0;
		        if(StringUtils.isNotEmpty(detail.getTrap()))
		        {
		        	String[] lossArr = detail.getTrap().split(",");
		        	for(int i=0;i<lossArr.length;i++)
		        	{
		        		String loss = lossArr[i];
		        		if(StringUtils.isNotEmpty(loss))
		        		{
		        			isloss = true;
		        			String[] itemArr = loss.split(":");
		        			if(itemArr.length == 2 && StringUtils.isNumeric(itemArr[1]))
		        			{
		        				numLoss += Integer.parseInt(itemArr[1]);
		        			}
		        		}
		        	}
		        }
		        
		        if(isExp&&isloss){
		            contentText = "EXP" + exptem + "  " + LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_105019) + " " + numLoss;
		        }else if(isExp){
		           contentText = "EXP" + exptem;
		        }else if(isloss){
		            contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_105019) + " " + numLoss;
		        }
				
		        if(contentText.length()>50)
				{
					contentText = contentText.substring(0, 50);
					contentText = contentText + "...";
				}
		        hasParseCompex = true;
			} catch (Exception e) {
				LogUtil.trackMessage("[ExploreMailContents parseContents error]: contents:"+getContents(), "", "");
			}
		}
	}
}
