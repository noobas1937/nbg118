//
//  AllianceMarketView.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-4.
//
//

#include "AllianceMarketView.h"
#include "FunBuildController.h"
#include "PopupViewController.h"
#include "BuildMoreInfoView.h"
#include "AllianceResourceTradeView.h"
#include "AllianceManager.h"
#include "GetAllianceRankCommand.h"
#include "JoinAllianceView.h"

AllianceMarketView* AllianceMarketView::create(){
    AllianceMarketView* ret = new AllianceMarketView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool AllianceMarketView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    auto tmpCCB = CCBLoadFile("AllianceMarket",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    this->setTitleName(_lang("102002").c_str());
    m_loadingIcon = NULL;
    
    int oldBgHeight = m_viewBg->getContentSize().height;
    changeBGHeight(m_viewBg);
    int newBgHeight = m_viewBg->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    m_data = CCArray::create();
    
    
    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_TAVERN);
    if (buildId>0) {
    }
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        GetAllianceRankCommand* cmd = new GetAllianceRankCommand(GlobalData::shared()->playerInfo.getAllianceId());
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceMarketView::updateInfo), NULL));
        cmd->sendAndRelease();
        m_noAllianeNode->setVisible(false);
        this->addLoadingAni();
    }else{
        m_noAllianeNode->setVisible(true);
        m_noAllianceTip->setString(_lang("115067"));
        CCCommonUtils::setButtonTitle(m_btnJoin, _lang("115068").c_str());
    }
    
    m_tableView = CCTableView::create(this, m_infoList->getContentSize());
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setDelegate(this);
    m_tableView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tableView);
    
    return true;
}

CCSize AllianceMarketView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    return CCSize(604,145);
}

CCSize AllianceMarketView::cellSizeForTable(CCTableView *table)
{
    return CCSizeZero;
}
CCTableViewCell* AllianceMarketView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    AllianceMarketCell* cell = (AllianceMarketCell*)table->dequeueCell();
    if(idx < m_data->count()){
        PlayerInfo* info = (PlayerInfo*)m_data->objectAtIndex(idx);
        if(cell){
           cell->setData(info);
        }else{
            cell = AllianceMarketCell::create(info);
        }
    }
    return cell;
}

ssize_t AllianceMarketView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void AllianceMarketView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{

}

void AllianceMarketView::addLoadingAni(){
    m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
    m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
    auto cSize = m_infoList->getContentSize();
    m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    m_infoList->addChild(m_loadingIcon,1000000);
}

void AllianceMarketView::onCloseClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void AllianceMarketView::updateInfo(CCObject* p)
{
    if(m_loadingIcon!=NULL){
        m_loadingIcon->removeFromParent();
    }
    m_data->removeAllObjects();
    map<string, PlayerInfo* >::iterator it;
    int count = 0;
    for(int j=0;j<5;j++){
        for(it = AllianceManager::getInstance()->rankPlayerList.begin(); it != AllianceManager::getInstance()->rankPlayerList.end(); it++){
            if(it->second->rank==(j+1) && it->second->uid!=GlobalData::shared()->playerInfo.uid){
                count += 1;
                m_data->addObject(it->second);
            }
//            if(it->second->rank==(j+1)){//
//                count += 1;
//                m_data->addObject(it->second);
//            }
        }
    }
    m_tableView->reloadData();
    if(count<=0){
        m_infoTxt->setString(_lang("115085").c_str());
    }else{
        m_infoTxt->setString("");
    }
}

void AllianceMarketView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("115066"));
}

void AllianceMarketView::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler AllianceMarketView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "openJoinAlliance", AllianceMarketView::openJoinAlliance);
    
    return NULL;
}

bool AllianceMarketView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTxt", CCLabelIF*, this->m_infoTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAllianeNode", CCNode*, this->m_noAllianeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAllianceTip", CCLabelIF*, this->m_noAllianceTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnJoin", CCControlButton*, this->m_btnJoin);
    return false;
}

void AllianceMarketView::openJoinAlliance(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
}

AllianceMarketCell* AllianceMarketCell::create(PlayerInfo* player)
{
    auto ret = new AllianceMarketCell(player);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceMarketCell::init()
{
    CCBLoadFile("AllianceMarketCell",this,this);
    setData(m_info);
    return true;
}

void AllianceMarketCell::setData(PlayerInfo* info){
    m_info = info;
    std::string icon = m_info->pic;
    m_head->removeAllChildrenWithCleanup(true);
    auto pic = CCLoadSprite::createSprite(info->getPic().c_str());
    if (pic) {
        m_head->addChild(pic);
        pic->setPosition(ccp(2,0));
        CCCommonUtils::setSpriteMaxSize(pic, 85);
    }
    std::string str = _lang_1("115029", CC_CMDITOAL(m_info->power).c_str());
    m_powerTxt->setString(str);
    m_nameTxt->setString(m_info->name.c_str());
    CCCommonUtils::setButtonTitle(m_btnHelp, _lang("108729").c_str());
}

void AllianceMarketCell::onEnter() {
    CCNode::onEnter();
}

void AllianceMarketCell::onExit() {
    CCNode::onExit();
}

void AllianceMarketCell::onBtnHelpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(AllianceResourceTradeView::create(m_info));
}

SEL_CCControlHandler AllianceMarketCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnHelpClick", AllianceMarketCell::onBtnHelpClick);
    return NULL;
}


bool AllianceMarketCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnHelp", CCControlButton*, this->m_btnHelp); 
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_head", CCNode*, this->m_head); 
    return false;
}