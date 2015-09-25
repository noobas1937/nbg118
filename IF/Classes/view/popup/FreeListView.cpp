//
//  FreeListView.cpp
//  IF
//
//  Created by fubin on 14-8-29.
//
//

#include "FreeListView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "ToolController.h"
#include "TipsView.h"
#include "EquipmentController.h"

FreeListView* FreeListView::create(int itemId){
    FreeListView* ret = new FreeListView();
    if(ret && ret->init(itemId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FreeListView::init(int itemId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    
    m_itemId = itemId;
    auto tmpCCB = CCBLoadFile("FreeListView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    CCCommonUtils::setButtonTitle(m_okBtn, _lang("confirm").c_str());
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    auto iter = GlobalData::shared()->goldExchangeList.find(CC_ITOA(m_itemId));
    if(iter!=GlobalData::shared()->goldExchangeList.end()){
        auto& m_dataItem = GlobalData::shared()->goldExchangeList[CC_ITOA(m_itemId)];
        m_goldNumText->setString(m_dataItem->gold_doller.c_str());
        m_goldTitleText->setString(_lang("107502").c_str());
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
                    auto cell = RechargeLargeCell::create(atoi(vector2[0].c_str()), atoi(vector2[1].c_str()),0);
                    cell->setPosition(ccp(0, curY));
                    m_scrollView->cocos2d::CCNode::addChild(cell);
                    curY += 80;
                }
                i++;
            }
        }
        if(m_dataItem->equipment!=""){
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
                    auto cell = RechargeLargeCell::create(atoi(vector2[0].c_str()), atoi(vector2[1].c_str()),1);
                    cell->setPosition(ccp(0, curY));
                    m_scrollView->cocos2d::CCNode::addChild(cell);
                    curY += 80;
                }
                i++;
            }
        }
        
        m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
        m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
        
        updateInfo();
    }
    return true;
}

void FreeListView::updateInfo()
{
}

void FreeListView::onEnter()
{
    CCNode::onEnter();
}

void FreeListView::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler FreeListView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCollectBtn", FreeListView::onClickCollectBtn);
    return NULL;
}

bool FreeListView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNumText", CCLabelBMFont*, this->m_goldNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldTitleText", CCLabelBMFont*, this->m_goldTitleText);
    return false;
}

void FreeListView::onClickCollectBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

/////

RechargeLargeCell* RechargeLargeCell::create(int itemId, int num,int type)
{
    auto ret = new RechargeLargeCell();
    if (ret && ret->init(itemId, num,type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool RechargeLargeCell::init(int itemId, int num,int type)
{
    CCBLoadFile("RechargeLargeCell",this,this);
    string name ="";
    string picStr = "";
    if(type==0){
        if (itemId < 10) {// 资源
            picStr = CCCommonUtils::getResourceIconByType(itemId);
            name = CCCommonUtils::getResourceNameByType(itemId);
            auto icon = CCLoadSprite::createSprite(picStr.c_str());
            CCCommonUtils::setSpriteMaxSize(icon, 60, true);
            m_iconNode->addChild(icon);
            m_nameLabel->setString(name);
        }
        else {//道具
            CCCommonUtils::createGoodsIcon(itemId, m_iconNode, CCSize(60, 60),NULL,m_nameLabel);
        }
    }else if(type==1){
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[itemId];
        name = _lang(eInfo.name) + "   "+_lang("102203")+" " + CC_ITOA(eInfo.level);
        string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
        auto iconBg = CCLoadSprite::createSprite(bgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(iconBg, 60, true);
        m_iconNode->addChild(iconBg);
        picStr = CCCommonUtils::getIcon(CC_ITOA(itemId));
        auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(icon, 55, true);
        m_iconNode->addChild(icon);
        m_nameLabel->setString(name);
    }
    m_numLabel->setString(CC_ITOA(num));
    bool ret = true;
    return ret;
}

void RechargeLargeCell::onEnter() {
    CCNode::onEnter();
}

void RechargeLargeCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler RechargeLargeCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool RechargeLargeCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    return false;
}