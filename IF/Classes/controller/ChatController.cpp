//
//  ChatController.cpp
//  IF
//
//  Created by fubin on 13-9-26.
//
//

#include "ChatController.h"
#include "CountryChatCommand.h"
#include "UIComponent.h"
#include "SceneController.h"
#include "UIComponent.h"
#include "TranslateCommand.h"
#include "MailController.h"
#include "PopupViewController.h"
#include "GetUserInfoCommand.h"
#include "RoleInfoView.h"
#include "ChatUnLockCommand.h"
#include "ChatLockCommand.h"
#include "CheckAllianceInfoView.h"
#include "JoinAllianceView.h"
#include "MailReadCommand.h"
#include "BattleReportMailPopUpView.h"
#include "InvitesAllianceCommand.h"
#include "ApplyAllianceCommand.h"
#include "DetectMailPopUpView.h"
#include "ChatServiceCocos2dx.h"
#include "ToolController.h"
#include "LuaController.h"
#include "GetMultiUserInfoCommand.h"
#include "GetMsgBySeqIdCommand.h"
#include "GetNewMsgCommand.h"
#include "GetNewMailMsgCommand.h"
#include "GetChatRoomMemberCommand.h"
#include "NetController.h"
#include "ChatTestCommand.h"
#include "EquipmentBagView.h"
#include "ReportCustomHeadPicCommand.h"

static ChatController *_instance = NULL;

ChatController::~ChatController() {
    CC_SAFE_RELEASE_NULL(m_chatInfoSendDic);
    CC_SAFE_RELEASE_NULL(m_userInfoDic);
    CC_SAFE_RELEASE_NULL(m_chatUserInfoDic);
}

ChatController* ChatController::getInstance() {
    if (!_instance) {
        
        _instance = new ChatController();
    }
    return _instance;
}

void ChatController::purgeData() {
    delete _instance;
    _instance = NULL;
}

//发送聊天信息
bool ChatController::sendCountryChat(const char* msg, int type, int post,std::string sendLocalTime/*=""*/)
{
    string tmp = "";
    tmp = tmp + msg;
    int begin = 0;
    begin = tmp.find(" ",begin);
    while(begin != -1){
        tmp.replace(begin, 1, "");
        begin = tmp.find(" ",begin);
    }
    
    if (msg==NULL || tmp.length()==0) {
        auto _scene = CCDirector::sharedDirector()->getRunningScene();
        if (_scene->getPositionY() != 0) {
            _scene->setPositionY(0);
        }
//        CCCommonUtils::flyText(_lang("105311"));
        CCCommonUtils::flyHint("icon_Chat.png", _lang("105316"), _lang("105311"));
        return false;
    }
    
    CCLOG("sendCountryChat type :%d",type);
    
    if (type == CHAT_COUNTRY) {
        CountryChatCommand * cmd = new CountryChatCommand(CHAT_STATE_COUNTRY_COMMAND,msg,post,sendLocalTime);
        cmd->sendAndRelease();
    }
    else if (type == CHAT_ALLIANCE){
        CountryChatCommand * cmd = new CountryChatCommand(CHAT_STATE_ALLIANCE_COMMAND,msg,post,sendLocalTime);
        cmd->sendAndRelease();
    }
    return true;
}

bool ChatController::sendSelfChat(const char* msg, const char* uid, const char* name)
{
//    if (msg==NULL) {
//        return false;
//    }
//    
//    SelfChatCommand * cmd = new SelfChatCommand(msg, uid, name);
//    cmd->sendAndRelease();
//    
//    ChatInfo info = ChatInfo();
//    info.name = GlobalData::shared()->playerInfo.name;
//    info.uid = GlobalData::shared()->playerInfo.uid;
//    info.msg = msg;
//    info.type = CHAT_SELF;
//    info.time = GlobalData::shared()->getWorldTime()*1000;
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAT_UPDATE);
    
    return true;
}

/**收到新消息，包括自己发送的消息状态*/
void ChatController::receiveChat(CCDictionary* dict)
{
    auto info = ChatInfo(dict);
    if (info.type == CHAT_COUNTRY) {
        m_chatCountryPool.push_back(info);
        if(info.sequenceId>ChatController::getInstance()->m_latestCountryMsg.sequenceId)
            ChatController::getInstance()->m_latestCountryMsg=info;
        if(info.post == CHAT_TYPE_NOTICE){
            LuaController::getInstance()->addChatNotice(dict);
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        CCLOG("receiveChat");
        ChatMailInfo* chatInfo=ChatMailInfo::create(info,true);
        CCArray* chatInfoArr=CCArray::create();
        chatInfoArr->addObject(chatInfo);
        ChatServiceCocos2dx::handleChatPush(chatInfoArr,CHANNEL_TYPE_COUNTRY,"0");
#endif
    }
    else if (info.type == CHAT_ALLIANCE) {
        m_chatAlliancePool.push_back(info);
        if(info.sequenceId>ChatController::getInstance()->m_latestAllianceMsg.sequenceId)
            ChatController::getInstance()->m_latestAllianceMsg=info;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        CCLOG("receiveChat");
        ChatMailInfo* chatInfo=ChatMailInfo::create(info,true);
        CCArray* chatInfoArr=CCArray::create();
        chatInfoArr->addObject(chatInfo);
        ChatServiceCocos2dx::handleChatPush(chatInfoArr,CHANNEL_TYPE_ALLIANCE,"1");
#endif
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG("receiveChat");
    ChatMailInfo* chatInfo=ChatMailInfo::create(info,true);
    CCArray* chatInfoArr=CCArray::create();
    chatInfoArr->addObject(chatInfo);
    notifyChatMsgToAndroid(chatInfoArr);
#endif
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAT_UPDATE);

    if (SceneController::getInstance()->currentSceneId==SCENE_ID_BATTLE){
        return;
    }
    else {
        
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        if(ChatServiceCocos2dx::enableNativeChat){
            if(!ChatServiceCocos2dx::isChatShowing)
                UIComponent::getInstance()->refreshChatInfo(info.type);
        }
        else
        {
            UIComponent::getInstance()->updateChatInfo(info.type);
        }
#else
        if(ChatServiceCocos2dx::enableNativeChat){
            if(!ChatServiceCocos2dx::isChatShowing_fun())
                UIComponent::getInstance()->updateChatInfoIOS(info.type);
        }
        else
        {
            UIComponent::getInstance()->updateChatInfo(info.type);
        }
        //IOS需要添加自己的方法
#endif
    }
}

void ChatController::notifyChatMsgToAndroid(CCArray* chatInfoArr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_chatInfoSendDic->setObject(chatInfoArr, ChatServiceCocos2dx::m_curSendChatIndex);
    ChatServiceCocos2dx::notifyMessageIndex(ChatServiceCocos2dx::m_curSendChatIndex,"0");
    ChatServiceCocos2dx::m_curSendChatIndex++;
#endif
}

void ChatController::notifyChatMsgToIOS(cocos2d::CCArray *chatInfoArr, int channelType, string id)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChatServiceCocos2dx::handleChatPush(chatInfoArr, channelType,id);
    ChatServiceCocos2dx::m_curSendChatIndex++;
#endif
}

