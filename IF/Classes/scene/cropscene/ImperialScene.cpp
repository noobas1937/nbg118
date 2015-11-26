//
//  ImperialScene.cpp
//  IF
//
//  Created by fubin on 13-10-31.
//
//

#include "ImperialScene.h"
#include "SceneController.h"
#include "ParticleController.h"
#include "SoundController.h"
#include "FunBuild.h"
#include "FunBuildController.h"
#include "FunBuildInfo.h"
#include "PopupViewController.h"
#include "YesNoDialog.h"
#include "GuideController.h"
#include "SetAccountView.h"
#include "AppLibHelper.h"
#include "UserBindCommand.h"
#include "UIComponent.h"
#include "QueueController.h"
#include "TileOpenView.h"
#include "BattlefieldScene.h"
#include "SuggestionView.h"
#include "MainCityArmy.h"
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
#include "ScienceCommand.h"
#include "FiveStarTaskView.h"
#include "FacebookLikeView.h"
#include "NpcTalkView.h"
#include "CityCrashView.h"
#include "PayController.h"
#include "EquipFinishView.h"
#include "PortActView.h"
#include "FeedRewardCommand.h"
#include "IFDockWalloperNode.h"
#include "ToolCreateView.h"
#include "../../Ext/CCDevice.h"
#include "ActivityEventObj.h"
#include "CCFlagWaveSprite.h"
#include "AnniversaryInfoView.h"
#include "NewPlayerQianDaoView.h"
#include "BranchController.h"
#include "AllianceManager.h"
#include "DynamicResourceController.h"
#include "IFSkeletonDataManager.h"
#include "TitanInfoCommand.h"

//begin a by ljf
#include "Walker.h"
#include "Enemy.h"
//end a by ljf

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

#define HD_SCALE 1.2f

using namespace cocos2d;

static const ccBlendFunc kCCBlendFuncTree = {GL_ONE, GL_ONE};
static int guideTypeNew = 0;

bool ImperialScene::init()
{
    unsigned long ulc = clock();
    if ( !CCLayer::init() )
    {
        return false;
    }
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("ImperialScene", "init");
    
    m_personArray = CCArray::create();
    m_soldierArray = CCArray::create();
    //begin a by ljf
    //m_walkerArray = CCArray::create();
    //end a by ljf
    ParticleController::initParticle();
    m_singleTouchState = false;
    m_exit = false;
    m_helpCount = 0;
    startDarkTime = 1;
    completeDarkTime = 2;
    startLightTime = 3;
    completeLightTime = 4;

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    int sumWidth = (1020+1020+1020)*1.5;
    int extH = 0;
    int sumHight = (1020+1020+300)*1.5+extH;
    
   
    
    m_touchLayer = CCLayer::create();
    m_bgParticleLayer = CCLayer::create();
    m_nightLayer = CCLayer::create();
    m_nameLayer =CCLabelBatchNode::create("Arial_Bold.fnt");
    m_popLayer = CCLayer::create();
    m_signLayer = CCLayer::create();
    m_funLayer = CCLayer::create();
    m_buildBtnsView = FunBuildBtnsView::create();
    m_buildBtnsView->setVisible(false);
    m_funLayer->addChild(m_buildBtnsView);
    m_speBuildBtnsView = SpeBuildBtnsView::create();
    m_speBuildBtnsView->setVisible(false);
    m_funLayer->addChild(m_speBuildBtnsView);
    m_battleLayer = CCLayer::create();
    m_soldierLayer = CCLayer::create();
    //begin a by ljf
    m_walkerLayer = CCLayer::create();
    m_touchLayer->addChild(m_walkerLayer,2999);
    //end a by ljf
    m_torchPar = CCArray::create();
    m_spineLayer = CCLayer::create();
    
    m_bgParticleLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_touchLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_nightLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_nameLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_popLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_signLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_funLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_spineLayer->setContentSize(CCSize(sumWidth, sumHight));
//    m_touchLayer = Layer::create();
//    m_touchLayer->setContentSize(CCSize(sumWidth,sumHight));
    
//    m_Layer3d = Sprite3D::create();
//    m_Layer2d = Sprite3D::create();
//    m_Layer2d->addChild(m_touchLayer);
//    m_touchLayer->setGlobalZOrder(1);
//    m_touchLayer->addChild(m_Layer2d);
//    m_touchLayer->addChild(m_Layer3d);
    this->addChild(m_touchLayer);
    m_touchLayer->setTag(IMPERIAL_SCENE_TOUCH_LAYER_TAG); //a by ljf
    
    m_sunNode = CCNode::create();//太阳光节点
    m_sunNode->setPosition(CCDirector::sharedDirector()->getWinSize().width + 50, CCDirector::sharedDirector()->getWinSize().height - 50);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_sunNode->setScale(2.4);
        m_sunNode->setPosition(50 * 2.4 + CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height - 50 * 2.4);
    }
    this->addChild(m_sunNode);
    
    m_waterLayer = CCLayer::create();
    m_touchLayer->addChild(m_waterLayer, 91);
    
    auto node = CCBLoadFile("ImperialCities_2",m_touchLayer,this);
    node->setZOrder(90);
    
    for (int i=0; i<BUILD_COUNT; i++) {
        int tag = m_nodeBuildings[i]->getTag();
        if (tag != -1) {
            m_nodeBuildings[i]->setZOrder(tag*200);
        }
        else
        {
            int od = m_nodeBuildings[i]->getOrderOfArrival();
            m_nodeBuildings[i]->setZOrder(od);
        }
    }
    
    m_mcNode1->setZOrder(m_mcNode1->getOrderOfArrival());
    m_mcNode2->setZOrder(m_mcNode2->getOrderOfArrival());
    m_mcNode3->setZOrder(m_mcNode3->getOrderOfArrival());
    m_mcNode4->setZOrder(m_mcNode4->getOrderOfArrival());
    m_mcNode5->setZOrder(m_mcNode5->getOrderOfArrival());
    m_desNode1->setZOrder(m_desNode1->getOrderOfArrival());
    m_desNode2->setZOrder(m_desNode2->getOrderOfArrival());
    m_desNode3->setZOrder(m_desNode3->getOrderOfArrival());
    m_flagNode->setZOrder(m_flagNode->getOrderOfArrival());
    m_hdNode->setZOrder(m_hdNode->getOrderOfArrival());
    m_goldMineNode->setZOrder(m_goldMineNode->getOrderOfArrival());
    m_goldBoxNode->setZOrder(m_goldBoxNode->getOrderOfArrival());
    m_shipNode->setZOrder(m_shipNode->getOrderOfArrival());
    m_merchanteNode->setZOrder(m_merchanteNode->getOrderOfArrival());
    m_cargoNode->setZOrder(m_cargoNode->getOrderOfArrival());
    m_newRDNode->setZOrder(m_newRDNode->getOrderOfArrival());
    m_xiongdiNode->setZOrder(m_xiongdiNode->getOrderOfArrival());
    
    m_wallZOrder_0->setZOrder(m_wallZOrder_0->getOrderOfArrival());
    m_wallZOrder_1->setZOrder(m_wallZOrder_1->getOrderOfArrival());
    m_wallZOrder_2->setZOrder(m_wallZOrder_2->getOrderOfArrival());
    m_wallZOrder_3->setZOrder(m_wallZOrder_3->getOrderOfArrival());
    m_wallZOrder_4->setZOrder(m_wallZOrder_4->getOrderOfArrival());
    
    map<int, CCNode*>::iterator it=m_bigTileNodes.begin();
    for (; it!=m_bigTileNodes.end(); it++) {
        (it->second)->setZOrder((it->second)->getOrderOfArrival());
    }
    
    m_touchLayer->addChild(m_bgParticleLayer,101);
    
    auto sp = CCLoadSprite::loadResource("build_lock.png");
    m_arrbatchNode = CCSpriteBatchNode::createWithTexture(sp->getTexture());
    m_touchLayer->addChild(m_arrbatchNode,2030);
    
    m_torchBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("Brasier.png")->getTexture());
    m_torchNode->addChild(m_torchBatchNode);

    auto tile = CCLoadSprite::loadResource("water.png");
    m_tilebatchNode = CCSpriteBatchNode::createWithTexture(tile->getTexture());
    m_touchLayer->addChild(m_tilebatchNode,2011);
    
    m_touchLayer->addChild(m_nightLayer,2020);
    m_touchLayer->addChild(m_nameLayer,2040);
    m_touchLayer->addChild(m_signLayer,2050);
    m_touchLayer->addChild(m_popLayer,2060);
    m_touchLayer->addChild(m_funLayer,2110);
    m_touchLayer->addChild(m_spineLayer, 251);
    
    m_cloudLayer = CCLayer::create();
    m_touchLayer->addChild(m_cloudLayer,2070);
    
    m_fountainNode = CCNode::create();
    m_fountainNode->setPosition(m_FountainTouchNode->getPosition());
    m_touchLayer->addChild(m_fountainNode, 2120);
    
    m_flyArrow = CCLoadSprite::createSprite("UI_hand.png");
    m_flyArrow->setAnchorPoint(ccp(0, 0.5));
//    m_flyArrow->setFlipY(true);
    m_flyArrow->setVisible(false);
    m_touchLayer->addChild(m_flyArrow,2090);
    
    m_tileGlowNode = CCNode::create();
    m_touchLayer->addChild(m_tileGlowNode,1180);
    
    m_touchLayer->addChild(m_battleLayer,3000);
    m_touchLayer->addChild(m_soldierLayer,2999);
    
    auto cloudSp = CCLoadSprite::loadResource("Cloud.png");
    m_cludeBatchNode = CCSpriteBatchNode::createWithTexture(cloudSp->getTexture());
    m_cludeBatchNode->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
    m_touchLayer->addChild(m_cludeBatchNode, 2010);
    
    m_viewPort = HFViewport::create();
    m_viewPort->isCanMoveAction = true;//fusheng 开启update
    m_viewPort->setViewPortTarget(m_touchLayer);
    m_viewPort->setSceneSize(sumWidth, sumHight);
    m_viewPort->setTouchDelegate(this);
    m_viewPort->setWorldBound(CCRect(0,0, sumWidth, sumHight));
    
    m_touchLayer->setScale(FunBuildController::getInstance()->oldScale);
    
    m_node3d = CCNode::create();
    m_touchLayer->addChild(m_node3d);
    m_node3d->setTag(JUST3D_NODE_TAG);
    
    float maxZoomScale = 1.2;
//    float minZoomScale = m_viewPort->getMinZoom() * 1.4;
    float minZoomScale = 0.55;
    float curZoomScale = 0.7;
    TargetPlatform target = CCApplication::sharedApplication()->getTargetPlatform();
    if(target == kTargetIpad || CCCommonUtils::isAdriodPad()){
        maxZoomScale = 1.1;
        minZoomScale = 0.66;
        curZoomScale = HD_SCALE;
        m_oldScale = HD_SCALE;
    }
    
    m_viewPort->setZoomLimitationMax(maxZoomScale);
    m_viewPort->setZoomLimitationMin(minZoomScale);
    addChild(m_viewPort);
    m_touchLayer->setScale(curZoomScale);
    m_isLogin = false;
    if (FunBuildController::getInstance()->oldx == 99999 || (GlobalData::shared()->playerInfo.level==1 && GlobalData::shared()->playerInfo.exp==0)) {
        m_isLogin = true;
        string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
        if (GlobalData::shared()->playerInfo.level==1 && GlobalData::shared()->playerInfo.exp==0 && (gFake=="" || gFake=="start_1")) {
//            onMoveToPos(1780, 800, TYPE_POS_MID, 0, 0.8, true);
            if (true || CCCommonUtils::isTestPlatformAndServer("guide_skip_2")) {
                onMoveToPos(480, 1560, TYPE_POS_MID, 0, 1.2, true);
            }else {
                onMoveToPos(1780, 800, TYPE_POS_MID, 0, 0.8, true);
            }
        }
        else {
            if (PortActController::getInstance()->isCanRwd() && !(GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0))) {
                m_viewPort->updatePosition(ccp(-1547, (winSize.height-sumHight+extH)));
            } else {
                m_viewPort->updatePosition(ccp(-1050, (winSize.height-sumHight+extH)));
            }
        }
    }
    else{
        if (PortActController::getInstance()->isCanRwd() && !(GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0))) {
            m_viewPort->updatePosition(ccp(-1547, (winSize.height-sumHight+extH)));
        } else {
            m_viewPort->updatePosition(ccp(FunBuildController::getInstance()->oldx, FunBuildController::getInstance()->oldy));
        }

    }

    m_torchNode->setZOrder(2);
    
    // 设置背景图抗锯齿
    for (auto child : m_cityBgNode->getChildren()) {
        if (child) {
            auto tex = ((Sprite*)child)->getTexture();
            if (tex) tex->setAliasTexParameters();
        }
    }
    
    setCleanFunction([](){
        CCLoadSprite::doResourceByImperialIndex(22, false);
        CCLoadSprite::doResourceByImperialIndex(28, false);
    });
    struct timeval  tm;
    gettimeofday(&tm, NULL);
    m_mainPatPlayTime = 60+(tm.tv_usec / 1000)%60;
    //域名修复代码
    string s1IP = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP, "");
    if (s1IP != "" && s1IP == S1_IP_0) {
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, S1_ACCOUNT_IP);
        CCUserDefault::sharedUserDefault()->flush();
    }
    else if (s1IP != "" && s1IP == S2_IP_0)
    {
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, S2_ACCOUNT_IP);
        CCUserDefault::sharedUserDefault()->flush();
    }
    // tao.yu xml更新关闭  NBTODO @Guojiang
    if (false) { // (!GlobalData::shared()->isXMLInitFlag) {
        scheduleOnce(schedule_selector(ImperialScene::downloadXML), 3);
    }
//    m_sprBG1->visit();
    CCLOG("ImperialScene Init finish %lu",clock() - ulc);
//    playWatchGlow();
    if(WorldController::getInstance()->selfPoint.x < 0 && ActivityController::getInstance()->ckfNeedLeave!=1 ){
        CCDirector::sharedDirector()->getRunningScene()->addChild(CityCrashView::create());
    }
    
    //begin a by ljf
    m_isVikingShipMove = false;
    //end a by ljf
    return true;
}
void ImperialScene::downloadXML(float _time)
{
    GlobalData::shared()->isXMLInitFlag = true;
    GameController::getInstance()->m_manager = new UpdateManager();
    GameController::getInstance()->m_manager->setDelegate(GameController::getInstance());
    GameController::getInstance()->m_manager->update();
}

void ImperialScene::buildingCallBack(CCObject* params)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("ImperialScene", "buildingCallBack");
    if (m_exit) {
        return;
    }
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    GuideController::share()->openSciencePanel();
    auto resSp = CCLoadSprite::loadResource("pic400000_2.png");//dsg_house_1
    resSp->getTexture()->setAntiAliasTexParameters();
    m_resbatchNode = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
    
    m_touchLayer->addChild(m_resbatchNode,199);
    
    m_soldierFlagNode = CCNode::create();
    m_touchLayer->addChild(m_soldierFlagNode,200);
    
    m_resBlentBatchNode = NULL;
    
    //为城内个别建筑，每个建筑建一个spriteBatchNode, 并根据ccb里的节点顺序，来设置batch在parent里的顺序，并空留出来100间隔，为跑兵使用
    for (int i=6; i<=16; i++) {
        int tmpZorder = 1000;
        if(i==6) {
            tmpZorder = 701;
        }else if(i==7) {
            tmpZorder = 600;
        }else if(i==8) {
            tmpZorder = 900;
        }else if(i==9) {
            tmpZorder = 700;
        }else if(i==10) {
            tmpZorder = 1000;
        }else if(i==11) {
            tmpZorder = 800;
        }else if(i==12) {
            tmpZorder = 200;
        }else if(i==13) {
            tmpZorder = 400;
        }else if(i==14) {
            tmpZorder = 300;
        }else if(i==15) {
            tmpZorder = 500;
        }else if(i==16) {
            tmpZorder = 310;
        }
        
        auto bdBatch = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_buildBatchMap[i] = bdBatch;
        m_touchLayer->addChild(bdBatch,tmpZorder);
    }
    
    if(true) {
        auto bdBatch = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_buildBatchMap[52] = bdBatch;
        m_touchLayer->addChild(bdBatch,1010);
        
        auto bdBatch53 = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_buildBatchMap[53] = bdBatch53;
        m_touchLayer->addChild(bdBatch53,1008);
        
        auto bdBatch1 = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_buildBatchMap[100] = bdBatch1;
        m_touchLayer->addChild(bdBatch1,1009);
        
        auto bdBatch2 = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_buildBatchMap[101] = bdBatch2;
        m_touchLayer->addChild(bdBatch2,702);
    }
    // tao.yu
    if (true) {//两个箭塔 和 一个城门
        auto doorBatch = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_buildBatchMap[1] = doorBatch;
        m_touchLayer->addChild(doorBatch,1900);
        auto towerLeftBatch = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_buildBatchMap[2] = towerLeftBatch;
        m_touchLayer->addChild(towerLeftBatch,2000);
        auto towerRightBatch = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_buildBatchMap[3] = towerRightBatch;
        m_touchLayer->addChild(towerRightBatch,1800);
        
        
        // tao.yu 一个奇怪的箭塔 用来做层级用的
//        auto innerWall = CCLoadSprite::createSprite("wall_inner.png");
//        innerWall->setPosition(ccp(554, 856));
//        m_touchLayer->addChild(innerWall,2000);
        // tao.yu 船 第一版本不开放
//        auto chrTree = CCLoadSprite::loadResource("pichuochuan_01.png");
//        chrTree->getTexture()->setAntiAliasTexParameters();
//        m_chrTreeBatchNode = CCSpriteBatchNode::createWithTexture(chrTree->getTexture());
//        m_touchLayer->addChild(m_chrTreeBatchNode,250);
//        // tao.yu 船的光
//        auto chrBlentTree = CCLoadSprite::loadResource("pichuochuanGlow.png");
//        chrBlentTree->getTexture()->setAntiAliasTexParameters();
//        m_chrTreeBlentBatchNode = CCSpriteBatchNode::createWithTexture(chrBlentTree->getTexture());
//        m_chrTreeBlentBatchNode->setBlendFunc(kCCBlendFuncTree);
//        m_touchLayer->addChild(m_chrTreeBlentBatchNode,1997);
    }
    
    //新建筑层次结构
    int baseZorder = 1200;
    for (int i=0; i<BUILD_COUNT; i++) {
        auto bdLayer = CCLayer::create();
        m_buildSpineMap[i] = bdLayer;
        m_touchLayer->addChild(bdLayer,baseZorder+i);
        
        auto bdBatch = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        bdBatch->setTag(1);
        bdLayer->addChild(bdBatch);
    }

    //发生 22资源释放不掉的 区域 start start start start
    onUpdateInfo();
    checkTileGlow(NULL);
    
    initMc2();
    
    showWaterfall();
    
    m_buildingInitState = true;
    onCreateBridge();
    onCreateTitan();
    this->titanChangeStatus(NULL);
    onEnterFrame(0);
    initBigTile();

    onOpenNewBuild(NULL);
    //发生 22资源释放不掉的 区域 end end end end end end
    
    // tao.yu 砍掉场景下方的树丛 和 云
//    initBgTree();
    UIComponent::getInstance()->updateBuildState(true);
    
    bool canMoveToRequest = true;
    if(!FunBuildController::getInstance()->isFirst)
    {
        m_curBuildPosx = 1010;
        m_curBuildPosy = 1256;
        float dt = 0.35;
        float scale = 0.8;
        bool newPlayerST = false;
        string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
        if (GlobalData::shared()->playerInfo.level==1 && GlobalData::shared()->playerInfo.exp==0 && (gFake=="" || gFake=="start_1")) {
//            onPlayBattle();
            newPlayerST = true;
            m_curBuildPosx = 1780;
            m_curBuildPosy = 800;
            if (true || CCCommonUtils::isTestPlatformAndServer("guide_skip_2")) {
                m_curBuildPosx = 480;
                m_curBuildPosy = 1560;
                scale = 1.2;
                canMoveToRequest = false;
            }
            dt = 0;
        } else {
            if (PortActController::getInstance()->isCanRwd() && !(GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0))) {
                m_curBuildPosx = 2530;
                m_curBuildPosy = 1430;
                dt = 0;
                canMoveToRequest = false;
            }
        }
        if(CCCommonUtils::isIosAndroidPad())
        {
            scale = HD_SCALE;
        }
//        resumeSchedulerAndActions();
//        m_bRunning = true;
        
        m_curBuildPosx = 1400;//fusheng 修改位置
        m_curBuildPosy = 2106;//fusheng 修改位置
        scale = 0.7;//fusheng 修改放缩
        onMoveToPos(m_curBuildPosx, m_curBuildPosy, TYPE_POS_MID, dt, scale, true);
        if (newPlayerST) {
//            onPlayBattle();
        }
        FunBuildController::getInstance()->isFirst=true;
        scheduleOnce(schedule_selector(ImperialScene::startGuide), 0.5f);
    }
    else
    {
        startGuide(0);
        if (PortActController::getInstance()->isCanRwd() && !(GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0))) {
            m_curBuildPosx = 1500;//fusheng 修改位置
            m_curBuildPosy = 2106;//fusheng 修改位置
            float scale = 0.8;//fusheng 修改放缩
            if(CCCommonUtils::isIosAndroidPad())
            {
                scale = HD_SCALE;
            }
            scale = 0.7;//fusheng 修改放缩
            canMoveToRequest = false;
            onMoveToPos(m_curBuildPosx, m_curBuildPosy, TYPE_POS_MID, 0, scale, true);//fusheng 这里是进入城里的屏幕的位置
        }
    }


    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Battle/Battle_soldier.plist");
    m_soldierBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("a010_0_N_move_0.png")->getTexture());
    m_touchLayer->addChild(m_soldierBatchNode, 1999);
    
    refreshSoldiers(NULL);
    //begin a by ljf
    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Imperial/Imperial_22.plist");
    m_walkerBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("b010_0_N_move_0.png")->getTexture());
    m_touchLayer->addChild(m_walkerBatchNode, 1999);
    
    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Imperial/Imperial_1.plist");
    m_jianBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("jian_0.png")->getTexture());
    m_touchLayer->addChild(m_jianBatchNode, 1999);
    
    this->schedule(schedule_selector(ImperialScene::createWalker), 0.25, 1, 0.0f);
    this->schedule(schedule_selector(ImperialScene::createEnemy), 60.0, CC_REPEAT_FOREVER, 0.0f);
    //this->schedule(schedule_selector(ImperialScene::shootArrow), 5.0, CC_REPEAT_FOREVER, 5.0f);
    //end a by ljf
    m_talkACTCell = TalkNoticeCell::create(0);
    m_talkACTCell->setVisible(false);
    m_funLayer->addChild(m_talkACTCell, -1);
    
    m_talkACTCell2 = TalkNoticeCell::create(1);
    m_talkACTCell2->setVisible(false);
    m_funLayer->addChild(m_talkACTCell2, -1);
    
    if(canMoveToRequest && QuestController::getInstance()->m_returnFromWorld){
        QuestInfo *info = QuestController::getInstance()->getRecommendQuest(false);
        QuestController::getInstance()->goToQuestTarget(info, true);
    }
    
    getScienceData();
    
    
    //fail fire
    
    auto now = WorldController::getInstance()->getTime();
    if(now < GlobalData::shared()->cityFireStamp){
        if(m_failEffectNode->getChildrenCount()<=0){
           this->addDefenseFailEffect();
        }
    }else{
        m_failEffectNode->removeAllChildrenWithCleanup(true);
    }
    
    int tmpMoveToBuild = FunBuildController::getInstance()->willMoveToBuildType;
    if (tmpMoveToBuild > 0) {
        FunBuildController::getInstance()->willMoveToBuildType = 0;
        int bid = FunBuildController::getInstance()->getMaxLvBuildByType(tmpMoveToBuild);
        if (bid>0) {
            onMoveToBuildAndPlay(bid);
        }else {
            int itemId = findCanBuildTile(tmpMoveToBuild);
            if(itemId==-1){
                itemId = findCanBuildTile(2);
                if(itemId==-1) {
                    return ;
                }
            }
            onMoveToBuildAndPlay(itemId);
        }
    }
    
    int tmpMoveToBuildItemID = FunBuildController::getInstance()->willMoveToBuildItemID; //fusheng 直接goto一个Item
    if (tmpMoveToBuildItemID > 0) {
       
        onMoveToBuildAndPlay(tmpMoveToBuildItemID);
         FunBuildController::getInstance()->willMoveToBuildItemID = 0;
        
    }

    
    GameController::getInstance()->enableQueryHistoryPurchase();
}

