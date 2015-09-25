//
//  ShieldUnlockPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-2-8.
//
//

#include "ShieldUnlockPopUpView.h"
#include "PopupViewController.h"
#include "SettingPopUpView.h"
#include "ChatLockInfoCommand.h"
#include "ChatUnLockCommand.h"

ShieldUnlockPopUpView *ShieldUnlockPopUpView::create(){
    ShieldUnlockPopUpView *ret = new ShieldUnlockPopUpView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ShieldUnlockPopUpView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ShieldUnlockPopUpView::refreshView), REFRESH_SHIELD_PANEL, NULL);
}

void ShieldUnlockPopUpView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, REFRESH_SHIELD_PANEL);
    PopupBaseView::onExit();
}

bool ShieldUnlockPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        auto bg = CCBLoadFile("ShieldUnLockView", this, this);
        this->setContentSize(bg->getContentSize());
        setTitleName(_lang("105315").c_str());
        changeBGHeight(m_BG);
        m_container->setContentSize(CCSizeMake(m_container->getContentSize().width, getBGHeight()-10));
        m_tabView = CCMultiColTableView::create(this, CCSize(m_container->getContentSize().width, m_container->getContentSize().height));
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(1);
        this->m_container->addChild(m_tabView);
        if(GlobalData::shared()->chatShieldInfo.isGetInfo){
            loadDataBack();
        }else{
            getData();
        }
        ret = true;
    }
    return ret;
}

bool ShieldUnlockPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_container", CCNode*, this->m_container);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BG", CCScale9Sprite*, m_BG);
    return true;
}

void ShieldUnlockPopUpView::getData(){
    ChatLockInfoCommand *cmd = new ChatLockInfoCommand();
    cmd->sendAndRelease();
}

SEL_CCControlHandler ShieldUnlockPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", ShieldUnlockPopUpView::onCloseBtnClick);
    return NULL;
}

void ShieldUnlockPopUpView::onCloseBtnClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->goBackPopupView();
}

cocos2d::CCSize ShieldUnlockPopUpView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSizeMake(365, 118);
}

CCTableViewCell* ShieldUnlockPopUpView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    auto &arr = GlobalData::shared()->chatShieldInfo.shieldList;
    if(idx >= arr->count()){
        return NULL;
    }
    ShieldInfo *info = dynamic_cast<ShieldInfo*>(arr->objectAtIndex(idx));
    ShieldUnlockCellView *cell = (ShieldUnlockCellView*)table->dequeueGrid();
    if(idx < arr->count()){
        if(cell){
            cell->setCellData(info);
        }else{
            cell = ShieldUnlockCellView::create(info);
        }
        return cell;
    }    
    return NULL;
}

ssize_t ShieldUnlockPopUpView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = ceil(GlobalData::shared()->chatShieldInfo.shieldList->count() / NUM_PER_COL);
    return num;
}

unsigned int ShieldUnlockPopUpView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return NUM_PER_COL;
}

void ShieldUnlockPopUpView::refreshView(CCObject* p){
    m_tabView->reloadData();
}

void ShieldUnlockPopUpView::loadDataBack(){
    refreshView(NULL);
}

//----------------

ShieldUnlockCellView* ShieldUnlockCellView::create(ShieldInfo *info){
    ShieldUnlockCellView *ret = new ShieldUnlockCellView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ShieldUnlockCellView::onEnter(){
    CCNode::onEnter();
}

void ShieldUnlockCellView::onExit(){
    CCNode::onExit();
}

bool ShieldUnlockCellView::init(){
    CCBLoadFile("ShieldUnlockCellBG", this, this);
    CCCommonUtils::setButtonTitle(this->m_unBlockBtn, _lang("105315").c_str());
    m_headImgNode = HFHeadImgNode::create();
    setCellData(m_info);
    return true;
}

bool ShieldUnlockCellView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unBlockBtn", CCControlButton*, this->m_unBlockBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picContainer", CCNode*, this->m_picContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_power", CCLabelIF*, this->m_power);
    return true;
}

SEL_CCControlHandler ShieldUnlockCellView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUnBlockClick", ShieldUnlockCellView::onUnBlockClick);
    return NULL;
}

void ShieldUnlockCellView::onUnBlockClick(CCObject *pSender, CCControlEvent event){
    ChatUnLockCommand *cmd = new ChatUnLockCommand(m_info->uuid);
    cmd->sendAndRelease();
//    GlobalData::shared()->chatShieldInfo.removeFromList(m_info->uid);

//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REFRESH_SHIELD_PANEL);
}

void ShieldUnlockCellView::setCellData(ShieldInfo *info){
    m_info = info;
//    auto dictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_info->rank));
//    if(dictInfo && dictInfo->objectForKey("icon"))
//    {
        m_picContainer->removeAllChildren();
    string picName = m_info->pic;//dictInfo->valueForKey("icon")->getCString();
    if (picName == "") {
        picName = "g044";
    }
        picName += ".png";
        auto pic = CCLoadSprite::createSprite(picName.c_str());
        if (pic) {
            pic->setScale(0.6);
            m_picContainer->addChild(pic);
        }
    if (CCCommonUtils::isUseCustomPic(info->picVer))
    {
        m_headImgNode->initHeadImgUrl2(m_picContainer, CCCommonUtils::getCustomPicUrl(info->uid, info->picVer), 1.0f, 81, true);
    }
//    }
    string _name = "";
    if (m_info->abbr != "") {
        _name = "(" + m_info->abbr + ") ";
    }
    _name += m_info->name;
    this->m_nameText->setString(_name.c_str());
    m_power->setString(CC_ITOA(m_info->power));
}