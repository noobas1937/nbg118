//
//  LeaveAllianceCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-9.
//
//

#include "LeaveAllianceCommand.h"
#include "AllianceManager.h"
#include "MailController.h"
#include "FunBuildController.h"

bool LeaveAllianceCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_AL_LEAVE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    GameController::getInstance()->removeWaitInterface();
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
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