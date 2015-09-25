//
//  AllianceShopView.cpp
//  IF
//
//  Created by lifangkai on 14-9-1.
//
//

//
//  StoreView.cpp
//  IF
//
//  Created by fubin on 14-2-27.
//
//

#include "AllianceShopView.h"
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
#include "AllianceShopShowCommand.h"
#include "TipsView.h"
#include "FBUtilies.h"
#include "UIComponent.h"
#include "StoreBuyConfirmDialog.h"

const float numPerRow = 1.0;
AllianceShopView* AllianceShopView::create(StoreType storeType, StoreResourceType storeResourceType){
    AllianceShopView* ret = new AllianceShopView(storeType, storeResourceType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool AllianceShopView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    m_isTab = false;
    CCLoadSprite::doResourceByCommonIndex(4, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    auto tmpCCB = CCBLoadFile("AllianceShopView",this,this);
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
    
    CCCommonUtils::setButtonTitle(m_bagBtn, _lang("104900").c_str());
    CCCommonUtils::setButtonTitle(m_storeBtn, _lang("115814").c_str());
    m_noMSG->setString(_lang("115830"));
    
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    
    m_cityLv = FunBuildController::getInstance()->getMainCityLv();
    
    //    m_packView = BackpackView::create();
    //    this->addChild(m_packView,100000);
    //    m_packView->setVisible(false);
  
    int count = ToolController::getInstance()->getTotalCountInBag();
    if(count>0){
        m_type = 1;
    }else{
        m_type = 0;
    }
    m_type = 1;
    
    m_page = 2;
  //  updateInfo();
    getData();
    
    return true;
}
void AllianceShopView::getData(){
    AllianceShopShowCommand* cmd = new AllianceShopShowCommand();
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
    
}

void AllianceShopView::updateInfo(CCObject* p)
{
    refreshPoint(NULL);
    m_dataList = NULL;
    if (m_page == 1) {
        m_dataList = &(ToolController::getInstance()->m_allianceTools);
    }
    else if (m_page == 2) {
        m_dataList = &(ToolController::getInstance()->m_allianceTools);
    }
    else if (m_page == 3) {
        m_dataList = &(ToolController::getInstance()->m_allianceTools);
    }
    else if (m_page == 4) {
        m_dataList = &(ToolController::getInstance()->m_allianceTools);
    }
    
    m_curList.clear();
    //m_data.clear();
    for (int i=0; i<m_dataList->size(); i++) {
        int tmpToolId = (*m_dataList)[i];
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
        if ((m_type == 1 && info.allianceCnt > 0)||info.onsale==1) {
            m_curList.push_back(tmpToolId);
        }
        else if (m_type == 0 && info.price_all>0 &&info.onsale!=1&& info.limitLv<=m_cityLv){
            m_curList.push_back(tmpToolId);
        }
    }
    if(m_curList.size()==0){
        m_noMSG->setVisible(true);
    }else{
        m_noMSG->setVisible(false);
    }
    
    bool ishasChild = false;
    CCPoint pos;
    if(m_tabView->getContainer()->getChildrenCount()==0){
        ishasChild = true;
    }else{
        pos = m_tabView->getContentOffset();
    }
    m_tabView->reloadData();
    if(ishasChild||m_isTab){
        m_isTab = false;
    }else{
        
        CCPoint minPt = m_tabView->minContainerOffset();
        CCPoint maxPt = m_tabView->maxContainerOffset();
        if (pos.y > 0) {
            return;
        }
        if (pos.y > maxPt.y) {
            pos.y = maxPt.y;
        }
        else if (pos.y < minPt.y) {
            pos.y = minPt.y;
        }
        m_tabView->setContentOffset(pos);
    }
}

void AllianceShopView::refreashData()
{
    CCPoint curr = m_tabView->getContentOffset();
    CCPoint max = m_tabView->maxContainerOffset();
    CCPoint min = m_tabView->minContainerOffset();
    int total = m_curList.size();
    m_curList.clear();
    for (int i=0; i<m_dataList->size(); i++) {
        int tmpToolId = (*m_dataList)[i];
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
        if ((m_type == 1 && info.getCNT() > 0)||info.onsale==1) {
            m_curList.push_back(tmpToolId);
        }
        else if (m_type == 0 && info.price>0 && info.limitLv<=m_cityLv){
            m_curList.push_back(tmpToolId);
        }
    }
    if(m_type==1){
        sortData();
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
        int currentNum = m_curList.size();
        if(total==currentNum && currentNum>=5){
            if (curr.y > max.y) {
                m_tabView->setContentOffset(m_tabView->maxContainerOffset());
            }else
            {
                curr.y += m_tabView->minContainerOffset().y - min.y;
                m_tabView->setContentOffset(curr);
            }
        }
    }
}
bool AllianceShopView::sortByUse(int itemId1, int itemId2){
    auto& info1 = ToolController::getInstance()->getToolInfoById(itemId1);
    auto& info2 = ToolController::getInstance()->getToolInfoById(itemId2);
    return (info1.use>info2.use);
    
}
void AllianceShopView::sortData(){
    if (m_curList.size()>0) {
        std::sort(m_curList.begin(),m_curList.end(),sortByUse);
    }
}

CCRect AllianceShopView::getStoreBtnRec()
{
    CCSize size = m_bagBtn->getContentSize();
    CCPoint pos = m_bagBtn->convertToWorldSpace(ccp(size.width*0.5,size.height*0.5));
    return CCRect(pos.x, pos.y, size.width, size.height);
}

CCSize AllianceShopView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(m_curList.size()<=0){
        return CCSizeZero;
    }
    int size =(m_curList.size()-1)/3+1;
    if(idx >= size){
        return CCSizeZero;
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1500, 518);
    }
    return CCSize(620, 290);
    
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
CCSize AllianceShopView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1500, 518);
    }
    return CCSize(620, 270);
}
CCTableViewCell* AllianceShopView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    int size =0;
    if(m_curList.size()<=0){
        size = 0;
    }else{
        size =(m_curList.size()-1)/3+1;
    }
    
    if(idx >= size){
        return NULL;
    }
    
    AllianceShopCell* cell = (AllianceShopCell*)table->dequeueCell();
    if(idx < size){
        if(cell){
            cell->setData(idx,m_type,m_curList);
        }else{
            cell = AllianceShopCell::create(idx, m_type, m_curList);
        }
    }
    return cell;
}
ssize_t AllianceShopView::numberOfCellsInTableView(CCTableView *table)
{
    int size =0;
    if(m_curList.size()<=0){
        size = 0;
    }else{
        size =(m_curList.size()-1)/3+1;
    }
    int num = ceil(size / numPerRow);
    return num;
}
void AllianceShopView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void AllianceShopView::onEnter()
{
    UIComponent::getInstance()->showPopupView(1);
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceShopView::updateInfo), ALLIANCE_SHOP_SHOW, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceShopView::refreshPoint), ALLIANCE_SHOP_BUY_LEADER, NULL);
      CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceShopView::updateInfo), ALLIANCE_SHOP_BUY_USER, NULL);
