//
//  DragonScene.cpp
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#include "DragonBuild.h"
#include "DragonScene.h"
#include "ParticleController.h"
#include "SoundController.h"
#include "FunBuild.h"
#include "DragonBuildingController.h"
#include "FunBuildInfo.h"
#include "PopupViewController.h"
#include "YesNoDialog.h"
#include "GuideController.h"
#include "AppLibHelper.h"
#include "UserBindCommand.h"
#include "UIComponent.h"
#include "QueueController.h"
#include "WorldController.h"
#include "ArmyController.h"
#include "fireandcomman.h"
#include "QuestController.h"
#include "HowToPlayView.h"
#include "HelpshiftCocos2dx.h"
#include "NetController.h"
#include "../../view/guiExt/md5.h"
#include "ToolController.h"
#include "RechargeACTVCell.h"
#include "ScienceController.h"
#include "ActivityController.h"
#include "cocos2d.h"
#include "VipUtil.h"
#include "Utf8Utils.h"
#include "UserUpgradeView.h"
#include "PayController.h"
#include "../../Ext/CCDevice.h"
#include "CCMathUtils.h"

#include "SceneController.h"
#include "DragonBuildingController.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

using namespace cocos2d;

static DragonScene *_instance = NULL;
DragonScene* DragonScene::instance() {
    return _instance;
}
void DragonScene::leaveDragonScene()
{
    SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
}

bool DragonScene::init()
{
    unsigned long ulc = clock();
    if ( !CCLayer::init() )
    {
        return false;
    }
    _instance = this;
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("DragonScene", "init");

    m_singleTouchState = false;
    m_exit = false;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    int sumWidth = 1020+1020+1020+505;
    int sumHight = 1020+1020;
    
    m_touchLayer = CCLayer::create();
    m_nameLayer = CCLabelBatchNode::create();//(getNBFont(NB_FONT_Bold));
    m_popLayer = CCLayer::create();
    m_signLayer = CCLayer::create();
    m_funLayer = CCLayer::create();
    m_buildBtnsView = DragonBuildBtnsView::create();
    m_buildBtnsView->setVisible(false);
    m_funLayer->addChild(m_buildBtnsView);
    
    m_touchLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_nameLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_popLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_signLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_funLayer->setContentSize(CCSize(sumWidth, sumHight));
    this->addChild(m_touchLayer);
    
    m_sunNode = CCNode::create();//太阳光节点
    m_sunNode->setPosition(CCDirector::sharedDirector()->getWinSize().width + 50, CCDirector::sharedDirector()->getWinSize().height - 50);
    this->addChild(m_sunNode);
    
    m_waterLayer = CCLayer::create();
    m_touchLayer->addChild(m_waterLayer, 91);
    
    auto node = CCBLoadFile("ImperialCities_2",m_touchLayer,this);
    node->setZOrder(90);
    
    for (int i=0; i<BUILD_COUNT; i++) {
        int od = m_nodeBuildings[i]->getOrderOfArrival();
        m_nodeBuildings[i]->setZOrder(od);
    }
    
    m_touchLayer->addChild(m_nameLayer,10004);
    m_touchLayer->addChild(m_signLayer,10006);
    m_touchLayer->addChild(m_popLayer,10008);
    m_touchLayer->addChild(m_funLayer,10015);
    
    m_flyArrow = CCLoadSprite::createSprite("UI_hand.png");
    m_flyArrow->setAnchorPoint(ccp(0, 0.5));
    m_flyArrow->setVisible(false);
    m_touchLayer->addChild(m_flyArrow,10012);
    
    m_tileGlowNode = CCNode::create();
    m_touchLayer->addChild(m_tileGlowNode,900);
    
    m_viewPort = HFViewport::create();
    m_viewPort->setViewPortTarget(m_touchLayer);
    m_viewPort->setSceneSize(sumWidth, sumHight);
    m_viewPort->setTouchDelegate(this);
    m_viewPort->setWorldBound(CCRect(0,0, sumWidth, sumHight));
    
    m_touchLayer->setScale(DragonBuildingController::getInstance()->oldScale);
    
    float maxZoomScale = 1.5*1.3;
    float minZoomScale = m_viewPort->getMinZoom();
    float curZoomScale = 0.82;
    TargetPlatform target = CCApplication::sharedApplication()->getTargetPlatform();
    if(target == kTargetIpad || CCCommonUtils::isAdriodPad()){
        maxZoomScale = 2.5;
        minZoomScale = 1.6;
        curZoomScale = 1.5;
    }
    m_viewPort->setZoomLimitationMax(maxZoomScale);
    m_viewPort->setZoomLimitationMin(minZoomScale);
    addChild(m_viewPort);
    m_touchLayer->setScale(curZoomScale);
    
    m_isLogin = false;
    if (DragonBuildingController::getInstance()->oldx == 99999 || (GlobalData::shared()->playerInfo.level==1 && GlobalData::shared()->playerInfo.exp==0)) {
        m_isLogin = true;
        m_viewPort->updatePosition(ccp(-1050, (winSize.height-sumHight)));
    }
    else{
        m_viewPort->updatePosition(ccp(DragonBuildingController::getInstance()->oldx, DragonBuildingController::getInstance()->oldy));
    }
    
    setCleanFunction([](){
        CCLoadSprite::doResourceByImperialIndex(22, false);
        CCLoadSprite::doResourceByImperialIndex(28, false);
    });
    
    return true;
}

void DragonScene::buildingCallBack(CCObject* params)
{
    if (m_exit) {
        return;
    }
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    auto resSp = CCLoadSprite::loadResource("pic400000_2.png");//dsg_house_1
    resSp->getTexture()->setAntiAliasTexParameters();
    for (int i = 0; i < BUILD_COUNT; ++i)
    {
        auto bdBatch = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_buildBatchMap[i] = bdBatch;
        bdBatch->setTag(1000 + i * 100);
        bdBatch->setPosition(m_nodeBuildings[i]->getPosition());
        m_touchLayer->addChild(bdBatch);
    }
    for (int i = 0; i < BUILD_COUNT; ++i) {
        m_buildBatchMap[i]->setZOrder(1000 + i * 100);
        // to do
    }
    onUpdateInfo();
    m_buildingInitState = true;
    onEnterFrame(0);
    onOpenNewBuild(NULL);
    UIComponent::getInstance()->updateBuildState(true);
    
    if(!DragonBuildingController::getInstance()->isFirst)
    {
        m_curBuildPosx = 1010;
        m_curBuildPosy = 1256;
        float dt = 0.35;
        onMoveToPos(m_curBuildPosx, m_curBuildPosy, TYPE_POS_MID, dt, 0.8, true);
        DragonBuildingController::getInstance()->isFirst=true;
        scheduleOnce(schedule_selector(DragonScene::startGuide), 0.5f);
    }
    else
    {
        startGuide(0);
    }
}

void DragonScene::startGuide(float _time)
{
    setUnMoveScence(false);
    if(WorldController::getInstance()->selfPoint.x < 0){
        return;
    }
    GuideController::share()->start();
}

void DragonScene::onEnter()
{
    CCLayer::onEnter();
    
    CCLoadSprite::doLoadResourceAsync(IMPERIAL_PATH, CCCallFuncO::create(this, callfuncO_selector(DragonScene::buildingCallBack), NULL), 2+10*GlobalData::shared()->contryResType);
    
    UIComponent::getInstance()->onCreateCropSceneUI();
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DragonScene::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onOpenNewBuild), MSG_FUNBUILD_OPEN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onGoBackOldPos), MSG_MAINSCENCE_GOBACK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onSaveCurPos), MSG_MAINSCENCE_SAVEPOS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onRemoveTmpBuild), MSG_MAINSCENCE_REMOVE_TMPBUILD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::unLockTile), MSG_UNLOCK_TILE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::moveMapToPosition), MSG_MOVE_TO_POSITION, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::guideEnd), GUIDE_END, NULL);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onPowerADD), MSG_SCIENCE_POWER_ADD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::checkTileGlow), QUEST_STATE_UPDATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::showHarvestEffect), MSG_SHANGE_SCENCE_SHOW_HARVEST, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onFinishMoveBuild), MSG_FINISH_MOVE_BUILD, NULL);
    
    /////////建造建筑相关的消息
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onGetInitDirChange), DS_INIT_DIRECT_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onGetBuildInitEnd), DS_FUN_BUILD_INIT_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onGetBuildChangeEnd), DS_FUN_BUILD_CHANGE_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onGetBuildUpingEndEnd), DS_FUN_BUILD_UPING_END_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onGetBuildCreateErr), DS_CREATE_BUILD_HAS_ERROR, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onGetBuildUpingErrEnd), DS_FUN_BUILD_UPING_ERROR_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onGetBuildNormalErrEnd), DS_FUN_BUILD_NORMAL_ERROR_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onGetBuildCancelOthEnd), DS_FUN_BUILD_CANCEL_OTH_END, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onGetBuildCanShowState), DS_FUN_BUILD_CANSHOWSTATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onGetBuildSetTime), DS_BUILD_SETTIME, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonScene::onGetBuildNormalIng), DS_FUN_BUILD_NORMAL_ING, NULL);
    //////////
    
    string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
    if (GlobalData::shared()->playerInfo.level==1 && GlobalData::shared()->playerInfo.exp==0 && (gFake==""||gFake=="start_1")) {
    }else{
        SoundController::sharedSound()->playBGMusic(Music_M_city_1);
    }
    
    m_isDay = true;//默认白天
    checkDayTime(true);//判断是否是白天
    
    if(!m_praticle){
        if (GlobalData::shared()->contryResType == 1) {
            
        }else if (GlobalData::shared()->contryResType == 2) {
            m_waterType = 1;
            m_dropNode = CCNode::create();
            m_dropNode->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2,CCDirector::sharedDirector()->getWinSize().height/2));
            this->addChild(m_dropNode);
            
            int rIdx = CCMathUtils::getRandomInt(0, 9);
            if (rIdx < 5) {
                showRain();
            }
        }
        else {
            m_praticle = ParticleController::createFightingParticle("snow"
                                                                    , 60
                                                                    , ccp(CCDirector::sharedDirector()->getWinSize().width / 2 - 50
                                                                          , CCDirector::sharedDirector()->getWinSize().height + 10)
                                                                    );
            this->addChild(m_praticle);
        }
    }
    
    int userLv = GlobalData::shared()->lordInfo.levelUp;
    if(userLv<GlobalData::shared()->playerInfo.level){
        CCDictionary* tmp = CCDictionary::create();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAYER_LEVELUP);
    }
    this->scheduleOnce(schedule_selector(DragonScene::scheduleHarvestEffect), 0.45);
}

