//
//  AllianceResourceTradeView.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-4.
//
//

#include "AllianceResourceTradeView.h"
#include "FunBuildController.h"
#include "PopupViewController.h"
#include "BuildMoreInfoView.h"
#include "WorldController.h"
#include "AllianceManager.h"
#include "WorldCommand.h"
#include "SceneController.h"
#include "CCShake.h"
#include "GetUserInfoCommand.h"

void AllianceResourceTradeView::createResurceTradeByUid(const string& uid,int pointId,int mainLv) {
    auto ret = new AllianceResourceTradeView(uid,pointId,mainLv);
    auto cmd = new GetUserInfoCommand(uid);
    cmd->setCallback(CCCallFuncO::create(ret, callfuncO_selector(AllianceResourceTradeView::getPlayerData), nullptr));
    cmd->sendAndRelease();
    
}

void AllianceResourceTradeView::getPlayerData(CCObject* obj){
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        this->release();
        return;
    }
    
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (!info) {
        this->release();
        return;
    }
    int haveMarket = 1;
    if (info->objectForKey("market")) {
        haveMarket = info->valueForKey("market")->intValue();
    }
    if (haveMarket == 0) {
        CCCommonUtils::flyText(_lang("115500"));
        this->release();
        return;
    }
    else if (haveMarket == 1) {
        this->m_info = new PlayerInfo();
        this->m_info->updateInfo(info);
        this->m_info->pointId = m_pointId;
        this->m_info->mainCityLv = m_mainLv;
        this->init();
        PopupViewController::getInstance()->addPopupInView(this);
        this->autorelease();
    }
}

AllianceResourceTradeView* AllianceResourceTradeView::create(PlayerInfo* info){
    AllianceResourceTradeView* ret = new AllianceResourceTradeView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool AllianceResourceTradeView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
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

    m_cityNameTxt->setString(_lang("115071"));
    m_tradePlayerTxt->setString(_lang_1("115073",m_info->name.c_str()));
    m_infoTxt->setString(_lang("108729"));
    
    m_scrollView = CCScrollView::create(m_scrollNode->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollNode->addChild(m_scrollView);
 
    m_resList = CCArray::create();
    int pos[4] = {0,3,2,1};
    int listH = 83*3;
    int num = 2;
    int offY = 83*2;
    if (CCCommonUtils::isIosAndroidPad()) {
        listH = 180*3;
        offY = 180*2;
    }
    if(FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k4){
        num = 4;
        offY = 0;
    }else if (FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k3){
        num = 3;
        offY = 83;
        if (CCCommonUtils::isIosAndroidPad()) {
            offY = 180;
        }
    }
    for(int i=0;i<num;i++){
        AllianceResourceCell* cell = AllianceResourceCell::create(m_info,pos[i]);
        cell->setAnchorPoint(ccp(0, 0.5));
        if (CCCommonUtils::isIosAndroidPad()) {
            cell->setPosition(ccp(0,listH-i*180));
        }
        else
            cell->setPosition(ccp(0,listH-i*83));
        m_infoList->addChild(cell);
        m_resList->addObject(cell);
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
        
        m_scrollView->addChild(m_funNode1);
        m_scrollView->addChild(m_funNode2);
        
        int contentHeight = 840;
        m_scrollView->setContentSize(CCSize(640,contentHeight));
        m_scrollView->setContentOffset(ccp(0, m_scrollNode->getContentSize().height - contentHeight));
    }
    
    m_ironNode->setVisible(FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k3);
    m_ironTradeNode->setVisible(m_ironNode->isVisible());
    m_coverNode1->setVisible(m_info->mainCityLv<FunBuildController::getInstance()->building_base_k3);
    
    m_stoneNode->setVisible(FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k4);
    m_stoneTradeNode->setVisible(m_stoneNode->isVisible());
    m_coverNode2->setVisible(m_info->mainCityLv<FunBuildController::getInstance()->building_base_k4);
    
//    CCPoint endPos = WorldController::getPointByIndex(m_info->pointId);
//    CCPoint selfPoint = WorldController::getInstance()->selfPoint;
//    float distance = WorldController::getInstance()->getDistance(selfPoint, endPos, true, true);
//    
//    double k1 = GlobalData::shared()->worldConfig.distance[0];
//    double k4 = GlobalData::shared()->worldConfig.distance[3];
//    int walkTime = pow(distance, k1) * k4;
//    walkTime = walkTime*1.0/(1+CCCommonUtils::getEffectValueByNum(ALLIANCE_TRADE_SPEED_TIME_EFFECT)/100.0);
    int startPt = WorldController::getIndexByPoint(WorldController::getInstance()->selfPoint);
    int walkTime = WorldController::getInstance()->getMarchTime(startPt, m_info->pointId, CityTile, MethodDeal, true);
    walkTime = walkTime / 1000;
    m_timeTxt->setString(CC_SECTOA(walkTime));
    
    AllianceManager::getInstance()->currentTrade = 0;
    AllianceManager::getInstance()->maxTrade = 10;
    m_rate = 22;
    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_TAVERN);
    int max = 1000;
    if (buildId>0) {
        FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(buildId);
        max = atoi(info.para[1].c_str()) + CCCommonUtils::getEffectValueByNum(ALLIANCE_TRADE_MAX_EFFECT);
        m_rate = atoi(info.para[2].c_str()) - CCCommonUtils::getEffectValueByNum(ALLIANCE_TRADE_RATE_EFFECT);
        AllianceManager::getInstance()->maxTrade = max;
    }
    
    AllianceManager::getInstance()->rate = m_rate*0.01;
    AllianceManager::getInstance()->maxTrade = max;
    AllianceManager::getInstance()->lastTrade = 0;
    m_tradeToServerFood = 0;
    m_lastTrade = 0;
    this->schedule(schedule_selector(AllianceResourceTradeView::updateChangeStatus), 0.1);
    this->updateInfo();
    return true;
}


