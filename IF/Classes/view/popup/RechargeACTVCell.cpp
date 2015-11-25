//
//  RechargeACTVCell.cpp
//  IF
//
//  Created by fubin on 14-8-28.
//
//

#include "RechargeACTVCell.h"
#include "SceneController.h"
#include "ImperialScene.h"
#include "QuestInfo.h"
#include "QuestController.h"
#include "YesNoDialog.h"
#include "QueueController.h"
#include "FunBuildController.h"
#include "GoldExchangeSaleView.h"
#include "ToolController.h"
#include "DataRecordCommand.h"
#include "ActivityController.h"
#include "GoldExchangeAdvertisingView.h"
#include "PopupViewController.h"
#include "PortActView.h"
#include "GuideController.h"
#include "LuaController.h"
#include "BaoZangView.h"
#include "NewPlayerQianDaoView.h"


using namespace cocos2d;

RechargeACTVCell::~RechargeACTVCell(){
    if(m_openType==14){
        ActivityController::getInstance()->enterGameOpenView(1);
    }else{
//        ActivityController::getInstance()->enterGameOpenView(0);
    }
}

RechargeACTVCell* RechargeACTVCell::create(int opentype)
{
    RechargeACTVCell *pRet = new RechargeACTVCell(opentype);
    if (pRet && pRet->initRechargeACTVCell())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
RechargeACTVCell* RechargeACTVCell::createMonthCard(int opentype){
    RechargeACTVCell *pRet = new RechargeACTVCell(opentype);
    if (pRet && pRet->initMonthCard()){
        pRet->autorelease();
    }else{
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
RechargeACTVCell* RechargeACTVCell::createDailyRwd(int opentype){
    RechargeACTVCell *pRet = new RechargeACTVCell(opentype);
    if (pRet && pRet->initDailyRwd()){
        pRet->autorelease();
    }else{
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
RechargeACTVCell* RechargeACTVCell::createDailyGuideRwd(int opentype){
    RechargeACTVCell *pRet = new RechargeACTVCell(opentype);
    if (pRet && pRet->initDailyGuideRwd()){
        pRet->autorelease();
    }else{
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
bool RechargeACTVCell::initDailyGuideRwd(){
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        this->setScale(1.8f);
//    }
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->setModelLayerOpacity(180);
    this->setContentSize(winSize);
    
    NewPlayerQianDaoPopView* dailyRwd = NewPlayerQianDaoPopView::create();
    dailyRwd->setPosition(ccp(0, 0));
    this->addChild(dailyRwd);
    return true;
}
bool RechargeACTVCell::initDailyRwd(){
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        this->setScale(1.8f);
//    }
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->setModelLayerOpacity(180);
    this->setContentSize(winSize);

    DailyRwdView* dailyRwd = DailyRwdView::create(1);
    dailyRwd->setPosition(ccp(0, 0));
    this->addChild(dailyRwd);
    return true;
}
bool RechargeACTVCell::initMonthCard(){
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->setScale(1.8f);
    }
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->setModelLayerOpacity(180);
    this->setContentSize(winSize);
    int extH = getExtendHeight();
    
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
    });
    
    GoldExchangeItem* dataItem = NULL;
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        if(!it->second)
            continue;
        if(it->second->type=="5"){
            dataItem = it->second;
            break;
        }
    }
    
    MonthCardPayCell *cell = MonthCardPayCell::create(dataItem);
    cell->setAnchorPoint(ccp(0.5, 0.5));
    cell->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(cell);

     return true;
}
bool RechargeACTVCell::initRechargeACTVCell()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->setScale(1.8f);
    }
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->setModelLayerOpacity(180);
    this->setContentSize(winSize);
    int extH = getExtendHeight();
    GoldExchangeItem* dataItemNormal=NULL;
    GoldExchangeItem* dataItem=NULL;
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        if(!it->second)
            continue;
        if(it->second->type=="5"){
        }else{
            if(it->second->type == "3"){
                if(it->second->bought||it->second->popup_image=="close"){
                    continue;
                }
                if(it->second->end>GlobalData::shared()->getWorldTime()){
                    if(dataItem==NULL ||dataItem->popup<it->second->popup){
                        dataItem = it->second;
                    }
                }
            }else if(it->second->type == "1"){
                if(it->second->bought||it->second->popup_image=="hide" || it->second->popup_image.empty()){
                    continue;
                }
                if(it->second->end>GlobalData::shared()->getWorldTime()){
                    if(LuaController::getInstance()->showNoramlIcon(it->second->popup_image)==true){
                        if(dataItemNormal==NULL ||dataItemNormal->popup<it->second->popup){
                            dataItemNormal = it->second;
                        }
                    }
                }
            }
        }
    }
    if (dataItem) {
        if(dataItem->popup_image =="month"){
        }else{
            CCLoadSprite::doResourceByCommonIndex(7, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(7, false);
            });
            if(dataItem->popup_image=="new_recharge"){//首充
                GoldExchangeFirstPayCell* cell =  GoldExchangeFirstPayCell::create(dataItem, 1);
                cell->ignoreAnchorPointForPosition(false);
                cell->setAnchorPoint(ccp(0.5, 0.5));
                cell->setPosition(ccp(winSize.width/2, winSize.height/2));
                this->addChild(cell);
            }else{
                if(LuaController::getInstance()->checkLuaValid(dataItem->popup_image,dataItem->md5,dataItem->type)){
                    LuaController::getInstance()->showExchangeSale(this,dataItem);
                }else{
                    GoldExchangeSaleView* cell =  GoldExchangeSaleView::create(dataItem, 1);
                    cell->ignoreAnchorPointForPosition(false);
                    cell->setAnchorPoint(ccp(0.5, 0.5));
                    cell->setPosition(ccp(winSize.width/2, winSize.height/2));
                    this->addChild(cell);
                }
            }
        }
    }else if(dataItemNormal){
        if(LuaController::getInstance()->checkLuaValid(dataItemNormal->popup_image,dataItemNormal->md5,dataItemNormal->type)){
            LuaController::getInstance()->showExchangeSale(this,dataItemNormal);
        }else{
            GoldExchangeSaleView* cell =  GoldExchangeSaleView::create(dataItemNormal, 1);
            cell->ignoreAnchorPointForPosition(false); //ljf
            cell->setAnchorPoint(ccp(0.5, 0.5));
            cell->setPosition(ccp(winSize.width/2, winSize.height/2));
            this->addChild(cell);
        }
    }
    
    return true;
}

