//
//  GuidePlotView.cpp
//  IF
//
//  Created by 李锐奇 on 14-4-9.
//
//

#include "GuidePlotView.h"
#include "GuideController.h"

bool GuidePlotView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentText", CCLabelIF*, m_contentText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_colorLayer", CCLayerColor*, m_colorLayer);
    
    return false;
}

GuidePlotView *GuidePlotView::create(std::string did){
    GuidePlotView *ret = new GuidePlotView(did);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GuidePlotView::init(){
    CCBLoadFile("GuidePlotViewCCB", this, this);
    refreshView();
    if (m_did == "3070710") {
        m_bg->setVisible(false);
        
        if (GuideController::share()->IsSkip) {
            m_contentText->setVisible(false);
            
            CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(GuidePlotView::guideToNext));
            CCCallFunc * battleEnd= CCCallFunc::create(this, callfunc_selector(GuidePlotView::guideBattleEnd));
            CCActionInterval * fadeTo1 = CCFadeTo::create(1,255);
            CCActionInterval * delay1Time = CCDelayTime::create(0.5);
            CCActionInterval * fadeTo2 = CCFadeTo::create(0.5,0);
            m_colorLayer->runAction(CCSequence::create(fadeTo1, battleEnd, delay1Time, fadeTo2, funcall, NULL));
            
        }else {
            
            m_contentText->setOpacity(0);
            m_contentText->setScale(2.0);
            CCActionInterval * txtdelay = CCDelayTime::create(1);
            CCActionInterval * fadeIn = CCFadeIn::create(0.5);
            CCActionInterval * delayTime = CCDelayTime::create(2);
            CCCallFunc * battleEnd= CCCallFunc::create(this, callfunc_selector(GuidePlotView::guideBattleEnd));
            CCActionInterval * fadeOut = CCFadeOut::create(0.5);
            CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(GuidePlotView::guideToNext));
            m_contentText->runAction(CCSequence::create(txtdelay,fadeIn, delayTime, battleEnd, fadeOut, funcall, NULL));
            
            CCActionInterval * fadeTo1 = CCFadeTo::create(1,255);
            CCActionInterval * delay1Time = CCDelayTime::create(2.75);
            CCActionInterval * fadeTo2 = CCFadeTo::create(1,0);
            m_colorLayer->runAction(CCSequence::create(fadeTo1, delay1Time, fadeTo2, NULL));
        }
        
        
    }
    else {
        auto size = CCDirector::sharedDirector()->getWinSize();
        this->setPositionY(320 - size.height/2);
    }
    return true;
}

void GuidePlotView::refreshView(){
    this->m_contentText->setString(_lang(m_did).c_str());
}

void GuidePlotView::guideToNext()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("%s", m_did.c_str()));
}

void GuidePlotView::guideBattleEnd()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_BATTLE_END);
}
