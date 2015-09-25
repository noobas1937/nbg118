//
//  CrossServerMoveCityPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-10-17.
//
//

#include "CrossServerMoveCityPopUpView.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
bool CrossServerMoveCityPopUpView::init(){
    bool ret = false;
    if(MoveCityPopUpView::init(MoveCity_Castle)){
        ret = true;
    }
    return ret;
}

void CrossServerMoveCityPopUpView::onEnter(){
    MoveCityPopUpView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CrossServerMoveCityPopUpView::doClick), CROSS_SERVER_CONFIRM, NULL);
}

void CrossServerMoveCityPopUpView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CROSS_SERVER_CONFIRM);
    MoveCityPopUpView::onExit();
}

int CrossServerMoveCityPopUpView::getFreeMoveItemId(){
    return ITEM_CROSS_FREE_CITY;
}

int CrossServerMoveCityPopUpView::getMoveItemId(){
    return ITEM_CROSS_MOVE_CITY;
}

void CrossServerMoveCityPopUpView::doWhenHasItem(CCDictionary *dict){
    if (GlobalData::shared()->trans_res_forbidden_swith == 1) {
        if (GlobalData::shared()->resourceInfo.lFood > GlobalData::shared()->trans_res_limit || GlobalData::shared()->resourceInfo.lWood > GlobalData::shared()->trans_res_limit) {
            CCCallFuncO* fun = CCCallFuncO::create(this, callfuncO_selector(CrossServerMoveCityPopUpView::alertShow), dict);
            YesNoDialog::marchAlertShow(_lang_1("104959", CC_ITOA(GlobalData::shared()->trans_res_limit)), fun, NULL);
        }
        else {
            WorldMapView::instance()->afterCrossCityMove(dict);
            this->closeSelf();
        }
    }
    else {
        WorldMapView::instance()->afterCrossCityMove(dict);
        this->closeSelf();
    }
}

void CrossServerMoveCityPopUpView::alertShow(CCObject *dict){
    CCDictionary* dic = dynamic_cast<CCDictionary*>(dict);
    WorldMapView::instance()->afterCrossCityMove(dic);
    this->closeSelf();
}

void CrossServerMoveCityPopUpView::onMoveCityClick(CCObject *pSender, CCControlEvent event){
    if(FunBuildController::getInstance()->getMainCityLv() > _limit_cross_server){
        CCCommonUtils::flyHint("", "", _lang("108751"));
        return;
    }
    PopupViewController::getInstance()->addPopupView(CrossServerAlertPopUpView::create());
}
void CrossServerMoveCityPopUpView::doWhenUseMoney(CCDictionary *dict)
{
    onUseTool();
}
//-----

bool CrossServerAlertPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        auto bg = CCBLoadFile("crossserveralert", this, this);
        this->setContentSize(bg->getContentSize());
        this->m_title->setString(_lang("105261"));
        this->m_hintText->setString(_lang("108750"));
        CCCommonUtils::setButtonTitle(m_confirmBtn, _lang("confirm").c_str());
        CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("cancel_btn_label").c_str());

        ret = true;
    }
    return ret;
}

void CrossServerAlertPopUpView::onEnter(){
    PopupBaseView::onEnter();
}

void CrossServerAlertPopUpView::onExit(){
    PopupBaseView::onExit();
}

cocos2d::SEL_MenuHandler CrossServerAlertPopUpView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

bool CrossServerAlertPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_confirmBtn", CCControlButton*, this->m_confirmBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn", CCControlButton*, this->m_cancelBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title", CCLabelIF*, this->m_title);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, this->m_hintText);
    
    return false;
}

SEL_CCControlHandler CrossServerAlertPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickOKBtn", CrossServerAlertPopUpView::onClickOKBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCancelBtn", CrossServerAlertPopUpView::onClickCancelBtn);
    return NULL;
}

void CrossServerAlertPopUpView::onClickOKBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CROSS_SERVER_CONFIRM);
    this->closeSelf();
}

void CrossServerAlertPopUpView::onClickCancelBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    this->closeSelf();
}
