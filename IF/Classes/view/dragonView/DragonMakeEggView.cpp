//
//  DragonMakeEggView.cpp
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#include "DragonMakeEggView.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "QueueController.h"
#include "GuideController.h"

DragonMakeEggView* DragonMakeEggView::create(){
    DragonMakeEggView* ret = new DragonMakeEggView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonMakeEggView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    auto bg = CCBLoadFile("DragonMakeEggView",this,this);
    setContentSize(bg->getContentSize());
    
    setTitleName(_lang("119008"));
    
    return true;
}

void DragonMakeEggView::onEnter(){
    CCNode::onEnter();
}

void DragonMakeEggView::onExit(){
    CCNode::onExit();
}

SEL_CCControlHandler DragonMakeEggView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOKClick", DragonMakeEggView::onOKClick);
    return NULL;
}

bool DragonMakeEggView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_OKBtn", CCControlButton*, this->m_OKBtn);
    return false;
}

void DragonMakeEggView::onOKClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    CCLOG(" OOOOOOOOOO ok");
}