//
//  DragonController.cpp
//  IF
//
//  Created by 张军 on 15/6/5.
//
//

#include "DragonController.h"
#include "DragonInfo.h"
#include "DragonCommand.h"

static DragonController *_instance = NULL;

DragonController::DragonController(){
}

DragonController::~DragonController() {
}

DragonController* DragonController::getInstance() {
    if (!_instance) {
        _instance = new DragonController();
    }
    return _instance;
}

void DragonController::setDragonDataFromServer(CCDictionary* dict){
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("dragon"));
    if( arr==NULL ){
        GameController::getInstance()->removeWaitInterface();
        return;
    }
    
    CCDictionary *item = NULL;
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        string place = item->valueForKey("place")->getCString();
        int exp = item->valueForKey("exp")->intValue();
        int id = item->valueForKey("id")->intValue();
        int level = item->valueForKey("level")->intValue();
        string uuid = item->valueForKey("uuid")->getCString();
        
        CCDictionary* xmlDictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(id));
        DragonInfo oneDragon;
        if (GlobalData::shared()->dragonInfoMap.find(id) != GlobalData::shared()->dragonInfoMap.end()) {
            oneDragon = (GlobalData::shared()->dragonInfoMap)[id];
            oneDragon.SetInfoFromXml(xmlDictInfo);
        }else {
            oneDragon = DragonInfo(xmlDictInfo);
        }
        oneDragon.SetInfoFromServer(item);
        (GlobalData::shared()->dragonInfoMap)[id] = oneDragon;
    }
}

//喂龙
bool DragonController::startFeedDragon(int dragonId){
    map<int, DragonInfo> *curDragonInfo = &(GlobalData::shared()->dragonInfoMap);
//    DragonInfo *build1 = &(GlobalData::shared()->dragonInfoMap[dragonId]);
    if (curDragonInfo->find(dragonId) == curDragonInfo->end()) {
        return false;
    }
    auto &info = (*curDragonInfo)[dragonId];
    
    DragonFeedCommand * cmd=new DragonFeedCommand(info.uuid, DRAGON_CMD_FEED);
    cmd->sendAndRelease();
    
    return true;
}

void DragonController::endFeedDragon(CCDictionary* dict){
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        /*
         (int) dragonFood: 999999749
         (sfs_object) dragon:
         (utf_string) place: 2efff876bfe54f04a045f6ab63da8255
         (int) exp: 250
         (utf_string) id: 5030012
         (int) level: 1
         (utf_string) uuid: test
         */
        if (dict->valueForKey("dragonFood")) {
            int dragonFood = dict->valueForKey("dragonFood")->intValue();
            
            //更新资源
        }
    }
}

//卖龙/蛋
bool DragonController::startSellDragon(int dragonId){
    map<int, DragonInfo> *curDragonInfo = &(GlobalData::shared()->dragonInfoMap);
    if (curDragonInfo->find(dragonId) == curDragonInfo->end()) {
        return false;
    }
    auto &info = (*curDragonInfo)[dragonId];
    
    DragonSellCommand* cmd=new DragonSellCommand(info.uuid, DRAGON_CMD_SELL);
    cmd->sendAndRelease();
    
    return true;
}

void DragonController::endSellDragon(CCDictionary* dict){
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        /*
         (sfs_object) p:
         (int) dragonGold: 1000000299
         */
        CCDictionary* pDict = dynamic_cast<CCDictionary*>(dict->objectForKey("p"));
        if (pDict) {
            int dragonGold = pDict->valueForKey("dargonGold")->intValue();
            //更新资源
        }
    }
}

