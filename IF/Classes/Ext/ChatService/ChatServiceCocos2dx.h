//
//  ChatServiceCocos2dx.h
//  ChatService Cocos2dx SDK
//

#ifndef ChatServiceCocos2dx_h
#define ChatServiceCocos2dx_h

#include "cocos2d.h"
#include <string>
#include "ChatServiceMessageListener.h"

using namespace std;

class ChatServiceCocos2dx
{
public:
    static ChatServiceMessageListener* sendMessageListener;
    static bool isChatShowing;
    static bool isForumShowing;
    static bool  enableNativeMail;
    static bool enableChatRoom;
    static bool enableNativeChat;
    static bool isChatDialogNeedBack;
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
    //弹出提示窗
    static void flyHint(std::string icon, std::string titleText, std::string contentText, float time, float dy, bool useDefaultIcon);
    //系统倒计时弹出框
    static void flySystemUpdateHint(double countDown,bool isFlyHintLogin, bool isLogin, string tip, string icon);
    /**退出联盟时重置联盟信息*/
    static void resetPlayerIsInAlliance();
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
    static bool  DB_UserInfo_switch;//DB 用户信息存储开关
    static bool  DB_ChatChannel_switch;//DB chatChannel存储
    static bool    DB_MsgItem_switch;//DB msgItem存储开关
    static bool  Mail_OC_Native_Enable;//iOS 原生邮件开关
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
    

    
    static void resetPlayerFirstJoinAlliance();
    
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
    
    //系统邮件
    static void pushSystemMailMsgWithMsgArray(cocos2d::CCArray *vSystemMailArray);
    //聊天室和邮件
    static void  pushChatMailMsgWithArray(cocos2d::CCArray *vChatArray);
    //比较版本
    static bool compareVersionWithKeyVersionString( string vKeyString);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	static void setSendMessageListener(ChatServiceMessageListener* messageListener);
    //显示android聊天
    static void showChatActivityFrom2dx();
    static void showForumFrom2dx(const char* url);
    static void showMemberSelectorFrom2dx();
    static void handleChatPush(bool isNewMsg,bool isSelf,int  type,int  post,int  gmod,const char* vip,const char* time,const char* uid,const char* name,const char* asn,const char* allianceId,const char* msg,const char* translateMsg,const char*  headPic,const char* battleReportUid,const char* detectReportUid,const char* originalLang) ;
    static void exitChatActivityFrom2dx();
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
    static void initDatabase();
    static void deleteUserMail(string fromUid);
    static void flyHint(std::string icon, std::string titleText, std::string contentText, float time, float dy, bool useDefaultIcon);
    static void flySystemUpdateHint(double countDown,bool isFlyHintLogin, bool isLogin, string tip, string icon);
    static void resetPlayerFirstJoinAlliance();
    static void postNoMoreMessage(int sessionType);
    static void postChannelNoMoreData(int sessionType,bool hasNoMoreData);
    static void clearCurMailData();
    static void setChatHorn();
    static bool getHasRequestDataBefore(string fromUid);
    static void onJoinAnnounceInvitationSuccess();
    static void notifyUserInfo(int index);
    static void setChannelMemberArray(int sessionType,string fromUid,string uidArr,string roomName) ;
    static void setChatRoomFounder(string groupId,string founderUid) ;
    static void notifyUserUids(string uidStr,string lastUpdateTimeStr);
    static void trackExceptionOnFB(string exceptionType, string stackInfo);
    static string getChannelInfo();
    static string getMailLastUpdateTime();
    static int m_channelType;
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
    static void setGlobalMailCount(int noticeR,int studioR,int fightR,int modR);
    static void showChannelListFrom2dx(bool isGoBack = false);
    static void setMailRewardStatus(string mailId,int type);
    static void postMailUpdate(string updateData);
    static void setMailSave(string mailId,int type,int saveFlag);
    static void postMailDeleteStatus(string mailUid);
    static void postMailParseStart();
    static void postMailDealStatus(string mailUid);
    static void postTranslatedResult(string translatedRet);
    static void postTranslateByLuaStart();
    static void postUIShow();
    static void setContactModState();
    static void setMailNewUIEnable(bool enabled);
    
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
    void updateMailCell(float time);
    void refreshMailWriteName(float time);
    void getMultiUserInfo(float time);
    void popupCloseEvent(CCObject *params);
    int chatType;
    std::string actionAfterResume;
    std::string uid;
    std::string name;
    std::string attachmentId;
    bool returnToChatAfterPopup;
    bool waitingForReturnToChatAfterPopup;
    void stopReturnToChat();
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
    
    void handleResume(float time);
    void showLatestMessage(float time);
    void updateMailCell(float time);
    void popupCloseEvent(CCObject *params);
    int chatType;
    std::string actionAfterResume;
    std::string uid;
    std::string name;
    std::string reportUid;
    std::string detectReportUid;
    bool returnToChatAfterPopup;
    bool waitingForReturnToChatAfterPopup;
    void stopReturnToChat();
    void resetResume();
};
#endif
#endif