//发送邮件
void ChatController::sendMail(string name,string uid)
{
    MailController::getInstance()->openMailDialogViewFirst(name, uid);
}


//显示玩家信息
void ChatController::showPlayerInfo(string uid)
{
    if(uid==GlobalData::shared()->playerInfo.uid){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(!ChatServiceCocos2dx::isChatDialogNeedBack)
            PopupViewController::getInstance()->removeAllPopupView();
#else
        PopupViewController::getInstance()->removeAllPopupView();
#endif
        PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(&GlobalData::shared()->playerInfo,1));
    }else{
        RoleInfoView::createInfoByUid(uid);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //    ChatServiceCocos2dx::hideChatInputView();
#endif
//        GetUserInfoCommand* cmd = new GetUserInfoCommand(uid);
//        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatController::getUserInfoCallback), NULL));
//        cmd->sendAndRelease();
    }
}

//接受联盟广播邀请
void ChatController::joinAnnounceInvitation(string allianceId)
{
    ApplyAllianceCommand* cmd = new ApplyAllianceCommand(allianceId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatController::joinAnnounceInvitationSuccess), NULL));
    cmd->sendAndRelease();
//    GameController::getInstance()->showWaitInterface();
}

//查看玩家战报
void ChatController::viewBattleReport(string uid,string reportUid)
{
    m_battleReportId=reportUid;
    m_battleReportPlayerUid=uid;
    MailReadCommand* cmd = new MailReadCommand(reportUid, CC_ITOA(MAIL_BATTLE_REPORT));
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChatController::viewBattleReportSuccess), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

//查看装备
void ChatController::viewEquipment(string equipId)
{
    CCLOGFUNCF("equipId:%s",equipId.c_str());
    int id = atoi(equipId.c_str());
    PopupViewController::getInstance()->addPopupInView(EquipInfoView::createWithEquipId(id));
}

//查看玩家侦察战报
void ChatController::viewDetectReport(string uid,string detectReportUid)
{
    CCLOG("detectReportUid :%s\n",detectReportUid.c_str());
    CCLOG("uid :%s\n",uid.c_str());
    UIComponent::getInstance()->loadMailResource();
    m_detectReportId=detectReportUid;
    m_detectReportPlayerUid=uid;
    MailReadCommand* cmd = new MailReadCommand(detectReportUid, CC_ITOA(MAIL_DETECT_REPORT));
    cmd->setFromChat();
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChatController::viewDetectReportSuccess), NULL));
    cmd->sendAndRelease();
}

void ChatController::viewDetectReportSuccess(CCObject* pObj)
{
    CCLOG("viewDetectReportSuccess");
    CCLOG("m_detectReportPlayerUid :%s\n",m_detectReportPlayerUid.c_str());
    CCLOG("m_detectReportId :%s\n",m_detectReportId.c_str());
    NetResult* result = dynamic_cast<NetResult*>(pObj);
    if(result==NULL) return;
    auto dictInfo = _dict(result->getData());
    CCArray* arr = NULL;
    if(dictInfo && dictInfo->objectForKey("result")){
        arr = dynamic_cast<CCArray*>(dictInfo->objectForKey("result"));
    }
    if(arr && arr->count()>0){
        auto dic = _dict(arr->objectAtIndex(0));
        MailInfo* m_mailInfo = MailInfo::create();
        if (m_detectReportId!="" && m_detectReportPlayerUid!="") {
            m_mailInfo->uid = m_detectReportId;
            m_mailInfo->shareUid = m_detectReportPlayerUid;
        }
        m_mailInfo->type = MAIL_DETECT_REPORT;
        m_mailInfo->fromName = _lang("105522");
        m_mailInfo->isReadContent = true;
        m_mailInfo->detectReport = dic;
        m_mailInfo->detectReport->retain();
        CCString *str = dynamic_cast<CCString*>(dic->objectForKey("pointType"));
        if(str && str->getCString()){
            m_mailInfo->pointType =  str->intValue();
        }
        m_mailInfo->isFromChat = true;
        m_mailInfo->createTime = dic->valueForKey("createTime")->doubleValue() / 1000;
        m_mailInfo->retain();
        
        PopupViewController::getInstance()->addPopupInView(DetectMailPopUpView::create(m_mailInfo));
    }else{
        CCCommonUtils::flyHint("", "", _lang("115299"));
    }
    
    m_detectReportId="";
    m_detectReportPlayerUid="";
    
}

//邀请加入联盟
void ChatController::inviteJoinAlliance(string uid,string name)
{
    m_invitePlayerName=name;
    InvitesAllianceCommand* cmd = new InvitesAllianceCommand(uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatController::inviteFunc), NULL));
    cmd->sendAndRelease();
}

void ChatController::inviteFunc(CCObject* obj){
    if(m_invitePlayerName=="")
        return ;
    string name = m_invitePlayerName;
    string msg = _lang_1("115182", name.c_str());
    msg.append("  (").append(_lang("115181")).append(")");
    CCLOG("invite %s",msg.c_str());
    ChatController::getInstance()->sendCountryChat(msg.c_str(), CHAT_ALLIANCE, 2);
    m_invitePlayerName="";
}

void ChatController::joinAnnounceInvitationSuccess(CCObject* pObj)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::onJoinAnnounceInvitationSuccess();
#endif
}