void RechargeACTVCell::onEnter() {
    CCNode::onEnter();
}
void RechargeACTVCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler RechargeACTVCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool RechargeACTVCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    return false;
}

/////// ActivityBox
ActivityBox* ActivityBox::create(int type)
{
    ActivityBox *pRet = new ActivityBox(type);
    if (pRet && pRet->initActivityBox())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool ActivityBox::initActivityBox()
{
//    if (GlobalData::shared()->goldExchangeList.find("9010") == GlobalData::shared()->goldExchangeList.end()) {
//        return false;
//    }
    m_dataItem = NULL;
    setData();
    if(!m_dataItem){
        return false;
    }
    if(m_dataItem->type.compare("1")==0 && LuaController::getInstance()->showNoramlIcon(m_dataItem->popup_image)==false){
        LuaController::getInstance()->checkLuaValid(m_dataItem->popup_image,m_dataItem->md5,m_dataItem->type);
        return false;
    }
    
    string popImg = m_dataItem->popup_image;
    if (LuaController::getInstance()->checkLuaValid(popImg,m_dataItem->md5,m_dataItem->type)) {
        std::string info = m_dataItem->toString();
        CCNode* a = CCNode::create();
        this->addChild(a);
        LuaController::getInstance()->createExchangeIcon(a, info);
    }else{
    //    m_dataItem = GlobalData::shared()->goldExchangeList["9010"];
        if(popImg=="newyear"){
            CCBLoadFile("ActivitiesNewYear",this,this);
        }else if (popImg=="christmas"){
            CCBLoadFile("ActivitiesChristmas",this,this);
        }else if(popImg=="silent"){
            CCBLoadFile("ActivitiesChristmasEve",this,this);
        }else if(popImg=="1"||popImg=="new_recharge"){
            CCBLoadFile("ActivitiesCoin",this,this);
        }else if(popImg=="30"){
            CCBLoadFile("ActivityEXPBook",this,this);
        }else if(popImg=="thanksgiving"){
            CCNode* m_partAnimNode = CCNode::create();
            CCBLoadFile("ActivityBarbecue",this,this);
            this->addChild(m_partAnimNode);
            auto particle = ParticleController::createParticle("UiFireBarbecue_1");
            auto particle1 = ParticleController::createParticle("UiFireBarbecue_2");
            auto particle2 = ParticleController::createParticle("UiFireBarbecue_3");
            m_partAnimNode->addChild(particle);
            m_partAnimNode->addChild(particle1);
            m_partAnimNode->addChild(particle2);
            m_partAnimNode->setPositionY(-25);
        }else if(popImg=="winter"){
            CCNode* m_partAnimNode = CCNode::create();
            CCBLoadFile("ActivitiesSnowflake",this,this);
            this->addChild(m_partAnimNode);
            auto particle1 = ParticleController::createParticle("Snowflake_1");
            auto particle2 = ParticleController::createParticle("Snowflake_2");
            auto particle3 = ParticleController::createParticle("Snowflake_3");
            m_partAnimNode->addChild(particle1);
            m_partAnimNode->addChild(particle2);
            m_partAnimNode->addChild(particle3);
            
        }else if(popImg == "throne" || popImg == "equip" || popImg =="lover" || popImg == "chunjie"){
            std::string tmp = m_dataItem->popup_image;
            char aa = tmp.at(0);
            aa = std::toupper(aa);
            std::string subStr = aa + tmp.substr(1);
            subStr = "Activities"+subStr;
            CCBLoadFile(subStr.c_str(), this, this, false);
            if(m_dataItem->popup_image =="lover"){
                initLoverParticle();
            }else if(m_dataItem->popup_image == "chunjie" ){
                initChunjieParticle();
            }
        }else{
            CCBLoadFile("ActivityBox",this,this);
        }
        
        if(popImg == "month"){
        }else{
            onEnterFrame(0);
            this->schedule(schedule_selector(ActivityBox::onEnterFrame));
        }
    }
    return true;
}
void ActivityBox::setData(){
    m_dataItem = NULL;
    GoldExchangeItem *normalItem = NULL;
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        if(!it->second)
            continue;
        if(it->second->type=="5"){
        }else{
            if(it->second->type == "3"){
                if(it->second->bought||it->second->popup_image=="close"){
                    continue;
                }
                if(it->second->end>GlobalData::shared()->getWorldTime()){
                    if(m_dataItem&&m_dataItem->popup<it->second->popup){
                        m_dataItem =it->second;
                    }else if(m_dataItem==NULL){
                        m_dataItem =it->second;
                    }
                    
                }
            }else if(it->second->type == "1"){
                //if(it->second->bought|| it->second->popup_image=="hide" || it->second->popup_image.empty()){ //d by ljf
                if(it->second->bought|| it->second->popup_image=="hide" || it->second->popup_image.empty() ){
                    continue;
                }
                if(it->second->end>GlobalData::shared()->getWorldTime()){
                    if(normalItem==NULL ||normalItem->popup<it->second->popup){
                        normalItem = it->second;
                    }
                }
            }
        }
    }
    if(m_dataItem == NULL){
        m_dataItem = normalItem;
    }
    if(m_dataItem==NULL){
        if (this->getParent()) {
            this->getParent()->setVisible(false);
        }
    }
}
void ActivityBox::onEnter() {
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityBox::setData), "GoldExchange.Change", NULL);
}
void ActivityBox::onExit() {
    setTouchEnabled(false);
  //      CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GoldExchange.Change");
    CCNode::onExit();
}

