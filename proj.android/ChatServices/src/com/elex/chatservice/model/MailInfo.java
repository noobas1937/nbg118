package com.elex.chatservice.model;

public class MailInfo
{
    // 由JNI设置的邮件相关属性
    public String opponentUid="";
    public String mailUid="";
    public String opponentName="";
    public int type=-1;
    // 发送邮件需要的参数，后台未使用，可去掉
    public boolean isCurChannelFirstVisit = false;

    public MailInfo()
	{

	}
}