void ImperialScene::onCreateTitan()
{
    m_Titan = Titan::create(GlobalData::shared()->titanInfo.tid);
    if (!m_Titan) {
        CCLOG("Titan create error!!!!!!!!!!!!");
        return;
    }
    m_Titan->turnFront();
    
//    auto node = Node::create();
    titanRootNode = Node::create();
    titanRootNode->setRotation3D(Vec3(38, 39, -24));
    titanRootNode->addChild(m_Titan);
    titanRootNode->setPosition(m_touchLayer->convertToNodeSpace(m_titanNode->convertToWorldSpace(Point(0, 0))));
    

    m_node3d->addChild(titanRootNode);
    auto listener = EventListenerCustom::create(Animate3DDisplayedNotification,[this](EventCustom* ev)
    {

        auto particle = ParticleController::createFightingParticle("Dragon_landing",0.1);
        

        m_titanNode->addChild(particle);
        
        particle->setScaleY(0.7);
        

        particle->setCameraMask(m_touchLayer->getCameraMask(), true);
    }
                                                );
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, titanRootNode);

    m_touchLayer->setCameraMask((unsigned short)CameraFlag::USER4, true);
    m_node3d->setCameraMask((unsigned short) CameraFlag::USER2, true);
    
    
//    //end a by ljf
//    std::vector<Vec2> movePoint;
//    movePoint.push_back(m_tpath_1->getPosition());
//    movePoint.push_back(m_tpath_2->getPosition());
//    movePoint.push_back(m_tpath_3->getPosition());
//    movePoint.push_back(m_tpath_4->getPosition());
//    m_Titan->initMovePoint(movePoint);
    
}

void ImperialScene::onCreateVikingsShip()
{
    m_vikings3D = NBSprite3D::create("3d/ship/ship_3_skin.c3b");
    m_vikings3D->setTexture("3d/ship/ship_3.jpg");
    auto vikingsRootNode = CCNode::create();
    vikingsRootNode->setRotation3D(Vec3(38, 39, -24));
//    vikingsRootNode->setPosition(-200,-50);
    vikingsRootNode->addChild(m_vikings3D);
    
    vikingsRootNode->setPosition(m_touchLayer->convertToNodeSpace(m_vikingNode->convertToWorldSpace(Point(0, 0))));
    //begin a by ljf
    m_vikingsParticleNode = Node::create();
    vikingsRootNode->addChild(m_vikingsParticleNode);
    auto pVikingNode = Node::create();
    pVikingNode->addChild(vikingsRootNode);
    m_node3d->addChild(pVikingNode);
    m_vikings3D->setScale(1.4);
    //end a by ljf
    // m_node3d->addChild(vikingsRootNode);//d by ljf
    
//    std::vector<std::string> stand;
//    int standActIndex = 0;
//    int idleActIndex = 1;
//    m_info->getModelAniByName(standActIndex,stand);
    auto anim_stand = Animation3D::create("3d/ship/ship_3_stand.c3b");
    if (anim_stand) {
        auto pAnim = Animate3D::createWithFrames(anim_stand, 1, 100);
        if (pAnim) {
            auto act = RepeatForever::create(pAnim);
            m_vikings3D->runAction(act);
        }
    }
    
    m_touchLayer->setCameraMask((unsigned short)CameraFlag::USER4, true);
    m_node3d->setCameraMask((unsigned short) CameraFlag::USER2, true);
    
    //begin a by ljf
    //onVikingsShipMove(m_vikings3D);
    //end a by ljf
}

//begin a by ljf
bool ImperialScene::onVikingsShipTouched(CCTouch* pTouch)
{
    if(m_vikings3D == nullptr || m_vikingTouchNode == nullptr)
    {
        return false;
    }
    Vec2 touchPoint = m_vikingTouchNode->convertToNodeSpace(pTouch->getLocation());
    // 下面的touch点转换是为了让点击区域在模型内
    float originX = -1 * m_vikingTouchNode->getContentSize().width * m_vikingTouchNode->getAnchorPoint().x;
    float originY = -1 * m_vikingTouchNode->getContentSize().height * m_vikingTouchNode->getAnchorPoint().y;
    touchPoint.x = touchPoint.x + originX;
    touchPoint.y = touchPoint.y + originY;
    
    Rect boundingBox(originX, originY, m_vikingTouchNode->getContentSize().width, m_vikingTouchNode->getContentSize().height);
    //    Rect boundingBox = this->getBoundingBox();
    bool isTouched = boundingBox.containsPoint(touchPoint);
    if (!isTouched) {
        return false;
    }
    onVikingsShipMove(m_vikings3D);
    return true;
}

void ImperialScene::onVikingsShipIdle(NBSprite3D * pSprite3d)
{
    if (pSprite3d)
    {
        auto anim_stand = Animation3D::create("3d/ship/ship_3_stand.c3b");
        if (anim_stand) {
            auto pAnim = Animate3D::createWithFrames(anim_stand, 1, 100);
            if (pAnim) {
                auto act = RepeatForever::create(pAnim);
                pSprite3d->stopAllActions();
                pSprite3d->runAction(act);
            }
        }
    }
}

void ImperialScene::onVikingsShipMove(NBSprite3D * pSprite3d)
{
    if(m_isVikingShipMove)
    {
        return;
    }
    if (pSprite3d)
    {
        m_isVikingShipMove = true;
        auto anim_stand = Animation3D::create("3d/ship/ship_3_move.c3b");
        if (anim_stand) {
            auto pAnim = Animate3D::createWithFrames(anim_stand, 1, 9, 8.f);
            if (pAnim) {
                auto act = RepeatForever::create(pAnim);
                pSprite3d->stopAllActions();
                pSprite3d->runAction(act);
            }
        }
        auto viking3dPositon = m_vikings3D->getPosition3D();
        auto particleNode = Node::create();
        particleNode->setTag(233632);
        //pSprite3d->addChild(particleNode);
        m_vikingsParticleNode->addChild(particleNode);
        //船尾水花
       for( int i = 0; i <= 1; i++)
        {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d","CityBoat_back_",i)->getCString());
            //particle->setPosition(ccp(pSprite3d->getContentSize().width/2, 0)); //左侧船桨位置
            //particle->setPosition(ccp( 0, pSprite3d->getContentSize().height / 2)); //桅杆中央
            //particle->setPosition3D(Vec3(0, 0, pSprite3d->getContentSize().height / 2)); //快到船头位置
            ////particle->setPosition3D(Vec3(0, 0, 0 - pSprite3d->getContentSize().height / 2 ) );
            particle->setPosition3D(Vec3(0, 0, 0 - 130) );
            particle->setRotation3D(Vec3(90, -60, 0));
            
            particleNode->addChild(particle);
            //pSprite3d->getParent()->addChild(particle);
                
        }
        //船浆水花
        for( int i = 0; i <= 1; i++)
        {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s","CityBoat_spray")->getCString());
            ////particle->setPosition(ccp(pSprite3d->getContentSize().width / 3 - i * (pSprite3d->getContentSize().width * 2 / 3), 0 ));
            particle->setRotation3D(Vec3(90, 90, 0));
            //particle->setRotation3D(Vec3(90, 180, 0)); //不对
            //particle->setRotation3D(Vec3(180, 90, 0)); //不对
            //particle->setRotation3D(Vec3(90, 90, 90)); //效果不佳
            //particle->setRotation3D(Vec3(90, 90, 180)); //水花小
            particle->setPosition3D(Vec3(155 - i * 310, 0, 0)); //左侧船桨位置
            particleNode->addChild(particle);
        }
        //船侧水花
        for(int i = 0; i <= 1; i++)
        {
            for(int j = 0; j <=1; j++)
            {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d","CityBoat_water_",i)->getCString());
                ////particle->setPosition3D(Vec3(pSprite3d->getContentSize().width/10 - j * (pSprite3d->getContentSize().width / 5) , 0, 20));
                //particle->setRotation3D(Vec3(90, 180, 180 * j));
                //particle->setRotation3D(Vec3(90 , 180 * j, 180 * j));
                particle->setRotation3D(Vec3(90, 0, 180 * j));
                particle->setPosition3D(Vec3(45 - j * 90, 0, 20));
                particleNode->addChild(particle);
            }
        }
        /*
        auto action = RotateBy::create(3, Vec3(0, 360, 0));
        
        //auto action = MoveBy::create(3, Vec3(0, 1000, 0));
        auto action_back = action->reverse();
        auto seq = Sequence::create( action, action_back, nullptr );
        
        pSprite3d->runAction( RepeatForever::create(seq) );
        */
        float rotateTime = 0.2;
        float moveTime = 8;
        auto move1 = MoveBy::create(moveTime, Vec3(m_vikingPath1->getPositionX() - m_vikingNode->getPositionX(), m_vikingPath1->getPositionY() - m_vikingNode->getPositionY(), 0));
        //auto move2 = MoveBy::create(3, Vec3(m_vikingPath2->getPositionX() - m_vikingPath1->getPositionX(), m_vikingPath2->getPositionY() - m_vikingPath1->getPositionY(), 0));
        auto move2 = MoveBy::create(moveTime , Vec3(m_vikingPath2->getPositionX() - m_vikingPath1->getPositionX(), m_vikingPath2->getPositionY() - m_vikingPath1->getPositionY(), 0));
        auto move3 = MoveBy::create(moveTime, Vec3(m_vikingPath3->getPositionX() - m_vikingPath2->getPositionX(), m_vikingPath3->getPositionY() - m_vikingPath2->getPositionY(), 0));
        auto moveDelay = DelayTime::create(rotateTime);
        //auto moveSeq = Sequence::create( move1, moveDelay, move2, moveDelay, move3,  moveDelay, move3->reverse(),  moveDelay, move2->reverse(), moveDelay, move1->reverse(),nullptr);
        auto moveSeq = Sequence::create(moveDelay, move1, moveDelay, move2, move3, moveDelay, move3->reverse(), move2->reverse(), moveDelay, move1->reverse(),nullptr); //0.2 + 3 + 0.2 + 6 + 0.2 + 6 + 0.2 + 3 = 18.8
        
        //pSprite3d->getParent()->getParent()->runAction( RepeatForever::create(moveSeq) );
        pSprite3d->getParent()->getParent()->runAction( moveSeq);
        
        auto rotate1 = RotateBy::create(rotateTime, Vec3(0, -10, 0));
        auto rotate2 = RotateBy::create(rotateTime, Vec3(0, 40, 0));
        auto rotate3 = RotateBy::create(rotateTime, Vec3(0, 180, 0));
        auto rotate4 = RotateBy::create(rotateTime * 10, Vec3(0, -170, 0));
        
        auto rotateDelay = DelayTime::create(moveTime);
        auto callbackAction = CallFuncN::create([&](Node* sender){
            //回调动作代码
            auto anim_stand = Animation3D::create("3d/ship/ship_3_stand.c3b");
            if (anim_stand) {
                auto pAnim = Animate3D::createWithFrames(anim_stand, 1, 100);
                if (pAnim) {
                    auto act = RepeatForever::create(pAnim);
                    //if(sender == pSprite3d)
                    {
                        sender->stopAllActions();
                        sender->runAction(act);
                    }
                    
                }
            }
            if(m_vikingsParticleNode ->getChildByTag(233632))
            {
                m_vikingsParticleNode ->getChildByTag(233632)->removeFromParent();
            }
            m_isVikingShipMove = false;
        });
        auto rotateSeq = Sequence::create( rotate1, rotateDelay, rotate2, rotateDelay, rotateDelay, rotate3,   rotateDelay, rotateDelay,  rotate2->reverse(), rotateDelay, rotate4, callbackAction,  nullptr); //0.2 + 3 + 0.2 + 3 + 3 + 0.2  + 3 + 3 + 0.2 + 3 + 0.2 = 19
        //pSprite3d->runAction( RepeatForever::create(rotateSeq) );
        pSprite3d->runAction( rotateSeq);
        
        particleNode->runAction(rotateSeq->clone());
    }
}

void ImperialScene::createWalker(float t)
{
    //method 2
    if(true)
    {
        for (int i = 1; i <= 1; i++)
        {
            string m_icon = "b020";
            Walker* soldier = Walker::create(m_walkerBatchNode, NULL,0,0,m_icon,"NE",false);
            soldier->getShadow()->setScale(0.5);
            
            soldier->setAnchorPoint(ccp(0.5, 0.5));
            
            m_walkerLayer->addChild(soldier);
            soldier->setSprScale(1);
        }
        
        for (int i = 1; i <= 1; i++)
        {
            string m_icon = "b010";
            Walker* soldier = Walker::create(m_walkerBatchNode, NULL, 0,0,m_icon,"NE",false);
            soldier->getShadow()->setScale(0.5);
            
            soldier->setAnchorPoint(ccp(0.5, 0.5));
            
            m_walkerLayer->addChild(soldier);
            soldier->setSprScale(1);
        }

    }
}

void ImperialScene::createEnemy(float t)
{
    for (int i = 1; i <= 2; i++)
    {
        OutsideEnemy* soldier = OutsideEnemy::create(m_walkerBatchNode, m_jianBatchNode, ENEMY_TYPE_WILDMAN);
        //soldier->getShadow()->setScale(0.5);
        
        soldier->setAnchorPoint(ccp(0.5, 0.5));
        
        m_walkerLayer->addChild(soldier);
        
        soldier->start();
        //soldier->setSprScale(1);
    }
}

void ImperialScene::shootArrow(float t)
{
    CCPoint p1 = OutsideEnemy::ArrowBegin1;
    CCPoint p11 = OutsideEnemy::ArrowBegin2;
    CCPoint p2 = OutsideEnemy::PathEnd;
    //CCPoint localP = m_jianBatchNode->convertToNodeSpaceAR(p2);
    //CCPoint pos = ccp(localP.x+CCMathUtils::getRandomInt(450, 600), localP.y-CCMathUtils::getRandomInt(300, 400));
    //CCPoint pos = m_jianBatchNode->convertToNodeSpaceAR(p1);
    GongJian2* gong1 = GongJian2::create(m_jianBatchNode, p1, p2, 0, "jian_0.png", 1.5);
    GongJian2* gong2 = GongJian2::create(m_jianBatchNode, p11, p2, 0, "jian_0.png", 1.5);
    //addChild(gong);
    //auto sprite = CCLoadSprite::createSprite("jian_0.png");
    //m_jianBatchNode->addChild(sprite);
    //sprite->setPosition(p2);
    //gong->setPosition(localP);
}

//end a by ljf


// 桥

void ImperialScene::onCreateBridge()
{
    auto bridgeRootNode = CCNode::create();
    bridgeRootNode->setRotation3D(Vec3(38, 39, -24));
//    bridgeRootNode->setRotation3D(Vec3(0, 0, 0));
    bridgeRootNode->setPosition(m_touchLayer->convertToNodeSpace(m_bridgeNode->convertToWorldSpace(Point(0, 0))));
    auto pBridgeNode = Node::create();
    pBridgeNode->addChild(bridgeRootNode);
    m_node3d->addChild(pBridgeNode);
    
    m_bridge3D_Up = NBSprite3D::create("3d/bridge/bridge_1_skin.c3b");
    m_bridge3D_Up->setTexture("3d/bridge/bridge_1.jpg");
    m_bridge3D_Up->setScale(6);
    bridgeRootNode->addChild(m_bridge3D_Up);
    
    m_bridge3D_Down = NBSprite3D::create("3d/bridge/bridge_1_skin.c3b");
    m_bridge3D_Down->setTexture("3d/bridge/bridge_1.jpg");
    m_bridge3D_Down->setScale(6);
    m_bridge3D_Down->setRotation3D(Vec3(0, 180, 0));
    bridgeRootNode->addChild(m_bridge3D_Down);
    
    auto anim = Animation3D::create("3d/bridge/bridge_1_open.c3b");
    if (anim) {
        auto pAnim = Animate3D::createWithFrames(anim, 0, 1);
        if (pAnim) {
            auto act = Repeat::create(pAnim,1);
            auto act2 = act->clone();
            m_bridge3D_Up->runAction(act);
            m_bridge3D_Down->runAction(act2);
        }
    }
    
    m_touchLayer->setCameraMask((unsigned short)CameraFlag::USER4, true);
    m_node3d->setCameraMask((unsigned short) CameraFlag::USER2, true);
}

bool ImperialScene::onBridgeTouched(CCTouch* pTouch)
{
    if(!m_isBridgeCanClick || m_bridge3D_Up == nullptr || m_bridge3D_Down == nullptr || m_bridgeTouchNode == nullptr)
    {
        return false;
    }
    Vec2 touchPoint = m_bridgeTouchNode->convertToNodeSpace(pTouch->getLocation());
    // 下面的touch点转换是为了让点击区域在模型内
    float originX = -1 * m_bridgeTouchNode->getContentSize().width * m_bridgeTouchNode->getAnchorPoint().x;
    float originY = -1 * m_bridgeTouchNode->getContentSize().height * m_bridgeTouchNode->getAnchorPoint().y;
    touchPoint.x = touchPoint.x + originX;
    touchPoint.y = touchPoint.y + originY;
    
    Rect boundingBox(originX, originY, m_bridgeTouchNode->getContentSize().width, m_bridgeTouchNode->getContentSize().height);

    bool isTouched = boundingBox.containsPoint(touchPoint);
    if (!isTouched) {
        return false;
    }
    m_isBridgeCanClick = false;
    
    if (m_bridgeOpened) {
        onBridgeClose();
    }
    else {
        onBridgeOpen();
    }
    return true;
}

void ImperialScene::onBridgeOpen()
{
//    m_bridgeOpened = true;
    auto anim1 = Animation3D::create("3d/bridge/bridge_1_open.c3b");
    if (anim1) {
        auto pAnim = Animate3D::createWithFrames(anim1, 0, 104); //close 105-256
        if (pAnim) {
            auto act = Repeat::create(pAnim,1);
            auto act2 = act->clone();
            m_bridge3D_Up->runAction(act);
            m_bridge3D_Down->runAction(act2);
            this->runAction(CCSequence::create(CCDelayTime::create(4.0), CCCallFuncN::create(this, callfuncN_selector(ImperialScene::changeBridgeState)), NULL));
        }
    }
}

void ImperialScene::onBridgeClose()
{
//    m_bridgeOpened = false;
    auto anim1 = Animation3D::create("3d/bridge/bridge_1_close.c3b");
    if (anim1) {
        auto pAnim = Animate3D::createWithFrames(anim1, 150, 256); //close 150-256
        if (pAnim) {
            auto act = Repeat::create(pAnim,1);
            auto act2 = act->clone();
            m_bridge3D_Up->runAction(act);
            m_bridge3D_Down->runAction(act2);
            this->runAction(CCSequence::create(CCDelayTime::create(4.0), CCCallFuncN::create(this, callfuncN_selector(ImperialScene::changeBridgeState)), NULL));
        }
    }
}

void ImperialScene::changeBridgeState(CCNode* p)
{
    m_bridgeOpened = !m_bridgeOpened;
    m_isBridgeCanClick = true;
}


void ImperialScene::wallCallBack(CCObject* params)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("ImperialScene", "wallCallBack");
    if (m_exit) {
        return;
    }
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    
    m_wallBatchs[0] = CCSpriteBatchNode::createWithTexture(CCLoadSprite::createSprite("wall_1.png")->getTexture());
    m_touchLayer->addChild(m_wallBatchs[0], m_buildBatchMap[3]->getZOrder()-1);

    m_wallBatchs[1] = CCSpriteBatchNode::createWithTexture(CCLoadSprite::createSprite("wall_1.png")->getTexture());
    m_touchLayer->addChild(m_wallBatchs[1], m_buildBatchMap[3]->getZOrder()+1);
    
    m_wallBatchs[2] = CCSpriteBatchNode::createWithTexture(CCLoadSprite::createSprite("wall_1.png")->getTexture());
    m_touchLayer->addChild(m_wallBatchs[2], m_buildBatchMap[2]->getZOrder()-1);
    
    m_wallBatchs[3] = CCSpriteBatchNode::createWithTexture(CCLoadSprite::createSprite("wall_1.png")->getTexture());
    m_touchLayer->addChild(m_wallBatchs[3], m_buildBatchMap[2]->getZOrder()+1);
    
    m_wallBatchs[4] = CCSpriteBatchNode::createWithTexture(CCLoadSprite::createSprite("wall_1.png")->getTexture());
    m_touchLayer->addChild(m_wallBatchs[4], m_buildBatchMap[3]->getZOrder()-2);

    WallBuild* pWall = WallBuild::create();
    pWall->setNamePos(m_wallNode->getPositionX(), m_wallNode->getPositionY(), this, &m_wallBatchs,100);
    m_wallNode->addChild(pWall);
}

void ImperialScene::onPlayBattle()
{
    BattlefieldScene* battle = BattlefieldScene::create();
    m_battleLayer->addChild(battle);
    
    m_guideParticleNode = CCNode::create();
    m_guideParticleNode->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, 0));
    this->addChild(m_guideParticleNode);
    for (int i=1; i<=4; i++) {
        if (i!=3 && i!=4) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("OPFire_%d",i)->getCString());
            addGuideParticleToBatch(particle);
        }
    }
}

void ImperialScene::onPlayMoveTroops(int buildId)
{
    MainCityArmy* army = MainCityArmy::create(m_touchLayer, m_soldierBatchNode, buildId);
    if(army) {
        m_soldierLayer->addChild(army);
    }
}

