//
//  TrainSoldierCommand.cpp
//  IF
//
//  Created by jiaohongye on 14-2-20.
//
//

#include "TrainSoldierCommand.h"
#include "fireandcomman.h"
#include "QueueController.h"

bool TrainSoldierCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(SOLDIER) != 0){
        return false;
    }
     CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        parseResult(params);
        params->setObject(CCString::create(CC_ITOA(m_num)), "train");
        GlobalData::shared()->armyList[m_id].setFinishiTime(params);
        auto queue = _dict(params->objectForKey("queue"));
        QueueController::getInstance()->addQueueInfo(queue);
        callSuccess(NetResult::create());
    }
    return true;
}

void TrainSoldierCommand::parseResult(CCDictionary *dict){
    if(dict->objectForKey("gold")) {
        int tmpInt = dict->valueForKey("gold")->intValue();
        UIComponent::getInstance()->updateGold(tmpInt);
    }
    GlobalData::shared()->resourceInfo.setResourceData(dict);
}