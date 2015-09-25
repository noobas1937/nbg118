package com.elex.chatservice.model.mail.inviteteleport;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.model.MailIconName;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.LogUtil;

public class InviteTeleportMailData extends MailData {
	private InviteTeleportMailContents detail;
	
	public InviteTeleportMailContents getDetail()
	{
		return detail;
	}

	public void setDetail(InviteTeleportMailContents detail)
	{
		this.detail = detail;
	}

	public void parseContents() {
		super.parseContents();
		if(!getContents().equals(""))
		{
			try {
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_INVITE_MOVE);
				detail=JSON.parseObject(getContents(),InviteTeleportMailContents.class);
				if(detail!=null)
					hasParseLocal = true;
			} catch (Exception e) {
				LogUtil.trackMessage("[InviteTeleportMailContents parseContents error]: contents:"+getContents(), "", "");
			}
		}
	}
	
	@Override
	public void setMailDealStatus()
	{
		if(detail!=null)
		{
			detail.setDeal(1);
			if(!getContents().equals("") && getContents().contains("deal"))
			{
				setContents(JSON.toJSONString(detail));
			}
		}
	}
}
