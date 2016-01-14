//
//  ChatServiceCocos2dx.h
//  ChatService Cocos2dx SDK
//

#ifndef ChatServiceCocos2dx_h
#define ChatServiceCocos2dx_h

#include "cocos2d.h"
#include <string>
#include "ChatServiceMessageListener.h"
#include "PopupBaseView.h"

using namespace std;

class ChatServiceCocos2dx
{
public:
    static ChatServiceMessageListener* sendMessageListener;
    static bool isChatShowing;
    static bool databaseInited;
    static bool initChatServiceLater;
    static bool isForumShowing;
    static bool isTranslationWebViewShowing;
    static bool  enableNativeMail;
    static bool enableChatRoom;
    static bool enableNativeChat;
    static bool isChatDialogNeedBack;
    static bool useWebSocketServer;
    static bool isRecieveFromWebSocket;
    static bool isSendFromWebSocket;

	static void showChatInputView();
	static void hideChatInputView();
    static bool isAccountChanged;
    static string uidStr;
    
	static string getChatInputText();
	static void enableChatInputView();
	static void disableChatInputView();
	static void setSendButtonText(const char* text);
	static void setEditTextHintText(const char* text);
	static void toggleFullScreen(bool enabled);
    static void resetResume();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    static void install( const char *apiKey, const char *domainName, const char *appID);
    static void install( const char *apiKey, const char *domainName, const char *appID, cocos2d::CCDictionary *config);
    static void pauseDisplayOfInAppNotification(bool pauseInApp);
    static int  getNotificationCountFromRemote(bool isRemote, void (*notificationHandler) (int count));
    static void handleLocalNotification(const char *issueId);
    static void handleRemoteNotification(cocos2d::CCDictionary *notification);
    static void setInAppNotificationHandler (void (*inAppNotificationHandler) (int count));
    static void showChatIOSFrom2dx();
    static void handleChatPush(cocos2d::CCArray *chatInfoArr,int channelType,string channelId) ;
    static void setPlayerAllianceInfo(const char* asn,const char* allianceId,int allianceRank,bool isFirstJoinAlliance);
    static void setPlayerInfo(int gmod,int headPicVer,const char* customHeadImageUrl,const char* name,const char* uid,const char* headPic,const char* vipInfo) ;
    
    static int m_curSendChatIndex;      //当前发送的消息体序号
    static int m_curSendMailIndex;      //当前发送的邮件体序号
    static std::string m_curMailFromUid;      //当前打开的邮件对话框收件人Uid
    static int m_channelType;
    static bool m_isInMailDialog;
    static bool m_rememberPosition;
    static int m_curPopupWindowNum;
    
    static void setMailInfo(const char* mailInfoFromUid,const char* mailInfoUid,const char* mailInfoName,int mailInfoType);
    static std::string& StringReplace(std::string& strBig, const std::string& strsrc, const std::string& strdst);
    //通过滑动选择聊天频道
    static void selectAsnORCountries();
    //当cocos2d弹出提示窗的时候隐藏聊天窗口
    static void hiddenChatIosFrom2dx();
    //判断聊天窗口是否打开
    static bool isChatShowing_fun();
    static bool isNewMailShowing_fun();
    //弹出提示窗
    static void flyHint(std::string icon, std::string titleText, std::string contentText, float time, float dy, bool useDefaultIcon);
    //系统倒计时弹出框
    static void flySystemUpdateHint(double countDown,bool isFlyHintLogin, bool isLogin, string tip, string icon);
    /**退出联盟时重置联盟信息*/
    static void resetPlayerIsInAlliance();
      /**清缓存消息*/
   static  void  resetPlayerFirstJoinAlliance();
    /**清除国家数据*/
    static void clearCountryMsg();
    /**切换语言*/
    static void notifyChangeLanguage();
    /**卸载所有窗口*/
    static void unInstallWin();
    /**删除邮件*/
    static void deleteUserMail(string fromUid);
    /**重置ios聊天数据*/
    static void resetIOS();

    static void setCurrentUserId(string uid);
    
