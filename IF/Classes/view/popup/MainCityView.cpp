//
//  MainCityView.cpp
//  IF
//
//  Created by fubin on 14-2-26.
//
//

#include "MainCityView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "MainCityMoreView.h"
#include "ArmyController.h"
#include "WorldController.h"
#include "NetController.h"
#include "AddDefenseCommand.h"
#include "BuildMoreInfoView.h"
#include "QueueController.h"
#include "UseCDToolView.h"

MainCityView* MainCityView::create(int buildId){
    MainCityView* ret = new MainCityView();
    if(ret && ret->init(buildId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MainCityView::init(int buildId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    m_dataCnt = 4;
    m_buildId = buildId;
    m_isFree = false;
    auto tmpCCB = CCBLoadFile("MainCityView02",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    
    FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    string title = _lang(m_info.name)+" "+_lang_1("102272", CC_ITOA(m_info.level));
    if(m_info.level>=GlobalData::shared()->MaxBuildLv) {
        title += _lang("102183");
    }
    setTitleName(title.c_str());
    
    m_nameDefLabel->setString(_lang("102206"));
    
    if (!CCCommonUtils::isIosAndroidPad()) {
        int oldBgHeight = m_buildBG->getContentSize().height;
        changeBGMaxHeight(m_buildBG);
        int newBgHeight = m_buildBG->getContentSize().height;
        int addHeight = newBgHeight - oldBgHeight;
        int oldWidth = m_infoList->getContentSize().width;
        int oldHeight = m_infoList->getContentSize().height;
        m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    }
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    m_defArmyNode->removeFromParent();
    onEnterFrame(0);
    updateInfo();
    
    return true;
}

void MainCityView::updateInfo()
{
    int _itemH = 145;
    int curY = 30;
    int curX = 10;
    if (CCCommonUtils::isIosAndroidPad()) {
        _itemH = 280;
        curY = 0;
        curX = 50;
    }
//    m_infoList->removeChild(m_moreInfoBtn);
//    m_scrollView->addChild(m_moreInfoBtn);
//    CCCommonUtils::setButtonTitle(m_moreInfoBtn, _lang("102105").c_str());
    
    if (m_isFull) {
//        m_defArmyNode->setPosition(ccp(curX, curY));
//        m_scrollView->addChild(m_defArmyNode);
//        curY += _itemH;
    }
    
    //  -1表示兵力 0表示木头，1表示石头，2表示铁，3表示粮食
    vector<int> m_tmpType;
//    m_tmpType.push_back(-1);
    m_tmpType.push_back(Wood);
    m_tmpType.push_back(Food);
    m_tmpType.push_back(Iron);
    m_tmpType.push_back(Stone);
    
    int mlv = FunBuildController::getInstance()->getMainCityLv();
    for (int i=m_tmpType.size()-1; i>=0; i--) {
        bool canSt = true;
        int curType = m_tmpType[i];
        if (curType==Stone) {
            if (mlv<FunBuildController::getInstance()->building_base_k4) {
                canSt = false;
            }
        }
        if (curType==Iron) {
            if (mlv<FunBuildController::getInstance()->building_base_k3) {
                canSt = false;
            }
        }
        if (true || canSt) {
            auto cell = ResInfoCell::create(curType, m_buildId);
            cell->setPosition(ccp(curX, curY));
            cell->setTouchNode(m_infoList);
            curY += _itemH;
            m_scrollView->addChild(cell);
        }
    }
    
    if (!m_isFull) {
//        m_defArmyNode->setPosition(ccp(curX, curY));
//        m_scrollView->addChild(m_defArmyNode);
//        curY += _itemH;
    }
    
    if (curY<=m_infoList->getContentSize().height) {
        m_scrollView->setTouchEnabled(false);
    }
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
}

void MainCityView::onEnter()
{
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(MainCityView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

void MainCityView::onExit()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(MainCityView::onEnterFrame), this);
    CCNode::onExit();
}

void MainCityView::onEnterFrame(float dt)
{
}

void MainCityView::onClickAddDefense(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    m_defBtn->setEnabled(false);
    m_defBtn->setColor(ccGRAY);
    
    auto cmd = new AddDefenseCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(MainCityView::onUpdateDefenseCallback), NULL));
    cmd->sendAndRelease();
}

void MainCityView::onUpdateDefenseCallback(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    GlobalData::shared()->cityDefenseVal = info->valueForKey("cityDefValue")->intValue();
    GlobalData::shared()->cityLastUpdateDefenseStamp = info->valueForKey("lastCityDefTime")->doubleValue();
    GlobalData::shared()->cityStartFireStamp = WorldController::getInstance()->getTime();
}

SEL_CCControlHandler MainCityView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", MainCityView::onDeleteClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUpBtn", MainCityView::onClickUpBtn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMoreInfoClick", MainCityView::onMoreInfoClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickAddDefense", MainCityView::onClickAddDefense);
    return NULL;
}

