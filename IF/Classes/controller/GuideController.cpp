//
//  GuideController.cpp
//  IF
//
//  Created by ganxiaohua on 13-11-6.
//
//

#include "GuideController.h"
#include "LocalController.h"
#include "SceneController.h"
#include "PopupViewController.h"
#include "UIComponent.h"
#include "PlotInfo.h"
#include "ImperialScene.h"
#include "ChangeNickNameView.h"
#include "GuideRecordCommand.h"
#include "ParticleController.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "MainCityView.h"
#include "ScienceListView.h"
#include "ScienceView.h"
#include "SceneContainer.h"
#include "BuildListView.h"
#include "BuildUpgradeView.h"
#include "BattleView.h"
#include "MakingSoldierView.h"
#include "ProductionSoldiersView.h"
#include "MonsterDetailView.h"
#include "WorldMapView.h"
#include "ScienceController.h"
#include "AchievementNewPopUpView.h"
#include "StoreView.h"

#include "EquipSiteView.h"
#include "EquipmentListView.h"
#include "EquipmentCreateView.h"
#include "QueueController.h"
#include "ArmyController.h"
#include "NewPlayerQianDaoView.h"
#include "GeneralsPopupView.h"
#include "SettingPopUpView.h"
#include "SetAccountView.h"

static GuideController* _instance;

GuideController* GuideController::share(){
    if(!_instance){
        _instance = GuideController::create();
        _instance->retain();
        auto topLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_CONTROL);
        topLayer->addChild(_instance,999);
    }
    return _instance;
}

bool GuideController::init(){
    if(CCNode::init()){
        m_willGuideId = "";
        m_startGuide = false;
        m_node = NULL;
        m_gView = NULL;
        m_panel = NULL;
        updateWorldInfo = true;
        m_recordBuildingId = 0;
        arr = CCArray::create();
        tmpTime = 0;
        openScienceFlag = false;
        IsSkip = false;
        return true;
    }
    return false;
}

void GuideController::purgeData(){
    m_startGuide = false;
    //移出引导
    if(m_gView){
        if(m_gView->getParent()){
            m_gView->getParent()->removeChild(m_gView);
        }
        m_gView = NULL;
    }
    if(m_node){
        m_node->release();
        m_node = NULL;
        PopupViewController::getInstance()->CanPopPushView = true;
        PopupViewController::getInstance()->showPushPop();
    }
    openScienceFlag = false;
    arr->removeAllObjects();
    m_recordBuildingId = 0;
    removeLevelUpPop();
    updateWorldInfo = true;
    m_currentId = "";
    
    _instance->release();
    _instance = NULL;
}

void GuideController::willGuide(std::string guideId)
{
    if(guideId == ""){
        return;
    }
    if(PopupViewController::getInstance()->getCurrentPopupView() != NULL){
        m_willGuideId = guideId;
        return;
    }else{
        m_willGuideId = "";
    }
    setGuide(guideId);
}

void GuideController::buildingUpdate(int buildingId)
{
    if(buildingId == 0){
        return;
    }
    if(PopupViewController::getInstance()->getCurrentPopupView() != NULL){
        m_recordBuildingId = buildingId;
        return;
    }else{
        m_recordBuildingId = 0;
    }
    map<int, FunBuildInfo>::iterator it;
    auto &info = GlobalData::shared()->imperialInfo[buildingId];
    for(it = GlobalData::shared()->imperialInfo.begin(); it != GlobalData::shared()->imperialInfo.end(); it++){
        if(it->first / 1000 == buildingId / 1000 && it ->first != buildingId){
            if(it->second.level >= info.level){
                return;
            }
        }
    }
    auto dict = LocalController::shared()->DBXMLManager()->getGroupByKey("guide");
    CCDictElement *ele;
    CCDICT_FOREACH(dict, ele) {
        std::string itemId = ele->getStrKey();
        int openType = atoi(CCCommonUtils::getPropById(itemId, "open").c_str());
        if(openType == BUILDING_OPEN){
            std::string param1 = CCCommonUtils::getPropById(itemId, "para1");
            int bid = atoi(param1.c_str());
            int level = bid%100;
            bid = bid-level;
            
            if(bid == buildingId / 1000 && info.level == level){
                setGuide(itemId);
            }
        }
    }
}

