//
//  FriendsController.cpp
//  IF
//
//  Created by xxrdsg on 15-9-29.
//
//

#include "FriendsController.h"
#include "FriendsCommand.h"
#include "ChatController.h"
#include "ChatServiceCocos2dx.h"
#include "WorldController.h"

#pragma mark FriendInfo
FriendInfo::FriendInfo(CCDictionary* dict)
{
    updateInfo(dict);
}

FriendInfo* FriendInfo::create()
{
    auto ret = new FriendInfo();
    ret->autorelease();
    return ret;
}

FriendInfo* FriendInfo::create(CCDictionary* dict)
{
    auto ret = new FriendInfo();
    ret->updateInfo(dict);
    ret->autorelease();
    return ret;
}

void FriendInfo::updateInfo(cocos2d::CCDictionary *dict)
{
    isNew=false;
    
    if (dict->objectForKey("pic")) {
        pic = dict->valueForKey("pic")->getCString();
    }
    if (dict->objectForKey("picVer")) {
        picVer = dict->valueForKey("picVer")->intValue();
    }
    if (dict->objectForKey("serverId")) {
        serverId = dict->valueForKey("serverId")->intValue();
    }
    if (dict->objectForKey("relation")) {
        relation = dict->valueForKey("relation")->intValue();
    }
    if (dict->objectForKey("vipEndTime")) {
        vipEndTime = dict->valueForKey("vipEndTime")->doubleValue();
    }
    if (dict->objectForKey("allianceId")) {
        allianceId = dict->valueForKey("allianceId")->getCString();
    }
    if (dict->objectForKey("vipLevel")) {
        vipLevel = dict->valueForKey("vipLevel")->intValue();
    }
    if (dict->objectForKey("uid")) {
        uid = dict->valueForKey("uid")->getCString();
    }
    if (dict->objectForKey("crossFightSrcServerId")) {
        crossFightSrcServerId = dict->valueForKey("crossFightSrcServerId")->intValue();
    }
    if (dict->objectForKey("offLineTime")) {
        offLineTime = dict->valueForKey("offLineTime")->doubleValue();
    }
    if (dict->objectForKey("gmFlag")) {
        gmFlag = dict->valueForKey("gmFlag")->intValue();
    }
    if (dict->objectForKey("name")) {
        name = dict->valueForKey("name")->getCString();
    }
    if (dict->objectForKey("online")) {
        online = dict->valueForKey("online")->intValue();
    }
    if (dict->objectForKey("rank")) {
        rank = dict->valueForKey("rank")->intValue();
    }
    if (dict->objectForKey("power")) {
        power = dict->valueForKey("power")->doubleValue();
    }
    if (dict->objectForKey("abbr")) {
        abbr = dict->valueForKey("abbr")->getCString();
    }
    if (dict->objectForKey("lang")) {
        lang = dict->valueForKey("lang")->getCString();
    }
    if (dict->objectForKey("mainBuildingLevel")) {
        mainBuildingLevel = dict->valueForKey("mainBuildingLevel")->intValue();
    }
    
    if (dict->objectForKey("stat")) {
        relation = dict->valueForKey("stat")->intValue();
    }
    if (dict->objectForKey("description")) {
        description = dict->valueForKey("description")->getCString();
    }
    if (dict->objectForKey("friendUid")) {
        uid = dict->valueForKey("friendUid")->getCString();
    }
    if (dict->objectForKey("fbUid")) {
        fbUid = dict->valueForKey("fbUid")->getCString();
    }else{
        fbUid="";
    }
    if (dict->objectForKey("fbName")) {
        fbName = dict->valueForKey("fbName")->getCString();
    }else{
        fbName="";
    }
}

#pragma mark FriendSearchInfo
FriendSearchInfo::FriendSearchInfo(CCDictionary* dict,int type,bool open /*=false*/,int num /*=0*/,int onNum /*=0*/)
{
    updateInfo(dict,type,open,num,onNum);
}

