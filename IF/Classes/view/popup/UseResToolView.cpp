//
//  UseResToolView.cpp
//  IF
//
//  Created by fubin on 14-3-13.
//
//

#include "UseResToolView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "ToolInfo.h"
#include "ToolController.h"
#include "ArmyController.h"
#include "WorldController.h"
#include "WorldMapView.h"
#include "StoreBagView.h"

const float numPerRow = 1.0;

UseResToolView* UseResToolView::create(int resType, string itemId, string parentView){
    UseResToolView* ret = new UseResToolView();
    if(ret && ret->init(resType, itemId, parentView)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UseResToolView::init(int resType, string itemId, string parentView)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    m_resType = resType;
    m_itemId = itemId;
    m_parentView = parentView;
    //TODO setCleanFunction
//    setCleanFunction([](){
//        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
//    });
    auto tmpCCB = CCBLoadFile("UseResToolView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    
    if (!CCCommonUtils::isIosAndroidPad()) {
        int oldBgHeight = m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int newBgHeight = m_buildBG->getContentSize().height;
        int addHeight = newBgHeight - oldBgHeight;
        int oldWidth = m_infoList->getContentSize().width;
        int oldHeight = m_infoList->getContentSize().height;
        m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    }
    
    m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
   // m_tabView->setTouchPriority(Touch_Popup_Item);
    m_infoList->addChild(m_tabView);
    
    int mlv = FunBuildController::getInstance()->getMainCityLv();
    if(m_resType==13){
        m_ironBtn->setEnabled(false);
        m_stoneBtn->setEnabled(false);
        m_ironNode->setVisible(false);
        m_stoneNode->setVisible(false);
        m_foodNode->setVisible(false);
        
        m_itemIcon->initWithSpriteFrame(CCLoadSprite::loadResource("Ui_tili.png"));
        m_itemIcon->setScale(0.65);
        m_woodBtn->setPreferredSize(CCSizeMake(640, 54));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_itemIcon->setScale(1.35);
            m_woodBtn->setPreferredSize(CCSizeMake(1536, 108));
        }
        m_woodNode->setPositionX(0);
        m_woodBtn->setEnabled(false);
    }else if (mlv>=FunBuildController::getInstance()->building_base_k3 && mlv>=FunBuildController::getInstance()->building_base_k4) {
    }
    else if (mlv>=FunBuildController::getInstance()->building_base_k3 || mlv>=FunBuildController::getInstance()->building_base_k4) {
        if (mlv>=FunBuildController::getInstance()->building_base_k3) {
            m_stoneBtn->setEnabled(false);
            m_stoneNode->setVisible(false);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_ironNode->setPositionX(512);
                m_ironBtn->setPreferredSize(CCSizeMake(512, 108));
            }
            else {
                m_ironNode->setPositionX(213);
                m_ironBtn->setPreferredSize(CCSizeMake(213, 54));
            }
        }
        else {
            m_ironBtn->setEnabled(false);
            m_ironNode->setVisible(false);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_stoneNode->setPositionX(512);
                m_stoneBtn->setPreferredSize(CCSizeMake(512, 108));
            }
            else {
                m_stoneNode->setPositionX(213);
                m_stoneBtn->setPreferredSize(CCSizeMake(213, 54));
            }
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            m_woodBtn->setPreferredSize(CCSizeMake(512, 108));
            m_foodBtn->setPreferredSize(CCSizeMake(512, 108));
            m_woodNode->setPositionX(-512);
            m_foodNode->setPositionX(0);
        }
        else {
            m_woodBtn->setPreferredSize(CCSizeMake(213, 54));
            m_foodBtn->setPreferredSize(CCSizeMake(213, 54));
            m_woodNode->setPositionX(-213);
            m_foodNode->setPositionX(0);
        }
    }
    else {
        m_ironBtn->setEnabled(false);
        m_stoneBtn->setEnabled(false);
        m_ironNode->setVisible(false);
        m_stoneNode->setVisible(false);
        
        if (CCCommonUtils::isIosAndroidPad()) {
            m_woodBtn->setPreferredSize(CCSizeMake(768, 108));
            m_foodBtn->setPreferredSize(CCSizeMake(768, 108));
            m_woodNode->setPositionX(-384);
            m_foodNode->setPositionX(384);
        }
        else {
            m_woodBtn->setPreferredSize(CCSizeMake(320, 54));
            m_foodBtn->setPreferredSize(CCSizeMake(320, 54));
            m_woodNode->setPositionX(-160);
            m_foodNode->setPositionX(160);
        }
    }
    
    updateResInfo(NULL);
    
    setTitleName(_lang("104904").c_str());
    m_resList = &(ToolController::getInstance()->m_typeTools[ITEM_TYPE_RES]);
    
    updateInfo();
    
    return true;
}

