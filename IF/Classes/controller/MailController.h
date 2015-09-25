//
//  MailController.h
//  IF
//
//  Created by 李锐奇 on 13-10-28.
//
//

#ifndef IF_MailController_h
#define IF_MailController_h

#include "CommonInclude.h"
#include "MailResourceCellInfo.h"
#include "MailMonsterCellInfo.h"
#include "MailResourceHelpCellInfo.h"
#include "MailDialogInfo.h"
#include "MailInfo.h"
#include <spine/Json.h>

#define MAIL_LIST_CHANGE "mailListChange"
#define MAIL_SAVE_LIST_CHANGE "mailsaveListChange"
#define MAIL_CONTENT_READ "mailContentRead"
#define MAIL_RELOAD_MORE "mailreloadmore"
#define MAIL_LIST_ADD "mailListAdd"
#define MAIL_LIST_DELETE "mailListDelete"
#define MAIL_LIST_DELETERECOVER "mailListdeleterecover"

#define MAIL_EDIT_MSG "mail_edit_msg"
#define MAIL_SEL_MGS "mail_sel_msg"
#define MAIL_DEL_MSG "mail_del_msg"
#define MAIL_DEL_ALL_MSG "mail_del_all_msg"
//#define MAIL_FST_DEL_ALL_MSG "mail_fst_del_all_msg"
#define MAIL_PERSON_CHAT_CHANGE "mailPerson_Chat_Change"
#define MAIL_RESOURCE_LIST_CHANGE "mailResource_List_Change"
#define MAIL_MONSTER_LIST_CHANGE "mailMonster_List_Change"
#define MAIL_SEND_SUCCESS "mail_send_success"
#define MAIL_READ_MAIL_SUCCESS "mail_read_mail_success"
#define MAIL_SEND_GIFT_SUCCESS "mail_send_gift_success"
#define CHAT_ROOM_NAME_REFRESH "mail.room.name.refresh"
enum MailType {
    MAIL_SELF_SEND, //0
    MAIL_USER,      //1
    MAIL_SYSTEM,    //2
    MAIL_SERVICE,   //3
    MAIL_BATTLE_REPORT,//4
    MAIL_RESOURCE,     //5
    MAIL_DETECT,        //6
    MAIL_GENERAL_TRAIN,//7
    MAIL_DETECT_REPORT,//8
    MAIL_ENCAMP,//9
    MAIL_FRESHER,//10
    MAIL_WOUNDED,//11
    MAIL_DIGONG,//12
    ALL_SERVICE,//13
    WORLD_NEW_EXPLORE,//14
    MAIL_SYSNOTICE,//15
    MAIL_SYSUPDATE,//16
    MAIL_ALLIANCEINVITE,//17
    MAIL_ATTACKMONSTER,//18
    WORLD_MONSTER_SPECIAL,//19

    MAIL_Alliance_ALL,
    MAIL_RESOURCE_HELP,

    MAIL_PERSONAL,
    MAIL_MOD_PERSONAL,
    MAIL_MOD_SEND,
    MAIL_ALLIANCEAPPLY,//25
    MAIL_INVITE_TELEPORT,//26
    MAIL_ALLIANCE_KICKOUT,//27
    MAIL_GIFT,//28
    MAIL_DONATE,// 29
    MAIL_WORLD_BOSS,//30
    CHAT_ROOM,   //31
    MAIL_ACTIVITY,//32
    MAIL_REFUSE_ALL_APPLY, //33 拒绝申请加入联盟
    };

enum ChannelMsgType{
    CHANNEL_TYPE_COUNTRY=0,
    CHANNEL_TYPE_ALLIANCE,
    CHANNEL_TYPE_USER,
    CHANNEL_TYPE_CHATROOM,
    CHANNEL_TYPE_OFFICIAL,
    
};

class MailController : public CCObject{
public:
    static MailController* getInstance();
    MailController();
    ~MailController();
    void updateMailList();
    void handleMailMessage(MailInfo* mailInfo,MailDialogInfo* mailDialogInfo,bool isNewMsg=false,int msgType=CHANNEL_TYPE_USER);  //处理android邮件的消息
    void handleMailMessageForOpen(MailInfo* maiInfo);
    void showMailPopupFromAnroid(MailInfo* mailInfo);
    void notifyMailMsgToAndroid(CCArray* mailInfoArr,string fromUid,string customName="",bool isModMsg = false);
    void notifyMailMsgToIOS(CCArray* mailInfoArr,int channelType,string fromUid);
    //count 表示当前已对话多少封邮件 偏移量写死 20条
    void requestMoreMail(string fromUid,string uid,int count);
    
    
    CCDictionary* m_mailInfoSendDic;        //用于通过JNI向android传送邮件消息，key对应消息序号，value对应消息体，消息体可能有多条消息也可能是一条消息
    CCDictionary* m_allianceMemberInfoDic;        //用于通过JNI向android传送的联盟成员，key对应成员信息序号，value对应成员信息
    CCDictionary* m_mailDataDic;        //用于通过JNI向android传送邮件数据包括所有类型邮件，key对应消息序号，value对应邮件MailInfo对象数组，对象数组可能有多个对象也可能是一个对象
    
