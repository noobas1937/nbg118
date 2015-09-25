//
//  GeneralUpdateDataPush.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-10.
//
//

#include "GeneralUpdateDataPush.h"

void GeneralUpdateDataPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* params = _dict(dict->objectForKey("params"));
    //army初始化
    CCArray *armyArr = dynamic_cast<CCArray*>(params->objectForKey("army"));
    CCObject *armyObject;
    CCARRAY_FOREACH(armyArr, armyObject){
        CCDictionary *armyDic = _dict(armyObject);
        if(armyDic){
            std::string itemId = armyDic->valueForKey("id")->getCString();
            map<std::string,ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(itemId);
            if(it != GlobalData::shared()->armyList.end()){
                it->second.setArmyInfo(armyDic);
            }
        }
    }
    //fort 初始化
    CCArray *fortArr = dynamic_cast<CCArray*>(params->objectForKey("fort"));
    CCObject *fortObject;
    CCARRAY_FOREACH(fortArr, fortObject){
        CCDictionary *forDic = _dict(fortObject);
        if(forDic){
            std::string itemId = forDic->valueForKey("id")->getCString();
            map<std::string,ArmyInfo>::iterator it = GlobalData::shared()->fortList.find(itemId);
            if(it != GlobalData::shared()->fortList.end()){
                it->second.setArmyInfo(forDic);
            }
        }
    }
    return;
}