//
//  AllianceAcceptInviteCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-16.
//
//

#include "AllianceAcceptInviteCommand.h"
#include "NetController.h"
#include "AllianceManager.h"
#include "GameController.h"
#include "ChatServiceCocos2dx.h"
#include "ChatController.h"

AllianceAcceptInviteCommand::AllianceAcceptInviteCommand(std::string Id)
: CommandBase(ACCEPT_INVITE),m_Id(Id)
{
    putParam("mailId", CCString::create(m_Id.c_str()));
}

bool AllianceAcceptInviteCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ACCEPT_INVITE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    GameController::getInstance()->removeWaitInterface();
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        CCDictionary* dict =_dict(params->objectForKey("allianceInfo"));
        CCDictionary* dictAlliance =_dict(dict->objectForKey("alliance"));;
        GlobalData::shared()->playerInfo.allianceInfo.updateAllianceInfo(dictAlliance);
        GlobalData::shared()->playerInfo.setAllianceId(GlobalData::shared()->playerInfo.allianceInfo.uid);
//        AllianceManager::getInstance()->invitesAllianceList.erase(m_Id);
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INVITES_DATA,NULL);
        callSuccess(NetResult::create());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::setPlayerAllianceInfo(GlobalData::shared()->playerInfo.allianceInfo.shortName.c_str(),GlobalData::shared()->playerInfo.getAllianceId().c_str(),GlobalData::shared()->playerInfo.allianceInfo.rank,GlobalData::shared()->playerInfo.isfirstJoin);
        string param = ChatServiceCocos2dx::getChannelInfo();
        ChatController::getInstance()->getNewMsg(param);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChatServiceCocos2dx::setPlayerAllianceInfo(GlobalData::shared()->playerInfo.allianceInfo.shortName.c_str(),GlobalData::shared()->playerInfo.getAllianceId().c_str(),GlobalData::shared()->playerInfo.allianceInfo.rank ,GlobalData::shared()->playerInfo.isfirstJoin);
#endif
    }
    return true;
}