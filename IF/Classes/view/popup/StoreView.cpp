//
//  StoreView.cpp
//  IF
//
//  Created by fubin on 14-2-27.
//
//

#include "StoreView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "UseCDToolView.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "ChangeNickNameView.h"
#include "WorldMapView.h"
#include "ChangePicView.h"
#include "BuildingAddOutPutCommand.h"

const float numPerRow = 1.0;
StoreView* StoreView::create(StoreType storeType, StoreResourceType storeResourceType){
    StoreView* ret = new StoreView(storeType, storeResourceType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool StoreView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    auto tmpCCB = CCBLoadFile("StoreView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    setTitleName(_lang("102160").c_str());
    
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGMaxHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
//    int bgcount = newBgHeight/100+1;
//    for (int i = 0; i<=bgcount; i++) {
//        auto pic = CCLoadSprite::createSprite("technology_09.png");
//        pic->setPositionY(-i*100);
//        pic->setScaleX(0.98);
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            pic->setPositionY((i-3)*100);
//            pic->setScaleX(2.4f);
//            pic->setAnchorPoint(ccp(0, 1));
//        }
//        m_BGNode->addChild(pic);
//    }
    
    m_BGNode->setPreferredSize(Size(tmpCCB->getContentSize().width, tmpCCB->getContentSize().height + addHeight)); // guo.jiang
    
    CCCommonUtils::setButtonTitle(m_storeBtn, _lang("104900").c_str());
    CCCommonUtils::setButtonTitle(m_bagBtn, _lang("104901").c_str());
//    CCCommonUtils::setButtonTitle(m_warBtn, _lang("104902").c_str());
//    CCCommonUtils::setButtonTitle(m_spdBtn, _lang("108522").c_str());
//    CCCommonUtils::setButtonTitle(m_resBtn, _lang("104904").c_str());
//    CCCommonUtils::setButtonTitle(m_othBtn, _lang("104905").c_str());
    
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    
    m_cityLv = FunBuildController::getInstance()->getMainCityLv();
    
//    m_packView = BackpackView::create();
//    this->addChild(m_packView,100000);
//    m_packView->setVisible(false);
    if(m_type==STORE_TYPE_GOTO_STORE){
        m_type=0;
    }else{
        int count = ToolController::getInstance()->getTotalCountInBag();
        if(count>0){
            m_type = 1;
        }else{
            m_type = 0;
        }
    }
    ToolController::getInstance()->m_currentUseItemId=0;
    m_bagView = StoreBagView::create();
    m_mallView = StoreMallView::create();
    float dy = 852-170;//fusheng m_bagNode的坐标为离左上角 170
//    float dy = 0;
    if(CCCommonUtils::isIosAndroidPad())
        dy = 2048-340;
    m_mallView->setPositionY(-dy);
    m_bagView->setPositionY(-dy);
    


    m_bagBtn->setZoomOnTouchDown(false);
    m_storeBtn->setZoomOnTouchDown(false);
//    m_mallView->setVisible(false);
//    m_bagView->setVisible(false);
    m_page = 2;

    updateInfo();
    
    return true;
}
CCRect StoreView::getBagTabRect(){
    CCSize size = m_bagBtn->getContentSize();
    CCPoint pos = m_bagBtn->convertToWorldSpace(ccp(size.width*0.5,size.height*0.5));
    return CCRect(pos.x, pos.y, size.width, size.height);
}
void StoreView::updateInfo()
{
    if (m_type==0) {
        m_storeBtn->setEnabled(false);
        m_bagBtn->setEnabled(true);
        
        this->m_bagNode->removeAllChildren();
        m_mallNode->addChild(m_mallView);
    }
    else {
        this->m_mallNode->removeAllChildren();
        m_bagNode->addChild(m_bagView);
        m_storeBtn->setEnabled(true);
        m_bagBtn->setEnabled(false);
    }
    
    CCCommonUtils::setSpriteGray(m_bagIcon, m_bagBtn->isEnabled());
    
    CCCommonUtils::setSpriteGray(m_storeIcon, m_storeBtn->isEnabled());
//    
//    m_warBtn->setEnabled(true);
//    m_spdBtn->setEnabled(true);
//    m_resBtn->setEnabled(true);
//    m_othBtn->setEnabled(true);
//    
//    m_dataList = NULL;
//    if (m_page == 1) {
//        m_dataList = &(ToolController::getInstance()->m_warTools);
//        m_warBtn->setEnabled(false);
//    }
//    else if (m_page == 2) {
//        m_dataList = &(ToolController::getInstance()->m_allTools);
//        m_spdBtn->setEnabled(false);
//    }
//    else if (m_page == 3) {
//        m_dataList = &(ToolController::getInstance()->m_resTools);
//        m_resBtn->setEnabled(false);
//    }
//    else if (m_page == 4) {
//        m_dataList = &(ToolController::getInstance()->m_othTools);
//        m_othBtn->setEnabled(false);
//    }
//    
//    m_curList.clear();
//    for (int i=0; i<m_dataList->size(); i++) {
//        int tmpToolId = (*m_dataList)[i];
//        ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
//        if (m_type == 1 && info.cnt > 0) {
//            m_curList.push_back(tmpToolId);
//        }
//        else if (m_type == 0 && info.price>0 && info.limitLv<=m_cityLv){
//            m_curList.push_back(tmpToolId);
//        }
//    }
//        if(m_page == 2){
//            sortDataByOrderNum();
//        }
//        m_tabView->reloadData();
}

void StoreView::refreashData()
{
    return;
    CCPoint curr = m_tabView->getContentOffset();
    CCPoint max = m_tabView->maxContainerOffset();
    CCPoint min = m_tabView->minContainerOffset();
//    int total = m_curList.size();
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
    if(m_page == 2){
        sortDataByOrderNum();
    }
    m_tabView->reloadData();
    if (m_type==0) {
        if (curr.y > max.y) {
            m_tabView->setContentOffset(m_tabView->maxContainerOffset());
        }else
        {
            curr.y += m_tabView->minContainerOffset().y - min.y;
            m_tabView->setContentOffset(curr);
        }
    }else{
//        if (curr.y > max.y) {
//            m_tabView->setContentOffset(m_tabView->maxContainerOffset());
//        }else
//        {
            curr.y += m_tabView->minContainerOffset().y - min.y;
            if(curr.y>m_tabView->maxContainerOffset().y){
                curr.y = m_tabView->maxContainerOffset().y;
            }
            if(curr.y<m_tabView->minContainerOffset().y){
                curr.y = m_tabView->minContainerOffset().y;
            }
            m_tabView->setContentOffset(curr);
//        }

    }
}
bool StoreView::sortByUse(int itemId1, int itemId2){
    auto& info1 = ToolController::getInstance()->getToolInfoById(itemId1);
    auto& info2 = ToolController::getInstance()->getToolInfoById(itemId2);
    return (info1.use>info2.use);

}
void StoreView::sortData(){
    if (m_curList.size()>0) {
        std::sort(m_curList.begin(),m_curList.end(),sortByUse);
    }
}
bool StoreView::sortByUseOder(int itemId1, int itemId2){
    auto& info1 = ToolController::getInstance()->getToolInfoById(itemId1);
    auto& info2 = ToolController::getInstance()->getToolInfoById(itemId2);
    return (info1.orderNum<info2.orderNum);
    
}
void StoreView::sortDataByOrderNum(){
    if (m_curList.size()>0) {
        std::sort(m_curList.begin(),m_curList.end(),sortByUseOder);
    }
}

CCNode* StoreView::getGuideNode(string _key)
{
    if (_key=="GD_first" || _key=="GD_use") {
        if(m_bagView && m_bagNode->getChildrenCount()>0) {
            return m_bagView->getGuideNode(_key);
        }
        return NULL;
    }
    return NULL;
}

CCSize StoreView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_curList.size()){
        return CCSizeZero;
    }
    if (m_type==0) {
        auto& info = ToolController::getInstance()->getToolInfoById(m_curList[idx]);
        if (info.use==1) {
            auto rewards = ToolController::getInstance()->findToolRewards(info.itemId);
            if (rewards && rewards->size()>0) {
                int tmp = 197+rewards->size()*70+20;
                return CCSize(600, tmp);
            } else {
                return CCSize(600,197);
            }
        } else {
            return CCSize(600,197);
        }
    }
    else {
        return CCSize(600, 197);
    }
}
CCSize StoreView::cellSizeForTable(CCTableView *table)
{
    return CCSize(600, 197);
}
CCTableViewCell* StoreView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_curList.size()){
        return NULL;
    }
    
    ToolCell* cell = (ToolCell*)table->dequeueCell();
    if(idx < m_curList.size()){
        if(cell){
            cell->setData(m_curList[idx],m_type);
        }else{
            cell = ToolCell::create(m_curList[idx], m_type, m_infoList);
        }
    }
    return cell;
}
ssize_t StoreView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_curList.size() / numPerRow);
    return num;
}
void StoreView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void StoreView::onEnter()
{
    CCNode::onEnter();
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StoreView::refreashData), MSG_REFREASH_TOOL_DATA, NULL);
}