void ChatController::viewBattleReportSuccess(CCObject* pObj)
{
    NetResult* result = dynamic_cast<NetResult*>(pObj);
    if(result==NULL) return;
    auto dictInfo = _dict(result->getData());
    CCArray* arr = NULL;
    if(dictInfo && dictInfo->objectForKey("result")){
        arr = dynamic_cast<CCArray*>(dictInfo->objectForKey("result"));
    }
    if(arr && arr->count()>0){
        auto dic = _dict(arr->objectAtIndex(0));
        MailInfo* m_mailInfo = MailInfo::create();
        if (m_battleReportId!="" && m_battleReportPlayerUid!="") {
            m_mailInfo->uid = m_battleReportId;
            m_mailInfo->shareUid = m_battleReportPlayerUid;
        }
        
        m_mailInfo->type = MAIL_BATTLE_REPORT;
        m_mailInfo->isReadContent = true;
        
        m_mailInfo->parseBattleMail(dic);
        PopupViewController::getInstance()->addPopupInView(BattleReportMailPopUpView::create(m_mailInfo));
    }else{
        CCCommonUtils::flyHint("", "", _lang("115282"));
    }
    
    m_battleReportId="";
    m_battleReportPlayerUid="";

}


void ChatController::getUserInfoCallback(CCObject *pObj)
{
    NetResult* net = dynamic_cast<NetResult*>(pObj);
    if(net==NULL) return ;
    CCDictionary* dic = dynamic_cast<CCDictionary*>(net->getData());
    if(dic==NULL) return ;
    PlayerInfo* info = new PlayerInfo();
    info->updateInfo(dic);
    if(dic->objectForKey("alliance")){
        auto dict = dynamic_cast<CCDictionary*>(dic->objectForKey("alliance"));
        if(dict->objectForKey("alliance")){
            auto alliance = dynamic_cast<CCDictionary*>(dict->objectForKey("alliance"));
            if(alliance){
                info->setAllianceId(alliance->valueForKey("uid")->getCString());
            }
            info->allianceInfo.updateAllianceInfo(alliance);
        }
    }
    PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(info,1));
    info->release();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    ChatServiceCocos2dx::hideChatInputView();
#endif

}

void ChatController::testCommand()
{
//    testMultiUserInfo();
//    testGetMsgBySeqId();
//    testGetNewMsg();
//    testGetChatRoomMember();
    testMailCommand();
}

void ChatController::testMailCommand()
{
    ChatTestCommand* cmd=new ChatTestCommand();
    cmd->sendAndRelease();
}

void ChatController::testGetChatRoomMember()
{
    GetChatRoomMemberCommand* cmd=new GetChatRoomMemberCommand("2001758000001");
    cmd->sendAndRelease();
}

void ChatController::testGetNewMsg()
{
    GetNewMsgCommand* cmd = new GetNewMsgCommand("0|0|4");//,1392930000001|0|3
    cmd->sendAndRelease();
}

void ChatController::testGetMsgBySeqId()
{
//    GetMsgBySeqIdCommand* cmd=new GetMsgBySeqIdCommand(1,380,CHANNEL_TYPE_COUNTRY,"");
//    GetMsgBySeqIdCommand* cmd = new GetMsgBySeqIdCommand(0,100,CHANNEL_TYPE_CHATROOM,"1403943000001");

//    cmd = new GetMsgBySeqIdCommand(310,330,0,"");
    GetMsgBySeqIdCommand* cmd = new GetMsgBySeqIdCommand(0,20,CHANNEL_TYPE_ALLIANCE,"e6a74bcf632447ac890b7fe248e78fa0");
    cmd->sendAndRelease();
}

void ChatController::testMultiUserInfo()
{
    vector<std::string> *uids = new vector<std::string>();
    // 88服
//    uids->push_back("267262635000088");
//    uids->push_back("209664375000088");
    // 72服
    uids->push_back("3832223000002");
    uids->push_back("3649332000002");
    uids->push_back("1369109574000001");
    uids->push_back("1374807026000001");
    
//    uids->push_back("131948236000001");
//    uids->push_back("1361742518000001");
//    uids->push_back("1362481135000001");
//    uids->push_back("1362706369000001");
//    uids->push_back("1365075373000001");
//    uids->push_back("1369604617000001");
//    uids->push_back("136961907000001");
//    uids->push_back("3177853000002");
//    uids->push_back("3292888000002");
//    uids->push_back("407914000002");
    getMultiUserInfo(uids);
    delete uids;
}

void ChatController::reportCustomHeadPic(std::string uid)
{
    CCLOGFUNCF("uid:%s",uid.c_str());
    ReportCustomHeadPicCommand* cmd = new ReportCustomHeadPicCommand(uid);
    cmd->sendAndRelease();
}

//根据玩家uid获取玩家信息
void ChatController::getMultiUserInfo(vector<std::string> *uids)
{
    CCLOG("getMultiUserInfo");
    GetMultiUserInfoCommand* cmd = new GetMultiUserInfoCommand(uids);
//    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatController::getMultiUserInfoCallback), NULL));
    
    if(!NetController::shared()->isProcessing()){
        cmd->sendAndRelease();
    }else{
        CCLOG("isProcessing 会导致超时");
        cmd->release();
    }
}

void ChatController::getMultiUserInfoCallback(CCDictionary* dict)
{
    CCLOG("getMultiUserInfoCallback");
    CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("uids"));
    if(arr==NULL)
    {
        CCLOG("arr==NULL");
        return;
    }
    
    CCDictionary *item = NULL;

    CCArray* memberArr=CCArray::create();
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        CCDictionary* userInfoDic=CCDictionary::create();
        
