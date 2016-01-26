//
//  ActivityTipView.cpp
//  IF
//
//  Created by ganxiaohua on 14-9-2.
//
//

#include "ActivityTipView.h"
#include "ActivityBeginView.h"
#include "ParticleFireAni.h"
#include "ActivityController.h"
#include "RewardController.h"

ActivityTipView *ActivityTipView::create(ActivityEventObj* info){
    ActivityTipView *ret = new ActivityTipView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ActivityTipView::onEnter(){
    PopupBaseView::onEnter();
    this->setTitleName(_lang("105807"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
   // //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    this->schedule(schedule_selector(ActivityTipView::updateTime), 1);
    this->updateTime(0);
}

void ActivityTipView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool ActivityTipView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void ActivityTipView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_clickArea, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool ActivityTipView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(11, true);
        //TODO setCleanFunction
//        setCleanFunction([](){
//            CCLoadSprite::doResourceByCommonIndex(11, false);
//        });
        this->setModelLayerOpacity(160);
        auto node = CCBLoadFile("ActivityReadyView", this, this);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winSize);
//        m_txt1->setString(_lang("105807"));
//        m_txt2->setString(_lang("105809"));
        
        m_txt1->setString(m_info->name);
        m_txt2->setString(m_info->desc);
        
        m_txt4->setString(_lang("105847"));
        m_txt5->setString(_lang("105848"));
        
//        ParticleFireAni* par = ParticleFireAni::create();
//        m_fireNode1->addChild(par);
//        
//        ParticleFireAni* par2 = ParticleFireAni::create();
//        m_fireNode2->addChild(par2);
        this->initReward();
        ActivityController::getInstance()->activityRecord(1);
        ret = true;
    }
    return ret;
}