void ImperialScene::startGuide(float _time)
{
    setUnMoveScence(false);
    if(WorldController::getInstance()->selfPoint.x < 0){
        return;
    }
    GuideController::share()->start();
//    GuideController::share()->setGuide("3076500");//fusheng 教程test
    UserUpgradeView*  pop = dynamic_cast<UserUpgradeView*>(PopupViewController::getInstance()->getCurrentPopupView());
    
// tao.yu 第一版不需要活动界面
    if (false) {
        if (m_isLogin && !GuideController::share()->isInTutorial() && pop==NULL) {//没有弹开有升级界面才弹活动界面
            if(GlobalData::shared()->analyticID != "common"){
                if (!CCCommonUtils::isIosAndroidPad()) {
                    ActivityController::getInstance()->showEnterView();
                }
            }
        }
    }
    
    if (m_isLogin) {//弹开装备锻造完成的界面
        string uuid = QueueController::getInstance()->getLoginFinishQueueUuId(TYPE_FORGE);
        if (uuid != "") {
            string itemId = QueueController::getInstance()->loginFinishQueues[uuid];
            auto equipView = EquipFinishView::create(atoi(itemId.c_str()));
            if (!CCCommonUtils::isIosAndroidPad())
            {
                if (PopupViewController::getInstance()->getCurrViewCount()>0) {
                    PopupViewController::getInstance()->pushPop(equipView);
                }else {
                    PopupViewController::getInstance()->addPopupView(equipView);
                }
            }
        }
    }
    
}

void ImperialScene::onEnter()
{
    CCLayer::onEnter();
    
    CCLoadSprite::doLoadResourceAsync(IMPERIAL_PATH, CCCallFuncO::create(this, callfuncO_selector(ImperialScene::buildingCallBack), NULL), 2+10*GlobalData::shared()->contryResType);

    CCLoadSprite::doLoadResourceAsync(IMPERIAL_PATH, CCCallFuncO::create(this, callfuncO_selector(ImperialScene::wallCallBack), NULL), 8+10*GlobalData::shared()->contryResType);
    
    CCLoadSprite::doLoadCommonResourceAsync();
    
    UIComponent::getInstance()->onCreateCropSceneUI();
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(ImperialScene::update), this, 0, false);
    
    //    //CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, Touch_Default);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onOpenNewBuild), MSG_FUNBUILD_OPEN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onGoBackOldPos), MSG_MAINSCENCE_GOBACK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onSaveCurPos), MSG_MAINSCENCE_SAVEPOS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onRemoveTmpBuild), MSG_MAINSCENCE_REMOVE_TMPBUILD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::unLockTile), MSG_UNLOCK_TILE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::moveMapToPosition), MSG_MOVE_TO_POSITION, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::refreshSoldiers), MSG_TROOPS_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::guideEnd), GUIDE_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onPowerADD), MSG_SCIENCE_POWER_ADD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::checkTileGlow), QUEST_STATE_UPDATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::createActiveBd), MSG_INIT_ACTIVITY_EVENT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onShowTalkView), MSG_TALK_OPEN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::showHarvestEffect), MSG_SHANGE_SCENCE_SHOW_HARVEST, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onFinishMoveBuild), MSG_FINISH_MOVE_BUILD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onFlyCargoOutPut), MSG_NEW_TIME_RWD_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onStopAllActions), MSG_STOP_CITY_MOVE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onRemoveGuideParticle), GUIDE_BATTLE_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::showFirework), "showFirework", NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::titanChangeStatus), MSG_TITAN_STATUS_CHANGE, NULL);//fusheng 泰坦状态改变
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::handleTitanUpgrade), MSG_TITAN_UPGRADE_COMPLETE, NULL);
    
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::titanUpgradeComplete), MSG_TITAN_UPGRADE_COMPLETE, NULL);//fusheng 泰坦升级粒子效果
    
    string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
    if (GlobalData::shared()->playerInfo.level==1 && GlobalData::shared()->playerInfo.exp==0 && (gFake==""||gFake=="start_1")) {
    }else{
        SoundController::sharedSound()->playBGMusic(Music_M_city_1);//fusheng for 黄迪
//        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/background/m_city.mp3");
    }
    
    m_isDay = true;//默认白天
    checkDayTime(true);//判断是否是白天
//    changeDayOrNight(!m_isDay , false);
    
    if(!m_praticle){
        if (GlobalData::shared()->contryResType == 1) {
            
        }else if (GlobalData::shared()->contryResType == 2) {
            m_waterType = 1;
            // tao.yu 第一版本去掉
//            makeWater(0);
            
            m_dropNode = CCNode::create();
            m_dropNode->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2,CCDirector::sharedDirector()->getWinSize().height/2));
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_dropNode->setScale(2.4);
            }
            this->addChild(m_dropNode);
            
            int rIdx = CCMathUtils::getRandomInt(0, 9);
            if (rIdx < 0) {
                // tao.yu 去掉雨
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
            makeWater(0);
        }
    }

    int userLv = GlobalData::shared()->lordInfo.levelUp;
    if(userLv<GlobalData::shared()->playerInfo.level){
        CCDictionary* tmp = CCDictionary::create();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAYER_LEVELUP);
    }
    this->scheduleOnce(schedule_selector(ImperialScene::scheduleHarvestEffect), 0.45);
    //    GuideController::share()->start();
    BranchController::getInstance()->excute("InviteForGift");
    
    
    
//    if (m_Titan) {//fusheng 龙的形象
//        m_Titan->resetDisplay(GlobalData::shared()->titanInfo.tid);
//    }
}

void ImperialScene::showWaterfall()
{
    CCBLoadFile("waterfall",m_waterfallNode,this);
    // add particle
    
    auto prt5 = ParticleController::createParticle("Waterfall_water_z");
    m_waterfall_prt->getChildByTag(1)->addChild(prt5);
    
    auto prt6 = ParticleController::createParticle("Waterfall_water_w");
    m_waterfall_prt->getChildByTag(2)->addChild(prt6);
    
    auto prt7 = ParticleController::createParticle("Waterfall_fall_z");
    m_waterfall_prt->getChildByTag(3)->addChild(prt7);
    
    auto prt8 = ParticleController::createParticle("Waterfall_fall_w");
    m_waterfall_prt->getChildByTag(4)->addChild(prt8);

}

void ImperialScene::showRain()
{
    m_isRain = true;
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    m_praticle = ParticleController::createFightingParticle("rain", 60, ccp(winSize.width / 2 - 50, winSize.height + 10));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_praticle->setScale(2.4);
    }
    this->addChild(m_praticle);
    
    for (int i=1; i<=3; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("Drop_%d",i)->getCString());
        m_dropNode->addChild(particle);
    }
    this->scheduleOnce(schedule_selector(ImperialScene::clearDrop), 65);
}

void ImperialScene::showFirework(CCObject* obj){
    CCDictionary* dict = dynamic_cast<CCDictionary*>(obj);
    m_useItemId = dict->valueForKey("itemId")->intValue();
    if (m_useItemId==ITEM_FIREWORK2) {
        SoundController::sharedSound()->playEffects(Music_Sfx_firework_in2);
        this->schedule(schedule_selector(ImperialScene::startShowFirework), 0.25, 20, 0.0f);
    }else{
        SoundController::sharedSound()->playEffects(Music_Sfx_firework_in1);
        this->schedule(schedule_selector(ImperialScene::startShowFirework), 0.25, 12, 0.0f);
    }
}

void ImperialScene::titanChangeStatus(CCObject* obj){
    int status = GlobalData::shared()->titanInfo.status;
    
    if (status == 0) {//fusheng 正常显示
        
        if(m_Titan&&!m_Titan->isVisible())
        {
            m_Titan->setVisible(true);
            m_Titan->reset();
        }
        
    }else if(status == 1)//fusheng 出征
    {
        if (m_Titan&&m_Titan->isVisible()) {
            m_Titan->setVisible(false);
        }
    }
}
void ImperialScene::handleTitanUpgrade(CCObject* obj)
{
    string evolution = CCCommonUtils::getPropById(CC_ITOA(GlobalData::shared()->titanInfo.tid), "evolution");
    
    if(evolution == "1")
    {
//        if(GlobalData::shared()->titanInfo.level == 2)//fusheng 龙需要重新设置状态
        {
            
            if (m_Titan){
                
                m_Titan->removeFromParent();
                m_Titan = Titan::create(GlobalData::shared()->titanInfo.tid);
                
                if (!m_Titan) {
                    CCLOG("Titan create error!!!!!!!!!!!!");
                    return;
                }
                m_Titan->turnFront();
                m_Titan->changeTitanState(Titan::eActState::Stand);
                titanRootNode->addChild(m_Titan);
                
                m_node3d->setCameraMask((unsigned short) CameraFlag::USER2, true);
            }
        }
    }
    CCLOG("ImperialScene titan sheng ji ");
}
void ImperialScene::titanUpgradeComplete(CCObject* obj){
    
    auto tmp_ptArray = CCPointArray::create(20);
    tmp_ptArray->addControlPoint(ccp(100, 130));
    tmp_ptArray->addControlPoint(ccp(240, 300));
    tmp_ptArray->addControlPoint(ccp(340, 150));
    
    for (int j=0; j<tmp_ptArray->count(); j++) {
        for (int i=1; i<5; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("Fireworks_%d",i)->getCString());
            particle->setPosition(tmp_ptArray->getControlPointAtIndex(j));
            
            m_Titan->addChild(particle);
            
//            Vec3 vec = particle->getPosition3D();
//            
//            particle->setPosition3D(Vec3(vec.x,vec.y,203));
            particle->setPositionZ(888);
            
        }
    }
    string tmpStart = "CollectionOut_";
    for (int i=1; i<9; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        
        
        
        particle->setPosition(ccp(m_Titan->getContentSize().width/2, 0));
        
        m_Titan->addChild(particle);
        
        particle->setPositionZ(888);
//
//        Vec3 vec =  particle->getPosition3D();
//        
//        particle->setPosition3D(Vec3(vec.x,vec.y,202));
    }
}
void ImperialScene::startShowFirework(float t){
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    float ww = winSize.width;
    float wh = winSize.height;
    int fIdx = CCMathUtils::getRandom(50, ww-50);
    int fIdy = CCMathUtils::getRandom(300, wh-200);
    int colorIndex = CCMathUtils::getRandomInt(1,6);
    for (int i=1; i<=5; i++) {
        CCParticleSystemQuad* particle=NULL;
        if (m_useItemId==ITEM_FIREWORK2) {
            particle = ParticleController::createParticle(CCString::createWithFormat("Fireworks_%d_%d",colorIndex,i)->getCString());
            particle->setPosition(ccp(fIdx , fIdy));
        }else{
            particle = ParticleController::createParticle(CCString::createWithFormat("Fireworks_1_%d",i)->getCString());
            particle->setPosition(ccp(fIdx , fIdy));
        }
        float rdmSize = CCMathUtils::getRandom(0.8,2.5);
        particle->setScale(rdmSize);
        this->addChild(particle);
    }
}

void ImperialScene::addNightLight(){
    if(!m_nightLights){
        return;
    }
    auto addToNightLightLayer = [](CCNode *parentNode, int tag, const CCPoint &pt){
        std::string ccbPath = CCString::createWithFormat("pic%d_2_Night", tag)->getCString();
        if(tag == 5){
            ccbPath = "picMerchante_Night";
        }else if(tag == 6){
            ccbPath = "picjiuguan_2_Night";
        }

        auto node = CCNode::create();
        node->setTag(tag);
        node->setPosition(pt);
        auto ccb = CCBLoadFile(ccbPath.c_str(), node, NULL, true);
        parentNode->addChild(node);
    };
    
    for (auto it = m_buildItems.begin(); it != m_buildItems.end(); it++) {
        int itemId = it->first / 1000;
        if(
           itemId == FUN_BUILD_MAIN
           || itemId == FUN_BUILD_TAVERN
           || itemId == FUN_BUILD_STABLE
           || itemId == FUN_BUILD_SCIENE
           || itemId == FUN_BUILD_CELLAR
           || itemId == FUN_BUILD_SMITHY
           || itemId == FUN_BUILD_FORT
           || itemId == FUN_BUILD_WATCH_TOWER
           || itemId == FUN_BUILD_BARRACK1
           || itemId == FUN_BUILD_BARRACK2
           || itemId == FUN_BUILD_BARRACK3
           || itemId == FUN_BUILD_BARRACK4
           || itemId == FUN_BUILD_FORGE
           ){
            if(!m_nightLights->getChildByTag(itemId)){
                int buildPosX = it->second->getParent()->getPositionX();
                int buildPosY = it->second->getParent()->getPositionY();
                addToNightLightLayer(m_nightLights, itemId, ccp(buildPosX, buildPosY));
            }
        }
    }
    if(m_merchanteBuild && m_merchanteBuild->isVisible()){
        if(!m_nightLights->getChildByTag(5)){
            int buildPosX = m_merchanteBuild->getParent()->getPositionX();
            int buildPosY = m_merchanteBuild->getParent()->getPositionY();
            addToNightLightLayer(m_nightLights, 5, ccp(buildPosX, buildPosY));
        }
    }else{
        m_nightLights->removeChildByTag(5);
    }
    
    if(m_goldBoxBuild && m_goldBoxBuild->isVisible() && !m_nightLights->getChildByTag(6)){
        int buildPosX = m_goldBoxBuild->getParent()->getPositionX();
        int buildPosY = m_goldBoxBuild->getParent()->getPositionY();
        addToNightLightLayer(m_nightLights, 6, ccp(buildPosX, buildPosY));
    }
}

void ImperialScene::removeAllNightLight(){
    if(m_nightLights){
        m_nightLights->removeAllChildren();
    }
}

void ImperialScene::changeDayOrNight(bool isNight, bool isAni)
{
    // tao.yu 白天黑夜的变换，第一版关闭
    struct tm tm;
    char h[100];
    char m[100];

    time_t timer = time(0);
    tm = *localtime(&timer);
    strftime(h, sizeof(h), "%H", &tm);
    int hh = atoi(h);
    strftime(m, sizeof(m), "%M", &tm);
    int mm = atoi(m);
    
    int maxOpacity1 = 50;
    int maxOpacity2 = 50;
    int maxOpacity3 = 180;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    maxOpacity1 = 90;
    maxOpacity2 = 90;
    maxOpacity3 = 200;
#endif
    bool addTorchFlag = false;
    if(hh >= completeLightTime || hh < startDarkTime){
        addTorchFlag = false;
        m_nightLayer->removeAllChildren();
        if(!m_isDay){
            m_isDay = true;
            showSunshine();
        }
        m_isDay = true;
    }else{
        m_sunNode->removeAllChildren();
        this->unschedule(schedule_selector(ImperialScene::setSunAngle));

        auto layerSize = m_nightLayer->getContentSize();
        CCLayerColor *blacklayer = NULL;
        int layerColorTag = 10;
        int blueColorTag = 11;
        int whiteColorTag = 12;
        
        int opacity1 = 0;
        int opacity2 = 0;
        int opacity3 = 0;
        if(hh >= completeDarkTime && hh < startLightTime){
            opacity1 = maxOpacity1;
            opacity2 = maxOpacity2;
            opacity3 = maxOpacity3;
            addTorchFlag = true;
        }else{
            float f = 1.0;
            if(hh >= startDarkTime){
                int totalDarkTime = (completeDarkTime - startDarkTime) * 60;
                int dh = hh - startDarkTime;
                int totalTime = dh * 60 + mm;
                f = totalTime * 1.0 / totalDarkTime;
            }else if(hh < completeLightTime){
                int totalLightTime = (completeLightTime - startLightTime) * 60;
                int dh = completeLightTime - hh;
                int totalTime = dh * 60 - mm;
                f = totalTime * 1.0 / totalLightTime;
            }
            f = MAX(0.0, f);
            f = MIN(1.0, f);
            opacity1 = f * maxOpacity1;
            opacity2 = f * maxOpacity2;
            opacity3 = f * maxOpacity3;
            addTorchFlag = true;
        }
        
        if(m_nightLayer->getChildByTag(layerColorTag) == NULL){
            blacklayer = CCLayerColor::create(ccc4(0,0,0,255), layerSize.width, layerSize.height);
            blacklayer->setTag(layerColorTag);
            m_nightLayer->addChild(blacklayer);
        }else{
            blacklayer = dynamic_cast<CCLayerColor*>(m_nightLayer->getChildByTag(layerColorTag));
        }
        blacklayer->setOpacity(opacity1);
        
        CCScale9Sprite *blueSpr = NULL;
        if(m_nightLayer->getChildByTag(blueColorTag) == NULL){
            blueSpr = CCLoadSprite::createScale9Sprite("nightShadow.png");
            blueSpr->setAnchorPoint(ccp(0, 0));
            blueSpr->setPreferredSize(CCSize(layerSize.width, layerSize.height));
            blueSpr->setRotation(180);
            blueSpr->setPosition(ccp(layerSize.width, layerSize.height));
            blueSpr->setColor(ccc3(0, 55, 255));
            m_nightLayer->addChild(blueSpr);
            blueSpr->setTag(blueColorTag);
        }else{
            blueSpr = dynamic_cast<CCScale9Sprite*>(m_nightLayer->getChildByTag(blueColorTag));
        }
        blueSpr->setOpacity(opacity2);
        
        CCScale9Sprite *whiteSpr = NULL;
        if(m_nightLayer->getChildByTag(whiteColorTag) == NULL){
            whiteSpr = CCLoadSprite::createScale9Sprite("nightShadow.png");
            whiteSpr->setAnchorPoint(ccp(0, 0));
            whiteSpr->setPreferredSize(CCSize(layerSize.width, layerSize.height/4));
            whiteSpr->setRotation(180);
            whiteSpr->setPosition(ccp(layerSize.width, layerSize.height));
            whiteSpr->setColor(ccBLACK);
            whiteSpr->setTag(whiteColorTag);
            m_nightLayer->addChild(whiteSpr);
        }else{
            whiteSpr = dynamic_cast<CCScale9Sprite*>(m_nightLayer->getChildByTag(whiteColorTag));
        }
        whiteSpr->setOpacity(opacity3);
        m_isDay = false;
    }
    
    auto addTorch = [&](CCNode *parParent, CCNode * picParent){
        auto sprite = CCLoadSprite::createSprite("Brasier.png");
        picParent->addChild(sprite);
        sprite->setAnchorPoint(ccp(0.5, 0));
        sprite->setPosition(parParent->getPosition());

        string tmpStart = "UiFire_";
        int count = 4;
        for (int i=1; i<count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(parParent->getPosition() + ccp(-5, 55));
            addCloudParticleToBatch(particle, true);
        }
    };
    // tao.yu 暂时关闭夜晚灯光
    addTorchFlag = false;
    if(addTorchFlag){
        if(m_torchPar->count() == 0){
            addTorch(m_torchNode1, m_torchBatchNode);
            addTorch(m_torchNode2, m_torchBatchNode);
            addTorch(m_torchNode3, m_torchBatchNode);
            addTorch(m_torchNode4, m_torchBatchNode);
            addTorch(m_torchNode5, m_torchBatchNode);
            addTorch(m_torchNode6, m_torchBatchNode);
            addTorch(m_torchNode7, m_torchBatchNode);
            addTorch(m_torchNode8, m_torchBatchNode);
        }
        addNightLight();
    }else{
        CCObject *obj = NULL;
        CCARRAY_FOREACH(m_torchPar, obj){
            auto node = dynamic_cast<CCNode*>(obj);
            if(node){
                node->removeFromParent();
            }
        }
        m_torchPar->removeAllObjects();
        m_torchBatchNode->removeAllChildren();
        removeAllNightLight();
    }
}