//        CCDictElement * pElement;
//        CCDICT_FOREACH(item, pElement)
//        {
//            const char * key = pElement->getStrKey();
//            CCObject * value = pElement->getObject();
//            if(typeid(*value) == typeid(CCInteger)){
//                CCLOG("%s : %i", key, dynamic_cast<CCInteger*>(value)->getValue());
//            }else if(typeid(*value) == typeid(CCString)){
//                CCLOG("%s : %s", key, dynamic_cast<CCString*>(value)->getCString());
//            }
//        }

        string uid="";
        if(item->objectForKey("uid")){
            uid=item->valueForKey("uid")->getCString();
            userInfoDic->setObject(CCString::create(item->valueForKey("uid")->getCString()), "uid");
        }
        if(item->objectForKey("allianceId")){
//            CCLOGFUNCF("allianceId:%s",item->valueForKey("allianceId")->getCString());
            userInfoDic->setObject(CCString::create(item->valueForKey("allianceId")->getCString()), "allianceId");
        }
        // 没有联盟时不含此字段 联盟成员等级
        if(item->objectForKey("rank")){
            userInfoDic->setObject(CCInteger::create(item->valueForKey("rank")->intValue()), "allianceRank");
        }
        if(item->objectForKey("name")){
//            CCLOGFUNCF("getmutiuserName:%s",item->valueForKey("name")->getCString());
            userInfoDic->setObject(CCString::create(item->valueForKey("name")->getCString()), "userName");
        }
        if(item->objectForKey("pic")){
            userInfoDic->setObject(CCString::create(item->valueForKey("pic")->getCString()), "headPic");
        }
        if(item->objectForKey("picVer")){
            userInfoDic->setObject(CCInteger::create(item->valueForKey("picVer")->intValue()), "headPicVer");
        }
        if(item->objectForKey("gmFlag")){
            userInfoDic->setObject(CCInteger::create(item->valueForKey("gmFlag")->intValue()), "mGmod");
        }
        if(item->objectForKey("lastUpdateTime")){
            userInfoDic->setObject(CCInteger::create(item->valueForKey("lastUpdateTime")->intValue()), "lastUpdateTime");
        }
        if(item->objectForKey("vipLevel")){
            userInfoDic->setObject(CCInteger::create(item->valueForKey("vipLevel")->longValue()), "vipLevel");
        }
        if(item->objectForKey("vipEndTime")){
            userInfoDic->setObject(CCInteger::create(item->valueForKey("vipEndTime")->longValue()), "vipEndTime");
        }
        if(item->objectForKey("abbr")){
            userInfoDic->setObject(CCString::create(item->valueForKey("abbr")->getCString()), "asn");
        }
        if(item->objectForKey("serverId")){
            userInfoDic->setObject(CCInteger::create(item->valueForKey("serverId")->intValue()), "serverId");
        }
        if(item->objectForKey("crossFightSrcServerId")){
            userInfoDic->setObject(CCInteger::create(item->valueForKey("crossFightSrcServerId")->intValue()), "crossFightSrcServerId");
        }
        if(uid!="")
        {
            m_chatUserInfoDic->setObject(userInfoDic, uid);
        }
        
        memberArr->addObject(userInfoDic);
    }
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if (memberArr->count()>0) {
        if(memberArr->count()>20)
        {
            CCArray* userArr=CCArray::create();
            int count=0;
            for (int i=0; i<memberArr->count(); i++) {
                if(count==20)
                {
                    ChatController::getInstance()->m_userInfoDic->setObject(userArr, ChatServiceCocos2dx::m_curUserInfoIndex);
                    ChatServiceCocos2dx::notifyUserInfo(ChatServiceCocos2dx::m_curUserInfoIndex);
                    ChatServiceCocos2dx::m_curUserInfoIndex++;
                    userArr->removeAllObjects();
                    count=0;
                }
                userArr->addObject(memberArr->objectAtIndex(i));
                count++;
            }
            
            if(userArr->count()>0)
            {
                ChatController::getInstance()->m_userInfoDic->setObject(userArr, ChatServiceCocos2dx::m_curUserInfoIndex);
                ChatServiceCocos2dx::notifyUserInfo(ChatServiceCocos2dx::m_curUserInfoIndex);
                ChatServiceCocos2dx::m_curUserInfoIndex++;
            }
            
        }
        else
        {
            CCLOG("memberArr->count():%d",memberArr->count());
            ChatController::getInstance()->m_userInfoDic->setObject(memberArr, ChatServiceCocos2dx::m_curUserInfoIndex);
            ChatServiceCocos2dx::notifyUserInfo(ChatServiceCocos2dx::m_curUserInfoIndex);
            ChatServiceCocos2dx::m_curUserInfoIndex++;
        }
    }
#elif (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if (memberArr->count()>0) {
        CCLOG("memberArr->count():%d",memberArr->count());
        ChatServiceCocos2dx::notifyUserInfo(memberArr);
    }
#endif
}

//如果在屏蔽列表内，解除屏蔽玩家，否则告诉android端可以屏蔽
void ChatController::unShieldPlayer(string uid,string name)
{
    if(uid!="" && name!="")
    {
        if (GlobalData::shared()->chatShieldInfo.isShield(uid)) {
            string _uuid = GlobalData::shared()->chatShieldInfo.getUuid(uid);
            ChatUnLockCommand* cmd = new ChatUnLockCommand(_uuid);
            cmd->sendAndRelease();
        }
        else {
//            string infoStr=_lang_1("105313",name.c_str());
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//            ChatServiceCocos2dx::setShieldYesNoInfo(infoStr.c_str());
//#endif
        }
    }
}

//屏蔽玩家
void ChatController::shieldPlayer(string uid)
{
    if(uid!="") {
        ChatLockCommand* cmd = new ChatLockCommand(uid);
        cmd->sendAndRelease();
    }
}

//解除玩家禁言
void ChatController::unBanPlayer(string uid)
{
   if(uid!="")
    {
        ChatUnBanCommand* cmd = new ChatUnBanCommand(uid);
        cmd->sendAndRelease();
    }
}

//禁言玩家
void ChatController::banPlayer(string uid,int banTimeIndex)
{
    int banTime=getBanTime(banTimeIndex);
    CCLOGFUNCF("banTime : %d",banTime);
    if(banTime==0)
        banTime=3600;
    if(uid!="") {
        ChatBanCommand* cmd = new ChatBanCommand(uid,banTime);
        cmd->sendAndRelease();
    }
}

int ChatController::getBanTime(int index)
{
    int bannedTime = 0;
    switch (index)
    {
        case 0:
            bannedTime = 3600;
            break;
        case 1:
            bannedTime = 3600*2;
            break;
        case 2:
            bannedTime = 3600*3;
            break;
        case 3:
            bannedTime = 3600*4;
            break;
        default:
            break;
    }
    return bannedTime;
}

//加入联盟
void ChatController::joinAlliance(string uid)
{
    GetUserInfoCommand* cmd = new GetUserInfoCommand(uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatController::joinAllianceCallback), NULL));
    cmd->sendAndRelease();
}