    static void setPlayerInfo();
    /**创建聊天室*/
    static void showMemberSelectorFrom2dx();
    /**设置原生端聊天室成员*/
    static void updateChatChannelRoom(int channelType , string channelID , string uidArr , string roomOwner , string customName) ;
    static int m_curUserInfoIndex;      //当前发送的用户信息序号
    static void notifyUserInfo(cocos2d::CCArray* memberArr);
    
    /**创建群聊选人页面*/
    static void groupChatVCInit();
    
    static void initPlayerInfo();
    
    static void notifyUserUids(string uidStr,string lastUpdateTimeStr);
    
    /**通知聊天室名字更改*/
    static void notifyChatRoomNameChanged(string name);
    /**设置聊天室房主*/
    static void setChatRoomFounder(string groupId,string founderUid) ;
    static bool m_isNoticItemUsed;       //是否使用了喇叭道具
    static bool m_autoTranslateWithGoogle;      //是否对没有译文的历史消息进行google翻译
    static bool m_autoTranslate;      //是否对没有译文的历史消息进行google翻译
    static bool  DB_UserInfo_switch;//DB 用户信息存储开关
    static bool  DB_ChatChannel_switch;//DB chatChannel存储
    static bool    DB_MsgItem_switch;//DB msgItem存储开关
    static bool    DB_SystemMail_switch;//DB 系统邮件存储开关
    static bool  Mail_OC_Native_Enable;//iOS 原生邮件开关
    static bool CS_CountryChat_OC_Native_New;//国家联盟与新版邮件整合
    static bool    CS_ChatCell_OC_Native_New;//新版聊天cell开关
    
    static int CS_Country_SpeakingLevelLimit;//国家发消息限制等级
    
    static bool CS_WebSocket_Msg_Get;//独立聊天服务器消息接受
    static bool  CS_WebSocket_Msg_Send;//独立聊天服务器消息发送
    
    static bool  CS_AllianceTask_Helped;//联盟宝藏帮助
    
    static void gettingChatMsgWithAppOpen();//应用打开第一次调用getnewmailmsg接口
    static string getChannelInfo();
    /**更新聊天室*/
    static void updateChannelMemberArray(int channelType,string fromUid,string uidArr,int changeStatus);

    /**判断是否是第一次打开和该玩家的邮件对话*/
    static bool isFirstOpenMailToPlayer(string fromUid);

    static void settingGroupChatMailVCChatChannel();
    /**初始化数据*/
    static void initDatabase();
    
    /**搜索玩家*/
    static void selectPlayer(cocos2d::CCArray* players);
    /**清除邮件信息*/
    static void removeMail2fromUid(string fromUid);
    
    /**打开论坛*/
    static void openBBS();
    
    /**刷新C++端最新消息的显示*/
    static void refreshChatInfo(int channelType);
    /**删除所有聊天数据*/
    static void clearChatData();
    
    /**创建聊天室*/
    static void createChatChannelRoom(string groupId , string memberUids);
    
    /**更新频道信息*/
    static void updateChatChannelInfo(string newMsgInfo);

    
    static void creatingMailListWith_OC_Native();//初始化oc原生邮件列表
    
    //国家消息
    static void csPushCountryChatMsgWithMsgArray(int commandType,cocos2d::CCArray *countryMailArray,string maxServerSeq,string minServerSeq);
   //联盟消息
    static void csPushAllianceChatMsgWithMsgArray(int commandType,cocos2d::CCArray *allianceMailArray,string maxServerSeq,string minServerSeq);
    //历史消息
    static void cs_get_mail_Old_AllianceChatMsgWithMsgArray( cocos2d::CCArray *allianceMailArray ,int channleType);
    //系统邮件
    static void pushSystemMailForOnlineStatusWithCCDic(cocos2d::CCDictionary *vDict);//在线推送邮件接口
    static void pushSystemMailMsgWithMsgArray(cocos2d::CCArray *vSystemMailArray ,int isHasMoreMsg);
    //邮件
    static void  pushChatMailMsgWithArray(int commandType,cocos2d::CCArray *vChatArray);
    //聊天室
    static void  pushChatRoomMsgWihtArray(int commandType,string channelIDString,string roomNameString,string memberUidString ,string founderUid,cocos2d::CCArray *vChatRoomArray);
    //比较版本
    static bool compareVersionWithKeyVersionString( string vKeyString);
    /**
     *  接触屏蔽或禁言
     *
     *  @param palyerName 目标玩家名字
     *  @param type       屏蔽 或 禁言
     *
     */
    static void chatUnBanOrUnBlock(string palyerName , int type);
    //调用系统邮件请求的command
    static void gettingSystemMsgWithString(string vCommandStr);
    static void gettingGetNewMailMsgCommand(string vCommandStr);
    static void loadingFinishedSingel();
    
