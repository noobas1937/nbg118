//
//  TerritoryWarehouseView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/7/8.
//
//

#include "TerritoryWarehouseView.h"
#include "PopupViewController.h"
#include "WorldController.h"
#include "WorldCommand.h"
#include "SceneController.h"
#include "CCShake.h"
#include "FunBuildController.h"
#include "AllianceTerritoryShowCommand.h"

#define CURRENT_LOAD_CHANGE "current_load_change"
TerritoryWarehouseView* TerritoryWarehouseView::create(int pointId,bool storage){
    TerritoryWarehouseView* ret = new TerritoryWarehouseView(pointId,storage);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool TerritoryWarehouseView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    auto tmpCCB = CCBLoadFile("AllianceResourceTrade",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else {
        this->setContentSize(tmpCCB->getContentSize());
        int oldBgHeight = m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        int newBgHeight = m_viewBg->getContentSize().height;
        int addHeight = newBgHeight - oldBgHeight;
        int oldWidth = m_scrollNode->getContentSize().width;
        int oldHeight = m_scrollNode->getContentSize().height;
        m_scrollNode->setContentSize(CCSize(oldWidth,oldHeight+addHeight));
        m_scrollNode->setPositionY(m_scrollNode->getPositionY() - addHeight);
        m_buttomNode->setPositionY(m_buttomNode->getPositionY() - addHeight);
    }
    
    if (m_storage) {
        m_cityNameTxt->setString(_lang("115458"));
        m_tradePlayerTxt->setString(_lang("115453"));
        m_infoTxt->setString(_lang("115454"));
        m_rateTxt1->setVisible(true);
        m_rateTxt2->setVisible(true);
        m_loadTxt1->setVisible(true);
        m_loadTxt2->setVisible(true);

        if (CCCommonUtils::isIosAndroidPad()) {
            m_loadTxt1->setFontSize(40);
            m_rateTxt1->setFontSize(40);
            m_loadTxt2->setFontSize(40);
            m_rateTxt2->setFontSize(40);
        }
        else {
            m_loadTxt1->setFontSize(20);
            m_rateTxt1->setFontSize(20);
            m_loadTxt2->setFontSize(20);
            m_rateTxt2->setFontSize(20);
        }
    }
    else {
        m_cityNameTxt->setString(_lang("115364"));
        m_tradePlayerTxt->setString(_lang("115458"));
        m_infoTxt->setString(_lang("115457"));
        m_rateTxt1->setVisible(false);
        m_loadTxt1->setVisible(false);
        m_rateTxt2->setVisible(false);
        m_loadTxt2->setVisible(false);
    }
    
    m_scrollView = CCScrollView::create(m_scrollNode->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollNode->addChild(m_scrollView);
    
    if (m_storage == true) {
      getPlayerStorageLimit();
    }
    
    m_resList = CCArray::create();
    int num = 2;
    int offY = 88*2;
    if (CCCommonUtils::isIosAndroidPad()) {
        offY = 180*2;
    }
    if(FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k4){
        num = 4;
        offY = 0;
    }else if (FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k3){
        num = 3;
        offY = 88;
        if (CCCommonUtils::isIosAndroidPad()) {
            offY = 180;
        }
    }
    
    if (CCCommonUtils::isIosAndroidPad()) {
        int nodeW = m_scrollNode->getContentSize().width/2;
        int nodeH = 1800/2+70;
        m_funNode1->removeFromParent();
        m_funNode1->setPosition(m_funNode1->getPositionX() + nodeW, m_funNode1->getPositionY() + nodeH);
        m_funNode2->removeFromParent();
        m_funNode2->setPosition(m_funNode2->getPositionX() + nodeW, m_funNode2->getPositionY() + nodeH+offY);
        
        m_scrollView->addChild(m_funNode1);
        m_scrollView->addChild(m_funNode2);
        
        int contentHeight = 1800;
        m_scrollView->setContentSize(CCSize(1536,contentHeight));
        m_scrollView->setContentOffset(ccp(0, m_scrollNode->getContentSize().height - contentHeight));
    }
    else {
        int nodeW = m_scrollNode->getContentSize().width/2;
        int nodeH = 840/2+70;
        m_funNode1->removeFromParent();
        m_funNode1->setPosition(m_funNode1->getPositionX() + nodeW, m_funNode1->getPositionY() + nodeH);
        m_funNode2->removeFromParent();
        m_funNode2->setPosition(m_funNode2->getPositionX() + nodeW, m_funNode2->getPositionY() + nodeH+offY);
        CCSize size = m_infoListBG->getContentSize();
        m_infoListBG->setContentSize(CCSize(size.width,size.height-offY));//fusheng 调整infoBG 大小
        
        m_scrollView->addChild(m_funNode1);
        m_scrollView->addChild(m_funNode2);
        
        int contentHeight = 840;
        m_scrollView->setContentSize(CCSize(640,contentHeight));
        m_scrollView->setContentOffset(ccp(0, m_scrollNode->getContentSize().height - contentHeight));
    }
    
    m_ironNode->setVisible(FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k3);
    m_ironTradeNode->setVisible(m_ironNode->isVisible());
    m_coverNode1->setVisible(FunBuildController::getInstance()->getMainCityLv()<FunBuildController::getInstance()->building_base_k3);
    
    m_stoneNode->setVisible(FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k4);
    m_stoneTradeNode->setVisible(m_stoneNode->isVisible());
    m_coverNode2->setVisible(FunBuildController::getInstance()->getMainCityLv()<FunBuildController::getInstance()->building_base_k4);
    
    int startPt = WorldController::getIndexByPoint(WorldController::getInstance()->selfPoint);
    int walkTime = WorldController::getInstance()->getMarchTime(startPt, m_warehousePtId, tile_wareHouse, MethodWarehouse, true);
    walkTime = walkTime / 1000 * 2;
    m_timeTxt->setString(CC_SECTOA(walkTime));
    
    if (m_storage) {
        m_cityIcon->setVisible(true);
        m_cityIcon1->setVisible(false);
        m_warehouseIcon->setVisible(false);
        m_warehouseIcon1->setVisible(false);
        m_warehouseIcon2->setVisible(true);
    }
    else {
        m_cityIcon->setVisible(false);
        m_cityIcon1->setVisible(true);
        m_warehouseIcon->setVisible(false);
        m_warehouseIcon1->setVisible(true);
        m_warehouseIcon->setVisible(false);
    }
    currentStorage = 0;
    lastStorage = 0;
    m_tradeToServerFood = 0;
    m_lastTrade = 0;
    this->schedule(schedule_selector(TerritoryWarehouseView::updateChangeStatus), 0.1);
    this->updateInfo();
    
    m_btnTrade->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
    return true;
}


void TerritoryWarehouseView::updateInfo()
{
    this->updateTradeData();
    m_btnTrade->setEnabled(false);
}

void TerritoryWarehouseView::tradeSuccess(CCObject* p){
    int temp = m_lastTotalFood - GlobalData::shared()->resourceInfo.lFood;
    if(m_tradeToServerFood!=0 && m_tradeToServerFood!= temp && temp>0){
        
    }
    int num = m_resList->count();
    for(int i=0;i<num;i++){
        TerritoryWarehouseCell* cell = (TerritoryWarehouseCell*)m_resList->objectAtIndex(i);
        cell->resetData();
    }
    this->updateInfo();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE,NULL);
    
    PopupViewController::getInstance()->removeAllPopupView();
    SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
}

void TerritoryWarehouseView::updateTradeData(bool isEnd){
    std::string str = _lang("115451");
    m_rateTxt1->setString(str.c_str());
    str = "";
    str.append(CC_ITOA_K((currentStorage+GlobalData::shared()->territory_warehouse_dayStorage)*1.0));
    str.append("/");
    str.append(CC_ITOA_K(m_dayStorageLimit*1.0));
    m_rateTxt2->setPositionX(m_rateTxt1->getPositionX()+m_rateTxt1->getContentSize().width*m_rateTxt1->getOriginScaleX());//fusheng 设置文本位置
    m_rateTxt2->setString(str.c_str());
    str = _lang("115450");
    m_loadTxt1->setString(str.c_str());
    str = "";
    str.append(CC_ITOA_K((currentStorage+GlobalData::shared()->territory_warehouse_totalStorage)*1.0));
    str.append("/");
    str.append(CC_ITOA_K(m_totalStorageLimit*1.0));
    m_loadTxt2->setPositionX(m_loadTxt1->getPositionX()+m_loadTxt1->getContentSize().width*m_loadTxt1->getOriginScaleX());//fusheng 设置文本位置
    m_loadTxt2->setString(str.c_str());
    if (currentStorage+GlobalData::shared()->territory_warehouse_dayStorage >= m_dayStorageLimit && isEnd) {
//        m_rateTxt->stopAllActions();
//        lastStorage = currentStorage;
//        CCSequence* sc1 = CCSequence::create(CCScaleTo::create(0.1, 1.05),NULL);
//        CCSequence* sc2 = CCSequence::create(CCTintTo::create(0.25, 255, 0, 0),CCTintTo::create(0.01, 0, 244, 0),CCScaleTo::create(0.08, 1),NULL);
//        CCSpawn* sp = CCSpawn::create(sc1,sc2,NULL);
//        m_rateTxt->runAction(sp);
    }
    if (currentStorage+GlobalData::shared()->territory_warehouse_totalStorage >= m_totalStorageLimit && isEnd) {
//        m_loadTxt->stopAllActions();
//        lastStorage = currentStorage;
//        CCSequence* sc1 = CCSequence::create(CCScaleTo::create(0.1, 1.05),NULL);
//        CCSequence* sc2 = CCSequence::create(CCTintTo::create(0.25, 255, 0, 0),CCTintTo::create(0.01, 0, 244, 0),CCScaleTo::create(0.08, 1),NULL);
//        CCSpawn* sp = CCSpawn::create(sc1,sc2,NULL);
//        m_loadTxt->runAction(sp);
    }
    m_btnTrade->setEnabled(currentStorage>0);
    
}

void TerritoryWarehouseView::playResourceAni(){
    int num = m_resList->count();
    for(int i=0;i<num;i++){
        TerritoryWarehouseCell* cell = (TerritoryWarehouseCell*)m_resList->objectAtIndex(i);
        CCShake* action = CCShake::create(0.4, 7);
        CCSequence* sc1 = CCSequence::create(CCScaleTo::create(0.1, 1.05),NULL);
        CCSequence* sc2 = CCSequence::create(CCTintTo::create(0.25, 255, 0, 0),CCTintTo::create(0.01, 0, 244, 0),CCScaleTo::create(0.08, 1),NULL);
        CCSpawn* sp = CCSpawn::create(sc1,sc2,NULL);
        switch (cell->m_index) {
            case 0:
                if(cell->m_currentValue>=GlobalData::shared()->resourceInfo.lWood){
                    m_woodIcon->stopAllActions();
                    m_woodIcon->runAction(action);
                    m_wood->stopAllActions();
                    m_wood->runAction(sp);
                }
                break;
            case 1:
                if(cell->m_currentValue>=GlobalData::shared()->resourceInfo.lStone){
                    m_stoneIcon->stopAllActions();
                    m_stoneIcon->runAction(action);
                    m_stone->stopAllActions();
                    m_stone->runAction(sp);
                }
                break;
            case 2:
                if(cell->m_currentValue>=GlobalData::shared()->resourceInfo.lIron){
                    m_ironIcon->stopAllActions();
                    m_ironIcon->runAction(action);
                    m_iron->stopAllActions();
                    m_iron->runAction(sp);
                }
                break;
            case 3:
                if(cell->m_currentValue>=GlobalData::shared()->resourceInfo.lFood){
                    m_foodIcon->stopAllActions();
                    m_foodIcon->runAction(action);
                    m_food->stopAllActions();
                    m_food->runAction(sp);
                }
                break;
            default:
                
                break;
        }
    }
}

bool TerritoryWarehouseView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void TerritoryWarehouseView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    updateTradeData(true);
}

