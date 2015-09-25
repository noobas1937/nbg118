//
//  ChangeNickNameCommand.cpp
//  IF
//
//  Created by wangzhenlei on 13-11-4.
//
//

#include "ChangeNickNameCommand.h"
#include "PopupViewController.h"
#include "UIComponent.h"
#include "ToolController.h"
#include "WorldMapView.h"
#include "ChatServiceCocos2dx.h"

bool ChangeNickNameCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(USER_MODIFY_NICKNAME_COMMAND) != 0)
        return false;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_NAME_BACK);
    CCDictionary *params=_dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        if(params->objectForKey("gold")){
            GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->intValue();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        }

        GlobalData::shared()->playerInfo.nickName = m_newNickName;
        GlobalData::shared()->playerInfo.name = m_newNickName;
//        PopupViewController::getInstance()->goBackPopupView();
        PopupViewController::getInstance()->removeLastPopupView();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::setPlayerInfo();
        UIComponent::getInstance()->refreshChatInfo(CHAT_TYPE_COUNTRY);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChatServiceCocos2dx::setPlayerInfo();
#endif
        auto world = WorldMapView::instance();
        if (world) {
            world->updateSelfName();
        }
        CCCommonUtils::flyHint("", "", _lang("105227"));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_PLAYER_NAME);
    }
    return true;
}