    static void setDisableLang(std::string disableLang);

    static void CSApplicationDidBecomeActiveLessThanSixtySec();
    static void CSApplicationWillDidGotoBackGround();
    
    /**
     *  清空跳转视图状态
     */
    static void stopReturnToChat();
    static void updateUnreadCountOnMastermind();
    static int  settingMailUnreadCount();
    
    static void setMailRewardStatusToReadWithMailID(string MailID);
    static void setMailRewardStatusForBatchToReadWithMailIDs(string MailID,int rewardStatus);
    static void settingMailRewadToReadFor_batchWithRewardString(string MailIDs,string rewardString,int rewardStatus);
    static void settingUnReadStatusToReadWithMailID(string MailID);
    static void settingUnReadStatusToreadWith_mailType(int mailType ,bool readStatus);
    static void settingUnReadStatusToReadWIth_ChatUserUid(string uid,bool readstatus);
    static void deleteingbanchMailsWithMailID(string uid);
    static void deleteingKnightMail();
    static void  settingOC_Native_MailViewShowFromCocos(PopupBaseView *popView);
    //设置禁言的时间，服务器配置
    //设置国王UID
    static void postKingUid(string kingUid);
    static void settingDisableSendMsgWithServiewListKey(string listKeyStr);
    
    static void popUpNewMail(string uid);
    
    static void showChatView2IOS();
    /**
     *   更新红包状态
     *
     *  @param uids 所有已领取过的红包uid
     */
    static void postRedPackageGotUids(string uids);
    static void postNotify_NewMailChatCreat_OC_Native(int sucessed,string name,string uid);
 
    /**
     *  邮件分页接口
     *
     *  @param channelId 频道类型
     */
    static void loadMoreMailFromIOS(string channelId);
    /**
     *  更新邮件状态
     *
     */
    static void postMailDealStatus(string mailUid);
    /**
     *  根据后台返回的红包状态刷新UI
     *
     *  @param uids   红包uid
     *  @param status 状态 返回 "status" int (0 1 2 3)   0已领取 1未领取  2  被抢光了 3 到期了
     */
    static void postRedPackageStatus(string uids,int status);
    
    /**
     *  获取红包过期时间
     *
     *  @param time 过期时间
     */
    static void postRedPackageDuringTime(int time);
    /**
     *  锁定邮件
 
     */
    static void setMailSave(string mailId,int saveFlag);
    /**
     *  删除邮件
     *
     *  @param mailId      邮件ID
 
     */
    static void deleteMail(string mailId,int channelType,int type);
    //删除一份邮件
    static void deleteOneMailWithMailID(string mailID,int type);
    //获取10分钟之内的城堡侦查战报
    static void  gettingTenMinuteDectCityMailList();
    //更新战报接口
     static void  updateDectCityMail(string mailID ,string name ,int creatTime );
    //删除战报接口
     static void  deleteDectCityMail(map<string,pair<string,int>> mailDectMap);
    /**
     获取原生IOS禁言列表
     - parameter cocos2d: C++禁言列表
     */
    static void initBanList(cocos2d::CCArray* shieldList);
    
