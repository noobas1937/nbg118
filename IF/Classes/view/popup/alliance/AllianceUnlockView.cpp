//
//  AllianceUnlockView.cpp
//  IF
//
//  Created by ganxiaohua on 15-1-15.
//
//

#include "AllianceUnlockView.h"
#include "PopupViewController.h"
#include "SettingPopUpView.h"
#include "AllianceCommentShieldRemove.h"
#include "AllianceCommentShieldList.h"
#include "AllianceFlagPar.h"
#include "GameController.h"

AllianceUnlockView *AllianceUnlockView::create(){
    AllianceUnlockView *ret = new AllianceUnlockView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceUnlockView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceUnlockView::refreshView), REFRESH_DATA_LIST, NULL);
}

void AllianceUnlockView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, REFRESH_DATA_LIST);
    PopupBaseView::onExit();
}

bool AllianceUnlockView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto bg = CCBLoadFile("AllianceUnlockView", this, this);
        this->setContentSize(bg->getContentSize());
        setTitleName(_lang("105315").c_str());
        int prev = m_BG->getContentSize().height;
        changeBGHeight(m_BG);
        int add = m_BG->getContentSize().height - prev;
        if (CCCommonUtils::isIosAndroidPad())
        {
            add = CCDirector::sharedDirector()->getWinSize().height - 2048;
            m_container->setContentSize(CCSizeMake(m_container->getContentSize().width, m_container->getContentSize().height + add / 2.4));
            m_container->setPositionY(m_container->getPositionY()-add / 2.4);
        } else {
            m_container->setContentSize(CCSizeMake(m_container->getContentSize().width, m_container->getContentSize().height + add));
            m_container->setPositionY(m_container->getPositionY()-add);
        }
        
        m_srcData = CCArray::create();
        m_data = CCArray::create();
        
        m_tabView = CCMultiColTableView::create(this, CCSize(m_container->getContentSize().width, m_container->getContentSize().height));
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(1);
        this->m_container->addChild(m_tabView);
        
        CCCommonUtils::setButtonTitle(m_tab1, _lang("115922").c_str());
        CCCommonUtils::setButtonTitle(m_tab2, _lang("115923").c_str());
        m_tab1->setEnabled(false);
        m_tab2->setEnabled(true);
        getData();
        ret = true;
    }
    return ret;
}

bool AllianceUnlockView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_container", CCNode*, this->m_container);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BG", CCScale9Sprite*, m_BG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab1", CCControlButton*, this->m_tab1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab2", CCControlButton*, this->m_tab2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tip", CCLabelIF*, this->m_tip);
    return true;
}

void AllianceUnlockView::getData(){
    AllianceCommentShieldList *cmd = new AllianceCommentShieldList();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceUnlockView::getServerData), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void AllianceUnlockView::getServerData(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* arr = dynamic_cast<CCArray*>(dic->objectForKey("list"));
        if (arr) {
            int num = arr->count();
            for (int i=0; i<num; i++) {
                auto dicInfo = _dict(arr->objectAtIndex(i));
                if (dicInfo) {
                    AllianceShieldInfo* info = new AllianceShieldInfo();
                    info->uuid = dicInfo->valueForKey("uuid")->getCString();
                    info->allianceId = dicInfo->valueForKey("allianceId")->getCString();
                    info->type = dicInfo->valueForKey("type")->intValue();
                    info->icon = dicInfo->valueForKey("icon")->getCString();
                    info->abbr = dicInfo->valueForKey("abbr")->getCString();
                    info->name = dicInfo->valueForKey("name")->getCString();
                    info->uid = dicInfo->valueForKey("uid")->getCString();
                    info->picVer = dicInfo->valueForKey("picVer")->intValue();
                    m_srcData->addObject(info);
                    CC_SAFE_RELEASE_NULL(info);
                }
            }
        }
    }
    getShowData();
}

void AllianceUnlockView::getShowData(){
    m_data->removeAllObjects();
    int type = 1;
    if (!m_tab2->isEnabled()) {
        type = 2;
    }
    int num = m_srcData->count();
    for (int i=0; i<num; i++) {
        AllianceShieldInfo* info = dynamic_cast<AllianceShieldInfo*>(m_srcData->objectAtIndex(i));
        if (info->type==type) {
            m_data->addObject(info);
        }
    }
    m_tabView->reloadData();
    if(m_data->count()<=0){
        m_tip->setString(type==1?_lang("115267"):_lang("115268"));
    }else{
        m_tip->setString("");
    }
}

