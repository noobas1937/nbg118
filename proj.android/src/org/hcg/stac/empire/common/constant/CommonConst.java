package org.hcg.stac.empire.common.constant;

import java.util.HashMap;
import java.util.Map;

public class CommonConst {
	public static final String REG_USERNAME = "^[\\w]{4,12}$";//与服务器同步
	public static final String MATCH_PWD = "^[\\w]{6,12}$";//6-12位
	/** 由于合服的原因，登陆时，允许用户在原有登陆名的后面追加_axx，因此扩充4个字符*/
	public static final String LOGIN_USERNAME = "^[\\w]{4,16}$";//与服务器同步
	public static final long DELAY_SPEED = 60;
    //MAP任务块
	public static final int QUEST_COUNT = 1024;
	//debug模式
	public static final boolean DEBUG_STATUS = false;
	//服务器配置信息版本
	public static final int DATA_VERSION = 13;
	//本地配置文件
	public static final String ASSERT_FILE="file/base.xml";
	// 服务器域名
	public static final String SERVER_GLOBAL = "aoead.hcgapp.com";
	public static final String SERVER_CN = "aoeadcn.hcgapp.com";
	public static final String SERVER_JP = "aoeadjp.hcgapp.com";
	// 体验服
	public static final String SERVER_EXPERIENCE = "50.22.172.71";
	// 友盟在线参数：服务器维护 不同的key值
	public static final String SERVER_MAINTAIN_GLOBAL = "server_maintain_global";
	public static final String SERVER_MAINTAIN_CN = "server_maintain_cn";
	public static final String SERVER_MAINTAIN_JP = "server_maintain_jp";
	/**
	 * GCM 消息传递使用的service所在的包路径
	 */
	public static final String GCM_SERVICE_PACKAGE_PATH = "org.hcg.stac.empire.common.manager";
	
	// 资源同步刷新
	public static final long REFRESH_DELAYED = 5 * 60 * 1000;
	//聊天数据同步
    public static final long REFRESH_CHAT = 5 * 1000;
    /** 登录游戏超时时长*/
    public static final long LOGIN_TIMEOUT_TIME = 90000;
    /** 网络连接超时时长*/
	public static final long CONNECT_TIMEOUT_TIME = 60000;
	
	public static final String EMPTY = "";
	
	// 文件名
	public static final String FILE_NAME = "base.xml";
	//assets文件xml路径
	public static final String ASSETS_XML_PATH = "file/";
	
	//http方式获取服务器列表
	public static final String URL_HTTP = "http://";
	public static final String URL_SERVERLIST = ":8088/uuidgetter/getserverlist";
	public static final String URL_MERGED_SERVER_INFO=":8088/uuidgetter/getmergeserverinfo";
	// 市政厅
	public static final int GBT_CITYHALL = 1;
	// 兵营
	public static final int GBT_BARRACK = 2;
	// 仓库
	public static final int GBT_WAREHOUSE = 3;
	// 城墙
	public static final int GBT_WALL = 4;
	// 科技中心
	public static final int GBT_TECH = 20;
	// 农田1
	public static final int GBT_CROP1 = 101;
	// 木材场1
	public static final int GBT_WOOD1 = 201;
	// 铁矿1
	public static final int GBT_IRON1 = 301;
	// 石矿1
	public static final int GBT_STONE1 = 401;
	
	// 士兵1
	public static final int GBT_SOLDIER1 = 2001;
	// 士兵2
	public static final int GBT_SOLDIER2 = 2002;
	// 士兵3
	public static final int GBT_SOLDIER3 = 2003;

	public static final int GBT_MARCH = 10001;
	
	// 快速升级需要的金钱
	public static final int UPGRADE_POINTS = 8;
	// 快速造兵需要的金钱
	public static final int PRODUCE_POINTS = 8;
	// 花钱快速完成升级
	public static final int UPGRADE_CASH_POINTS = 6;
	// 花钱快速完成训练
	public static final int PRODUCE_CASH_POINTS = 6;