//     setTitleName(_lang("115206").c_str());
    m_titleTxt->setString(_lang("115206"));
}

void AllianceShopView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_SHOP_SHOW);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_SHOP_BUY_LEADER);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_SHOP_BUY_USER);
    CCNode::onExit();
}

AllianceShopView::~AllianceShopView()
{
    //    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_REMOVE,CCString::create(CC_ITOA(PVT_store)));
}

SEL_CCControlHandler AllianceShopView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickStoreBtn", AllianceShopView::onClickStoreBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBagBtn", AllianceShopView::onClickBagBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", AllianceShopView::onTipBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpBtnClick", AllianceShopView::onHelpBtnClick);
    return NULL;
}

bool AllianceShopView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_storeBtn", CCControlButton*, this->m_storeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagBtn", CCControlButton*, this->m_bagBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noMSG", CCLabelIF*, m_noMSG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pointNumLabel", CCLabelIF*, this->m_pointNumLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pointLabel", CCLabelIF*, this->m_pointLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alliancePointSpr", CCSprite*, this->m_alliancePointSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceContributionSpr", CCSprite*, this->m_allianceContributionSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    return false;
}

void AllianceShopView::onCloseClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}
void AllianceShopView::refreshPoint(CCObject* p){
    if (m_type==0) {
        m_storeBtn->setEnabled(false);
        m_bagBtn->setEnabled(true);
        m_pointLabel->setString(_lang("115815"));
        m_pointNumLabel->setString(CC_CMDITOA(GlobalData::shared()->playerInfo.allianceInfo.alliancepoint));
        m_alliancePointSpr->setVisible(true);
        m_allianceContributionSpr->setVisible(false);
    }
    else {
        m_pointLabel->setString(_lang("115810"));
        m_storeBtn->setEnabled(true);
        m_bagBtn->setEnabled(false);
        m_pointNumLabel->setString(CC_CMDITOA(GlobalData::shared()->playerInfo.allianceInfo.accPoint));
        m_alliancePointSpr->setVisible(false);
        m_allianceContributionSpr->setVisible(true);
        //        m_curList.clear();
        //        m_tabView->reloadData();
        //        return ;
    }

}
void AllianceShopView::onClickStoreBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_type != 0) {
        m_type = 0;
        m_isTab = true;
        //m_packView->showList(false);
        updateInfo();
    }
}

void AllianceShopView::onClickBagBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    //m_packView->showList(true);
    if (m_type != 1) {
        m_isTab = true;
        m_type = 1;
        updateInfo();
    }
}