SEL_CCControlHandler ActivityBox::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool ActivityBox::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentNode", CCNode*, this->m_contentNode);
    return false;
}

void ActivityBox::onEnterFrame(float dt)
{
    if(m_dataItem==NULL){
        if (this->getParent()) {
            this->getParent()->setVisible(false);
        }
        return;
    }
    int curTime = GlobalData::shared()->getWorldTime();
    int lastTime = m_dataItem->end - curTime;
    if(m_dataItem->time>0){
        int count =(m_dataItem->end - curTime)/(m_dataItem->time*3600);
        lastTime=m_dataItem->end-count*(m_dataItem->time*3600)-curTime;
    }else{
        lastTime = m_dataItem->end - curTime;
    }
    m_timeLabel->setString(CC_SECTOA(lastTime));
    if ((m_dataItem->end - curTime)<=0||m_dataItem->bought) {
        if (this->getParent()) {
            this->getParent()->setVisible(false);
        }
//        ToolController::getInstance()->m_lastTime += 24*3600;
//        ToolController::getInstance()->m_canBuy = 1;
    }else{
        if (this->getParent()) {
            this->getParent()->setVisible(true);
        }
    }
    
//    if(ToolController::getInstance()->m_canBuy==1) {
//        if (m_dataItem->end<curTime) {
//            if (this->getParent()) {
//                this->getParent()->setVisible(false);
//            }
//        }
//        else {
//            if (this->getParent()) {
//                this->getParent()->setVisible(true);
//            }
//        }
//    }
//    else {
//        if (this->getParent()) {
//            this->getParent()->setVisible(false);
//        }
//    }
}

