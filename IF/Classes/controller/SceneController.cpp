//
//  SceneController.cpp
//  IF
//
//  Created by 邹 程 on 13-8-29.
//
//

#include "SceneController.h"
#include "LocalController.h"
#include "SceneContainer.h"
#include "LoadingScene.h"
#include "UIComponent.h"
#include "TransitionCallback.h"
#include "ImperialScene.h"
#include "WorldMapView.h"
#include "BattlefieldScene.h"
#include "DragonScene.h"

#define SAVE_TAG 1000
#define CLEANUP_TAG 2000

static SceneController *_instance = NULL;

SceneController::~SceneController() {
    for (auto &map : m_levelMap) {
        for (auto &layer : map.second) {
            CC_SAFE_RELEASE(layer.second);
        }
        map.second.clear();
    }
    m_levelMap.clear();
}

SceneController* SceneController::getInstance() {
    if (!_instance) {
        _instance = new SceneController();
        _instance->init();
    } else {
        CCAssert(_instance->m_sceneContainer,"SceneContainer should add to runningScene before using this");
    }
    return _instance;
}

void SceneController::init() {
    showBG = NULL;
    currentSceneId = SCENE_ID_LOADING;
    for (int i=0;i<LEVEL_MAX; i++) {
        m_levelMap.insert(pair<int, map<int, CCLayer*> > (i,map<int, CCLayer*>()));
    }
    m_maincityCustom=false;
}

CCNode* SceneController::getLevelNode(int level) {
    return m_sceneContainer->getChildByTag(level);
}

CCLayer* SceneController::getCurrentLayerByLevel(int level){
    auto levelNode = m_sceneContainer->getChildByTag(level);
    if (levelNode->getChildrenCount()) {
        return dynamic_cast<CCLayer*>(levelNode->getChildren().at(0));
    }
    auto layer = CCLayer::create();
    levelNode->addChild(layer);
    return layer;
}

CCLayer* SceneController::getLayerByLevelAndSceneId(int level, int sceneId) {
    if (m_levelMap[level].find(sceneId) == m_levelMap[level].end()) {
        return NULL;
    }
    return m_levelMap[level][sceneId];
}

void SceneController::clearLayerByLevelAndSceneId(int level, int sceneId) {
    auto currentLayer = getCurrentLayerByLevel(level);
    if (getSceneIdByTag(currentLayer->getTag()) == sceneId) {
        m_sceneContainer->getChildByTag(level)->removeAllChildrenWithCleanup(true);
    }
    auto oldLayer = (m_levelMap[level])[sceneId];
    if (oldLayer) {
        CC_SAFE_RELEASE(oldLayer);
        (m_levelMap[level]).erase(sceneId);
    }
}

void SceneController::doLastSceneClear(int nextSceneId) {
    
    PopupViewController::getInstance()->removeAllPopupView();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_GCMREWARD_UPDATE);
    auto levelNode = getLevelNode(LEVEL_SCENE);
    if(!levelNode){
        return;
    }
    auto arr = levelNode->getChildren();
    if(arr.size() == 0){
        return;
    }
    CCLayer* oldLayer = dynamic_cast<CCLayer*>(levelNode->getChildren().at(0));
    if (!oldLayer) {
        return;
    }
    int oldTag = oldLayer->getTag();
    
    /* Prevent cache layer to clear layer texture*/
//    if (shouldSaveSceneByTag(oldTag)) {
//        int oldSceneId = getSceneIdByTag(oldTag);
//        if (!(m_levelMap[LEVEL_SCENE])[oldSceneId]) {
//            (m_levelMap[LEVEL_SCENE])[oldSceneId] = oldLayer;
//            oldLayer->retain();
//        } else if ((m_levelMap[LEVEL_SCENE])[oldSceneId] != oldLayer) {
//            (m_levelMap[LEVEL_SCENE])[oldSceneId]->release();
//            (m_levelMap[LEVEL_SCENE])[oldSceneId] = oldLayer;
//            oldLayer->retain();
//        }
//    }
    /* If need cache again, uncomment code above */
    
    //begin a by ljf
    //重新创建spine,因为上一帧的spine在onExit中被purge掉了
    UIComponent::getInstance()->unLoadSpineActivityBox();
    //end a by ljf
    
    // release node
    levelNode->removeChild(oldLayer,shouldCleanUpSceneByTag(oldTag));
    
    // as every texture node hold a ref to texture , texture release must after node release
    
    int oldSceneId = getSceneIdByTag(oldTag);
    
    // release scene common texture here
    CCLoadSprite::releaseResourceBySceneId(oldSceneId);
    
    // release scene specific texture here
    if (oldSceneId == SCENE_ID_BATTLE) {
//        CCLoadSprite::releaseResourceBySceneId(SCENE_ID_BATTLE,BattleObjectManager::shared()->getResDict());
//        BattleObjectManager::shared()->getResDict()->removeAllObjects();
//        
//        // because battle scene release all common resource , we must reload it here
//        CCLoadSprite::loadCommonResource();
//        addGUI();
    }
    
    // if next scene is battle scene , we should discard commmon resource here
    if (oldSceneId != SCENE_ID_BATTLE && nextSceneId == SCENE_ID_BATTLE) {
        CCLoadSprite::releaseCommonResourceForBattle();
    }
}