void DragonScene::showRain()
{
    m_isRain = true;
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    m_praticle = ParticleController::createFightingParticle("rain", 60, ccp(winSize.width / 2 - 50, winSize.height + 10));
    this->addChild(m_praticle);
    
    for (int i=1; i<=3; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("Drop_%d",i)->getCString());
        m_dropNode->addChild(particle);
    }
    this->scheduleOnce(schedule_selector(DragonScene::clearDrop), 65);
}

void DragonScene::checkDayTime(bool isInit) {
    struct tm tm;
    char s[100];
    time_t timer = time(0);
    tm = *localtime(&timer);
    strftime(s, sizeof(s), "%H", &tm);
    int hh = atoi(s);
    if (hh >= 18 || hh < 6) {
        if (m_isDay) {
            m_sunNode->removeAllChildren();
            this->unschedule(schedule_selector(DragonScene::setSunAngle));
        }
        m_isDay = false;
    }else{
        if (m_sysTime>60) {
            if (!m_isDay) {
                m_isDay = true;
                showSunshine();
            }
        }
        m_isDay = true;
    }
}

void DragonScene::showSunshine()
{
    if (!m_isDay || m_isRain) {
        return;
    }
    m_sunNode->removeAllChildren();
    auto sunparticle1 = ParticleController::createParticle("OpticalFlares_4");
    auto sunparticle2 = ParticleController::createParticle("OpticalFlares_1");
    auto sunparticle3 = ParticleController::createParticle("OpticalFlares_3");
    auto sunparticle4 = ParticleController::createParticle("OpticalFlares_5");
    m_sunNode->addChild(sunparticle1);
    m_sunNode->addChild(sunparticle2);
    m_sunNode->addChild(sunparticle3);
    m_sunNode->addChild(sunparticle4);
    this->unschedule(schedule_selector(DragonScene::setSunAngle));
    this->schedule(schedule_selector(DragonScene::setSunAngle), 1.0/24);
}

void DragonScene::setSunAngle(float _time) {
    if (!m_isDay) {
        return;
    }
    auto cornerPos = onGetSceenPt(3570, 2040);//右上角顶点在屏幕中的位置
    float dx = cornerPos.x - CCDirector::sharedDirector()->getWinSize().width * 0.5;
    float dy = cornerPos.y;
    auto cornerAngle = atan2(dx, dy) * 180 / PI;
    auto maxcornerAngle = atan2(3570 - CCDirector::sharedDirector()->getWinSize().width * 0.5, CCDirector::sharedDirector()->getWinSize().height) * 180 / PI;
    auto mincornerAngle = atan2(CCDirector::sharedDirector()->getWinSize().width * 0.5, 2040) * 180 / PI;
    m_sunNode->setRotation(30 + (cornerAngle - mincornerAngle) / (maxcornerAngle - mincornerAngle) * 40);
}

void DragonScene::clearDrop(float _time){
    m_isRain = false;
    m_dropNode->removeAllChildren();
}

void DragonScene::moveMapToPosition(CCObject* obj){
    auto dic = _dict(obj);
    if(dic){
        float time = dic->valueForKey("time")->floatValue();
        float posx = dic->valueForKey("x")->floatValue();
        float posy = dic->valueForKey("y")->floatValue();
        float scale = dic->valueForKey("scale")->floatValue();
        onMoveToPos(posx, posy, TYPE_POS_MID, time, scale, true);
    }
}

void DragonScene::guideEnd(CCObject* obj)
{
    m_curGuideEnd = true;
    scheduleOnce(schedule_selector(DragonScene::retGuideEnd), 0.5f);
    scheduleOnce(schedule_selector(DragonScene::clearGuideState), 2.0f);
}
void DragonScene::retGuideEnd(float _time)
{
    auto size = CCDirector::sharedDirector()->getWinSize();
    auto tmp2Pt = m_touchLayer->convertToNodeSpace(ccp(size.width/2, size.height/2));
    onMoveToPos(tmp2Pt.x, tmp2Pt.y, TYPE_POS_MID, 0.5, 0.8, true);
}
void DragonScene::clearGuideState(float _time)
{
    m_curGuideEnd = false;
}

void DragonScene::onExit()
{
    m_exit = true;
    if(GlobalData::shared()->isUiInti){
        UIComponent::getInstance()->updateBuildState(false);
        //        UIComponent::getInstance()->onDeleteCropSceneUI();
    }
    
    DragonBuildingController::getInstance()->oldScale = m_touchLayer->getScale();
    DragonBuildingController::getInstance()->oldx = m_touchLayer->getPositionX();
    DragonBuildingController::getInstance()->oldy = m_touchLayer->getPositionY();
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(DragonScene::onEnterFrame), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FUNBUILD_OPEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAINSCENCE_GOBACK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAINSCENCE_SAVEPOS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAINSCENCE_REMOVE_TMPBUILD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UNLOCK_TILE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MOVE_TO_POSITION);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TROOPS_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_END);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_POWER_ADD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, QUEST_STATE_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INIT_ACTIVITY_EVENT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TALK_OPEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHANGE_SCENCE_SHOW_HARVEST);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FINISH_MOVE_BUILD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NEW_TIME_RWD_END);
    
///////清除建造建筑相关的消息
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DS_INIT_DIRECT_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DS_FUN_BUILD_INIT_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DS_FUN_BUILD_CHANGE_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DS_FUN_BUILD_UPING_END_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DS_CREATE_BUILD_HAS_ERROR);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DS_FUN_BUILD_UPING_ERROR_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DS_FUN_BUILD_NORMAL_ERROR_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DS_FUN_BUILD_CANCEL_OTH_END);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DS_FUN_BUILD_CANSHOWSTATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DS_BUILD_SETTIME);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DS_FUN_BUILD_NORMAL_ING);
//////////
    
    if (m_praticle) {
        m_praticle->stopAllActions();
        this->removeChild(m_praticle);
    }
    m_praticle = NULL;
    
    for (int i=BUILD_COUNT-1; i>=0; i--) {
        DragonBuild* build = dynamic_cast<DragonBuild*>(m_nodeBuildings[i]->getChildByTag(1));
        if (build) {
            build->onBuildDelete();
        }
        m_nodeBuildings[i]->removeAllChildren();
        CC_SAFE_RELEASE(m_nodeBuildings[i]);
    }
    m_buildItems.clear();
    
    map<int,CCSpriteBatchNode*>::iterator it;
    for (it=m_buildBatchMap.begin(); it!=m_buildBatchMap.end(); it++) {
        (it->second)->removeAllChildren();
    }
    m_buildBatchMap.clear();
    
    m_nameLayer->removeAllChildren();
    m_popLayer->removeAllChildren();
    m_signLayer->removeAllChildren();
    
    m_buildingInitState = false;
    m_touchLayer->removeAllChildren();
    CCLayer::onExit();
}

void DragonScene::onOpenNewBuild(CCObject* obj)
{
    map<int,FunBuildInfo>::iterator it;
    it = DragonBuildingController::getInstance()->curDragonBuildsInfo->begin();
    for (; it != DragonBuildingController::getInstance()->curDragonBuildsInfo->end(); it++)
    {
        if (m_buildItems.find(it->first) != m_buildItems.end()) {
            m_buildItems[it->first]->onOpenLock();
        }
    }
    
//    refreashTile();
}

void DragonScene::onUpgradeNewBuild(int buildId)
{
    int key = buildId;
    int pos = buildId%1000;
    if (m_buildItems.find(key)!=m_buildItems.end()) {
        m_buildItems.erase(m_buildItems.find(key));
    }
    
    DragonBuild* delbuild = dynamic_cast<DragonBuild*>(m_nodeBuildings[pos]->getChildByTag(1));
    delbuild->onBuildDelete();
    m_nodeBuildings[pos]->removeChild(delbuild);
    
    DragonBuild* build = DragonBuild::create( key , m_nameLayer);
    build->setTag(1);
    m_nodeBuildings[pos]->addChild(build);
    m_nodeBuildings[pos]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
    m_buildItems[key] = build;
    int od = m_nodeBuildings[pos]->getZOrder();
    build->m_key = 1000-od;
    build->setNamePos(m_nodeBuildings[pos]->getPositionX()
                      , m_nodeBuildings[pos]->getPositionY(), m_signLayer, m_popLayer, m_buildBatchMap[pos], od, nullptr);
}

float DragonScene::getTouchLayerScale(){
    return m_touchLayer->getScale();
}