void AllianceShopView::onTipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    string content = _lang("115818")+"\n"+"\n"+"\n"+"\n"+_lang("115819");
    string title = "";
    if(m_type==1){
        title =_lang("115810");
        content = _lang("115818")+"\n"+"\n"+"\n"+"\n"+_lang("115819");
    }else{
        title =_lang("115815");
        content = _lang("115820")+"\n"+"\n"+"\n"+"\n"+_lang("115821");
    }
    PopupViewController::getInstance()->addPopupView(TipsVipView::create(title,content));
}

void AllianceShopView::onHelpBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45207");
}

AllianceShopCell* AllianceShopCell::create(int index, int type, vector<int> &cellData)
{
    auto ret = new AllianceShopCell(index,type,cellData);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceShopCell::init()
{
    bool ret = true;
    CCBLoadFile("AllianceShopCell",this,this);

    setData(m_index, m_type,m_cellData);
    return ret;
}

void AllianceShopCell::setData(int index, int type,vector<int> &cellData)
{
    m_index = index;
    m_type = type;
    m_cellData = cellData;
    m_clickInSide = false;
    this->m_upnode1->setVisible(true);
    this->m_downnode1->setVisible(true);
    this->m_upnode2->setVisible(true);
    this->m_downnode2->setVisible(true);
    this->m_upnode3->setVisible(true);
    this->m_downnode3->setVisible(true);
    
    if (m_type==0) {
        m_numText1->setVisible(false);
        m_numText2->setVisible(false);
        m_numText3->setVisible(false);
        m_nunBGSpr1->setVisible(false);
        m_nunBGSpr2->setVisible(false);
        m_nunBGSpr3->setVisible(false);
        m_alliancePointSpr1->setVisible(true);
        m_allianceContributionSpr1->setVisible(false);
        m_alliancePointSpr2->setVisible(true);
        m_allianceContributionSpr2->setVisible(false);
        m_alliancePointSpr3->setVisible(true);
        m_allianceContributionSpr3->setVisible(false);
    }
    else {
        m_numText1->setVisible(true);
        m_numText2->setVisible(true);
        m_numText3->setVisible(true);
        m_nunBGSpr1->setVisible(true);
        m_nunBGSpr2->setVisible(true);
        m_nunBGSpr3->setVisible(true);
        m_alliancePointSpr1->setVisible(false);
        m_allianceContributionSpr1->setVisible(true);
        m_alliancePointSpr2->setVisible(false);
        m_allianceContributionSpr2->setVisible(true);
        m_alliancePointSpr3->setVisible(false);
        m_allianceContributionSpr3->setVisible(true);
    }
    ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_cellData[index*3]);
    m_nameText1->setString(info1.getName().c_str());
    //防爆框
    if (CCCommonUtils::isIosAndroidPad())
    {
        if (m_nameText1->getContentSize().height>92) {
            m_nameText1->setFontSize(m_nameText1->getFontSize()-4);
        }
    } else {
        if (m_nameText1->getContentSize().height>43) {
            m_nameText1->setFontSize(m_nameText1->getFontSize()-2);
        }
    }
    m_priceText1->setString(CC_CMDITOA(info1.price_all));
    
    m_picNode1->removeAllChildren();
    auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info1.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
    if (pic) {
        auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info1.color).c_str());
        CCCommonUtils::setSpriteMaxSize(iconBg, 130,true);
        m_picNode1->addChild(iconBg);
        
        CCCommonUtils::setSpriteMaxSize(pic, 130,true);
        m_picNode1->addChild(pic);
    }
    
    m_numText1->setString(CC_CMDITOA(info1.allianceCnt));
    if(info1.onsale==1){
        m_numText1->setString(_lang("115822"));
        m_nunBGSpr1->setVisible(false);
    }
    if(m_index*3+1>=cellData.size()){
        this->m_upnode2->setVisible(false);
        this->m_downnode2->setVisible(false);
    }else{
        ToolInfo& info2 = ToolController::getInstance()->getToolInfoById(m_cellData[index*3+1]);
        m_nameText2->setString(info2.getName().c_str());
        //防爆框
        if (CCCommonUtils::isIosAndroidPad())
        {
            if (m_nameText1->getContentSize().height>92) {
                m_nameText1->setFontSize(m_nameText1->getFontSize()-4);
            }
        } else {
            if (m_nameText2->getContentSize().height>43) {
                m_nameText2->setFontSize(m_nameText2->getFontSize()-2);
            }
        }
        
        m_priceText2->setString(CC_CMDITOA(info2.price_all));
        
        m_picNode2->removeAllChildren();
        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info2.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
        if (pic) {
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info2.color).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, 130,true);
            m_picNode2->addChild(iconBg);
            
            CCCommonUtils::setSpriteMaxSize(pic, 130,true);
            m_picNode2->addChild(pic);
        }
        
        m_numText2->setString(CC_CMDITOA(info2.allianceCnt));
        if(info2.onsale==1){
            m_numText2->setString(_lang("115822"));
            m_nunBGSpr2->setVisible(false);
        }
    }
    if(m_index*3+2>=cellData.size()){
        this->m_upnode3->setVisible(false);
        this->m_downnode3->setVisible(false);
    }else{
        ToolInfo& info3 = ToolController::getInstance()->getToolInfoById(m_cellData[index*3+2]);
        m_nameText3->setString(info3.getName().c_str());
        //防爆框
        if (CCCommonUtils::isIosAndroidPad()) {
            if (m_nameText1->getContentSize().height>92) {
                m_nameText1->setFontSize(m_nameText1->getFontSize()-4);
            }
        } else {
            if (m_nameText3->getContentSize().height>43) {
                m_nameText3->setFontSize(m_nameText3->getFontSize()-2);
            }
        }
        m_priceText3->setString(CC_CMDITOA(info3.price_all));
        
        m_picNode3->removeAllChildren();
        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info3.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
        if (pic) {
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info3.color).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, 130,true);
            m_picNode3->addChild(iconBg);

            CCCommonUtils::setSpriteMaxSize(pic, 130,true);
            m_picNode3->addChild(pic);
        }
        
        m_numText3->setString(CC_CMDITOA(info3.allianceCnt));
        if(info3.onsale==1){
            m_numText3->setString(_lang("115822"));
            m_nunBGSpr3->setVisible(false);
        }
    }

    int rank =GlobalData::shared()->playerInfo.allianceInfo.rank;
    if(m_type == 0&&rank>=4){
       // m_priceNode->setVisible(true);
    }else{
       // m_priceNode->setVisible(false);
    }
}

void AllianceShopCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, true);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceShopCell::onRetBuyTool), ALLIANCE_SHOP_BUY_LEADER, NULL);
    //CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceShopCell::onUseTool), ALLIANCE_SHOP_BUY_USER, NULL);
        //CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceShopCell::refreashData), ALLIANCE_SHOP_SHOW, NULL);
}

void AllianceShopCell::onExit() {
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_SHOP_BUY_LEADER);
   // CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_SHOP_BUY_USER);
        //CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_SHOP_SHOW);
    CCNode::onExit();
}
void AllianceShopCell::refreashData(CCObject* obj)
{
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    string numStr = _lang_1("104907", CC_CMDITOA(info.allianceCnt).c_str());
 //   m_numLabel->setString(numStr);
    
    if (info.allianceCnt <= 0) {
       // m_priceNode->setVisible(true);
      //  m_useBtn->setVisible(false);
        
        if (m_type == 1 && info.price<=0) {//背包内，使用完不可购买的道具
        }
        else if (m_type == 1) {
            m_type = 2;
        }
        else if (m_type == 0 && info.use==1) {
        //    m_unUseBtn->setVisible(true);
         //   m_storeUseBtn->setVisible(false);
        }
    }
    int rank =GlobalData::shared()->playerInfo.allianceInfo.rank;
    if(m_type == 0&&rank>=4){
       // m_priceNode->setVisible(true);
    }else{
       // m_priceNode->setVisible(false);
    }
}

SEL_CCControlHandler AllianceShopCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUseBtn", AllianceShopCell::onClickUseBtn);
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn", AllianceShopCell::onClickBuyBtn);
   // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickStoreUseBtn", AllianceShopCell::onClickStoreUseBtn);
    return NULL;
}

void AllianceShopCell::onClickStoreUseBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    if(!m_clickInSide) return ;
//    onUseTool();
}