void SceneController::loadSceneResource(int sceneId, bool shouldSave, bool shouldCleanUp,bool firstInit,int subSceneId,MapType mapType){
    CCDirector::sharedDirector()->setVisitFlag(false);
    CCLoadSprite::loadResourceBySceneId(sceneId);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SceneController::loadSceneResourceBack), SPRITE_LOAD_COMPLETE, NULL);
    m_sceneId = sceneId;
    m_shouldSave = shouldSave;
    m_shouldCleanUp = shouldCleanUp;
    m_fisrtInit = firstInit;
    m_subSceneId = subSceneId;
    m_mapType = mapType;
}

void SceneController::loadSceneResourceBack(CCObject *obj){
    CCDirector::sharedDirector()->setVisitFlag(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SPRITE_LOAD_COMPLETE);
    doSceneInit(m_sceneId, m_shouldSave, m_shouldCleanUp, m_fisrtInit, m_subSceneId, m_mapType);
}

void SceneController::doSceneInit(int sceneId, bool shouldSave, bool shouldCleanUp,bool firstInit,int subSceneId,MapType mapType) {
    auto layer = getLayerByLevelAndSceneId(LEVEL_SCENE, sceneId);
    if(GlobalData::shared()->sceneResourceStepLoad != 1){
        CCLoadSprite::loadResourceBySceneId(sceneId);
    }
    if (!layer) {
        switch (sceneId) {
            case SCENE_ID_LOADING:{
                layer = LoadingScene::create();
            }
                break;
            case SCENE_ID_IMPERIAL:
            case SCENE_ID_MAIN:{

                layer = ImperialScene::create();
            }
                break;
            case SCENE_ID_DRAGON:{
                layer = DragonScene::create();
            }
                break;
            case SCENE_ID_BATTLE:{
//                layer = BattleScene::create();
            }
                break;
            case SCENE_ID_BATTLE2:{
                layer = BattlefieldScene::create();
            }
                break;
            case SCENE_ID_WORLD: {
                if(mapType == DEFAULT_MAP){
                    mapType = WorldController::getInstance()->getMapTypeByDefault();
                }
                auto viewPoint = ccp(-1, -1);
                if (subSceneId != -1) {
                    viewPoint = WorldController::getPointByIndex(subSceneId, WorldController::getInstance()->getMapXByType(mapType), WorldController::getInstance()->getMapYByType(mapType));
                }
                layer = WorldMapView::create(viewPoint, mapType);
            }
            case SCENE_ID_CROP:
            case SCENE_ID_WOOD:
            case SCENE_ID_IRON:
            case SCENE_ID_BARRACKS:
            case SCENE_ID_MARKET:
            {
            }
                break;
                
            default:
                return;
        }
    }
    setLayerTag(layer, sceneId, shouldSave, shouldCleanUp);
    getLevelNode(LEVEL_SCENE)->addChild(layer);
    currentSceneId = sceneId;
    //begin a by ljf
    //重新创建spine,因为上一帧的spine在onExit中被purge掉了
    UIComponent::getInstance()->loadSpineActivityBox();
    //end a by ljf
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCENE_CHANGE_FINISH);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCENE_CHANGED);
}

