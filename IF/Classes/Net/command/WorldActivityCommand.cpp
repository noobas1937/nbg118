//
//  WorldActivityCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14/12/4.
//
//

#include "WorldActivityCommand.h"
#include "WorldController.h"
#include "SceneController.h"
#include "PopupViewController.h"
#include "WorldMapView.h"

bool WorldActivityGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_WORLD_ACTIVITY_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
    }else{
        CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("act"));
        if(arr){
            CCObject *obj;
            CCARRAY_FOREACH(arr, obj){
                WorldController::getInstance()->updateWorldActivityTime(_dict(obj));
            }
        }
    }
    return true;
}

bool CrossWorldActivityGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_CROSS_WORLD_ACTIVITY_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
    } else {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GET_CROSS_WORLD_DATA_BACK, params);
    }
    
    return true;
}

bool GetFightInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_WAR_BUILD_FIGHT_INFO) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
    } else {
        callSuccess(NetResult::createWithSuccess(params));
    }
    
    return true;
}

bool GetACTBossCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_ACT_BOSS_PT) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
    } else {
        //跳转到世界某个点上
        int pos = params->valueForKey("id")->intValue();
        WorldController::getInstance()->openTargetIndex = pos;
        CCPoint pt = WorldController::getPointByIndex(pos);
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            WorldMapView::instance()->gotoTilePoint(pt);
        }else{
            int index = WorldController::getIndexByPoint(pt);
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
        }
        PopupViewController::getInstance()->removeAllPopupView();
    }
    
    return true;
}

bool AnniversaryGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(Ann_Get_Info) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
    } else {
        callSuccess(NetResult::createWithSuccess(params));
    }
    
    return true;
}
