//
//  FBAppRequestFriendView.cpp
//  IF
//
//  Created by ganxiaohua on 15/3/27.
//
//

#include "FBAppRequestFriendView.h"
#include "PopupViewController.h"
#include "FBUtilies.h"
#include "LoadWebImg.h"
#include "ParticleFireAni.h"
#include "FBMineInviteCommand.h"
#include "DataRecordCommand.h"
#include <spine/Json.h>
#include <cctype>
#include <algorithm>
#include "FacebookInviteView.h"
#include "InviteFBIdAcceptCommand.h"
#include "GameController.h"
#include "YesNoDialog.h"
#include "InviteForGiftView.h"

FBAppRequestFriendView::~FBAppRequestFriendView()
{
    
}

FBAppRequestFriendView::FBAppRequestFriendView(){
    
}

FBAppRequestFriendView *FBAppRequestFriendView::create(CCArray* arr){
    FBAppRequestFriendView *ret = new FBAppRequestFriendView();
    if(ret && ret->init(arr)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


void FBAppRequestFriendView::onEnter(){
    PopupBaseView::onEnter();
    this->setTitleName(_lang("107092"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void FBAppRequestFriendView::onExit(){
    if (m_waitInterface1) {
        m_waitInterface1->remove();
        m_waitInterface1 = NULL;
    }
    GameController::getInstance()->removeWaitInterface();
    GlobalData::shared()->isBind = false;
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool FBAppRequestFriendView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void FBAppRequestFriendView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void FBAppRequestFriendView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}

bool FBAppRequestFriendView::init(CCArray* arr){
    bool ret = false;
    if(PopupBaseView::init()){
        auto node = CCBLoadFile("FBAppRequestFriendView", this, this);
        this->setContentSize(node->getContentSize());
        int prev = m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height-prev;
        
        m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width,m_listContainer->getContentSize().height+add));
        m_listContainer->setPositionY(m_listContainer->getPositionY() - add);
        //CCCommonUtils::setButtonTitle(m_searchBtn, _lang("115269").c_str());
        auto spriteText = CCLoadSprite::createScale9Sprite("UI_chat_text.png");
        m_inputName = CCEditBox::create(CCSizeMake(490.0,47),spriteText);
        m_inputName->setMaxLength(20);
        m_inputName->setFontSize(24);
        m_inputName->setFontColor(ccBLACK);
        m_inputName->setReturnType(kKeyboardReturnTypeDone);
        m_inputName->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
        m_inputName->setText("");
        m_inputName->setPlaceHolder("");
        m_searchNode->addChild(m_inputName);
        
        m_data = CCArray::create();
        m_srcData = CCArray::create();
        if(arr){
            m_srcData->addObjectsFromArray(arr);
            m_data->addObjectsFromArray(arr);
        }
        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(Touch_Popup);
        m_listContainer->addChild(m_tabView);
        
        m_tip->setString(_lang("107104"));
        m_tip->setVisible(m_srcData->count()<=0);
        
        m_tabView->reloadData();
        m_fbUids = "";
        m_waitInterface1 = NULL;
        ret = true;
    }
    return ret;
}

void FBAppRequestFriendView::refreshData(float t){
    if(m_data->count()>0){
        this->unschedule(schedule_selector(FBAppRequestFriendView::refreshData));
        m_tabView->reloadData();
    }
    m_tip->setVisible(m_data->count()<=0);
}


void FBAppRequestFriendView::onSearchClick(CCObject *pSender, CCControlEvent event){
    std::string sName = m_inputName->getText();
    int num = m_srcData->count();
    m_data->removeAllObjects();
    std::transform(sName.begin(),sName.end(),sName.begin(),::toupper);
    for (int i=0; i<num; i++) {
        auto dic = _dict(m_srcData->objectAtIndex(i));
        if (dic!=NULL) {
            string name = dic->valueForKey("fromName")->getCString();
            std::transform(name.begin(),name.end(),name.begin(),::toupper);
            if (name.find(sName) < name.length()) {
                m_data->addObject(dic);
            }
        }
    }
    refreshData(0.0);
}

bool FBAppRequestFriendView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tip", CCLabelIF*, this->m_tip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_searchBtn", CCControlButton*, this->m_searchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_searchNode", CCNode*, this->m_searchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_container", CCNode*, this->m_container);
    return false;
}

SEL_CCControlHandler FBAppRequestFriendView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSearchClick", FBAppRequestFriendView::onSearchClick);
    return NULL;
}