    void addMails(CCArray* arr,bool isreadContent=false);
    void addMail(CCDictionary* dic,bool isreadContent=false,bool postNotification = true);
    void removeMail(std::string uids, std::string type,int index=-1);
    void removeDialogMail(std::string fromUid, std::string mailUid,int index=-1);
    void removeResourceMail(std::string mailUid,int index=-1);
    void addMailFromAndroidToList(cocos2d::CCDictionary *dic,bool isreadContent);

    int getCountByType(int type);
    void removeMail(std::string uids);
    bool sendMail(std::string toName, std::string title, std::string contents, std::string allianceId="", std::string mailUid="",bool isflag = false, int type = MAIL_SELF_SEND,std::string thxMail = "",bool showTip=true);
    bool sendMailToServer(std::string toName, std::string title, std::string contents, std::string sendLocalTime, std::string allianceId="", std::string mailUid="",bool isflag = false, int type = MAIL_SELF_SEND,std::string targetUid = "",std::string thxMail = "",bool showTip=true);
    void readMailFromServer(std::string uid, std::string type);
    void refreshMailContent(std::string uid, std::string content);
    void refreshMailContent(std::string uid, CCDictionary *dict);
    void refreshGeneralTrainConten(std::string uid, CCDictionary *dict);
    void refreshDetectContent(std::string uid, CCDictionary *dict);
    void refreshOcupyContnet(std::string uid, CCDictionary *dict);
    void refreshDiGongContnet(std::string uid, CCDictionary *dict);
    void refreshAttackMonsterContnet(std::string uid, CCDictionary *dict);
    void refreshExploreContnet(std::string uid, CCDictionary *dict);
    void refreshAllianceInviteContnet(std::string uid, CCDictionary *dict);
    void refreshMailDialogContent(std::string uid, CCDictionary *dict);
    void refreshAllianceApplyContnet(std::string uid, CCDictionary *dict);
    void refreshInviteTeleportContent(std::string uid, CCDictionary *dict);
    void refreshRefuseAllApplyContent(std::string uid, CCDictionary * dict);
    void refreshAllianceKickOutContent(std::string uid, CCDictionary *dict);
    void refreshWorldBossConten(std::string uid, CCDictionary *dict);
    void addOneDialogToMail(MailInfo* mailInfo,CCDictionary *dict,bool isNotify = true);
    void addOneDialogToMail(CCDictionary *dict,std::string mailUid);
    void addOneDialogToMailEnd(CCDictionary *dict,std::string mailUid,int msgType=CHANNEL_TYPE_USER);
    
    void addOneResourceToMail(CCDictionary *dict,std::string mailUid);
    void addOneResourceToMail(CCDictionary *dict,MailResourceCellInfo* mailInfo);
    
    void addOneResourceHelpToMail(CCDictionary *dict,std::string mailUid);
    void addOneResourceHelpToMail(CCDictionary *dict,MailResourceHelpCellInfo* mailInfo);
    
    void addOneMonsterToMail(CCDictionary *dict,std::string mailUid);
    void addOneMonsterToMail(CCDictionary *dict,MailMonsterCellInfo* mailInfo);

    bool isMailFull(int type);
    void readMailContent(int type);
    void reloadMailMore(int type,int num,int offset);
    void notyfyReadMail(std::string uid, int type);
    void notyfyReadChatMail(std::string fromUser,bool isMod);
    CCArray* getSortMailByTime(CCArray* data);
    int getMailTabTypeByInfo(MailInfo* mail);
    void removeMailDialog(std::string mailUid,std::string dialogUid,std::string type);
    void removeOneMailResource(std::string mailUid,std::string dialogUid,std::string type);
    void openMailDialogViewFirst(std::string fromName,std::string fromUid, string modLanguage="", int type = MAIL_SELF_SEND);
    void pushMailDialogFirst(CCDictionary* dic);
    
    void pushMailResourceFirst(CCDictionary *dict);
    void pushMailMonsterFirst(CCDictionary *dict);
    void pushMailResourceHelpFirst(CCDictionary *dict);
    
    bool CanRemoveMail;
    
    void addToOpMails(std::string mailUid, int mailType = 0);
    void removeFromOpMails(std::string mailUid, int mailType = 0);
    void clearOpMails();
    bool canOp();
    bool canOpReward();
    bool isInOpMails(std::string mailUid,int mailType=0);
    void removeAllOpMails();
    void endRemoveAllOpMails(CCDictionary *dict);
    void rewardAllOpMails();
    void endRewardAllOpMails();
    