void AllianceResourceTradeView::updateInfo()
{
    this->m_wood->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lWood));
    this->m_stone->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lStone));
    this->m_iron->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lIron));
    this->m_food->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lFood));
    this->m_tradeWood->setString("0");
    this->m_tradeFood->setString("0");
    this->m_tradeIron->setString("0");
    this->m_tradeStone->setString("0");
    
    AllianceManager::getInstance()->currentTrade = 0;
    this->updateTradeData();
    m_btnTrade->setEnabled(false);
}

void AllianceResourceTradeView::tradeSuccess(CCObject* p){
    int temp = m_lastTotalFood - GlobalData::shared()->resourceInfo.lFood;
    if(m_tradeToServerFood!=0 && m_tradeToServerFood!= temp && temp>0){
        
    }
    int num = m_resList->count();
    for(int i=0;i<num;i++){
        AllianceResourceCell* cell = (AllianceResourceCell*)m_resList->objectAtIndex(i);
        cell->resetData();
    }
    AllianceManager::getInstance()->currentTrade = 0;
    this->updateInfo();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE,NULL);
    CCCommonUtils::flyHint("", "", _lang("115089"));
    
    PopupViewController::getInstance()->removeAllPopupView();
    SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
}

void AllianceResourceTradeView::updateTradeData(bool isEnd){
    std::string str = CC_ITOA(m_rate);
    str.append("%");
    str = _lang_1("115072", str.c_str());
    m_rateTxt->setString(str);
    str = CC_ITOA(AllianceManager::getInstance()->currentTrade);
    str.append("/");
    str.append(CC_ITOA(AllianceManager::getInstance()->maxTrade));
    m_loadTxt->setString(_lang_1("115074", str.c_str()));
    if(AllianceManager::getInstance()->currentTrade>=AllianceManager::getInstance()->maxTrade && isEnd){
        m_loadTxt->stopAllActions();
        m_lastTrade = AllianceManager::getInstance()->currentTrade;
        CCSequence* sc1 = CCSequence::create(CCScaleTo::create(0.1, 1.05),NULL);
        CCSequence* sc2 = CCSequence::create(CCTintTo::create(0.25, 255, 0, 0),CCTintTo::create(0.01, 0, 244, 0),CCScaleTo::create(0.08, 1),NULL);
        CCSpawn* sp = CCSpawn::create(sc1,sc2,NULL);
        m_loadTxt->runAction(sp);
    }
    m_btnTrade->setEnabled(AllianceManager::getInstance()->currentTrade>0);

}