FriendSearchInfo* FriendSearchInfo::create(CCDictionary* dict,int type,bool open /*=false*/,int num /*=0*/,int onNum /*=0*/)
{
    auto ret = new FriendSearchInfo();
    ret->updateInfo(dict,type,open,num,onNum);
    ret->autorelease();
    return ret;
}

void FriendSearchInfo::updateInfo(cocos2d::CCDictionary *dict,int type,bool open /*=false*/,int num /*=0*/,int onNum /*=0*/)
{
    cellType = type;
    cellOpen=open;
    cellNum=num;
    cellOnNum=onNum;
    if (dict==NULL){
        return;
    }
    if (dict->objectForKey("pic")) {
        pic = dict->valueForKey("pic")->getCString();
    }
    if (dict->objectForKey("picVer")) {
        picVer = dict->valueForKey("picVer")->intValue();
    }
    if (dict->objectForKey("serverId")) {
        serverId = dict->valueForKey("serverId")->intValue();
    } else {
        serverId = -1;
    }
    if (dict->objectForKey("abbr")) {
        abbr = dict->valueForKey("abbr")->getCString();
    } else {
        abbr = "";
    }
    if (dict->objectForKey("relation")) {
        relation = dict->valueForKey("relation")->intValue();
    }
    if (dict->objectForKey("vipEndTime")) {
        vipEndTime = dict->valueForKey("vipEndTime")->doubleValue();
    }
    if (dict->objectForKey("allianceId")) {
        allianceId = dict->valueForKey("allianceId")->getCString();
    }
    if (dict->objectForKey("vipLevel")) {
        vipLevel = dict->valueForKey("vipLevel")->intValue();
    }
    if (dict->objectForKey("uid")) {
        uid = dict->valueForKey("uid")->getCString();
    }
    if (dict->objectForKey("crossFightSrcServerId")) {
        crossFightSrcServerId = dict->valueForKey("crossFightSrcServerId")->intValue();
    }
    if (dict->objectForKey("offLineTime")) {
        offLineTime = dict->valueForKey("offLineTime")->doubleValue();
    }
    if (dict->objectForKey("gmFlag")) {
        gmFlag = dict->valueForKey("gmFlag")->intValue();
    }
    if (dict->objectForKey("name")) {
        name = dict->valueForKey("name")->getCString();
    }
    if (dict->objectForKey("online")) {
        online = dict->valueForKey("online")->intValue();
    }
    if (dict->objectForKey("rank")) {
        rank = dict->valueForKey("rank")->intValue();
    }
    if (dict->objectForKey("power")) {
        power = dict->valueForKey("power")->doubleValue();
    }
    if (dict->objectForKey("lang")) {
        lang = dict->valueForKey("lang")->getCString();
    }
    if (dict->objectForKey("mainBuildingLevel")) {
        mainBuildingLevel = dict->valueForKey("mainBuildingLevel")->intValue();
    }
    if (dict->objectForKey("mainCityLv")) {
        mainBuildingLevel = dict->valueForKey("mainCityLv")->intValue();
    }
    
    if (dict->objectForKey("stat")) {
        relation = dict->valueForKey("stat")->intValue();
    }
    if (dict->objectForKey("description")) {
        description = dict->valueForKey("description")->getCString();
    }
    if (dict->objectForKey("friendUid")) {
        uid = dict->valueForKey("friendUid")->getCString();
    }
    //alliance
    if (dict->objectForKey("jointime")) {
        jointime = dict->valueForKey("jointime")->doubleValue();
    }
    if (dict->objectForKey("armyKill")) {
        armyKill = dict->valueForKey("armyKill")->intValue();
    }
    if (dict->objectForKey("pointId")) {
        pointId = dict->valueForKey("pointId")->intValue();
    }
    if (dict->objectForKey("fight")) {
        fight = dict->valueForKey("fight")->intValue();
    }
    //search
    if (dict->objectForKey("alliancename")) {
        alliancename = dict->valueForKey("alliancename")->getCString();
    }
    if (dict->objectForKey("level")) {
        level = dict->valueForKey("level")->intValue();
    }
}

