//
//  AllianceDonateInfo.cpp
//  IF
//
//  Created by lifangkai on 14-9-2.
//
//

#include "AllianceDonateInfo.h"
#include "GlobalData.h"

AllianceDonateInfo* AllianceDonateInfo::create(){
    AllianceDonateInfo* ret = new AllianceDonateInfo();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
void AllianceDonateInfo::update(CCDictionary* dict,double time){
    name = dict->valueForKey("name")->getCString();
    uid = dict->valueForKey("uid")->getCString();
    if (dict->objectForKey("donation")) {
        donation= dict->valueForKey("donation")->intValue();
    } else {
        donation = 0;
    }
    x= dict->valueForKey("x")->intValue();
    y= dict->valueForKey("y")->intValue();
    if (dict->objectForKey("alliance_honor")) {
        alliance_honor = dict->valueForKey("alliance_honor")->intValue();
    } else {
        alliance_honor = 0;
    }
    pic =dict->valueForKey("pic")->getCString();
    rank= dict->valueForKey("rank")->intValue();
    picVer = dict->valueForKey("picVer")->intValue();
    if (time>0) {
        time = GlobalData::shared()->changeTime(time);
    }
    refreshTime = time;

}