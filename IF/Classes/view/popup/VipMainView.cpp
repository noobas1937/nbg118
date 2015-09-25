//
//  VipDetailView.cpp
//  IF
//
//  Created by chenliang on 14-3-18.
//
//

#include "VipMainView.h"
#include "VipDetailView.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "VipDetailCell.h"
#include "LocalController.h"
#include "VipUtil.h"


VipMainView::VipMainView()
{
}

bool VipMainView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addPointsBtn", CCControlButton*, this->m_addPointsBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_activateVipBtn", CCControlButton*, this->m_activateVipBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLvText", CCLabelIF*, this->m_vipLvText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLeftTimeText", CCLabelIF*, this->m_vipLeftTimeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loginDayText", CCLabelIF*, this->m_loginDayText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLoginPointText", CCLabelIF*, this->m_vipLoginPointText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vip1text", CCLabelIF*, this->m_vip1text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vip2text", CCLabelIF*, this->m_vip2text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipPoint1Text", CCLabelIF*, this->m_vipPoint1Text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipPoint2Text", CCLabelIF*, this->m_vipPoint2Text);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picHeadNode", CCNode*, this->m_picHeadNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    
    return true;
}

SEL_CCControlHandler VipMainView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoInfoBtnClick", VipMainView::onAddPointBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoInfoBtnClick", VipMainView::onActivityPointBtnClick);
    return NULL;
}
void VipMainView::onAddPointBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    
    
}
void VipMainView::onActivityPointBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    
    
}
VipMainView* VipMainView::create(){
    VipMainView* ret = new VipMainView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
    
}

void VipMainView::onEnter(){
    PopupBaseView::onEnter();
    setTitleName(_lang("103018"));
}

void VipMainView::onExit(){
    PopupBaseView::onExit();
}


bool VipMainView::init(){
    
    bool ret = false;
    if(!PopupBaseView::init())
    {
        // CCBLoadFile("VipMain", this, this);
        return false;
    }
    m_data = CCArray::create();
    auto bg = CCBLoadFile("MailReadView", this, this);
    this->setContentSize(bg->getContentSize());
    setTitleName(_lang("103018"));
    int preHeight = this->m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int dh = m_buildBG->getContentSize().height - preHeight;
    // this->m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height +dh));
    this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
    this->m_listContainer->setPositionY(m_listContainer->getPositionY() - dh);
    
//    m_tableView = CCTableView::create(this, CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height));
//    //tableView->setDelegate(this);
//    m_tableView->setDirection(kCCScrollViewDirectionHorizontal);
//    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
//    m_tableView->setTouchPriority(Touch_Popup);
   // m_listContainer->addChild(m_tableView);
    ret = true;
    VipDetailView* cell = VipDetailView::create();
    m_listContainer->addChild(cell);
    //refresh();
    return ret;
}

void VipMainView::refresh(){
    map<int, CCDictionary*>::iterator it;
    
    for(it = GlobalData::shared()->vipListMap.begin(); it != GlobalData::shared()->vipListMap.end(); it++){
        
        m_data->addObject(it->second);
    }
    

    m_tableView->reloadData();
    
}
//CCSize VipMainView::tableCellSizeForIndex(CCTableView *table, ssize_t idx) {
//    return CCSize(642, 852);
//};
//CCSize VipMainView::cellSizeForTable(CCTableView *table) {
//    return CCSize(642, 852);
//};
//
//CCTableViewCell* VipMainView::tableCellAtIndex(CCTableView *table, ssize_t idx){
//    //CCTableViewCell AchievementNewPopUpView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
//    if(idx >= m_data->count()){
//        return NULL;
//    }
//    VipDetailView* cell = (VipDetailView*)table->dequeueCell();
//    //QuestInfo* info = (QuestInfo*)m_data->objectAtIndex(idx);
//    if(idx < m_data->count()){
//        if(cell){
//            cell->refresh();
//        }else{
//            cell = VipDetailView::create();
//            //  m_cells->addObject(cell);
//        }
//    }
//    return NULL;
//}
//unsigned int VipMainView::numberOfCellsInTableView(CCTableView *table)
//{
//    return m_data->count();
//}
bool VipMainView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (isTouchInside(m_buildBG,pTouch)){
        return true;
    }
    return false;
}
void VipMainView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (fabs(m_touchPoint.y - pTouch->getLocation().y)> 30) {
        return;
    }
    //    if (isTouchInside(m_recommandBG,pTouch)) {
    //        PopupViewController::getInstance()->addPopupInView(AchievementDetailPopUpView::create(m_recommendinfo));
    //    }
}
void VipMainView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (fabs(m_touchPoint.y - pTouch->getLocation().y)> 30) {
        return;
    }
    //    if (isTouchInside(m_recommandBG,pTouch)) {
    //        PopupViewController::getInstance()->addPopupInView(AchievementDetailPopUpView::create(m_recommendinfo));
    //    }
}
