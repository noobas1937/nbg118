//
//  YuanJunView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-11.
//
//

#include "YuanJunView.h"
#include "FunBuildController.h"
#include "PopupViewController.h"
#include "BuildMoreInfoView.h"
#include "AllianceResourceTradeView.h"
#include "AllianceManager.h"
#include "GetAllianceRankCommand.h"
#include "JoinAllianceView.h"
#include "YuanJunTipView.h"

YuanJunView* YuanJunView::create(){
    YuanJunView* ret = new YuanJunView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool YuanJunView::init()
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
    
    PlayerInfo* info = new PlayerInfo();
    info->name = "123";
    info->setAllianceId("111");
    m_data->addObject(info);
    
    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_TAVERN);
    if (buildId>0) {
    }
    m_tableView = CCTableView::create(this, m_infoList->getContentSize());
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setDelegate(this);
    m_tableView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tableView);
    m_tableView->reloadData();
    
//    if(GlobalData::shared()->playerInfo.isInAlliance()){
//        GetAllianceRankCommand* cmd = new GetAllianceRankCommand(GlobalData::shared()->playerInfo.getAllianceId());
//        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(YuanJunView::updateInfo), NULL));
//        cmd->sendAndRelease();
//        m_noAllianeNode->setVisible(false);
//        this->addLoadingAni();
//    }else{
//        m_noAllianeNode->setVisible(true);
//        m_noAllianceTip->setString(_lang("115067"));
//        CCCommonUtils::setButtonTitle(m_btnJoin, _lang("115068").c_str());
//    }
    return true;
}

CCSize YuanJunView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    return CCSize(604,145);
}

CCSize YuanJunView::cellSizeForTable(CCTableView *table)
{
    return CCSizeZero;
}
CCTableViewCell* YuanJunView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    YuanJunCell* cell = (YuanJunCell*)table->dequeueCell();
    if(idx < m_data->count()){
        PlayerInfo* info = (PlayerInfo*)m_data->objectAtIndex(idx);
        if(cell){
            cell->setData(info);
        }else{
            cell = YuanJunCell::create(info);
        }
    }
    return NULL;
}

ssize_t YuanJunView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void YuanJunView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}

void YuanJunView::addLoadingAni(){
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

void YuanJunView::onCloseClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void YuanJunView::updateInfo()
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

void YuanJunView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("115066"));
}

void YuanJunView::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler YuanJunView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "openJoinAlliance", YuanJunView::openJoinAlliance);
    
    return NULL;
}

bool YuanJunView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTxt", CCLabelIF*, this->m_infoTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAllianeNode", CCNode*, this->m_noAllianeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAllianceTip", CCLabelIF*, this->m_noAllianceTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnJoin", CCControlButton*, this->m_btnJoin);
    return false;
}

void YuanJunView::openJoinAlliance(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
}

YuanJunCell* YuanJunCell::create(PlayerInfo* player)
{
    auto ret = new YuanJunCell(player);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool YuanJunCell::init()
{
    CCBLoadFile("AllianceMarketCell",this,this);
    setData(m_info);
    return true;
}

void YuanJunCell::setData(PlayerInfo* info){
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
    CCCommonUtils::setButtonTitle(m_btnHelp, _lang("115151").c_str());
}

void YuanJunCell::onEnter() {
    CCNode::onEnter();
}

void YuanJunCell::onExit() {
    CCNode::onExit();
}

void YuanJunCell::onBtnHelpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    CCCommonUtils::flyHint("", "", _lang("E100008"));return;//fusheng 12.29 屏蔽士兵援助
    PopupViewController::getInstance()->addPopupView(YuanJunTipView::create(m_info->pointId));
}

SEL_CCControlHandler YuanJunCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnHelpClick", YuanJunCell::onBtnHelpClick);
    return NULL;
}


bool YuanJunCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnHelp", CCControlButton*, this->m_btnHelp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_head", CCNode*, this->m_head);
    return false;
}