bool AllianceShopCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
        m_curPt = pTouch->getLocation();
    if(m_clickInSide){
        return false;
    }
    float ScX = 1;
    float ScY = 1;
    if (CCCommonUtils::isIosAndroidPad()) {
        ScX = 1.86;
        ScY = 1.8;
    }
    
    if (isTouchInside(m_hintNode1,pTouch)) {
        if(!this->m_upnode1->isVisible()){
            return false;
        }
        m_nameBGSpr1->setScaleX(ScX * 1.2);
        m_nameBGSpr1->setScaleY(ScY * 1.2);
        return true;
    }else if(isTouchInside(m_hintNode2,pTouch)) {
        if(!this->m_upnode2->isVisible()){
            return false;
        }
        m_nameBGSpr2->setScaleX(ScX * 1.2);
        m_nameBGSpr2->setScaleY(ScY * 1.2);
        return true;
    }else if(isTouchInside(m_hintNode3,pTouch)) {
        if(!this->m_upnode3->isVisible()){
            return false;
        }
        m_nameBGSpr3->setScaleX(ScX * 1.2);
        m_nameBGSpr3->setScaleY(ScY * 1.2);
        return true;
    }
    return false;
}
void AllianceShopCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (fabs(pTouch->getLocation().x-m_curPt.x) > 30 || fabs(pTouch->getLocation().y-m_curPt.y) > 30)
    {
        if (!CCCommonUtils::isIosAndroidPad()) {
            m_nameBGSpr1->setScale(1.0);
            m_nameBGSpr2->setScale(1.0);
            m_nameBGSpr3->setScale(1.0);
        } else {
            m_nameBGSpr1->setScaleX(1.86);
            m_nameBGSpr1->setScaleY(1.8);
            m_nameBGSpr2->setScaleX(1.86);
            m_nameBGSpr2->setScaleY(1.8);
            m_nameBGSpr3->setScaleX(1.86);
            m_nameBGSpr3->setScaleY(1.8);
        }
    }

}
void AllianceShopCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!CCCommonUtils::isIosAndroidPad()) {
        m_nameBGSpr1->setScale(1.0);
        m_nameBGSpr2->setScale(1.0);
        m_nameBGSpr3->setScale(1.0);
    } else {
        m_nameBGSpr1->setScaleX(1.86);
        m_nameBGSpr1->setScaleY(1.8);
        m_nameBGSpr2->setScaleX(1.86);
        m_nameBGSpr2->setScaleY(1.8);
        m_nameBGSpr3->setScaleX(1.86);
        m_nameBGSpr3->setScaleY(1.8);
    }
    if (fabs(pTouch->getLocation().x-m_curPt.x) > 30 || fabs(pTouch->getLocation().y-m_curPt.y) > 30)
        
    {
        
        return;
        
    }
    if (isTouchInside(m_hintNode1,pTouch)) {
        m_clickInSide = true;
       m_itemId =  m_cellData[m_index*3];
        m_pos = 1;
    }else if(isTouchInside(m_hintNode2,pTouch)) {
        m_clickInSide = true;
        m_itemId =  m_cellData[m_index*3+1];
        m_pos = 2;
    }else if(isTouchInside(m_hintNode3,pTouch)) {
        m_clickInSide = true;
        m_itemId =  m_cellData[m_index*3+2];
        m_pos = 3;
    }
    if(m_type==1){
        onClickUseBtn(NULL,Control::EventType::TOUCH_DOWN);
    }else{
        int rank =GlobalData::shared()->playerInfo.allianceInfo.rank;
        string rankId = CC_ITOA(rank+7100);
        string m_desc = CCCommonUtils::getPropById(rankId, "allianceshop");
        if(m_desc=="1"){
            onClickBuyBtn(NULL,Control::EventType::TOUCH_DOWN);
        }else{
            CCCommonUtils::flyHint("", "", _lang("115828"));
        }
    }
    m_clickInSide = false;
}

void AllianceShopCell::onClickUseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(!m_clickInSide) return ;
    auto& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    CCLog("point:%d",GlobalData::shared()->playerInfo.allianceInfo.accPoint);
    if(GlobalData::shared()->playerInfo.allianceInfo.accPoint<info.price_all){
         CCCommonUtils::flyHint("", "", _lang("115826"));
        return;
    }
//    YesNoDialog::showButtonAndPriceType(_lang_1("115816", info.getName().c_str()), CCCallFunc::create(this, callfunc_selector(AllianceShopCell::onUseAction)),_lang("102148").c_str(),7,info.price_all);
    auto view = AllToolNumSelectView::create(m_itemId);
    PopupViewController::getInstance()->addPopupView(view);
    //onUseAction();
}

void AllianceShopCell::onUseAction(CCObject* obj)
{
    int num = 1;
    if(obj){
        CCInteger* cInt = dynamic_cast<CCInteger*>(obj);
        if(cInt){
            num = cInt->getValue();
        }
    }
    GameController::getInstance()->showWaitInterface();
    AllianceShopUserBuyCommand* cmd = new AllianceShopUserBuyCommand(CC_ITOA(m_itemId), num);
    cmd->sendAndRelease();
    string itemName = CCCommonUtils::getNameById(CC_ITOA(m_itemId));
    FBUtilies::appEventAllianceHonorExchange(itemName, m_itemId,FunBuildController::getInstance()->getMainCityLv(), GlobalData::shared()->playerInfo.level,GlobalData::shared()->playerInfo.allianceInfo.rank);
}
void AllianceShopCell::onUseAction(){
    int num = 1;
     GameController::getInstance()->showWaitInterface();
    AllianceShopUserBuyCommand* cmd = new AllianceShopUserBuyCommand(CC_ITOA(m_itemId), num);
    cmd->sendAndRelease();
    string itemName = CCCommonUtils::getNameById(CC_ITOA(m_itemId));
    FBUtilies::appEventAllianceHonorExchange(itemName, m_itemId,FunBuildController::getInstance()->getMainCityLv(), GlobalData::shared()->playerInfo.level,GlobalData::shared()->playerInfo.allianceInfo.rank);
}
void AllianceShopCell::onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(!m_clickInSide) return ;
    if (m_type==0) {
        auto& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        CCLog("Point:%d",GlobalData::shared()->playerInfo.allianceInfo.alliancepoint);
        if(GlobalData::shared()->playerInfo.allianceInfo.alliancepoint<info.price_all){
            CCCommonUtils::flyHint("", "", _lang("115827"));
            return;
        }
//        YesNoDialog::show(_lang_1("115816", _lang(info.name).c_str()), CCCallFunc::create(this, callfunc_selector(AllianceShopCell::onBuyAction)));
//        YesNoDialog::showButtonAndPriceType(_lang_1("115817", info.getName().c_str()), CCCallFunc::create(this, callfunc_selector(AllianceShopCell::onBuyAction)),_lang("102148").c_str(),6,info.price_all);
        
        CCNode* node = nullptr;
        switch (m_pos) {
            case 1:
                node = m_picNode1;
                break;
                
            case 2:
                node = m_picNode2;
                break;
                
            case 3:
                node = m_picNode3;
                break;
                
            default:
                break;
        }
        AllianceShopView *view = dynamic_cast<AllianceShopView*>(PopupViewController::getInstance()->getCurrentPopupView());
        CCPoint p(0, 0);
        p = node->convertToWorldSpace(CCPointZero);
        StoreBuyConfirmDialog* dialog = StoreBuyConfirmDialog::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), info.getName().c_str(), _lang(info.des).c_str(), info.price_all,info.color, CCCallFuncO::create(this, callfuncO_selector(AllianceShopCell::onBuyAction), NULL), p, -1);
        if (dialog)
        {
            int maxNum = floor(GlobalData::shared()->playerInfo.allianceInfo.alliancepoint/info.price_all);
            maxNum = MAX(1, maxNum);
            dialog->showSliderBar(maxNum);
            dialog->setEffNodeRect(view->getStoreBtnRec());
//            dialog->setCloseCallback(CCCallFunc::create(this, callfunc_selector(AllianceShopCell::onCloseConfirm)));
        }
    }

}