void ImperialScene::checkDayTime(bool isInit) {
    struct tm tm;
    char s[100];
    
    char d[100];
    time_t timerG = GlobalData::shared()->getTimeStamp();
    tm = *localtime(&timerG);
    strftime(d, sizeof(d), "%d", &tm);
    int day = atoi(d);
    if (ChristmasActController::getInstance()->m_Today!=0 && ChristmasActController::getInstance()->m_Today!=day) {
        ChristmasActController::getInstance()->m_isNewDay=true;
    }
    ChristmasActController::getInstance()->m_Today = day;
    if (PortActController::getInstance()->m_Today != 0 && PortActController::getInstance()->m_Today != day) {
        PortActController::getInstance()->m_isNewDay = true;
    }
    PortActController::getInstance()->m_Today = day;
    
    time_t timer = time(0);
    tm = *localtime(&timer);
    strftime(s, sizeof(s), "%H", &tm);
    int hh = atoi(s);
    if (hh >= startDarkTime && hh < completeLightTime) {
        if (m_isDay) {
            m_sunNode->removeAllChildren();
            this->unschedule(schedule_selector(ImperialScene::setSunAngle));
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

void ImperialScene::showSunshine()
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
    this->unschedule(schedule_selector(ImperialScene::setSunAngle));
    this->schedule(schedule_selector(ImperialScene::setSunAngle), 1.0/24);
}

void ImperialScene::setSunAngle(float _time) {
    if (!m_isDay) {
        return;
    }
    auto cornerPos = onGetSceenPt(3565, 2190);//右上角顶点在屏幕中的位置
    float dx = cornerPos.x - CCDirector::sharedDirector()->getWinSize().width * 0.5;
    float dy = cornerPos.y;
    auto cornerAngle = atan2(dx, dy) * 180 / PI;
    auto maxcornerAngle = atan2(3565 - CCDirector::sharedDirector()->getWinSize().width * 0.5, CCDirector::sharedDirector()->getWinSize().height) * 180 / PI;
    auto mincornerAngle = atan2(CCDirector::sharedDirector()->getWinSize().width * 0.5, 2190) * 180 / PI;
    m_sunNode->setRotation(30 + (cornerAngle - mincornerAngle) / (maxcornerAngle - mincornerAngle) * 40);
}

void ImperialScene::clearDrop(float _time){
    m_isRain = false;
    m_dropNode->removeAllChildren();
}

void ImperialScene::moveMapToPosition(CCObject* obj){
    auto dic = _dict(obj);
    if(dic){
        float time = dic->valueForKey("time")->floatValue();
        float posx = dic->valueForKey("x")->floatValue();
        float posy = dic->valueForKey("y")->floatValue();
        float scale = dic->valueForKey("scale")->floatValue();
        onMoveToPos(posx, posy, TYPE_POS_MID, time, scale, true);
    }
}

void ImperialScene::guideEnd(CCObject* obj)
{
    PopupBaseView* curView = PopupViewController::getInstance()->getCurrentPopupView();
    if (curView) {
        ToolCreateView* toolCreateView = dynamic_cast<ToolCreateView*>(curView);
        if (toolCreateView) {
            return;
        }
    }
    
    if (guideTypeNew==1) {//跳转到 m_newRDBuild
//        onMoveToSpeBuildAndPlay(SPE_BUILD_NEWED);
//        return;
    }
    
    m_curGuideEnd = true;
    scheduleOnce(schedule_selector(ImperialScene::retGuideEnd), 0.5f);
    scheduleOnce(schedule_selector(ImperialScene::clearGuideState), 2.0f);
}
void ImperialScene::retGuideEnd(float _time)
{
    auto size = CCDirector::sharedDirector()->getWinSize();
    auto tmp2Pt = m_touchLayer->convertToNodeSpace(ccp(size.width/2, size.height/2));
    float _scale = 0.8;
    if (CCCommonUtils::isIosAndroidPad())
    {
        _scale = 1.3;
    }
//    onMoveToPos(tmp2Pt.x, tmp2Pt.y, TYPE_POS_MID, 0.5, _scale, true);//fusheng 不放缩
}
void ImperialScene::clearGuideState(float _time)
{
    m_curGuideEnd = false;
}

void ImperialScene::onExit()
{
    m_exit = true;
    //begin a by ljf
    m_walkerBatchNode->removeAllChildren();
    m_jianBatchNode->removeAllChildren();
    //m_walkerArray->removeAllObjects();
    m_walkerLayer->removeFromParent();
    //end a by ljf
    if(GlobalData::shared()->isUiInti){
        UIComponent::getInstance()->updateBuildState(false);
        //        UIComponent::getInstance()->onDeleteCropSceneUI();
    }
    
    FunBuildController::getInstance()->oldScale = m_touchLayer->getScale();
    FunBuildController::getInstance()->oldx = m_touchLayer->getPositionX();
    FunBuildController::getInstance()->oldy = m_touchLayer->getPositionY();
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onEnterFrame), this);
    // tao.yu
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(ImperialScene::update), this);
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAnim), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAgainAnim), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAgainLaterAnim), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FUNBUILD_OPEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAINSCENCE_GOBACK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAINSCENCE_SAVEPOS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAINSCENCE_REMOVE_TMPBUILD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UNLOCK_TILE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MOVE_TO_POSITION);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TROOPS_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_POWER_ADD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, QUEST_STATE_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INIT_ACTIVITY_EVENT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TALK_OPEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHANGE_SCENCE_SHOW_HARVEST);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FINISH_MOVE_BUILD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NEW_TIME_RWD_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STOP_CITY_MOVE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_BATTLE_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "showFirework");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TITAN_STATUS_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TITAN_UPGRADE_COMPLETE);
    
    
    if (m_praticle) {
        m_praticle->stopAllActions();
        this->removeChild(m_praticle);
    }
    m_praticle = NULL;
    m_feedBackHead = NULL;
    m_feedlabel = NULL;
    for (int i=0; i<m_personArray->count(); i++) {
        Person* person = dynamic_cast<Person*>(m_personArray->objectAtIndex(i));
        person->onPersonDelete();
    }
    m_personArray->removeAllObjects();
    
    for (int i=BUILD_COUNT-1; i>=0; i--) {
        FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
        if (build) {
            build->onBuildDelete();
        }
        m_nodeBuildings[i]->removeAllChildren();
        m_nodeBuildings[i]->release();
    }
    m_buildItems.clear();
    
    if (m_flagBuild) {
        m_flagBuild->onBuildDelete();
        m_flagBuild->removeFromParent();
    }
    
    if (m_hdBuild) {
        m_hdBuild->onBuildDelete();
        m_hdBuild->removeFromParent();
    }
    
    if (m_goldMineBuild) {
        m_goldMineBuild->onBuildDelete();
        m_goldMineBuild->removeFromParent();
    }
    
    if (m_goldBoxBuild) {
        m_goldBoxBuild->onBuildDelete();
        m_goldBoxBuild->removeFromParent();
    }
    
    if(m_merchanteBuild){
        m_merchanteBuild->onBuildDelete();
        m_merchanteBuild->removeFromParent();
    }
    if (m_cargoBuild) {
        m_cargoBuild->onBuildDelete();
        m_cargoBuild->removeFromParent();
    }
    if (m_xiongdiBuild) {
        m_xiongdiBuild->onBuildDelete();
        m_xiongdiBuild->removeFromParent();
    }
    if (m_newRDBuild) {
        m_newRDBuild->onBuildDelete();
        m_newRDBuild->removeFromParent();
    }
    
    if (m_lotteryBuild)
    {
        m_lotteryBuild->onBuildDelete();
        m_lotteryBuild->removeFromParent();
    }
    
    if (m_shipBuild) {
        m_shipBuild->onBuildDelete();
        m_shipBuild->removeFromParent();
    }
    
    if (m_mailBuild) {
        m_mailBuild->onBuildDelete();
        m_mailBuild->removeFromParent();
    }
    if (m_dragonBuild) {
        m_dragonBuild->onBuildDelete();
        m_dragonBuild->removeFromParent();
    }
    m_nameLayer->removeAllChildren();
    m_popLayer->removeAllChildren();
    m_signLayer->removeAllChildren();
    m_spineLayer->removeAllChildren();
    if (m_resbatchNode) {
        m_resbatchNode->removeAllChildren();
    }
    if(m_rescustombatchNode){
        m_rescustombatchNode->removeAllChildren();
    }
    if (m_chrTreeBatchNode) {
        m_chrTreeBatchNode->removeAllChildren();
    }
    if (m_arrbatchNode) {
        m_arrbatchNode->removeAllChildren();
    }
    if (m_resBlentBatchNode) {
        m_resBlentBatchNode->removeAllChildren();
    }
    if (m_chrTreeBlentBatchNode) {
        m_chrTreeBlentBatchNode->removeAllChildren();
    }
    
    map<int,CCSpriteBatchNode*>::iterator it;
    for (it=m_buildBatchMap.begin(); it!=m_buildBatchMap.end(); it++) {
        (it->second)->removeAllChildren();
    }
    m_buildBatchMap.clear();
    
    m_buildingInitState = false;
    m_touchLayer->removeAllChildren();
    m_failEffectNode->removeAllChildrenWithCleanup(true);
    IFSkeletonDataManager::shared()->purge();
    CCLayer::onExit();
}

void ImperialScene::onOpenNewBuild(CCObject* obj)
{
    map<int,FunBuildInfo>::iterator it;
    it = FunBuildController::getInstance()->curBuildsInfo->begin();
    for (; it != FunBuildController::getInstance()->curBuildsInfo->end(); it++)
    {
        if (m_buildItems.find(it->first) != m_buildItems.end()) {
            m_buildItems[it->first]->onOpenLock();
        }
    }
    
    refreashTile();
}

void ImperialScene::onUpgradeNewBuild(int buildId)
{
    //    CCLOG("build: %d *********************", buildId);
    //    struct timeval  tm;
    //    unsigned long ms;
    //    gettimeofday(&tm, NULL);
    //    ms = (tm.tv_sec%1000) * 1000 + tm.tv_usec / 1000;
    //    CCLOG("start time: %ld", ms);
    
//    // tao.yu 主建筑改成泰坦底座
//    if (buildId == FUN_BUILD_MAIN) {
//        // 泰坦升级
//    }
    int key = buildId;
    int pos = buildId%1000;
    if (m_buildItems.find(key)!=m_buildItems.end()) {
        m_buildItems.erase(m_buildItems.find(key));
    }
    
    FunBuild* delbuild = dynamic_cast<FunBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
    delbuild->onBuildDelete();
    m_nodeBuildings[pos]->removeChild(delbuild);
    
    FunBuild* build = FunBuild::create( key , m_nameLayer);
    build->setTag(pos);
    m_nodeBuildings[pos]->addChild(build);
    m_nodeBuildings[pos]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
    m_buildItems[key] = build;
    
    CCSpriteBatchNode* curBatch = m_resbatchNode;
    CCSpriteBatchNode* curBlentBatch = m_resBlentBatchNode;
    if (m_buildBatchMap.find(pos) != m_buildBatchMap.end()) {
        curBatch = m_buildBatchMap[pos];
        curBlentBatch = NULL;
    }
    
    int od = m_nodeBuildings[pos]->getZOrder();
    build->m_key = 1000-od;
    build->setNamePos(m_nodeBuildings[pos]->getPositionX()
                      , m_nodeBuildings[pos]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, curBatch, od, curBlentBatch);
    
    if (buildId == FUN_BUILD_WALL_ID  && m_wallBuild) {
        m_wallBuild->onBuildDelete();
        m_wallNode->removeChild(m_wallBuild);
        m_wallBuild = NULL;
        m_wallBuild = WallBuild::create();
        m_wallBuild->setNamePos(m_wallNode->getPositionX(), m_wallNode->getPositionY(), m_signLayer, &m_wallBatchs, 0);
        m_wallNode->addChild(m_wallBuild);
    }
    
}

float ImperialScene::getTouchLayerScale(){
    return m_touchLayer->getScale();
}

void ImperialScene::onMoveToBuild(int itemId, bool st)
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
    if(CCCommonUtils::isIosAndroidPad())
    {
        endS = HD_SCALE;
    }
    if(itemId == FUN_BUILD_MAIN){
        m_touchLayer->setScale(endS);
    }else{
        endS = 1.0f;
        if(st && itemId != FUN_BUILD_BARRACK1 && itemId != FUN_BUILD_BARRACK2) {
            endS = 1.3f;
        }
        if (st && (itemId == FUN_BUILD_BARRACK1 || itemId == FUN_BUILD_BARRACK2)) {
            TargetPlatform target = CCApplication::sharedApplication()->getTargetPlatform();
            if(target == kTargetIpad || CCCommonUtils::isAdriodPad()){
                endS = 0.75f;
            }
            if (CCCommonUtils::isIosAndroidPad())
            {
                endS = HD_SCALE;
            }
        }
        m_touchLayer->setScale(endS);
    }
    int id = FunBuildController::getInstance()->getMaxLvBuildByType(itemId);
    
    map<int, FunBuild*>::iterator it = m_buildItems.find(id);
    float f = m_touchLayer->getScale();
    cocos2d::CCPoint anchor = ccp(m_touchLayer->getContentSize().width * m_touchLayer->getAnchorPoint().x,
                                  m_touchLayer->getContentSize().height * m_touchLayer->getAnchorPoint().y);
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    anchor = ccpMult(anchor, (1.0f - f));
    FunBuild *build = NULL;
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
        build = dynamic_cast<FunBuild*>(node->getChildren().at(0));
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

void ImperialScene::onMoveToSpeBuildAndPlay(int itemId, bool st)
{
    SpeBuild* speBuild = NULL;
    switch (itemId)
    {
        case SPE_BUILD_SHIP:
            speBuild = m_shipBuild;
            break;
        
        case SPE_BUILD_HD:
            speBuild = m_hdBuild;
            break;
            
        case SPE_BUILD_GOLD:
            speBuild = m_goldMineBuild;
            break;
            
        case SPE_BUILD_GOLD_BOX:
            speBuild = m_goldBoxBuild;
            break;
            
        case SPE_BUILD_MERCHANTE:
            speBuild = m_merchanteBuild;
            break;
            
        case SPE_BUILD_CARGO:
            speBuild = m_cargoBuild;
            break;
        case SPE_BUILD_XIONGDI:
            speBuild = m_xiongdiBuild;
            break;
        case SPE_BUILD_NEWED:
            speBuild = m_newRDBuild;
            break;
        case SPE_BUILD_DRAGON:
            speBuild = m_dragonBuild;
            break;
        default:
            break;
    }
    if (speBuild == NULL)
    {
        return;
    }
    int buildPosX = speBuild->getParent()->getPositionX() + speBuild->mainWidth / 2 ;
    int buildPosY = speBuild->getParent()->getPositionY() + speBuild->mainHeight;
    if (itemId == SPE_BUILD_SHIP)
    {
        buildPosY -= speBuild->mainHeight / 3;
    }
    
    float endS = 1.3f;
    onMoveToPos(buildPosX, buildPosY, TYPE_POS_CLICK, 1.0f, endS, true);
    
    if (!GuideController::share()->isInTutorial()) {
        m_flyArrow->setPosition(ccp(buildPosX, buildPosY));
        m_flyArrow->setVisible(true);
        CCActionInterval * moveBy = CCMoveBy::create(0.5,ccp(30, 50));
        CCActionInterval * moveRBy = CCMoveBy::create(0.5,ccp(-30, -50));
        CCSequence* fadeAction = CCSequence::create(moveBy,moveRBy,NULL);
        CCActionInterval * fadeForever =CCRepeatForever::create((CCActionInterval* )fadeAction);
        m_flyArrow->runAction(fadeForever);
        
        scheduleOnce(schedule_selector(ImperialScene::hideFlyArrow), 5.0f);
    }
    
    m_forceMove = st;
}

void ImperialScene::onMoveToBuildAndPlay(int itemId, bool st)
{
    FunBuild *build = NULL;
    if(itemId>1000){
        map<int, FunBuild*>::iterator it = m_buildItems.find(itemId);
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
        build = dynamic_cast<FunBuild*>(node->getChildren().at(0));
    }
    int buildPosX = build->getParent()->getPositionX() + build->mainWidth / 2 ;
    int buildPosY = build->getParent()->getPositionY() + build->mainHeight;
    if (itemId == FUN_BUILD_MAIN_CITY_ID) {
        buildPosX = build->getParent()->getPositionX() + build->mainWidth * 0.8;
        buildPosY -= build->mainHeight/4;
    }
    m_flyArrow->setPosition(ccp(buildPosX, buildPosY));
    m_flyArrow->setVisible(true);
    CCActionInterval * moveBy = CCMoveBy::create(0.5,ccp(30, 50));
    CCActionInterval * moveRBy = CCMoveBy::create(0.5,ccp(-30, -50));
    CCSequence* fadeAction = CCSequence::create(moveBy,moveRBy,NULL);
    CCActionInterval * fadeForever =CCRepeatForever::create((CCActionInterval* )fadeAction);
    m_flyArrow->runAction(fadeForever);
    
    m_forceMove = st;
    scheduleOnce(schedule_selector(ImperialScene::hideFlyArrow), 5.0f);
}
void ImperialScene::hideFlyArrow(float _time){
    m_forceMove = false;
    m_flyArrow->stopAllActions();
    m_flyArrow->setVisible(false);
}

void ImperialScene::onMoveToBuildAndOpen(int itemId, int type, float dt, bool bound)
{
    float endS = 1.3;
    if (CCCommonUtils::isIosAndroidPad())
    {
        endS = 2.4;
    }
    if(itemId>1000){
        int buildType = itemId/1000;
        if (buildType == FUN_BUILD_MAIN || buildType == FUN_BUILD_WALL || buildType == FUN_BUILD_WATCH_TOWER || buildType == FUN_BUILD_PRISON) {
            endS = 0.8f;
            if (CCCommonUtils::isIosAndroidPad())
            {
                endS = 1.5;
            }
        }
    }
    FunBuild *build = NULL;
    if(1){
        if (itemId>1000) {
            map<int, FunBuild*>::iterator it = m_buildItems.find(itemId);
            if (it != m_buildItems.end()) {
                build = m_buildItems[itemId];
            }else {
                return;
            }
        }
        else {
            CCNode *node = m_nodeBuildings[itemId];
            build = dynamic_cast<FunBuild*>(node->getChildren().at(0));
        }
        
        m_curBuildPosx = build->getParent()->getPositionX() + build->mainWidth/2 ;
        m_curBuildPosy = build->getParent()->getPositionY() + build->mainHeight/2;
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_curBuildPosy = build->getParent()->getPositionY() + build->mainHeight;
        }
        onMoveToPos(m_curBuildPosx, m_curBuildPosy, type, dt, endS, bound);
    }
}

void ImperialScene::onMoveToPos(float x, float y, int type, float dt, float scale, bool bound)
{
    float s = m_touchLayer->getScale();
    if (CCCommonUtils::isIosAndroidPad())
    {
        s = HD_SCALE;
//        if (y >= 1025 && y <= 1040 && x >= 3195 && x <= 3210) {
//            s = 2.4;
//        }
    }
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
    else if (type == TYPE_POS_MID_UP) {
        _wf = 0.5;
        _hf = 0.6;
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
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_touchLayer->setScale(HD_SCALE);
            f = HD_SCALE;
        }
        
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
        scheduleOnce(schedule_selector(ImperialScene::onEnableMoveScence), dt+0.1f);
    }
}

void ImperialScene::onEnableMoveScence(float _time)
{
    setUnMoveScence(false);
}

CCPoint ImperialScene::onGetSceenPt(float x, float y)
{
    cocos2d::CCPoint anchor = ccp(m_touchLayer->getContentSize().width * m_touchLayer->getAnchorPoint().x,
                                  m_touchLayer->getContentSize().height * m_touchLayer->getAnchorPoint().y);
    float f = m_touchLayer->getScale();
    anchor = ccpMult(anchor, (1.0f - f));
    auto tPt = m_touchLayer->getPosition();
    return ccp(x, y)*f + anchor +tPt;
}

float ImperialScene::onGetTouchLayerScale()
{
    return m_touchLayer->getScale();
}

void ImperialScene::onMoveToPosDir(float sx, float sy, float ex, float ey, float dt, bool bound)
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
        scheduleOnce(schedule_selector(ImperialScene::onEnableMoveScence), dt+0.1f);
    }
}

void ImperialScene::onSaveCurPos(CCObject* obj)
{
    m_isSave = true;
    m_oldScale = m_touchLayer->getScale();
    m_oldPosX = m_touchLayer->getPositionX();
    m_oldPosY = m_touchLayer->getPositionY();
}

void ImperialScene::setPxPy(float x,float y)
{
    m_oldPosX = x;
    m_oldPosY = y;
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_oldScale = 1.2;
    }else{
        m_oldScale = 0.8;
    }
}

void ImperialScene::onGoBackOldPos(CCObject* obj)
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
        scheduleOnce(schedule_selector(ImperialScene::onEnableMoveScence), 0.15+0.1f);
    }
}

void ImperialScene::onRemoveTmpBuild(CCObject* obj)
{
    hideTmpBuild(m_tmpBuildPos);
}

void ImperialScene::onTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
}

void ImperialScene::onTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
}

void ImperialScene::onTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
}

void ImperialScene::onTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    ccTouchesEnded(pTouches,pEvent);
}

void ImperialScene::onSingleTouchBegin(CCTouch* pTouch)
{
    if (!m_canClick || m_battleLayer->getChildrenCount()>0) {
        return;
    }
    
    int key = 99999;
    if (curTouchBuildId > -1) {
        FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[curTouchBuildId]->getChildByTag(curTouchBuildId));
        if (build) {
            build->setWhite();
            build->stopShadow();
        }
    }
    curTouchBuildId = -1;
    
    m_beginTouchType = 0;
    
    if (m_beginTouchType==0 && m_talkACTCell && m_talkACTCell->isVisible() && m_talkACTCell->onTouchBegan(pTouch, NULL)) {
        m_beginTouchType = 1;
    }
    
    if (m_beginTouchType==0 && m_talkACTCell2 && m_talkACTCell2->isVisible() && m_talkACTCell2->onTouchBegan(pTouch, NULL)) {
        m_beginTouchType = 1;
    }
    
    if (m_beginTouchType==0) {
        for (int i=0; i<BUILD_COUNT; i++) {
            FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
            if(build && build->onTouchBegan(pTouch, NULL))
            {
                if (build->m_key < key) {
                    key = build->m_key;
                    curTouchBuildId = i;
                }
            }
        }
    }
    
    if(curTouchBuildId == -1 || FunBuildController::getInstance()->OpenMoveBuildStat) {
        lastTouchBuildId = -1;
    }
}

void ImperialScene::unLockTouch(float _time) {
    m_canClick = true;
}

void ImperialScene::onSingleTouchEnd(CCTouch* pTouch)
{
    if (!m_canClick || m_battleLayer->getChildrenCount()>0) {
        return;
    }
    m_canClick = false;
    this->scheduleOnce(schedule_selector(ImperialScene::unLockTouch), 0.5);
    
    m_buildBtnsView->onHide();
    m_speBuildBtnsView->onHide();
    
    // tao.yu titan touch
    if (m_Titan && m_Titan->onTouched(pTouch)) {
        return;
    }
    //begin a by ljf
    if(onVikingsShipTouched(pTouch))
    {
        return;
    }
    //end a by ljf
    if (onBridgeTouched(pTouch)) {
        return;
    }
    if (curTouchBuildId > -1) {
        if(lastTouchBuildId != curTouchBuildId)
        {
            lastTouchBuildId = curTouchBuildId;
            FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[curTouchBuildId]->getChildByTag(curTouchBuildId));
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
        if (FunBuildController::getInstance()->OpenMoveBuildStat) {
            st = false;
        }
        
        if(st && m_beginTouchType==1 && m_talkACTCell && m_talkACTCell->isVisible() && m_talkACTCell->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_talkACTCell->onTouchEnded(pTouch, NULL);
        }
        if(st && m_beginTouchType==1 && m_talkACTCell2 && m_talkACTCell2->isVisible() && m_talkACTCell2->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_talkACTCell2->onTouchEnded(pTouch, NULL);
        }
        if (st) {
            map<int, SpeBuild*>::iterator it = m_bigTiles.begin();
            for (; it!=m_bigTiles.end(); it++) {
                if (it->second && it->second->onTouchBegan(pTouch, NULL)) {
                    it->second->onTouchEnded(pTouch, NULL);
                    st = false;
                    break;
                }
            }
        }
        if (st && m_flagBuild && m_flagBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_flagBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_hdBuild && m_hdBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_hdBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_goldMineBuild && m_goldMineBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_goldMineBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_goldBoxBuild && m_goldBoxBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_goldBoxBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_shipBuild && m_shipBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_shipBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_merchanteBuild && m_merchanteBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_merchanteBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_cargoBuild && m_cargoBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_cargoBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_xiongdiBuild && m_xiongdiBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_xiongdiBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_newRDBuild && m_newRDBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_newRDBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_lotteryBuild && m_lotteryBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_lotteryBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && (isTouchInside(m_desNode1, pTouch) || (m_HTPHead && m_HTPHead->isVisible() && isTouchInside(m_HTPHead, pTouch)))) {//help
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            st = false;
            if ( m_mailBuild ) {
                m_mailBuild->getAnimationManager()->runAnimationsForSequenceNamed("Default");
            }
            CCCommonUtils::showHelpShiftFAQ();
        }
        if (st && isTouchInside(m_mcNode4, pTouch) && false) {//HOW TO PLAY
            st = false;
            HowToPlayView * popUp = HowToPlayView::create();
            PopupViewController::getInstance()->addPopupView(popUp,true);
        }
        if (st && m_dragonBuild && m_dragonBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_dragonBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_wallBuild && m_wallBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            curTouchBuildId = FUN_BUILD_WALL_ID%100;
            if (lastTouchBuildId != curTouchBuildId) {
                lastTouchBuildId = curTouchBuildId;
                FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[curTouchBuildId]->getChildByTag(curTouchBuildId));
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
        if (st && isTouchInside(m_FountainTouchNode, pTouch)) { // && m_fountainNode->getChildrenCount()<=0
            st = false;
            playFountain();
        }
        if (st && isTouchInside(m_fountainNode->getChildByTag(ACTIVITY_BOTTLE_TAG), pTouch)) {
            st = false;
            CCDirector::sharedDirector()->getRunningScene()->addChild(AnniversaryInfoView::create());
        }
        
        if(st) {
            auto pt = pTouch->getLocation();
            UIComponent::getInstance()->showClickEff(pt);
        }
        if (st && isTouchInside(m_crossClickNode, pTouch)) {
            st = false;
            ActivityController::getInstance()->openCrossServerActivity();
        }
    }
    
    scheduleOnce(schedule_selector(ImperialScene::canSingleTouch), 0.2f);
    
}

void ImperialScene::onResetLastBuildId()
{
    lastTouchBuildId = -1;
}

void ImperialScene::canSingleTouch(float _time){
    m_singleTouchState = false;
}
void ImperialScene::onShowFeedbackAgainAnim(float dt){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAgainAnim), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAgainLaterAnim), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAgainLaterAnim), this, 60,kCCRepeatForever, 0.0f, false);
    
    if(m_feedlabel!=NULL){
        m_feedlabel->setVisible(false);
    }
    if(m_feedBackHead){
        m_feedBackHead->setVisible(false);
    }
}
void ImperialScene::onShowFeedbackAgainLaterAnim(float dt){
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAnim), this, 0.2,kCCRepeatForever, 0.0f, false);
}
void ImperialScene::onShowFeedbackAnim(float dt){
    string feedbak = "Help";
    int totallen = feedbak.length();
    if(m_feedlabel!=NULL){
        m_feedlabel->setVisible(true);
        if(m_feedBackHead){
            m_feedBackHead->setVisible(true);
        }
        if(m_noticeCount>0){
            m_icon_op_mail->setVisible(true);
            m_HTPlabel->setString("");
            return ;
        }else{
            m_icon_op_mail->setVisible(false);
        }
        string msg =  m_feedlabel->getString();
        int len = msg.length();
        if(len==totallen){
            msg =feedbak[0];
        }else{
            msg+=feedbak[len];
        }
        if(len==totallen-1){
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAnim), this);
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAgainAnim), this, 2,1, 0.0f, false);
        }
        m_feedlabel->setString(msg.c_str());
    }
}
void ImperialScene::onShowHTPAgainAnim(float dt){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowHTPAgainAnim), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowHTPAgainLaterAnim), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowHTPAgainLaterAnim), this, 10,kCCRepeatForever, 0.0f, false);
    
    if(m_HTPlabel!=NULL){
        m_HTPlabel->setVisible(false);
    }
    if(m_HTPHead){
        m_HTPHead->setVisible(false);
    }
    if(m_HTPHead1){
        m_HTPHead1->setVisible(false);
    }
    if(m_icon_op_mail){
        m_icon_op_mail->setVisible(false);
    }
}
void ImperialScene::onShowHTPAgainLaterAnim(float dt){
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowHTPAnim), this, 0.2,kCCRepeatForever, 0.0f, false);
}
void ImperialScene::onShowHTPAnim(float dt){
    string HTP = _lang("101230")+_lang("101231");//todo:放到语言文件中

    int totallen = Utf8Utils::strlen(HTP.c_str());
    if(m_HTPlabel!=NULL){
        if(m_HTPHead){
            m_HTPHead->setVisible(true);
        }
        if(m_HTPHead1){
            m_HTPHead1->setVisible(true);
        }
        m_icon_op_mail->setVisible(m_noticeCount>0);
        m_HTPlabel->setVisible(!m_icon_op_mail->isVisible());
        std::string language = LocalController::shared()->getLanguageFileName();
        m_HTPHead->setScaleX(-1.5);//-1.2
        if (language=="id"||language=="ru") {
            m_HTPHead->setScaleX(-1.6);
        }
        if (language=="zh_CN"||language=="zh_TW") {
            m_HTPHead->setScaleX(-0.8);
        }
        if(m_noticeCount>0){
            m_HTPHead->setScaleX(-0.9);
        }

        string msg =  m_HTPlabel->getString();

        int len = Utf8Utils::strlen(msg.c_str());

        if(len==totallen){
            msg =Utf8Utils::utf8sub(HTP.c_str(),0,1);
        }else{
            msg =Utf8Utils::utf8sub(HTP.c_str(),0,len+1);
        }
        if(len==totallen-1){
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowHTPAnim), this);
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowHTPAgainAnim), this, 2,1, 0.0f, false);
        }
        if (language=="ru") {
            return;
        }