void UseResToolView::updateInfo()
{
    m_woodBtn->setEnabled(true);
    m_stoneBtn->setEnabled(true);
    m_foodBtn->setEnabled(true);
    m_ironBtn->setEnabled(true);
//    m_silverBtn->setEnabled(true);
    
    if (m_resType == Wood) {
        m_woodBtn->setEnabled(false);
    }
    else if (m_resType == Stone) {
        m_stoneBtn->setEnabled(false);
    }
    else if (m_resType == Food) {
        m_foodBtn->setEnabled(false);
    }
    else if (m_resType == Iron) {
        m_ironBtn->setEnabled(false);
    }else if(m_resType == 13){
        m_woodBtn->setEnabled(false);
    }
//    else if (m_resType == Silver) {
//        m_silverBtn->setEnabled(false);
//    }
    
    int t_cityLv = FunBuildController::getInstance()->getMainCityLv();
    m_curList.clear();
    for (int i = 0; i<m_resList->size(); i++) {
        auto& info = ToolController::getInstance()->getToolInfoById((*m_resList)[i]);
        if (info.type2 == m_resType) {
            if (info.price > 0 && info.limitLv<=t_cityLv) {
                m_curList.push_back(info.itemId);
            }
            else if (info.price<=0 && info.getCNT()>0) {
                m_curList.push_back(info.itemId);
            }
        }
    }
    
    m_tabView->reloadData();
}

void UseResToolView::refreashData(CCObject* p)
{
    int t_cityLv = FunBuildController::getInstance()->getMainCityLv();
    m_curList.clear();
    for (int i = 0; i<m_resList->size(); i++) {
        auto& info = ToolController::getInstance()->getToolInfoById((*m_resList)[i]);
        if (info.type2 == m_resType) {
            if (info.price > 0 && info.limitLv<=t_cityLv) {
                m_curList.push_back(info.itemId);
            }
            else if (info.price<=0 && info.getCNT()>0) {
                m_curList.push_back(info.itemId);
            }
        }
    }
    CCPoint offset = m_tabView->getContentOffset();
    m_tabView->reloadData();
    offset.y = MAX(m_tabView->minContainerOffset().y, MIN(m_tabView->maxContainerOffset().y, offset.y));
    m_tabView->setContentOffset(offset);
    this->updateResInfo(NULL);
}

void UseResToolView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseResToolView::updateResInfo), MSG_CITY_RESOURCES_UPDATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseResToolView::refreashData), MSG_REFREASH_TOOL_DATA, NULL);
}

void UseResToolView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CITY_RESOURCES_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_TOOL_DATA);
    CCNode::onExit();
}

void UseResToolView::updateResInfo(CCObject* p)
{
    m_woodLabel->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lWood));
    m_stoneLabel->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lStone));
    m_foodLabel->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lFood));
    m_ironLabel->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lIron));
//    m_silverLabel->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lMoney));
    if(m_resType == 13){
        int currt = WorldController::getInstance()->currentStamine;
        int total = GlobalData::shared()->worldConfig.stamineMax;
        m_woodLabel->setString(std::string("") + CC_ITOA(currt) + "/" + CC_ITOA(total));
    }
}

void UseResToolView::onClickWoodBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_resType == Wood) {
        return;
    }
    m_resType = Wood;
    updateInfo();
}

void UseResToolView::onClickStoneBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_resType == Stone) {
        return;
    }
    m_resType = Stone;
    updateInfo();
}

void UseResToolView::onClickFoodBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_resType == Food) {
        return;
    }
    m_resType = Food;
    updateInfo();
}

void UseResToolView::onClickIronBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_resType == Iron) {
        return;
    }
    m_resType = Iron;
    updateInfo();
}