void TerritoryWarehouseView::updateChangeStatus(float t){
    if (m_storage) {
        this->m_food->setString(CC_CMDITOA(GlobalData::shared()->resourceInfo.lFood));
        m_scrollView->setTouchEnabled(true);
    }
}

void TerritoryWarehouseView::onDetailCallback(cocos2d::CCObject *obj){
    removeLoadingAni();
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (!info) {
        return;
    }
    
    unsigned long woodTotal = 0;
    unsigned long foodTotal = 0;
    unsigned long ironTotal = 0;
    unsigned long stoneTotal = 0;
    GlobalData::shared()->resourceInfo.warehouseWood = 0;
    GlobalData::shared()->resourceInfo.warehouseFood = 0;
    GlobalData::shared()->resourceInfo.warehouseIron = 0;
    GlobalData::shared()->resourceInfo.warehouseStone = 0;
    if (info->objectForKey("resourceList")) {
        CCArray* array = (CCArray*)info->objectForKey("resourceList");
        if(array){
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = _dict(array->objectAtIndex(i));
                if (item->objectForKey("uid")) {
                    std::string playerUid = item->valueForKey("uid")->getCString();
                    if (playerUid == GlobalData::shared()->playerInfo.uid) {
                        m_totalStorage = 0;
                        if (item->objectForKey("wood")) {
                            woodTotal = item->valueForKey("wood")->longValue();
                            GlobalData::shared()->resourceInfo.warehouseWood = woodTotal;
                            m_totalStorage += woodTotal * GlobalData::shared()->resources_weight_1;
                        }
                        if (item->objectForKey("food")) {
                            foodTotal = item->valueForKey("food")->longValue();
                            GlobalData::shared()->resourceInfo.warehouseFood = foodTotal;
                            m_totalStorage += foodTotal * GlobalData::shared()->resources_weight_2;
                        }
                        if (item->objectForKey("stone")) {
                            stoneTotal = item->valueForKey("stone")->longValue();
                            GlobalData::shared()->resourceInfo.warehouseStone = stoneTotal;
                            m_totalStorage += stoneTotal * GlobalData::shared()->resources_weight_3;
                        }
                        if (item->objectForKey("iron")) {
                            ironTotal = item->valueForKey("iron")->longValue();
                            GlobalData::shared()->resourceInfo.warehouseIron = ironTotal;
                            m_totalStorage += ironTotal * GlobalData::shared()->resources_weight_4;
                        }
                        break;
                    }
                }
            }
        }
    }

    if (m_storage) {
        this->m_wood->setString(CC_CMDITOA(GlobalData::shared()->resourceInfo.lWood));
        this->m_stone->setString(CC_CMDITOA(GlobalData::shared()->resourceInfo.lStone));
        this->m_iron->setString(CC_CMDITOA(GlobalData::shared()->resourceInfo.lIron));
        this->m_food->setString(CC_CMDITOA(GlobalData::shared()->resourceInfo.lFood));
        this->m_tradeWood->setString(CC_ITOA(woodTotal*1.0));
        this->m_tradeFood->setString(CC_ITOA(foodTotal*1.0));
        this->m_tradeIron->setString(CC_ITOA(ironTotal*1.0));
        this->m_tradeStone->setString(CC_ITOA(stoneTotal*1.0));
    }
    else {
        this->m_wood->setString(CC_ITOA(woodTotal*1.0));
        this->m_stone->setString(CC_ITOA(stoneTotal*1.0));
        this->m_iron->setString(CC_ITOA(ironTotal*1.0));
        this->m_food->setString(CC_ITOA(foodTotal*1.0));
        this->m_tradeWood->setString(CC_CMDITOA(GlobalData::shared()->resourceInfo.lWood));
        this->m_tradeFood->setString(CC_CMDITOA(GlobalData::shared()->resourceInfo.lFood));
        this->m_tradeIron->setString(CC_CMDITOA(GlobalData::shared()->resourceInfo.lIron));
        this->m_tradeStone->setString(CC_CMDITOA(GlobalData::shared()->resourceInfo.lStone));
    }
    
    if (m_storage == false) {
        m_dayStorageLimit = GlobalData::shared()->territory_warehouse_totalStorage;
        m_totalStorageLimit = GlobalData::shared()->territory_warehouse_totalStorage;
    }
    
    int pos[4] = {0,3,2,1};
    int listH = 88*3;
    int num = 2;
    int offY = 88*2;
    if (CCCommonUtils::isIosAndroidPad()) {
        listH = 180*3;
        offY = 180*2;
    }
    if(FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k4){
        num = 4;
        offY = 0;
    }else if (FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k3){
        num = 3;
        offY = 88;
        if (CCCommonUtils::isIosAndroidPad()) {
            offY = 180;
        }
    }
    for(int i=0;i<num;i++){
        if (m_storage) {
            TerritoryWarehouseCell* cell = TerritoryWarehouseCell::create(MAX(0, (long)(m_dayStorageLimit - GlobalData::shared()->territory_warehouse_dayStorage)),MAX(0, (long)(m_totalStorageLimit - GlobalData::shared()->territory_warehouse_totalStorage)),pos[i],m_storage);
            cell->setAnchorPoint(ccp(0, 0.5));
            if (CCCommonUtils::isIosAndroidPad()) {
                cell->setPosition(ccp(0,listH-i*180));
            }
            else
                cell->setPosition(ccp(0,listH-i*88));
            m_infoList->addChild(cell);
            m_resList->addObject(cell);
        }
        else {
            TerritoryWarehouseCell* cell = TerritoryWarehouseCell::create(m_dayStorageLimit,m_totalStorageLimit,pos[i],m_storage);
            cell->setAnchorPoint(ccp(0, 0.5));
            if (CCCommonUtils::isIosAndroidPad()) {
                cell->setPosition(ccp(0,listH-i*180));
            }
            else
                cell->setPosition(ccp(0,listH-i*88));
            m_infoList->addChild(cell);
            m_resList->addObject(cell);
        }
    }
    updateTradeData();
}