void DragonScene::onMoveToBuild(int itemId, bool st)
{
    if(itemId == 1){
        itemId = 0;
    }
    
    if(m_buildItems.size()<=0)
    {
        return;
    }
    float s = m_touchLayer->getScale();
    float endS = 0.8;
    
//    if(itemId == FUN_BUILD_MAIN){
//        m_touchLayer->setScale(endS);
//    }else{
        endS = 1.0f;
//        if(st && itemId != FUN_BUILD_BARRACK1 && itemId != FUN_BUILD_BARRACK2) {
//            endS = 1.3f;
//        }
//        if (st && (itemId == FUN_BUILD_BARRACK1 || itemId == FUN_BUILD_BARRACK2)) {
//            TargetPlatform target = CCApplication::sharedApplication()->getTargetPlatform();
//            if(target == kTargetIpad || CCCommonUtils::isAdriodPad()){
//                endS = 0.75f;
//            }
//        }
        m_touchLayer->setScale(endS);
//    }
    int id = DragonBuildingController::getInstance()->getMaxLvBuildByType(itemId);
    
    map<int, DragonBuild*>::iterator it = m_buildItems.find(id);
    float f = m_touchLayer->getScale();
    cocos2d::CCPoint anchor = ccp(m_touchLayer->getContentSize().width * m_touchLayer->getAnchorPoint().x,
                                  m_touchLayer->getContentSize().height * m_touchLayer->getAnchorPoint().y);
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    anchor = ccpMult(anchor, (1.0f - f));
    DragonBuild *build = NULL;
    CCNode *node = NULL;
    m_isSave = false;
    if(it != m_buildItems.end()){
        build = m_buildItems[id];
        int buildPosX = m_buildItems[id]->getParent()->getPositionX() + m_buildItems[id]->getContentSize().width / 2 ;
        int buildPosY = m_buildItems[id]->getParent()->getPositionY() + m_buildItems[id]->getContentSize().height / 2;
        m_viewPort->updatePosition(ccp(winSize.width / 2 - buildPosX * f - anchor.x, winSize.height / 2 - buildPosY * f - anchor.y), true);
    }
    if(itemId < BUILD_COUNT - 1){
        node = m_nodeBuildings[itemId];
        build = dynamic_cast<DragonBuild*>(node->getChildByTag(1));
        int buildPosX = m_nodeBuildings[itemId]->getPositionX() + build->mainWidth / 2 ;
        int buildPosY = m_nodeBuildings[itemId]->getPositionY() + build->mainHeight / 2;
        m_viewPort->updatePosition(ccp(winSize.width / 2 - buildPosX * f - anchor.x, winSize.height / 2 - buildPosY * f - anchor.y), true);
    }
    m_touchLayer->setScale(s);
    m_touchLayer->runAction(CCScaleTo::create(1.0, endS));
    if(build){
        build->playShadow();
    }
}

void DragonScene::onMoveToBuildAndPlay(int itemId, bool st)
{
    DragonBuild *build = NULL;
    if(itemId>1000){
        map<int, DragonBuild*>::iterator it = m_buildItems.find(itemId);
        if( it == m_buildItems.end()){
            return;
        }
        m_touchLayer->setScale(m_oldScale);
        m_viewPort->updatePosition(m_touchLayer->getPosition(), false, 0, true);
        onMoveToBuildAndOpen(itemId, TYPE_POS_CLICK, 1.0, true);
        build = m_buildItems[itemId];
        
    }else{
        m_touchLayer->setScale(m_oldScale);
        m_viewPort->updatePosition(m_touchLayer->getPosition(), false, 0, true);
        onMoveToBuildAndOpen(itemId, TYPE_POS_CLICK, 1.0, true);
        CCNode *node = m_nodeBuildings[itemId];
        build = dynamic_cast<DragonBuild*>(node->getChildByTag(1));
    }
    int buildPosX = build->getParent()->getPositionX() + build->mainWidth / 2 ;
    int buildPosY = build->getParent()->getPositionY() + build->mainHeight;
    if (itemId == DBUILDING_HATCHER_ItemId) {
        buildPosY -= build->mainHeight/2;
    }
    m_flyArrow->setPosition(ccp(buildPosX, buildPosY));
    m_flyArrow->setVisible(true);
    CCActionInterval * moveBy = CCMoveBy::create(0.5,ccp(30, 50));
    CCActionInterval * moveRBy = CCMoveBy::create(0.5,ccp(-30, -50));
    CCSequence* fadeAction = CCSequence::create(moveBy,moveRBy,NULL);
    CCActionInterval * fadeForever =CCRepeatForever::create((CCActionInterval* )fadeAction);
    m_flyArrow->runAction(fadeForever);
    
    m_forceMove = st;
    scheduleOnce(schedule_selector(DragonScene::hideFlyArrow), 5.0f);
}
void DragonScene::hideFlyArrow(float _time){
    m_forceMove = false;
    m_flyArrow->stopAllActions();
    m_flyArrow->setVisible(false);
}

void DragonScene::onMoveToBuildAndOpen(int itemId, int type, float dt, bool bound)
{
    float endS = 1.3;
    DragonBuild *build = NULL;
    if(1){
        if (itemId>1000) {
            map<int, DragonBuild*>::iterator it = m_buildItems.find(itemId);
            if (it != m_buildItems.end()) {
                build = m_buildItems[itemId];
            }else {
                return;
            }
        }
        else {
            CCNode *node = m_nodeBuildings[itemId];
            build = dynamic_cast<DragonBuild*>(node->getChildByTag(1));
        }
        
        m_curBuildPosx = build->getParent()->getPositionX() + build->mainWidth/2 ;
        m_curBuildPosy = build->getParent()->getPositionY() + build->mainHeight/2;
        onMoveToPos(m_curBuildPosx, m_curBuildPosy, type, dt, endS, bound);
    }
}

void DragonScene::onMoveToPos(float x, float y, int type, float dt, float scale, bool bound)
{
    float s = m_touchLayer->getScale();
    
    float _wf = 0.31;
    float _hf = 0.55;
    if (type == TYPE_POS_UP) {
        _wf = 0.31;
        _hf = 0.63;
    }
    else if (type == TYPE_POS_MID) {
        _wf = 0.5;
        _hf = 0.5;
    }
    
    float f = scale;
    if (type == TYPE_POS_CLICK) {
        _wf = 0.5;
        _hf = 0.5;
        f = m_touchLayer->getScale();
        if(y<1000 && bound==false) {
            bound = false;
        }
        else {
            bound = true;
        }
    }
    else {
        m_touchLayer->setScale(scale);
    }
    
    if (m_curGuideEnd) {
        f = 0.8;
        m_touchLayer->setScale(0.8);
    }
    
    cocos2d::CCPoint anchor = ccp(m_touchLayer->getContentSize().width * m_touchLayer->getAnchorPoint().x,
                                  m_touchLayer->getContentSize().height * m_touchLayer->getAnchorPoint().y);
    
    anchor = ccpMult(anchor, (1.0f - f));
    m_curBuildPosx = x;
    m_curBuildPosy = y;
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    m_tmpMoveX = winSize.width*_wf-m_curBuildPosx*f-anchor.x;
    m_tmpMoveY = winSize.height*_hf-m_curBuildPosy*f-anchor.y;
    
    if (dt<=0) {
        m_viewPort->updatePosition(ccp(m_tmpMoveX, m_tmpMoveY), false, dt, bound);
    }
    else {
        if(m_viewPort->notMove) {
            return;
        }
        setUnMoveScence(true);
        m_viewPort->updatePosition(ccp(m_tmpMoveX, m_tmpMoveY), true, dt, bound);
        m_touchLayer->setScale(s);
        m_touchLayer->runAction(CCScaleTo::create(dt, f));
        scheduleOnce(schedule_selector(DragonScene::onEnableMoveScence), dt+0.1f);
    }
}

void DragonScene::onEnableMoveScence(float _time)
{
    setUnMoveScence(false);
}

CCPoint DragonScene::onGetSceenPt(float x, float y)
{
    cocos2d::CCPoint anchor = ccp(m_touchLayer->getContentSize().width * m_touchLayer->getAnchorPoint().x,
                                  m_touchLayer->getContentSize().height * m_touchLayer->getAnchorPoint().y);
    float f = m_touchLayer->getScale();
    anchor = ccpMult(anchor, (1.0f - f));
    auto tPt = m_touchLayer->getPosition();
    return ccp(x, y)*f + anchor +tPt;
}

float DragonScene::onGetTouchLayerScale()
{
    return m_touchLayer->getScale();
}

void DragonScene::onMoveToPosDir(float sx, float sy, float ex, float ey, float dt, bool bound)
{
    if(m_viewPort->notMove) {
        return;
    }
    float f = m_touchLayer->getScale();
    cocos2d::CCPoint anchor = ccp(m_touchLayer->getContentSize().width * m_touchLayer->getAnchorPoint().x,
                                  m_touchLayer->getContentSize().height * m_touchLayer->getAnchorPoint().y);
    
    anchor = ccpMult(anchor, (1.0f - f));
    m_curBuildPosx = sx;
    m_curBuildPosy = sy;
    m_tmpMoveX = ex-m_curBuildPosx*f-anchor.x;
    m_tmpMoveY = ey-m_curBuildPosy*f-anchor.y;
    if (dt<=0) {
        m_viewPort->updatePosition(ccp(m_tmpMoveX, m_tmpMoveY), false, dt, bound);
    }
    else {
        setUnMoveScence(true);
        m_viewPort->updatePosition(ccp(m_tmpMoveX, m_tmpMoveY), true, dt, bound);
        scheduleOnce(schedule_selector(DragonScene::onEnableMoveScence), dt+0.1f);
    }
}

