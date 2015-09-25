//
//  PushInit.cpp
//  IF
//
//  Created by 邹 程 on 13-9-13.
//
//

#include "PushInit.h"
#include "CommonInclude.h"
#include "YesNoDialog.h"
#include "CCThreadManager.h"

void PushInit::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;

    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_LoginInit);

    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (params && !GlobalData::shared()->isInitFlag) {
        DataParse *parse = new DataParse();
        parse->parseObject(params);
    }
}

void DataParse::parseObject(cocos2d::CCDictionary *dict){
    m_dict = dict;
    m_dict->retain();
    startIndex = CCCommonUtils::InitStart;
    CCCommonUtils::resetData(dict);
    
    update(0.0f);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DataParse::killSelf), "xiaomiChangeAccount", NULL);
}

void DataParse::update(float time){
    if(startIndex > CCCommonUtils::InitEnd){
        killSelf();
    }else{
        CCCommonUtils::initOperation(startIndex, m_dict);
        startIndex++;
    }
}

void DataParse::killSelf(CCObject *obj){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    m_dict->release();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "xiaomiChangeAccount");
    this->release();
}

void PushUserOff::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDirector::sharedDirector()->pause();

    YesNoDialog::showVariableTitle(_lang("E100083").c_str(),CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame), NULL),_lang("confirm").c_str(),true);
}

void PushUserKick::handleResponse(cocos2d::CCDictionary *dict){
    if (!dict)
        return;
    cocos2d::CCDirector::sharedDirector()->pause();
    
    YesNoDialog::showVariableTitle(_lang("E100087").c_str(),CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame), NULL),_lang("confirm").c_str(),true);
}

void PushUserGmOff::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict) return;
    cocos2d::CCDirector::sharedDirector()->pause();
    
    YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame), NULL),_lang("105248").c_str(),true);
}

void PushInitError::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_LoginInitError);
}