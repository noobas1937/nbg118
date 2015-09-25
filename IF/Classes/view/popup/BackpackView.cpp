//
//  BackpackView.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-24.
//
//

#include "BackpackView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "ToolController.h"
#include "UseBackpackItemView.h"
#include "FunBuildController.h"
#include "ChangePicView.h"
#include "ChangeNickNameView.h"
#include "WorldMapView.h"

const float numPerRow = 1.0;
BackpackView* BackpackView::create(StoreType storeType, StoreResourceType storeResourceType){
    BackpackView* ret = new BackpackView(storeType, storeResourceType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool BackpackView::init()
{
    if (!CCNode::init()) {
        return false;
    }
    setCleanFunction([](){
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    auto tmpCCB = CCBLoadFile("BackpackView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    CCCommonUtils::setButtonTitle(m_warBtn, _lang("104902").c_str());
    CCCommonUtils::setButtonTitle(m_spdBtn, _lang("104903").c_str());
    CCCommonUtils::setButtonTitle(m_resBtn, _lang("104904").c_str());
    CCCommonUtils::setButtonTitle(m_othBtn, _lang("104905").c_str());
    
    m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
    m_tabView->setTouchPriority(Touch_Popup);
    m_infoList->addChild(m_tabView);

    m_minNum = (m_infoList->getContentSize().height/110)*6;
    m_type = 1;
    updateInfo();
    return true;
}

void BackpackView::showList(bool flag){
    if(flag){
        this->setVisible(true);
        m_page = 1;
        updateInfo();
    }else{
        m_curList.clear();
        m_tabView->reloadData();
        this->setVisible(false);
    }
}

void BackpackView::updateInfo()
{
    if(!this->isVisible()){
        return;
    }
    m_warBtn->setEnabled(true);
    m_spdBtn->setEnabled(true);
    m_resBtn->setEnabled(true);
    m_othBtn->setEnabled(true);
    
    m_dataList = NULL;
    if (m_page == 1) {
        m_dataList = &(ToolController::getInstance()->m_warTools);
        m_warBtn->setEnabled(false);
    }
    else if (m_page == 2) {
        m_dataList = &(ToolController::getInstance()->m_spdTools);
        m_spdBtn->setEnabled(false);
    }
    else if (m_page == 3) {
        m_dataList = &(ToolController::getInstance()->m_resTools);
        m_resBtn->setEnabled(false);
    }
    else if (m_page == 4) {
        m_dataList = &(ToolController::getInstance()->m_othTools);
        m_othBtn->setEnabled(false);
    }
    m_cityLv = FunBuildController::getInstance()->getMainCityLv();
    m_curList.clear();
    for (int i=0; i<m_dataList->size(); i++) {
        int tmpToolId = (*m_dataList)[i];
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
        if (m_type == 1 && info.getCNT() > 0) {
            m_curList.push_back(tmpToolId);
        }
        else if (m_type == 0 && info.price>0 && info.limitLv<=m_cityLv){
            m_curList.push_back(tmpToolId);
        }
    }
    int num = m_curList.size();
    if(num>=m_minNum){
        m_tabView->setTouchEnabled(true);
    }else{
        m_tabView->setTouchEnabled(false);
        int temp = m_minNum - num + 1;
        for (int i=0; i<temp; i++) {
            m_curList.push_back(-1);
        }
    }
    m_tabView->reloadData();
}

void BackpackView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){

}

cocos2d::CCSize BackpackView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(103, 110);
}

CCTableViewCell* BackpackView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_curList.size()){
        return NULL;
    }
    BackpackCell* cell = (BackpackCell*)table->dequeueGrid();
    if(idx < m_curList.size()){
        if(cell){
            cell->setData(m_curList[idx]);
        }else{
            cell = BackpackCell::create(m_curList[idx]);
        }
    }
    return cell;
}

ssize_t BackpackView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return m_curList.size();
}

unsigned int BackpackView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 6;
}

void BackpackView::refreashData(CCObject* obj)
{
    m_curList.clear();
    for (int i=0; i<m_dataList->size(); i++) {
        int tmpToolId = (*m_dataList)[i];
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
        if (m_type == 1 && info.getCNT() > 0) {
            m_curList.push_back(tmpToolId);
        }
        else if (m_type == 0 && info.price>0 && info.limitLv<=m_cityLv){
            m_curList.push_back(tmpToolId);
        }
    }
    int num = m_curList.size();
    if(num>=m_minNum){
        m_tabView->setTouchEnabled(true);
    }else{
        m_tabView->setTouchEnabled(false);
        int temp = m_minNum - num + 1;
        for (int i=0; i<temp; i++) {
            m_curList.push_back(-1);
        }
    }
    m_tabView->reloadData();
}