void DragonScene::onSaveCurPos(CCObject* obj)
{
    m_isSave = true;
    m_oldScale = m_touchLayer->getScale();
    m_oldPosX = m_touchLayer->getPositionX();
    m_oldPosY = m_touchLayer->getPositionY();
}

void DragonScene::onGoBackOldPos(CCObject* obj)
{
    lastTouchBuildId = -1;
    if (m_isSave) {
        if(m_flyArrow->isVisible() && m_forceMove){
            m_isSave = false;
            return;
        }
        setUnMoveScence(true);
        float s = m_touchLayer->getScale();
        m_touchLayer->setScale(m_oldScale);
        m_viewPort->updatePosition(ccp(m_oldPosX, m_oldPosY), true, 0.15);
        m_touchLayer->setScale(s);
        m_touchLayer->runAction(CCScaleTo::create(0.15, m_oldScale));
        m_isSave = false;
        scheduleOnce(schedule_selector(DragonScene::onEnableMoveScence), 0.15+0.1f);
    }
}

void DragonScene::onRemoveTmpBuild(CCObject* obj)
{
    hideTmpBuild(m_tmpBuildPos);
}

void DragonScene::onSingleTouchBegin(CCTouch* pTouch)
{
    if (!m_canClick) {
        return;
    }
    int key = 99999;
    if (curTouchBuildId > -1) {
        DragonBuild* build = dynamic_cast<DragonBuild*>(m_nodeBuildings[curTouchBuildId]->getChildByTag(1));
        if (build) {
            build->setWhite();
            build->stopShadow();
        }
    }
    curTouchBuildId = -1;
    for (int i=0; i<BUILD_COUNT; i++) {
        DragonBuild* build = dynamic_cast<DragonBuild*>(m_nodeBuildings[i]->getChildByTag(1));
        if(build && build->onTouchBegan(pTouch, NULL))
        {
            if (build->m_key < key) {
                key = build->m_key;
                curTouchBuildId = i;
            }
        }
    }
    
    if(curTouchBuildId == -1 || DragonBuildingController::getInstance()->OpenMoveBuildStat) {
        lastTouchBuildId = -1;
    }
}

void DragonScene::unLockTouch(float _time) {
    m_canClick = true;
}

void DragonScene::onSingleTouchEnd(CCTouch* pTouch)
{
    if (!m_canClick) {
        return;
    }
    m_canClick = false;
    this->scheduleOnce(schedule_selector(DragonScene::unLockTouch), 0.5);
    
    m_buildBtnsView->onHide();
    if (curTouchBuildId > -1) {
        if(lastTouchBuildId != curTouchBuildId)
        {
            lastTouchBuildId = curTouchBuildId;
            DragonBuild* build = dynamic_cast<DragonBuild*>(m_nodeBuildings[curTouchBuildId]->getChildByTag(1));
            if (build) {
                if (m_singleTouchState) {
                    return;
                }
                m_singleTouchState = true;
                build->onTouchEnded(pTouch, NULL);
            }
        }
        else {
            lastTouchBuildId = -1;
        }
    }
    else {
        bool st = true;
        if (DragonBuildingController::getInstance()->OpenMoveBuildStat) {
            st = false;
        }
        
        if(st) {
            auto pt = pTouch->getLocation();
            UIComponent::getInstance()->showClickEff(pt);
        }
    }
    scheduleOnce(schedule_selector(DragonScene::canSingleTouch), 0.2f);
}

void DragonScene::onResetLastBuildId()
{
    lastTouchBuildId = -1;
}

void DragonScene::canSingleTouch(float _time){
    m_singleTouchState = false;
}

//收到消息后相应的方法
void DragonScene::onGetInitDirChange(CCObject* obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    int value = ccint->getValue();
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    if ((*curDragonBuildsInfo).find(value) == (*curDragonBuildsInfo).end()) {
        return;
    }
    onCreateBuild((*curDragonBuildsInfo)[value].itemId);
}
void DragonScene::onGetBuildInitEnd(CCObject* obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    int value = ccint->getValue();
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    if ((*curDragonBuildsInfo).find(value) == (*curDragonBuildsInfo).end()) {
        return;
    }
    onCreateBuild((*curDragonBuildsInfo)[value].itemId);
    showBuildBtns((*curDragonBuildsInfo)[value].itemId);
    if ((*curDragonBuildsInfo)[value].pos<=16) {
        checkTileGlow(NULL);
    }
}
void DragonScene::onGetBuildChangeEnd(CCObject* obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    int value = ccint->getValue();
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    if ((*curDragonBuildsInfo).find(value) == (*curDragonBuildsInfo).end()) {
        return;
    }
    onOpenBuildEnd((*curDragonBuildsInfo)[value].itemId);
    onUpgradeNewBuild((*curDragonBuildsInfo)[value].itemId);
    onResetBuild((*curDragonBuildsInfo)[value].itemId);
}
void DragonScene::onGetBuildUpingEndEnd(CCObject* obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    int value = ccint->getValue();
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    if ((*curDragonBuildsInfo).find(value) == (*curDragonBuildsInfo).end()) {
        return;
    }
    onUpgradeNewBuild((*curDragonBuildsInfo)[value].itemId);
    onResetBuild((*curDragonBuildsInfo)[value].itemId);
}
void DragonScene::onGetBuildCreateErr(CCObject* obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    int key = ccint->getValue();
    int pos = key % 1000;
    if (m_buildItems.find(key) != m_buildItems.end()) {
        m_buildItems.erase(m_buildItems.find(key));
    }
    DragonBuild* delbuild = dynamic_cast<DragonBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
    delbuild->onBuildDelete();
    m_nodeBuildings[pos]->removeChild(delbuild);
    DragonBuild* build = DragonBuild::create(pos, m_nameLayer);
    build->setTag(pos);
    m_nodeBuildings[pos]->addChild(build);
    m_nodeBuildings[pos]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
    int od = m_nodeBuildings[pos]->getZOrder();
    build->setTileBatch(m_nodeBuildings[pos]->getPositionX(), m_nodeBuildings[pos]->getPositionY(), m_buildBatchMap[pos], od);
    build->m_key = 1000 - od;
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    curDragonBuildsInfo->erase(curDragonBuildsInfo->find(key));
}
void DragonScene::onGetBuildUpingErrEnd(CCObject* obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    int key = ccint->getValue();
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    if ((*curDragonBuildsInfo).find(key) == (*curDragonBuildsInfo).end()) {
        return;
    }
    m_buildItems[(*curDragonBuildsInfo)[key].itemId]->setTime(0);
}
void DragonScene::onGetBuildNormalErrEnd(CCObject* obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    int key = ccint->getValue();
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    if ((*curDragonBuildsInfo).find(key) == (*curDragonBuildsInfo).end()) {
        return;
    }
    m_buildItems[(*curDragonBuildsInfo)[key].itemId]->setTime(0, true);
}
void DragonScene::onGetBuildCancelOthEnd(CCObject* obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    int key = ccint->getValue();
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    if ((*curDragonBuildsInfo).find(key) == (*curDragonBuildsInfo).end()) {
        return;
    }
    m_buildItems[(*curDragonBuildsInfo)[key].itemId]->setTime(0, true);
}
void DragonScene::onGetBuildCanShowState(CCObject* obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    int key = ccint->getValue();
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    if ((*curDragonBuildsInfo).find(key) == (*curDragonBuildsInfo).end()) {
        return;
    }
    if (m_buildItems.find((*curDragonBuildsInfo)[key].itemId) == m_buildItems.end()) {
        return;
    }
    m_buildItems[(*curDragonBuildsInfo)[key].itemId]->canShowState();
}
void DragonScene::onGetBuildSetTime(CCObject* obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    int key = ccint->getValue();
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    if ((*curDragonBuildsInfo).find(key) == (*curDragonBuildsInfo).end()) {
        return;
    }
    if (m_buildItems.find((*curDragonBuildsInfo)[key].itemId) == m_buildItems.end()) {
        return;
    }
    auto curTime = GlobalData::shared()->getWorldTime();
    int langTime = (*curDragonBuildsInfo)[key].updateTime - curTime;
    m_buildItems[(*curDragonBuildsInfo)[key].itemId]->setTime(langTime);
}
void DragonScene::onGetBuildNormalIng(CCObject* obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    int key = ccint->getValue();
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    if ((*curDragonBuildsInfo).find(key) == (*curDragonBuildsInfo).end()) {
        return;
    }
    if (m_buildItems.find((*curDragonBuildsInfo)[key].itemId) == m_buildItems.end()) {
        return;
    }
    m_buildItems[(*curDragonBuildsInfo)[key].itemId]->onUpdateUpIcon(0);
}

void DragonScene::onEnterFrame(float dt)
{
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    m_helpCount += 1;
    if(m_helpCount%5==0){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        m_noticeCount = HelpshiftCocos2dx::getNotificationCount(false,NULL);
#else
        m_noticeCount = HelpshiftCocos2dx::getNotificationCountFromRemote(false,NULL);
#endif
    }
    if (!m_buildingInitState) {
        return;
    }
    checkDayTime();//判断是否是白天
    m_sysTime++;
    m_talkTime++;
    if (m_sysTime%2==0) {
        //每隔2秒执行一次
    }
    if (m_sysTime>70 && m_sysTime%80==0) {
        //每隔80秒执行一次
        if (m_sysTime%160==0) {
            int rIdx = CCMathUtils::getRandomInt(0, 9);
            if (rIdx < 5) {
                showRain();
            }else {
                if (m_isDay && !m_isRain) {
                    showSunshine();
                }
            }
        }else {
            if (m_isDay && !m_isRain) {
                showSunshine();
            }
        }
    }
}

