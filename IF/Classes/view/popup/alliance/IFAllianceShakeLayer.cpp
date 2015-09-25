//
//  IFAllianceShakeLayer.cpp
//  IF
//
//  Created by wangdianzhen on 15/8/26.
//
//

#include "IFAllianceShakeLayer.h"
#include "ShakeController.h"
#include "CCSafeNotificationCenter.h"
#include "PopupViewController.h"
#include "AllianceManager.h"
#include "GetAllianceListCommand.h"
#include "ApplyAllianceCommand.h"
#include "YesNoDialog.h"
#include "AllianceFlagPar.h"
#include "ChatController.h"
#include "AllianceInfoView.h"
#include "IFSkeletonAnimation.h"
#include "ShakeController.h"
#define MAX_TIPS_COUNT 8
const char* tip[MAX_TIPS_COUNT] = {"102386","102387","102388","102389","102390","102391","102392","102393"};
IFAllianceShakeLayer::IFAllianceShakeLayer()
{

}
IFAllianceShakeLayer::~IFAllianceShakeLayer()
{
    
}

IFAllianceShakeLayer* IFAllianceShakeLayer::create(){
    IFAllianceShakeLayer *ret = new IFAllianceShakeLayer();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
    
}
void IFAllianceShakeLayer::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
}
void IFAllianceShakeLayer::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool IFAllianceShakeLayer::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leaderTxt", CCLabelIFTTF*, this->m_leaderTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceNumTxt", CCLabelIFTTF*, this->m_allianceNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_language", CCLabelIFTTF*, this->m_language);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_forceTxt", CCLabelIFTTF*, this->m_forceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg", CCLabelIFTTF*, this->m_msg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coinTxt", CCLabelIFTTF*, this->m_coinTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceIcon", CCNode*, this->m_allianceIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewNode", CCNode*, this->m_viewNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewNode2", CCNode*, this->m_viewNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shakebtn", CCControlButton*, this->m_shakebtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgNode", CCNode*, this->m_msgNode);
    
    
    return true;
}

SEL_CCControlHandler IFAllianceShakeLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickOKBtn", IFAllianceShakeLayer::onClickOKBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCloseBtn", IFAllianceShakeLayer::onClickCloseBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickShakeBtn", IFAllianceShakeLayer::onClickShakeBtn);
    
    return NULL;
}
bool IFAllianceShakeLayer::init(){
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(205, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(205, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
        });
        setIsHDPanel(true);
        auto bg = CCBLoadFile("AllianceRecommendView", this, this);
        this->setContentSize(bg->getContentSize());
        auto animationObj = new IFSkeletonAnimation("Spine/Imperial/AllianceRecommendAni.json","Common/Common_511.atlas");
        if (animationObj) {
            m_bg->addChild(animationObj);
            animationObj->setPositionY(m_bg->getContentSize().height);
            spTrackEntry* entry = animationObj->setAnimation(0, "AllianceRecommendAni", false);
        }


        CCRotateBy * roaction = CCRotateBy::create(0.1f,60);
        CCRepeat * r1 = CCRepeat::create(CCSequence::create(roaction,roaction->reverse(),nullptr),4);
        CCDelayTime * d = CCDelayTime::create(5.0f);
        m_shakebtn->runAction(CCRepeatForever::create(CCSequence::create(r1,d,nullptr)));
        initLayerInfo();
        return true;
    }
    return false;
}





bool  IFAllianceShakeLayer::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}
void  IFAllianceShakeLayer::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_bg && isTouchInside(m_bg,pTouch)) {
        return;
    }
    else
    {
        PopupViewController::getInstance()->removePopupView(this);
    }
}



void IFAllianceShakeLayer::nodeAni()
{
    m_allianceIcon->setPositionY(558+200);
    CCMoveTo* run1 = CCMoveTo::create(0.2f, ccp(m_allianceIcon->getPositionX(),548));
    CCMoveTo* run2 = CCMoveTo::create(0.15f, ccp(m_allianceIcon->getPositionX(),563));
    CCMoveTo* run3 =CCMoveTo::create(0.15f, ccp(m_allianceIcon->getPositionX(),558));
    m_allianceIcon->runAction(CCSequence::create(run1, run2, run3, NULL));
}


