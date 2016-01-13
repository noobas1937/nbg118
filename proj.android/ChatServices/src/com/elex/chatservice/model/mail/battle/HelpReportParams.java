package com.elex.chatservice.model.mail.battle;

import java.util.List;

public class HelpReportParams
{
	private List<Integer>		genKill;
	private List<GenParams>		genInfo;
	private String				name;
	private List<ArmyParams>	armyInfo;

	public List<Integer> getGenKill()
	{
		return genKill;
	}

	public void setGenKill(List<Integer> genKill)
	{
		this.genKill = genKill;
	}

	public List<GenParams> getGenInfo()
	{
		return genInfo;
	}

	public void setGenInfo(List<GenParams> genInfo)
	{
		this.genInfo = genInfo;
	}

	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	public List<ArmyParams> getArmyInfo()
	{
		return armyInfo;
	}

	public void setArmyInfo(List<ArmyParams> armyInfo)
	{
		this.armyInfo = armyInfo;
	}

}