//void UseResToolView::onClickSilverBtn(CCObject * pSender, Control::EventType pCCControlEvent)
//{
//    if (m_resType == Silver) {
//        return;
//    }
//    m_resType = Silver;
//    updateInfo();
//}

SEL_CCControlHandler UseResToolView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickWoodBtn", UseResToolView::onClickWoodBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickStoneBtn", UseResToolView::onClickStoneBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickFoodBtn", UseResToolView::onClickFoodBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickIronBtn", UseResToolView::onClickIronBtn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSilverBtn", UseResToolView::onClickSilverBtn);
    return NULL;
}

bool UseResToolView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodBtn", CCControlButton*, this->m_woodBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneBtn", CCControlButton*, this->m_stoneBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodBtn", CCControlButton*, this->m_foodBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironBtn", CCControlButton*, this->m_ironBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_silverBtn", CCControlButton*, this->m_silverBtn);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_silverLabel", CCLabelIF*, this->m_silverLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodLabel", CCLabelIF*, this->m_woodLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneLabel", CCLabelIF*, this->m_stoneLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodLabel", CCLabelIF*, this->m_foodLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironLabel", CCLabelIF*, this->m_ironLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodNode", CCNode*, this->m_woodNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodNode", CCNode*, this->m_foodNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironNode", CCNode*, this->m_ironNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneNode", CCNode*, this->m_stoneNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemIcon", CCSprite*, this->m_itemIcon);
    return false;
}

void UseResToolView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
}

cocos2d::CCSize UseResToolView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1476, 290);
    }
    return CCSize(610, 160);
}

CCTableViewCell* UseResToolView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx)
{
    if(idx >= m_curList.size()){
        return NULL;
    }
    
    UseResToolCell* cell = (UseResToolCell*)table->dequeueGrid();
    if(idx < m_curList.size()){
        if(cell){
            cell->setData(m_resType, m_curList[idx]);
        }else{
            cell = UseResToolCell::create(m_resType, m_curList[idx],m_infoList);;
        }
//        cell->setTag(100);
    }
    return cell;
}

ssize_t UseResToolView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = ceil(m_curList.size() / numPerRow);
    return num;
}

unsigned int UseResToolView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return numPerRow;
}

void UseResToolView::doWhenClose()
{
}

UseResToolCell* UseResToolCell::create(int resType, int itemId, CCNode *touchNode)
{
    auto ret = new UseResToolCell();
    if (ret && ret->init(resType, itemId,touchNode)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UseResToolCell::init(int resType, int itemId, CCNode *touchNode)
{
    bool ret = true;
    CCBLoadFile("UseToolCell",this,this);
    mTouchNode = touchNode;
    m_buyBtn->setSwallowsTouches(false);
    m_useBtn->setSwallowsTouches(false);
    m_waitInterface = NULL;
    setData(resType,itemId);
    return ret;
}

void UseResToolCell::setData(int resType, int itemId)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_itemId = itemId;
    m_resType = resType;
    m_des2Label->setString("");
    ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
    
    m_nameLabel->setString(toolInfo.getName().c_str());
    m_desLabel->setString(_lang(toolInfo.des).c_str());
    m_numLabel->setString(CC_ITOA(toolInfo.getCNT()));
    m_price = toolInfo.price;
    
    CCCommonUtils::createGoodsIcon(toolInfo.itemId, m_picNode, CCSize(80, 80));
    
//    auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(toolInfo.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//    if (pic) {
//        auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//        CCCommonUtils::setSpriteMaxSize(iconBg, 80);
//        m_picNode->addChild(iconBg);
//        
//        CCCommonUtils::setSpriteMaxSize(pic, 80);
//        m_picNode->addChild(pic);
//    }
    
    m_inBtnGoldNum->setString(CC_CMDITOA(m_price));
    CCCommonUtils::setButtonTitle(m_useBtn, _lang("102137").c_str());
    m_btnMsgLabel->setString(_lang("104906").c_str());
    
    if (toolInfo.getCNT()>0) {
        m_buyNode->setVisible(false);
        m_buyBtn->setEnabled(false);
        m_useBtn->setVisible(true);
        m_useBtn->setEnabled(true);
    }
    else {
        m_buyNode->setVisible(true);
        m_buyBtn->setEnabled(true);
        m_useBtn->setVisible(false);
        m_useBtn->setEnabled(false);
    }
}

void UseResToolCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);