bool GuideController::start(){
    if(!m_startGuide){
        m_startGuide = true;
        std::string guideId = "";
        string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
        if(GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0 && (gFake=="" || gFake=="start_1")){
            guideId = "3070100";//第一个引导
        }else{
            std::string stepKey = GlobalData::shared()->playerInfo.uid + GUIDE_STEP;
            guideId = CCUserDefault::sharedUserDefault()->getStringForKey(stepKey.c_str());
            guideId = checkNetStep(guideId);
            if (guideId=="") {
                guideId = checkOpenSpeGuide();
            }
        }
        setGuide(guideId);
        if(guideId != ""){
            return false;
        }
        return true;
    }
    return true;
}

void GuideController::setGuide(std::string guideId){
//    guideId = "";
    if (m_currentId == guideId && guideId != "") {
        return;
    }
    
    m_currentId = guideId;
    //保存步骤到本地
    std::string markStr = CCCommonUtils::getPropById(m_currentId, "mark");
    if(markStr == "1" || guideId == ""){
        saveCurrentStepToLocal();
    }
    if(guideId == ""){
        if(m_panel){
            PopupViewController::getInstance()->addPopupView(m_panel);
        }
        removeLevelUpPop();
    }
    //移出引导
    if(m_gView){
        if(m_gView->getParent()){
            m_gView->getParent()->removeChild(m_gView);
        }
        m_gView = nullptr;
    }
    if(m_node){
        m_node->release();
        m_node = NULL;
    }
    
    //开始当前引导
    if(m_currentId != ""){
        if(checkStep(m_currentId)){
            std::string goTo = CCCommonUtils::getPropById(guideId, "goto");
            setGuide(goTo);
        }else{
            doGuide();
        }
    }
    else {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_END);
    }
}

void GuideController::doGuide(){
    if(m_currentId == ""){
        return;
    }
    createGuideView();
    bool b_lock = true;
    std::string lag = CCCommonUtils::getPropById(m_currentId, "lag");
    if(lag != ""){
        b_lock = false;
        m_gView->setModelLayerOpacity(0);
        double time = atof(lag.c_str());
        this->scheduleOnce(schedule_selector(GuideController::next), time);
    }
    else {
//        m_gView->setModelLayerOpacity(0);
    }
//    if(m_currentId == "3050200") {
//        m_gView->setModelLayerOpacity(0);
//    }
    
    std::string plotStr = CCCommonUtils::getPropById(m_currentId, "plot");
    //plot引导
    if(plotStr != ""){
        m_gView->doPlot(plotStr, true);
        b_lock = false;
    }
    
    std::string plotStr1 = CCCommonUtils::getPropById(m_currentId, "explanation");
    //plot引导1
    if(plotStr1 != ""){
        m_gView->doPlot1(plotStr1, true);
        b_lock = false;
    }
    
    if (b_lock) {
        m_gView->setModelLayerOpacity(0);
        this->scheduleOnce(schedule_selector(GuideController::clearLock), 5);
    }

    //指向引导
    std::string areaStr = CCCommonUtils::getPropById(m_currentId, "area");
    if(areaStr != ""){
        vector<std::string> vector;
        CCCommonUtils::splitString(areaStr, "_", vector);
        this->schedule(schedule_selector(GuideController::getPositionNode), 0.05);

        if(vector.size()>0 && vector[0] != "BC"){
            getPositionNode(0.0);
        }
    }
    
    //播放动画
    std::string cartoon = CCCommonUtils::getPropById(m_currentId, "cartoon");
    if(cartoon != ""){
        if (cartoon == "begin1") {
            m_gView->setModelLayerOpacity(0);
            this->scheduleOnce(schedule_selector(GuideController::clearLock), 10);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_BEGIN1);
        }
        else if (cartoon == "begin2") {
            m_gView->setModelLayerOpacity(0);
            this->scheduleOnce(schedule_selector(GuideController::clearLock), 10);
           CCLOG("CCSafeNotificationCenter::postNotification::GUIDE_BEGIN2");
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_BEGIN2);
        }
        else if(cartoon == "songbing1"){
            WorldController::getInstance()->addGuideData();
            m_gView->setModelLayerOpacity(0);
            this->scheduleOnce(schedule_selector(GuideController::next), 5);
        }
        else if(cartoon == "fengshou1")
        {
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            if (layer) {
                m_gView->setModelLayerOpacity(0);
                m_gView->isMoving = true;
                this->scheduleOnce(schedule_selector(GuideController::removeGuidView), 1);
                layer->makeCanPlayArrowPoint();
            }
            else {
                PopupViewController::getInstance()->removeAllPopupView();
                auto world = WorldMapView::instance();
                if (world) {
                    world->leaveWorld();
                }
            }
            
        }
    }
}