    /**
     *  网络断开连接接口，或者帐号其他端登录 负责关闭原生聊天
     *
     *  @param needRemeberActivityStack
     */
    static void exitChatActivityFrom2dx(bool needRemeberActivityStack = true);
    /**
     *  本地语言改变
     */
    static void changeLanguage();
    /**是否进入巨龙战场*/
    static bool isGoIntoServer_Dragon_Battle();
     /**更新用户信息*/
    static void  updateUserInfoWithUidString(string uidStr);
      /**原生端关闭回掉*/
    static void nativeUIClosed();
    /**返回好友列表人员的最后一条消息*/
    static  map<string,string> lastMsgWithFriendList(std::vector<std::string> friendUidVec);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	static void setSendMessageListener(ChatServiceMessageListener* messageListener);
    static void reset();
    static void completeInitDatabase();
    static void initChatService();
    //显示android聊天
    static void showChatActivityFrom2dx();
    static void showForumFrom2dx(const char* url);
    static void showTranslationOptimizationFrom2dx(const char* url);
    static void showMemberSelectorFrom2dx();
    static void handleChatPush(bool isNewMsg,bool isSelf,int  type,int  post,int  gmod,const char* vip,const char* time,const char* uid,const char* name,const char* asn,const char* allianceId,const char* msg,const char* translateMsg,const char*  headPic,const char* battleReportUid,const char* detectReportUid,const char* originalLang) ;
    static void exitChatActivityFrom2dx(bool needRemeberActivityStack = true);
    static void notifyReturn2dxGame();
    static void setGameLanguage();
    static void setPlayerIsInAlliance();
    static void resetPlayerIsInAlliance();
    static void notifyMessageIndex(int chatInfoNo,string msgType,string customName="",bool isModMsg = false);
    static void onPlayerChanged();//切服
    static void notifyChangeLanguage();
    static void setMailInfo(const char* mailInfoFromUid,const char* mailInfoUid,const char* mailInfoName,int mailInfoType);
    static void onCreateChatroomSuccess();
    static void setPlayerAllianceInfo(const char* asn,const char* allianceId,int allianceRank,bool isFirstJoinAlliance) ;
    static void setCurrentUserId(string uid);
    static void setIsNewMailListEnable(bool enabled);
    static void setPlayerInfo();
    static void initDatabase(bool isNewUser);
    static void deleteUserMail(string fromUid);
    static void flyHint(std::string icon, std::string titleText, std::string contentText, float time, float dy, bool useDefaultIcon);
    static void flySystemUpdateHint(double countDown,bool isFlyHintLogin, bool isLogin, string tip, string icon);
    static void resetPlayerFirstJoinAlliance();
    static void postNoMoreMessage(int sessionType);
    static void postChannelNoMoreData(int sessionType,bool hasNoMoreData);
    static void setChatHorn();
    static void onJoinAnnounceInvitationSuccess();
    static void notifyUserInfo(int index);
    static void setChannelMemberArray(int sessionType,string fromUid,string uidArr,string roomName) ;
    static void setChatRoomFounder(string groupId,string founderUid) ;
    static void notifyUserUids(string uidStr,string lastUpdateTimeStr,int type = 0);
    static void trackExceptionOnFB(string exceptionType, string stackInfo);
    static string getChannelInfo();
    static string getMailLastUpdateTime();
    static void stopReturnToChat();
    static void postIsChatRoomMemberFlag(string groupId,bool flag);
    static void postChannelInfo(string newMsgInfo);
    static void handleGetNewMailMsg(string newMsgInfo);
    static void notifyChatRoomNameChanged(string name);
    static void deleteChatRoom(string groupId);
    static void deleteMail(string mailId,int channelType,int type);
    static void updateChannelMemberArray(int channelType,string fromUid,string uidArr,bool isAdd);
    static void notifySearchedUserInfo(int index);
    static void setAutoTranlateEnable();
    static void setDisableLang(std::string disableLang);
    static void notifyMailDataIndex(int mailDataIndex, bool isGetNew);
    static bool isDontKeepActivitiesEnabled();
    static void gotoDevelopmentSetting();
    static void showChannelListFrom2dx(bool isGoBack = false);
    static void setMailRewardStatus(string mailId);
    static void postMailUpdate(string updateData);
    static void setMailSave(string mailId,int saveFlag);
    static void postMailDeleteStatus(string mailUid);
    static void postMailParseStart();
    static void postMailDealStatus(string mailUid);
    static void postTranslatedResult(string translatedRet);
    static void postTranslateByLuaStart();
    static void postUIShow();
    static void setContactModState();
    static void setMailNewUIEnable(bool enabled);
    static bool isStickMsg(string msg);
    static void setMutiMailRewardStatus(string mailUids);
    static void setMailSortType(int sortType);
    static void setDefaultTranslateEnable(bool isEnable);
    static void setFriendEnable(bool isEnable);
    static void setDetectInfoEnable(bool isEnable);
    static void setStandaloneServerEnable(int index, bool isEnable);
    static void rmDataBaseFile();
    static void setMutiMailStatusByConfigType(string mailUids,int configType,bool isUnLock = false);
    static void setMutiMailStatusByType(int type,int configType,bool isUnLock = false);
    static void getDetectMailByMailUid(string mailUid);
    static void loadMoreMailFromAndroid(string channelId);
    static void setChannelPopupOpen(string channelId);
    static void postMutiRewardItem(string json);
    static void postRedPackageGotUids(string uids);
    static void postRedPackageStatus(string uids,int status);
    static string getFriendLatestMails(string uids);
    static void postRedPackageDuringTime(int time);
    static void postKingUid(string kingUid);
    static void postBanTime(string banTime);
    static void postShieldUids(string shieldUids);
    static void postServerType();
    static string getChatLatestMessage();
    static void showAllianceDialog();
    static void postAddedMailListMail(string mailUid);
    static string getNeighborMail(string mailUid,int type);
    static void postSwitch(string switchKey,string switchValue);
    static void postPlayerLevel();
    
