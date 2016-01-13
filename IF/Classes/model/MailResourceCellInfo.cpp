//
//  MailResourceCellInfo.cpp
//  IF
//
//  Created by lifangkai on 14-10-9.
//
//

#include "MailResourceCellInfo.h"
#include "MailResourceInfo.h"

MailResourceCellInfo* MailResourceCellInfo::create(){
    MailResourceCellInfo* ret = new MailResourceCellInfo();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailResourceCellInfo::init(){
    collect = CCArray::create();
    collect->retain();
    return true;
}

void MailResourceCellInfo::parse(cocos2d::CCDictionary *dic){

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
        int i = 0;
        CCARRAY_FOREACH(arr, obj){
            CCDictionary* dictDialog = dynamic_cast<CCDictionary*>(obj);
            MailResourceInfo* dialogInfo = MailResourceInfo::create();
            dialogInfo->parse(dictDialog);

            this->collect->addObject(dialogInfo);
            if(i==0){
                if (dialogInfo->reward==NULL) {
                    resourceResult = false;
                }
                else{
                    resourceResult = true;
                    CCDictionary *dict = dynamic_cast<CCDictionary*>(dialogInfo->reward->objectAtIndex(i));
                    resourceType = dict->valueForKey("t")->intValue();
                    resourceValue = dict->valueForKey("v")->intValue();
                }
            }
            i++;
        }
    }
}

MailResourceCellInfo::~MailResourceCellInfo(){

    if(collect){
        collect->removeAllObjects();
        collect->release();
        collect = NULL;
    }

}