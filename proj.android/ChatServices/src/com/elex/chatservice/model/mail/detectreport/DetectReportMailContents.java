package com.elex.chatservice.model.mail.detectreport;

import java.util.List;

public class DetectReportMailContents
{

	private List<ReinDetailParams>				rein_detail;
	private List<TowerParams>					tower;
	private DefenceParams						defence;
	private String								name;
	private ResourceParams						resource;
	private long								pointId;
	private List<List<ReinAboutDetailParams>>	rein_about_detail;
	private ReinAboutParams						rein_about;
	private FortParams							fort;
	private UserInfoParams						user;
	private List<ScienceParams>					science;
	private int									pointType;
	private String								contents;
	private List<String>						ability;
	private int									ckf;

	public ReinAboutParams getRein_about()
	{
		return rein_about;
	}

	public void setRein_about(ReinAboutParams rein_about)
	{
		this.rein_about = rein_about;
	}

	public List<ReinDetailParams> getRein_detail()
	{
		return rein_detail;
	}

	public void setRein_detail(List<ReinDetailParams> rein_detail)
	{
		this.rein_detail = rein_detail;
	}

	public List<TowerParams> getTower()
	{
		return tower;
	}

	public void setTower(List<TowerParams> tower)
	{
		this.tower = tower;
	}

	public DefenceParams getDefence()
	{
		return defence;
	}

	public void setDefence(DefenceParams defence)
	{
		this.defence = defence;
	}

	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	public ResourceParams getResource()
	{
		return resource;
	}

	public void setResource(ResourceParams resource)
	{
		this.resource = resource;
	}

	public long getPointId()
	{
		return pointId;
	}

	public void setPointId(long pointId)
	{
		this.pointId = pointId;
	}

	public List<List<ReinAboutDetailParams>> getRein_about_detail()
	{
		return rein_about_detail;
	}

	public void setRein_about_detail(List<List<ReinAboutDetailParams>> rein_about_detail)
	{
		this.rein_about_detail = rein_about_detail;
	}

	public FortParams getFort()
	{
		return fort;
	}

	public void setFort(FortParams fort)
	{
		this.fort = fort;
	}

	public UserInfoParams getUser()
	{
		return user;
	}

	public void setUser(UserInfoParams user)
	{
		this.user = user;
	}

	public List<ScienceParams> getScience()
	{
		return science;
	}

	public void setScience(List<ScienceParams> science)
	{
		this.science = science;
	}

	public int getPointType()
	{
		return pointType;
	}

	public void setPointType(int pointType)
	{
		this.pointType = pointType;
	}

	public String getContents()
	{
		return contents;
	}

	public void setContents(String contents)
	{
		this.contents = contents;
	}

	public List<String> getAbility()
	{
		return ability;
	}

	public void setAbility(List<String> ability)
	{
		this.ability = ability;
	}

	public int getCkf()
	{
		return ckf;
	}

	public void setCkf(int ckf)
	{
		this.ckf = ckf;
	}

}
