//
//  AllianceLeavelPush.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-14.
//
//

#include "AllianceLeavelPush.h"
#include "YesNoDialog.h"
#include "AllianceManager.h"
#include "MailController.h"
#include "FunBuildController.h"

void AllianceLeavelPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    GlobalData::shared()->playerInfo.setAllianceId("");
    GlobalData::shared()->playerInfo.allianceInfo.reset();
    AllianceManager::getInstance()->clearEvents();
    YesNoDialog::showConfirmCloseAllView(_lang("115098").c_str(), _lang("confirm").c_str());
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MARK_LINE_AL);
}