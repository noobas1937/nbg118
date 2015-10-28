package com.clash.of.publish;

import java.util.HashMap;
import java.util.Random;

import org.nbg.util.GameContext;
import org.nbg.stac.empire.ad.AdSponsorPayPublisher;
//import org.nbg.stac.empire.ad.AdTapjoyPublisher;
import org.nbg.stac.empire.common.constant.CommonConst;
import org.nbg.stac.empire.publish.AbstractPublishImpl;

/**
 * AOE美国区服务器 market全球，包括波兰和日本，中国香港台湾
 * 
 * @author anychen
 * 
 */
public class GlobalPublishImpl extends AbstractPublishImpl {
	// admob 英文广告 推广ref[备份用]
	// private static final String
	// AD_ADMOB_REF="us;age@androidmarket;admob;en_ad";
	public void initlize() {
		this.setPublish_XingCould_PayAppId("5700e79ca658055477c5dcec8bf007cc");
	}
	@Override
	public void initAdPublisher(){
		//TODO 随机接一个广告实现
		Random random=new Random();
		 
		if(random.nextBoolean()){
//			this.mAdPublisher= new AdTapjoyPublisher(GameContext.getActivityInstance());
		}else {
			this.mAdPublisher= new AdSponsorPayPublisher();
//		}else{
//			this.mAdPublisher= new AdAarkiPublisher();
		}
		
	}
	@Override
	public String getPublish_ServerIp() {
		return CommonConst.SERVER_GLOBAL;
	}

	@Override
	public int getPublish_ServerPort() {
		return 80;
	}

	@Override
	public String getPublish_ServerZone() {
		return "ChaosAge_aoead_1";
	}

	public String getPublishChannel() {
		return APP_ID_HEAD + "global.";
	}

	public String getPublishPayAppId(HashMap<String, String> mapAppId) {
		return mapAppId.get(DEFAULT_KEY);
	}

	@Override
	public String getPublish_ServerCV() {
		return "2.4.3";
	}

	@Override
	public String getPublish_Region() {
		return PUBLISH_GLOBAL;
	}

	@Override
	public String getPublish_ChannelMark() {
		return CommonConst.CHANNEL_MARKET_GLOBAL;
	}

}
