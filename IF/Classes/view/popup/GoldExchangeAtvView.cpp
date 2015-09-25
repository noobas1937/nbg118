//
//  GoldExchangeAtvView.cpp
//  IF
//
//  Created by zhaohf on 14-4-16.
//
//

#include "GoldExchangeAtvView.h"
#include "PayController.h"
#include "DataRecordCommand.h"
#include "ToolController.h"
#include "ToolInfo.h"
#include "FreeListView.h"
#include "PopupViewController.h"
#include "PaymentCommand.h"
#include "GoldExchangeMoreListView.h"
#include "ButtonLightEffect.h"
#include "EquipmentController.h"

GoldExchangeAtvView* GoldExchangeAtvView::create(GoldExchangeItem *goldExchangeItem, int parentType){
    GoldExchangeAtvView* ret = new GoldExchangeAtvView(goldExchangeItem, parentType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldExchangeAtvView::init()
{
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(102, true);

    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(102, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    
    CCBLoadFile("GoldRechargeCommActView",this,this);
    this->setContentSize(CCSizeMake(640, 635));
    
    
    m_titleLabel->setString(_lang("102148").c_str());
    m_getLabel->setString(_lang_1("115073",""));
    m_timeLabel->setString("23:59:59");
    m_titleLabel->setString(_lang("101224").c_str());
    //CCCommonUtils::setButtonTitle(m_moreBtn, _lang("102162").c_str());
    this->m_moreLabel->setString(_lang("102162"));
    m_scrollView = CCScrollView::create(m_listNode->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setTouchPriority(Touch_Default);
    m_listNode->addChild(m_scrollView);
    m_scrollView->setTouchEnabled(false);
    
    string dollar = PayController::getInstance()->getDollarText(m_dataItem->dollar,m_dataItem->product_id);
    //m_oldPriceLabel->setString(PayController::getInstance()->getDollarText(m_dataItem->oldprice));
    m_newPriceLabel->setString(dollar);
    string dollarPercent =  m_dataItem->percent+"%";
    m_percentLabel->setString(dollarPercent.c_str());
    m_getGoldNumText->setString(m_dataItem->gold_doller.c_str());
    
    int curY = 0;
    if (m_dataItem->item != "") {
        vector<string> vector1;
        vector<string> vector2;
        CCCommonUtils::splitString(m_dataItem->item, "|", vector1);
        int i = vector1.size()-3;
        if(i<0){
            i=0;
        }
        while (i < vector1.size()) {
            vector2.clear();
            CCCommonUtils::splitString(vector1[i], ";", vector2);
            if (vector2.size()==2) {
                auto cell = RechargeCell::create(atoi(vector2[0].c_str()), atoi(vector2[1].c_str()),0);
                cell->setPosition(ccp(0, curY));
                m_scrollView->cocos2d::CCNode::addChild(cell);
                curY += 60;
            }
            i++;
        }
    }
    if(!m_dataItem->equipment.empty()){
        vector<string> vector1;
        vector<string> vector2;
        CCCommonUtils::splitString(m_dataItem->equipment, "|", vector1);
        int i = vector1.size()-3;
        if(i<0){
            i=0;
        }
        while (i < vector1.size()) {
            vector2.clear();
            CCCommonUtils::splitString(vector1[i], ";", vector2);
            if (vector2.size()==2) {
                auto cell = RechargeCell::create(atoi(vector2[0].c_str()), atoi(vector2[1].c_str()),1);
                cell->setPosition(ccp(0, curY));
                m_scrollView->addChild(cell);
                curY += 60;
            }
            i++;
        }
    }
    m_scrollView->setContentSize(CCSize(m_listNode->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_listNode->getContentSize().height - curY));
    
    m_soleOutSpr->setVisible(false);
    m_costBtn->setEnabled(true);
    if (m_dataItem->bought) {
        m_soleOutSpr->setVisible(true);
        m_costBtn->setEnabled(false);
    }
    m_animNode = CCNode::create();
    m_costBtn->getParent()->addChild(m_animNode);
    m_animNode->setPosition(m_costBtn->getPosition());
    CCSize btnSize = m_costBtn->getContentSize();
    ButtonLightEffect* btnEffect = ButtonLightEffect::create(btnSize);
    m_animNode->addChild(btnEffect);
//    CCSprite* Spr = CCSprite::create();
//    m_animNode->addChild(Spr);
//    string curPath = "ButtonLight_%d.png";
//    CCCommonUtils::makeInternalEffectSpr(Spr, curPath, 12, 0.07,0,2.0);
//    //Spr->setContentSize(CCSizeMake(m_costBtn->getContentSize().width, 10));
//    // Spr->setPreferredSize(CCSizeMake(m_costBtn->getContentSize().width, m_costBtn->getContentSize().height));
//    Spr->setAnchorPoint(ccp(0.5, 0.5));
//    float scalex = m_costBtn->getContentSize().width/Spr->getContentSize().width;
//    float scaley = (m_costBtn->getContentSize().height)/Spr->getContentSize().height;
//    Spr->setScaleX(scalex);
//    Spr->setScaleY(scaley);
////    Spr->setPosition(ccp(-Spr->getContentSize().width/2, -Spr->getContentSize().height/2));
//    Spr->setOpacity(175);
    onEnterFrame(0);
    return true;
}

void GoldExchangeAtvView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GoldExchangeAtvView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

void GoldExchangeAtvView::onExit()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GoldExchangeAtvView::onEnterFrame), this);
    setTouchEnabled(false);
    GlobalData::shared()->isBind = false;
    CCNode::onExit();
}

SEL_CCControlHandler GoldExchangeAtvView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCostBtn", GoldExchangeAtvView::onClickCostBtn);
   // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMoreBtnClick", GoldExchangeAtvView::onMoreBtnClick);
    return NULL;
}

