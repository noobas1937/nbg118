//
//  AllianceJoinPush.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-14.
//
//

#include "AllianceJoinPush.h"
#include "AllianceInfoView.h"
#include "PopupViewController.h"
#include "YesNoDialog.h"
#include "UIComponent.h"
#include "ActivityController.h"
#include "ChatServiceCocos2dx.h"
#include "FunBuildController.h"
#include "ChatController.h"

void AllianceJoinPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    auto dic = _dict(params->objectForKey("alliance"));
    GlobalData::shared()->playerInfo.allianceInfo.updateAllianceInfo(dic);
    GlobalData::shared()->playerInfo.setAllianceId(GlobalData::shared()->playerInfo.allianceInfo.uid);
    YesNoDialog::showAllianceConfirm(_lang_1("115024",GlobalData::shared()->playerInfo.allianceInfo.name.c_str()),_lang("confirm").c_str());
    if(GlobalData::shared()->playerInfo.isfirstJoin == 1){
       // GlobalData::shared()->playerInfo.isfirstJoin = 0;
        UIComponent::getInstance()->showFlygold();
    }
    //GlobalData::shared()->playerInfo.isfirstJoin = 0;
    if (GlobalData::shared()->playerInfo.isInAlliance()) {
        CCArray* arr = CCArray::create();
        arr->addObject(CCString::create(GlobalData::shared()->playerInfo.allianceInfo.name));
        ActivityController::getInstance()->facebookPostMethod("join_alliance",arr);//post one feed
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MARK_LINE_AL);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOGFUNC("");
    ChatServiceCocos2dx::setPlayerAllianceInfo(GlobalData::shared()->playerInfo.allianceInfo.shortName.c_str(),GlobalData::shared()->playerInfo.getAllianceId().c_str(),GlobalData::shared()->playerInfo.allianceInfo.rank ,GlobalData::shared()->playerInfo.isfirstJoin);
    string param = ChatServiceCocos2dx::getChannelInfo();
    ChatController::getInstance()->getNewMsg(param);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChatServiceCocos2dx::setPlayerAllianceInfo(GlobalData::shared()->playerInfo.allianceInfo.shortName.c_str(),GlobalData::shared()->playerInfo.getAllianceId().c_str(),GlobalData::shared()->playerInfo.allianceInfo.rank ,GlobalData::shared()->playerInfo.isfirstJoin);
#endif
    
}