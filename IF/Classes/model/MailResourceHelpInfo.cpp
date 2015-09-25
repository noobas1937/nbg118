//
//  MailResourceHelpInfo.cpp
//  IF
//
//  Created by lifangkai on 14-11-26.
//
//

#include "MailResourceHelpInfo.h"
MailResourceHelpInfo* MailResourceHelpInfo::create(){
    MailResourceHelpInfo* pret = new MailResourceHelpInfo();
    if(pret){
        pret->init();
        pret->autorelease();
    }else{
        CC_SAFE_DELETE(pret);
    }
    return pret;
}
//add one dialog
void MailResourceHelpInfo::parse(CCDictionary *dic){
    uid = dic->valueForKey("uid")->getCString();
    fromName = dic->valueForKey("fromName")->getCString();
    pic = dic->valueForKey("pic")->getCString();
    alliance = dic->valueForKey("alliance")->getCString();
    traderAlliance = dic->valueForKey("traderAlliance")->getCString();
    level = dic->valueForKey("level")->intValue();
    pointId = dic->valueForKey("pointId")->intValue();
    createTime =  dic->valueForKey("createTime")->doubleValue()/1000;
    if(dic->objectForKey("reward")){
        reward = CCArray::create();
        reward->retain();
        CCArray *arr = dynamic_cast<CCArray*>(dic->objectForKey("reward"));
        CCObject *ccObj = NULL;
        CCARRAY_FOREACH(arr, ccObj){
            CCDictionary *dict = dynamic_cast<CCDictionary*>(ccObj);
            if(!dict)
                continue;
            int value = dict->valueForKey("v")->intValue();
            if(value == 0 || value <0 ){
                continue;
            }
            reward->addObject(dict);
        }
    }
}

MailResourceHelpInfo::~MailResourceHelpInfo(){
    
}