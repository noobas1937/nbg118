package com.elex.chatservice.model.mail.detectreport;

public class UserInfoParams
{
	private int		level;
	private String	name;
	private long	pointId;
	private String	abbr;
	private String	pic;
	private int		picVer;
	private String	allianceName;
	private int		noDef;
	private int		count;
	private int		serverType;

	public int getNoDef()
	{
		return noDef;
	}

	public void setNoDef(int noDef)
	{
		this.noDef = noDef;
	}

	public int getCount()
	{
		return count;
	}

	public void setCount(int count)
	{
		this.count = count;
	}

	public int getLevel()
	{
		return level;
	}

	public void setLevel(int level)
	{
		this.level = level;
	}

	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	public long getPointId()
	{
		return pointId;
	}

	public void setPointId(long pointId)
	{
		this.pointId = pointId;
	}

	public String getAbbr()
	{
		return abbr;
	}

	public void setAbbr(String abbr)
	{
		this.abbr = abbr;
	}

	public String getPic()
	{
		return pic;
	}

	public void setPic(String pic)
	{
		this.pic = pic;
	}

	public String getAllianceName()
	{
		return allianceName;
	}

	public void setAllianceName(String allianceName)
	{
		this.allianceName = allianceName;
	}

	public int getPicVer()
	{
		return picVer;
	}

	public void setPicVer(int picVer)
	{
		this.picVer = picVer;
	}

	public int getServerType()
	{
		return serverType;
	}

	public void setServerType(int serverType)
	{
		this.serverType = serverType;
	}
}
