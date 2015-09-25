//
//  SocialController.cpp
//  IF
//
//  Created by ganxiaohua on 14-10-22.
//
//

#include "SocialController.h"

static SocialController* _instance;

SocialController* SocialController::getInstance(){
    if(!_instance){
        _instance = new SocialController();
    }
    return _instance;
}

void SocialController::parse(CCDictionary* dic){
    if(dic==NULL) return ;
    id = dic->valueForKey("id")->intValue();
    exp= dic->valueForKey("exp")->intValue();
    miner = dic->valueForKey("miner")->intValue();
    auto tmp = dic->valueForKey("helpTime")->doubleValue()/1000;
    if (tmp == 0) {
        helpTime = 0;
    }else{
        this->helpTime = GlobalData::shared()->changeTime(tmp);
    }
    tmp = dic->valueForKey("mineTime")->doubleValue()/1000;
    if (tmp == 0) {
        mineTime = 0;
    }else{
        this->mineTime = GlobalData::shared()->changeTime(tmp);
    }
    call = dic->valueForKey("call")->intValue();
    goldReward= dic->valueForKey("goldReward")->intValue();
    xml_gold = dic->valueForKey("xml_gold")->intValue();
    xml_exp = dic->valueForKey("xml_exp")->intValue();
    xml_lv = dic->valueForKey("level")->intValue();
    xml_miner = dic->valueForKey("xml_miner")->intValue();
    xml_weektime = dic->valueForKey("xml_weektime")->doubleValue();
}

void SocialController::parseCollect(CCDictionary* dic){
    if(dic==NULL) return ;
    if(dic->objectForKey("id")){
        id = dic->valueForKey("id")->intValue();
    }
    if(dic->objectForKey("exp")){
        exp = dic->valueForKey("exp")->intValue();
    }
    if(dic->objectForKey("miner")){
        miner = dic->valueForKey("miner")->intValue();
    }
    if(dic->objectForKey("helpTime")){
        auto tmp = dic->valueForKey("helpTime")->doubleValue()/1000;
        if (tmp == 0) {
            helpTime = 0;
        }else{
            this->helpTime = GlobalData::shared()->changeTime(tmp);
        }
    }
    if(dic->objectForKey("mineTime")){
        auto tmp = dic->valueForKey("mineTime")->doubleValue()/1000;
        if (tmp == 0) {
            mineTime = 0;
        }else{
            this->mineTime = GlobalData::shared()->changeTime(tmp);
        }
    }
    if(dic->objectForKey("goldReward")){
        goldReward = dic->valueForKey("goldReward")->intValue();
    }
    if(dic->objectForKey("xml_gold")){
        xml_gold = dic->valueForKey("xml_gold")->intValue();
    }
    if(dic->objectForKey("xml_exp")){
        xml_exp = dic->valueForKey("xml_exp")->intValue();
    }
    if(dic->objectForKey("level")){
        xml_lv = dic->valueForKey("level")->intValue();
    }
    if(dic->objectForKey("xml_miner")){
        xml_miner = dic->valueForKey("xml_miner")->intValue();
    }
    if(dic->objectForKey("xml_weektime")){
        xml_weektime = dic->valueForKey("xml_weektime")->doubleValue();
    }
}