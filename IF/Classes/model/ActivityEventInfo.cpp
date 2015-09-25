//
//  ActivityEventInfo.cpp
//  IF
//
//  Created by ganxiaohua on 14-9-2.
//
//

#include "ActivityEventInfo.h"

void ActivityEventInfo::parse(CCDictionary *dict){
    if(dict==NULL) return ;
    if(dict->objectForKey("readyTime")){
        readyTime = dict->valueForKey("readyTime")->doubleValue() / 1000;
        readyTime = GlobalData::shared()->changeTime(readyTime);
    }
    if(dict->objectForKey("beginTime")){
        beginTime = dict->valueForKey("beginTime")->doubleValue() / 1000;
        beginTime = GlobalData::shared()->changeTime(beginTime);
    }
    if(dict->objectForKey("endTime")){
        endTime = dict->valueForKey("endTime")->doubleValue() / 1000;
        endTime = GlobalData::shared()->changeTime(endTime);
    }
    if(dict->objectForKey("id")){
        id = dict->valueForKey("id")->getCString();
    }
    if(dict->objectForKey("events")){
        events = dict->valueForKey("events")->getCString();
    }
    if(dict->objectForKey("target")){
        target = dict->valueForKey("target")->getCString();
    }
    if(dict->objectForKey("reward")){
        reward = dict->valueForKey("reward")->getCString();
    }
    if(dict->objectForKey("curPhase")){
        curPhase = dict->valueForKey("curPhase")->intValue()+1;
    }
    if(dict->objectForKey("maxPhase")){
        maxPhase = dict->valueForKey("maxPhase")->intValue()+1;
    }
    if(dict->objectForKey("rankReward")){
       m_rankReward = dynamic_cast<CCArray*>(dict->objectForKey("rankReward"));
    }
    if(dict->objectForKey("totalRankReward")){
        m_totalRankReward = dynamic_cast<CCArray*>(dict->objectForKey("totalRankReward"));
    }
    if(m_rankReward==NULL){
        m_rankReward = CCArray::create();
    }
    if(m_totalRankReward==NULL){
        m_totalRankReward = CCArray::create();
    }
}

void ActivityEventInfo::reset(){
    readyTime = 0;
    beginTime = 0;
    endTime = 0;
    id = "";
    events = "";
    target = "";
    reward = "";
    curPhase = 0;
    maxPhase = 0;
    if(m_rankReward){
        m_rankReward->removeAllObjects();
    }
    if(m_totalRankReward){
        m_totalRankReward->removeAllObjects();
    }
}