void DragonScene::addToMovingLayer(CCNode* node){
    this->m_touchLayer->addChild(node);
}

void DragonScene::stopShadow(int buildId, CCTouch *pTouch, CCEvent *pEvent)
{
    map<int, DragonBuild*>::iterator it;
    for(it=m_buildItems.begin(); it != m_buildItems.end(); ++it)
    {
        if ( it->first == buildId && it->second != NULL) {
            (it->second)->stopShadow();
        }
        else{
        }
    }
}

void DragonScene::onResetBuild(int itemId)
{
    if (m_buildItems.find(itemId) != m_buildItems.end()) {
        m_buildItems[itemId]->setTime(0,true);
        m_buildItems[itemId]->onPlayUpEnd();
    }
}

void DragonScene::onFlyOutPut(int itemId, int output, int forceResType)
{
    if (m_buildItems.find(itemId) != m_buildItems.end())
    {
        if (m_flyArrow->isVisible()) {
            stopArrowClick(0);
        }
        
        DragonBuild *build = m_buildItems[itemId];
        if (forceResType == -1) {
            build->onFlyOutPut(0);
        }
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        int startPosx = build->getParent()->getPositionX() + build->mainWidth/2 ;
        int startPosy = build->getParent()->getPositionY() + build->mainHeight/2;
        auto pt = onGetSceenPt(startPosx, startPosy);
        pt.y = pt.y-winSize.height;
        
        string picStr = "Food";
        auto endPt = ccp(0,0);
        int resType = Wood;
//        if (itemId/1000 == FUN_BUILD_FOOD || forceResType==Food) {
//            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_farm);
//            endPt = ccp(200,-60);//winSize.height
//            picStr = "Food";
//            resType = Food;
//        }else if (itemId/1000 == FUN_BUILD_WOOD || forceResType==Wood) {
//            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_wood);
//            endPt = ccp(90,-40);
//            picStr = "Wood";
//            resType = Wood;
//        }
//        else if (itemId/1000 == FUN_BUILD_IRON || forceResType==Iron) {
//            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_iron);
//            endPt = ccp(320,-40);
//            picStr = "Iron";
//            resType = Iron;
//        }
//        else if (itemId/1000 == FUN_BUILD_STONE || forceResType==Stone) {
//            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_mithril);
//            endPt = ccp(430,-40);
//            picStr = "Stone";
//            resType = Stone;
//        }
        
        srand((unsigned)time(0));
        int tmp = output*10/1000;
        tmp = MAX(tmp, 2);
        tmp = MIN(tmp, 10);
        int idx = 0;
        while (tmp>0) {
            auto flyCell = FlyCell::create(pt, endPt,resType, picStr, 5, UIComponent::getInstance()->m_collectBatch, idx*0.2);
            UIComponent::getInstance()->m_collectNode->addChild(flyCell);
            tmp--;
            idx++;
        }
    }
}

void DragonScene::onUpdateInfo()
{
    map<int,FunBuildInfo>::iterator it;
    it = DragonBuildingController::getInstance()->curDragonBuildsInfo->begin();
    for (; it != DragonBuildingController::getInstance()->curDragonBuildsInfo->end(); it++)
    {
        if (it->first>0 && ((it->second).state != FUN_BUILD_LOCK) && (it->second).pos<BUILD_COUNT)
        {
            if(it->second.level==0) {
                it->second.level=1;
            }
            DragonBuild* build = DragonBuild::create( it->first , m_nameLayer);
            build->setTag(1);
            m_nodeBuildings[(it->second).pos]->addChild(build);
            m_nodeBuildings[(it->second).pos]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
        
            int od = m_nodeBuildings[(it->second).pos]->getZOrder();
            build->setNamePos(m_nodeBuildings[(it->second).pos]->getPositionX()
                              , m_nodeBuildings[(it->second).pos]->getPositionY(), m_signLayer, m_popLayer, m_buildBatchMap[(it->second).pos], od, nullptr);
            m_buildItems[it->first] = build;
            build->m_key = 1000-od;
        }
    }
    
    for (int i = 0; i<BUILD_COUNT; i++) {
        int cnt = m_nodeBuildings[i]->getChildrenCount();
        if (cnt<=0) {
            //加入地块
            DragonBuild* build = DragonBuild::create( i , m_nameLayer);
            build->setTag(1);
            m_nodeBuildings[i]->addChild(build);
            m_nodeBuildings[i]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
            int od = m_nodeBuildings[i]->getZOrder();
            build->setTileBatch(m_nodeBuildings[i]->getPositionX(), m_nodeBuildings[i]->getPositionY(), m_buildBatchMap[i], od);
            build->m_key = 1000-od;
        }
    }
}

int DragonScene::findCanBuildTile(int pos)
{
    if (pos>=0 && pos<BUILD_COUNT) {
        DragonBuild* build = dynamic_cast<DragonBuild*>(m_nodeBuildings[pos]->getChildByTag(1));
        if (build && build->m_buildingKey == pos) {
            return pos;
        }
        else {
            if (pos>16 && pos!=52 && pos!=53 && pos!=54) {
                for (int i=17; i<BUILD_COUNT; i++) {
                    if (i>=52 && i<=54) {
                        continue;
                    }
                    
                    DragonBuild* build = dynamic_cast<DragonBuild*>(m_nodeBuildings[i]->getChildByTag(1));
                    if (build && build->m_buildingKey == i) {
                        auto& tileInfo = DragonBuildingController::getInstance()->m_tileMap[i];
                        if (!(tileInfo.state == FUN_BUILD_LOCK || tileInfo.xmlOpen==1)) {
                            return i;
                        }
                    }
                }
            }
            else {
                for (int i=0; i<=16; i++) {
                    DragonBuild* build = dynamic_cast<DragonBuild*>(m_nodeBuildings[i]->getChildByTag(1));
                    if (build && build->m_buildingKey == i) {
                        auto& tileInfo = DragonBuildingController::getInstance()->m_tileMap[i];
                        if (!(tileInfo.state == FUN_BUILD_LOCK || tileInfo.xmlOpen==1)) {
                            return i;
                        }
                    }
                }
                
                if (true) {
                    for (int i=52; i<=54; i++) {
                        DragonBuild* build = dynamic_cast<DragonBuild*>(m_nodeBuildings[i]->getChildByTag(1));
                        if (build && build->m_buildingKey == i) {
                            auto& tileInfo = DragonBuildingController::getInstance()->m_tileMap[i];
                            if (!(tileInfo.state == FUN_BUILD_LOCK || tileInfo.xmlOpen==1)) {
                                return i;
                            }
                        }
                    }
                }
            }
        }
    }
    return -1;
}

void DragonScene::onOpenBuildEnd(int itemId)
{
    if (m_buildItems.find(itemId) == m_buildItems.end()) {
        onCreateBuild(itemId);
    }
    m_buildItems[itemId]->changeBg();
}

void DragonScene::onCreateBuild(int itemId)
{
    int pos = itemId%1000;
    
    DragonBuild* build = dynamic_cast<DragonBuild*>(m_nodeBuildings[pos]->getChildByTag(1));
    if (build) {
        build->initDragonBuild(itemId, m_nameLayer);
        int od = m_nodeBuildings[pos]->getZOrder();
        build->setNamePos(m_nodeBuildings[pos]->getPositionX()
                          , m_nodeBuildings[pos]->getPositionY(), m_signLayer, m_popLayer, m_buildBatchMap[pos], od, nullptr);
        
        m_buildItems[itemId] = build;
        build->m_key = 1000-od;
    }
}

void DragonScene::onOpenBuild(int itemId)
{
}

CCNode* DragonScene::getBuildById(int itemId){
    if(m_buildItems.size() == 0){
        return NULL;
    }
    int id = DragonBuildingController::getInstance()->getMaxLvBuildByType(itemId);
    
    map<int, DragonBuild*>::iterator it = m_buildItems.find(id);
    if(it != m_buildItems.end()){
//        if(itemId == FUN_BUILD_FOOD) {
//            setUnMoveScence(true);
//            return m_buildItems[id]->m_spr;
//        }
        setUnMoveScence(true);
        return m_buildItems[id];
    }
    if(itemId < BUILD_COUNT - 1){
        map<int, FunBuildInfo>::iterator it;
        for(it = GlobalData::shared()->imperialInfo.begin(); it != GlobalData::shared()->imperialInfo.end(); it++){
            if(it->second.pos == itemId){
                return NULL;
            }
        }
        DragonBuild* build = dynamic_cast<DragonBuild*>(m_nodeBuildings[itemId]->getChildByTag(1));
        setUnMoveScence(true);
        return build->m_tile;
    }
    return NULL;
}

