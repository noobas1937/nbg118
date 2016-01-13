package com.elex.chatservice.model.mail.detectreport;

import java.util.List;

import org.apache.commons.lang.StringUtils;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.MathUtil;

public class DetectReportMailData extends MailData
{
	private DetectReportMailContents	detail;

	public DetectReportMailContents getDetail()
	{
		return detail;
	}

	public void setDetail(DetectReportMailContents detail)
	{
		this.detail = detail;
	}

	public void parseContents()
	{
		super.parseContents();
		if (!getContents().equals(""))
		{
			if (getContents().equals("114005"))
			{
				nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_114102);
				contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_114005);
				detail = new DetectReportMailContents();
				detail.setContents(getContents());
				hasMailOpend = true;
			}
			else
			{
				try
				{
					detail = JSON.parseObject(getContents(), DetectReportMailContents.class);
					hasMailOpend = true;

					if (detail == null || needParseByForce)
						return;

					switch (detail.getPointType())
					{
						case MailManager.Throne:
						{ // 王座
							nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_PALACE);
							break;
						}
						case MailManager.Trebuchet:
						{// 投石机
							nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_CATAPULT);
							break;
						}
						case MailManager.Tile_allianceArea:
						{
							nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_CASTLE_SUCESS);
							break;
						}
						case MailManager.tile_banner:
						{
							nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_BANNER_SUCESS);
							break;
						}
						default:
						{
							nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_SUCESS);
							break;
						}
					}

					String nameStr = "";
					contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_SUCESS);
					UserInfoParams user = detail.getUser();
					if (user != null)
					{
						if (user.getNoDef() == 1)
						{

							String asn = user.getAbbr();
							if (StringUtils.isNotEmpty(asn))
								nameStr += "(" + asn + ")";
							String name = user.getName();
							if (StringUtils.isNotEmpty(name))
							{
								nameStr += name;
							}
							else
							{
								if (detail.getPointType() == MailManager.Tile_allianceArea)
								{
									if (user.getCount() > 0)
										nameStr += LanguageManager.getLangByKey(LanguageKeys.MAIL_ALLIANCE_CASTLE, "" + user.getCount());
									else
										nameStr += LanguageManager.getLangByKey(LanguageKeys.MAIL_ALLIANCE_CASTLE, "1");
								}
								else if (detail.getPointType() == MailManager.tile_banner)
								{
									nameStr += LanguageManager.getLangByKey(LanguageKeys.MAIL_BANNER);
								}
							}
							contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_INFO, nameStr);
						}
						else
						{
							nameStr = user.getName();
							String asn = user.getAbbr();
							if (StringUtils.isNotEmpty(asn))
								nameStr = "(" + asn + ")" + nameStr;
							contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_INFO, nameStr);
						}
					}

					int defenceNum = -1;
					if (detail.getDefence() != null)
					{
						defenceNum = detail.getDefence().getTotal();
					}
					String defenceInfo = MathUtil.getRoundFormatNumber(defenceNum);

					int resourceNum = 0;
					if (detail.getResource() != null)
					{
						resourceNum = (int) (detail.getResource().getFood() + detail.getResource().getFood_not_collected()
								+ detail.getResource().getWood() + detail.getResource().getWood_not_collected()
								+ detail.getResource().getIron() + detail.getResource().getIron_not_collected()
								+ detail.getResource().getStone() + detail.getResource().getStone_not_collected());
					}
					String resourInfo = MathUtil.getRoundFormatNumber(resourceNum);

					int helperNum = -1;

					if (detail.getRein_about_detail() != null)
					{
						helperNum = 0;
						if (detail.getRein_about() == null)
						{
							List<List<ReinAboutDetailParams>> rein_about_detailList = detail.getRein_about_detail();
							for (int i = 0; i < rein_about_detailList.size(); i++)
							{
								List<ReinAboutDetailParams> reinAboutDetailParamsList = rein_about_detailList.get(i);
								if (reinAboutDetailParamsList != null && reinAboutDetailParamsList.size() > 0)
								{
									for (int j = 0; j < reinAboutDetailParamsList.size(); j++)
									{
										ReinAboutDetailParams params = reinAboutDetailParamsList.get(j);
										if (params != null)
										{
											helperNum += params.getCount();
										}
									}
								}
							}
						}
					}
					else
					{
						if (detail.getRein_about() != null)
						{
							helperNum = detail.getRein_about().getTotal();
						}
					}

					String helperInfo = MathUtil.getRoundFormatNumber(helperNum);
					if (helperNum < 0)
						helperInfo = "NA";
					if (defenceNum < 0)
						defenceInfo = "NA";

					contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_132111, defenceInfo, resourInfo, helperInfo);

					if (contentText.length() > 50)
					{
						contentText = contentText.substring(0, 50);
						contentText = contentText + "...";
					}

					if (StringUtils.isNotEmpty(nameStr))
						contentText = nameStr + "\n" + contentText;
				}
				catch (Exception e)
				{
					LogUtil.trackMessage("[DetectReportMailContents parseContents error]: contents:" + getContents(), "", "");
				}
			}

		}

	}
}
