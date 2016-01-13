package com.elex.chatservice.model.mail.resouce;

import java.util.List;

import com.elex.chatservice.model.mail.battle.RewardParams;

public class ResourceMailContents
{

	private String				uid;
	private int					level;
	private List<RewardParams>	reward;
	private List<DropParams>	drop;
	private int					pointId;
	private String				createTime;

	public int getLevel()
	{
		return level;
	}

	public void setLevel(int level)
	{
		this.level = level;
	}

	public List<RewardParams> getReward()
	{
		return reward;
	}

	public void setReward(List<RewardParams> reward)
	{
		this.reward = reward;
	}

	public List<DropParams> getDrop()
	{
		return drop;
	}

	public void setDrop(List<DropParams> drop)
	{
		this.drop = drop;
	}

	public int getPointId()
	{
		return pointId;
	}

	public void setPointId(int pointId)
	{
		this.pointId = pointId;
	}

	public String getUid()
	{
		return uid;
	}

	public void setUid(String uid)
	{
		this.uid = uid;
	}

	public String getCreateTime()
	{
		return createTime;
	}

	public void setCreateTime(String createTime)
	{
		this.createTime = createTime;
	}

}