void ChatController::joinAllianceCallback(CCObject* pObj)
{
    NetResult* net = dynamic_cast<NetResult*>(pObj);
    if(net==NULL) return ;
    CCDictionary* dic = dynamic_cast<CCDictionary*>(net->getData());
    if(dic==NULL) return ;
    if(dic->objectForKey("alliance")){
        auto dict = dynamic_cast<CCDictionary*>(dic->objectForKey("alliance"));
        if(dict->objectForKey("alliance")){
            
            auto alliance = dynamic_cast<CCDictionary*>(dict->objectForKey("alliance"));
            if(alliance){
                AllianceInfo* allianceInfo = new AllianceInfo();
                allianceInfo->updateAllianceInfo(alliance);
                PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(allianceInfo));
                allianceInfo->autorelease();
            }
        }
    }
}



void ChatController::setTranslationSucc(CCObject* pObj)
{
    NetResult* result = dynamic_cast<NetResult*>(pObj);
    auto dic = _dict(result->getData());
    if(dic && dic->objectForKey("translationMsg")){
        string msg = dic->valueForKey("translationMsg")->getCString();
//        m_msgLabel->setString(msg.c_str());
//        if(m_type==CHAT_ALLIANCE){
//            auto& info = ChatController::getInstance()->m_chatAlliancePool[m_idx];
//            ChatController::getInstance()->m_chatAlliancePool[m_idx].translationMsg = msg;
//            if(info.originalLang.c_str()){
//                m_translateLabel->setString(_lang_1("105321", info.originalLang.c_str()));
//            }
//        }else if(m_type==CHAT_COMMENT){
//            auto& info = ChatController::getInstance()->m_commentPool[m_idx];
//            ChatController::getInstance()->m_commentPool[m_idx].translationMsg = msg;
//            if(info.originalLang.c_str()){
//                m_translateLabel->setString(_lang_1("105321", info.originalLang.c_str()));
//            }
//        }else
//        {
//            auto& info = ChatController::getInstance()->m_chatCountryPool[m_idx];
//            ChatController::getInstance()->m_chatCountryPool[m_idx].translationMsg = msg;
//            if(info.originalLang.c_str()){
//                m_translateLabel->setString(_lang_1("105321", info.originalLang.c_str()));
//            }
//        }
    }

}

void ChatController::translateMsg(int sessionType,int index)
{
//    auto &info=ChatController::getInstance()->m_chatAlliancePool[index];
//    if(sessionType == CHAT_ALLIANCE)
//        info = ChatController::getInstance()->m_chatAlliancePool[index];
//    else if(sessionType == CHAT_COUNTRY)
//        info = ChatController::getInstance()->m_chatCountryPool[index];
//    
//    if(info.translationMsg != ""){
////        m_msgLabel->setString(info.translationMsg.c_str());
//    }else{
//        //调后台
//        TranslateCommand* cmd = new TranslateCommand(info.msg);
//        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChatController::setTranslationSucc), NULL));
//        cmd->sendAndRelease();
//    }
//    
//    info.showOriginal = false;
//    
    
}

bool ChatController::checkSameLangBySystem(string oLang){
    bool flag = false;
    std::string sLang = LocalController::shared()->getLanguageFileName();
    if(oLang.find("zh")<oLang.length() && sLang.find("zh")<sLang.length()){
        flag = true;
    }else if(oLang==sLang || oLang==""){
        flag = true;
    }
    return flag;
}

void ChatController::joinAllianceBtnClick()
{
    CCLOG("ChatController::joinAllianceBtnClick");
    GlobalData::shared()->playerInfo.isChatView = true;
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
    GlobalData::shared()->playerInfo.isChatView = false;
}

bool ChatController::sendRequestChatFromAndroid(int type)
{
    if (type == CHAT_COUNTRY) {
        m_tmpType = type;
        GetChatCommand* cmd = new GetChatCommand(type, m_chatCountryPool.size(), m_chatCountryPool.size()+(CHAT_REQUEST_NUM-1));
        cmd->sendAndRelease();
    }
    else if (type == CHAT_ALLIANCE) {
        m_tmpType = type;
        CCLOG("sendRequestChat CHAT_ALLIANCE start:%d",(int)m_chatAlliancePool.size());
        GetChatCommand* cmd = new GetChatCommand(type, m_chatAlliancePool.size(), m_chatAlliancePool.size()+(CHAT_REQUEST_NUM-1));
        cmd->sendAndRelease();
    }
    else {
        return false;
    }
    return true;
}

void ChatController::callXCApiForGoogleTranslate()
{
    string lang=LocalController::shared()->getLanguageFileName();
    string googleTransPrefix="action=Google_Translate_TargetLang.";
    if (lang!="") {
        GameController::getInstance()->callXCApi(googleTransPrefix.append(lang));
    }
}
/**发送获取数据请求，0是国家 2是联盟*/
bool ChatController::sendRequestChat(int type)
{
    CCLOG("sendRequestChat type:%d",type);
    if (type == CHAT_COUNTRY && canCountryRequest) {
        m_tmpType = type;
        GetChatCommand* cmd = new GetChatCommand(type, m_chatCountryPool.size(), m_chatCountryPool.size()+(CHAT_REQUEST_NUM-1));
        cmd->sendAndRelease();
    }
    else if (type == CHAT_ALLIANCE && canAllianceRequest) {
        m_tmpType = type;
        CCLOG("sendRequestChat CHAT_ALLIANCE start:%d",(int)m_chatAlliancePool.size());
        GetChatCommand* cmd = new GetChatCommand(type, m_chatAlliancePool.size(), m_chatAlliancePool.size()+(CHAT_REQUEST_NUM-1));
        cmd->sendAndRelease();
    }
    else {
        return false;
    }
    return true;
}

void ChatController::showLatestMessage(int chatType)
{
    CCLOG("showLatestMessage %d",chatType);
    if (chatType==CHAT_COUNTRY || chatType==CHAT_ALLIANCE) {
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        if(ChatServiceCocos2dx::enableNativeChat){
            if(!ChatServiceCocos2dx::isChatShowing)
                UIComponent::getInstance()->refreshChatInfo(chatType);
        }
#else
        if(ChatServiceCocos2dx::enableNativeChat){
            if(!ChatServiceCocos2dx::isChatShowing_fun())
                UIComponent::getInstance()->updateChatInfoIOS(chatType);
        }
#endif
        LuaController::getInstance()->changeChatNotice();
    }
}
/**没有历史消息，调用方法*/
void ChatController::receiveNoMessage()
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::postNoMoreMessage(m_tmpType);
#endif
}
/**下拉加载，历史数据*/
void ChatController::retRequestChat(CCDictionary* dict)
{
    CCLOG("retRequestChat");
    CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("msg"));
    if(arr==NULL)
    {
        CCLOG("arr==NULL");
        receiveNoMessage();
        return;
    }
    std::string allianceId = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    int channelType = -1;
    if (m_tmpType == CHAT_COUNTRY) {
        channelType = CHANNEL_TYPE_COUNTRY;
    }else{
        channelType = CHANNEL_TYPE_ALLIANCE;
    }
