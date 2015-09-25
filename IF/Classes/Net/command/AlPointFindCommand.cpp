//
//  AlPointFindCommand.cpp
//  IF
//
//  Created by xxrdsg on 15-7-2.
//
//

#include "AlPointFindCommand.h"
#include "AllianceManager.h"
#include "PopupViewController.h"
#include "WorldController.h"
#include "SceneContainer.h"
#include "WorldMapView.h"

bool AlPointFindCommand::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare(AL_POINT_FIND) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        CCCommonUtils::flyText(_lang(params->valueForKey("errorCode")->getCString()));
    } else {
        if (params->objectForKey("targetPoint")) {
            int point = params->valueForKey("targetPoint")->intValue();
            if (point <= 0) {
                if (m_op == 0)
                {
                    if (point == 0)
                    {
                        CCCommonUtils::flyText(_lang("115427"));
                    }
                } else if (m_op == 1) {
                    if (point == 0)
                    {
                        CCCommonUtils::flyText(_lang("115426"));
                    } else if (point == -1) {
                        CCCommonUtils::flyText(_lang("115425"));
                    }
                }
            } else {
                PopupViewController::getInstance()->removeAllPopupView();
                auto& info = ToolController::getInstance()->getToolInfoById(ITEM_ALLIANCE_CITY_MOVE);
                if (info.getCNT() <= 0) {
                    return true;
                }
                if (m_op == 0)
                {
                    AllianceManager::getInstance()->goToWorldType = 2;
                } else if (m_op == 1){
                    AllianceManager::getInstance()->goToWorldType = 3;
                }
                CCPoint pt = WorldController::getPointByIndex(point);
                WorldController::getInstance()->openTargetIndex = point;
                std::string isFirstPopKey = GlobalData::shared()->playerInfo.uid + "isFirstPop";
                int isFirstPop = CCUserDefault::sharedUserDefault()->getIntegerForKey(isFirstPopKey.c_str(),0);
                if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                    WorldMapView::instance()->gotoTilePoint(pt, GlobalData::shared()->playerInfo.selfServerId);
                }else{
                    if (isFirstPop==0) {
                        WorldController::getInstance()->firstInWorld = true;
                        isFirstPop = 1;
                        CCUserDefault::sharedUserDefault()->setIntegerForKey(isFirstPopKey.c_str(), isFirstPop);
                        CCUserDefault::sharedUserDefault()->flush();
                    }
                    SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, point);
                }
            }
        }
    }
    return true;
}