//
//  MailResourceInfo.cpp
//  IF
//
//  Created by lifangkai on 14-10-9.
//
//

#include "MailResourceInfo.h"

MailResourceInfo* MailResourceInfo::create(){
    MailResourceInfo* pret = new MailResourceInfo();
    if(pret){
        pret->init();
        pret->autorelease();
    }else{
        CC_SAFE_DELETE(pret);
    }
    return pret;
}
//add one dialog
void MailResourceInfo::parse(CCDictionary *dic){
    uid = dic->valueForKey("uid")->getCString();
    level = dic->valueForKey("level")->intValue();
    pointId = dic->valueForKey("pointId")->intValue();
    createTime =  dic->valueForKey("createTime")->doubleValue()/1000;
    if(dic->objectForKey("reward")){
        reward = dynamic_cast<CCArray*>(dic->objectForKey("reward"));
        if(reward){
            reward->retain();
        }
    }
    if(dic->objectForKey("drop")){
        drop = dynamic_cast<CCArray*>(dic->objectForKey("drop"));
        if(drop){
            drop->retain();
        }
    }
}

MailResourceInfo::~MailResourceInfo(){
    
}