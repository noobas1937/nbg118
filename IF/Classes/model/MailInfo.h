//
//  MailInfo.h
//  IF
//
//  Created by 李锐奇 on 13-10-11.
//
//

#ifndef IF_MailInfo_h
#define IF_MailInfo_h

#include "cocos2d.h"
//#include "PlayerInfo.h"
USING_NS_CC;

enum MailStatus{
    UNREAD,
    READ,
};

#define MAIL_DETECT_REPORT_PROTECT 10000

class MailInfo : public CCNode{
public:
    static MailInfo* create();
    void parse(CCDictionary* dic);
    void parseBattleMail(CCDictionary* dic);
    void parseDetectRepMail(CCDictionary* dic);
    ~MailInfo();
    std::string contents;
    std::string uid;
    time_t createTime;
    std::string rewardId;
    std::string pic;
    int picVer;
    int status;
    std::string fromName;
    std::string modLanguage;
    std::string nowName;
    std::string fromUid;
    std::string reportUid;
    std::string version;
    int rewardStatus;
    int tabType;       //标示邮件属于那个tab大类
    std::string title;
    bool prepareToDelete;
    int type;
    int itemIdFlag;//1需要读语言文件
    int save;//0未保存,1保存,2删除保存过
    bool isRead;
    bool isReadContent;
    std::string showContent;
    int reply;
    int goToDonate;
    int pointType; //MAIL_DETECT_REPORT_PROTECT - 10000 (前台自用,侦查邮件使用反侦察道具), 10 王座 , 12 投石机 , .. 其他 ( 类型: 4(战斗) 8(侦查报告) 9(扎营)添加该字段)
    
    //联盟内容
    std::string alliance;
    int deal;
    std::string learderName;
    std::string alliancename;
    std::string leaderName;
    std::string allianceLang;
    int curMember;
    std::string allianceId;
    std::string iconAlliance;
    int fightpower;
    int maxMember;
    //以下为战报
    CCDictionary *attUser;
    CCDictionary *defUser;
    CCDictionary *defArmyTotal;
    CCDictionary *atkArmyTotal;
    CCArray *atkHelpReport;
    CCArray *defHelpReport;
    CCDictionary *atkAlliance;//"allianceName" 、"allianceIcon
    CCDictionary *defAlliance;
    CCArray *atkHelper;
    CCArray *defHelper;
    CCArray *atkGen;
    bool isResourceShieldState;
    CCArray *defGen;
    bool isShowDefEndTxt;
    CCArray *atkWarEffect;
    CCArray *dfWarEffect;
    CCArray *reward;
    std::string winner;
    CCPoint warPoint;
    CCArray *attReport;
    CCArray *defReport;
    int resLevel;
    CCArray * defFortLost;
    int atkPowerLost;
    int defPowerLost;
    int battleType;  // 6 为怪物攻城 7 联盟领地战斗报告 8 联盟国旗战斗报告
    //武将训练
    CCArray *trainGenerals;
    CCArray *trainGeneralExp;
    
    //侦查
    CCDictionary *detectReport;
    //驻扎
    CCArray *occupyGeneral;
    int occupyPointId;
    int pointId;
    CCArray * defTowerKill;
    CCArray *atkGenKill;
    CCArray *defGenKill;
    //迷宫邮件
    CCArray *monsters;
    CCArray *mazeReward;
    CCDictionary *mazeSodiler;
    CCDictionary *user;//扎营君主信息
    int mazeTime;
    int mazeIndex;
    //怪物邮件
//    CCArray *normalReward;
//    CCArray *killReward;
//    int monsterTotalHp;
//    int monsterCurrentHp;
//    std::string monsterId;
//    bool firstKill;
    int exp;
//    int hurt;
//    int survived;
//    int dead;
//    int total;
//    int monsterDead;
//    int monsterX;
//    int monsterY;
//    int selfX;
//    int selfY;
//    int selfPowerLost;
//    int monsterPowerLost;
    //个人邮件
    CCArray *dialogs;// record two persons talks
    int dialogNum;      // total talks
    int unreadDialogNum;  //unreaded dialogs
    std::string preSaveContent;//保存的内容
    
    std::string applicantName;
    std::string applicantPic;
    int applicantPower;
    std::string applicantId;
    bool checkMail;
    time_t shareTime;
    std::string shareUid;
    
    //联盟成员邀请迁城
    //是否已经对邮件做出应答
    int inviteTeleDeal;
    std::string inviteeUid;
    std::string inviterUid;
    std::string inviterName;
    std::string inviteeName;
    std::string message;
    int targetPoint;
    int userKill;//当前击杀怪物数量
    int allKill;//联盟击杀怪物数量
    int msReport;//联盟击杀结束标志 1结束
    int failTimes;//联盟击杀 失败次数
    int winPercent;//联盟杀怪 击杀比例
    int killRound;//联盟杀怪 波数
    int userScore;//联盟杀怪 现在是积分
    int monsterLevel;//联盟杀怪 lv
    int like;//1显示facebook like ,0 不显示
    //以下是从chat中打开detectReportMail相关
    bool isFromChat;
    CCDictionary *ckfWarInfo;//跨服胜点对象
    bool isShare;
    std::string ckfContents;
    int ckf;//跨服邮件 转换坐标什么的，要用这转getPointByMapTypeAndIndex
    bool isWorldBossKillRewardMail;
    
    //聊天室使用
    std::string crGroupId;          //聊天室群组ID
    CCArray* crMemberUidArray;      //群组成员Uid Array
    CCDictionary *saveDict;
    
    //宝藏
    bool isTreasureMap;
};

#endif
