//
//  NBAllianceHelpReportView.cpp
//  DragonClans
//
//  Created by gj on 1/15/16.
//
//

#include "NBAllianceHelpReportView.hpp"

#include "PopupViewController.h"

#include "UIComponent.h"
#include "ChangePicCommand.h"
#include "ToolController.h"
#include "UseToolView.h"
#include "WorldController.h"
#include "YuanJunDetailView.h"
#include "AllianceManager.h"
#include "IFAllianceHelpReportInfo.hpp"
#include "AllRankCommand.h"
#include "PlayerInfoBtnPopUpView.h"

NBAllianceHelpReportView *NBAllianceHelpReportView::create(){
    NBAllianceHelpReportView *ret = new NBAllianceHelpReportView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void NBAllianceHelpReportView::onEnter(){
    this->setTitleName(_lang("115563"));
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void NBAllianceHelpReportView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool NBAllianceHelpReportView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    m_touchPos = pTouch->getLocation();
//    m_isMove = false;
    if (isTouchInside(m_infoList, pTouch)) {
        return true;
    }
    return false;
}

void NBAllianceHelpReportView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
//    CCPoint pos = pTouch->getLocation();
//    if (fabsf(m_touchPos.y - pos.y)>30) {
//        m_isMove = true;
//    }
}

void NBAllianceHelpReportView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
//    if (m_isMove) {
//        m_isMove = false;
//        CCPoint min = m_tabView->minContainerOffset();
//        CCPoint curr = m_tabView->getContentOffset();
//        if (curr.y > 30 && m_data->count()>10 && m_data->count()<500) {
//            this->callMore();
//        }
//    }
}

bool NBAllianceHelpReportView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        auto node = CCBLoadFile("AllianceHelpReportView", this, this);
        this->setContentSize(node->getContentSize());
        
        CCLoadSprite::doResourceByCommonIndex(204, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(204, false);
            CCLoadSprite::doResourceByCommonIndex(206, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERLITTLE);
        });
        //        auto tbg = CCLoadSprite::loadResource("technology_09.png");
        //        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        //        int maxHight = CCDirector::sharedDirector()->getWinSize().height;
        //        int curHight = -500;
        //        while (curHight<maxHight) {
        //            auto bg = CCLoadSprite::createSprite("technology_09.png");
        //            bg->setAnchorPoint(ccp(0, 1));
        //            bg->setPosition(ccp(0, curHight));
        //            curHight += bg->getContentSize().height;
        //            tBatchNode->addChild(bg);
        //        }
        //        if (CCCommonUtils::isIosAndroidPad()) {
        //            tBatchNode->setScaleX(1536.0 / 640.0);
        //        }
        //        this->addChild(tBatchNode,-10);
        int preHeight = this->m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        m_viewBg->setVisible(false);
        int dh = m_viewBg->getContentSize().height - preHeight;
        if (CCCommonUtils::isIosAndroidPad()) {
            dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        this->m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + dh));
        m_infoList->setPositionY(m_infoList->getPositionY()-dh);
        
        m_typeTxt->setString("");
        m_attTxt->setString(_lang("115564"));
        m_desTxt->setString(_lang("115565"));
        m_timeTxt->setString(_lang("115235"));
        
        getRankData();
        
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        
        
        ret = true;
    }
    return ret;
}

void NBAllianceHelpReportView::getRankData()
{
    GameController::getInstance()->showWaitInterface();
    HelpReportCmd* cmd = new HelpReportCmd();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector( NBAllianceHelpReportView::refreashData), NULL));
    cmd->sendAndRelease();
}

void NBAllianceHelpReportView::updateInfo()
{
    if (AllianceManager::getInstance()->m_helpReportListVec.size() == 0) {
        m_tip->setVisible(true);
        m_tip->setString(_lang("115569"));
    }
    else{
        m_tip->setVisible(false);
    }
    GameController::getInstance()->removeWaitInterface();
    m_tabView->reloadData();
}