void ActivityTipView::initReward(){
    if(m_info->getReward()==NULL || m_info->getReward()->count()<=0) return ;
    
    
    int TOOL_BG[] = {PURPLE, ORANGE, BLUE, BLUE};
    
    string name = "";
    string icon = "";
    CCDictionary* reward = NULL;
    if(m_info->getReward()->count()>0){
        reward = dynamic_cast<CCDictionary*>(m_info->getReward()->objectAtIndex(0));
        if(reward){
            int type = reward->valueForKey("type")->intValue();
            int value = reward->valueForKey("value")->intValue();
            if(type==R_GOODS){
                auto valueObj = _dict(reward->objectForKey("value"));
                int id = valueObj->valueForKey("id")->intValue();
                name = RewardController::getInstance()->getNameByType(type,id);
                icon = RewardController::getInstance()->getPicByType(type,id);
                value = valueObj->valueForKey("num")->intValue();
            }else{
                name = RewardController::getInstance()->getNameByType(type,value);
                icon = RewardController::getInstance()->getPicByType(type,value);
            }
            m_numTxt1->setString(CC_ITOA(value));
            m_rewardTxt1->setString(name);
            CCSprite* spr = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(TOOL_BG[0]).c_str());
            CCCommonUtils::setSpriteMaxSize(spr,  80);
            m_rewardIcon1->addChild(spr);
            spr = CCLoadSprite::createSprite(icon.c_str());
            CCCommonUtils::setSpriteMaxSize(spr,  84,true);
            m_rewardIcon1->addChild(spr);
        }
    }
    
    if(m_info->getReward()->count()>1){
        reward = dynamic_cast<CCDictionary*>(m_info->getReward()->objectAtIndex(1));
        if(reward){
            int type = reward->valueForKey("type")->intValue();
            int value = reward->valueForKey("value")->intValue();
            if(type==R_GOODS){
                auto valueObj = _dict(reward->objectForKey("value"));
                int id = valueObj->valueForKey("id")->intValue();
                name = RewardController::getInstance()->getNameByType(type,id);
                icon = RewardController::getInstance()->getPicByType(type,id);
                value = valueObj->valueForKey("num")->intValue();
            }else{
                name = RewardController::getInstance()->getNameByType(type,value);
                icon = RewardController::getInstance()->getPicByType(type,value);
            }
            m_rewardTxt2->setString(name);
            m_numTxt2->setString(CC_ITOA(value));
            CCSprite* spr = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(TOOL_BG[1]).c_str());
            CCCommonUtils::setSpriteMaxSize(spr,  80);
            m_rewardIcon2->addChild(spr);
            spr = CCLoadSprite::createSprite(icon.c_str());
            CCCommonUtils::setSpriteMaxSize(spr,  84,true);
            m_rewardIcon2->addChild(spr);
        }
    }
    
    if(m_info->getReward()->count()>2){
        reward = dynamic_cast<CCDictionary*>(m_info->getReward()->objectAtIndex(2));
        if(reward){
            int type = reward->valueForKey("type")->intValue();
            int value = reward->valueForKey("value")->intValue();
            if(type==R_GOODS){
                auto valueObj = _dict(reward->objectForKey("value"));
                int id = valueObj->valueForKey("id")->intValue();
                name = RewardController::getInstance()->getNameByType(type,id);
                icon = RewardController::getInstance()->getPicByType(type,id);
                value = valueObj->valueForKey("num")->intValue();
            }else{
                name = RewardController::getInstance()->getNameByType(type,value);
                icon = RewardController::getInstance()->getPicByType(type,value);
            }
            m_rewardTxt3->setString(name);
            m_numTxt3->setString(CC_ITOA(value));
            CCSprite* spr = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(TOOL_BG[2]).c_str());
            CCCommonUtils::setSpriteMaxSize(spr,  80);
            m_rewardIcon3->addChild(spr);
            spr = CCLoadSprite::createSprite(icon.c_str());
            CCCommonUtils::setSpriteMaxSize(spr,  84,true);
            m_rewardIcon3->addChild(spr);
        }
    }
    
    CCSprite* spr = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(TOOL_BG[3]).c_str());
    CCCommonUtils::setSpriteMaxSize(spr,  80);
    m_rewardIcon4->addChild(spr);
    spr = CCLoadSprite::createSprite("MonsterAttack_3.png");
    CCCommonUtils::setSpriteMaxSize(spr,  84);
    m_rewardIcon4->addChild(spr);
    m_rewardTxt4->setString(_lang("105849"));
}

void ActivityTipView::updateTime(float _time){
    double tempTime = m_info->startTime - GlobalData::shared()->getWorldTime();
    double endTime = m_info->endTime - GlobalData::shared()->getWorldTime();
    if (tempTime>0) {
        m_txt3->setString(_lang_1("105804",CC_SECTOA(tempTime)));
    }else if(endTime>0){
        m_txt3->setString(_lang_1("105805",CC_SECTOA(endTime)));
    }
    if(m_info->startTime==0 && m_info->endTime==0){
        m_txt3->setString(_lang_1("105800",CC_SECTOA(endTime)));
    }
}

void ActivityTipView::onCloseClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

bool ActivityTipView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt4", CCLabelIF*, this->m_txt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt5", CCLabelIF*, this->m_txt5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode1", CCNode*, this->m_fireNode1);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardIcon1", CCNode*, this->m_rewardIcon1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardTxt1", CCLabelIF*, this->m_rewardTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardIcon2", CCNode*, this->m_rewardIcon2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardTxt2", CCLabelIF*, this->m_rewardTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardIcon3", CCNode*, this->m_rewardIcon3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardTxt3", CCLabelIF*, this->m_rewardTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardIcon4", CCNode*, this->m_rewardIcon4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardTxt4", CCLabelIF*, this->m_rewardTxt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt1", CCLabelIF*, this->m_numTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt2", CCLabelIF*, this->m_numTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt3", CCLabelIF*, this->m_numTxt3);
    return false;
}

SEL_CCControlHandler ActivityTipView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", ActivityTipView::onCloseClick);
    return NULL;
}