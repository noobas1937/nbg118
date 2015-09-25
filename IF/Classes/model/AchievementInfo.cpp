//
//  AchievementInfo.cpp
//  IF
//
//  Created by ganxiaohua on 13-12-23.
//
//

#include "AchievementInfo.h"
#include "LocalController.h"
#include "CCCommonUtils.h"
#include "AchievementController.h"
#include "FunBuildController.h"

void AchievementInfo::initData(cocos2d::CCDictionary *dict){
    setXmlData(dict);
//    totalLevel = 5;
//    currentLevel = 1;
//    currentNum = 0;
    
//    refreshVisibleFlag();
}

void AchievementInfo::initData(std::string itemId){
    auto itemDict = LocalController::shared()->DBXMLManager()->getObjectByKey(itemId);
    initData(itemDict);
}

void AchievementInfo::setXmlData(std::string itemId){
    auto itemDict = LocalController::shared()->DBXMLManager()->getObjectByKey(itemId);
    setXmlData(itemDict);
}

void AchievementInfo::setXmlData(cocos2d::CCDictionary *dict){
    currentItemId = dict->valueForKey("id")->getCString();
    groupId = dict->valueForKey("series")->getCString();
    totalNum = dict->valueForKey("para1")->intValue();
    showLevel = dict->valueForKey("show")->intValue();
    showOrder = dict->valueForKey("level")->intValue();
    series = dict->valueForKey("series")->intValue();
    medalId = dict->valueForKey("medal")->getCString();
    iconMain = dict->valueForKey("icon_main")->getCString();
    iconMain = iconMain + ".png";
    iconAdditional = dict->valueForKey("icon_sub")->getCString();
    iconAdditional = iconAdditional + ".png";
    std::string dialogId = CCCommonUtils::getPropById(currentItemId, "type");
    name = _lang(dialogId);
    dialogId = CCCommonUtils::getPropById(currentItemId, "dialog");
    description = _lang_1(dialogId, CC_ITOA_K(double(totalNum)));
    if(currentItemId=="260135"&&dict->valueForKey("time")->getCString()){
        string time = dict->valueForKey("time")->getCString();
        description = _lang_1(dialogId, time.c_str());
    }
    if((currentItemId=="260136"||currentItemId=="260137"||currentItemId=="260138")&&dict->valueForKey("time")->getCString()){
        string time = dict->valueForKey("time")->getCString();
        description = _lang_2(dialogId, time.c_str(), CC_CMDITOA(totalNum).c_str());
    }
    reward = CCArray::create();
    reward->retain();
}

void AchievementInfo::updateData(cocos2d::CCDictionary *dict){
    std::string itemId = dict->valueForKey("id")->getCString();
    auto itemDict = LocalController::shared()->DBXMLManager()->getObjectByKey(itemId);
    setXmlData(itemDict);
    
    if(dict->valueForKey("num")){
        currentNum = dict->valueForKey("num")->intValue();
    }
    state = dict->valueForKey("state")->intValue();
    if ((currentNum>=totalNum) && (state==ACHIEVEMENT_NORMAL))
    {
        state = ACHIEVEMENT_COMPELETE;
    }
    reward->removeAllObjects();
    if(dict->objectForKey("reward")){
        CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        int i = 0;
        while (i < arr->count()) {
            auto dic = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
            reward->addObject(dic);
            i++;
        }
    }
    
    if (dict->valueForKey("start")) {
        start = dict->valueForKey("start")->doubleValue();
        start/=1000;
    }
    
    if (dict->valueForKey("end")) {
        end = dict->valueForKey("end")->doubleValue();
        end/=1000;
    }
    
    double nowTime = GlobalData::shared()->getWorldTime();
    if (nowTime>=start && nowTime<=end) {
        isVisible=true;
    }else{
        isVisible=false;
    }

    refreshVisibleFlag();
}

bool AchievementInfo::refreshVisibleFlag(){
    bool flag = false;
    int mainLevel = FunBuildController::getInstance()->getMainCityLv();
    if(mainLevel >= showLevel){
        flag = true;
    }
    
    if(isVisible != flag){
        isVisible = flag;
        return true;
    }
    return false;
}

void AchievementInfo::updateNumData(cocos2d::CCDictionary *dict){
    if(dict->objectForKey("num")){
        currentNum = dict->valueForKey("num")->intValue();
        if(isVisible){
            if (state != ACHIEVEMENT_COMPELETE) {
                state = ACHIEVEMENT_COMPELETE;
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACHIEVEMENT_VISIBLE_STATE_CHANGE);
            }
        }
    }
}