#endif
    
    if (arr->count()<CHAT_REQUEST_NUM &&(m_tmpType==CHAT_COUNTRY || m_tmpType==CHAT_ALLIANCE)) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::postChannelNoMoreData(m_tmpType,true);
        if(arr->count()==0)
        {
            CCLOG("arr->count()==0");
            receiveNoMessage();
            return;
        }
#endif
    }
    if(m_chatInfoSendDic==NULL)
        m_chatInfoSendDic=CCDictionary::create();
    
    if (m_tmpType == CHAT_COUNTRY && arr) {
        
        CCLOG("retRequestChat m_tmpType == CHAT_COUNTRY ");
        isFirstRequestCountry=false;
        if (arr->count()<CHAT_REQUEST_NUM) {
            canCountryRequest = false;
        }
        
        CCDictionary *item = NULL;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        CCArray* chatInfoArr=CCArray::create();
#endif
        
        for (int i=0; i<arr->count(); i++) {
            item = _dict(arr->objectAtIndex(i));
            m_chatCountryPool.insert(m_chatCountryPool.begin(), ChatInfo(item));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            ChatMailInfo* chatInfo=ChatMailInfo::create(ChatInfo(item),false);
            
            CCLOG("%s",chatInfo->msg.c_str());
            
            chatInfoArr->addObject(chatInfo);
#endif
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        notifyChatMsgToAndroid(chatInfoArr);
        if(ChatServiceCocos2dx::enableNativeChat){
            if(!ChatServiceCocos2dx::isChatShowing)
                UIComponent::getInstance()->refreshChatInfo(CHAT_COUNTRY);
        }
        else
        {
            UIComponent::getInstance()->updateChatInfo(CHAT_COUNTRY);
        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChatServiceCocos2dx::handleChatPush(chatInfoArr,channelType,"0");
        
        if(ChatServiceCocos2dx::enableNativeChat){
            if(!ChatServiceCocos2dx::isChatShowing_fun())
                UIComponent::getInstance()->updateChatInfoIOS(CHAT_COUNTRY);
        }
        else
        {
            UIComponent::getInstance()->updateChatInfo(CHAT_COUNTRY);
        }
        
#endif
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAT_UPDATE);
    }
    else if (m_tmpType == CHAT_ALLIANCE && arr) {
        CCLOG("retRequestChat m_tmpType == CHAT_ALLIANCE ");
        if (arr->count()<CHAT_REQUEST_NUM) {
            canAllianceRequest = false;
        }
        CCDictionary *item = NULL;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        CCArray* chatInfoArr=CCArray::create();
#endif
        
        for (int i=0; i<arr->count(); i++) {
            item = _dict(arr->objectAtIndex(i));
            m_chatAlliancePool.insert(m_chatAlliancePool.begin(), ChatInfo(item));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            ChatMailInfo* chatInfo=ChatMailInfo::create(ChatInfo(item),false);
            chatInfoArr->addObject(chatInfo);
            if (allianceId == "") {
                allianceId = chatInfo->allianceId;
            }
#endif
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        notifyChatMsgToAndroid(chatInfoArr);
        if(ChatServiceCocos2dx::enableNativeChat){
            if(!ChatServiceCocos2dx::isChatShowing)
                UIComponent::getInstance()->refreshChatInfo(CHAT_ALLIANCE);
        }
        else
        {
            UIComponent::getInstance()->updateChatInfo(CHAT_ALLIANCE);
        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChatServiceCocos2dx::handleChatPush(chatInfoArr,channelType,allianceId);
        
        if(ChatServiceCocos2dx::enableNativeChat){
            if(!ChatServiceCocos2dx::isChatShowing_fun())
                UIComponent::getInstance()->updateChatInfoIOS(CHAT_ALLIANCE);
        }
        else
        {
            UIComponent::getInstance()->updateChatInfo(CHAT_ALLIANCE);
        }
        
#endif
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAT_UPDATE);
        
    }
}

void ChatController::retFindUser(CCDictionary* dict)
{
    string uid = dict->valueForKey("uid")->getCString();
    string errorCode = dict->valueForKey("errorCode")->getCString();
    if (uid != "")
    {
        ChatController::getInstance()->uid = uid;
    }
    else
    {
        if(errorCode != "")
        {
            CCLOG("此用户不存在");
        }
    }
}
void ChatController::sendNotice(std::string msg, int itemid, bool usePoint,std::string sendLocalTime){
    NoticeChatCommand *cmd = new NoticeChatCommand(msg,usePoint,itemid,sendLocalTime);
    cmd->sendAndRelease();
}

void ChatController::addLocalTranslate(ChatInfo *cInfo){
    if(!cInfo)
        return;
   if(!cInfo->translationMsg.empty()){
        return;
    }
    if(mTransChat){
        mTransQueue.push_back(cInfo);
        return;
    }
    mTransChat = cInfo;
    localTranslate();
}
void ChatController::localTranslate(){
    if(!mTransChat)
        return;
    callXCApiForGoogleTranslate();
    CCHttpRequest* request = new CCHttpRequest();
    string url =  CCString::createWithFormat("https://translate.google.com/translate_a/single")->getCString();
    CCLOG("change URL: %s",url.c_str());
    request->setUrl(url.c_str());
    request->setRequestType(CCHttpRequest::Type::POST);
    std::string orgMsg = mTransChat->msg;
    const char* postData = CCString::createWithFormat("client=t&sl=auto&tl=%s&dt=t&ie=UTF-8&oe=UTF-8&source=btn&q=%s", LocalController::shared()->getLanguageFileName().c_str(),orgMsg.c_str())->getCString();
    request->setRequestData(postData ,strlen(postData));
    request->setResponseCallback(this, httpresponse_selector(ChatController::onLocalTranslate));
    request->setTag("chat_translate_request");
    CCHttpClient::getInstance()->send(request);
    request->release();
}
void ChatController::onLocalTranslate(CCHttpClient *client, CCHttpResponse *response){
    if(!mTransChat)
        return;
    
    if (!response){
        return;
    }
    if (0 != strlen(response->getHttpRequest()->getTag())){
        CCLOG("%s completed", response->getHttpRequest()->getTag());
        if (strcmp("chat_translate_request",response->getHttpRequest()->getTag())!=0) {
            // return;
        }
    }
    int m_responseCode = response->getResponseCode();
    CCLOG("response code: %d", m_responseCode);
    if (!response->isSucceed())
    {
        CCLOG("response failed!\nerror buffer: %s", response->getErrorBuffer());
        return;
    }
    std::vector<char>* iter = response->getResponseData();
    std::string serverInfo(iter->begin(),iter->end());
    CCLog("translate result: %s, %d", serverInfo.c_str(), serverInfo.length());
    std::string finalStr = "";
    std::string orgLan = "";
    ChatController::getTranslateInfo(serverInfo,orgLan,finalStr);
    
    mTransChat->originalLang = orgLan;
    mTransChat->translationMsg = finalStr;
    mTransChat->showOriginal=false;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAT_UPDATE);
    
    if(mTransQueue.size()>0){
        mTransChat = mTransQueue.at(0);
        mTransQueue.erase(mTransQueue.begin());
        localTranslate();
    }else{
        mTransChat =NULL;
    }
}
void ChatController::getTranslateInfo(std::string serverInfo,std::string &orgLan,std::string &finalStr){
    vector<string> mVect0;
    auto iter1 = serverInfo.end()-1;
    int index = serverInfo.length()-1;
    int pos = index;
    while (iter1>=serverInfo.begin()) {
        if(iter1 >= serverInfo.begin()+ 3){
            if ((*iter1) == ','){
                if ((*(iter1-1)) == ','){
                    if((*(iter1-2)) == ','){
                        if((*(iter1-3)) == ','){
                            pos = index-3;
                            break;
                        }else{
                            index-=3;
                            iter1-=3;
                        }
                    }else{
                        index-=2;
                        iter1-=2;
                    }
                }else{
                    index-=1;
                    iter1-=1;
                }
            }
        }else{
            break;
        }
        --index;
        --iter1;
    }
    string temp = serverInfo.substr(4,pos-4);
    CCLOG("translate temp1: %s",temp.c_str());
    iter1 = temp.end()-1;
    index = temp.length()-1;
    pos = index;
    while (iter1 >= temp.begin()) {
        if(iter1 >= temp.begin() + 5 ){
            if( ((*iter1) == '\"') && ((*(iter1-1)) == ',') && ((*(iter1-2)) == ',') && ((*(iter1-3)) == ']') && ((*(iter1-4)) == ']') && ((*(iter1-5)) == '\"') ){
                pos = index;
                break;
            }
        }else{
            break;
        }
        --index;
        --iter1;
    }
    string temp1 = temp.substr(0,pos-5);
    string temp2 = temp.substr(pos+1,temp.length()- pos - 2);
    CCLOG("translate temp2: %s",temp1.c_str());
    CCLOG("translate org lang: %s",temp2.c_str());
    orgLan = temp2;

    iter1 = temp1.begin();
    index = 0;
    int sPos = 0;
    int ePos = 0;
    std::vector<std::string> vector;
    while (iter1 < temp1.end()) {
        if(iter1 < temp1.end() - 5 ){
            if( ((*iter1) == '\"') && ((*(iter1+1)) == ']') && ((*(iter1+2)) == ',') && ((*(iter1+3)) == '[') && ((*(iter1+4)) == '\"') ){
                ePos = index;
                string temp3 = temp1.substr(sPos,ePos-sPos);
                
                auto iter3 = temp3.begin();
                int index3=0;
                int pos3=0;
                while (iter3!=temp3.end()) {
                    if(iter3 < temp3.end() - 3){
                        if( ((*iter3) == '\"') && ((*(iter3+1)) == ',') && ((*(iter3+2)) == '\"') ){
                            pos3 = index3;
                            string temp4 = temp3.substr(0,pos3);
                            vector.push_back(temp4);
                            
                            CCLOG("translate section_%d: %s",vector.size(),temp4.c_str());
                            break;
                        }
                    }else{
                        break;
                    }
                    ++index3;
                    ++iter3;
                }
                
                sPos = ePos+5;
                iter1 += 5;
                index += 5;
                continue;
            }
        }else{
            string temp3 = temp1.substr(sPos);
            
            auto iter3 = temp3.begin();
            int index3=0;
            int pos3=0;
            while (iter3!=temp3.end()) {
                if(iter3 < temp3.end() - 3){
                    if( ((*iter3) == '\"') && ((*(iter3+1)) == ',') && ((*(iter3+2)) == '\"') ){
                        pos3 = index3;
                        string temp4 = temp3.substr(0,pos3);
                        vector.push_back(temp4);
                        CCLOG("translate section_%d: %s",vector.size(),temp4.c_str());
                        break;
                    }
                }else{
                    break;
                }
                ++index3;
                ++iter3;
            }
            
            break;
        }
        ++index;
        ++iter1;
    }
    auto iter2 = vector.begin();
    finalStr = (*iter2);
    ++iter2;
    while (iter2!=vector.end()) {
        string temp3 = (*iter2);
        finalStr = finalStr + " " + temp3;
        ++iter2;
    }
    CCLOG("translate final: %s",finalStr.c_str());
}

