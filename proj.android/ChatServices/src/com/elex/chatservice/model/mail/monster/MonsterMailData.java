package com.elex.chatservice.model.mail.monster;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang.StringUtils;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.LogUtil;

public class MonsterMailData extends MailData {
	private int unread;
	private int totalNum;
	private List<MonsterMailContents> monster;
	
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

	public List<MonsterMailContents> getMonster()
	{
		return monster;
	}

	public void setMonster(List<MonsterMailContents> monster)
	{
		this.monster = monster;
	}

	public void parseContents() {
		super.parseContents();
		if(!getContents().equals(""))
		{
			try {
				if(getStatus()==0)
					setUnread(1);
				else
					setUnread(0);
				setTotalNum(1);
				monster=new ArrayList<MonsterMailContents>();
//				System.out.println("MonsterMailData contents:"+getContents());
				MonsterMailContents detail=JSON.parseObject(getContents(),MonsterMailContents.class);
				if(detail == null)
					return;
				detail.setUid(getUid());
				long time=((long)getCreateTime())*1000;
				detail.setCreateTime(""+time);
				detail.setType(getType());
				monster.add(detail);
				if(monster!=null)
					hasParseLocal = true;
				
				if(hasParseCompex || detail==null)
					return;
				
				DefParams def = detail.getDef();
				if(def==null)
					return;
				String name = "";
				String level = "";
				if(StringUtils.isNotEmpty(def.getId()))
				{
					ChatServiceController.getInstance().host.getNameById(def.getId());
					level = ChatServiceController.getInstance().host.getPropById(def.getId(),"level"); 
				}
				name+=" Lv.";
		        name+=level;
		        contentText = name;
		        contentText += "  ";
		        
		        int monsterResult=-1;
		        if(StringUtils.isNotEmpty(detail.getStat())){
		            monsterResult = 4; //怪物消失
		        }else{
		            if(detail.getFirstKill() == 1){
		                monsterResult = 1;//首次杀
		            }else if(def.getMchp()>0){//失败
		                monsterResult = 2;//
		            }else{
		                monsterResult = 3;//
		            }
		        }
		        
		        if(monsterResult==1){
		        	contentText += LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_103758);
		        }else if(monsterResult==2){
		        	contentText += LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_105118);
		        }else if(monsterResult == 4){
		        	contentText += LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_103786);
		        }else{
		        	contentText += LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_105117);
		        }
		        if(contentText.length()>50)
				{
					contentText = contentText.substring(0, 50);
					contentText = contentText + "...";
				}
		        hasParseCompex = true;
			} catch (Exception e) {
				LogUtil.trackMessage("[MonsterMailData parseContents error]: contents:"+getContents(), "", "");
			}
		}
	}
}