void StoreView::onExit()
{
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_TOOL_DATA);
    CCNode::onExit();
}

StoreView::~StoreView()
{
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_REMOVE,CCString::create(CC_ITOA(PVT_store)));
    //由于采取了先创建后addchild的方式，会导致内存泄漏，这里做个特殊处理
    m_mallView->removeAllChildren();
    m_bagView->removeAllChildren();
}

SEL_CCControlHandler StoreView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", StoreView::onCloseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickStoreBtn", StoreView::onClickStoreBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBagBtn", StoreView::onClickBagBtn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickWarBtn", StoreView::onClickWarBtn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSpdBtn", StoreView::onClickSpdBtn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickResBtn", StoreView::onClickResBtn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickOthBtn", StoreView::onClickOthBtn);
    return NULL;
}

bool StoreView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_storeBtn", CCControlButton*, this->m_storeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagBtn", CCControlButton*, this->m_bagBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_warBtn", CCControlButton*, this->m_warBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spdBtn", CCControlButton*, this->m_spdBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resBtn", CCControlButton*, this->m_resBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_othBtn", CCControlButton*, this->m_othBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagNode", CCNode*, this->m_bagNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mallNode", CCNode*, this->m_mallNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BGNode", CCScale9Sprite*, this->m_BGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagIcon", CCSprite*, this->m_bagIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_storeIcon", CCSprite*, this->m_storeIcon);
    
    
    return false;
}