void ChatController::notifyMsgToAndroid(CCArray* chatMailInfoArray,int channelType,string id,string customName,bool isModMsg)
{
    CCLOGFUNCF("chatMailInfoArray count:%d channelType:%d id:%s customName:%s",chatMailInfoArray->count(),channelType,id.c_str(),customName.c_str());
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(chatMailInfoArray->count()>20)
    {
        CCArray* chatMailArr=CCArray::create();
        int count=0;
        for (int i=0; i<chatMailInfoArray->count(); i++) {
            if(count==20)
            {
                if (channelType==CHANNEL_TYPE_COUNTRY || channelType==CHANNEL_TYPE_ALLIANCE) {
                    notifyChatMsgToAndroid(chatMailArr);
                }
                else if(channelType==CHANNEL_TYPE_CHATROOM || channelType==CHANNEL_TYPE_USER)
                {
                    MailController::getInstance()->m_mailInfoSendDic->setObject(chatMailArr, ChatServiceCocos2dx::m_curSendMailIndex);
                    ChatServiceCocos2dx::notifyMessageIndex(ChatServiceCocos2dx::m_curSendMailIndex,id,customName,isModMsg);
                    ChatServiceCocos2dx::m_curSendMailIndex++;
                }
                chatMailArr->removeAllObjects();
                count=0;
            }
            chatMailArr->addObject(chatMailInfoArray->objectAtIndex(i));
            count++;
        }
        
        if(chatMailArr->count()>0)
        {
            if (channelType==CHANNEL_TYPE_COUNTRY || channelType==CHANNEL_TYPE_ALLIANCE) {
                notifyChatMsgToAndroid(chatMailArr);
            }
            else if(channelType==CHANNEL_TYPE_CHATROOM  || channelType==CHANNEL_TYPE_USER)
            {
                MailController::getInstance()->m_mailInfoSendDic->setObject(chatMailArr, ChatServiceCocos2dx::m_curSendMailIndex);
                ChatServiceCocos2dx::notifyMessageIndex(ChatServiceCocos2dx::m_curSendMailIndex,id,customName,isModMsg);
                ChatServiceCocos2dx::m_curSendMailIndex++;
            }
        }
        
    }
    else
    {
        if (channelType==CHANNEL_TYPE_COUNTRY || channelType==CHANNEL_TYPE_ALLIANCE) {
            notifyChatMsgToAndroid(chatMailInfoArray);
        }
        else if(channelType==CHANNEL_TYPE_CHATROOM  || channelType==CHANNEL_TYPE_USER)
        {
            MailController::getInstance()->m_mailInfoSendDic->setObject(chatMailInfoArray, ChatServiceCocos2dx::m_curSendMailIndex);
            ChatServiceCocos2dx::notifyMessageIndex(ChatServiceCocos2dx::m_curSendMailIndex,id,customName,isModMsg);
            ChatServiceCocos2dx::m_curSendMailIndex++;
        }
    }
    
    if (!ChatServiceCocos2dx::isChatShowing) {
        if (channelType==CHANNEL_TYPE_COUNTRY || channelType==CHANNEL_TYPE_ALLIANCE) {
            if (channelType==CHANNEL_TYPE_COUNTRY) {
                UIComponent::getInstance()->refreshChatInfo(CHAT_COUNTRY);
            }else if (channelType==CHANNEL_TYPE_ALLIANCE) {
                UIComponent::getInstance()->refreshChatInfo(CHAT_ALLIANCE);
            }
            
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAT_UPDATE);
        }
        else if (channelType==CHANNEL_TYPE_CHATROOM) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_ADD, CCInteger::create(chatMailInfoArray->count()));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        }
    }

