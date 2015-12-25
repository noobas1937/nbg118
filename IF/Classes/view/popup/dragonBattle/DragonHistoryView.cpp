//
//  DragonHistoryView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/27.
//
//

#include "DragonHistoryView.h"
#include "QuestController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "ChangePicCommand.h"
#include "ToolController.h"
#include "UseToolView.h"
#include "WorldController.h"
#include "ParticleFireAni.h"
#include "YuanJunDetailView.h"
#include "CancelAllianceTeamCommand.h"
#include "AllianceFlagPar.h"
#include "AllianceManager.h"
#include "AllianceFightInfo.h"
#include "WorldCommand.h"
#include "AllianceFightBulletinCommand.h"

#define  HISTORY_CELL_CLICK "HISTORY_CELL_CLICK"

DragonHistoryView *DragonHistoryView::create(){
    DragonHistoryView *ret = new DragonHistoryView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonHistoryView::onEnter(){
    this->setTitleName(_lang("140034"));
    PopupBaseView::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonHistoryView::updateData), HISTORY_CELL_CLICK, NULL);

}

void DragonHistoryView::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, HISTORY_CELL_CLICK);
    PopupBaseView::onExit();
}

bool DragonHistoryView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void DragonHistoryView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

}

void DragonHistoryView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}

bool DragonHistoryView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(204, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(204, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
        });
        auto node = CCBLoadFile("DragonHistoryView", this, this);
        this->setContentSize(node->getContentSize());
        
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
        m_buttom->setPositionY(m_buttom->getPositionY() - dh);
 
        m_aTitleTxt->setString(_lang("140070"));
        m_aRankTxt->setString(_lang("140071"));
        CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("138003").c_str());
        
        m_data = CCArray::create();
        
        m_scrollView = CCScrollView::create(m_infoList->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_infoList->addChild(m_scrollView);
        
        for (int i=0; i<2; i++) {
            DragonHistoryRecordCell* record = DragonHistoryRecordCell::create(CCDictionary::create(), i,m_infoList);
            m_scrollView->addChild(record);
            m_data->addObject(record);
        }
        updatePosition(0);
        ret = true;
    }
    return ret;
}

void DragonHistoryView::updateData(CCObject* param){
    CCString* strObj = dynamic_cast<CCString*>(param);
    if (strObj) {
        string str = strObj->getCString();
        int index = -1;
        int type = -1;
        std::vector<std::string> vArr;
        CCCommonUtils::splitString(str,"_",vArr);
        if (vArr.size()==1) {
            type = atoi(vArr[0].c_str());
        }
        if (vArr.size()==2) {
            type = atoi(vArr[0].c_str());
            index = atoi(vArr[1].c_str());
        }
        this->updatePosition(type,index);
    }
}

void DragonHistoryView::updatePosition(int type,int index){
    int total = 0;
    int num = m_data->count();
    for (int i=0; i<num; i++) {
        DragonHistoryRecordCell* record = dynamic_cast<DragonHistoryRecordCell*>(m_data->objectAtIndex(i));
        if(type==i && index != -1){
            record->showTowMembers(index);
        }else if (type==i && !record->isOpen) {
            record->showTowMembers(index);
        }else{
            record->reset();
        }
        total += record->getCellHeight();
    }
    
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,total));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - total));
    
    total -= 80;
    for (int i=0; i<num; i++) {
        DragonHistoryRecordCell* record = dynamic_cast<DragonHistoryRecordCell*>(m_data->objectAtIndex(i));
        record->setPositionY(total);
        total -= record->getCellHeight();
    }
}

bool DragonHistoryView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_aTitleTxt", CCLabelIF*, this->m_aTitleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_aRankTxt", CCLabelIF*, this->m_aRankTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardBtn", CCControlButton*, this->m_rewardBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tip", CCLabelIF*, this->m_tip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_buttom", CCNode*, this->m_buttom);
    return false;
}

SEL_CCControlHandler DragonHistoryView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}

//

DragonHistoryRecordCell *DragonHistoryRecordCell::create(CCDictionary* info,int type,CCNode* clickArea){
    DragonHistoryRecordCell *ret = new DragonHistoryRecordCell(info,type,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonHistoryRecordCell::onEnter(){
    setSwallowsTouches(false);
    setTouchEnabled(true);
    
    CCNode::onEnter();
}

void DragonHistoryRecordCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool DragonHistoryRecordCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_bg, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        return true;
    }
    return false;
}

void DragonHistoryRecordCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void DragonHistoryRecordCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_bg, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(HISTORY_CELL_CLICK,CCString::create(CC_ITOA(m_type)));
    }
}

bool DragonHistoryRecordCell::init(){
    auto node = CCBLoadFile("DragonHistoryRecordCell", this, this);
    this->setContentSize(node->getContentSize());
    
    m_srcData = CCArray::create();
    m_srcData->addObject(CCDictionary::create());
    m_srcData->addObject(CCDictionary::create());
    m_srcData->addObject(CCDictionary::create());
    
    m_data = CCArray::create();
    isOpen = false;
    m_totalH = 0;
    for (int i=0; i<3; i++) {
        DragonHistoryTowCell* record = DragonHistoryTowCell::create(CCDictionary::create(),i,m_type,m_clickArea);
        m_data->addObject(record);
    }
    return true;
}

