//
//  WorldMarchCheck.cpp
//  IF
//
//  Created by ganxiaohua on 15-1-15.
//
//

#include "WorldMarchCheck.h"
#include "SceneController.h"
#include "WorldController.h"
#include "WorldMapView.h"

bool WorldMarchCheck::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_MARCH_CHECK) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        std::string monsterFailure = "103730";
        std::string resourceFailure = "108896";
        if(pStr->getCString() == monsterFailure)
        {
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD)
            {
                CCCommonUtils::flyHint("", "", _lang("103730"));
                WorldMapView::instance()->m_map->updateDynamicMap(ccp(-1, -1));
                WorldController::getInstance()->currentStamine += GlobalData::shared()->worldConfig.stamineIntervalNum;
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CURRENT_STAMINE);
            }
            callFail(NetResult::createWithFail(params));
        }
        else if(pStr->getCString() == resourceFailure)
        {
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                CCCommonUtils::flyHint("", "", _lang("108896"));
                WorldMapView::instance()->m_map->updateDynamicMap(ccp(-1, -1));
            }
            callFail(NetResult::createWithFail(params));
        }
        else
        {
            CCLOG("WORLD_MARCH_CHECK=%s",pStr->getCString());
            callFail(NetResult::create(Error_OK,params));
        }
    }else{
        callSuccess(NetResult::create(Error_OK,params));
    }
    return true;
}