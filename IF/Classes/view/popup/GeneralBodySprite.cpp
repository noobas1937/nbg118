//
//  GeneralBodySprite.cpp
//  IF
//
//  Created by ganxiaohua on 14-5-8.
//
//

#include "GeneralBodySprite.h"
#include "GeneralManager.h"
#include "CCGraySprite.h"

GeneralBodySprite* GeneralBodySprite::create(std::string id,int h,bool gray){
    GeneralBodySprite* ret = new GeneralBodySprite(id,h,gray);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GeneralBodySprite::init(){
    if (!CCNode::init()) {
        return false;
    }
    this->scheduleOnce(schedule_selector(GeneralBodySprite::delayFunc), 0.1);
    return true;
}

void GeneralBodySprite::delayFunc(float t){
    int resIndx = GeneralManager::getInstance()->getBustPlistIndex(m_id);
    CCLoadSprite::doResourceByGeneralIndex(resIndx, true);
    CCNode* head = NULL;
    if(m_gray){
        head =  CCGraySprite::createWithSpriteFrameName(CCCommonUtils::getGeneralBustPicById(m_id).c_str());//
    }else{
        head = CCLoadSprite::createSprite(CCCommonUtils::getGeneralBustPicById(m_id).c_str());
    }
    if(head){
        if(m_height>0){
            head->setScale((m_height - 10.0) / head->getContentSize().height);
        }
        this->addChild(head);
    }
    setCleanFunction([resIndx](){
        CCLoadSprite::doResourceByGeneralIndex(resIndx, false);
    });
}

void GeneralBodySprite::onEnter(){
    CCNode::onEnter();
}

void GeneralBodySprite::onExit(){
    
    CCNode::onExit();
}