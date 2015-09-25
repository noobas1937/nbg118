//
//  PlayerRankInfo.cpp
//  IF
//
//  Created by lifangkai on 14-9-16.
//
//

#include "PlayerRankInfo.h"
#include "GlobalData.h"

PlayerRankInfo* PlayerRankInfo::create(){
    PlayerRankInfo* ret = new PlayerRankInfo();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
void PlayerRankInfo::update(CCDictionary* dict,double time){
    alliancename = dict->valueForKey("alliancename")->getCString();
    abbr = dict->valueForKey("abbr")->getCString();
    uid = dict->valueForKey("uid")->getCString();
    pic = dict->valueForKey("pic")->getCString();
    name = dict->valueForKey("name")->getCString();
    power= (unsigned long)dict->valueForKey("power")->doubleValue();
    armyKill= (unsigned long)dict->valueForKey("armyKill")->doubleValue();
    level= dict->valueForKey("level")->intValue();
    picVer = dict->valueForKey("picVer")->intValue();
}