#pragma mark FriendsController
static FriendsController* _instance = nullptr;

FriendsController::FriendsController()
{
    m_data = CCDictionary::create();
    m_fbData = CCDictionary::create();
    m_appliedData = CCDictionary::create();
    m_isHasFriendsViewPop = false;
    m_isDataInit = false;
    m_bOpen = false;
}

FriendsController* FriendsController::getInstance()
{
    if (!_instance) {
        _instance = new FriendsController();
    }
    return _instance;
}

void FriendsController::purgeData()
{
    CC_SAFE_RELEASE_NULL( _instance );
     _instance = nullptr;
}

void FriendsController::handleResponse(cocos2d::CCDictionary *dic)
{
    string cmd = dic->valueForKey("cmd")->getCString();
    
    if (cmd == "push.contact.accepted") {
        auto params = _dict(dic->objectForKey("params"));
        auto info = FriendInfo::create(params);
        if (!(info->uid).empty()) {
            m_appliedData->removeObjectForKey(info->uid);
            m_data->setObject(info, info->uid);
            delInfoFromApplySeenLocal(info->uid);
//            ChatController::getInstance()->getFriendMember(); simon
            info->isNew=true;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(RECEIVE_MY_APPLY_ACCEPTED_PUSH);
            CCCommonUtils::flyHint("", "", _lang_1("105787",info->name.c_str()), 2, 5, false,0.75);//_lang("")//105787={0}通过了您的好友申请
            delInfoFromApplyByMeLocal(info->uid);
        }
    }
    
    if (cmd == "push.contact.apply") {
        auto params = _dict(dic->objectForKey("params"));
        auto info = FriendInfo::create(params);
        if (!(info->uid).empty()) {
            m_data->setObject(info, info->uid);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(RECEIVE_APPLY_PUSH, CCString::create(info->uid));
            if (m_isHasFriendsViewPop) {
                addInfoToApplySeenLocal(info->uid);
            } else {
                if (!bHasInfoInApplySeenLocal(info->uid)) {
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(HAS_NEW_FRIEND_APPLY, CCString::create(info->uid));
                }
            }
//            CCCommonUtils::flyHint("", "", "apply apply apply apply ", 2, 5, true);//_lang("")
            CCCommonUtils::flyHint("", "", _lang("105785"), 2, 5, false,0.75);//_lang("")//您有新的好友申请
        }
    }
    
    if (cmd == "push.contact.delete") {
        auto params = _dict(dic->objectForKey("params"));
        string fuid("");
        if (params->objectForKey("friendUid")) {
            fuid = params->valueForKey("friendUid")->getCString();
            if (!fuid.empty())
            {
                m_data->removeObjectForKey(fuid);
                m_appliedData->removeObjectForKey(fuid);
                delInfoFromApplySeenLocal(fuid);
//                ChatController::getInstance()->getFriendMember(); simon
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(RECEIVE_ME_DELETED_PUSH, CCString::create(fuid));
            }
        }
    }
}

void FriendsController::startGetFriendsData()
{
    auto cmd = new ContactGetCmd();
    cmd->sendAndRelease();
}

