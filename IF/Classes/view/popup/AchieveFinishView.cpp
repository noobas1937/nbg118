//
//  AchieveFinishView.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-25.
//
//

#include "AchieveFinishView.h"
#include "UIComponent.h"
#include "SceneController.h"
#include "GuideController.h"

AchieveFinishView *AchieveFinishView::create(std::string id){
    AchieveFinishView *ret = new AchieveFinishView(id);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AchieveFinishView::onEnter(){
    PopupBaseView::onEnter();
    UIComponent::getInstance()->showPopupView(UIPopupViewType_2);
    UIComponent::getInstance()->hideReturnBtn();
//    SceneController::getInstance()->addGUI();
}

void AchieveFinishView::onExit(){
    PopupBaseView::onExit();
}

bool AchieveFinishView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        m_ccbNode = CCBLoadFile("AchieveFinishView", this, this);
        this->setContentSize(m_ccbNode->getContentSize());
        
        CCCommonUtils::setButtonTitle(m_btnOk, _lang("107516").c_str());
        m_titleText->setString(_lang("107502"));
        m_descTxt->setString(_lang("107504"));
        map<std::string, QuestInfo*>::iterator it = QuestController::getInstance()->achList.find(m_id);
        if(it!=QuestController::getInstance()->achList.end()){
            m_nameTxt->setString(_lang_1("107503", it->second->name.c_str()));
            it->second->haveOpen = true;
        }
        ret = true;
    }
    return ret;
}

bool AchieveFinishView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOk", CCControlButton*, this->m_btnOk);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCancel", CCControlButton*, this->m_btnCancel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descTxt", CCLabelIF*, this->m_descTxt);
    
    return false;
}

SEL_CCControlHandler AchieveFinishView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keypressedBtnOk", AchieveFinishView::keypressedBtnOk);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnCancel", AchieveFinishView::keypressedBtnCancel);
    return NULL;
}

void AchieveFinishView::closehandle2(){
    std::string guideId = "";
    map<std::string, QuestInfo*>::iterator it = QuestController::getInstance()->achList.find(m_id);
    if(it!=QuestController::getInstance()->achList.end()){
        guideId = it->second->guide;
    }
    this->closeSelf();
    if(guideId!=""){
        GuideController::share()->setGuide(guideId);
    }
}

void AchieveFinishView::closehandle(){
    m_ccbNode->setVisible(false);
    auto node = UIComponent::getInstance()->getNodeByIndex("UI_target2");
    if(node){
        CCPoint p = node->getPosition();
        CCPoint p2 = node->getParent()->convertToWorldSpace(p);
        CCPoint localP = this->convertToNodeSpaceAR(p2);
        CCSequence* sc = CCSequence::create(CCMoveTo::create(0.5, ccp(localP.x-140, localP.y+20)),CCCallFunc::create(this, callfunc_selector(AchieveFinishView::closehandle2)),NULL);
        CCSequence* sc2 = CCSequence::create(CCScaleTo::create(0.4, 0.1),NULL);
        CCSpawn* sp = CCSpawn::create(sc,sc2,NULL);
       // m_achIcon->runAction(sp);
    }else{
        closehandle2();
    }
}

void AchieveFinishView::keypressedBtnOk(CCObject * pSender, Control::EventType pCCControlEvent) {
    QuestController::getInstance()->getReward(m_id);
    this->closehandle2();
}

void AchieveFinishView::keypressedBtnCancel(CCObject * pSender, Control::EventType pCCControlEvent) {
    this->closehandle2();
}

