//
//  TileOpenView.cpp
//  IF
//
//  Created by fubin on 14-5-23.
//
//

#include "TileOpenView.h"
#include "TileInfo.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "UIComponent.h"

//tileId指ItemSpec id="9990"这个字段  //a by ljf

TileOpenView* TileOpenView::create(int tileId){
    TileOpenView* ret = new TileOpenView();
    if(ret && ret->init(tileId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TileOpenView::init(int tileId)
{
    if (!ArcPopupBaseView::init(TYPE_POS_UP)) {
        return false;
    }
    setIsHDPanel(true);
    m_isReturn = true;
    m_tileId = tileId;
    auto tmpCCB = CCBLoadFile("TileOpenView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    m_upBtnMsgLabel->setString(_lang("102113").c_str());
//    m_btnMsgLabel->setString(_lang("102127").c_str());
//    m_inBtnGoldNum->setString("1");
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    updateInfo();
    m_openNum=0;
    
    m_mainNode->setPositionY(m_mainNode->getPositionY()-200);
    CCActionInterval * moveBy2 = CCMoveBy::create(0.25, ccp(0,200));
    m_mainNode->runAction(CCSequence::create(moveBy2, NULL));
    
    return true;
}

void TileOpenView::updateInfo(CCObject* p)
{
    bool isCanUp = true;
    
    string name = "";
    int silver_need = 0;
    int wood_need = 0;
    int time_need = 0;
    int stone_need = 0;
    int food_need = 0;
    int iron_need = 0;
    string tool_need = "";
    string building = "";
    string destip = "";
    int level = 0;
    
    
    auto m_info = FunBuildController::getInstance()->m_bigTileMap[m_tileId];
    //auto m_info = FunBuildController::getInstance()->m_tilePositionUnlockMap[m_tileId]; //a by ljf
    
    silver_need = m_info.silver_need;
    wood_need = m_info.wood_need;
    stone_need = m_info.stone_need;
    food_need = m_info.food_need;
    iron_need = m_info.iron_need;
//    destip = m_info.destip;
    
    _tmpGold = 0;
    m_nextLabel->setString("");
    m_nextMsgLabel->setString(_lang("102112"));
    
    int _itemH = 46;
    int curY = 25;
    int curX = 30;
    if (CCCommonUtils::isIosAndroidPad())
    {
        _itemH = 87;
        curX = 60;
        curY = 50;
    }
    
    m_scrollView->getContainer()->removeAllChildren();
    if (silver_need>0) {
        auto item1 = UpgradeCell::create(0, Silver, silver_need);
        item1->setPosition(ccp(curX, curY));
        item1->setTouchNode(m_infoList,m_tileId);
        m_scrollView->addChild(item1);
        curY += _itemH;
        
        if (!item1->m_isOk) {
            isCanUp = false;
        }
        
        int tmpSub = silver_need - GlobalData::shared()->resourceInfo.lMoney;
        if (tmpSub>0) {
            _tmpGold += CCCommonUtils::getResGlodByType(Silver, tmpSub);
        }
    }
    
    if (stone_need>0) {
        auto item3 = UpgradeCell::create(0, Stone, stone_need);
        item3->setPosition(ccp(curX, curY));
        item3->setTouchNode(m_infoList,m_tileId);
        m_scrollView->addChild(item3);
        curY += _itemH;
        
        if (!item3->m_isOk) {
            isCanUp = false;
        }
        
        int tmpSub = stone_need - GlobalData::shared()->resourceInfo.lStone;
        if (tmpSub>0) {
            _tmpGold += CCCommonUtils::getResGlodByType(Stone, tmpSub);
        }
    }
    
    if (iron_need>0) {
        auto item4 = UpgradeCell::create(0, Iron, iron_need);
        item4->setPosition(ccp(curX, curY));
        item4->setTouchNode(m_infoList,m_tileId);
        m_scrollView->addChild(item4);
        curY += _itemH;
        
        if (!item4->m_isOk) {
            isCanUp = false;
        }
        
        int tmpSub = iron_need - GlobalData::shared()->resourceInfo.lIron;
        if (tmpSub>0) {
            _tmpGold += CCCommonUtils::getResGlodByType(Iron, tmpSub);
        }
    }
    
    if (food_need>0) {
        auto item5 = UpgradeCell::create(0, Food, food_need);
        item5->setPosition(ccp(curX, curY));
        item5->setTouchNode(m_infoList,m_tileId);
        m_scrollView->addChild(item5);
        curY += _itemH;
        
        if (!item5->m_isOk) {
            isCanUp = false;
        }
        
        int tmpSub = food_need - GlobalData::shared()->resourceInfo.lFood;
        if (tmpSub>0) {
            _tmpGold += CCCommonUtils::getResGlodByType(Food, tmpSub);
        }
    }
    
    if (wood_need>0) {
        auto item2 = UpgradeCell::create(0, Wood, wood_need);
        item2->setPosition(ccp(curX, curY));
        item2->setTouchNode(m_infoList,m_tileId);
        m_scrollView->addChild(item2);
        curY += _itemH;
        
        if (!item2->m_isOk) {
            isCanUp = false;
        }
        
        int tmpSub = wood_need - GlobalData::shared()->resourceInfo.lWood;
        if (tmpSub>0) {
            _tmpGold += CCCommonUtils::getResGlodByType(Wood, tmpSub);
        }
    }
//    m_inBtnGoldNum->setString(CC_ITOA(_tmpGold));
    
    m_upBtn->setEnabled(isCanUp);
    
    if (CCCommonUtils::isIosAndroidPad()) {
        curY -= 46;
    } else {
        curY -= 23;
        curY -= 46;//fusheng 原本显示少一个
    }
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
    m_scrollView->setTouchEnabled(false);
}

void TileOpenView::onEnter()
{
    CCNode::onEnter();
    UIComponent::getInstance()->showResourceBar(true);
    if (m_openNum>0) {
        updateInfo();
    }
    
    m_openNum++;
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TileOpenView::updateInfo), MSG_REFREASH_BUILD_UPGRADE, NULL);
}

void TileOpenView::onExit()
{
    UIComponent::getInstance()->showResourceBar(false);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_BUILD_UPGRADE);
    CCNode::onExit();
}

SEL_CCControlHandler TileOpenView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCreateOrUpClick", TileOpenView::onCreateOrUpClick);
    return NULL;
}

bool TileOpenView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, this->m_upBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLabel", CCLabelIF*, this->m_nextLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextMsgLabel", CCLabelIF*, this->m_nextMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtnMsgLabel", CCLabelIF*, this->m_upBtnMsgLabel);
    return false;
}

CCNode *TileOpenView::getGuideNode(int itemId){
    return NULL;
}

void TileOpenView::onCreateOrUpClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UNLOCK_TILE, CCInteger::create(m_tileId));
    FunBuildController::getInstance()->startOpenTile(m_tileId, _tmpGold);
    PopupViewController::getInstance()->removePopupView(this);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_GOBACK);
}

void TileOpenView::onOkUp()
{
}