void FriendsController::endGetFriendsData(CCDictionary* dict)
{
    if (dict->objectForKey("errorCode")) {
        string err = dict->valueForKey("errorCode")->getCString();
//        CCCommonUtils::flyText(_lang(err));
        return;
    }
    if (dict->objectForKey("contactList")) {
        m_data->removeAllObjects();
        m_appliedData->removeAllObjects();
        CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("contactList"));
        CCObject* obj = nullptr;
        CCDictionary* ele = nullptr;
        bool st = false;
        CCARRAY_FOREACH(arr, obj)
        {
            ele = _dict(obj);
            FriendInfo* info = FriendInfo::create(ele);
            if (!(info->uid).empty()) {
                if (info->relation == FRIEND_APPLYING) {
                    m_appliedData->setObject(info, info->uid);
                } else {
                    m_data->setObject(info, info->uid);
                }
                if (!st && isApplyingMe(info->uid)) {
                    if (!bHasInfoInApplySeenLocal(info->uid)) {
                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(HAS_NEW_FRIEND_APPLY, CCString::create(info->uid));
                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(NEW_FRIENDS_TIPS_SHOW);
//                        CCCommonUtils::flyHint("", "", _lang("105785"), 2, 5, false ,0.75);//_lang("")//您有新的好友申请
                        st = true;
                    }
                }
                if (!st && isMyFriend(info->uid)) {
                    if (bHasInfoInApplyByMeLocal(info->uid)) {
                        info->isNew=true;
                        CCCommonUtils::flyHint("", "", _lang_1("105787",info->name.c_str()), 2, 5, false ,0.75);//105787={0}通过了您的好友申请
                        st = true;
                        delInfoFromApplyByMeLocal(info->uid);
                    }
                }
            }
        }
        DelFBPutongFriendToFB();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GET_FRIEND_DATA_SUC);
        m_isDataInit = true;
    }
}

void FriendsController::DelFBPutongFriendToFB(){
    CCDictElement* pElement;
    FriendInfo* info;
    CCDICT_FOREACH(m_data, pElement){
        const char* key = pElement->getStrKey();
        info = dynamic_cast<FriendInfo*>(m_data->objectForKey(key));
        if(info){
            if (info->fbUid != ""  && info->relation == FRIEND_FRIEND) {
                if (info->uid != "") {
                    m_data->removeObjectForKey(info->uid);
                }
            }
        }
    }
    return ;
}

void FriendsController::startApplyFriend(string uid)
{
    if (uid.empty())
    {
        return;
    }
    if (m_appliedData->objectForKey(uid))
    {
        CCCommonUtils::flyHint("", "", _lang("105389"));
        return;
    }
    auto cmd = new ContactApplyCmd(uid);
    cmd->sendAndRelease();
}

void FriendsController::endApplyFriend(CCDictionary* dict)
{
    if (dict->objectForKey("errorCode")) {
        string err = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(err));
    } else {
        auto info = FriendInfo::create(dict);
        if (!(info->uid).empty()) {
            if (info->relation == FRIEND_APPLYING) {
                m_appliedData->setObject(info, info->uid);
            } else if (info->relation == FRIEND_FRIEND || info->relation == FRIEND_FAVO) {
                FriendInfo* preInfo = dynamic_cast<FriendInfo*>(m_data->objectForKey(info->uid));
                if (preInfo)
                {
                    preInfo->updateInfo(dict);
                    preInfo->isNew = true;
                    m_appliedData->removeObjectForKey(info->uid);
                    CCCommonUtils::flyHint("", "", _lang("132114"));//心有灵犀。对方也给你发送了好友请求，你们现在将直接成为好友。
                } else {
                    m_appliedData->removeObjectForKey(info->uid);
                }
            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(APPLY_FRIEND_SEND_SUC, CCString::create(info->uid));
            addInfoToApplyByMeLocal(info->uid);
        }
    }
}

void FriendsController::startDeleteFriend(string uid)
{
    auto cmd = new ContactDeleteCmd(uid, 0);
    cmd->sendAndRelease();
}

void FriendsController::endDeleteFriend(cocos2d::CCDictionary *dic)
{
    if (dic->objectForKey("errorCode")) {
        string err = dic->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(err));
    } else {
        if (dic->objectForKey("m_friendUid")) {
            string uid = dic->valueForKey("m_friendUid")->getCString();
            if ((!uid.empty())) {
                if (m_appliedData->objectForKey(uid)) {
                    m_appliedData->removeObjectForKey(uid);
                }
                if (m_data->objectForKey(uid)) {
                    m_data->removeObjectForKey(uid);
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DELETE_FRIEND_SUC, CCString::create(uid));
                }
            }
            delInfoFromApplySeenLocal(uid);
        }
    }
}