//        if (msg=="") {
//            return;
//        }
        m_HTPlabel->setString(msg.c_str());
    }
}

void ImperialScene::update(float dt)
{
    if (m_Titan)
    {
        m_Titan->update(dt);
    }
    
}


void ImperialScene::onEnterFrame(float dt)
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
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
    {
        bool tmpSt = false;
        if ((it->second).itemId > 0 && ((it->second).state==FUN_BUILD_CREATE || (it->second).state==FUN_BUILD_UPING || (it->second).state==FUN_BUILD_DESTROY || (it->second).state==FUN_BUILD_INIT))
        {
            auto curTime = GlobalData::shared()->getWorldTime();
            if ( (it->second).updateTime <= curTime)
            {
                if((it->second).state==FUN_BUILD_CREATE || (it->second).state==FUN_BUILD_INIT)
                {
                    if((it->second).state==FUN_BUILD_INIT)
                    {
                        onCreateBuild((it->second).itemId);
                    }
                    (it->second).state = FUN_BUILD_CHANGE;
                }
                else if((it->second).state==FUN_BUILD_DESTROY)
                {
                    (it->second).state = FUN_BUILD_DESTROY_END;
                }
                else
                {
                    (it->second).state = FUN_BUILD_UPING_END;
                }
                tmpSt = true;
            }
            else
            {
                if (m_buildItems.find(it->first) != m_buildItems.end()) {
                    int langTime = (it->second).updateTime - curTime;
                    m_buildItems[it->first]->setTime(langTime);
                }
            }
        }
        
        if((it->second).state==FUN_BUILD_NORMAL && m_buildItems.find(it->first) != m_buildItems.end()) {
            m_buildItems[it->first]->onUpdateUpIcon(0);
        }
        
        if((it->second).state==FUN_BUILD_INIT)//可建造状态 转变为 建造中状态
        {
            (it->second).state = FUN_BUILD_CREATE;
            onCreateBuild((it->second).itemId);
            showBuildBtns((it->second).itemId);
            if ((it->second).pos<=16) {
                checkTileGlow(NULL);
            }
        }
        
        if((it->second).state==FUN_BUILD_CHANGE)//建造中状态 转变为 正常状态
        {
            if( FunBuildController::getInstance()->completeUpOrCreate((it->second).itemId, true) )
            {
                onOpenBuildEnd((it->second).itemId);
                onUpgradeNewBuild((it->second).itemId);
                onResetBuild((it->second).itemId);
//                SoundController::sharedSound()->playEffects(Music_Sfx_city_build);
                if (tmpSt) {
                    FunBuildController::getInstance()->startFinish((it->second).itemId);
                }
                else {
                    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA((it->second).itemId));
                    QueueController::getInstance()->startFinishQueue(qid, false);
                    FunBuildController::getInstance()->clearUpBuildingInfo((it->second).itemId);
                }
                //begin a by ljf
                //修改创建码头的时候未创建船的bug
                if ((it->second).type == FUN_BUILD_TRAINFIELD) {
                    onCreateVikingsShip();
                }
                //end a by ljf
            }
        }
        
        if((it->second).state==FUN_BUILD_UPING_END)//升级中状态 转变为 正常状态
        {
//            SoundController::sharedSound()->playEffects(Music_Sfx_city_build);
            if (FunBuildController::getInstance()->completeUpOrCreate((it->second).itemId))
            {
                onUpgradeNewBuild((it->second).itemId);
                onResetBuild((it->second).itemId);
                //                onUpgradeNewBuild((it->second).itemId);
                
                if (tmpSt) {
                    FunBuildController::getInstance()->startFinish((it->second).itemId);
                }
                else {
                    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA((it->second).itemId));
                    QueueController::getInstance()->startFinishQueue(qid, false);
                    FunBuildController::getInstance()->clearUpBuildingInfo((it->second).itemId);
                }
            }
        }
        
        if (it->first==FUN_BUILD_MAIN_CITY_ID || (it->second).type==FUN_BUILD_SCIENE || (it->second).type==FUN_BUILD_BARRACK1 || (it->second).type==FUN_BUILD_BARRACK2 || (it->second).type==FUN_BUILD_BARRACK3 || (it->second).type==FUN_BUILD_BARRACK4 || (it->second).type==FUN_BUILD_FORT || (it->second).type==FUN_BUILD_HOSPITAL || (it->second).type==FUN_BUILD_STABLE || (it->second).type==FUN_BUILD_SACRIFICE || (it->second).type==FUN_BUILD_FORGE || (it->second).type==FUN_BUILD_WORKSHOP)
        {
            if ((it->second).state==FUN_BUILD_UPING || (it->second).state==FUN_BUILD_NORMAL) {
                if (m_buildItems.find((it->second).itemId) != m_buildItems.end()) {
                    m_buildItems[(it->second).itemId]->canShowState();
                }
            }
        }
        else if ((it->second).type==FUN_BUILD_WOOD || (it->second).type==FUN_BUILD_FOOD || (it->second).type==FUN_BUILD_IRON || (it->second).type==FUN_BUILD_STONE) {
            if ((it->second).state==FUN_BUILD_NORMAL) {
                if (m_buildItems.find((it->second).itemId) != m_buildItems.end()) {
                    m_buildItems[(it->second).itemId]->canShowState();
                }
            }
        }
        
        if((it->second).type==FUN_BUILD_WOOD || (it->second).type==FUN_BUILD_FOOD || (it->second).type==FUN_BUILD_IRON || (it->second).type==FUN_BUILD_STONE){
            if (m_buildItems.find((it->second).itemId) != m_buildItems.end()) {
                m_buildItems[(it->second).itemId]->checkIncreasedStatus();
            }
        }
        if ((it->second).state==FUN_BUILD_CREATE_ERROR || (it->second).state == FUN_BUILD_DESTROY_END || (it->second).state == FUN_BUILD_CANCEL_CREATE)
        {
            bool bOK = true;
            if ((it->second).state == FUN_BUILD_DESTROY_END) {
                bOK = FunBuildController::getInstance()->completeDestroy(it->first);
                if (bOK) {
                    FunBuildController::getInstance()->startFinish((it->second).itemId);
                }
                else {
                    FunBuildController::getInstance()->clearUpBuildingInfo((it->second).itemId);
                }
            }
            //建造出错
            if (bOK) {
                int key = it->first;
                int pos = it->first%1000;
                if (m_buildItems.find(key)!=m_buildItems.end()) {
                    m_buildItems.erase(m_buildItems.find(key));
                }
                FunBuild* delbuild = dynamic_cast<FunBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
                delbuild->onBuildDelete();
                m_nodeBuildings[pos]->removeChild(delbuild);
                
                FunBuild* build = FunBuild::create( pos , m_nameLayer);
                build->setTag(pos);
                m_nodeBuildings[pos]->addChild(build);
                m_nodeBuildings[pos]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
                int od = m_nodeBuildings[pos]->getZOrder();
                build->setTileBatch(m_nodeBuildings[pos]->getPositionX(), m_nodeBuildings[pos]->getPositionY(), m_resbatchNode, od);
                build->m_key = 1000-od;
                
                curBuildsInfo->erase(it);
                break;
            }
        }
        else if((it->second).state==FUN_BUILD_UPING_ERROR)
        {
            //升级出错
            (it->second).state = FUN_BUILD_NORMAL;
            m_buildItems[(it->second).itemId]->setTime(0);
        }
        else if((it->second).state==FUN_BUILD_NORMAL_ERROR)
        {
            //已经升完级 更新级别信息
            (it->second).state = FUN_BUILD_NORMAL;
            m_buildItems[(it->second).itemId]->setTime(0,true);
        }
        else if((it->second).state==FUN_BUILD_CANCEL_OTH)
        {
            //已经升完级 更新级别信息
            (it->second).state = FUN_BUILD_NORMAL;
            m_buildItems[(it->second).itemId]->setTime(0,true);
        }
        
        //主城自定义状态
        if(SceneController::getInstance()->getMaincityCustom()==true && (it->second).type ==FUN_BUILD_MAIN){
            onUpgradeNewBuild((it->second).itemId);
            SceneController::getInstance()->setMaincityCustom(false);
        }
    }

    checkDayTime();//判断是否是白天
//    changeDayOrNight(true, true);
    m_sysTime++;
    m_talkTime++;
    if (m_sysTime%2==0) {
        //每隔2秒执行一次
    }
    if (m_sysTime>70 && m_sysTime%80==0) {
        //每隔80秒执行一次
        if (m_sysTime%160==0) {
            int rIdx = CCMathUtils::getRandomInt(0, 9);
            if (rIdx < 0) {
                // tao.yu 去掉雨
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
//    if (m_sysTime>10 && m_sysTime%70==0) {
//        onDragonFly();
//    }
    
    auto now = WorldController::getInstance()->getTime();
    if(now < GlobalData::shared()->cityFireStamp){
        if(m_failEffectNode->getChildrenCount()<=0){
            this->addDefenseFailEffect();
        }
    }else{
        m_failEffectNode->removeAllChildrenWithCleanup(true);
    }
}

void ImperialScene::addToMovingLayer(CCNode* node){
    this->m_touchLayer->addChild(node);
}

void ImperialScene::stopShadow(int buildId, CCTouch *pTouch, CCEvent *pEvent)
{
    map<int, FunBuild*>::iterator it;
    for(it=m_buildItems.begin(); it != m_buildItems.end(); ++it)
    {
        if ( it->first == buildId && it->second != NULL) {
            (it->second)->stopShadow();
        }
        else{
            //            (it->second)->retTouch(pTouch, pEvent);
        }
    }
}

void ImperialScene::onResetBuild(int itemId)
{
    if (m_buildItems.find(itemId) != m_buildItems.end()) {
        m_buildItems[itemId]->setTime(0,true);
        m_buildItems[itemId]->onPlayUpEnd();
    }
}

void ImperialScene::onFlyOutPut(int itemId, int output, int forceResType)
{
    if (m_buildItems.find(itemId) != m_buildItems.end())
    {
        if (m_flyArrow->isVisible()) {
            stopArrowClick(0);
        }
        
        FunBuild *build = m_buildItems[itemId];
        if (forceResType == -1) {
            build->onFlyOutPut(0);
        }
        if (forceResType == -2){
            build->onFlyOutPutByValue(output);
        }
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        int startPosx = build->getParent()->getPositionX() + build->mainWidth/2 ;
        int startPosy = build->getParent()->getPositionY() + build->mainHeight/2;
        auto pt = onGetSceenPt(startPosx, startPosy);
//        pt.y = pt.y-winSize.height; //fusheng 为什么减去height
        
        string picStr = "Food";
        auto endPt = ccp(0,0);
        int resType = Wood;
        if (itemId/1000 == FUN_BUILD_FOOD || forceResType==Food) {
            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_farm);
//            endPt = ccp(200,-60);//winSize.height
//            if(CCCommonUtils::isIosAndroidPad())
//            {
//                endPt = ccpMult(endPt, 2.4);
//            }
            endPt = UIComponent::getInstance()->m_foodIcon->convertToWorldSpace(Point::ZERO);
            picStr = "Food";
            resType = Food;
        }else if (itemId/1000 == FUN_BUILD_WOOD || forceResType==Wood) {
            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_wood);
//            endPt = ccp(90,-40);
//            if(CCCommonUtils::isIosAndroidPad())
//            {
//                endPt = ccpMult(endPt, 2.4);
//            }
            endPt = UIComponent::getInstance()->m_woodIcon->convertToWorldSpace(Point::ZERO);
            picStr = "Wood";
            resType = Wood;
        }
        else if (itemId/1000 == FUN_BUILD_IRON || forceResType==Iron) {
            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_iron);
//            endPt = ccp(320,-40);
//            if(CCCommonUtils::isIosAndroidPad())
//            {
//                endPt = ccpMult(endPt, 2.4);
//            }
            endPt = UIComponent::getInstance()->m_ironIcon->convertToWorldSpace(Point::ZERO);
            picStr = "Iron";
            resType = Iron;
        }
        else if (itemId/1000 == FUN_BUILD_STONE || forceResType==Stone) {
            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_mithril);
//            endPt = ccp(430,-40);
//            if(CCCommonUtils::isIosAndroidPad())
//            {
//                endPt = ccpMult(endPt, 2.4);
//            }
            endPt = UIComponent::getInstance()->m_stoneIcon->convertToWorldSpace(Point::ZERO);
            picStr = "Stone";
            resType = Stone;
        }
        
        srand((unsigned)time(0));
//        int tmp = output*10/FunBuildController::getInstance()->getMaxOutPut(itemId);
        int tmp = output*10/1000;
        tmp = MAX(tmp, 2);
        tmp = MIN(tmp, 10);
        int idx = 0;
        while (tmp>0) {
            
//            auto flyCell = FlyCell::create(pt, endPt, picStr, 5, m_collectBatch, idx*0.2);
//            m_collectNode->addChild(flyCell);
            
            auto flyCell = FlyCell::create(pt, endPt,resType, picStr, 5, UIComponent::getInstance()->m_collectBatch, idx*0.2);//fusheng 创建一个资源采集动画  加入到UIComponent::getInstance()->m_collectBatch中
            UIComponent::getInstance()->m_collectNode->addChild(flyCell);
            
//            flyOutPut(itemId, m_flyNodes[tmp-1], idx);
            tmp--;
            idx++;
        }
    }
}