	// 渐变时间
	public static final long SCENE_DISAPPEAR_TIME = 1000;
	// 渐变时alpha变化量
	public static final long SCENE_DISAPPEAR_ALPHA = 10;

	
	/* 地图 */
	// 空地
	public final static int GMUT_EMPTY = 0;
	// 城市
	public final static int GMUT_CITY = 1;
	// 木矿
	public final static int GMUT_WOOD = 4;
	// 石矿
	public final static int GMUT_STONE = 5;
	// 铁矿
	public final static int GMUT_IRON = 6;
	// 田矿
	public final static int GMUT_CROP = 7;
	// 据点
	public final static int GMUT_TOWER = 3;
	//宝藏
	public final static int GMUT_TREASURE=8;
	// 龙坛
	public final static int GMUT_DRAGON = 99;
	// 地图横向块数
	public final static int MAP_X_COUNT = 13;
	// 地图纵向块数
	public final static int MAP_Y_COUNT = 27;
	// 符号
	public final static String MARK_1 = "->";
	public final static String MARK_2 = "/";
	
	//战斗
	//攻方
	public final static int FT_ATTACK = 0;
	//防守
	public final static int FT_DEFENSE = 1;
	//胜利
	public final static int FT_R_WIN = 1;
	//失败
	public final static int FT_R_FAIL = 0;
	
	//邮件
	//未读
	public final static int MAIL_NO_READ = 0;
	//已读
	public final static int MAIL_READED = 1;
	
	//是否盟主
	public final static int ALLIANCE_NO_LEADER = 0;
	public final static int ALLIANCE_LEADER = 1;
	
	//时间格式
	public final static String RORMAT_TIME_1 = "MM-dd HH:mm";
	
	//quest状态
	//未完成
	public final static int QUEST_STATUS_UNFINISHED = 0;
	//完成
	public final static int QUEST_STATUS_FINISHED = 1;
	/**
	 * 代表战斗结果
	 */
	public final static String BATTLE_RESULT = "battle_result";
	
	//下载渠道
	public final static String CHANNEL_GOOLE = "Google";
	public final static String CHANNEL_360 = "360";
	public final static String CPB_APP_ID = "218";
	
    public final static String SERVICE_MAIL_ALL = "stac.empire@gmail.com";
    public final static String SERVICE_MAIL_JP = "kandasakura1@gmail.com";
    public final static String SERVICE_MAIL_AOW = "service.aop@gmail.com";
    
    //tapjoy COK
    public final static String GOLBAL_TAPJOY_APPID = "d4d1ca20-b329-4291-8b5e-91f06ebf67cb";
    public final static String GOLBAL_TAPJOY_SECRET_KEY = "08TXAHnP1rct5pmNydsA";
 
    //字体
    public final static int FONT_SMALL_SIZE = 10; //=9; 增大小屏手机字体
    public final static int FONT_LARGE_SIZE = 16;
    
    //公告显示 
    public final static String NEWS_FLG_OPEN = "1";
    //公告不显示 
    public final static String NEWS_FLG_CLOSE = "0";
    //花费进入塔需要扣除的金币
    public final static int PVE_COST_GOLD = 10;
    
    //下载地址
    public final static String CHECK_VERSION_URL = "http://%1$s/uuidgetter/checkclientversion";
    //全球
    public final static String CHANNEL_MARKET_GLOBAL = "1";
    public final static String CHANNEL_MARKET_TSORE = "2";//KR TSTORE
    public final static String CHANNEL_MARKET_AMAZON = "3";//AMAZON
//    public final static String CHANNEL_360_CN = "2";
//    public final static String CHANNEL_MARKET_JP = "3";
//    public final static String CHANNEL_MARKET_CN = "4";
//    public final static String CHANNEL_MARKET_PL = "5";
//    public final static String CHANNEL_PL_PLUS = "6";
//    public final static String CHANNEL_PL_ORANGE = "7";
//    public final static String CHANNEL_PL_TMOBILE = "8";
//    public final static String CHANNEL_PL_PLAY = "9";
//    public final static String CHANNEL_OPEN_MARKET = "10";
    