//void FriendsController::startGetApplyingIds()
//{
//    auto cmd = new ContactAuditCmd();
//    cmd->sendAndRelease();
//}
//
//void FriendsController::endGetApplyingIds(cocos2d::CCDictionary *dict)
//{
//    if (dict->objectForKey("errorCode")) {
//        
//    } else {
//        if (dict->objectForKey("uid")) {
//            CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("uid"));
//            CCObject* obj = nullptr;
//            CCARRAY_FOREACH(arr, obj)
//            {
//                CCString * cs = dynamic_cast<CCString*>(obj);
//                if (!bHasInfoInApplyLocal(cs->getCString())) {
//                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(HAS_NEW_FRIEND_APPLY, CCString::create(cs->getCString()));
//                    break;
//                }
//            }
//        }
//    }
//}

void FriendsController::startAcceptApply(string uid)
{
    auto cmd = new ContactAcceptCmd(uid);
    cmd->sendAndRelease();
}

void FriendsController::endAcceptApply(cocos2d::CCDictionary *dict)
{
    if (dict->objectForKey("errorCode")) {
        string err = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(err));
    } else {
        if (dict->objectForKey("m_friendUid")) {
            string uid = dict->valueForKey("m_friendUid")->getCString();
            if (uid.empty()) {
                return;
            }
            if (m_data->objectForKey(uid)) {
                auto info = dynamic_cast<FriendInfo*>(m_data->objectForKey(uid));
                if (info)
                {
                    info->relation = FRIEND_FRIEND;
                    info->isNew = true;
                    delInfoFromApplySeenLocal(info->uid);
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACCEPT_FRIEND_APPLY_SUC, CCString::create(uid));
                    CCCommonUtils::flyHint("", "", _lang_1("105788",info->name.c_str()));//接受申请
                    
                    string content = _lang("105790");//我们已经是好友了，现在开始聊天吧
                    string toName = info->name;
                    double tmpCurTime = WorldController::getInstance()->getTime();
                    tmpCurTime/=1000;
                    MailController::getInstance()->sendMailToServer(toName, "", content,CC_ITOA(tmpCurTime),"",uid,false,MAIL_SELF_SEND,uid,"",true);
//                    MailController::getInstance()->sendMailToServer(toName, "", content,CC_ITOA(tmpCurTime),"",toName,false,MAIL_SELF_SEND,toName,"",true);
                }
            }
            if (m_appliedData->objectForKey(uid)) {
                m_appliedData->removeObjectForKey(uid);
            }
//            ChatController::getInstance()->getFriendMember(); simon
        }
    }
}

void FriendsController::startFBUpToStarFriend(string uid,string fbUid,string fbName)
{
    if (!m_fbData->objectForKey(uid)) {
        return;
    }
    auto cmd = new ContactFBFavoCmd(uid,fbUid,fbName);
    cmd->sendAndRelease();
}

void FriendsController:: endFBUpToStarFriend(CCDictionary* dic){
    if (dic->objectForKey("errorCode"))
    {
        string err = dic->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(err));
    } else {
        if (dic->objectForKey("m_friendUid")) {
            string uid = dic->valueForKey("m_friendUid")->getCString();
            if (uid.empty()) {
                return;
            }
            if (m_fbData->objectForKey(uid)) {
                auto info = dynamic_cast<FriendInfo*>(m_fbData->objectForKey(uid));
                if (info)
                {
                    info->updateInfo(dic);
                    m_data->setObject(info,info->uid);
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UP_FRIEND_STAR_SUC, CCString::create(uid));
                }
            }
            if (m_fbData->objectForKey(uid)) {
                m_fbData->removeObjectForKey(uid);
            }
        }
    }
}

