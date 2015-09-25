//
//  DismissAllianceCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-9.
//
//

#include "DismissAllianceCommand.h"
#include "AllianceManager.h"
#include "FunBuildController.h"

bool DismissAllianceCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_AL_DISMISS) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
    }else{
        GlobalData::shared()->playerInfo.allianceInfo.reset();
        GlobalData::shared()->playerInfo.setAllianceId("");
        AllianceManager::getInstance()->clearEvents();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_HELP_NUM_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MARK_LINE_AL);
        callSuccess(NetResult::create());
    }
    return true;
}