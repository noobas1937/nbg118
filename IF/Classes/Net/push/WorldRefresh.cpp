//
//  WorldRefresh.cpp
//  IF
//
//  Created by 李锐奇 on 14-9-1.
//
//

#include "WorldRefresh.h"
#include "SceneController.h"
#include "WorldMapView.h"

void WorldAllianceRefreshPush::handleResponse(CCDictionary *dict)
{
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params)
        return;
    
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        if(WorldMapView::instance() && WorldMapView::instance()->m_map){
            if(params->valueForKey("pointId")){
                int pointId = params->valueForKey("pointId")->intValue();
                auto pt = WorldController::getInstance()->getPointByIndex(pointId);
                WorldMapView::instance()->m_map->updateDynamicMap(pt);
            }else{
                WorldMapView::instance()->m_map->updateDynamicMap();
            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(WORLD_ALLIANCE_REFRESH);
        }
    }
}