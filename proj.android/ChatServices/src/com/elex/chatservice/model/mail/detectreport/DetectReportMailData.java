package com.elex.chatservice.model.mail.detectreport;

import org.apache.commons.lang.StringUtils;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.LogUtil;

public class DetectReportMailData extends MailData {
	private DetectReportMailContents detail;
	
	public DetectReportMailContents getDetail()
	{
		return detail;
	}

	public void setDetail(DetectReportMailContents detail)
	{
		this.detail = detail;
	}

	public void parseContents() {
		super.parseContents();
		if(!getContents().equals(""))
		{
			if(getContents().equals("114005"))
			{
				nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_114102);
				contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_114005);
				detail = new DetectReportMailContents();
				detail.setContents(getContents());
			}
			else
			{
				try {
					detail=JSON.parseObject(getContents(),DetectReportMailContents.class);
					
					if(hasParseCompex || detail==null)
						return;
					

		            switch (detail.getPointType()) {
		                case MailManager.Throne:{ //王座
		                	nameText=LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_PALACE);
		                    break;
		                }
		                case MailManager.Trebuchet:{//投石机
		                	nameText=LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_CATAPULT);
		                    break;
		                }
		                case MailManager.Tile_allianceArea:{
		                	nameText=LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_CASTLE_SUCESS);
		                    break;
		                }
		                case MailManager.tile_banner:{
		                	nameText=LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_BANNER_SUCESS);
		                    break;
		                }
		                default:{
		                	nameText =LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_SUCESS);
		                    break;
		                }
		            }
		            contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_SUCESS);
		            UserInfoParams user=detail.getUser();
		            if(user!=null)
		            {
		            	if(user.getNoDef() == 1)
		            	{
		            		String nameStr="";
			            	String asn=user.getAbbr();
			            	if(StringUtils.isNotEmpty(asn))
			            		nameStr+="("+asn+")";
			            	String name=user.getName();
			            	if(StringUtils.isNotEmpty(name))
			            	{
			            		nameStr+=name;
			            	}
			            	else
			            	{
			            		if (detail.getPointType() == MailManager.Tile_allianceArea) {
			            			if(user.getCount()>0)
				            			nameStr+=LanguageManager.getLangByKey(LanguageKeys.MAIL_ALLIANCE_CASTLE,""+user.getCount()) ;
				            		else
				            			nameStr+=LanguageManager.getLangByKey(LanguageKeys.MAIL_ALLIANCE_CASTLE,"1") ;
		                        }
		                        else if (detail.getPointType() == MailManager.tile_banner) {
		                            nameStr+=LanguageManager.getLangByKey(LanguageKeys.MAIL_BANNER);
		                        }
			            	}
			            	contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_INFO,nameStr) ;
		            	}
		            	else
			            {
			            	String nameStr=user.getName();
			            	String asn=user.getAbbr();
			            	if(StringUtils.isNotEmpty(asn))
			            		nameStr="("+asn+")"+nameStr;
			            	contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_DETECT_INFO,nameStr) ;
			            }
		            }
		            
		            if(contentText.length()>50)
		    		{
		    			contentText = contentText.substring(0, 50);
		    			contentText = contentText + "...";
		    		}
		            hasParseCompex = true;
				} catch (Exception e) {
					LogUtil.trackMessage("[DetectReportMailContents parseContents error]: contents:"+getContents(), "", "");
				}
			}
			
		}
		
	}
}