void AllianceShopCell::onBuyAction(CCObject* obj)
{
    int num = 1;
    if(obj){
        CCInteger* cInt = dynamic_cast<CCInteger*>(obj);
        if(cInt){
            num = cInt->getValue();
        }
    }
    GameController::getInstance()->showWaitInterface();
    AllianceShopLeaderBuyCommand* cmd = new AllianceShopLeaderBuyCommand(CC_ITOA(m_itemId), num);
    cmd->sendAndRelease();
    string itemName = CCCommonUtils::getNameById(CC_ITOA(m_itemId));
    FBUtilies::appEventAllianceScoreUsage(itemName, m_itemId, GlobalData::shared()->playerInfo.allianceInfo.rank);

}
void AllianceShopCell::onBuyAction(){
//    int num = 1;
//     GameController::getInstance()->showWaitInterface();
//    AllianceShopLeaderBuyCommand* cmd = new AllianceShopLeaderBuyCommand(CC_ITOA(m_itemId), num);
//    cmd->sendAndRelease();
//    string itemName = CCCommonUtils::getNameById(CC_ITOA(m_itemId));
//    FBUtilies::appEventAllianceScoreUsage(itemName, m_itemId, GlobalData::shared()->playerInfo.allianceInfo.rank);
}
void AllianceShopCell::onRetBuyTool(CCObject* obj)
{
    int itemId = dynamic_cast<CCInteger*>(obj)->getValue();
    if(itemId!=m_itemId){
        return;
    }
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    string numStr = _lang_1("104907", CC_CMDITOA(info.allianceCnt).c_str());
    //m_numLabel->setString(numStr);
    if (m_type==0&& info.use==1) {
//        m_unUseBtn->setVisible(false);
//        m_storeUseBtn->setVisible(true);
    }
}

void AllianceShopCell::onUseTool(CCObject* obj)
{
    int itemId = dynamic_cast<CCInteger*>(obj)->getValue();
    if(itemId!=m_itemId){
        return;
    }
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    string numStr = _lang_1("104907", CC_CMDITOA(info.allianceCnt).c_str());
   // m_numLabel->setString(numStr);
    if (m_type==0&& info.use==1) {
    //    m_unUseBtn->setVisible(false);
    //    m_storeUseBtn->setVisible(true);
    }
}



bool AllianceShopCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText1", CCLabelIF*, m_nameText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numText1", CCLabelIF*, m_numText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceText1", CCLabelIF*, m_priceText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, m_picNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintNode1", CCNode*, m_hintNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upnode1", CCNode*, m_upnode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downnode1", CCNode*, m_downnode1);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText2", CCLabelIF*, m_nameText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numText2", CCLabelIF*, m_numText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceText2", CCLabelIF*, m_priceText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", CCNode*, m_picNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintNode2", CCNode*, m_hintNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upnode2", CCNode*, m_upnode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downnode2", CCNode*, m_downnode2);
    
  
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText3", CCLabelIF*, m_nameText3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numText3", CCLabelIF*, m_numText3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceText3", CCLabelIF*, m_priceText3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode3", CCNode*, m_picNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintNode3", CCNode*, m_hintNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upnode3", CCNode*, m_upnode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downnode3", CCNode*, m_downnode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nunBGSpr1", CCSprite*, m_nunBGSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nunBGSpr2", CCSprite*, m_nunBGSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nunBGSpr3", CCSprite*, m_nunBGSpr3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameBGSpr1", CCSprite*, m_nameBGSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameBGSpr2", CCSprite*, m_nameBGSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameBGSpr3", CCSprite*, m_nameBGSpr3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alliancePointSpr1", CCSprite*, m_alliancePointSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceContributionSpr1", CCSprite*, m_allianceContributionSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceContributionSpr2", CCSprite*, m_allianceContributionSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alliancePointSpr2", CCSprite*, m_alliancePointSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alliancePointSpr3", CCSprite*, m_alliancePointSpr3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceContributionSpr3", CCSprite*, m_allianceContributionSpr3);
    return false;
}