bool MainCityView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_instantBtn", CCControlButton*, this->m_instantBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreInfoBtn", CCControlButton*, this->m_moreInfoBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, this->m_upBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, this->m_msg1Label);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, this->m_msg2Label);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barLabel", CCLabelIF*, this->m_barLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, this->m_upNode);
   
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, this->m_btnMsgLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, this->m_inBtnGoldNum);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnNode", CCNode*, this->m_inBtnNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, this->m_bar);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defArmyNode", CCNode*, this->m_defArmyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defBtn", CCControlButton*, this->m_defBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defLabel", CCLabelIF*, this->m_defLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireLabel", CCLabelIF*, this->m_fireLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defbar", CCScale9Sprite*, this->m_defbar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defbarLabel", CCLabelIF*, this->m_defbarLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameDefLabel", CCLabelIF*, this->m_nameDefLabel);
    
    return false;
}

void MainCityView::onMoreInfoClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupView(BuildMoreInfoView::create(m_buildId));
}

void MainCityView::onDeleteClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    YesNoDialog::show( _lang("102195").c_str() , CCCallFunc::create(this, callfunc_selector(MainCityView::onYesCancel)));
}

void MainCityView::onYesCancel()
{
    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA(m_buildId));
    if (qid>0) {
        QueueController::getInstance()->startCancelQueue(qid);
        FunBuildController::getInstance()->cancelBuildState(m_buildId);
    }
}

void MainCityView::onClickUpBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
}

ResInfoCell* ResInfoCell::create(int type, int buildId)
{
    auto ret = new ResInfoCell();
    if (ret && ret->init(type, buildId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ResInfoCell::init(int type, int buildId)
{
    bool ret = true;
    CCBLoadFile("ResInfoCell",this,this);
    
    m_type = type;
    m_buildId = buildId;
    m_iconNode->removeAllChildren();
    string picName = CCCommonUtils::getResourceIconByType(m_type);
    if(m_type == -1) {
        picName = "force_icon.png";
    }
    if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
    {
        std::vector<string> _strvet;
        CCCommonUtils::splitString(picName.c_str(), ".", _strvet);
        picName = _strvet[0] + "_hd." + _strvet[1];
        
    }
    auto pic = CCLoadSprite::createSprite(picName.c_str());
    
    if (pic) {
        if(m_type == -1) {
            if (CCCommonUtils::isIosAndroidPad()) {
                CCCommonUtils::setSpriteMaxSize(pic, 170, true);
            }
            else
                CCCommonUtils::setSpriteMaxSize(pic, 90, true);
        }
        else {
            if (CCCommonUtils::isIosAndroidPad()) {
                CCCommonUtils::setSpriteMaxSize(pic, 170, true);
            }
            else
                CCCommonUtils::setSpriteMaxSize(pic, 80, true);
        }
        m_iconNode->addChild(pic);
    }
    
    updateInfo(NULL);
    return ret;
}

void ResInfoCell::setTouchNode(CCNode* touchNode)
{
    m_touchNode = touchNode;
}

void ResInfoCell::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ResInfoCell::updateInfo), MSG_CITY_RESOURCES_UPDATE, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
   //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, true);
}

void ResInfoCell::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CITY_RESOURCES_UPDATE);
    setTouchEnabled(false);
    CCNode::onExit();
}