void FriendsController::startUpToStarFriend(string uid)
{
    if (!m_data->objectForKey(uid)) {
        return;
    }
    auto cmd = new ContactFavoCmd(uid);
    cmd->sendAndRelease();
}
void FriendsController::endUpToStarFriend(CCDictionary* dic)
{
    if (dic->objectForKey("errorCode"))
    {
        string err = dic->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(err));
    } else {
        FriendInfo* info = nullptr;
        string uid("");
        if (dic->objectForKey("m_friendUid")) {
            uid = dic->valueForKey("m_friendUid")->getCString();
            if (!uid.empty() && m_data->objectForKey(uid)) {
                info = dynamic_cast<FriendInfo*>(m_data->objectForKey(uid));
                if (info)
                {
                    info->updateInfo(dic);
                }
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UP_FRIEND_STAR_SUC, CCString::create(uid));
    }
}

void FriendsController::startDownToNormalFriend(string uid)
{
    if (!m_data->objectForKey(uid)) {
        return;
    }
    auto cmd = new ContactFavoCancelCmd(uid);
    cmd->sendAndRelease();
}
void FriendsController::endDownToNormalFriend(CCDictionary* dic)
{
    if (dic->objectForKey("errorCode"))
    {
        string err = dic->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(err));
    } else {
        FriendInfo* info = nullptr;
        string uid("");
        if (dic->objectForKey("m_friendUid")) {
            uid = dic->valueForKey("m_friendUid")->getCString();
            if (!uid.empty() && m_data->objectForKey(uid)) {
                info = dynamic_cast<FriendInfo*>(m_data->objectForKey(uid));
                if (info)
                {
                    info->updateInfo(dic);
                    if(info->fbUid != ""){
                        m_fbData->setObject(info, info->uid);
                        info->relation = FRIEND_FACEBOOK;
                        if (m_data->objectForKey(uid)) {
                            m_data->removeObjectForKey(uid);
                        }
                    }
                }
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DOWN_FRIEND_NORMAL_SUC, CCString::create(uid));
    }
}

void FriendsController::startChangeDescription(string uid, string des)
{
    if (!m_data->objectForKey(uid)) {
        return;
    }
    auto cmd = new ContactDescCmd(uid, des);
    cmd->sendAndRelease();
}

void FriendsController::endChangeDescription(CCDictionary* dic)
{
    if (dic->objectForKey("errorCode"))
    {
        string err = dic->valueForKey("errorCode")->getCString();
//        CCCommonUtils::flyHint("", "", _lang(err));
    } else {
        FriendInfo* info = nullptr;
        string uid("");
        if (dic->objectForKey("m_friendUid")) {
            uid = dic->valueForKey("m_friendUid")->getCString();
            if (!uid.empty() && m_data->objectForKey(uid)) {
                info = dynamic_cast<FriendInfo*>(m_data->objectForKey(uid));
                if (info) {
                    info->updateInfo(dic);
                }
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(FRIEND_DESC_CHANGE_SUC, CCString::create(uid));
    }
}

void FriendsController::startRejectApply(string uid)
{
    if (!m_data->objectForKey(uid)) {
        return;
    }
    auto cmd = new ContactDeleteCmd(uid, 1);
    cmd->sendAndRelease();
}

void FriendsController::endRejectApply(CCDictionary* dic)
{
    if (dic->objectForKey("errorCode"))
    {
        string err = dic->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(err));
    } else {
        string uid("");
        if (dic->objectForKey("m_friendUid"))
        {
            uid = dic->valueForKey("m_friendUid")->getCString();
        }
        if (!uid.empty() && m_data->objectForKey(uid)) {
            m_data->removeObjectForKey(uid);
        }
        delInfoFromApplySeenLocal(uid);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REJECT_APPLY_SUC, CCString::create(uid));
    }
}

