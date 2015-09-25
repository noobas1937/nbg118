//
//  TalkNoticeCell.cpp
//  IF
//
//  Created by fubin on 14-9-4.
//
//

#include "TalkNoticeCell.h"
#include "SceneController.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "QueueController.h"
#include "ToolController.h"
#include "WorldController.h"
#include "ImperialScene.h"
#include "ActivityEventObj.h"
#include "ActivityController.h"

using namespace cocos2d;

TalkNoticeCell* TalkNoticeCell::create(int type)
{
    TalkNoticeCell *pRet = new TalkNoticeCell();
    if (pRet && pRet->initTalkNoticeCell(type))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool TalkNoticeCell::initTalkNoticeCell(int type)
{
    m_type = type;
    m_count = 0;
    if (type == 0) {
//        m_dialogs.push_back("105931");
//        m_dialogs.push_back("105932");
//        m_dialogs.push_back("105933");
//        m_dialogs.push_back("105934");
//        m_dialogs.push_back("105935");
        
        m_dialogs.push_back("105829");
        m_dialogs.push_back("105830");
        m_dialogs.push_back("105831");
        m_dialogs.push_back("105832");
        m_dialogs.push_back("105833");
    }else if(type == 1){
        m_dialogs.push_back("138040");
    }
    else {
        return false;
    }
    CCBLoadFile("TalkNoticeCell",this,this);
    return true;
}

void TalkNoticeCell::onEnter() {
    CCNode::onEnter();
    if (m_type==1) {
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TalkNoticeCell::onShow), MSG_TALK_CROSS_SERVER_ACTIVITY, NULL);
    }else{
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TalkNoticeCell::onShow), MSG_TALK_ACT, NULL);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TalkNoticeCell::onRemove), MSG_TALK_REMOVE, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(TalkNoticeCell::onEnterFrame), this, 0,kCCRepeatForever, 0.0f, false);
}

void TalkNoticeCell::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TALK_ACT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TALK_REMOVE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TALK_CROSS_SERVER_ACTIVITY);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(TalkNoticeCell::onEnterFrame), this);
    CCNode::onExit();
}

bool TalkNoticeCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_touchNode && isTouchInside(m_touchNode, pTouch)) {
        return true;
    }
    return false;
}

void TalkNoticeCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    onClickThis();
}

void TalkNoticeCell::onClickThis()
{
    if (m_type==1) {
        ActivityController::getInstance()->openCrossServerActivity();
    }else {
        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        if(layer) {
            layer->onMoveToPos(300, 1430,TYPE_POS_CLICK,0.3,1,true);
        }
    }
}

void TalkNoticeCell::onRemove(CCObject* obj)
{
    CCNode* TmpTarget = dynamic_cast<CCNode*>(obj);
    if (m_target && TmpTarget && m_target==TmpTarget) {
        onHide(0.0);
    }
}

void TalkNoticeCell::onShow(CCObject* obj)
{
    if (this->isVisible()){
        return;
    }
    if (m_type==1 && m_dIdx>=m_dialogs.size()) {
        m_dIdx = 0;
    }else if (m_dIdx>=m_dialogs.size()) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PERSON_STOP_TALK);
        return;
    }
    
    if (obj) {
        m_target = dynamic_cast<CCNode*>(obj);
        if (m_target) {
            this->setPosition(m_target->getPosition()+ccp(0,90));
//            if (m_dIdx>=m_dialogs.size()) {
//                m_dIdx = 0;
//            }
            
            this->setVisible(true);
            string msgStr = _lang(m_dialogs[m_dIdx]);
            if (m_type==1) {
                unschedule(schedule_selector(TalkNoticeCell::onHide));
                schedule(schedule_selector(TalkNoticeCell::onHide), 5);
                if (ActivityController::getInstance()->activityArr && ActivityController::getInstance()->activityArr->count()>0) {
                    int num = ActivityController::getInstance()->activityArr->count();
                    for (int i=0; i<num; i++) {
                        auto temp = dynamic_cast<ActivityEventObj*>(ActivityController::getInstance()->activityArr->objectAtIndex(i));
                        double nowTime = GlobalData::shared()->getWorldTime();
                        if (temp && temp->type==7 && temp->startTime>nowTime) {
                            double leftTime = temp->startTime - nowTime;
                            msgStr = _lang_1("105804", CC_SECTOA(leftTime));
                            break;
                        }
                    }
                }
            }else{
                this->scheduleOnce(schedule_selector(TalkNoticeCell::onHide), 5);
            }
            m_msgLabel->setString(msgStr);
            float setbgH = m_msgLabel->getContentSize().height*0.75+10>60?m_msgLabel->getContentSize().height*0.75+10:60;
            m_bg9Spr->setContentSize(CCSize(240, setbgH+20));
            m_msgLabel->setPositionY(m_bg9Spr->getPositionY()+m_bg9Spr->getContentSize().height/2);
            m_dIdx++;
        }
    }
}

void TalkNoticeCell::onHide(float _time)
{
    m_count += 1;
    if (m_type==1) {
        this->setVisible(m_count%2==0);
        if (this->isVisible()) {
            string msgStr = _lang(m_dialogs[0]);
            if (ActivityController::getInstance()->activityArr && ActivityController::getInstance()->activityArr->count()>0) {
                int num = ActivityController::getInstance()->activityArr->count();
                for (int i=0; i<num; i++) {
                    auto temp = dynamic_cast<ActivityEventObj*>(ActivityController::getInstance()->activityArr->objectAtIndex(i));
                    double nowTime = GlobalData::shared()->getWorldTime();
                    if (temp && temp->type==7 && temp->startTime>nowTime) {
                        double leftTime = temp->startTime - nowTime;
                        msgStr = _lang_1("105804", CC_SECTOA(leftTime));
                        break;
                    }
                }
            }
            m_msgLabel->setString(msgStr);
            float setbgH = m_msgLabel->getContentSize().height*0.75+10>60?m_msgLabel->getContentSize().height*0.75+10:60;
            m_bg9Spr->setContentSize(CCSize(240, setbgH+20));
            m_msgLabel->setPositionY(m_bg9Spr->getPositionY()+m_bg9Spr->getContentSize().height/2);
        }
    }else{
        this->setVisible(false);
        m_target = NULL;
    }
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler TalkNoticeCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler TalkNoticeCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool TalkNoticeCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg9Spr", CCScale9Sprite*, this->m_bg9Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    return false;
}

void TalkNoticeCell::onEnterFrame(float dt)
{
    if (this->isVisible() && m_target) {
        this->setPosition(m_target->getPosition()+ccp(40,50));
    }
}