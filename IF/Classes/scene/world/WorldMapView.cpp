//
//  WorldMapView.cpp
//  IF
//
//  Created by 邹 程 on 13-11-19.
//
//

#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "CCThreadManager.h"
#include "SceneController.h"
#include "AddFavorite.h"
#include "FavoriteList.h"
#include "PlayerTile.h"
#include "ResourceTile.h"
#include "BlankTile.h"
#include "UIComponent.h"
#include "QueueController.h"
#include "MoveToView.h"
#include "MarchDetailView.h"
#include "MonsterDetailView.h"
#include "TroopInformationView.h"
#include "NetController.h"
#include "CCLineSprite.h"
#include "ParticleController.h"
#include "FunBuildController.h"
#include "SoundController.h"
#include "WildMonsterPopUp.h"
#include "MonsterTile.h"
#include "CCMathUtils.h"
#include "CCLabelIFTTF.h"
#include "EnemyInfoController.h"
#include "MinimapView.h"
#include "ServerListPopUpView.h"
#include "ThroneTile.h"
#include "TrebuchetTile.h"
#include "CCLineBatchedSprite.h"
#include "TiledFourCell.h"
#include "IFFieldMonsterNode.h"
#include "AllianceManager.h"
#include "MoveCityPopUpView.h"
#include <spine/Json.h>
#include "IFSkeletonDataManager.h"
#include "IFAddTowerPopup.h"
#include "TerritoryTile.h"
#include "AllianceAreaPopupView.h"
#include "AAreaBuildCCB.h"
#include "ActBossPopUp.h"
#include "CCFlagWaveSprite.h"
#include "SuperMinePopupView.h"
#include "TerritoryResourceTile.h"
#include "TerritoryTowerTile.h"
#include "DynamicResourceController.h"
#include "TerritoryWarehouseTile.h"
#include "WareHousePopupView.h"
#include "IFMapMaskLayer.h"
#include "UseCKFSkillView.h"
#include "ActivityController.h"
#include "ShakeController.h"
#include "TerritoryBannerTile.h"
#include "TerritoryBannerPopupView.h"
#include "IFShakeLayer.h"
#include "IFShakeCmd.h"
#include "InnerSettingView.h"
#include "DynamicTiledMap.h"
#include "NBWorldMonster.hpp"
#include "NBWorldMapMainCity.hpp"
#include "NBWaterSprite.hpp"

//begin a by ljf
#include "NBWaterMap.h"
//end a by ljf

static WorldMapView* worldMapInstance = nullptr;
static float FLAG_SCALE = 0.4;

WorldMapView* WorldMapView::instance() {
    return worldMapInstance;
}

void WorldMapView::setInstance(WorldMapView *view) {
    worldMapInstance = view;
}

void WorldMapView::unsetInstance() {
    if(worldMapInstance && worldMapInstance->getParent() == NULL){
        worldMapInstance = NULL;
    }
}

WorldMapView* WorldMapView::create(CCPoint& tilePoint, MapType mapType) {
    auto ret = new WorldMapView();
    if (ret && ret->init(tilePoint, mapType)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool WorldMapView::init(cocos2d::CCPoint &viewPoint, MapType mapType) {
    if (!CCLayer::init()) {
        return false;
    }
    
    m_water_wave1 = nullptr;
    m_water_wave2 = nullptr;
    
    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Battle/Battle_soldier.plist");
    CCLoadSprite::doResourceByCommonIndex(205, true);
    CCLoadSprite::doResourceByCommonIndex(507, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(205, false);
        CCLoadSprite::doResourceByCommonIndex(DEFAULT_THRONE_INDEX, false);
        DynamicResourceController::getInstance()->loadNameTypeResource("Common_301", true);
        DynamicResourceController::getInstance()->loadNameTypeResource("Common_303", true);
        DynamicResourceController::getInstance()->loadNameTypeResource("Common_304", true);
        DynamicResourceController::getInstance()->loadNameTypeResource("Common_309", true);
    });
    m_mapType = mapType;
    isCrossServer = false;
    isDownloadingMinimap = false;
    WorldController::getInstance()->enterMap(mapType);
    WorldController::getInstance()->getCityInfos().clear();
    WorldController::getInstance()->isInWorld = true;
    TimeUpMarchProcesser::getInstance()->resetRetryTime();
    tilePops = CCArray::create();
    tilePops->retain();
    roadIndex = 0;
    inFollowView = false;
    followMarchId = "";
    m_inCityUpdate = false;
    m_needOpenInfoView = false;
    currentThroneIndex = 0;
    m_viewPort = CCRectZero;
    m_monsterRect = CCRectZero;
    m_lastScrollTime = WorldController::getInstance()->getTime();
    needClearAll = false;
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    m_winSize = winSize;
    halfWinSize = CCSize(winSize.width/2,winSize.height/2);
    CCLayerColor* tempLayerColor = CCLayerColor::create(ccc4(255, 255, 255, 100), 10, 10);
    tempLayerColor->setPosition(ccp(halfWinSize.width,halfWinSize.height));
    tempLayerColor->ignoreAnchorPointForPosition(false);
//    UIComponent::getInstance()->addChild(tempLayerColor);//todowdz
    auto gotoPoint = WorldController::getInstance()->selfPoint;
    if (!viewPoint.equals(ccp(-1, -1))) {
        gotoPoint = viewPoint;
    }
    unsigned long ulc = clock();
    // because map is moved by touch delegate view , we must add BG CITY ROAD layer at map
    // guo.jiang
    std::string tmxPath = "nb_world.tmx";
//    std::string tmxPath = "WorldMap.tmx";
    int loopSize = _big_tilecountX;
    if(m_mapType == SERVERFIGHT_MAP){
        std::string _writeablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
        tmxPath = _writeablePath + DynamicResource_Folder + "ServerFight.tmx";
        loopSize = WorldController::getInstance()->_current_tile_count_x;
    }
    
    m_map = DynamicTiledMap::create(tmxPath.c_str(), gotoPoint, loopSize);
    m_map->setTag(WM_MAP_TAG);
    CCLOG("tmx Load Time %lu",clock() - ulc);
    this->addChild(m_map);
    
    m_touchDelegateView = HFViewport::create();
    for (int i=0; i<LAYER_COUNT; i++) {
        m_layers[i] = CCNode::create();
    }
    
    // guo.jiang
    {
        m_map->addChild(m_layers[WM_BG], WM_BG);
        m_layers[WM_BG]->setTag(WM_BG_TAG);
        
//        m_map->layerNamed("ocean")->setGlobalZOrder(-100);
    }

    if(m_mapType == NORMAL_MAP) {
        m_map->addChild(m_layers[WM_BETWEEN_SERVER_MAP], WM_BETWEEN_SERVER_MAP);
        m_layers[WM_BETWEEN_SERVER_MAP]->setTag(WM_BETWEEN_SERVER_MAP_TAG);

//        auto map = DynamicTiledMap::create("WorldMap1.tmx", gotoPoint, loopSize);
        // guo.jiang
        auto link_area_pos = Vec2(-256 * (_big_tilecountX - _tile_count_x) / 2, -128 * (_big_tilecountY - _tile_count_y));
        auto link_area = DynamicTiledMap::create("nb_world_link_area.tmx", gotoPoint, loopSize);
        link_area->setTag(WM_BETWEEN_SERVER_MAP_TAG);
        link_area->setPosition(link_area_pos);
//        link_area->layerNamed("tile")->setGlobalZOrder(-100);
//        link_area->layerNamed("ocean")->setGlobalZOrder(-100);
        m_layers[WM_BETWEEN_SERVER_MAP]->addChild(link_area);
    }
    m_map->addChild(m_layers[WM_CITY], WM_CITY);
    m_map->addChild(m_layers[WM_ROAD], WM_ROAD);
    m_map->addChild(m_layers[WM_SKY], WM_SKY);
    m_map->addChild(m_layers[WM_TILE], WM_TILE);
    m_layers[WM_GUI]->setTag(WM_GUI_TAG);
    m_layers[WM_POPUP]->setTag(WM_POPUP_TAG);
    
    //    auto scene = CCDirector::sharedDirector()->getRunningScene();
    auto scene = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_WORLD_UI);
    scene->addChild(m_layers[WM_GUI], WM_GUI);
    scene->addChild(m_layers[WM_POPUP], WM_POPUP);
    
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_MINIMAP);
    layer->addChild(m_layers[WM_COVER], WM_COVER);
    
    addCover();
    // todo : add fog
    int tempX = WorldController::getInstance()->_current_map_width;
    int tempY = WorldController::getInstance()->_current_map_height;
    if(WorldController::getInstance()->currentMapType == NORMAL_MAP){
         tempX = WorldController::getInstance()->serverMap_width;
         tempY = WorldController::getInstance()->serverMap_height;
    }

    m_touchDelegateView->setViewPortTarget(m_map);
    m_touchDelegateView->setSceneSize(tempX, tempY);
    m_touchDelegateView->setWorldBound(CCRect(0,0, tempX, tempY));
    m_touchDelegateView->setTouchDelegate(this);
    m_touchDelegateView->isForDiamond = true;
    auto target = CCApplication::sharedApplication()->getTargetPlatform();
    if (CCCommonUtils::isIosAndroidPad()) {
        m_touchDelegateView->setZoomLimitationMax(WORLD_HD_MAX);
        m_touchDelegateView->setZoomLimitationMin(WORLD_HD_MIN);
        m_map->setScale(WORLD_DEFAULT_HD_SCALE);
    }
    else if (target == kTargetIpad || CCCommonUtils::isAdriodPad()) {
        m_touchDelegateView->setZoomLimitationMax(1.1);
        m_touchDelegateView->setZoomLimitationMin(0.7);
        m_map->setScale(0.6);
    }
    else {
        m_touchDelegateView->setZoomLimitationMax(1.5);
        m_touchDelegateView->setZoomLimitationMin(0.8);
        m_map->setScale(0.9);
    }
    
    m_map->setScale(WorldController::getInstance()->currentMapZoom);
    
    m_touchDelegateView->limitSpeedX = _tile_width/10;
    m_touchDelegateView->limitSpeedY = _tile_height/10;
    
    this->addChild(m_touchDelegateView);
    
    m_drawRoadNode = CCLineBatchedLayer::create();
    m_layers[WM_ROAD]->addChild(m_drawRoadNode, roadIndex++);
    
    m_touchTroopNode = CCNode::create();
    m_layers[WM_ROAD]->addChild(m_touchTroopNode, roadIndex++);
    
    m_armyDustParticleNode = CCNode::create();
    m_layers[WM_ROAD]->addChild(m_armyDustParticleNode, roadIndex++);
    
    m_flagParticleNode = CCNode::create();
    m_flagParticleNode->setScale(FLAG_SCALE);
    m_layers[WM_ROAD]->addChild(m_flagParticleNode, roadIndex++);
    
    m_csNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("liansheng_00.png")->getTexture(), 448);
    m_layers[WM_ROAD]->addChild(m_csNode, roadIndex++);
    
    m_lianNode = CCNode::create();
    m_layers[WM_ROAD]->addChild(m_lianNode, roadIndex++);
    
    m_mapMarchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("JY_body_-45_0.png")->getTexture(), 448);
    m_layers[WM_ROAD]->addChild(m_mapMarchNode, roadIndex++);
    
    m_mapMarchNode1 = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("a010_0_N_move_0.png")->getTexture(), 448);
    m_layers[WM_ROAD]->addChild(m_mapMarchNode1, roadIndex++);
    
    m_heiqishiShadowBachNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("building_level_overlay.png")->getTexture(), 448);
    m_layers[WM_ROAD]->addChild(m_heiqishiShadowBachNode, roadIndex++);
    
    m_mapHeiqishiMarchNode = IFSkeletonBatchLayer::create();
    m_layers[WM_ROAD]->addChild(m_mapHeiqishiMarchNode, roadIndex++);
    
    m_heiqishiLvBgBachNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("building_level_overlay.png")->getTexture(), 448);
    m_layers[WM_ROAD]->addChild(m_heiqishiLvBgBachNode, roadIndex++);
    
    m_heiqishiLvLbNode = CCLabelBatchNode::create("Arial_Bold_Regular.fnt");
    m_layers[WM_ROAD]->addChild(m_heiqishiLvLbNode, roadIndex++);
    
    m_spinLabelNode = CCNode::create();
    m_layers[WM_ROAD]->addChild(m_spinLabelNode, roadIndex++);
    
    m_mapArrowSpNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("a010_0_N_move_0.png")->getTexture(), 448);
    m_layers[WM_ROAD]->addChild(m_mapArrowSpNode, roadIndex++);
    
    m_mapTowerBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("a010_0_N_move_0.png")->getTexture(), 448);
    m_layers[WM_ROAD]->addChild(m_mapTowerBatchNode, roadIndex++);
    
    m_occupyPointerNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("world_occupy_self.png")->getTexture(), 448);
    m_layers[WM_ROAD]->addChild(m_occupyPointerNode, roadIndex++);
    
    m_mapMaskNode = CCNode::create();
    m_layers[WM_ROAD]->addChild(m_mapMaskNode, roadIndex++);
    
    m_mapMaskCloudNode = CCNode::create();
    m_layers[WM_ROAD]->addChild(m_mapMaskCloudNode, roadIndex++);
    
    m_line = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("line_1.png")->getTexture(), 448);
    m_layers[WM_ROAD]->addChild(m_line, roadIndex++);
    roadIndex++;
    
    m_tileLayer = m_map->layerNamed("ground");
    // 指示距离自己城堡的方向和距离
    m_directionIcon = CCLoadSprite::createSprite("milePointer.png");
    m_directionIcon->setVisible(false);
    m_layers[WM_GUI]->addChild(m_directionIcon);
    m_directionLabel = CCLabelIF::create("temp");
    m_directionLabel->setFontSize(20);
    m_directionLabel->setVisible(false);
    m_layers[WM_GUI]->addChild(m_directionLabel);
    m_directionLBPoint = ccp(0,DOWN_HEIGHT);
    m_directionRTPoint = ccp(winSize.width, winSize.height - TOP_HEIGHT - 82);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_directionIcon->setScale(2);
        m_directionLabel->setFontSize(40);
        m_directionLBPoint = ccp(0, 254);
        m_directionRTPoint = ccp(winSize.width, 1933);
    }
    
    // 转着圈圈表示正在加载的图片
    m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
    if (CCCommonUtils::isIosAndroidPad()) {
        m_loadingIcon->setScale(2);
        auto loadingSize = m_loadingIcon->getContentSize();
        m_loadingIcon->setPosition(ccp(loadingSize.width/2 * m_loadingIcon->getScaleX() + 40, DOWN_HEIGHT_HD + loadingSize.height/2 * m_loadingIcon->getScaleY() + 70));
    }
    else {
        auto loadingSize = m_loadingIcon->getContentSize();
        m_loadingIcon->setPosition(ccp(loadingSize.width/2 + 5, DOWN_HEIGHT + loadingSize.height/2 + 45));
    }
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    m_layers[WM_GUI]->addChild(m_loadingIcon);
    
    tmpTouchPoint = WorldController::getInstance()->selfPoint;
    selfViewPoint = m_map->getViewPointByTilePoint(tmpTouchPoint);
    
    
    //    auto cloudLayer = CloudLayer::create();
    //    auto testCloud = CCLoadSprite::createSprite("cloud_1.png");
    //    testCloud->setScale(3*MapGlobalScale);
    //    cloudLayer->addChild(testCloud, 1, ccp(1.2,1.2), m_map->getViewPointByTilePoint(ccp(250, 250)));
    //    m_layers[WM_ROAD]->addChild(cloudLayer);
    
    m_towerRangeMaskNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("1200.png")->getTexture(), 448);
    m_layers[WM_CITY]->addChild(m_towerRangeMaskNode, 0);
    
    m_conflictBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("range_1.png")->getTexture(), 448);
    m_layers[WM_CITY]->addChild(m_conflictBatchNode, 0);
    

    //m_cityBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("41.png")->getTexture(), 448 * 4);
    m_cityBatchNode = CCNode::create();
    m_layers[WM_CITY]->addChild(m_cityBatchNode, 1);
    
    //    m_throneNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("throne.png")->getTexture(), 1);
    m_throneNode = CCNode::create();
    m_layers[WM_CITY]->addChild(m_throneNode, 2);
    
    m_cityAttackNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("a010_0_N_move_0.png")->getTexture(), 448);
    m_layers[WM_CITY]->addChild(m_cityAttackNode, 3);
    
    m_towerNode = CCNode::create();
    m_layers[WM_CITY]->addChild(m_towerNode, 4);
    
    m_batchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("building_level_overlay.png")->getTexture(), 448);
    m_layers[WM_CITY]->addChild(m_batchNode, 5);
    
    m_bannerNode = CCNode::create();
    m_layers[WM_CITY]->addChild(m_bannerNode, 6);
    
    m_simNameUnbatchNode = CCNode::create();
    m_layers[WM_CITY]->addChild(m_simNameUnbatchNode, 7);
    
    m_aAreaAniNode = CCNode::create();
    m_layers[WM_CITY]->addChild(m_aAreaAniNode, 8);
    
    m_mapMonsterShadowNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("shadow.png")->getTexture(), 448);
    m_layers[WM_CITY]->addChild(m_mapMonsterShadowNode, 12);
    
    m_mapMonsterNode = IFSkeletonBatchLayer::create();
    m_layers[WM_CITY]->addChild(m_mapMonsterNode, 13);
    m_mapMonstersNode = Node::create();
    m_layers[WM_CITY]->addChild(m_mapMonstersNode, 13);
    
    //    m_mapMonsterBossNode = IFSkeletonBatchLayer::create();
    //    m_layers[WM_CITY]->addChild(m_mapMonsterBossNode, 12);
    
    m_labelNode = CCLabelBatchNode::create("Arial_Bold_Regular.fnt");
    m_layers[WM_CITY]->addChild(m_labelNode, 10);
    
    m_labelNode1 = CCLabelBatchNode::create("Arial_Bold_Border.fnt");
    m_layers[WM_CITY]->addChild(m_labelNode1, 11);
    
    m_unBatchLabelNode = CCNode::create();
    m_layers[WM_CITY]->addChild(m_unBatchLabelNode, 9);
    
    m_aAreaUnbachLbNode = CCNode::create();
    m_layers[WM_CITY]->addChild(m_aAreaUnbachLbNode, 15);
    
//    m_flagBatch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("Warhammer.png")->getTexture(), 448);
    m_flagBatch = CCNode::create();
    m_layers[WM_CITY]->addChild(m_flagBatch, 16);
    ParticleController::initParticle();
    WorldController::getInstance()->initFavoriteInfo();
    gotoTilePoint(gotoPoint,true);
    setTouchEnabled(true);
    m_touchDelegateView->notMove = false;
    m_mapMaskNode->removeAllChildren();
    for (int index = 0 ; index < 9; index++) {
        CCPoint changeServerPos = WorldController::getInstance()->getChangePointByTypeNum(index);
        CCPoint maskServerPos = WorldController::getServerPosById(GlobalData::shared()->playerInfo.currentServerId)  + changeServerPos;
        int serverId = WorldController::getServerIdByServerPoint(maskServerPos);
        IFMapMaskLayer* LayerColor = IFMapMaskLayer::create(ccc4(0, 0, 0, 80), _map_height/2 * sqrt(5), _map_height/2 * sqrt(5) * sin((180 -(atan2(2, 1)* 180 /M_PI * 2)) * M_PI / 180));
        LayerColor->ignoreAnchorPointForPosition(false);
        LayerColor->setAnchorPoint(CCPointZero);
        LayerColor->setSkewX(90 - atan2(1, 2)* 180 /M_PI * 2);//(90 - atan2(1, 2)* 180 /M_PI * 2);
        LayerColor->setRotation(atan2(1, 2)* 180 /M_PI);
        LayerColor->setTag(WM_MASK_TAG + index);
        LayerColor->setVisible(false);
        LayerColor->setserverId(serverId);
        m_mapMaskNode->addChild(LayerColor,6);
    }
    WorldMapView::instance()->m_map->onShowMapMask();
    onShowMarkLine(NULL);
    if (canShowShakeGuide()) {
        scheduleOnce(schedule_selector(WorldMapView::onShowShakeGuide), 3.0);
    }
    return true;
}

void WorldMapView::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
    m_viewPort.setRect(-m_map->getPositionX() / WorldController::getInstance()->currentMapZoom,
                       -m_map->getPositionY() / WorldController::getInstance()->currentMapZoom,
                       m_winSize.width / WorldController::getInstance()->currentMapZoom,
                       m_winSize.height/ WorldController::getInstance()->currentMapZoom);
    
    CCLayer::visit(renderer,parentTransform,parentFlags);
}

void WorldMapView::addCover(){
//    auto winSize = CCDirector::sharedDirector()->getWinSize();
//    m_layers[WM_COVER]->removeAllChildren();
//    if(GlobalData::shared()->playerInfo.currentServerId != GlobalData::shared()->playerInfo.selfServerId){
//        auto cover = CCLayerColor::create(ccc4(0, 0, 0, 80), winSize.width, winSize.height);
//        cover->setAnchorPoint(ccp(0, 0));
//        m_layers[WM_COVER]->addChild(cover);
//    }
}

void WorldMapView::removeCover(){
    m_layers[WM_COVER]->removeAllChildren();
}

CCNode *WorldMapView::getNodeByType(std::string typeStr){
    if(typeStr == "wood" || typeStr == "relic" || typeStr == "food"){
        if(GuideController::share()->arr->count() != 1){
            int count = GuideController::share()->arr->count();
            return NULL;
        }
        
        unsigned int index = 0;
        if(typeStr == "wood" || typeStr == "food"){
            index = dynamic_cast<CCInteger*>(GuideController::share()->arr->objectAtIndex(0))->getValue();
        }
        
        map<unsigned int, WorldCityInfo>::iterator it = m_cityInfo.find(index);
        CCNode *node = NULL;
        if(it != m_cityInfo.end()){
            node = m_cityBatchNode->getChildByTag(it->second.cityIndex);
            if(GuideController::share()->updateWorldInfo){
                CCPoint pt = m_map->getPosition();
                gotoTilePoint(WorldController::getPointByIndex(index));
                GuideController::share()->updateWorldInfo = false;
                CCPoint pt1 = m_map->getPosition();
                m_map->setPosition(pt);
                m_map->runAction(CCMoveTo::create(1.0, pt1));
            }
        }
        m_touchDelegateView->notMove = false;
        if (node) {
            m_touchDelegateView->notMove = true;
        }
        return node;
    }else if(typeStr == "attack"){
        auto arr = m_layers[WM_TILE]->getChildren();
        int index = 0;
        for(auto child : arr)
        {
            auto tile = dynamic_cast<class ResourceTile*>(child);
            if(tile){
                return tile->getNodeByType(typeStr);
            }
        }
//        
//        while (arr && index < arr->count()) {
//            auto tile = dynamic_cast<class ResourceTile*>(arr->objectAtIndex(index++));
//            if(tile){
//                return tile->getNodeByType(typeStr);
//            }
//        }
    }
    return NULL;
}


void WorldMapView::updateDirection() {
    if(!WorldMapView::instance() || !m_directionIcon){
        return;
    }
    auto newViewPoint = m_touchDelegateView->getCurrentCenterGridPoint();
    auto vs = ccpSub(newViewPoint, WorldMapView::instance()->selfViewPoint);
    
    if ((fabsf(vs.x) > halfWinSize.width || fabsf(vs.y) > halfWinSize.height) && GlobalData::shared()->playerInfo.isInSelfServer()) {
        
        auto rotate = 90.0f - atan2f(vs.y, vs.x)/ M_PI * 180.0f;
        
        m_directionIcon->setVisible(true);
        m_directionIcon->setRotation(rotate);
        
        m_directionLabel->setVisible(true);
        m_directionLabel->setString("%d%s",true,(int)(vs.getLength() / 100),_lang("108653").c_str());
        m_directionLabel->setRotation(fabsf(rotate - 90.0) > 90.0 ? rotate + 90.0 : rotate - 90.0);
        
        
        auto center = ccp(halfWinSize.width, halfWinSize.height);
        
        CCPoint corner,position;
        
        float pointerSqure = 60.0;
        if (CCCommonUtils::isIosAndroidPad())
        {
            pointerSqure = 120.0;
        }
        
        if (vs.x >= 0 && vs.y >= 0) {
            // left bottom
            corner = m_directionLBPoint + ccp(pointerSqure, pointerSqure);
        } else if (vs.x >= 0 && vs.y < 0) {
            // left top
            corner = ccp(m_directionLBPoint.x + pointerSqure,m_directionRTPoint.y - pointerSqure);
        } else if (vs.x < 0 && vs.y >= 0) {
            // right bottom
            corner = ccp(m_directionRTPoint.x - pointerSqure,m_directionLBPoint.y + pointerSqure);
        } else {
            // right top
            corner = m_directionRTPoint - ccp(pointerSqure, pointerSqure);
        }
        
        auto diff = center - corner;
        
        if (vs.y == 0 || fabsf(diff.x/diff.y) <= fabsf(vs.x/vs.y)) {
            position.x = corner.x;
            if(vs.x == 0){
                vs.x = 1;
            }
            position.y = center.y - diff.x * vs.y/vs.x;
        } else {
            position.y = corner.y;
            position.x = center.x - diff.y * vs.x/vs.y;
        }
        
        m_directionIcon->setPosition(position);
        m_directionLabel->setPosition(position);
    } else {
        if(m_directionIcon == NULL){
            return;
        }
        m_directionIcon->setVisible(false);
        m_directionLabel->setVisible(false);
    }
}

void WorldMapView::hideGUI(cocos2d::CCObject *obj) {
    m_layers[WM_GUI]->setVisible(false);
}

void WorldMapView::showGUI(cocos2d::CCObject *obj) {
    if(!PopupViewController::getInstance()->getCurrentPopupView() && !PopupViewController::getInstance()->getGoBackViewCount()) {
        m_layers[WM_GUI]->setVisible(true);
    }
}

void WorldMapView::showFirework(cocos2d::CCObject *obj) {
    CCDictionary* dict = dynamic_cast<CCDictionary*>(obj);
    int firePosistion = dict->valueForKey("pointId")->intValue();
    int useItemId = dict->valueForKey("itemId")->intValue();
//    this->schedule(schedule_selector(WorldMapView::startShowFirework), 1.0f, 1, 0.0f);
    SoundController::sharedSound()->playEffects(Music_Sfx_firework_out);
    startShowFirework(firePosistion,useItemId);
}

void WorldMapView::startShowFirework(int firePosistion,int useItemId){
    if (m_walkparticleVec.find(firePosistion)!=m_walkparticleVec.end()) {
        for (int i=0; i<m_walkparticleVec[firePosistion].size(); i++) {
            m_walkparticleVec[firePosistion][i]->removeFromParentAndCleanup(true);
        }
        m_walkparticleVec.erase(firePosistion);
    }
    auto fPos = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(firePosistion));
    int colorIndex = CCMathUtils::getRandomInt(1,6);
    for (int i=1; i<=5; i++) {
        CCParticleSystemQuad* particle=NULL;
        if (useItemId==ITEM_FIREWORK2) {
            particle = ParticleController::createParticle(CCString::createWithFormat("Fireworks_%d_%d",colorIndex,i)->getCString());
            particle->setPosition(CCPoint(fPos.x, fPos.y+200));
        }else{
            particle = ParticleController::createParticle(CCString::createWithFormat("Fireworks_1_%d",i)->getCString());
            particle->setPosition(CCPoint(fPos.x, fPos.y+200));
        }
        addWalkParticleToBatch(particle, firePosistion);
    }
}

void WorldMapView::updateGUI(bool setVisible) {
    auto ui = UIComponent::getInstance();
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    ui->setVisible(setVisible);
    if (setVisible) {
        m_directionLBPoint = ccp(0,DOWN_HEIGHT);
        m_directionRTPoint = ccp(winSize.width, winSize.height - TOP_HEIGHT - 82);
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_directionLBPoint = ccp(0, DOWN_HEIGHT_HD);
            m_directionRTPoint = ccp(winSize.width, 1933);
        }
    } else {
        m_directionLBPoint = ccp(0,0);
        m_directionRTPoint = ccp(winSize.width, winSize.height - 60);
    }
    updateDirection();
}

void WorldMapView::removeTouchEffect(){
    m_layers[WM_ROAD]->removeChildByTag(2000000);
}

void WorldMapView::openTilePanel(unsigned int index) {
    auto &info = m_cityInfo[index];
    if(m_layers[WM_TILE]->getChildrenCount() > 0){
        auto tilePop = dynamic_cast<NewBaseTileInfo*>(m_layers[WM_TILE]->getChildren().at(0));
        tilePop->closeThis();
        if(info.cityType == OriginTile){
            return;
        }
    }
    if (index < 1 || index > WorldController::getInstance()->_current_index_limit) {
        return;
    }
    tilePops->removeAllObjects();
    if(m_untouchableTiles.size() != 0){
        return;
    }
    if (!info.cityIndex) {
        return;
    }
    
    PopupBaseView *view1 = NULL;
    PopupBaseView *view2 = NULL;
    NewBaseTileInfo *view3 = NULL;
    
    std::string guideStr = GuideController::share()->getCurrentId();
    switch (info.cityType) {
        case OriginTile:{
            SoundController::sharedSound()->playEffects(Music_Sfx_world_click_space);
            m_untouchableTiles.push_back(index);
            view3 = BlankTile::create(info);
        }
            break;
        case Tile_allianceArea:{
            SoundController::sharedSound()->playEffects(Music_Sfx_world_click_space);
            m_untouchableTiles.push_back(index);
            view3 = TerritoryTile::create(info);
        }
            break;
        case tile_superMine:{
            SoundController::sharedSound()->playEffects(Music_Sfx_world_click_space);
            m_untouchableTiles.push_back(index);
            view3 = TerritoryResourceTile::create(info);
        }
            break;
        case tile_tower:{
            SoundController::sharedSound()->playEffects(Music_Sfx_world_click_space);
            m_untouchableTiles.push_back(index);
            view3 = TerritoryTowerTile::create(info);
            if (m_towerRangeMaskNode->getChildByTag(index)) {
                m_towerRangeMaskNode->getChildByTag(index)->setVisible(true);
            }
        }
            break;
        case tile_banner:{
            SoundController::sharedSound()->playEffects(Music_Sfx_world_click_space);
            m_untouchableTiles.push_back(index);
            view3 = TerritoryBannerTile::create(info);
        }
            break;
        case tile_wareHouse:{
            SoundController::sharedSound()->playEffects(Music_Sfx_world_click_space);
            m_untouchableTiles.push_back(index);
            view3 = TerritoryWarehouseTile::create(info);
        }
            break;
        case ResourceTile:{
            int level = FunBuildController::getInstance()->getMainCityLv();
            switch (info.resource.type) {
                case Iron:
                    SoundController::sharedSound()->playEffects(Music_Sfx_city_iron);
                    break;
                case Stone:
                    SoundController::sharedSound()->playEffects(Music_Sfx_city_mithril);
                    break;
                case Wood:
                    SoundController::sharedSound()->playEffects(Music_Sfx_city_wood);
                    break;
                case Food:
                    SoundController::sharedSound()->playEffects(Music_Sfx_city_farm);
                    break;
                case Silver:
                    SoundController::sharedSound()->playEffects(Music_Sfx_city_iron);
                    break;
                case Gold:
                    SoundController::sharedSound()->playEffects(Music_Sfx_city_iron);
                    break;
                    
                default:
                    break;
            }
            if(info.resource.type == Iron){
                if(level < FunBuildController::getInstance()->building_base_k3){
                    CCCommonUtils::flyHint("", "", _lang_2("108666", CC_ITOA(FunBuildController::getInstance()->building_base_k3), _lang("108641").c_str()));
                    return;
                }
            }else if (info.resource.type == Stone){
                if(level < FunBuildController::getInstance()->building_base_k4){
                    CCCommonUtils::flyHint("", "", _lang_2("108666", CC_ITOA(FunBuildController::getInstance()->building_base_k4), _lang("108640").c_str()));
                    return;
                }
            }
            m_untouchableTiles.push_back(index);
            view3 = ResourceTile::create(info);
        }
            break;
            
        case CityTile:{
            SoundController::sharedSound()->playEffects(Music_Sfx_city_castle);
            m_untouchableTiles.push_back(index);
            view3 = PlayerTile::create(info);
            if(info.cross==1 && info.playerName != GlobalData::shared()->playerInfo.name){
                CCCommonUtils::flyHint("", "", _lang("138088"));
            }
        }
            break;
        case CampTile:{//Castle
            SoundController::sharedSound()->playEffects(Music_Sfx_world_click_space);
            m_untouchableTiles.push_back(index);
            view3 = PlayerTile::create(info);
        }
            break;
        case MonsterTile:{//探索
            // guojiang
            CCCommonUtils::flyHint("", "", _lang("E100008"));//fusheng 这个界面 美术没做
            return;
            
            // todo
            SoundController::sharedSound()->playEffects(Music_Sfx_city_castle);
            int index = info.parentCityIndex;
            auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
            auto it = selfMarch.find(index);
            if (it == selfMarch.end()) {
                if(WorldController::getInstance()->isHasMonsterAttackMarch()){
                    YesNoDialog::showYesDialog(_lang("108668").c_str());
                }else{
                    m_untouchableTiles.push_back(index);
                    view3 = MonsterTile::create(info);
                }
            }else{
                view2 = MonsterAttackDetailView::create(index);
            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                   , CCString::createWithFormat("WL_relic"));
        }
            break;
        case FieldMonster:{//野怪
            auto panel = dynamic_cast<WildMonsterPopUp*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(panel){
                return;
            }
            SoundController::sharedSound()->playEffects(Music_Sfx_world_click_monster);
            view1 = WildMonsterPopUp::create(info);
        }
            break;
        case ActBossTile:{//活动怪物面板
            auto panel = dynamic_cast<ActBossPopUp*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(panel){
                return;
            }
            SoundController::sharedSound()->playEffects(Music_Sfx_world_click_monster);
            view1 = ActBossPopUp::create(info);
        }
            break;
        case Throne:{
            SoundController::sharedSound()->playEffects(Music_Sfx_city_castle);
            if(info.kingBuildInfo.openTime == 0){
                //                CCCommonUtils::flyHint("", "", _lang("110020"));
                PopupViewController::getInstance()->addPopupView(WarNoticeInfoView::create());
                return;
            }
            
            WorldActivityState state = WorldController::getInstance()->getKingActivityState(info.kingBuildInfo.openTime, info.kingBuildInfo.startTime, info.kingBuildInfo.endTime);
            if (state == OpenNoKing) {
                PopupViewController::getInstance()->addPopupView(WarNoticeInfoView::create());
                return;
            }
            
            m_untouchableTiles.push_back(index);
            view3 = ThroneTile::create(info);
        }
            break;
        case Trebuchet:{
            SoundController::sharedSound()->playEffects(Music_Sfx_city_military);
            if(info.trebuchetInfo.openTime == 0){
                CCCommonUtils::flyHint("", "", _lang("110020"));
                return;
            }
            //
            m_untouchableTiles.push_back(index);
            view3 = TrebuchetTile::create(info);
        }
            break;
        default:
            return;
    }
    
    // add touch effect
    float fafeOut = 0.5;
    int cityIndex = 0;
    auto touchEffect = CCLoadSprite::createSprite("world_tile_white.png");
    touchEffect->setAnchorPoint(ccp(0, 0));
    float effectW = touchEffect->getContentSize().width;
    float s = 1.0;
    int dy = 0;
    int dx = 0;
    if(info.parentCityIndex != -1){
        cityIndex = info.parentCityIndex;
        s = _tile_width * 1.0f * 2.0f / effectW;
        dx = -_tile_width / 2;
        if(info.cityType == Throne){
            s = _tile_width * 1.0f * _throne_w / effectW;
            dx = -_halfTileSize.width * (_throne_w - 1);
            dy = -_halfTileSize.height * (_throne_h - 1);
        }
    }else{
        cityIndex = info.cityIndex;
        s = _tile_width * 1.0f / effectW;
    }
    touchEffect->setScale(s);
    auto touchPos = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(cityIndex),info.tileServerId);
    
    touchEffect->setPosition(touchPos - _halfTileSize + ccp(dx, dy));
    CCCallFuncO *fun = NULL;
    if(view1){
        view1->setTag(index);
        fun = CCCallFuncO::create(this, callfuncO_selector(WorldMapView::addPopupView1), view1);
    }else if(view2){
        view2->setTag(index);
        fun = CCCallFuncO::create(this, callfuncO_selector(WorldMapView::addPopupView2), view2);
    }else if(view3){
        view3->setTag(index);
        tilePops->addObject(view3);
    }
    
    if(fun){
        m_untouchableTiles.push_back(index);
        touchEffect->runAction(CCSequence::create(CCFadeOut::create(fafeOut),CCRemoveSelf::create(), fun,NULL));
        m_layers[WM_ROAD]->addChild(touchEffect, 9);
    }else if(view3){
        touchEffect->setOpacity(0);
        auto seque = CCSequence::create(CCFadeTo::create(fafeOut, 128),
                                        CCFadeTo::create(fafeOut, 0),
                                        CCCallFuncO::create(this, callfuncO_selector(WorldMapView::resetClickState), NULL),
                                        NULL);
        
        touchEffect->runAction(CCRepeatForever::create(seque));
        m_layers[WM_ROAD]->addChild(touchEffect, 9);
        removeTouchEffect();
        touchEffect->setTag(2000000);
    }
}

bool WorldMapView::onBeginScroll(const cocos2d::CCPoint &worldLocation) {
    CCLOG("begin scroll");
    this->setPraticesEffect(false);
    m_isIdle = false;
    return true;
}

void WorldMapView::onStepScroll(const cocos2d::CCPoint &worldLocation, const cocos2d::CCPoint &delta) {
    m_lastScrollTime = WorldController::getInstance()->getTime();
    CCLOG("step scroll");
}

void WorldMapView::onEndScroll(const cocos2d::CCPoint &worldLocation) {
    CCLOG("end scroll");
    onShowAAreaLb(false);
//    this->setPraticesEffect(true);
//    m_isIdle = true;
    
}

void WorldMapView::autoMoveEnd() {
    this->setPraticesEffect(true);
    m_isIdle = true;
}

void WorldMapView::onSingleTouchBegin(const cocos2d::CCPoint &worldLocation) {
    CCLOG("begin touch");
    CCLOGERROR("centerPoint x:%d y:%d",WorldController::getInstance()->serverMap_centerViewPoint.x,WorldController::getInstance()->serverMap_centerViewPoint.y);
    CCLOG("xy%f %f",worldLocation.x,worldLocation.y);
    if(isMiniMapModel()){
        return;
    }
    onShowAAreaLb(true);
    tmpTouchPoint = m_map->getTilePointByViewPoint(worldLocation);
}

void WorldMapView::onSingleTouchEnd(const cocos2d::CCPoint &worldLocation) {
    if(isCrossServer){
        return;
    }
    onShowAAreaLb(false);
    if(isMiniMapModel()){
        return;
    }
    CCLOG("end touch");
    auto & childrenArr = m_towerRangeMaskNode->getChildren();
    CCObject* elemObj = NULL;
    for (auto child:childrenArr)
    {
        dynamic_cast<CCNode*>(child)->setVisible(false);
    }
    if(this->isTouchEnabled()){
        openTilePanel(WorldController::getIndexByPoint(tmpTouchPoint));
    }
    
    this->m_isIdle = true;
    this->setPraticesEffect(true);
}

void WorldMapView::onBeginZoom(const cocos2d::CCPoint &point1, const cocos2d::CCPoint &point2) {
    CCLOG("begin zoom");
}

void WorldMapView::onStepZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2){
    CCLOG("step zoom");
}

bool WorldMapView::isMiniMapModel(){
    if(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP_IN)->getChildByTag(miniMapTag)){
        return true;
    }
    return false;
}

void WorldMapView::onEndZoom(float targetZoom) {
    CCLOG("end zoom");
    if(targetZoom < 0.2){
        targetZoom = 1.0f;
    }
    WorldController::getInstance()->currentMapZoom = targetZoom;
}

void WorldMapView::onEnter() {
    CCLayer::onEnter();
    SoundController::sharedSound()->playBGMusic(Music_M_city_3);
    //    SoundController::sharedSound()->playBGMusic(Music_M_city_3_preview);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WorldMapView::showGUI), MSG_POPUP_VIEW_OUT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WorldMapView::hideGUI), MSG_POPUP_VIEW_IN, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WorldMapView::showFirework), "showFirework", NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WorldMapView::onShowMarkLine), MSG_MARK_LINE_AL, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WorldMapView::onShake), MSG_ON_SHAKE, NULL);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, true);
    setTouchEnabled(true);

    ShakeController::getInstance()->addAccelerometer();
}

void WorldMapView::onExit() {
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_VIEW_IN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_VIEW_OUT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "showFirework");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MARK_LINE_AL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ON_SHAKE);
    ShakeController::getInstance()->removeAccelerometer();
    removeCover();
    if(m_mapType != SERVERFIGHT_MAP){
        GlobalData::shared()->playerInfo.currentServerId = GlobalData::shared()->playerInfo.selfServerId;
    }
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
    CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(WM_GUI_TAG);
    CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(WM_POPUP_TAG);
    SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_WORLD_UI)->removeAllChildren();
    GuideController::share()->updateWorldInfo = true;
    m_directionIcon = NULL;
    m_loadingIcon = NULL;
    for(auto march = WorldController::getInstance()->m_marchInfo.begin(); march != WorldController::getInstance()->m_marchInfo.end(); march++){
        if(march->second.action == MarchActionPrepareAttack || march->second.action == MarchActionAttack){
            if (march->second.marchType == MethodHeiqishi) {
                CCArray* tempArr = CCArray::create();
                CC_SAFE_RETAIN(tempArr);
                tempArr->addObject(CCString::create(march->second.uuid));
                int tag = march->second.marchTag;
                string oldSpinTag =  CC_ITOA(tag);
                tempArr->addObject(CCString::create(oldSpinTag));
                int firIndex = MarchArmy::getStartIndex(march->second);
                string firIndexStr =  CC_ITOA(firIndex);
                tempArr->addObject(CCString::create(firIndexStr));
                onFinishSpinBattle(tempArr);
            }
            else{
                finishBattleAni(CCString::create(march->second.uuid));
            }
        }
    }
    for(auto it = m_marchArmy.begin(); it != m_marchArmy.end(); it++){
        it->second->armyDelete();
        it->second->release();
    }
    if(tilePops){
        tilePops->removeAllObjects();
        tilePops->release();
        tilePops = NULL;
    }
    m_marchArmy.clear();
    WorldController::getInstance()->purge();
    
    CC_SAFE_RELEASE_NULL(m_water_wave1);
    CC_SAFE_RELEASE_NULL(m_water_wave2);
    
    CCLayer::onExit();
}

void WorldMapView::asyncCityInfoParse(cocos2d::CCObject *obj) {
    if (!WorldController::getInstance()->isInWorld) return;
    if (!WorldMapView::instance()) return;
    
    WorldController::getInstance()->isRefreshQueue = false;
    auto params = dynamic_cast<CCDictionary*>(obj);
    
    if (params) {
        int tileServerId = params->valueForKey("serverId")->intValue();
        int x = params->valueForKey("x")->intValue();
        int y = params->valueForKey("y")->intValue();
        int type = params->valueForKey("t")->intValue();
        CCArray* pointsArr = dynamic_cast<CCArray*>(params->objectForKey("points"));
        CCString* pointStr = dynamic_cast<CCString*>(params->objectForKey("points"));
        
        string jsonStr("[");
        if (pointsArr) {
            CCLOGFUNC("");
            CCObject* pElem = NULL;
            CCARRAY_FOREACH(pointsArr, pElem) {
                string elemStr = dynamic_cast<CCString*>(pElem)->getCString();
                elemStr = elemStr.substr(1, elemStr.length() - 2);
                jsonStr = jsonStr + elemStr + string(",");
            }
            jsonStr = jsonStr.substr(0, jsonStr.length() - 1) + string("]");
        }
        if (pointStr) {
            CCLOGFUNC("");
            jsonStr = pointStr->getCString();
        }
        
        auto currentCenterPoint = ccp(x, y);// m_map->currentTilePoint;
        std::map<int, std::map<int, int>> cityMap;
        auto addToMap = [](std::map<int, std::map<int, int>> &map, int index, CCPoint &centerPt, int realIndex) {
            auto getDis = [](int index, CCPoint &centerPt) {
                auto pt = WorldController::getPointByIndex(index);
                return abs(int(pt.x - centerPt.x)) + abs(int(pt.y - centerPt.y));
            };
            
            int dis = getDis(realIndex, centerPt);
            if (map.find(dis) != map.end()) {
                std::map<int, int> &cityMap = map.find(dis)->second;
                cityMap[index] = 1;
            } else {
                std::map<int, int> cityMap;
                cityMap[index] = 1;
                map[dis] = cityMap;
            }
        };
        auto json = Json_create(jsonStr.c_str());
        //        if (!json) {
        //            CCLOGFUNC("");
        //            params->release();
        //            return;
        //        }
        CCLOGFUNC("");
        int itemIndex = 0;
        Json* tmp = NULL;
        if (json) {
            tmp = Json_getItemAt(json, itemIndex);
        }
        auto next = [&]() {
            itemIndex++;
            tmp = Json_getItemAt(json, itemIndex);
        };
        CCLOGFUNC("");
        // init
        pthread_mutex_lock(&m_addCityLock);
        m_cacheToAddCity.clear();
        CCPoint pt = ccp(x, y);
        if (type == 0) {
            std::map<int, unsigned int> allTile = getCurrentTile(pt, 26, 11, WorldController::getInstance()->_current_tile_count_x);
            std::map<int, unsigned int>::iterator tileIt;
            for (tileIt = allTile.begin(); tileIt != allTile.end(); tileIt++) {
                int tempIndex = tileIt->first;
                CCPoint tempPoint = WorldController::getInstance()->getPointByIndex(tempIndex);
//                CCLOG("log+++++++: %f,%f,%d",tempPoint.x,tempPoint.y,tileServerId);
//                if (m_cityInfo.find(tileIt->first) != m_cityInfo.end()) {
//                    CCLOG("log++%d",m_cityInfo[tileIt->first].tileServerId);
//                }
                if (tileIt->first < 1 || tileIt->first > WorldController::getInstance()->_current_index_limit) {
                    continue;
                }
                WorldController::getInstance()->getCityInfos()[tileIt->first].setInfo(tileIt->first, tileServerId);
                addToMap(cityMap, tileIt->first, currentCenterPoint, tileIt->first);
            }
            allTile.clear();
        }
        while (tmp != NULL) {
            unsigned int index = Json_getInt(tmp, "i", 0);
            
            if (!index) {
                next();
                continue;
            }
            if (index > 0 && index <= WorldController::getInstance()->_current_index_limit) {
                unsigned int resType = Json_getInt(tmp, "t", 0);
                int cityType = (WorldCityType)resType;
                string ownerName = Json_getString(tmp, "o", ""); //没有名字的城 不处理数据
                string dirStr = Json_getString(tmp, "tdirection", "");
                if (
                   (cityType == MonsterRange
                    || cityType == CityRange
                    || cityType == Tile_allianceRange
                    || cityType == tile_superMineRange
                    || cityType == ActBossTileRange
                    || cityType == tile_wareHouseRange
                    )
                   && ("" == dirStr)
                   ) {
                    next();
                    continue;
                }
                if (ownerName == "" && cityType == CityTile) {
                    next();
                    continue;
                }
                if (cityType == MonsterTile) {
                    unsigned int refresh = Json_getInt(tmp, "rt", 0);
                    unsigned int start = Json_getInt(tmp, "st", 0);
                    double rStamp = (double)refresh * 1000.0;
                    double sStamp = (double)start * 1000.0;
                    auto now = WorldController::getInstance()->getTime();
                    if (now > rStamp || now < sStamp) {
                        next();
                        continue;   //隐藏已经过期的遗迹
                    }
                }
                WorldController::getInstance()->getCityInfos()[index].setInfo(tmp,false,tileServerId);
                hideSameBoard(index);
                addToMap(cityMap, index, currentCenterPoint, index);
                //tutorial
//                if (GuideController::share()->getCurrentId() == "3031200"
//                   || GuideController::share()->getCurrentId() == "3031300"
//                   || GuideController::share()->getCurrentId() == "3031400") {
                if (GuideController::share()->getCurrentId() == "3074100" || GuideController::share()->getCurrentId() == "3074200"){//fusheng world里的教程
                    CCPoint addPt = WorldController::getPointByIndex(index);
                    if (WorldController::getInstance()->getCityInfos()[index].cityType == ResourceTile
                       && WorldController::getInstance()->getCityInfos()[index].resource.type == Food
                       && WorldController::getInstance()->getCityInfos()[index].playerName == "") {
                        if (GuideController::share()->arr->count() == 0) {
                            GuideController::share()->arr->addObject(CCInteger::create(WorldController::getInstance()->getCityInfos()[index].cityIndex));
                        } else {
                            int idx = dynamic_cast<CCInteger*>(GuideController::share()->arr->objectAtIndex(GuideController::share()->arr->count() - 1))->getValue();
                            map<unsigned int, WorldCityInfo>::iterator it = WorldController::getInstance()->getCityInfos().find(idx);
                            if (it->second.cityType != ResourceTile) {
                                GuideController::share()->arr->addObject(CCInteger::create(WorldController::getInstance()->getCityInfos()[index].cityIndex));
                            } else {
                                CCPoint pt1 = WorldController::getPointByIndex(idx);
                                int dis = abs(int(pt1.x - pt.x)) + abs(int(pt1.y - pt.y));
                                int dis1 = abs(int(addPt.x - pt.x)) + abs(int(addPt.y - pt.y));
                                
                                if (dis1 < dis) {
                                    GuideController::share()->arr->removeObjectAtIndex(GuideController::share()->arr->count() - 1);
                                    GuideController::share()->arr->addObject(CCInteger::create(WorldController::getInstance()->getCityInfos()[index].cityIndex));
                                }
                            }
                        }
                    }
                }
                //tutorial
                if(cityType == MonsterTile
                   || cityType == CityTile
                   || cityType == Trebuchet
                   || cityType == ActBossTile
                   || cityType == tile_wareHouse
                   || cityType == tile_superMine
                   || cityType == Tile_allianceArea
                   ){
                    unsigned int indexL = index - 1;
                    unsigned int indexR = index - WorldController::getInstance()->_current_tile_count_x;
                    unsigned int indexU = indexR - 1;
                    WorldController::getInstance()->getCityInfos()[indexL].setInfo(tmp,true,tileServerId);
                    WorldController::getInstance()->getCityInfos()[indexR].setInfo(tmp,true,tileServerId);
                    WorldController::getInstance()->getCityInfos()[indexU].setInfo(tmp,true,tileServerId);
                    addToMap(cityMap, indexL, currentCenterPoint, index);
                    addToMap(cityMap, indexR, currentCenterPoint, index);
                    addToMap(cityMap, indexU, currentCenterPoint, index);
                    WorldController::getInstance()->getCityInfos()[index].parentCityIndex = index;
                    WorldController::getInstance()->getCityInfos()[indexL].parentCityIndex = index;
                    WorldController::getInstance()->getCityInfos()[indexR].parentCityIndex = index;
                    
                    WorldController::getInstance()->getCityInfos()[indexU].parentCityIndex = index;
                    WorldController::getInstance()->getCityInfos()[indexL].setCityIndex(indexL);
                    WorldController::getInstance()->getCityInfos()[indexR].setCityIndex(indexR);
                    WorldController::getInstance()->getCityInfos()[indexU].setCityIndex(indexU);
                }else if(cityType == Throne){
                    int startX = -_throne_w / 2;
                    int endX = _throne_w / 2;
                    int startY = -_throne_h / 2;
                    int endY = _throne_h / 2;
                    auto centerPt = ccp(_relic_center_x, _relic_center_y);
                    int x = startX;
                    int y = startY;
                    while (x <= endX) {
                        y = startY;
                        while (y <= endY) {
                            auto cityPt = centerPt + ccp(x, y);
                            int cityIndex = WorldController::getIndexByPoint(cityPt);
                            
                            WorldController::getInstance()->getCityInfos()[cityIndex].setInfo(tmp,false,tileServerId);
                            addToMap(cityMap, cityIndex, currentCenterPoint, index);
                            WorldController::getInstance()->getCityInfos()[cityIndex].parentCityIndex = index;
                            WorldController::getInstance()->getCityInfos()[cityIndex].setCityIndex(cityIndex);
                            y++;
                        }
                        x++;
                    }
                }
            }
            
            next();
        }
        for (auto cityIt = cityMap.rbegin(); cityIt != cityMap.rend(); cityIt++) {
            for (auto it = cityIt->second.begin(); it != cityIt->second.end(); it++) {
                m_cacheToAddCity.push_back(it->first);
            }
        }
        pthread_mutex_unlock(&m_addCityLock);
        
        WorldController::getInstance()->initMarchInfo(dynamic_cast<CCArray*>(params->objectForKey("m")),tileServerId);
        //        WorldController::getInstance()->addTestData();
        
        WorldController::getInstance()->initWorld = true;
        
        auto isSinglePoint = new CCBoolean(m_cacheToAddCity.size() == 1 ? true : false);
        
        CCThreadManager::getInstance()->runInMainThread(this, callfuncO_selector(WorldMapView::addCityInMainThread),isSinglePoint);
        
        Json_dispose(json);
    }
    
    if( obj )
    {
        CCThreadManager::getInstance()->runInMainThread(this, callfuncO_selector(WorldMapView::asyncReleaseInMainThread),obj);
    }
    
}

void WorldMapView::asyncReleaseInMainThread(CCObject* obj)
{
    if( obj != NULL ) {
        obj->release();
    }
}

void WorldMapView::addAllianceArea(int cityIndex,bool isMyAlliance){
    CCPoint tilePoint = WorldController::getInstance()->getPointByIndex(cityIndex);
    if (m_conflictBatchNode->getChildByTag(cityIndex)) {
        CCNode* tempNode = m_conflictBatchNode->getChildByTag(cityIndex);
        tempNode->removeAllChildrenWithCleanup(true);
        tempNode->removeFromParentAndCleanup(true);
    }
    CCSprite*  tempBatchSpNode = NULL;
    CCNode* tempLbNode = NULL;
    /* 11* 11 tile */
    int index = 0;
    int radius = 5;
    int startX = tilePoint.x - radius;
    int startY = tilePoint.y - radius;
    int endX = tilePoint.x + radius;
    int endY = tilePoint.y + radius;
    CCLOG("logInfo=====startx: %d,starty: %d,endx: %d,endy: %d",startX,startY,endX,endY);
    std::vector<CCPoint> PointVec_bottom;
    std::vector<CCPoint> PointVec_left;
    std::vector<CCPoint> PointVec_up;
    std::vector<CCPoint> PointVec_right;
    for (int index = startX; index <= endX; index++) {
        PointVec_bottom.push_back(CCPoint(index, endY));
    }
    for (int index = startY; index <= endY; index++) {
        PointVec_left.push_back(CCPoint(startX, index));
    }
    for (int index = startX; index <= endX; index++) {
        PointVec_up.push_back(CCPoint(index, startY));
    }
    for (int index = startY; index <= endY; index++) {
        PointVec_right.push_back(CCPoint(endX, index));
    }
    auto addPic = [&](std::vector<CCPoint> pointVec,int scaleType){
        int _index = 0;
        while (_index < pointVec.size()) {
            CCPoint tempPoint = pointVec.at(_index);
            int forceServerId = GlobalData::shared()->playerInfo.currentServerId;
            if(m_cityInfo.find(_index) != m_cityInfo.end()){
                forceServerId = m_cityInfo[_index].tileServerId;
            }
            auto tempTileInfo = m_cityInfo[_index];
            CCPoint worldTilePoint =  m_map->getViewPointByTilePoint(tempPoint,forceServerId);
            string picStr = "AllianceTerritory_1.png";
            //            if (!isMyAlliance) {
            //                picStr = "allianceLine_red.png";
            //            }
            auto pic = CCLoadSprite::createSprite(picStr.c_str());
            auto ani = createMarchAni(AniAllianceArea, 0, 1.0);
            pic->runAction(ani);
            CCPoint addPt = ccp(_tile_width / 4, _tile_height / 4);
            switch (scaleType) {
                case 0:
                    pic->setSkewY(-26.5);
                    addPt = ccp(0, -_tile_height / 2);
                    pic->setAnchorPoint(ccp(1, 0));
                    break;
                case 1:
                    pic->setSkewY(26.5);
                    addPt = ccp(-_tile_width / 2, 0);
                    pic->setAnchorPoint(ccp(0, 0));
                    break;
                case 2:
                    pic->setSkewY(-26.5);
                    pic->setAnchorPoint(ccp(0, 0));
                    addPt = ccp(0, _tile_height / 2);
                    break;
                case 3:
                    pic->setSkewY(26.5);
                    pic->setAnchorPoint(ccp(1, 0));
                    addPt = ccp(_tile_width / 2, 0);
                    break;
                default:
                    pic->setSkewY(-26.5);
                    addPt = ccp(0, -_tile_height / 2);
                    pic->setAnchorPoint(ccp(1, 0));
                    break;
            }
            CCPoint realPos = worldTilePoint + addPt;
            pic->setPosition(realPos);
            _index++;
            tempBatchSpNode->addChild(pic);
            m_cityItem[cityIndex].push_back(pic);
        }
    };
    
    tempBatchSpNode = CCLoadSprite::createSprite("range_1.png");
    tempBatchSpNode->setContentSize(CCSizeZero);
    tempBatchSpNode->setTag(cityIndex);
    m_conflictBatchNode->addChild(tempBatchSpNode);
    
    addPic(PointVec_bottom,0);
    addPic(PointVec_left,1);
    addPic(PointVec_up,2);
    addPic(PointVec_right,3);
    
    bool shouldNew = true;
    if (m_aAreaUnbachLbNode->getChildByTag(cityIndex)) {
        CCNode* tempNode = m_aAreaUnbachLbNode->getChildByTag(cityIndex);
        if (tempNode && tempNode->getChildrenCount() > 0) {
            shouldNew = false;
        }
    }
    
    if (shouldNew) {
        m_aAreaUnbachLbNode->removeChildByTag(cityIndex);
        tempLbNode = CCNode::create();
        tempLbNode->setTag(cityIndex);
        m_aAreaUnbachLbNode->addChild(tempLbNode);
        tempLbNode->setVisible(false);
    }
    /*add lb*/
    vector<CCPoint> lbVec;
    for (int j = startY; j <= endY; j+= 2) {
        for (int i = startX; i <= endX; i+= 2) {
            lbVec.push_back(ccp(i,j));
        }
    }
    auto &info = m_cityInfo[cityIndex];
    int lbIndex = 0;
    while (lbIndex < lbVec.size()) {
        CCPoint lbPoint = m_map->getViewPointByTilePoint(lbVec.at(lbIndex));
        lbPoint = lbPoint + ccp(_tile_width/2,0);
        string fullName = info.m_aArea_name;
        if(info.m_aArea_simpleName != ""){
            fullName = CCString::createWithFormat("%s%s%s%s", "(", info.m_aArea_simpleName.c_str(), ")", fullName.c_str())->getCString();
        }
        auto name = CCLabelIFTTF::create();
        name->setString(fullName.c_str());
        name->setFontSize(20);
        if(isMyAlliance){
            name->setColor(ccc3(138,181,177));
        }
        else{
            name->setColor(ccc3(227,158,158));
        }
        name->setPosition(lbPoint);
        tempLbNode->addChild(name);
        m_cityItem[cityIndex].push_back(name);
        lbIndex ++;
    }
    /*add lb*/
}

void WorldMapView::clearAllMarch(){
    if(needClearAll){
        for(auto it = m_marchArmy.begin(); it != m_marchArmy.end(); it++){
            it->second->armyDelete();
            it->second->release();
        }
        m_marchArmy.clear();
        WorldMapView::instance()->m_mapMarchNode1->removeAllChildren();
        WorldMapView::instance()->m_mapArrowSpNode->removeAllChildren();
        WorldMapView::instance()->m_mapTowerBatchNode->removeAllChildren();
        WorldMapView::instance()->m_mapHeiqishiMarchNode->removeAllChildren();
        WorldMapView::instance()->m_spinLabelNode->removeAllChildren();
        m_heiqishiLvBgBachNode->removeAllChildren();
        m_heiqishiShadowBachNode->removeAllChildren();
        m_heiqishiLvLbNode->removeAllChildren();
        WorldMapView::instance()->m_cityAttackNode->removeAllChildren();
        WorldMapView::instance()->m_armyDustParticleNode->removeAllChildren();
        
        m_parWalkBatchVec.clear();
        m_walkparticleVec.clear();
        
        WorldMapView::instance()->m_mapMarchNode->removeAllChildren();
        WorldMapView::instance()->m_line->removeAllChildren();
        WorldMapView::instance()->m_drawRoadNode->removeAllChildren();
        WorldMapView::instance()->m_towerRangeMaskNode->removeAllChildren();
    }
}

void WorldMapView::addCityInMainThread(cocos2d::CCObject *obj) {
    
    if (!WorldMapView::instance()) {
        // view had been clean up
        return;
    }
    clearAllMarch();
    pthread_mutex_lock(&m_addCityLock);
    
    auto boolObj = dynamic_cast<CCBoolean*>(obj);
    bool isSinglePoint = boolObj->getValue();
    boolObj->release();
    
    // clear old under node
    if (isSinglePoint) {
        
        createCity(m_cityInfo[m_cacheToAddCity.back()]);
        
    } else {
        
        m_inCityUpdate = true;
        if(m_loadingIcon){
            m_loadingIcon->setVisible(true);
        }
    }
    pthread_mutex_unlock(&m_addCityLock);
    if (WorldController::getInstance()->openTargetIndex!=-1) {
        if(m_layers[WM_TILE]->getChildrenCount() > 0){
            auto tilePop = dynamic_cast<NewBaseTileInfo*>(m_layers[WM_TILE]->getChildren().at(0));
            tilePop->closeImmediately();
        }
        tilePops->removeAllObjects();
        if (AllianceManager::getInstance()->goToWorldType == 0 && WorldController::getInstance()->buildAllianceTerritory == 0) {
            openTilePanel(WorldController::getInstance()->openTargetIndex);
        }else if (AllianceManager::getInstance()->goToWorldType > 200000){
            auto size = CCDirector::sharedDirector()->getWinSize();
            CCPoint p(size.width / 2 , size.height / 2);
            PopupViewController::getInstance()->setTouchPos(p);
            
            PopupViewController::getInstance()->addPopupView(MoveCityPopUpView::create(MoveCity_Wood,AllianceManager::getInstance()->goToWorldType));
            AllianceManager::getInstance()->goToWorldType = 0;
        }else {
            if (AllianceManager::getInstance()->goToWorldType == 1) {
                auto index = getBestLocIndex(WorldController::getInstance()->openTargetIndex);
                auto point = WorldController::getInstance()->getPointByIndex(index);
                AllianceManager::getInstance()->notUpDate = 1;
                gotoTilePoint(point, false);
                AllianceManager::getInstance()->notUpDate = 0;
                auto size = CCDirector::sharedDirector()->getWinSize();
                CCPoint p(size.width / 2 - _tile_width / 2, size.height / 2);
                PopupViewController::getInstance()->setTouchPos(p);
                MoveCityPopUpView* moveView = MoveCityPopUpView::create(MoveCity_Castle);
                moveView->addMailUid(AllianceManager::getInstance()->tmpMailUid);
                PopupViewController::getInstance()->addPopupView(moveView);
                AllianceManager::getInstance()->goToWorldType = 0;
            }
            else if (AllianceManager::getInstance()->goToWorldType == 2 || AllianceManager::getInstance()->goToWorldType == 3)
            {
                auto size = CCDirector::sharedDirector()->getWinSize();
                CCPoint p(size.width / 2, size.height / 2 + _tile_height / 2);
                PopupViewController::getInstance()->setTouchPos(p);
                MoveCityPopUpView* moveView = MoveCityPopUpView::create(MoveCity_Castle);
                moveView->setAlMoveType(AllianceManager::getInstance()->goToWorldType);
                PopupViewController::getInstance()->addPopupView(moveView);
                AllianceManager::getInstance()->goToWorldType = 0;
            }
            if (WorldController::getInstance()->buildAllianceTerritory == 1) {
                string aareaId = WorldController::getInstance()->allianceTerritoryId;
                int aareaIndex = WorldController::getInstance()->allianceTerritoryIndex;
                WorldCityType aareaType = WorldController::getInstance()->allianceTerritoryType;
                WorldResourceType aareaResType = WorldController::getInstance()->allianceTerritoryResourceType;
                string aareaBanner = WorldController::getInstance()->allianceTerritoryBanner;
                if (aareaType == Tile_allianceArea) {
                    PopupViewController::getInstance()->addPopupView(AllianceAreaPopupView::create(aareaId, aareaIndex));
                }
                else if (aareaType == tile_superMine) {
                    PopupViewController::getInstance()->addPopupView(SuperMinePopupView::create(aareaResType, aareaIndex));
                }
                else if (aareaType == tile_tower) {
                    PopupViewController::getInstance()->addPopupView(IFAddTowerPopup::create(aareaId, aareaIndex));
                }
                else if (aareaType == tile_banner) {
                    PopupViewController::getInstance()->addPopupView(TerritoryBannerPopupView::create(aareaIndex, aareaBanner));
                }
                else if (aareaType == tile_wareHouse) {
                    PopupViewController::getInstance()->addPopupView(WareHousePopupView::create(aareaIndex));
                }
                WorldController::getInstance()->buildAllianceTerritory = 0;
            }
        }
        AllianceManager::getInstance()->tmpMailUid = "";
        WorldController::getInstance()->openTargetIndex = -1;
    }
    if(WorldController::getInstance()->openMarchId != ""){
        WorldMapView::instance()->showTroopInfo(WorldController::getInstance()->openMarchId);
        WorldController::getInstance()->openMarchId = "";
    }
    if (WorldController::getInstance()->playAniTargetIndex!=-1) {
        playFreshResAni(WorldController::getInstance()->playAniTargetIndex);
        WorldController::getInstance()->playAniTargetIndex = -1;
    }
    if(WorldController::getInstance()->isRefreshQueue){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
    }
    WorldController::getInstance()->isRefreshQueue = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(WORLD_MAP_REFRESH);
}

void WorldMapView::callBackMapInfo(CCDictionary *params) {
    if (params->objectForKey("timeStamp")) {
        int time = params->valueForKey("timeStamp")->intValue();
        if (m_updateTimeStamp > time) {
            // old update info , discard
            return;
        }
        // todo : update info
        params->retain();
        CCThreadManager::getInstance()->runInNewThread(this, callfuncO_selector(WorldMapView::asyncCityInfoParse),params);
    }
}
 
int WorldMapView::getBestLocIndex(int index) {
    auto it = m_cityInfo.find(index);
    if (it == m_cityInfo.end()) {
        return index;
    }
    mp.clear();
    if (isFourTileCanRelocate(index)) {
        return index;
    }
    int retIndex = index;
    for (int i = 1; i < 7; ++i) {
        int k1 = index + i, k2 = index - i, k3 = index + i * _tile_count_x, k4 = index - i * _tile_count_x;
        if (k1 <= _indexLimit && k2 > 0) {
            if (ceil(k1 * 1.0 / _tile_count_x) != ceil(index * 1.0 / _tile_count_x)) {
                k1 = ceil(index * 1.0 / _tile_count_x) * _tile_count_x;
            }
            if (ceil(k2 * 1.0 / _tile_count_x) != ceil(index * 1.0 / _tile_count_x)) {
                k2 = ceil(index * 1.0 / _tile_count_x) * _tile_count_x - _tile_count_x + 1;
            }
        } else {
            if (k1 > _indexLimit) {
                k1 = _indexLimit;
            }
            if (k2 <=0) {
                k2 = 1;
            }
        }
        if (k3 > _indexLimit) {
            k3 = _indexLimit + index % _tile_count_x;
            k3 = k3 > _indexLimit ? (k3 - _tile_count_x) : k3;
        }
        if (k4 <= 0) {
            k4 = index % _tile_count_x;
            k4 = k4 <= 0 ? (k4 + _tile_count_x) : k4;
        }
        int ret = -1;
        for (int j = 0; j <= i; ++j) {
            int tCnt = _tile_count_x * j;
            if (isFourTileCanRelocate(k1 + tCnt)) {
                ret = k1 + tCnt;
                break;
            }
            if (isFourTileCanRelocate(k1 - tCnt)) {
                ret = k1 - tCnt;
                break;
            }
            if (isFourTileCanRelocate(k2 - tCnt)) {
                ret = k2 - tCnt;
                break;
            }
            if (isFourTileCanRelocate(k2 + tCnt)) {
                ret = k2 + tCnt;
                break;
            }
            if (ceil((k3 + j) * 1.0 / _tile_count_x) == ceil(k3 * 1.0 / _tile_count_x)) {
                if (isFourTileCanRelocate(k3 + j)) {
                    ret = k3 + j;
                    break;
                }
            }
            if (ceil((k3 - j) * 1.0 / _tile_count_x) == ceil(k3 * 1.0 / _tile_count_x)) {
                if (isFourTileCanRelocate(k3 - j)) {
                    ret = k3 - j;
                    break;
                }
            }
            if (ceil((k4 + j) * 1.0 / _tile_count_x) == ceil(k4 * 1.0 / _tile_count_x)) {
                if (isFourTileCanRelocate(k4 + j)) {
                    ret = k4 + j;
                    break;
                }
            }
            if (ceil((k4 - j) * 1.0 / _tile_count_x) == ceil(k4 * 1.0 / _tile_count_x)) {
                if (isFourTileCanRelocate(k4 - j)) {
                    ret = k4 - j;
                    break;
                }
            }
        }
        if (ret != -1) {
            retIndex = ret;
            break;
        }
    }
    return retIndex;
}

bool WorldMapView::isFourTileCanRelocate(int index) {
    bool b1 = false, b2 = false, b3 = false, b4 = false;
    if (m_cityInfo.find(index) != m_cityInfo.end()) {
        if (mp.find(index) != mp.end()) {
            b1 = mp[index];
        } else {
            b1 = isCanRelocationTile(index);
            mp[index] = b1;
        }
    } else {
        return false;
    }
    if (index % _tile_count_x != 1) {
        --index;
        if (m_cityInfo.find(index) != m_cityInfo.end()) {
            if (mp.find(index) != mp.end()) {
                b2 = mp[index];
            } else {
                b2 = isCanRelocationTile(index);
                mp[index] = b2;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
    if ((index / _tile_count_x) < (_tile_count_y - 1)) {
        index += _tile_count_x;
        if (m_cityInfo.find(index) != m_cityInfo.end()) {
            if (mp.find(index) != mp.end()) {
                b3 = mp[index];
            } else {
                b3 = isCanRelocationTile(index);
                mp[index] = b3;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
    ++index;
    if (m_cityInfo.find(index) != m_cityInfo.end()) {
        if (mp.find(index) != mp.end()) {
            b4 = mp[index];
        } else {
            b4 = isCanRelocationTile(index);
            mp[index] = b4;
        }
    } else {
        return false;
    }
    
    return b1 && b2 && b3 && b4;
}

bool WorldMapView::isCanRelocationTile(int index){
    if (index <= 0 || index > _indexLimit) {
        return false;
    }
    if (m_cityInfo.find(index) == m_cityInfo.end()) {
        return false;
    }
    if(m_cityInfo[index].cityType != OriginTile){
        return false;
    }
    if(m_cityInfo[index].cityOriginType == "stop"){
        return false;
    }
    return true;
}

void WorldMapView::leaveWorld() {
    NetController::shared()->cancelForTarget(this);
    if(GlobalData::shared()->playerInfo.uid != ""){
        auto cmd = new WorldLeaveCommand();
        cmd->sendAndRelease();
    }
    WorldController::getInstance()->isInWorld = false;
    SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
    UIComponent::getInstance()->removeShakeGuide();
}

void WorldMapView::updateSelfName() {
    m_map->updateDynamicMap();
//    update_water_shader();
}

void WorldMapView::clearPopupView() {
    auto popup = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_WORLD_UI)->getChildByTag(WM_POPUP_TAG);
    if (popup->getChildrenCount()) {
        popup->removeAllChildren();
        inFollowView = false;
        followMarchId = "";
    } else {
        leaveWorld();
    }
}

void WorldMapView::addPopupView(BaseTileInfo *view) {
    if (!view) {
        return;
    }
    m_untouchableTiles.clear();
    SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_WORLD_UI)->getChildByTag(WM_POPUP_TAG)->addChild(view);
}

void WorldMapView::addPopupView4(NewBaseTileInfo* view) {
    if (!view) {
        return;
    }
    m_untouchableTiles.clear();
    SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_WORLD_UI)->getChildByTag(WM_POPUP_TAG)->addChild(view);
    
    
    m_layers[WM_TILE]->removeAllChildren();
    tilePops->removeAllObjects();
    
    removeTouchEffect();
}

void WorldMapView::resetClickState(CCObject* obj){
    m_untouchableTiles.clear();
}

void WorldMapView::addPopupView2(CCObject* view){
    if (!view) {
        return;
    }
    m_untouchableTiles.clear();
    PopupViewController::getInstance()->addPopupInView(dynamic_cast<PopupBaseView*>(view), false);
}

void WorldMapView::addPopupView1(CCObject* view){
    if (!view) {
        return;
    }
    PopupViewController::getInstance()->addPopupView(dynamic_cast<PopupBaseView*>(view), false);
    m_untouchableTiles.clear();
}

void WorldMapView::addPopupView3(NewBaseTileInfo* view){
    if (!view) {
        return;
    }
    auto &info = view->m_cityInfo;
    auto addPt = ccp(0, 0);
    unsigned int index = info.cityIndex;
    if(info.parentCityIndex != -1){
        index = info.parentCityIndex;
        addPt = ccp(0, _tile_height / 2);
    }
    auto size = view->getContentSize();
    auto touchPos = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(index),info.tileServerId) + addPt - CCPoint(size.width / 2, size.height / 2);
    CCPoint pt = touchPos;
    view->resetPos(pt);
    m_untouchableTiles.clear();
    m_layers[WM_TILE]->removeAllChildren();
    m_layers[WM_TILE]->addChild(view);
    tilePops->removeAllObjects();
}

//void WorldMapView::autoAddResourceView(MarchInfo &info)
//{
//    auto it = m_cityInfo.find(info.endPointIndex);
//    if(it != m_cityInfo.end())
//    {
//        ResourceTile::create(it->second);
//    }
//}

void WorldMapView::gotoTilePoint(const cocos2d::CCPoint &point,bool forceUpdate,int forceServerId/* = -1*/) {
    auto viewPoint = m_map->getViewPointByTilePoint(point,forceServerId);
    
    float scaleX = m_map->getScaleX();
    float scaleY = m_map->getScaleY();
    
    float x = halfWinSize.width - viewPoint.x*scaleX;
    float y = halfWinSize.height - viewPoint.y*scaleY;
    m_map->isSendCmd = false;
    m_map->setPosition(ccp(x, y));
    UIComponent::getInstance()->m_xCoordText->setString(CC_ITOA((int)point.x));
    UIComponent::getInstance()->m_yCoordText->setString(CC_ITOA((int)point.y));
    UIComponent::getInstance()->m_zCoordText->setString(CC_ITOA(GlobalData::shared()->playerInfo.currentServerId));
    UIComponent::getInstance()->setSerchCoordState();
    if (WorldController::getInstance()->openTargetIndex!=-1 || WorldController::getInstance()->openMarchId != ""){
        forceUpdate = true;
    }
    if (AllianceManager::getInstance()->notUpDate == 1)
    {
        forceUpdate = false;
    }
    if (forceUpdate && !m_map->isSendCmd) {
        m_map->updateDynamicMap();
//        update_water_shader();
    }
    m_map->isSendCmd = false;
    update_water_shader(Vec2(x, y));
}

void WorldMapView::gotoTilePoint(const CCPoint &point, int serverId, bool isForceUpdate){
    if(serverId == GlobalData::shared()->playerInfo.currentServerId){
        gotoTilePoint(point, isForceUpdate);
        m_map->onShowMapMask();
    }else{
        changeServer(serverId);
        gotoTilePoint(point, true);
        m_map->onShowMapMask();
    }
}

void WorldMapView::addFavorite(const std::string &name, unsigned int index, unsigned int type) {
    int serverId = GlobalData::shared()->playerInfo.currentServerId;
    if (m_cityInfo.find(index) != m_cityInfo.end()) {
        serverId = m_cityInfo[index].tileServerId;
    }
    auto cmd = new WorldAddFavoriteCommand(name,index,type,serverId);
    cmd->sendAndRelease();
}

void WorldMapView::onAddFavoriteCallback(CCDictionary *info) {
    unsigned int index = dynamic_cast<CCInteger*>(info->objectForKey("point"))->getValue();
    auto type = (FavoriteType)dynamic_cast<CCInteger*>(info->objectForKey("type"))->getValue();
    std::string name = info->valueForKey("name")->getCString();
    CCPoint tilePoint = WorldController::getPointByIndex(index);
    int serverId = info->valueForKey("server")->intValue();
    long createTime = info->valueForKey("createTime")->longValue();
    double key = WorldController::getInstance()->getFavoriteKey(index, serverId);
    WorldController::getInstance()->m_favoriteTiles[key] = FavoriteTile{tilePoint,name,type, true, serverId, createTime};
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WORLD_FAVO_ADD);
}

void WorldMapView::onCityMoveCallback(cocos2d::CCObject *obj) {
    EnemyInfoController::getInstance()->m_enemyInfos.clear();
    
    unsigned int index = dynamic_cast<CCString*>(obj)->intValue();
    auto selfPoint = WorldController::getPointByIndex(index);
    WorldController::getInstance()->selfPoint = selfPoint;
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        selfViewPoint = m_map->getViewPointByTilePoint(selfPoint);
        gotoTilePoint(selfPoint);
    }
}

void WorldMapView::afterMarchDeploy(cocos2d::CCDictionary *dict,int bType,std::string other, int targetType) {
    unsigned int targetIndex = dynamic_cast<CCInteger*>(dict->objectForKey("targetIndex"))->getValue();
    unsigned int haveOwner = dynamic_cast<CCInteger*>(dict->objectForKey("haveOwner"))->getValue();
    int monster = -1;
    if (haveOwner > 1) {//1这个参数不要占用
        // this is pve mode , convert haveOwner param to monsterId
        monster = haveOwner - 1;
        haveOwner = 0;
    }
    auto soldier = dynamic_cast<CCDictionary*>(dict->objectForKey("soldier"));
    auto generals = dynamic_cast<CCArray*>(dict->objectForKey("generals"));
    MarchMethodType type = MethodBattle;
    auto info = m_cityInfo.find(targetIndex);
    if(targetType==ActBossTile || info != m_cityInfo.end()){
        if(targetType==ActBossTile || info->second.cityType == FieldMonster || info->second.cityType == ActBossTile)
        {
            int tmpUsePower = 0;
            if (info != m_cityInfo.end()) {
                if (info->second.cityType == FieldMonster) {
                    tmpUsePower = WorldController::getInstance()->getMonsterUsePower(targetIndex);
                    if (tmpUsePower == 0) {
                        tmpUsePower = GlobalData::shared()->worldConfig.stamineCostPerTime;
                    }
                }else if (info->second.cityType == ActBossTile) {
                    tmpUsePower = GlobalData::shared()->worldConfig.boss_decr;
                }else if (targetType==ActBossTile) {
                    tmpUsePower = GlobalData::shared()->worldConfig.boss_decr;
                }
                
            }else {
                if (targetType==ActBossTile) {
                    tmpUsePower = GlobalData::shared()->worldConfig.boss_decr;
                }
            }
            
            if (WorldController::getInstance()->currentStamine < tmpUsePower) {
                CCCommonUtils::flyHint("", "", _lang("137490"));
                return;
            }
            
            WorldController::getInstance()->currentStamine -= tmpUsePower;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CURRENT_STAMINE);
            auto size = CCDirector::sharedDirector()->getWinSize();
            auto pNode = CCNode::create();
            string tmpStart = "Collection_Star_";
            int count = 4;
            pNode->setRotation(165);
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                pNode->addChild(particle);
            }
            CCPoint startPt = ccp(50, size.height - 50);
            CCPoint endPt = ccp(size.width / 2, size.height / 2);
            pNode->setPosition(startPt);
            ccBezierConfig  beziercofig;
            beziercofig.controlPoint_1 = ccp(startPt.x + 50, startPt.y);
            beziercofig.controlPoint_2 = ccp(endPt.x + 50, endPt.y);
            float useTime = 0.6f;
            beziercofig.endPosition = endPt;
            CCActionInterval *forward = CCBezierTo::create(useTime, beziercofig);
            CCSequence* sp = CCSequence::create(forward, CCRemoveSelf::create(), NULL);
            pNode->runAction(sp);
            auto top = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP);
            top->addChild(pNode);
            
            auto stamineNod = CCNode::create();
            stamineNod->setPosition(endPt);
            auto sprite = CCLoadSprite::createSprite("Ui_tili.png");
            sprite->setScale(0.6);
            stamineNod->addChild(sprite);
            auto label = CCLabelIF::create((std::string("-") + CC_ITOA(tmpUsePower)).c_str());
            label->setColor(ccRED);
            label->setFontSize(24);
            stamineNod->addChild(label);
            label->setAnchorPoint(ccp(0, 0.5));
            label->setPositionX(sprite->getContentSize().width / 2);
            
            top->addChild(stamineNod);
            
            CCSequence *sequence1 = CCSequence::create(CCBlink::create(useTime * 2, 1),
                                                       CCMoveTo::create(0.5, endPt + ccp(0, 150)),
                                                       CCRemoveSelf::create(),
                                                       NULL
                                                       );
            stamineNod->runAction(sequence1);
            
            type = MethodFieldMonster;
        }
    }
    int wtIndex = -1;
    std::string teamId = "";
    if(bType!=-1){
        type = (MarchMethodType)bType;
        switch(type){
            case MethodRally:
                wtIndex = atoi(other.c_str());
                break;
            case MethodUnion:
                teamId = other;
                break;
        }
    }
    auto cmd = new WorldMarchCommand(targetIndex, type,haveOwner,soldier,NULL,monster, generals,wtIndex,teamId);
    if (GuideController::share()->getCurGuideID()==GUIDE_BATTLE_ED) {
        cmd->putParam("guideStep", CCString::create(GUIDE_BATTLE_ED));
    }
    cmd->sendAndRelease();
    gotoTilePoint(WorldController::getInstance()->selfPoint,GlobalData::shared()->playerInfo.selfServerId);
}

void WorldMapView::afterMarchSpeedUp(cocos2d::CCDictionary *dict) {
    string uuid = dict->valueForKey("marchId")->getCString();
    string itemUuid = dict->valueForKey("itemId")->getCString();
    
    auto cmd = new WorldMarchSpeedUpCommand(uuid,itemUuid);
    cmd->sendAndRelease();
}

void WorldMapView::afterMarchCancel(cocos2d::CCDictionary *dict) {
    string uuid = dict->valueForKey("marchId")->getCString();
    string itemUuid = dict->valueForKey("goodsId")->getCString();
    
    auto cmd = new WorldMarchCancelCommand(uuid,itemUuid);
    cmd->sendAndRelease();
}

void WorldMapView::afterCityMove(cocos2d::CCDictionary *dict) {
    unsigned int index = dict->valueForKey("point")->intValue();
    string itemUuid = dict->valueForKey("goodsId")->getCString();
    for (auto it = ToolController::getInstance()->m_toolInfos.begin(); it != ToolController::getInstance()->m_toolInfos.end(); it++) {
        if(it->second.uuid == itemUuid && it->first == ITEM_RANDOM){
            unsigned int current = WorldController::getInstance()->getCurrentMarchCount();
            if (current >= 1) {
                CCCommonUtils::flyHint("", "", _lang("108744"));
                return;
            }
            if (EnemyInfoController::getInstance()->m_curType==ENEMY_TYPE_BATTLE || EnemyInfoController::getInstance()->m_curType==ENEMY_TYPE_TEAM) {
                CCCommonUtils::flyHint("", "", _lang("108745"));
                return;
            }
        }
    }
    auto cmd = new WorldCityMoveCommand(index,itemUuid);
    string mailUid = "";
    if (dict->objectForKey("mailUid")) {
        mailUid = dict->valueForKey("mailUid")->getCString();
        cmd->addMailUid(mailUid);
    }
    if (dict->objectForKey("alMoveType")) {
        int t = dict->valueForKey("alMoveType")->intValue();
        if (t == 2 || t == 3)
        {
            cmd->setAlMovType(t);
        }
    }
    //    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(WorldMapView::onCityMoveCallback), NULL));
    cmd->sendAndRelease();
}

void WorldMapView::afterCrossCityMove(CCDictionary* dict){
    unsigned int index = dict->valueForKey("point")->intValue();
    string itemUuid = dict->valueForKey("goodsId")->getCString();
    auto cmd = new WorldCrossCityMoveCommand(index,itemUuid, GlobalData::shared()->playerInfo.currentServerId);
    cmd->sendAndRelease();
    m_touchDelegateView->notMove = true;
    isCrossServer = true;
}
void WorldMapView::gotoAddWorldItems(int itemid){
    if(!GlobalData::shared()->playerInfo.isInSelfServer()){
        CCCommonUtils::flyText(_lang("104948"));
        return;
    }
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        PopupViewController::getInstance()->removeAllPopupView();
        auto size = CCDirector::sharedDirector()->getWinSize();
        CCPoint p(size.width / 2 , size.height / 2);
        PopupViewController::getInstance()->setTouchPos(p);
        PopupViewController::getInstance()->addPopupView(MoveCityPopUpView::create(MoveCity_Wood,itemid));
    }else{
        int index = WorldController::getIndexByPoint(WorldController::getInstance()->selfPoint);
        WorldController::getInstance()->openTargetIndex = index;
        AllianceManager::getInstance()->goToWorldType = itemid;
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
}
void WorldMapView::afterAddWorldItems(cocos2d::CCDictionary *dict){
    unsigned int index = dict->valueForKey("point")->intValue();
    string itemUuid = dict->valueForKey("goodsId")->getCString();

    auto cmd = new WorldAddItemCommand(index,itemUuid);
    cmd->sendAndRelease();
}
void WorldMapView::onMarchCallback(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (ret && ret->getErrorCode() == Error_OP_FAILURE) {
        auto params = _dict(ret->getData());
        // refresh map point
        auto point = params->valueForKey("point")->intValue();
        WorldMapView::instance()->m_map->updateDynamicMap(WorldController::getPointByIndex(point));
//        update_water_shader();
    }
}
void WorldMapView::doAllianceArea(unsigned int type ,unsigned int index,bool isSuperMine/* false*/,bool isWarehouse/* false*/,WorldResourceType resType/* Wood*/,string aaid){
    m_untouchableTiles.push_back(index);
    auto &info = m_cityInfo[index];
    auto &info1 = m_cityInfo[index - 1];
    auto &info2 = m_cityInfo[index - WorldController::getInstance()->_current_tile_count_x - 1];
    auto &info3 = m_cityInfo[index - WorldController::getInstance()->_current_tile_count_x];
    if (type == 1) {
        int nIcon = 9;
        WorldCityType tempCityType = Tile_allianceArea;
        
        if(isSuperMine){
            nIcon = 10;
            tempCityType = tile_superMine;
        }
        else if (isWarehouse){
            nIcon = 12;
            tempCityType = tile_wareHouse;
        }
        
        info.cityIcon = nIcon;
        info.cityType = tempCityType;
        
        info1.cityIcon = nIcon;
        info1.cityType = tempCityType;
        
        
        info2.cityIcon = nIcon;
        info2.cityType = tempCityType;
        
        info3.cityIcon = nIcon;
        info3.cityType = tempCityType;
        
        
        info.parentCityIndex = index;
        info1.parentCityIndex = index;
        info2.parentCityIndex = index;
        info3.parentCityIndex = index;
        if(isSuperMine && !isWarehouse){
            info.m_superMineInfo.trstat = Astate_BeforeBuild;
            info.m_superMineInfo.type = resType;
            info.m_aArea_id = aaid;
            
            info1.m_superMineInfo.trstat = Astate_BeforeBuild;
            info1.m_superMineInfo.type = resType;
            info1.m_aArea_id = aaid;
            
            info2.m_superMineInfo.trstat = Astate_BeforeBuild;
            info2.m_superMineInfo.type = resType;
            info2.m_aArea_id = aaid;
            
            info3.m_superMineInfo.trstat = Astate_BeforeBuild;
            info3.m_superMineInfo.type = resType;
            info3.m_aArea_id = aaid;
        }else if (!isSuperMine && isWarehouse){
            info.m_warehouseInfo.state = Astate_BeforeBuild;
            info.m_aArea_id = aaid;
            
            info1.m_warehouseInfo.state = Astate_BeforeBuild;
            info1.m_aArea_id = aaid;
            
            info2.m_warehouseInfo.state = Astate_BeforeBuild;
            info2.m_aArea_id = aaid;
            
            info3.m_warehouseInfo.state = Astate_BeforeBuild;
            info3.m_aArea_id = aaid;
        }else{
            info.m_allianceAreaInfo.state = Astate_BeforeBuild;
        }
        
    }
    // remove origin
    delUnderNode(info.cityIndex);
    delUnderNode(info1.cityIndex);
    delUnderNode(info2.cityIndex);
    delUnderNode(info3.cityIndex);
    m_tileLayer->revertTileGID(info.cityTilePoint);
    m_tileLayer->revertTileGID(info1.cityTilePoint);
    m_tileLayer->revertTileGID(info2.cityTilePoint);
    m_tileLayer->revertTileGID(info3.cityTilePoint);
    // add tmp city
    float moveHeight = 50.0;
    float waitTime = 2.0;
    float moveTime = 0.25;
    float delayTime = 1.5;
    auto cityNode = CCNode::create();
    
    int mapIndex = 3;
    while (mapIndex >= 0) {
        auto arr = getCityPicArr(mapIndex, 0, false,getPicIdByCityInfo(info));
        int posX = 0;
        int posY = 0;
        if(mapIndex == 1){
            posX = -_tile_width / 2;
            posY = _tile_height / 2;
        }
        if(mapIndex == 2){
            posY = _tile_height;
        }
        if(mapIndex == 3){
            posX = _tile_width / 2;
            posY = _tile_height / 2;
        }
        CCObject *obj = NULL;
        CCARRAY_FOREACH(arr, obj){
            std::string picName = _dict(obj)->valueForKey("pic")->getCString();
            int addX = _dict(obj)->valueForKey("x")->intValue();
            int addY = _dict(obj)->valueForKey("y")->intValue();
            auto sprite = CCLoadSprite::createSprite(picName.c_str());
            sprite->setAnchorPoint(ccp(0, 0));
            cityNode->addChild(sprite);
            sprite->setPosition(ccp(posX, posY) - ccp(_tile_width / 2, _tile_height / 2) + ccp(addX, addY));
        }
        mapIndex--;
    }
    
	// the resource is in 507 which has been loaded in init method
    if (44996 == getPicIdByCityInfo(info)) {
        CCPointArray* pointArr = CCPointArray::create(3);
        pointArr->addControlPoint(ccp(-70,59));
        pointArr->addControlPoint(ccp(-2,23));
        pointArr->addControlPoint(ccp(66,57));
        for (int i = 0; i < 3; i++) {
            auto bannerSprite = CCFlagWaveSprite::create(CCLoadSprite::loadResource("1212_double.png"));
            bannerSprite->setScale(0.18);
            bannerSprite->setPosition(ccp(0, _tile_height / 2)+pointArr->getControlPointAtIndex(i) + ccp(12, 20));
            cityNode->addChild(bannerSprite);
        }
    }

    cityNode->setTag(index);
    auto pos = m_map->getViewPointByTilePoint(info.cityTilePoint);
    pos = ccp(pos.x, pos.y + (_tile_true_height - _tile_height) / 2);
    m_layers[WM_CITY]->addChild(cityNode);
    auto callbackArr = CCArray::create();
    callbackArr->addObject(CCInteger::create(index));
    callbackArr->addObject(CCInteger::create(type));
    int tempType = 0;
    if(isSuperMine||isWarehouse){
        tempType = 1;
    }
    callbackArr->addObject(CCInteger::create(tempType));//isSuperMine||isWareHouse
    auto callback = CCCallFuncO::create(this, callfuncO_selector(WorldMapView::finishAllianceArea), callbackArr);
    auto callbackPar = CCCallFuncO::create(this, callfuncO_selector(WorldMapView::addCityMoveParticle), callbackArr);
    
    if (type == 1) {
        // teleport in
        int cityIndex = 0;
        Vector<FiniteTimeAction*> actionArr;
        while(cityIndex < cityNode->getChildrenCount()){
            auto sprite = dynamic_cast<CCSprite*>(cityNode->getChildren().at(cityIndex));
            if(sprite){
                sprite->setOpacity(0);
                actionArr.pushBack(CCTargetedAction::create(sprite, CCFadeIn::create(moveTime)));
            }
            cityIndex++;
        }
        
        // add target particle
        string tmpUIStart = "CityMoveUI_";
        int uiCount = 4;
        for (int i=1; i<=uiCount; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpUIStart.c_str(),i)->getCString());
            particle->setPosition(m_map->getViewPointByTilePoint(WorldController::getPointByIndex(index))+ ccp(0, _tile_height / 2));
            addParticleToBatch(particle,getBatchTag(CityMoveInTag, index));
        }
        
        CCSpawn *spawn = CCSpawn::create(actionArr);
        
        auto action = CCSequence::create(CCDelayTime::create(waitTime + 2*moveTime)
                                         ,spawn
                                         ,CCMoveTo::create(moveTime, pos),
                                         callbackPar,
                                         CCDelayTime::create(delayTime)
                                         ,callback,
                                         nullptr);
        cityNode->setPosition(ccp(pos.x, pos.y + moveHeight));
        cityNode->runAction(action);
        
    } else {
        // allianceArea out
        info.cityIcon = -1;
        info.cityType = OriginTile;
        info.playerName = "";
        
        info1.cityIcon = -1;
        info1.cityType = OriginTile;
        info1.playerName = "";
        
        info2.cityIcon = -1;
        info2.cityType = OriginTile;
        info2.playerName = "";
        
        info3.cityIcon = -1;
        info3.cityType = OriginTile;
        info3.playerName = "";
        
        info.parentCityIndex = -1;
        info1.parentCityIndex = -1;
        info2.parentCityIndex = -1;
        info3.parentCityIndex = -1;
        
        int cityIndex = 0;
        Vector<FiniteTimeAction*> actionArr;
        while(cityIndex < cityNode->getChildrenCount()){
            auto sprite = dynamic_cast<CCSprite*>(cityNode->getChildren().at(cityIndex));
            if(sprite){
                actionArr.pushBack(CCTargetedAction::create(sprite, CCFadeOut::create(moveTime)));
            }
            cityIndex++;
        }
        
        waitTime =  0.25;
        auto action = CCSequence::create(CCDelayTime::create(waitTime),
                                         CCSpawn::createWithTwoActions(callbackPar,
                                                                       CCSpawn::createWithTwoActions(CCSpawn::create(actionArr) ,
                                                                                                     CCMoveTo::create(moveTime, ccp(pos.x, pos.y + moveHeight))
                                                                                                     )),
                                         CCDelayTime::create(delayTime),
                                         callback,nullptr);
        cityNode->setPosition(pos);
        cityNode->runAction(action);
        CCNode* aniNode =  m_aAreaAniNode->getChildByTag(info.cityIndex);
        if (aniNode && aniNode->getParent()) {
            aniNode->removeFromParentAndCleanup(true);
        }
    }
}
void WorldMapView::finishAllianceArea(CCObject* pObj){
    auto teleportInfo = dynamic_cast<CCArray*>(pObj);
    unsigned int index = dynamic_cast<CCInteger*>(teleportInfo->objectAtIndex(0))->getValue();
    unsigned int type = dynamic_cast<CCInteger*>(teleportInfo->objectAtIndex(1))->getValue();
    int isSuperMine = dynamic_cast<CCInteger*>(teleportInfo->objectAtIndex(2))->getValue();
    auto &info = m_cityInfo[index];
    // remove tmp city
    auto tmp = m_layers[WM_CITY]->getChildByTag(index);
    if (tmp) {
        tmp->removeFromParent();
    }
    if (type == 1) {
        // teleport in
        // remove particle
        delBatchItem(CityMoveInTag, index);
        if (isSuperMine == 0) {
            info.m_allianceAreaInfo.state = Astate_BeforeBuild;
        }
        else{
            info.m_superMineInfo.trstat = Astate_BeforeBuild;
        }
        createCity(info);
    } else {
        // remove particle
        delBatchItem(CityMoveOutTag, index);
    }
    if (!isSuperMine) {
        m_map->updateDynamicMap();
//        update_water_shader();
    }
    m_untouchableTiles.erase(std::remove(m_untouchableTiles.begin(), m_untouchableTiles.end(), index),m_untouchableTiles.end());
}
void WorldMapView::pushAddTower(unsigned int type, unsigned int index,int dialog,string aaid,bool isBanner/*false*/,std::string banner/*""*/) {
    m_untouchableTiles.push_back(index);
    auto &info = m_cityInfo[index];
    
    if (type == 1) {
        info.cityIcon = 11;
        info.cityType = tile_tower;
        info.m_towerInfo.dialog = dialog;
        info.m_towerInfo.state = Astate_BeforeBuild;
        info.m_aArea_id = aaid;
        
        if (isBanner) {
            info.cityIcon = 13;
            info.cityType = tile_banner;
            info.m_bannerInfo.state = Astate_BeforeBuild;
            info.m_aArea_id = aaid;
        }
    }
    // remove origin
    delUnderNode(info.cityIndex);
    
    m_tileLayer->revertTileGID(info.cityTilePoint);
    
    // add tmp city
    float moveHeight = 50.0;
    float waitTime = 2.0;
    float moveTime = 0.25;
    float delayTime = 1.5;
    auto cityNode = CCNode::create();
    if (isBanner) {
        string spStr = "allianceTowerBad.png";
        if (type == 0) {
            if (info.m_bannerInfo.state == Astate_BeforeBuild) {
                spStr = "allianceTowerBad.png";
            }else if (info.m_bannerInfo.state == Astate_Building) {
                spStr = "allianceTowerBad.png";
            }else if (info.m_bannerInfo.state == Astate_UnGarrison) {
                spStr = "allianceTower.png";
            }else if (info.m_bannerInfo.state == Astate_Garrison) {
                spStr = "allianceTower.png";
            }else if (info.m_bannerInfo.state == Astate_Damaged) {
                spStr = "allianceTowerBad.png";
            }else if (info.m_bannerInfo.state == Astate_FixIng) {
                spStr = "allianceTowerBad.png";
            }else if (info.m_bannerInfo.state == Astate_Destroy) {
                spStr = "allianceTowerBad.png";
            }
        }
        auto citySp = CCLoadSprite::createSprite(spStr.c_str());
        citySp->setAnchorPoint(ccp(0, 0));
        citySp->setPosition(ccp(59, 29) - ccp(_halfTileSize.width, _halfTileSize.height));
        cityNode->addChild(citySp);
        
//        banner.append(".png");
//        CCLoadSprite::doResourceByCommonIndex(208, true);
//        auto bannerSprite = CCFlagWaveSprite::create(CCLoadSprite::loadResource(banner.c_str()));
//        bannerSprite->setAnchorPoint(ccp(0, 0));
//        if (CCCommonUtils::isIosAndroidPad()) {
//            bannerSprite->setScale(1.0);
//        }
//        else
//            bannerSprite->setScale(0.5);
//        bannerSprite->setPosition(citySp->getPosition() + citySp->getContentSize() * citySp->getScale() / 2  + ccp(0, 10));
//        cityNode->addChild(bannerSprite);
//        CCLoadSprite::doResourceByCommonIndex(208, false);

    }
    else {
        string spStr = "allianceTowerBad.png";
        if (type == 0 && info.m_towerInfo.state == AAreaState::Astate_UnGarrison) {
            spStr = "allianceTower.png";
        }
        auto citySp = CCLoadSprite::createSprite(spStr.c_str());
        citySp->setAnchorPoint(ccp(0, 0));
        citySp->setPosition(ccp(59, 29) - ccp(_halfTileSize.width, _halfTileSize.height));
        cityNode->addChild(citySp);
    }
    
    cityNode->setTag(index);
    auto pos = m_map->getViewPointByTilePoint(info.cityTilePoint);
    //    pos = ccp(pos.x, pos.y + (_tile_true_height - _tile_height) / 2);
    m_layers[WM_CITY]->addChild(cityNode);
    
    auto callbackArr = CCArray::create();
    callbackArr->addObject(CCInteger::create(index));
    callbackArr->addObject(CCInteger::create(type));
    callbackArr->addObject(CCInteger::create(0));//isAArea
    auto callback = CCCallFuncO::create(this, callfuncO_selector(WorldMapView::onFinishAddTower), callbackArr);
    auto callbackPar = CCCallFuncO::create(this, callfuncO_selector(WorldMapView::addCityMoveParticle), callbackArr);
    
    if (type == 1) {
        // teleport in
        int cityIndex = 0;
        Vector<FiniteTimeAction*> actionArr;
        
        while(cityIndex < cityNode->getChildrenCount()){
            auto sprite = dynamic_cast<CCSprite*>(cityNode->getChildren().at(cityIndex));
            if(sprite){
                sprite->setOpacity(0);
                actionArr.pushBack(CCTargetedAction::create(sprite, CCFadeIn::create(moveTime)));
            }
            cityIndex++;
        }
        
        // add target particle
        string tmpUIStart = "CityMoveUI_";
        int uiCount = 4;
        for (int i=1; i<=uiCount; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpUIStart.c_str(),i)->getCString());
            particle->setPosition(m_map->getViewPointByTilePoint(WorldController::getPointByIndex(index)));
            addParticleToBatch(particle,getBatchTag(CityMoveInTag, index));
        }
        
        CCSpawn *spawn = CCSpawn::create(actionArr);
        
        auto action = CCSequence::create(CCDelayTime::create(waitTime + 2*moveTime)
                                         ,spawn
                                         ,CCMoveTo::create(moveTime, pos),
                                         callbackPar,
                                         CCDelayTime::create(delayTime)
                                         ,callback,
                                         nullptr);
        cityNode->setPosition(ccp(pos.x, pos.y + moveHeight));
        cityNode->runAction(action);
        
    } else {
        // teleport out
        if (!isBanner) {
            m_towerRangeMaskNode->removeChildByTag(index);
        }
        info.cityIcon = -1;
        info.cityType = OriginTile;
        info.playerName = "";
        int cityIndex = 0;
        Vector<FiniteTimeAction*> actionArr;
        
        while(cityIndex < cityNode->getChildrenCount()){
            auto sprite = dynamic_cast<CCSprite*>(cityNode->getChildren().at(cityIndex));
            if(sprite){
                actionArr.pushBack(CCTargetedAction::create(sprite, CCFadeOut::create(moveTime)));
            }
            cityIndex++;
        }
        
        auto action = CCSequence::create(CCDelayTime::create(waitTime),
                                         CCSpawn::createWithTwoActions(callbackPar,
                                                                       CCSpawn::createWithTwoActions(CCSpawn::create(actionArr) ,
                                                                                                     CCMoveTo::create(moveTime, ccp(pos.x, pos.y + moveHeight))
                                                                                                     )),
                                         CCDelayTime::create(delayTime),
                                         callback,nullptr);
        cityNode->setPosition(pos);
        cityNode->runAction(action);
    }
}
void WorldMapView::doTeleport(unsigned int type, unsigned int index, const string &name) {
    m_untouchableTiles.push_back(index);
    auto &info = m_cityInfo[index];
    auto &info1 = m_cityInfo[index - 1];
    auto &info2 = m_cityInfo[index - WorldController::getInstance()->_current_tile_count_x - 1];
    auto &info3 = m_cityInfo[index - WorldController::getInstance()->_current_tile_count_x];
    
    if (type == 1) {
        info.cityIcon = 6;
        info.cityType = CityTile;
        info.playerName = name;
        
        info1.cityIcon = 6;
        info1.cityType = CityTile;
        
        info1.playerName = name;
        
        info2.cityIcon = 6;
        info2.cityType = CityTile;
        info2.playerName = name;
        
        info3.cityIcon = 6;
        info3.cityType = CityTile;
        info3.playerName = name;
        info.parentCityIndex = index;
        info1.parentCityIndex = index;
        info2.parentCityIndex = index;
        info3.parentCityIndex = index;
    }
    
    auto &player = WorldController::getInstance()->m_playerInfo[name];
    
    // remove origin
    delUnderNode(info.cityIndex);
    delUnderNode(info1.cityIndex);
    delUnderNode(info2.cityIndex);
    delUnderNode(info3.cityIndex);
    
    m_tileLayer->revertTileGID(info.cityTilePoint);
    m_tileLayer->revertTileGID(info1.cityTilePoint);
    m_tileLayer->revertTileGID(info2.cityTilePoint);
    m_tileLayer->revertTileGID(info3.cityTilePoint);
    
    // add tmp city
    float moveHeight = 50.0;
    float waitTime = 2.0;
    float moveTime = 0.25;
    float delayTime = 1.5;
    auto cityNode = CCNode::create();
    
    int level = player.cityLv;
    int mapIndex = 3;
    while (mapIndex >= 0) {
        auto arr = getCityPicArr(mapIndex, level, player.officer == KINGDOM_KING_ID);
        int posX = 0;
        int posY = 0;
        if(mapIndex == 1){
            posX = -_tile_width / 2;
            posY = _tile_height / 2;
        }
        if(mapIndex == 2){
            posY = _tile_height;
        }
        if(mapIndex == 3){
            posX = _tile_width / 2;
            posY = _tile_height / 2;
        }
        CCObject *obj = NULL;
        CCARRAY_FOREACH(arr, obj){
            std::string picName = _dict(obj)->valueForKey("pic")->getCString();
            int addX = _dict(obj)->valueForKey("x")->intValue();
            int addY = _dict(obj)->valueForKey("y")->intValue();
            auto sprite = CCLoadSprite::createSprite(picName.c_str());
            sprite->setAnchorPoint(ccp(0, 0));
            cityNode->addChild(sprite);
            sprite->setPosition(ccp(posX, posY) - ccp(_tile_width / 2, _tile_height / 2) + ccp(addX, addY));
        }
        mapIndex--;
    }
    
    cityNode->setTag(index);
    auto pos = m_map->getViewPointByTilePoint(info.cityTilePoint);
    pos = ccp(pos.x, pos.y + (_tile_true_height - _tile_height) / 2);
    m_layers[WM_CITY]->addChild(cityNode);
    
    auto callbackArr = CCArray::create();
    callbackArr->addObject(CCInteger::create(index));
    callbackArr->addObject(CCInteger::create(type));
    callbackArr->addObject(CCInteger::create(0));//isAArea
    auto callback = CCCallFuncO::create(this, callfuncO_selector(WorldMapView::finishTeleport), callbackArr);
    auto callbackPar = CCCallFuncO::create(this, callfuncO_selector(WorldMapView::addCityMoveParticle), callbackArr);
    
    if (type == 1) {
        // teleport in
        int cityIndex = 0;
        Vector<FiniteTimeAction*> actionArr;
        
        auto& arr = cityNode->getChildren();
        for(auto child : arr)
        {
            auto sprite = dynamic_cast<CCSprite*>(child);
            if(sprite){
                sprite->setOpacity(0);
                actionArr.pushBack(CCTargetedAction::create(sprite, CCFadeIn::create(moveTime)));
            }
        }
//        while(cityIndex < cityNode->getChildrenCount()){
//            auto sprite = dynamic_cast<CCSprite*>(cityNode->getChildren()->objectAtIndex(cityIndex));
//            if(sprite){
//                sprite->setOpacity(0);
//                actionArr->addObject(CCTargetedAction::create(sprite, CCFadeIn::create(moveTime)));
//            }
//            cityIndex++;
//        }
        
        // add target particle
        string tmpUIStart = "CityMoveUI_";
        int uiCount = 4;
        for (int i=1; i<=uiCount; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpUIStart.c_str(),i)->getCString());
            particle->setPosition(m_map->getViewPointByTilePoint(WorldController::getPointByIndex(index)) + ccp(0, _tile_height / 2));
            addParticleToBatch(particle,getBatchTag(CityMoveInTag, index));
        }
        
        CCSpawn *spawn = CCSpawn::create(actionArr);
        
        auto action = CCSequence::create(CCDelayTime::create(waitTime + 2*moveTime)
                                         ,spawn
                                         ,CCMoveTo::create(moveTime, pos),
                                         callbackPar,
                                         CCDelayTime::create(delayTime)
                                         ,callback,
                                         nullptr);
        cityNode->setPosition(ccp(pos.x, pos.y + moveHeight));
        cityNode->runAction(action);
        
    } else {
        // teleport out
        
        info.cityIcon = -1;
        info.cityType = OriginTile;
        info.playerName = "";
        
        info1.cityIcon = -1;
        info1.cityType = OriginTile;
        info1.playerName = "";
        
        info2.cityIcon = -1;
        info2.cityType = OriginTile;
        info2.playerName = "";
        
        info3.cityIcon = -1;
        info3.cityType = OriginTile;
        info3.playerName = "";
        
        info.parentCityIndex = -1;
        info1.parentCityIndex = -1;
        info2.parentCityIndex = -1;
        info3.parentCityIndex = -1;
        
        int cityIndex = 0;
        Vector<FiniteTimeAction*> actionArr;
        auto& arr = cityNode->getChildren();
        for(auto child : arr)
        {
            auto sprite = dynamic_cast<CCSprite*>(child);
            if(sprite){
                actionArr.pushBack(CCTargetedAction::create(sprite, CCFadeOut::create(moveTime)));
            }
        }
        
        auto action = CCSequence::create(CCDelayTime::create(waitTime),
                                         CCSpawn::createWithTwoActions(callbackPar,
                                                                       CCSpawn::createWithTwoActions(CCSpawn::create(actionArr) ,
                                                                                                     CCMoveTo::create(moveTime, ccp(pos.x, pos.y + moveHeight))
                                                                                                     )),
                                         CCDelayTime::create(delayTime),
                                         callback,nullptr);
        cityNode->setPosition(pos);
        cityNode->runAction(action);
    }
}

void WorldMapView::updateWinNum(int index){
    int shengIndex = index*10;
    auto liansheng = m_csNode->getChildByTag(shengIndex);
    if (liansheng) {
        float delayTime = 3;
        auto &info = m_cityInfo[index];
        bool flag = info.winNum>=ActivityController::getInstance()->wb_info_k11;
        CCActionInterval * rotateto1 = CCRotateTo::create(1, 150);
        CCFiniteTimeAction* rotateAction = CCSequence::create(CCDelayTime::create(delayTime),rotateto1,NULL);
        liansheng->runAction(rotateAction);
        liansheng->setVisible(flag);
        
        auto liansheng2 = m_csNode->getChildByTag(shengIndex+1);
        if (liansheng2) {
            liansheng2->setVisible(flag);
        }
        auto liansheng3 = m_csNode->getChildByTag(shengIndex+2);
        auto lianTxt = dynamic_cast<CCLabelIFBMFont*>(m_lianNode->getChildByTag(index));
        
        CCFiniteTimeAction* cs2 = CCSequence::create(CCDelayTime::create(delayTime+1.25),CCFadeTo::create(0.1,180),CCFadeTo::create(0.1,120),CCFadeTo::create(0.05,0),NULL);
        if (liansheng3) {
           liansheng3->runAction(cs2);
           liansheng3->setVisible(flag);
        }
        
        CCScaleTo* action1 = CCScaleTo::create(0.1, 2.2);
        CCDelayTime* action2 = CCDelayTime::create(0.02);
        CCScaleTo* action3 = CCScaleTo::create(0.1, 1);
        CCFiniteTimeAction* cs = CCSequence::create(CCDelayTime::create(delayTime+1),action1,action2,action3,NULL);
        if (lianTxt) {
            auto &info = m_cityInfo[index];
            lianTxt->setString(CC_ITOA(info.winNum));
            lianTxt->runAction(cs);
            lianTxt->setVisible(flag);
        }
    }else{
        createWinNum(index);
    }
}

void WorldMapView::createWinNum(int index){
    if(GlobalData::shared()->playerInfo.currentServerId < CROSS_SERVER_START_INDEX){
        return;
    }
    auto &info = m_cityInfo[index];
    if (info.winNum<ActivityController::getInstance()->wb_info_k11) {
        return ;
    }
    auto pos = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(index),info.tileServerId);
    int shengIndex = index*10;
    auto liansheng =  CCLoadSprite::createSprite("liansheng_00.png");
    liansheng->setPosition(ccp(0, 15)+pos);
    liansheng->setTag(shengIndex);
    m_csNode->addChild(liansheng);
    m_cityItem[index].push_back(liansheng);
    
    float delayTime = 0.2;
    CCActionInterval * rotateto1 = CCRotateTo::create(1, 150);
    CCFiniteTimeAction* rotateAction = CCSequence::create(CCDelayTime::create(delayTime),rotateto1,NULL);
    liansheng->runAction(rotateAction);
    
    auto liansheng2 =  CCLoadSprite::createSprite("liansheng_01.png");
    liansheng2->setPosition(ccp(0, 15)+pos);
    liansheng2->setTag(shengIndex+1);
    m_csNode->addChild(liansheng2);
    m_cityItem[index].push_back(liansheng2);
    
    auto liansheng3 =  CCLoadSprite::createSprite("liansheng_02.png");
    liansheng3->setPosition(ccp(0, 15)+pos);
    liansheng3->setTag(shengIndex+2);
    m_csNode->addChild(liansheng3);
    liansheng3->setOpacity(0);
    m_cityItem[index].push_back(liansheng3);
    
    CCFiniteTimeAction* cs2 = CCSequence::create(CCDelayTime::create(delayTime+1.25),CCFadeTo::create(0.1,180),CCFadeTo::create(0.1,120),CCFadeTo::create(0.05,0),NULL);
    liansheng3->runAction(cs2);
    
    CCLabelIFBMFont* lianTxt = CCLabelIFBMFont::create("",15);
    lianTxt->setFntFile("number.fnt");
    lianTxt->setString(CC_ITOA(info.winNum));
    lianTxt->setAnchorPoint(CCPoint(0.5, 0));
    lianTxt->setTag(index);
    lianTxt->setColor({255,0,0});
    lianTxt->setPosition(ccp(0, 4)+pos);
    m_lianNode->addChild(lianTxt);
    m_cityItem[index].push_back(lianTxt);
    
    CCScaleTo* action1 = CCScaleTo::create(0.1, 2.2);
    CCDelayTime* action2 = CCDelayTime::create(0.02);
    CCScaleTo* action3 = CCScaleTo::create(0.1, 1);
    CCFiniteTimeAction* cs = CCSequence::create(CCDelayTime::create(delayTime+1),action1,action2,action3,NULL);
    lianTxt->runAction(cs);
}

void WorldMapView::addCityMoveParticle(cocos2d::CCObject *obj) {
    auto infoArr = dynamic_cast<CCArray*>(obj);
    auto index = dynamic_cast<CCInteger*>(infoArr->objectAtIndex(0))->getValue();
    auto type = dynamic_cast<CCInteger*>(infoArr->objectAtIndex(1))->getValue();
    auto pos = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(index));
    BatchTagType batchType;
    int count;
    int start;
    string tmpStart = "CityMove_";
    if (type == 1) {
        // teleport in
        batchType = CityMoveInTag;
        start = 4;
        count = 6;
        // remove target particle
        delBatchItem(CityMoveInTag, index);
    } else {
        // teleport out
        batchType = CityMoveOutTag;
        start = 0;
        count = 6;
    }
    for (int i=start; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(pos);
        particle->setAutoRemoveOnFinish(true);
        addParticleToBatch(particle,getBatchTag(batchType, index));
    }
}
void WorldMapView::onFinishAddTower(cocos2d::CCObject *obj) {
    auto teleportInfo = dynamic_cast<CCArray*>(obj);
    unsigned int index = dynamic_cast<CCInteger*>(teleportInfo->objectAtIndex(0))->getValue();
    unsigned int type = dynamic_cast<CCInteger*>(teleportInfo->objectAtIndex(1))->getValue();
    auto &info = m_cityInfo[index];
    
    // remove tmp city
    auto tmp = m_layers[WM_CITY]->getChildByTag(index);
    if (tmp) {
        tmp->removeFromParent();
    }
    if (type == 1) {
        // in
        // remove particle
        delBatchItem(CityMoveInTag, index);
        createCity(info);
    } else {
        // remove particle
        delBatchItem(CityMoveOutTag, index);
    }
    
    m_untouchableTiles.erase(std::remove(m_untouchableTiles.begin(), m_untouchableTiles.end(), index),m_untouchableTiles.end());
}

void WorldMapView::finishTeleport(cocos2d::CCObject *obj) {
    auto teleportInfo = dynamic_cast<CCArray*>(obj);
    unsigned int index = dynamic_cast<CCInteger*>(teleportInfo->objectAtIndex(0))->getValue();
    unsigned int type = dynamic_cast<CCInteger*>(teleportInfo->objectAtIndex(1))->getValue();
    auto &info = m_cityInfo[index];
    auto &info1 = m_cityInfo[index - 1];
    auto &info2 = m_cityInfo[index - WorldController::getInstance()->_current_tile_count_x - 1];
    auto &info3 = m_cityInfo[index - WorldController::getInstance()->_current_tile_count_x];
    
    // remove tmp city
    auto tmp = m_layers[WM_CITY]->getChildByTag(index);
    if (tmp) {
        tmp->removeFromParent();
    }
    
    if (type == 1) {
        // teleport in
        
        // remove particle
        delBatchItem(CityMoveInTag, index);
        createCity(info);
        createCity(info1);
        createCity(info2);
        createCity(info3);
    } else {
        // remove particle
        delBatchItem(CityMoveOutTag, index);
    }
    
    m_untouchableTiles.erase(std::remove(m_untouchableTiles.begin(), m_untouchableTiles.end(), index),m_untouchableTiles.end());
}
void WorldMapView::onPurgeSpinNode(CCObject* obj){
    CCArray* dataArr = dynamic_cast<CCArray*>(obj);
    string uuid = dynamic_cast<CCString*>(dataArr->objectAtIndex(0))->getCString();
    int spinTag = dynamic_cast<CCString*>(dataArr->objectAtIndex(1))->intValue();
    int firIndex = dynamic_cast<CCString*>(dataArr->objectAtIndex(2))->intValue();
    
    auto & info = WorldController::getInstance()->m_marchInfo[uuid];
    info.action = MarchActionNone;
    delBatchItem(WarFire, firIndex);
    if (info.ownerType == PlayerSelf && info.stateType == StateReturn) {
        QueueController::getInstance()->addMarchQueueInfo(uuid);
    }
    if(m_mapHeiqishiMarchNode->getChildByTag(spinTag)){
        m_mapHeiqishiMarchNode->removeChildByTag(spinTag);
    }
    
    if (m_heiqishiLvBgBachNode->getChildByTag(spinTag)) {
        m_heiqishiLvBgBachNode->removeChildByTag(spinTag);
    }
    if (m_heiqishiShadowBachNode->getChildByTag(spinTag)) {
        m_heiqishiShadowBachNode->removeChildByTag(spinTag);
    }
    if (m_spinLabelNode->getChildByTag(spinTag)) {
        m_spinLabelNode->removeChildByTag(spinTag);
    }
    if (m_heiqishiLvLbNode->getChildByTag(spinTag)) {
        m_heiqishiLvLbNode->removeChildByTag(spinTag);
    }
    if(m_drawRoadNode->getChildByTag(info.marchTag)){
        m_drawRoadNode->getChildByTag(info.marchTag)->removeFromParentAndCleanup(true);
    }
    if(m_touchTroopNode->getChildByTag(info.marchTag)){
        m_touchTroopNode->getChildByTag(info.marchTag)->removeFromParentAndCleanup(true);
    }
    CC_SAFE_RELEASE_NULL(dataArr);
    info.endStamp = 1;
}
void WorldMapView::onFinishSpinBattle(CCObject* obj){
    CCLOGFUNC("onFinishSpinBattle");
    CCArray* dataArr = dynamic_cast<CCArray*>(obj);
    string uuid = dynamic_cast<CCString*>(dataArr->objectAtIndex(0))->getCString();
    auto & info = WorldController::getInstance()->m_marchInfo[uuid];
    IFHeiqishiNode* tempNode = dynamic_cast<IFHeiqishiNode*>(m_mapHeiqishiMarchNode->getChildByTag(info.marchTag));
    if (tempNode) {
        tempNode->onStopCityAttack();
    }
    if(false){//info.isKilled
        info.endStamp = 1;
        if(m_mapHeiqishiMarchNode->getChildByTag(info.marchTag)){
            if(tempNode){
                tempNode->setSpineState(tempNode->getSkeName(), MonsterDead, 0,false,NULL);
                CCDelayTime* delay = CCDelayTime::create(1.0);
                CCCallFuncO* callfun = CCCallFuncO::create(this, callfuncO_selector(WorldMapView::onPurgeSpinNode), dataArr);
                CCSequence* sq = CCSequence::create(delay,callfun,NULL);
                tempNode->runAction(sq);
            }
        }
    }
    else{
        onPurgeSpinNode(dataArr);
    }
}
void WorldMapView::finishBattleAni(cocos2d::CCObject *obj) {
    auto str = dynamic_cast<CCString*>(obj);
    string uuid = str->getCString();
    auto & info = WorldController::getInstance()->m_marchInfo[uuid];
    info.action = MarchActionNone;
    if(info.isKilled){
        info.endStamp = 1;
    }
    if (info.ownerType == PlayerSelf && info.stateType == StateReturn) {
        QueueController::getInstance()->addMarchQueueInfo(uuid);
    }
    
    map<unsigned int, MarchArmy*>::iterator it = m_marchArmy.find(info.marchTag);
    if(it != m_marchArmy.end()){
        it->second->armyDelete();
        it->second->release();
        m_marchArmy.erase(it);
    }
    if(m_drawRoadNode->getChildByTag(info.marchTag)){
        m_drawRoadNode->getChildByTag(info.marchTag)->removeFromParentAndCleanup(true);
    }
    if(m_touchTroopNode->getChildByTag(info.marchTag)){
        m_touchTroopNode->getChildByTag(info.marchTag)->removeFromParentAndCleanup(true);
    }
}

bool WorldMapView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(isMiniMapModel()){
        return false;
    }
    if(isTouchInside(UIComponent::getInstance()->m_miniBG, pTouch)){
        return true;
    }
    if(isTouchInside(m_directionIcon, pTouch) && m_directionIcon->isVisible()){
        return true;
    }
    if(isTouchInside(UIComponent::getInstance()->m_coordBG, pTouch)){
        return true;
    }
    if(isTouchInside(UIComponent::getInstance()->m_faveBG, pTouch)){
        return true;
    }
    if(isTouchInside(UIComponent::getInstance()->m_goHomeBtn, pTouch) && !GlobalData::shared()->playerInfo.isInSelfServer()){
        return true;
    }
    return false;
}

void WorldMapView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
}

void WorldMapView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(isTouchInside(UIComponent::getInstance()->m_miniBG, pTouch)){
        if(!isMiniMapModel()){
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            if (GlobalData::shared()->isCrossService) {
                if (GlobalData::shared()->playerInfo.currentServerId >CROSS_SERVER_START_INDEX) {
                    //todo
                    if (!WorldMapView::instance()) {
                        return;
                    }
                    if(GlobalData::shared()->playerInfo.uid == ""){
                        return;
                    }
                    unsigned int type = 1; // 0 : normal 1: single point
                    if(!GuideController::share()->updateWorldInfo){
                        return;
                    }
                    // do other update request here
                    
                    WorldMapView::instance()->m_updateTimeStamp = time(NULL);
                    CCLOG("update timeStamp: %d",WorldMapView::instance()->m_updateTimeStamp);
                    
                    //CCPoint point = WorldController::getInstance()->getPointByIndex(GlobalData::shared()->playerInfo.srcServerPointId);
                    //gotoTilePoint(point, GlobalData::shared()->playerInfo.crossFightSrcServerId, false);
                    
                    if(m_mapType == SERVERFIGHT_MAP){
                        UIComponent::getInstance()->setServerText(GlobalData::shared()->playerInfo.currentServerId);
                        GlobalData::shared()->playerInfo.currentServerId = GlobalData::shared()->playerInfo.crossFightSrcServerId;
                        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, GlobalData::shared()->playerInfo.srcServerPointId, NORMAL_MAP);
                    }else{
                        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, -1, SERVERFIGHT_MAP);
                    }
                    return ;
                }
            }
            if (DynamicResourceController::getInstance()->checkMinimapViewResource()) {
                auto miniMap = MinimapView::create(GlobalData::shared()->playerInfo.currentServerId);
                PopupViewController::getInstance()->addPopupInView(miniMap);
                miniMap->setTag(miniMapTag);
                if(m_layers[WM_TILE]->getChildrenCount() > 0){
                    auto tilePop = dynamic_cast<NewBaseTileInfo*>(m_layers[WM_TILE]->getChildren().at(0));
                    tilePop->closeThis();
                }
                tilePops->removeAllObjects();
            }
            else {
//                isDownloadingMinimap = true;
                CCCommonUtils::flyHint("", "", _lang("E100091"), 3, 0, true);
                auto temploadingIcon = CCLoadSprite::createSprite("loading_1.png");
                auto loadingSize = m_loadingIcon->getContentSize();
                if (CCCommonUtils::isIosAndroidPad()) {
                    temploadingIcon->setScale(2);
                }
                temploadingIcon->setPosition(UIComponent::getInstance()->m_miniBG->getParent()->getPosition());
                m_layers[WM_GUI]->addChild(temploadingIcon);
                CCActionInterval * rotateto1 = CCRotateTo::create(3.0, 4320);
                CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,CCRemoveSelf::create(),NULL);
                temploadingIcon->runAction(rotateAction);
//                CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
//                CCActionInterval * rotateSixTimes = CCRepeat::create(rotateAction, 6);
//                temploadingIcon->setTag(MinimapLoadingTag);
            }
        }
    }
    if(isMiniMapModel()){
        return;
    }
    
    if(isTouchInside(m_directionIcon, pTouch) && m_directionIcon->isVisible()){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        gotoTilePoint(WorldController::getInstance()->selfPoint, GlobalData::shared()->playerInfo.selfServerId);
        m_touchDelegateView->notMove = false;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("WL_myempire"));
    }else if(isTouchInside(UIComponent::getInstance()->m_coordBG, pTouch)){
        auto world = WorldMapView::instance();
        if (world) {
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            //world->addPopupView(MoveToView::create());
            PopupViewController::getInstance()->addPopupView(MoveToView::create());
        }
    }else if(isTouchInside(UIComponent::getInstance()->m_faveBG, pTouch)){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        if(GlobalData::shared()->isCrossService && GlobalData::shared()->playerInfo.currentServerId > CROSS_SERVER_START_INDEX && ActivityController::getInstance()->checkShengDianOpen()){
            PopupViewController::getInstance()->addPopupView(UseCKFSkillView::create(),false);
        }else{
            PopupViewController::getInstance()->addPopupInView(FavoriteList::create(),false);
        }
    }else if(isTouchInside(UIComponent::getInstance()->m_goHomeBtn, pTouch) && !GlobalData::shared()->playerInfo.isInSelfServer()){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        if (GlobalData::shared()->isCrossService && GlobalData::shared()->playerInfo.selfServerId >CROSS_SERVER_START_INDEX) {
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, -1, SERVERFIGHT_MAP);
            WorldMapView::instance()->m_map->onShowMapMask();
        }else{
            gotoTilePoint(WorldController::getInstance()->selfPoint, GlobalData::shared()->playerInfo.selfServerId);
            removeCover();
        }
        m_touchDelegateView->notMove = false;
    }
}

void WorldMapView::onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
}

bool WorldMapView::updateMarchTarget(MarchInfo &info, double now, float delta) {
    if (info.serverId != GlobalData::shared()->playerInfo.currentServerId) {
        return false;
    }
    auto clearMarchNode = [&](CCNode* node,MarchInfo& info) {
        if (node) {
            map<unsigned int, MarchArmy*>::iterator it = m_marchArmy.find(info.marchTag);
            if(it != m_marchArmy.end()){
                it->second->armyDelete();
                it->second->release();
                m_marchArmy.erase(it);
            }
            node->removeFromParentAndCleanup(true);
            if(m_drawRoadNode->getChildByTag(info.marchTag)){
                m_drawRoadNode->getChildByTag(info.marchTag)->removeFromParentAndCleanup(true);
            }
            if(m_touchTroopNode->getChildByTag(info.marchTag)){
                m_touchTroopNode->getChildByTag(info.marchTag)->removeFromParentAndCleanup(true);
            }
            if (m_heiqishiLvBgBachNode->getChildByTag(info.marchTag)) {
                m_heiqishiLvBgBachNode->removeChildByTag(info.marchTag);
            }
            if (m_heiqishiShadowBachNode->getChildByTag(info.marchTag)) {
                m_heiqishiShadowBachNode->removeChildByTag(info.marchTag);
            }
            if (m_spinLabelNode->getChildByTag(info.marchTag)) {
                m_spinLabelNode->removeChildByTag(info.marchTag);
            }
            if (m_heiqishiLvLbNode->getChildByTag(info.marchTag)) {
                m_heiqishiLvLbNode->removeChildByTag(info.marchTag);
            }
        }
        //删除行军粒子特效
        unsigned int batchTag=info.marchTag * 1000;
        if (m_walkparticleVec.find(batchTag)!=m_walkparticleVec.end()) {
            for (int i=0; i<m_walkparticleVec[batchTag].size(); i++) {
                m_walkparticleVec[batchTag][i]->removeFromParentAndCleanup(true);
            }
            m_walkparticleVec.erase(batchTag);
        }
    };
    
    auto drawMarchNode = [&](CCNode* node,MarchInfo& info) {
        auto endPos = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(info.endPointIndex),info.serverId);
        auto startPos = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(info.startPointIndex),info.serverId);
        auto currentPos = ccp(-1, -1);
        auto realEndPos = ccp(-1, -1);
        auto realStartPos = ccp(-1, -1);
        auto nextPos = ccp(-1, -1);
        
        if(info.stateType == StateMarch ||
           (info.stateType == StateReturn &&
            (info.targetType == CityTile
             || info.targetType == MonsterTile
             || info.targetType == Trebuchet
             || info.targetType == Tile_allianceArea
             || info.targetType == tile_superMine
             || info.targetType == tile_wareHouse
             ))){
                startPos = startPos + ccp(0, _tile_height / 2);
            }//march起点，四格位置要调整一下
        
        if(info.stateType == StateReturn ||
           (info.stateType == StateMarch &&
            (info.targetType == CityTile
             || info.targetType == MonsterTile
             || info.targetType == Trebuchet
             || info.targetType == Tile_allianceArea
             || info.targetType == tile_superMine
             || info.targetType == tile_wareHouse
             ))){
                endPos = endPos + ccp(0, _tile_height / 2);
            }//march终点，四格位置要调整一下
        if(
           info.targetType != MonsterTile
           && info.targetType != CampTile
           && info.targetType != ResourceTile
           && info.targetType != tile_wareHouse
           && info.targetType != tile_superMine
           && info.targetType != Tile_allianceArea
           && info.targetType != tile_tower
           && info.targetType != tile_banner
           && info.targetType != OriginTile
           && info.marchType != MethodScout
           && info.marchType != MethodDeal
           && info.marchType != MethodWarehouse
           && info.marchType != MethodYuanSolider
           && info.marchType != MethodUnion
           && info.stateType == StateMarch){//出征里面的除侦查，交易 要在终点位置前开始冲锋
            auto pt = WorldController::getInstance()->getCrossPointAtCity(info);
            if(pt.x != -1){
                realEndPos = pt;
            }
        }else{
            realEndPos = endPos;
        }
        
        if ((info.targetType == Throne || info.targetType == Trebuchet) && info.stateType == StateReturn) {
            realStartPos = WorldController::getInstance()->getCrossPointAtCity(info);
        }else{
            realStartPos = startPos;
        }
        //        if(info.marchType == MethodBattle){//TODO:wdz
        //            realStartPos = realStartPos - ccp(0, 70);
        //        }
        if(node){//计算当前位置
            currentPos = node->getPosition();
        }else{
            currentPos = info.getCurrentPos(realStartPos, realEndPos);
        }
        
        if(info.isChangingSpeed){
            clearMarchNode(node, info);
            node = NULL;
        }
        
        double timeLeft = info.getCurrentLineTimeLeft();
        
        auto distance = info.calculateCurrentLineLeft(realStartPos, realEndPos, currentPos);
        info.speedX = distance.x / timeLeft;
        info.speedY = distance.y / timeLeft;
        nextPos = currentPos + ccp(info.speedX * delta * 1000, info.speedY * delta * 1000);
        //行军技能减速粒子特效
        if(info.slowDown&&info.marchType==MethodScout){//
            unsigned int batchTag=info.marchTag * 1000;
            if (m_walkparticleVec.find(batchTag)!=m_walkparticleVec.end()) {
                int count = m_walkparticleVec[batchTag].size();
                for(int m=0;m<m_walkparticleVec[batchTag].size();m++){
                    m_walkparticleVec[batchTag][m]->setPosition(nextPos);
                }
            }else{
                for (int i=1; i<=2; i++) {
                    auto particle = ParticleController::createParticle(CCString::createWithFormat("ArmyMud_%d",i)->getCString());//ArmyMud_ //Loading_
                    particle->setPosition(nextPos);
                    addWalkParticleToBatch(particle, batchTag);
                }
            }
        }
        //行军技能减速粒子特效
        info.rotate = -atan2f(distance.y, distance.x)/ M_PI * 180.0f;
        if(!node){
            //部队
            if(info.marchType == MethodHeiqishi){//MethodHeiqishi //TODOwdz  MethodBattle
                node = createHeiqishiSoldier(info);
            }
            else{
                node = createMarchSprite(info);
            }
            if(node == NULL){
                return;
            }
            node->setTag(info.marchTag);
            //行军路线
            double orginalTime = info.mt;
            if(orginalTime <= 0){
                orginalTime = info.endStamp - info.startStamp;
            }
            auto changeSpeed = 80 * orginalTime / (info.endStamp - info.startStamp);
            ccColor3B color = ccWHITE;
            auto selfIndex = WorldController::getIndexByPoint(WorldController::getInstance()->selfPoint);
            if ((info.endPointIndex == selfIndex && info.stateType == StateMarch && info.marchType != MethodDeal  && info.marchType != MethodWarehouse && info.marchType != MethodYuanSolider && info.marchType != MethodUnion)
                || (info.startPointIndex == selfIndex && info.stateType == StateReturn && info.marchType != MethodDeal && info.marchType != MethodWarehouse && info.marchType != MethodYuanSolider /*&& info.marchType != MethodUnion*/)) {
                // info.marchType == MethodUnion表示队员，非队长 info.marchType == MethodYuanSolider表示援军
                color = ccRED;
            }
            
            auto &selfInfo = GlobalData::shared()->playerInfo;
            if(selfInfo.getAllianceId() != ""){
                if(info.marchToIndexLeague == selfInfo.getAllianceId() && info.marchType != MethodDeal && info.marchType != MethodWarehouse && info.marchType != MethodYuanSolider && info.marchType != MethodUnion){
                    color = ccRED;
                }
            }
            if(selfInfo.getAllianceId() != ""){
                if(info.marchToIndexLeague == selfInfo.getAllianceId() && info.marchType == MethodHeiqishi){
                    color = ccRED;
                }
            }
            
            if(info.ownerType == PlayerSelf || info.ownerType == PlayerAlliance){
                color = ccGREEN;
            }
            if (color.r == ccRED.r && color.g == ccRED.g && color.b == ccRED.b && CCUserDefault::sharedUserDefault()->getIntegerForKey(DALTONSTYLE, 0) == 1) {
                color = ccBLUE;
            }
            auto line = CCLineBatchedSprite::create(startPos, endPos, 25, changeSpeed);
            line->setColor(color);
            line->setTag(info.marchTag);
            m_drawRoadNode->addChild(line);
            
            info.isChangingSpeed = false;
            if(info.marchType == MethodScout || info.marchType == MethodDeal || info.marchType == MethodWarehouse || info.targetType == ResourceTile || info.targetType == tile_superMine || info.targetType == tile_tower || info.targetType == tile_banner || info.targetType == tile_wareHouse){
                auto spChild = dynamic_cast<CCSprite*>(node->getChildren().at(0));
                auto spSize = spChild->getContentSize();
                
                auto touch = CCCommonUtils::ButtonNode::create(spSize*MapGlobalScale);
                touch->setTag(info.marchTag);
                touch->setTouchDoEnd([&](bool isTouchInside){
                    if (isTouchInside) {
                        this->showTroopInfo(info.uuid);
                    }
                });
                m_touchTroopNode->addChild(touch);
                touch->setPriority(3);
                touch->setSwallow(true);
            }
        }
        
        if(info.marchType != MethodScout && info.marchType != MethodDeal && info.marchType != MethodWarehouse && info.marchType != MethodYuanSolider && info.marchType != MethodUnion){
            if(info.stateType == StateReturn){//
                if(ccpDistance(startPos, nextPos) > ccpDistance(startPos, realEndPos)){
                    nextPos = realEndPos;
                }
            }else{//出征走到终点后如果后台没有返回设置假数据播放攻击动画
                //投石车和王座要根据当前状态决定是不是要攻击
                bool attackFlag = ccpDistance(startPos, nextPos) > ccpDistance(startPos, realEndPos);
                if(info.targetType == Throne || info.targetType == Trebuchet){
                    auto activityIt = WorldController::getInstance()->m_worldActivity.find(FIGHT_OF_KING);
                    if(activityIt != WorldController::getInstance()->m_worldActivity.end()){
                        auto &activityInfo = activityIt->second;
                        if (activityInfo.currentState != WarTime) {
                            attackFlag = false;
                        }
                        auto it = m_cityInfo.find(info.endPointIndex);
                        if(it != m_cityInfo.end()){
                            if(info.targetType == Throne && WorldController::getInstance()->isSelfThrone(it->second)){
                                attackFlag = false;
                            }
                            if(info.targetType == Trebuchet && WorldController::getInstance()->isSelfTrebuchet(it->second)){
                                attackFlag = false;
                            }
                            
                        }else{
                            attackFlag = false;
                        }
                    }else{
                        attackFlag = false;
                    }
                }
                if(attackFlag){
                    nextPos = realEndPos;
                    
                    if(info.targetType != OriginTile
                       && info.targetType != MonsterTile
                       && info.targetType != CampTile
                       && info.targetType != ResourceTile
                       && info.targetType != tile_wareHouse
                       && info.targetType != Tile_allianceArea
                       && info.targetType != tile_superMine
                       && info.targetType != tile_tower
                       && info.targetType != tile_banner
                       && info.marchType != MethodScout
                       && info.marchType != MethodDeal
                       && info.marchType != MethodWarehouse
                       && info.marchType != MethodYuanSolider
                       && info.marchType != MethodUnion
                       && info.action != MarchActionPrepareAttack
                       ){
                        auto itTarget = WorldController::getInstance()->getCityInfos().find(info.endPointIndex);
                        bool pFlag = false;
                        if (itTarget != WorldController::getInstance()->getCityInfos().end() && itTarget->second.playerName==GlobalData::shared()->playerInfo.name) {
                            auto player = WorldController::getInstance()->m_playerInfo.find(itTarget->second.playerName);
                            if (player!=WorldController::getInstance()->m_playerInfo.end()) {
                                auto now = WorldController::getInstance()->getTime();
                                int left = (player->second.protectStamp - now)/1000;
                                if (left>=1) {
                                    pFlag = true;
                                }
                            }
                        }
                        if (pFlag) {
                            info.action = MarchActionNone;
                        }else{
                            info.action = MarchActionPrepareAttack;
                        }
                        unsigned int tmp = info.startPointIndex;
                        info.startPointIndex = info.endPointIndex;
                        info.endPointIndex = tmp;
                        info.stateType = StateReturn;
                        info.startStamp = WorldController::getInstance()->getTime();
                        double dis = ccpSub(WorldController::getPointByIndex(info.endPointIndex), WorldController::getPointByIndex(info.startPointIndex)).getLength();
                        double time = WorldController::getInstance()->getMarchTime(dis, CityTile);
                        info.endStamp = WorldController::getInstance()->getTime() + time;
                        info.mt = info.endStamp - info.startStamp;
                        if(info.marchType ==  MethodHeiqishi){
                            info.endStamp = WorldController::getInstance()->getTime() + 8 * 1000;
                        }
                    }
                }
            }
        }
        if(info.marchType == MethodScout || info.marchType == MethodDeal || info.marchType == MethodWarehouse || info.targetType == ResourceTile || info.targetType == tile_superMine || info.targetType == tile_tower || info.targetType == tile_banner || info.targetType == tile_wareHouse){
            if(m_touchTroopNode->getChildByTag(info.marchTag)){
                m_touchTroopNode->getChildByTag(info.marchTag)->setPosition(nextPos);
            }
        }
        node->setPosition(nextPos);
        onTowerAttack(node, info);
        if (info.marchType == MethodHeiqishi) {//todowdz
            if (m_heiqishiLvBgBachNode->getChildByTag(info.marchTag)) {
                CCSprite* lvBg = dynamic_cast<CCSprite*>(m_heiqishiLvBgBachNode->getChildByTag(info.marchTag));
                if (lvBg) {
                    lvBg->setPosition(nextPos+ccp(-45, -55));
                }
            }
            if (m_heiqishiShadowBachNode->getChildByTag(info.marchTag)) {
                CCSprite* shadowBg = dynamic_cast<CCSprite*>(m_heiqishiShadowBachNode->getChildByTag(info.marchTag));
                if (shadowBg) {
                    shadowBg->setPosition(nextPos+ccp(0, 0));
                }
            }
            if (m_heiqishiLvLbNode->getChildByTag(info.marchTag)) {
                m_heiqishiLvLbNode->getChildByTag(info.marchTag)->setPosition(nextPos+ccp(-45, -55));
                
            }
            
            if (m_spinLabelNode->getChildByTag(info.marchTag)) {
                CCLabelIFTTF* ttf = dynamic_cast<CCLabelIFTTF*>(m_spinLabelNode->getChildByTag(info.marchTag));
                if (ttf) {
                    ttf->setPosition(nextPos+ccp(-20, -60));
                }
            }
        }
        //        CCLOG("nextPoint   X:%f,y:%f",nextPos.x,nextPos.y);
        // set follow in view
        if (inFollowView && info.uuid == followMarchId) {
            float scaleX = m_map->getScaleX();
            float scaleY = m_map->getScaleY();
            float x = halfWinSize.width - nextPos.x*scaleX;
            float y = halfWinSize.height - nextPos.y*scaleY;
            m_map->setPosition(ccp(x, y));
        }
    };
    
    auto node = m_mapMarchNode->getChildByTag(info.marchTag);
    if(node == NULL){
        node = m_mapMarchNode1->getChildByTag(info.marchTag);
    }
    if(node == NULL){
        node = m_cityAttackNode->getChildByTag(info.marchTag);
    }
    if(node == NULL){
        node = m_mapHeiqishiMarchNode->getChildByTag(info.marchTag);
    }
    
    if(info.targetType == ResourceTile && info.action == MarchActionPrepareAttack){
        if(node){
            clearMarchNode(node, info);
        }
        node = NULL;
        finishBattleAni(CCString::create(info.uuid));
    }
    if (info.action != MarchActionNone) {
        if (info.action != MarchActionAttack && info.action != MarchActionPrepareAttack) {
            clearMarchNode(node,info);
        }else{
            if(info.action == MarchActionPrepareAttack){
                if(m_drawRoadNode->getChildByTag(info.marchTag)){
                    m_drawRoadNode->getChildByTag(info.marchTag)->removeFromParentAndCleanup(true);
                }
            }
        }
        
        switch (info.action) {
            case MarchActionPrepareAttack: {
                if(info.marchType == MethodHeiqishi ){//MethodHeiqishi //TODOwdz MethodBattle
                    auto attack = createHeiqishiSoldier(info);
                    if(!attack){
                        return false;
                    }
                    attack->setTag(info.marchTag);
                    unsigned int targetIndex = info.stateType == StateReturn ? info.startPointIndex : info.endPointIndex;
                    auto endPos = m_map->getViewPointByTilePoint(WorldController::getInstance()->getPointByIndex(targetIndex),info.serverId);
                    if(info.marchType != MethodScout && info.marchType != MethodDeal && info.marchType != MethodWarehouse && info.marchType != MethodYuanSolider && info.marchType != MethodUnion){
                        auto pt = WorldController::getInstance()->getCrossPointAtCity(info);
                        if(pt.x != -1){
                            endPos = pt;
                        }
                    }
                    attack->setPosition(endPos);
                }
                else{
                    auto attack = createMarchSprite(info);
                    attack->setScale(MapGlobalScale);
                    attack->setTag(info.marchTag);
                    unsigned int targetIndex = info.stateType == StateReturn ? info.startPointIndex : info.endPointIndex;
                    auto endPos = m_map->getViewPointByTilePoint(WorldController::getInstance()->getPointByIndex(targetIndex),info.serverId);
                    if(info.marchType != MethodScout && info.marchType != MethodDeal && info.marchType != MethodWarehouse && info.marchType != MethodYuanSolider && info.marchType != MethodUnion){
                        auto pt = WorldController::getInstance()->getCrossPointAtCity(info);
                        if(pt.x != -1){
                            endPos = pt;
                        }
                    }
                    attack->setPosition(endPos);
                    
                }
                info.action = MarchActionAttack;
            }
                break;
            case MarchActionSetOccupy: {
                switch (info.targetType) {
                    case CampTile: {
                        // add camp icon
                        auto &resInfo = m_cityInfo[info.endPointIndex];
                        delUnderNode(resInfo.cityIndex);
                        resInfo.cityType = CampTile;
                        resInfo.cityIcon = 7;
                        // for troop retreat
                        resInfo.citySubType = info.uuid;
                        createCity(resInfo);
                    }
                        break;
                    case ResourceTile: {
                        auto city = m_cityInfo.find(info.endPointIndex);
                        if(city == m_cityInfo.end() || city->second.cityType != ResourceTile){
                            return false;
                        }
                        auto &resInfo = m_cityInfo[info.endPointIndex];
                        delUnderNode(resInfo.cityIndex);
                        
                        // add occupy pointer
                        string tmp = "world_occupy_self.png";
                        switch (info.ownerType) {
                            case PlayerSelf:
                                break;
                            case PlayerAlliance:
                                tmp = "world_occupy_alliance.png";
                                break;
                            case PlayerOther:
                                tmp = "world_occupy_other.png";
                                break;
                                
                            default:
                                break;
                        }
                        auto occupyPointer = CCLoadSprite::createSprite(tmp.c_str());
                        occupyPointer->setAnchorPoint(ccp(0.5, 0));
                        occupyPointer->setPosition(m_map->getViewPointByTilePoint(WorldController::getPointByIndex(info.endPointIndex),info.serverId));
                        occupyPointer->setTag(info.marchTag);
                        occupyPointer->setScale(MapGlobalScale);
                        m_occupyPointerNode->addChild(occupyPointer);
                        
                        // for troop retreat
                        if (info.ownerType == PlayerSelf) {
                            WorldController::getInstance()->m_selfMarchUuid[info.endPointIndex] = info.uuid;
                        }
                        createCity(resInfo);
                    }
                        break;
                    case tile_superMine: {
                        auto city = m_cityInfo.find(info.endPointIndex);
                        if(city == m_cityInfo.end() || city->second.cityType != tile_superMine || info.ownerType != PlayerSelf || city->second.m_superMineInfo.trstat != Astate_Destroy){
                            return false;
                        }
                        auto &resInfo = m_cityInfo[info.endPointIndex];
                        delUnderNode(resInfo.cityIndex);
                        
                        // add occupy pointer
                        if (!m_occupyPointerNode->getChildByTag(info.marchTag)) {
                            string tmp = "world_occupy_self.png";
                            auto occupyPointer = CCLoadSprite::createSprite(tmp.c_str());
                            occupyPointer->setAnchorPoint(ccp(0.5, 0));
                            occupyPointer->setPosition(m_map->getViewPointByTilePoint(WorldController::getPointByIndex(info.endPointIndex)) + ccp(0,_halfTileSize.height));
                            occupyPointer->setTag(info.marchTag);
                            //                            CCLOG("testlog---------addtag0---------%d",info.marchTag);
                            occupyPointer->setScale(MapGlobalScale);
                            m_occupyPointerNode->addChild(occupyPointer);
                        }
                        // for troop retreat
                        if (info.ownerType == PlayerSelf) {
                            WorldController::getInstance()->m_selfMarchUuid[info.endPointIndex] = info.uuid;
                        }
                        createCity(resInfo);
                    }
                        break;
                    default:
                        break;
                }
            }
                break;
            case MarchActionUnsetOccupy: {
                switch (info.targetType) {
                    case CampTile: {
                        // remove camp icon
                        auto &resInfo = m_cityInfo[info.startPointIndex];
                        delUnderNode(resInfo.cityIndex);
                        resInfo.cityIcon = -1;
                        resInfo.cityType = OriginTile;
                        resInfo.playerName = "";
                        resInfo.citySubType = "";
                        m_tileLayer->revertTileGID(resInfo.cityTilePoint);
                    }
                        break;
                    case ResourceTile: {
                        auto &resInfo = m_cityInfo[info.startPointIndex];
                        auto pointer = m_occupyPointerNode->getChildByTag(info.marchTag);
                        if (pointer) {
                            pointer->removeFromParentAndCleanup(true);
                        }
                        resInfo.playerName = "";
                        createCity(resInfo);
                    }
                        break;
                    case tile_superMine: {
                        auto &resInfo = m_cityInfo[info.startPointIndex];
                        //                        CCLOG("testlog---------deletag---------%d",info.marchTag);
                        auto pointer = m_occupyPointerNode->getChildByTag(info.marchTag);
                        if (pointer) {
                            pointer->removeFromParentAndCleanup(true);
                        }
                        resInfo.playerName = "";
                        createCity(resInfo);
                    }
                        break;
                    case MonsterTile: {
                        auto &resInfo = m_cityInfo[info.startPointIndex];
                        auto pointer = m_occupyPointerNode->getChildByTag(info.marchTag);
                        if (pointer) {
                            pointer->removeFromParentAndCleanup(true);
                        }
                        resInfo.playerName = "";
                    }
                        break;
                    default:
                        break;
                }
            }
                break;
            case MarchActionUnsetResource: {
                switch (info.targetType) {
                    case ResourceTile: {
                        auto &resInfo = m_cityInfo[info.startPointIndex];
                        auto pointer = m_occupyPointerNode->getChildByTag(info.marchTag);
                        if (pointer) {
                            pointer->removeFromParentAndCleanup(true);
                        }
                        resInfo.playerName = "";
                        resInfo.cityIcon = -1;
                        resInfo.cityType = OriginTile;
                        int index = WorldController::getIndexByPoint(resInfo.cityTilePoint);
                        delUnderNode(index);
                    }
                        break;
                    default:
                        break;
                }
            }
                break;
            default:
                break;
        }
        
        if (info.action != MarchActionAttack) {
            info.action = MarchActionNone;
        }
        
        return false;
    }
//    if(info.targetType == CampTile && info.stateType != StateOccupy){
//        auto pointer = m_occupyPointerNode->getChildByTag(info.marchTag);
//        if (pointer) {
//            pointer->removeFromParentAndCleanup(true);
//        }
//    }
    if (info.stateType == StateOccupy) {
        if (info.targetType == ResourceTile || (info.targetType == MonsterTile && info.ownerType == PlayerSelf) || info.targetType == tile_superMine) {
            // add occupy pointer
            auto pointer = m_occupyPointerNode->getChildByTag(info.marchTag);
            if (!pointer) {
                auto city = m_cityInfo.find(info.endPointIndex);
                if(city == m_cityInfo.end() || (city->second.cityType != ResourceTile && city->second.cityType != tile_superMine)){
                    return false;
                }
                bool isSuperMine = city->second.cityType == tile_superMine?true:false;
                string tmp = "world_occupy_self.png";
                switch (info.ownerType) {
                    case PlayerSelf:
                        break;
                    case PlayerAlliance:
                        tmp = "world_occupy_alliance.png";
                        break;
                    case PlayerOther:
                        tmp = "world_occupy_other.png";
                        break;
                        
                    default:
                        break;
                }
                if (isSuperMine) {
                    if(info.ownerType == PlayerSelf && city->second.m_superMineInfo.trstat == Astate_Destroy){
                        tmp = "world_occupy_self.png";
                    }
                    else{
                        return false;
                    }
                    
                }
                auto occupyPointer = CCLoadSprite::createSprite(tmp.c_str());
                occupyPointer->setAnchorPoint(ccp(0.5, 0));
                auto pt = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(info.endPointIndex),info.serverId);
                if(info.targetType == MonsterTile){
                    pt = pt + ccp(0, _tile_height);
                }
                if(isSuperMine){
                    pt = pt + ccp(0, _tile_height / 2);
                }
                occupyPointer->setPosition(pt);
                occupyPointer->setTag(info.marchTag);
                occupyPointer->setScale(MapGlobalScale);
                m_occupyPointerNode->addChild(occupyPointer);
            }
        }/*else if(info.targetType == CampTile && info.treasureMapFinishTime>0 && info.treasureMapTime>0 && WorldController::getInstance()->getTime() < info.treasureMapFinishTime){
            auto pointer = m_occupyPointerNode->getChildByTag(info.marchTag);
            if (!pointer) {
                auto occupyPointer = CCLoadSprite::createSprite("treasure_icon.png");
                occupyPointer->setAnchorPoint(ccp(0.5, 0));
                occupyPointer->setPosition(m_map->getViewPointByTilePoint(WorldController::getPointByIndex(info.endPointIndex)) + ccp(0,20));
                occupyPointer->setTag(info.marchTag);
                occupyPointer->setScale(MapGlobalScale);
                m_occupyPointerNode->addChild(occupyPointer);
            }
        }*/
        return false;
    }
    
    if (info.endStamp > now + delta){
        bool addFlag = false;
        if(node == NULL){
            addFlag = true;
        }
        drawMarchNode(node,info);
        return addFlag;
    } else {
        // 这个地方添加部队返回到家提示
        clearMarchNode(node,info);
    }
    return false;
}

void WorldMapView::showTroopInfo(const string &marchId) {
    CCLOG("march info uid:%s", marchId.c_str());
    if(marchId.empty()){
        return;
    }
    auto &marchInfos = WorldController::getInstance()->m_marchInfo;
    auto it = marchInfos.find(marchId);
    if (it == marchInfos.end() || marchId == followMarchId) {
        return;
    }
    CCLOG("march info detail:%s%s", it->second.uuid.c_str(), CC_ITOA(int(it->second.marchTag)));
    followMarchId = marchId;
    inFollowView = true;
    addPopupView4(TroopInfoPanel::create(marchId));
}

void WorldMapView::update(float delta) {
    if (!WorldController::getInstance()->initTime || !WorldController::getInstance()->initWorld) {
        return;
    }
    double now = WorldController::getInstance()->getTime();
/*shake test code*/
//    int  shakeTime = CCUserDefault::sharedUserDefault()->getIntegerForKey(MSG_SHAKETIME ,-1);
//    int currentTime = WorldController::getInstance()->getTime() / 1000;
//    
//    if (currentTime - shakeTime >= 10) {
//        onShowShakeGuide(0);
//    }
/*shake test code*/
    if(needClearAll && m_inCityUpdate){
        needClearAll = false;
        
        for(auto vec : m_particleVec){
            for (auto &particle : vec.second) {
                particle->removeFromParent();
            }
        }
        
        for(auto vec : m_flagParticleVec){
            for (auto &particle : vec.second) {
                particle->removeFromParent();
            }
            
        }
        
        WorldMapView::instance()->m_flagParticleNode->removeAllChildren();
        WorldMapView::instance()->m_flagBatch->removeAllChildren();
        WorldMapView::instance()->m_occupyPointerNode->removeAllChildren();
        WorldMapView::instance()->m_cityBatchNode->removeAllChildren();
        WorldMapView::instance()->m_aAreaAniNode->removeAllChildren();
        WorldMapView::instance()->m_bannerNode->removeAllChildren();
        WorldMapView::instance()->m_batchNode->removeAllChildren();
        WorldMapView::instance()->m_labelNode->removeAllChildren();
        WorldMapView::instance()->m_labelNode1->removeAllChildren();
        WorldMapView::instance()->m_unBatchLabelNode->removeAllChildren();
        WorldMapView::instance()->m_simNameUnbatchNode->removeAllChildren();
        WorldMapView::instance()->m_aAreaUnbachLbNode->removeAllChildren();
        WorldMapView::instance()->m_mapMonsterNode->removeAllChildren();
        WorldMapView::instance()->m_mapMonstersNode->removeAllChildren();
        WorldMapView::instance()->m_mapMonsterShadowNode->removeAllChildren();
        WorldMapView::instance()->m_lianNode->removeAllChildren();
        WorldMapView::instance()->m_csNode->removeAllChildren();
        WorldMapView::instance()->m_parBatchVec.clear();
        WorldMapView::instance()->m_particleVec.clear();
        WorldMapView::instance()->m_parFlagBatchVec.clear();
        WorldMapView::instance()->m_flagParticleVec.clear();
        WorldMapView::instance()->m_cityItem.clear();
    }
    
//    if (isDownloadingMinimap) {
//        if (DynamicResourceController::getInstance()->checkMinimapViewResource()) {
//            m_layers[WM_GUI]->getChildByTag(MinimapLoadingTag)->stopAllActions();
//            m_layers[WM_GUI]->getChildByTag(MinimapLoadingTag)->setVisible(false);
//            m_layers[WM_GUI]->removeChildByTag(MinimapLoadingTag);
//            isDownloadingMinimap = false;
//        }
//    }
    // update city
    if (m_inCityUpdate) {
        static unsigned int cache = 10;
        
        unsigned int addedCache = 0;
        pthread_mutex_lock(&m_addCityLock);
        while (m_cacheToAddCity.size()) {
            if (addedCache > cache) {
                break;
            }
            addedCache++;
            createCity(m_cityInfo[m_cacheToAddCity.back()]);
            m_cacheToAddCity.pop_back();
        }
        
        if (!m_cacheToAddCity.size()) {
            WorldController::getInstance()->setisAsyEnd(true);
            onSendHelpCmd();
            m_inCityUpdate = false;
            m_loadingIcon->setVisible(false);
        }
        showAndHideUnBatchLabel();
        showAndHideFieldMonster();
        onShowAndHideSpinLb();
        pthread_mutex_unlock(&m_addCityLock);
    }
    
    // check city status
    for (auto &player : WorldController::getInstance()->m_playerInfo) {
        if (player.second.protectStamp > 1) {
            if (player.second.protectStamp < now) {
                delBatchItem(ProtectTag, player.second.cityIndex);
                player.second.protectStamp = 0;
                if(player.second.resourceProtectStamp > now){
                    addBatchItem(ResourceProtectTag, player.second.cityIndex);
                }
            }
        }
        if (player.second.resourceProtectStamp > 1) {
            if (player.second.resourceProtectStamp < now) {
                delBatchItem(ResourceProtectTag, player.second.cityIndex);
                player.second.resourceProtectStamp = 0;
            }
        }
        if (player.second.fireStamp > 1) {
            if (player.second.fireStamp < now) {
                delBatchItem(FireTag, player.second.cityIndex);
                player.second.fireStamp = 0;
            }
        }
    }
    //check
    auto cellMap = getCurrentTile(m_map->currentTilePoint, 26, 11, WorldController::getInstance()->_current_tile_count_x);
    for (auto &cell : cellMap){
        int cellIndex = cell.first;
        auto city = m_cityInfo.find(cellIndex);
        if(city != m_cityInfo.end()){
            auto &info = m_cityInfo[cellIndex];
            if(info.cityType == CampTile){
                auto iter = WorldController::getInstance()->m_marchInfo.begin();
                while (iter!=WorldController::getInstance()->m_marchInfo.end()) {
                    if((*iter).second.targetType == CampTile && (*iter).second.endPointIndex == info.cityIndex){
                        double now = WorldController::getInstance()->getTime();
                        if(((*iter).second.treasureMapFinishTime>0) && (*iter).second.treasureMapFinishTime > now){
                            int timeLeft = ((*iter).second.treasureMapFinishTime - now) / 1000;
                            unsigned int idx = info.cityIndex;
                            if(m_cityItem.find(idx)!=m_cityItem.end()){
                                if(m_cityItem[idx].size()>0){
                                    auto iter0 = (m_cityItem[idx].end()-1);
                                    CCNode *node = (*iter0);
                                    if(node && dynamic_cast<CCLabelBatch*>(node)){
                                        CCLabelBatch* lbl = dynamic_cast<CCLabelBatch*>(node);
                                        lbl->setString(CC_SECTOA(timeLeft));
                                    }
                                }
                            }
                        }
                        break;
                    }
                     ++iter;
                }
                
            }else if(info.cityType == FieldMonster || info.cityType == ActBossTile){
                if(info.refreshStamp < now){
                    releaseCity(info);
                    delBatchItem(MonsterBreath, info.cityIndex);
                    delBatchItem(MonsterBreath1, info.cityIndex);
                    info.cityIcon = -1;
                    info.cityType = OriginTile;
                }else{
                    if(info.fieldMonsterInfo.isHpChange){
                        createCity(info);
                    }
                    info.fieldMonsterInfo.isHpChange = false;
                }
            }else if(info.cityType == Throne || info.cityType == Trebuchet){
                auto activityIt = WorldController::getInstance()->m_worldActivity.find(FIGHT_OF_KING);
                if(activityIt != WorldController::getInstance()->m_worldActivity.end() && WorldController::isInSelfServer(info.tileServerId)){
                    auto &activityInfo = activityIt->second;
                    if(m_currentState != activityInfo.currentState){
                        createCity(info);
                    }
                    if(info.cityIndex == info.parentCityIndex){
                        auto label = dynamic_cast<CCLabelIFTTF*>(m_unBatchLabelNode->getChildByTag(info.cityIndex));
                        
                        if(label){
                            WorldActivityState state = NotOpen;
                            std::string dialogId = "110087";
                            std::string allianceName = info.allianceName;
                            std::string playerName = info.playerName;
                            double time = 0;
                            double openTime = activityInfo.timeInfo.openTime;
                            double startTime = activityInfo.timeInfo.startTime;
                            double endTime = activityInfo.timeInfo.endTime;
                            
                            state = WorldController::getInstance()->getKingActivityState(openTime, startTime, endTime);
                            if(state == OpenNoKing){
                                time = startTime - now;
                            }else if(state == PeaceTime){
                                time = startTime - now;
                            }else if(state == WarTime){
                                time = endTime - now;
                            }
                            time = time / 1000;
                            if(time < 0){
                                time = 0;
                            }
                            if(state == NotOpen){
                                label->setString("");
                            }else{
                                std::string timeStr = CCCommonUtils::timeLeftToCountDown(time);
                                if(info.cityType == Trebuchet && state == WarTime){
                                    timeStr = "";
                                }
                                if (state == OpenNoKing) {
                                    if (time > 5 * 24 * 3600) {
                                        timeStr = "";
                                    }
                                }
                                label->setString(timeStr.c_str());
                            }
                        }
                    }
                }
            }
        }
    }
    
    if( m_isIdle ) {
        for( auto& item : m_flagParDatas ) {
            addFlagPar(item.type, item.pt, item.index);
        }
        m_flagParDatas.clear();
    }
    
    auto checkVisible = [](const Size& size, CCNode* node){
        Mat4 transform = node->getNodeToWorldTransform();
        auto director = Director::getInstance();
        Rect visiableRect(director->getVisibleOrigin(), director->getVisibleSize());
        
        // transform center point to screen space
        float hSizeX = size.width/2;
        float hSizeY = size.height/2;
        Vec3 v3p(hSizeX, hSizeY, 0);
        transform.transformPoint(&v3p);
        Vec2 v2p = Vec2(v3p.x, v3p.y);
        
        // convert content size to world coordinates
        float wshw = std::max(fabsf(hSizeX * transform.m[0] + hSizeY * transform.m[4]), fabsf(hSizeX * transform.m[0] - hSizeY * transform.m[4]));
        float wshh = std::max(fabsf(hSizeX * transform.m[1] + hSizeY * transform.m[5]), fabsf(hSizeX * transform.m[1] - hSizeY * transform.m[5]));
        
        // enlarge visable rect half size in screen coord
        visiableRect.origin.x -= wshw;
        visiableRect.origin.y -= wshh;
        visiableRect.size.width += wshw * 2;
        visiableRect.size.height += wshh * 2;
        bool ret = visiableRect.containsPoint(v2p);
        return ret;
    };
    
    for( auto& child : this->m_flagParticleNode->getChildren())
    {
        Size size(100.0f, 100.0f);
//        Mat4 transform = child->getNodeToWorldTransform();
//        auto director = Director::getInstance();
//        Rect visiableRect(director->getVisibleOrigin(), director->getVisibleSize());
//        
//        // transform center point to screen space
//        float hSizeX = size.width/2;
//        float hSizeY = size.height/2;
//        Vec3 v3p(hSizeX, hSizeY, 0);
//        transform.transformPoint(&v3p);
//        Vec2 v2p = Vec2(v3p.x, v3p.y);
//        
//        // convert content size to world coordinates
//        float wshw = std::max(fabsf(hSizeX * transform.m[0] + hSizeY * transform.m[4]), fabsf(hSizeX * transform.m[0] - hSizeY * transform.m[4]));
//        float wshh = std::max(fabsf(hSizeX * transform.m[1] + hSizeY * transform.m[5]), fabsf(hSizeX * transform.m[1] - hSizeY * transform.m[5]));
//        
//        // enlarge visable rect half size in screen coord
//        visiableRect.origin.x -= wshw;
//        visiableRect.origin.y -= wshh;
//        visiableRect.size.width += wshw * 2;
//        visiableRect.size.height += wshh * 2;
//        bool ret = visiableRect.containsPoint(v2p);

        child->setVisible(checkVisible(size, child));
           
    }
    
    for(auto& child: m_flagBatch->getChildren()) {
        child->setVisible(checkVisible(child->getContentSize(), child));
    }
    
    for(auto& child: m_cityBatchNode->getChildren()) {
        child->setVisible(checkVisible(child->getContentSize(), child));
    }
    
    m_currentState = WorldController::getInstance()->getKingActivityStateByType(FIGHT_OF_KING);
    
    update_water_shader(m_map->getViewPointByTilePoint(m_map->currentTilePoint));
}
IFHeiqishiNode* WorldMapView::createHeiqishiSoldier(MarchInfo& info){//default info for （info.marchType MethodHeiqishi）、(info.targetType CityTile)
    float fNodeScale = 0.8;
    string spineName = CCCommonUtils::getPropById(info.ouid, "monster");
    if (spineName == "") {
        spineName = "heiqishi_1";
    }
    bool needFlipx = false;
    int direction = WorldController::getInstance()->getArmyDirectionByInfo(info);
    if (direction == 3 || direction == 4 || direction == 5) {
        needFlipx = true;
    }
    direction = IFHeiqishiNode::getExchangeDir(direction);
    bool flag = false;
    IFHeiqishiNode* spinNode = NULL;
    if(m_mapHeiqishiMarchNode->getChildByTag(info.marchTag)){
        spinNode = dynamic_cast<IFHeiqishiNode*>(m_mapHeiqishiMarchNode->getChildByTag(info.marchTag));
    }
    if(!spinNode){
        flag = true;
        spinNode = IFHeiqishiNode::create(info);
        if (spineName == "heiqishi_3") {
            fNodeScale  = 1.1;
        }
        spinNode->setScale(fNodeScale);
        if (!spinNode->isSkeExit(spineName)) {
            return NULL;
        }
    }
    if(flag){
        CCPoint tempPoint = spinNode->getPosition();
        spinNode->setTag(info.marchTag);
        m_mapHeiqishiMarchNode->addChild(spinNode);
    }
    
    if (info.action == MarchActionPrepareAttack)
    {
        SoundController::sharedSound()->playEffects(Music_Sfx_world_attack);
        
        CCArray* tempArr = CCArray::create();
        CC_SAFE_RETAIN(tempArr);
        tempArr->addObject(CCString::create(info.uuid));
        int tag = info.marchTag;
        string oldSpinTag =  CC_ITOA(tag);
        tempArr->addObject(CCString::create(oldSpinTag));
        
        int firIndex = MarchArmy::getStartIndex(info);
        string firIndexStr =  CC_ITOA(firIndex);
        tempArr->addObject(CCString::create(firIndexStr));
        CCCallFuncO* func = CCCallFuncO::create(this, callfuncO_selector(WorldMapView::onFinishSpinBattle), tempArr);
        if(spinNode->getState() != MonsterAttack){
            addBatchItem(WarFire, firIndex);
            spinNode->onStartCityAttack();
            spinNode->setSpineState(spineName, MonsterAttack, spinNode->getDirection(),true,func,5);
        }
    }
    else
    {
        spinNode->setSpineState(spineName, MonsterBreath, direction);
        if (needFlipx) {
            spinNode->setScaleX(-fNodeScale);
        }
        else
        {
            spinNode->setScaleX(fNodeScale);
        }
    }
    /*lb*/
    CCSprite* lvBg = NULL;
    if (m_heiqishiLvBgBachNode->getChildByTag(info.marchTag)) {
        lvBg = dynamic_cast<CCSprite*>(m_heiqishiLvBgBachNode->getChildByTag(info.marchTag));
    }
    else{
        lvBg =  CCLoadSprite::createSprite("world_lv_bg.png");
        lvBg->setTag(info.marchTag);
        m_heiqishiLvBgBachNode->addChild(lvBg);
    }
    
    CCSprite* shadowBg = NULL;
    if (m_heiqishiShadowBachNode->getChildByTag(info.marchTag)) {
        shadowBg = dynamic_cast<CCSprite*>(m_heiqishiShadowBachNode->getChildByTag(info.marchTag));
    }
    else{
        shadowBg =  CCLoadSprite::createSprite("shadow.png");
        shadowBg->setTag(info.marchTag);
        m_heiqishiShadowBachNode->addChild(shadowBg);
    }
    
    CCLabelBatch* bachLvLb = NULL;
    if (m_heiqishiLvLbNode->getChildByTag(info.marchTag)) {
        bachLvLb = dynamic_cast<CCLabelBatch*>(m_heiqishiLvLbNode->getChildByTag(info.marchTag));
    }
    else{
        bachLvLb = CCLabelBatch::create(CC_ITOA(info.monsterLevel) ,m_heiqishiLvLbNode);
        bachLvLb->setScale(MapGlobalScale);
        bachLvLb->setTag(info.marchTag);
        bachLvLb->setColor(ccc3(255, 235, 180));
    }
    
    CCLabelIFTTF* nameLb = NULL;
    std::string nameStr = CCCommonUtils::getNameById(info.ouid);
    if (m_spinLabelNode->getChildByTag(info.marchTag)) {
        nameLb = dynamic_cast<CCLabelIFTTF*>(m_spinLabelNode->getChildByTag(info.marchTag));
        nameLb->setString(nameStr.c_str());
    }
    else{
        nameLb = CCLabelIFTTF::create();
        nameLb->setTag(info.marchTag);
        nameLb->setColor(ccc3(225, 174, 101));
        nameLb->setFontSize(17);
        nameLb->setAnchorPoint(ccp(0, 0.5));
        nameLb->setString(nameStr.c_str());
        m_spinLabelNode->addChild(nameLb);
    }
    /*lb*/
    return spinNode;
}

CCSprite* WorldMapView::createMarchSprite(MarchInfo& info) {
    float fix = info.rotate < 0 ? -180.0 - info.rotate : 180.0 - info.rotate;
    auto fabFix = fabsf(fix);
    int direction = (fix > 0 ? 1 : -1) * ((fabFix + 22.5) / 45.0);
    direction *= 45;
    if (direction == -180) {
        direction = 180;
    }
    
    bool isNeedFlipX = false;
    switch (direction) {
        case 180:{
            direction = 0;
            isNeedFlipX = true;
        }
            break;
        case -135:{
            direction = -45;
            isNeedFlipX = true;
        }
            break;
        case 135:{
            direction = 45;
            isNeedFlipX = true;
        }
            break;
            
        default:
            break;
    }
    
    const char* tmpName = "JY_body_-45_0.png";
    
    auto sp = CCLoadSprite::createSprite(tmpName);
    sp->setTextureRect(CCRectZero);
    bool flag = false;
    auto sp1 = CCLoadSprite::createSprite("a010_0_N_move_0.png");
    sp1->setTextureRect(CCRectZero);
    switch (info.marchType) {
        case MethodRally:
        case MethodUnion:
        case MethodFieldMonster:
        case MethodYuanSolider:
        case MethodBattle:{
            if (info.targetType == ResourceTile || info.targetType == tile_superMine) {
                tmpName = CCString::createWithFormat("CJ_shadow_%s_0.png",CC_ITOA(direction))->getCString();
                auto body = CCLoadSprite::createSprite(tmpName);
                auto cloth = CCLoadSprite::createSprite(tmpName);
                auto shadow = CCLoadSprite::createSprite(tmpName);
                auto bodyAni = createMarchAni(AniCollect,direction);
                auto clothAni = createMarchAni(AniCollectCloth,direction);
                body->runAction(bodyAni);
                cloth->runAction(clothAni);
                sp->addChild(body);
                sp->addChild(cloth);
                sp->addChild(shadow,-1);
                cloth->setColor(info.getMarchColor());
            }else{
                bool inMapFlag = false;
                MarchArmy *march = NULL;
                if(m_mapMarchNode1->getChildByTag(info.marchTag)){
                    inMapFlag = true;
                    sp1 = dynamic_cast<CCSprite*>(m_mapMarchNode1->getChildByTag(info.marchTag));
                    march = m_marchArmy[info.marchTag];
                }
                if(m_cityAttackNode->getChildByTag(info.marchTag)){
                    inMapFlag = true;
                    sp1 = dynamic_cast<CCSprite*>(m_cityAttackNode->getChildByTag(info.marchTag));
                    march = m_marchArmy[info.marchTag];
                }
                
                if(!march){
                    flag = true;
                    march = MarchArmy::create(info, sp1);
                    m_marchArmy[info.marchTag] = march;
                    march->retain();
                }
                
                if (info.action == MarchActionPrepareAttack) {
                    march->attck();
                    SoundController::sharedSound()->playEffects(Music_Sfx_world_attack);
                    march->beAttacked();
                    sp1->runAction(CCSequence::create(CCDelayTime::create(10.0f),CCCallFuncO::create(this, callfuncO_selector(WorldMapView::finishBattleAni), CCString::create(info.uuid)),         CCRemoveSelf::create(),NULL));
                } else {
                    march->walk();
                }
                if(inMapFlag){
                    if (info.action == MarchActionPrepareAttack && info.targetType == Throne){
                        sp1->retain();
                        sp1->removeFromParentAndCleanup(false);
                        m_cityAttackNode->addChild(sp1);
                        sp1->release();
                    }
                    return sp1;
                }
            }
        }
            break;
        case MethodDeal:{
            tmpName = CCString::createWithFormat("ZC_shadow_%s_0.png",CC_ITOA(direction))->getCString();
            auto body = CCLoadSprite::createSprite(tmpName);
            auto cloth = CCLoadSprite::createSprite(tmpName);
            auto shadow = CCLoadSprite::createSprite(tmpName);
            auto bodyAni = createMarchAni(AniDeal,direction);
            auto clothAni = createMarchAni(AniDealCloth,direction);
            body->runAction(bodyAni);
            cloth->runAction(clothAni);
            cloth->setColor(info.getMarchColor());
            sp->addChild(body);
            sp->addChild(cloth);
            sp->addChild(shadow,-1);
        }
            break;
        case MethodWarehouse:{
            tmpName = CCString::createWithFormat("ZC_shadow_%s_0.png",CC_ITOA(direction))->getCString();
            auto body = CCLoadSprite::createSprite(tmpName);
            auto cloth = CCLoadSprite::createSprite(tmpName);
            auto shadow = CCLoadSprite::createSprite(tmpName);
            auto bodyAni = createMarchAni(AniDeal,direction);
            auto clothAni = createMarchAni(AniDealCloth,direction);
            body->runAction(bodyAni);
            cloth->runAction(clothAni);
            cloth->setColor(info.getMarchColor());
            sp->addChild(body);
            sp->addChild(cloth);
            sp->addChild(shadow,-1);
        }
            break;
        case MethodScout:{
            tmpName = CCString::createWithFormat("ZC_shadow_%s_0.png",CC_ITOA(direction))->getCString();
            auto body = CCLoadSprite::createSprite(tmpName);
            auto cloth = CCLoadSprite::createSprite(tmpName);
            auto shadow = CCLoadSprite::createSprite(tmpName);
            auto bodyAni = createMarchAni(AniScout,direction);
            auto clothAni = createMarchAni(AniScoutCloth,direction);
            body->runAction(bodyAni);
            cloth->runAction(clothAni);
            sp->addChild(body);
            sp->addChild(cloth);
            sp->addChild(shadow,-1);
            cloth->setColor(info.getMarchColor());
        }
            break;
            
        default:
            return nullptr;
    }
    CCObject* child = nullptr;
    auto& arr = sp->getChildren();
    for(auto child : arr)
    {
        auto childSP = dynamic_cast<CCSprite*>(child);
        if (childSP) {
            childSP->setFlipX(isNeedFlipX);
        }
    }
    if(flag){
        if (info.action == MarchActionPrepareAttack && info.targetType == Throne){
            m_cityAttackNode->addChild(sp1);
        }else{
            m_mapMarchNode1->addChild(sp1);
        }
        return sp1;
    }else{
        m_mapMarchNode->addChild(sp);
    }
    return sp;
}

CCAnimate* WorldMapView::createMarchAni(MarchAniType type,int direction, float deltaTime, std::string para) {
    Vector<SpriteFrame*> tmpAniArr;
    auto directionStr = CC_ITOA(direction);
    string temp = "";
    int frameCount = 0;
    int frameStart = 0;
    int randIndex = 0;
    switch (type) {
        case AniAllianceArea:{
            directionStr = "";
            temp = "AllianceTerritory_%s%d.png";
            frameCount = 12;
            frameStart = 1;
        }
            break;
        case AniTrebuchetWait:{
            directionStr = "";
            temp = "dj_%s%d.png";
            frameCount = 8;
        }
            break;
        case AniTrebuchetAttack:{
            temp = "gj_%s_%d.png";
            frameCount = 8;
        }
            break;
            
        case AniCollect:{
            temp = "CJ_body_%s_%d.png";
            frameCount = 8;
        }
            break;
        case AniCollectCloth:{
            temp = "CJ_yifu_%s_%d.png";
            frameCount = 0;
        }
            break;
        case AniDeal:{
            frameCount = 8;
            temp = "JY_body_%s_%d.png";
        }
            break;
        case AniDealCloth:{
            frameCount = 0;
            temp = "JY_yifu_%s_%d.png";
        }
            break;
        case AniScout:{
            frameCount = 8;
            temp = "ZC_body_%s_%d.png";
        }
            break;
        case AniScoutCloth:{
            frameCount = 0;
            temp = "ZC_yifu_%s_%d.png";
        }
            break;
        case AniCityProtect:{
            frameCount = 8;
            frameStart = 1;
            directionStr = "";
            temp = "Protect_B_%s%d.png";
        }
            break;
        case AniCityResourceProtect:{
            frameCount = 8;
            frameStart = 1;
            directionStr = "";
            temp = "Protect_Y_%s%d.png";
        }
            break;
        case AniEdgy:{
            frameCount = 7;
            directionStr = "";
            temp = "edge_light_%s%d.png";
        }
            break;
        case AniMonsterAttack:{
            if(para == "rock"){
                frameCount = 10;
            }else if(para == "rm"){
                frameCount = 9;
            }else if(para == "Kulou" || para == "Shixianggui" || para == "Yeren"){
                frameCount = 8;
            }else{
                frameCount = 9;
            }
            directionStr = CC_ITOA(direction);
            temp = para + "_gj_%s_%d.png";
        }
            break;
        case AniMonsterBreath:{
            if(para == "rock"){
                frameCount = 16;
            }else if(para == "Lion"){
                frameCount = 9;
                deltaTime = 0.8;
            }else{
                frameCount = 8;
                deltaTime = 0.8;
            }
            randIndex = rand() % frameCount;
            directionStr = "";
            temp = para + "_dj_0_%s%d.png";
        }
            break;
        case AniMonsterDead:{
            if(para == "Griffin"){
                frameCount = 10;
            }else if(para == "Kulou" || para == "Shixianggui" || para == "Yeren"){
                frameCount = 8;
            }else{
                frameCount = 9;
            }
            directionStr = "";
            temp = para + "_sw_0_%s%d.png";
        }
            break;
        default:
            return NULL;
    }
    
    if (temp.empty() || frameCount == 0) {
        return nullptr;
    }
    for (int i=frameStart; i<frameCount; ++i) {
        if((type == AniMonsterDead&& i == frameCount - 1) || (AniTrebuchetWait == type && i == frameStart)){
            int index = 0;
            int count = 10;
            while (index < count) {
                tmpAniArr.pushBack(CCLoadSprite::loadResource(CCString::createWithFormat(temp.c_str(),directionStr,i)->getCString()));
                index++;
            }
        }
        if(type == AniMonsterBreath){
            tmpAniArr.pushBack(CCLoadSprite::loadResource(CCString::createWithFormat(temp.c_str(),directionStr,(i + randIndex) % frameCount)->getCString()));
        }else{
            tmpAniArr.pushBack(CCLoadSprite::loadResource(CCString::createWithFormat(temp.c_str(),directionStr,i)->getCString()));
        }
    }
    
    auto ani = CCAnimation::createWithSpriteFrames(tmpAniArr, deltaTime / tmpAniArr.size());
    
    switch (type) {
        case AniMonsterAttack:
        {
            ani->setLoops(6);
        }
            break;
        case AniMonsterDead:
        case AniTrebuchetAttack:
            break;
            
        default:
            ani->setLoops(std::numeric_limits<int>::max());
    }
    
    return CCAnimate::create(ani);
}

void WorldMapView::createCity(WorldCityInfo &info) {
    if (true) {
        CCNode * tempNode = m_conflictBatchNode->getChildByTag(info.cityIndex);
        if(tempNode){
            tempNode->removeAllChildrenWithCleanup(true);
            m_conflictBatchNode->removeChildByTag(info.cityIndex);
        }
        CCNode* tempLbNode = m_aAreaUnbachLbNode->getChildByTag(info.cityIndex);
        if (tempLbNode) {
            m_aAreaUnbachLbNode->removeChildByTag(info.cityIndex);
        }
        string tempDirectionStr = info.m_aArea_direction;
        CCPoint tilePoint = WorldController::getInstance()->getPointByIndex(info.cityIndex);
        CCPoint pos = m_map->getViewPointByTilePoint(tilePoint,info.tileServerId);
        CCSprite* tempBatchSpNode = NULL;
        auto addPic = [&](std::vector<string> dicVec,string picStr ,bool _isMyAlliance){
            int _index = 0;
            tempBatchSpNode = CCLoadSprite::createSprite(picStr.c_str());
            tempBatchSpNode->setContentSize(CCSizeZero);
            tempBatchSpNode->setTag(info.cityIndex);
            m_conflictBatchNode->addChild(tempBatchSpNode);
            while (_index < dicVec.size()) {
                auto pic = CCLoadSprite::createSprite(picStr.c_str());
                pic->setAnchorPoint(ccp(0.5, 0.5));
                pic->setPosition(pos);
                string dirStr = dicVec.at(_index);
                if (dirStr == "l") {
                    pic->setScaleX(-1);
                }else if (dirStr == "r"){
                    pic->setScaleY(-1);
                }else if (dirStr == "t"){
                    
                }
                else if (dirStr == "b"){
                    pic->setScale(-1);
                }
                if (_isMyAlliance) {
                    pic->setOpacity(125);
                }
                else{
                    pic->setOpacity(175);
                }
                tempBatchSpNode->addChild(pic);
                _index++;
            }
        };
        
        int pointx = tilePoint.x;
        int pointy = tilePoint.y;
//                CCLOG("logInfo-----point%d%d",pointx,pointy);
        //        CCLOG("logInfo-----directionInfo%s",tempDirectionStr.c_str());
        //        CCLOG("logInfo-----cityIndex %d",info.cityIndex);
        bool isMyAlliance = false;
        int shouldAddLb = 0;
        if(tempDirectionStr != ""){ //new style "0_lbt"，"1_lbt";
            //            CCLOG("logInfo-----1212121212212121: %s",tempDirectionStr.c_str());
            string spStr = "allianceLine_red.png";
            if (info.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                isMyAlliance = true;
            }
            if (isMyAlliance) {
                spStr = "allianceLine_blue.png";
            }
            hideSameBoard(info.cityIndex);
            vector<string> directionVec;
            CCCommonUtils::splitString(info.m_aArea_direction, "_", directionVec);
            string dirStr("o");
            if (directionVec.size() == 2) {
                shouldAddLb = atoi(directionVec.at(0).c_str());
                dirStr = directionVec.at(1).c_str();
            }
            else if (directionVec.size() == 1){
                shouldAddLb = atoi(directionVec.at(0).c_str());
            }
            std::vector<string> dirStrVec;
            if (dirStr.find("l") != string::npos) {
                dirStrVec.push_back("l");
            }
            if (dirStr.find("r") != string::npos) {
                dirStrVec.push_back("r");
            }
            if (dirStr.find("t") != string::npos) {
                dirStrVec.push_back("t");
            }
            if (dirStr.find("b") != string::npos) {
                dirStrVec.push_back("b");
            }
            if(dirStrVec.size() > 0){
                addPic(dirStrVec,spStr,isMyAlliance);
            }
        }
        if (shouldAddLb == 1) {
            string fullName = info.m_aArea_name;
            if(info.m_aArea_simpleName != ""){
                fullName = CCString::createWithFormat("%s%s%s%s", "(", info.m_aArea_simpleName.c_str(), ")", fullName.c_str())->getCString();
            }
            auto name = CCLabelIFTTF::create();
            name->setString(fullName.c_str());
            name->setFontSize(20);
            if(isMyAlliance){
                name->setColor(ccc3(138,181,177));
            }
            else{
                name->setColor(ccc3(227,158,158));
            }
            name->setPosition(pos + ccp(0, -_tile_height/2));
            name->setTag(info.cityIndex);
            name->setVisible(false);
            m_aAreaUnbachLbNode->addChild(name);
        }
    }
    if(info.cityType == Throne && info.cityIndex != info.parentCityIndex){
        return;
    }
    
    releaseCity(info);
//    CCPoint tilePoint = WorldController::getInstance()->getPointByIndex(info.cityIndex);
//    int pointx = tilePoint.x;
//    int pointy = tilePoint.y;
//    CCLOG("testwdz-----point%d %d",pointx,pointy);
//    CCLOG("testwdz cityIndex = %d",info.cityIndex);
//    CCLOG("testwdz sid = %d",info.tileServerId);
    if((info.cityType == FieldMonster || info.cityType == ActBossTile) && info.fieldMonsterInfo.currentHp == 0 && !info.fieldMonsterInfo.isHpChange){
        return;
    }
    bool isCityBuilding = false;
    bool isDefaultBG = true;
    switch (info.cityType) {
        case OriginTile: {
            m_tileLayer->revertTileGID(info.cityTilePoint);
        }
            break;
        case ResourceTile: {
            isCityBuilding = true;
            if(isDefaultBG){
                m_tileLayer->setTileGID(DefaultBlankBG, info.cityTilePoint);
            }
        }
            break;
        case CityTile: {
            isCityBuilding = true;
            if(isDefaultBG){
                m_tileLayer->setTileGID(DefaultBlankBG, info.cityTilePoint);
            }
        }
            break;
        case CampTile: {
            isCityBuilding = true;
            if(isDefaultBG){
                m_tileLayer->setTileGID(DefaultBlankBG, info.cityTilePoint);
            }
        }
            break;
        case MonsterTile: {
            isCityBuilding = true;
            if(isDefaultBG){
                m_tileLayer->setTileGID(DefaultBlankBG, info.cityTilePoint);
            }
        }
            break;
        case FieldMonster:{
            isCityBuilding = true;
        }
            break;
        case ActBossTile:{
            isCityBuilding = true;
        }
            break;
        case Trebuchet:{
            isCityBuilding = true;
        }
            break;
        case Throne:{
            isCityBuilding = true;
        }
            break;
        case Tile_allianceArea:{
            isCityBuilding = true;
        }
            break;
        case tile_superMine:{
            isCityBuilding = true;
        }
        case tile_tower:{
            isCityBuilding = true;
        }
        case tile_banner:{
            isCityBuilding = true;
        }
        case tile_wareHouse:{
            isCityBuilding = true;
        }
            break;
        default:
            return;
    }
    if(info.cityTilePoint.x == 0
       || info.cityTilePoint.x == WorldController::getInstance()->_current_tile_count_x - 1
       || info.cityTilePoint.y == 0
       || info.cityTilePoint.y == WorldController::getInstance()->_current_tile_count_y - 1
       ){
        addBatchItem(EdgyTag, info.cityIndex);
        bool isInFightMap = WorldController::getInstance()->currentMapType == SERVERFIGHT_MAP ? true:false;
        if(info.cityTilePoint.x == 0 && info.cityTilePoint.y == 0){
            addBatchItem(EdgyTag1, info.cityIndex);
            if (!isInFightMap) {
                addBatchItem(WorldBorder1, info.cityIndex);}
        }
        if(info.cityTilePoint.x == WorldController::getInstance()->_current_tile_count_x - 1 && info.cityTilePoint.y == 0){
            addBatchItem(EdgyTag1, info.cityIndex);
            if (!isInFightMap) {
                addBatchItem(WorldBorder1, info.cityIndex);}
        }
        if(info.cityTilePoint.x == 0 && info.cityTilePoint.y == WorldController::getInstance()->_current_tile_count_y - 1){
            addBatchItem(EdgyTag1, info.cityIndex);
            if (!isInFightMap) {
                addBatchItem(WorldBorder1, info.cityIndex);}
        }
        if(info.cityTilePoint.x == WorldController::getInstance()->_current_tile_count_x - 1 && info.cityTilePoint.y == WorldController::getInstance()->_current_tile_count_y - 1){
            addBatchItem(EdgyTag1, info.cityIndex);
            if (!isInFightMap) {
                addBatchItem(WorldBorder1, info.cityIndex);}
        }
        if (!isInFightMap) {
            addBatchItem(WorldBorder, info.cityIndex);
        }
        
        
        
    }
    // clear overflow citys
    static unsigned int displayedCityMaxCount = 500;
    static unsigned int displayedCityReduceCount = 400;
    if (isCityBuilding) {
        if (m_displayedCity.size() > displayedCityMaxCount) {
            while (m_displayedCity.size() > displayedCityReduceCount) {
                releaseCity(m_cityInfo[m_displayedCity.front()]);
                m_displayedCity.pop_front();
            }
        }
        m_displayedCity.remove(info.cityIndex);
        m_displayedCity.push_back(info.cityIndex);
    }
    addUnderNode(info.cityIndex);
}

void WorldMapView::releaseCity(WorldCityInfo &info) {
    delUnderNode(info.cityIndex);
    delBatchItem(AllianceParticle, info.cityIndex);
    m_tileLayer->revertTileGID(info.cityTilePoint);
    vector<FlagParInfo>::iterator it = m_flagParDatas.begin();
    for(; it != m_flagParDatas.end();){
        if( (*it).index == info.cityIndex ) {
            it = m_flagParDatas.erase(it);
        }
        else {
            ++it;
        }
    }
}


void WorldMapView::addUnderNode(unsigned int index) {
    auto getWarBuildBGArr = [](WorldActivityState &state, const CCPoint &pt, WorldCityInfo &info){
        CCArray *arr = CCArray::create();
        if(state != WarTime){
            return arr;
        }
        
        int num = 0;
        if(state != NotOpen){
            num = 1;
            if(info.playerName != ""){
                num = 2;
            }
            if(info.allianceName != ""){
                num = 3;
            }
        }
        
        int cellH = 33;
        int startY = 0;
        if(info.cityType == Throne){
            startY -= 130;
        }
        int index = 0;
        
        
        while (index < num) {
            int x = pt.x;
            int y = startY + pt.y;
            startY -= cellH;
            CCDictionary *dict = CCDictionary::create();
            dict->setObject(CCString::create(CC_ITOA(x)), "x");
            dict->setObject(CCString::create(CC_ITOA(y)), "y");
            arr->addObject(dict);
            index++;
        }
        return arr;
    };
    
    auto addWarBuildLabel = [](std::string str, const CCPoint &pt, ccColor3B &color, CCNode *parent, int tag, map<unsigned int , vector<CCSafeObject<CCNode> > > &map, int index){
        auto label = CCLabelIFTTF::create(str.c_str());
        label->setPosition(pt);
        label->setAlignment(kCCTextAlignmentLeft);
        label->setAnchorPoint(ccp(0, 0.5));
        parent->addChild(label);
        label->setTag(tag);
        label->setColor(color);
        map[index].push_back(label);
        return label;
    };
    
    if (m_cityBatchNode->getChildByTag(index)) {
        return;
    }
    auto &info = m_cityInfo[index];
    auto pos = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(index),info.tileServerId);
    switch (info.cityType) {
        case ResourceTile:{
            Node* under = Node::create();
            under->setAnchorPoint(ccp(0, 0));
            under->setTag(index);
            under->setPosition(ccp(pos.x-_halfTileSize.width,pos.y-_halfTileSize.height)); // left-bottom corner
            m_cityItem[index].push_back(under);
            m_cityBatchNode->addChild(under, index);
            
            auto island = CCLoadSprite::createSprite("decor_island_100.png");
            island->setAnchorPoint(Vec2(0, 0));
            under->addChild(island);

            string house_img = getSpriteName(info);
            auto house = CCLoadSprite::createSprite(house_img.c_str());
            house->setAnchorPoint(Vec2(0, 0));
            if (house_img == "0020.png")
            {
                house->setPositionY(11);
            }
            else if (house_img == "0019.png")
            {
                house->setPositionY(6);
            }
            else if (house_img == "0021.png")
            {
                house->setPositionY(19);
            }
            under->addChild(house);
            under->setContentSize(house->getContentSize());
            
            addBatchItem(LevelTag, index);
            auto lv = CCLabelBatch::create(CC_ITOA((int)info.resource.lv),m_labelNode);
            lv->setScale(0.6*MapGlobalScale);
            lv->setPosition(ccp(30, -25)+pos);
            m_cityItem[index].push_back(lv);
        }
            break;
        case tile_tower:{
            if (GlobalData::shared()->alliance_territory_tower_switch == 1){
                AAreaState _state = AAreaState(info.m_towerInfo.state);
                string spStr("");
                if (_state == Astate_UnGarrison) {
                    spStr = "allianceTower.png";
                }else{
                    spStr = "allianceTowerBad.png";
                }
                auto under = CCLoadSprite::createSprite(spStr.c_str());
                under->setAnchorPoint(ccp(0, 0));
                under->setTag(index);
                under->setPosition(ccp(pos.x-_halfTileSize.width,pos.y-_halfTileSize.height) + ccp(59, 29)); // left-bottom corner
                m_cityItem[index].push_back(under);
                m_batchNode->addChild(under, index);
                
                string fullName =  _lang_1("115406", CC_ITOA(info.m_towerInfo.dialog));
                if (info.m_aArea_simpleName != "") {
                    fullName = string("") + "(" + info.m_aArea_simpleName + ")" + fullName;
                }
                string stateStr("");
                string ccbName("");
                bool bAddBuildPar = false;
                if(_state == Astate_Building){
                    stateStr = _lang("115305");
                    ccbName = "WordBuild_tower";
                    bAddBuildPar = true;
                }else if (_state == Astate_BeforeBuild){
                    stateStr = _lang("115306");
                }else if (_state == Astate_UnGarrison){
                    stateStr = _lang("115409");
                }
                if (stateStr != "") {
                    stateStr = string("(") + stateStr + string(")");
                }
                fullName = fullName + stateStr;
                auto name = CCLabelIFTTF::create();
                name->setString(fullName.c_str());
                name->setFontSize(20);
                name->setColor(ccc3(242, 237, 222));
                if (info.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                    name->setColor(ccc3(0, 253, 255));
                }
                name->setPosition(ccp(11, -50)+pos);
                
                initTowerRange(index, true);
                
                m_unBatchLabelNode->addChild(name);
                m_cityItem[index].push_back(name);
                
                addBatchItem(NameTag, index);
                if(ccbName != ""){
                    CCNode* buildAniNode = AAreaBuildCCB::create()->setCCBName(ccbName);
                    buildAniNode->setTag(info.cityIndex);
                    buildAniNode->setPosition(pos + ccp(-15,-47+_halfTileSize.height));
                    m_aAreaAniNode->addChild(buildAniNode);
                    m_cityItem[index].push_back(buildAniNode);
                }
                addTowerRange(index);
            }
        }
            break;
        case tile_banner:{
            if (GlobalData::shared()->alliance_territory_tower_switch == 1){
                AAreaState _state = AAreaState(info.m_bannerInfo.state);
                string spStr("");
                if (_state == Astate_BeforeBuild) {
                    spStr = "allianceTowerBad.png";
                }else if (_state == Astate_Building) {
                    spStr = "allianceTowerBad.png";
                }else if (_state == Astate_UnGarrison) {
                    spStr = "allianceTower.png";
                }else if (_state == Astate_Garrison) {
                    spStr = "allianceTower.png";
                }else if (_state == Astate_Damaged) {
                    spStr = "allianceTowerBad.png";
                }else if (_state == Astate_FixIng) {
                    spStr = "allianceTowerBad.png";
                }else if (_state == Astate_Destroy) {
                    spStr = "allianceTowerBad.png";
                }
                auto under = CCLoadSprite::createSprite(spStr.c_str());
                under->setAnchorPoint(ccp(0, 0));
                under->setTag(index);
                under->setPosition(ccp(pos.x-_halfTileSize.width,pos.y-_halfTileSize.height) + ccp(59, 29)); // left-bottom corner
                m_cityItem[index].push_back(under);
                m_batchNode->addChild(under, index);
                
//                std::string banner = info.m_bannerInfo.banner;
//                banner.append(".png");/////
//                CCLoadSprite::doResourceByCommonIndex(208, true);
//                auto bannerSprite = CCFlagWaveSprite::create(CCLoadSprite::loadResource(banner.c_str()));
//                bannerSprite->setAnchorPoint(ccp(0, 0));
//                if (CCCommonUtils::isIosAndroidPad()) {
//                    bannerSprite->setScale(1.0);
//                }
//                else
//                    bannerSprite->setScale(0.5);
//                bannerSprite->setPosition(under->getPosition() + under->getContentSize() * under->getScale() / 2  + ccp(0, 10));
//                m_cityItem[index].push_back(bannerSprite);
//                m_batchNode->addChild(bannerSprite);
//                CCLoadSprite::doResourceByCommonIndex(208, false);
                
                string fullName =  _lang("115534");
                if (info.m_aArea_simpleName != "") {
                    fullName = string("") + "(" + info.m_aArea_simpleName + ")" + fullName;
                }
                string stateStr("");
                string ccbName("");
                bool bAddBuildPar = false;
                if (_state == Astate_BeforeBuild) {
                    stateStr = _lang("115306");
                }else if (_state == Astate_Building) {
                    stateStr = _lang("115305");
                    ccbName = "WordBuild_tower";
                    bAddBuildPar = true;
                }else if (_state == Astate_UnGarrison) {
                    stateStr = _lang("115308");
                }else if (_state == Astate_Garrison) {
                    stateStr = _lang("115307");
                }else if (_state == Astate_Damaged) {
                    stateStr = _lang("115309");
                }else if (_state == Astate_FixIng) {
                    stateStr = _lang("115310");
                    ccbName = "WordBuild_tower";
                }else if (_state == Astate_Destroy) {
                    stateStr = _lang("115311");
                    ccbName = "WordRemove_tower";
                }
                if (stateStr != "") {
                    stateStr = string("(") + stateStr + string(")");
                }
                fullName = fullName + stateStr;
                auto name = CCLabelIFTTF::create();
                name->setString(fullName.c_str());
                name->setFontSize(20);
                name->setColor(ccc3(242, 237, 222));
                if (info.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                    name->setColor(ccc3(0, 253, 255));
                }
                name->setPosition(ccp(11, -50)+pos);
                
                m_unBatchLabelNode->addChild(name);
                m_cityItem[index].push_back(name);
                
                addBatchItem(NameTag, index);
                if(ccbName != ""){
                    CCNode* buildAniNode = AAreaBuildCCB::create()->setCCBName(ccbName);
                    buildAniNode->setTag(info.cityIndex);
                    buildAniNode->setPosition(pos + ccp(-15,-47+_halfTileSize.height));
                    m_aAreaAniNode->addChild(buildAniNode);
                    m_cityItem[index].push_back(buildAniNode);
                }
            }
        }
            break;
        case tile_superMine:{
            if (true) {
                CCArray *arr = getCityPicArr(info, 1,getPicIdByCityInfo(info));
                int i = 0;
                while (i < arr->count()) {
                    auto pics = _dict(arr->objectAtIndex(i));
                    std::string picStr = pics->valueForKey("pic")->getCString();
                    int x = pics->valueForKey("x")->intValue();
                    int y = pics->valueForKey("y")->intValue();
                    auto under = CCLoadSprite::createSprite(picStr.c_str());
                    under->setAnchorPoint(ccp(0, 0));
                    under->setTag(index);
                    under->setPosition(ccp(pos.x-_halfTileSize.width,pos.y-_halfTileSize.height) + ccp(x, y)); // left-bottom corner
                    m_cityItem[index].push_back(under);
                    m_batchNode->addChild(under, index + i);
                    i++;
                }
                if(info.parentCityIndex == info.cityIndex){
                    string fullName = "";
                    switch (info.m_superMineInfo.type) {
                        case Food:
                            fullName = _lang("115376");
                            break;
                        case Wood:
                            fullName = _lang("115377");
                            break;
                        case Iron:
                            fullName = _lang("115378");
                            break;
                        case Stone:
                            fullName = _lang("115379");
                            break;
                        default:
                            break;
                    }
                    AAreaState _state = AAreaState(info.m_superMineInfo.trstat);
                    string stateStr("");
                    string ccbName("");
                    bool bAddBuildPar = false;
                    if(_state == Astate_Building){
                        stateStr = _lang("115305");
                        ccbName = "WordRemove";
                        bAddBuildPar = true;
                    }else if (_state == Astate_BeforeBuild){
                        stateStr = _lang("115306");
                    }else if (_state == Astate_Damaged){
                        stateStr = _lang("115381");
                    }else if (_state == Astate_Destroy){
                        stateStr = _lang("115380");
                    }
                    if (info.m_aArea_simpleName != "") {
                        fullName = string("") + "(" + info.m_aArea_simpleName + ")" + fullName;
                    }
                    if (stateStr != "") {
                        stateStr = string("(") + stateStr + string(")");
                    }
                    fullName = fullName + stateStr;
                    auto name = CCLabelIFTTF::create();
                    name->setString(fullName.c_str());
                    name->setFontSize(20);
                    name->setColor(ccc3(242, 237, 222));
                    if (info.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                        name->setColor(ccc3(0, 253, 255));
                    }
                    if(info.m_superMineInfo.type == Food){
                        name->setPosition(ccp(11, -45)+pos);
                    }else{
                        name->setPosition(ccp(11, -25)+pos);
                    }
                    
                    m_unBatchLabelNode->addChild(name);
                    m_cityItem[index].push_back(name);
                    
                    addBatchItem(NameTag, index);
                    if(ccbName != ""){
                        CCNode* buildAniNode = AAreaBuildCCB::create()->setCCBName(ccbName);
                        buildAniNode->setTag(info.cityIndex);
                        buildAniNode->setPosition(pos + ccp(-20,-20+_halfTileSize.height)+ ccp(23,7));
                        m_aAreaAniNode->addChild(buildAniNode);
                        m_cityItem[index].push_back(buildAniNode);
                    }
                    if(info.m_aArea_flag != ""){
                        addBatchItem(AllianceFlag, index);
                    }
                }
            }
        }
            break;
        case tile_wareHouse:{
            CCArray *arr = getCityPicArr(info, 1,getPicIdByCityInfo(info));
            int i = 0;
            while (i < arr->count()) {
                auto pics = _dict(arr->objectAtIndex(i));
                std::string picStr = pics->valueForKey("pic")->getCString();
                int x = pics->valueForKey("x")->intValue();
                int y = pics->valueForKey("y")->intValue();
                auto under = CCLoadSprite::createSprite(picStr.c_str());
                under->setAnchorPoint(ccp(0, 0));
                under->setTag(index);
                under->setPosition(ccp(pos.x-_halfTileSize.width,pos.y-_halfTileSize.height) + ccp(x, y)); // left-bottom corner
                m_cityItem[index].push_back(under);
                m_batchNode->addChild(under, index + i);
                i++;
            }
            if(info.parentCityIndex == info.cityIndex){
                string fullName = info.m_aArea_nickname;
                if (fullName == "") {
                    fullName = _lang("115364");
                }
                if(info.m_aArea_simpleName != ""){
                    fullName = CCString::createWithFormat("%s%s%s%s", "(", info.m_aArea_simpleName.c_str(), ")", fullName.c_str())->getCString();
                }
                AAreaState _state = AAreaState(info.m_warehouseInfo.state);
                string stateStr("");
                string ccbName("");
                bool bAddBuildPar = false;
                if(_state == Astate_Building){
                    stateStr = _lang("115305");
                    ccbName = "WordBuild";
                    bAddBuildPar = true;
                }else if (_state == Astate_BeforeBuild){
                    stateStr = _lang("115306");
                }else if (_state == Astate_UnGarrison){
                    stateStr = _lang("115448");
                }
                if (stateStr != "") {
                    stateStr = string("(") + stateStr + string(")");
                }
                fullName = fullName + stateStr;
                auto name = CCLabelIFTTF::create();
                name->setString(fullName.c_str());
                name->setFontSize(20);
                name->setColor(ccc3(242, 237, 222));
                if (info.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                    name->setColor(ccc3(0, 253, 255));
                }
                name->setPosition(ccp(0, -25)+pos);
                m_unBatchLabelNode->addChild(name);
                m_cityItem[index].push_back(name);
                
                addBatchItem(NameTag, index);
                if(ccbName != ""){
                    CCNode* buildAniNode = AAreaBuildCCB::create()->setCCBName(ccbName);
                    buildAniNode->setTag(info.cityIndex);
                    buildAniNode->setPosition(pos + ccp(-20,-20+_halfTileSize.height)+ ccp(0,-10));
                    m_aAreaAniNode->addChild(buildAniNode);
                    m_cityItem[index].push_back(buildAniNode);
                }
                if(info.m_aArea_flag != ""){
                    addBatchItem(AllianceFlag, index);
                }
            }
        }
            break;
        case CityTile:{
            auto &player = WorldController::getInstance()->m_playerInfo[info.playerName];
            CCArray *arr = getCityPicArr(info, player.cityLv, -1, pos);
            int count = arr->count();
            int i = 0;
            while (i < count) {
                auto pics = _dict(arr->objectAtIndex(i));
                std::string picStr = pics->valueForKey("pic")->getCString();
                int x = pics->valueForKey("x")->intValue();
                int y = pics->valueForKey("y")->intValue();
                
                Node* under = Node::create();
                under->setAnchorPoint(ccp(0, 0));
                under->setTag(index);
                under->setPosition(ccp(pos.x - _halfTileSize.width, pos.y - _halfTileSize.height) + ccp(x, y)); // left-bottom corner
                m_cityItem[index].push_back(under);
                m_cityBatchNode->addChild(under, index + i);
                i++;
                
                // guo.jiang todo
                // 暂时按照 COK 的规则填充 4 个 tile
                Node* house = nullptr;
                if (i == 1) // 附加图不考虑 worldcastle.xls/addPic
                {
                    int island_idx = NBWorldMapMainCity::getMainCityIslandImageIndex(&info, player.cityLv, -1);
                    if (island_idx >= 0)
                    {
                        auto island = CCLoadSprite::createSprite(NBWorldMapMainCity::getMainCityIslandImage(island_idx, pos.x, pos.y));
                        island->setAnchorPoint(Vec2(0, 0));
                        under->addChild(island);
                        
                        house = NBWorldMapMainCity::getMainCity(island_idx, player.cityLv, -1);
                    }
//                    if (island_idx == 0)
//                    {
//                        static const char* images[] = {"z_island_001.png", "z_island_002.png", "z_island_003.png", "z_island_004.png"};
//                        float xy[] = {0,       0 + 256, 0, 0 + 256,
//                                      0 + 128, 0 + 128, 0,       0, };
//                        
//                        for (int iii = 0; iii < 4; iii++)
//                        {
//                            auto island = CCLoadSprite::createSprite(images[iii]);
//                            island->setAnchorPoint(Vec2(0, 0));
//                            island->setPosition(Vec2(xy[iii], xy[iii + 4]));
//                            under->addChild(island);
//                        }
//                        
//                        house = NBWorldMapMainCity::getMainCity(island_idx, player.cityLv, -1);
//                    }
//                    else
//                    {
//                        house = NBWorldMapMainCity::getMainCity(island_idx, player.cityLv, -1);
//                    }
                }
                else
                {
                    house = NBWorldMapMainCity::getMainCity(-1, player.cityLv, -1);
                }
                if (house)
                {
                    under->addChild(house);
                    under->setContentSize(house->getContentSize());
                }
            }
            
            if (info.parentCityIndex == info.cityIndex) {
                
                auto &player = WorldController::getInstance()->m_playerInfo[info.playerName];
                auto now = WorldController::getInstance()->getTime();
                
                addBatchItem(LevelTag, index);
                
                auto lv = CCLabelBatch::create(CC_ITOA((int)player.cityLv), m_labelNode);
                lv->setScale(0.6 * MapGlobalScale);
                lv->setPosition(ccp(30, 10) + pos);
                m_cityItem[index].push_back(lv);
                string fullName = info.playerName;
                
                if (player.type == PlayerSelf) {
                    fullName = _lang("108678");
                }
                if(info.leagueAsn != ""){
                    fullName = CCString::createWithFormat("%s%s%s%s", "(", info.leagueAsn.c_str(), ")", fullName.c_str())->getCString();
                }
                if(info.srcServer>=0 && GlobalData::shared()->isCrossService){
                    fullName.append("#");
                    fullName.append(CC_ITOA(info.srcServer));
                }else{
                    if (!WorldController::isInSelfServer(info.tileServerId)) {
                        int currentServer = GlobalData::shared()->playerInfo.currentServerId;
                        fullName = fullName + string("#") + CC_ITOA(currentServer);
                    }
                }
            
                auto name = CCLabelIFTTF::create();
                name->setString(fullName.c_str());
                name->setFontSize(20);
                if (player.type == PlayerSelf) {
                    name->setColor(ccc3(255, 192, 0));
                }
                else if (player.type == PlayerAlliance){
                    name->setColor(ccc3(0, 253, 255));
                }else if(GlobalData::shared()->isCrossService && info.srcServer>0 && info.srcServer != GlobalData::shared()->playerInfo.crossFightSrcServerId){
                    name->setColor(ccc3(255, 0, 0));
                }else {
                    name->setColor(ccc3(242, 237, 222));
                }
                name->setPosition(ccp(0, -15)+pos);
                m_unBatchLabelNode->addChild(name);
                m_cityItem[index].push_back(name);
                
                addBatchItem(NameTag, index);
                if (player.userType == 11) {
                    int x = name->getPosition().x+name->getContentSize().width/2+2;
                    int y = name->getPosition().y;
                    CCDictionary *dict = CCDictionary::create();
                    dict->setObject(CCString::create(CC_ITOA(x)), "x");
                    dict->setObject(CCString::create(CC_ITOA(y)), "y");
                    addBatchItem(NameTag2, index,dict);
                }
                if (player.type == PlayerSelf) {
                    addBatchItem(NameTag1, index);
                }
                if (player.fireStamp > now) {
                    addBatchItem(FireTag, index);
                }
                
                if (player.protectStamp > now) {
                    addBatchItem(ProtectTag, index);
                }else{
                    if (player.resourceProtectStamp > now) {
                        addBatchItem(ResourceProtectTag, index);
                    }
                }
                if(player.allianceFlag != ""){
                    addBatchItem(AllianceFlag, index);
                }
                if(player.officer != ""){
                    addBatchItem(OfficerTagBG, index);
                    addBatchItem(OfficerTag, index);
                    addBatchItem(OfficerTagParticle, index);
                }
                if (info.cross==1) {
                    addBatchItem(CSProtectTag, index);
                }
                
                if (GlobalData::shared()->isCrossService && info.winNum>0) {
                    createWinNum(index);
                }
                
            }
        }
            break;
        case Tile_allianceArea:{
            if (true) {
                bool isMyalliance = false;
                if (GlobalData::shared()->playerInfo.isInAlliance() && info.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                    isMyalliance = true;
                }
                if (m_aAreaAniNode->getChildByTag(info.cityIndex)) {
                    m_aAreaAniNode->removeChildByTag(info.cityIndex);
                }
                CCArray *arr = getCityPicArr(info, 1,getPicIdByCityInfo(info));
                int i = 0;
                while (i < arr->count()) {
                    auto pics = _dict(arr->objectAtIndex(i));
                    std::string picStr = pics->valueForKey("pic")->getCString();
                    int x = pics->valueForKey("x")->intValue();
                    int y = pics->valueForKey("y")->intValue();
                    auto under = CCLoadSprite::createSprite(picStr.c_str());
                    under->setAnchorPoint(ccp(0, 0));
                    under->setTag(index);
                    under->setPosition(ccp(pos.x-_halfTileSize.width,pos.y-_halfTileSize.height) + ccp(x, y)); // left-bottom corner
                    m_cityItem[index].push_back(under);
                    m_batchNode->addChild(under, index + i);
                    i++;
                }
                if(info.parentCityIndex == info.cityIndex){
                    if (0 < info.m_allianceAreaInfo.range && info.m_allianceAreaInfo.range < 100) {
                        initAAreaBlankInfo(info.cityIndex, info.m_allianceAreaInfo.range,isMyalliance);
                    }
                    string fullName = info.m_aArea_nickname;
                    if (fullName == "") {
                        fullName = _lang_1("115312", CC_ITOA(info.m_allianceAreaInfo.dialogId));
                    }
                    if(info.m_aArea_simpleName != ""){
                        fullName = CCString::createWithFormat("%s%s%s%s", "(", info.m_aArea_simpleName.c_str(), ")", fullName.c_str())->getCString();
                    }
                    // the resource is in 507 which has been loaded in init method
                    if (44996 == getPicIdByCityInfo(info)) {
                        CCPointArray* pointArr = CCPointArray::create(3);
                        pointArr->addControlPoint(ccp(-70,59));
                        pointArr->addControlPoint(ccp(-2,23));
                        pointArr->addControlPoint(ccp(66,57));
                        for (int i = 0; i < 3; i++) {
                            auto bannerSprite = CCFlagWaveSprite::create(CCLoadSprite::loadResource("1212_double.png"));
                            bannerSprite->setScale(0.18);
                            bannerSprite->setPosition(pos + pointArr->getControlPointAtIndex(i) + ccp(0, _halfTileSize.height) + ccp(12, 20));
                            m_bannerNode->addChild(bannerSprite);
                            m_cityItem[index].push_back(bannerSprite);
                        }
                    }

                    AAreaState _state = AAreaState(info.m_allianceAreaInfo.state);
                    string stateStr("");
                    string ccbName("");
                    bool bAddBuildPar = false;
                    if(_state == Astate_Building){
                        stateStr = _lang("115305");
                        ccbName = "WordBuild";
                        bAddBuildPar = true;
                    }else if (_state == Astate_BeforeBuild){
                        stateStr = _lang("115306");
                    }else if (_state == Astate_Garrison){
                        stateStr = _lang("115307");
                    }else if (_state == Astate_UnGarrison){
                        stateStr = _lang("115308");
                    }else if (_state == Astate_Damaged){
                        stateStr = _lang("115309");
                    }else if (_state == Astate_FixIng){
                        stateStr = _lang("115310");
                        ccbName = "WordBuild";
                    }else if (_state == Astate_Destroy){
                        stateStr = _lang("115311");
                        ccbName = "WordRemove";
                    }
                    if (stateStr != "") {
                        stateStr = string("(") + stateStr + string(")");
                    }
                    fullName = fullName + stateStr;
                    auto name = CCLabelIFTTF::create();
                    name->setString(fullName.c_str());
                    name->setFontSize(20);
                    name->setColor(ccc3(242, 237, 222));
                    if (info.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                        name->setColor(ccc3(0, 253, 255));
                    }
                    name->setPosition(ccp(0, -15)+pos);
                    m_unBatchLabelNode->addChild(name);
                    m_cityItem[index].push_back(name);
                    if("" != info.m_aArea_simpleName){
                        auto simpName = CCLabelIFTTF::create();
                        simpName->setString(info.m_aArea_simpleName.c_str());
                        simpName->setFontSize(20);
                        simpName->setColor(ccc3(10, 98, 161));
                        simpName->setPosition(ccp(-60,_halfTileSize.height -30)+pos);
                        simpName->setSkewY(-26.5);
                        m_simNameUnbatchNode->addChild(simpName);
                        m_cityItem[index].push_back(simpName);
                    }
                    addBatchItem(NameTag, index);
                    
                    if(info.m_aArea_flag != ""){
                        addBatchItem(AllianceFlag, index);
                    }
                    if(isMyalliance){
                        auto heartBeatContainer = CCLoadSprite::createSprite("AllianceTerritory_action.png");
                        heartBeatContainer->setTextureRect(CCRectZero);
                        auto centerPt = WorldController::getPointByIndex(index);
                        std::map<int, int> m_tempMap;
                        int radius = info.m_allianceAreaInfo.range / 2;
                        int i = -radius;
                        while (i < radius) {
                            int j = -radius;
                            while (j < radius) {
                                auto pt = centerPt + ccp(i, j);
                                int currentIndex = WorldController::getIndexByPoint(pt);
                                if (currentIndex > 0 && currentIndex <= WorldController::getInstance()->_current_index_limit) {
                                    m_tempMap[currentIndex] = 1;
                                }
                                j++;
                            }
                            i++;
                        }
                        for (auto it = m_tempMap.begin(); it != m_tempMap.end(); it++) {
                            int currentIndex = it->first;
                            if(m_cityInfo.find(currentIndex) != m_cityInfo.end()){
                                auto &cityInfo = m_cityInfo.find(currentIndex)->second;
                                string areaStr = cityInfo.m_aArea_direction;
                                if (cityInfo.m_aArea_id != info.m_aArea_id || info.m_aArea_pointId != cityInfo.m_aArea_pointId) {
                                    continue;
                                }
                                vector<string> directionVec;
                                CCCommonUtils::splitString(areaStr, "_", directionVec);
                                if(directionVec.size() != 2){
                                    continue;
                                }
                                std::string str = directionVec[1];
                                auto addPt = ccp(0, 0);
                                int charIndex = 0;
                                while (charIndex < str.length()) {
                                    std::string dirStr = str.substr(charIndex, 1);
                                    if(dirStr != ""){
                                        auto pic = CCLoadSprite::createSprite("AllianceTerritory_action.png");
                                        //                                        pic->setColor(ccRED);
                                        if (dirStr == "x" || dirStr == "b") {
                                            pic->setSkewY(-26.5);
                                            addPt = ccp(0, -_tile_height / 2);
                                            pic->setAnchorPoint(ccp(1, 0));
                                        }
                                        else if (dirStr == "z" || dirStr == "l"){
                                            pic->setSkewY(26.5);
                                            addPt = ccp(-_tile_width / 2, 0);
                                            pic->setAnchorPoint(ccp(0, 0));
                                        }
                                        else if (dirStr == "s" || dirStr == "t"){
                                            pic->setSkewY(-26.5);
                                            pic->setAnchorPoint(ccp(0, 0));
                                            addPt = ccp(0, _tile_height / 2);
                                        }
                                        else if (dirStr == "y" || dirStr == "r"){
                                            pic->setSkewY(26.5);
                                            pic->setAnchorPoint(ccp(1, 0));
                                            addPt = ccp(_tile_width / 2, 0);
                                        }
                                        else{
                                            charIndex++;
                                            continue;
                                        }
                                        pic->setOpacity(55);
                                        heartBeatContainer->addChild(pic);
                                        auto cityPt = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(currentIndex),info.tileServerId);
                                        auto realPt = cityPt - pos + addPt;
                                        pic->setPosition(realPt + ccp(0,-_halfTileSize.height));
                                    }
                                    charIndex++;
                                }
                            }
                        }
                        heartBeatContainer->setScale(0);
                        CCSequence *seque = CCSequence::create(CCScaleTo::create(3.5, 1.0)
                                                               , CCScaleTo::create(0.0, 0)
                                                               , NULL
                                                               );
                        heartBeatContainer->runAction(CCRepeatForever::create(seque));
                        m_batchNode->addChild(heartBeatContainer);
                        m_cityItem[index].push_back(heartBeatContainer);
                        heartBeatContainer->setPosition(pos + ccp(0, _halfTileSize.height));
                    }
                    
                    if(ccbName != ""){
                        CCNode* buildAniNode = AAreaBuildCCB::create()->setCCBName(ccbName);
                        buildAniNode->setTag(info.cityIndex);
                        buildAniNode->setPosition(pos + ccp(-20,-20+_halfTileSize.height));
                        m_aAreaAniNode->addChild(buildAniNode);
                        m_cityItem[index].push_back(buildAniNode);
                    }
                    if (bAddBuildPar) {
                        for (int j=4; j<=6; j++) {
                            string particleStr("CityMove_%d");
                            auto particle1 = ParticleController::createParticle(CCString::createWithFormat(particleStr.c_str(),j)->getCString());
                            particle1->setDuration(-1);
                            particle1->setPosition(pos);
                            addParticleToBatch(particle1,getBatchTag(Partical_AAreaBuild, index));
                            m_cityItem[index].push_back(particle1);
                            CCDelayTime* delayTime = CCDelayTime::create(2.0);
                            CCCallFuncO* callFun = CCCallFuncO::create(this, callfuncO_selector(WorldMapView::onSAHaAreaParcital), CCInteger::create(index));
                            CCSequence* seq = CCSequence::create(delayTime,callFun,NULL);
                            CCRepeatForever* repeat = CCRepeatForever::create(seq);
                            particle1->runAction(repeat);
                        }
                    }
                }
                
            }
            
        }
            break;
        case CampTile:{
            auto under = CCLoadSprite::createSprite(getSpriteName(info).c_str());
            under->setAnchorPoint(ccp(0, 0));
            under->setTag(index);
            under->setPosition(ccp(pos.x-_halfTileSize.width,pos.y-_halfTileSize.height)); // left-bottom corner
            m_cityItem[index].push_back(under);
            m_cityBatchNode->addChild(under, index);
            
            auto buildAniNode = m_aAreaAniNode->getChildByTag(info.cityIndex);
            if (buildAniNode) {
                buildAniNode->removeFromParentAndCleanup(true);
            }
            //如果是camp的话,要判断是否为宝藏
            auto iter = WorldController::getInstance()->m_marchInfo.begin();
            while (iter!=WorldController::getInstance()->m_marchInfo.end()) {
                if((*iter).second.targetType == CampTile && ((*iter).second.endPointIndex == info.cityIndex)){
                    double now = WorldController::getInstance()->getTime();
                    if(((*iter).second.treasureMapFinishTime>0) && (*iter).second.treasureMapFinishTime > now){
                        under->setPosition(ccp(pos.x-_halfTileSize.width + 35 ,pos.y-_halfTileSize.height + 10));
                        buildAniNode = AAreaBuildCCB::create()->setCCBName("WordBuild_treasure");
                        buildAniNode->setTag(info.cityIndex);
                        buildAniNode->setPosition(pos + ccp(10,0));
                        m_aAreaAniNode->addChild(buildAniNode);
                        m_cityItem[index].push_back(buildAniNode);

                        int timeLeft = ((*iter).second.treasureMapFinishTime - now) / 1000;
                        int timeTotal = (*iter).second.treasureMapTime;
                        std::string tmpTime = CC_ITOA(timeLeft)+std::string("_")+CC_ITOA(timeTotal);
//                        addBatchItem(TreasureMapCDIcon, index);
                        addBatchItem(TreasureMapCDBarBG, index);
//                        addBatchItem(TreasureMapCDBar, index,CCString::create(tmpTime));
                        auto lv = CCLabelBatch::create(CC_SECTOA(timeLeft),m_labelNode);
                        lv->setScale(0.6*MapGlobalScale);
                        lv->setPosition(pos- ccp(0,_tile_height*0.5));
                        m_cityItem[index].push_back(lv);
                    }
                    break;
                }
                ++iter;
            }
            
        }
            break;
        case MonsterTile:{
            string picName = getSpriteName(info);
            auto under = CCLoadSprite::createSprite(picName.c_str());
            under->setAnchorPoint(ccp(0, 0));
            under->setTag(index);
            under->setPosition(ccp(pos.x-_halfTileSize.width,pos.y-_halfTileSize.height)); // left-bottom corner
            m_cityItem[index].push_back(under);
            m_cityBatchNode->addChild(under, index);
            int index1 = info.parentCityIndex;
            int index2 = info.cityIndex;
            if(index1 == index2){//&& info.mc_state==1
                addBatchItem(Partical_fieldMonster, index);
            }
            
            if (picName == "0013.png" || picName == "0014.png" || picName == "0015.png" || picName == "0016.png")
            {
                CCCommonUtils::setSpriteGray(under, true);
            }
        }
            break;
        case FieldMonster: {
            NBWorldMonster::addFieldMonsterUnderNode(info, pos, index);
        }
            break;
        case ActBossTile:{
            NBWorldMonster::addActBossTileUnderNode(info, pos, index);
        }
            break;
        case Trebuchet:{
            auto under = CCLoadSprite::createSprite(getSpriteName(info).c_str());
            under->setAnchorPoint(ccp(0, 0));
            under->setTag(index);
            under->setPosition(ccp(pos.x-_halfTileSize.width,pos.y-_halfTileSize.height));
            m_cityItem[index].push_back(under);
            m_cityBatchNode->addChild(under, index);
            if(info.parentCityIndex == info.cityIndex){
                if(info.trebuchetInfo.state == Wait){
                    addBatchItem(TrebuchetWait, info.cityIndex);
                }else{
                    addBatchItem(TrebuchetAttack, info.cityIndex);
                    auto rock = RockAni::create(m_mapMarchNode1);
                    auto centerPt = m_map->getViewPointByTilePoint(ccp(_relic_center_x, _relic_center_y),info.tileServerId);
                    auto rockAttackPt = centerPt;
                    int addX = 20;
                    int addY = 0;
                    if(info.trebuchetInfo.direction == 1){
                        addY = 260;
                        rockAttackPt = rockAttackPt + ccp(-30, -_tile_height/4);
                    }else if(info.trebuchetInfo.direction == 2){
                        rockAttackPt = rockAttackPt + ccp(-_tile_width+_tile_width/4, _tile_height/4);
                        addX = -100;
                        addY =  260;
                    }else if(info.trebuchetInfo.direction == 3){
                        addY = 260;
                        rockAttackPt = rockAttackPt + ccp(0, _tile_height);
                    }else if(info.trebuchetInfo.direction == 4){
                        rockAttackPt = rockAttackPt + ccp(_tile_width-_tile_width/4, _tile_height/4);
                        addX = 80;
                        addY = 240;
                    }
                    
                    CCPoint startPt =  pos + ccp(addX, addY);
                    float timeTotal = 4.0f;
                    rock->attack2(startPt, rockAttackPt, info.cityIndex, 4,info.trebuchetInfo.direction);
                    auto seque = CCSequence::create(CCDelayTime::create(timeTotal + 1.0),
                                                    CCCallFuncO::create(this, callfuncO_selector(WorldMapView::resetTrebuchet), CCInteger::create(index)),
                                                    NULL
                                                    );
                    this->runAction(seque);
                }
                WorldActivityState state = WorldController::getInstance()->getKingActivityState(info.trebuchetInfo.openTime, info.trebuchetInfo.startTime, info.trebuchetInfo.endTime);
                
                if (state == PeaceTime || state == OpenNoKing || state == NotOpen) {
                    addBatchItem(ProtectTag, index);
                }
                
                CCArray *arr = getWarBuildBGArr(state, pos, info);
                int tag = WarBuildTextTag1;
                int i = 0;
                while (i < arr->count()) {
                    addBatchItem(BatchTagType(tag), info.cityIndex, arr->objectAtIndex(i));
                    tag++;
                    i++;
                }
                arr->removeAllObjects();
                
                auto colorYellow = ccc3(255,206,132);
                auto colorRed = ccc3(255,0,0);
                auto startPos = pos - ccp(105, 0);
                if(state == WarTime){
                    std::string dialogId = "110085";
                    if(info.playerName == ""){
                        dialogId = "110090";
                    }
                    addWarBuildLabel(_lang(dialogId), startPos, colorRed, m_unBatchLabelNode, -1, m_cityItem, index);
                    int addY = 0;
                    if(info.allianceName != ""){
                        addY -= 34;
                        addWarBuildLabel(info.allianceName, startPos + ccp(0, addY), colorYellow, m_unBatchLabelNode, -1, m_cityItem, index);
                    }
                    if(info.playerName != ""){
                        addY -= 34;
                        addWarBuildLabel(info.playerName, startPos + ccp(0, addY), colorYellow, m_unBatchLabelNode, -1, m_cityItem, index);
                    }
                    auto playerIt = WorldController::getInstance()->m_playerInfo.find(info.playerName);
                    if(playerIt != WorldController::getInstance()->m_playerInfo.end() && playerIt->second.allianceFlag != ""){
                        addBatchItem(AllianceFlag, index);
                    }
                }
            }
        }
            break;
        case Throne:{
            m_throneNode->removeAllChildren();
            m_towerNode->removeAllChildren();
            
            int picIndex = getThroneIndex(atoi(info.throne_pic.c_str()));
            if(picIndex != currentThroneIndex){
                loadThroneResource(picIndex);
            }
            auto posIt = ThronePos.find(picIndex);
            std::string posStr = "0,0";
            if(posIt != ThronePos.end()){
                posStr = posIt->second;
            }
            if(posStr == ""){
                posStr = "0,0";
            }
            std::vector<std::string> posVector;
            CCCommonUtils::splitString(posStr, ",", posVector);
            int throneAddY = atoi(posVector[1].c_str());
            int throneTowerAddY = atoi(posVector[0].c_str());
            float s = 1.2f;
            std::string underPic = std::string("throne_") + CC_ITOA(picIndex) + ".png";
            auto under = CCLoadSprite::createSprite(underPic.c_str());
            under->setAnchorPoint(ccp(0.5, 0));
            under->setScale(s);
            int addH = -_halfTileSize.height * _throne_h;
            under->setTag(index);
            under->setPosition(pos + ccp(0, addH));
            m_cityItem[index].push_back(under);
            m_throneNode->addChild(under, index);
            
            std::string towerPic = std::string("throne_tower_") + CC_ITOA(picIndex) + ".png";
            auto tower = CCLoadSprite::createSprite(towerPic.c_str());
            tower->setAnchorPoint(ccp(0.5, 0));
            int dx = 0;
            int dy = (throneTowerAddY - throneAddY) * s;
            tower->setScale(s);
            tower->setPosition(under->getPosition() + ccp(dx, dy));
            m_cityItem[index].push_back(tower);
            m_towerNode->addChild(tower);
            
            WorldActivityState state = WorldController::getInstance()->getKingActivityState(info.kingBuildInfo.openTime, info.kingBuildInfo.startTime, info.kingBuildInfo.endTime);
            
            if (state == PeaceTime || state == OpenNoKing || state == NotOpen) {
                addBatchItem(ProtectTag, index);
            }
            
            auto colorWhite = ccc3(220,225,224);
            auto colorRed = ccc3(255,53,53);
            auto startPos = pos;
            
            if(state != NotOpen){
                
                bool isShow = true;
                if (state == OpenNoKing) {
                    double time = 0;
                    double now = WorldController::getInstance()->getTime();
                    double startTime = info.kingBuildInfo.startTime;
                    time = startTime - now;
                    time = time / 1000;
                    if (time > 5 * 24 * 3600) {
                        isShow = false;
                    }
                }
                
                if (isShow) {
                    addBatchItem(WarBuildStateBG, index);
                    std::string stateDialogId = "110087";
                    if(state == OpenNoKing){
                        stateDialogId = "110087";
                    }else if(state == PeaceTime){
                        stateDialogId = "110086";
                    }else if(state == WarTime){
                        stateDialogId = "110085";
                    }
                    auto label = addWarBuildLabel(_lang(stateDialogId), startPos + ccp(_tile_width / 2, _tile_height * 2 + 32), colorRed, m_unBatchLabelNode, -1, m_cityItem, index);
                    label->setDimensions(CCSize(200, 0));
                    label->setAnchorPoint(ccp(0.5, 0.5));
                    label->setAlignment(kCCTextAlignmentCenter);
                    label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
                    label->setFontSize(20);
                    
                    label = addWarBuildLabel("", label->getPosition() + ccp(0, -43), colorWhite, m_unBatchLabelNode, index, m_cityItem, index);
                    label->setAnchorPoint(ccp(0.5, 0.5));
                    label->setAlignment(kCCTextAlignmentCenter);
                    label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
                }
            }
            
            if((state == PeaceTime || state == WarTime) && info.playerName != ""){
                auto nameBG1 = CCLoadSprite::createScale9Sprite("Prompt_9.png");
                m_throneNode->addChild(nameBG1);
                nameBG1->setPosition(pos + ccp(0, -_tile_height * 3 / 2 - 50));
                m_cityItem[index].push_back(nameBG1);
                
                auto textStarPos = ccp(nameBG1->getPositionX() + 50 - nameBG1->getContentSize().width / 2, nameBG1->getPositionY() - 23 + nameBG1->getContentSize().height / 2);
                int addY = 0;
                if(info.allianceName != ""){
                    addWarBuildLabel(info.allianceName, textStarPos + ccp(0, addY), colorWhite, m_unBatchLabelNode, -1, m_cityItem, index);
                    addY -= 27;
                }
                if(info.playerName != ""){
                    addWarBuildLabel(info.playerName, textStarPos + ccp(0, addY), colorWhite, m_unBatchLabelNode, -1, m_cityItem, index);
                }
            }
            
            auto playerIt = WorldController::getInstance()->m_playerInfo.find(info.playerName);
            
            if(playerIt != WorldController::getInstance()->m_playerInfo.end() && playerIt->second.allianceFlag != ""){
                addBatchItem(AllianceFlag, index);
            }
            
            if (state == OpenNoKing || state == NotOpen) {
                auto msg = CCLabelIFTTF::create(_lang("110140").c_str());
                msg->setColor({255, 240, 0});
                msg->setAnchorPoint(ccp(0.5, 0.5));
                msg->setFontSize(20);
                msg->setPosition(pos + ccp(0, -_tile_height * 3 / 2 - 30));
                msg->setAlignment(kCCTextAlignmentCenter);
                msg->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
                auto bg = CCLoadSprite::createScale9Sprite("UI_biaotikuang.png");
                bg->setPreferredSize(CCSize(msg->getContentSize().width * msg->getOriginScaleX() + 100, msg->getContentSize().height * msg->getOriginScaleY() + 10));
                bg->setAnchorPoint(ccp(0.5, 0.5));
                bg->setPosition(msg->getPosition());
                m_unBatchLabelNode->addChild(msg);
                m_towerNode->addChild(bg);
                m_cityItem[index].push_back(bg);
                m_cityItem[index].push_back(msg);
            }
        }
            break;
            
        default:
            break;
    }
}

std::string WorldMapView::getSpriteName(WorldCityInfo &info){
    std::string picName = "0022.png";
    int index = -1;
    if(info.cityType == MonsterTile){
        if(info.cityIndex == info.parentCityIndex){
            index = 1;
        }else if(info.cityIndex == info.parentCityIndex - 1){
            index = 2;
        }else if(info.cityIndex == info.parentCityIndex - 1 - WorldController::getInstance()->_current_tile_count_x){
            index = 3;
        }else if(info.cityIndex == info.parentCityIndex - WorldController::getInstance()->_current_tile_count_x){
            index = 4;
        }
        picName = CCString::createWithFormat("00%d.png", index + 12)->getCString();
    }
    else if(info.cityType == Trebuchet){
        if(info.cityIndex == info.parentCityIndex){
            index = 1;
        }else if(info.cityIndex == info.parentCityIndex - 1){
            index = 2;
        }else if(info.cityIndex == info.parentCityIndex - 1 - WorldController::getInstance()->_current_tile_count_x){
            index = 3;
        }else if(info.cityIndex == info.parentCityIndex - WorldController::getInstance()->_current_tile_count_x){
            index = 4;
        }
        picName = CCString::createWithFormat("00%d.png", index + 22)->getCString();
    }
    else if(info.cityType == CampTile){
        bool isTreasure=false;
        auto iter = WorldController::getInstance()->m_marchInfo.begin();
        while (iter!=WorldController::getInstance()->m_marchInfo.end()) {
            if((*iter).second.targetType == CampTile && (*iter).second.endPointIndex == info.cityIndex){
                if(((*iter).second.treasureMapFinishTime>0) && ((*iter).second.treasureMapTime>0) && ((*iter).second.treasureMapFinishTime>WorldController::getInstance()->getTime())){
                    isTreasure=true;
                }
                break;
            }
            ++iter;
        }
        if(isTreasure==true){
            picName = "927.png";
        }else{
            picName = "0022.png";
        }
    }
    else if(info.cityType == ResourceTile){
        if(info.resource.type == Wood){
            picName = "0017.png";
        }else if(info.resource.type == Stone){
            picName = "0018.png";
        }else if(info.resource.type == Iron){
            picName = "0019.png";
        }else if(info.resource.type == Food){
            picName = "0020.png";
        }else if(info.resource.type == Gold){
            std::string iconName = CCCommonUtils::getPropById("100101", "resource");
            if(iconName == ""){
                picName = "0021.png";
            }else{
                picName = iconName + ".png";
            }
        }else if(info.resource.type == Silver){
            picName = "0041.png";
        }
    }
    return picName;
}

void WorldMapView::delUnderNode(unsigned int index) {
    auto items = m_cityItem.find(index);
    if (items != m_cityItem.end()) {
        auto & itemArr = (*items).second;
        while (itemArr.size()) {
            auto &item = itemArr.back();
            item->removeFromParent();
            itemArr.pop_back();
        }
    }
}

unsigned int WorldMapView::getBatchTag(BatchTagType type, unsigned int index) {
    return index + (int)type * 1000000;
}

void WorldMapView::setPraticesEffect( bool isEnable )
{
    for(auto vec : m_particleVec){
        for (auto &particle : vec.second) {
            if(!isEnable)
            {
                particle->unscheduleUpdate();
            }
            else
            {
                particle->scheduleUpdateWithPriority(1);
            }
            
        }
    }
    
    for(auto vec : m_flagParticleVec ) {
        for (auto &particle : vec.second) {
            if(!isEnable)
            {
                particle->unscheduleUpdate();
            }
            else
            {
                particle->scheduleUpdateWithPriority(1);
            }
        }
    }
    
    for(auto vec : m_walkparticleVec ) {
        for (auto &particle : vec.second) {
            if(!isEnable)
            {
                particle->setVisible(false);
                particle->unscheduleUpdate();
            }
            else
            {
                particle->setVisible(true);
                particle->scheduleUpdateWithPriority(1);
            }
        }
    }
    
    
    m_armyDustParticleNode->setVisible(isEnable);
    m_flagParticleNode->setVisible(isEnable);

}

void WorldMapView::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle,unsigned int batchTag) {
    auto batchCount = m_parBatchVec.size();
    while (batchCount--) {
        auto &batch = m_parBatchVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst && particle->getTexture()->getName() == batch->getTexture()->getName()) {
                batch->addChild(particle);
                m_particleVec[batchTag].push_back(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch(particle);
    m_layers[WM_ROAD]->addChild(newBatch, roadIndex++);
    newBatch->addChild(particle);
    m_parBatchVec.push_back(newBatch);
    m_particleVec[batchTag].push_back(particle);
}

void WorldMapView::addFlagParticleToBatch(CCParticleSystemQuad* particle,unsigned int batchTag){
//    auto batchCount = m_parFlagBatchVec.size();
//    while (batchCount--) {
//        auto &batch = m_parFlagBatchVec[batchCount];
//        if (batch) {
//            //auto batchBlend = batch->getBlendFunc();
//            auto particleBlend = particle->getBlendFunc();
//            //if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
//                batch->addChild(particle);
//                m_flagParticleVec[batchTag].push_back(particle);
//                return;
//            //}
//        }
//    }
    //auto newBatch = CCNode::create();
    
    m_flagParticleNode->addChild(particle);
    //newBatch->addChild(particle);
    //m_parFlagBatchVec.push_back(newBatch);
    
    
    m_flagParticleVec[batchTag].push_back(particle);

}

void WorldMapView::addWalkParticleToBatch(CCParticleSystemQuad* particle,unsigned int batchTag){
    auto batchCount = m_parWalkBatchVec.size();
    while (batchCount--) {
        auto &batch = m_parWalkBatchVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                m_walkparticleVec[batchTag].push_back(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_armyDustParticleNode->addChild(newBatch);
    newBatch->addChild(particle);
    m_parWalkBatchVec.push_back(newBatch);
    m_walkparticleVec[batchTag].push_back(particle);
}

void WorldMapView::addBatchItem(BatchTagType type, unsigned int index,CCObject* obj) {
    if (m_batchNode->getChildByTag(getBatchTag(type, index))) {
        return;
    }
    if (m_mapMonsterNode->getChildByTag(getBatchTag(type, index))) {
        return;
    }
    auto &info = m_cityInfo[index];
    
    auto pos = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(index),info.tileServerId);
    CCSprite* item = NULL;
    
    int zorder = 0;
    switch (type) {
        case WarBuildStateBG:{
            item = CCLoadSprite::createSprite("prompt.png");
            item->setPosition(pos + ccp(_tile_width / 2, _tile_height * 2));
        }
            break;
        case WarBuildTextTag1:
        case WarBuildTextTag2:
        case WarBuildTextTag3:{
            auto dict = _dict(obj);
            int x = dict->valueForKey("x")->intValue();
            int y = dict->valueForKey("y")->intValue();
            item = CCLoadSprite::createSprite("name_bg.png");
            item->setScaleY(0.8);
            item->setPosition(ccp(x, y));
        }
            break;
        case TrebuchetWait:{
            item = CCLoadSprite::createSprite("dj_1.png");
            item->setAnchorPoint(ccp(0, 0));
            int dx = -151;
            int dy = -26;
            item->setPosition(ccp(pos.x,pos.y + _halfTileSize.height) + ccp(dx, dy));
            auto ani = createMarchAni(AniTrebuchetWait, 0, 4);
            if (ani) {
                item->runAction(ani);
            }
        }
            break;
        case TrebuchetAttack:{
            item = CCLoadSprite::createSprite("dj_1.png");
            item->setAnchorPoint(ccp(0, 0));
            int dx = -151;
            int dy = -26;
            item->setPosition(ccp(pos.x,pos.y + _halfTileSize.height) + ccp(dx, dy));
            auto ani = createMarchAni(AniTrebuchetAttack, info.trebuchetInfo.direction, 2);
            if (ani) {
                item->runAction(ani);
            }
        }
            break;
        // flag allicnce
        case AllianceFlag:{
            float fFixScale = 1.0;
            // performance optimize 205 has loaded in init method
            // CCLoadSprite::doResourceByCommonIndex(205, true);
            auto &player = WorldController::getInstance()->m_playerInfo[info.playerName];
            std::string flagStr = player.allianceFlag + ".png";
            bool isAAreaFlag = false;
            if (info.cityType == Tile_allianceArea) {
                isAAreaFlag = true;
            }
            if (isAAreaFlag || info.cityType == tile_superMine || info.cityType == tile_wareHouse) {
                flagStr = info.m_aArea_flag + ".png";
            }
            if(flagStr.find("Alliance") != -1){
                return;
            }
            if(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(flagStr.c_str()) == NULL){
                return;
            }
            auto flag = CCLoadSprite::createSprite(flagStr.c_str());
            flag->setPosition(ccp(pos.x - _tile_width / 2 - 5, pos.y + 10));
            if(info.cityType == Trebuchet){
                flag->setPosition(ccp(pos.x - _tile_width / 2 - 15, pos.y - 20));
            }
            if(info.cityType == Throne){
                flag->setPosition(ccp(pos.x - _tile_width + 100, pos.y - 245));
            }
            if (info.cityType == tile_superMine) {
                if (info.m_superMineInfo.type == Food) {
                    flag->setPosition(ccp(pos.x - _tile_width / 2 - 10, pos.y +10));
                }else{
                    flag->setPosition(ccp(pos.x - _tile_width / 2 - 10, pos.y +20));
                }
                
                fFixScale = 0.8;
            }
            if (info.cityType == tile_wareHouse) {
                flag->setPosition(ccp(pos.x - _tile_width / 2 - 20, pos.y + 25));
                fFixScale = 0.8;
            }
            flag->setScale(FLAG_SCALE * fFixScale);
            flag->setTag(getBatchTag(type, index));
            m_flagBatch->addChild(flag);
            m_cityItem[index].push_back(flag);
            if (isAAreaFlag) {
                flag->setPosition(ccp(pos.x , pos.y)+ ccp(-20,_halfTileSize.height + 80));
                flag->setScale(FLAG_SCALE * 0.68);
                flag->setSkewY(-26.5);
                addFlagPar(info.m_aArea_flag, flag->getPosition(), index);
            }
            else{
                if (info.cityType == tile_wareHouse) {
                    addFlagPar(info.m_aArea_flag, flag->getPosition(), index);
                }
                else {
                    addFlagPar(player.allianceFlag, flag->getPosition(), index);
                }
            }
        }
            break;
        case OfficerTag:{
            auto &player = WorldController::getInstance()->m_playerInfo[info.playerName];
            std::string iconStr = std::string("icon_") + player.officer + ".png";
            item = CCLoadSprite::createSprite(iconStr.c_str());
            item->setScale(0.8);
            item->setPosition(pos + ccp(0, 160));
            zorder = 2;
        }
            break;
        case OfficerTagBG:{
            auto &player = WorldController::getInstance()->m_playerInfo[info.playerName];
            std::string iconStr = "title_officer.png";
            int officer = atoi(player.officer.c_str());
            if(officer > 216008){
                iconStr = "title_slave.png";
            }
            item = CCLoadSprite::createSprite(iconStr.c_str());
            item->setScale(1.2);
            item->setPosition(pos + ccp(0, 160));
            zorder = 1;
        }
            break;
        case OfficerTagParticle:{
            auto &player = WorldController::getInstance()->m_playerInfo[info.playerName];
            string tmpStart = "Official_";
            int count = 3;
            int officer = atoi(player.officer.c_str());
            if(officer > 216008){
                tmpStart = "Slave_";
                count = 3;
            }
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                particle->setPosition(pos + ccp(0, 160));
                addParticleToBatch(particle,getBatchTag(type, index));
                m_cityItem[index].push_back(particle);
            }
            
            return;
        }
            break;
            
        case Shadow:{
            auto shadow = CCLoadSprite::createSprite("shadow.png");
            shadow->setPosition(ccp(pos.x, pos.y + 15));
            m_mapMonsterShadowNode->addChild(shadow);
            m_cityItem[index].push_back(shadow);
        }
            break;
        case MonsterProBG1:{
            item = CCLoadSprite::createSprite("world_lv_bg.png");
            item->setPosition(ccp(pos.x - 45, pos.y-55));
            zorder = 3;
            
        }
            break;
        case LevelTag:{
            string bgStr = "building_level_overlay.png";
            if (info.cityType == ResourceTile) {
                int kl = 0;
                int kh = 0;
                if(info.resource.type == Wood || info.resource.type == Food){
                    kl = WorldController::getInstance()->resTileKey["k1"];
                    kh = WorldController::getInstance()->resTileKey["k2"];
                }else if(info.resource.type == Iron){
                    kl = WorldController::getInstance()->resTileKey["k3"];
                    kh = WorldController::getInstance()->resTileKey["k4"];
                }else if(info.resource.type == Stone){
                    kl = WorldController::getInstance()->resTileKey["k5"];
                    kh = WorldController::getInstance()->resTileKey["k6"];
                }else if(info.resource.type == Gold){
                    kl = WorldController::getInstance()->resTileKey["k7"];
                    kh = WorldController::getInstance()->resTileKey["k8"];
                }
                if(info.resource.sum >= kh) {
                    bgStr = "resbd_level_01.png";
                }else if (info.resource.sum <= kl) {
                    bgStr = "building_level_overlay.png";
                }else {
                    bgStr = "resbd_level_02.png";
                }
            }
            item = CCLoadSprite::createSprite(bgStr.c_str());
            item->setScale(0.35*MapGlobalScale);
            item->setPosition(ccp(pos.x+30, pos.y-25));
            if(info.cityType == CityTile){
                item->setPosition(ccp(pos.x+30, pos.y + 10));
            }
            zorder = 1;
        }
            break;
        case NameTag:{
            item = CCLoadSprite::createSprite("name_bg.png");
            item->setPosition(ccp(pos.x, pos.y-50));
            if(info.cityType == CityTile || info.cityType == Tile_allianceArea){
                item->setPosition(ccp(pos.x, pos.y - 15));
            }else if (info.cityType == tile_superMine){
                if (info.m_superMineInfo.type == Food) {
                    item->setPosition(ccp(pos.x + 11, pos.y - 45));
                }
                else{
                    item->setPosition(ccp(pos.x + 11, pos.y - 25));
                }
            }else if (info.cityType == tile_wareHouse){
                item->setPosition(ccp(pos.x, pos.y - 25));
            }
            zorder = 2;
        }
            break;
        case NameTag1:{
            item = CCLoadSprite::createSprite("name_bg_title.png");
            item->setPosition(ccp(pos.x, pos.y));
            zorder = 3;
        }
            break;
        case NameTag2:{
            auto dict = _dict(obj);
            int x = dict->valueForKey("x")->intValue();
            int y = dict->valueForKey("y")->intValue();
            item = CCLoadSprite::createSprite("renzheng.png");
            item->setAnchorPoint(CCPoint(0,0.5));
            item->setPosition(ccp(x, y));
            zorder = 4;
        }
            break;
        case FireTag:{
            string tmpStart = "CityFire_";
            int count = 4;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                particle->setPosition(pos + ccp(0, _tile_height / 2));
                addParticleToBatch(particle,getBatchTag(type, index));
                m_cityItem[index].push_back(particle);
            }
            
            return;
        }
            break;
        case Partical_mapMask:{
            string tmpStart = "Clouds";
            auto particle = ParticleController::createParticle(tmpStart.c_str());
            particle->setPosition(pos);
            addParticleToBatch(particle,getBatchTag(type, index));
            m_cityItem[index].push_back(particle);
            return;
        }
            break;
        case Partical_fieldMonster:{
            string tmpStart = "CityBoss_";
            int count = 5;
            if(info.mc_state != 1){
                count = 2;
            }
            CCPointArray* pointArr = CCPointArray::create(5);
            pointArr->addControlPoint(ccp(-7,190));
            pointArr->addControlPoint(ccp(-7,190));
            pointArr->addControlPoint(ccp(20,20));
            pointArr->addControlPoint(ccp(20,20));
            pointArr->addControlPoint(ccp(-32,102));
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                particle->setPosition(pos + pointArr->getControlPointAtIndex(i-1));
                addParticleToBatch(particle,getBatchTag(type, index));
                m_cityItem[index].push_back(particle);
            }
            return;
        }
            break;
        case ProtectTag:{
            item = CCLoadSprite::createSprite("protect.png");
            item->setScale(0.7);
            int addY = _tile_height + 20;
            if(info.cityType == Throne){
                addBatchItem(ThroneProtectParticle, index);
                item->setScale(2.0);
                addY += 40;
            }
            item->setOpacity(155);
            item->setPosition(pos + ccp(0, addY));
            auto seque = CCSequence::create(CCDelayTime::create(1.2), CCFadeTo::create(0.6, 255), CCDelayTime::create(0.6),CCFadeTo::create(1.2, 155), NULL);
            item->runAction(CCRepeatForever::create(seque));
        }
            break;
            
        case ThroneProtectParticle:
        {
            string tmpStart = "KingCityProtect_";
            int count = 2;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                particle->setPosition(pos);
                addParticleToBatch(particle,getBatchTag(type, index));
                m_cityItem[index].push_back(particle);
            }
            return;
            
        }
            break;
        case ResourceProtectTag:{
            item = CCLoadSprite::createSprite("protect_resource.png");
            item->setScale(2);
            item->setOpacity(155);
            item->setPosition(pos + ccp(0, _tile_height));
            auto seque = CCSequence::create(CCDelayTime::create(1.2), CCFadeTo::create(0.6, 255), CCDelayTime::create(0.6),CCFadeTo::create(1.2, 155), NULL);
            item->runAction(CCRepeatForever::create(seque));
        }
            break;
        case SmokeTag:{
            string tmpStart = "CitySmoke_";
            int count = 2;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                particle->setPosition(pos);
                addParticleToBatch(particle,getBatchTag(type, index));
                m_cityItem[index].push_back(particle);
            }
            return;
        }
            break;
        case CityMoveInViewTag:{
            string tmpStart = "Collection_Out_";
            int count = 9;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                particle->setPosition(pos + ccp(0, _tile_height / 2));
                addParticleToBatch(particle,getBatchTag(type, index));
                m_cityItem[index].push_back(particle);
            }
            return;
        }
            break;
        case WarFire:{
            for (int j=1; j<=5; j++) {
                auto particle1 = ParticleController::createParticle(CCString::createWithFormat("WarFire_%d",j)->getCString());
                auto addPt = ccp(0, 0);
                if(WorldController::getInstance()->isFourGrid(info.cityType)){
                    addPt = ccp(0, _tile_height / 2);
                }
                
                particle1->setPosition(pos + addPt);
                addParticleToBatch(particle1,getBatchTag(type, index));
                m_cityItem[index].push_back(particle1);
            }
        }
            break;
        case AllianceTerritoryParticle:{
            auto particle = ParticleController::createParticle("AllianceTerritory");
            particle->setPosition(pos);
            addParticleToBatch(particle,getBatchTag(type, index));
            m_cityItem[index].push_back(particle);
        }
            break;
        case Rock1:
        case Rock2:
        case Rock:{
            for (int j=1; j<=6; j++) {
                auto particle1 = ParticleController::createParticle(CCString::createWithFormat("CityCollapse_%d",j)->getCString());
                auto addPt = ccp(0, 0);
                if(info.cityType == CityTile){
                    addPt = ccp(0, _tile_height / 2);
                }
                particle1->setPosition(pos + addPt + ccp(60 - rand() % 120, 60 - rand() % 80));
                addParticleToBatch(particle1,getBatchTag(type, index));
                m_cityItem[index].push_back(particle1);
            }
        }
            break;
        case Rock3:{
            auto addPt = ccp(0, 0);
            std::string particleStr = "Collapse_L_%d";
            int cellNum = 4;
            if(info.trebuchetInfo.direction == 1){
                particleStr = "Collapse_D_%d";
                addPt = ccp(-20, _tile_height * cellNum + 100);
            }else if(info.trebuchetInfo.direction == 4){
                particleStr = "Collapse_L_%d";
                addPt = ccp(-_tile_width * cellNum+50, 130);
            }else if(info.trebuchetInfo.direction == 3){
                particleStr = "Collapse_U_%d";
                addPt = ccp(0, -_tile_height * (cellNum - 1));
            }else if(info.trebuchetInfo.direction == 2){
                particleStr = "Collapse_R_%d";
                addPt = ccp(_tile_width * cellNum-50, 130);
            }
            for (int j=1; j<=4; j++) {
                auto particle1 = ParticleController::createParticle(CCString::createWithFormat(particleStr.c_str(),j)->getCString());
                //                particle1->setPosition(pos + addPt + ccp(20 - rand() % 40, 20 - rand() % 40));
                particle1->setPosition(pos + addPt );
                addParticleToBatch(particle1,getBatchTag(type, index));
                m_cityItem[index].push_back(particle1);
            }
            for (int j=6; j<=8; j++) {
                auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Collapse_%d",j)->getCString());
                //                particle1->setPosition(pos + addPt + ccp(60 - rand() % 120, 60 - rand() % 80));
                particle1->setPosition(pos + addPt);
                addParticleToBatch(particle1,getBatchTag(type, index));
                m_cityItem[index].push_back(particle1);
            }
            
        }
            break;
        case EdgyTag1:
        case EdgyTag:{
            CCPoint pt = WorldController::getInstance()->getPointByIndex(index);
            item = CCLoadSprite::createSprite("edge_light_0.png");
            int dy = 0;
            int dx = 0;
            float s = 1.0f;
            item->setScale(s);
            if((pt.x == 0 || pt.x == WorldController::getInstance()->_current_tile_count_x - 1) && type != EdgyTag1){
                item->setSkewY(26.5);
                item->setAnchorPoint(CCPoint(0, 0));
                if(pt.x == 0){
                    dx = -_tile_width / 2;
                }else if(pt.x == WorldController::getInstance()->_current_tile_count_x - 1){
                    dy = -_tile_height / 2;
                }
            }else{
                item->setSkewY(-26.5);
                item->setAnchorPoint(CCPoint(1, 0));
                if(pt.y == 0){
                    dx = _tile_width / 2;
                }else if(pt.y == WorldController::getInstance()->_current_tile_count_y - 1){
                    dy = -_tile_height / 2;
                }
            }
            item->setPosition(pos + ccp(dx, dy));
            auto ani = createMarchAni(AniEdgy, 0, 1);
            if (ani) {
                item->runAction(ani);
            }
        }
            break;
        case WorldBorder:
        case WorldBorder1:{
            CCPoint pt = WorldController::getInstance()->getPointByIndex(index);
            CCPoint tempPoint = pt;
            item = CCLoadSprite::createSprite("edge_light_0.png");
            int dy = 0;
            int dx = 0;
            float s = 1.0f;
            item->setScale(s);
            if((pt.x == 0 || pt.x == WorldController::getInstance()->_current_tile_count_x - 1) && type != WorldBorder1){
                item->setSkewY(26.5);
                item->setAnchorPoint(CCPoint(0, 0));
                if(pt.x == 0){
                    dx = -_tile_width / 2;
                    tempPoint.x = -3;
                }else if(pt.x == WorldController::getInstance()->_current_tile_count_x - 1){
                    dy = -_tile_height / 2;
                    tempPoint.x = WorldController::getInstance()->_current_tile_count_x - 1 + 3;
                }
            }else{
                item->setSkewY(-26.5);
                item->setAnchorPoint(CCPoint(1, 0));
                if(pt.y == 0){
                    dx = _tile_width / 2;
                    tempPoint.y = -3;
                }else if(pt.y == WorldController::getInstance()->_current_tile_count_y - 1){
                    dy = -_tile_height / 2;
                    tempPoint.y = WorldController::getInstance()->_current_tile_count_x - 1 + 3;
                }
            }
            
            pos = m_map->getViewPointByTilePoint(tempPoint,info.tileServerId);
            item->setPosition(pos + ccp(dx, dy));
            auto ani = createMarchAni(AniEdgy, 0, 1);
            if (ani) {
                item->runAction(ani);
            }
        }
            break;

// move to NBWorldMonster
//        case MonsterDead:
//        case MonsterBreath:
//        case MonsterAttack:
//        {
//            NBWorldMonster::createMonsterBatchItem(type, index);
//            return;
//        }
//            break;
            
        case CSProtectTag:{
            item = CCLoadSprite::createSprite("cs_protect.png");
            item->setScale(0.7);
            int addY = _tile_height + 20;
            if(info.cityType == Throne){
                addBatchItem(ThroneProtectParticle, index);
                item->setScale(2.0);
                addY += 40;
            }
            item->setPosition(pos + ccp(0, addY));
            
            auto particle = ParticleController::createParticle("Collection_Out_2");
            particle->setPosition(pos + ccp(0, addY) + ccp(0, 30));
            addParticleToBatch(particle,getBatchTag(CSProtectTag, index));
            m_cityItem[index].push_back(particle);
            
            particle = ParticleController::createParticle("Protect_CS_1");
            particle->setPosition(pos + ccp(0, addY) + ccp(-20, 50));
            addParticleToBatch(particle,getBatchTag(CSProtectTag, index));
            m_cityItem[index].push_back(particle);
            
            particle = ParticleController::createParticle("Protect_CS_2");
            particle->setPosition(pos + ccp(0, addY) + ccp(-30, 0));
            addParticleToBatch(particle,getBatchTag(CSProtectTag, index));
            m_cityItem[index].push_back(particle);
            
            particle = ParticleController::createParticle("Protect_CS_3");
            particle->setPosition(pos + ccp(0, addY) + ccp(0, 0));
            addParticleToBatch(particle,getBatchTag(CSProtectTag, index));
            m_cityItem[index].push_back(particle);
            
            particle = ParticleController::createParticle("Protect_CS_2");
            particle->setPosition(pos + ccp(0, addY) + ccp(8, 12));
            addParticleToBatch(particle,getBatchTag(CSProtectTag, index));
            m_cityItem[index].push_back(particle);
            
        }
            break;
//        case TreasureMapCDIcon:{
//            item = CCLoadSprite::createSprite("treasure_icon.png");
//            item->setPosition(pos + ccp(0, _tile_height*0.7));
//            break;
//        }
        case TreasureMapCDBarBG:{
            item = CCLoadSprite::createSprite("treasure_jindutiaodi.png");
            item->setPosition(pos - ccp(0,_tile_height*0.5));
            break;
        }
//        case TreasureMapCDBar:{
//            item = CCLoadSprite::createSprite("treasure_jindutiao.png");
//            item->setAnchorPoint(ccp(0,0.5));
//            item->setPosition(pos - ccp(item->getContentSize().width*0.5, _tile_height*0.5));
//            if(obj && dynamic_cast<CCString*>(obj)){
//                string timeStr = dynamic_cast<CCString*>(obj)->getCString();
//                if(timeStr.find("_")!=string::npos){
//                    vector<string> vTmp;
//                    CCCommonUtils::splitString(timeStr, "_", vTmp);
//                    if(vTmp.size()>1){
//                        int leftT = atoi(vTmp.at(0).c_str());
//                        int totalT = atoi(vTmp.at(1).c_str());
//                        float fScaleX = (totalT - leftT) * 1.0 / totalT;
//                        item->setScaleX(fScaleX);
//                        item->runAction(CCSequence::create(CCScaleTo::create(leftT, 1.0),NULL));
//                    }
//                }
//            }
//            break;
//        }
        default:
            return;
    }
    if (item) {
        item->setTag(getBatchTag(type, index));
        m_cityItem[index].push_back(item);
        m_batchNode->addChild(item, zorder);
    }
}

void WorldMapView::removeWalkParticle(int tag){
    auto vec = m_walkparticleVec.find(tag);
    if (vec != m_walkparticleVec.end()) {
        for (auto &particle:vec->second) {
            particle->removeFromParent();
        }
        m_walkparticleVec.erase(vec);
    }
    return;
}

void WorldMapView::delBatchItem(BatchTagType type, unsigned int index) {
    // remove particle
    if (type == FireTag || type == SmokeTag || type == CityMoveInTag || type == CityMoveOutTag || type == CityMoveInViewTag || type == Rock || type == Rock1 || type == Rock2 || type == Rock3 || type == WarFire || type == OfficerTagParticle || type == Partical_fieldMonster || type == AllianceTerritoryParticle|| type == Partical_mapMask) {
        auto vec = m_particleVec.find(getBatchTag(type, index));
        if (vec != m_particleVec.end()) {
            for (auto &particle:vec->second) {
                particle->removeFromParent();
            }
            m_particleVec.erase(vec);
        }
        return;
    }
    
    if (type == AllianceParticle) {
        auto vec = m_flagParticleVec.find(getBatchTag(type, index));
        if (vec != m_flagParticleVec.end()) {
            for (auto &particle:vec->second) {
                particle->removeFromParent();
            }
            m_flagParticleVec.erase(vec);
        }
        return;
    }
    
    auto item = m_batchNode->getChildByTag(getBatchTag(type, index));
    if (item) {
        item->removeFromParentAndCleanup(true);
    }
    auto monster = m_mapMonsterNode->getChildByTag(getBatchTag(type, index));
    if (monster) {
        monster->removeFromParentAndCleanup(true);
    }
    // guo.jiang
    monster = m_mapMonstersNode->getChildByTag(getBatchTag(type, index));
    if (monster) {
        monster->removeFromParentAndCleanup(true);
    }
}

std::map<int, unsigned int> WorldMapView::getCurrentTile(CCPoint &pt, int rowNum, int colNum, int tileX){
    std::map<int, unsigned int> map;
    int col = pt.x;
    int row = pt.y;
    int designHeightRow = rowNum;
    int designWidthCol = colNum;
    
    auto DoAddAtlas = [&](int i,int j, int tileX) {
        if (i >= 0 && i <  WorldController::getInstance()->_current_tile_count_x && j >= 0 && j < WorldController::getInstance()->_current_tile_count_y) {
            int globalIndex = i + j * tileX;
            map.insert(std::pair<int,unsigned int>((int)(1 + globalIndex), 1));
        }
    };
    
    int value1 = designWidthCol/2 + designHeightRow/2;
    int value2 = designHeightRow/2 - designWidthCol/2;
    for (int i=col-value1; i<col+value1; i++) {
        int startY = 0;
        int countY = 0;
        int thisRow = i - col + value1;
        if (thisRow < designWidthCol) {
            startY = row - value2 - 1 - thisRow;
            countY = (thisRow + 1)*2;
        } else if (thisRow < designHeightRow) {
            startY = row - value1 + thisRow - designWidthCol;
            countY = 2*designWidthCol;
        } else {
            startY = row - value1 + thisRow - designWidthCol;
            countY = (designWidthCol + designHeightRow - thisRow)*2;
        }
        for (int j=startY;j<startY+countY;j++) {
            DoAddAtlas(i,j, tileX);
        }
    }
    
    return map;
}

CCPoint WorldMapView::getMarchPoint(MarchInfo &info){
    auto node = m_mapMarchNode->getChildByTag(info.marchTag);
    if(node == NULL){
        node = m_mapMarchNode1->getChildByTag(info.marchTag);
    }
    if(node == NULL){
        node = m_cityAttackNode->getChildByTag(info.marchTag);
    }
    if(node == NULL){
        node = m_mapHeiqishiMarchNode->getChildByTag(info.marchTag);
    }
    
    if(node){
        return node->getPosition();
    }
    return ccp(-1, -1);
}

void WorldMapView::addFlagPar(std::string type, const CCPoint &pt, int index){
    
    if( !this->m_isIdle )
    {
        FlagParInfo fpInfo;
        fpInfo.type = type;
        fpInfo.pt = pt;
        fpInfo.index = index;
        m_flagParDatas.push_back(fpInfo);
        return;
    }

    
    
    auto addPar = [](std::string nameStr, const CCPoint &pt, map<unsigned int , vector<CCSafeObject<CCNode> > >  &map, int index){
        auto particle = ParticleController::createParticle(nameStr);
        particle->setPosition(pt * 1.0f / FLAG_SCALE);
        particle->setScale(FLAG_SCALE);
        map[index].push_back(particle);
        return particle;
    };
    
    auto parPt = ccp(pt.x, pt.y);
    float s = FLAG_SCALE;
    if(type == "Blood"){
        addFlagParticleToBatch(addPar("FlagBlood_1", parPt + ccp(-18, -7) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagBlood_2", parPt + ccp(-18, -7) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagBlood_3", parPt + ccp(-20, -9) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagBlood_3", parPt + ccp(20, -9) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
    
    if(type == "Snow"){
        addFlagParticleToBatch(addPar("FlagSnow_1", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagSnow_2", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagSnow_3", parPt + ccp(0, 30) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagSnow_4", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagSnow_5", parPt + ccp(0, -50) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
    if(type == "Warhammer"){
        addFlagParticleToBatch(addPar("FlagWarhammer_1", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagWarhammer_2", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagWarhammer_3", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagWarhammer_4", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagWarhammer_5", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagWarhammer_6", parPt + ccp(0, -50) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
    if(type == "Tree"){
        addFlagParticleToBatch(addPar("FlagTree_1", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagTree_2", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagTree_3", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagTree_4", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
    if(type == "Cure"){
        addFlagParticleToBatch(addPar("FlagCure_1", parPt + ccp(25, 37) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagCure_2", parPt + ccp(-25, 37) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagCure_3", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagCure_4", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagCure_5", parPt + ccp(0, 40) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
    if(type == "Death"){
        addFlagParticleToBatch(addPar("FlagDeath_1", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagDeath_2", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagDeath_3", parPt + ccp(-5, -18) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagDeath_4", parPt + ccp(12, 2) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagDeath_4", parPt + ccp(-17, 2) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
    if(type == "Fire"){
        addFlagParticleToBatch(addPar("FlagFire_1", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagFire_2", parPt + ccp(23, -5) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagFire_3", parPt + ccp(-23, -5) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
    if(type == "Ocean"){
        addFlagParticleToBatch(addPar("FlagOcean_1", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagOcean_2", parPt + ccp(0, -35) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagOcean_3", parPt + ccp(0, -35) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
    if(type == "Sun"){
        addFlagParticleToBatch(addPar("FlagSun_1", parPt + ccp(0, 30) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagSun_2", parPt + ccp(0, 16) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagSun_3", parPt + ccp(0, 16) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagSun_4", parPt + ccp(0, 16) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
    if(type == "Wolf"){
        addFlagParticleToBatch(addPar("FlagWolf_1", parPt + ccp(0, 30) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagWolf_2", parPt + ccp(6, 18) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
    if(type == "Angel"){
        addFlagParticleToBatch(addPar("FlagAngel_1", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagAngel_2", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagAngel_3", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagAngel_4", parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
    if(type == "Cattle"){
        for (int i=1; i<6; i++) {
            addFlagParticleToBatch(addPar(CCString::createWithFormat("FlagCattle_%d",i)->getCString(), parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        }
    }
    if(type == "FireWolf"){
        addFlagParticleToBatch(addPar("FireWolf_1", parPt + ccp(-6, 18) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FireWolf_2", parPt + ccp(-12, 22) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FireWolf_3", parPt + ccp(6, 20) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
    if(type == "Grail"){
        for (int i=1; i<5; i++) {
            addFlagParticleToBatch(addPar(CCString::createWithFormat("FlagGrail_%d",i)->getCString(), parPt + ccp(0, 0) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        }
    }
    if(type == "Knight"){
        addFlagParticleToBatch(addPar("FlagDeath_4", parPt + ccp(3, 17) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagDeath_4", parPt + ccp(-6, 21) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagKnight_R", parPt + ccp(48, -17) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagKnight_L", parPt + ccp(-50, -15) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagKnight_1", parPt + ccp(0, 18) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagKnight_2", parPt + ccp(0, -32) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagKnight_3", parPt + ccp(0, 91) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagKnight_4", parPt + ccp(-3, -40) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagKnight_4", parPt + ccp(-40, -60) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagKnight_4", parPt + ccp(-20, -54) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
        addFlagParticleToBatch(addPar("FlagKnight_4", parPt + ccp(45, -40) * s, m_cityItem, index),getBatchTag(AllianceParticle, index));
    }
}

void WorldMapView::showAndHideUnBatchLabel(){
    
    auto showAndHideNode = [](CCNode* parentNode){
        int index = 0;
        auto arr = parentNode->getChildren();
        while (index < arr.size()) {
            CCNode *childNode = dynamic_cast<CCNode*>(arr.at(index));
            if(childNode && childNode->getParent()){
                CCPoint p = childNode->getPosition();
                CCPoint p2 = childNode->getParent()->convertToWorldSpace(p);
                auto size = CCDirector::sharedDirector()->getWinSize();
                if(p2.x < -_tile_width || p2.x > size.width + _tile_width || p2.y < -_tile_height || p2.y > size.height + _tile_height){
                    childNode->setVisible(false);
                }else{
                    childNode->setVisible(true);
                }
            }
            index++;
        }
        
    };
    showAndHideNode(m_aAreaAniNode);
}
void WorldMapView::onShowAndHideSpinLb(){
    auto showAndHideNode = [](CCNode* parentNode){
        int index = 0;
        auto arr = parentNode->getChildren();
        while (index < arr.size()) {
            CCLabelIFTTF *ttf = dynamic_cast<CCLabelIFTTF*>(arr.at(index));
            if(ttf && ttf->getParent()){
                CCPoint p = ttf->getPosition();
                CCPoint p2 = ttf->getParent()->convertToWorldSpace(p);
                auto size = CCDirector::sharedDirector()->getWinSize();
                if(p2.x < -_tile_width || p2.x > size.width + _tile_width || p2.y < -_tile_height || p2.y > size.height + _tile_height){
                    ttf->setVisible(false);
                }else{
                    ttf->setVisible(true);
                }
            }
            index++;
        }
        
    };
    showAndHideNode(m_unBatchLabelNode);
    showAndHideNode(m_spinLabelNode);
    showAndHideNode(m_simNameUnbatchNode);
}
void WorldMapView::onShowAndHideCloud(){
    CCNode * tempNode = WorldMapView::instance()->m_mapMaskCloudNode->getChildByTag(Partical_mapMask);
    auto & childrenArr = tempNode->getChildren();
    CCObject* elem = NULL;
    for (auto child : childrenArr)
    {
        CCNode* childNode = dynamic_cast<CCNode*>(child);
        CCPoint childPios = childNode->getPosition();
        int tempServerId = WorldMapView::instance()->m_map->getServerIdByViewPoint(childPios + tempNode->getPosition());
        if (tempServerId != GlobalData::shared()->playerInfo.selfServerId) {
            childNode->setVisible(true);
        }
        else{
            childNode->setVisible(false);
        }
    }
}
void WorldMapView::showAndHideFieldMonster(){return;
//    int index = 0;
//    auto arr = m_mapMonsterNode->getChildren();
//    while (index < arr.size())
//    {
//        IFFieldMonsterNode* monsterNode = dynamic_cast<IFFieldMonsterNode*>(arr.at(index));
//        if(monsterNode && monsterNode->getParent())
//        {
//            CCPoint p = monsterNode->getPosition();
//            CCPoint p2 = monsterNode->getParent()->convertToWorldSpace(p);
//            auto size = CCDirector::sharedDirector()->getWinSize();
//            if(p2.x < -_tile_width || p2.x > size.width + _tile_width || p2.y < -_tile_height || p2.y > size.height + _tile_height){
//                monsterNode->setVisible(false);
//            }else{
//                monsterNode->setVisible(true);
//            }
//            //            m_monsterRect.origin.x = monsterNode->getPositionX()-_tile_width/2;
//            //            m_monsterRect.origin.y = monsterNode->getPositionY()-_tile_height/2;
//            //            m_monsterRect.size = CCSize(_tile_width, _tile_height*2);
//            //            if(m_viewPort.intersectsRect(m_monsterRect))
//            //            {
//            //                monsterNode->setVisible(true);
//            //            }
//            //            else
//            //            {
//            //                monsterNode->setVisible(false);
//            //            }
//        }
//        index++;
//    }
}

CCArray *WorldMapView::getCityPicArr(int addIndex, int level, bool isKing ,int nSpecialId, const Vec2& pos){
    int id = 44100 - 1 + level;
    if(isKing){
        id = 44999;
    }
    if (nSpecialId != -1) {
        id = nSpecialId;
    }
    int mainCityIndex = atoi(CCCommonUtils::getPropById(CC_ITOA(id), "Basics").c_str());
    CCArray *arr = CCArray::create();
    int startBaseIndex = 41;
    int defaultStartBaseIndex = 41;
    int baseNumPerCity = 4;
    string isNewStyle = CCCommonUtils::getPropById(CC_ITOA(id), "isNewStyle");
    if(mainCityIndex != -1){
        std::string addPic = CCCommonUtils::getPropById(CC_ITOA(id), "addPic");
        int startIndex = startBaseIndex + mainCityIndex * baseNumPerCity;
        auto dict = CCDictionary::create();
        dict->setObject(CCString::createWithFormat("%d.png", startIndex + addIndex), "pic");
        dict->setObject(CCString::create("0"), "x");
        dict->setObject(CCString::create("0"), "y");
        if (isNewStyle == "0" || (isNewStyle == "" && nSpecialId == -1)) {
            arr->addObject(dict);
        }
        //添加附加图，全部添加到下面的那个格子。
        
//        log("****** %d.png (%d, %d)", startIndex + addIndex, 0, 0);
        
        if(addPic != "" && addIndex == 0){
            std::vector<string> addtionalVector;
            CCCommonUtils::splitString(addPic, "|", addtionalVector);
            int addIndex = 0;
            while (addIndex < addtionalVector.size()) {
                std::string picStr = addtionalVector[addIndex];
                std::vector<std::string> picVector;
                CCCommonUtils::splitString(picStr, ":", picVector);
                int indexPic = atoi(picVector[0].c_str());
                
                std::string posStr = picVector[1];
                std::vector<std::string> posVector;
                CCCommonUtils::splitString(posStr, ",", posVector);
                int x = atoi(posVector[0].c_str()) + _tile_width / 2;
                int y = atoi(posVector[1].c_str()) + _tile_height;
                CCDictionary *addDict = CCDictionary::create();
                addDict->setObject(CCString::createWithFormat("%d.png", indexPic), "pic");
                addDict->setObject(CCString::create(CC_ITOA(x)), "x");
                addDict->setObject(CCString::create(CC_ITOA(y)), "y");
                arr->addObject(addDict);
                addIndex++;
                
//                log("       %d.png (%d, %d)", indexPic, x, y);
            }
        }
    }
    if(arr->count() == 0 && isNewStyle == "0"){
        auto dict = CCDictionary::create();
        dict->setObject(CCString::createWithFormat("%d.png", defaultStartBaseIndex + addIndex), "pic");
        dict->setObject(CCString::create("0"), "x");
        dict->setObject(CCString::create("0"), "y");
        arr->addObject(dict);
    }
    
    return arr;
}

// item_worldcastle.xml
CCArray *WorldMapView::getCityPicArr(WorldCityInfo &info, int level,int nSpecialId, const Vec2& pos){
    int addIndex = 0;
    if(info.cityIndex == info.parentCityIndex - 1){
        addIndex = 1;
    }else if(info.cityIndex == info.parentCityIndex - 1 - WorldController::getInstance()->_current_tile_count_x){
        addIndex = 2;
    }else if(info.cityIndex == info.parentCityIndex - WorldController::getInstance()->_current_tile_count_x){
        addIndex = 3;
    }
    bool isKing = false;
    auto playerInfo = WorldController::getInstance()->m_playerInfo.find(info.playerName);
    if(playerInfo != WorldController::getInstance()->m_playerInfo.end() && playerInfo->second.officer == KINGDOM_KING_ID){
        isKing = true;
    }
    return getCityPicArr(addIndex, level, isKing ,nSpecialId, pos);
}
void WorldMapView::changeServer(int serverId){
    GlobalData::shared()->playerInfo.currentServerId = serverId;
    GlobalData::shared()->isPressedHomeKey = false;
    ChangeServerCover *pop = ChangeServerCover::create(GlobalData::shared()->playerInfo.currentServerId);
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
    if (pop && layer) {
        layer->removeChildByTag(WM_COVER_TAG);
        pop->setTag(WM_COVER_TAG);
        layer->addChild(pop);
    }
    needClearAll = true;
    WorldController::getInstance()->getCityInfos().clear();
    if(!GlobalData::shared()->playerInfo.isInSelfServer()){
        UIComponent::getInstance()->setServerText(serverId);
    }
}

void WorldMapView::trebuchetAttack(CCDictionary *params){
    int pointId = params->valueForKey("pointId")->intValue();
    auto it = m_cityInfo.find(pointId);
    if(it != m_cityInfo.end()){
        it->second.trebuchetInfo.state = Attack;
        createCity(it->second);
    }
}

void WorldMapView::resetTrebuchet(CCObject *obj){
    CCInteger *integer = dynamic_cast<CCInteger*>(obj);
    int pointId = integer->getValue();
    auto it = m_cityInfo.find(pointId);
    if(it != m_cityInfo.end()){
        it->second.trebuchetInfo.state = Wait;
        createCity(it->second);
    }
}

void WorldMapView::playFreshResAni(int index){
    CCNode* node = CCNode::create();
    m_layers[WM_ROAD]->addChild(node, roadIndex);
    string tmpUIStart = "CityMoveUI_";
    int uiCount = 4;
    for (int i=1; i<=uiCount; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpUIStart.c_str(),i)->getCString());
        particle->setPosition(m_map->getViewPointByTilePoint(WorldController::getPointByIndex(index)) + ccp(0, _tile_height / 2));
        node->addChild(particle);
    }
    node->runAction(CCSequence::create(CCDelayTime::create(1.8),CCRemoveSelf::create(),NULL));
}

void WorldMapView::loadThroneResource(int index){
    if(index == DEFAULT_THRONE_INDEX){
        if(currentThroneIndex != 0){
            CCLoadSprite::doResourceByCommonIndex(currentThroneIndex, false);
        }
        currentThroneIndex = index;
        CCLoadSprite::doResourceByCommonIndex(currentThroneIndex, true);
    }else{
        std::string icon = CCString::createWithFormat("Common_%d", currentThroneIndex)->getCString();
        if(currentThroneIndex != 0){
            DynamicResourceController::getInstance()->loadNameTypeResource(icon, true);
        }
        currentThroneIndex = index;
        icon = CCString::createWithFormat("Common_%d", currentThroneIndex)->getCString();
        DynamicResourceController::getInstance()->loadNameTypeResource(icon, false);
    }
}

int WorldMapView::getThroneIndex(int picId){
    int defaultIndex = DEFAULT_THRONE_INDEX;
    if(picId == 0){
        return defaultIndex;
    }
    if(ThronePos.find(picId) == ThronePos.end()){
        return defaultIndex;
    }
    std::string file = CCString::createWithFormat("Common_%d", picId)->getCString();
    std::string fileName = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder + file + ".plist";
    if(!CCFileUtils::sharedFileUtils()->isFileExist(fileName)){
        return defaultIndex;
    }
    return picId;
}
void WorldMapView::onShowAAreaLb(bool visiable){
    auto arr = m_aAreaUnbachLbNode->getChildren();
    int index = 0;
    while (index < arr.size()) {
        CCNode *lbParentNode = dynamic_cast<CCNode*>(arr.at(index));
        if(lbParentNode && lbParentNode->getParent()){
            if (!visiable) {
                lbParentNode->setVisible(visiable);
            }
            else{
                CCPoint viewPoint = lbParentNode->getParent()->convertToWorldSpace(lbParentNode->getPosition());
                CCSize winSize = CCDirector::sharedDirector()->getWinSize();
                if (viewPoint.x < -_tile_width || viewPoint.x > winSize.width + _tile_width || viewPoint.y > winSize.height + _tile_height || viewPoint.y < -_tile_height) {
                    lbParentNode->setVisible(false);
                }
                else{
                    lbParentNode->setVisible(true);
                }
            }
        }
        index++;
    }
}
CCPoint WorldMapView::getViewCenterTilePoint(){
    float fmapScale = m_map->getScale();
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    CCPoint   centerPt = (m_map->getPosition() - ccp(winsize.width/2,winsize.height/2)) / fmapScale;
    CCPoint mapPt1 = CCPointZero;
    mapPt1 = WorldMapView::instance()->m_map->getTilePointByViewPoint(-centerPt);
    int logx = mapPt1.x;
    int logy = mapPt1.y;
    CCLOG("loginfo=====centePoint  %d,%d",logx,logy);
    return mapPt1;
}
void WorldMapView::onSAHaAreaParcital(CCObject* pObj){
    int index = dynamic_cast<CCInteger*>(pObj)->getValue();
    int vecTag =  getBatchTag(Partical_AAreaBuild, index);
    auto vec = m_particleVec[vecTag];
    int vecIndex = 0;
    
    while (vecIndex < vec.size()) {
        bool isActive = vec.at(vecIndex)->isActive();
        if (isActive) {
            vec.at(vecIndex)->stopSystem();
        }
        else{
            vec.at(vecIndex)->resetSystem();
        }
        
        vecIndex++;
    }
}
void WorldMapView::initTowerRange(int tileIndex,bool  isInRange){
    auto tileInfo = m_cityInfo[tileIndex];
    int range = WorldController::getInstance()->m_aTerritory11;//5
    AAreaState state = m_cityInfo[tileIndex].m_towerInfo.state;
    CCPoint tilePoint = WorldController::getPointByIndex(tileIndex);
    
    int radius = range / 2;
    radius = radius * 2;
    int startX = tilePoint.x - radius;
    int startY = tilePoint.y - radius;
    int endX = tilePoint.x + radius;
    int endY = tilePoint.y + radius;
    
    int indexX = startX;
    while (indexX <= endX) {
        int indexY = startY;
        while (indexY <= endY) {
            int tempTilePoint = WorldController::getIndexByPoint(ccp(indexX,indexY));
            //CCLOG("addIndex0  :%d,%d",indexX,indexY);
            m_cityInfo[tempTilePoint].m_isInTowerRange = isInRange;
            if (isInRange) {
                m_cityInfo[tempTilePoint].m_bigRangeAid = tileInfo.m_aArea_id;
            }
            else{
                m_cityInfo[tempTilePoint].m_bigRangeAid = "";
            }
            
            indexY++;
        }
        indexX++;
    }
    
    int radius2 = radius - 2;
    int startX2 = tilePoint.x - radius2;
    int startY2 = tilePoint.y - radius2;
    int endX2 = tilePoint.x + radius2;
    int endY2 = tilePoint.y + radius2;
    
    int indexX2 = startX2;
    while (indexX2 <= endX2) {
        int indexY2 = startY2;
        while (indexY2 <= endY2) {
            int tempTilePoint = WorldController::getIndexByPoint(ccp(indexX2,indexY2));
            //CCLOG("addIndex-- :%d,%d",indexX2,indexY2);
            int tempIndex = m_cityInfo[tempTilePoint].m_parentTowerIndex;
            if (isInRange) {
                if (tempIndex == -1) {
                    m_cityInfo[tempTilePoint].m_parentTowerIndex = tileIndex;
                }else{
                    m_cityInfo[tempTilePoint].m_parentTowerIndex2 = tileIndex;
                    if (m_cityInfo[tempTilePoint].m_parentTowerIndex2 == m_cityInfo[tempTilePoint].m_parentTowerIndex) {
                        m_cityInfo[tempTilePoint].m_parentTowerIndex2 = -1;
                    }
                }
                m_cityInfo[tempTilePoint].m_isInTowerAttackRange = true;
            }
            else{
                m_cityInfo[tempTilePoint].m_parentTowerIndex = - 1;
                m_cityInfo[tempTilePoint].m_parentTowerIndex2 = - 1;
                m_cityInfo[tempTilePoint].m_isInTowerAttackRange = false;
            }
            
            indexY2++;
        }
        indexX2++;
    }
    
    
}
void WorldMapView::resetAAreaInfo(int tileIndex,int range){
    CCPoint tilePoint = WorldController::getPointByIndex(tileIndex);
    bool isOdd = true;
    if (range % 2 == 0) {
        isOdd = false;
    }
    int radius = range / 2;
    int startX = tilePoint.x - radius;
    int startY = tilePoint.y - radius;
    int endX = tilePoint.x + radius;
    int endY = tilePoint.y + radius;
    if (!isOdd) {
        endX = endX - 1;
        endY = endY - 1;
    }
    int indexX = startX;
    while (indexX <= endX) {
        int indexY = startY;
        while (indexY <= endY) {
            int tempTilePoint = WorldController::getIndexByPoint(ccp(indexX,indexY));
            m_cityInfo[tempTilePoint].m_aArea_direction = "";
            m_cityInfo[tempTilePoint].m_isInAArea = false;
            m_cityInfo[tempTilePoint].m_forceActive = false;
            m_cityInfo[tempTilePoint].m_aArea_id = "";
            m_cityInfo[tempTilePoint].m_isMyAArea = false;
            if(m_aAreaUnbachLbNode->getChildByTag(tempTilePoint)){
                CCNode* tempNode = m_aAreaUnbachLbNode->getChildByTag(tempTilePoint);
                tempNode->removeAllChildrenWithCleanup(true);
                m_aAreaUnbachLbNode->removeChildByTag(tempTilePoint);
            }
            if (m_conflictBatchNode->getChildByTag(tempTilePoint)) {
                m_conflictBatchNode->removeChildByTag(tempTilePoint);
            }
            indexY++;
        }
        indexX++;
    }
}
void WorldMapView::initAAreaBlankInfo(int tileIndex,int range,bool _isMyalliance){
    static int count = 0;
    AAreaState state = m_cityInfo[tileIndex].m_allianceAreaInfo.state;
    CCPoint tilePoint = WorldController::getPointByIndex(tileIndex);
    bool isOdd = true;
    if (range % 2 == 0) {
        isOdd = false;
    }
    int radius = range / 2;
    int startX = tilePoint.x - radius;
    int startY = tilePoint.y - radius;
    int endX = tilePoint.x + radius;
    int endY = tilePoint.y + radius;
    if (!isOdd) {
        endX = endX - 1;
        endY = endY - 1;
    }
    int indexX = startX;
    while (indexX <= endX) {
        int indexY = startY;
        while (indexY <= endY) {
            int tempTilePoint = WorldController::getIndexByPoint(ccp(indexX,indexY));
            m_cityInfo[tempTilePoint].m_isInAArea = true;
            if(_isMyalliance){
                m_cityInfo[tempTilePoint].m_isMyAArea = true;
            }
            if (state == Astate_Garrison || state == Astate_UnGarrison || state == Astate_Destroy || state == Astate_Damaged || state == Astate_FixIng) {
                m_cityInfo[tempTilePoint].m_forceActive = true;
                //                CCLOG("-----%d,%d",indexX,indexY);
            }
            indexY++;
            count++;
            //            CCLOG("count:%d",count);
            //            CCLOG("-----%d,%d",indexX,indexY);
        }
        indexX++;
    }
}
void WorldMapView::onRefreshAAreaBuilding(int tileIndex, AAreaState state,bool isSuperMine /* false*/,bool isTower/* = false*/,bool isWarehouse/* false*/,bool isBanner/*false*/){
    auto cityInfo = m_cityInfo.find(tileIndex);
    if (cityInfo != m_cityInfo.end()) {
        if(isSuperMine){
            cityInfo->second.m_superMineInfo.trstat = state;
        }else if(isTower){
            cityInfo->second.m_towerInfo.state = state;
        }else if (isWarehouse){
            cityInfo->second.m_warehouseInfo.state = state;
        }else if (isBanner){
            cityInfo->second.m_bannerInfo.state = state;
        }
        else{
            cityInfo->second.m_allianceAreaInfo.state = state;
        }
        delBatchItem(NameTag, tileIndex);
        delBatchItem(AllianceFlag, tileIndex);
        createCity(cityInfo->second);
    }
}
int WorldMapView::getPicIdByCityInfo(WorldCityInfo &info){
    int defaultId = 44996;
    AAreaState tempState = info.m_allianceAreaInfo.state;
    if (info.cityType == tile_superMine) {
        tempState = info.m_superMineInfo.trstat;
        WorldResourceType resType = info.m_superMineInfo.type;
        switch (resType) {
            case Wood:
                return 44993;
                break;
            case Food:
                return 44995;
                break;
            case Iron:
                return 44994;
                break;
            case Stone:
                return 44992;
                break;
            default:
                break;
        }
        return 44993;
    }
    else if (info.cityType == tile_wareHouse){
        tempState = info.m_warehouseInfo.state;
        switch (tempState) {
            case Astate_None:
                return 44991;
                break;
            case Astate_BeforeBuild:
                return 44990;
                break;
            case Astate_Building:
                return 44990;
                break;
            case Astate_UnGarrison:
                return 44991;
                break;
            default:
                break;
        }
        return 44991;
    }
    switch (tempState) {
        case Astate_None:
            defaultId =  44996;
            break;
        case Astate_Building:
            defaultId =  44997;
            break;
        case Astate_BeforeBuild:
            defaultId =  44997;
            break;
        case Astate_Garrison:
            defaultId =  44996;
            break;
        case Astate_UnGarrison:
            defaultId =  44996;
            break;
        case Astate_Damaged:
            defaultId =  44998;
            break;
        case Astate_FixIng:
            defaultId =  44998;
            break;
        case Astate_Destroy:
            defaultId =  44998;
            break;
            
        default:
            break;
    }
    return defaultId;
}
void WorldMapView::hideSameBoard(int cityIndex){
    auto &info  = m_cityInfo[cityIndex];
    
    int left = cityIndex - 1;
    int right = cityIndex + 1;
    int top = cityIndex - WorldController::getInstance()->_current_tile_count_x;
    int bottom = cityIndex + WorldController::getInstance()->_current_tile_count_x;
    
    auto &tempInfo_l = m_cityInfo[left];
    auto &tempInfo_r = m_cityInfo[right];
    auto &tempInfo_t = m_cityInfo[top];
    auto &tempInfo_b = m_cityInfo[bottom];
    
    string tempDirectionStr = info.m_aArea_direction;
    
    if (tempInfo_l.m_aArea_id == info.m_aArea_id) {
        string tempDirStr = info.m_aArea_direction;
        if (tempDirStr.find("l") != string::npos) {
            info.m_aArea_direction.replace(tempDirStr.find("l"), 1, "z");
            //            info.m_aArea_direction.erase(tempDirStr.find("l"), 1);
        }
    }
    if (tempInfo_r.m_aArea_id == info.m_aArea_id) {
        string tempDirStr = info.m_aArea_direction;
        if (tempDirStr.find("r") != string::npos) {
            info.m_aArea_direction.replace(tempDirStr.find("r"), 1, "y");
            //            info.m_aArea_direction.erase(tempDirStr.find("r"), 1);
        }
    }
    if (tempInfo_t.m_aArea_id == info.m_aArea_id) {
        string tempDirStr = info.m_aArea_direction;
        if (tempDirStr.find("t") != string::npos) {
            info.m_aArea_direction.replace(tempDirStr.find("t"), 1, "s");
            //            info.m_aArea_direction.erase(tempDirStr.find("t"), 1);
        }
    }
    if (tempInfo_b.m_aArea_id == info.m_aArea_id) {
        string tempDirStr = info.m_aArea_direction;
        if (tempDirStr.find("b") != string::npos) {
            info.m_aArea_direction.replace(tempDirStr.find("b"), 1, "x");
            //            info.m_aArea_direction.erase(tempDirStr.find("b"), 1);
        }
    }
}
string WorldMapView::getResNameByType(WorldResourceType type){
    string sName("");
    switch (type) {
        case Food:
            sName=_lang("115376");
            break;
        case Wood:
            sName=_lang("115377");
            break;
        case Iron:
            sName=_lang("115378");
            break;
        case Silver:
            sName=_lang("115379");
            break;
            
        default:
            break;
    }
    return sName;
}
void WorldMapView::onTowerAttack(CCNode* marchNode ,MarchInfo& info){
    if(info.marchType ==  MethodHeiqishi || MethodScout == info.marchType || MethodYuanSolider == info.marchType || MethodUnion == info.marchType){
        return;
    }
    CCPoint currentPoint = marchNode->getPosition();
    CCPoint inScenePoint = currentPoint + m_map->getPosition();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    bool isInWinSize = false;
    if (inScenePoint.x >= -_tile_width && inScenePoint.x < winSize.width + _tile_width  && inScenePoint.y > -_tile_height && inScenePoint.y < winSize.height + _tile_height) {
        isInWinSize = true;
    }
    if (!isInWinSize) {
        return;
    }
    CCPoint tilePoint =  m_map->getTilePointByViewPoint(currentPoint);
    int cityindex = WorldController::getInstance()->getIndexByPoint(tilePoint);
    //    auto cityInfo = NULL;
    if (m_cityInfo.find(cityindex) != m_cityInfo.end()) {
        auto cityInfo = m_cityInfo[cityindex];
        if (info.asn == m_cityInfo[cityInfo.m_parentTowerIndex].m_aArea_simpleName && cityInfo.m_parentTowerIndex2 == -1) {//发兵方
            return;
        }
        if (info.stateType == StateReturn) {
            return;
        }
        if (isInWinSize
            && cityInfo.m_parentTowerIndex != -1
            && cityInfo.m_isInTowerAttackRange//在射程内
            && m_cityInfo[cityInfo.m_parentTowerIndex].m_towerInfo.state == Astate_UnGarrison//状态必须为建成
            && GlobalData::shared()->getTimeStamp() % 2 == 0//每两秒
            ) {
            string aaid = info.marchToIndexLeague;
            if (cityInfo.m_parentTowerIndex2!= -1) {
                if (aaid == m_cityInfo[cityInfo.m_parentTowerIndex2].m_aArea_id && aaid != "") {
                    int index = 0;
                    while(index < 2){
                        onTowerAttackAnimation(cityInfo.m_parentTowerIndex2,marchNode->getPosition(),info.marchTag);
                        index++;
                    }
                }
                else if(aaid == m_cityInfo[cityInfo.m_parentTowerIndex].m_aArea_id && aaid != ""){
                    int index = 0;
                    while(index < 2){
                        onTowerAttackAnimation(cityInfo.m_parentTowerIndex,marchNode->getPosition(),info.marchTag);
                        index++;
                    }
                }
            }else if(aaid == m_cityInfo[cityInfo.m_parentTowerIndex].m_aArea_id && m_cityInfo[cityInfo.m_parentTowerIndex].m_aArea_id != ""){
                int index = 0;
                while(index < 2){
                    onTowerAttackAnimation(cityInfo.m_parentTowerIndex,marchNode->getPosition(),info.marchTag);
                    index++;
                }
            }
            
        }
    }
}
void WorldMapView::onTowerAttackAnimation(int startCityIndex,CCPoint endViewPos,int marchTag){
    auto addPt = ccp(0, 0);
    CCPoint m_startPoint = (WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(startCityIndex))) - endViewPos
    + ccp(10 - rand() % 20, -rand() % 10) + ccp(132,150) - ccp(_halfTileSize.width, _halfTileSize.height);
    CCPoint m_endPoint = ccp(100 - rand() % 200, 80 - rand() % 160);
    CCSprite* sp1 = NULL;
    CCSpriteBatchNode* towerBatchNode = WorldMapView::instance()->m_mapTowerBatchNode;
    if(towerBatchNode->getChildByTag(marchTag)){
        sp1 = dynamic_cast<CCSprite*>(towerBatchNode->getChildByTag(marchTag));
        sp1->setPosition(endViewPos);
    }
    else{
        sp1 =CCLoadSprite::createSprite("a010_0_N_move_0.png");
        sp1->setTextureRect(CCRectZero);
        sp1->setPosition(endViewPos);
        sp1->setTag(marchTag);
        towerBatchNode->addChild(sp1);
    }
    auto arrow = Arrow::create(sp1);
    arrow->attack(m_startPoint, m_endPoint, 2.1);
}
void WorldMapView::addTowerRange(int tileIndex){
    if (m_towerRangeMaskNode->getChildByTag(tileIndex)) {
        return;
    }
    CCSprite* parentSp = CCLoadSprite::createSprite("ArrowTower_red.png");
    parentSp->setContentSize(CCSizeZero);
    parentSp->setTag(tileIndex);
    m_towerRangeMaskNode->addChild(parentSp);
    parentSp->setVisible(false);
    auto info  = m_cityInfo[tileIndex];
    bool isMyAlliance = false;
    if (info.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
        isMyAlliance = true;
    }
    string spStr = "ArrowTower_red.png";
    if (isMyAlliance) {
        spStr = "ArrowTower_blue.png";
    }
    vector<pair<string,CCPoint>> capacityVec;
    CCPoint centerPoint = WorldController::getPointByIndex(tileIndex);
    int startX = centerPoint.x - 2;
    int startY = centerPoint.y - 2;
    int endX = centerPoint.x + 2;
    int endY = centerPoint.y + 2;
    
    for (int index = startX; index <= endX; index++) {
        capacityVec.insert(capacityVec.begin(), make_pair("t", ccp(index, startY)));
    }
    for (int index = startY; index <= endY; index++) {
        capacityVec.insert(capacityVec.begin(), make_pair("l", ccp(startX, index)));
    }
    for (int index = startX; index <= endX; index++) {
        capacityVec.insert(capacityVec.begin(), make_pair("b", ccp(index, endY)));
    }
    for (int index = startY; index <= endY; index++) {
        capacityVec.insert(capacityVec.begin(), make_pair("r", ccp(endX, index)));
    }
    vector<pair<string,CCPoint>>::iterator it = capacityVec.begin();
    for (; it!= capacityVec.end(); it++) {
        CCSprite* sp = CCLoadSprite::createSprite(spStr.c_str());
        string type = it->first;
        if (type == "l") {
            sp->setScaleX(-1);
        }else if (type == "r"){
            sp->setScaleY(-1);
        }else if (type == "b"){
            sp->setScale(-1);
        }else if (type == "t"){
            
        }
        CCPoint Temppoint = it->second;
        CCPoint pos = WorldMapView::instance()->m_map->getViewPointByTilePoint(Temppoint,info.tileServerId);
        sp->setPosition(pos);
        parentSp->addChild(sp);
    }
}
void WorldMapView::onShowTowerRange(bool visable){
    auto & nodeArr =  m_towerRangeMaskNode->getChildren();
    CCObject* pElemObj = NULL;
    for(auto child : nodeArr )
    {
        CCNode* parentNode = dynamic_cast<CCNode*>(child);
        int index = parentNode->getTag();
        CCPoint viewPoint =   m_map->getViewPointByTilePoint(WorldController::getPointByIndex(index)) + m_map->getPosition();
        if (isInWinsize(viewPoint,5) && visable) {
            parentNode->setVisible(true);
        }
        else{
            parentNode->setVisible(false);
        }
    }
}
bool WorldMapView::isInWinsize(CCPoint inScenePoint,int modelNum){
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    bool isInWinSize = false;
    if (inScenePoint.x >= -_tile_width * modelNum && inScenePoint.x < winSize.width + _tile_width * modelNum  && inScenePoint.y > -_tile_height * modelNum && inScenePoint.y < winSize.height + _tile_height * modelNum) {
        isInWinSize = true;
    }
    return isInWinSize;
}
void WorldMapView::onSendHelpCmd(){
    auto vec = WorldController::getInstance()->m_helpCmdVec;
    if (vec.size() > 0) {
        int serverId = vec.begin()->first;
        CCPoint forcePoint = vec.begin()->second;
        m_map->updataBoderMap(forcePoint, serverId);
        WorldController::getInstance()->m_helpCmdVec.erase(WorldController::getInstance()->m_helpCmdVec.begin());
    }
}
void WorldMapView::onCloseTilePopup(){
    WorldMapView::instance()->m_layers[WM_TILE]->removeAllChildren();
    removeTouchEffect();
}

void WorldMapView::onShowMarkLine(CCObject* obj)
{
//    return;
    if(m_drawRoadNode->getChildByTag(8989898)){
        m_drawRoadNode->getChildByTag(8989898)->removeFromParentAndCleanup(true);
    }
    m_occupyPointerNode->removeChildByTag(8989898);
    if (GlobalData::shared()->playerInfo.isInAlliance()) {
        int rallyPoint = GlobalData::shared()->playerInfo.allianceInfo.rallyPoint;
        if (rallyPoint != 0) {
            int leaderPoint = GlobalData::shared()->playerInfo.allianceInfo.leaderPoint;
            auto endPos = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(rallyPoint));
            auto startPos = m_map->getViewPointByTilePoint(WorldController::getPointByIndex(leaderPoint));
            auto line = CCLineBatchedSprite::create(startPos, endPos, 25, 10);
            line->setColor(ccc3(255, 177, 0));
            line->setTag(8989898);
            m_drawRoadNode->addChild(line);
            
            auto occupyPointer = CCLoadSprite::createSprite("mark_flag.png");
            occupyPointer->setAnchorPoint(ccp(0.5, 0));
            occupyPointer->setPosition(endPos);
            occupyPointer->setTag(8989898);
            occupyPointer->setScale(MapGlobalScale);
            m_occupyPointerNode->addChild(occupyPointer);
        }
    }
}
void WorldMapView::onShake(CCObject* pObj){
    if (GlobalData::shared()->shakeFlag == 1
        && WorldController::getInstance()->currentMapType == NORMAL_MAP
        && GlobalData::shared()->playerInfo.isInSelfServer()
        && !GuideController::share()->isInTutorial()
        && WorldController::getInstance()->isInWorld
        ){
        auto popup = PopupViewController::getInstance()->getCurrentPopupView();
        auto view =  dynamic_cast<IFShakeLayer*>(PopupViewController::getInstance()->getCurrentPopupView());
        if (popup && view == NULL) {
            return;
        }
        
        
        int modelDelayTime = GlobalData::shared()->shakeCmdDelayTime;
        
        int shakeCmdTime =  CCUserDefault::sharedUserDefault()->getIntegerForKey(MSG_SHAKECMDTIME ,-1);
        int currentTime = WorldController::getInstance()->getTime()/1000;
        int delayTime = currentTime - shakeCmdTime;
        CCLOG("shakelog: shakeCmdTime %d",shakeCmdTime);
        CCLOG("shakelog: delayTime %d",delayTime);
        CCLOG("shakelog: modelDelayTime %d",modelDelayTime);
        CCLOG("shakelog: dataSize %d",ShakeController::getInstance()->m_shakeDataVec.size());
        bool forceSend = false;
        bool shouldAddRound = true;
        bool resetRounde = false;
        if (shakeCmdTime == -1)
        {
            forceSend = true;
        }else if (delayTime > modelDelayTime)
        {
            forceSend = true;
            shouldAddRound = false;
            if (delayTime > 3600* 4) {
                resetRounde = true;
            }
        }
        if (ShakeController::getInstance()->m_shakeDataVec.size() == 0) {
            forceSend = true;
        }
        if (forceSend && ShakeController::getInstance()->getcanshake()) {
            int round = CCUserDefault::sharedUserDefault()->getIntegerForKey("round", 0);
            if (shouldAddRound) {
                round = round+1;
            }
            if (resetRounde) {
                round = 1;
            }
            IFShakeCmd* cmd = new IFShakeCmd(round);
            cmd->sendAndRelease();
            CCUserDefault::sharedUserDefault()->setIntegerForKey("round", round);
            CCUserDefault::sharedUserDefault()->setIntegerForKey(MSG_SHAKECMDTIME, currentTime);
            ShakeController::getInstance()->setcanshake(false);
        }
        
        if (ShakeController::getInstance()->m_shakeDataVec.size() > 0) {
            auto itemIt =    ShakeController::getInstance()->m_shakeDataVec.back();
            ShakeController::getInstance()->m_shakeDataVec.pop_back();
            int index = itemIt.first;
            int monsterId = itemIt.second;
            CCPoint tilePoint = WorldController::getPointByIndex(index);
            if (m_cityInfo.find(index) == m_cityInfo.end()) {
                m_map->updateDynamicMap(tilePoint);
//                update_water_shader();
                CCLOG("shakeLog sendcmd point%f, %f",tilePoint.x,tilePoint.y);
            }
            auto view =  dynamic_cast<IFShakeLayer*>(PopupViewController::getInstance()->getCurrentPopupView());
            if (view==NULL) {
                PopupViewController::getInstance()->addPopupView(IFShakeLayer::create(index,monsterId));
            }
            else{
                view->refreshInfo(index,monsterId);
            }
        }
    }else{
        //todo nothing
    }
    

}
void WorldMapView::onShowShakeGuide(float time){
    int currentTime = WorldController::getInstance()->getTime()/1000;
    UIComponent::getInstance()->showShakeGuideLayer();
    CCUserDefault::sharedUserDefault()->setIntegerForKey(MSG_SHAKETIME, currentTime);

}
bool WorldMapView::canShowShakeGuide(){
    if(GlobalData::shared()->shakeGuideFlag == 0){
        return false;
    }
    if(GuideController::share()->isInTutorial()){
        return false;
    }
    if(WorldController::getInstance()->getCurrentMarchCount() >= WorldController::getInstance()->getMaxMarchCount()){
        return false;
    }
    if (WorldController::getInstance()->currentMapType != NORMAL_MAP) {
        return false;
    }
    if(!GlobalData::shared()->playerInfo.isInSelfServer()){
        return false;
    }
    auto popup = PopupViewController::getInstance()->getCurrentPopupView();
    auto view =  dynamic_cast<IFShakeLayer*>(PopupViewController::getInstance()->getCurrentPopupView());
    if (popup && view == NULL) {
        return false;
    }
    int  shakeTime = CCUserDefault::sharedUserDefault()->getIntegerForKey(MSG_SHAKETIME ,-1);
    int currentTime = WorldController::getInstance()->getTime()/1000;
    if (shakeTime == -1 ||  currentTime - shakeTime >= 3600 * 24)
        {
            return true;
        }
    return false;
}
void WorldMapView::testCastle(int level){
    CCPoint myPoint = WorldController::getInstance()->selfPoint;
    createCity(m_cityInfo[WorldController::getIndexByPoint(ccpAdd(myPoint, ccp(0, -1)))]);
    createCity(m_cityInfo[WorldController::getIndexByPoint(ccpAdd(myPoint, ccp(-1, 0)))]);
    createCity(m_cityInfo[WorldController::getIndexByPoint(ccpAdd(myPoint, ccp(-1, -1)))]);
    createCity(m_cityInfo[WorldController::getIndexByPoint(ccpAdd(myPoint, ccp(0, 0)))]);

}

void WorldMapView::update_water_shader(const Vec2& position)
{
    static const int WATER_SHADER_TAG = WM_BG_TAG + 1;
    static const int OFFSET = 25 * 512;
    NBWaterMap* m_pWaterSprite0 = dynamic_cast<NBWaterMap*>(m_layers[WM_BG]->getChildByTag(WATER_SHADER_TAG + 0));
    NBWaterMap* m_pWaterSprite1 = dynamic_cast<NBWaterMap*>(m_layers[WM_BG]->getChildByTag(WATER_SHADER_TAG + 1));
    NBWaterMap* m_pWaterSprite2 = dynamic_cast<NBWaterMap*>(m_layers[WM_BG]->getChildByTag(WATER_SHADER_TAG + 2));
    NBWaterMap* m_pWaterSprite3 = dynamic_cast<NBWaterMap*>(m_layers[WM_BG]->getChildByTag(WATER_SHADER_TAG + 3));
    if (!m_pWaterSprite0)
    {
        m_pWaterSprite0 = NBWaterMap::create("shaders/spritemap.png");
        m_pWaterSprite0->setScale(25.0f);
        m_pWaterSprite0->setTag(WATER_SHADER_TAG + 0);
        m_pWaterSprite0->setPosition(0, 0);
        m_layers[WM_BG]->addChild(m_pWaterSprite0);
    }
    if (!m_pWaterSprite1)
    {
        m_pWaterSprite1 = NBWaterMap::create("shaders/spritemap.png");
        m_pWaterSprite1->setScale(25.0f);
        m_pWaterSprite1->setTag(WATER_SHADER_TAG + 1);
        m_pWaterSprite1->setPosition(OFFSET, 0);
        m_layers[WM_BG]->addChild(m_pWaterSprite1);
    }
    if (!m_pWaterSprite2)
    {
        m_pWaterSprite2 = NBWaterMap::create("shaders/spritemap.png");
        m_pWaterSprite2->setScale(25.0f);
        m_pWaterSprite2->setTag(WATER_SHADER_TAG + 2);
        m_pWaterSprite2->setPosition(0, OFFSET);
        m_layers[WM_BG]->addChild(m_pWaterSprite2);
    }
    if (!m_pWaterSprite3)
    {
        m_pWaterSprite3 = NBWaterMap::create("shaders/spritemap.png");
        m_pWaterSprite3->setScale(25.0f);
        m_pWaterSprite3->setTag(WATER_SHADER_TAG + 3);
        m_pWaterSprite3->setPosition(OFFSET, OFFSET);
        m_layers[WM_BG]->addChild(m_pWaterSprite3);
    }
    
    int xs = (int)position.x / OFFSET;
    int ys = (int)position.y / OFFSET;
    if (m_pWaterSprite0)
    {
        // 2 * OFFSET, 2 * OFFSET
        int x = xs % 2 == 0 ? xs * OFFSET : (xs + 1) * OFFSET;
        int y = ys % 2 == 0 ? ys * OFFSET : (ys + 1) * OFFSET;
        m_pWaterSprite0->setPosition(x, y);
    }
    if (m_pWaterSprite1)
    {
        // 1 * OFFSET, 2 * OFFSET
        int x = xs % 2 == 1 ? xs * OFFSET : (xs + 1) * OFFSET;
        int y = ys % 2 == 0 ? ys * OFFSET : (ys + 1) * OFFSET;
        m_pWaterSprite1->setPosition(x, y);
    }
    if (m_pWaterSprite2)
    {
        // 2 * OFFSET, 1 * OFFSET
        int x = xs % 2 == 0 ? xs * OFFSET : (xs + 1) * OFFSET;
        int y = ys % 2 == 1 ? ys * OFFSET : (ys + 1) * OFFSET;
        m_pWaterSprite2->setPosition(x, y);
    }
    if (m_pWaterSprite3)
    {
        // 1 * OFFSET, 1 * OFFSET
        int x = xs % 2 == 1 ? xs * OFFSET : (xs + 1) * OFFSET;
        int y = ys % 2 == 1 ? ys * OFFSET : (ys + 1) * OFFSET;
        m_pWaterSprite3->setPosition(x, y);
    }
    
//    static const int WATER_SHADER_TAG = WM_BG_TAG + 1;
//    static const int WATER_SHADER_X_CNT = 4;
//    static const int WATER_SHADER_Y_CNT = 6;
//    
//    static const int count_x = 2;
//    static const int count_y = 2;
//    static const float scale = 5.0;
//    static const float offset = 512 * scale;
//    NBWaterSprite* m_pWaterSprite = dynamic_cast<NBWaterSprite*>(m_layers[WM_BG]->getChildByTag(WATER_SHADER_TAG + 0));
//    if (!m_pWaterSprite)
//    {
//        m_pWaterSprite = NBWaterSprite::create(WATER_NORMALS);
//        m_pWaterSprite->setTag(WATER_SHADER_TAG);
//        m_pWaterSprite->setScale(scale);
//        m_layers[WM_BG]->addChild(m_pWaterSprite);
//        
////        for (int i = 0; i < count_x; i++)
////        {
////            for (int j = 0; j < count_y; j++)
////            {
////                m_pWaterSprite = NBWaterSprite::create(WATER_NORMALS);
////                m_pWaterSprite->setTag(WATER_SHADER_TAG + i + j * count_x);
////                m_pWaterSprite->setScale(scale);
////                m_layers[WM_BG]->addChild(m_pWaterSprite);
////    
////                m_pWaterSprite->setPosition(451584 + i * offset, 275520 + j * offset);
////            }
////        }
//    }
//    m_pWaterSprite->setPosition(position.x + 256 * scale, position.y + 256 * scale);
//    return;
    
    
//    for (int i = -WATER_SHADER_X_CNT / 2; i < WATER_SHADER_X_CNT / 2; i++)
//    {
//        for (int j = -WATER_SHADER_Y_CNT / 2; j < WATER_SHADER_Y_CNT / 2; j++)
//        {
//            Sprite* sp = dynamic_cast<Sprite*>(m_layers[WM_BG]->getChildByTag(WATER_SHADER_TAG + tag));
//            if (!sp)
//            {
//                sp = Sprite::create("shaders/water2d.png", Rect(0, 0, 512, 512));
//                sp->setAnchorPoint(Vec2(0, 0));
//                sp->setTag(WATER_SHADER_TAG + tag);
//                m_layers[WM_BG]->addChild(sp);
//#define USE_WATER_SHADER
//#ifdef USE_WATER_SHADER
//                auto TexCache = Director::getInstance()->getTextureCache();
//                if (!m_water_wave1) {
//                    m_water_wave1 = TexCache->addImage("shaders/wave2d_0.jpg");
//                    m_water_wave1->retain();
//                }
//                if (!m_water_wave2) {
//                    m_water_wave2 = TexCache->addImage("shaders/wave2d_1.png");
//                    m_water_wave2->retain();
//                }
//                m_water_wave1->setTexParameters(Texture2D::TexParams{GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT});
//                m_water_wave2->setTexParameters(Texture2D::TexParams{GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT});
//                
//                const char* pszVertSource =
//                "attribute vec4 a_position; \n\
//                attribute vec2 a_texCoord; \n\
//                attribute vec4 a_color; \n\
//                 \n\
//                \n#ifdef GL_ES\n \n\
//                varying lowp vec4 v_fragmentColor; \n\
//                varying mediump vec2 v_texCoord; \n\
//                \n#else\n \n\
//                    varying vec4 v_fragmentColor; \n\
//                varying vec2 v_texCoord; \n\
//                \n#endif\n \n\
//                 \n\
//                void main() \n\
//                { \n\
//                    gl_Position = CC_PMatrix * a_position; \n\
//                    v_fragmentColor = a_color; \n\
//                    v_texCoord = a_texCoord; \n\
//                } \n";
//                
//                const char* pszFragSource =
//                "#ifdef GL_ES \n\
//                precision mediump float; \n\
//                #endif \n\
//                \n\
//                varying vec4 v_fragmentColor; \n\
//                varying vec2 v_texCoord; \n\
//                \n\
//                uniform sampler2D u_wave1; \n\
//                uniform sampler2D u_wave2; \n\
//                uniform float u_interpolate; \n\
//                uniform float saturateValue; \n\
//                uniform float wave_time; \n\
//                float verticalSpeed = 0.03797; \n\
//                float horizontalSpeed = 0.077; \n\
//                void main() { \n\
//                    vec2 textCoord1 = v_texCoord; \n\
//                    textCoord1.x += verticalSpeed * wave_time; \n\
//                    textCoord1.x = fract(textCoord1.x); \n\
//                    textCoord1.y += horizontalSpeed * wave_time; \n\
//                    textCoord1.y = fract(textCoord1.y); \n\
//                    vec3 color = texture2D(u_wave1, textCoord1).xyz; \n\
//                    color += texture2D(u_wave2, v_texCoord).xyz; \n\
//                    if (color.x > saturateValue) { \n\
//                        color = vec3(1.0); \n\
//                        gl_FragColor = vec4(color, 1.0); \n\
//                    } else { \n\
//                        color = texture2D(CC_Texture0, v_texCoord).xyz; \n\
//                        gl_FragColor = vec4(color, 0.5); \n\
//                    } \n\
//                }";
//                auto glprogram = GLProgram::createWithByteArrays(pszVertSource, pszFragSource);
//                auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
//                sp->setGLProgramState(glprogramstate);
//                
//                glprogramstate->setUniformTexture("u_wave1", m_water_wave1);
//                glprogramstate->setUniformTexture("u_wave2", m_water_wave2);
//                glprogramstate->setUniformFloat("saturateValue", 1.2);
//                glprogramstate->setUniformFloat("wave_time", 0);
//                
//                static float wave_time = 0;
//                sp->schedule([sp, glprogramstate](float dt){
//                    wave_time += dt / 50.0;
//                    glprogramstate->setUniformFloat("wave_time", wave_time);
//                }, "water");
//#endif
//            }
//            sp->setPosition(position.x + i * 512, position.y + j * 512);
//            tag++;
//        }
//    }
}
