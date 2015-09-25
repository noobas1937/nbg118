//
//  AllianceRankInfo.cpp
//  IF
//
//  Created by lifangkai on 14-9-16.
//
//

#include "AllianceRankInfo.h"
#include "GlobalData.h"

AllianceRankInfo* AllianceRankInfo::create(){
    AllianceRankInfo* ret = new AllianceRankInfo();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
void AllianceRankInfo::update(CCDictionary* dict){
    alliancename = dict->valueForKey("alliancename")->getCString();
    abbr =dict->valueForKey("abbr")->getCString();
    uid = dict->valueForKey("uid")->getCString();
    if(dict->objectForKey("icon")){
        icon = dict->valueForKey("icon")->getCString();
    }
    leader = dict->valueForKey("leader")->getCString();
    if(dict->objectForKey("fightpower")){
        fightpower= (unsigned long)(dict->valueForKey("fightpower")->doubleValue());
    }
    nums= dict->valueForKey("nums")->intValue();
    if(dict->objectForKey("armyKill")){
        armyKill= (unsigned long)(dict->valueForKey("armyKill")->doubleValue());
    }
    
}