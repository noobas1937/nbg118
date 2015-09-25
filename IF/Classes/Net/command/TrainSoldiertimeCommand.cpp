//
//  TrainSoldiertimeCommand.cpp
//  IF
//
//  Created by jiaohongye on 14-2-26.
//
//

#include "TrainSoldiertimeCommand.h"
#include "fireandcomman.h"
#include "QueueController.h"
#include "QueueInfo.h"
#include "FunBuild.h"

bool TrainSoldiertimeCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(COMPLETE) != 0){
        return false;
    }
    if (!dict)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
        GlobalData::shared()->playerInfo.armyPower =  GlobalData::shared()->playerInfo.armyPower -  GlobalData::shared()->playerInfo.addPower;//如果失败，把加的减回去
    }else{
        if(GlobalData::shared()->armyList.find(m_id) == GlobalData::shared()->armyList.end()){
            return true;
        }
        GlobalData::shared()->armyList[m_id].free = params->valueForKey("free")->intValue();
        GlobalData::shared()->armyList[m_id].finishTime = 0;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ARMY_NUM_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPDATE_ARMY_DATA);
        map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(GlobalData::shared()->armyList[m_id].armyId);
        if(it == GlobalData::shared()->armyList.end() || it->second.armyId == ""){
            return true;
        }
        std::string icon = it->second.getBodyIcon();
        std::string name = it->second.getName();
        if(!CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(icon.c_str())){
            int index = atof(GlobalData::shared()->armyList[m_id].armyId.c_str())/100 - 1070 + 200;
            CCLoadSprite::doResourceByCommonIndex(index, true);
            CCCommonUtils::flyHint(icon, _lang("103673"), _lang_1("103674", name.c_str()));
            CCLoadSprite::doResourceByCommonIndex(index, false);
        }else{
            CCCommonUtils::flyHint(icon, _lang("103673"), _lang_1("103674", name.c_str()));
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUICK_TROOPS_HARVEST);
    }
    GlobalData::shared()->playerInfo.addPower = 0;
    return true;
}

