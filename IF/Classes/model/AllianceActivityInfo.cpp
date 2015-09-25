//
//  AllianceActivityInfo.cpp
//  IF
//
//  Created by ganxiaohua on 15/4/1.
//
//

#include "AllianceActivityInfo.h"

void AllianceActivityInfo::parse(CCDictionary* dicData){
    if(dicData==NULL) return ;
    if(dicData->objectForKey("activityST")){
        activityST = dicData->valueForKey("activityST")->doubleValue()/1000;
        activityST = GlobalData::shared()->changeTime(activityST);
    }
    if(dicData->objectForKey("activityET")){
        activityET = dicData->valueForKey("activityET")->doubleValue()/1000;
        activityET = GlobalData::shared()->changeTime(activityET);
    }
    if(dicData->objectForKey("siegeST")){
        siegeST = dicData->valueForKey("siegeST")->doubleValue()/1000;
        siegeST = GlobalData::shared()->changeTime(siegeST);
    }
    if(dicData->objectForKey("siegeET")){
        siegeET = dicData->valueForKey("siegeET")->doubleValue()/1000;
        siegeET = GlobalData::shared()->changeTime(siegeET);
    }
    round = dicData->valueForKey("round")->intValue();
    posX = dicData->valueForKey("x")->intValue();
    posY = dicData->valueForKey("y")->intValue();
    if(dicData->objectForKey("nextRoundTime")){
        nextRoundTime = dicData->valueForKey("nextRoundTime")->doubleValue()/1000;
        nextRoundTime = GlobalData::shared()->changeTime(nextRoundTime);
    }
    monsterId = dicData->valueForKey("monsterId")->intValue();
    count = dicData->valueForKey("count")->intValue();
    if (m_monsterInfo==NULL) {
        m_monsterInfo = CCArray::create();
    }
    m_monsterInfo->removeAllObjects();
    CCArray* tempArr = dynamic_cast<CCArray*>(dicData->objectForKey("monsterInfo"));
    if (tempArr!=NULL) {
        m_monsterInfo->addObjectsFromArray(tempArr);
    }
    userKill = dicData->valueForKey("userKill")->intValue();
    userRank = dicData->valueForKey("userRank")->intValue();
    allKill = dicData->valueForKey("allKill")->intValue();
    allRank = dicData->valueForKey("allRank")->intValue();
    state = dicData->valueForKey("state")->intValue();
    userState = dicData->valueForKey("userState")->intValue();
    if(dicData->objectForKey("rewardTime")){
        rewardTime = dicData->valueForKey("rewardTime")->doubleValue()/1000;
        rewardTime = GlobalData::shared()->changeTime(rewardTime);
    }
}