void StoreView::onCloseClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void StoreView::onClickStoreBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_type != 0) {
        m_type = 0;
        //m_packView->showList(false);
        updateInfo();
    }
}

void StoreView::onClickBagBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    //m_packView->showList(true);
    if (m_type != 1) {
        m_type = 1;
        updateInfo();
    }
}

void StoreView::onClickWarBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_page != 1) {
        m_page = 1;
        updateInfo();
    }
}

void StoreView::onClickSpdBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_page != 2) {
        m_page = 2;
        updateInfo();
    }
}

void StoreView::onClickResBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_page != 3) {
        m_page = 3;
        updateInfo();
    }
}

void StoreView::onClickOthBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_page != 4) {
        m_page = 4;
        updateInfo();
    }
}

ToolCell* ToolCell::create(int itemId, int type, CCNode* touchNode)
{
    auto ret = new ToolCell();
    if (ret && ret->init(itemId, type, touchNode)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ToolCell::init(int itemId, int type, CCNode* touchNode)
{
    bool ret = true;
    CCBLoadFile("ToolCell",this,this);
    m_waitInterface = NULL;
    m_touchNode = touchNode;
    setData(itemId, type);
    return ret;
}

void ToolCell::setData(int itemId, int type)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_itemId = itemId;
    m_type = type;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(itemId);
    m_price = info.price;
    m_nameLabel->setString(info.getName().c_str());
    m_desLabel->setString(_lang(info.des).c_str());
    m_priceLabel->setString(CC_CMDITOA(info.price));
    string numStr = _lang_1("104907", CC_CMDITOA(info.getCNT()).c_str());
    m_numLabel->setString(numStr);
    CCCommonUtils::setButtonTitle(m_useBtn, _lang("102137").c_str());
    m_inBtnLabel->setString(_lang("102148").c_str());
    
    m_picNode->removeAllChildren();
    CCCommonUtils::createGoodsIcon(info.itemId, m_picNode, CCSize(92, 92));
//    auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//    if (pic) {
//        auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info.color).c_str());
//        CCCommonUtils::setSpriteMaxSize(iconBg, 92,true);
//        m_picNode->addChild(iconBg);
//        
//        CCCommonUtils::setSpriteMaxSize(pic, 92,true);
//        m_picNode->addChild(pic);
//    }
    
    m_priceNode->setVisible(true);
    m_useBtn->setVisible(true);
    if (m_type==0) {
        m_useBtn->setVisible(false);
    }
    else {
        m_priceNode->setVisible(false);
        if(info.use == 0){
            m_useBtn->setVisible(false);
        }
    }
    
    m_numLabel->setColor(ccBLACK);
    m_storeUseBtn->setVisible(false);
    m_unUseBtn->setVisible(false);
    //m_numLabel->setPositionY(23);
    m_mainNode->setPositionY(0);
    m_rewardNode->removeAllChildren();
    int curY = 30;
    m_itemBg->setContentSize(CCSize(604,180));
    int offY = 0;
    if (m_type==0 && info.use==1) {
        auto rewards = ToolController::getInstance()->findToolRewards(info.itemId);
        if (rewards) {
            for (int i=0; i<rewards->size(); i++) {
                auto& redInfo = (*rewards)[i];
                auto tRedCell = ToolRewardCell::create(redInfo.resId, redInfo.itemId, redInfo.num);
                tRedCell->setPosition(ccp(10, curY));
                curY += 70;
                m_rewardNode->addChild(tRedCell);
            }
            int maxH = 180+rewards->size()*70+20;
            //m_itemBg->setScaleY(maxH/180.0);
            m_itemBg->setContentSize(CCSize(604,maxH));
            offY = maxH-180;
        }
        
        m_mainNode->setPositionY(offY);
        //m_numLabel->setPositionY(13);
        if (info.getCNT()>0) {
            m_storeUseBtn->setVisible(true);
        }else {
            m_unUseBtn->setVisible(true);
        }
        m_numLabel->setColor(ccWHITE);
    }
    
    if (m_type == 1 && info.getCNT() <= 0) {
        m_priceNode->setVisible(true);
        m_useBtn->setVisible(false);
    }
}

void ToolCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ToolCell::refreashData), MSG_TOOL_CHANGE, NULL);
}

