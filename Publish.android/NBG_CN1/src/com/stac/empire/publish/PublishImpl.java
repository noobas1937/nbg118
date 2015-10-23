package com.stac.empire.publish;

import java.util.HashMap;

import org.nbg.util.GameContext;
import org.nbg.stac.empire.common.constant.CommonConst;
//import com.stac.empire.pay.PayFactory;
import com.stac.empire.pay.platform.PlatformPay;
import org.nbg.stac.empire.pay.platform.IPlatformPay;
import org.nbg.stac.empire.pay.platform.PayPlatformConst;
import org.nbg.stac.empire.publish.IPublishAD;
import org.nbg.stac.empire.publish.IPublishChannel;

/**
 * AOE --360简体中文
 * 
 * @author anychen
 * 
 */
public class PublishImpl implements IPublishChannel 
{
	public void initlize() {
//		this.setPublish_XingCould_PayAppId("b5357a4065b1012fbfaa782bcb1b6cfd");
	}
	@Override
	public String getPublish_ServerIp() {
		return CommonConst.SERVER_CN;
	}

	@Override
	public int getPublish_ServerPort() {
		return 80;
	}

	@Override
	public String getPublish_ServerZone() {
		return "ChaosAge_an_1";
	}
	
	String getPublishChannel() {
		return IPublishChannel.APP_ID_HEAD_CN;
	}

	String getPublishPayAppId(HashMap<String, String> mapAppId) {
		if(mapAppId.containsKey("360")){
			return  mapAppId.get("360");
		}
		return mapAppId.get(IPublishChannel.DEFAULT_KEY);
	}

	@Override
	public String getPublish_ServerCV() {
		return "2.4.3";
	}

 
	@Override
	public String getPublish_Region() {
		return IPublishChannel.PUBLISH_CN1;
	}

	@Override
	public String getPublish_ChannelMark(){
		 return CommonConst.CHANNEL_MARKET_GLOBAL;
	}

	public IPlatformPay getPlatformPay() {
		return (IPlatformPay) new PlatformPay();
	}
	public String getGooglePublishBase64Key(){
		return "0";//PayPlatformConst.GOOGLE_PUBLISH_KEY_ELEX;
	}
	@Override
	public int getGameId() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPublish_ResetPwdPort() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public int getPublish_ServerPort2() {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public String getPublish_XingCould_AppId() {
		// TODO Auto-generated method stub
		return null;
	}
	@Override
	public String getPublish_XingCould_PayAppId() {
		// TODO Auto-generated method stub
		return null;
	}
	@Override
	public void initPublish_XingCould_AppInfo(int serverId, String payArgs) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPublish_XingCould_AppId(String appId) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setPublish_XingCould_PayAppId(String payAppId) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public IPublishAD getPublishAD() {
		// TODO Auto-generated method stub
		return null;
	}
	@Override
	public int getAOE_ChannelId() {
		// TODO Auto-generated method stub
		return 0;
	}
}