void DragonScene::onFinishMoveBuild(CCObject* obj)
{
//    CCDictionary* posData = dynamic_cast<CCDictionary*>(obj);
//    if (posData) {
//        int oldBuild1 = posData->valueForKey("oldBuild1")->intValue();
//        int oldBuild2 = posData->valueForKey("oldBuild2")->intValue();
//        int newBuild1 = posData->valueForKey("newBuild1")->intValue();
//        int newBuild2 = posData->valueForKey("newBuild2")->intValue();
//        int pos1 = posData->valueForKey("pos1")->intValue();
//        int pos2 = posData->valueForKey("pos2")->intValue();
//        
//        if (oldBuild1>0 && m_buildItems.find(oldBuild1)!=m_buildItems.end()) {
//            m_buildItems.erase(m_buildItems.find(oldBuild1));
//        }
//        if (oldBuild2>0 && m_buildItems.find(oldBuild2)!=m_buildItems.end()) {
//            m_buildItems.erase(m_buildItems.find(oldBuild2));
//        }
//        
//        DragonBuild* delbuild1 = dynamic_cast<DragonBuild*>(m_nodeBuildings[pos1]->getChildByTag(pos1));
//        delbuild1->onBuildDelete();
//        m_nodeBuildings[pos1]->removeChild(delbuild1);
//        DragonBuild* delbuild2 = dynamic_cast<DragonBuild*>(m_nodeBuildings[pos2]->getChildByTag(pos2));
//        delbuild2->onBuildDelete();
//        m_nodeBuildings[pos2]->removeChild(delbuild2);
//        
//        if (newBuild1>0) {
//            DragonBuild* build = DragonBuild::create( newBuild1 , m_nameLayer);
//            build->setTag(pos2);
//            m_nodeBuildings[pos2]->addChild(build);
//            m_nodeBuildings[pos2]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
//            m_buildItems[newBuild1] = build;
//            
//            CCSpriteBatchNode* curBatch = m_resbatchNode;
//            CCSpriteBatchNode* curBlentBatch = NULL;
//            if (m_buildBatchMap.find(pos2) != m_buildBatchMap.end()) {
//                curBatch = m_buildBatchMap[pos2];
//                curBlentBatch = NULL;
//            }
//            
//            curBatch->setPosition(ccp(0, 0));
//            int od = m_nodeBuildings[pos2]->getZOrder();
//            build->m_key = 1000-od;
//            build->setNamePos(m_nodeBuildings[pos2]->getPositionX()
//                              , m_nodeBuildings[pos2]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, curBatch, od, curBlentBatch);
//            build->onPlayUpEnd(true);
//        }else {
//            DragonBuild* build = DragonBuild::create( pos2 , m_nameLayer);
//            build->setTag(pos2);
//            m_nodeBuildings[pos2]->addChild(build);
//            m_nodeBuildings[pos2]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
//            int od = m_nodeBuildings[pos2]->getZOrder();
//            m_nodeBuildings[pos2]->setZOrder(od);
//            build->setTileBatch(m_nodeBuildings[pos2]->getPositionX(), m_nodeBuildings[pos2]->getPositionY(), m_resbatchNode, od);
//            build->m_key = 1000-od;
//        }
//        
//        if (newBuild2>0) {
//            DragonBuild* build = DragonBuild::create( newBuild2 , m_nameLayer);
//            build->setTag(pos1);
//            m_nodeBuildings[pos1]->addChild(build);
//            m_nodeBuildings[pos1]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
//            m_buildItems[newBuild2] = build;
//            
//            CCSpriteBatchNode* curBatch = m_resbatchNode;
//            CCSpriteBatchNode* curBlentBatch = NULL;
//            if (m_buildBatchMap.find(pos1) != m_buildBatchMap.end()) {
//                curBatch = m_buildBatchMap[pos1];
//                curBlentBatch = NULL;
//            }
//            
//            curBatch->setPosition(ccp(0, 0));
//            int od = m_nodeBuildings[pos1]->getZOrder();
//            build->m_key = 1000-od;
//            build->setNamePos(m_nodeBuildings[pos1]->getPositionX()
//                              , m_nodeBuildings[pos1]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, curBatch, od, curBlentBatch);
//            build->onPlayUpEnd(true);
//        }else {
//            FunBuild* build = FunBuild::create( pos1 , m_nameLayer);
//            build->setTag(pos1);
//            m_nodeBuildings[pos1]->addChild(build);
//            m_nodeBuildings[pos1]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
//            int od = m_nodeBuildings[pos1]->getZOrder();
//            m_nodeBuildings[pos1]->setZOrder(od);
//            build->setTileBatch(m_nodeBuildings[pos1]->getPositionX(), m_nodeBuildings[pos1]->getPositionY(), m_resbatchNode, od);
//            build->m_key = 1000-od;
//        }
//    }
}

void DragonScene::setUnMoveScence(bool st)
{
    m_viewPort->notMove = st;
}

void DragonScene::showBuildBtns(int itemId)
{
    map<int, DragonBuild*>::iterator it = m_buildItems.find(itemId);
    if(it != m_buildItems.end()){
        lastTouchBuildId = -1;
        m_buildItems[itemId]->onSetClickType();
        m_buildItems[itemId]->onClickThis(0);
    }
}

CCNode* DragonScene::getBuildNameById(int itemId, string _key)
{
    if(m_viewPort->notMove) {
        return NULL;
    }
    if (_key=="up" || _key=="detail" || _key=="soldier" || _key=="science" || _key=="forge") {
        return m_buildBtnsView->getGuideNode(_key);
    }
    else if (_key=="cd") {
        CCNode* ret=NULL;
        int id = DragonBuildingController::getInstance()->getMaxLvBuildByType(itemId);
        map<int, DragonBuild*>::iterator it = m_buildItems.find(id);
        if(it != m_buildItems.end()){
            ret = m_buildItems[id]->getGuideNode(_key);
        }
        if (ret==NULL) {
            ret = m_buildBtnsView->getGuideNode(_key);
        }
        return ret;
    }
    else if (_key=="troop" || _key=="collect") {
        int id = DragonBuildingController::getInstance()->getMaxLvBuildByType(itemId);
        map<int, DragonBuild*>::iterator it = m_buildItems.find(id);
        if(it != m_buildItems.end()){
            return m_buildItems[id]->getGuideNode(_key);
        }
    }
    return NULL;
}

//void DragonScene::onPowerADD(CCObject* obj)
//{
//    int bid = DragonBuildingController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SCIENE);
//    if (bid >0 ) {
//        flyBall(bid, 1, false);
//    }
//}

void DragonScene::flyBall(int buildId, int type, bool toScene)
{
    auto &info = (*DragonBuildingController::getInstance()->curDragonBuildsInfo)[buildId];
    if (m_buildItems.find(buildId) == m_buildItems.end()) {
        return;
    }
    auto sp = m_nodeBuildings[info.pos]->getPosition();
    auto &sz = m_buildItems[buildId]->getContentSize();
    sp.x += sz.width/2;
    sp.y += sz.height/2;
    
    auto pt = this->onGetSceenPt(sp.x, sp.y);
    if (type == 0) {
        long addPower = info.mapPower[info.level];
        GlobalData::shared()->playerInfo.buildingPower = GlobalData::shared()->playerInfo.buildingPower + addPower;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_COLLECT_SOLDIER_ADD_POWER);
    }
    if (type == 1) { // 只播放战斗力
        onFlyTroopParticle(pt.x,pt.y,1);
    }
    else {
        long addPower = info.mapPower[info.level];
        int addExp = info.oldExp;
        if (addExp > 0 && addPower <= 0) {
            onFlyTroopParticle(pt.x,pt.y,2);
        }else if (addExp <= 0 && addPower > 0) {
            onFlyTroopParticle(pt.x,pt.y,1);
        }else {
            onFlyTroopParticle(pt.x,pt.y,0);
        }
    }
}

void DragonScene::flyBallEnd(CCNode* pSender)
{
    pSender->stopAllActions();
    pSender->setVisible(false);
}

void DragonScene::flyOutPut(int buildId, CCNode* flNode, int idx)
{
    flNode->stopAllActions();
    flNode->removeAllChildren();
    
    auto &info = (*DragonBuildingController::getInstance()->curDragonBuildsInfo)[buildId];
    makeFlyMc(info.type, flNode);
    
    auto sp = m_nodeBuildings[info.pos]->getPosition();
    auto &sz = m_buildItems[buildId]->getContentSize();
    sp.x += sz.width/2;
    sp.y += sz.height/2;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    int tx = 0;
    int ty = 0;
    if(info.type == 1) {
        tx = (winSize.width*0.8 - (m_touchLayer->getPositionX()-m_viewPort->bottomLeft.x))/m_touchLayer->getScale();
        ty = (winSize.height*0.9 - (m_touchLayer->getPositionY()-m_viewPort->bottomLeft.y))/m_touchLayer->getScale();
    }
    else if(info.type == 2) {
        tx = (winSize.width*0.8 - (m_touchLayer->getPositionX()-m_viewPort->bottomLeft.x))/m_touchLayer->getScale();
        ty = (winSize.height*0.8 - (m_touchLayer->getPositionY()-m_viewPort->bottomLeft.y))/m_touchLayer->getScale();
    }
    
    int sx = sp.x-0;
    int sy = sp.y;
    
    //    CCActionInterval * delaytime = CCDelayTime::create(0.2*idx);
    
    int fh = 0;
    int fw = 0;
    float ration = 0;
    int add = rand()%6 * 15 - 45;
    int rand = sz.height*1.0 + add;
    float ra = 15;
    if (idx==0) {
        fh = rand;
    }
    else {
        if (idx%2==0) {
            ration = (idx/2)*ra*3.14/180;
            fw = -rand*sinf(ration);
            fh = rand*cosf(ration);
        }
        else {
            ration = (idx+1/2)*ra*3.14/180;
            fw = rand*sinf(ration);
            fh = rand*cosf(ration);
        }
    }
    
    CCActionInterval* moveBy = CCMoveBy::create(0.3,ccp(fw, fh));
    CCMoveTo* move = CCMoveTo::create(1.0, ccp(tx,ty));
    CCActionInterval* act = CCEaseSineIn::create(move);
    auto  actionBy = CCSequence::create(moveBy,act,CCCallFuncN::create(this, callfuncN_selector(DragonScene::flyOutPutEnd)), NULL);
    flNode->setPosition(ccp(sx, sy));
    flNode->setVisible(true);
    flNode->runAction(actionBy);
}

void DragonScene::flyOutPutEnd(CCNode* pSender)
{
    pSender->setVisible(false);
    pSender->stopAllActions();
    pSender->removeAllChildren();
}

