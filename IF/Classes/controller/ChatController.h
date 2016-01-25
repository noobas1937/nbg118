//
//  ChatController.h
//  IF
//
//  Created by fubin on 13-9-26.
//
//

#ifndef IF_ChatController_h
#define IF_ChatController_h

#include "CommonInclude.h"
#include "ChatInfo.h"
#include "ChatMailInfo.h"

#define CHAT_COUNTRY 0
#define CHAT_SELF 1
#define CHAT_ALLIANCE 2
#define CHAT_COMMENT 3
#define CHAT_GENERAL 99

#define CHAT_LIMIT_SIZE 20
#define CHAT_REQUEST_NUM 20

#define MSG_CHAT_NAME "chat_name"
#define MSG_CHAT_BAN "chat_ban"
#define MSG_CHAT_UPDATE "chat_update"

#define MSG_RECORD 0
#define MSG_RECEIVE 1

#define GET_MUTIUSER_TYPE_ALLIANCE  0
#define GET_MUTIUSER_TYPE_FRIEND    1

class ChatController:public CCObject {
public:
    static ChatController* getInstance();
    ChatController():uid(""),chat_interval(1),chat_time(600),canCountryRequest(true),canAllianceRequest(true),m_tmpType(0),m_tmpSaveMsg(""),newChaterAllianceId(""),idx(0),m_banTime(0),isFirstRequestCountry(true),mIsNoticeOpen(false),mNoticeShowTime(2),mNoticeCharMax(140),m_noticeBanTime(0),mTransChat(NULL),translateOptimizationUrl(""),m_banTimeSet("1|2|3|4"){
        m_chatInfoSendDic=CCDictionary::create();
        m_chatInfoSendDic->retain();
        m_userInfoDic=CCDictionary::create();
        m_userInfoDic->retain();
        m_chatUserInfoDic=CCDictionary::create();
        m_chatUserInfoDic->retain();
    };
    ~ChatController();
    static void purgeData();
    
    string translateOptimizationUrl;
    string uid;
    int chat_interval;
    int chat_time;
    bool isFirstRequestCountry;
    bool canCountryRequest;
    bool canAllianceRequest;
    int m_tmpType;
    string m_tmpSaveMsg;
    string newChaterAllianceId;//更新聊天记录联盟信息
    int idx;//当前点中的聊天记录
    
    std::vector<ChatInfo> m_chatCountryPool;
    std::vector<ChatInfo> m_chatAlliancePool;
    std::vector<ChatInfo> m_commentPool;
    
    //最新一条国家消息
    ChatInfo m_latestCountryMsg;
    //最新一条联盟消息
    ChatInfo m_latestAllianceMsg;
    
    CCDictionary* m_chatInfoSendDic;        //用于通过JNI向android传送聊天和邮件消息，key对应消息序号，value对应消息体，消息体可能有多条消息也可能是一条消息
    CCDictionary* m_userInfoDic;                //用于通过JNI向android传送的所有玩家信息，key对应玩家信息体序号，value对应玩家信息
    
    //发送 接收 消息
    bool sendCountryChat(const char* msg, int type,int post=0,std::string sendLocalTime="",const char* dialog ="",CCArray* msgArr = NULL,std::string thxuuid="");
    bool sendSelfChat(const char* msg, const char* uid=NULL, const char* name=NULL);
    void receiveChat(CCDictionary* dict);
    
    bool sendRequestChat(int type);
    void retRequestChat(CCDictionary* dict);
    
    //查看用户是否存在
    void retFindUser(CCDictionary* dict);
    bool checkSameLangBySystem(string oLang);
    
    //翻译消息
    void translateMsg(int sessionType,int index);
    
    
    //发送邮件
    void sendMail(string name,string uid);
    
    //显示玩家信息
    void showPlayerInfo(string uid);
    