    public final static String NO_UPDATE = "0";// 不需要升级
    public final static String UPDATE_TIP = "1";// 升级提示
    public final static String FORCE_UPDATE = "2";// 强制升级
    //普通出征
    public final static int ATTACK_NORMAL = 0;
    //快速出征
    public final static int ATTACK_SPEEDUP = 1;
    //进攻次数限制-8
    public final static int ATTACK_TIMES_LIMIT = 8;
    
     //波兰运营商渠道appid和支付key
    public final static String APPID_PL_PLUS = "age@hcg337_pl_android.pl.plus,f1d4dd30d588012f4e07782bcb1b6cfd";
    public final static String APPID_PL_ORANGE = "age@hcg337_pl_android.pl.orange,0cfa9710d589012f4e0a782bcb1b6cfd";
    public final static String APPID_PL_TMOBILE = "age@hcg337_pl_android.pl.tmobile,26d192a0d589012f4e0d782bcb1b6cfd";
    public final static String APPID_PL_PLAY = "age@hcg337_pl_android.pl.play,426875b0d589012f4e10782bcb1b6cfd";
    public final static String APPID_OPEN_MARKET = "age@hcg337_pl_android.pl.openmarket,d960a280d589012f4e14782bcb1b6cfd";  

	//文字颜色
	public static final int CONTENT_TEXT_COLOR = 0xFFE89300;
	
	//建筑开放相关
	public static final String MARKET_CLICKED = "MARKET_CLICKED";
	public static final String ARENA_CLICKED = "ARENA_CLICKED";
	
	public static final String MARKET_OPENED = "MARKET_OPENED";
	public static final String ARENA_OPENED = "ARENA_OPENED";
	
	//图标开放相关
	public static final String ICON_GENERAL_SYN_CLKED = "ICON_GENERAL_SYN_CLKED";
	public static final String ICON_ALLIANCE_CLKED = "ICON_ALLIANCE_CLKED";
	public static final String ICON_MAIL_CLKED = "ICON_MAIL_CLKED";
	public static final String MILITARY_INFO_EVER_ARRIVED = "MILITARY_INFO_EVER_ARRIVED";
	public static final String ICON_MILITARY_CLKED = "ICON_MILITARY_CLKED";
	
	public static final String ICON_GENERAL_SYN_OPENED = "ICON_GENERAL_OPENED";
	public static final String ICON_ALLIANCE_OPENED = "ICON_ALLIANCE_OPENED";
	public static final String ICON_MILITARY_OPENED = "ICON_MILITARY_OPENED";
	
	public static final String TOMORROW_LOGIN_REWARD_TYPE = "tomorrow_login_reward_type";
	
	//建筑类型
	//市政厅
	public static final String BT_CITYHALL = "cityhall";
	//混乱之塔
	public static final String BT_TOWER = "tower";
	//竞技场
	public static final String BT_ARENA = "arena";
	//科技
	public static final String BT_SCIENCE = "science";
	//兵营
	public static final String BT_BARRACKS = "barracks";
	//市场
	public static final String BT_MARKET = "market";
	//仓库
	public static final String BT_WAREHOUSE = "warehouse";
	//资源建筑
	public static final String BT_RESBUILDING = "resbuilding";
	//英雄
	public static final String BT_HERO = "hero";
	//英雄强化
	public static final String BT_STRENGTHEN = "strengthen";
	//野地
	public static final String BT_WILD = "wild";
	//城墙
	public static final String BT_WALL = "wall";
	//抽奖
	public static final String BT_LOTTERY = "lottery";
	//商店
	public static final String BT_SHOP = "shop";
	//任务
	public static final String BT_TASK = "task";
	//装备
	public static final String BT_EQUIPMENT = "equipment";
	//宝石
	public static final String BT_DIAMONDS = "diamonds";
	
	public static final String TOMORROW_LOGIN_REWARD = "tomorrow_login_reward";
}
