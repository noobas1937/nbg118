package com.elex.chatservice.model.mail.battle;

import java.util.List;

public class BattleMailContents
{
	private boolean					isResourceShieldState;
	private int						atkPowerLost;
	private int						defPowerLost;
	private String					winner;
	private int						battleType;
	private String					reportUid;
	private int						pointType;
	private String					warPoint;

	private ArmyTotalParams			atkArmyTotal;
	private ArmyTotalParams			defArmyTotal;
	private UserParams				defUser;
	private UserParams				atkUser;
	private AllianceParams			atkAlliance;
	private AllianceParams			defAlliance;

	private List<HelpReportParams>	atkHelpReport;
	private List<HelpReportParams>	defHelpReport;
	private List<String>			atkHelper;
	private List<String>			defHelper;
	private List<GenParams>			defGen;
	private List<GenParams>			atkGen;
	private List<ArmyParams>		atkReport;
	private List<ArmyParams>		defReport;
	private List<String>			atkWarEffect;
	private List<String>			dfWarEffect;
	private List<ArmyParams>		defFortLost;
	private List<RewardParams>		reward;
	private List<TowerKillParams>	defTowerKill;
	private List<Integer>			atkGenKill;
	private List<Integer>			defGenKill;

	private int						userKill;
	private int						failTimes;
	private int						winPercent;
	private int						monsterLevel;
	private int						userScore;
	private int						allKill;
	private int						msReport;
	private int						killRound;

	private int						defBeKilledCount;

	private boolean					defProtectActivate;
	private int						level;

	private int						ckf;
	private CkfWarInfoParams		ckfWarInfo;
	private RemainResourceParams	remainResource;
	private int						serverType;
	private String					uid;
	private int						type;
	private String					createTime;

	public String getUid()
	{
		return uid;
	}

	public void setUid(String uid)
	{
		this.uid = uid;
	}

	public int getType()
	{
		return type;
	}

	public void setType(int type)
	{
		this.type = type;
	}

	public String getCreateTime()
	{
		return createTime;
	}

	public void setCreateTime(String createTime)
	{
		this.createTime = createTime;
	}

	public boolean isResourceShieldState()
	{
		return isResourceShieldState;
	}

	public void setResourceShieldState(boolean isResourceShieldState)
	{
		this.isResourceShieldState = isResourceShieldState;
	}

	public int getAtkPowerLost()
	{
		return atkPowerLost;
	}

	public void setAtkPowerLost(int atkPowerLost)
	{
		this.atkPowerLost = atkPowerLost;
	}

	public int getDefPowerLost()
	{
		return defPowerLost;
	}

	public void setDefPowerLost(int defPowerLost)
	{
		this.defPowerLost = defPowerLost;
	}

	public String getWinner()
	{
		return winner;
	}

	public void setWinner(String winner)
	{
		this.winner = winner;
	}

	public int getBattleType()
	{
		return battleType;
	}

	public void setBattleType(int battleType)
	{
		this.battleType = battleType;
	}

	public String getReportUid()
	{
		return reportUid;
	}

	public void setReportUid(String reportUid)
	{
		this.reportUid = reportUid;
	}

	public int getPointType()
	{
		return pointType;
	}

	public void setPointType(int pointType)
	{
		this.pointType = pointType;
	}

	public String getWarPoint()
	{
		return warPoint;
	}

	public void setWarPoint(String warPoint)
	{
		this.warPoint = warPoint;
	}

	public ArmyTotalParams getAtkArmyTotal()
	{
		return atkArmyTotal;
	}

	public void setAtkArmyTotal(ArmyTotalParams atkArmyTotal)
	{
		this.atkArmyTotal = atkArmyTotal;
	}

	public ArmyTotalParams getDefArmyTotal()
	{
		return defArmyTotal;
	}

	public void setDefArmyTotal(ArmyTotalParams defArmyTotal)
	{
		this.defArmyTotal = defArmyTotal;
	}

	public UserParams getDefUser()
	{
		return defUser;
	}

	public void setDefUser(UserParams defUser)
	{
		this.defUser = defUser;
	}

	public UserParams getAtkUser()
	{
		return atkUser;
	}

	public void setAtkUser(UserParams atkUser)
	{
		this.atkUser = atkUser;
	}

	public AllianceParams getAtkAlliance()
	{
		return atkAlliance;
	}

	public void setAtkAlliance(AllianceParams atkAlliance)
	{
		this.atkAlliance = atkAlliance;
	}

	public AllianceParams getDefAlliance()
	{
		return defAlliance;
	}

	public void setDefAlliance(AllianceParams defAlliance)
	{
		this.defAlliance = defAlliance;
	}

	public List<HelpReportParams> getAtkHelpReport()
	{
		return atkHelpReport;
	}

	public void setAtkHelpReport(List<HelpReportParams> atkHelpReport)
	{
		this.atkHelpReport = atkHelpReport;
	}

	public List<HelpReportParams> getDefHelpReport()
	{
		return defHelpReport;
	}

