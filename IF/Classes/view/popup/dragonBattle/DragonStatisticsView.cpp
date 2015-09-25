//
//  DragonStatisticsView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/31.
//
//

#include "DragonStatisticsView.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "ToolController.h"
#include "SceneController.h"
#include "YesNoDialog.h"
#include "MoveCityPopUpView.h"
#include "WorldMapView.h"
#include "BuildQueueView.h"
#include "WinPointsUseCommand.h"

DragonStatisticsView *DragonStatisticsView::create(){
    DragonStatisticsView *ret = new DragonStatisticsView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonStatisticsView::onEnter(){
    PopupBaseView::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);

}

void DragonStatisticsView::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    PopupBaseView::onExit();
}


bool DragonStatisticsView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void DragonStatisticsView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30){
        return ;
    }
}

bool DragonStatisticsView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(105, true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(105, false);
            CCLoadSprite::doResourceByCommonIndex(500, false);
        });
        CCBLoadFile("DragonStatisticsView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        m_data = CCArray::create();
        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionHorizontal);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(Touch_Default);
        m_listContainer->addChild(m_tabView);
        
        m_tabView->reloadData();
        
        ret = true;
    }
    return ret;
}

void DragonStatisticsView::onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->removePopupView(this);
}

bool DragonStatisticsView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_list", CCNode*, this->m_list);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_speedTxt", CCLabelIF*, this->m_speedTxt);
    return false;
}

SEL_CCControlHandler DragonStatisticsView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", DragonStatisticsView::onCloseBtnClick);
    return NULL;
}



void DragonStatisticsView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize DragonStatisticsView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(640, 120);
}

CCTableViewCell* DragonStatisticsView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    DragonStatisticsCell* cell = (DragonStatisticsCell*)table->dequeueGrid();
    CCString* info = (CCString*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData();
        }else{
            cell = DragonStatisticsCell::create();
        }
    }
    return cell;
}

ssize_t DragonStatisticsView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return m_data->count();
}

unsigned int DragonStatisticsView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}

DragonStatisticsCell *DragonStatisticsCell::create(){
    DragonStatisticsCell *ret = new DragonStatisticsCell();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonStatisticsCell::setData(){
    m_txt1->setString(_lang("R2"));
    m_txt2->setString("test");
    m_txt3->setString("9999");
    CCCommonUtils::setButtonTitle(m_battleBtn, _lang("140049").c_str());
}

void DragonStatisticsCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
}

void DragonStatisticsCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool DragonStatisticsCell::init(){
    auto bg = CCBLoadFile("DragonStatisticsCell", this, this);
    this->setContentSize(bg->getContentSize());
    setData();
    return true;
}

bool DragonStatisticsCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void DragonStatisticsCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30) {
        return ;
    }
    
}

void DragonStatisticsCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}

void DragonStatisticsCell::onClickBatlle(CCObject *pSender, CCControlEvent event){
    
}

bool DragonStatisticsCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_battleBtn", CCControlButton*, this->m_battleBtn);
    
    return false;
}
SEL_CCControlHandler DragonStatisticsCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onManageClick", DragonStatisticsCell::onClickBatlle);
    return NULL;
}