AllToolNumSelectView* AllToolNumSelectView::create(int id) {
    auto ret = new AllToolNumSelectView(id);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllToolNumSelectView::init() {
    bool ret = false;
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(11, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(11, false);
        });
        CCBLoadFile("AllToolNumSelect", this, this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        
        m_titleLabel->setString(info.getName());
        //label可滑动，放在scrollview上
        int maxWidth = m_subNode->getContentSize().width;
        int maxHeight = m_subNode->getContentSize().height;
        CCLabelIF* label = CCLabelIF::create();
        label->setDimensions(CCSize(maxWidth, 0));
        label->setString(_lang(info.des));
        label->setColor(ccc3(169, 132, 71));
        label->setFontSize(24);
        if (CCCommonUtils::isIosAndroidPad())
        {
            label->setFontSize(42);
        }
        label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        int totalHeight = label->getContentSize().height * label->getOriginScaleY();
        if(totalHeight > maxHeight){
            CCScrollView* scroll = CCScrollView::create(CCSize(maxWidth, maxHeight));
            scroll->setContentSize(CCSize(maxWidth, totalHeight));
            scroll->addChild(label);
            m_subNode->addChild(scroll);
            scroll->setPosition(CCPointZero);
            scroll->setDirection(kCCScrollViewDirectionVertical);
            scroll->setAnchorPoint(ccp(0, 0));
            scroll->setContentOffset(ccp(0, maxHeight-totalHeight));
        }else{
            m_subNode->addChild(label);
            label->setAnchorPoint(ccp(0, 1));
            label->setPosition(ccp(0,maxHeight));
        }
        m_tipLabel->setString(_lang("104919"));
        CCCommonUtils::createGoodsIcon(m_itemId, m_iconNode, CCSize(m_iconBg->getContentSize().width - 20, m_iconBg->getContentSize().height - 20));
        
        m_numAll = MAX(MIN(floor(GlobalData::shared()->playerInfo.allianceInfo.accPoint / info.price_all), info.allianceCnt), 0);
        if (info.onsale == 1)
        {
            m_numAll = floor(GlobalData::shared()->playerInfo.allianceInfo.accPoint / info.price_all);
        }
        m_numCurrent = 1;
        std::string numMax = std::string("/") + CC_ITOA(m_numAll);
        m_numLabel->setString(CC_CMDITOA(m_numCurrent * info.price_all));
        
        auto costIcon = CCLoadSprite::createSprite("Contribution_icon.png");
        costIcon->setScale(0.7);
        if (costIcon) {
            m_sprNode->addChild(costIcon);
        }
        m_btnLabel->setString(_lang("102148"));

        CCSize barSize = m_barNode->getContentSize();
        auto m_sliderBg = CCLoadSprite::createScale9Sprite("huadongtiao3.png");
        m_sliderBg->setInsetBottom(5);
        m_sliderBg->setInsetLeft(5);
        m_sliderBg->setInsetRight(5);
        m_sliderBg->setInsetTop(5);
        m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
        m_sliderBg->setContentSize(CCSize(barSize.width,18));
        m_sliderBg->setPosition(ccp(barSize.width*0.5,barSize.height*0.5+9));
        
        auto proSp = CCLoadSprite::createSprite("huadongtiao2.png");
        auto thuSp = CCLoadSprite::createSprite("huadongtiao1.png");
        
        
        m_slider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
        float minVal = m_numCurrent*1.0/m_numAll;
        m_slider->setMinimumValue(minVal);
        m_slider->setMaximumValue(1.0f);
        m_slider->setValue(minVal);
        m_slider->setProgressScaleX(barSize.width/proSp->getContentSize().width);
        m_slider->setTag(1);
        m_slider->setLimitMoveValue(20);
        m_slider->setPosition(ccp(barSize.width*0.5,barSize.height*0.5));
        m_slider->addTargetWithActionForControlEvents(this, cccontrol_selector(AllToolNumSelectView::moveSlider), CCControlEventValueChanged);
        m_barNode->addChild(m_slider);
        m_invalidSlider = false;
        
        auto editSize = m_editNode->getContentSize();
        auto editpic =CCLoadSprite::createScale9Sprite("frame_text2.png");
        editpic->setInsetBottom(8);
        editpic->setInsetTop(8);
        editpic->setInsetRight(10);
        editpic->setInsetLeft(10);
        editpic->setPreferredSize(editSize);
        m_editBox = CCEditBox::create(editSize,editpic);
        m_editBox->setInputMode(kEditBoxInputModeNumeric);
        m_editBox->setText(CC_ITOA(m_numCurrent));
        m_editBox->setDelegate(this);
        //m_editBox->setTouchPriority(Touch_Popup);
        m_editBox->setMaxLength(12);
        m_editBox->setReturnType(kKeyboardReturnTypeDone);
        m_editBox->setPosition(ccp(editSize.width/2, editSize.height/2));
        //m_editBox->setTouchPriority(Touch_Default);
        m_editNode->addChild(m_editBox);
        
        ret = true;
    }
    return ret;
}