SEL_CCControlHandler AllianceUnlockView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", AllianceUnlockView::onCloseBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTab1Click", AllianceUnlockView::onTab1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTab2Click", AllianceUnlockView::onTab2Click);
    return NULL;
}

void AllianceUnlockView::onCloseBtnClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->goBackPopupView();
}

void AllianceUnlockView::onTab1Click(CCObject *pSender, CCControlEvent event){
    m_tab1->setEnabled(false);
    m_tab2->setEnabled(true);
    getShowData();
}

void AllianceUnlockView::onTab2Click(CCObject *pSender, CCControlEvent event){
    m_tab1->setEnabled(true);
    m_tab2->setEnabled(false);
    getShowData();
}
cocos2d::CCSize AllianceUnlockView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSizeMake(365, 118);
}

CCTableViewCell* AllianceUnlockView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    AllianceShieldInfo *info = dynamic_cast<AllianceShieldInfo*>(m_data->objectAtIndex(idx));
    AllianceUnlockCell *cell = (AllianceUnlockCell*)table->dequeueGrid();
    if(idx < m_data->count()){
        if(cell){
            cell->setCellData(info);
        }else{
            cell = AllianceUnlockCell::create(info);
        }
        return cell;
    }
    return NULL;
}

ssize_t AllianceUnlockView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = m_data->count();
    return num;
}

unsigned int AllianceUnlockView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}

void AllianceUnlockView::refreshView(CCObject* p){
    if (p!=NULL) {
        CCString* str = dynamic_cast<CCString*>(p);
        if (str!=NULL) {
            string uuid = str->getCString();
            int num = m_srcData->count();
            for (int i=0; i<num; i++) {
                AllianceShieldInfo* info = dynamic_cast<AllianceShieldInfo*>(m_srcData->objectAtIndex(i));
                if (info->uuid==uuid) {
                    m_srcData->removeObjectAtIndex(i);
                    break;
                }
            }
        }
    }
    getShowData();
}

void AllianceUnlockView::loadDataBack(){
    refreshView(NULL);
}

//----------------

AllianceUnlockCell* AllianceUnlockCell::create(AllianceShieldInfo *info){
    AllianceUnlockCell *ret = new AllianceUnlockCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceUnlockCell::onEnter(){
    CCNode::onEnter();
}

void AllianceUnlockCell::onExit(){
    CCNode::onExit();
}

bool AllianceUnlockCell::init(){
    CCBLoadFile("AllianceUnlockCell", this, this);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_nameText->setScale(0.5);
        m_nameText->setFontSize(48);
        m_unBlockBtn->setScale(0.5);
        m_unBlockBtn->setPreferredSize(CCSize(280, 128));
        m_unBlockBtn->setTitleTTFSizeForState(40, CCControlStateNormal);
        m_unBlockBtn->setTitleTTFSizeForState(40, CCControlStateHighlighted);
        m_unBlockBtn->setTitleTTFSizeForState(40, CCControlStateDisabled);
    }
    CCCommonUtils::setButtonTitle(this->m_unBlockBtn, _lang("105315").c_str());
    m_headImgNode = HFHeadImgNode::create();
    setCellData(m_info);
    return true;
}

bool AllianceUnlockCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unBlockBtn", CCControlButton*, this->m_unBlockBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picContainer", CCNode*, this->m_picContainer);
    return true;
}

SEL_CCControlHandler AllianceUnlockCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUnBlockClick", AllianceUnlockCell::onUnBlockClick);
    return NULL;
}

void AllianceUnlockCell::onUnBlockClick(CCObject *pSender, CCControlEvent event){
    AllianceCommentShieldRemove *cmd = new AllianceCommentShieldRemove(m_info->uuid);
    cmd->sendAndRelease();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REFRESH_DATA_LIST,CCString::create(m_info->uuid));
}

void AllianceUnlockCell::setCellData(AllianceShieldInfo *info){
    m_info = info;
    m_picContainer->removeAllChildren();
    string picName = m_info->icon;
    if(m_info->type==2){
        if(picName==""){
            picName = "Allance_flay.png";
        }else{
            picName.append(".png");
        }
        AllianceFlagPar* flag = AllianceFlagPar::create(picName.c_str());
        flag->setScale(0.4);
        m_picContainer->addChild(flag);
    }else{
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
            m_headImgNode->initHeadImgUrl2(m_picContainer, CCCommonUtils::getCustomPicUrl(info->uid, info->picVer), 1.0f, 75, true);
        }
    }

    //    }
    string _name = "";
    if (m_info->abbr != "") {
        _name = "(" + m_info->abbr + ") ";
    }
    _name += m_info->name;
    this->m_nameText->setString(_name.c_str());
}