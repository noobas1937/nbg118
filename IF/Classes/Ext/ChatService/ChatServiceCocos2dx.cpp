#include "ChatServiceCocos2dx.h"
#include "ChatController.h"
#include "GlobalData.h"
#include "CCDirector.h"
#include "CCSafeNotificationCenter.h"
#include "PopupViewController.h"
#include "LocalController.h"
#include "WorldController.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "UIComponent.h"
#include "MailController.h"
#include "LocalController.h"
#include "VipUtil.h"
#include "ChatMailInfo.h"
#include "ToolController.h"
#include "CCCommonUtils.h"
#include "HFUrlImageCache.h"
#include "UploadImageController.h"
#include "YesNoDialog.h"
#include "CCTypesExt.h"
#include "CCSafeNotificationCenter.h"
#include "GetMsgBySeqIdCommand.h"
#include "NetController.h"
#include "LocalController.h"
#include "RewardController.h"
#include "LuaController.h"
//#include "FriendsView.h"
//#include "NetController.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include <android/log.h>
#include "platform/android/jni/JniHelper.h"

#define J2CSTRING(_cstring, _jstring)    \
string _cstring; \
if(env!=NULL && _jstring != NULL){ \
    _cstring = env->GetStringUTFChars(_jstring, NULL); \
}else{ \
    _cstring = ""; \
    string stackInfo = CCString::createWithFormat("%s(%s:%d)", __FUNCTION__, __SHORT_FORM_OF_FILE__, __LINE__)->getCString(); \
    ChatServiceCocos2dx::trackExceptionOnFB("GetStringUTFChars", stackInfo); \
}

//聊天消息结构体
struct MsgItem
{
    string mailId;              //标识邮件的id
    bool isNewMsg;      //是否是新消息
    int channelType;		//会话类型
    string msg;		//消息体
    string translateMsg;	//翻译信息
    //    string time;		//时间
    int createTime;		//时间
    string name;		//发送者名称
    string vip;		//vip信息
    string headPic;	//头像链接
    bool isSelfMsg;	//是否是自己的信息
    string uid;		//uid
    string asn;		//联盟简称
    int post;		//是否为系统信息，“0”表示不是，非“0”表示是
    int gmod;		//gm和mod信息，如果为"2"、"4"、"5"表示为mod，如果为“3”表示为gm，
    //当为mod时名字颜色为(65,119,41)，如果为gm时名字颜色为(33,111,169),否则名字颜色为(111,26,0)
    //mod为"2"、"4"、"5"时对应的图片分别为mod.png、tmod.png、smod.png，gm为3时对应的图片为gm.png，其他的不显示mod和gm
    string attachmentId;
    //    string allianceId;	//联盟ID
    //    string reportUid;		//战报UID
    //    string detectReportUid;	//侦察战报UID
    string originalLang;	//源语言
    int headPicVer;     //自定义头像使用，>=1 && <=999999表示使用自定义头像
    int sendLocalTime;     //本地发送时间戳
    int sequenceId;
    int lastUpdateTime;
    //    int hornItemId;     //喇叭ItemId
};

//联盟成员信息
struct UserInfo
{
    int allianceRank;
    string uid;
    string userName;
    string headPic;
    int headPicVer;
    int mGmod;
    int vipLevel;
    int vipEndTime;
    string asn;
    int lastUpdateTime;
    string allianceId;
    int serverId;
    int crossFightSrcServerId; // 跨服战时的原服id，若为-1表示没有跨服
    string lang;
};

//邮件数据
struct MailData
{
    int type;
    int createTime;
    int status;
    int reply;
    int rewardStatus;
    int itemIdFlag;//1需要读语言文件
    int save;//0未保存,1保存,2删除保存过
    int mbLevel;
    
    std::string uid;
    std::string title;
    std::string contents;
    std::string fromName;
    std::string fromUid;
    std::string rewardId;
};

static void (*alertToRateAppListener) (int result);
bool ChatServiceCocos2dx::isChatShowing = false;
bool ChatServiceCocos2dx::databaseInited = false;
bool ChatServiceCocos2dx::initChatServiceLater = false;
bool ChatServiceCocos2dx::isForumShowing = false;
bool ChatServiceCocos2dx::isTranslationWebViewShowing = false;
bool ChatServiceCocos2dx::enableNativeChat = true;
int ChatServiceCocos2dx::m_channelType=-1;
bool ChatServiceCocos2dx::m_isNoticItemUsed=false;
bool ChatServiceCocos2dx::m_isInMailDialog=false;
bool ChatServiceCocos2dx::enableNativeMail=false;
bool ChatServiceCocos2dx::isChatDialogNeedBack=false;
bool ChatServiceCocos2dx::m_rememberPosition=false;
bool ChatServiceCocos2dx::useWebSocketServer=false;
bool ChatServiceCocos2dx::isRecieveFromWebSocket=false;
bool ChatServiceCocos2dx::isSendFromWebSocket=false;
int ChatServiceCocos2dx::m_curPopupWindowNum=0;
int ChatServiceCocos2dx::m_curSendChatIndex=0;
int ChatServiceCocos2dx::m_curSendMailIndex=0;
int ChatServiceCocos2dx::m_curUserInfoIndex=0;
int ChatServiceCocos2dx::m_curAllianceMemberIndex=0;
bool ChatServiceCocos2dx::enableChatRoom=true;
string ChatServiceCocos2dx::m_curMailUid="";
bool ChatServiceCocos2dx::isAccountChanged = false;
int ChatServiceCocos2dx::m_autoTranslateWithGoogle=0;
string ChatServiceCocos2dx::uidStr="";

ChatServiceMessageListener* ChatServiceCocos2dx::sendMessageListener = NULL;

void ChatServiceCocos2dx::setSendMessageListener(ChatServiceMessageListener* messageListener) {
	CCLOG("ChatServiceCocos2dx::setSendMessageListener");
		ChatServiceCocos2dx::sendMessageListener = messageListener;
}

void ChatServiceCocos2dx::reset()
{
    ChatServiceCocos2dx::resetPlayerIsInAlliance();
    //    ChatServiceCocos2dx::clearCountryMsg();
    //    ChatServiceCocos2dx::clearMailMsg();
    ChatServiceCocos2dx::onPlayerChanged();
    ChatServiceCocos2dx::m_curSendChatIndex=0;
    ChatServiceCocos2dx::m_curSendMailIndex=0;
    ChatServiceCocos2dx::m_curAllianceMemberIndex=0;
    ChatServiceCocos2dx::m_curUserInfoIndex=0;
    ChatServiceCocos2dx::postChannelNoMoreData(0,false);
    ChatServiceCocos2dx::postChannelNoMoreData(2,false);
    ChatServiceCocos2dx::postChannelNoMoreData(3,false);
    
    ChatServiceCocos2dx::databaseInited = false;
    ChatServiceCocos2dx::initChatServiceLater = false;
}

void ChatServiceCocos2dx::initDatabase(bool isNewUser)
{
    CCLOG("ChatServiceCocos2dx::initDatabase()");
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "org/hcg/IF/IF",
                                                             "initDatabase",
                                                             "(ZZ)V");
    
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, (jboolean)ChatServiceCocos2dx::isAccountChanged, (jboolean)isNewUser);
        minfo.env->DeleteLocalRef(minfo.classID);
        if(ChatServiceCocos2dx::isAccountChanged){
            ChatServiceCocos2dx::isAccountChanged = false;
        }
    }
}

void ChatServiceCocos2dx::completeInitDatabase()
{
    databaseInited = true;
    if(initChatServiceLater) {
        initChatService();
        initChatServiceLater = false;
    }
}

// 重登陆会第二次调用，但必然和前面两个函数依次调用
void ChatServiceCocos2dx::initChatService()
{
    if(!databaseInited) {
        initChatServiceLater = true;
        return;
    }
    
    ChatServiceCocos2dx::setPlayerInfo();
    auto &playerInfo = GlobalData::shared()->playerInfo;
    ChatServiceCocos2dx::setPlayerAllianceInfo(playerInfo.allianceInfo.shortName.c_str(),playerInfo.getAllianceId().c_str(),playerInfo.allianceInfo.rank,playerInfo.isfirstJoin);
    
    ChatServiceCocos2dx::setGameLanguage();
    ChatServiceCocos2dx::setAutoTranlateEnable();
    ChatServiceCocos2dx::notifyChangeLanguage();
    if(ChatServiceCocos2dx::enableNativeChat){
        string param = ChatServiceCocos2dx::getChannelInfo();
        ChatController::getInstance()->getNewMsg(param);
    }
}