void TerritoryWarehouseView::updateCurrentLoad(cocos2d::CCObject *obj) {
    m_scrollView->setTouchEnabled(false);
    currentStorage = 0;
    int num = m_resList->count();
    for(int i=0;i<num;i++){
        TerritoryWarehouseCell* cell = (TerritoryWarehouseCell*)m_resList->objectAtIndex(i);
        unsigned long addValue = 0;
        switch (cell->m_index) {
            case 0:{
                addValue = cell->m_currentValue*GlobalData::shared()->resources_weight_1;
                if (m_storage) {
                    std::string temp = CC_ITOA(GlobalData::shared()->resourceInfo.warehouseWood);
                    if (addValue > 0) {
                        temp.append("+");
                        temp.append(CC_ITOA(cell->m_currentValue*1.0));
                    }
                    m_tradeWood->setString(temp);
                }
                else{
                    std::string temp = CC_ITOA(GlobalData::shared()->resourceInfo.lWood);
                    if (addValue > 0) {
                        temp.append("+");
                        temp.append(CC_ITOA(cell->m_currentValue*1.0));
                    }
                    m_tradeWood->setString(temp);
                }
            }
                break;
            case 1:{
                addValue = cell->m_currentValue*GlobalData::shared()->resources_weight_2;
                if (m_storage) {
                    std::string temp = CC_ITOA(GlobalData::shared()->resourceInfo.warehouseStone);
                    if (addValue > 0) {
                        temp.append("+");
                        temp.append(CC_ITOA(cell->m_currentValue*1.0));
                    }
                    m_tradeStone->setString(temp);
                }
                else{
                    std::string temp = CC_ITOA(GlobalData::shared()->resourceInfo.lStone);
                    if (addValue > 0) {
                        temp.append("+");
                        temp.append(CC_ITOA(cell->m_currentValue*1.0));
                    }
                    m_tradeStone->setString(temp);
                }
            }
                break;
            case 2:{
                addValue = cell->m_currentValue*GlobalData::shared()->resources_weight_3;
                if (m_storage) {
                    std::string temp = CC_ITOA(GlobalData::shared()->resourceInfo.warehouseIron);
                    if (addValue > 0) {
                        temp.append("+");
                        temp.append(CC_ITOA(cell->m_currentValue*1.0));
                    }
                    m_tradeIron->setString(temp);
                }
                else{
                    std::string temp = CC_ITOA(GlobalData::shared()->resourceInfo.lIron);
                    if (addValue > 0) {
                        temp.append("+");
                        temp.append(CC_ITOA(cell->m_currentValue*1.0));
                    }
                    m_tradeIron->setString(temp);
                }
            }
                break;
            case 3:{
                addValue = cell->m_currentValue*GlobalData::shared()->resources_weight_4;
                if (m_storage) {
                    std::string temp = CC_ITOA(GlobalData::shared()->resourceInfo.warehouseFood);
                    if (addValue > 0) {
                        temp.append("+");
                        temp.append(CC_ITOA(cell->m_currentValue*1.0));
                    }
                    m_tradeFood->setString(temp);
                }
                else{
                    std::string temp = CC_ITOA(GlobalData::shared()->resourceInfo.lFood);
                    if (addValue > 0) {
                        temp.append("+");
                        temp.append(CC_ITOA(cell->m_currentValue*1.0));
                    }
                    m_tradeFood->setString(temp);
                }
            }
                break;
            default:
                break;
        }
        currentStorage += addValue;
    }
    for(int i=0;i<num;i++){
        TerritoryWarehouseCell* cell = (TerritoryWarehouseCell*)m_resList->objectAtIndex(i);
        cell->setData(currentStorage);
    }
    updateTradeData();
}