void GuideController::clearLock(float t)
{
    if(m_gView) {
        m_gView->removeModelLayer();
    }
}

void GuideController::removeGuidView(float t)
{
    if(m_gView){
        if(m_gView->getParent()){
            m_gView->getParent()->removeChild(m_gView);
        }
        m_gView = NULL;
    }
}

void GuideController::saveCurrentStepToLocal()
{
    std::string stepKey = GlobalData::shared()->playerInfo.uid + GUIDE_STEP;
    CCUserDefault::sharedUserDefault()->setStringForKey(stepKey.c_str(), m_currentId);
    if ( m_currentId == GUIDE_MERCHANTE_ST ) {
        CCUserDefault::sharedUserDefault()->setStringForKey(GUIDE_MAINCITY_LEVEL_UP, "");
    }
    CCUserDefault::sharedUserDefault()->flush();
}

//是否满足跳过条件
bool GuideController::checkStep(std::string guideId){
    return false;
}

bool GuideController::hasGuide(){
    if(m_gView){
        return true;
    }
    return false;
}

void GuideController::checkUserAction(CCObject *obj){
    if(m_currentId != ""&&obj){
        if(m_node){
            FunBuild *build = dynamic_cast<FunBuild*>(m_node);
            if(build){
                build->stopShadow();
            }
        }
        tmpTime = 0;
        std::string str = dynamic_cast<CCString*>(obj)->getCString();
        std::string areaStr = CCCommonUtils::getPropById(m_currentId, "area");
        std::string cartoon = CCCommonUtils::getPropById(m_currentId, "cartoon");
        std::string explanation = CCCommonUtils::getPropById(m_currentId, "explanation");
        if(areaStr != "" && str == areaStr){
            next();
        }
        else if (cartoon != "" && str == cartoon) {
            next();
        }
        else if (explanation != "" && str == explanation) {
            next();
        }
        else if (str=="BU_424000_cd" && (m_currentId=="3073700" || m_currentId=="3073800" || m_currentId=="3073900")) {
            m_currentId = "3073900";//造兵，不秒兵，等待时间完成时 调用
            next();
        }
    }
}

void GuideController::next(float t){
    std::string next = CCCommonUtils::getPropById(m_currentId, "next");
    int tmpRecordStep = atoi(m_currentId.c_str());
    recordStep(CC_ITOA(tmpRecordStep+2));
    
    if (m_currentId=="3031000" &&  !CCCommonUtils::isTestPlatformAndServer("guide_3")) {//从世界回来
        next = GUIDE_HAVEST_ST;
    }
    setGuide(next);
}

bool GuideController::isInTutorial(){
    return m_currentId != "";
}

string GuideController::getCurGuideID()
{
    return m_currentId;
}
///

void GuideController::onEnter(){
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GuideController::checkUserAction), GUIDE_INDEX_CHANGE, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GuideController::onEnterFrame), this, 0.1,kCCRepeatForever, 0.0f, false);
}

void GuideController::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GuideController::onEnterFrame), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_INDEX_CHANGE);

    CCNode::onExit();
}

