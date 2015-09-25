//
//  ChangeAllianceFlagView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-4.
//
//

#include "ChangeAllianceFlagView.h"
#include "QuestController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "ChangePicCommand.h"
#include "ToolController.h"
#include "UseToolView.h"
#include "ChangeAllianceAttributesCommand.h"

static std::string selectPic = "";

ChangeAllianceFlagView *ChangeAllianceFlagView::create(){
    ChangeAllianceFlagView *ret = new ChangeAllianceFlagView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ChangeAllianceFlagView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
//  CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChangeAllianceFlagView::onPicClick), "on_pic_click", NULL);
    this->refreshView();
}

void ChangeAllianceFlagView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "on_pic_click");
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool ChangeAllianceFlagView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void ChangeAllianceFlagView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_viewBg, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool ChangeAllianceFlagView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCBLoadFile("ChangeAllianceFlagView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        CCLoadSprite::doResourceByCommonIndex(205, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(205, false);
        });
        m_titleText->setString(_lang("115116"));
        CCCommonUtils::setButtonTitle(m_changeBtn, _lang("confirm").c_str());
        m_data = CCArray::create();
        m_cells = CCArray::create();
        for (int i=1; i<=36; i++) {
            std::string ico = "Alliance_symbol0";
            if(i<10){
                ico.append("0");
            }
            ico.append(CC_ITOA(i));
            m_data->addObject(CCString::create(ico));
        }
        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(1);
        m_listContainer->addChild(m_tabView);
        
        if(GlobalData::shared()->playerInfo.allianceInfo.reicon!=0){
            m_goldNode->setVisible(true);
            m_btnTxt->setString(_lang("115061"));
            m_goldTxt->setString(CC_ITOA(GlobalData::shared()->alliance_cost_k3));
            CCCommonUtils::setButtonTitle(m_changeBtn, "");
        }else{
            m_goldNode->setVisible(false);
            CCCommonUtils::setButtonTitle(m_changeBtn, _lang("115061").c_str());
        }
        selectPic = GlobalData::shared()->playerInfo.allianceInfo.icon;
        m_currentPic = "";
        m_tabView->reloadData();
        ret = true;
    }
    return ret;
}

void ChangeAllianceFlagView::onPicClick(CCObject *data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        m_currentPic = str->getCString();
        selectPic = m_currentPic;
    }
}

void ChangeAllianceFlagView::onChangeClick(CCObject *pSender, CCControlEvent event){
    if(m_currentPic==GlobalData::shared()->playerInfo.allianceInfo.icon){
        CCCommonUtils::flyHint("", "", _lang("105228"));
    }else if(m_currentPic!=""){
        ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
        command->putParam("icon", CCString::create(m_currentPic));
        command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChangeAllianceFlagView::saveSuccess), NULL));
        command->sendAndRelease();
    }
    PopupViewController::getInstance()->removePopupView(this);
}

void ChangeAllianceFlagView::saveSuccess(CCObject* p){
    selectPic = m_currentPic;
    GlobalData::shared()->playerInfo.allianceInfo.icon = m_currentPic;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_ALLIANCE_ICON);
    
}
void ChangeAllianceFlagView::refreshView(){
    
}

bool ChangeAllianceFlagView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_changeBtn", CCControlButton*, this->m_changeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goldNode", CCNode*, this->m_goldNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnTxt", CCLabelIF*, this->m_btnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goldTxt", CCLabelIF*, this->m_goldTxt);
    return false;
}

SEL_CCControlHandler ChangeAllianceFlagView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChangeClick", ChangeAllianceFlagView::onChangeClick);
    return NULL;
}

void ChangeAllianceFlagView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize ChangeAllianceFlagView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(100, 103);
}

CCTableViewCell* ChangeAllianceFlagView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    ChangeAllianceFlagCell* cell = (ChangeAllianceFlagCell*)table->dequeueGrid();
    CCString* info = (CCString*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info->getCString());
        }else{
            cell = ChangeAllianceFlagCell::create(info->getCString(),m_listContainer);
            m_cells->addObject(cell);
        }
    }
    return cell;
}

ssize_t ChangeAllianceFlagView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return m_data->count();
}

unsigned int ChangeAllianceFlagView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 6;
}

ChangeAllianceFlagCell *ChangeAllianceFlagCell::create(std::string pic,CCNode* clickNode){
    ChangeAllianceFlagCell *ret = new ChangeAllianceFlagCell(pic,clickNode);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ChangeAllianceFlagCell::setData(std::string pic){
    m_pic = pic;
    std::string head = m_pic;
    head.append(".png");
    m_head->removeAllChildrenWithCleanup(true);
    m_headNode = CCLoadSprite::createSprite(head.c_str());
    m_head->addChild(m_headNode);
    m_select->setVisible(m_pic==selectPic);
}

void ChangeAllianceFlagCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChangeAllianceFlagCell::clickHead), "on_pic_click", NULL);
}

void ChangeAllianceFlagCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "on_pic_click");
    CCNode::onExit();
}

bool ChangeAllianceFlagCell::init(){
    
    this->setContentSize(CCSize(100,100));
    m_head = CCNode::create();
    m_head->setPosition(ccp(55, 45));
    this->addChild(m_head);
    m_select = CCLoadSprite::createScale9Sprite("rect.png");
    m_select->setAnchorPoint(ccp(0.5, 0.5));
    m_select->setContentSize(CCSize(100,100));
    m_select->setPosition(ccp(55, 45));
    this->addChild(m_select);
    
    setData(m_pic);
    return true;
}

bool ChangeAllianceFlagCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_clickNode, pTouch) && isTouchInside(m_headNode, pTouch)){
        m_touchPos = pTouch->getLocation();
        return true;
    }
    return false;
}

void ChangeAllianceFlagCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_headNode, pTouch) && fabsf(pTouch->getLocation().y - m_touchPos.y)<30){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("on_pic_click",CCString::create(m_pic));
        m_select->setVisible(true);
    }
}

void ChangeAllianceFlagCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void ChangeAllianceFlagCell::clickHead(CCObject* data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        std::string clickPic = str->getCString();
        m_select->setVisible(clickPic==m_pic);
    }
}

bool ChangeAllianceFlagCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    return false;
}

SEL_CCControlHandler ChangeAllianceFlagCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}