void TerritoryWarehouseView::removeLoadingAni(CCObject* p)
{
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}

void TerritoryWarehouseView::addLoadingAni(){
    if (m_loadingIcon==NULL) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_infoList->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_infoList->addChild(m_loadingIcon,1000000);
    }
}

void TerritoryWarehouseView::onEnter()
{
    CCNode::onEnter();
    auto cmd = new AllianceTerritoryDetailCommand(m_warehousePtId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryWarehouseView::onDetailCallback), nullptr));
    cmd->sendAndRelease();
    addLoadingAni();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TerritoryWarehouseView::updateCurrentLoad), CURRENT_LOAD_CHANGE, NULL);
    setTouchEnabled(true);
   // CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    if (m_storage) {
        this->setTitleName(_lang("115452"));
    }
    else
        this->setTitleName(_lang("115456"));
}

void TerritoryWarehouseView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CURRENT_LOAD_CHANGE);
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler TerritoryWarehouseView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnTradeClick", TerritoryWarehouseView::onBtnTradeClick);
    
    return NULL;
}

bool TerritoryWarehouseView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoListBG", CCScale9Sprite*, this->m_infoListBG);

    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wood", CCLabelIF*, this->m_wood);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stone", CCLabelIF*, this->m_stone);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iron", CCLabelIF*, this->m_iron);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_food", CCLabelIF*, this->m_food);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rateTxt1", CCLabelIF*, this->m_rateTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadTxt1", CCLabelIF*, this->m_loadTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rateTxt2", CCLabelIF*, this->m_rateTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadTxt2", CCLabelIF*, this->m_loadTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resTxt", CCLabelIF*, this->m_resTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTxt", CCLabelIF*, this->m_infoTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTrade", CCControlButton*, this->m_btnTrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityNameTxt", CCLabelIF*, this->m_cityNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tradeWood", CCLabelIF*, this->m_tradeWood);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tradeFood", CCLabelIF*, this->m_tradeFood);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tradeIron", CCLabelIF*, this->m_tradeIron);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tradeStone", CCLabelIF*, this->m_tradeStone);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tradePlayerTxt", CCLabelIF*, this->m_tradePlayerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironNode", CCNode*, this->m_ironNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneNode", CCNode*, this->m_stoneNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coverNode1", CCNode*, this->m_coverNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coverNode2", CCNode*, this->m_coverNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funNode1", CCNode*, this->m_funNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funNode2", CCNode*, this->m_funNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttomNode", CCNode*, this->m_buttomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollNode", CCNode*, this->m_scrollNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironTradeNode", CCNode*, this->m_ironTradeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneTradeNode", CCNode*, this->m_stoneTradeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityIcon", CCNode*, this->m_cityIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityIcon1", CCNode*, this->m_cityIcon1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_warehouseIcon", CCNode*, this->m_warehouseIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_warehouseIcon1", CCNode*, this->m_warehouseIcon1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_warehouseIcon2", CCNode*, this->m_warehouseIcon2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodIcon", CCNode*, this->m_woodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodIcon", CCNode*, this->m_foodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironIcon", CCNode*, this->m_ironIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneIcon", CCNode*, this->m_stoneIcon);
    return false;
}

