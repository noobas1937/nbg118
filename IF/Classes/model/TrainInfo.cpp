//
//  TrainInfo.cpp
//  IF
//
//  Created by ganxiaohua on 14-3-17.
//
//

#include "TrainInfo.h"
#include "WorldController.h"

TrainInfo::TrainInfo(CCDictionary* dict) {
    m_trainId = dict->valueForKey("id")->getCString();
    m_name = dict->valueForKey("name")->getCString();
    m_cost = dict->valueForKey("cost")->intValue();
    m_icon = dict->valueForKey("icon")->getCString();
    m_rateExp = dict->valueForKey("rate_exp")->intValue();
    m_totalTime = dict->valueForKey("total_time")->intValue();
    m_spending = dict->valueForKey("spending")->intValue();
    m_suddenly = dict->valueForKey("suddenly")->intValue();
    m_entryTime = dict->valueForKey("entry_time")->intValue();
    if(m_cost==0){
        m_open = true;
    }else{
        m_open = false;
    }
    m_end = -1.0;
    m_finish = -1.0;
    m_genUid = "";
}

void TrainInfo::updateTrainInfo(CCDictionary* dict){
    const CCString *pStr = dict->valueForKey("genUid");
    if (pStr->compare("")!=0) {
        m_genUid = dict->valueForKey("genUid")->getCString();
    }
    pStr = dict->valueForKey("finish");
    if (pStr->compare("")!=0) {
         m_finish = dict->valueForKey("finish")->doubleValue();
    }
    m_end = dict->valueForKey("end")->doubleValue();
    int curTime = (m_end - WorldController::getInstance()->getTime())/1000;
    if(curTime>0 || m_cost==0 || m_end==0){
        m_open = true;
    }
}