void ResInfoCell::updateInfo(CCObject* params)
{
//  -1表示兵力 0表示木头，1表示石头，2表示铁，3表示粮食
//    m_capNode->setVisible(false);
    int curNum = 0;
    int curStorage = 0;
    int outputPreHour = 0;
    int add = 0;
    
//    m_moreBtn->setEnabled(false);
    
    m_info1TitleLabel->setString("Wood Income");
    m_info2TitleLabel->setString(_lang("102125"));
    m_info3Label->setString("/h");
    m_info5Label->setString("/h");
    string resName = CCCommonUtils::getResourceNameByType(m_type);
    m_info1TitleLabel->setString(_lang_1("102134", resName.c_str()));
    m_lockNameLabel->setString(resName);
    
    m_lockNode->setVisible(false);
    m_info2Node->setVisible(false);
    m_info1Node->setVisible(true);
    int buildType = FUN_BUILD_WOOD;
    float effectNum = 0;
    if (m_type == Wood) {
        outputPreHour = FunBuildController::getInstance()->oriWoodPreH;
        effectNum = CCCommonUtils::getEffectValueByNum(50);
        add = outputPreHour*effectNum*1.0/100;
        buildType = FUN_BUILD_WOOD;
    }
    else if (m_type == Stone) {
        outputPreHour = FunBuildController::getInstance()->StoneOutPreSed*3600;
        int mlv = FunBuildController::getInstance()->getMainCityLv();
        if (mlv<FunBuildController::getInstance()->building_base_k4) {
            m_lockInfoLabel->setString(_lang_1("102299", CC_ITOA(FunBuildController::getInstance()->building_base_k4)));
            m_info1Node->setVisible(false);
            m_lockNode->setVisible(true);
        }
        else {
            outputPreHour = FunBuildController::getInstance()->oriStonePreH;
            effectNum = CCCommonUtils::getEffectValueByNum(51);
            add = outputPreHour*effectNum*1.0/100;
        }
        buildType = FUN_BUILD_STONE;
    }
    else if (m_type == Iron) {
        outputPreHour = FunBuildController::getInstance()->IronOutPreSed*3600;
        int mlv = FunBuildController::getInstance()->getMainCityLv();
        if (mlv<FunBuildController::getInstance()->building_base_k3) {
            m_lockInfoLabel->setString(_lang_1("102299", CC_ITOA(FunBuildController::getInstance()->building_base_k3)));
            m_info1Node->setVisible(false);
            m_lockNode->setVisible(true);
        }
        else {
            outputPreHour = FunBuildController::getInstance()->oriIronPreH;
            effectNum = CCCommonUtils::getEffectValueByNum(52);
            add = outputPreHour*effectNum*1.0/100;
        }
        buildType = FUN_BUILD_IRON;
    }
    else if (m_type == Food) {
        outputPreHour = FunBuildController::getInstance()->oriFoodPreH;
        effectNum = CCCommonUtils::getEffectValueByNum(53);
        add = outputPreHour*effectNum*1.0/100;
        
        m_info4Label->setString(CC_CMDITOA(-ArmyController::getInstance()->getTotalUpKeep()));
        m_info5Label->setPositionX(m_info4Label->getPositionX()+m_info4Label->getContentSize().width*m_info4Label->getOriginScaleX());
        
        m_info2Node->setVisible(true);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_info1Node->setPositionY(34);
            m_info2Node->setPositionY(-34);
        }
        else {
            m_info1Node->setPositionY(20);
            m_info2Node->setPositionY(-20);
        }
        
        buildType = FUN_BUILD_FOOD;
    }
    
    if (!m_lockNode->isVisible()) {
        map<int,FunBuildInfo>::iterator it;
        it = FunBuildController::getInstance()->curBuildsInfo->begin();
        for (; it != FunBuildController::getInstance()->curBuildsInfo->end(); it++)
        {
            if (it->second.type == buildType) {
                double effectTime = it->second.effectTime;
                int gapTime = effectTime - GlobalData::shared()->getWorldTime();
                if(gapTime>0 && effectTime>0){
                    int oriOneBuildOut = atoi( (it->second).para[1].c_str() );
                    add += oriOneBuildOut*(1+effectNum*1.0/100);
                }
            }
        }
    }
    
    m_info2Label->setString("");