void TerritoryWarehouseView::onBtnTradeClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    unsigned int current =  WorldController::getInstance()->getCurrentMarchCount();
    unsigned int max = WorldController::getInstance()->getMaxMarchCount();
    if (current >= max) {
        WorldController::getInstance()->showMarchAlert(max);
        return;
    }
    CCArray* arr = CCArray::create();
    int num = m_resList->count();
    for(int i=0;i<num;i++){
        TerritoryWarehouseCell* cell = (TerritoryWarehouseCell*)m_resList->objectAtIndex(i);
        if(cell->m_index==3){
            m_tradeToServerFood = cell->m_currentValue;
        }
        auto dic = CCDictionary::create();
        dic->setObject(CCInteger::create(cell->m_currentValue),"v");
        dic->setObject(CCInteger::create(cell->m_index),"t");
        arr->addObject(dic);
    }
    m_lastTotalFood = GlobalData::shared()->resourceInfo.lFood;
    if (m_storage) {
        WorldMarchCommand* cmd = new WorldMarchCommand(m_warehousePtId,MethodWarehouse,1,NULL,arr);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryWarehouseView::tradeSuccess), NULL));
        cmd->sendAndRelease();
    }
    else {
        WorldMarchCommand* cmd = new WorldMarchCommand(m_warehousePtId,MethodWarehouse,1,NULL,arr,-1,NULL,-1,"",0);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryWarehouseView::tradeSuccess), NULL));
        cmd->sendAndRelease();
    }
    m_btnTrade->setEnabled(false);
}

