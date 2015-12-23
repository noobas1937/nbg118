//
//  GoldExchangeMoreListView.cpp
//  IF
//
//  Created by lifangkai on 14-9-10.
//
//

#include "GoldExchangeMoreListView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "ToolController.h"
#include "TipsView.h"
#include "PayController.h"
#include "PaymentCommand.h"
#include "GoldExchangeAdvertisingView.h"
#include "ButtonLightEffect.h"
#include "EquipmentController.h"

GoldExchangeMoreListView* GoldExchangeMoreListView::create(int itemId){
    GoldExchangeMoreListView* ret = new GoldExchangeMoreListView();
    if(ret && ret->init(itemId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldExchangeMoreListView::init(int itemId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
//    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
//        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });

    this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
        // detect touch inside panel
        if (!isTouchInside(m_buildBG, pTouch)) {
            PopupViewController::getInstance()->removePopupView(this);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_ADVERTISING_SCROLL);
        }
    });
    m_itemId = itemId;
    auto tmpCCB = CCBLoadFile("GoldExchangeMoreListView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    //CCCommonUtils::setButtonTitle(m_okBtn, "collect");
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    m_getLabel->setString(_lang_1("115073",""));
    auto& m_dataItem = GlobalData::shared()->goldExchangeList[CC_ITOA(m_itemId)];
    string dollar = PayController::getInstance()->getDollarText(m_dataItem->dollar,m_dataItem->product_id);
    m_oldPriceLabel->setString(PayController::getInstance()->getDollarText(m_dataItem->oldprice,""));
    m_newPriceLabel->setString(dollar);


    m_goldNumText->setString(m_dataItem->gold_doller.c_str());
    m_soleOutSpr->setVisible(false);
    if (m_dataItem->bought) {
        m_soleOutSpr->setVisible(true);
        m_costBtn->setEnabled(false);
    }
    
    m_liBaoName->setString(_lang(m_dataItem->name));
    //fusheng d begin
//    m_animNode = CCNode::create();
//    m_costBtn->getParent()->addChild(m_animNode);
//    m_animNode->setPosition(m_costBtn->getPosition());
//    CCSize btnSize = m_costBtn->getContentSize();
//    ButtonLightEffect* btnEffect = ButtonLightEffect::create(btnSize);
//    m_animNode->addChild(btnEffect);
    //fusheng d end
    
//    CCSprite* Spr = CCSprite::create();
//    m_animNode->addChild(Spr);
//    string curPath = "ButtonLight_%d.png";
//    CCCommonUtils::makeInternalEffectSpr(Spr, curPath, 12, 0.07,0,2.0);
//    //Spr->setContentSize(CCSizeMake(m_costBtn->getContentSize().width, 10));
//    // Spr->setPreferredSize(CCSizeMake(m_costBtn->getContentSize().width, m_costBtn->getContentSize().height));
//    Spr->setAnchorPoint(ccp(0.5, 0.5));
//    CCSize size = m_costBtn->getContentSize();
//    CCSize size1 = Spr->getContentSize();
//    float scalex = m_costBtn->getContentSize().width/Spr->getContentSize().width;
//    float scaley = 1.0;
//    if(m_dataItem->popup_image=="thanksgiving" || m_dataItem->popup_image=="winter"){
//        scaley = (m_costBtn->getContentSize().height)/Spr->getContentSize().height;
//        Spr->setPositionY(0);
//    }else{
//        scaley =(m_costBtn->getContentSize().height+10)/Spr->getContentSize().height;
//        Spr->setPositionY(3);
//    }
//    Spr->setScaleX(scalex);
//    Spr->setScaleY(scaley);
//    CCSize size2 = Spr->getContentSize();
//    
//    //    Spr->setPositionX(-size2.width*scalex/2);
//    // Spr->setPosition(ccp(-Spr->getContentSize().width/2, -Spr->getContentSize().height/2));
//    Spr->setOpacity(175);
    int curY = 0;
    if (m_dataItem->item != "") {
        vector<string> vector1;
        vector<string> vector2;
        CCCommonUtils::splitString(m_dataItem->item, "|", vector1);
        int i = 0;
        while (i < vector1.size()) {
            vector2.clear();
            CCCommonUtils::splitString(vector1[i], ";", vector2);
            if (vector2.size()==2) {
                int itemId = atoi(vector2[0].c_str());
                int num = atoi(vector2[1].c_str());
                auto cell = GoldExchangeMoreListCell::create(itemId, num,0);
                cell->setPosition(ccp(8, curY));
                m_scrollView->cocos2d::CCNode::addChild(cell);
                curY += 95;
            }
            i++;
        }
    }
    if (!m_dataItem->equipment.empty()) {
        vector<string> vector1;
        vector<string> vector2;
        CCCommonUtils::splitString(m_dataItem->equipment, "|", vector1);
        int i = 0;
        while (i < vector1.size()) {
            vector2.clear();
            CCCommonUtils::splitString(vector1[i], ";", vector2);
            if (vector2.size()==2) {
                int itemId = atoi(vector2[0].c_str());
                int num = atoi(vector2[1].c_str());
                auto cell = GoldExchangeMoreListCell::create(itemId, num,1);
                cell->setPosition(ccp(8, curY));
                m_scrollView->cocos2d::CCNode::addChild(cell);
                curY += 80;
            }
            i++;
        }
    }
    if(m_dataItem->type=="2"){
        m_soleOutSpr1->setVisible(false);
        m_oldPriceLabel->setVisible(false);
        m_getLabel->setPosition(-93, -210+8);
        m_arrowSpr->setPositionY(m_arrowSpr->getPositionY()+8);
        m_newPriceLabel->setPositionY(-210+8);
    }
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
    
    updateInfo();
    return true;
}

void GoldExchangeMoreListView::updateInfo()
{
}

void GoldExchangeMoreListView::onEnter()
{
    CCNode::onEnter();
}

void GoldExchangeMoreListView::onExit()
{
    GlobalData::shared()->isBind = false;
    CCNode::onExit();
}

SEL_CCControlHandler GoldExchangeMoreListView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCostBtn", GoldExchangeMoreListView::onClickCostBtn);
    return NULL;
}