//    if(add>0) {
//        string tmpAdd = "+";
//        tmpAdd = tmpAdd + CC_CMDITOA(add);
//        m_info2Label->setString(tmpAdd);
//    }
//    else {
//        m_info2Label->setString("");
//    }
    m_info1Label->setString(CC_CMDITOA(outputPreHour+add));
    
    m_info2Label->setPositionX(m_info1Label->getPositionX()+m_info1Label->getContentSize().width*m_info1Label->getOriginScaleX());
    m_info3Label->setPositionX(m_info2Label->getPositionX()+m_info2Label->getContentSize().width*m_info2Label->getOriginScaleX());
    
    
    
//    if (m_type == -1) {
//        m_nameLabel->setString(_lang("102172").c_str());
//        auto& info = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
//        int machingNum = 1+CCCommonUtils::getEffectValueByNum(55);
//        string tmpMsg = _lang("102141")+": "+CC_ITOA(machingNum);//info.para[2]
//        string incomMsg = _lang("102149") + ": " + CC_CMDITOA(ArmyController::getInstance()->getTotalArmy());
//        m_incomeLabel->setString(incomMsg);
//        m_numLabel->setString(tmpMsg.c_str());
//        m_stroageLabel->setString("");
//        m_info3Node->setVisible(false);
//        m_info4Node->setVisible(false);
//    }
//    else {
//        if (m_type == Wood) {
//            m_nameLabel->setString(_lang("103023").c_str());
//            
//            curNum = GlobalData::shared()->resourceInfo.lWood;
//            curStorage = GlobalData::shared()->resourceInfo.lStorageWood;
//            outputPreHour = FunBuildController::getInstance()->WoodOutPreSed*3600;
//            m_info3Node->setVisible(false);
//            m_info4Node->setVisible(false);
//        }
//        else if (m_type == Stone) {
//            m_nameLabel->setString(_lang("103024").c_str());
//            
//            curNum = GlobalData::shared()->resourceInfo.lStone;
//            curStorage = GlobalData::shared()->resourceInfo.lStorageStone;
//            outputPreHour = FunBuildController::getInstance()->StoneOutPreSed*3600;
//            m_info3Node->setVisible(false);
//            m_info4Node->setVisible(false);
//        }
//        else if (m_type == Iron) {
//            m_nameLabel->setString(_lang("103025").c_str());
//            
//            curNum = GlobalData::shared()->resourceInfo.lIron;
//            curStorage = GlobalData::shared()->resourceInfo.lStorageIron;
//            outputPreHour = FunBuildController::getInstance()->IronOutPreSed*3600;
//            m_info3Node->setVisible(false);
//            m_info4Node->setVisible(false);
//        }
//        else if (m_type == Food) {
//            m_nameLabel->setString(_lang("103022").c_str());
//            
//            curNum = GlobalData::shared()->resourceInfo.lFood;
//            curStorage = GlobalData::shared()->resourceInfo.lStorageFood;
//            outputPreHour = FunBuildController::getInstance()->FoodOutPreSed*3600;
//            string tmpInfo = _lang("102125") + ": ";
//            m_capLabel->setString(tmpInfo.c_str());
//            m_capNumLabel->setString(CC_CMDITOA(-ArmyController::getInstance()->getTotalUpKeep()));
//            int tmpX = m_capLabel->getContentSize().width*m_capLabel->getOriginScaleX()+m_capLabel->getPositionX();
//            m_capNumLabel->setPositionX(tmpX);
//            
//            if(m_curNum>=0 && m_curNum>curNum) {
//                m_numLabel->stopAllActions();
//                CCActionInterval * tinto1 = CCTintTo ::create(0.3, 255, 0, 0);
//                CCActionInterval * tinto2 = CCTintTo ::create(0.3, 238, 220, 183);
//                auto sque = CCSequence::create(tinto1,tinto2,NULL);
//                m_numLabel->runAction(sque);
//            }
//            m_curNum = curNum;
//            
//            m_info4Node->setPosition(m_info3Node->getPosition());
//            m_info3Node->setVisible(false);
//        }
//        else if (m_type == Silver) {
//            m_nameLabel->setString(_lang("103026").c_str());
//            
//            curNum = GlobalData::shared()->resourceInfo.lMoney;
//            curStorage = GlobalData::shared()->resourceInfo.lStorageMoney;
//            outputPreHour = FunBuildController::getInstance()->SliverOutPreSed*3600;
//        }
//        string tmp = _lang("102124") + ": ";
//        tmp = tmp + CC_CMDITOA(outputPreHour);
//        m_incomeLabel->setString(tmp.c_str());
//        tmp = _lang("102133") + ": " + CC_CMDITOA(curNum);
//        m_numLabel->setString(tmp.c_str());
//    }
}

