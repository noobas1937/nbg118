//
//  AgainstInfoView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/27.
//
//

#include "AgainstInfoView.h"
#include "PopupViewController.h"
#include "CommandBase.h"

AgainstInfoView *AgainstInfoView::create(){
    AgainstInfoView *ret = new AgainstInfoView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AgainstInfoView::onEnter(){
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);//Touch_Default
    PopupBaseView::onEnter();
}

void AgainstInfoView::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    PopupBaseView::onExit();
}

bool AgainstInfoView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void AgainstInfoView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30) {
        return ;
    }
    if (!isTouchInside(m_clickArea, pTouch)) {
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool AgainstInfoView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(506, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(506, false);
        });
        auto node = CCBLoadFile("AgainstInfoView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        
        m_infoTxt->setString(_lang("140021"));
        m_timeTitle->setString(_lang("140022"));
        m_timeTxt->setString("00:03:04");
        m_allianceNameTxt->setString("aaa");
        m_leaderTxt->setString("test");
        m_powerTxt->setString(_lang_1("140030",CC_CMDITOAL(3).c_str()));
        m_rankTxt->setString(_lang_1("140023",CC_CMDITOAL(3).c_str()));
        m_kingdomTxt->setString(_lang_1("140023",CC_CMDITOAL(3).c_str()));
        m_memberTxt->setString(_lang_2("140023","1","3"));
        m_winTxt->setString(_lang_1("140023","30%"));
        ret = true;
    }
    return ret;
}


bool AgainstInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoTxt", CCLabelIF*, this->m_infoTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTitle", CCLabelIF*, this->m_timeTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceNameTxt", CCLabelIF*, this->m_allianceNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_leaderTxt", CCLabelIF*, this->m_leaderTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankTxt", CCLabelIF*, this->m_rankTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_kingdomTxt", CCLabelIF*, this->m_kingdomTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_memberTxt", CCLabelIF*, this->m_memberTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_winTxt", CCLabelIF*, this->m_winTxt);
    return false;
}

SEL_CCControlHandler AgainstInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}