    //解除屏蔽玩家
    void unShieldPlayer(string uid,string name);
    //屏蔽玩家
    void shieldPlayer(string uid);
    //禁言玩家
    void banPlayer(string uid,int banTimeIndex);
    int getBanTime(int index);
    //解除禁言玩家
    void unBanPlayer(string uid);
    //禁言玩家喇叭消息
    void banPlayerNotice(string uid,int banTimeIndex);
    //解除禁言喇叭消息玩家
    void unBanPlayerNotice(string uid);
    //加入联盟
    void joinAlliance(string uid);
    void joinAllianceBtnClick();
    void reportPlayerChatContent(string uid,string content);
    //查看玩家战报
    void viewBattleReport(string uid,string reportUid);
    //查看玩家侦察战报
    void viewDetectReport(string uid,string detectReportUid);
    void inviteJoinAlliance(string uid,string name);
    void showLatestMessage(int chatType);
    //查看装备
    void viewEquipment(string equipId);
    //查看集结信息
    void viewRallyInfo(string teamUid);
    //查看转盘
    void viewLotteryInfo(string lotteryInfo);
    //查看转盘入口IOS
    void viewLotteryInfoFromIOS(string lotteryInfo);
    //查看联盟任务
    void viewAllianceTaskInfo();
    //查看红包
    void viewRedPackage(string redPackageUid,string server,bool isViewOnly = false);
    //获取已领取的红包uid
    void getHandledRedPackages();
    //获取当前红包状态
    void getRedPackageStatus(string redPackageUid,string serverId);
    void changeNickName();
    void getLatestMessage();
    ChatInfo parseLatestChatInfo(Json* jsonObject);
    void parseLatestChatInfoStr(string jsonStr);
    
    void postShieldUids(Array* uidArray);
    
    void joinAnnounceInvitation(string allianceId);
    void receiveNoMessage();
    
    bool sendRequestChatFromAndroid(int type);
    void sendNotice(std::string msg, int itemid, bool usePoint,std::string sendLocalTime="");
    void testCommand();
    void testMailCommand();
    void testGetNewMsg();
    void testGetMsgBySeqId();
    void testMultiUserInfo();
    void testGetChatRoomMember();
    void getMultiUserInfo(vector<std::string> *uids);
    void getMultiUserInfoCallback(CCDictionary* dict);
    CCDictionary* m_chatUserInfoDic;
    
    void addLocalTranslate(ChatInfo *cInfo);
    void onLocalTranslate(CCHttpClient* client, CCHttpResponse* response);
    static void getTranslateInfo(std::string transStr,std::string &orgLan,std::string &transMsg);
    
    void callXCApiForGoogleTranslate();
    
    void notifyMsgToAndroid(CCArray* chatMailInfo,int sessionType,string id,string customName="",bool isModMsg = false);
    void notifyMsgToIOS(CCArray* chatMailInfo,int sessionType,string id);
    void getNewMsg(std::string comandStr);
    void getNewMailMsg(std::string comandStr);
    
   
    void setDisableTranslateLang();
    void reportCustomHeadPic(std::string uid);
    void translateOptimize(std::string method,std::string originalLang = "",std::string userLang = "",std::string msg = "",std::string translationMsg = "");
    void getFriendMember();
    void getRedPackageTime();
    void getKingOfServer();
    void setBanTimeSet(std::string banTimeSet);
    
private:
    //喇叭
    CC_SYNTHESIZE(bool, mIsNoticeOpen, IsNoticeOpen);
    CC_SYNTHESIZE(int, mNoticeShowTime, NoticeShowTime); //单位 s
    CC_SYNTHESIZE(int, mNoticeCharMax, NoticeCharMax);
    CC_SYNTHESIZE(int, mNoticePrice, NoticePrice);
    CC_SYNTHESIZE(double,m_noticeBanTime,NoticeBanTime);
    
    CC_SYNTHESIZE(int, m_banTime, BanTime);
    
    void getOfficersData(CCObject* data);
    void notifyChatMsgToAndroid(CCArray* chatInfoArr);
    void notifyChatMsgToIOS(CCArray* chatInfoArr ,int channelType , string id);
    void setTranslationSucc(CCObject* pObj);
    void getUserInfoCallback(CCObject* pObj);
    void joinAllianceCallback(CCObject* pObj);
    void viewBattleReportSuccess(CCObject* pObj);
    void viewDetectReportSuccess(CCObject* pObj);
    void joinAnnounceInvitationSuccess(CCObject* pObj);
    void inviteFunc(CCObject* obj);
    
    
    string m_battleReportId="";    //战报ID
    string m_battleReportPlayerUid=""; //战报消息玩家的UID
    string m_detectReportId="";     //侦察战报ID
    string m_detectReportPlayerUid="";  //侦察战报消息玩家的UID
    string m_invitePlayerName="";   //邀请玩家的名字
    string m_banTimeSet;       //禁言时间
    
    ChatInfo *mTransChat;
    vector<ChatInfo*> mTransQueue;
    void localTranslate();
    
};

#endif /* defined(__IF__ChartController__) */