    static int m_channelType;
    static bool m_isInMailDialog;
    static int m_curPopupWindowNum;
    static bool m_rememberPosition;
    static int m_curSendChatIndex;      //当前发送的消息体序号
    static int m_curSendMailIndex;      //当前发送的邮件体序号
    static int m_curAllianceMemberIndex;      //当前发送的联盟成员序号
    static int m_curUserInfoIndex;      //当前发送的用户信息序号
    static bool m_isNoticItemUsed;       //是否使用了喇叭道具
    static int m_autoTranslateWithGoogle;      //是否对没有译文的历史消息进行google翻译
    static std::string m_curMailUid;
    // static bool m_returnToChannelListNextTime; //从系统邮件打开邮件界面，从邮件返回后，退出系统邮件的时候，需要回到邮件列表界面

#endif
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "CommandBase.h"
USING_NS_CC;
class JNIScheduleObject : public CCObject
{
public:
    JNIScheduleObject();
    ~JNIScheduleObject();
    static JNIScheduleObject* getInstance();
    void goBackToNewMailList();
    void refreshUI(float time);
    void handleResume(float time);
    void showLatestMessage(float time);
    void flyMutiMailReward(float time);
    void updateMailCell(float time);
    void refreshMailWriteName(float time);
    void postNotifyUIRefresh(float time);
    void getMultiUserInfo(float time);
    void popupCloseEvent(CCObject *params);
    void postRetrunGame(float time);
    int chatType;
    std::string actionAfterResume;
    std::string uid;
    std::string name;
    std::string attachmentId;
    bool returnToChatAfterPopup;
    bool waitingForReturnToChatAfterPopup;
    void stopReturnToChat();
    void confirmDisableDontKeepActivities();
    void onConfirmDisableDontKeepActivities();
};
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "CommandBase.h"
USING_NS_CC;
class IOSScheduleObject : public CCObject
{
public:
    IOSScheduleObject();
    ~IOSScheduleObject();
    static IOSScheduleObject* getInstance();
    void goBackToNewMailList();
    void handleResume(float time);
    void showLatestMessage(float time);
    void updateMailCell(float time);
    void popupCloseEvent(CCObject *params);
    int chatType;
    std::string actionAfterResume;
    std::string uid;
    std::string name;
    std::string attachmentId;
    std::string reportUid;
    std::string detectReportUid;
    int equipId;
    bool returnToChatAfterPopup;
    bool waitingForReturnToChatAfterPopup;
    void stopReturnToChat();
    void resetResume();
};
#endif
#endif
