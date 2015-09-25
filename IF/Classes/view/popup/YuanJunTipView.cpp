//
//  YuanJunTipView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-11.
//
//

#include "YuanJunTipView.h"

#include "QuestController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "ChangePicView.h"
#include "ToolController.h"
#include "UseToolView.h"
#include "WorldMapView.h"
#include "ChangeNickNameView.h"
#include "BattleView.h"
#include "AllianceReinforceSoldier.h"

YuanJunTipView *YuanJunTipView::create(int index){
    YuanJunTipView *ret = new YuanJunTipView(index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void YuanJunTipView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void YuanJunTipView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "on_pic_click");
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool YuanJunTipView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void YuanJunTipView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_bg, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool YuanJunTipView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(502, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(502, false);
        });
        CCBLoadFile("YuanJunTipView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());

        m_titleText->setString(_lang("115151"));
        m_tipTxt1->setString(_lang("115152"));
        m_tipTxt2->setString(_lang("115153"));
        CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("cancel_btn_label").c_str());
        CCCommonUtils::setButtonTitle(m_sendBtn, _lang("115154").c_str());
        m_current = 0;
        m_maxSoldier = 0;
        m_numNode->setVisible(false);
        m_sendBtn->setEnabled(false);
        
        AllianceReinforceSoldier* cmd = new AllianceReinforceSoldier();
        cmd->putParam("point", CCInteger::create(m_index));
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(YuanJunTipView::getData), NULL));
        cmd->sendAndRelease();
        ret = true;
    }
    return ret;
}

void YuanJunTipView::getData(CCObject *param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){
        m_current = dic->valueForKey("forces")->intValue();
        m_maxSoldier = dic->valueForKey("max")->intValue();
        std::string str = CC_CMDITOA(m_current);
        str.append("/");
        str.append(CC_CMDITOA(m_maxSoldier));
        m_numTxt->setString(str);
        if(m_maxSoldier<=0){
            m_numNode->setVisible(false);
            m_sendBtn->setEnabled(false);
            m_tipTxt1->setString(_lang("115174"));
            m_tipTxt1->setColor(ccRED);
        }else{
            m_numNode->setVisible(true);
        }
        int gapNum = m_maxSoldier - m_current;
        m_sendBtn->setEnabled(gapNum>0);
    }
}

void YuanJunTipView::onSendClick(CCObject *pSender, CCControlEvent event){
    int pointId = m_index;
    PopupViewController::getInstance()->removePopupView(this);
    int gapNum = m_maxSoldier - m_current;
    WorldController::getInstance()->openMarchDeploy(pointId,1,gapNum,MethodYuanSolider);
}

void YuanJunTipView::onCancelClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

bool YuanJunTipView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cancelBtn", CCControlButton*, this->m_cancelBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sendBtn", CCControlButton*, this->m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt1", CCLabelIF*, this->m_tipTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt2", CCLabelIF*, this->m_tipTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numNode", CCNode*, this->m_numNode);
    return false;
}

SEL_CCControlHandler YuanJunTipView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelClick", YuanJunTipView::onCancelClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSendClick", YuanJunTipView::onSendClick);
    return NULL;
}