void IFAllianceShakeLayer::onClickOKBtn(CCObject *pSender, CCControlEvent event)
{
    
    ApplyAllianceCommand* cmd = new ApplyAllianceCommand(m_allianceInfo->uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(IFAllianceShakeLayer::success), NULL));
    cmd->sendAndRelease();
}
void IFAllianceShakeLayer::success(CCObject* p){
  
    if(m_allianceInfo->recruit>0)
    {
        if(GlobalData::shared()->playerInfo.isInAlliance()){
            PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
        }
        ChatController::getInstance()->sendCountryChat(_lang("115188").append("  (").append(_lang("115181")).append(")").c_str(), CHAT_ALLIANCE, 2);
        PopupViewController::getInstance()->removePopupView(this);
    }
    else{
        YesNoDialog::showVariableTitle(_lang_1("115023",GlobalData::shared()->playerInfo.allianceInfo.name.c_str()),NULL,_lang("115022").c_str());
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void IFAllianceShakeLayer::onClickShakeBtn(CCObject *pSender, CCControlEvent event)
{
    
}
void IFAllianceShakeLayer::onClickCloseBtn(CCObject *pSender, CCControlEvent event)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void IFAllianceShakeLayer::refesh(){
    m_msgNode->setVisible(false);
    m_allianceIcon->setVisible(false);
    initLayerInfo();
    CCPoint defPos1 = CCPoint(0, 0);
    CCPoint defPos2 = CCPoint(210, 558);
    m_msgNode->setPositionX(m_msgNode->getPositionX()+ 250);
    m_allianceIcon->setPositionY(m_allianceIcon->getPositionY()+250);
    
    CCMoveTo * moveTo1 = CCMoveTo::create(0.3, ccpSub(defPos1, ccp(60, 0)));
    CCMoveTo * moveBack1 = CCMoveTo::create(0.1, defPos1);
    
    CCMoveTo * moveTo2 = CCMoveTo::create(0.3, ccpSub(defPos2, ccp(0, 60)));
    CCMoveTo * moveBack2 = CCMoveTo::create(0.1, defPos2);
    m_msgNode->setVisible(true);
    m_allianceIcon->setVisible(true);
    m_msgNode->runAction(CCSequence::create(moveTo1,moveBack1,NULL));
    m_allianceIcon->runAction(CCSequence::create(moveTo2,moveBack2,NULL));
    
}
void IFAllianceShakeLayer::initLayerInfo(){
    if (ShakeController::getInstance()->m_shakeAllianceVec.size() == 0) {
        return;
    }
    AllianceInfo*  m_info = ShakeController::getInstance()->m_shakeAllianceVec.back();
    ShakeController::getInstance()->m_shakeAllianceVec.popBack();
    m_allianceInfo = m_info;
    std::string str = "(";
    str.append(m_info->shortName.c_str());
    str.append(") ");
    if (m_info->shortName == "") {
        str = "";
    }
    str.append(m_info->name.c_str());
    if(m_nameTxt){
        if(str.length()>18){
            str = CCCommonUtils::subStrByUtf8(str,0,17);
            str.append("...");
        }
        m_nameTxt->setString(str.c_str());
    }
    if (m_info->leader != "") {
        str = _lang_1("115015",m_info->leader.c_str());
    }
    
    if(m_leaderTxt){
        m_leaderTxt->setString(str.c_str());
    }
    if(m_allianceNumTxt){
        m_allianceNumTxt->setString(CCString::createWithFormat("%d/%d",m_info->currentNum,m_info->maxNum)->getCString());
    }
    str = CC_CMDITOA(m_info->totalForce);
    if(m_forceTxt){
        m_forceTxt->setString(str.c_str());
    }
    if(m_msg)
    {
        int index= rand()%MAX_TIPS_COUNT;
        m_msg->setString(_lang(tip[index]).c_str());
    }
    m_allianceIcon->removeAllChildrenWithCleanup(true);// 210 558
    AllianceFlagPar* flag = AllianceFlagPar::create(m_info->getAllianceIcon().c_str());
    //flag->setScale(105.0/flag->getContentSize().width);
    m_allianceIcon->addChild(flag,0,444);
    m_allianceIcon->setPositionX(m_allianceIcon->getPositionX()+3);
    
    string lang = (strcmp(m_info->language.c_str(), "")==0?"115600":m_info->language );
    if(m_language){
        m_language->setString(_lang(lang).c_str());
    }
    if(m_info->recruit>0){
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("115020").c_str());
    }
    else
    {
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("115019").c_str());
    }
}