void DragonScene::makeFlyMc(int type, CCNode* flNode)
{
    string picName = "";
    if (type == 1) {
        picName = "fly_money_";
    }
    else if (type == 2) {
        picName = "fly_wood_";
    }
    else if (type == 3) {
        picName = "fly_iron_";
    }
    if (picName == "") {
        return;
    }
    
    auto sprAni = CCSprite::create();
    sprAni->setAnchorPoint(ccp(0.5, 0.5));
    sprAni->setScale(1.0);
    int size = 8;
    Vector<SpriteFrame*> myArray;
    CCSpriteFrame *myframe[size];
    for (int i=0; i<size; i++) {
        string tmp = picName+CC_ITOA(i+1)+".png";
        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat(tmp.c_str(), i+1)->getCString());
        myframe[i]->getTexture()->setAntiAliasTexParameters();
        myArray.pushBack(myframe[i]);
    }
    int idx = rand()%8;
    sprAni->setDisplayFrame(myframe[idx]);
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, 0.07f);
    CCAnimate* animate = CCAnimate::create(animation);
//    myArray->release();
    int ra = rand()%18 * 10;
    sprAni->setRotation(ra-180);
    sprAni->stopAllActions();
    sprAni->runAction(CCRepeatForever::create(animate));
    
    flNode->addChild(sprAni);
}

#pragma mark -
#pragma mark CocosBuilder Part
bool DragonScene::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    if (pTarget == this && strncmp(pMemberVariableName, "m_node",6) == 0) {
        char index[5] = "";
        strncpy(index, pMemberVariableName + 6, strlen(pMemberVariableName) - 6);
        
        int cityIndex = atoi(index);
        CCAssert(cityIndex < BUILD_COUNT,"Node citys could not more than m_nodeCity!");
        m_nodeBuildings[cityIndex] = pNode;
        m_nodeBuildings[cityIndex]->retain();
        return true;
    }
    return false;
}

void DragonScene::showAttention(){
    std::string steps = CCUserDefault::sharedUserDefault()->getStringForKey(GUIDE_STEP);
    if(steps != ""){
        GlobalData::shared()->focusBuildId = -1;
        return;
    }
    if(GlobalData::shared()->focusBuildId != -1){
        this->onMoveToBuild(GlobalData::shared()->focusBuildId);
        GlobalData::shared()->focusBuildId = -1;
        return;
    }
    map<int, FunBuildInfo>::iterator buildingIt;
    for(buildingIt = GlobalData::shared()->imperialInfo.begin(); buildingIt != GlobalData::shared()->imperialInfo.end(); buildingIt++){
        if(buildingIt->second.state == FUN_BUILD_CREATE || buildingIt->second.state == FUN_BUILD_UPING){
            this->onMoveToBuild(buildingIt->second.itemId);
            return;
        }
    }
    
    auto& build = GlobalData::shared()->imperialInfo.find(400000)->second;
    int level = build.level;
    
    auto dict = _dict(LocalController::shared()->DBXMLManager()->getGroupByKey("dragon_building")->objectForKey("5020000"));
    std::string guideStr = dict->valueForKey("guard")->getCString();
    vector<std::string> bvector;
    vector<std::string> lvector;
    vector<std::string> vector;
    CCCommonUtils::splitString(guideStr, "|", vector);
    
    int buildingId = -1;
    int i = 0;
    while(i < vector.size()){
        std::string levelStr = vector[i];
        lvector.clear();
        CCCommonUtils::splitString(levelStr, ",", lvector);
        if (lvector.size()>=2)
        {
            int xmlLevel = atoi(lvector[0].c_str());
            std::string xmlStr = lvector[1];
            if(xmlLevel == level){
                CCCommonUtils::splitString(xmlStr, ";", bvector);
                int j = 0;
                while (j < bvector.size()) {
                    int xmlBuildingId = atoi(bvector[j].c_str()) / 1000 * 1000;
                    int xmlBuildingLv = atoi(bvector[j].c_str()) - xmlBuildingId;
                    map<int, FunBuildInfo>::iterator it;
                    it = GlobalData::shared()->imperialInfo.find(xmlBuildingId);
                    if(it != GlobalData::shared()->imperialInfo.end()){
                        auto& xmlBuild = it->second;
                        if(xmlBuild.level < xmlBuildingLv && isGeneralOnFormationBuilding(it->first)){
                            buildingId = xmlBuildingId;
                            break;
                        }
                    }
                    j++;
                }
                break;
            }
        }
        i++;
    }
    if(buildingId != -1){
        map<int, DragonBuild*>::iterator it = m_buildItems.find(buildingId);
        if(it == m_buildItems.end()){
            return;
        }
        
        m_count = 0;
        m_viewPort->applyZoom(1);
        this->onMoveToBuild(buildingId);
        m_touchLayer->runAction(CCSequence::create(CCDelayTime::create(1.05)
                                                   , CCCallFunc::create(this, callfunc_selector(DragonScene::showAttention1))
                                                   , NULL
                                                   ));
    }
}

bool DragonScene::isGeneralOnFormationBuilding(int buildingId){
    return true;
}

void DragonScene::showAttention1(){
    if(m_touchLayer->getScale() > 1.35){
        return;
    }
    if(m_count == 0){
        auto pt = ccp(-m_touchLayer->getPosition().x + CCDirector::sharedDirector()->getWinSize().width / 2
                      , -m_touchLayer->getPosition().y + CCDirector::sharedDirector()->getWinSize().height / 2);
        
        m_viewPort->setNSZoomCenter(pt);
        m_viewPort->setWSZoomCenter(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2));
    }
    m_count++;
    
    m_viewPort->applyZoom(m_touchLayer->getScale() + 0.04);
    m_touchLayer->runAction(CCSequence::create(CCDelayTime::create(0.01)
                                               , CCCallFunc::create(this, callfunc_selector(DragonScene::showAttention1))
                                               , NULL
                                               ));
}

void DragonScene::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_waterLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void DragonScene::showTmpBuild(int pos, int itemId)
{
    m_tmpBuildPos = pos;
    DragonBuild* build = dynamic_cast<DragonBuild*>(m_nodeBuildings[pos]->getChildByTag(1));
    build->showTmpBuild(itemId);
}

void DragonScene::hideTmpBuild(int pos)
{
    DragonBuild* build = dynamic_cast<DragonBuild*>(m_nodeBuildings[pos]->getChildByTag(1));
    if(build){
        build->hideTmpBuild();
    }
}

void DragonScene::unLockTile(CCObject* params)
{
//    if(params) {
//        int itemId = dynamic_cast<CCInteger*>(params)->getValue();
//        map<int, SpeBuild*>::iterator it = m_bigTiles.find(itemId);
//        if (it != m_bigTiles.end()) {
//            m_removeSpeBId = itemId;
//            it->second->onPlayUnlock();
//        }
//    }
}

void DragonScene::refreashTile()
{
//    map<int, BigTileInfo>::iterator it=FunBuildController::getInstance()->m_bigTileMap.begin();
//    for (; it!=FunBuildController::getInstance()->m_bigTileMap.end(); it++) {
//        if(it->second.state == FUN_BUILD_LOCK && it->second.level<=FunBuildController::getInstance()->getMainCityLv()) {
//            int itemId = it->first;
//            int pos = it->second.pos;
//            
//            map<int, SpeBuild*>::iterator tileIt = m_bigTiles.find(itemId);
//            if (tileIt != m_bigTiles.end() && tileIt->second->m_isOpen==false)
//            {
//                tileIt->second->onBuildDelete();
//                m_bigTileNodes[pos]->removeAllChildren();
//                m_bigTiles.erase(tileIt);
//                
//                if (m_bigTileNodes.find(pos) != m_bigTileNodes.end()) {
//                    
//                    SpeBuild* build = SpeBuild::create(it->first);
//                    m_bigTileNodes[pos]->addChild(build);
//                    
//                    int od = m_bigTileNodes[pos]->getZOrder();
//                    build->setNamePos(m_bigTileNodes[pos]->getPositionX()
//                                      , m_bigTileNodes[pos]->getPositionY(), m_signLayer, m_arrbatchNode, m_tilebatchNode, od);
//                    m_bigTiles[it->first] = build;
//                    build->m_key = 1000-od;
//                }
//            }
//        }
//    }
}

void DragonScene::onShowBtnsView(int x, int y, int buildId)
{
    int buildType = buildId/1000;
    switch (buildType) {
//        case FUN_BUILD_MAIN:
//            SoundController::sharedSound()->playEffects(Music_Sfx_city_castle);
//            break;
        default:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
            break;
    }
    
    m_buildBtnsView->setPosition(ccp(x,y));
    m_buildBtnsView->onShow(buildId);
}

void DragonScene::onFlyTroopParticle(int x, int y, int onlyPower)
{
    m_flyX = x;
    m_flyY = y;
    m_isOnlyPower = onlyPower;
    flyCount = 1;
    playFlyTroopParticle(0);
    this->schedule(schedule_selector(DragonScene::playFlyTroopParticle), 0.5);
}

