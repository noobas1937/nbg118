//
//  ChangePicView.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-23.
//
//

#include "ChangePicView.h"
#include "QuestController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "ChangePicCommand.h"
#include "ToolController.h"
#include "UseToolView.h"

ChangePicView *ChangePicView::create(){
    ChangePicView *ret = new ChangePicView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ChangePicView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChangePicView::onPicClick), "on_pic_click", NULL);
    this->refreshView();
}

void ChangePicView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "on_pic_click");
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool ChangePicView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void ChangePicView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_viewBg, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool ChangePicView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCBLoadFile("ChangePicView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        m_titleText->setString(_lang("105226"));
        CCCommonUtils::setButtonTitle(m_changeBtn, _lang("confirm").c_str());
        m_data = CCArray::create();
        m_cells = CCArray::create();
        const char* icons[11] = {"g044","g041","g024","g026","g015","g032","g008","g038","g012","g007","g045"};
        //const char* icons[8] = {"g044","g041","g024","g026","g015","g032","g008","g038"};
        ////for (int i=1; i<9; i++) {
        for (int i=1; i<12; i++) {
            m_data->addObject(CCString::create(icons[i-1]));
        }
        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(Touch_Popup);
        m_listContainer->addChild(m_tabView);
        
        m_currentPic = "";
        m_tabView->reloadData();
        ret = true;
    }
    return ret;
}

void ChangePicView::onPicClick(CCObject *data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        m_currentPic = str->getCString();
    }
}

void ChangePicView::onChangeClick(CCObject *pSender, CCControlEvent event){
    if(m_currentPic==GlobalData::shared()->playerInfo.pic){
        CCCommonUtils::flyHint("", "", _lang("105228"));
    }else if(m_currentPic!=""){
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(ITEM_CHANGE_PIC);
        if (info.getCNT() >= 1) {
            ToolController::getInstance()->useItemChnagePic(ITEM_CHANGE_PIC, m_currentPic);
        }else{
            auto dict = CCDictionary::create();
            dict->setObject(CCString::create(m_currentPic), "pic");
            PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_CHANGE_PIC,dict));
        }
    }
    PopupViewController::getInstance()->removePopupView(this);
}

void ChangePicView::refreshView(){

}

bool ChangePicView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_changeBtn", CCControlButton*, this->m_changeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    return false;
}


SEL_CCControlHandler ChangePicView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChangeClick", ChangePicView::onChangeClick);
    return NULL;
}

void ChangePicView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize ChangePicView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(150, 180);
}

CCTableViewCell* ChangePicView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    ChangePicCell* cell = (ChangePicCell*)table->dequeueGrid();
    CCString* info = (CCString*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info->getCString());
        }else{
            cell = ChangePicCell::create(info->getCString());
            m_cells->addObject(cell);
        }
    }
    return cell;
}

ssize_t ChangePicView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return m_data->count();
}

unsigned int ChangePicView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 4;
}

ChangePicCell *ChangePicCell::create(std::string pic){
    ChangePicCell *ret = new ChangePicCell(pic);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ChangePicCell::setData(std::string pic){
    m_pic = pic;
    std::string head = m_pic;
    head.append(".png");
    m_head->removeAllChildrenWithCleanup(true);
    m_headNode = CCLoadSprite::createSprite(head.c_str());
    m_headNode->setScale(0.7);
    m_head->addChild(m_headNode);
    m_selectNode->setVisible(head==GlobalData::shared()->playerInfo.getPic());
}

void ChangePicCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChangePicCell::clickHead), "on_pic_click", NULL);
}

void ChangePicCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "on_pic_click");
    CCNode::onExit();
}

bool ChangePicCell::init(){
    auto bg = CCBLoadFile("ChangePicCell", this, this);
    this->setContentSize(bg->getContentSize());

    setData(m_pic);
    return true;
}

bool ChangePicCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_isDraging = false;
    if(isTouchInside(m_clickNode, pTouch)){
        return true;
    }
    return false;
}

void ChangePicCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_headNode, pTouch)){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("on_pic_click",CCString::create(m_pic));
        m_selectNode->setVisible(true);
    }
}

void ChangePicCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

}

void ChangePicCell::changePic(){

}

void ChangePicCell::clickHead(CCObject* data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        std::string clickPic = str->getCString();
        m_selectNode->setVisible(clickPic==m_pic);
    }
}

void ChangePicCell::changeSuccess(){
    GlobalData::shared()->playerInfo.pic = m_pic;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_PLAYER_PIC);
}

bool ChangePicCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_head", CCNode*, this->m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_selectNode", CCNode*, this->m_selectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCNode*, this->m_clickNode);
    return false;
}

SEL_CCControlHandler ChangePicCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}