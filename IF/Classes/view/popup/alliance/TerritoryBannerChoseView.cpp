//
//  TerritoryBannerChoseView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/8/20.
//
//

#include "TerritoryBannerChoseView.h"
#include "PopupViewController.h"
#include "KingdomExecCommand.h"
#include "YesNoDialog.h"
#include "GameController.h"
#include "KingdomBannerCommand.h"
#include "ManagerKingdomView.h"
#include "WorldController.h"
#include "WorldMapView.h"
#include "SceneController.h"
static const char* icons[85] = {"UN","US","KR","GB","RU","ID","TH","AE","VN","TR","FR","IN","DE","TW",
    "IT","CN","CA","ES","PL","PH","MY","BR","AU","RO","NL","EG","IR","BG",
    "SE","HU","BE","AT","RS","CH","HK","HR","SK","UA","MX","FI","PT","LT",
    "ZA","NZ","GR","DK","CZ","IL","IE","SG","NO","JP","SI","MM","LV","CL",
    "EE","MK","AZ","AR","SA","YE","KH","CP","BY","PE","BD","KZ","TN",
    "AL","AM","LI","PA","SY","DZ","KW","VE","BH","LA","LU","EC","UZ","PK",
    "AO","PS"
};////////////////

TerritoryBannerChoseView *TerritoryBannerChoseView::create(unsigned int index){
    TerritoryBannerChoseView *ret = new TerritoryBannerChoseView();
    if(ret && ret->init(index)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void TerritoryBannerChoseView::onEnter(){
    PopupBaseView::onEnter();
    this->setTitleName(_lang("115536"));
    setTouchEnabled(true);
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TerritoryBannerChoseView::onPicClick), SELECT_TERRITORY_BANNER_CELL, NULL);
    this->refreshView();
}

void TerritoryBannerChoseView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SELECT_TERRITORY_BANNER_CELL);
    setTouchEnabled(false);
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    PopupBaseView::onExit();
}

bool TerritoryBannerChoseView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void TerritoryBannerChoseView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
}

bool TerritoryBannerChoseView::init(unsigned int index){
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
        
        m_index = index;
        CCCommonUtils::setButtonTitle(m_changeBtn, _lang("confirm").c_str());
        m_data = CCArray::create();
        for (int i=0; i<85; i++) {
            std::string language = LocalController::shared()->getLanguageFileName();
            string lType = icons[i];
            //简体中文的，看到台湾的，都成了大陆的国旗
            if((lType=="TW" || lType=="HK") && CCCommonUtils::checkTaiWanFlag()){
                continue;
            }
            lType = CCCommonUtils::changeChinaFlag(lType);
            m_data->addObject(CCString::create(lType));
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
        
        ret = true;
    }
    return ret;
}

void TerritoryBannerChoseView::onPicClick(CCObject *data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        m_currentFlag = str->getCString();
    }
    if (m_currentFlag!="") {
        m_changeBtn->setEnabled(true);
    }else{
        m_changeBtn->setEnabled(false);
    }
}

void TerritoryBannerChoseView::onChangeClick(CCObject *pSender, CCControlEvent event){
//    YesNoDialog::show(_lang("110092").c_str(), CCCallFunc::create(this, callfunc_selector(TerritoryBannerChoseView::confirmFun)));
    confirmFun();   
}

void TerritoryBannerChoseView::confirmFun(){
    auto tPoint = WorldController::getPointByIndex(m_index);
    std::string isFirstPopKey = GlobalData::shared()->playerInfo.uid + "isFirstPop";
    int isFirstPop = CCUserDefault::sharedUserDefault()->getIntegerForKey(isFirstPopKey.c_str(),0);
    WorldController::getInstance()->buildAllianceTerritory = 1;
    WorldController::getInstance()->allianceTerritoryIndex = m_index;
    WorldController::getInstance()->openTargetIndex = m_index;
    WorldController::getInstance()->allianceTerritoryType = tile_banner;
    WorldController::getInstance()->allianceTerritoryBanner = m_currentFlag;
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(tPoint,true);
    }else{
        if (isFirstPop==0) {
            WorldController::getInstance()->firstInWorld = true;
            isFirstPop = 1;
            CCUserDefault::sharedUserDefault()->setIntegerForKey(isFirstPopKey.c_str(), isFirstPop);
            CCUserDefault::sharedUserDefault()->flush();
        }
        
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, m_index);
    }
    PopupViewController::getInstance()->removeAllPopupView();
}

void TerritoryBannerChoseView::refreshView(){
    
}

bool TerritoryBannerChoseView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_changeBtn", CCControlButton*, this->m_changeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    return false;
}


SEL_CCControlHandler TerritoryBannerChoseView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChangeClick", TerritoryBannerChoseView::onChangeClick);
    return NULL;
}

void TerritoryBannerChoseView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize TerritoryBannerChoseView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(360, 300);
    }
    return CCSize(150, 125);
}

CCTableViewCell* TerritoryBannerChoseView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    TerritoryBannerChoseCell* cell = (TerritoryBannerChoseCell*)table->dequeueGrid();
    CCString* info = (CCString*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info->getCString());
        }else{
            cell = TerritoryBannerChoseCell::create(info->getCString(),m_listContainer);
        }
    }
    return cell;
}

ssize_t TerritoryBannerChoseView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num  = m_data->count();
    return (num/4 + (num%4==0?0:1));
}

unsigned int TerritoryBannerChoseView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 4;
}

TerritoryBannerChoseCell *TerritoryBannerChoseCell::create(std::string pic,CCNode* showArea){
    TerritoryBannerChoseCell *ret = new TerritoryBannerChoseCell(pic,showArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void TerritoryBannerChoseCell::setData(std::string pic){
    m_pic = pic;
    std::string head = m_pic;
    head.append(".png");
    m_head->removeAllChildrenWithCleanup(true);
    m_headNode = CCLoadSprite::createSprite(head.c_str());
    CCCommonUtils::setSpriteMaxSize(m_headNode, (int)(0.86*157));
    m_head->addChild(m_headNode);
    m_selectNode->setVisible(false);
}

void TerritoryBannerChoseCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TerritoryBannerChoseCell::clickHead), SELECT_TERRITORY_BANNER_CELL, NULL);
}

void TerritoryBannerChoseCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SELECT_TERRITORY_BANNER_CELL);
    CCNode::onExit();
}

bool TerritoryBannerChoseCell::init(){
    CCLoadSprite::doResourceByCommonIndex(208, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(208, false);
    });
    auto bg = CCBLoadFile("CountryFlagCell", this, this);
    this->setContentSize(bg->getContentSize());
    
    setData(m_pic);
    return true;
}

bool TerritoryBannerChoseCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_isDraging = false;
    if(isTouchInside(m_clickNode, pTouch) && isTouchInside(m_showArea, pTouch)){
        return true;
    }
    return false;
}

void TerritoryBannerChoseCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_headNode, pTouch)){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(SELECT_TERRITORY_BANNER_CELL,CCString::create(m_pic));
        m_selectNode->setVisible(true);
    }
}

void TerritoryBannerChoseCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void TerritoryBannerChoseCell::changePic(){
    
}

void TerritoryBannerChoseCell::clickHead(CCObject* data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        std::string clickPic = str->getCString();
        m_selectNode->setVisible(clickPic==m_pic);
    }
}

bool TerritoryBannerChoseCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_head", CCNode*, this->m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_selectNode", CCNode*, this->m_selectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    return false;
}

SEL_CCControlHandler TerritoryBannerChoseCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}