bool GoldExchangeMoreListView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
   // CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNumText", CCLabelIF*, this->m_goldNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_oldPriceLabel", CCLabelIF*, this->m_oldPriceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newPriceLabel", CCLabelIF*, this->m_newPriceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getLabel", CCLabelIF*, this->m_getLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_liBaoName", CCLabelIF*, this->m_liBaoName);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costBtn", CCControlButton*, this->m_costBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soleOutSpr", CCSprite*, this->m_soleOutSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soleOutSpr1", CCSprite*, this->m_soleOutSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSpr", CCSprite*, this->m_arrowSpr);
    return false;
}

void GoldExchangeMoreListView::onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    auto& m_dataItem = GlobalData::shared()->goldExchangeList[CC_ITOA(m_itemId)];
    PayController::getInstance()->callPayment(m_dataItem);
}

/////

GoldExchangeMoreListCell* GoldExchangeMoreListCell::create(int itemId, int num,int type)
{
    auto ret = new GoldExchangeMoreListCell();
    if (ret && ret->init(itemId, num,type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldExchangeMoreListCell::init(int itemId, int num,int type)
{
    CCBLoadFile("RechargeLargeCell",this,this);
    string name ="";
    string picStr = "";
    if(type==0){
        if (itemId < 10) {// 资源
            picStr = CCCommonUtils::getResourceIconByType(itemId);
            name = CCCommonUtils::getResourceNameByType(itemId);
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(2).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, 65,true);
            m_iconNode->addChild(iconBg);
            auto icon =CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_DEFAULT);
            CCCommonUtils::setSpriteMaxSize(icon, 60, true);
            icon->setScale(iconBg->getScale());
            m_iconNode->addChild(icon);
            m_nameLabel->setString(name);
            string str = CC_ITOA(num);
            str = "X" + str;
            m_numLabel->setString(str);
        }
        else {//道具
            CCCommonUtils::createGoodsIcon(itemId, m_iconNode, CCSize(65, 65),NULL,m_nameLabel);
            
            string str = CC_ITOA(num);
            str = "X" + str;
            m_numLabel->setString(str);

//            m_numLabel->setString(CC_ITOA(num));
//            auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
//            name = _lang(toolInfo.name);
//            picStr = CCCommonUtils::getIcon(CC_ITOA(itemId));
//            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, 65,true);
//            m_iconNode->addChild(iconBg);
//            dPath=true;
        }
    }else if(type == 1){
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[itemId];
        name = _lang(eInfo.name);
        m_nameLabel->setString(name);
        string str = CC_ITOA(num);
        str = "X" + str;
        m_numLabel->setString(str);
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

void GoldExchangeMoreListCell::onEnter() {
    CCNode::onEnter();
}

void GoldExchangeMoreListCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler GoldExchangeMoreListCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool GoldExchangeMoreListCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    return false;
}