void ToolCell::onExit() {
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOOL_CHANGE);
    CCNode::onExit();
}

void ToolCell::refreashData(CCObject* obj)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    string numStr = _lang_1("104907", CC_CMDITOA(info.getCNT()).c_str());
    m_numLabel->setString(numStr);
    
    if (info.getCNT() <= 0) {
        m_priceNode->setVisible(true);
        m_useBtn->setVisible(false);
        
        if (m_type == 1 && info.price<=0) {//背包内，使用完不可购买的道具
        }
        else if (m_type == 1) {
            m_type = 2;
        }
        else if (m_type == 0 && info.use==1) {
            m_unUseBtn->setVisible(true);
            m_storeUseBtn->setVisible(false);
        }
    }
}

SEL_CCControlHandler ToolCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUseBtn", ToolCell::onClickUseBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn", ToolCell::onClickBuyBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickStoreUseBtn", ToolCell::onClickStoreUseBtn);
    return NULL;
}

void ToolCell::onClickStoreUseBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    if(!m_clickInSide) return ;
     onUseTool();
}

bool ToolCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_clickInSide = false;
    if (isTouchInside(m_touchNode,pTouch)) {
        m_clickInSide = true;
        m_buyBtn->setEnabled(true);
        m_storeUseBtn->setEnabled(true);
        m_useBtn->setEnabled(true);
        if (isTouchInside(m_useBtn,pTouch) || isTouchInside(m_storeUseBtn,pTouch)) {
            return true;
        }
    }else{
        m_buyBtn->setEnabled(false);
        m_storeUseBtn->setEnabled(false);
        m_useBtn->setEnabled(false);
    }
    return false;
}

void ToolCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
//    if (isTouchInside(m_useBtn,pTouch)) {
//        if (m_useBtn->isVisible()) {
//            onUseTool();
//        }
//        else {
//            if (m_type==0) {
//                ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(ToolCell::onRetBuyTool)));
//            }
//            else if (m_type == 2) {
//                onYes();
//            }
//        }
//    }
//    else {
//        if (m_storeUseBtn->isVisible()) {
//            onUseTool();
//        }
//    }
}

void ToolCell::onClickUseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(!m_clickInSide) return ;
    onUseTool();
}

void ToolCell::onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(!m_clickInSide) return ;
    if (m_type==0) {
        //ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(ToolCell::onRetBuyTool)));
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        YesNoDialog::showButtonAndGold(_lang("104919").c_str(), CCCallFunc::create(this, callfunc_selector(ToolCell::onBuyTool)), _lang("102148").c_str(), info.price);
    }
    else if (m_type == 2) {
        onYes();
    }
}
void ToolCell::onBuyTool()
{
    m_waitInterface=GameController::getInstance()->showWaitInterface(m_buyBtn);
    ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(ToolCell::onRetBuyTool)),0,false,false,"StoreView");
}
void ToolCell::onRetBuyTool()
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    string numStr = _lang_1("104907", CC_CMDITOA(info.getCNT()).c_str());
    m_numLabel->setString(numStr);
    if (m_type==0&& info.use==1) {
        m_unUseBtn->setVisible(false);
        m_storeUseBtn->setVisible(true);
    }
}

void ToolCell::onYes()
{
    ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(ToolCell::onUseTool)),0,false,false,"StoreView");
}

void ToolCell::onUseTool()
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
    else if(m_itemId == ITEM_WOOD_SPD || m_itemId == ITEM_FOOD_SPD || m_itemId == ITEM_IRON_SPD || m_itemId == ITEM_STONE_SPD) {
        int bType = FUN_BUILD_WOOD;
        if (m_itemId == ITEM_FOOD_SPD) {
            bType = FUN_BUILD_FOOD;
        }else if (m_itemId == ITEM_IRON_SPD) {
            bType = FUN_BUILD_IRON;
        }if (m_itemId == ITEM_STONE_SPD) {
            bType = FUN_BUILD_STONE;
        }
        int buildId = FunBuildController::getInstance()->getCanBoostBuild(bType);
        if (buildId>0) {
            m_boostBid = buildId;
            auto& bInfo = FunBuildController::getInstance()->getFunbuildById(m_boostBid);
            string tmpStr = _lang_2("104920", CC_ITOA(bInfo.level), _lang(bInfo.name).c_str());
            YesNoDialog::show( tmpStr.c_str() , CCCallFunc::create(this, callfunc_selector(ToolCell::onBoostBuild)));
        }
        else {//没有可以加速生产的资源建筑
            CCCommonUtils::flyHint("", "", _lang_1("104925",CCCommonUtils::getNameById(CC_ITOA(bType)).c_str() ));
        }
    }
    else {
        bool ret = ToolController::getInstance()->useTool(m_itemId, 1 , true);
        if (ret) {
            m_waitInterface=GameController::getInstance()->showWaitInterface(m_useBtn);
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
            string numStr = _lang_1("104907", CC_CMDITOA(info.getCNT()).c_str());
            m_numLabel->setString(numStr);
        }
    }
}

