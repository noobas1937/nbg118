package com.elex.chatservice.model.mail.updatedata;

public class UpdateParam
{
	private String 					uid;
	private int						status;						// 是否未读，大于1则是
	private int						rewardStatus;
	private int						saveFlag;						// 0未保存,1保存,2删除保存过
	private long 					mailLastUpdateTime;
	
	public String getUid()
	{
		return uid;
	}
	public void setUid(String uid)
	{
		this.uid = uid;
	}
	public int getStatus()
	{
		return status;
	}
	public void setStatus(int status)
	{
		this.status = status;
	}
	public int getRewardStatus()
	{
		return rewardStatus;
	}
	public void setRewardStatus(int rewardStatus)
	{
		this.rewardStatus = rewardStatus;
	}
	public int getSaveFlag()
	{
		return saveFlag;
	}
	public void setSaveFlag(int saveFlag)
	{
		this.saveFlag = saveFlag;
	}
	public long getMailLastUpdateTime()
	{
		return mailLastUpdateTime;
	}
	public void setMailLastUpdateTime(long mailLastUpdateTime)
	{
		this.mailLastUpdateTime = mailLastUpdateTime;
	}
	
	
}
