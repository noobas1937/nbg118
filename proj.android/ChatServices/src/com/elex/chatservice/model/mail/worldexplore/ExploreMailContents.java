package com.elex.chatservice.model.mail.worldexplore;

import java.util.List;

public class ExploreMailContents {

//	{"point":701618,"exploredTime":3600,
//		"reward":[{"value":2964,"type":0},{"value":988,"type":3},{"value":245,"type":6}],"trap":{"107308":6,"107008":24,"107108":6,"107208":6}}
	
	private int point;
	private long exploredTime;
	private List<ExporeRewardParams> reward;
	private String trap;
	public int getPoint() {
		return point;
	}
	public void setPoint(int point) {
		this.point = point;
	}
	public long getExploredTime()
	{
		return exploredTime;
	}
	public void setExploredTime(long exploredTime)
	{
		this.exploredTime = exploredTime;
	}
	public List<ExporeRewardParams> getReward() {
		return reward;
	}
	public void setReward(List<ExporeRewardParams> reward) {
		this.reward = reward;
	}
	public String getTrap() {
		return trap;
	}
	public void setTrap(String trap) {
		this.trap = trap;
	}
	
	
}