bool GoldExchangeAtvView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreLabel", CCLabelIF*, this->m_moreLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelBMFont*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_percentLabel", CCLabelBMFont*, this->m_percentLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getGoldNumText", CCLabelIF*, this->m_getGoldNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_oldPriceLabel", CCLabelIF*, this->m_oldPriceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newPriceLabel", CCLabelIF*, this->m_newPriceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getLabel", CCLabelIF*, this->m_getLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costBtn", CCControlButton*, this->m_costBtn);
  //  CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreBtn", CCControlButton*, this->m_moreBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soleOutSpr", CCSprite*, this->m_soleOutSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchMoreNode", CCNode*, this->m_touchMoreNode);
    return false;
}

bool GoldExchangeAtvView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    if (isTouchInside(m_touchMoreNode, pTouch)) {
        m_startPoint =pTouch->getLocation();
        return true;
    }
    return false;
}
void GoldExchangeAtvView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    //if(m_parentType==1) {
      //  if (!isTouchInside(m_touchNode, pTouch)) {
          //  PopupViewController::getInstance()->removeLastPopupView();
      //  }
    //}
    
//    if (m_parentScrollView) {
//        m_parentScrollView->setTouchEnabled(true);
//    }
    if (isTouchInside(m_touchMoreNode, pTouch)) {
        if(fabsf(pTouch->getLocation().y-m_startPoint.y)>20||fabsf(pTouch->getLocation().x-m_startPoint.x)>20){
            return;
        }
        onMoreBtnClick(NULL,Control::EventType::TOUCH_DOWN);
    }
}
void GoldExchangeAtvView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//    if (isTouchInside(m_listNode, pTouch)) {
//        if (m_parentScrollView) {
//            m_parentScrollView->setTouchEnabled(false);
//        }
//        m_scrollView->onTouchMoved(pTouch, pEvent);
//    }
//    else {
//        if (m_parentScrollView) {
//            m_parentScrollView->setTouchEnabled(true);
//        }
//        m_parentScrollView->onTouchMoved(pTouch, pEvent);
//    }
}

void GoldExchangeAtvView::setParentScrollView(CCScrollView* parentScrollView)
{
    m_parentScrollView = parentScrollView;
}
void GoldExchangeAtvView::onMoreBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupView(GoldExchangeMoreListView::create(atoi(m_dataItem->id.c_str())));

}
void GoldExchangeAtvView::onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PayController::getInstance()->callPayment(m_dataItem);
}

void GoldExchangeAtvView::onEnterFrame(float dt)
{
    int curTime = GlobalData::shared()->getWorldTime();
    int lastTime = m_dataItem->end - curTime;
    m_timeLabel->setString(CC_SECTOA(lastTime));
    if (lastTime<=0) {
        m_dataItem->end+= 3600*24;
        m_dataItem->bought = false;
       // ToolController::getInstance()->m_canBuy = 1;
        m_soleOutSpr->setVisible(false);
        m_costBtn->setEnabled(true);
    }
}

////////
RechargeCell* RechargeCell::create(int itemId, int num,int type)
{
    auto ret = new RechargeCell();
    if (ret && ret->init(itemId, num,type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool RechargeCell::init(int itemId, int num,int type)
{
    CCBLoadFile("RechargeSaleCell",this,this);
    string name ="";
    string picStr = "";
    if(type==0){
        if (itemId < 10) {// 资源
            picStr = CCCommonUtils::getResourceIconByType(itemId);
            name = CCCommonUtils::getResourceNameByType(itemId);
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(2).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, 55,true);
            m_iconNode->addChild(iconBg);
            auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_DEFAULT);
            CCCommonUtils::setSpriteMaxSize(icon, 52, true);
            m_iconNode->addChild(icon);
            m_nameLabel->setString(name);
            m_numLabel->setString(CC_ITOA(num));
        }else {//道具
//            auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
//            name = _lang(toolInfo.name);
//            picStr = CCCommonUtils::getIcon(CC_ITOA(itemId));
//            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, 55,true);
//            m_iconNode->addChild(iconBg);
//            dPath = true;
            CCCommonUtils::createGoodsIcon(itemId, m_iconNode, CCSize(55, 55),NULL,m_nameLabel);
            m_numLabel->setString(CC_ITOA(num));
        }
    }else if(type==1){
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[itemId];
        name = _lang(eInfo.name);
        string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
        auto iconBg = CCLoadSprite::createSprite(bgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(iconBg, 55, true);
        m_iconNode->addChild(iconBg);
        picStr = CCCommonUtils::getIcon(CC_ITOA(itemId));
        auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(icon, 52, true);
        m_iconNode->addChild(icon);
    }
    return true;
}

void RechargeCell::onEnter() {
    CCNode::onEnter();
}

void RechargeCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler RechargeCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool RechargeCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    return false;
}