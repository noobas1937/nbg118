//
//  MailHeiqishiCellInfo.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/10/28.
//
//

#include "MailHeiqishiCellInfo.hpp"
#include "MailController.h"
#include "MailHeiqishiInfo.hpp"
MailHeiqishiCellInfo* MailHeiqishiCellInfo::create(){
    MailHeiqishiCellInfo* ret = new MailHeiqishiCellInfo();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MailHeiqishiCellInfo::parse(cocos2d::CCDictionary *dic){
    MailInfo::parse(dic);
    if(knight==NULL){
        knight=CCArray::create();
        knight->retain();
    }
    unread = dic->valueForKey("unread")->intValue();
    CCLOGFUNCF("knight unread:%d",unread);
    totalNum = dic->valueForKey("totalNum")->intValue();
    CCLOGFUNCF("knight totalNum:%d",totalNum);
    if(dic->objectForKey("knight")){
        CCArray *arr = dynamic_cast<CCArray*>(dic->objectForKey("knight"));
        CCLOGFUNCF("knight arr size:%d",arr->count());
        CCObject* obj;
        int i = 0;
        CCARRAY_FOREACH(arr, obj){
            CCDictionary* dictDialog = dynamic_cast<CCDictionary*>(obj);
            MailHeiqishiInfo* dialogInfo = MailHeiqishiInfo::create();
            if(dictDialog)
            {
                CCArray* dfWarEffectArr=dynamic_cast<CCArray*>(dictDialog->objectForKey("dfWarEffect"));
                CCArray* warEffArr=MailController::getInstance()->parseWarEffect(dfWarEffectArr);
                CCLOGFUNCF("dfWarEffectArr size :%d",warEffArr->count());
                dictDialog->setObject(warEffArr, "dfWarEffect");
                
                CCArray* atkWarEffectArr=dynamic_cast<CCArray*>(dictDialog->objectForKey("atkWarEffect"));
                CCArray* atkEffArr=MailController::getInstance()->parseWarEffect(atkWarEffectArr);
                CCLOGFUNCF("atkWarEffectArr size :%d",atkEffArr->count());
                dictDialog->setObject(atkEffArr, "atkWarEffect");
                
                MailController::getInstance()->parseBattleContentAbility(dynamic_cast<CCArray*>(dictDialog->objectForKey("defGen")));
                MailController::getInstance()->parseBattleContentAbility(dynamic_cast<CCArray*>(dictDialog->objectForKey("atkGen")));
                dialogInfo->parse(dictDialog);
            }
            this->knight->addObject(dialogInfo);
        }
    }
}

MailHeiqishiCellInfo::~MailHeiqishiCellInfo(){
    if(knight){
        knight->removeAllObjects();
        knight->release();
        knight = NULL;
    }
    
}