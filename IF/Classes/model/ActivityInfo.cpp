//
//  ActivityInfo.cpp
//  IF
//
//  Created by 李锐奇 on 14-4-3.
//
//

#include "ActivityInfo.h"
#include "ActivityController.h"
#include "FunBuildController.h"

void ActivityInfo::parse(CCDictionary *dict){
    startTime = dict->valueForKey("st")->doubleValue() / 1000;
    startTime = GlobalData::shared()->changeTime(startTime);
    endTime = dict->valueForKey("et")->doubleValue() / 1000;
    endTime = GlobalData::shared()->changeTime(endTime);
    activityId = dict->valueForKey("id")->getCString();
    type = dict->valueForKey("t")->intValue();
    rewardFlag = dict->valueForKey("rewardFlag")->getCString();

    std::string actionStr = dict->valueForKey("action")->getCString();
    vector<std::string> vector1 ;
    vector<std::string> vector2 ;
    CCCommonUtils::splitString(actionStr, "|", vector1);
    int i = 0;
    while (i < vector1.size()) {
        vector2.clear();
        CCCommonUtils::splitString(vector1[i], ";", vector2);
        int key = atoi(vector2[0].c_str());
        int value = atoi(vector2[0].c_str());
        actionMap[key] = value;
        i++;
    }
    
    if(dict->objectForKey("level")){
        currentLevel = dict->valueForKey("level")->intValue();
    }else{
        if(type == ACTIVITY_SOLO){
            int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_MAIN);
            currentLevel = GlobalData::shared()->imperialInfo[bid].level;
        }else{
            currentLevel = 0;
        }
    }
    lv = dict->valueForKey("lv")->getCString();
    vector<std::string> vector;
    CCCommonUtils::splitString(lv, ";", vector);
    int levelIndex = 0;
    while(levelIndex < vector.size()){
        int level = atoi(vector[levelIndex].c_str());
        if(level > currentLevel || levelIndex == vector.size() - 1){
            break;
        }
        levelIndex++;
    }
    
    CCDictionary *prizeDict = dynamic_cast<CCDictionary*>(dict->objectForKey("prize"));
    std::string indexStr = std::string("lv") + CC_ITOA(levelIndex);
    
    
    scoreArr = CCArray::create();
    vector.clear();
    std::string scoreStr = dict->valueForKey("scoreRange")->getCString();
    CCCommonUtils::splitString(scoreStr, "|", vector);
    scoreStr = vector[levelIndex];
    vector.clear();
    CCCommonUtils::splitString(scoreStr, ";", vector);

    int index = 0;
    while(index < vector.size()){
        scoreArr->addObject(CCInteger::create(atoi(vector[index].c_str())));
        index++;
    }
    
    prizeArr = CCArray::create();
    auto arr = dynamic_cast<CCArray*>(prizeDict->objectForKey(indexStr.c_str()));
    index = 0;
    while(index < arr->count()){
        CCArray *arr1 = NULL;
        arr1 = dynamic_cast<CCArray*>(arr->objectAtIndex(index));
        int need = dynamic_cast<CCInteger*>(scoreArr->objectAtIndex(index))->getValue();
        CCObject *obj;
        CCARRAY_FOREACH(arr1, obj){
            CCDictionary *d = dynamic_cast<CCDictionary*>(obj);
            int type = d->valueForKey("type")->intValue();
            int value = d->valueForKey("value")->intValue();
            CCDictionary *insertDict = CCDictionary::create();
            insertDict->setObject(CCString::create(CC_ITOA(type)), "type");
            insertDict->setObject(CCString::create(CC_ITOA(value)), "value");
            insertDict->setObject(CCString::create(CC_ITOA(need)), "need");
            prizeArr->addObject(insertDict);
        }
        index++;
    }
    CCDictionary *board = dynamic_cast<CCDictionary*>(dict->objectForKey("board"));
    boardArr = dynamic_cast<CCArray*>(board->objectForKey("1"));
    
    if(dict->objectForKey("score")){
        currentScore = dict->valueForKey("score")->intValue();
    }else{
        currentScore = 0;
    }
}

ActivityInfo::~ActivityInfo(){
    scoreArr->removeAllObjects();
    prizeArr->removeAllObjects();
    boardArr->removeAllObjects();
}