void FBAppRequestFriendView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize FBAppRequestFriendView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(600, 170);
}

CCTableViewCell* FBAppRequestFriendView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    RequestFriendCell* cell = (RequestFriendCell*)table->dequeueGrid();
    CCDictionary* friendInfo = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    if(idx < m_data->count()){
        if(cell){
            cell->setData(friendInfo);
        }else{
            cell = RequestFriendCell::create(friendInfo,m_listContainer);
        }
    }
    return cell;
}

ssize_t FBAppRequestFriendView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = m_data->count();
    return num;
}

unsigned int FBAppRequestFriendView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}

RequestFriendCell *RequestFriendCell::create(CCDictionary* info,CCNode* clickArea){
    RequestFriendCell *ret = new RequestFriendCell(info,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void RequestFriendCell::setData(CCDictionary* info){
    m_info = info;
    if (m_info==NULL) {
        return;
    }
    m_head->removeAllChildrenWithCleanup(true);
    m_fromId = info->valueForKey("fromId")->getCString();
    string pic = CCString::createWithFormat("https://graph.facebook.com/%s/picture?type=square",m_fromId.c_str())->getCString();
    LoadWebImg* img = LoadWebImg::create();
    img->setLoadURL(pic);
    img->setScale(1.7);
    m_head->addChild(img);
    string name = m_info->valueForKey("fromName")->getCString();
    m_nameTxt->setString(name.c_str());
    string timeStr = m_info->valueForKey("created_time")->getCString();
    if(timeStr.length()>11){
        timeStr = CCCommonUtils::subStrByUtf8(timeStr, 0, 10);
    }
    m_timeTxt->setString(timeStr.c_str());
}

void RequestFriendCell::onEnter(){
    CCNode::onEnter();
    
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void RequestFriendCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool RequestFriendCell::init(){
    auto bg = CCBLoadFile("RequestFriendCell", this, this);
    this->setContentSize(bg->getContentSize());
    m_acceptBtn->setTouchPriority(Touch_Popup_Item);
    CCCommonUtils::setButtonTitle(m_acceptBtn, _lang("107093").c_str());
    m_fromId = "";
    setData(m_info);
    return true;
}

bool RequestFriendCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_isDraging = false;
    if (isTouchInside(m_acceptBtn, pTouch)) {
        m_acceptBtn->setHighlighted(true);
    }
    if(isTouchInside(m_clickArea, pTouch)){
        return true;
    }
    return false;
}

void RequestFriendCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    m_acceptBtn->setHighlighted(false);
    if(isTouchInside(m_clickArea, pTouch) && isTouchInside(m_acceptBtn, pTouch)){
        CCLOG("click m_acceptBtn");
        YesNoDialog* dialog = YesNoDialog::showVariableTitle(_lang("107094").c_str(),CCCallFunc::create(this, callfunc_selector(RequestFriendCell::confirmFun)),_lang("confirm").c_str());
        CCCommonUtils::setButtonTitle(dialog->m_btnCancel, _lang("cancel_btn_label").c_str());
        dialog->showCancelButton();
    }
}

void RequestFriendCell::serverCallBack(CCObject* data){
    GameController::getInstance()->removeWaitInterface();
    NetResult* result = dynamic_cast<NetResult*>(data);
    if (result==NULL) {
        return ;
    }
    CCDictionary* params = _dict(result->getData());
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(FB_ACCEPT_APP_REQUEST,params);
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(InviteForGiftView::create());
    CCLOG("serverCallBack");
}

void RequestFriendCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_acceptBtn, pTouch)) {
        m_acceptBtn->setHighlighted(true);
    }
}

void RequestFriendCell::confirmFun(){
    if (m_fromId!="") {
        InviteFBIdAcceptCommand* cmd = new InviteFBIdAcceptCommand(m_fromId);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(RequestFriendCell::serverCallBack), NULL));
        cmd->sendAndRelease();
        GameController::getInstance()->showWaitInterface();
    }
}

bool RequestFriendCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_head", CCNode*, this->m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIFTTF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_acceptBtn", CCControlButton*, this->m_acceptBtn);
    return false;
}

SEL_CCControlHandler RequestFriendCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}