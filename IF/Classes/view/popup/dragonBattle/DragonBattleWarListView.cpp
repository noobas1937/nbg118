//
//  DragonBattleWarListView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/25.
//
//

#include "DragonBattleWarListView.h"
#include "PopupViewController.h"
#include "CommandBase.h"
#include "YesNoDialog.h"
#include "DragonBattleApplyCommand.h"

DragonBattleWarListView *DragonBattleWarListView::create(){
    DragonBattleWarListView *ret = new DragonBattleWarListView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonBattleWarListView::onEnter(){
    PopupBaseView::onEnter();
}

void DragonBattleWarListView::onExit(){
    PopupBaseView::onExit();
}

bool DragonBattleWarListView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void DragonBattleWarListView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}

bool DragonBattleWarListView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(506, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(506, false);
        });
        auto node = CCBLoadFile("DragonBattleWarListView", this, this);
        this->setContentSize(node->getContentSize());
        int prev = m_viewBg->getContentSize().height;
        this->changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height - prev;
        m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height + add));
        m_infoList->setPositionY(m_infoList->getPositionY() - add);
        
        m_tipTxt->setString(_lang("140015").c_str());
        
        m_data = CCArray::create();
        m_data->addObject(CCDictionary::create());
        m_data->addObject(CCDictionary::create());
        m_data->addObject(CCDictionary::create());
        m_data->addObject(CCDictionary::create());
        
        m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        
        m_tabView->reloadData();
        
        ret = true;
    }
    return ret;
}

void DragonBattleWarListView::getData(CCObject *param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){
        
    }
}


bool DragonBattleWarListView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt", CCLabelIF*, this->m_tipTxt);
    return false;
}

SEL_CCControlHandler DragonBattleWarListView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}

void DragonBattleWarListView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize DragonBattleWarListView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(320, 160);
}

CCTableViewCell* DragonBattleWarListView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    DragonWarCell* cell = (DragonWarCell*)table->dequeueGrid();
    CCDictionary* dic = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    if(idx < m_data->count()){
        if(cell){
            cell->setData();
        }else{
            cell = DragonWarCell::create();
        }
    }
    return cell;
}

ssize_t DragonBattleWarListView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = m_data->count();
    return num/2 + (num%2==1?1:0);
}

unsigned int DragonBattleWarListView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 2;
}

//
DragonWarCell *DragonWarCell::create(){
    DragonWarCell *ret = new DragonWarCell();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonWarCell::setData(){
    m_txt1->setString(_lang("140016"));
    time_t infoTime = GlobalData::shared()->getTimeStamp();
    string dateTime = CCCommonUtils::timeStampToYMD(infoTime);
    m_txt2->setString(dateTime);
    m_txt3->setString(_lang_2("138037","2","4"));
    m_txt4->setString(_lang_1("140017","11"));
}

void DragonWarCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
}

void DragonWarCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool DragonWarCell::init(){
    auto bg = CCBLoadFile("DragonWarCell", this, this);
    this->setContentSize(bg->getContentSize());
    setData();
    return true;
}

bool DragonWarCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_clickNode, pTouch)){
        return true;
    }
    return false;
}

void DragonWarCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30) {
        return ;
    }
    if(isTouchInside(m_clickNode, pTouch)){
        YesNoDialog::show(_lang_2("140019","2","3"), CCCallFunc::create(this, callfunc_selector(DragonWarCell::confirmFun)));
    }

}

void DragonWarCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}

void DragonWarCell::confirmFun(){
    DragonBattleApplyCommand* cmd = new DragonBattleApplyCommand();
    cmd->sendAndRelease();
}

bool DragonWarCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt4", CCLabelIF*, this->m_txt4);
    
    return false;
}

SEL_CCControlHandler DragonWarCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
