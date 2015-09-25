//
//  FBHelpRecordView.cpp
//  IF
//
//  Created by ganxiaohua on 14-11-18.
//
//

#include "FBHelpRecordView.h"
#include "MineHelpInfoCommand.h"
#include "PopupViewController.h"

FBHelpRecordView *FBHelpRecordView::create(){
    FBHelpRecordView *ret = new FBHelpRecordView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void FBHelpRecordView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void FBHelpRecordView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool FBHelpRecordView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void FBHelpRecordView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos = pTouch->getLocation();
    if (fabsf(m_touchPos.y - pos.y)>30) {
    }
}

void FBHelpRecordView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_viewBg, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool FBHelpRecordView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto node = CCBLoadFile("FBHelpRecordView", this, this);
        this->setContentSize(node->getContentSize());
        
        m_rewardTitle->setString(_lang("107065"));
        
        m_data = CCArray::create();
        
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        
        m_tip->setString(_lang("107066"));
        m_tip->setVisible(false);
        MineHelpInfoCommand* cmd = new MineHelpInfoCommand();
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(FBHelpRecordView::getServerData), NULL));
        cmd->sendAndRelease();
        ret = true;
    }
    return ret;
}

void FBHelpRecordView::getServerData(CCObject* data){
    this->removeLoadingAni();
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        m_data->removeAllObjects();
        CCArray* array = (CCArray*)dic->objectForKey("result");
        m_data->addObjectsFromArray(array);
        CCPoint curr = m_tabView->getContentOffset();
        CCPoint max = m_tabView->maxContainerOffset();
        CCPoint min = m_tabView->minContainerOffset();
        m_tabView->reloadData();
    }
    m_tip->setVisible(m_data->count()<=0);
}

void FBHelpRecordView::callMore(){
    int start = m_data->count();
    int end = start+20;
    //addLoadingAni();
}

void FBHelpRecordView::addLoadingAni(){
    if (m_loadingIcon==NULL) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_infoList->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_infoList->addChild(m_loadingIcon,1000000);
    }
}

void FBHelpRecordView::removeLoadingAni(){
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}

bool FBHelpRecordView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardTitle", CCLabelIF*, this->m_rewardTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tip", CCLabelIF*, this->m_tip);
    return false;
}

SEL_CCControlHandler FBHelpRecordView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}

CCSize FBHelpRecordView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    return CCSize(590, 45);
}

CCSize FBHelpRecordView::cellSizeForTable(CCTableView *table)
{
    return CCSize(590, 45);
}

CCTableViewCell* FBHelpRecordView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    FBHelpRecordCell* cell = (FBHelpRecordCell*)table->dequeueCell();
    auto info = _dict(m_data->objectAtIndex(idx));
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = FBHelpRecordCell::create(info);
        }
    }
    return cell;
}

ssize_t FBHelpRecordView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void FBHelpRecordView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

//

FBHelpRecordCell *FBHelpRecordCell::create(CCDictionary* info){
    FBHelpRecordCell *ret = new FBHelpRecordCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void FBHelpRecordCell::onEnter(){
    CCNode::onEnter();
}

void FBHelpRecordCell::onExit(){
    CCNode::onExit();
}

bool FBHelpRecordCell::init(){
    auto node = CCBLoadFile("FBHelpRecordCell", this, this);
    this->setContentSize(node->getContentSize());
    setData(m_info);
    return true;
}

void FBHelpRecordCell::setData(CCDictionary* info){
    m_info = info;
    string name = m_info->valueForKey("name")->getCString();
    m_descTxt->setString(_lang_1("107064",name.c_str()));
    auto helpTime = m_info->valueForKey("time")->doubleValue();
    auto dt = GlobalData::shared()->getWorldTime() - GlobalData::shared()->changeTime(helpTime/1000);
    string timestr = "";
    int timedt = 0;
    if(dt>=24*60*60){
        timedt =dt/(24*60*60);
        timestr = CC_ITOA(timedt);
        timestr.append(_lang("105592"));
    }else if(dt>=60*60){
        timedt =dt/(60*60);
        timestr = CC_ITOA(timedt);
        timestr.append(_lang("105591"));
    }else if(dt>=60){
        timedt =dt/60;
        timestr = CC_ITOA(timedt);
        timestr.append(_lang("105590"));
    }else{
        timestr ="1";
        timestr.append(_lang("105590"));
    }
    timestr.append(" ");
    timestr.append(_lang("105593"));
    this->m_timeTxt->setString(timestr);
}

bool FBHelpRecordCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_descTxt", CCLabelIFTTF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    return false;
}

SEL_CCControlHandler FBHelpRecordCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
