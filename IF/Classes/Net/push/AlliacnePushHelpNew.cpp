//
//  AlliacnePushHelpNew.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-17.
//
//

#include "AlliacnePushHelpNew.h"

void AlliacnePushHelpNew::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        std::string senderId = params->valueForKey("senderId")->getCString();
        if(GlobalData::shared()->playerInfo.uid!=senderId){
            GlobalData::shared()->playerInfo.allianceInfo.helpcount += 1;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_HELP_NUM_CHANGE);
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_NEW_HELP,params);
}