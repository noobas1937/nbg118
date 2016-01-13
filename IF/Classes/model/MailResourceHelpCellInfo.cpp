//
//  MailResourceHelpCellInfo.cpp
//  IF
//
//  Created by lifangkai on 14-11-26.
//
//

#include "MailResourceHelpCellInfo.h"
#include "MailResourceHelpInfo.h"
MailResourceHelpCellInfo* MailResourceHelpCellInfo::create(){
    MailResourceHelpCellInfo* ret = new MailResourceHelpCellInfo();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MailResourceHelpCellInfo::parse(cocos2d::CCDictionary *dic){
    
    MailInfo::parse(dic);
    if(collect==NULL){
        collect=CCArray::create();
        collect->retain();
    }
    unread = dic->valueForKey("unread")->intValue();
    totalNum = dic->valueForKey("totalNum")->intValue();
    if(dic->objectForKey("collect")){
        CCArray *arr = dynamic_cast<CCArray*>(dic->objectForKey("collect"));
        CCObject* obj;

        CCARRAY_FOREACH(arr, obj){
            CCDictionary* dictDialog = dynamic_cast<CCDictionary*>(obj);
            MailResourceHelpInfo* dialogInfo = MailResourceHelpInfo::create();
            dialogInfo->parse(dictDialog);
            this->collect->addObject(dialogInfo);
        }
    }
}

MailResourceHelpCellInfo::~MailResourceHelpCellInfo(){
    
    if(collect){
        collect->removeAllObjects();
        collect->release();
        collect = NULL;
    }
    
}