void BackpackView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("102160").c_str());
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BackpackView::refreashData), MSG_REFREASH_TOOL_DATA, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BackpackView::refreashData), MSG_TOOL_CHANGE, NULL);
}

void BackpackView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_TOOL_DATA);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOOL_CHANGE);
    CCNode::onExit();
}

BackpackView::~BackpackView()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_REMOVE,CCString::create(CC_ITOA(PVT_store)));
}

SEL_CCControlHandler BackpackView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickWarBtn", BackpackView::onClickWarBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSpdBtn", BackpackView::onClickSpdBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickResBtn", BackpackView::onClickResBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickOthBtn", BackpackView::onClickOthBtn);
    return NULL;
}

bool BackpackView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_warBtn", CCControlButton*, this->m_warBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spdBtn", CCControlButton*, this->m_spdBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resBtn", CCControlButton*, this->m_resBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_othBtn", CCControlButton*, this->m_othBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    return false;
}


void BackpackView::onClickWarBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_page != 1) {
        m_page = 1;
        updateInfo();
    }
}

void BackpackView::onClickSpdBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_page != 2) {
        m_page = 2;
        updateInfo();
    }
}

void BackpackView::onClickResBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_page != 3) {
        m_page = 3;
        updateInfo();
    }
}

void BackpackView::onClickOthBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_page != 4) {
        m_page = 4;
        updateInfo();
    }
}


BackpackCell *BackpackCell::create(int itemId){
    BackpackCell *ret = new BackpackCell(itemId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BackpackCell::init(){
    this->setContentSize(CCSize(102,102));
    m_flagNode = CCLoadSprite::createSprite("UI_dikuang.png");
    m_flagNode->setPosition(ccp(50, 50));
    this->addChild(m_flagNode);
    
    m_numNode = CCNode::create();
    this->addChild(m_numNode);
    
    CCSprite* numBg = CCLoadSprite::createSprite("Ui_shuzi_frame.png");
    numBg->setPosition(ccp(87, 82));
    m_numNode->addChild(numBg);
    
    m_numTxt= CCLabelIF::create("0", 20);
    m_numTxt->setAnchorPoint(ccp(0.5, 0.5));
    m_numTxt->setPosition(ccp(87,85));
    m_numTxt->setColor(ccWHITE);
    m_numNode->addChild(m_numTxt);
    if(m_itemId!=-1){
        setData(m_itemId);
        m_numNode->setVisible(true);
    }else{
        m_numNode->setVisible(false);
    }
    return true;
}

void BackpackCell::setData(int itemId){
    m_itemId = itemId;
    m_flagNode->removeAllChildrenWithCleanup(true);
    if(m_itemId==-1){
        m_numNode->setVisible(false);
        return ;
    }
    m_numNode->setVisible(true);
    CCCommonUtils::createGoodsIcon(itemId, m_flagNode, CCSize(92, 92),m_numTxt);
//    ToolInfo& info = ToolController::getInstance()->getToolInfoById(itemId);
//    m_numTxt->setString(CC_ITOA(info.getCNT()));
//    m_flagNode->removeAllChildren();
//    auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//    pic->setPosition(ccp(51, 51));
//    if (pic) {
//        auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info.color).c_str());
//        iconBg->setPosition(ccp(51, 51));
//        CCCommonUtils::setSpriteMaxSize(iconBg, 92,true);
//        m_flagNode->addChild(iconBg);
//        
//        CCCommonUtils::setSpriteMaxSize(pic, 92,true);
//        m_flagNode->addChild(pic);
//    }
}

void BackpackCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void BackpackCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool BackpackCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    return true;
}

void BackpackCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void BackpackCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(isTouchInside(m_flagNode, pTouch) && fabs(pTouch->getLocation().y - m_touchPos.y)<30 && m_itemId!=-1){
        
        if(m_itemId == ITEM_RENAME) {
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
            if (info.getCNT() > 0) {
                PopupViewController::getInstance()->addPopupView(ChangeNickNameView::create(info.uuid),true,true);
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
        }
        else if(m_itemId == ITEM_CHANGE_PIC) {
            PopupViewController::getInstance()->addPopupView(ChangePicView::create(),true,true);
        }else{
            PopupViewController::getInstance()->addPopupView(UseBackpackItemView::create(m_itemId),true,true);
        }
    }
}