	public void setDefHelpReport(List<HelpReportParams> defHelpReport)
	{
		this.defHelpReport = defHelpReport;
	}

	public List<String> getAtkHelper()
	{
		return atkHelper;
	}

	public void setAtkHelper(List<String> atkHelper)
	{
		this.atkHelper = atkHelper;
	}

	public List<String> getDefHelper()
	{
		return defHelper;
	}

	public void setDefHelper(List<String> defHelper)
	{
		this.defHelper = defHelper;
	}

	public List<GenParams> getDefGen()
	{
		return defGen;
	}

	public void setDefGen(List<GenParams> defGen)
	{
		this.defGen = defGen;
	}

	public List<GenParams> getAtkGen()
	{
		return atkGen;
	}

	public void setAtkGen(List<GenParams> atkGen)
	{
		this.atkGen = atkGen;
	}

	public List<ArmyParams> getAtkReport()
	{
		return atkReport;
	}

	public void setAtkReport(List<ArmyParams> atkReport)
	{
		this.atkReport = atkReport;
	}

	public List<ArmyParams> getDefReport()
	{
		return defReport;
	}

	public void setDefReport(List<ArmyParams> defReport)
	{
		this.defReport = defReport;
	}

	public List<String> getAtkWarEffect()
	{
		return atkWarEffect;
	}

	public void setAtkWarEffect(List<String> atkWarEffect)
	{
		this.atkWarEffect = atkWarEffect;
	}

	public List<String> getDfWarEffect()
	{
		return dfWarEffect;
	}

	public void setDfWarEffect(List<String> dfWarEffect)
	{
		this.dfWarEffect = dfWarEffect;
	}

	public List<ArmyParams> getDefFortLost()
	{
		return defFortLost;
	}

	public void setDefFortLost(List<ArmyParams> defFortLost)
	{
		this.defFortLost = defFortLost;
	}

	public List<RewardParams> getReward()
	{
		return reward;
	}

	public void setReward(List<RewardParams> reward)
	{
		this.reward = reward;
	}

	public List<TowerKillParams> getDefTowerKill()
	{
		return defTowerKill;
	}

	public void setDefTowerKill(List<TowerKillParams> defTowerKill)
	{
		this.defTowerKill = defTowerKill;
	}

	public List<Integer> getAtkGenKill()
	{
		return atkGenKill;
	}

	public void setAtkGenKill(List<Integer> atkGenKill)
	{
		this.atkGenKill = atkGenKill;
	}

	public List<Integer> getDefGenKill()
	{
		return defGenKill;
	}

	public void setDefGenKill(List<Integer> defGenKill)
	{
		this.defGenKill = defGenKill;
	}

	public boolean isDefProtectActivate()
	{
		return defProtectActivate;
	}

	public void setDefProtectActivate(boolean defProtectActivate)
	{
		this.defProtectActivate = defProtectActivate;
	}

	public int getLevel()
	{
		return level;
	}

	public void setLevel(int level)
	{
		this.level = level;
	}

	public int getUserKill()
	{
		return userKill;
	}

	public void setUserKill(int userKill)
	{
		this.userKill = userKill;
	}

	public int getFailTimes()
	{
		return failTimes;
	}

	public void setFailTimes(int failTimes)
	{
		this.failTimes = failTimes;
	}

	public int getWinPercent()
	{
		return winPercent;
	}

	public void setWinPercent(int winPercent)
	{
		this.winPercent = winPercent;
	}

	public int getMonsterLevel()
	{
		return monsterLevel;
	}

	public void setMonsterLevel(int monsterLevel)
	{
		this.monsterLevel = monsterLevel;
	}

	public int getUserScore()
	{
		return userScore;
	}

	public void setUserScore(int userScore)
	{
		this.userScore = userScore;
	}

	public int getAllKill()
	{
		return allKill;
	}

	public void setAllKill(int allKill)
	{
		this.allKill = allKill;
	}

	public int getMsReport()
	{
		return msReport;
	}

	public void setMsReport(int msReport)
	{
		this.msReport = msReport;
	}

	public int getKillRound()
	{
		return killRound;
	}

	public void setKillRound(int killRound)
	{
		this.killRound = killRound;
	}

	public int getDefBeKilledCount()
	{
		return defBeKilledCount;
	}

	public void setDefBeKilledCount(int defBeKilledCount)
	{
		this.defBeKilledCount = defBeKilledCount;
	}

	public int getCkf()
	{
		return ckf;
	}

	public void setCkf(int ckf)
	{
		this.ckf = ckf;
	}

	public CkfWarInfoParams getCkfWarInfo()
	{
		return ckfWarInfo;
	}

	public void setCkfWarInfo(CkfWarInfoParams ckfWarInfo)
	{
		this.ckfWarInfo = ckfWarInfo;
	}

	public RemainResourceParams getRemainResource()
	{
		return remainResource;
	}

	public void setRemainResource(RemainResourceParams remainResource)
	{
		this.remainResource = remainResource;
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