void GuideController::onEnterFrame(float dt){
    willGuide(m_willGuideId);
    buildingUpdate(m_recordBuildingId);
    if(checkStep(m_currentId)){
        setGuide(m_currentId);
    }
    bool deleteFlag = true;
    if(m_node){
        if(m_node->getParent() == NULL){
            deleteFlag = true;
        }else{
            CCNode* tmp = m_node;
            
            while (tmp) {
                if(!tmp->isVisible()){
                    break;
                }
                SceneContainer *scence = dynamic_cast<SceneContainer*>(tmp);
                if(scence){
                    deleteFlag = false;
                    break;
                }
                tmp = tmp->getParent();
            }
        }
    }
    //3010800,3021100,3061100,3022500  秒cd 相关步骤
    if(deleteFlag && m_gView && m_node && (m_currentId != "3031200" && m_currentId != "3031300" && m_currentId != "3031400" && m_currentId != "3031500" && m_currentId != "3030300" && m_currentId != "3030400" && m_currentId != "3030500" && m_currentId != "3030600")){
//    if(deleteFlag && m_gView && m_node && (m_currentId == "3010800" || m_currentId == "3021100" || m_currentId == "3061100" || m_currentId == "3022500")){
        if(m_gView->getParent()){
            m_gView->getParent()->removeChild(m_gView);
        }
        m_node->release();
        m_node = NULL;
        m_gView = NULL;
        next();
    }
    
    if (m_currentId == "" && !PopupViewController::getInstance()->CanPopPushView) {
        tmpTime++;
        if (tmpTime >=2) {
            tmpTime = 0;
            PopupViewController::getInstance()->CanPopPushView = true;
            PopupViewController::getInstance()->showPushPop();
        }
    }
}

//引导 对引的node进行获取，取得即停止
void GuideController::getPositionNode(float t){
    if(m_currentId == ""){
        this->unschedule(schedule_selector(GuideController::getPositionNode));
        return;
    }
    std::string area = CCCommonUtils::getPropById(m_currentId, "area");
    m_node = getNode(area);

    if(m_node!=NULL){
        PopupViewController::getInstance()->CanPopPushView = false;
        m_node->retain();
        m_offX = 0;
        m_offY = 0;
        createGuideView();
        std::vector<std::string> opers;
        CCCommonUtils::splitString(area, "_", opers);
        this->unschedule(schedule_selector(GuideController::getPositionNode));
        if(opers.size() == 2 && opers[0] == "MC"){
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            layer->setTouchEnabled(false);
            int itemId = atoi(opers[1].c_str());
            m_gView->isMoving = true;
            m_gView->setModelLayerOpacity(0);
            layer->onMoveToBuild(itemId, true);
            this->runAction(CCSequence::create(
                                               CCDelayTime::create(1.0)
                                               , CCCallFuncN::create(this, callfuncN_selector(GuideController::movingCallBack))
                                               , NULL
                                               ));
        }
        else if (opers.size() == 2 && opers[0] == "SMC") {
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            layer->setTouchEnabled(false);
            int itemId = atoi(opers[1].c_str());
            m_gView->isMoving = true;
            m_gView->setModelLayerOpacity(0);
            layer->onMoveToSpeBuildAndPlay(itemId, true);
            this->runAction(CCSequence::create(
                                               CCDelayTime::create(1.0)
                                               , CCCallFuncN::create(this, callfuncN_selector(GuideController::movingCallBack))
                                               , NULL
                                               ));
        }
        else if(area == "WL_wood" || area == "WL_relic" || area == "WL_food"){
            auto layer = dynamic_cast<WorldMapView*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            layer->setTouchEnabled(false);
            m_gView->setModelLayerOpacity(0);
            this->runAction(CCSequence::create(
                                               CCDelayTime::create(1.1)
                                               , CCCallFuncN::create(this, callfuncN_selector(GuideController::movingCallBack))
                                               , NULL
                                               ));
        }
        else{
            addGuideView(false);
        }
    }
}

void GuideController::movingCallBack(CCNode* p){
    addGuideView(true);
}

