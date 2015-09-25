//
//  TreatCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-6.
//
//

#include "TreatCommand.h"
#include "fireandcomman.h"
#include "ArmyController.h"
#include "QueueController.h"

bool TreatStartCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(TREAT_START_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        auto dict = CCDictionary::create();
        time_t finishTime = params->valueForKey("finish")->doubleValue() / 1000;
        ArmyController::getInstance()->setTreatFinishTime(finishTime);
        dict->setObject(params, "resource");
        TreatResultParser::parseResult(TREAT_START_COMMAND, dict);
        
        CCDictElement *element;
        CCDICT_FOREACH(m_dict, element) {
            std::string armyId = element->getStrKey();
            int num = dynamic_cast<CCInteger*>(element->getObject())->getValue();
            GlobalData::shared()->treatList[armyId].heal = num;
            GlobalData::shared()->treatList[armyId].dead -= num;
        }
        
        auto queue = _dict(params->objectForKey("queue"));
        QueueController::getInstance()->addQueueInfo(queue);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ARMY_NUM_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GO_TO_HEALTHING);
    }
    return true;
}

bool TreatEndCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(TREAT_END_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        ArmyController::getInstance()->setTreatEndTime(0);
        TreatResultParser::parseResult(TREAT_END_COMMAND, params);
    }
    return true;
}

bool TreatCancelCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(TREAT_CANCEL_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        map<std::string, TreatInfo>::iterator it;
        for(it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++){
            it->second.heal = 0;
        }
        ArmyController::getInstance()->treatFinishTime = 0;
        TreatResultParser::parseResult(TREAT_CANCEL_COMMAND, params);
    }
    return true;
}

bool TreatStartUseGoldCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(TREAT_START_USE_GOLD_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        TreatResultParser::parseResult(TREAT_START_USE_GOLD_COMMAND, params);
    }
    return true;
}

void TreatResultParser::parseResult(std::string cmd, cocos2d::CCDictionary *dict){
    if(dict->objectForKey("army")){
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("army"));
        CCObject *obj;
        bool bst = false;
        CCARRAY_FOREACH(arr, obj){
            auto armyDict = dynamic_cast<CCDictionary*>(obj);
            ArmyController::getInstance()->refreshArmyFreeNum(armyDict,false);
            bst = true;
        }
    }
    ArmyController::getInstance()->setTreatEndTime(0);    
    if(dict->objectForKey("hospital")){
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("hospital"));
        CCObject *obj;
        CCARRAY_FOREACH(arr, obj){
            auto armyDict = dynamic_cast<CCDictionary*>(obj);
            ArmyController::getInstance()->refreshTreat(armyDict);
        }
    }
    if(dict->objectForKey("gold")){
        int gold = dict->valueForKey("gold")->intValue();
        GlobalData::shared()->playerInfo.gold = gold;
    }
    if(dict->objectForKey("resource")){
        auto resDict = _dict(dict->objectForKey("resource"));
        if(resDict->objectForKey("gold")){
            int gold = resDict->valueForKey("gold")->intValue();
            GlobalData::shared()->playerInfo.gold = gold;
        }
        GlobalData::shared()->resourceInfo.setResourceData(resDict);
    }
}