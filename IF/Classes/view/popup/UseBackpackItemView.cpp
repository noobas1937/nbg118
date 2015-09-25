//
//  UseBackpackItemView.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-24.
//
//

#include "UseBackpackItemView.h"
#include "QuestController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "ChangePicView.h"
#include "ToolController.h"
#include "UseToolView.h"
#include "WorldMapView.h"
#include "ChangeNickNameView.h"

UseBackpackItemView *UseBackpackItemView::create(int itemId){
    UseBackpackItemView *ret = new UseBackpackItemView(itemId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void UseBackpackItemView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void UseBackpackItemView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "on_pic_click");
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool UseBackpackItemView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void UseBackpackItemView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_bg, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool UseBackpackItemView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setCleanFunction([](){
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
        });
        CCBLoadFile("UseBackpackItemView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        m_titleText->setString(info.getName());
        m_desc->setString(_lang(info.des));
        m_btnMsgLabel->setString(_lang("104906"));
        m_numTxt->setString(CC_ITOA(info.getCNT()));
        m_inBtnGoldNum->setString(CC_ITOA(info.price));
        CCCommonUtils::setButtonTitle(m_useBtn, _lang("102137").c_str());
        m_useBtn->setEnabled(info.use != 0);
        if(info.getCNT()>0){
            m_goldNode->setVisible(false);
            m_useBtn->setVisible(true);
        }else{
            m_goldNode->setVisible(true);
            m_useBtn->setVisible(false);
        }
        m_iconNode->removeAllChildren();
        CCCommonUtils::createGoodsIcon(info.itemId, m_iconNode, CCSize(92, 92));
//        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//        if (pic) {
//            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info.color).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, 92,true);
//            m_iconNode->addChild(iconBg);
//            
//            CCCommonUtils::setSpriteMaxSize(pic, 92,true);
//            m_iconNode->addChild(pic);
//        }
        ret = true;
    }
    return ret;
}


void UseBackpackItemView::onUseClick(CCObject *pSender, CCControlEvent event){
    onUseTool();
}

void UseBackpackItemView::onUseAndBuyClick(CCObject *pSender, CCControlEvent event){
    //PopupViewController::getInstance()->removePopupView(this);
    ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(UseBackpackItemView::onRetBuyTool)),0,false,false,"UseBackpackItemView");
}

void UseBackpackItemView::onYes(){
    ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(UseBackpackItemView::onUseTool)),0,false,false,"UseBackpackItemView");
}

void UseBackpackItemView::onRetBuyTool(){
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    m_numTxt->setString(CC_ITOA(info.getCNT()));
    onUseTool();
}

void UseBackpackItemView::onUseTool()
{
    if(m_itemId == ITEM_RENAME) {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        if (info.getCNT() > 0) {
            PopupViewController::getInstance()->addPopupView(ChangeNickNameView::create(info.uuid));
        }
    }
    else if(m_itemId == ITEM_RANDOM) {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        if (info.getCNT() > 0) {
            auto dict = CCDictionary::create();
            dict->setObject(CCInteger::create(0), "point");
            dict->setObject(CCString::create(info.uuid), "goodsId");
            
            WorldMapView::instance()->afterCityMove(dict);
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }else if(m_itemId == ITEM_CHANGE_PIC) {
        PopupViewController::getInstance()->addPopupView(ChangePicView::create());
    }
    else {
        bool ret = ToolController::getInstance()->useTool(m_itemId, 1 , true);
        if (ret) {
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
            m_numTxt->setString(CC_ITOA(info.getCNT()));
        }
    }
    PopupViewController::getInstance()->removePopupView(this);
}

bool UseBackpackItemView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_useBtn", CCControlButton*, this->m_useBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_useAnBuyBtn", CCControlButton*, this->m_useAnBuyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_desc", CCLabelIF*, this->m_desc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnMsgLabel", CCLabelIF*, this->m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_inBtnGoldNum", CCLabelIF*, this->m_inBtnGoldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goldNode", CCNode*, this->m_goldNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_itemNode", CCNode*, this->m_itemNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCNode*, this->m_bg);
    return false;
}

SEL_CCControlHandler UseBackpackItemView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUseClick", UseBackpackItemView::onUseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUseAndBuyClick", UseBackpackItemView::onUseAndBuyClick);
    return NULL;
}