#endif
}

void ChatController::notifyMsgToIOS(CCArray* chatMailInfoArray,int channelType,string id)
{
    CCLOGFUNCF("chatMailInfoArray count:%d channelType:%d id:%s",chatMailInfoArray->count(),channelType,id.c_str());
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if (channelType==CHANNEL_TYPE_COUNTRY ) {
        notifyChatMsgToIOS(chatMailInfoArray,channelType,"0");
    }else{
        notifyChatMsgToIOS(chatMailInfoArray,channelType,id);
    }
    
    if (!ChatServiceCocos2dx::isChatShowing_fun()) {
        if (channelType==CHANNEL_TYPE_COUNTRY || channelType==CHANNEL_TYPE_ALLIANCE) {
            if (channelType==CHANNEL_TYPE_COUNTRY) {
                //IOS需要判断原生是否开着
//                UIComponent::getInstance()->updateChatInfo(CHAT_COUNTRY);
                
                if(ChatServiceCocos2dx::enableNativeChat){
                    if(!ChatServiceCocos2dx::isChatShowing_fun())
                        UIComponent::getInstance()->updateChatInfoIOS(CHAT_COUNTRY);
                }
                else
                {
                    UIComponent::getInstance()->updateChatInfo(CHAT_COUNTRY);
                }
                
            }else if (channelType==CHANNEL_TYPE_ALLIANCE) {
                //IOS需要判断原生是否开着
//                UIComponent::getInstance()->updateChatInfo(CHAT_ALLIANCE);
                
                if(ChatServiceCocos2dx::enableNativeChat){
                    if(!ChatServiceCocos2dx::isChatShowing_fun())
                        UIComponent::getInstance()->updateChatInfoIOS(CHAT_ALLIANCE);
                }
                else
                {
                    UIComponent::getInstance()->updateChatInfo(CHAT_ALLIANCE);
                }
            }
            
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAT_UPDATE);
        }
        else if (channelType==CHANNEL_TYPE_CHATROOM) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_ADD, CCInteger::create(chatMailInfoArray->count()));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        }
    }
#endif
}

//获取聊天、聊天室最新消息
//如果参数为“”，获取所有聊天室的信息
void ChatController::getNewMsg(string comandStr)
{
    CCLOGFUNCF("comandStr %s",comandStr.c_str());
    if(MailController::getInstance()->getIsNewMailListEnable())
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        string updateTime = ChatServiceCocos2dx::getMailLastUpdateTime();
        GetNewMailMsgCommand* getNewCmd = new GetNewMailMsgCommand(comandStr,updateTime);
        getNewCmd->sendAndRelease();
#endif
    }
    else
    {
        GetNewMsgCommand* getNewCmd = new GetNewMsgCommand(comandStr);
        getNewCmd->sendAndRelease();
    }
}

void ChatController::getNewMailMsg(string comandStr)
{
    GetNewMailMsgCommand* getNewCmd = new GetNewMailMsgCommand(comandStr);
    getNewCmd->sendAndRelease();
}

void ChatController::setDisableTranslateLang()
{
    const char* langType[21] = {"en","zh-Hans","zh-Hant","es","fr","pt","de","ru","ko","it","da","el","fi","ja","nl","pl","sv","th","tr","vi","ms"};
    
    string disableLang="";
    for(int i=0;i<21;i++)
    {
        string type = langType[i];
        string typeCacheKey = "LanguageTranslation.set."+type;
        string typeCache = CCUserDefault::sharedUserDefault()->getStringForKey(typeCacheKey.c_str());
        if(typeCache!=""){
            if(type=="zh-Hans")
                type="zh-CN";
            else if(type=="zh-Hant")
                type="zh-TW";
            if(disableLang=="")
                disableLang=type;
            else
                disableLang.append("|").append(type);
        }
    }
    
    CCLOGFUNCF("disableLang %s:",disableLang.c_str());
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::setDisableLang(disableLang);
#endif
}