void DragonHistoryRecordCell::updateTowData(int index){
    int total = -80;
    int num = m_data->count();
    for (int i=0; i<num; i++) {
        DragonHistoryTowCell* record = dynamic_cast<DragonHistoryTowCell*>(m_data->objectAtIndex(i));
        if (index==i && !record->isOpen) {
            record->showMember();
        }else{
            record->reset();
        }
        record->setPositionY(total);
        total -= record->getCellHeight();
        if (record->getParent()==NULL) {
            this->addChild(record);
        }
    }
    m_totalH = -total;
}

int DragonHistoryRecordCell::getCellHeight(){
    int h = 94;
    if (isOpen) {
        h += m_totalH;
    }
    return h;
}

void DragonHistoryRecordCell::reset(){
    isOpen = false;
    m_arrow->setRotation(0);
    int num = m_data->count();
    for (int i=0; i<num; i++) {
        DragonHistoryTowCell* record = dynamic_cast<DragonHistoryTowCell*>(m_data->objectAtIndex(i));
        record->reset();
        record->removeFromParent();
    }
}

void DragonHistoryRecordCell::showTowMembers(int index){
    isOpen = true;
    m_arrow->setRotation(90);
    updateTowData(index);
}

void DragonHistoryRecordCell::setData(CCDictionary* info,int type){

}

bool DragonHistoryRecordCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIFTTF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankIcon", CCNode*, this->m_rankIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCSprite*, this->m_arrow);
    return false;
}

SEL_CCControlHandler DragonHistoryRecordCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
// tow
DragonHistoryTowCell *DragonHistoryTowCell::create(CCDictionary* info,int index,int type,CCNode* clickArea){
    DragonHistoryTowCell *ret = new DragonHistoryTowCell(info,index,type,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonHistoryTowCell::onEnter(){
    setSwallowsTouches(false);
    setTouchEnabled(true);
    CCNode::onEnter();
}

void DragonHistoryTowCell::onExit(){
    setTouchEnabled(false);
    
    CCNode::onExit();
}

bool DragonHistoryTowCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_bg, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        return true;
    }
    return false;
}

void DragonHistoryTowCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void DragonHistoryTowCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_bg, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        string temp = CC_ITOA(m_type);
        temp.append("_");
        temp.append(CC_ITOA(m_index));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(HISTORY_CELL_CLICK,CCString::create(temp.c_str()));
    }
}

bool DragonHistoryTowCell::init(){
    auto node = CCBLoadFile("DragonHistoryTowCell", this, this);
    this->setContentSize(node->getContentSize());
    
    m_srcData = CCArray::create();
    m_srcData->addObject(CCDictionary::create());
    m_srcData->addObject(CCDictionary::create());
    //m_srcData->addObject(CCDictionary::create());
    
    m_data = CCArray::create();
    isOpen = false;
    m_tableHeight = 300;
    
    CCNode* tNode = CCNode::create();
    tNode->setContentSize(CCSize(640,m_tableHeight));
    tNode->setAnchorPoint(ccp(0, 0));
    node->addChild(tNode,-1000);
    tNode->setPositionY(-m_tableHeight);
    
    m_tabView = CCMultiColTableView::create(this, tNode->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    tNode->addChild(m_tabView);
    
    m_tabView->setTouchEnabled(false);
    
    return true;
}

int DragonHistoryTowCell::getCellHeight(){
    int h = 94;
    if (isOpen) {
        h += m_tableHeight;
    }
    return h;
}

void DragonHistoryTowCell::reset(){
    isOpen = false;
    m_arrow->setRotation(0);
    m_data->removeAllObjects();
    m_tabView->reloadData();
}

void DragonHistoryTowCell::showMember(){
    isOpen = true;
    m_arrow->setRotation(90);
    m_data->removeAllObjects();
    m_data->addObjectsFromArray(m_srcData);
    m_tabView->reloadData();
}

void DragonHistoryTowCell::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize DragonHistoryTowCell::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(320, 230);
}

CCTableViewCell* DragonHistoryTowCell::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    DragonHistoryMemberCell* cell = (DragonHistoryMemberCell*)table->dequeueGrid();
    CCDictionary* info = (CCDictionary*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = DragonHistoryMemberCell::create(info);
        }
    }
    return cell;
}

ssize_t DragonHistoryTowCell::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = m_data->count();
    return num/2 + (num%2==1?1:0);
}

unsigned int DragonHistoryTowCell::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 2;
}

void DragonHistoryTowCell::setData(CCDictionary* info,int index){
    
}

bool DragonHistoryTowCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIFTTF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankIcon", CCNode*, this->m_rankIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCSprite*, this->m_arrow);
    return false;
}

SEL_CCControlHandler DragonHistoryTowCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

//
DragonHistoryMemberCell *DragonHistoryMemberCell::create(CCDictionary* info){
    DragonHistoryMemberCell *ret = new DragonHistoryMemberCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonHistoryMemberCell::onEnter(){
    CCNode::onEnter();
}

void DragonHistoryMemberCell::onExit(){
    CCNode::onExit();
}

bool DragonHistoryMemberCell::init(){
    auto node = CCBLoadFile("DragonHistoryMemberCell", this, this);
    this->setContentSize(node->getContentSize());

    return true;
}

void DragonHistoryMemberCell::setData(CCDictionary* info){
    
}

bool DragonHistoryMemberCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){

    return false;
}

SEL_CCControlHandler DragonHistoryMemberCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}