void GuideController::addGuideView(bool movingFlag){
    if(m_node == NULL){
        return;
    }
    float s = 1.0;
    if(movingFlag){
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN){
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            s = layer->getTouchLayerScale();
            if(layer){
                layer->setTouchEnabled(true);
            }
        }else if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            auto layer = dynamic_cast<WorldMapView*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            if(layer){
                layer->setTouchEnabled(true);
            }
        }
    }
    std::string area = CCCommonUtils::getPropById(m_currentId, "area");
    if(area == "WL_wood" || area == "WL_relic" || area == "WL_food"){
        if(m_node){
            m_node->release();
        }
        m_node = getNode(area);
        updateWorldInfo = true;

        if(m_node){
            m_node->retain();
        }else{
            setGuide("");
            return;
        }
        if(area == "WL_relic"){
            m_offY = _tile_height / 2;
        }
    }
    if(m_gView->getParent()){
        m_gView->getParent()->removeChild(m_gView);
    }
    m_gView->setPosition(ccp(0, 0));
    m_gView->isMoving = false;

    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_GUIDE);
    layer->addChild(m_gView);

    CCPoint p = m_node->getPosition();
    CCDirector::sharedDirector()->convertToGL(p);
    CCPoint p2 = m_node->getParent()->convertToWorldSpace(p);
    CCPoint localP = m_gView->convertToNodeSpaceAR(p2);
    
    CCRect rect = cocos2d::CCRect(m_node->boundingBox());
    float dx = localP.x + m_offX + (rect.origin.x - m_node->getPositionX());
    float dy = localP.y + m_offY + (rect.origin.y - m_node->getPositionY());
    float w = m_node->getContentSize().width * s;
    float h = m_node->getContentSize().height * s;
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        w = m_node->getContentSize().width * m_node->getScaleX();
        h = m_node->getContentSize().height * m_node->getScaleY();
        if (CCCommonUtils::isIosAndroidPad() )
        {
            if (!strcasecmp("UI_target", area.c_str()))
            {
                w *= 1.8;
                h *= 1.8;
            }
        }

    }
    
    std::vector<std::string> opers;
    CCCommonUtils::splitString(area, "_", opers);
    if(area == "WL_wood" || area == "WL_food")
    {
        dx += 40;
        dy += 20;
        if (!CCCommonUtils::isIosAndroidPad())
        {
            w -= 90;
            h -= 60;
        }
    }
    else if (area == "SC2_out") {
        dx -= 3;
        dy -= 3;
        w += 20;
        h += 20;
    }
    else if(opers.size()>0 && opers[0] == "MC")
    {
        if (!CCCommonUtils::isIosAndroidPad())
        {
            dx -= 20;
            dy -= 10;
        }
        
        w += 40;
        h += 20;
    }
    else if(opers.size()>=3 && opers[2] == "collect")
    {
        dx -= 20;
        dy -= 20;
        w += 40;
        h += 40;
    }
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        if(!strcasecmp("SMC_9990013", area.c_str()))
        {
            dy += 40;
            dx += 10;
        }
        else if (!strcasecmp("General_set", area.c_str()))
        {
//            dx -= 85;
//            dy -= 85;
        }
    }
    
    std::string enForcce = CCCommonUtils::getPropById(m_currentId, "enforce");
    std::string arrow = CCCommonUtils::getPropById(m_currentId, "arrow");
    if(enForcce == "1"){
        this->unschedule(schedule_selector(GuideController::clearLock));
        CCRect rect = cocos2d::CCRect(dx, dy, w, h);
        m_gView->addCover(rect, atoi(arrow.c_str()));
    }
    m_gView->setArrow(ccp(dx, dy), w, h, atoi(arrow.c_str()), false);
    
    int tmpRecordStep = atoi(m_currentId.c_str());
    recordStep(CC_ITOA(tmpRecordStep+1));
}

void GuideController::createGuideView(){
    if(m_gView == nullptr){
        m_gView = GuideView::create();
        auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_GUIDE);
        layer->addChild(m_gView);
    }
}

void GuideController::addLevelUpPop(UserUpgradeView *pop){
    removeLevelUpPop();
    pop->retain();
    m_panel = pop;
}

void GuideController::removeLevelUpPop(){
    if(m_panel){
        m_panel->release();
    }
    m_panel = NULL;
}

void GuideController::recordStep(std::string stepId){
    CCCommonUtils::recordStep(stepId);
}