    void initTranslateMails();
    void backInitTranslateMails(CCArray* arr,std::string refuids);
    
    void showMailWarning();
    std::string getAddDlgByNum(std::string numStr);
    
    void showMailTips(string mailId);
    void openMailReadPop(string mailId);
    string calculateKillandLoss(MailInfo* m_mailInfo);
    
    //聊天室相关
    void createChatRoom(std::string content);
    /**
     *
     *  @param  memberName = 所有选中成员的名字 顿号分割
     *          memberUidArray ＝ 所有选中成员名字uid的集合
     *          roomName = 聊天室房间名称
     *          content = 初始化聊天室的第一句话 (创建聊天室时文本框内容)
     */
    void createChatRoom(std::string memberName,CCArray* memberUidArray,std::string roomName,std::string content);
    void inviteChatRoomMembers(std::string memberName,CCArray* memberUidArray,std::string groupId);
    void kickChatRoomMembers(std::string memberName,CCArray* memberUidArray,std::string groupId);
    void modifyChatRoomName(std::string name,std::string groupId);
    void sendChatRoomMsg(std::string msg,std::string groupId,std::string sendLocalTime = "");
    void quitChatRoom(std::string groupId);
    void getChatRoomMsgRecord(bool isShowing,std::string groupId);
    void requestChatRoomMsgRecord(bool isShowing,int start,int count,std::string groupId);
    void initChatRoomList();
    MailDialogInfo* parseChatRoomDic(CCDictionary* dict,bool isNewMsg,bool isGetAllChatRoom=false,bool isPush=false);
    CCDictionary* parseChatRoomJsonMsg(Json* jsonParent,Json* bodyJson);
    CCArray* m_chatRoomMemberArr;   //聊天室成员Array
    void getAllianceMember();
    std::string m_chatRoomName;
    std::string m_inviteMemberName;
    std::string m_inviteMemberUid;
    void setChatRoomInfo(std::string roomName,std::string memberName,std::string memberUid);
    void quitAllChatRoom();
    void removeChatRoomDialog(string groupId,int index);
    void removeSlectChatRoom(string groupId);
    void getChatRoomMember(string groupId);
    void deleteChatRoomById(string groupId);
    void searchPlayer(string name);
    
    void addLocalTranslate(MailDialogInfo *cInfo);
    void onMailLocalTranslate(CCHttpClient *client, CCHttpResponse *response);
    void addFromUidsToOpMail(std::string fromUid);
    void removeFromUidFromOpMails(std::string fromUid);
    void deleteMailFromAndroid(int tabType,int type,std::string mailUid,std::string fromUid);
    void deleteMailBySelectFromAndroid(std::string mailUidStr,std::string types);
    void getUpdateMail(std::string modifyTime);
    string getPointByOccupyIdx(int index);
    void parseBattleContentAbility(CCArray* genArr);
    CCArray* parseWarEffect(CCArray* effectArr);
    void parseAbility(CCDictionary* dic);
private:
    CC_SYNTHESIZE(int, m_newMailUnreadNum, NewMailUnreadNum);
    CC_SYNTHESIZE(bool, m_isSearchUser, IsSearchUser);
    CC_SYNTHESIZE(CCDictionary*, m_tempChatRoomMsgDic, TempChatRoomMsgDic);
    CC_SYNTHESIZE(CCArray*, m_chatRoomIdArray, ChatRoomIdArray);
    CC_SYNTHESIZE(bool, m_isChatRoomEnable, ChatRoomEnable);
    CC_SYNTHESIZE(bool, m_isNewMailListEnable, IsNewMailListEnable);
    CC_SYNTHESIZE(bool, m_isNewMailUIEnable, IsNewMailUIEnable);
    CC_SYNTHESIZE(MailInfo*, m_oldOpenMailInfo, OldOpenMailInfo);
    void exitMailDialog(MailInfo* mailInfo);
    void addMailToList(CCDictionary* dic,bool isreadContent=false);
    void removeMailFromServer(std::string uids, int count);
    void removeAllMailFromServer();
    int mailDialogUid;//用于首次发邮件给对方的
    MailResourceCellInfo* createResourceCellInfo(CCDictionary* dic);
    
    std::vector<std::string> mOpMails;
    std::vector<std::string> mOpMailFromUids;
    std::vector<int> mOpFolderMails;
    int updateMailCount(std::string mailUid);
    int updateMailCountByType(int type);
    bool m_isTransInit;
    bool m_isShowWarning;
    
    enum RoomChatType{
        MK,             //创建
        INVITE,     //邀请
        KICK,         //踢人
        QUIT,           //退出
        CHAT,          //消息
        NAME        //改名
    };
    

    MailDialogInfo *mTransMailDialog;
    vector<MailDialogInfo*> mTransQueue;
    void localTranslate();
    CC_SYNTHESIZE(int, m_curNotifyMailDataIndex, CurNotifyMailDataIndex);
    
};


#endif
