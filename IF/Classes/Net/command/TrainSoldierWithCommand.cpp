//
//  TrainSoldierWithCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-4.
//
//

#include "TrainSoldierWithCommand.h"
#include "fireandcomman.h"
#include "QueueController.h"

bool TrainSoldierWithCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(TRAIN_WITH_GOLD_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        parseResult(params);
        //GlobalData::shared()->armyList[m_id].free = params->valueForKey("free")->intValue();
        //map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(m_id);
        
//        std::string icon = it->second.getBodyIcon();
//        std::string name = it->second.getName();
//        int index = atof(it->second.armyId.c_str())/100 - 1070 + 200;
//        CCLoadSprite::doResourceByCommonIndex(index, true);
//        CCCommonUtils::flyHint(icon, _lang("103673"), _lang_1("103674", name.c_str()));
//        CCLoadSprite::doResourceByCommonIndex(index, false);
        auto queue = _dict(params->objectForKey("queue"));
        QueueController::getInstance()->addQueueInfo(queue);
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ARMY_NUM_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPDATE_ARMY_DATA);
        callSuccess(NetResult::create());
    }
    return true;
}

void TrainSoldierWithCommand::parseResult(CCDictionary *dict){
    const CCString *pStr = dict->valueForKey("gold");
    if(pStr->compare("")!=0){
        int gold = pStr->intValue();
        GlobalData::shared()->playerInfo.gold = gold;
    }
    GlobalData::shared()->resourceInfo.setResourceData(dict);
}