bool ActivityBox::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return false;
}

void ActivityBox::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void ActivityBox::initLoverParticle(){
    auto particle1 = ParticleController::createParticle("Rose_1");
    addChild(particle1, -2);
    //    particle1->setPositionY(10);
    auto particle2 = CCParticleSystemQuad::create("particleImg/Rose_2.plist");
    addChild(particle2,-1);
    particle2->setPositionY(-43);
    auto particle3 = ParticleController::createParticle("Rose_3");
    addChild(particle3);
}
void ActivityBox::initChunjieParticle(){
    auto particle1 = CCParticleSystemQuad::create("particleImg/Firecrackers_2.plist");
    addChild(particle1);
    particle1->setPosition(ccp(45,33));
    auto particle2 = CCParticleSystemQuad::create("particleImg/Firecrackers_1.plist");
    addChild(particle2);
    particle2->setPosition(ccp(45,8));
    auto particle3 = ParticleController::createParticle("Firecrackers_3");
    addChild(particle3);
    particle3->setPosition(ccp(45,-18));
    auto particle4 = ParticleController::createParticle("Firecrackers_4");
    addChild(particle4);
    particle4->setPosition(ccp(45,-18));
    auto particle5 = ParticleController::createParticle("Firecrackers_5");
    addChild(particle5);
    particle5->setPosition(ccp(45,-18));
    auto particle6 = ParticleController::createParticle("Firecrackers_6");
    addChild(particle6);
    particle6->setPosition(ccp(45,-18));
    
    
    auto particle11 = CCParticleSystemQuad::create("particleImg/Firecrackers_2.plist");
    addChild(particle11);
    particle11->setPosition(ccp(-45,33));
    auto particle12 = CCParticleSystemQuad::create("particleImg/Firecrackers_1.plist");
    addChild(particle12);
    particle12->setPosition(ccp(-45,8));
    auto particle13 = ParticleController::createParticle("Firecrackers_3");
    addChild(particle13);
    particle13->setPosition(ccp(-45,-18));
    auto particle14 = ParticleController::createParticle("Firecrackers_4");
    addChild(particle14);
    particle14->setPosition(ccp(-45,-18));
    auto particle15 = ParticleController::createParticle("Firecrackers_5");
    addChild(particle15);
    particle15->setPosition(ccp(-45,-18));
    auto particle16 = ParticleController::createParticle("Firecrackers_6");
    addChild(particle16);
    particle16->setPosition(ccp(-45,-18));
}