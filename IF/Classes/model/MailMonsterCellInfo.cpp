//
//  MailMonsterCellInfo.cpp
//  IF
//
//  Created by lifangkai on 14-11-3.
//
//

#include "MailMonsterCellInfo.h"
#include "MailMonsterInfo.h"

MailMonsterCellInfo* MailMonsterCellInfo::create(){
    MailMonsterCellInfo* ret = new MailMonsterCellInfo();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MailMonsterCellInfo::parse(cocos2d::CCDictionary *dic){
    
    MailInfo::parse(dic);
    if(monster==NULL){
        monster=CCArray::create();
        monster->retain();
    }
    unread = dic->valueForKey("unread")->intValue();
    totalNum = dic->valueForKey("totalNum")->intValue();
    if(dic->objectForKey("monster")){
        CCArray *arr = dynamic_cast<CCArray*>(dic->objectForKey("monster"));
        CCObject* obj;
        int i = 0;
        CCARRAY_FOREACH(arr, obj){
            CCDictionary* dictDialog = dynamic_cast<CCDictionary*>(obj);
            MailMonsterInfo* dialogInfo = MailMonsterInfo::create();
            dialogInfo->parse(dictDialog);
            
            this->monster->addObject(dialogInfo);
            if(i==0){
                exp =dialogInfo->exp;
            }
            i++;
        }
    }
}

MailMonsterCellInfo::~MailMonsterCellInfo(){
    
    if(monster){
        monster->removeAllObjects();
        monster->release();
        monster = NULL;
    }
    
}