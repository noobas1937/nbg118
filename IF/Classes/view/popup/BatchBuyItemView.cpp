//
//  BatchBuyItemView.cpp
//  IF
//
//  Created by fubin on 14-4-21.
//
//

#include "BatchBuyItemView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "ToolInfo.h"
#include "ToolController.h"
#include "ArmyController.h"
#include "WorldController.h"
#include "WorldMapView.h"

const float numPerRow = 1.0;
BatchBuyItemView* BatchBuyItemView::create(int toolId, int needNum){
    BatchBuyItemView* ret = new BatchBuyItemView();
    if(ret && ret->init(toolId, needNum)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BatchBuyItemView::init(int toolId, int needNum)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    m_toolId = toolId;
    m_needNum = needNum;
    
    setCleanFunction([](){
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    auto tmpCCB = CCBLoadFile("BatchBuyItemView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    if (CCCommonUtils::isIosAndroidPad())
    {
        addHeight = CCDirector::sharedDirector()->getWinSize().height - 2048;
    }
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    setTitleName(_lang("104910").c_str());
    
    m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
    m_tabView->setTouchPriority(Touch_Popup);
    m_infoList->addChild(m_tabView);
    
    updateInfo();
    
    return true;
}

void BatchBuyItemView::updateInfo()
{
    m_numList.clear();
    m_priceList.clear();
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_toolId);
    refreashData(NULL);
    
    vector<string> vecItems;
    CCCommonUtils::splitString(info.sales, "|", vecItems);
    for (int i=0; i<vecItems.size(); i++) {
        vector<string> cellItems;
        CCCommonUtils::splitString(vecItems[i], ";", cellItems);
        if (cellItems.size()==2) {
            m_numList.push_back(atoi(cellItems[0].c_str()));
            m_priceList.push_back(atoi(cellItems[1].c_str()));
        }
    }
    m_tabView->reloadData();
}


void BatchBuyItemView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BatchBuyItemView::refreashData), MSG_TOOL_CHANGE, NULL);
}

void BatchBuyItemView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOOL_CHANGE);
    CCNode::onExit();
}

void BatchBuyItemView::refreashData(CCObject* p)
{
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_toolId);
    string msg = _lang_3("104911", info.getName().c_str(), CC_CMDITOA(info.getCNT()).c_str(), CC_CMDITOA(m_needNum).c_str());
    m_nameLabel->setString(msg);
    if(m_toolId == 209174)
    {
        m_nameLabel->setVisible(false);
    }
}

SEL_CCControlHandler BatchBuyItemView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool BatchBuyItemView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    
    return false;
}

void BatchBuyItemView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
}
cocos2d::CCSize BatchBuyItemView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1440, 270);
    }
    return CCSize(600, 160);
}
CCTableViewCell* BatchBuyItemView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx)
{
    if(idx >= m_numList.size()){
        return NULL;
    }
    
    BatchItemCell* cell = (BatchItemCell*)table->dequeueGrid();
    if(idx < m_numList.size()){
        if(cell){
            cell->setData(m_toolId, m_numList[idx], m_priceList[idx]);
        }else{
            cell = BatchItemCell::create(m_toolId, m_numList[idx], m_priceList[idx]);
        }
    }
    return cell;
}
ssize_t BatchBuyItemView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = ceil(m_numList.size() / numPerRow);
    return num;
}
unsigned int BatchBuyItemView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return numPerRow;
}

////

BatchItemCell* BatchItemCell::create(int itemId, int num, float price)
{
    auto ret = new BatchItemCell();
    if (ret && ret->init(itemId, num, price)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BatchItemCell::init(int itemId, int num, float price)
{
    bool ret = true;
    CCBLoadFile("UseToolCell",this,this);
    m_useBtn->setEnabled(false);
    m_useBtn->setVisible(false);
    m_numLabel->setVisible(false);
    
    setData(itemId, num, price);
    return ret;
}

void BatchItemCell::setData(int itemId, int num, float price)
{
    m_itemId = itemId;
    m_num = num;
    m_price = price;

    ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
    
    string name = toolInfo.getName();
    if (m_num > 1) {
        name = name+ " *" + CC_CMDITOA(m_num);
    }
    m_nameLabel->setString(name);
    m_desLabel->setString(_lang(toolInfo.des).c_str());
    
    m_picNode->removeAllChildren();
    auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(toolInfo.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
    if (pic) {
        CCCommonUtils::setSpriteMaxSize(pic, 80);
        m_picNode->addChild(pic);
    }
    m_inBtnGoldNum->setString(CC_CMDITOA(m_price));
    m_btnMsgLabel->setString(_lang("102148").c_str());
}

void BatchItemCell::onEnter() {
    CCNode::onEnter();
}

void BatchItemCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler BatchItemCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn", BatchItemCell::onClickBuyBtn);
    return NULL;
}

void BatchItemCell::onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_waitInterface)
        return;
    m_waitInterface=GameController::getInstance()->showWaitInterface(m_buyBtn);
    
    ToolController::getInstance()->buyTool(m_itemId, m_num, CCCallFunc::create(this, callfunc_selector(BatchItemCell::onRetBuyTool)), m_price,"BatchBuyItemView");
}
void BatchItemCell::onRetBuyTool(){
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface=NULL;
    }
}
bool BatchItemCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useBtn", CCControlButton*, this->m_useBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtn", CCControlButton*, this->m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyNode", CCNode*, m_buyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, m_inBtnGoldNum);
    return false;
}