//按uid搜索Facebook好友
void FriendsController::startGetFbFriends(string uids){
//    if(m_fbData->count()==0){
        auto cmd = new ContactGetFbCmd(uids);
        cmd->sendAndRelease();
//    }
}
void FriendsController::endGetFbFriends(CCDictionary* dict){
    if (dict->objectForKey("errorCode")){
        string err = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(err));
    } else {
        m_fbData->removeAllObjects();
        CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("list"));
        CCObject* obj = nullptr;
        CCDictionary* ele = nullptr;
        bool st = false;
        CCARRAY_FOREACH(arr, obj)
        {
            ele = _dict(obj);
            FriendInfo* info = FriendInfo::create(ele);
            info->relation = FRIEND_FACEBOOK;
            if (!(info->uid).empty()) {
                m_fbData->setObject(info, info->uid);
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UP_FRIEND_STAR_SUC);
    }
}

void FriendsController::delInfoFromApplySeenLocal(string uid)
{
    if (uid.empty()) return;
    string uids = CCUserDefault::sharedUserDefault()->getStringForKey(APPLYING_UID_SEEN, "");
    if (!uids.empty()) {
        uid.append(";");
        int pos = uids.find(uid);
        if (string::npos != pos) {
            uids.replace(pos, uid.size(), "");
            CCUserDefault::sharedUserDefault()->setStringForKey(APPLYING_UID_SEEN, uids);
            CCUserDefault::sharedUserDefault()->flush();
        }
    }
}

void FriendsController::addInfoToApplySeenLocal(string uid)
{
    if (uid.empty()) {
        return;
    }
    string uids = CCUserDefault::sharedUserDefault()->getStringForKey(APPLYING_UID_SEEN, "");
    uid.append(";");
    int pos = uids.find(uid);
    if (string::npos != pos) {
        return;
    }
    uids.append(uid);
    CCUserDefault::sharedUserDefault()->setStringForKey(APPLYING_UID_SEEN, uids);
    CCUserDefault::sharedUserDefault()->flush();
}

bool FriendsController::bHasInfoInApplySeenLocal(string uid)
{
    if (uid.empty()) {
        return true;
    }
    string uids = CCUserDefault::sharedUserDefault()->getStringForKey(APPLYING_UID_SEEN, "");
    uid.append(";");
    int pos = uids.find(uid);
    if (string::npos != pos) {
        return true;
    } else {
        return false;
    }
}

void FriendsController::delInfoFromApplyByMeLocal(string uid)
{
    if (uid.empty()) return;
    string uids = CCUserDefault::sharedUserDefault()->getStringForKey(FRIEND_I_APPLYING_GROUP, "");
    if (!uids.empty()) {
        uid.append(";");
        int pos = uids.find(uid);
        if (string::npos != pos) {
            uids.replace(pos, uid.size(), "");
            CCUserDefault::sharedUserDefault()->setStringForKey(FRIEND_I_APPLYING_GROUP, uids);
            CCUserDefault::sharedUserDefault()->flush();
        }
    }
}

void FriendsController::addInfoToApplyByMeLocal(string uid)
{
    if (uid.empty()) {
        return;
    }
    string uids = CCUserDefault::sharedUserDefault()->getStringForKey(FRIEND_I_APPLYING_GROUP, "");
    uid.append(";");
    int pos = uids.find(uid);
    if (string::npos != pos) {
        return;
    }
    uids.append(uid);
    CCUserDefault::sharedUserDefault()->setStringForKey(FRIEND_I_APPLYING_GROUP, uids);
    CCUserDefault::sharedUserDefault()->flush();
}

bool FriendsController::bHasInfoInApplyByMeLocal(string uid)
{
    if (uid.empty()) {
        return true;
    }
    string uids = CCUserDefault::sharedUserDefault()->getStringForKey(FRIEND_I_APPLYING_GROUP, "");
    uid.append(";");
    int pos = uids.find(uid);
    if (string::npos != pos) {
        return true;
    } else {
        return false;
    }
}

bool FriendsController::isMyFriend(string uid)
{
    return (isMyPutongFriend(uid) || isMyStarFriend(uid) || isMyPutongFBFriend(uid));
}