CCNode* GuideController::getNode(std::string str){
    CCNode* node = NULL;
    std::vector<std::string> opers;
    if(str!=""){
        CCCommonUtils::splitString(str,"_",opers);
        if(opers.size()<=1) return NULL;

        if(opers[0]=="UI" && SceneController::getInstance()->currentSceneId != SCENE_ID_BATTLE){//主UI上的引导
            node = UIComponent::getInstance()->getNodeByIndex(str);
        }
                
        if(opers[0] == "MC" && opers.size() == 2){//主城建筑或者地块
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN){
                ImperialScene* layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                if(layer){
                    node = layer->getBuildById(atoi(opers[1].c_str()));
                }
            }
        }
        
        if(opers[0] == "SMC" && opers.size() == 2){//主城特殊建筑
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN){
                ImperialScene* layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                if(layer){
                    node = layer->getBuildById(atoi(opers[1].c_str()));
                }
            }
        }
        
        if(opers[0] == "AC"){//成就面板引导
            AchievementNewPopUpView *achView = dynamic_cast<AchievementNewPopUpView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(achView && achView->isAniComplete()){
                node = achView->getGuideNode(str);
            }
            
            NewPlayerQianDaoView *curView = dynamic_cast<NewPlayerQianDaoView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(str);
            }
        }
        
        if(opers[0] == "plot"){//引导面板区域
            if(m_gView){
                node = m_gView->getGuideNode();
            }
        }
        
        if(opers[0] == "LI"){//建筑建造列表面板
            BuildListView *buildListView = dynamic_cast<BuildListView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(buildListView && buildListView->isAniComplete()){
                if (opers.size()>=2) {
                    node = buildListView->getGuideNode(opers[1]);
                }
            }
        }
        if(opers[0] == "BC"){//建造升级面板
            BuildUpgradeView *curView = dynamic_cast<BuildUpgradeView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                if (opers.size()>=2) {
                    int itemId = atoi(opers[1].c_str());
                    node = curView->getGuideNode(itemId);
                }
            }
        }
        if(opers[0] == "BU" && opers.size()>=3){//点击建筑出现的面板
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN){
                ImperialScene* layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                if(layer){
                    node = layer->getBuildNameById(atoi(opers[1].c_str()), opers[2]);
                }
            }
        }
        
        if(opers[0] == "CZ"){//出征面板
            BattleView *curView = dynamic_cast<BattleView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(str);
            }
        }
        
        if(str == "SO_train"){//训练士兵面板 opers[0] == "SO"
            ProductionSoldiersView *curView = dynamic_cast<ProductionSoldiersView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(str);
            }
        }
        if(str == "SO_back"){//训练士兵中 的 面板 opers[0] == "SO"
            MakingSoldierView *curView = dynamic_cast<MakingSoldierView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = UIComponent::getInstance()->m_popupReturnBtn;
            }
        }
        
        if(str == "TX_15"){//探险面板 opers[0] == "TX"
            MonsterDetailView *curView = dynamic_cast<MonsterDetailView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(str);
            }
        }
        
        if(opers[0] == "SC1" && opers.size()>=2){//科技1级面板
            ScienceListView *curView = dynamic_cast<ScienceListView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                string sType = opers[1];
                node = curView->getGuideNode(sType);
            }
        }
        if(opers[0] == "SC2" && opers.size()>=2){//科技1级（科技树）面板
            if(opers[1] == "out") {
                node = UIComponent::getInstance()->getNodeByIndex(str);
            }
            else {
                ScienceView *curView = dynamic_cast<ScienceView*>(PopupViewController::getInstance()->getCurrentPopupView());
                if(curView && curView->isAniComplete()){
                    int sid = atoi(opers[1].c_str());
                    node = curView->getGuideNode(sid);
                }
            }
        }
        if(opers[0] == "SC3" && opers.size()>=2){//科技3级（科技升级）面板
            SciencePopupView *curView = dynamic_cast<SciencePopupView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                int sid = atoi(opers[1].c_str());
                node = curView->getGuideNode(sid);
            }
        }
        if(opers[0] == "WL" && opers.size()>=2){
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                node = WorldMapView::instance()->getNodeByType(opers[1]);
            }
        }
        if(opers[0] == "WR" && opers.size()>=2){
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                node = WorldMapView::instance()->getNodeByType(opers[1]);
            }
        }
        if(opers[0] == "ED" && opers.size()>=2){
            BattleView *curView = dynamic_cast<BattleView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(opers[1]);
            }
        }
        if(opers[0] == "GD" && opers.size()>=2){
            StoreView *curView = dynamic_cast<StoreView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(str);
            }
        }
        if(opers[0] == "FR1" && opers.size()>=2){
            EquipSiteView *curView = dynamic_cast<EquipSiteView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(str);
            }
        }
        if(opers[0] == "FR2" && opers.size()>=2){
            EquipmentListView *curView = dynamic_cast<EquipmentListView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(str);
            }
        }
        if(opers[0] == "FR3" && opers.size()>=2){
            EquipmentCreateView *curView = dynamic_cast<EquipmentCreateView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(str);
            }
        }
        if(str == "NEW_reward"){
            NewPlayerQianDaoView *curView = dynamic_cast<NewPlayerQianDaoView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(str);
            }
        }
        
        if(str == "General_set"){
            GeneralsPopupView *curView = dynamic_cast<GeneralsPopupView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(str);
            }
        }
        if(str == "Set_bangding"){
            SettingPopUpView *curView = dynamic_cast<SettingPopUpView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(str);
            }
        }
        if(str == "SetAccount_bangding"){
            SetAccountView *curView = dynamic_cast<SetAccountView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                node = curView->getGuideNode(str);
            }
        }
    }
    
    if(node == NULL || node->getParent() == NULL){
        node = NULL;
    }else{
        CCNode *tmp = node;
        while (tmp->getParent()) {
            if(!tmp->isVisible()){
                node = NULL;
                break;
            }
            tmp = tmp->getParent();
        }
    }
    return node;
}