void ToolCell::onBoostBuild()
{
    if (m_boostBid>0) {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        auto& bInfo = FunBuildController::getInstance()->getFunbuildById(m_boostBid);
        bInfo.effectTime = GlobalData::shared()->getWorldTime()+24*60*60-1;
        BuildingAddOutPutCommand* cmd = new BuildingAddOutPutCommand(bInfo.uuid,m_boostBid, info.uuid);
        cmd->sendAndRelease();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_FUNCTION_EFFECT,CCInteger::create(m_boostBid));
        m_waitInterface=GameController::getInstance()->showWaitInterface(m_useBtn);
        ToolController::getInstance()->useTool(m_itemId,1,false);
        CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(info.itemId)), "", _lang_1("104909",info.getName().c_str()));
    }
}

bool ToolCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useBtn", CCControlButton*, this->m_useBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtn", CCControlButton*, this->m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceLabel", CCLabelIF*, m_priceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnLabel", CCLabelIF*, m_inBtnLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceNode", CCNode*, m_priceNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemBg", CCScale9Sprite*, m_itemBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_storeUseBtn", CCControlButton*, m_storeUseBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unUseBtn", CCControlButton*, m_unUseBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardNode", CCNode*, m_rewardNode);
    return false;
}


ToolRewardCell* ToolRewardCell::create(int r_type, int itemId, int num)
{
    auto ret = new ToolRewardCell();
    if (ret && ret->init(r_type, itemId, num)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool ToolRewardCell::init(int r_type, int itemId, int num)
{
    bool ret = true;
    CCBLoadFile("ToolRewardCell",this,this);
    
    m_type = r_type;
    m_itemId = itemId;
    m_num = num;
    
    int color = -1;
    
    string tmpCellName = "";
    string picName = "";
    if (m_type == R_WOOD || m_type == R_STONE || m_type == R_IRON || m_type == R_FOOD || m_type == R_SILVER) {//资源
        picName = CCCommonUtils::getResourceIconByType(m_type);
        tmpCellName = CCCommonUtils::getResourceNameByType(m_type);
    }
    else if (m_type == R_GOODS) {//道具
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(m_itemId);
        tmpCellName = toolInfo.getName();
        picName = toolInfo.icon + ".png";
        color = toolInfo.color;
    }
    else if (m_type == R_POWER) {
        picName = "icon_combat.png";
        tmpCellName = "Power";
    }
    else if (m_type == R_EXP) {
        picName = "icon_exp.png";
        tmpCellName = "Exp";
    }
    else if (m_type == R_GOLD) {
        picName = "ui_gold.png";
        tmpCellName = "Glod";
    }
    
    m_picNode->removeAllChildren();
    auto pic = CCLoadSprite::createSprite(picName.c_str());
    if(pic)
    {
        if (color>=0) {
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, 60);
            m_picNode->addChild(iconBg);
        }
        
        CCCommonUtils::setSpriteMaxSize(pic, 60, true);
        m_picNode->addChild(pic);
    }
    
    m_nameLabel->setString(tmpCellName.c_str());
    m_numLabel->setString(CC_CMDITOA(m_num));
    return ret;
}

void ToolRewardCell::onEnter() {
    CCNode::onEnter();
}
void ToolRewardCell::onExit() {
    CCNode::onExit();
}
SEL_CCControlHandler ToolRewardCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool ToolRewardCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    return false;
}