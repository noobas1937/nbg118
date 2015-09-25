package org.hcg.stac.empire.publish;

import java.util.HashMap;

import android.util.Log;

import org.hcg.stac.empire.ad.EmptyAdPublisher;
import org.hcg.stac.empire.common.constant.CommonConst;

public abstract class AbstractPublishImpl implements IPublishChannel {
	public static final int GAME_ID_AOE=0;
	public static final int GAME_ID_AOW=1;
	String mXingCloudAppId;
	String mXingCloudPayAppId;
	protected IPublishAD mAdPublisher;
	public AbstractPublishImpl() {
		initlize();
		if (CommonConst.DEBUG_STATUS) {
			Log.d(TAG, "--initlize serverid:" + DEFAULT_SERVERID);
		}
		initPublish_XingCould_AppInfo(DEFAULT_SERVERID, null);
		if (CommonConst.DEBUG_STATUS) {
			Log.d(TAG, "Def-serverIp:" + getPublish_ServerIp() + " port:"
					+ getPublish_ServerPort() + " zone:"
					+ getPublish_ServerZone());
			Log.d(TAG, "Def-AppId:" + getPublish_XingCould_AppId()
					+ " PayAppId:" + getPublish_XingCould_PayAppId());
		}
		initAdPublisher();
	}
	
	public abstract void initlize();

	protected abstract String getPublishChannel();

	public abstract String getPublishPayAppId(HashMap<String, String> mapAppId);
	
	public final int getAOE_ChannelId(){
		String channel=this.getPublish_Region();
		if(IPublishChannel.PUBLISH_360.equals(channel)){
			return IPublishChannel.AOE_VERSION_CN;
		}else if(IPublishChannel.PUBLISH_CHINA.equals(channel)){
			return IPublishChannel.AOE_VERSION_CN;
		}else if(IPublishChannel.PUBLISH_GLOBAL.equals(channel)){
			return IPublishChannel.AOE_VERSION_STANDARD;
		}else if(IPublishChannel.PUBLISH_GLOBAL_FREE.equals(channel)){
			return IPublishChannel.AOE_VERSION_FREE;
		}else if(IPublishChannel.PUBLISH_JAPANESE.equals(channel)){
			return IPublishChannel.AOE_VERSION_JP;
		}else if(IPublishChannel.PUBLISH_PL.equals(channel)){
			return IPublishChannel.AOE_VERSION_PL;
		}else if(IPublishChannel.PUBLISH_PL_MOBILE.equals(channel)){
			return IPublishChannel.AOE_VERSION_PL;
		}
		return IPublishChannel.AOE_VERSION_STANDARD;
	}
	public final IPublishAD getPublishAD(){
		return mAdPublisher;
	}
	public void initAdPublisher(){
		//空的实现，无广告接入
		mAdPublisher=new EmptyAdPublisher();
	}
	@Override
	public int getGameId(){
		return GAME_ID_AOE;
	}
	@Override
	public int getPublish_ResetPwdPort() {
		return 8088;
	}

	@Override
	public int getPublish_ServerPort2() {
		return 8080;
	}

	@Override
	public String getPublish_XingCould_PayAppId() {
		return mXingCloudPayAppId;
	}

	@Override
	public String getPublish_XingCould_AppId() {
		return mXingCloudAppId;
	}

	@Override
	public void setPublish_XingCould_PayAppId(String payAppId) {
		mXingCloudPayAppId = payAppId;
	}

	@Override
	public void setPublish_XingCould_AppId(String appId) {
		mXingCloudAppId = appId;
	}

	@Override
	public void initPublish_XingCould_AppInfo(int serverId, String payArgs) {
		mXingCloudAppId = getPublishChannel() + "s" + serverId;
		if (payArgs != null) {
			// 兼容下老版本： payArgs=55994db08136012fe36a782bcb1b6cfd
			if (!payArgs.startsWith(DEFAULT_KEY)) {
				payArgs = new StringBuffer(DEFAULT_KEY).append("=")
						.append(payArgs).toString();
			}
			HashMap<String, String> mapAppId = new HashMap<String, String>();
			// default=55994db08136012fe36a782bcb1b6cfd|onenet=ac868bf08ba8012ff1be782bcb1b6cfd
			String[] args = payArgs.split("[|]");
			for (int i = 0; i < args.length; i++) {
				String[] payInfo = args[i].split("[=]");
				mapAppId.put(payInfo[0], payInfo[1]);
			}
			mXingCloudPayAppId = getPublishPayAppId(mapAppId);
		}
		if (CommonConst.DEBUG_STATUS) {
			Log.d(TAG, serverId + "," + payArgs);
			Log.d(TAG, "appId:" + mXingCloudAppId + " pay:"
					+ mXingCloudPayAppId);
		}
	}

}