void DragonScene::playFlyTroopParticle(float _time){
    if(flyCount>0){
        flyCount = flyCount - 1;
        int toX = CCDirector::sharedDirector()->getWinSize().width / 2;
        int toY = CCDirector::sharedDirector()->getWinSize().height-80;
        
        CCPoint startPoint = ccp(m_flyX, m_flyY);
        CCPoint endPoint = ccp(toX, toY);
        
        if (m_isOnlyPower != 2)//战斗力
        {
            string tmpStart = "Collection_expF_";//Collection_Star_ Collection_expF_
            int count = 3;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                if (i==1) {
                    particle->setStartColor(ccc4f(1.0, 0.77, 0, 1.0));
                    particle->setEndColor(ccc4f(0, 0, 0, 0));
                }else if (i==2) {
                    particle->setStartColor(ccc4f(1.0, 0.55, 0, 1.0));
                    particle->setEndColor(ccc4f(1, 0.96, 0.5, 0));
                }else if (i==3) {
                    particle->setStartColor(ccc4f(1.0, 0.9, 0, 1.0));
                    particle->setEndColor(ccc4f(0, 0, 0, 0));
                }
                
                if (i == 2) {
                    particle->setPositionType(kCCPositionTypeFree);
                } else {
                    particle->setPositionType(kCCPositionTypeRelative);
                }
                particle->setPosition(startPoint);
                this->addChild(particle);
                
                auto func = CCCallFuncO::create(this, callfuncO_selector(DragonScene::onFlyTroopEnd), particle);
                
                CCActionInterval * moveTo = CCMoveTo::create(1,endPoint);
                CCActionInterval * easeExponentialIn= CCEaseExponentialOut::create(moveTo);
                
                CCActionInterval * delT = CCDelayTime::create(0.5);
                CCActionInterval * fadeOut = CCFadeOut::create(0.2);
                if (flyCount == 0 && i==count) {
                    auto funcPowerUI = CCCallFuncO::create(this, callfuncO_selector(DragonScene::onFlyTroopEndToUI), CCString::create("power"));
                    CCActionInterval * seqFadeOut = CCSequence::create(delT, funcPowerUI, fadeOut, func, NULL);
                    particle->runAction(CCSpawn::create(easeExponentialIn,seqFadeOut,NULL));
                }else {
                    CCActionInterval * seqFadeOut = CCSequence::create(delT, fadeOut, func, NULL);
                    particle->runAction(CCSpawn::create(easeExponentialIn,seqFadeOut,NULL));
                }
            }
        }
        
        if(m_isOnlyPower != 1)//经验
        {
            string tmpStart = "Collection_expF_";//Collection_Star_
            int count = 3;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                if (i == 2) {
                    particle->setPositionType(kCCPositionTypeFree);
                } else {
                    particle->setPositionType(kCCPositionTypeRelative);
                }
                
                particle->setPosition(ccp(m_flyX, m_flyY));
                this->addChild(particle);
                
                CCActionInterval * moveTo = CCMoveTo::create(1,ccp(80, toY));
                CCActionInterval * easeExponentialIn= CCEaseExponentialOut::create(moveTo);
                
                CCActionInterval * delT = CCDelayTime::create(0.5);
                CCActionInterval * fadeOut = CCFadeOut::create(0.2);
                auto func = CCCallFuncO::create(this, callfuncO_selector(DragonScene::onFlyTroopEnd), particle);
                
                if (flyCount == 0 && i==count) {
                    auto funcUI = CCCallFuncO::create(this, callfuncO_selector(DragonScene::onFlyTroopEndToUI), CCString::create("exp"));
                    CCActionInterval * seqFadeOut = CCSequence::create(delT, funcUI, fadeOut, func, NULL);
                    particle->runAction(CCSpawn::create(easeExponentialIn,seqFadeOut,NULL));
                }else {
                    CCActionInterval * seqFadeOut = CCSequence::create(delT, fadeOut, func, NULL);
                    particle->runAction(CCSpawn::create(easeExponentialIn,seqFadeOut,NULL));
                }
            }
        }
        
    }else{
        this->unschedule(schedule_selector(DragonScene::playFlyTroopParticle));
        flyCount = 0;
    }
}

void DragonScene::onFlyTroopEnd(CCObject *obj)
{
    CCNode* node = dynamic_cast<CCNode*>(obj);
    if (node) {
        node->stopAllActions();
        node->removeFromParent();
    }
}

void DragonScene::onFlyTroopEndToUI(CCObject *obj)
{
    CCString* ccStr = dynamic_cast<CCString*>(obj);
    if (ccStr) {
        string msg = ccStr->getCString();
        if (msg == "power") {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_POWER_PARTICLE);
        }else if (msg == "exp") {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_EXP_PARTICLE);
        }
    }
}

void DragonScene::scheduleHarvestEffect(float _time)
{
    showHarvestEffect(NULL);
}

void DragonScene::showHarvestEffect(CCObject* obj){
    string harvestInfo =  GlobalData::shared()->m_harvestInfo;
    if(harvestInfo==""){
        return ;
    }
    GlobalData::shared()->m_harvestInfo = "";
    std::vector<std::string> data;
    CCCommonUtils::splitString(harvestInfo,"|",data);
    if(data.size()!=3) return ;
    int outPutSec = atoi(data[0].c_str());
    int px = atoi(data[1].c_str());
    int py = atoi(data[2].c_str());
    onMoveToPos(px, py, TYPE_POS_MID, 0.5, 1, true);
    
    DragonBuildingController::getInstance()->addOutPut(outPutSec);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_SKILL_HARVEST,CCString::create(CC_ITOA(outPutSec)));
}

void DragonScene::playWatchGlow()
{
    auto spr = CCLoadSprite::createSprite("watch_golw.png");
    spr->setAnchorPoint(ccp(0, 0.5));
    spr->setScaleX(15);
    spr->setScaleY(3);
    spr->setPosition(ccp(1462, 1745));
    m_touchLayer->addChild(spr,11001);
    
    CCActionInterval * rotateto1 = CCRotateTo::create(2, 180);
    CCActionInterval * rotateto2 = CCRotateTo::create(2, 360);
    CCActionInterval * rotateto3 = CCRotateTo::create(0, 0);
    CCActionInterval * seq = CCSequence::create(rotateto1, rotateto2, rotateto3, NULL);
    spr->runAction(CCRepeatForever::create(seq));
    
    CCActionInterval * fadeTo1 = CCFadeTo::create(0.5,200);
    CCActionInterval * fadeTo2 = CCFadeTo::create(0.5, 100);
    CCActionInterval * fadeSeq = CCSequence::create(fadeTo1, fadeTo2, NULL);
    spr->runAction(CCRepeatForever::create(fadeSeq));
}

void DragonScene::checkTileGlow(CCObject* obj)
{
    QuestInfo *qinfo = QuestController::getInstance()->getRecommendQuest(false);
    int buildType = 0;
//    if (qinfo && qinfo->itemId == "2200101") {//建造步兵营任务
//        buildType = FUN_BUILD_BARRACK1;
//    }
//    else if (qinfo && qinfo->itemId == "2400101") {//建造学院任务
//        buildType = FUN_BUILD_SCIENE;
//    }
    
    if (buildType == 0) {
        m_tileGlowNode->removeAllChildren();
    }
    else {
        int bid = DragonBuildingController::getInstance()->getMaxLvBuildByType(buildType);
        int pos = -1;
        if (bid > 0) {
            FunBuildInfo& info = DragonBuildingController::getInstance()->getFunbuildById(bid);
            pos = info.pos;
        }
        else {
            pos = findCanBuildTile(qinfo->go);
        }
        if (pos>0) {
            m_tileGlowNode->removeAllChildren();
            m_tileGlowNode->setPosition( m_nodeBuildings[pos]->getPosition()+ccp(130,65) );
            for (int i=1; i<=8; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("GuideRegional_%d",i)->getCString());
                m_tileGlowNode->addChild(particle);
            }
        }
    }
}

void DragonScene::makeCanPlayArrowPoint()
{
    onMoveToPos(2250, 630, TYPE_POS_MID, 1.0, 0.7, true);
    
    m_ptArrowClick = CCPointArray::create(20);
    for (int i=17; i<=26; i++) {//城门门口10个地块
        int bid = DragonBuildingController::getInstance()->getBuildIdByPos(i);
        if (bid > 0) {
            auto& info = DragonBuildingController::getInstance()->getFunbuildById(bid);
//            if (info.type == FUN_BUILD_WOOD || info.type == FUN_BUILD_FOOD) {
//                FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
//                if (build) {
//                    int buildPosX = build->getParent()->getPositionX() + build->mainWidth / 2 ;
//                    int buildPosY = build->getParent()->getPositionY() + build->mainHeight + 50;
//                    m_ptArrowClick->addControlPoint(ccp(buildPosX, buildPosY));
//                }
//            }
        }
    }
    _arrowPtIdx = 0;
    playArrowClickEnd();
    
    scheduleOnce(schedule_selector(DragonScene::stopArrowClick), 10.0f);
}

void DragonScene::playArrowClickEnd()
{
    if (_arrowPtIdx>=m_ptArrowClick->count()) {
        _arrowPtIdx = 0;
    }
    m_flyArrow->setPosition( m_ptArrowClick->getControlPointAtIndex(_arrowPtIdx) );
    playArrowClick();
}

void DragonScene::playArrowClick()
{
    _arrowPtIdx++;
    m_flyArrow->setVisible(true);
    CCActionInterval * moveBy = CCMoveBy::create(0.5,ccp(30, 50));
    CCActionInterval * moveRBy = CCMoveBy::create(0.5,ccp(-30, -50));
    CCSequence* fadeAction = CCSequence::create(moveBy,moveRBy,NULL);
    CCActionInterval * fadeRepeat = CCRepeat::create(fadeAction, 2);
    CCCallFunc * callfunc = CCCallFunc::create(this, callfunc_selector(DragonScene::playArrowClickEnd));
    
    m_flyArrow->runAction(CCSequence::create(fadeRepeat, callfunc, NULL));
}

void DragonScene::stopArrowClick(float _time)
{
    m_flyArrow->stopAllActions();
    m_flyArrow->setVisible(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("fengshou1"));
}