void ImperialScene::onFlyCargoOutPut(CCObject* obj)
{
    return;
    if (!m_cargoBuild || !m_cargoNode) {
        return;
    }
    auto dict = dynamic_cast<CCDictionary*>(obj);
    if (dict->objectForKey("reward")) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        int startPosx = m_cargoNode->getPositionX() + m_cargoBuild->mainWidth/2 ;
        int startPosy = m_cargoNode->getPositionY() + m_cargoBuild->mainHeight/2;
        auto pt = onGetSceenPt(startPosx, startPosy);
        pt.y = pt.y-winSize.height;
        
        string picStr = "Food";
        auto endPt = ccp(0,0);
        int resType = Wood;
        
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        int rwdCnt = arr->count();
        if (rwdCnt < 1) {
            return;
        }
        CCDictionary* tdic;
        for (int i = 0; i < 1; ++i) {
            tdic = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
            int type = tdic->valueForKey("type")->intValue();
            if (type == R_FOOD || type == R_WOOD || type == R_IRON || type == R_STONE) {
                switch (type) {
                    case R_FOOD:
                        endPt = ccp(200,-60);//winSize.height
                        picStr = "Food";
                        resType = Food;
                        break;
                        
                    case R_WOOD:
                        endPt = ccp(90,-40);
                        picStr = "Wood";
                        resType = Wood;
                        
                    case R_IRON:
                        endPt = ccp(320,-40);
                        picStr = "Iron";
                        resType = Iron;
                        
                    case R_STONE:
                        endPt = ccp(430,-40);
                        picStr = "Stone";
                        resType = Stone;
                        
                    default:
                        break;
                }
                int num = tdic->valueForKey("value")->intValue();
                int tmp = num*10/1000;
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
    }
}

void ImperialScene::onUpdateInfo()
{
    map<int,FunBuildInfo>::iterator it;
    it = FunBuildController::getInstance()->curBuildsInfo->begin();
    for (; it != FunBuildController::getInstance()->curBuildsInfo->end(); it++)
    {
        if (it->first>0 && ((it->second).state != FUN_BUILD_LOCK) && (it->second).pos<BUILD_COUNT)
        {
            if(it->second.level==0) {
                it->second.level=1;
            }
            FunBuild* build = FunBuild::create( it->first , m_nameLayer); //ljf, 此时funbuild的itemId为ItemSpec id="9990"字段
            build->setTag((it->second).pos);
            m_nodeBuildings[(it->second).pos]->addChild(build);
            m_nodeBuildings[(it->second).pos]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
            
            CCSpriteBatchNode* curBatch = m_resbatchNode;
            CCSpriteBatchNode* curBlentBatch = m_resBlentBatchNode;
            if (m_buildBatchMap.find((it->second).pos) != m_buildBatchMap.end()) {
                curBatch = m_buildBatchMap[(it->second).pos];
                curBlentBatch = NULL;
            }
            
            int od = m_nodeBuildings[(it->second).pos]->getZOrder();
            build->setNamePos(m_nodeBuildings[(it->second).pos]->getPositionX()
                              , m_nodeBuildings[(it->second).pos]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, curBatch, od, curBlentBatch);
            if (m_buildSpineMap.find((it->second).pos) != m_buildSpineMap.end()) {
                build->setSpineLayer( m_buildSpineMap[(it->second).pos] ); //a by ljf
            }
            
            m_buildItems[it->first] = build;
            build->m_key = 1000-od;
            
            if ((it->second).type == FUN_BUILD_TRAINFIELD) {
                onCreateVikingsShip();
            }
        }
    }
    
    for (int i = 0; i<BUILD_COUNT; i++) {
        int cnt = m_nodeBuildings[i]->getChildrenCount();
        if (cnt<=0) {
            //加入地块
            FunBuild* build = FunBuild::create( i , m_nameLayer);  //ljf, 此时funbuild的itemId为position字段
            if (build) {
                build->setTag(i);
                m_nodeBuildings[i]->addChild(build);
                
                m_nodeBuildings[i]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
                int od = m_nodeBuildings[i]->getZOrder();
                m_nodeBuildings[i]->setZOrder(od);
                
                build->setTileBatch(m_nodeBuildings[i]->getPositionX(), m_nodeBuildings[i]->getPositionY(), m_resbatchNode, od);
                build->m_key = 1000-od;
                //begin a by ljf
                if (m_buildSpineMap.find(i) != m_buildSpineMap.end()) {
                    build->setSpineLayer( m_buildSpineMap[(it->second).pos] ); //a by ljf
                }
                //end a by ljf
            }
        }
    }
}

int ImperialScene::findCanBuildTile(int pos)
{
    if (pos>=0 && pos<BUILD_COUNT) {
        FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
        if (build && build->m_buildingKey == pos) {
            return pos;
        }
        else {
            if (pos>16 && pos!=52 && pos!=53 && pos!=54) {
                for (int i=17; i<BUILD_COUNT; i++) {
                    if (i>=52 && i<=54) {
                        continue;
                    }
                    
                    FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
                    if (build && build->m_buildingKey == i) {
                        auto& tileInfo = FunBuildController::getInstance()->m_tileMap[i];
                        if (!(tileInfo.state == FUN_BUILD_LOCK || tileInfo.xmlOpen==1)) {
                            return i;
                        }
                    }
                }
            }
            else {
                for (int i=0; i<=16; i++) {
                    FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
                    if (build && build->m_buildingKey == i) {
                        auto& tileInfo = FunBuildController::getInstance()->m_tileMap[i];
                        if (!(tileInfo.state == FUN_BUILD_LOCK || tileInfo.xmlOpen==1)) {
                            return i;
                        }
                    }
                }
                
                if (true) {
                    for (int i=52; i<=54; i++) {
                        FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
                        if (build && build->m_buildingKey == i) {
                            auto& tileInfo = FunBuildController::getInstance()->m_tileMap[i];
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

void ImperialScene::onOpenBuildEnd(int itemId)
{
    if (m_buildItems.find(itemId) == m_buildItems.end()) {
        onCreateBuild(itemId);
    }
    m_buildItems[itemId]->changeBg();
}

void ImperialScene::onCreateBuild(int itemId)
{
    int pos = itemId%1000;
    
    FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
    if (build) {
        build->initFunBuild(itemId, m_nameLayer);
        CCSpriteBatchNode* curBatch = m_resbatchNode;
        CCSpriteBatchNode* curBlentBatch = m_resBlentBatchNode;
        if (m_buildBatchMap.find(pos) != m_buildBatchMap.end()) {
            curBatch = m_buildBatchMap[pos];
            curBlentBatch = NULL;
        }
        int od = m_nodeBuildings[pos]->getZOrder();
        build->setNamePos(m_nodeBuildings[pos]->getPositionX()
                          , m_nodeBuildings[pos]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, curBatch, od, curBlentBatch);
        
        m_buildItems[itemId] = build;
        build->m_key = 1000-od;
    }
}

void ImperialScene::onOpenBuild(int itemId)
{
}

CCNode* ImperialScene::getBuildById(int itemId){
    if (itemId == SPE_BUILD_NEWED || itemId == SPE_BUILD_MERCHANTE) {
        if (m_newRDBuild && itemId == SPE_BUILD_NEWED) {
            return m_newRDBuild->m_guideNode;
        }else if (m_merchanteBuild && itemId == SPE_BUILD_MERCHANTE) {
            return m_merchanteBuild->m_guideNode;
        }else {
            return NULL;
        }
    }
    if(m_buildItems.size() == 0){
        return NULL;
    }
    int id = FunBuildController::getInstance()->getMaxLvBuildByType(itemId);
    
    map<int, FunBuild*>::iterator it = m_buildItems.find(id);
    if(it != m_buildItems.end()){
        if(itemId == FUN_BUILD_FOOD) {
            setUnMoveScence(true);
            return m_buildItems[id]->m_spr;
        }
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
        FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[itemId]->getChildByTag(itemId));
        setUnMoveScence(true);
        return build->m_tile;
    }
    return NULL;
}

void ImperialScene::onFinishMoveBuild(CCObject* obj)
{
    CCDictionary* posData = dynamic_cast<CCDictionary*>(obj);
    if (posData) {
        int oldBuild1 = posData->valueForKey("oldBuild1")->intValue();
        int oldBuild2 = posData->valueForKey("oldBuild2")->intValue();
        int newBuild1 = posData->valueForKey("newBuild1")->intValue();
        int newBuild2 = posData->valueForKey("newBuild2")->intValue();
        int pos1 = posData->valueForKey("pos1")->intValue();
        int pos2 = posData->valueForKey("pos2")->intValue();
        
        if (oldBuild1>0 && m_buildItems.find(oldBuild1)!=m_buildItems.end()) {
            m_buildItems.erase(m_buildItems.find(oldBuild1));
        }
        if (oldBuild2>0 && m_buildItems.find(oldBuild2)!=m_buildItems.end()) {
            m_buildItems.erase(m_buildItems.find(oldBuild2));
        }
        
        FunBuild* delbuild1 = dynamic_cast<FunBuild*>(m_nodeBuildings[pos1]->getChildByTag(pos1));
        delbuild1->onBuildDelete();
        m_nodeBuildings[pos1]->removeChild(delbuild1);
        FunBuild* delbuild2 = dynamic_cast<FunBuild*>(m_nodeBuildings[pos2]->getChildByTag(pos2));
        delbuild2->onBuildDelete();
        m_nodeBuildings[pos2]->removeChild(delbuild2);
        
        if (newBuild1>0) {
            FunBuild* build = FunBuild::create( newBuild1 , m_nameLayer);
            build->setTag(pos2);
            m_nodeBuildings[pos2]->addChild(build);
            m_nodeBuildings[pos2]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
            m_buildItems[newBuild1] = build;
            
            CCSpriteBatchNode* curBatch = m_resbatchNode;
            CCSpriteBatchNode* curBlentBatch = m_resBlentBatchNode;
            if (m_buildBatchMap.find(pos2) != m_buildBatchMap.end()) {
                curBatch = m_buildBatchMap[pos2];
                curBlentBatch = NULL;
            }
            
            curBatch->setPosition(ccp(0, 0));
            int od = m_nodeBuildings[pos2]->getZOrder();
            build->m_key = 1000-od;
            build->setNamePos(m_nodeBuildings[pos2]->getPositionX()
                              , m_nodeBuildings[pos2]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, curBatch, od, curBlentBatch);
            build->onPlayUpEnd(true);
        }else {
            FunBuild* build = FunBuild::create( pos2 , m_nameLayer);
            build->setTag(pos2);
            m_nodeBuildings[pos2]->addChild(build);
            m_nodeBuildings[pos2]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
            int od = m_nodeBuildings[pos2]->getZOrder();
            m_nodeBuildings[pos2]->setZOrder(od);
            build->setTileBatch(m_nodeBuildings[pos2]->getPositionX(), m_nodeBuildings[pos2]->getPositionY(), m_resbatchNode, od);
            build->m_key = 1000-od;
        }
        
        if (newBuild2>0) {
            FunBuild* build = FunBuild::create( newBuild2 , m_nameLayer);
            build->setTag(pos1);
            m_nodeBuildings[pos1]->addChild(build);
            m_nodeBuildings[pos1]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
            m_buildItems[newBuild2] = build;
            
            CCSpriteBatchNode* curBatch = m_resbatchNode;
            CCSpriteBatchNode* curBlentBatch = m_resBlentBatchNode;
            if (m_buildBatchMap.find(pos1) != m_buildBatchMap.end()) {
                curBatch = m_buildBatchMap[pos1];
                curBlentBatch = NULL;
            }
            
            curBatch->setPosition(ccp(0, 0));
            int od = m_nodeBuildings[pos1]->getZOrder();
            build->m_key = 1000-od;
            build->setNamePos(m_nodeBuildings[pos1]->getPositionX()
                              , m_nodeBuildings[pos1]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, curBatch, od, curBlentBatch);
            build->onPlayUpEnd(true);
        }else {
            FunBuild* build = FunBuild::create( pos1 , m_nameLayer);
            build->setTag(pos1);
            m_nodeBuildings[pos1]->addChild(build);
            m_nodeBuildings[pos1]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
            int od = m_nodeBuildings[pos1]->getZOrder();
            m_nodeBuildings[pos1]->setZOrder(od);
            build->setTileBatch(m_nodeBuildings[pos1]->getPositionX(), m_nodeBuildings[pos1]->getPositionY(), m_resbatchNode, od);
            build->m_key = 1000-od;
        }
    }
}

void ImperialScene::setUnMoveScence(bool st)
{
    if(m_viewPort && m_viewPort->notMove!=st){
        m_viewPort->notMove = st;
    }
}

void ImperialScene::showBuildBtns(int itemId)
{
    map<int, FunBuild*>::iterator it = m_buildItems.find(itemId);
    if(it != m_buildItems.end()){
        lastTouchBuildId = -1;
        m_buildItems[itemId]->onSetClickType();
        m_buildItems[itemId]->onClickThis(0);
    }
}
void ImperialScene::clickWorkshop(int itemId){
//    m_buildItems[itemId]->onClickThis(0);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
    setUnMoveScence(false);
//    if(m_viewPort && m_viewPort->notMove){
//        m_viewPort->notMove = false;
//    }
    onMoveToBuildAndOpen(itemId, TYPE_POS_MID);
    PopupViewController::getInstance()->addPopupView(ToolCreateView::create());
}

CCNode* ImperialScene::getBuildNameById(int itemId, string _key)
{
    if(m_viewPort->notMove) {
        return NULL;
    }
    if (_key=="up" || _key=="detail" || _key=="soldier" || _key=="science" || _key=="forge") {
        return m_buildBtnsView->getGuideNode(_key);
    }
    else if (_key=="cd") {
        CCNode* ret=NULL;
        int id = FunBuildController::getInstance()->getMaxLvBuildByType(itemId);
        map<int, FunBuild*>::iterator it = m_buildItems.find(id);
        if(it != m_buildItems.end()){
            ret = m_buildItems[id]->getGuideNode(_key);
        }
        if (ret==NULL) {
            ret = m_buildBtnsView->getGuideNode(_key);
        }
        return ret;
    }
    else if (_key=="troop" || _key=="collect") {
        int id = FunBuildController::getInstance()->getMaxLvBuildByType(itemId);
        map<int, FunBuild*>::iterator it = m_buildItems.find(id);
        if(it != m_buildItems.end()){
            return m_buildItems[id]->getGuideNode(_key);
        }
    }
    return NULL;
}

void ImperialScene::onPowerADD(CCObject* obj)
{
    int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SCIENE);
    if (bid >0 ) {
        flyBall(bid, 1, false);
    }
}

void ImperialScene::flyBall(int buildId, int type, bool toScene)
{
    auto &info = (*FunBuildController::getInstance()->curBuildsInfo)[buildId];
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

void ImperialScene::flyBallEnd(CCNode* pSender)
{
    pSender->stopAllActions();
    pSender->setVisible(false);
}

void ImperialScene::flyOutPut(int buildId, CCNode* flNode, int idx)
{
    flNode->stopAllActions();
    flNode->removeAllChildren();
    
    auto &info = (*FunBuildController::getInstance()->curBuildsInfo)[buildId];
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
    //delaytime,
    auto  actionBy = CCSequence::create(moveBy,act,CCCallFuncN::create(this, callfuncN_selector(ImperialScene::flyOutPutEnd)), NULL);
    flNode->setPosition(ccp(sx, sy));
    flNode->setVisible(true);
    flNode->runAction(actionBy);
}

void ImperialScene::flyOutPutEnd(CCNode* pSender)
{
    pSender->setVisible(false);
    pSender->stopAllActions();
    pSender->removeAllChildren();
}

void ImperialScene::makeFlyMc(int type, CCNode* flNode)
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
   
    int ra = rand()%18 * 10;
    sprAni->setRotation(ra-180);
    sprAni->stopAllActions();
    sprAni->runAction(CCRepeatForever::create(animate));
    
    flNode->addChild(sprAni);
}

void ImperialScene::initMc2()
{
    bool flag = false;
    if (ActivityController::getInstance()->activityArr && ActivityController::getInstance()->activityArr->count()>0) {
        int num = ActivityController::getInstance()->activityArr->count();
        for (int i=0; i<num; i++) {
            auto temp = dynamic_cast<ActivityEventObj*>(ActivityController::getInstance()->activityArr->objectAtIndex(i));
            if (temp && temp->type==7) {
                flag = true;
                break;
            }
        }
    }
    // tao.yu 第一版本不开放人物对话
    if (false && flag) {
        int zOrder = m_mcNode2->getZOrder();
        auto pArray = CCPointArray::create(20);
        pArray->addControlPoint(ccp(7.0, 0));
        pArray->addControlPoint(ccp(36.0, 16.0));
        pArray->addControlPoint(ccp(68, 34));
        pArray->addControlPoint(ccp(90.0, 20));
        pArray->addControlPoint(ccp(60.0, 5));
        pArray->addControlPoint(ccp(34.0, -12));
        pArray->addControlPoint(ccp(75.0, -25.0));
        pArray->addControlPoint(ccp(129.0, 4.0));
        for (int i=0; i<8; i++) {
            string sIcon = "soldier_Pikeman.png";
            if(i>5){
                sIcon = "soldier_banner.png";
            }
            auto bing = CCLoadSprite::createSprite(sIcon.c_str());
            bing->setPosition(pArray->getControlPointAtIndex(i) + m_crossNode->getPosition());
            m_resbatchNode->addChild(bing);
        }
        auto soldier_flag = CCLoadSprite::loadResource("soldier_flag.png");
        CCFlagWaveSprite* flag = CCFlagWaveSprite::create(soldier_flag);
        m_soldierFlagNode->addChild(flag);
        flag->setAnchorPoint(ccp(0, 1));
        flag->setFlipX(true);
        flag->setScale(0.2);
        flag->setPosition(ccp(49, 9) + m_crossNode->getPosition());
        
        auto soldier_flag2 = CCLoadSprite::loadResource("soldier_flag.png");
        CCFlagWaveSprite* flag2 = CCFlagWaveSprite::create(soldier_flag2);
        m_soldierFlagNode->addChild(flag2);
        flag2->setAnchorPoint(ccp(0, 1));
        flag2->setFlipX(true);
        flag2->setScale(0.2);
        flag2->setPosition(ccp(103, 39) + m_crossNode->getPosition());
        
        auto pArr = CCPointArray::create(20);
        pArr->addControlPoint(ccp(1, 0));
        pArr->addControlPoint(ccp(1, 0));
        
        m_pt2Array = CCPointArray::create(20);
        //pArray->addControlPoint(ccp(1.0, 0));
        //pArray->addControlPoint(ccp(0, 1.0));
        auto person = Person::create(m_crossNode->getPositionX() + 30,m_crossNode->getPositionY() -30 , m_resbatchNode, zOrder*1000, m_pt2Array, 12,1);
        m_personArray->addObject(person);
        person->onSetType(1);
    }
    
    if (0) {
        int zOrder = m_mcNode2->getZOrder();
        m_ptArray = CCPointArray::create(20);
        m_ptArray->addControlPoint(ccp(400, -200));
        m_ptArray->addControlPoint(ccp(-400, 200));
        
        for (int i=0; i<5; i++) {
            auto person = Person::create(m_mcNode2->getPositionX()-45+i*20,m_mcNode2->getPositionY()-45+i*(-10), m_resbatchNode, zOrder*1000, m_ptArray, 15);
            m_personArray->addObject(person);
            if(i==0) {
                person->onSetType(0);
            }
        }
    }
    
    if (0) {
        int zOrder = m_mcNode4->getZOrder();
        auto effSpr0 = CCSprite::create();
        CCCommonUtils::makeTwoAnimatSpr(effSpr0, "tanhua_%d.png", 7, 1, 0.2 );
        effSpr0->setPosition(m_mcNode4->getPosition());
        m_resbatchNode->addChild(effSpr0);
        effSpr0->setZOrder(zOrder*1000);
    }
    
    if (0) {
        int zOrder = m_mcNode5->getZOrder();
        auto effSpr0 = CCSprite::create();
        CCCommonUtils::makeTwoAnimatSpr(effSpr0, "tanhua_%d.png", 7, 1, 0.2 );
        effSpr0->setPosition(m_mcNode5->getPosition());
        m_resbatchNode->addChild(effSpr0);
        effSpr0->setZOrder(zOrder*1000);
        
        
    }
    
    // tao.yu 暂时不开放feedback功能
    if (1) {
        int zOrder = m_desNode1->getZOrder();
        m_mailBuild = SpeBuild::create(SPE_BUILD_MAIL);
        m_desNode1->addChild(m_mailBuild);
        m_mailBuild->setNamePos(m_desNode1->getPositionX(), m_desNode1->getPositionY(), m_signLayer, m_arrbatchNode, m_resbatchNode, zOrder);
        
        int m = 5;
        
        m_HTPHead = CCLoadSprite::createSprite("feedback_head.png");
        CCSize size = m_HTPHead->getContentSize();
        std::string language = LocalController::shared()->getLanguageFileName();
        m_HTPHead->setScaleX(-1.5);//-2
        if (language=="id"||language=="ru") {
            m_HTPHead->setScaleX(-1.6);
        }
        if (language=="zh_CN"||language=="zh_TW") {
            m_HTPHead->setScaleX(-0.8);
        }
        m_HTPHead->setScaleY(0.6);
        m_HTPHead->setPosition(m_desNode1->getPosition()+ccp(-50+50, 90-10));
        m_resbatchNode->addChild(m_HTPHead);
        m_HTPHead->setZOrder(zOrder*1000+1+m);
        
        m_HTPHead1 = CCLoadSprite::createSprite("feedback_head1.png");
        m_HTPHead1->setPosition(m_desNode1->getPosition()+ccp(-30+30, 51-11));
        m_HTPHead1->setScaleX(-1);
        m_resbatchNode->addChild(m_HTPHead1);
        m_HTPHead1->setZOrder(zOrder*1000+2+m);
        
        m_desNode1->setContentSize(CCSizeMake(150, 150));
        m_HTPlabel = CCLabelIF::create();
        m_HTPlabel->setString(_lang("101230")+_lang("101231"));//todo:放到语言文件中
        m_HTPlabel->setColor({108,58,25});
        m_HTPlabel->setAnchorPoint(ccp(0.5,0.5));
        m_cloudLayer->addChild(m_HTPlabel);
        m_HTPlabel->setPosition(m_HTPHead->getPosition()+ccp(0,0));
        
        m_icon_op_mail = CCLoadSprite::createSprite("icon_op_mail.png");
        m_icon_op_mail->setScale(1.2);
        m_icon_op_mail->setPosition(m_HTPHead->getPosition());
        m_resbatchNode->addChild(m_icon_op_mail);
        m_icon_op_mail->setZOrder(zOrder*1000+2+m);
        
        CCSequence* sequ = CCSequence::create(CCScaleTo::create(0.8, 1.3),CCScaleTo::create(1, 1.0), NULL);
        m_icon_op_mail->runAction(CCRepeatForever::create(sequ));
        
        m_icon_op_mail->setVisible(false);

        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowHTPAnim), this);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowHTPAnim), this, 0.2,kCCRepeatForever, 0.0f, false);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        m_noticeCount = HelpshiftCocos2dx::getNotificationCount(false,NULL);
#else
        m_noticeCount = HelpshiftCocos2dx::getNotificationCountFromRemote(false,NULL);
#endif
        if(m_noticeCount>0){
            m_HTPHead->setScaleX(-0.9);
        }
        m_mailBuild->getAnimationManager()->runAnimationsForSequenceNamed("Default");
    }
}

#pragma mark -
#pragma mark CocosBuilder Part
bool ImperialScene::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    if (pTarget == this && strncmp(pMemberVariableName, "m_node",6) == 0) {
        char index[5] = "";
        strncpy(index, pMemberVariableName + 6, strlen(pMemberVariableName) - 6);
        
        int cityIndex = atoi(index);
//        CCAssert(cityIndex < BUILD_COUNT,"Node citys could not more than m_nodeCity!");
        if (cityIndex < BUILD_COUNT) {
            m_nodeBuildings[cityIndex] = pNode;
            m_nodeBuildings[cityIndex]->retain();
        }
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_bigTileNode",13) == 0) {
        char index[5] = "";
        strncpy(index, pMemberVariableName + 13, strlen(pMemberVariableName) - 13);
        int idx = atoi(index);
        m_bigTileNodes[idx] = pNode;
        return true;
    }
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vikingNode", CCNode*, this->m_vikingNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mcNode1", CCNode*, this->m_mcNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mcNode2", CCNode*, this->m_mcNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mcNode3", CCNode*, this->m_mcNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mcNode4", CCNode*, this->m_mcNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mcNode5", CCNode*, this->m_mcNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode1", CCNode*, this->m_desNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode2", CCNode*, this->m_desNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode3", CCNode*, this->m_desNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode", CCNode*, this->m_torchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode1", CCNode*, this->m_torchNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode2", CCNode*, this->m_torchNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode3", CCNode*, this->m_torchNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode4", CCNode*, this->m_torchNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode5", CCNode*, this->m_torchNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode6", CCNode*, this->m_torchNode6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode7", CCNode*, this->m_torchNode7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode8", CCNode*, this->m_torchNode8);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagNode", CCNode*, this->m_flagNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hdNode", CCNode*, this->m_hdNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wallNode", CCNode*, this->m_wallNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_FountainTouchNode", CCNode*, this->m_FountainTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldMineNode", CCNode*, this->m_goldMineNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldBoxNode", CCNode*, this->m_goldBoxNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shipNode", CCNode*, this->m_shipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_failEffectNode", CCNode*, this->m_failEffectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_merchanteNode", CCNode*, this->m_merchanteNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cargoNode", CCNode*, this->m_cargoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newRDNode", CCNode*, this->m_newRDNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lotteryNode", CCNode*, this->m_lotteryNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_xiongdiNode", CCNode*, this->m_xiongdiNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_forestNode", CCNode*, this->m_forestNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_couldNode", CCNode*, this->m_couldNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crossNode", CCNode*, this->m_crossNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crossClickNode", CCNode*, this->m_crossClickNode);
    
    // titan node
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanNode", CCNode*, this->m_titanNode);
    // titan move path
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tpath_1", CCNode*, this->m_tpath_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tpath_2", CCNode*, this->m_tpath_2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tpath_3", CCNode*, this->m_tpath_3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tpath_4", CCNode*, this->m_tpath_4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wallZOrder_0", CCNode*, this->m_wallZOrder_0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wallZOrder_1", CCNode*, this->m_wallZOrder_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wallZOrder_2", CCNode*, this->m_wallZOrder_2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wallZOrder_3", CCNode*, this->m_wallZOrder_3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wallZOrder_4", CCNode*, this->m_wallZOrder_4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wallZOrder_4", CCNode*, this->m_wallZOrder_4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waterfallNode", CCNode*, this->m_waterfallNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waterfall_prt", CCNode*, this->m_waterfall_prt);
    //begin a by ljf
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vikingPath1", CCNode*, this->m_vikingPath1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vikingPath2", CCNode*, this->m_vikingPath2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vikingPath3", CCNode*, this->m_vikingPath3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vikingPath4", CCNode*, this->m_vikingPath4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vikingTouchNode", CCNode*, this->m_vikingTouchNode);
    
    //end a by ljf
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityBgNode", CCNode*, this->m_cityBgNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bridgeNode", CCNode*, this->m_bridgeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bridgeTouchNode", CCNode*, this->m_bridgeTouchNode);
    
    return false;
}

void ImperialScene::showAttention(){
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
    
    auto dict = _dict(LocalController::shared()->DBXMLManager()->getGroupByKey("building")->objectForKey("400000"));
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
        map<int, FunBuild*>::iterator it = m_buildItems.find(buildingId);
        if(it == m_buildItems.end()){
            return;
        }
        
        m_count = 0;
        m_viewPort->applyZoom(1);
        this->onMoveToBuild(buildingId);
        m_touchLayer->runAction(CCSequence::create(CCDelayTime::create(1.05)
                                                   , CCCallFunc::create(this, callfunc_selector(ImperialScene::showAttention1))
                                                   , NULL
                                                   ));
    }
}

bool ImperialScene::isGeneralOnFormationBuilding(int buildingId){
    return true;
}

void ImperialScene::showAttention1(){
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
                                               , CCCallFunc::create(this, callfunc_selector(ImperialScene::showAttention1))
                                               , NULL
                                               ));
}

void ImperialScene::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
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

