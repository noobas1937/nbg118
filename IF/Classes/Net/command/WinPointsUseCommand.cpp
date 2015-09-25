//
//  WinPointsUseCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/17.
//
//

#include "WinPointsUseCommand.h"
#include "SceneController.h"
#include "WorldMapView.h"

bool WinPointsUseCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WIN_POINTS_USE) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        int itemId = params->valueForKey("itemId")->intValue();
        if (params->objectForKey("remainPoints")) {
            long remainPoints = params->valueForKey("remainPoints")->doubleValue();
            GlobalData::shared()->playerInfo.winPoint = remainPoints;
        }
        if(params && params->objectForKey("itemEffectObj"))
        {
            auto effectObj = _dict(params->objectForKey("itemEffectObj"));
            
            if (effectObj->objectForKey("oldStatus")) {//删除该状态的作用
                int reStatusId = effectObj->valueForKey("oldStatus")->intValue();
                if (GlobalData::shared()->statusMap.find(reStatusId) != GlobalData::shared()->statusMap.end()) {
                    GlobalData::shared()->statusMap[reStatusId] = 0;
                }
            }
            
            if (effectObj->objectForKey("effectState")) {
                auto stateDict = _dict(effectObj->objectForKey("effectState"));
                CCDictElement* element;
                CCDICT_FOREACH(stateDict, element)
                {
                    string key = element->getStrKey();
                    int effectId = atoi(key.c_str());
                    if(effectId>=PLAYER_PROTECTED_TIME1 && effectId<=PLAYER_PROTECTED_TIME5){
                        GlobalData::shared()->playerInfo.protectTimeStamp = stateDict->valueForKey(key)->doubleValue();
                        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                            WorldMapView::instance()->m_map->updateDynamicMap(WorldController::getInstance()->selfPoint);
                        }
                    }
                    double time = stateDict->valueForKey(key)->doubleValue()/1000;
                    if (time>0) {
                        time = GlobalData::shared()->changeTime(time);
                    }
                    if (key!="startTime") {
                        GlobalData::shared()->statusMap[atoi(key.c_str())] = time;
                    }
                    auto info = ToolController::getInstance()->getToolInfoById(itemId);
                    map<int, CCDictionary* >::iterator it = ToolController::getInstance()->m_statusItems.find(info.type2);
                    CCObject* obj = element->getObject();
                    CCString* str = (CCString*)obj;
                    if(info.type==4){
                        if(it!=ToolController::getInstance()->m_statusItems.end()){
                            auto dic = it->second;
                            if(!dic->objectForKey("startTime")){
                                dic->setObject(CCString::create(CC_ITOA(WorldController::getInstance()->getTime())), "startTime");
                            }else if(key!="" && key!="startTime"){
                                dic->setObject(CCString::create(str->getCString()), "endTime");
                            }else if(key=="startTime"){
                                dic->setObject(CCString::create(str->getCString()), "startTime");
                            }
                            ToolController::getInstance()->m_statusItems[info.type2] = dic;
                        }else{
                            auto infoDic = CCDictionary::create();
                            infoDic->retain();
                            if(key!="" && key!="startTime"){
                                infoDic->setObject(CCString::create(str->getCString()), "endTime");
                            }else if(key=="startTime"){
                                infoDic->setObject(CCString::create(str->getCString()), "startTime");
                            }
                            ToolController::getInstance()->m_statusItems[info.type2] = infoDic;
                        }
                    }
                    if (effectObj->objectForKey("status")) {
                        auto arr = dynamic_cast<CCArray*>(effectObj->objectForKey("status"));
                        if (arr) {
                            CCDictionary *item = NULL;
                            for (int i=0; i<arr->count(); i++) {
                                item = _dict(arr->objectAtIndex(i));
                                auto effState = stateEffect();
                                effState.effectId = item->valueForKey("effNum")->intValue();
                                effState.value = item->valueForKey("effVal")->intValue();
                                effState.stateId = item->valueForKey("stateId")->intValue();
                                
                                if (true) {//去除重复的stateId
                                    vector<stateEffect>::iterator it = GlobalData::shared()->effectStateMap[effState.effectId].begin();
                                    for (; it != GlobalData::shared()->effectStateMap[effState.effectId].end(); it++) {
                                        if (effState.stateId == it->stateId) {
                                            GlobalData::shared()->effectStateMap[effState.effectId].erase(it);
                                            break;
                                        }
                                    }
                                }
                                GlobalData::shared()->effectStateMap[effState.effectId].push_back(effState);
                            }
                        }
                    }
                }
            }
            }
        callSuccess(NetResult::create(Error_OK, params));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WIN_POINT_CHANGE);
    }
    return true;
}