std::string GuideController::getCurrentId(){
    return m_currentId;
}

void GuideController::startScienceGuide(){
    GuideController::share()->openScienceFlag = true;
    GuideController::share()->runAction(CCSequence::create(CCDelayTime::create(0.1f)
                                       , CCCallFuncN::create(GuideController::share(), callfuncN_selector(GuideController::goToMainScence))
                                       , NULL
                                       ));
}

void GuideController::goToMainScence(CCNode* p){
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN){
        PopupViewController::getInstance()->removeAllPopupView();
        openSciencePanel();
    }else{
        int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SCIENE);
        if(bid == 0){
            SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
        }else{
            openSciencePanel();
        }
    }
}

void GuideController::openSciencePanel(){
    if(!openScienceFlag){
        return;
    }
    openScienceFlag = false;
    int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SCIENE);
    if(bid == 0){
        CCCommonUtils::flyHint("", "", _lang("108866"));
        return;
    }
    auto& bInfo = FunBuildController::getInstance()->getFunbuildById(bid);
    if (bInfo.state != FUN_BUILD_NORMAL) {
        CCCommonUtils::flyHint("", "", _lang("121988"));
        return;
    }
    
    auto popUp = ScienceView::create(0, CapTecIndex);
    int index = 0;
    while(index < _capTech.size()){
        std::string sid = _capTech[index];
        if(ScienceController::getInstance()->getScienceLevel(atoi(sid.c_str())) == 0){
            popUp->setAttentionTo(atoi(sid.c_str()));
            break;
        }
        index++;
    }
    
    PopupViewController::getInstance()->addPopupInView(popUp);
}

int GuideController::getWillBuildItemId()
{
    if (m_currentId!="") {
        std::string areaStr = CCCommonUtils::getPropById(m_currentId, "area");
        std::string next = CCCommonUtils::getPropById(m_currentId, "next");
        std::vector<std::string> opers;
        CCCommonUtils::splitString(areaStr,"_",opers);
        if(opers.size() >= 2 && opers[0] == "MC"){//主城建筑或者地块
            if (next!="") {
                std::string nextAreaStr = CCCommonUtils::getPropById(next, "area");
                std::vector<std::string> nextOpers;
                CCCommonUtils::splitString(nextAreaStr,"_",nextOpers);
                if (nextOpers.size()>=2 && nextOpers[0] == "LI") {
                    int ret = atoi(nextOpers[1].c_str());
                    return ret;
                }else {
                    return 0;
                }
            }else {
                return 0;
            }
        }
        else {
            return 0;
        }
    }
    return 0;
}