void ImperialScene::addCloudParticleToBatch(cocos2d::CCParticleSystemQuad *particle, bool isTorch) {
    auto batchCount = m_cloudVec.size();
    if(isTorch){
        m_torchPar->addObject(particle);
    }
    while (batchCount--) {
        auto &batch = m_cloudVec[batchCount];
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
    m_cloudLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_cloudVec.push_back(newBatch);
}

void ImperialScene::makeWater(int type)
{
    m_waterLayer->removeAllChildren();
    m_parVec.clear();
    do {
        string tmpStart = "Fountain_";
        int count = 5;
        for (int i=1; i<count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            int tmpH = (3 - (i+1)/2)*20;
            particle->setPosition(ccp(1010, 1420+tmpH));
            addCloudParticleToBatch(particle);
        }
    } while (0);
}

void ImperialScene::showTmpBuild(int pos, int itemId)
{
    m_tmpBuildPos = pos;
    FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
    build->showTmpBuild(itemId);
}

void ImperialScene::hideTmpBuild(int pos)
{
    FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
    if(build){
        build->hideTmpBuild();
    }
}

void ImperialScene::initBigTile()
{
    // tao.yu 不初始化树
    
    if (m_exit) {
        return;
    }
    
    /*
    map<int, BigTileInfo>::iterator it=FunBuildController::getInstance()->m_bigTileMap.begin();
    for (; it!=FunBuildController::getInstance()->m_bigTileMap.end(); it++) {
        if(it->second.state == FUN_BUILD_LOCK) {
            int pos = it->second.pos;
            if (m_bigTileNodes.find(pos) != m_bigTileNodes.end()) {
                if(m_bigTileNodes[pos]) {
                    SpeBuild* build = SpeBuild::create(it->first);
                    if (build) {
                        m_bigTileNodes[pos]->addChild(build);
                        
                        int od = m_bigTileNodes[pos]->getZOrder();
                        build->setNamePos(m_bigTileNodes[pos]->getPositionX()
                                          , m_bigTileNodes[pos]->getPositionY(), m_signLayer, m_arrbatchNode, m_tilebatchNode, od);
                        m_bigTiles[it->first] = build;
                        build->m_key = 1000-od;
                    }
                }
            }
       }
    }
    */
    
    m_flagBuild = SpeBuild::create(SPE_BUILD_FLAG);
    m_flagNode->addChild(m_flagBuild);
    int od = m_flagNode->getZOrder();
    m_flagBuild->setNamePos(m_flagNode->getPositionX(), m_flagNode->getPositionY(), m_signLayer, m_arrbatchNode, m_tilebatchNode, od);
    // tao.yu 第一版不开放活动
    if(false) {
        m_hdBuild = SpeBuild::create(SPE_BUILD_HD);
        m_hdNode->addChild(m_hdBuild);
        int hod = m_hdNode->getZOrder();
        m_hdBuild->setNamePos(m_hdNode->getPositionX(), m_hdNode->getPositionY(), m_signLayer, m_arrbatchNode, m_resbatchNode, hod);
    }
    
    if(false) {//false
        m_goldMineBuild = SpeBuild::create(SPE_BUILD_GOLD);
        m_goldMineNode->addChild(m_goldMineBuild);
        int hod = m_goldMineNode->getZOrder();
        m_goldMineBuild->setNamePos(m_goldMineNode->getPositionX(), m_goldMineNode->getPositionY(), m_signLayer, m_arrbatchNode, m_resbatchNode, hod);
    }
    // tao.yu 第一版不开放酒馆
    if(false) {// GlobalData::shared()->playerInfo.gmFlag==1
        m_goldBoxBuild = SpeBuild::create(SPE_BUILD_GOLD_BOX);
        m_goldBoxNode->addChild(m_goldBoxBuild);
        int hod = m_goldBoxNode->getZOrder();
        m_goldBoxBuild->setNamePos(m_goldBoxNode->getPositionX(), m_goldBoxNode->getPositionY(), m_signLayer, m_arrbatchNode, m_buildBatchMap[101], hod);
    }
    
    // tao.yu 第一版不开放码头
    if (false) {
        m_shipBuild = SpeBuild::create(SPE_BUILD_SHIP);
        m_shipNode->addChild(m_shipBuild);
        int hod = m_shipNode->getZOrder();
        m_shipBuild->setNamePos(m_shipNode->getPositionX(), m_shipNode->getPositionY(), m_signLayer, m_arrbatchNode, m_chrTreeBatchNode, hod);
        m_shipBuild->updateShipState();
    }
    // tao.yu 第一版不开放旅行商人
    if(false) {
        m_merchanteBuild = SpeBuild::create(SPE_BUILD_MERCHANTE);
        m_merchanteNode->addChild(m_merchanteBuild);
        int hod = m_merchanteNode->getZOrder();
        m_merchanteBuild->setNamePos(m_merchanteNode->getPositionX(), m_merchanteNode->getPositionY(), m_signLayer, m_arrbatchNode, m_resbatchNode, hod);
    }
    // tao.yu 第一版不开放邀请建筑
//    if(!GlobalData::shared()->isXiaoMiPlatForm()) {
    if(false) {
        m_xiongdiBuild = SpeBuild::create(SPE_BUILD_XIONGDI);
        m_xiongdiNode->addChild(m_xiongdiBuild);
        int hod = m_xiongdiNode->getZOrder();
        m_xiongdiBuild->setNamePos(m_xiongdiNode->getPositionX(), m_xiongdiNode->getPositionY(), m_signLayer, m_arrbatchNode, m_resbatchNode, hod);
    }
    if(CCCommonUtils::isTestPlatformAndServer("guide_3") && PortActController::getInstance()->m_newPalyerRDMap.size()>0 && PortActController::getInstance()->m_newLoginDay<7) {
        m_newRDBuild = SpeBuild::create(SPE_BUILD_NEWED);
        m_newRDNode->addChild(m_newRDBuild);
        int hod = m_newRDNode->getZOrder();
        m_newRDBuild->setNamePos(m_newRDNode->getPositionX(), m_newRDNode->getPositionY(), m_signLayer, m_arrbatchNode, m_resbatchNode, hod);
    }
    // tao.yu 第一版本不开放在线时间奖励
    if (false && PortActController::getInstance()->m_isNewTimeRwd) {
        m_cargoBuild = SpeBuild::create(SPE_BUILD_CARGO);
        m_cargoNode->addChild(m_cargoBuild);
        int hod = m_cargoNode->getZOrder();
        m_cargoBuild->setNamePos(m_cargoNode->getPositionX(), m_cargoNode->getPositionY(), m_signLayer, m_arrbatchNode, m_resbatchNode, hod);
        
//        IFDockWalloperNode* worker = IFDockWalloperNode::create();
//        if (worker) {
//            m_spineLayer->addChild(worker);
//        }
    }
    
    if(ActivityController::getInstance()->checkOpenDragonBattle()) {
        m_dragonBuild = SpeBuild::create(SPE_BUILD_DRAGON);
        m_dragonNode->addChild(m_dragonBuild);
        int hod = m_dragonNode->getZOrder();
        m_dragonBuild->setNamePos(m_dragonNode->getPositionX(), m_dragonNode->getPositionY(), m_signLayer, m_arrbatchNode, m_resbatchNode, hod);
    }
}

void ImperialScene::unLockTile(CCObject* params)
{
    if(params) {
        int itemId = dynamic_cast<CCInteger*>(params)->getValue();
        map<int, SpeBuild*>::iterator it = m_bigTiles.find(itemId);
        if (it != m_bigTiles.end()) {
            m_removeSpeBId = itemId;
            it->second->onPlayUnlock();
        }
    }
}

void ImperialScene::onRemoveSpeBuild()
{
    int itemId = m_removeSpeBId;
    map<int, SpeBuild*>::iterator it = m_bigTiles.find(itemId);
    if (it != m_bigTiles.end()) {
        it->second->onBuildDelete();
        int pos = FunBuildController::getInstance()->m_bigTileMap[itemId].pos;
        m_bigTileNodes[pos]->removeAllChildren();
        m_bigTiles.erase(it);
    }
}

void ImperialScene::refreashTile()
{
    map<int, BigTileInfo>::iterator it=FunBuildController::getInstance()->m_bigTileMap.begin();
    for (; it!=FunBuildController::getInstance()->m_bigTileMap.end(); it++) {
        if(it->second.state == FUN_BUILD_LOCK && it->second.level<=FunBuildController::getInstance()->getMainCityLv()) {
            int itemId = it->first;
            int pos = it->second.pos;
            
            map<int, SpeBuild*>::iterator tileIt = m_bigTiles.find(itemId);
            if (tileIt != m_bigTiles.end() && tileIt->second->m_isOpen==false)
            {
                tileIt->second->onBuildDelete();
                m_bigTileNodes[pos]->removeAllChildren();
                m_bigTiles.erase(tileIt);
                
                if (m_bigTileNodes.find(pos) != m_bigTileNodes.end()) {
                    
                    SpeBuild* build = SpeBuild::create(it->first);
                    m_bigTileNodes[pos]->addChild(build);
                    
                    int od = m_bigTileNodes[pos]->getZOrder();
                    build->setNamePos(m_bigTileNodes[pos]->getPositionX()
                                      , m_bigTileNodes[pos]->getPositionY(), m_signLayer, m_arrbatchNode, m_tilebatchNode, od);
                    m_bigTiles[it->first] = build;
                    build->m_key = 1000-od;
                }
            }
        }
    }
}

void ImperialScene::initBgTree()
{
    // tao.yu 下面这段代码毛用没有 tmpSpr是个null 原因是couldArr的size只有1 具体看ccb的cloudnode
//    auto & couldArr = m_couldNode->getChildren();
//    if (true) {
//        for (int i=0; i<couldArr.size(); i++)
//        {
//            CCSprite* tmpSpr = dynamic_cast<CCSprite*>(couldArr.at(i));
//            if (tmpSpr) {
//                tmpSpr->retain();
//                tmpSpr->removeFromParent();
//                tmpSpr->getTexture()->setAntiAliasTexParameters();
//                tmpSpr->setPosition(tmpSpr->getPosition());
//                m_cludeBatchNode->addChild(tmpSpr);
//                tmpSpr->release();
//                i--;
//            }
//        }
//    }
//    this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
}

void ImperialScene::onShowBtnsView(int x, int y, int buildId)
{
//    m_buildBtnsView->setVisible(true);
    int buildType = buildId/1000;
    switch (buildType) {
        case FUN_BUILD_MAIN:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_castle);
            break;
        case FUN_BUILD_BARRACK1://barrack步兵
            SoundController::sharedSound()->playEffects(Music_Sfx_city_barrack);
            break;
        case FUN_BUILD_BARRACK2:{//stable骑兵
            SoundController::sharedSound()->playEffects(Music_Sfx_city_stable);
            break;
        }
        case FUN_BUILD_BARRACK3://range弓箭手
            SoundController::sharedSound()->playEffects(Music_Sfx_city_range);
            break;
        case FUN_BUILD_BARRACK4://chario plant 战斗车
            SoundController::sharedSound()->playEffects(Music_Sfx_city_chariot);
            break;
        case FUN_BUILD_CELLAR://仓库
            SoundController::sharedSound()->playEffects(Music_Sfx_city_depot);
            break;
        case FUN_BUILD_FORT://
            SoundController::sharedSound()->playEffects(Music_Sfx_city_fortress);
            break;
        case FUN_BUILD_TAVERN://市场
            SoundController::sharedSound()->playEffects(Music_Sfx_city_market);
            break;
        case FUN_BUILD_STABLE://Embassy
            SoundController::sharedSound()->playEffects(Music_Sfx_city_embassy);
            break;
        case FUN_BUILD_SCIENE:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_college);
            break;
        case FUN_BUILD_STONE:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_mithril);
            break;
        case FUN_BUILD_WOOD:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_wood);
            break;
        case FUN_BUILD_IRON:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_iron);
            break;
        case FUN_BUILD_FOOD:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_farm);
            break;
        case FUN_BUILD_HOSPITAL:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_hospital);
            break;
        case FUN_BUILD_WATCH_TOWER:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_watchtower);
            break;
        case FUN_BUILD_ARROW_TOWER:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_turret);
            break;
        case FUN_BUILD_WALL:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_walls);
            break;
        case FUN_BUILD_BARRACK://兵营帐篷
            SoundController::sharedSound()->playEffects(Music_Sfx_city_military);//
            break;
        case FUN_BUILD_SMITHY:{
            SoundController::sharedSound()->playEffects(Music_Sfx_city_hall);//
            break;
        }
        case FUN_BUILD_TRAINFIELD:{
            SoundController::sharedSound()->playEffects(Music_Sfx_city_drill);
            break;
        }
        case FUN_BUILD_FORGE:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_blacksmith);
            break;
        case FUN_BUILD_SACRIFICE:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_wishing);
            break;
        default:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
            break;
    }
    
    m_buildBtnsView->setPosition(ccp(x,y));
    m_buildBtnsView->onShow(buildId);
}

void ImperialScene::onShowSpeBtnsView(int x, int y, int buildId)
{
    if (m_speBuildBtnsView->isVisible() && buildId == m_speBuildBtnsView->m_buildingKey) {
        m_speBuildBtnsView->onHide();
        return;
    }
    m_speBuildBtnsView->setPosition(ccp(x,y));
    m_speBuildBtnsView->onShow(buildId);
}

void ImperialScene::onFlyTroopParticle(int x, int y, int onlyPower)
{
    m_flyX = x;
    m_flyY = y;
    m_isOnlyPower = onlyPower;
    flyCount = 1;
    playFlyTroopParticle(0);
    this->schedule(schedule_selector(ImperialScene::playFlyTroopParticle), 0.5);
    this->scheduleOnce(schedule_selector(ImperialScene::playPowerAni), 0.1);
}

void ImperialScene::playFlyTroopParticle(float _time){
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
                
                auto func = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onFlyTroopEnd), particle);
                
                CCActionInterval * moveTo = CCMoveTo::create(1,endPoint);
                CCActionInterval * easeExponentialIn= CCEaseExponentialOut::create(moveTo);
                
                CCActionInterval * delT = CCDelayTime::create(0.5);
                CCActionInterval * fadeOut = CCFadeOut::create(0.2);
                if (flyCount == 0 && i==count) {
                    auto funcPowerUI = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onFlyTroopEndToUI), CCString::create("power"));
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
                auto func = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onFlyTroopEnd), particle);
                
                if (flyCount == 0 && i==count) {
                    auto funcUI = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onFlyTroopEndToUI), CCString::create("exp"));
                    CCActionInterval * seqFadeOut = CCSequence::create(delT, funcUI, fadeOut, func, NULL);
                    particle->runAction(CCSpawn::create(easeExponentialIn,seqFadeOut,NULL));
                }else {
                    CCActionInterval * seqFadeOut = CCSequence::create(delT, fadeOut, func, NULL);
                    particle->runAction(CCSpawn::create(easeExponentialIn,seqFadeOut,NULL));
                }
            }
        }

    }else{
        this->unschedule(schedule_selector(ImperialScene::playFlyTroopParticle));
        flyCount = 0;
    }
}

void ImperialScene::onFlyTroopEnd(CCObject *obj)
{
    CCNode* node = dynamic_cast<CCNode*>(obj);
    if (node) {
        node->stopAllActions();
        node->removeFromParent();
    }
}

void ImperialScene::onFlyTroopEndToUI(CCObject *obj)
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

void ImperialScene::playPowerAni(float _time){
    
}

void ImperialScene::refreshSoldiers(CCObject* obj)
{
    if (!m_soldierBatchNode) {
        return;
    }
    m_soldierBatchNode->removeAllChildren();
    m_soldierArray->removeAllObjects();
    
    FunBuildController::getInstance()->lastSqerPos.clear();
    int numPerSqer = 3600/10;
    int sqerIdx = 0;
    vector<int> soldierSqerNum = ArmyController::getInstance()->getSoldierSqerNum();//返回各兵种方阵数量，依次为 骑， 步， 弓， 车
//    vector<int> soldierSqerNum;
//    soldierSqerNum.push_back(7);
//    soldierSqerNum.push_back(7);
//    soldierSqerNum.push_back(7);
//    soldierSqerNum.push_back(9);
//    soldierSqerNum.push_back(301);
//    soldierSqerNum.push_back(301);
//    soldierSqerNum.push_back(301);
//    soldierSqerNum.push_back(301);
    for (int i=0; i<4; i++)
    {
        int sType = i+1;
        if (i==0) {
            sType = 2;
        }
        else if (i==1) {
            sType = 1;
        }
        else if (i==2) {
            sType = 3;
        }
        else if (i==3) {
            sType = 4;
        }
        
        int soldierCnt = soldierSqerNum[i+4];
        int sqerCnt = soldierSqerNum[i];
        if (i==3) {
            if (sqerIdx>=0 && sqerIdx<6) {
                if(sqerIdx>0 && sqerCnt<=(7+7+6+4)) {
                    sqerIdx = 6;
                }
            }
            else if (sqerIdx>=6 && sqerIdx<13) {
                if(sqerIdx>6 && sqerCnt<=(7+6+4)) {
                    sqerIdx = 13;
                }
            }
            else if (sqerIdx>=13 && sqerIdx<20) {
                if(sqerIdx>13 && sqerCnt<=(6+4)) {
                    sqerIdx = 20;
                }
            }
            else if (sqerIdx>=20 && sqerIdx<26) {
                if(sqerIdx>20 && sqerCnt<=4) {
                    sqerIdx = 26;
                }
            }
            else if (sqerIdx>=26) {
                //车兵已经到最后一排了
            }
        }
        
        for (int j=0; j<sqerCnt; j++) {
            int sqrSoldierNum = numPerSqer;
            if (soldierCnt>=numPerSqer) {
                soldierCnt -= numPerSqer;
            }else {
                sqrSoldierNum = soldierCnt;
            }
            auto pt = FunBuildController::getInstance()->sqerPoints[sqerIdx];
            
            FunBuildController::getInstance()->lastSqerPos[sType] = sqerIdx;
            addSoldierToMap(sType, sqrSoldierNum, pt.x, pt.y);
            sqerIdx++;
        }
    }
    
    for (int st=1; st<=4; st++) {
        if (FunBuildController::getInstance()->lastSqerPos.find(st) == FunBuildController::getInstance()->lastSqerPos.end()) {
            FunBuildController::getInstance()->lastSqerPos[st] = sqerIdx;
            sqerIdx++;
        }
    }
}

void ImperialScene::addSoldierToMap(int stype, int num, int ptx, int pty)
{
    float r_s = 1.5;
    float c_s = 1.5;
    // tao.yu 场景中的兵阵
    int rowWidth = 30 * r_s;//不同行x偏移
    int colHeight = 20 * c_s;//不同行y偏移
    int perX = 30 * r_s;//同一行，每一个兵的x偏移
    int perY = 20 * c_s;//同一行，每一个兵的y偏移
    int preNum = 3*200/10;
    int m_row = 6;
    int m_col = 3;
    string m_icon = "";
    float scale = 1;
    float shadow_scale = 0.5;
    
    int tX = 0;
    int tY = 0;
    
    switch (stype) {
        case 1://步兵
            m_icon = "a010";
            preNum = 4*200/10;
            break;
        case 5://弓兵
            m_icon = "a060";
            preNum = 4*700/10;
            shadow_scale = 0.75 * 0.65;
            break;
        case 2://骑兵
            rowWidth = 30 * r_s;
            colHeight = 15 * c_s;
//            perX = 15;
//            perY = 7;
            m_row = 4;
            m_col = 2;
            m_icon = "a020";
            preNum = 3*450/10;
            shadow_scale = 0.65;
//            scale = 1;
            break;
        case 4://战车
            m_row = 3;
            m_col = 1;
//            perX = 20;
//            perY = 10;
            m_icon = "zhanche";
            preNum = 4*300/10;
//            scale = 1;
            tX = 0;
            tX = -20;
            break;
        case 3://长弓兵
            m_icon = "a060";
            shadow_scale = 0.75 * 0.65;
            break;
        default:
            m_icon = "a020";
            shadow_scale = 0.65;
            break;
    }
    
    int sNum = ceil(num*1.0/preNum);
    
//    sNum = 18;
//    if (stype == 4) {
//        sNum = 3;
//    }
//    else if (stype == 2) {
//        sNum = 8;
//    }
    
    srand((unsigned int)time(0));
    for (int i=0; i<sNum; i++) {
        int row = i%m_row;//某排上第几位
        int col = i/m_row;//第几排
        CCPoint endPos = ccp(ptx+row*perX-col*rowWidth,pty-row*perY-col*colHeight);
        
        BattleSoldier2* soldier = BattleSoldier2::create(m_soldierBatchNode, NULL,0,0,m_icon,"SW",false);
        soldier->setSoldierPosition(endPos+ccp(tX, tY));
        soldier->setAnchorPoint(ccp(0.5, 0.5));
        soldier->setSprScale(scale);
        soldier->getShadow()->setScale(shadow_scale);
        soldier->playAnimation(ActionStatus::ACTION_STAND);
        m_soldierArray->addObject(soldier);
    }
    
}

void ImperialScene::scheduleHarvestEffect(float _time)
{
    showHarvestEffect(NULL);
}

void ImperialScene::showHarvestEffect(CCObject* obj){
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
    
    FunBuildController::getInstance()->addOutPut(outPutSec);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_SKILL_HARVEST,CCString::create(CC_ITOA(outPutSec)));
}

void ImperialScene::playFountain()
{
    auto node = CCNode::create();
    int tag = m_fountainNode->getChildrenCount();
    node->setTag(tag);
    m_fountainNode->addChild(node);
    
    struct timeval  tm;
    gettimeofday(&tm, NULL);
    int randIdx = tm.tv_usec%10;
    int start = 1;
    int end = 6;
    if(randIdx>=0 && randIdx<=5) {
        start = 1;
        end = 4;
    }
    else if (randIdx>=6 && randIdx<=8) {
        start = 5;
        end = 6;
    }
    
    for (int i=start; i<=end; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("FountainOut_%d",i)->getCString());
        if (i==1) {
            particle->setPosition(ccp(42, 105));
        }
        else if (i==2) {
            particle->setPosition(ccp(108, 105));
        }
        else if (i==3) {
            particle->setPosition(ccp(105, 75));
        }
        else if (i==4) {
            particle->setPosition(ccp(42, 75));
        }
        else {
            particle->setPosition(ccp(75, 100));
        }
        node->addChild(particle);
    }
    CCActionInterval * delayT = CCDelayTime::create(3.0);
    auto func = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::removeFountain), CCInteger::create(tag));
    node->runAction(CCSequence::create(delayT, func, NULL));
}

void ImperialScene::removeFountain(CCObject *obj)
{
    int tag = dynamic_cast<CCInteger*>(obj)->getValue();
    m_fountainNode->removeChildByTag(tag);
}

void ImperialScene::playWatchGlow()
{
    auto spr = CCLoadSprite::createSprite("watch_golw.png");
    spr->setAnchorPoint(ccp(0, 0.5));
    spr->setScaleX(15);
    spr->setScaleY(3);
    spr->setPosition(ccp(1462, 1745));
    m_touchLayer->addChild(spr,3001);
    
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

void ImperialScene::checkTileGlow(CCObject* obj)
{
    QuestInfo *qinfo = QuestController::getInstance()->getRecommendQuest(false);
    int buildType = 0;
    if (qinfo && qinfo->itemId == "2200101") {//建造步兵营任务
        buildType = FUN_BUILD_BARRACK1;
    }
    else if (qinfo && qinfo->itemId == "2400101") {//建造学院任务
        buildType = FUN_BUILD_SCIENE;
    }
    
    if (buildType == 0) {
        m_tileGlowNode->removeAllChildren();
    }
    else {
        int bid = FunBuildController::getInstance()->getMaxLvBuildByType(buildType);
        int pos = -1;
        if (bid > 0) {
            FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(bid);
            pos = info.pos;
        }
        else {
            pos = findCanBuildTile(qinfo->go);
        }
        if (pos>0) {
            m_tileGlowNode->removeAllChildren();
            m_tileGlowNode->setPosition( m_nodeBuildings[pos]->getPosition()+ccp(130,65) );
//            for (int i=1; i<=8; i++) {
//                auto particle = ParticleController::createParticle(CCString::createWithFormat("GuideRegional_%d",i)->getCString());
//                m_tileGlowNode->addChild(particle);
//            }
            for (int i=0; i<=3; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("NewRegional2_%d",i)->getCString());
                m_tileGlowNode->addChild(particle);
            }
        }
    }
}