void TerritoryWarehouseView::getPlayerStorageLimit(){
    CCDictElement *element = NULL;
    auto dictEffect = LocalController::shared()->DBXMLManager()->getGroupByKey("role");
    CCDICT_FOREACH(dictEffect, element){
        std::string roleId = element->getStrKey();
        int compareId = atoi(roleId.c_str());
        if (compareId == FunBuildController::getInstance()->getMainCityLv() + 100100) {
            std::string dayStr = CCCommonUtils::getPropById(roleId, "storage");
            m_dayStorageLimit = atol(dayStr.c_str());
            std::string totalStr = CCCommonUtils::getPropById(roleId, "content");
            m_totalStorageLimit = atol(totalStr.c_str());
            break;
        }
    }
}

TerritoryWarehouseCell* TerritoryWarehouseCell::create(unsigned long dayLimit,unsigned long totalLimit,int index,bool storage)
{
    auto ret = new TerritoryWarehouseCell(dayLimit,totalLimit,index,storage);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TerritoryWarehouseCell::init()
{
    CCBLoadFile("AllianceTradeCell",this,this);
    std::string file = "";
    std::string resName = "";
    bool silderFlag = true;
    switch (m_index) {
        case 0:
            file = "ui_wood.png";
            resName = _lang("102209");
            break;
        case 1:
            file = "ui_stone.png";
            resName = _lang("102208");
            if(FunBuildController::getInstance()->getMainCityLv() < FunBuildController::getInstance()->building_base_k4){
                m_coverTxt->setString(_lang("115088"));
                silderFlag = false;
            }else{
                m_coverTxt->setString("");
                silderFlag = true;
            }
            m_coverNode->setVisible(!silderFlag);
            m_coverTxt->setVisible(m_coverNode->isVisible());
            break;
        case 2:
        {
            file = "ui_iron.png";
            resName = _lang("102210");
            if(FunBuildController::getInstance()->getMainCityLv() < FunBuildController::getInstance()->building_base_k3){
                m_coverTxt->setString(_lang("115088"));
                silderFlag = false;
            }else{
                m_coverTxt->setString("");
                silderFlag = true;
            }
            m_coverNode->setVisible(!silderFlag);
            m_coverTxt->setVisible(m_coverNode->isVisible());
        }
            
            break;
        case 3:
            file = "ui_food.png";
            resName = _lang("102211");
            break;
        default:
            break;
    }
    m_nameTxt->setString(resName.c_str());
    auto pic = CCLoadSprite::createSprite(file.c_str());
    if (pic) {
        m_resNode->addChild(pic);
        pic->setPosition(ccp(2,0));
    }
    
    m_load = 0;
    m_limit = MIN(m_totalStorageLimit, m_dayStorageLimit);
    
    m_slider = NBSlider::create("nb_progressBarBg.png", "nb_bar_lv.png", "nb_warehouse_huakuai.png",NBSlider::TextureResType::PLIST);
    m_slider->setCapInsets(Rect(8, 1, 30, 13));
    auto ts = m_sliderNode->getContentSize();
    m_slider->setContentSize(ts);

    m_slider->addEventListener(CC_CALLBACK_2(TerritoryWarehouseCell::sliderCallBack, this));
//    m_slider->setPosition(-230 / 2, -15 / 2);
    m_slider->setValue(0);
    m_sliderNode->addChild(m_slider, 1);
    
//    auto m_sliderBg = CCLoadSprite::createScale9Sprite("huadongtiao3.png");
//    m_sliderBg->setInsetBottom(5);
//    m_sliderBg->setInsetLeft(5);
//    m_sliderBg->setInsetRight(5);
//    m_sliderBg->setInsetTop(5);
//    m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
//    m_sliderBg->setPosition(ccp(290/2, 25));
//    m_sliderBg->setContentSize(CCSize(290,18));
//    
//    auto bgSp = CCLoadSprite::createSprite("huadongtiao3.png");
//    bgSp->setVisible(false);
//    auto proSp = CCLoadSprite::createSprite("huadongtiao4.png");
//    auto thuSp = CCLoadSprite::createSprite("huadongtiao1.png");
//    
//    m_slider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
//    m_slider->setMinimumValue(0.0f);
//    m_slider->setMaximumValue(1.0f);
//    m_slider->setProgressScaleX(290/proSp->getContentSize().width);
//    m_slider->setTag(1);
//    m_slider->setLimitMoveValue(20);
//    m_slider->addTargetWithActionForControlEvents(this, cccontrol_selector(TerritoryWarehouseCell::sliderCallBack), CCControlEventValueChanged);
//    m_sliderNode->addChild(m_slider, 1);
    m_slider->setEnabled(silderFlag);
    
    m_valueTxt->setString(CC_ITOA(0));
    m_currentValue = 0;
    m_invalidSliderMove = false;
    return true;
}

void TerritoryWarehouseCell::resetData(){
    m_currentValue = 0;
    m_invalidSliderMove = true;
    m_slider->setValue(0);
}

void TerritoryWarehouseCell::sliderCallBack(Ref *pSender, NBSlider::EventType type){
    if(m_invalidSliderMove){
        m_invalidSliderMove = false;
        return ;
    }
    if (m_storage) {
        unsigned long max = 0;
        unsigned long temp = 0;
        float wRate = 1.0;
        switch (m_index) {
            case 0:
            {
                wRate = GlobalData::shared()->resources_weight_1;
                max = floor(m_limit*1.0/wRate);
                temp = GlobalData::shared()->resourceInfo.lWood;
            }
                break;
            case 1:
            {
                wRate = GlobalData::shared()->resources_weight_2;
                max = floor(m_limit*1.0/wRate);
                temp = GlobalData::shared()->resourceInfo.lStone;
            }
                break;
            case 2:
            {
                wRate = GlobalData::shared()->resources_weight_3;
                max = floor(m_limit*1.0/wRate);
                temp = GlobalData::shared()->resourceInfo.lIron;
            }
                break;
            case 3:
            {
                wRate = GlobalData::shared()->resources_weight_4;
                max = floor(m_limit*1.0/wRate);
                temp = GlobalData::shared()->resourceInfo.lFood;
            }
                break;
            default:
                break;
        }
        
        
        unsigned long canChange = floor(MAX(0, (long)(m_limit - m_load))/wRate);
        unsigned long canHave = MAX(0, (long)(temp - m_currentValue));
        unsigned long canLoad = MIN(canHave, canChange) + m_currentValue;
        unsigned long sliderValue = ceil(m_slider->getValue()*max);
        if(sliderValue>canLoad || max == 0){
            m_currentValue = canLoad;
            m_invalidSliderMove = true;
            if (max == 0) {
                float percent = 0;
                m_slider->setValue(percent);
            }else {
                float percent = m_currentValue*1.0/max;
                m_slider->setValue(percent);
            }
        }else{
            m_currentValue = floor(m_slider->getValue()*max);
        }
        m_valueTxt->setString(CC_CMDITOAL(m_currentValue));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CURRENT_LOAD_CHANGE,CCString::createWithFormat("%d",m_index));

    }
    else {
        unsigned long max = 0;
        unsigned long temp = 0;
        float wRate = 1.0;
        switch (m_index) {
            case 0:
            {
                wRate = GlobalData::shared()->resources_weight_1;
                max = floor(GlobalData::shared()->resourceInfo.warehouseWood*1.0);
                temp = GlobalData::shared()->resourceInfo.warehouseWood;
            }
                break;
            case 1:
            {
                wRate = GlobalData::shared()->resources_weight_2;
                max = floor(GlobalData::shared()->resourceInfo.warehouseStone*1.0);
                temp = GlobalData::shared()->resourceInfo.warehouseStone;
            }
                break;
            case 2:
            {
                wRate = GlobalData::shared()->resources_weight_3;
                max = floor(GlobalData::shared()->resourceInfo.warehouseIron*1.0);
                temp = GlobalData::shared()->resourceInfo.warehouseIron;
            }
                break;
            case 3:
            {
                wRate = GlobalData::shared()->resources_weight_4;
                max = floor(GlobalData::shared()->resourceInfo.warehouseFood*1.0);
                temp = GlobalData::shared()->resourceInfo.warehouseFood;
            }
                break;
            default:
                break;
        }
        
        unsigned long canHave = temp;
        unsigned long sliderValue = ceil(m_slider->getValue()*max);
        if(sliderValue>canHave || max == 0){
            m_currentValue = canHave;
            m_invalidSliderMove = true;
            if (max == 0) {
                float percent = 0;
                m_slider->setValue(percent);
            }else {
                float percent = m_currentValue*1.0/max;
                m_slider->setValue(percent);
            }
        }else{
            m_currentValue = floor(m_slider->getValue()*max);
        }
        m_valueTxt->setString(CC_CMDITOAL(m_currentValue));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CURRENT_LOAD_CHANGE,CCString::createWithFormat("%d",m_index));
    }
}

void TerritoryWarehouseCell::setData(unsigned long load){
    m_load = load;
}

void TerritoryWarehouseCell::onEnter() {
    CCNode::onEnter();
}

void TerritoryWarehouseCell::onExit() {
    CCNode::onExit();
}


SEL_CCControlHandler TerritoryWarehouseCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}


bool TerritoryWarehouseCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueTxt", CCLabelIF*, this->m_valueTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resNode", CCNode*, this->m_resNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sliderNode", CCNode*, this->m_sliderNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coverNode", CCNode*, this->m_coverNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coverTxt", CCLabelIF*, this->m_coverTxt);
    return false;
}