std::string GuideController::checkNetStep(std::string guideId)
{
    std::string ret = guideId;
    std::string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
    if (gFake == "end_3" || gFake == "start_4")//建造完3个建筑
    {
        int tmpQid1 = QueueController::getInstance()->getMinTimeQidByType(TYPE_RIDE_SOLDIER);//骑兵队列 待秒cd
        int tmpQid2 = QueueController::getInstance()->getCanRecQidByType(TYPE_RIDE_SOLDIER);//骑兵队列 待收兵
        if (m_netSaveStep == GUIDE_TROOP_CRT_ED)//建造过兵了
        {
            if (tmpQid2 != QID_MAX) {
//                ret = GUIDE_TROOP_GET;
            }else if (tmpQid1 != QID_MAX) {
//                ret = GUIDE_TROOP_CD_ST;
            }
        }
        else if (m_netSaveStep == GUIDE_TROOP_CD_ED) {//秒过cd了
            if (tmpQid2 != QID_MAX) {
//                ret = GUIDE_TROOP_GET;
            }
        }
        else if (m_netSaveStep == GUIDE_TROOP_GET) {//收过兵了
            
        }
        
        if(GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0){
            int totoalArmy = ArmyController::getInstance()->getTotalArmy();
            if (totoalArmy > 0) {
                ret = GUIDE_BUILD_4;
            }else if (tmpQid2 != QID_MAX) {
//                ret = GUIDE_TROOP_GET;
            }else if (tmpQid1 != QID_MAX) {
//                ret = GUIDE_TROOP_CD_ST;
            }
        }
        
    }
    else if (gFake == "end_4")//建造完4个建筑
    {
        if (m_netSaveStep == GUIDE_TROOP_GET || m_netSaveStep == GUIDE_TROOP_CD_ED) {//收过兵了
            ret = GUIDE_REWD_ST;
        }
        else if (m_netSaveStep == GUIDE_REWD_ED) {//领过奖励了
            
        }
        else if (m_netSaveStep == GUIDE_BATTLE_ED && ret == GUIDE_BATTLE_ST) {
            ret = GUIDE_HAVEST_ST;
        }
        
        if (CCCommonUtils::isTestPlatformAndServer("guide_3"))
        {
            if (m_netSaveStep == GUIDE_BATTLE_ED && ret!="") {//出征过 去领新手7日奖励
                ret = GUIDE_NEW_RD_ST;
            }
            else if (m_netSaveStep == GUIDE_NEW_RD_ED) {//领完新手7日奖励 去丰收
            }
        }
    }
    
    return ret;
}

std::string GuideController::checkOpenSpeGuide()
{
    std::string ret = "";
    std::string speGuide = CCUserDefault::sharedUserDefault()->getStringForKey(GUIDE_MAINCITY_LEVEL_UP,"");
    if(speGuide != "")
    {
        vector<string> tmpVec;
        CCCommonUtils::splitString(speGuide, "_", tmpVec);
        if (tmpVec.size()==2 && tmpVec[0]==GlobalData::shared()->playerInfo.uid && tmpVec[1]=="5") {
            int maincityLv = FunBuildController::getInstance()->getMainCityLv();
            if (maincityLv == 6) {//引导旅行商人
                ret = GUIDE_MERCHANTE_ST;
            }
        }
    }
    
    return ret;
}

void GuideController::clearGuideData()
{
    CCUserDefault::sharedUserDefault()->setStringForKey(GUIDE_MAINCITY_LEVEL_UP, "");
    std::string stepKey = GlobalData::shared()->playerInfo.uid + GUIDE_STEP;
    CCUserDefault::sharedUserDefault()->setStringForKey(stepKey.c_str(), "");
    CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","");
    CCUserDefault::sharedUserDefault()->flush();
}