bool FriendsController::isMyPutongFriend(string uid)
{
    return 1 == getContactState(uid);
}

bool FriendsController::isMyPutongFBFriend(string uid)
{
    return 5 == getContactState(uid);
}

bool FriendsController::isMyStarFriend(string uid)
{
    return 2 == getContactState(uid);
}

bool FriendsController::isApplyingMe(string uid)
{
    return 4 == getContactState(uid);
}

bool FriendsController::isBeingAppliedByMe(string uid)
{
    return 3 == getContactState(uid);
}

int FriendsController::getContactState(string uid)
{
    if (uid.empty()) {
        return 0;
    }
    if (!m_data->objectForKey(uid)&& !m_fbData->objectForKey(uid) && !m_appliedData->objectForKey(uid)) {
        return 0;
    }
    if (m_data->objectForKey(uid)) {
        auto info = dynamic_cast<FriendInfo*>(m_data->objectForKey(uid));
        if (!info) {
            return 0;
        }
        return info->relation;
    }else if(m_fbData->objectForKey(uid)){
        auto info = dynamic_cast<FriendInfo*>(m_fbData->objectForKey(uid));
        if (!info) {
            return 0;
        }
        return info->relation;
    }
    else {
        auto info = dynamic_cast<FriendInfo*>(m_appliedData->objectForKey(uid));
        if (!info) {
            return 0;
        }
        return info->relation;
    }
}

bool FriendsController::isStranger(string uid)
{
    return 0 == getContactState(uid);
}

void FriendsController::checkHasNewApplyForMe()
{
    CCDictElement* ele = nullptr;
    CCDictionary* dic = m_data;
    FriendInfo* info = nullptr;
    CCDICT_FOREACH(dic, ele)
    {
        info = dynamic_cast<FriendInfo*>(ele->getObject());
        if (info) {
            if (isApplyingMe(info->uid))
            {
                if (!bHasInfoInApplySeenLocal(info->uid)) {
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(HAS_NEW_FRIEND_APPLY, CCString::create(info->uid));
                }
            }
        }
    }
}

void FriendsController::resetLocalSeenApply()
{
    CCUserDefault::sharedUserDefault()->setStringForKey(APPLYING_UID_SEEN, "");
    CCUserDefault::sharedUserDefault()->flush();
    CCDictElement* ele = nullptr;
    CCDictionary* dic = m_data;
    CCDICT_FOREACH(dic, ele)
    {
        if (isApplyingMe(ele->getStrKey())) {
            addInfoToApplySeenLocal(ele->getStrKey());
        }
    }
}

void FriendsController::getNumByGroup(map< int, pair<int, int> >& mp)
{
    mp.clear();
    CCDictionary* dic = m_data;
    CCDictElement* ele = nullptr;
    FriendInfo* info = nullptr;
    CCDICT_FOREACH(dic, ele)
    {
        info = dynamic_cast<FriendInfo*>(ele->getObject());
        if (info)
        {
            mp[info->relation].second++;
            if (info->online > 0) {
                mp[info->relation].first++;
            }
        }
    }
}

bool FriendsController::isFaceBookOpen(){
    if (fbSwitch =="1") {
        return true;
    }else if (fbSwitch=="0"){
        return false;
    }else if(CCCommonUtils::checkVersion(fbSwitch)){
        return true;
    }
    return false;
}

bool FriendsController::isfbUidAddToFamily(string fbuid){
    CCDictElement* pElement;
    FriendInfo* info;
    CCDICT_FOREACH(m_data, pElement){
        const char* key = pElement->getStrKey();
        if(m_data->objectForKey(key)){
            info = dynamic_cast<FriendInfo*>(m_data->objectForKey(key));
            if(info){
                if (fbuid == info->fbUid && info->relation != FRIEND_FRIEND) {
                    return true;
                }
            }
        }
    }
    return false;
}