void AllToolNumSelectView::onEnter() {
    PopupBaseView::onEnter();
    setTouchEnabled(true);
    
}

void AllToolNumSelectView::onExit() {
    setTouchEnabled(false);
    
    PopupBaseView::onExit();
}

void AllToolNumSelectView::onUseClick(CCObject * pSender, CCControlEvent pCCControlEvent) {
    if (m_numCurrent == 1) {
        sureBuy();
    } else {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        string name = info.getName();
        name += " × ";
        name += CC_ITOA(m_numCurrent);
        YesNoDialog::show(_lang_1("104954", name.c_str()), CCCallFunc::create(this, callfunc_selector(AllToolNumSelectView::sureBuy)));
    }
}

void AllToolNumSelectView::sureBuy()
{
    GameController::getInstance()->showWaitInterface();
    AllianceShopUserBuyCommand* cmd = new AllianceShopUserBuyCommand(CC_ITOA(m_itemId), m_numCurrent);
    cmd->sendAndRelease();
    string itemName = CCCommonUtils::getNameById(CC_ITOA(m_itemId));
    FBUtilies::appEventAllianceHonorExchange(itemName, m_itemId,FunBuildController::getInstance()->getMainCityLv(), GlobalData::shared()->playerInfo.level,GlobalData::shared()->playerInfo.allianceInfo.rank);
    PopupViewController::getInstance()->removePopupView(this);
}

SEL_CCControlHandler AllToolNumSelectView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUseClick", AllToolNumSelectView::onUseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", AllToolNumSelectView::onAddClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSubClick", AllToolNumSelectView::onSubClick);
    return NULL;
}

bool AllToolNumSelectView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNode", CCNode*, m_subNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipLabel", CCLabelIF*, m_tipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useBtn", CCControlButton*, m_useBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode", CCNode*, m_sprNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnLabel", CCLabelIF*, m_btnLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, m_editNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barNode", CCNode*, m_barNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBtn", CCControlButton*, m_subBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn", CCControlButton*, m_addBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconBg", CCSprite*, m_iconBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);

    return false;
}

bool AllToolNumSelectView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}
void AllToolNumSelectView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return;
    }
    PopupViewController::getInstance()->removePopupView(this);
}

void AllToolNumSelectView::moveSlider(CCObject * pSender, CCControlEvent pCCControlEvent) {
    if (m_invalidSlider) {
        m_invalidSlider = false;
        return;
    }
    float percent = MAX(m_slider->getValue(),m_slider->getMinimumValue());
    percent = MIN(percent, m_slider->getMaximumValue());
    int num = round(percent * m_numAll);
    m_numCurrent = num;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    m_numLabel->setString(CC_CMDITOA(m_numCurrent * info.price_all));
    string tmp = CC_ITOA(m_numCurrent);
    m_editBox->setText(tmp.c_str());
}

void AllToolNumSelectView::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
//    std::string temp  =text;
//    size_t pos = temp.find_first_not_of("1234567890");
//    while (pos!=std::string::npos) {
//        temp.replace(pos, 1, "");
//        pos = temp.find_first_not_of("1234567890");
//    }
//    int a = atoi(temp.c_str());
//    editBox->setText(CC_ITOA(a));
}
void AllToolNumSelectView::editBoxReturn(CCEditBox *editBox){
    string numStr = m_editBox->getText();
    int num = atoi(numStr.c_str());
    m_numCurrent = MAX(MIN(num, m_numAll), 1);
    setNumString();
}
void AllToolNumSelectView::onAddClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    m_numCurrent++;
    m_numCurrent = MAX(MIN(m_numCurrent, m_numAll), 1);
    setNumString();
}
void AllToolNumSelectView::onSubClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    m_numCurrent--;
    m_numCurrent = MAX(MIN(m_numCurrent, m_numAll), 1);
    setNumString();
}
void AllToolNumSelectView::setNumString(){
    float percent = m_numCurrent * 1.0f / m_numAll;
    m_slider->setValue(percent);
    m_editBox->setText(CC_ITOA(m_numCurrent));
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    m_numLabel->setString(CC_CMDITOA(m_numCurrent * info.price_all));
}