bool ResInfoCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_touchNode && isTouchInside(m_touchNode,pTouch) && isTouchInside(m_mianNode,pTouch)) {
//        m_moreBtn->onTouchBegan(pTouch, pEvent);
        m_startY = pTouch->getLocation().y;
        return true;
    }
    return false;
}

void ResInfoCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    int endY = pTouch->getLocation().y;
    if (abs(endY-m_startY) >= 20) {
        return ;
    }
    if(m_lockNode->isVisible()) {
        return;
    }
//    m_moreBtn->onTouchEnded(pTouch, pEvent);
    PopupViewController::getInstance()->addPopupInView(MainCityMoreView::create(m_buildId, m_type));
}

SEL_CCControlHandler ResInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMoreBtn", ResInfoCell::onClickMoreBtn);
    return NULL;
}

void ResInfoCell::onClickMoreBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_lockNode->isVisible()) {
        return;
    }
    PopupViewController::getInstance()->addPopupInView(MainCityMoreView::create(m_buildId, m_type));
}

bool ResInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreBtn", CCControlButton*, this->m_moreBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_incomeLabel", CCLabelIF*, m_incomeLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stroageLabel", CCLabelIF*, m_stroageLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info1Node", CCNode*, m_info1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info2Node", CCNode*, m_info2Node);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info3Node", CCNode*, m_info3Node);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info4Node", CCNode*, m_info4Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mianNode", CCNode*, m_mianNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_capLabel", CCLabelIF*, m_capLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_capNumLabel", CCLabelIF*, m_capNumLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info1Label", CCLabelIF*, m_info1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info2Label", CCLabelIF*, m_info2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info3Label", CCLabelIF*, m_info3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info4Label", CCLabelIF*, m_info4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info5Label", CCLabelIF*, m_info5Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info1TitleLabel", CCLabelIF*, m_info1TitleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info2TitleLabel", CCLabelIF*, m_info2TitleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockNameLabel", CCLabelIF*, m_lockNameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockInfoLabel", CCLabelIF*, m_lockInfoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockNode", CCNode*, m_lockNode);
    return false;
}