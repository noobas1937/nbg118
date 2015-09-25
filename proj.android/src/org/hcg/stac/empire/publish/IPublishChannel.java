package org.hcg.stac.empire.publish;

public interface IPublishChannel {
	//===== 渠道，可选值[标准版:0|波兰版:1|免费版:2|2DX版:3|ios版:100] =====
	public static final int AOE_VERSION_STANDARD=0;
	public static final int AOE_VERSION_PL=1;
	public static final int AOE_VERSION_FREE=2;
	public static final int AOE_VERSION_2DX=3;
	public static final int AOE_VERSION_JP=4;
	public static final int AOE_VERSION_CN=5;
	public static final int AOE_VERSION_IOS=100;
	//===== 渠道，可选值[标准版:0|波兰版:1|免费版:2|2DX版:3|ios版:100] =====
	
	
	public static final String TAG="AOE-Publish";
	public static final String PUBLISH_CHINA="market_cn";
	public static final String PUBLISH_JAPANESE="market_jp";
	public static final String PUBLISH_PL="market_pl";
	public static final String PUBLISH_PL_MOBILE = "pl_mobile";
	public static final String PUBLISH_GLOBAL_FREE="global_free";
	// 豌豆荚
	public static final String PUBLISH_WANDOUJIA = "wandoujia";
	// 小米
	public static final String PUBLISH_XIAOMI = "xiaomi";
	//安智
	public static final String PUBLISH_ANZHI = "anzhi";
	public static final String APP_ID_HEAD = "age@gg_en_android.";
	public static final String APP_ID_HEAD_CN = "age@337_en_android.";
	public static final String APP_ID_HEAD_JP = "age@337_en_android.";
	public static final String APP_ID_HEAD_AOW = "aow@gg_en_android.";
	public static final int DEFAULT_SERVERID=1;
	public static final String DEFAULT_KEY="default";
	
	
	/**
	 * 上面渠道待删除
	 */
	public static final String PUBLISH_COMMON="common";//国内提审版本
	public static final String PUBLISH_BETA = "beta";//测试服
	public static final String PUBLISH_GLOBAL="market_global";//谷歌
	public static final String PUBLISH_TSTORE = "tstore";//韩国t
	public static final String PUBLISH_NSTORE = "nstore";//韩国n
	public static final String PUBLISH_AMAZON = "amazon";//亚马逊
	public static final String PUBLISH_CN1 = "cn1";//国内
	public static final String PUBLISH_ELEX337="elex337";
	public static final String PUBLISH_CAFEBAZAAR="cafebazaar";//中东
	public static final String PUBLISH_MOL="mol";//泰国
	public static final String PUBLISH_TENCENT="tencent";//腾讯
	public static final String PUBLISH_MYCARD="mycard";//台湾mycard
	public static final String PUBLISH_GASH="gash";//台湾gash
	public static final String PUBLISH_MI="cn_mi";//小米
	public static final String PUBLISH_MIHY="cn_mihy";//小米互娱
	public static final String PUBLISH_UC="cn_uc";//uc
	public static final String PUBLISH_360="cn_360";//360
	public static final String PUBLISH_BAIDU="cn_baidu";//百度
	public static final String PUBLISH_XG="cn_";//西瓜


	/**
	 * 游戏ID:AOE-0，AOE-1
	 * @return
	 */
	public int getGameId();
	/**
	 * 发布渠道对应的服务器IP
	 * 
	 * @return
	 */
	public String getPublish_ServerIp();
	/**
	 * 找回密码端口
	 * @return
	 */
	public int getPublish_ResetPwdPort();
	/**
	 * 发布渠道对应的服务器端口
	 * 
	 * @return
	 */
	public int getPublish_ServerPort();
	/**
	 * 默认的登陆第二端口
	 * @return
	 */
	public int getPublish_ServerPort2();
	/**
	 * 发布渠道对应的服务器:游戏服务器代号
	 * 
	 * @return
	 */
	public String getPublish_ServerZone();
	/**
	 * 返回服务器主版本号【一般情况下BBV优先BV】
	 * @return
	 */
	public String getPublish_ServerCV();
	/**
	 * 此渠道对应的行云统计ID<br>
	 * 
	 * 
	 * @return
	 */
	public String getPublish_XingCould_AppId();

	/**
	 * 此渠道对应的行云支付ID
	 * 
	 * @return
	 */
	public String getPublish_XingCould_PayAppId();
	/**
	 * 发布区域
	 * @return
	 */
    public String getPublish_Region();
    
    /**
     * 取得渠道标识 
     * @return
     */
	public String getPublish_ChannelMark();
    
	/**
	 * 根据serverid 生成对应的统计APPID<br>
	 * 统计ID=APP_ID_HEAD+[发布渠道-可无]+s+serverId<br>
	 * eg:<br>
	 * age@gg_en_andriod.s1---中国区服务器<br>
	 * age@gg_en_android.global.s2--全球<br>
	 * age@gg_en_android.pl.s1---全球-波兰<br>
	 * 
	 * @param serverId
	 */
	public void initPublish_XingCould_AppInfo(int serverId,String payArgs);
	public void setPublish_XingCould_AppId(String appId);
	public void setPublish_XingCould_PayAppId(String payAppId);

	/**
	 * 广告接入
	 * @return
	 */
	public IPublishAD getPublishAD();
	/**
	 * 对应服务端的渠道ID
	 * @return
	 */
	public int getAOE_ChannelId();
}