void ChatServiceCocos2dx::setPlayerAllianceInfo(const char* asn,const char* allianceId,int allianceRank,bool isFirstJoinAlliance) {
    if(!enableNativeChat) return;
    
    if (asn==NULL || allianceId==NULL || allianceRank<0) {
        CCLOG("setPlayerAllianceInfo error");
        return;
    }
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setPlayerAllianceInfo",
                                                             "(Ljava/lang/String;Ljava/lang/String;IZ)V");
    if(hasMethod) {
        CCLOGFUNC("has method setPlayerAllianceInfo");
         jstring asnStr = minfo.env->NewStringUTF(asn);
        jstring allianceIdStr = minfo.env->NewStringUTF(allianceId);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        asnStr,allianceIdStr,(jint)allianceRank,(jboolean)isFirstJoinAlliance);
        
        minfo.env->DeleteLocalRef(asnStr);
        minfo.env->DeleteLocalRef(allianceIdStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::notifyUserInfo(int index){
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "notifyUserInfo",
                                                             "(I)V");
    if(hasMethod) {
        CCLOG("notifyUserInfo");
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        (jint)index);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::notifySearchedUserInfo(int index){
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "notifySearchedUserInfo",
                                                             "(I)V");
    if(hasMethod) {
        CCLOG("notifySearchedUserInfo");
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        (jint)index);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::notifyUserUids(string uidStr,string lastUpdateTimeStr,int type){
    if(!enableNativeChat) return;
    CCLOGFUNCF("uidStr:%s  lastUpdateTimeStr:%s  type:%d",uidStr.c_str(),lastUpdateTimeStr.c_str(),type);
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "notifyUserUids",
                                                             "(Ljava/lang/String;Ljava/lang/String;I)V");
    if(hasMethod) {
        CCLOG("notifyUserUids");
        jstring uid = minfo.env->NewStringUTF(uidStr.c_str());
        jstring lastUpdateTime = minfo.env->NewStringUTF(lastUpdateTimeStr.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        uid,
                                        lastUpdateTime,
                                        (jint)type);
        minfo.env->DeleteLocalRef(uid);
        minfo.env->DeleteLocalRef(lastUpdateTime);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::notifyMessageIndex(int chatInfoNo,string msgType,string customName,bool isModMsg)
{
    CCLOG("notifyMessageIndex");
    if(!enableNativeChat) return;
    
    if(chatInfoNo<0 || msgType=="")
    {
        CCLOG("notifyMessageIndex error");
        return;
    }
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "notifyMessageIndex",
                                                             "(ILjava/lang/String;Ljava/lang/String;Z)V");
    if (hasMethod) {
        jstring msgTypeStr = minfo.env->NewStringUTF(msgType.c_str());
        jstring customNameStr = minfo.env->NewStringUTF(customName.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        (jint)chatInfoNo,
                                        msgTypeStr,
                                        customNameStr,
                                        (jboolean)isModMsg);
        minfo.env->DeleteLocalRef(msgTypeStr);
        minfo.env->DeleteLocalRef(customNameStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::notifyMailDataIndex(int mailDataIndex, bool isGetNew)
{
    CCLOGFUNCF("mailDataIndex %d",mailDataIndex);

    if(mailDataIndex<0)
    {
        CCLOGFUNC("notifyMailDataIndex error");
        return;
    }
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "notifyMailDataIndex",
                                                             "(IZ)V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        (jint)mailDataIndex,
                                        (jboolean)isGetNew);
        minfo.env->DeleteLocalRef(minfo.classID);
    }

}

void ChatServiceCocos2dx::setCurrentUserId(string uid)
{
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setCurrentUserId",
                                                             "(Ljava/lang/String;)V");
    if (hasMethod) {
        jstring uidStr = minfo.env->NewStringUTF(uid.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        uidStr);
        minfo.env->DeleteLocalRef(uidStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setIsNewMailListEnable(bool enabled)
{
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setIsNewMailListEnable",
                                                             "(Z)V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, (jboolean) enabled);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setMailNewUIEnable(bool enabled)
{
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setMailNewUIEnable",
                                                             "(Z)V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, (jboolean) enabled);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}


void ChatServiceCocos2dx::setMailSortType(int sortType)
{
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setMailSortType",
                                                             "(I)V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, (jint) sortType);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setDefaultTranslateEnable(bool isEnable)
{
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setDefaultTranslateEnable",
                                                             "(Z)V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, (jboolean) isEnable);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setFriendEnable(bool isEnable)
{
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setFriendEnable",
                                                             "(Z)V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, (jboolean) isEnable);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setDetectInfoEnable(bool isEnable)
{
    CCLOGFUNCF("isEnable  0:%d",isEnable);
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    CCLOGFUNCF("isEnable:%d",isEnable);
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setDetectInfoEnable",
                                                             "(Z)V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, (jboolean) isEnable);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setStandaloneServerEnable(int index, bool isEnable)
{
    if(!enableNativeChat) return;
    
    CCLOGFUNCF("%d isEnable: %d", index, isEnable);
    switch(index)
    {
        case 1:
            ChatServiceCocos2dx::useWebSocketServer = isEnable;
            break;
        case 2:
            ChatServiceCocos2dx::isRecieveFromWebSocket = isEnable;
            break;
        case 3:
            ChatServiceCocos2dx::isSendFromWebSocket = isEnable;
            break;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setStandaloneServerEnable",
                                                             "(IZ)V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, (jint) index, (jboolean) isEnable);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::rmDataBaseFile()
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "rmDataBaseFile",
                                                             "()V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::deleteUserMail(string fromUid)
{
    CCLOGFUNC("deleteUserMail");
    if(!enableNativeChat) return;
    
    if(fromUid=="")
    {
        CCLOGFUNC("fromUid==null");
        return;
    }
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "removeAllMailByUid",
                                                             "(Ljava/lang/String;)V");
    if (hasMethod) {
        jstring fromUidStr = minfo.env->NewStringUTF(fromUid.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        fromUidStr);
        minfo.env->DeleteLocalRef(fromUidStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setPlayerInfo() {
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setPlayerInfo",
                                                             "(IIIILjava/lang/String;Ljava/lang/String;Ljava/lang/String;IIII)V");
    if(hasMethod) {
//        CCLOG("GM flag %d",GlobalData::shared()->playerInfo.gmFlag);
//        CCLOG("userName %s",GlobalData::shared()->playerInfo.name.c_str());
//        CCLOG("Uid %s",GlobalData::shared()->playerInfo.uid.c_str());
        
        string uid=GlobalData::shared()->playerInfo.uid;
        int headPicVer=GlobalData::shared()->playerInfo.picVer;
//        bool isCustomHeadPicExist=false;
//        string customHeadPic="";
//        string customHeadPicUrl="";
//        if(uid!="" && headPicVer <=999999 && headPicVer >=1)
//        {
//            customHeadPicUrl=CCCommonUtils::getCustomPicUrl(uid, headPicVer);
//            
//            if(customHeadPicUrl!="")
//            {
//                CCLOG("customHeadPicUrl %s",customHeadPicUrl.c_str());
//                customHeadPic=HFUrlImageCache::shared()->getCachePath(customHeadPicUrl);
//                CCLOG("customHeadPic %s",customHeadPic.c_str());
//                isCustomHeadPicExist=HFUrlImageCache::shared()->isHeadImgExist(customHeadPic);
//            }
//        }
        
        int vipLevel = VipUtil::getVipLevel(GlobalData::shared()->playerInfo.vipPoints);
        int vipEndTime = GlobalData::shared()->playerInfo.vipEndTime;
        int lastUpdateTime = GlobalData::shared()->playerInfo.lastUpdateTime;
        int crossFightSrcServerId = GlobalData::shared()->playerInfo.crossFightSrcServerId;
        
        jstring nameStr = minfo.env->NewStringUTF(GlobalData::shared()->playerInfo.name.c_str());
        jstring uidStr = minfo.env->NewStringUTF(GlobalData::shared()->playerInfo.uid.c_str());
        jstring headPicStr = minfo.env->NewStringUTF(GlobalData::shared()->playerInfo.pic.c_str());
        
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        (jint)GlobalData::shared()->playerInfo.selfServerId,
                                        (jint)GlobalData::shared()->getTimeStamp(),
                                        (jint)GlobalData::shared()->playerInfo.gmFlag,
                                        (jint)headPicVer,nameStr,uidStr,headPicStr,(jint)vipLevel,(jint)vipEndTime, (jint)lastUpdateTime, (jint)crossFightSrcServerId);
        minfo.env->DeleteLocalRef(nameStr);
        minfo.env->DeleteLocalRef(uidStr);
        minfo.env->DeleteLocalRef(headPicStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::postNoMoreMessage(int channelType) {
    
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "postNoMoreMessage",
                                                             "(I)V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        (jint)channelType);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setChannelMemberArray(int channelType,string fromUid,string uidArr,string roomName) {
    
    if(!enableNativeChat) return;
    CCLOGFUNCF("fromUid %s uidArr %s  roomName %s",fromUid.c_str(),uidArr.c_str(),roomName.c_str());
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setChannelMemberArray",
                                                             "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if(hasMethod) {
        jstring fromUidStr=minfo.env->NewStringUTF(fromUid.c_str());
        jstring uidStr=minfo.env->NewStringUTF(uidArr.c_str());
        jstring roomNameStr=minfo.env->NewStringUTF(roomName.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        (jint)channelType,fromUidStr,uidStr,roomNameStr);
        minfo.env->DeleteLocalRef(fromUidStr);
        minfo.env->DeleteLocalRef(uidStr);
        minfo.env->DeleteLocalRef(roomNameStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::updateChannelMemberArray(int channelType,string fromUid,string uidArr,bool isAdd) {
    
    if(!enableNativeChat) return;
    CCLOGFUNCF("fromUid %s uidArr %s",fromUid.c_str(),uidArr.c_str());
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "updateChannelMemberArray",
                                                             "(ILjava/lang/String;Ljava/lang/String;Z)V");
    if(hasMethod) {
        jstring fromUidStr=minfo.env->NewStringUTF(fromUid.c_str());
        jstring uidStr=minfo.env->NewStringUTF(uidArr.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        (jint)channelType,fromUidStr,uidStr,(jboolean)isAdd);
        minfo.env->DeleteLocalRef(fromUidStr);
        minfo.env->DeleteLocalRef(uidStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setChatRoomFounder(string groupId,string founderUid) {
    
    if(!enableNativeChat) return;
    CCLOGFUNCF("groupId %s,founderUid %s",groupId.c_str(),founderUid.c_str());
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setChatRoomFounder",
                                                             "(Ljava/lang/String;Ljava/lang/String;)V");
    if(hasMethod) {
        jstring founderUidStr=minfo.env->NewStringUTF(founderUid.c_str());
        jstring groupIdStr=minfo.env->NewStringUTF(groupId.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        groupIdStr,founderUidStr);
        minfo.env->DeleteLocalRef(founderUidStr);
        minfo.env->DeleteLocalRef(groupIdStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::postChannelNoMoreData(int channelType,bool hasNoMoreData) {
    
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "postChannelNoMoreData",
                                                             "(IZ)V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        (jint)channelType,
                                        (jboolean)hasNoMoreData);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::postChannelInfo(string newMsgInfo)
{
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "postChannelInfo",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring newMsgInfoStr=minfo.env->NewStringUTF(newMsgInfo.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        newMsgInfoStr);
        minfo.env->DeleteLocalRef(newMsgInfoStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }

}

void ChatServiceCocos2dx::handleGetNewMailMsg(string newMsgInfo)
{
    CCLOG("handleGetNewMailMsg1");
    if(!enableNativeChat) return;
    CCLOG("handleGetNewMailMsg2");
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "handleGetNewMailMsg",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        CCLOG("handleGetNewMailMsg3");
        jstring newMsgInfoStr=minfo.env->NewStringUTF(newMsgInfo.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        newMsgInfoStr);
        minfo.env->DeleteLocalRef(newMsgInfoStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        CCLOG("handleGetNewMailMsg4");
    }
    
}

void ChatServiceCocos2dx::resetPlayerFirstJoinAlliance() {
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "resetPlayerFirstJoinAlliance",
                                                             "()V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::resetPlayerIsInAlliance() {
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "resetPlayerIsInAlliance",
                                                             "()V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::onPlayerChanged() {
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "onPlayerChanged",
                                                             "()V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::showChatInputView(){
    if(!enableNativeChat) return;
	cocos2d::JniMethodInfo minfo;
	if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/inputfield/InputViewServiceInterface","showChatInputView", "()V"))
	{
		return;
	}
	else
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
}

void ChatServiceCocos2dx::hideChatInputView(){
    if(!enableNativeChat) return;
	cocos2d::JniMethodInfo minfo;
	if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/inputfield/InputViewServiceInterface","hideChatInputView", "()V"))
	{
		return;
	}
	else
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
}

void ChatServiceCocos2dx::enableChatInputView(){
    if(!enableNativeChat) return;
	cocos2d::JniMethodInfo minfo;
	if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/inputfield/InputViewServiceInterface","enableChatInputView", "()V"))
	{
		return;
	}
	else
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
}

void ChatServiceCocos2dx::disableChatInputView(){
    if(!enableNativeChat) return;
	cocos2d::JniMethodInfo minfo;
	if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/inputfield/InputViewServiceInterface","disableChatInputView", "()V"))
	{
		return;
	}
	else
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
}

string ChatServiceCocos2dx::getChatInputText(){
    if(!enableNativeChat) return "";
	cocos2d::JniMethodInfo minfo;
	if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/inputfield/InputViewServiceInterface","getChatInputText", "()Ljava/lang/String;"))
	{
		return "";
	}
	else
	{
		jstring msg = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
        JNIEnv *env = minfo.env;
        J2CSTRING(result, msg);
		return result;
	}
}

//android 邮件相关，打开邮件窗口时传递
void ChatServiceCocos2dx::setMailInfo(const char* mailInfoFromUid,const char* mailInfoUid,const char* mailInfoName,int mailInfoType)
{
    if(!enableNativeChat) return;
    
    if (mailInfoFromUid==NULL || mailInfoUid==NULL || mailInfoName==NULL) {
        CCLOG("setMailInfo error");
        return;
    }
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setMailInfo",
                                                             "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
    if(hasMethod) {
        jstring mailInfoFromUidStr = minfo.env->NewStringUTF(mailInfoFromUid);
         jstring mailInfoUidStr = minfo.env->NewStringUTF(mailInfoUid);
        jstring mailInfoNameStr = minfo.env->NewStringUTF(mailInfoName);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        mailInfoFromUidStr,mailInfoUidStr,mailInfoNameStr,(jint)mailInfoType);
        minfo.env->DeleteLocalRef(mailInfoFromUidStr);
        minfo.env->DeleteLocalRef(mailInfoUidStr);
        minfo.env->DeleteLocalRef(mailInfoNameStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::onCreateChatroomSuccess(){
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "onCreateChatroomSuccess", "()V");
    if(hasMethod)
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::notifyChatRoomNameChanged(string name)
{
    if(!enableNativeChat)
        return;
    if (name=="")
        return;
    CCLOGFUNC("");
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "notifyChatRoomNameChanged",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring nameStr = minfo.env->NewStringUTF(name.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        nameStr);
        minfo.env->DeleteLocalRef(nameStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::deleteMail(string mailId,int channelType,int type)
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    if (channelType<0 || type<0)
        return;
    CCLOGFUNC("");
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "deleteMail",
                                                             "(Ljava/lang/String;II)V");
    if(hasMethod) {
        jstring mailIdStr = minfo.env->NewStringUTF(mailId.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        mailIdStr,
                                        (jint)channelType,
                                        (jint)type);
        minfo.env->DeleteLocalRef(mailIdStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setMailRewardStatus(string mailId)
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    if (mailId=="")
        return;
    CCLOGFUNC("");
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setMailRewardStatus",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring mailIdStr = minfo.env->NewStringUTF(mailId.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        mailIdStr);
        minfo.env->DeleteLocalRef(mailIdStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setMutiMailRewardStatus(string mailUids)
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    if (mailUids=="")
        return;
    CCLOGFUNCF("mailUids : %s",mailUids.c_str());
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setMutiMailRewardStatus",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring mailUidsStr = minfo.env->NewStringUTF(mailUids.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        mailUidsStr);
        minfo.env->DeleteLocalRef(mailUidsStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::getDetectMailByMailUid(string mailUid)
{
    CCLOGFUNCF("mailUid : %s",mailUid.c_str());
    if(!MailController::getInstance()->getIsNewMailListEnable() || mailUid=="")
        return;

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "getDetectMailByMailUid",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring mailUidStr = minfo.env->NewStringUTF(mailUid.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        mailUidStr);
        minfo.env->DeleteLocalRef(mailUidStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setMutiMailStatusByType(int type,int configType,bool isUnLock)
{
    CCLOGFUNCF("type : %d  configType:%d  isUnLock:%d",type,configType,isUnLock);
    if(!MailController::getInstance()->getIsNewMailListEnable() || type<0 || configType<1 || configType>3)
        return;
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setMutiMailStatusByType",
                                                             "(IIZ)V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        (jint)type,
                                        (jint)configType,
                                        (jboolean)isUnLock);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setMutiMailStatusByConfigType(string mailUids,int configType,bool isUnLock)
{
    CCLOGFUNCF("mailUids : %s  configType:%d  isUnLock:%d",mailUids.c_str(),configType,isUnLock);
    if(!MailController::getInstance()->getIsNewMailListEnable() || mailUids=="" || configType<1 || configType>3)
        return;
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setMutiMailStatusByConfigType",
                                                             "(Ljava/lang/String;IZ)V");
    if(hasMethod) {
        jstring mailUidsStr = minfo.env->NewStringUTF(mailUids.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        mailUidsStr,
                                        (jint)configType,
                                        (jboolean)isUnLock);
        minfo.env->DeleteLocalRef(mailUidsStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setMailSave(string mailId,int saveFlag)
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    if (mailId=="")
        return;
    CCLOGFUNC("");
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setMailSave",
                                                             "(Ljava/lang/String;I)V");
    if(hasMethod) {
        jstring mailIdStr = minfo.env->NewStringUTF(mailId.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        mailIdStr,
                                        (jint)saveFlag);
        minfo.env->DeleteLocalRef(mailIdStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::deleteChatRoom(string groupId)
{
    if(!enableNativeChat)
        return;
    if (groupId=="")
        return;
    CCLOGFUNC("");
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "deleteChatRoom",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring groupIdStr = minfo.env->NewStringUTF(groupId.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        groupIdStr);
        minfo.env->DeleteLocalRef(groupIdStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setAutoTranlateEnable()
{
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setAutoTranlateMode",
                                                             "(I)V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        jint(m_autoTranslateWithGoogle));
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setDisableLang(string disableLang)
{
    if(!enableNativeChat) return;

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setDisableLang",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring disableLangStr=minfo.env->NewStringUTF(disableLang.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        disableLangStr);
        minfo.env->DeleteLocalRef(disableLangStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setGameLanguage()
{
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setGameLanguage",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        string lang=LocalController::shared()->getLanguageFileName();
        CCLOG("lang: %s",lang.c_str());
        jstring langStr = minfo.env->NewStringUTF(lang.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        langStr);
        minfo.env->DeleteLocalRef(langStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::notifyChangeLanguage(){
    if(!enableNativeChat) return;
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "notifyChangeLanguage",
                                                             "()V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setChatHorn()
{
    if(!enableNativeChat) return;
    
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/elex/chatservice/controller/ServiceInterface",
                                                             "setChatHorn",
                                                             "(Z)V");
    if(hasMethod) {
        
        CCLOG("ChatController::getInstance()->getIsNoticeOpen() %d",ChatController::getInstance()->getIsNoticeOpen());
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        jboolean(ChatController::getInstance()->getIsNoticeOpen()));
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

bool ChatServiceCocos2dx::isDontKeepActivitiesEnabled()
{
    if(!enableNativeChat)
        return false;
    
    cocos2d::JniMethodInfo minfo;
    if(cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                               "com/elex/chatservice/controller/ServiceInterface",
                                               "isDontKeepActivitiesEnabled",
                                               "()Z"))
    {
        return minfo.env->CallStaticBooleanMethod(minfo.classID,
                                                  minfo.methodID);
    }
    
    return false;
}

void ChatServiceCocos2dx::gotoDevelopmentSetting()
{
    cocos2d::JniMethodInfo minfo;
    if(cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                               "com/elex/chatservice/controller/ServiceInterface",
                                               "gotoDevelopmentSetting",
                                               "()V"))
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::showMemberSelectorFrom2dx(){
    if(!enableNativeChat || ChatServiceCocos2dx::isChatShowing) return;
    if(isDontKeepActivitiesEnabled()){
        JNIScheduleObject::getInstance()->confirmDisableDontKeepActivities();
        return;
    }
    
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","showMemberSelectorFrom2dx", "()V"))
    {
        return;
    }
    else
    {
        CCLOG("call showMemberSelectorFrom2dx");
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

//android 聊天相关
void ChatServiceCocos2dx::showChannelListFrom2dx(bool isGoBack){
    if(!enableNativeChat || !MailController::getInstance()->getIsNewMailListEnable()) return;
    if(isDontKeepActivitiesEnabled()){
        JNIScheduleObject::getInstance()->confirmDisableDontKeepActivities();
        return;
    }
    
    cocos2d::JniMethodInfo minfo;
    if(cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                               "com/elex/chatservice/controller/ServiceInterface",
                                               "showChannelListFrom2dx", "(Z)V"))
    {
        JNIScheduleObject::getInstance()->stopReturnToChat();
        ChatServiceCocos2dx::isChatShowing=true;
        MailController::getInstance()->getAllianceMember();
        CCLOGFUNC("");
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,(jboolean)isGoBack);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}


void ChatServiceCocos2dx::showAllianceDialog(){
    if(!enableNativeChat)
        return;
    cocos2d::JniMethodInfo minfo;
    if(cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                               "com/elex/chatservice/controller/ServiceInterface",
                                               "showAllianceDialog", "()V"))
    {
        CCLOGFUNC("");
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}


//android 聊天相关
void ChatServiceCocos2dx::showChatActivityFrom2dx(){
    if(!enableNativeChat || ChatServiceCocos2dx::isChatShowing) return;
    if(isDontKeepActivitiesEnabled()){
        JNIScheduleObject::getInstance()->confirmDisableDontKeepActivities();
        return;
    }

    m_curPopupWindowNum=PopupViewController::getInstance()->getCurrViewCount()+PopupViewController::getInstance()->getGoBackViewCount();
    postServerType();
    postPlayerLevel();
    setGameLanguage();
    setChatHorn();
    
    //setPlayerInfo();
    
    //setPlayerAllianceInfo(GlobalData::shared()->playerInfo.allianceInfo.shortName.c_str(),GlobalData::shared()->playerInfo.getAllianceId().c_str(),GlobalData::shared()->playerInfo.allianceInfo.rank ,GlobalData::shared()->playerInfo.isfirstJoin);

    
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","showChatActivityFrom2dx", "(IIIZZZ)V"))
    {
        return;
    }
    else
    {
        CCLOG("call showChatActivityFrom2dx");
        JNIScheduleObject::getInstance()->stopReturnToChat();
        bool enableCustomHeadImg=UploadImageController::shared()->getUploadImageFlag()==1?true:false;
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,(jint)(ChatController::getInstance()->getNoticeCharMax()), (jint)m_channelType,(jint)(ChatController::getInstance()->chat_interval), (jboolean)m_rememberPosition, (jboolean)enableCustomHeadImg,(jboolean)m_isNoticItemUsed);
        minfo.env->DeleteLocalRef(minfo.classID);
        ChatServiceCocos2dx::isChatShowing=true;
        ChatServiceCocos2dx::m_rememberPosition=false;
    }
}

/**
 * @param url 为NULL时打开论坛首页，否则打开指定的url
 */
void ChatServiceCocos2dx::showForumFrom2dx(const char* url){
    if(!enableNativeChat || ChatServiceCocos2dx::isChatShowing || ChatServiceCocos2dx::isForumShowing) return;
    if(isDontKeepActivitiesEnabled()){
        JNIScheduleObject::getInstance()->confirmDisableDontKeepActivities();
        return;
    }
    
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","showForumFrom2dx", "(Ljava/lang/String;)V"))
    {
        return;
    }
    else
    {
        setGameLanguage();
        
//        setPlayerInfo();
        
        if(url != NULL){
            jstring urlStr = minfo.env->NewStringUTF(url);
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, urlStr);
        }else{
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, NULL);
        }
        minfo.env->DeleteLocalRef(minfo.classID);
        ChatServiceCocos2dx::isChatShowing=true;
        ChatServiceCocos2dx::isForumShowing=true;
    }
}

/**
 * @param url 为NULL时打开翻译优化首页，否则打开指定的url
 */
void ChatServiceCocos2dx::showTranslationOptimizationFrom2dx(const char* url){
    if(!enableNativeChat || ChatServiceCocos2dx::isChatShowing || ChatServiceCocos2dx::isTranslationWebViewShowing) return;
    if(isDontKeepActivitiesEnabled()){
        JNIScheduleObject::getInstance()->confirmDisableDontKeepActivities();
        return;
    }
    
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","showTranslationOptimizationFrom2dx", "(Ljava/lang/String;)V"))
    {
        return;
    }
    else
    {
        setGameLanguage();
        if(url != NULL){
            jstring urlStr = minfo.env->NewStringUTF(url);
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, urlStr);
            minfo.env->DeleteLocalRef(urlStr);
        }else{
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, NULL);
        }
        minfo.env->DeleteLocalRef(minfo.classID);
        ChatServiceCocos2dx::isChatShowing=true;
        ChatServiceCocos2dx::isTranslationWebViewShowing=true;
    }
}

void ChatServiceCocos2dx::exitChatActivityFrom2dx(bool needRemeberActivityStack){
    if(!enableNativeChat) return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","exitChatActivityFrom2dx", "(Z)V"))
    {
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,(jboolean)needRemeberActivityStack);
        minfo.env->DeleteLocalRef(minfo.classID);
        ChatServiceCocos2dx::isChatShowing=false;
        ChatServiceCocos2dx::isForumShowing=false;
        ChatServiceCocos2dx::isTranslationWebViewShowing = false;
    }
}

void ChatServiceCocos2dx::notifyReturn2dxGame(){
    if(!enableNativeChat) return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","onReturn2dxGame", "()V"))
    {
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::postIsChatRoomMemberFlag(string groupId,bool flag)
{
    if(!enableNativeChat) return;
    cocos2d::JniMethodInfo minfo;
    if (groupId=="") {
        return;
    }
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postIsChatRoomMemberFlag", "(Ljava/lang/String;Z)V"))
    {
        return;
    }
    else
    {
         jstring groupIdStr=minfo.env->NewStringUTF(groupId.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,groupIdStr,(jboolean)flag);
        minfo.env->DeleteLocalRef(groupIdStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setSendButtonText(const char* text){
    if(!enableNativeChat) return;
	cocos2d::JniMethodInfo minfo;
	if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/inputfield/InputViewServiceInterface","setSendButtonText", "(Ljava/lang/String;)V"))
	{
		return;
	}
	else
	{
		jstring textStr = minfo.env->NewStringUTF(text);
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, textStr);
        minfo.env->DeleteLocalRef(textStr);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
}

void ChatServiceCocos2dx::setEditTextHintText(const char* text){
    if(!enableNativeChat) return;
	cocos2d::JniMethodInfo minfo;
	if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/inputfield/InputViewServiceInterface","setEditTextHintText", "(Ljava/lang/String;)V"))
	{
		return;
	}
	else
	{
		jstring textStr = minfo.env->NewStringUTF(text);
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, textStr);
        minfo.env->DeleteLocalRef(textStr);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
}

void ChatServiceCocos2dx::toggleFullScreen(bool enabled){
    if(!enableNativeChat) return;
	cocos2d::JniMethodInfo minfo;
	if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","toggleFullScreen", "(Z)V"))
	{
		return;
	}
	else
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, (jboolean)enabled);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
}

void ChatServiceCocos2dx::flyHint(std::string icon, std::string titleText, std::string contentText, float time, float dy, bool useDefaultIcon){
    if(!enableNativeChat) return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","flyHint", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;FFZ)V"))
    {
        return;
    }
    else
    {
        jstring iconStr = minfo.env->NewStringUTF(icon.c_str());
        jstring titleTextStr = minfo.env->NewStringUTF(titleText.c_str());
        jstring contentTextStr = minfo.env->NewStringUTF(contentText.c_str());
        //如果有icon，但找不到对应图片，useDefaultIcon为true则显示"no_iconFlag.png"，为false则隐藏icon
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, iconStr, titleTextStr, contentTextStr, (jfloat)time, (jfloat)dy, (jboolean)useDefaultIcon);
        minfo.env->DeleteLocalRef(iconStr);
        minfo.env->DeleteLocalRef(titleTextStr);
        minfo.env->DeleteLocalRef(contentTextStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::flySystemUpdateHint(double countDown,bool isFlyHintLogin, bool isLogin, string tip, string icon)
{
    if(!enableNativeChat) return;
    if(countDown>0
       || tip==""
       || icon=="")
    {
        CCLOG("flySystemUpdateHint 1");
        return;
    }
    
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","flySystemUpdateHint", "(DZZLjava/lang/String;Ljava/lang/String;)V"))
    {
        CCLOG("flySystemUpdateHint 2");
        return;
    }
    else
    {
        CCLOG("flySystemUpdateHint 3");
        jstring tipStr = minfo.env->NewStringUTF(tip.c_str());
        jstring iconStr = minfo.env->NewStringUTF(icon.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, (jdouble)countDown, (jboolean)isFlyHintLogin, (jboolean)isLogin, tipStr, iconStr);
        minfo.env->DeleteLocalRef(tipStr);
        minfo.env->DeleteLocalRef(iconStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::onJoinAnnounceInvitationSuccess()
{
    if(!enableNativeChat) return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","onJoinAnnounceInvitationSuccess", "()V"))
    {
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::stopReturnToChat()
{
    if(!enableNativeChat) return;
    JNIScheduleObject::getInstance()->stopReturnToChat();
}

void ChatServiceCocos2dx::trackExceptionOnFB(string exceptionType, string functionInfo)
{
    if(!enableNativeChat) return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"org/hcg/stac/empire/sns/FBUtil","appEventException", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    //确保不为空，防止递归调用
    else
    {
        jstring exceptionTypeStr = minfo.env->NewStringUTF(exceptionType.c_str());
        jstring functionStr = minfo.env->NewStringUTF(functionInfo.c_str());
        jstring causeStr = minfo.env->NewStringUTF("");
        jstring messageStr = minfo.env->NewStringUTF("");
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, exceptionTypeStr, functionStr, causeStr, messageStr);
        minfo.env->DeleteLocalRef(exceptionTypeStr);
        minfo.env->DeleteLocalRef(functionStr);
        minfo.env->DeleteLocalRef(causeStr);
        minfo.env->DeleteLocalRef(messageStr);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

string ChatServiceCocos2dx::getChannelInfo()
{
    if(!enableNativeChat) return NULL;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","getChannelInfo", "()Ljava/lang/String;"))
    {
        return NULL;
    }
    else
    {
        jstring result = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        JNIEnv *env = minfo.env;
        J2CSTRING(param, result);
        minfo.env->DeleteLocalRef(minfo.classID);
        return param;
    }
}

string ChatServiceCocos2dx::getMailLastUpdateTime()
{
    if(!enableNativeChat) return NULL;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","getLastMailUpdateTime", "()Ljava/lang/String;"))
    {
        return NULL;
    }
    else
    {
        jstring result = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        JNIEnv *env = minfo.env;
        J2CSTRING(param, result);
        minfo.env->DeleteLocalRef(minfo.classID);
        return param;
    }
}

bool ChatServiceCocos2dx::isStickMsg(string msg)
{
    if(!enableNativeChat) return NULL;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","isStickMsg", "(Ljava/lang/String;)Z"))
    {
        return NULL;
    }
    else
    {
        
        JNIEnv *env = minfo.env;
        jstring msgStr = minfo.env->NewStringUTF(msg.c_str());
        jboolean result = (jboolean)minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID,msgStr);
        minfo.env->DeleteLocalRef(msgStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        return (bool)result;
    }
}

void ChatServiceCocos2dx::postMailUpdate(string updateData)
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    CCLOGFUNC("1");
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postMailUpdate", "(Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring updateDataStr = minfo.env->NewStringUTF(updateData.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, updateDataStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(updateDataStr);
    }
}

void ChatServiceCocos2dx::postMailDealStatus(string mailUid)
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postMailDealStatus", "(Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring mailUidStr = minfo.env->NewStringUTF(mailUid.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, mailUidStr);
        minfo.env->DeleteLocalRef(minfo.classID);
         minfo.env->DeleteLocalRef(mailUidStr);
    }
}

void ChatServiceCocos2dx::postMailDeleteStatus(string mailUid)
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postMailDeleteStatus", "(Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring mailUidStr = minfo.env->NewStringUTF(mailUid.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, mailUidStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(mailUidStr);
    }

}

void ChatServiceCocos2dx::loadMoreMailFromAndroid(string channelId)
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","loadMoreMailFromAndroid", "(Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring channelIdStr = minfo.env->NewStringUTF(channelId.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, channelIdStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(channelIdStr);
    }
}

void ChatServiceCocos2dx::postMailParseStart()
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postMailParseStart", "()V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
    
}

void ChatServiceCocos2dx::postTranslatedResult(string translatedRet)
{
    if(m_autoTranslateWithGoogle != 2)
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postTranslatedResult", "(Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring translatedRetStr = minfo.env->NewStringUTF(translatedRet.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,translatedRetStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(translatedRetStr);
    }
    
}

void ChatServiceCocos2dx::setChannelPopupOpen(string channelId)
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","setChannelPopupOpen", "(Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring channelIdStr = minfo.env->NewStringUTF(channelId.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,channelIdStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(channelIdStr);
    }
}

void ChatServiceCocos2dx::postKingUid(string kingUid)
{
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postKingUid", "(Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring kingUidStr = minfo.env->NewStringUTF(kingUid.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,kingUidStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(kingUidStr);
    }
}

void ChatServiceCocos2dx::postBanTime(string banTime)
{
    if(banTime == "")
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postBanTime", "(Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring banTimeStr = minfo.env->NewStringUTF(banTime.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,banTimeStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(banTimeStr);
    }
}

void ChatServiceCocos2dx::postTranslateByLuaStart()
{
    if(m_autoTranslateWithGoogle != 2)
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postTranslateByLuaStart", "()V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
    
}

void ChatServiceCocos2dx::postUIShow()
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postUIShow", "()V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::setContactModState()
{
    if(!enableNativeChat)
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","setContactModState", "()V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::postRedPackageGotUids(string uids)
{
    if(uids == "")
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postRedPackageGotUids", "(Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring uidsStr = minfo.env->NewStringUTF(uids.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,uidsStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(uidsStr);
    }
}

void ChatServiceCocos2dx::postShieldUids(string shieldUids)
{
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postShieldUids", "(Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring shieldUidsStr = minfo.env->NewStringUTF(shieldUids.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,shieldUidsStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(shieldUidsStr);
    }
}

void ChatServiceCocos2dx::postAddedMailListMail(string mailUid)
{
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postAddedMailListMail", "(Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring mailUidStr = minfo.env->NewStringUTF(mailUid.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,mailUidStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(mailUidStr);
    }
}

/**
 * @params mailUid 当前邮件的mailUid
 * @params type 滑动操作类型，获取上一封为1，获取下一封为2
 */
string ChatServiceCocos2dx::getNeighborMail(string mailUid,int type)
{
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","getNeighborMail", "(Ljava/lang/String;I)Ljava/lang/String;"))
    {
        CCLOGFUNC("2");
        return "";
    }
    else
    {
        jstring mailUidStr = minfo.env->NewStringUTF(mailUid.c_str());
        jstring result = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID,mailUidStr,(jint)type);
        JNIEnv *env = minfo.env;
        J2CSTRING(param, result);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(mailUidStr);
        return param;
    }
}


void ChatServiceCocos2dx::postServerType()
{
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postServerType", "(I)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,(jint)GlobalData::shared()->isCrossService);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::postRedPackageStatus(string uids,int status)
{
    if(uids == "" || status<0)
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postRedPackageStatus", "(Ljava/lang/String;I)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring uidsStr = minfo.env->NewStringUTF(uids.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,uidsStr,(jint)status);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(uidsStr);
    }
}

void ChatServiceCocos2dx::postSwitch(string switchKey,string switchValue)
{
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postSwitch", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring switchKeyStr = minfo.env->NewStringUTF(switchKey.c_str());
        jstring switchValueStr = minfo.env->NewStringUTF(switchValue.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,switchKeyStr,switchValueStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(switchKeyStr);
        minfo.env->DeleteLocalRef(switchValueStr);
    }
}

void ChatServiceCocos2dx::postPlayerLevel()
{
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postPlayerLevel", "(I)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,(jint)GlobalData::shared()->playerInfo.level);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

string ChatServiceCocos2dx::getFriendLatestMails(string uids)
{
    if(uids == "")
        return "";
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","getFriendLatestMails", "(Ljava/lang/String;)Ljava/lang/String;"))
    {
        CCLOGFUNC("2");
        return "";
    }
    else
    {
        jstring uidsStr = minfo.env->NewStringUTF(uids.c_str());
        jstring result = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID,uidsStr);
         JNIEnv *env = minfo.env;
        J2CSTRING(resultStr, result);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(uidsStr);
        return resultStr;
    }
}

string ChatServiceCocos2dx::getChatLatestMessage()
{
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","getChatLatestMessage", "()Ljava/lang/String;"))
    {
        CCLOGFUNC("2");
        return "";
    }
    else
    {
        jstring result = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        JNIEnv *env = minfo.env;
        J2CSTRING(resultStr, result);
        minfo.env->DeleteLocalRef(minfo.classID);
        return resultStr;
    }
}

void ChatServiceCocos2dx::postRedPackageDuringTime(int time)
{
    if(time < 0)
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postRedPackageDuringTime", "(I)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,(jint)time);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ChatServiceCocos2dx::postMutiRewardItem(string json)
{
    if(!MailController::getInstance()->getIsNewMailListEnable())
        return;
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"com/elex/chatservice/controller/ServiceInterface","postMutiRewardItem", "(Ljava/lang/String;)V"))
    {
        CCLOGFUNC("2");
        return;
    }
    else
    {
        jstring jsonStr = minfo.env->NewStringUTF(json.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jsonStr);
        minfo.env->DeleteLocalRef(minfo.classID);
        minfo.env->DeleteLocalRef(jsonStr);
    }
}


//void ChatServiceCocos2dx::onGetAllianceMembers(CCObject* data)
//{
//    
//}

//多语言结构体
struct LanguageItem
{
    string key;
    string langValue;
};


extern "C" {
    
    //返回多语言的结构数组
   JNIEXPORT jobjectArray JNICALL  Java_com_elex_chatservice_host_GameHost_getChatLangArray(JNIEnv *env, jobject _obj)
    {
        const char* chatLang[] = {"E100068","115020","105207","105209","105210","105300","105302","105304","105307","105308"
            ,"105309","105312","105313","105315","105316","105321","105322","105502","105602","108584","115922","115923"
            ,"115925","115926","115929","115181","115182","115281","115282","115168","115169","115170","105326","105327"
            ,"105328","105324","105325","115068","confirm","cancel_btn_label","114110","104932","105564","101205","105329"
            ,"105522","105591","105330","105332","137450","105333","104371","104912","105331","115100","115101","115102"
            ,"115104","103001","105348","105349","105350","105351","105352","105353","105354","105355","105344","119004"
            ,"113907","105356","105357","105369","103731","105569","103738","103783","133053","115337","115338","115339"
            ,"115340","132000","108675","105519","105590","105591","105592","105593","3000002","105734","105735","138067"
            ,"138068","105570","105512","105599","108523","4100013","4100014","114010","101007","115295","114012","115103"
            ,"105718","133017","105550","105710","105711","105537","105712","105713","105538","105708","105709","115356"
            ,"105527","115312","114101","138039","114121","114000","114002","105385","111665","105778","105781","105782"
            ,"137451","105516","103758","105373","105374","105547","102187","108678","105384","115540","115541","115542"
            ,"115543","115539","115534","111660","105383","105777","105504","105505"
            ,"114102","114006","114008","105523","114014","114016","114019","114020","114115","114116","114117","114022"
            ,"101227","114124","114128","105068","105069","113905","110014","110100","110119","110120","110121","105722"
            ,"105732","105742","111079","111080","111066","114135","133062","133026","133100","137460","137461","133270"
            ,"114111","114025","115464","115476","105714","105720","105726","105727","105728","105729","105730","3110118"
            ,"103786","105750","101019","110167","110191","115335","114144","115399","105757","105759","138065","115429"
            ,"138099","111504","105567","114005","105524","137429","137431","137430","105118","105117","111506","105700"
            ,"105704","105578","105702","105706","108554","105579","105019","115341","105701","105705","105583","105703"
            ,"105581","105707","105535","105582","105580","103715","108896","105305","115335","115544","105771","101041"
            ,"101042","103691","133083","115493","115494","115496","105347","105387","105388","105392","105393","105394","105395"};
        
        int len=sizeof(chatLang)/sizeof(char *);
        CCLOG("len:%d",len);
        //获取object所属类,一般为ava/lang/Object
        jclass objClass = env->FindClass("java/lang/Object");
        //新建object数组
        jobjectArray args = env->NewObjectArray(len, objClass, 0);
        
        //获取Java中的实例类
        jclass objectClass = env->FindClass("com/elex/chatservice/model/LanguageItem");
        //获取方法ID
        jmethodID methodId    = env->GetMethodID(objectClass,"<init>","()V");
        //获取类中每一个变量的定义
        jfieldID keyField = env->GetFieldID(objectClass, "key", "Ljava/lang/String;");
        jfieldID langValueField = env->GetFieldID(objectClass, "langValue", "Ljava/lang/String;");
        //给每一个实例的变量赋值，并且将实例作为一个object，添加到objcet数组中
        for(int  i = 0; i < len; i++)
        {
            jstring keyStr =env->NewStringUTF(chatLang[i]);
            jstring langStr = env->NewStringUTF(_lang(chatLang[i]).c_str());
            jobject obj=env->NewObject(objectClass,methodId);
            env->SetObjectField(obj, keyField, keyStr);
            env->SetObjectField(obj, langValueField, langStr);
            //添加到objcet数组中
            env->SetObjectArrayElement(args, i, obj);
            env->DeleteLocalRef(keyStr);
            env->DeleteLocalRef(langStr);
            env->DeleteLocalRef(obj);
        }
        //返回object数组
        env->DeleteLocalRef(objClass);
        env->DeleteLocalRef(objectClass);
//        if (gs_jvm) {
//            gs_jvm->DetachCurrentThread();
//        }
//         cocos2d::JniHelper::getJavaVM()->DetachCurrentThread();
        return args;
    }
    
    //返回聊天室成员的结构数组
    JNIEXPORT jobjectArray JNICALL Java_com_elex_chatservice_host_GameHost_getUserInfoArray(JNIEnv *env, jobject _obj,jint index)
    {
//        CCLOG("Java_com_elex_chatservice_host_GameHost_getUserInfoArray");
        CCArray* memberArray=dynamic_cast<CCArray*>(ChatController::getInstance()->m_userInfoDic->objectForKey((int)index)) ;
        int len=memberArray->count();
        CCLOG("len: %i", memberArray->count());
        //获取object所属类,一般为ava/lang/Object
        jclass objClass = env->FindClass("java/lang/Object");
        //新建object数组
        jobjectArray args = env->NewObjectArray(len, objClass, 0);
        
        //获取Java中的实例类
        jclass objectClass = env->FindClass("com/elex/chatservice/model/UserInfo");
        //获取类中每一个变量的定义

        //获取方法ID
        jmethodID methodId    = env->GetMethodID(objectClass,"<init>","()V");
        
        jfieldID allianceRankField = env->GetFieldID(objectClass, "allianceRank", "I");
        jfieldID serverIdField = env->GetFieldID(objectClass, "serverId", "I");
        jfieldID crossFightSrcServerIdField = env->GetFieldID(objectClass, "crossFightSrcServerId", "I");
        jfieldID uidField = env->GetFieldID(objectClass, "uid", "Ljava/lang/String;");
        jfieldID userNameField = env->GetFieldID(objectClass, "userName", "Ljava/lang/String;");
        jfieldID headPicField = env->GetFieldID(objectClass, "headPic", "Ljava/lang/String;");
        jfieldID headPicVerField = env->GetFieldID(objectClass, "headPicVer", "I");
        jfieldID mGmodField = env->GetFieldID(objectClass, "mGmod", "I");
        jfieldID asnField = env->GetFieldID(objectClass, "asn", "Ljava/lang/String;");
        jfieldID lastUpdateTimeField = env->GetFieldID(objectClass, "lastUpdateTime", "I");
        jfieldID vipLevelField = env->GetFieldID(objectClass, "vipLevel", "I");
        jfieldID vipEndTimeField = env->GetFieldID(objectClass, "vipEndTime", "I");
        jfieldID allianceIdField = env->GetFieldID(objectClass, "allianceId", "Ljava/lang/String;");
        jfieldID langField = env->GetFieldID(objectClass, "lang", "Ljava/lang/String;");
        
        //给每一个实例的变量赋值，并且将实例作为一个object，添加到objcet数组中
        for(int  i = 0; i < len; i++)
        {
            CCDictionary* dic=dynamic_cast<CCDictionary*>(memberArray->objectAtIndex(i));
 
            jstring uidStr =env->NewStringUTF(dic->valueForKey("uid")->getCString());
            jstring userNameStr =env->NewStringUTF(dic->valueForKey("userName")->getCString());
            jstring headPicStr =env->NewStringUTF(dic->valueForKey("headPic")->getCString());
            jobject obj=env->NewObject(objectClass,methodId);
            
            if(dic->objectForKey("allianceRank")){
                int allianceRank=(dynamic_cast<CCInteger*>(dic->objectForKey("allianceRank")))->getValue();
//                CCLOG("allianceRank: %i", allianceRank);
                env->SetIntField(obj, allianceRankField, (jint)allianceRank);
            }
            if(dic->objectForKey("allianceId")){
                jstring allianceIdStr =env->NewStringUTF(dic->valueForKey("allianceId")->getCString());
                env->SetObjectField(obj, allianceIdField, allianceIdStr);
            }
//            CCLOGFUNC("1");
            if(dic->objectForKey("asn")){
                jstring asnStr = env->NewStringUTF(dic->valueForKey("asn")->getCString());
                env->SetObjectField(obj, asnField, asnStr);
            }
            
            if(dic->objectForKey("lang")){
                jstring langStr = env->NewStringUTF(dic->valueForKey("lang")->getCString());
                CCLOGFUNCF("langStr:%s",dic->valueForKey("lang")->getCString());
                env->SetObjectField(obj, langField, langStr);
            }
//            CCLOGFUNC("2");
            env->SetObjectField(obj, uidField, uidStr);
            env->SetObjectField(obj, userNameField, userNameStr);
            env->SetObjectField(obj, headPicField, headPicStr);
            env->SetIntField(obj, headPicVerField, (jint)(dynamic_cast<CCInteger*>(dic->objectForKey("headPicVer")))->getValue());
            env->SetIntField(obj, mGmodField, (jint)(dynamic_cast<CCInteger*>(dic->objectForKey("mGmod")))->getValue());
            if (dic->objectForKey("lastUpdateTime")) {
                int lastUpdateTime=(dynamic_cast<CCInteger*>(dic->objectForKey("lastUpdateTime")))->getValue();
                env->SetIntField(obj, lastUpdateTimeField, (jint)lastUpdateTime);
            }
            env->SetIntField(obj, vipLevelField, (jint)(dynamic_cast<CCInteger*>(dic->objectForKey("vipLevel")))->getValue());
            if (dic->objectForKey("vipEndTime")) {
                int vipEndTime=dynamic_cast<CCInteger*>(dic->objectForKey("vipEndTime"))->getValue();
                env->SetIntField(obj, vipEndTimeField, (jint)(vipEndTime));
            }
            env->SetIntField(obj, serverIdField, (jint)(dynamic_cast<CCInteger*>(dic->objectForKey("serverId")))->getValue());
            
            if(dic->objectForKey("crossFightSrcServerId")){
                env->SetIntField(obj, crossFightSrcServerIdField, (jint)(dynamic_cast<CCInteger*>(dic->objectForKey("crossFightSrcServerId")))->getValue());
            }
            
            //添加到objcet数组中
            env->SetObjectArrayElement(args, i, obj);
            
            env->DeleteLocalRef(uidStr);
            env->DeleteLocalRef(userNameStr);
            env->DeleteLocalRef(headPicStr);
            env->DeleteLocalRef(obj);
        }
        //返回object数组
        env->DeleteLocalRef(objClass);
        env->DeleteLocalRef(objectClass);
        return args;
    }
    
    
    //返回聊天消息的结构数组
    JNIEXPORT jobjectArray JNICALL Java_com_elex_chatservice_host_GameHost_getChatInfoArray(JNIEnv *env, jobject _obj,jint chatInfoNo,jstring msgType)
    {
        CCLOG("Java_com_elex_chatservice_ChatServiceBridge_getChatInfoArray");
        J2CSTRING(msgTypeStr, msgType);
        
        if((int)chatInfoNo<0 || msgTypeStr=="")
        {
            CCLOG("(int)chatInfoNo<0");
            return NULL;
        }
        CCArray* chatInfoArr=NULL;
        if(msgTypeStr=="0")
            chatInfoArr=dynamic_cast<CCArray*>(ChatController::getInstance()->m_chatInfoSendDic->objectForKey((int)chatInfoNo)) ;
        else
            chatInfoArr=dynamic_cast<CCArray*>(MailController::getInstance()->m_mailInfoSendDic->objectForKey((int)chatInfoNo)) ;
        if(chatInfoArr && chatInfoArr->count()>0)
        {
            int len=chatInfoArr->count();
            CCLOG("chatInfoLen:%d",len);
            //获取object所属类,一般为ava/lang/Object
            jclass objClass = env->FindClass("java/lang/Object");
            //新建object数组
            jobjectArray args = env->NewObjectArray(len, objClass, 0);
            
            //获取Java中的实例类
            jclass objectClass = env->FindClass("com/elex/chatservice/model/MsgItem");
            //获取方法ID
            jmethodID methodId    = env->GetMethodID(objectClass,"<init>","()V");
            //获取类中每一个变量的定义
            
            jfieldID isNewMsgField = env->GetFieldID(objectClass, "isNewMsg", "Z");
            jfieldID isSelfMsgField = env->GetFieldID(objectClass, "isSelfMsg", "Z");
            jfieldID channelTypeField = env->GetFieldID(objectClass, "channelType", "I");
            jfieldID sequenceIdField = env->GetFieldID(objectClass, "sequenceId", "I");
            jfieldID lastUpdateTimeField = env->GetFieldID(objectClass, "lastUpdateTime", "I");
            jfieldID postField = env->GetFieldID(objectClass, "post", "I");
            jfieldID gmodField = env->GetFieldID(objectClass, "gmod", "I");
            jfieldID headPicVerField = env->GetFieldID(objectClass, "headPicVer", "I");
             jfieldID mailIdField = env->GetFieldID(objectClass, "mailId", "Ljava/lang/String;");
            jfieldID vipField = env->GetFieldID(objectClass, "vip", "Ljava/lang/String;");
//            jfieldID timeField = env->GetFieldID(objectClass, "time", "Ljava/lang/String;");
            jfieldID createTimeField = env->GetFieldID(objectClass, "createTime", "I");
            jfieldID uidField = env->GetFieldID(objectClass, "uid", "Ljava/lang/String;");
            jfieldID nameField = env->GetFieldID(objectClass, "name", "Ljava/lang/String;");
            jfieldID asnField = env->GetFieldID(objectClass, "asn", "Ljava/lang/String;");
            jfieldID msgField = env->GetFieldID(objectClass, "msg", "Ljava/lang/String;");
            jfieldID translateMsgField = env->GetFieldID(objectClass, "translateMsg", "Ljava/lang/String;");
            jfieldID headPicField = env->GetFieldID(objectClass, "headPic", "Ljava/lang/String;");
            jfieldID attachmentIdField = env->GetFieldID(objectClass, "attachmentId", "Ljava/lang/String;");
            jfieldID originalLangField = env->GetFieldID(objectClass, "originalLang", "Ljava/lang/String;");
            jfieldID sendLocalTimeField = env->GetFieldID(objectClass, "sendLocalTime", "I");

            for(int  i = 0; i < len; i++)
            {
                ChatMailInfo* chatMailInfo=dynamic_cast<ChatMailInfo*>(chatInfoArr->objectAtIndex(i));
                if (chatMailInfo) {
                    jstring idStr =env->NewStringUTF(chatMailInfo->id.c_str());
                    jstring vip =env->NewStringUTF(chatMailInfo->vip.c_str());
                    //                jstring time =env->NewStringUTF(chatMailInfo->time.c_str());
                    jstring uidStr =env->NewStringUTF(chatMailInfo->uid.c_str());
                    jstring nameStr =env->NewStringUTF(chatMailInfo->name.c_str());
                    jstring asnStr =env->NewStringUTF(chatMailInfo->asn.c_str());
                    jstring msgStr =env->NewStringUTF(chatMailInfo->msg.c_str());
                    jstring translationMsgStr =env->NewStringUTF(chatMailInfo->translateMsg.c_str());
                    jstring headPic =env->NewStringUTF(chatMailInfo->headPic.c_str());
                    jstring originalLangStr =env->NewStringUTF(chatMailInfo->originalLang.c_str());
                    jint sendLocalTime =(jint)(atoi(chatMailInfo->sendLocalTime.c_str()));
                    
                    jobject obj=env->NewObject(objectClass,methodId);
                    jstring attachmentIdStr = NULL;
                    int post = chatMailInfo->post;
                    if(post == CHAT_TYPE_INVITE){
                        attachmentIdStr = env->NewStringUTF(chatMailInfo->allianceId.c_str());
                    }else if(post == CHAT_TYPE_BATTLE_SHARE){
                        attachmentIdStr = env->NewStringUTF(chatMailInfo->reportUid.c_str());
                    }else if(post == CHAT_TYPE_DETECT_SHARE){
                        attachmentIdStr = env->NewStringUTF(chatMailInfo->detectReportUid.c_str());
                    }
//                    }else if(post == CHAT_TYPE_EQUIP_SHARE){
//                        attachmentIdStr = env->NewStringUTF(CC_ITOA(chatMailInfo->equipId));
//                    }
//                    else if(post == CHAT_TYPE_ALLIANCE_RALLY){
//                        attachmentIdStr = env->NewStringUTF(chatMailInfo->teamUid.c_str());
//                    }
//                    else if(post == CHAT_TYPE_LOTTERY_SHARE)
//                    {
//                        attachmentIdStr = env->NewStringUTF(chatMailInfo->lotteryInfo.c_str());
//                    }
//                    else if(post == CHAT_TYPE_RED_PACKAGE)
//                    {
//                        string attachmentId = chatMailInfo->redPackets;
//                        string serverId = "0";
//                        if(chatMailInfo->server>0)
//                            serverId = CC_ITOA(chatMailInfo->server);
//                        attachmentId.append("_").append(serverId);
//                        CCLOGFUNCF("attachmentId:%s",attachmentId.c_str());
//                        attachmentIdStr = env->NewStringUTF(attachmentId.c_str());
//                    }
                    //                CCLOG("set allianceId: %s", attachmentIdStr);
                    env->SetBooleanField(obj, isNewMsgField, (jboolean)(chatMailInfo->isNewMsg));
                    env->SetBooleanField(obj,  isSelfMsgField , (jboolean)(chatMailInfo->isSelfMsg));
                    env->SetIntField(obj,  channelTypeField , (jint)(chatMailInfo->channelMsgType));
                    env->SetIntField(obj,  sequenceIdField , (jint)(chatMailInfo->sequenceId));
                    env->SetIntField(obj,  lastUpdateTimeField , (jint)(chatMailInfo->lastUpdateTime));
                    env->SetIntField(obj,  postField ,  (jint)(chatMailInfo->post));
                    env->SetIntField(obj,  gmodField ,  (jint)(chatMailInfo->gmod));
                    env->SetIntField(obj,  headPicVerField ,  (jint)(chatMailInfo->headPicVer));
                    env->SetObjectField(obj,  vipField , vip);
                    //                env->SetObjectField(obj,  timeField , time);
                    env->SetIntField(obj,  createTimeField , (jint)(chatMailInfo->createTime));
                    env->SetObjectField(obj,  mailIdField ,  idStr);
                    env->SetObjectField(obj,  uidField ,  uidStr);
                    env->SetObjectField(obj,  nameField ,  nameStr);
                    env->SetObjectField(obj,  asnField ,  asnStr);
                    if(attachmentIdStr != NULL){
                        env->SetObjectField(obj,  attachmentIdField ,  attachmentIdStr);
                    }
                    env->SetObjectField(obj,  msgField ,  msgStr);
                    env->SetObjectField(obj,  translateMsgField , translationMsgStr);
                    env->SetObjectField(obj,  headPicField ,  headPic);
                    env->SetObjectField(obj,  originalLangField ,  originalLangStr);
                    env->SetIntField(obj,  sendLocalTimeField , sendLocalTime);
                    
                    //添加到objcet数组中
                    env->SetObjectArrayElement(args, i, obj);
                    env->DeleteLocalRef(vip);
                    env->DeleteLocalRef(idStr);
                    env->DeleteLocalRef(uidStr);
                    env->DeleteLocalRef(nameStr);
                    env->DeleteLocalRef(asnStr);
                    env->DeleteLocalRef(msgStr);
                    env->DeleteLocalRef(translationMsgStr);
                    env->DeleteLocalRef(headPic);
                    env->DeleteLocalRef(originalLangStr);
                    env->DeleteLocalRef(obj);
                    env->DeleteLocalRef(attachmentIdStr);
                }
            }
            if(msgTypeStr=="0")
                ChatController::getInstance()->m_chatInfoSendDic->removeObjectForKey((int)chatInfoNo);
            else
                MailController::getInstance()->m_mailInfoSendDic->removeObjectForKey((int)chatInfoNo);
            env->DeleteLocalRef(objClass);
            env->DeleteLocalRef(objectClass);
            //返回object数组
            return args;

        }
        return NULL;
    }
    
    
    //返回邮件消息的结构数组
    JNIEXPORT jobjectArray JNICALL Java_com_elex_chatservice_host_GameHost_getMailDataArray(JNIEnv *env, jobject _obj,jint mailDataIndex)
    {
        CCLOGFUNC("");
        
        if((int)mailDataIndex<0)
        {
            CCLOGFUNC("mailDataIndex<0");
            return NULL;
        }
        CCArray* mailDataArr=dynamic_cast<CCArray*>(MailController::getInstance()->m_mailDataDic->objectForKey((int)mailDataIndex)) ;

        if(mailDataArr && mailDataArr->count()>0)
        {
            int len=mailDataArr->count();
            CCLOGFUNCF("mailDataArr length:%d",len);
            //获取object所属类,一般为ava/lang/Object
            jclass objClass = env->FindClass("java/lang/Object");
            //新建object数组
            jobjectArray args = env->NewObjectArray(len, objClass, 0);
            
                        //获取Java中的实例类
            jclass objectClass = env->FindClass("com/elex/chatservice/model/mail/MailData");
            //获取方法ID
            jmethodID methodId    = env->GetMethodID(objectClass,"<init>","()V");

            
            //获取类中每一个变量的定义
            jfieldID typeField = env->GetFieldID(objectClass, "type", "I");
            jfieldID createTimeField = env->GetFieldID(objectClass, "createTime", "I");
            jfieldID statusField = env->GetFieldID(objectClass, "status", "I");
            jfieldID replyField = env->GetFieldID(objectClass, "reply", "I");
            jfieldID rewardStatusField = env->GetFieldID(objectClass, "rewardStatus", "I");
            jfieldID itemIdFlagField = env->GetFieldID(objectClass, "itemIdFlag", "I");//1需要读语言文件
            jfieldID saveField = env->GetFieldID(objectClass, "save", "I");//0未保存,1保存,2删除保存过
            jfieldID mbLevelField = env->GetFieldID(objectClass, "mbLevel", "I");//0未保存,1保存,2删除保存过
            
            
            jfieldID uidField = env->GetFieldID(objectClass, "uid", "Ljava/lang/String;");
            jfieldID titleField = env->GetFieldID(objectClass, "title", "Ljava/lang/String;");
            jfieldID contentsField = env->GetFieldID(objectClass, "contents", "Ljava/lang/String;");
            jfieldID fromNameField = env->GetFieldID(objectClass, "fromName", "Ljava/lang/String;");
            jfieldID fromUidField = env->GetFieldID(objectClass, "fromUid", "Ljava/lang/String;");
            jfieldID rewardIdField = env->GetFieldID(objectClass, "rewardId", "Ljava/lang/String;");
            
            for(int  i = 0; i < len; i++)
            {
                CCDictionary* dic=dynamic_cast<CCDictionary*>(mailDataArr->objectAtIndex(i));
                if (dic) {
                    jstring uidStr=env->NewStringUTF(dic->valueForKey("uid")->getCString());
                    jstring titleStr=env->NewStringUTF(dic->valueForKey("title")->getCString());
                    jstring contentsStr=env->NewStringUTF(dic->valueForKey("contents")->getCString());
                    jstring fromNameStr=env->NewStringUTF(dic->valueForKey("fromName")->getCString());
                    jstring fromUidStr=env->NewStringUTF(dic->valueForKey("fromUser")->getCString());
                    jstring rewardIdStr=env->NewStringUTF(dic->valueForKey("rewardId")->getCString());
                    
                    jobject obj=env->NewObject(objectClass,methodId);
                    
                    env->SetIntField(obj,  typeField , (jint)(dic->valueForKey("type")->intValue()));
                    env->SetIntField(obj,  createTimeField , (jint)(dic->valueForKey("createTime")->doubleValue()/1000));
                    env->SetIntField(obj,  statusField , (jint)(dic->valueForKey("status")->intValue()));
                    env->SetIntField(obj,  replyField , (jint)(dic->valueForKey("reply")->intValue()));
                    env->SetIntField(obj,  rewardStatusField , (jint)(dic->valueForKey("rewardStatus")->intValue()));
                    env->SetIntField(obj,  itemIdFlagField , (jint)(dic->valueForKey("itemIdFlag")->intValue()));//1需要读语言文件
                    env->SetIntField(obj,  saveField , (jint)(dic->valueForKey("save")->intValue()));//0未保存,1保存,2删除保存过
                    if(dic->objectForKey("mbLevel"))
                        env->SetIntField(obj,  mbLevelField , (jint)(dic->valueForKey("mbLevel")->intValue()));
                    env->SetObjectField(obj,   uidField ,  uidStr);
                    env->SetObjectField(obj,   titleField ,  titleStr);
                    env->SetObjectField(obj,   contentsField ,  contentsStr);
                    env->SetObjectField(obj,   fromNameField ,  fromNameStr);
                    env->SetObjectField(obj,   fromUidField ,  fromUidStr);
                    env->SetObjectField(obj,   rewardIdField ,  rewardIdStr);
                    
                    //添加到objcet数组中
                    env->SetObjectArrayElement(args, i, obj);
                    
                    env->DeleteLocalRef(uidStr);
                    env->DeleteLocalRef(titleStr);
                    env->DeleteLocalRef(contentsStr);
                    env->DeleteLocalRef(fromNameStr);
                    env->DeleteLocalRef(fromUidStr);
                    env->DeleteLocalRef(rewardIdStr);
                }
            }
            MailController::getInstance()->m_mailDataDic->removeObjectForKey((int)mailDataIndex);
            
            env->DeleteLocalRef(objClass);
            env->DeleteLocalRef(objectClass);
            //返回object数组
            return args;
            
        }
        return NULL;
    }
	
	JNIEXPORT void JNICALL  Java_com_elex_chatservice_host_GameHost_onTextChanged (JNIEnv *env,
																jobject object,
																jstring message) {
		if(ChatServiceCocos2dx::sendMessageListener) {
			J2CSTRING(msg, message);
			ChatServiceCocos2dx::sendMessageListener->onTextChanged(msg);
		}
	}
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_sendHornMessage(JNIEnv *env,
                                                                 jobject object,
                                                                 jstring msg,
                                                                 jboolean usePoint,
                                                                 jstring sendLocalTime){
        J2CSTRING(hornMsg, msg);
        J2CSTRING(sendLocalTimeStr, sendLocalTime);
        CCLOG("hornMsg %s:",hornMsg.c_str());
        CCLOG("usePoint:%d  sendLocalTimeStr:%s",usePoint,sendLocalTimeStr.c_str());
        ChatController::getInstance()->sendNotice(hornMsg,ITEM_SEND_NOTICE,(bool)usePoint,sendLocalTimeStr.c_str());
    }
    
    
    
    JNIEXPORT void JNICALL  Java_com_elex_chatservice_host_GameHost_getAllianceMember(JNIEnv *env,
                                                                 jobject object){
        CCLOG("Java_com_elex_chatservice_host_GameHost_getAllianceMember");
        MailController::getInstance()->getAllianceMember();
    }
    
    //获取消息记录（聊天和聊天室）
    JNIEXPORT void JNICALL  Java_com_elex_chatservice_host_GameHost_getMsgBySeqId(JNIEnv *env,
                                                      jobject object,
                                                      jint minSeq,jint maxSeq,jint channelType,jstring channelIdStr)
    {
        J2CSTRING(channelId, channelIdStr);
         CCLOGFUNC("Java_com_elex_chatservice_host_getMsgBySeqId");
        CCLOGFUNCF("minSeq %d:",(int)minSeq);
        CCLOGFUNCF("maxSeq %d:",(int)maxSeq);
        CCLOGFUNCF("channelType %d:",(int)channelType);
        CCLOGFUNCF("channelId %s:",channelId.c_str());
        
        GetMsgBySeqIdCommand* cmd = new GetMsgBySeqIdCommand((int)minSeq,(int)maxSeq,(int)channelType,channelId);
        cmd->sendAndRelease();
    }
    
    
    //创建聊天室
    JNIEXPORT void JNICALL  Java_com_elex_chatservice_host_GameHost_createChatRoom(JNIEnv *env,
                                                                      jobject object,
                                                                     jstring memberNameStr,jstring memberUidStr,jstring chatRoomNameStr,jstring contentStr)
    {
        J2CSTRING(chatRoomName, chatRoomNameStr);
        J2CSTRING(memberName, memberNameStr);
        J2CSTRING(memberUid, memberUidStr);
        J2CSTRING(content, contentStr);
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_selectChatRoomMember");
        CCLOGFUNCF("chatRoomName %s:",chatRoomName.c_str());
        CCLOGFUNCF("memberName %s:",memberName.c_str());
        CCLOGFUNCF("memberUid %s:",memberUid.c_str());
        CCLOGFUNCF("content %s:",content.c_str());
        
        if (memberName=="" || memberUid=="") {
            CCLOG("selectChatRoomMember return");
            return;
        }
        
        if (chatRoomName=="") {
            chatRoomName=memberName;
        }
        MailController::getInstance()->setChatRoomInfo(chatRoomName,memberName,memberUid);
        MailController::getInstance()->createChatRoom(content);
    }
    
    //选择群聊成员
    JNIEXPORT void JNICALL  Java_com_elex_chatservice_host_GameHost_selectChatRoomMember(JNIEnv *env,
                                                                      jobject object,
                                                                      jstring chatRoomNameStr,jstring memberNameStr,jstring memberUidStr)
    {
        J2CSTRING(chatRoomName, chatRoomNameStr);
        J2CSTRING(memberName, memberNameStr);
        J2CSTRING(memberUid, memberUidStr);
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_selectChatRoomMember");
        CCLOG("chatRoomName %s:",chatRoomName.c_str());
        CCLOG("memberName %s:",memberName.c_str());
        CCLOG("memberUid %s:",memberUid.c_str());

        if (memberName=="" || memberUid=="") {
            CCLOG("selectChatRoomMember return");
            return;
        }
        
        if (chatRoomName=="") {
            chatRoomName=memberName;
        }
        MailController::getInstance()->setChatRoomInfo(chatRoomName,memberName,memberUid);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(JNIScheduleObject::refreshMailWriteName), JNIScheduleObject::getInstance(), 0.0f, 0, 1.0f, false);
    }
    
    //邀请加入群聊
    JNIEXPORT void JNICALL  Java_com_elex_chatservice_host_GameHost_inviteChatRoomMember(JNIEnv *env,
                                                                 jobject object,
                                                                      jstring groupId,jstring memberNameStr,jstring memberUidStr){
        J2CSTRING(roomId, groupId);
        J2CSTRING(memberName, memberNameStr);
        J2CSTRING(memberUid, memberUidStr);
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_inviteChatRoomMember");
        CCLOG("roomId %s:",roomId.c_str());
        CCLOG("memberName %s:",memberName.c_str());
        CCLOG("memberUid %s:",memberUid.c_str());
        
        if (roomId=="" || memberName=="" || memberUid=="") {
            return;
        }
        vector<string> strVec1;
        CCArray* memberUidArray=CCArray::create();
        
        CCCommonUtils::splitString(memberUid, "|", strVec1);
        CCLOGFUNCF("strVec1.size() :%d",strVec1.size());
        for (int i = 0; i<strVec1.size(); i++)
        {
            CCLOGFUNCF("strVec1.at(i) :%s",strVec1.at(i).c_str());
            memberUidArray->addObject(CCString::create(strVec1.at(i).c_str()));
        }
        
        MailController::getInstance()->inviteChatRoomMembers(memberName, memberUidArray,roomId);
    }
    
    //将玩家移除群聊
    JNIEXPORT void JNICALL  Java_com_elex_chatservice_host_GameHost_kickChatRoomMember(JNIEnv *env,
                                                                      jobject object,
                                                                    jstring groupId,jstring memberNameStr,jstring memberUidStr){
        J2CSTRING(roomId, groupId);
        J2CSTRING(memberName, memberNameStr);
        J2CSTRING(memberUid, memberUidStr);
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_kickChatRoomMember");
        CCLOG("roomId %s:",roomId.c_str());
        CCLOG("memberName %s:",memberName.c_str());
        CCLOG("memberUid %s:",memberUid.c_str());
        
        if (roomId=="" || memberName=="" || memberUid=="") {
            return;
        }
        
        
        vector<string> strVec1;
        CCArray* memberUidArray=CCArray::create();
        
        CCCommonUtils::splitString(memberUid, "|", strVec1);
        CCLOGFUNCF("strVec1.size() :%d",strVec1.size());
        for (int i = 0; i<strVec1.size(); i++)
        {
            CCLOGFUNCF("strVec1.at(i) :%s",strVec1.at(i).c_str());
            memberUidArray->addObject(CCString::create(strVec1.at(i).c_str()));
        }

        MailController::getInstance()->kickChatRoomMembers(memberName, memberUidArray,roomId);
    }
    
    //退出群聊
    JNIEXPORT void JNICALL  Java_com_elex_chatservice_host_GameHost_quitChatRoom(JNIEnv *env,
                                                                    jobject object,
                                                                    jstring groupId){
        J2CSTRING(roomId, groupId);
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_quitChatRoom");
        CCLOG("roomId %s:",roomId.c_str());
        if (roomId=="") {
            return;
        }
        MailController::getInstance()->quitChatRoom(roomId);
    }
    
    //修改群聊名称
    JNIEXPORT void JNICALL  Java_com_elex_chatservice_host_GameHost_modifyChatRoomName(JNIEnv *env,
                                                              jobject object,
                                                              jstring groupId,
                                                              jstring roomNameStr){
        J2CSTRING(roomId, groupId);
        J2CSTRING(roomName, roomNameStr);
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_modifyChatRoomName");
        CCLOG("roomId %s:",roomId.c_str());
        CCLOG("roomName %s:",roomName.c_str());
        if (roomId=="" || roomName=="") {
            return;
        }
        MailController::getInstance()->modifyChatRoomName(roomName,roomId);
    }
    
    //获取群聊消息记录
    JNIEXPORT void JNICALL  Java_com_elex_chatservice_host_GameHost_getChatRoomMsgRecord(JNIEnv *env,
                                                                 jobject object,
                                                                 jstring groupId,
                                                                 jint start,
                                                                 jint count){
        J2CSTRING(roomId, groupId);
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_getChatRoomMsgRecord");
        CCLOG("roomId %s:",roomId.c_str());
        CCLOG("start %d:",(int)start);
        CCLOG("count %d:",(int)count);
        if (roomId==""  || (int)start<=0 || (int)count<=0) {
            return;
        }
        MailController::getInstance()->requestChatRoomMsgRecord(true,(int)start,(int)count,roomId);
    }

    JNIEXPORT void JNICALL  Java_com_elex_chatservice_host_GameHost_postCurChannel(JNIEnv *env,
                                                                 jobject object,
                                                                 jint channel){
        int curChatType=(int)channel;
        CCLOG("cur channel %d:",curChatType);
        ChatServiceCocos2dx::m_channelType=curChatType;
    }
    
    JNIEXPORT void JNICALL  Java_com_elex_chatservice_host_GameHost_callXCApi(JNIEnv *env,
                                                                jobject object){
       ChatController::getInstance()->callXCApiForGoogleTranslate();
    }
    
    JNIEXPORT int JNICALL  Java_com_elex_chatservice_host_GameHost_getHornBanedTime(JNIEnv *env,
                                                             jobject object){
        CCLOG("Java_com_elex_chatservice_host_GameHost_getHornBanedTime");
        auto tmpT = ChatController::getInstance()->getNoticeBanTime();
        int lastTime = GlobalData::shared()->changeTime(tmpT) - GlobalData::shared()->getWorldTime();
        if(lastTime>0){
            string dialogText=_lang("105334").c_str();
            dialogText.append("\n").append( _lang("105335").c_str()).append(CC_SECTOA(lastTime));
            YesNoDialog::showYesDialog(dialogText.c_str());
            return lastTime;
        }
        return 0;
    }

    
    JNIEXPORT int JNICALL Java_com_elex_chatservice_host_GameHost_isHornEnough(JNIEnv *env,
                                                          jobject object){
        CCLOG("Java_com_elex_chatservice_host_GameHost_isHornEnough  itemid %d",ITEM_SEND_NOTICE);
        auto& info = ToolController::getInstance()->getToolInfoById(ITEM_SEND_NOTICE);
        int count=0;
        if(info.getCNT()<=0){
            count=ChatController::getInstance()->getNoticePrice();
        }
        return count;
    }
    
    JNIEXPORT bool JNICALL Java_com_elex_chatservice_host_GameHost_isCornEnough(JNIEnv *env,
                                                         jobject object,
                                                         jint price){
        CCLOG("Java_com_elex_chatservice_host_GameHost_isCornEnough %d",(int)price);
        return CCCommonUtils::isEnoughResourceByType(Gold, (int)price);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_requestChatMsg (JNIEnv *env,
                                                                           jobject object,
                                                                           int type) {
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_requestChatMsg type:%d",type);
        if(type==CHAT_ALLIANCE)
            ChatController::getInstance()->sendRequestChatFromAndroid(CHAT_ALLIANCE);
        else if(type==CHAT_COUNTRY)
            ChatController::getInstance()->sendRequestChatFromAndroid(CHAT_COUNTRY);
	}
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_requestMoreMail (JNIEnv *env,
                                                                     jobject object,
                                                                     jstring fromUid,
                                                                      jstring uid,
                                                                      int count) {
        J2CSTRING(fromUidStr, fromUid);
        J2CSTRING(uidStr, uid);
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_requestMoreMail count:%d fromUid:%s uid:%s",count,fromUidStr.c_str(),uidStr.c_str());
        
        MailController::getInstance()->requestMoreMail(fromUidStr,uidStr,count);
    }
    
    //邮件窗口发送信息
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_sendMailMsg (JNIEnv *env,
                                                              jobject object,
                                                              jstring toName,
                                                              jstring  title,
                                                              jstring content,
                                                              jstring allianceUid,
                                                              jstring uid,
                                                              jboolean isFirstMsg,
                                                              jint type,
                                                              jstring sendLocalTime,
                                                              jstring toUid) {
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_sendMailMsg");
        J2CSTRING(toUidStr, toUid);
        J2CSTRING(toNameStr, toName);
        J2CSTRING(titleStr, title);
        J2CSTRING(contentStr, content);
        J2CSTRING(allianceUidStr, allianceUid);
        J2CSTRING(uidStr, uid);
        J2CSTRING(sendLocalTimeStr, sendLocalTime);
        
        CCLOG("toUidStr : %s", toUidStr.c_str());
        CCLOG("toNameStr : %s", toNameStr.c_str());
        CCLOG("titleStr : %s", titleStr.c_str());
        CCLOG("contentStr : %s", contentStr.c_str());
        CCLOG("allianceUidStr : %s", allianceUidStr.c_str());
        CCLOG("sendLocalTime : %s", sendLocalTimeStr.c_str());
        CCLOG("uidStr : %s", uidStr.c_str());
        CCLOG("type : %d",type);
        
        MailController::getInstance()->sendMailToServer(toNameStr, titleStr, contentStr, sendLocalTimeStr, allianceUidStr, uidStr, (bool)isFirstMsg, (int)type,toUidStr);
    }
    
    //发送群聊消息
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_sendChatRoomMsg(JNIEnv *env,
                                                                 jobject object,
                                                                 jstring groupId,
                                                                 jstring msgStr,
                                                                 jstring sendLocalTime){
        J2CSTRING(roomId, groupId);
        J2CSTRING(msg, msgStr);
        J2CSTRING(sendLocalTimeStr, sendLocalTime);
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_sendChatRoomMsg");
        CCLOG("roomId %s:",roomId.c_str());
        CCLOG("msg %s:",msg.c_str());
        CCLOG("sendLocalTime : %s", sendLocalTimeStr.c_str());
        if (roomId=="" || msg=="") {
            return;
        }
        MailController::getInstance()->sendChatRoomMsg(msg, roomId, sendLocalTimeStr);
    }
    
    //解除屏蔽玩家
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_unShieldPlayer (JNIEnv *env,
                                                                     jobject object,
                                                                     jstring  uid,jstring name) {
        CCLOG("Java_com_elex_chatservice_host_GameHost_unShieldPlayer");
        J2CSTRING(uidStr, uid);
        J2CSTRING(nameStr, name);
        CCLOG("uidStr : %s", uidStr.c_str());
        CCLOG("nameStr : %s", nameStr.c_str());
        ChatController::getInstance()->unShieldPlayer(uidStr,nameStr);
    }
    
    //屏蔽玩家
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_shieldPlayer (JNIEnv *env,
                                                                     jobject object,
                                                                     jstring  uid) {
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_shieldPlayer");
        J2CSTRING(uidStr, uid);
        
        ChatController::getInstance()->shieldPlayer(uidStr);
    }
    
    
    //解除禁言玩家
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_unBanPlayer (JNIEnv *env,
                                                                     jobject object,
                                                                     jstring  uid) {
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_unBanPlayer");
        J2CSTRING(uidStr, uid);
        CCLOG("uidStr : %s", uidStr.c_str());
        ChatController::getInstance()->unBanPlayer(uidStr);
    }
    
    //禁言玩家2
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_banPlayerByIndex (JNIEnv *env,
                                                                jobject object,
                                                                jstring  uid,
                                                                jint  banTimeIndex) {
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_banPlayer");
        J2CSTRING(uidStr, uid);
        CCLOG("uidStr : %s", uidStr.c_str());
        CCLOG("banTimeIndex : %d", (int)banTimeIndex);
        ChatController::getInstance()->banPlayer(uidStr,(int)banTimeIndex);
    }
    
    //解除禁言玩家喇叭消息
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_unBanPlayerNotice (JNIEnv *env,
                                                                                jobject object,
                                                                                jstring  uid) {
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_unBanPlayerNotice");
        J2CSTRING(uidStr, uid);
        CCLOG("uidStr : %s", uidStr.c_str());
//        ChatController::getInstance()->unBanPlayerNotice(uidStr);
    }
    
    //禁言玩家喇叭消息
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_banPlayerNoticeByIndex (JNIEnv *env,
                                                                                     jobject object,
                                                                                     jstring  uid,
                                                                                     jint  banTimeIndex) {
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_banPlayerNoticeByIndex");
        J2CSTRING(uidStr, uid);
        CCLOG("uidStr : %s", uidStr.c_str());
        CCLOG("banTimeIndex : %d", (int)banTimeIndex);
//        ChatController::getInstance()->banPlayerNotice(uidStr,(int)banTimeIndex);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_joinAnnounceInvitation(JNIEnv *env,
                                                                           jobject object,
                                                                           jstring allianceId) {
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_joinAlliance");
        J2CSTRING(id, allianceId);
        ChatController::getInstance()->joinAnnounceInvitation(id);
    }
    
	
	JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_sendChatMessage (JNIEnv *env,
																  jobject object,
																  jstring message,jint type,jstring sendLocalTime) {
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_sendChatMessage");
        
        J2CSTRING(msg, message);
        J2CSTRING(sendLocalTimeStr, sendLocalTime);
        CCLOG("msg : %s  type:%d sendLocalTime : %s", msg.c_str(), (int)type,sendLocalTimeStr.c_str());
        int channelType=-1;
        if((int)type==0)
            channelType=0;
        else if((int)type==1)
            channelType=2;
        CCLOGFUNCF("channelType %d",channelType);
        ChatController::getInstance()->sendCountryChat(msg.c_str(), channelType,0,sendLocalTimeStr.c_str());
	}
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_searchPlayer (JNIEnv *env,
                                                                  jobject object,
                                                                  jstring name) {
        
        CCLOG("Java_com_elex_chatservice_host_GameHost_searchPlayer");
        
        J2CSTRING(nameStr, name);
        CCLOGFUNCF("name : %s", nameStr.c_str());
        MailController::getInstance()->searchPlayer(nameStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_postDetectMailInfo (JNIEnv *env,
                                                               jobject object,
                                                               jstring json) {
        
        CCLOGFUNC("");
        
        J2CSTRING(jsonStr, json);
        CCLOGFUNCF("jsonStr : %s", jsonStr.c_str());
//        MailController::getInstance()->parseDetectInfo(jsonStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_postDeletedDetectMailInfo (JNIEnv *env,
                                                                     jobject object,
                                                                     jstring json) {
        
        CCLOGFUNC("");
        
        J2CSTRING(jsonStr, json);
        CCLOGFUNCF("jsonStr : %s", jsonStr.c_str());
//        MailController::getInstance()->postDeletedDetectMailInfo(jsonStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_postChangedDetectMailInfo (JNIEnv *env,
                                                                     jobject object,
                                                                     jstring json) {
        
        CCLOGFUNC("");
        
        J2CSTRING(jsonStr, json);
        CCLOGFUNCF("jsonStr : %s", jsonStr.c_str());
//        MailController::getInstance()->postChangedDetectMailInfo(jsonStr);
    }
    
	
	JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_sendMessage (JNIEnv *env,
																  jobject object,
																  jstring message) {
        if(ChatServiceCocos2dx::sendMessageListener) {
            J2CSTRING(msg, message);
            ChatServiceCocos2dx::sendMessageListener->sendMessage(msg);
        }
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_showDetectMailFromAndroid (JNIEnv *env,
                                                              jobject object,
                                                              jstring mailUid) {
        CCLOGFUNC("");
        J2CSTRING(mailUidStr, mailUid);
        CCLOGFUNCF("mailUidStr : %s", mailUidStr.c_str());
        auto search = GlobalData::shared()->mailList.find(mailUidStr.c_str());
        bool isExistMail=(search != GlobalData::shared()->mailList.end());
        
        MailInfo* mailInfo=NULL;
        if (isExistMail) {
            mailInfo=dynamic_cast<MailInfo*>(search->second);
        }
        
        if(mailInfo!=NULL)
        {
            CCLOGFUNC("mailInfo!=NULL");
          MailController::getInstance()->showMailPopupFromAnroid(mailInfo);
        }
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_postNotifyMailPopup (JNIEnv *env,
                                                              jobject object) {
        if (!ChatServiceCocos2dx::isChatShowing) {
            CCLOGFUNC("");
             CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_ADD, CCInteger::create(20));
        }
    }
	
	JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_onBackPressed (JNIEnv *env,
																	jobject object) {
		if(ChatServiceCocos2dx::sendMessageListener) {
			ChatServiceCocos2dx::sendMessageListener->onBackPressed();
		}
	}
	
	JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_setActionAfterResume (JNIEnv *env,
																  jobject object,
                                                                  jstring _actionAfterResume,
																  jstring _uid,
																  jstring _name,
                                                                  jstring _attachmentId,
                                                                  jboolean _returnToChatAfterPopup) {
        J2CSTRING(actionAfterResume, _actionAfterResume);
        J2CSTRING(uid, _uid);
        J2CSTRING(name, _name);
        J2CSTRING(attachmentId, _attachmentId);
        
        CCLOG("setActionAfterResume: actionAfterResume=%s uid=%s name=%s attachmentId=%s", actionAfterResume.c_str(), uid.c_str(), name.c_str(), attachmentId.c_str());
		JNIScheduleObject::getInstance()->actionAfterResume = actionAfterResume;
        JNIScheduleObject::getInstance()->uid = uid;
        JNIScheduleObject::getInstance()->name = name;
        JNIScheduleObject::getInstance()->attachmentId = attachmentId;
        JNIScheduleObject::getInstance()->returnToChatAfterPopup = (bool)_returnToChatAfterPopup;
	}
	
	JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_onResume (JNIEnv *env,
														   jobject object,
                                                           jint _chatType) {
        CCLOG("onResume: actionAfterResume=%s  chatType=%d", JNIScheduleObject::getInstance()->actionAfterResume.c_str(), (int)_chatType);
        
        ChatServiceCocos2dx::m_channelType=(int)_chatType;
        ChatServiceCocos2dx::isChatShowing=false;
        ChatServiceCocos2dx::isForumShowing=false;
        ChatServiceCocos2dx::isTranslationWebViewShowing = false;
        
        // 显示系统邮件时隐藏了下方主UI，显示了上方titleBar，此时需要恢复
        if((PopupViewController::getInstance()->getCurrViewCount() + PopupViewController::getInstance()->getGoBackViewCount()) == 0
           && MailController::getInstance()->getIsNewMailListEnable()
           && (JNIScheduleObject::getInstance()->actionAfterResume == "" || JNIScheduleObject::getInstance()->actionAfterResume == "showPlayerInfo")){
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(JNIScheduleObject::postNotifyUIRefresh), JNIScheduleObject::getInstance(), 0.0f, 0, 0.01f,false);
        }
        
        
        if(JNIScheduleObject::getInstance()->actionAfterResume != "") {
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(JNIScheduleObject::handleResume), JNIScheduleObject::getInstance(), 0.0f, 0, 0.01f, false);
        }
        
        if(ChatServiceCocos2dx::m_channelType==CHANNEL_TYPE_USER || ChatServiceCocos2dx::m_channelType==CHANNEL_TYPE_CHATROOM) {
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(JNIScheduleObject::updateMailCell), JNIScheduleObject::getInstance(), 0.0f, 0, 1.0f, false);
        }
        else if(ChatServiceCocos2dx::m_channelType==CHANNEL_TYPE_COUNTRY || ChatServiceCocos2dx::m_channelType==CHANNEL_TYPE_ALLIANCE) {
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(JNIScheduleObject::showLatestMessage), JNIScheduleObject::getInstance(), 0.0f, 0, 1.0f, false);
        }
        
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(JNIScheduleObject::postRetrunGame), JNIScheduleObject::getInstance(), 0.0f, 0, 1.0f, false);
        
//        if ((MailController::getInstance()->m_mutiFlyToolRewardArray!=NULL && MailController::getInstance()->m_mutiFlyToolRewardArray->count()>0) ||
//            (MailController::getInstance()->m_mutiFlyRewardArray!=NULL && MailController::getInstance()->m_mutiFlyRewardArray->count()>0)) {
//            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(JNIScheduleObject::flyMutiMailReward), JNIScheduleObject::getInstance(), 0.0f, 0, 1.0f, false);
//        }
        ChatServiceCocos2dx::notifyReturn2dxGame();
	}
	
	JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_set2dxViewHeight (JNIEnv *env,
																  jobject object,
																  int height,
																  int usableHeightSansKeyboard) {
		if(ChatServiceCocos2dx::sendMessageListener) {
			ChatServiceCocos2dx::sendMessageListener->setHeightFromNative(height, usableHeightSansKeyboard);
		}
    }
    
    JNIEXPORT jstring JNICALL Java_com_elex_chatservice_host_GameHost_getCustomHeadPicUrl(JNIEnv *env,
                                                              jobject object,
                                                              jstring _uid,
                                                              jint _headPicVer){
        J2CSTRING(uid, _uid);
        string url = CCCommonUtils::getCustomPicUrl(uid, (int)_headPicVer);
        return env->NewStringUTF(url.c_str());
    }
    
    JNIEXPORT jstring JNICALL Java_com_elex_chatservice_host_GameHost_getCustomHeadPic(JNIEnv *env,
                                                                    jobject object,
                                                                     jstring _customHeadPicUrl){
        J2CSTRING(customHeadPicUrl, _customHeadPicUrl);
        string customHeadPic=HFUrlImageCache::shared()->getCachePath(customHeadPicUrl);
        return env->NewStringUTF(customHeadPic.c_str());
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_getMultiUserInfo(JNIEnv *env,
                                                                  jobject object,
                                                                  jstring _uidsStr){
        J2CSTRING(uidsStr, _uidsStr);
        ChatServiceCocos2dx::uidStr = uidsStr;
        
        CCLOG("getMultiUserInfo: %s", ChatServiceCocos2dx::uidStr.c_str());
        if(!ChatServiceCocos2dx::isChatShowing)
        {
            CCLOG("getMultiUserInfo: 1 scheduled");
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(JNIScheduleObject::getMultiUserInfo), JNIScheduleObject::getInstance(), 0.0f, 0, 0.1f, false);
        }else if(!NetController::shared()->isProcessing()){
            CCLOG("getMultiUserInfo: 2 called");
            JNIScheduleObject::getInstance()->getMultiUserInfo(0.0f);
        }else{
            CCLOG("getMultiUserInfo: 3 aborted, since another command is processing");
        }
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_transportMailInfo(JNIEnv* env, jobject object, jlong mail,jboolean isShowDetectMail)
    {
        CCLOGFUNC("");
        CCDictionary* mailData=(CCDictionary*)mail;
        if (mailData==NULL) {
              CCLOGFUNC("mailData==NULL");
            return;
        }
        CCDictionary* mailInfoDic=dynamic_cast<CCDictionary*>(mailData->objectForKey("mailInfo"));
        if(mailInfoDic)
        {
            CCLOGFUNCF("mailInfoDic!=NULL isShowDetectMail:%d",(bool)isShowDetectMail);
            int time=mailInfoDic->valueForKey("createTime")->intValue();
            double tempTime=(double)time*1000;
            mailInfoDic->setObject(CCString::create(CC_ITOA(tempTime)) , "createTime");
            int value = mailInfoDic->valueForKey("reply")->intValue();
            int share = 2;
            int reply = 1;
            int like = 4;
            int donate = 8;
            if((value & share) == share)
            {
                mailInfoDic->setObject(CCString::create("1") , "share");
            }
            else
            {
                mailInfoDic->setObject(CCString::create("0") , "share");
            }
            
            if((value & reply) == reply)
            {
                mailInfoDic->setObject(CCString::create("1") , "reply");
            }
            else
            {
                mailInfoDic->setObject(CCString::create("0") , "reply");
            }
            
            if((value & like) == like)
            {
                mailInfoDic->setObject(CCString::create("1") , "like");
            }
            else
            {
                mailInfoDic->setObject(CCString::create("0") , "like");
            }
            
            if((value & donate) == donate)
            {
                mailInfoDic->setObject(CCString::create("1") , "donate");
            }
            else
            {
                mailInfoDic->setObject(CCString::create("0") , "donate");
            }
            
            CCLOGFUNCF("contents: %s",mailInfoDic->valueForKey("contents")->getCString());
            MailController::getInstance()->addMailFromAndroidToList(mailInfoDic,true);
            
            if(isShowDetectMail)
            {
                string mailUid = "";
                if(mailInfoDic->objectForKey("uid"))
                    mailUid = mailInfoDic->valueForKey("uid")->getCString();
                if(mailUid!="")
                {
                    auto search = GlobalData::shared()->mailList.find(mailUid.c_str());
                    bool isExistMail=(search != GlobalData::shared()->mailList.end());
                    
                    MailInfo* mailInfo=NULL;
                    if (isExistMail) {
                        mailInfo=dynamic_cast<MailInfo*>(search->second);
                    }
                    
                    if(mailInfo!=NULL)
                    {
                        CCLOGFUNC("mailInfo!=NULL");
                        MailController::getInstance()->showMailPopupFromAnroid(mailInfo);
                    }
                }
            }
        }
        mailData->release();
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_deleteSingleMail(JNIEnv* env, jobject object, jint tabType,jint type,jstring mailUid,jstring fromUid)
    {
        J2CSTRING(mailUidStr, mailUid);
        J2CSTRING(fromUidStr, fromUid);
        CCLOGFUNCF("tabType:%d  type:%d  mailUid:%s  fromUid:%s",(int)tabType,(int)type,mailUidStr.c_str(),fromUidStr.c_str());
        
        if (tabType<0 || mailUidStr=="") {
            return;
        }
        MailController::getInstance()->deleteMailFromAndroid(tabType,type,mailUidStr,fromUidStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_deleteMutiMail(JNIEnv* env, jobject object,jstring mailUids,jstring types)
    {
        J2CSTRING(mailUidStr, mailUids);
        J2CSTRING(typeStr, types);
        CCLOGFUNCF("mailUids:%s  types:%s",mailUidStr.c_str(),typeStr.c_str());
        
        if (mailUidStr=="" && typeStr=="") {
            return;
        }
        MailController::getInstance()->deleteMailBySelectFromAndroid(mailUidStr,typeStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_rewardMutiMail(JNIEnv* env, jobject object,jstring mailUids,jstring types)
    {
        J2CSTRING(mailUidStr, mailUids);
        J2CSTRING(typeStr, types);
        CCLOGFUNCF("mailUids:%s  types:%s",mailUidStr.c_str(),typeStr.c_str());
        
        if (mailUidStr=="" && typeStr=="") {
            return;
        }
//        MailController::getInstance()->getMailRewardBySelectFromAndroid(mailUidStr,typeStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_testMailCommand(JNIEnv* env, jobject object)
    {
        CCLOGFUNC("");
        ChatController::getInstance()->testMailCommand();
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_readMail(JNIEnv* env, jobject object,jstring mailUid,jint type)
    {
        CCLOGFUNC("");
         J2CSTRING(mailUidStr, mailUid);
        CCLOGFUNCF("mailUid:%s  type:%d",mailUidStr.c_str(),type);
         MailController::getInstance()->notyfyReadMail(mailUidStr, type);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_readMutiMail(JNIEnv* env, jobject object,jstring mailUids)
    {
        CCLOGFUNC("");
        J2CSTRING(mailUidsStr, mailUids);
        CCLOGFUNCF("mailUidsStr:%s",mailUidsStr.c_str());
//        MailController::getInstance()->notifyReadMutiMail(mailUidsStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_readChatMail(JNIEnv* env, jobject object,jstring fromUser,jboolean isMod)
    {
        CCLOGFUNC("");
        J2CSTRING(fromUserStr, fromUser);
        CCLOGFUNCF("fromUserStr:%s",fromUserStr.c_str());
        MailController::getInstance()->notyfyReadChatMail(fromUserStr, (bool)isMod);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_readDialogMail(JNIEnv* env, jobject object,jint type,jboolean isMod,jstring types)
    {
        CCLOGFUNC("");
         J2CSTRING(typesStr, types);
        CCLOGFUNCF("type:%d  typesStr:%s",type,typesStr.c_str());
//        MailController::getInstance()->notyfyReadDialogMail((int)type, (bool)isMod,typesStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_getUpdateMail(JNIEnv* env, jobject object,jstring lastModifyTime)
    {
        CCLOGFUNC("");
        J2CSTRING(lastModifyTimeStr, lastModifyTime);
        CCLOGFUNCF("lastModifyTimeStr:%s",lastModifyTimeStr.c_str());
        MailController::getInstance()->getUpdateMail(lastModifyTimeStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_postUnreadMailNum(JNIEnv* env, jobject object,jint unreadNum)
    {
        CCLOGFUNCF("unreadNum:%d",unreadNum);
        MailController::getInstance()->setNewMailUnreadNum(unreadNum);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(JNIScheduleObject::refreshUI), JNIScheduleObject::getInstance(), 0.0f, 0, 0.1f, false);
    }
    
    JNIEXPORT jstring JNICALL Java_com_elex_chatservice_host_GameHost_getNameById(JNIEnv *env,
                                                                     jobject object,
                                                                     jstring _xmlId){
        J2CSTRING(xmlId, _xmlId);
        CCLOGFUNCF("getNameById _xmlId:%s",xmlId.c_str());
        string name=CCCommonUtils::getNameById(xmlId);
        return env->NewStringUTF(name.c_str());
    }
    
    JNIEXPORT jstring JNICALL Java_com_elex_chatservice_host_GameHost_getPropById(JNIEnv *env,
                                                                jobject object,
                                                                jstring _xmlId,
                                                                jstring _proName){
        J2CSTRING(xmlId, _xmlId);
         J2CSTRING(proName, _proName);
        CCLOGFUNCF("getPropById _xmlId:%s  proName:%s",xmlId.c_str(),proName.c_str());
        string proValue=CCCommonUtils::getPropById(xmlId,proName);
        return env->NewStringUTF(proValue.c_str());
    }
    
    JNIEXPORT jstring JNICALL Java_com_elex_chatservice_host_GameHost_getPicByType(JNIEnv *env,
                                                                jobject object,
                                                                jint type,
                                                                jint value){
        CCLOGFUNCF("getPicByType type:%d  value:%d",type, value);
        string pic=RewardController::getInstance()->getPicByType(type, value);
        return env->NewStringUTF(pic.c_str());
    }
    
    JNIEXPORT jstring JNICALL Java_com_elex_chatservice_host_GameHost_getPointByIndex(JNIEnv *env,
                                                                jobject object,
                                                                jint occupyPointId){

        CCLOGFUNCF("getPointByIndex occupyPointId:%d",occupyPointId);
        string pointStr = MailController::getInstance()->getPointByOccupyIdx((int)occupyPointId);
        return env->NewStringUTF(pointStr.c_str());
    }
    
    JNIEXPORT jstring JNICALL Java_com_elex_chatservice_host_GameHost_getPointByMapTypeAndIndex(JNIEnv *env,
                                                                    jobject object,
                                                                    jint occupyPointId,
                                                                    jint serverType){
        CCLOGFUNCF("getPointByMapTypeAndIndex occupyPointId:%d  serverType:%d",occupyPointId,serverType);
        string pointStr = "hh";
//        MailController::getInstance()->getPointByMapTypeAndIndex((int)occupyPointId,(jint)serverType);
        return env->NewStringUTF(pointStr.c_str());
    }
    
    JNIEXPORT jstring JNICALL Java_com_elex_chatservice_host_GameHost_getLang(JNIEnv *env,
                                                                jobject object,
                                                                jstring lang){
        if (LocalController::shared()->TextINIManager()==NULL) {
            CCLOGFUNC("lang do not init!");
            return env->NewStringUTF("");
        }
        J2CSTRING(langStr, lang);
        CCLOGFUNCF("langStr %s:",langStr.c_str());
        return env->NewStringUTF(_lang(langStr.c_str()).c_str());
    }
    
    JNIEXPORT jstring JNICALL Java_com_elex_chatservice_host_GameHost_getLang1ByKey(JNIEnv *env,
                                                                jobject object,
                                                                jstring lang,
                                                                jstring key1){
        if (LocalController::shared()->TextINIManager()==NULL) {
            CCLOGFUNC("lang do not init!");
            return env->NewStringUTF("");
        }
        J2CSTRING(langStr, lang);
        J2CSTRING(keyStr1, key1);
        CCLOGFUNCF("langStr %s:",langStr.c_str());
        CCLOGFUNCF("keyStr1 %s:",keyStr1.c_str());
        return env->NewStringUTF(_lang_1(langStr.c_str(),keyStr1.c_str()));
    }
    
    JNIEXPORT jstring JNICALL Java_com_elex_chatservice_host_GameHost_getLang2ByKey(JNIEnv *env,
                                                                jobject object,
                                                                jstring lang,
                                                                jstring key1,
                                                                jstring key2){
        J2CSTRING(langStr, lang);
        J2CSTRING(keyStr1, key1);
        J2CSTRING(keyStr2, key2);
        if (LocalController::shared()->TextINIManager()==NULL) {
            CCLOGFUNC("lang do not init!");
            return env->NewStringUTF("");
        }
        CCLOGFUNCF("langStr %s:",langStr.c_str());
        CCLOGFUNCF("keyStr1 %s:",keyStr1.c_str());
        CCLOGFUNCF("keyStr2 %s:",keyStr2.c_str());
        return env->NewStringUTF(_lang_2(langStr.c_str(),keyStr1.c_str(),keyStr2.c_str()));
    }
    
    JNIEXPORT jstring JNICALL Java_com_elex_chatservice_host_GameHost_getLang3ByKey(JNIEnv *env,
                                                                  jobject object,
                                                                  jstring lang,
                                                                  jstring key1,
                                                                  jstring key2,
                                                                  jstring key3){
        J2CSTRING(langStr, lang);
        J2CSTRING(keyStr1, key1);
        J2CSTRING(keyStr2, key2);
        J2CSTRING(keyStr3, key3);
        if (LocalController::shared()->TextINIManager()==NULL) {
            CCLOGFUNC("lang do not init!");
            return env->NewStringUTF("");
        }
        CCLOGFUNCF("langStr %s:",langStr.c_str());
        CCLOGFUNCF("keyStr1 %s:",keyStr1.c_str());
        CCLOGFUNCF("keyStr2 %s:",keyStr2.c_str());
        CCLOGFUNCF("keyStr3 %s:",keyStr3.c_str());
        return env->NewStringUTF(_lang_3(langStr.c_str(),keyStr1.c_str(),keyStr2.c_str(),keyStr3.c_str()));
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_changeMailListSwitch(JNIEnv *env,
                                                              jobject object,
                                                              jboolean isOn){

        CCLOGFUNCF("isOn:%d",isOn);
        MailController::getInstance()->setIsNewMailListEnable((bool)isOn);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_translateMsgByLua(JNIEnv *env,
                                                                 jobject object,
                                                                 jstring originMsg,
                                                                 jstring targetLang){
        J2CSTRING(originMsgStr, originMsg);
        J2CSTRING(targetLangStr, targetLang);
        CCLOGFUNCF("originMsgStr : %s",originMsgStr.c_str());
        CCLOGFUNCF("targetLangStr : %s",targetLangStr.c_str());
        LuaController::getInstance()->translate(originMsgStr, targetLangStr);
    }
    
    JNIEXPORT bool JNICALL Java_com_elex_chatservice_host_GameHost_canTransalteByLua(JNIEnv *env,
                                                                   jobject object){
        CCLOG("Java_com_elex_chatservice_host_GameHost_canTransalteByLua");
        return LuaController::getInstance()->canTranslate();
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_reportCustomHeadImg(JNIEnv *env,
                                                                   jobject object,
                                                                   jstring uid){
        J2CSTRING(uidStr, uid);
        CCLOGFUNCF("uidStr %s:",uidStr.c_str());
        ChatController::getInstance()->reportCustomHeadPic(uidStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_reportPlayerChatContent(JNIEnv *env,
                                                                     jobject object,
                                                                     jstring uid,
                                                                         jstring msg){
        J2CSTRING(uidStr, uid);
        J2CSTRING(msgStr, msg);
        CCLOGFUNCF("uidStr: %s  msgStr:%s",uidStr.c_str(),msgStr.c_str());
//        ChatController::getInstance()->reportPlayerChatContent(uidStr,msgStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_getRedPackageStatus(JNIEnv *env,
                                                                                           jobject object,
                                                                                           jstring redPackageUid){
        J2CSTRING(redPackageUidStr, redPackageUid);
        CCLOGFUNCF("redPackageUidStr: %s",redPackageUidStr.c_str());
        std::vector<string> redPackInfoArr;
        CCCommonUtils::splitString(redPackageUidStr, "_", redPackInfoArr);
        if(redPackInfoArr.size() == 2)
        {
            CCLOGFUNCF("redPackageUidStr: %s  serverId:%s",redPackInfoArr.at(0).c_str(),redPackInfoArr.at(1).c_str());
//            ChatController::getInstance()->getRedPackageStatus(redPackInfoArr.at(0),redPackInfoArr.at(1));
        }
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_postFriendLatestMail(JNIEnv *env,
                                                                                       jobject object,
                                                                                       jstring json){
        J2CSTRING(jsonStr, json);
        CCLOGFUNCF("jsonStr: %s",jsonStr.c_str());
//        MailController::getInstance()->parseUserMailInfo(jsonStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_postChatLatestInfo(JNIEnv *env,
                                                                                        jobject object,
                                                                                        jstring json){
        J2CSTRING(jsonStr, json);
        CCLOGFUNCF("jsonStr: %s",jsonStr.c_str());
//        ChatController::getInstance()->parseLatestChatInfoStr(jsonStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_changeNickName(JNIEnv *env,
                                                                         jobject object){
        CCLOGFUNCF("");
//        ChatController::getInstance()->changeNickName();
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_translateOptimize(JNIEnv *env,
                                                                   jobject object,
                                                                   jstring method,
                                                                   jstring originalLang,
                                                                   jstring userLang,
                                                                   jstring msg,
                                                                   jstring translationMsg){
        J2CSTRING(methodStr, method);
        J2CSTRING(originalLangStr, originalLang);
        J2CSTRING(userLangStr, userLang);
        J2CSTRING(msgStr, msg);
        J2CSTRING(translationMsgStr, translationMsg);
        
        CCLOGFUNCF("methodStr : %s",methodStr.c_str());
        CCLOGFUNCF("originalLangStr : %s",originalLangStr.c_str());
        CCLOGFUNCF("userLangStr : %s",userLangStr.c_str());
        CCLOGFUNCF("msgStr : %s",msgStr.c_str());
        CCLOGFUNCF("translationMsgStr : %s",translationMsgStr.c_str());
        
//        ChatController::getInstance()->translateOptimize(methodStr, originalLangStr,userLangStr,msgStr,translationMsgStr);
    }
    
    JNIEXPORT void JNICALL Java_com_elex_chatservice_host_GameHost_completeInitDatabase(){
        CCLOGFUNCF("");
        ChatServiceCocos2dx::completeInitDatabase();
    }
}

static JNIScheduleObject* _scheduleObjectInstance = NULL;

JNIScheduleObject::JNIScheduleObject()
{
    actionAfterResume = "";
    uid = "";
    name = "";
    attachmentId="";
    waitingForReturnToChatAfterPopup = false;
    returnToChatAfterPopup = false;
}

void JNIScheduleObject::goBackToNewMailList()
{
    if(!(waitingForReturnToChatAfterPopup && ChatServiceCocos2dx::isChatDialogNeedBack))
        return;
    ChatServiceCocos2dx::isChatDialogNeedBack = false;
    waitingForReturnToChatAfterPopup = false;
    ChatServiceCocos2dx::m_rememberPosition = true;
    
    if(actionAfterResume=="showMailPopup")
    {
        ChatServiceCocos2dx::m_channelType=-1;
        ChatServiceCocos2dx::showChannelListFrom2dx(true);
    }
    else
    {
        if(!ChatServiceCocos2dx::isChatShowing)
        {
            if (actionAfterResume=="showPlayerInfo@mod") {
                ChatServiceCocos2dx::setContactModState();
            }
            ChatServiceCocos2dx::showChatActivityFrom2dx();
        }
    }
    
    actionAfterResume = "";
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_CLOSE_EVENT);
}

void JNIScheduleObject::popupCloseEvent(CCObject *params)
{
    CCLOG("popupCloseEvent isChatDialogNeedBack = %d", ChatServiceCocos2dx::isChatDialogNeedBack);
    CCLOG("popupCloseEvent waitingForReturnToChatAfterPopup = %d", waitingForReturnToChatAfterPopup);
    CCLOG("popupCloseEvent popUpCount = %d m_curPopupWindowNum = %d", (PopupViewController::getInstance()->getCurrViewCount() +PopupViewController::getInstance()->getGoBackViewCount()), ChatServiceCocos2dx::m_curPopupWindowNum);
    if((PopupViewController::getInstance()->getCurrViewCount() +PopupViewController::getInstance()->getGoBackViewCount())== ChatServiceCocos2dx::m_curPopupWindowNum && waitingForReturnToChatAfterPopup && ChatServiceCocos2dx::isChatDialogNeedBack)
    {
        goBackToNewMailList();
    }
}

void JNIScheduleObject::stopReturnToChat()
{
    CCLOGFUNC("stopReturnToChat 1");
    if(waitingForReturnToChatAfterPopup)
    {
        CCLOGFUNC("stopReturnToChat 2");
        waitingForReturnToChatAfterPopup = false;
        ChatServiceCocos2dx::isChatDialogNeedBack = false;
        actionAfterResume = "";
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_CLOSE_EVENT);
    }
}

JNIScheduleObject::~JNIScheduleObject()
{
}
JNIScheduleObject* JNIScheduleObject::getInstance()
{
    if (_scheduleObjectInstance == NULL) {
        _scheduleObjectInstance = new JNIScheduleObject();
    }
    
    return _scheduleObjectInstance;
}

void JNIScheduleObject::showLatestMessage(float time){
    CCLOG("JNIScheduleObject::showLatestMessage");
//    ChatController::getInstance()->getLatestMessage();
//    if (ChatServiceCocos2dx::m_channelType==CHANNEL_TYPE_COUNTRY) {
//        UIComponent::getInstance()->showCountryIcon(true);
//        ChatController::getInstance()->showLatestMessage(0);
//    }
//    else if (ChatServiceCocos2dx::m_channelType==CHANNEL_TYPE_ALLIANCE)
//    {
//        UIComponent::getInstance()->showCountryIcon(false);
//        ChatController::getInstance()->showLatestMessage(2);
//    }
    
    
    UIComponent::getInstance()->refreshUIComponent();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(JNIScheduleObject::showLatestMessage), this);
}

void JNIScheduleObject::postRetrunGame(float time){
    CCLOG("JNIScheduleObject::postRetrunGame");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("MSG_ReturnFrom_ChatOrMail");
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(JNIScheduleObject::postRetrunGame), this);
}

void JNIScheduleObject::flyMutiMailReward(float time){
    CCLOG("JNIScheduleObject::flyMutiMailReward");
//    MailController::getInstance()->flyMutiMailReward();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(JNIScheduleObject::flyMutiMailReward), this);
}

void JNIScheduleObject::updateMailCell(float time){
    CCLOG("updateMailCell");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("FriendsViewBackFromMail");
    MailController::getInstance()->updateMailList();
    UIComponent::getInstance()->refreshUIComponent();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(JNIScheduleObject::updateMailCell), this);
    MailController::getInstance()->m_mailInfoSendDic->removeAllObjects();
    ChatServiceCocos2dx::m_curSendMailIndex=0;
    if(!ChatServiceCocos2dx::isChatDialogNeedBack)
    {
        ChatServiceCocos2dx::m_channelType=-1;
//        ChatServiceCocos2dx::m_isInMailDialog=false;
//        ChatServiceCocos2dx::m_curMailUid="";
    }
}

void JNIScheduleObject::postNotifyUIRefresh(float time){
//    NetController::shared()->checkGameConnection();
    if((PopupViewController::getInstance()->getCurrViewCount() + PopupViewController::getInstance()->getGoBackViewCount()) > 0)
        return;
    CCLOG("postNotifyUIRefresh");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_VIEW_OUT);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCENE_CHANGED);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(JNIScheduleObject::postNotifyUIRefresh), this);
}

void JNIScheduleObject::refreshMailWriteName(float time){
    CCLOG("refreshMailWriteName");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHAT_ROOM_NAME_REFRESH);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(JNIScheduleObject::refreshMailWriteName), this);
}

void JNIScheduleObject::getMultiUserInfo(float time){
    CCLOG("JNIScheduleObject::getMultiUserInfo");
    
    vector<std::string> *uids = new vector<std::string>();
    vector<string> strVec;
//    CCLOGFUNCF("uidStr %s",ChatServiceCocos2dx::uidStr.c_str());
    CCCommonUtils::splitString(ChatServiceCocos2dx::uidStr, "|", strVec);
//    CCLOGFUNCF("strVec.size() :%d",strVec.size());
    
    for (int i = 0; i<strVec.size(); i++)
    {
//        CCLOGFUNCF("strVec.at(i) :%s",strVec.at(i).c_str());
        uids->push_back(strVec.at(i));
    }
    ChatServiceCocos2dx::uidStr = "";
    
    ChatController::getInstance()->getMultiUserInfo(uids);
    delete uids;
}

void JNIScheduleObject::refreshUI(float time){
    CCLOGFUNC("");
    
    UIComponent::getInstance()->updateMailNum(NULL);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(JNIScheduleObject::refreshUI), this);
}

void JNIScheduleObject::handleResume(float time){
    CCLOG("JNIScheduleObject::handleResume %s", actionAfterResume.c_str());
    
    ChatServiceCocos2dx::isChatDialogNeedBack=true;
    
    if(actionAfterResume == "joinAlliance"){
        ChatController::getInstance()->joinAlliance(uid);
    }else if(actionAfterResume == "sendMail"){
        if(ChatServiceCocos2dx::enableNativeMail)
            returnToChatAfterPopup = false;
        ChatController::getInstance()->sendMail(name,uid);
    }else if(actionAfterResume == "showPlayerInfo" || actionAfterResume == "showPlayerInfo@mod"){
        ChatController::getInstance()->showPlayerInfo(uid);
    }else if(actionAfterResume == "joinAllianceBtnClick"){
        ChatController::getInstance()->joinAllianceBtnClick();
    }
    else if(actionAfterResume == "viewBattleReport"){
        CCLOG("JNIScheduleObject viewBattleReport");
        ChatController::getInstance()->viewBattleReport(uid,attachmentId);
    }
    else if(actionAfterResume == "viewDetectReport"){
        CCLOG("JNIScheduleObject viewDetectReport");
        ChatController::getInstance()->viewDetectReport(uid,attachmentId);
    }
    else if(actionAfterResume == "banPlayer"){
        ChatController::getInstance()->shieldPlayer(uid);
              ChatController::getInstance()->inviteJoinAlliance(uid,name);
    }else if(actionAfterResume == "gotoCoordinate"){
        CCPoint pt = ccp(atoi(uid.c_str()), atoi(name.c_str()));
        int worldIndex = WorldController::getIndexByPoint(pt);
        WorldController::getInstance()->openTargetIndex = worldIndex;
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            if((PopupViewController::getInstance()->getCurrViewCount() + PopupViewController::getInstance()->getGoBackViewCount()) > 0){
                PopupViewController::getInstance()->removeAllPopupView();
                //zym 2015.12.11
//                PopupViewController::getInstance()->forceClearAll(true);
            }
            WorldMapView::instance()->gotoTilePoint(pt);
        }else{
            int index = WorldController::getIndexByPoint(pt);
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, worldIndex);
        }
    }
    else if(actionAfterResume == "showMailPopup"){
        CCLOGFUNCF("showMailPopup uid:%s",uid.c_str());
        auto search = GlobalData::shared()->mailList.find(uid.c_str());
        bool isExistMail=(search != GlobalData::shared()->mailList.end());
        
        
        MailInfo* mailInfo=NULL;
        if (isExistMail) {
            mailInfo=dynamic_cast<MailInfo*>(search->second);
        }
        
        if(mailInfo!=NULL)
        {
            CCLOGFUNC("mailInfo!=NULL");
            MailController::getInstance()->showMailPopupFromAnroid(mailInfo);
        }
    }
    else if(actionAfterResume == "showEquipment"){
        CCLOG("JNIScheduleObject showEquipment");
        ChatController::getInstance()->viewEquipment(attachmentId);
    }
    else if(actionAfterResume == "viewRallyInfo"){
        CCLOG("JNIScheduleObject viewRallyInfo");
//        ChatController::getInstance()->viewRallyInfo(attachmentId);
    }
    else if(actionAfterResume == "viewLotteryShare"){
        CCLOG("JNIScheduleObject viewLotteryShare");
//        ChatController::getInstance()->viewLotteryInfo(attachmentId);
    }
    else if(actionAfterResume == "viewAllianceTaskShare"){
        CCLOG("JNIScheduleObject viewAllianceTaskShare");
//        ChatController::getInstance()->viewAllianceTaskInfo();
    }
    else if(actionAfterResume == "changeNickName"){
        CCLOG("JNIScheduleObject changeNickName");
//      ChatController::getInstance()->changeNickName();
    }
    else if(actionAfterResume == "showFriend"){
        CCLOG("JNIScheduleObject showFriend");
//        PopupViewController::getInstance()->addPopupInView(FriendsView::create());  simon
    }
    else if(actionAfterResume == "viewRedPackage"){
        CCLOG("JNIScheduleObject viewRedPackage");
        std::vector<string> redPackInfoArr;
        CCCommonUtils::splitString(attachmentId, "_", redPackInfoArr);
        if(redPackInfoArr.size() == 2)
        {
//            ChatController::getInstance()->viewRedPackage(redPackInfoArr.at(0),redPackInfoArr.at(1),true);
        }
    }
    else if(actionAfterResume == "pickRedPackage"){
        CCLOG("JNIScheduleObject pickRedPackage");
        std::vector<string> redPackInfoArr;
        CCCommonUtils::splitString(attachmentId, "_", redPackInfoArr);
        if(redPackInfoArr.size() == 2)
        {
//            ChatController::getInstance()->viewRedPackage(redPackInfoArr.at(0),redPackInfoArr.at(1));
        }
    }
    else{
        return;
    }
    
    if(returnToChatAfterPopup && !waitingForReturnToChatAfterPopup){
        waitingForReturnToChatAfterPopup = true;
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(JNIScheduleObject::popupCloseEvent), MSG_POPUP_CLOSE_EVENT, NULL);
    }
    else{
         actionAfterResume = "";
    }
    
    uid = "";
    name = "";
    attachmentId = "";
    returnToChatAfterPopup = false;
}

void JNIScheduleObject::confirmDisableDontKeepActivities()
{
    YesNoDialog::showVariableTitle(_lang("132131").c_str(),
                                   CCCallFunc::create(this, callfunc_selector(JNIScheduleObject::onConfirmDisableDontKeepActivities)),
                                   _lang("132132").c_str(),
                                   false,
                                   0);
}

void JNIScheduleObject::onConfirmDisableDontKeepActivities()
{
    ChatServiceCocos2dx::gotoDevelopmentSetting();
}

#endif
