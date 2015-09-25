//
//  CountryChooseFlagView.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-24.
//
//

#include "CountryChooseFlagView.h"
#include "PopupViewController.h"
#include "KingdomExecCommand.h"
#include "YesNoDialog.h"
#include "GameController.h"
#include "KingdomBannerCommand.h"
#include "ManagerKingdomView.h"

static const char* icons[85] = {"UN","US","KR","GB","RU","ID","TH","AE","VN","TR","FR","IN","DE","TW",
                                "IT","CN","CA","ES","PL","PH","MY","BR","AU","RO","NL","EG","IR","BG",
                                "SE","HU","BE","AT","RS","CH","HK","HR","SK","UA","MX","FI","PT","LT",
                                "ZA","NZ","GR","DK","CZ","IL","IE","SG","NO","JP","SI","MM","LV","CL",
                                "EE","MK","AZ","AR","SA","YE","KH","CP","BY","PE","BD","KZ","TN",
                                "AL","AM","LI","PA","SY","DZ","KW","VE","BH","LA","LU","EC","UZ","PK",
                                "AO","PS"
                                };

CountryChooseFlagView *CountryChooseFlagView::create(){
    CountryChooseFlagView *ret = new CountryChooseFlagView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void CountryChooseFlagView::onEnter(){
    PopupBaseView::onEnter();
    this->setTitleName(_lang("110091"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CountryChooseFlagView::onPicClick), SELECT_BANNER_CELL, NULL);
    this->refreshView();
}

void CountryChooseFlagView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SELECT_BANNER_CELL);
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool CountryChooseFlagView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void CountryChooseFlagView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
//    if(!isTouchInside(m_viewBg, pTouch)){
//        PopupViewController::getInstance()->removePopupView(this);
//    }
}

bool CountryChooseFlagView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto ccbNode = CCBLoadFile("CountryChooseFlagView", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else {
            this->setContentSize(ccbNode->getContentSize());
            int prev = m_viewBg->getContentSize().height;
            this->changeBGHeight(m_viewBg);
            int add = m_viewBg->getContentSize().height - prev;
            m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width,m_listContainer->getContentSize().height+add));
            m_listContainer->setPositionY(m_listContainer->getPositionY()-add);
            m_changeBtn->setPositionY(m_changeBtn->getPositionY()-add);
        }
        
        CCCommonUtils::setButtonTitle(m_changeBtn, _lang("confirm").c_str());
        m_data = CCArray::create();
        m_cells = CCArray::create();
        for (int i=0; i<85; i++) {
            std::string language = LocalController::shared()->getLanguageFileName();
            string lType = icons[i];
            //简体中文的，看到台湾的、香港的，都成了大陆的国旗
            if((lType=="TW" || lType=="HK") && CCCommonUtils::checkTaiWanFlag()){
                continue;
            }
            m_data->addObject(CCString::create(icons[i]));
        }
        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(Touch_Popup);
        m_listContainer->addChild(m_tabView);
        
        m_currentFlag = "";
        m_changeBtn->setEnabled(false);
        m_tabView->reloadData();
        
        KingdomBannerCommand* cmd = new KingdomBannerCommand();
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(CountryChooseFlagView::getFlagSuccess), NULL));
        cmd->sendAndRelease();
        
        ret = true;
    }
    return ret;
}

void CountryChooseFlagView::getFlagSuccess(CCObject *data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        m_userFlag = dic->valueForKey("banner")->getCString();
        if (m_userFlag!="") {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(SELECT_BANNER_CELL,CCString::create(m_userFlag));
        }
    }
}

void CountryChooseFlagView::onPicClick(CCObject *data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        m_currentFlag = str->getCString();
    }
    if (m_userFlag!=m_currentFlag && m_currentFlag!="") {
        m_changeBtn->setEnabled(true);
    }else{
        m_changeBtn->setEnabled(false);
    }
}

void CountryChooseFlagView::onChangeClick(CCObject *pSender, CCControlEvent event){
    YesNoDialog::show(_lang("110092").c_str(), CCCallFunc::create(this, callfunc_selector(CountryChooseFlagView::confirmFun)));
}

void CountryChooseFlagView::confirmFun(){
    KingdomExecCommand* cmd = new KingdomExecCommand();
    cmd->putParam("type", CCInteger::create(CHANGE_COUNTRY_FLAG));
    cmd->putParam("banner", CCString::create(m_currentFlag));
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(CountryChooseFlagView::changeSuccess), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void CountryChooseFlagView::changeSuccess(CCObject *data){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(KINGDOM_BANNER_CHANGE,CCString::create(m_currentFlag));
    PopupViewController::getInstance()->goBackPopupView();
}

void CountryChooseFlagView::refreshView(){
    
}

bool CountryChooseFlagView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_changeBtn", CCControlButton*, this->m_changeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    return false;
}


SEL_CCControlHandler CountryChooseFlagView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChangeClick", CountryChooseFlagView::onChangeClick);
    return NULL;
}

void CountryChooseFlagView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize CountryChooseFlagView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(360, 300);
    }
    return CCSize(150, 125);
}

CCTableViewCell* CountryChooseFlagView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    CountryFlagCell* cell = (CountryFlagCell*)table->dequeueGrid();
    CCString* info = (CCString*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info->getCString());
        }else{
            cell = CountryFlagCell::create(info->getCString(),m_listContainer);
            m_cells->addObject(cell);
        }
    }
    return cell;
}

ssize_t CountryChooseFlagView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num  = m_data->count();
    return (num/4 + (num%4==0?0:1));
}

unsigned int CountryChooseFlagView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 4;
}

CountryFlagCell *CountryFlagCell::create(std::string pic,CCNode* showArea){
    CountryFlagCell *ret = new CountryFlagCell(pic,showArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void CountryFlagCell::setData(std::string pic){
    m_pic = pic;
    std::string head = m_pic;
    head.append(".png");
    m_head->removeAllChildrenWithCleanup(true);
    m_headNode = CCLoadSprite::createSprite(head.c_str());
    CCCommonUtils::setSpriteMaxSize(m_headNode, (int)(0.86*157));
    m_head->addChild(m_headNode);
    m_selectNode->setVisible(head==GlobalData::shared()->playerInfo.getPic());
}

void CountryFlagCell::onEnter(){
    CCNode::onEnter();

    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CountryFlagCell::clickHead), SELECT_BANNER_CELL, NULL);
}

void CountryFlagCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SELECT_BANNER_CELL);
    CCNode::onExit();
}

bool CountryFlagCell::init(){
    auto bg = CCBLoadFile("CountryFlagCell", this, this);
    this->setContentSize(bg->getContentSize());
    
    setData(m_pic);
    return true;
}

bool CountryFlagCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_isDraging = false;
    if(isTouchInside(m_clickNode, pTouch) && isTouchInside(m_showArea, pTouch)){
        return true;
    }
    return false;
}

void CountryFlagCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_headNode, pTouch)){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(SELECT_BANNER_CELL,CCString::create(m_pic));
        m_selectNode->setVisible(true);
    }
}

void CountryFlagCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void CountryFlagCell::changePic(){
    
}

void CountryFlagCell::clickHead(CCObject* data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        std::string clickPic = str->getCString();
        m_selectNode->setVisible(clickPic==m_pic);
    }
}

void CountryFlagCell::changeSuccess(){
    GlobalData::shared()->playerInfo.pic = m_pic;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_PLAYER_PIC);
}

bool CountryFlagCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_head", CCNode*, this->m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_selectNode", CCNode*, this->m_selectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    return false;
}

SEL_CCControlHandler CountryFlagCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}