void SceneController::gotoScene(int sceneId,bool shouldSave,bool shouldCleanUp,int subSceneId,MapType mapType) {
    if (sceneId == SCENE_ID_BATTLE || sceneId == SCENE_ID_BATTLE2) {
        removeGUI();
    }
    if(sceneId == SCENE_ID_WORLD && mapType == DEFAULT_MAP){
        mapType = WorldController::getInstance()->getMapTypeByDefault();
    }
    if (sceneId != SCENE_ID_BATTLE && sceneId != SCENE_ID_BATTLE2 && isGotoSelf(sceneId, mapType)) {
        return;
    }

    if ((sceneId == SCENE_ID_BATTLE || sceneId == SCENE_ID_BATTLE2)&& subSceneId == 1) {
        doLastSceneClear(sceneId);
        if(GlobalData::shared()->sceneResourceStepLoad == 1){
            loadSceneResource(sceneId, shouldSave, shouldCleanUp,false);
        }else{
            doSceneInit(sceneId, shouldSave, shouldCleanUp,false);
        }

        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCENE_CHANGED);
        return;
    }
    
    CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
    if (scene->getPositionY() != 0) {
        scene->setPositionY(0);
        return;
    }
    bool useTrasition = false;
    switch (sceneId) {
        case SCENE_ID_LOADING:
        {
            
        }
            break;
        case SCENE_ID_WORLD:
        case SCENE_ID_MAIN:
        {
            if(currentSceneId != SCENE_ID_LOADING){
                useTrasition = true;
            }
        }
            break;
        case SCENE_ID_DRAGON:
        case SCENE_ID_BATTLE:
        case SCENE_ID_CROP:
        case SCENE_ID_WOOD:
        case SCENE_ID_IRON:
        case SCENE_ID_BARRACKS:
        case SCENE_ID_MARKET:
        case SCENE_ID_IMPERIAL:
        {
            useTrasition = true;
        }
            break;
            
        default:
            return;
    }
    auto transtion = SceneTransition::createTransition(sceneId, shouldSave, shouldCleanUp, useTrasition,subSceneId, mapType);
    if(transtion){
        transtion->start();
    }
}
void SceneController::removeGUI(){
    clearLayerByLevelAndSceneId(LEVEL_GUI,SCENE_ID_GUI);
    UIComponent::purgeData();
}
void SceneController::addGUI() {
    if (getLevelNode(LEVEL_GUI)->getChildByTag(SCENE_ID_GUI+CLEANUP_TAG)) {
        return;
    }
    auto gui = UIComponent::getInstance();
    setLayerTag(gui, SCENE_ID_GUI, false, true);
    getLevelNode(LEVEL_GUI)->addChild(gui);
}

void SceneController::gotoLoading(bool clearFlag) {
    if (currentSceneId != SCENE_ID_LOADING || clearFlag) {
        doLastSceneClear(SCENE_ID_LOADING);
    }
//    loadSceneResource(SCENE_ID_LOADING, false, true);
    if(GlobalData::shared()->sceneResourceStepLoad == 1){
        loadSceneResource(SCENE_ID_LOADING, false, true);
    }else{
        doSceneInit(SCENE_ID_LOADING, false, true);
    }
}

void SceneController::updateSubSceneBuild(int itemId)
{
}

void SceneController::updateSubSceneBuildUI(int itemId, double saveTime)
{
}

void SceneController::updateSubSceneBuildPic(int buildId, int type)
{
}

int SceneController::curSceneIsCropScene()
{
    int sceneid = 0;
    return sceneid;
}

void SceneController::upBuildingFlyBall(int buildId, int qid, bool toScene)
{
}

void SceneController::upAutoBtnState()
{
}

void SceneController::updateBuildings() {
}

int SceneController::getSceneIdByTag(int tag) {
    if (tag >= (SAVE_TAG + CLEANUP_TAG)) {
        return tag - (SAVE_TAG + CLEANUP_TAG);
    } else if (tag >= CLEANUP_TAG) {
        return tag - CLEANUP_TAG;
    } else if (tag >= SAVE_TAG) {
        return tag - SAVE_TAG;
    } else {
        return tag;
    }
}

bool SceneController::shouldCleanUpSceneByTag(int tag) {
    return (tag >= CLEANUP_TAG);
}

bool SceneController::shouldSaveSceneByTag(int tag) {
    return ((tag >= SAVE_TAG && tag < CLEANUP_TAG)||(tag >= (SAVE_TAG + CLEANUP_TAG)));
}

void SceneController::setLayerTag(cocos2d::CCLayer *layer, int sceneId, bool shouldSave, bool shouldCleanUp) {
    int tag = sceneId;
    CCAssert((tag < SAVE_TAG && tag > -1), "Scene ID should bigger than -1 and smaller than saveTag , if it is not enough , enlarge saveTag and cleanUpTag!");
    if (shouldSave) {
        tag += SAVE_TAG;
    }
    if (shouldCleanUp) {
        tag += CLEANUP_TAG;
    }
    layer->setTag(tag);
}

bool SceneController::isGotoSelf(int sceneId, MapType type) {
    if(currentSceneId == SCENE_ID_WORLD && sceneId == SCENE_ID_WORLD){
        if(WorldController::getInstance()->currentMapType != type){
            return false;
        }
    }
    return currentSceneId == sceneId;
}

void SceneController::onFlyOutPut(int itemId, int output, int forceResType)
{
    if (currentSceneId == SCENE_ID_MAIN) {
        ImperialScene* mainScene = dynamic_cast<ImperialScene*>(getCurrentLayerByLevel(LEVEL_SCENE));
        if (mainScene) {
            if (forceResType!=-1) {
                mainScene->onFlyOutPut(itemId, output,forceResType);
            }else{
                mainScene->onFlyOutPut(itemId, output);
            }
        }
    }
}