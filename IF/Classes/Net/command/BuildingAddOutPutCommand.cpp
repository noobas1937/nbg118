//
//  BuildingAddOutPutCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-29.
//
//

#include "BuildingAddOutPutCommand.h"
#include "FunBuildController.h"
#include "FunBuildInfo.h"

bool BuildingAddOutPutCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(BUILDING_ADD_OUTPUT) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
        (*FunBuildController::getInstance()->curBuildsInfo)[m_buildingKey].effectTime = 0;
    }else{
        if(params->objectForKey("effectTime")){
            double effectTime = params->valueForKey("effectTime")->doubleValue()/1000;
            if(effectTime!=0){
                (*FunBuildController::getInstance()->curBuildsInfo)[m_buildingKey].effectTime = GlobalData::shared()->changeTime(effectTime);
            }else{
                (*FunBuildController::getInstance()->curBuildsInfo)[m_buildingKey].effectTime = 0;
            }
        }
        if(params->objectForKey("gold")){
            GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->intValue();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_FUNCTION_EFFECT,CCInteger::create(m_buildingKey));
        callSuccess(NetResult::create(Error_OK,params));
    }
    return true;
}