void AllianceResourceTradeView::playResourceAni(){
    int num = m_resList->count();
    for(int i=0;i<num;i++){
        AllianceResourceCell* cell = (AllianceResourceCell*)m_resList->objectAtIndex(i);
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

void AllianceResourceTradeView::updateTradeValue(CCObject* obj){
    m_scrollView->setTouchEnabled(false);
    AllianceManager::getInstance()->currentTrade = 0;
    AllianceManager::getInstance()->lastTrade = 0;
    int num = m_resList->count();
    for(int i=0;i<num;i++){
        AllianceResourceCell* cell = (AllianceResourceCell*)m_resList->objectAtIndex(i);
        unsigned long addValue = 0;
        switch (cell->m_index) {
            case 0:
                addValue = cell->m_currentValue*GlobalData::shared()->resources_weight_1;
                m_tradeWood->setString(CC_ITOA(cell->m_currentValue*(1-AllianceManager::getInstance()->rate)));
                break;
            case 1:
                addValue = cell->m_currentValue*GlobalData::shared()->resources_weight_2;
                m_tradeStone->setString(CC_ITOA(cell->m_currentValue*(1-AllianceManager::getInstance()->rate)));
                break;
            case 2:

                addValue = cell->m_currentValue*GlobalData::shared()->resources_weight_3;
                m_tradeIron->setString(CC_ITOA(cell->m_currentValue*(1-AllianceManager::getInstance()->rate)));
                break;
            case 3:
                addValue = cell->m_currentValue*GlobalData::shared()->resources_weight_4;
                m_tradeFood->setString(CC_ITOA(cell->m_currentValue*(1-AllianceManager::getInstance()->rate)));
                break;
            default:
                break;
        }
        AllianceManager::getInstance()->currentTrade += addValue;
        CCString* str = (CCString*)obj;
        if(str->compare(CC_ITOA(cell->m_index))!=0){
            AllianceManager::getInstance()->lastTrade += addValue;
        }
    }
    this->updateTradeData();
}

bool AllianceResourceTradeView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void AllianceResourceTradeView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    updateTradeData(true);
}

void AllianceResourceTradeView::updateChangeStatus(float t){
    this->m_food->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lFood));
    m_scrollView->setTouchEnabled(true);
}

void AllianceResourceTradeView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceResourceTradeView::updateTradeValue), MSG_UPDATE_TRADE_VALUE, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    this->setTitleName(_lang("108729"));
}

void AllianceResourceTradeView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UPDATE_TRADE_VALUE);
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler AllianceResourceTradeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
     CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnTradeClick", AllianceResourceTradeView::onBtnTradeClick);
    
    return NULL;
}

bool AllianceResourceTradeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wood", CCLabelIF*, this->m_wood);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stone", CCLabelIF*, this->m_stone);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iron", CCLabelIF*, this->m_iron);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_food", CCLabelIF*, this->m_food);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rateTxt", CCLabelIF*, this->m_rateTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadTxt", CCLabelIF*, this->m_loadTxt);
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
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodIcon", CCNode*, this->m_woodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodIcon", CCNode*, this->m_foodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironIcon", CCNode*, this->m_ironIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneIcon", CCNode*, this->m_stoneIcon);
    return false;
}

void AllianceResourceTradeView::onBtnTradeClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_info->pointId==0){
        CCCommonUtils::flyText((_lang("138063").c_str()));
        return;
    }
    unsigned int current =  WorldController::getInstance()->getCurrentMarchCount();
    unsigned int max = WorldController::getInstance()->getMaxMarchCount();
    if (current >= max) {
        WorldController::getInstance()->showMarchAlert(max);
        return;
    }
    CCArray* arr = CCArray::create();
    int num = m_resList->count();
    for(int i=0;i<num;i++){
        AllianceResourceCell* cell = (AllianceResourceCell*)m_resList->objectAtIndex(i);
        if(cell->m_index==3){
            m_tradeToServerFood = cell->m_currentValue;
        }
        auto dic = CCDictionary::create();
        dic->setObject(CCInteger::create(cell->m_currentValue),"v");
        dic->setObject(CCInteger::create(cell->m_index),"t");
        arr->addObject(dic);
    }
    m_lastTotalFood = GlobalData::shared()->resourceInfo.lFood;
    WorldMarchCommand* cmd = new WorldMarchCommand(m_info->pointId,MethodDeal,1,NULL,arr);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceResourceTradeView::tradeSuccess), NULL));
    cmd->sendAndRelease();
    m_btnTrade->setEnabled(false);
}


