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
    activityId = dict->valueForKey("actId")->getCString();
    type = dict->valueForKey("t")->intValue();
    eventId = dict->valueForKey("eventId")->getCString();
    getScoreMeth = dict->valueForKey("score")->getCString();
    
    currentScore = 0;
    if (dict->objectForKey("userScore")) {
        //{"id":"9001003","actId":"649510bfccc34ebb930513c4564dd73e","score":67400,"type":1}
        CCDictionary* tmpData = _dict(dict->objectForKey("userScore"));
        currentScore = tmpData->valueForKey("score")->intValue();
    }
    
    sumScore = 0;
    //解析目标 和 对应奖励
    string targets = dict->valueForKey("target")->getCString();
    vector<string> tagVec;
    CCCommonUtils::splitString(targets, "|", tagVec);
    string values = dict->valueForKey("value")->getCString();
    vector<string> valueVec;
    CCCommonUtils::splitString(values, "|", valueVec);
    CCArray* rewardInfo = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
    if (rewardInfo) {
        if (tagVec.size() == rewardInfo->count() && tagVec.size() == valueVec.size()) {
            for (int i=0; i<rewardInfo->count(); i++)
            {
                CCArray* rewardDict = dynamic_cast<CCArray*>(rewardInfo->objectAtIndex(i));
                if (rewardDict) {
                    int tag = atoi(tagVec[i].c_str());
                    m_scoreRwdMap[ tag ] = rewardDict;
                    m_rwdPriceMap[ tag ] = atoi(valueVec[i].c_str());
                    m_tagVec.push_back(tag);
                    if (sumScore <= tag) {
                        sumScore = tag;
                    }
                }
            }
        }
    }
}
CCArray* ActivityInfo::getScroeRweMapByKey(int key){
    if(m_scoreRwdMap.find(key) != m_scoreRwdMap.end()){
        return m_scoreRwdMap[key];
    }
    return NULL;
}
ActivityInfo::~ActivityInfo(){
    m_scoreRwdMap.clear();
}