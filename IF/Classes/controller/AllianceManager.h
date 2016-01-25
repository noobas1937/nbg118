//
//  AllianceManager.h
//  IF
//
//  Created by ganxiaohua on 14-4-3.
//
//

#ifndef __IF__AllianceManager__
#define __IF__AllianceManager__

#include "CommonInclude.h"
#include "AllianceGiftInfo.h"
#include "AllianceEventInfo.h"
#include "IFAllianceHelpReportInfo.hpp"
#define  ALLIANCE_NOTICE_KEY             "notice_0123456789"
#define MSG_INVITE_TELEPORT_SEND_SUCCESS "msg.invite.teleport.send.success"
#define MSG_GET_NEAREST_POINT_END        "msg.get.nearest.point.end"
#define MSG_ALLIANCE_BUY_FAIL            "msg.alliance.buy.fail"

#define MSG_ALLIANCE_HEAD_UPDATE         "msg.alliance.head.update"

enum RankType
{
    CHANGE_RANK_TITLE,//0
    CHANGE_BANNER,//1
    CHANGE_BRIEF_NAME,//2
    CHANGE_NAME,//3
    DISBAND_ALLIANCE,//4
    TRANSFER_LEADERSHIP,//5
    KICK_MEMBERS,//6
    CHANGE_SLOGAN,//7
    OPEN_RECRUITMENT,//8
    SEND_ALLIANCE_INVITE,//9
    EDIT_ALLIANCE_NOTICE,//10
    SHOW_ALLIANCE_LANGUAGE,//11
    SHOW_ALLIANCE_SHOP,//12
    RESEARCH_ALLIANCE_SCIENCE,//13
    OPEN_ALLIANCE_ACTIVITY,//14 世界联盟活动
    ALLIANCE_COMMENT,//15屏蔽联盟留言
    DEMOTE_MEMBERS,//16
    PROMOTE_MEMBERS,//17
    QUIT,//18
    CHECK_RANK_DETAIL,//19
    ALLIANCE_HELP,//20
    RESOURCE_TRADE,//21
    CHECK_ALLIANCE_MEMBER,//22
    MAIL_ALL,//23
    RESTRICTION,//24
};

class AllianceManager : public CCObject{
public:
    static AllianceManager* getInstance();
    //联盟列表
    map<string,AllianceInfo*> allianceList;
    //申请过的联盟
    map<string,AllianceInfo*> applyAllianceList;
    //邀请我加入的联盟
    map<string,AllianceInfo*> invitesAllianceList;
    //搜索出来的联盟用户
    map<string,AllianceInfo*> sendInvitesList;
    //搜索出来用户
    map<string,PlayerInfo*> searchInvitesList;
    //申请的用户
    map<string,PlayerInfo*> applyUserList;
    //发送邀请的用户
    map<string,PlayerInfo*> sentInvitesList;
    //联盟礼物
    map<string,AllianceGiftInfo*> giftsList;
    //rank的用户
    map<string,PlayerInfo*> rankPlayerList;
    //联盟商店列表
    map<string,CCDictionary*> storeList;
    //联盟可制造列表
    map<string,CCDictionary*> makeList;
    //联盟正在制造的列表
    map<string,CCDictionary*> storeMakeList;
    //联盟战争队伍列表
    map<string,CCDictionary*> warTeamList;
    
    vector<AllianceInfo*> recallianceArray;
    vector<IFAllianceHelpReportInfo*> m_helpReportListVec;
    map<string, string> recAllianceUrlMap;
    map<string, string> recAllianceNameMap;
    
    void init();
    void callHelp(int qid);
    void sendHelpAllToServer();
    void openYuanYunView(int pointId);
    void openMarketView();
    void openAllianceTradeView(std::string uid,int mainLV,int pointId);
    void openAllianceMassView(int pointId);
    void onAttackView(int pointId);
    bool checkRight(RankType type,int rank);
    void parseEvents(CCDictionary* dic);
    void clearEvents();
    void addAnnounceEvent();
    
    void starGetRecAlliance();
    void endGetRecAlliance(CCObject* data);
    void showRecAlliance();
    void startGetAllianceUrl(string uuid);
    void endGetAllianceUrl(CCDictionary* dic);
//    void starGetNewAlliance();
//    void endGetNewAlliance(CCDictionary* dic);
    
    //-------邀请迁城的方法
    void startInviteTeleport(string targetUid, string allianceId, int cityIndex = -1);
    void endInviteTeleport();
    void endTeleport(string);
    
    void joinAllianceSuccess(CCObject* obj);
    
    int maxAlliancePage;
    int applyMaxPage;
    int invitesMaxPage;
    int userMaxPage;
    int currentTrade;
    int lastTrade;
    int totalTrade;
    float rate;
    int tradeRate;
    int tradeValue;
    int maxTrade;
    bool needRefresh;

    
    std::vector<int> v_rank5;
    std::vector<int> v_rank4;
    std::vector<int> v_rank3;
    std::vector<int> v_rank2;
    std::vector<int> v_rank1;
    map<std::string, AllianceEventInfo*> eventsMap;
    
    //存储的邀请盟友迁城的城点
    int tmpCityIndex;
    //迁城跳转到世界时的类型 如果为0则弹blanktile 如果为1则弹迁城popup  格式为xxxxxx - 资源点，怪物放置的物品id
    int goToWorldType;
    //邀请迁城的邮件uid暂存
    std::string tmpMailUid;
    //世界寻点不更新
    int notUpDate;
    
    //推荐联盟的标记
    int recAllianceIdx;
    int recAllianceType;
    CC_SYNTHESIZE(bool, m_isOpenInMainUI, isOpenInMainUI);
    CC_SYNTHESIZE(int, m_joinType, joinType);
    CC_SYNTHESIZE(string, m_recommendAid, recommendAid);
};
#endif /* defined(__IF__AllianceManager__) */