AllianceResourceCell* AllianceResourceCell::create(PlayerInfo* player,int index)
{
    auto ret = new AllianceResourceCell(player,index);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceResourceCell::init()
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
            }else if(m_info->mainCityLv < FunBuildController::getInstance()->building_base_k4){
                m_coverTxt->setString(_lang("115083"));
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
            }else if(m_info->mainCityLv < FunBuildController::getInstance()->building_base_k3){
                m_coverTxt->setString(_lang("115083"));
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
    auto pic = CCLoadSprite::createSprite(file.c_str());//CCCommonUtils::getIcon(GlobalData::shared()->m_trainGeneralId).c_str()
    if (pic) {
        m_resNode->addChild(pic);
        pic->setPosition(ccp(2,0));
    }
    
    
    auto m_sliderBg = CCLoadSprite::createScale9Sprite("huadongtiao3.png");
    m_sliderBg->setInsetBottom(5);
    m_sliderBg->setInsetLeft(5);
    m_sliderBg->setInsetRight(5);
    m_sliderBg->setInsetTop(5);
    m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
    m_sliderBg->setPosition(ccp(290/2, 25));
    m_sliderBg->setContentSize(CCSize(290,18));
    
    auto bgSp = CCLoadSprite::createSprite("huadongtiao3.png");
    bgSp->setVisible(false);
    auto proSp = CCLoadSprite::createSprite("huadongtiao4.png");
    auto thuSp = CCLoadSprite::createSprite("huadongtiao1.png");
    
    m_slider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
    m_slider->setMinimumValue(0.0f);
    m_slider->setMaximumValue(1.0f);
    m_slider->setProgressScaleX(290/proSp->getContentSize().width);
    m_slider->setTag(1);
    m_slider->setLimitMoveValue(20);
    m_slider->addTargetWithActionForControlEvents(this, cccontrol_selector(AllianceResourceCell::sliderCallBack), CCControlEventValueChanged);
    m_sliderNode->addChild(m_slider, 1);
    m_slider->setEnabled(silderFlag);
    
    m_valueTxt->setString(CC_ITOA(0));
    m_currentValue = 0;
    m_invalidSliderMove = false;
    return true;
}

void AllianceResourceCell::resetData(){
    m_currentValue = 0;
    m_invalidSliderMove = true;
    m_slider->setValue(0);
}

void AllianceResourceCell::sliderCallBack(CCObject*sender,CCControlEvent even){
    if(m_invalidSliderMove){
        m_invalidSliderMove = false;
        return ;
    }
    unsigned long max = 0;
    unsigned long temp = 0;
    float wRate = 1.0;
    switch (m_index) {
        case 0:
        {
            wRate = GlobalData::shared()->resources_weight_1;
            temp = floor(AllianceManager::getInstance()->maxTrade*1.0/wRate);
            max = MIN(GlobalData::shared()->resourceInfo.lWood, temp);
        }
            break;
        case 1:
        {
            wRate = GlobalData::shared()->resources_weight_2;
            temp = floor(AllianceManager::getInstance()->maxTrade*1.0/wRate);
            max = MIN(GlobalData::shared()->resourceInfo.lStone, temp);
        }
            break;
        case 2:
        {
            wRate = GlobalData::shared()->resources_weight_3;
            temp = floor(AllianceManager::getInstance()->maxTrade*1.0/wRate);
            max = MIN(GlobalData::shared()->resourceInfo.lIron, temp);
        }
            break;
        case 3:
        {
            wRate = GlobalData::shared()->resources_weight_4;
            temp = floor(AllianceManager::getInstance()->maxTrade*1.0/wRate);
            max = MIN(GlobalData::shared()->resourceInfo.lFood, temp);
        }
            break;
        default:
            break;
    }
    unsigned long haveTrade = AllianceManager::getInstance()->maxTrade - AllianceManager::getInstance()->lastTrade;
    unsigned long canChange = floor(haveTrade/wRate);
    unsigned long sliderValue = ceil(m_slider->getValue()*max);
    if(sliderValue>canChange){
        m_currentValue = canChange;
        m_invalidSliderMove = true;
        float percent = m_currentValue*1.0/max;
        m_slider->setValue(percent);
    }else{
        m_currentValue = floor(m_slider->getValue()*max);
    }
    m_valueTxt->setString(CC_CMDITOAL(m_currentValue));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPDATE_TRADE_VALUE,CCString::createWithFormat("%d",m_index));
}

void AllianceResourceCell::onEnter() {
    CCNode::onEnter();
}

void AllianceResourceCell::onExit() {
    CCNode::onExit();
}


SEL_CCControlHandler AllianceResourceCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}


bool AllianceResourceCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueTxt", CCLabelIF*, this->m_valueTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resNode", CCNode*, this->m_resNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sliderNode", CCNode*, this->m_sliderNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coverNode", CCNode*, this->m_coverNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coverTxt", CCLabelIF*, this->m_coverTxt);
    return false;
}