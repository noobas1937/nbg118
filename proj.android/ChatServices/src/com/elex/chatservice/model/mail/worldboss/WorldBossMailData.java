package com.elex.chatservice.model.mail.worldboss;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.LogUtil;

public class WorldBossMailData extends MailData
{
	private WorldBossMailContents	detail;

	public WorldBossMailContents getDetail()
	{
		return detail;
	}

	public void setDetail(WorldBossMailContents detail)
	{
		this.detail = detail;
	}

	public void parseContents()
	{
		super.parseContents();
		if (!getContents().equals(""))
		{
			try
			{
				detail = JSON.parseObject(getContents(), WorldBossMailContents.class);
				hasMailOpend = true;
				if (detail == null || needParseByForce)
					return;

				if (detail.getAttList() != null && detail.getAttList().size() > 0)
				{
					String leaderName = "";
					for (int i = 0; i < detail.getAttList().size(); i++)
					{
						AttListParams att = detail.getAttList().get(i);
						if (att != null)
						{
							String name = att.getName();
							if (att.getLeader() == 1 && name != null)
							{
								leaderName = name;
								break;
							}
						}
					}
					contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_CONTENT_WORLDBOSS, leaderName);

					if (contentText.length() > 50)
					{
						contentText = contentText.substring(0, 50);
						contentText = contentText + "...";
					}
				}
			}
			catch (Exception e)
			{
				LogUtil.trackMessage("[WorldBossMailContents parseContents error]: contents:" + getContents(), "", "");
			}
		}
	}
}