void ImperialScene::makeCanPlayArrowPoint()
{
    float __scale = 0.7;
    if (CCCommonUtils::isIosAndroidPad())
    {
        __scale = HD_SCALE;
    }
    onMoveToPos(2250, 630, TYPE_POS_MID, 1.0, __scale, true);
    
    m_ptArrowClick = CCPointArray::create(20);
    for (int i=17; i<=26; i++) {//城门门口10个地块
        int bid = FunBuildController::getInstance()->getBuildIdByPos(i);
        if (bid > 0) {
            auto& info = FunBuildController::getInstance()->getFunbuildById(bid);
            if (info.type == FUN_BUILD_WOOD || info.type == FUN_BUILD_FOOD) {
                FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
                if (build) {
                    int buildPosX = build->getParent()->getPositionX() + build->mainWidth / 2 ;
                    int buildPosY = build->getParent()->getPositionY() + build->mainHeight + 50;
                    m_ptArrowClick->addControlPoint(ccp(buildPosX, buildPosY));
                }
            }
        }
    }
    _arrowPtIdx = 0;
    playArrowClickEnd();
    
    scheduleOnce(schedule_selector(ImperialScene::stopArrowClick), 10.0f);
}

void ImperialScene::playArrowClickEnd()
{
    if (_arrowPtIdx>=m_ptArrowClick->count()) {
        _arrowPtIdx = 0;
    }
    m_flyArrow->setPosition( m_ptArrowClick->getControlPointAtIndex(_arrowPtIdx) );
    playArrowClick();
}

void ImperialScene::playArrowClick()
{
    _arrowPtIdx++;
    m_flyArrow->setVisible(true);
    CCActionInterval * moveBy = CCMoveBy::create(0.5,ccp(30, 50));
    CCActionInterval * moveRBy = CCMoveBy::create(0.5,ccp(-30, -50));
    CCSequence* fadeAction = CCSequence::create(moveBy,moveRBy,NULL);
    CCActionInterval * fadeRepeat = CCRepeat::create(fadeAction, 2);
    CCCallFunc * callfunc = CCCallFunc::create(this, callfunc_selector(ImperialScene::playArrowClickEnd));
    
    m_flyArrow->runAction(CCSequence::create(fadeRepeat, callfunc, NULL));
}

void ImperialScene::stopArrowClick(float _time)
{
    m_flyArrow->stopAllActions();
    m_flyArrow->setVisible(false);
    if(PortActController::getInstance()->m_isGetNewRD==0 && PortActController::getInstance()->m_newLoginDay<7 && PortActController::getInstance()->m_newPalyerRDMap.size() > 0){
        guideTypeNew = 1;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("fengshou1"));
}

void ImperialScene::getScienceData()
{
    if (GlobalData::shared()->scienceInfo.IsHasData == false) {
        ScienceDataInitCommand * cmd=new ScienceDataInitCommand();
        cmd->sendAndRelease();
    }
    ActivityController::getInstance()->loginEventData();
    recordGaid();
    GameController::getInstance()->recordPushData();
    
    AllianceManager::getInstance()->starGetRecAlliance();
}

void ImperialScene::recordGaid(){
    if(GlobalData::shared()->gaid == ""){
        GlobalData::shared()->gaid = cocos2d::extension::CCDevice::getGaid();
        CCLOG("idfa %s ",GlobalData::shared()->gaid.c_str());
        UserBindGaidCommand* cmd = new UserBindGaidCommand(GlobalData::shared()->gaid.c_str());
        cmd->sendAndRelease();
        ImperialScene::trackDBM();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ImperialScene::trackTencent();
#endif
    }
}

void ImperialScene::trackDBM(){
    std::transform(GlobalData::shared()->gaid.begin(),GlobalData::shared()->gaid.end(),GlobalData::shared()->gaid.begin(),::toupper);
    MD5 md5;
    md5.update(GlobalData::shared()->gaid.c_str(), GlobalData::shared()->gaid.length());
    std::string muid = md5.toString();
    //            std::transform(muid.begin(),muid.end(),muid.begin(),::toupper);
    CCString* url;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    url = CCString::createWithFormat("http://ad.doubleclick.net/ddm/activity/src=4755477;cat=9bdrezia;type=invmedia;dc_muid=%s;ord=%ld",muid.c_str(),GlobalData::shared()->getTimeStamp());
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(!GlobalData::shared()->isGoogle())
        return;
    url = CCString::createWithFormat("http://ad.doubleclick.net/ddm/activity/src=4755474;cat=s3y7ybnb;type=invmedia;dc_muid=%s;ord=%ld",muid.c_str(),GlobalData::shared()->getTimeStamp());
#endif
    CCLOG("DBM track URL: %s",url->getCString());
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(ImperialScene::onTrackResponse));
    CCHttpClient::getInstance()->setTimeoutForConnect(10);
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void ImperialScene::trackTencent(){
    std::transform(GlobalData::shared()->gaid.begin(),GlobalData::shared()->gaid.end(),GlobalData::shared()->gaid.begin(),::toupper);
    MD5 md5;
    md5.update(GlobalData::shared()->gaid.c_str(), GlobalData::shared()->gaid.length());
    std::string muid = md5.toString();
    long time = GlobalData::shared()->getTimeStamp();
    string sign_key = "809e06139b668e64";
    string query_string = CCString::createWithFormat("muid=%s&conv_time=%ld",muid.c_str(),time)->getCString();
    string page = CCString::createWithFormat("http://t.gdt.qq.com/conv/app/945274928/conv?%s",query_string.c_str())->getCString();
    std::string property = CCString::createWithFormat("%s&GET&%s",sign_key.c_str(),ImperialScene::UrlEncode(page).c_str())->getCString();
    MD5 md5_;
    md5_.update(property.c_str(), property.length());
    std::string base_data = CCString::createWithFormat("%s&sign=%s",query_string.c_str(),ImperialScene::UrlEncode(md5_.toString().c_str()).c_str())->getCString();
    std::string encrypt_key = "8ac0a2161253cb5e";
    unsigned char *xorResult = new unsigned char[base_data.size()];
    ImperialScene::simple_xor(base_data, encrypt_key, xorResult);
    std::string data = cocos2d::extension::CCDevice::base64Encode(xorResult,base_data.size());
    CCString* url = CCString::createWithFormat("http://t.gdt.qq.com/conv/app/945274928/conv?v=%s&conv_type=MOBILEAPP_ACTIVITE&app_type=IOS&advertiser_id=552722",data.c_str());
    CCLOG("Tencent track URL: %s",url->getCString());
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(ImperialScene::onTrackResponse));
    CCHttpClient::getInstance()->setTimeoutForConnect(10);
    CCHttpClient::getInstance()->send(request);
    request->release();
}

std::string ImperialScene::UrlEncode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            unsigned char upper = (unsigned char)str[i] >> 4;
            strTemp += upper > 9 ? upper + 55 : upper + 48;
            unsigned char lower = (unsigned char)str[i] % 16;
            strTemp += lower > 9 ? lower + 55 : lower + 48;
        }
    }
    return strTemp;
}

bool ImperialScene::simple_xor(const std::string& info,const std::string& key,unsigned char* result) {
    if (result == NULL || info.empty() || key.empty()) {
        return false;
    }
//    result->clear();
    uint32_t i = 0;
    uint32_t j = 0;
    for (; i < info.size(); ++i) {
        result[i] = static_cast<unsigned char>(info[i] ^ key[j]);
//        result->push_back(static_cast<unsigned char>(info[i] ^ key[j]));
        j = (++j) % (key.length());
    }
    return true;
}

void ImperialScene::onTrackResponse(CCHttpClient* client, CCHttpResponse* response)
{
    CCLOG("response code: %d", response->getResponseCode());
    if (!response->isSucceed())
    {
        CCLOG("response failed!\nerror buffer: %s", response->getErrorBuffer());
    }else{
        std::vector<char>* iter = response->getResponseData();
        std::string serverInfo(iter->begin(),iter->end());
        CCLOG("track response %s", serverInfo.c_str());
    }
}

void ImperialScene::createActiveBd(CCObject* obj)
{
    // tao.yu 第一个版本不开放 活动
    if (true) {
        return;
    }
    
    if (m_hdBuild) {
    }
    else if(m_buildingInitState && m_resbatchNode && ActivityController::getInstance()->activityIsOpen()){
        m_hdBuild = SpeBuild::create(SPE_BUILD_HD);
        m_hdNode->addChild(m_hdBuild);
        int hod = m_hdNode->getZOrder();
        m_hdBuild->setNamePos(m_hdNode->getPositionX(), m_hdNode->getPositionY(), m_signLayer, m_arrbatchNode, m_resbatchNode, hod);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PERSON_TALK);
    }
}

void ImperialScene::onShowTalkView(CCObject* obj)
{
    if (m_talkTime>=600 && !GuideController::share()->isInTutorial() && PopupViewController::getInstance()->getCurrViewCount()==0) {
        string content = FunBuildController::getInstance()->getTalkTips();
        if (content!="") {
            PopupViewController::getInstance()->addPopupView(NpcTalkView::create(content));
            m_talkTime = 0;
        }
    }
}
void ImperialScene::reSetTalkTime()
{
    m_talkTime = 0;
}

void ImperialScene::addDefenseFailEffect(){
    m_failEffectNode->removeAllChildrenWithCleanup(true);
    CCSpriteFrame* cf1 = CCLoadSprite::getSF("fireBomb.png");
    CCSpriteFrame* cf2 = CCLoadSprite::getSF("a010_0_N_move_0.png");
    if(cf1==NULL || cf2==NULL){
        return ;
    }
    CCSpriteBatchNode* bombBN = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("fireBomb.png")->getTexture());
    m_failEffectNode->addChild(bombBN);
    
    CCSpriteBatchNode* soldierBN = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("a010_0_N_move_0.png")->getTexture());
    m_failEffectNode->addChild(soldierBN);
    
    string fireName = "WarFire_";
    vector<CCParticleBatchNode*> parVec;
    auto addPar = [](vector<CCParticleBatchNode*> parVec, CCNode* node, cocos2d::CCParticleSystemQuad *particle) {
        auto batchCount = parVec.size();
        while (batchCount--) {
            auto &batch = parVec[batchCount];
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
        node->addChild(newBatch);
        newBatch->addChild(particle);
        parVec.push_back(newBatch);
    };
    
    //城门点
    CCPoint pos = ccp(1931,820);
    auto bg1 = CCLoadSprite::createSprite("fireBomb.png");
    bg1->setScale(1.5);
    bg1->setOpacity(GlobalData::shared()->getRand(80,120));
    bg1->setPosition(pos+ccp(30,0));
    bombBN->addChild(bg1);
    
    auto soldier1 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier1->setPosition(pos+ccp(30,-60));
    soldierBN->addChild(soldier1);

    auto soldier1_1 = CCLoadSprite::createSprite("a060_0_N_die_5.png");
    soldier1_1->setPosition(pos+ccp(30,90));
    soldierBN->addChild(soldier1_1);
    
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setScale(0.2);
        particle1->setPosition(pos+ccp(80,0));
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    //城门内点
    pos = ccp(1519,998);
    auto bg2 = CCLoadSprite::createSprite("fireBomb.png");
    bg2->setScale(2);
    bg2->setOpacity(120);
    bg2->setPosition(pos);
    bombBN->addChild(bg2);
    
    auto soldier2 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier2->setPosition(pos+ccp(40,-15));
    soldierBN->addChild(soldier2);
    
    auto soldier2_1 = CCLoadSprite::createSprite("a060_0_N_die_5.png");
    soldier2_1->setPosition(pos+ccp(-50,-25));
    soldierBN->addChild(soldier2_1);
    
    auto soldier2_2 = CCLoadSprite::createSprite("a060_0_N_die_5.png");
    soldier2_2->setPosition(pos+ccp(8,-30));
    soldierBN->addChild(soldier2_2);
    
    auto soldier2_3 = CCLoadSprite::createSprite("a020_0_N_die_5.png");
    soldier2_3->setPosition(pos+ccp(-70,100));
    soldierBN->addChild(soldier2_3);
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setPosition(pos);
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    string tmpFile = "CombustionUI_";
    for (int i=1; i<=2; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpFile.c_str(),i)->getCString());
        particle->setPosition(pos);
        m_failEffectNode->addChild(particle);
    }
    
    //下陷的点
    pos = ccp(976,1189);
    auto bg3 = CCLoadSprite::createSprite("fireBomb.png");
    bg3->setScale(1.5);
    bg3->setOpacity(120);
    bg3->setPosition(pos);
    bombBN->addChild(bg3);
    
    auto soldier3 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier3->setPosition(pos+ccp(-40, -40));
    soldierBN->addChild(soldier3);
    
    auto soldier3_1 = CCLoadSprite::createSprite("a060_0_NW_die_5.png");
    soldier3_1->setPosition(pos+ccp(80, 90));
    soldierBN->addChild(soldier3_1);
    
    auto soldier3_2 = CCLoadSprite::createSprite("a060_0_S_die_5.png");
    soldier3_2->setPosition(pos+ccp(100, 120));
    soldierBN->addChild(soldier3_2);
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setPosition(pos);
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    //大本营前面的点
    pos = ccp(1152,1509);
    auto bg4 = CCLoadSprite::createSprite("fireBomb.png");
    bg4->setScale(2);
    bg4->setOpacity(120);
    bg4->setPosition(pos);
    bombBN->addChild(bg4);
    
    auto soldier4 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier4->setPosition(pos+ccp(30,-30));
    soldierBN->addChild(soldier4);
    
    auto soldier4_1 = CCLoadSprite::createSprite("a020_0_S_die_5.png");
    soldier4_1->setPosition(pos+ccp(-30,-30));
    soldierBN->addChild(soldier4_1);
    
    auto soldier4_2 = CCLoadSprite::createSprite("a060_0_N_die_5.png");
    soldier4_2->setPosition(pos+ccp(90,-20));
    soldierBN->addChild(soldier4_2);
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setPosition(pos);
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    //右前的点
    pos = ccp(1400,1718);
    auto bg5 = CCLoadSprite::createSprite("fireBomb.png");
    bg5->setScale(1.5);
    bg5->setOpacity(120);
    bg5->setPosition(pos);
    bombBN->addChild(bg5);
    
    auto soldier5 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier5->setPosition(pos+ccp(30,-30));
    soldierBN->addChild(soldier5);
    
    auto soldier5_1 = CCLoadSprite::createSprite("a060_0_N_die_5.png");
    soldier5_1->setPosition(pos+ccp(-30,-30));
    soldierBN->addChild(soldier5_1);
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setScale(0.8);
        particle1->setPosition(pos);
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    //左前的点
    pos = ccp(47,1222);
    auto bg6 = CCLoadSprite::createSprite("fireBomb.png");
    bg6->setScale(2);
    bg6->setOpacity(120);
    bg6->setPosition(pos);
    bombBN->addChild(bg6);
    
    auto soldier6 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier6->setPosition(pos+ccp(30,-30));
    soldierBN->addChild(soldier6);
    
    auto soldier6_1 = CCLoadSprite::createSprite("a060_0_NW_die_5.png");
    soldier6_1->setPosition(pos+ccp(50,-10));
    soldierBN->addChild(soldier6_1);
    
    auto soldier6_2 = CCLoadSprite::createSprite("a060_0_N_die_5.png");
    soldier6_2->setPosition(pos+ccp(-30,-20));
    soldierBN->addChild(soldier6_2);
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setPosition(pos);
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    //左下排兵的点
    pos = ccp(513,705);
    auto bg7 = CCLoadSprite::createSprite("fireBomb.png");
    bg7->setScale(2.5);
    bg7->setOpacity(GlobalData::shared()->getRand(80,120));
    bg7->setPosition(pos);
    bombBN->addChild(bg7);
    
    auto soldier7 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier7->setPosition(pos+ccp(30,-40));
    soldierBN->addChild(soldier7);
    
    auto soldier7_1 = CCLoadSprite::createSprite("a060_0_NW_die_5.png");
    soldier7_1->setPosition(pos+ccp(-50,-10));
    soldierBN->addChild(soldier7_1);
    
    auto soldier7_2 = CCLoadSprite::createSprite("a060_0_N_die_5.png");
    soldier7_2->setPosition(pos+ccp(20,-60));
    soldierBN->addChild(soldier7_2);
    
    auto soldier7_3 = CCLoadSprite::createSprite("a020_0_N_die_5.png");
    soldier7_3->setPosition(pos+ccp(-80,40));
    soldierBN->addChild(soldier7_3);
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setPosition(pos);
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    pos = ccp(576,1291);
    auto bg8 = CCLoadSprite::createSprite("fireBomb.png");
    bg8->setScale(1.5);
    bg8->setOpacity(GlobalData::shared()->getRand(80,120));
    bg8->setPosition(pos);
    bombBN->addChild(bg8);
    
    auto soldier8 = CCLoadSprite::createSprite("a020_0_S_die_5.png");
    soldier8->setPosition(pos+ccp(GlobalData::shared()->getRand(-30,-60), GlobalData::shared()->getRand(-30,-70)));
    soldierBN->addChild(soldier8);
    
    auto soldier8_1 = CCLoadSprite::createSprite("a060_0_N_die_5.png");
    soldier8_1->setPosition(pos+ccp(GlobalData::shared()->getRand(-60,30), GlobalData::shared()->getRand(-50,50)));
    soldierBN->addChild(soldier8_1);
    
    pos = ccp(806,1330);
    auto bg9 = CCLoadSprite::createSprite("fireBomb.png");
    bg9->setScale(1.5);
    bg9->setOpacity(GlobalData::shared()->getRand(90,110));
    bg9->setPosition(pos);
    bombBN->addChild(bg9);
    
    auto soldier9 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier9->setPosition(pos+ccp(GlobalData::shared()->getRand(-40,40), GlobalData::shared()->getRand(-30,70)));
    soldierBN->addChild(soldier9);
    
    auto soldier9_1 = CCLoadSprite::createSprite("a020_0_W_die_5.png");
    soldier9_1->setPosition(pos+ccp(30, GlobalData::shared()->getRand(-50,70)));
    soldierBN->addChild(soldier9_1);
    
    
    auto bg10 = CCLoadSprite::createSprite("fireBomb.png");
    bg10->setScale(0.7);
    bg10->setOpacity(GlobalData::shared()->getRand(90,110));
    bg10->setPosition(ccp(1887, 1137));
    bombBN->addChild(bg10);
    
    auto bg11 = CCLoadSprite::createSprite("fireBomb.png");
    bg11->setScale(0.9);
    bg11->setOpacity(GlobalData::shared()->getRand(80,110));
    bg11->setPosition(ccp(1107, 985));
    bombBN->addChild(bg11);
    
    auto bg12 = CCLoadSprite::createSprite("fireBomb.png");
    bg12->setScale(0.6);
    bg12->setOpacity(GlobalData::shared()->getRand(80,110));
    bg12->setPosition(ccp(1648, 1481));
    bombBN->addChild(bg12);
    
    auto bg13 = CCLoadSprite::createSprite("fireBomb.png");
    bg13->setScale(0.7);
    bg13->setOpacity(GlobalData::shared()->getRand(80,100));
    bg13->setPosition(ccp(836, 606));
    bombBN->addChild(bg13);
    
    auto bg14 = CCLoadSprite::createSprite("fireBomb.png");
    bg14->setScale(0.8);
    bg14->setOpacity(GlobalData::shared()->getRand(70,110));
    bg14->setPosition(ccp(1785, 1313));
    bombBN->addChild(bg14);
    
    auto bg15 = CCLoadSprite::createSprite("fireBomb.png");
    bg15->setScale(0.6);
    bg15->setOpacity(GlobalData::shared()->getRand(80,100));
    bg15->setPosition(ccp(1343, 1131));
    bombBN->addChild(bg15);
    
    auto bg16 = CCLoadSprite::createSprite("fireBomb.png");
    bg16->setScale(0.7);
    bg16->setOpacity(GlobalData::shared()->getRand(70,100));
    bg16->setPosition(ccp(1445, 1370));
    bombBN->addChild(bg16);
}
void ImperialScene::updateShipState(string state){
    if(m_shipBuild){
        m_shipBuild->updateShipState(state);
    }
}

void ImperialScene::onStopAllActions(CCObject* params)
{
    m_touchLayer->stopAllActions();
}

CCBAnimationManager* ImperialScene::getAnimationManager()
{
    return mActionManager;
}

void ImperialScene::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
    mActionManager = pAnimationManager;
}

void ImperialScene::onRemoveGuideParticle(CCObject* obj)
{
    if (m_guideParticleNode) {
        m_guideParticleVec.clear();
        m_guideParticleNode->removeAllChildren();
        m_guideParticleNode->removeFromParent();
    }
}

void ImperialScene::addGuideParticleToBatch(cocos2d::CCParticleSystemQuad *particle)
{
    auto batchCount = m_guideParticleVec.size();
    while (batchCount--) {
        auto &batch = m_guideParticleVec[batchCount];
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
    m_guideParticleNode->addChild(newBatch);
    newBatch->addChild(particle);
    m_guideParticleVec.push_back(newBatch);
}

CCSpriteBatchNode* ImperialScene::createCustomBatchLayer(string image){
    if(m_rescustombatchNode){
        auto &array = m_rescustombatchNode->getChildren();
        CCTexture2D *texture = m_rescustombatchNode->getTexture();
        if(texture){
            string releaseFile = texture->getFileName();
            auto pos1 = releaseFile.find_last_of("/");
            std::string picName = "";
            if(pos1!=std::string::npos){
                picName = releaseFile.substr(pos1+1);
                pos1 = picName.find_first_of(".");
                if(pos1!=std::string::npos){
                    picName = picName.substr(0,pos1);
                }
            }
            if(picName!=image){
                auto pos =releaseFile.find_last_of(".");
                releaseFile = releaseFile.substr(0,pos);
                pos =releaseFile.find_last_of(".");
                releaseFile = releaseFile.substr(0,pos);
                releaseFile.append(".plist");
                cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(releaseFile.c_str());
                CCTextureCache::sharedTextureCache()->removeTexture(texture);
                m_rescustombatchNode->removeFromParent();
                m_rescustombatchNode=NULL;
            }
        }
    }
    if(m_rescustombatchNode == NULL){
        DynamicResourceController::getInstance()->loadNameTypeResource(image, false);
        image.append(".png");
        auto resSp = CCLoadSprite::loadResource(image.c_str());//dsg_house_1
        resSp->getTexture()->setAntiAliasTexParameters();
        m_rescustombatchNode = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_touchLayer->addChild(m_rescustombatchNode,200);
        m_rescustombatchNode->setPosition(m_resbatchNode->getPosition());
        return m_rescustombatchNode;
    }else{
        m_rescustombatchNode->removeAllChildren();
        return m_rescustombatchNode;
    }
    return NULL;
}
void ImperialScene::removeCustomBatchLayer(){
    if(m_rescustombatchNode){
        m_rescustombatchNode->removeAllChildren();
        m_touchLayer->removeChild(m_rescustombatchNode);
        m_rescustombatchNode=NULL;
    }
}