void NBAllianceHelpReportView::refreashData(CCObject* obj)
{
    updateInfo();
}

bool NBAllianceHelpReportView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_typeTxt", CCLabelIF*, this->m_typeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_attTxt", CCLabelIF*, this->m_attTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_desTxt", CCLabelIF*, this->m_desTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tip", CCLabelIF*, this->m_tip);
    return false;
}

SEL_CCControlHandler NBAllianceHelpReportView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}

CCSize NBAllianceHelpReportView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    return CCSize(604, 92);
}

CCSize NBAllianceHelpReportView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1460, 217);
    }
    return CCSize(604, 92);
}

CCTableViewCell* NBAllianceHelpReportView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx > AllianceManager::getInstance()->m_helpReportListVec.size()){
        return NULL;
    }
    NBAllianceHelpReportCell* cell = (NBAllianceHelpReportCell*)table->dequeueCell();
    if(cell){
        cell->setData(idx);
    }else{
        cell = NBAllianceHelpReportCell::create(idx);
    }
    
    return cell;
}

ssize_t NBAllianceHelpReportView::numberOfCellsInTableView(CCTableView *table)
{
    int num = AllianceManager::getInstance()->m_helpReportListVec.size();
    return num;
}

void NBAllianceHelpReportView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

//

NBAllianceHelpReportCell *NBAllianceHelpReportCell::create(int index){
    auto ret = new NBAllianceHelpReportCell(index);
    if (ret && ret->init(index)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void NBAllianceHelpReportCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
}

void NBAllianceHelpReportCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool NBAllianceHelpReportCell::init(int index){
    auto node = CCBLoadFile("AllianceHelpReportCell", this, this);
    this->setContentSize(node->getContentSize());
    m_index = index;
    setData(m_index);
    return true;
}

void NBAllianceHelpReportCell::setData(int index){
    m_index = index;
    std::string uid = GlobalData::shared()->playerInfo.allianceInfo.uid;
    auto info = AllianceManager::getInstance()->m_helpReportListVec.at(index);
    if (info)
    {
        m_resultTxt1->setString(info->getname().c_str());
        
        time_t time = info->gettime();
        string timeStr = CCCommonUtils::timeStampToDate(time);
        m_timeTxt->setString(timeStr.c_str());
        
        string countStr = CC_ITOA(info->getcount());
        m_resultTxt2->setString(countStr.c_str());
        
        m_iconNode->removeAllChildren();
        CCSprite* pic = nullptr;
        if (info->getpic() == "")
        {
            pic = CCLoadSprite::createSprite("guidePlayerIcon.png"); // 66 * 66
            pic->setScale(105 / 66.0);
        }
        else
        {
            string mpic = info->getpic()+".png"; // 105 * 104
            pic = CCLoadSprite::createSprite(mpic.c_str());
        }
        m_iconNode->addChild(pic);
    }
}

bool NBAllianceHelpReportCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_resultTxt1", CCLabelIF*, this->m_resultTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_resultTxt2", CCLabelIF*, this->m_resultTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);

    return false;
}

SEL_CCControlHandler NBAllianceHelpReportCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

bool NBAllianceHelpReportCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_startPoint = pTouch->getLocation();
    return true;
    
}

void NBAllianceHelpReportCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(fabs(pTouch->getLocation().y - m_startPoint.y)>20||fabs(pTouch->getLocation().x - m_startPoint.x)>20){
        return;
    }
    // TODO:
//    if (isTouchInside(this, pTouch)) {
//        auto info = AllianceManager::getInstance()->m_helpReportListVec.at(m_index);
//        
//        PlayerInfoBtnPopUpView* pop =PlayerInfoBtnPopUpView::create();
//        // pop->initHelpBtns(info);
//        PopupViewController::getInstance()->addPopupView(pop, false);
//    }
}
