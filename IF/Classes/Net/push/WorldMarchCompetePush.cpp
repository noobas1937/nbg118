//
//  WorldMarchCompetePush.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/5/6.
//
//

#include "WorldMarchCompetePush.h"
#include "WorldController.h"
#include "CCCommonUtils.h"
void WorldMarchCompetePush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(params){
        int targetIndex = params->valueForKey("targetPoint")->intValue();
        CCPoint targetPt = WorldController::getInstance()->getPointByIndex(targetIndex);
        std::string coordStr = " (X:";
        coordStr.append(CC_ITOA(targetPt.x));
        coordStr.append(" Y:");
        coordStr.append(CC_ITOA(targetPt.y));
        coordStr.append(") ");
        std::string userName = params->valueForKey("userName")->getCString();
        CCCommonUtils::flyText(_lang_2("137475", userName.c_str(), coordStr.c_str()));
    }
}