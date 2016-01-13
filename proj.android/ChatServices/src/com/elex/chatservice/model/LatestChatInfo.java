package com.elex.chatservice.model;

import org.apache.commons.lang.StringUtils;

import com.alibaba.fastjson.annotation.JSONField;

public class LatestChatInfo
{
	private String	name;
	private String	asn;
	private String	msg;
	private int		vip;
	private int		isVersionValid;
	private int		sequenceId;
	private int		createTime;
	private int		post;

	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	public String getAsn()
	{
		return asn;
	}

	public void setAsn(String asn)
	{
		this.asn = asn;
	}

	public String getMsg()
	{
		return msg;
	}

	public void setMsg(String msg)
	{
		this.msg = msg;
	}

	public int getVip()
	{
		return vip;
	}

	public void setVip(int vip)
	{
		this.vip = vip;
	}

	public int getIsVersionValid()
	{
		return isVersionValid;
	}

	public void setIsVersionValid(int isVersionValid)
	{
		this.isVersionValid = isVersionValid;
	}

	public int getCreateTime()
	{
		return createTime;
	}

	public void setCreateTime(int createTime)
	{
		this.createTime = createTime;
	}

	public int getSequenceId()
	{
		return sequenceId;
	}

	public void setSequenceId(int sequenceId)
	{
		this.sequenceId = sequenceId;
	}

	public int getPost()
	{
		return post;
	}

	public void setPost(int post)
	{
		this.post = post;
	}

	@JSONField(serialize = false)
	public void setMsgInfo(MsgItem msgItem)
	{
		this.name = msgItem.getName();
		this.asn = msgItem.getASN();
		this.msg = StringUtils.isNotEmpty(msgItem.translateMsg) ? msgItem.translateMsg : msgItem.msg;
		this.vip = msgItem.getVipLevel();
		this.isVersionValid = msgItem.isVersionInvalid() ? 0 : 1;
		this.sequenceId = msgItem.sequenceId;
		this.createTime = msgItem.createTime;
		this.post = msgItem.post;
	}
}