//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseResToolCell::refreashData), MSG_TOOL_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseResToolCell::addWaitInterface), "addWaitInterface", NULL);
}

void UseResToolCell::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "addWaitInterface");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOOL_CHANGE);
    setTouchEnabled(false);
    CCNode::onExit();
}
bool UseResToolCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(mTouchNode, pTouch)){
        m_touchPoint =pTouch->getLocation();
        return true;
    }
    return false;
}
void UseResToolCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(ccpDistance(pTouch->getLocation(), m_touchPoint)<50){
        if (m_buyBtn->isEnabled() == true && isTouchInside(m_buyBtn,pTouch))
        {
            onClickBuyBtn(NULL,Control::EventType::TOUCH_DOWN);
        }else if(m_useBtn->isEnabled()==true && isTouchInside(m_useBtn,pTouch)){
            onClickUseBtn(NULL,Control::EventType::TOUCH_DOWN);
        }
    }
}
void UseResToolCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}
void UseResToolCell::refreashData(CCObject* obj)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
}
void UseResToolCell::addWaitInterface(CCObject* obj)
{
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("addWaitInterface");
    m_waitInterface=GameController::getInstance()->showWaitInterface(mTouchNode);
    m_waitInterface->setPosition(ccp(mTouchNode->getContentSize().width/2, mTouchNode->getContentSize().height/2));
}

SEL_CCControlHandler UseResToolCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUseBtn", UseResToolCell::onClickUseBtn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn", UseResToolCell::onClickBuyBtn);
    return NULL;
}

void UseResToolCell::onClickUseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onUseTool();
}

void UseResToolCell::onYes()
{
    ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(m_itemId);
    if(toolInfo.itemId == ITEM_ADD_STAMINE){
        if(WorldController::getInstance()->currentStamine >= GlobalData::shared()->worldConfig.stamineMax){
            CCCommonUtils::flyHint("", "", _lang("103746"));
            return ;
        }
    }
    m_waitInterface=GameController::getInstance()->showWaitInterface(mTouchNode);
    m_waitInterface->setPosition(ccp(mTouchNode->getContentSize().width/2, mTouchNode->getContentSize().height/2));
    ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(UseResToolCell::onUseTool)),0,true,false,"UseResToolView");
}

void UseResToolCell::onUseTool()
{
    if(!this->getParent()){
        return;
    }
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(m_itemId);
    if(toolInfo.itemId == ITEM_ADD_STAMINE){
        if(WorldController::getInstance()->currentStamine >= GlobalData::shared()->worldConfig.stamineMax){
            CCCommonUtils::flyHint("", "", _lang("103746"));
            return ;
        }
    }
    if(toolInfo.getCNT()==1){
        m_waitInterface=GameController::getInstance()->showWaitInterface(mTouchNode);
        m_waitInterface->setPosition(ccp(mTouchNode->getContentSize().width/2, mTouchNode->getContentSize().height/2));
        ToolController::getInstance()->useTool(m_itemId,1,true);
    }else{
        PopupViewController::getInstance()->addPopupView(ToolNumSelectView::create(m_itemId), false);
    }

    if (toolInfo.getCNT()<=0) {
        m_useBtn->setVisible(false);
        m_useBtn->setEnabled(false);
        if (toolInfo.price>0) {
            m_buyNode->setVisible(true);
            m_buyBtn->setEnabled(true);
        }
        else {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
        }
    }
}

void UseResToolCell::onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_waitInterface!=NULL){
        return;
    }
    YesNoDialog::showButtonAndGold(_lang("102371").c_str(), CCCallFunc::create(this, callfunc_selector(UseResToolCell::onYes)),m_btnMsgLabel->getString().c_str(), m_price);
//    onYes();
}

bool UseResToolCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useBtn", CCControlButton*, this->m_useBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_des2Label", CCLabelIF*, m_des2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtn", CCControlButton*, this->m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyNode", CCNode*, m_buyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, m_inBtnGoldNum);
    return false;
}