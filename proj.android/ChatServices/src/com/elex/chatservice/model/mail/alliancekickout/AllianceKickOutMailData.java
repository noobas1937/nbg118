package com.elex.chatservice.model.mail.alliancekickout;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.model.MailIconName;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.LogUtil;

public class AllianceKickOutMailData extends MailData {
	private AllianceKickOutMailContents detail;

	public AllianceKickOutMailContents getDetail()
	{
		return detail;
	}

	public void setDetail(AllianceKickOutMailContents detail)
	{
		this.detail = detail;
	}

	public void parseContents() {
		super.parseContents();
		if(!getContents().equals(""))
		{
			try {
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_INVITE_KICKEDOUT);
				detail=JSON.parseObject(getContents(),AllianceKickOutMailContents.class);
				if(detail!=null)
					hasParseLocal = true;
			} catch (Exception e) {
				LogUtil.trackMessage("[AllianceKickOutMailContents parseContents error]: contents:"+getContents(), "", "");
			}
		}
	}
}
