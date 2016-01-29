//
//  GameController.cpp
//  IF
//
//  Created by 邹 程 on 13-9-2.
//
//

#include "GameController.h"
#include "GlobalData.h"
#include "../Ext/CCDevice.h"
#include "SceneController.h"
#include "ParticleController.h"
#include "UIComponent.h"
#include "BattleManager.h"
#include "InitCommand.h"
#include "PayController.h"
#include "UserUpgradeView.h"
#include "GuideController.h"
#include "NetController.h"
#include "YesNoDialog.h"
#include "FBUtilies.h"
#include "PushRecordCommand.h"
#include "PushBase.h"
#include "AppLibHelper.h"

bool g_bQuitGame = false;

bool IsQuitGame()
{
    return g_bQuitGame;
}

void TimeStampObserver::doTimeTick(int timeLeft){
    if (m_target && (timeLeft >= 0)) {
        (m_target->*m_timeTick)(m_objectTick, timeLeft);
    }
}

void TimeStampObserver::doTimeFinish(){
    if (m_target) {
        (m_target->*m_timeFinish)(m_objectFinish);
    }
}

CCObject *TimeStampObserver::getTarget() const{
    return m_target;
}

SEL_TIMETICK TimeStampObserver::getTimeTick() const{
    return m_timeTick;
}

SEL_TIMEFINISH TimeStampObserver::getTimeFinish() const{
    return m_timeFinish;
}

CCObject *TimeStampObserver::getObjectTick() const{
    return m_objectTick;
}

CCObject *TimeStampObserver::getObjectFinish() const{
    return m_objectFinish;
}




static GameController *_instance = NULL;

GameController* GameController::getInstance() {
    if (!_instance) {
        _instance = new GameController();
    }
    return _instance;
}

void GameController::purgeData() {
    if(getInstance()->m_manager)
        getInstance()->m_manager->setDelegate(NULL);
    delete _instance;
    _instance = NULL;
}

void GameController::setLoadingLog(string _class,string _func)
{
    //loadingLog统计
    m_loadingLog = m_loadingLog + CCString::createWithFormat("%s:%s=%ld; ",_class.c_str(),_func.c_str(),(millisecondNow() - m_timeStep))->getCString();
    m_timeStep = millisecondNow();
    CCLOG("loadingLog:%s",m_loadingLog.c_str());
}

long GameController::millisecondNow()
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    //struct timeval now;
    //gettimeofdayCocos2d(&now, NULL);
    return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

GameController::GameController():_logseq(0),m_timeStep(millisecondNow()),m_connectRetryCount(0),m_manager(NULL)
{
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameController::onTime), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameController::doUserLevelUp), MSG_PLAYER_LEVELUP, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameController::doCmdConnectionLost), MSG_CMD_CONNECTION_LOST, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameController::doFlyText), MSG_FLY_TEXT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameController::doAppForegroundEvent), MSG_APP_FOREGROUND, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameController::onConnectError), SFS_CONNECT_ERROR, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameController::changeAccount), "xiaomiChangeAccount", NULL);

}

GameController::~GameController() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "xiaomiChangeAccount");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCENE_CHANGED);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PLAYER_LEVELUP);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CMD_CONNECTION_LOST);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FLY_TEXT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_APP_FOREGROUND);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GameController::onTime), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GameController::onTimer_remove), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GameController::checkPurchaseInfoList), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GameController::redoPushData), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SFS_CONNECT_ERROR);
}

// smartfox连接失败
void GameController::onConnectError(cocos2d::CCObject *obj)
{
    CCLOG("GameController::onConnectError");
    NetController::shared()->disconnect();
    // 切换备用端口
    if (NetController::shared()->getPort() == SERVER_PORT) {
        NetController::shared()->setPort(9933);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, 9933);
    } else {
        NetController::shared()->setPort(SERVER_PORT);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, SERVER_PORT);
    }
    
    std::string ip = "";
    if(NetController::shared()->m_ipConnectTime % 6 == 4 || NetController::shared()->m_ipConnectTime % 6 == 5){
        ip = NetController::getIpByType(1);
    }else{
        ip = NetController::getIpByType(0);
    }
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
    NetController::shared()->setIp(ip);
    NetController::shared()->m_ipConnectTime++;
    CCUserDefault::sharedUserDefault()->flush();

    if (m_connectRetryCount == 1) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_LOADINGS_FQA);
    }
    /**
     * 避开同一次 Main Loop 中schedule selector不被二次执行的问题。
    */
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameController::delayConnect), this, 0.0f,0, 0.2f, false);
}

void GameController::delayConnect(float dt)
{
    CCLOG("GameController::delayConnect");
    m_connectRetryCount++;
    NetController::shared()->connect();
}

void GameController::doAppForegroundEvent(cocos2d::CCObject *obj)
{
    CCDirector::sharedDirector()->setVisitFlag(true);
    if(!GuideController::share()->isInTutorial() && !GlobalData::shared()->isBind && !GlobalData::shared()->isUploadPic){
        PopupViewController::getInstance()->removeAllPopupView();
    }
    if (!GlobalData::shared()->isBind
        && !GlobalData::shared()->isPayBind
        && SceneController::getInstance()->currentSceneId != SCENE_ID_LOADING && GlobalData::shared()->getWorldTime() - GlobalData::shared()->pauseTime > 60) {
        if (NetController::shared()->isConnected()) {
            LogoutCommand* cmd = new LogoutCommand();
            cmd->sendAndRelease();
            CCLOG("doAppForegroundEvent -- LogoutCommand");
        }
        else
        {
            LogoutCommand* cmd = new LogoutCommand();
            CCDictionary* _params = CCDictionary::create();
            _params->setObject(CCString::create("cok"), "zoneName");
            _params->setObject(CCString::create("true"), "isReLogin");
            CCDictionary* dict = CCDictionary::create();
            dict->setObject(CCString::create("logout"), "cmd");
            dict->setObject(_params, "params");
            cmd->handleRecieve(dict);
            CCLOG("doAppForegroundEvent -- LogoutCommand->handleRecieve");
        }
        
//        GameController::getInstance()->removeWaitInterface();
    }
    else if(GlobalData::shared()->playerInfo.uid != "" && !GlobalData::shared()->isPayBind)
    {
        NetController::shared()->clearRequestQueue();
        InitCommand* cmd = new InitCommand();
        cmd->sendAndRelease();
//        GameController::getInstance()->removeWaitInterface();
        //从后台切到前台，可能是因为点击推送
        GameController::getInstance()->recordPushData();
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (GlobalData::shared()->isGoogle()) {
        PayController::getInstance()->initGoogle();
    }
#endif
    GlobalData::shared()->isBind = false;
    GlobalData::shared()->isPayBind = false;
    if(GlobalData::shared()->pushData != NULL){
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameController::redoPushData), this, 5.0f, 1, 0.0f, false);
    }
}

void GameController::doCmdConnectionLost(cocos2d::CCObject *obj)
{
    
    YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(GameController::reConnect), NULL),_lang("105248").c_str());
    
//    auto topLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP);
//    CCCommonUtils::flyTextOnParent(_lang("E100048").c_str(), topLayer, ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2));
}

void GameController::reConnect(CCObject* obj)
{
    NetController::shared()->connect();
}

void GameController::doFlyText(cocos2d::CCObject *obj)
{
    CCString* tmp = dynamic_cast<CCString*>(obj);
    auto topLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP);
    if(tmp && topLayer) {
        CCCommonUtils::flyTextOnParent(tmp->getCString(), topLayer, ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2));
    }
}

void GameController::addTimeObserver(TimeStampType timeType, cocos2d::CCObject *target, SEL_TIMETICK timeTick,SEL_TIMEFINISH timeFinish,CCObject *objTick,CCObject *objFinish) {
    if (timeObserverExisted(timeType, target)) {
        CCLOG("GameController addTimeObserver ALREADY EXISTED : name: %s for type: %d ,observer total count: %lu",typeid(*target).name(),(int)timeType,timeObservers.size());
        return;
    }
    timeObservers.insert(pair<TimeStampType, TimeStampObserver>(timeType,TimeStampObserver(target,timeTick,timeFinish,objTick,objFinish)));
    CCLOG("GameController addTimeObserver: name: %s for type: %d ,observer total count: %lu",typeid(*target).name(),(int)timeType,timeObservers.size());
}

void GameController::removeTimeObserver(TimeStampType timeType, cocos2d::CCObject *target) {
    auto equal_range = timeObservers.equal_range(timeType);
    for (auto it = equal_range.first; it != equal_range.second;++it) {
        if ((it->second).getTarget() == target) {
            timeObservers.erase(it++);
            CCLOG("GameController removeTimeObserver: name: %s for type: %d ,observer total count: %lu",typeid(*target).name(),(int)timeType,timeObservers.size());
            return;
        }
    }
    CCLOG("GameController removeTimeObserver NOT FOUND : name: %s for type: %d ,observer total count: %lu",typeid(*target).name(),(int)timeType,timeObservers.size());
}

void GameController::changeAccount(CCObject *obj){
    auto doLogOut = [](){
        NetController::shared()->clearAllRequest();
        LogoutCommand* cmd = new LogoutCommand();
        CCDictionary* _params = CCDictionary::create();
        _params->setObject(CCString::create("cok"), "zoneName");
        _params->setObject(CCString::create("true"), "isReLogin");
        _params->setObject(CCString::create("1"), "xiaomi");
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(CCString::create("logout"), "cmd");
        dict->setObject(_params, "params");
        cmd->handleRecieve(dict);
    };
    int currentSceneId = SceneController::getInstance()->currentSceneId;
    if(currentSceneId == SCENE_ID_LOADING){
        if(GlobalData::shared()->isInDataParsing || GlobalData::shared()->isInitFlag){//已经开始解析数据
            doLogOut();
        }else{
            NetController::shared()->disconnect();
            SceneController::getInstance()->gotoLoading(true);
        }
    }else{
        doLogOut();
    }
}


int GameController::timeObserverCount() {
    return timeObservers.size();
}

bool GameController::timeObserverExisted(TimeStampType timeType, cocos2d::CCObject *target) {
    auto equal_range = timeObservers.equal_range(timeType);
    for (auto it = equal_range.first; it != equal_range.second; ++it) {
        if ((it->second).getTarget() == target) {
            return true;
        }
    }
    return false;
}

void GameController::timeStampTick(TimeStamp &timeStamp, int leftTime) {
    auto equal_range = timeObservers.equal_range(timeStamp.type);
    for (auto it = equal_range.first; it != equal_range.second; ++it) {
        if ((it->second).getTarget() && (it->second).getTimeTick()) {
            (it->second).doTimeTick(leftTime);
        }
    }
}

void GameController::timeStampFinish(TimeStamp &timeStamp) {
    auto equal_range = timeObservers.equal_range(timeStamp.type);
    for (auto it = equal_range.first; it != equal_range.second;) {
        if ((it->second).getTarget() && (it->second).getTimeFinish()) {
            (it->second).doTimeFinish();
            timeObservers.erase(it++);
        }
    }
}

void GameController::onTime(float dt) {
    auto &stampMap = GlobalData::shared()->timeStampMap;
    auto now = GlobalData::shared()->getWorldTime();
    for (auto it = stampMap.begin();it != stampMap.end();) {
        if (it->second.finishTime <= 0) {
            // clear finished time stamp
            stampMap.erase(it++);
            continue;
        }
        int leftTime = it->second.finishTime - now;
        if (leftTime < 0) {
            // finish time stamp
            timeStampFinish(it->second);
            stampMap.erase(it++);
        } else {
            // tick time stamp
            timeStampTick(it->second, leftTime);
            ++it;
        }
    }
}

void GameController::quitGame(CCObject* p){
    CCLOG("game over");
    if(NetController::shared()->isConnected())
    {
        NetController::shared()->disconnect();
    }
    g_bQuitGame = true;
    cocos2d::extension::CCDevice::finishGame();
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameController::redoPushData(float dt){
    if(GlobalData::shared()->pushData != NULL){
        PushBase::handleResponse(GlobalData::shared()->pushData);
        CC_SAFE_RELEASE(GlobalData::shared()->pushData);
        GlobalData::shared()->pushData = NULL;
    }
}

void GameController::enableQueryHistoryPurchase(){
    if(!queryHistoryPurchaseEnabled){
        queryHistoryPurchaseEnabled = true;
        //进入主场景以后每10秒执行一次
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameController::checkPurchaseInfoList), this, 5.0f, kCCRepeatForever, 0.0f, false);
    }
}

void GameController::checkPurchaseInfoList(float dt){
    //当连上服务器以后，先处理本地缓存订单，再调用平台接口查询需要处理的订单
    if (NetController::shared()->isConnected()) {
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GameController::checkPurchaseInfoList), this);
        PayController::getInstance()->checkPurchaseInfoList();
        queryHistoryPurchaseEnabled = false;
    }
}


void GameController::platformQuit(){
    cocos2d::extension::CCDevice::doPlatformQuit();
}
void GameController::platformCollectUserInfo(const string &userId,const string &userName,const std::string &userLevel){
    cocos2d::extension::CCDevice::doPlatformCollectUserInfo(userId,userName,userLevel);
}

void GameController::recordPushData(){
    string pushRecord = cocos2d::extension::CCDevice::getPushRecordData();
    string pushClick = cocos2d::extension::CCDevice::getPushClickData();
    if(pushRecord != "" || pushClick != ""){
        PushRecordCommand* cmd = new PushRecordCommand(pushRecord,pushClick);
        cmd->sendAndRelease();
    }
}


#pragma mark - WaitInterface

void GameController::showWaitInterface()
{
    CCLOGFUNC();
    if(m_waitInterface)
        return;
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    m_waitInterface = WaitInterface::show(CCDirector::sharedDirector()->getRunningScene(), CCPoint(size.width/2,size.height/2), size);

    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameController::onTimer_remove), this, 60.0f, false);
}

void GameController::removeWaitInterface()
{
    CCLOGFUNC();
    if (m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface = NULL;
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GameController::onTimer_remove), this);
    }
}

void GameController::onTimer_remove(float f)
{
    removeWaitInterface();
}

WaitInterface* GameController::showWaitInterface(CCNode* targetNode,int opacity)
{
    CCLOGFUNC();

    CCSize size = CCSize(targetNode->getContentSize().width * targetNode->getScaleX(), targetNode->getContentSize().height * targetNode->getScaleY());
    CCPoint pos = targetNode->getPosition();
    CCNode* _parent = dynamic_cast<CCSpriteBatchNode*>(targetNode->getParent());
    if (_parent) {
        _parent = targetNode->getParent()->getParent();
    }
    else
    {
        _parent = targetNode->getParent();
    }
    return WaitInterface::show(_parent, pos, size, opacity);
    
}

void GameController::updateVersion(CCObject* p)
{
    CCLOG("version-url:%s",GlobalData::shared()->downloadUrl.c_str());
    cocos2d::extension::CCDevice::updateVersion(GlobalData::shared()->downloadUrl.c_str());
}
void GameController::goTurntoUrl(string url){
    cocos2d::extension::CCDevice::updateVersion(url);
}
void GameController::gotoFaceBookLike()
{
    CCLOG("version-url:%s","https://www.facebook.com/Dragon-Clans-1484362238534405");
    cocos2d::extension::CCDevice::updateVersion("https://www.facebook.com/Dragon-Clans-1484362238534405");
}

void GameController::callXCApi(string data){
    string url = XINGCLOUD_URl;
    url.append(GlobalData::shared()->playerInfo.uid).append("?").append(data);
    sendURLRequest(url);
}

void GameController::trackVisit(){
    string data("");
    data.append("action=visit&update=ref,xafrom=")
    .append(GlobalData::shared()->lang).append(";")
    .append("COK").append("@").append(GlobalData::shared()->platform).append(";")
    .append(GlobalData::shared()->analyticID).append(";")
    .append(GlobalData::shared()->version);
    callXCApi(data);
    trackUserLocale();
    
}

void GameController::trackUserLocale(){
    string accountZone = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_ZONE,"");
    //&update1=identifier,s7&update2=language,en&update3=platform,android
    string data("update0=language,");//{country}&timestamp={timestamp}";
    data.append(GlobalData::shared()->lang)
    .append("&timestamp=").append(CC_ITOA(GlobalData::shared()->getWorldTime()))
    .append("&update1=identifier,").append(accountZone)
    .append("&update2=platform,").append(GlobalData::shared()->platform);
    callXCApi(data);
}

void GameController::sendURLRequest(string url)
{
    CCLOG("xingcloud URL :%s", url.c_str());
    CCHttpRequest* pHttpRequest = new CCHttpRequest();
    pHttpRequest->setUrl(url.c_str());
    pHttpRequest->setRequestType(CCHttpRequest::Type::POST);
    string tag;
    tag.append("log");
    tag.append(CC_ITOA(_logseq++));
    pHttpRequest->setTag(tag.c_str());
    CCHttpClient::getInstance()->send(pHttpRequest);
    CC_SAFE_RELEASE(pHttpRequest);
}

void GameController::doUserLevelUp(cocos2d::CCObject *obj)
{
    if (SceneController::getInstance()->currentSceneId == SCENE_ID_BATTLE || BattleManager::shared()->isDuringRequestingBattleInfo) {
        // in battle , delay display
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameController::delayLevelUp), MSG_SCENE_CHANGED, NULL);
    } else {
        delayLevelUp(NULL);
    }
    
    return;
    auto giftArr = _dict(obj);
    if (giftArr && giftArr->count()) {
        auto has_gift = dynamic_cast<CCBool*>(giftArr->objectForKey("has_gift"));
        if (!has_gift)
            return;
        auto g1 = dynamic_cast<CCInteger*>(giftArr->objectForKey("gift1"));
        auto g2 = dynamic_cast<CCInteger*>(giftArr->objectForKey("gift2"));
        if (!g1 && !g2) {
            return;
        }
        if (g1) {
            m_userLevelUpGift1 = g1->getValue();
        } else {
            m_userLevelUpGift1 = 0;
        }
        if (g2) {
            m_userLevelUpGift2 = g2->getValue();
        } else {
            m_userLevelUpGift2 = 0;
        }
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_BATTLE || BattleManager::shared()->isDuringRequestingBattleInfo) {
            // in battle , delay display
            CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameController::delayLevelUp), MSG_SCENE_CHANGED, NULL);
        } else {
            delayLevelUp(NULL);
        }
    }
}

void GameController::delayLevelUp(cocos2d::CCObject *obj)
{
    if (SceneController::getInstance()->currentSceneId == SCENE_ID_BATTLE || BattleManager::shared()->isDuringRequestingBattleInfo) {
        return;
    }
    UserUpgradeView *pop = UserUpgradeView::create();
    if(GuideController::share()->isInTutorial()){//新手引导中 不显示升级面板
        GuideController::share()->addLevelUpPop(pop);
    }else{
        if (PopupViewController::getInstance()->getCurrViewCount()==0) {
            PopupViewController::getInstance()->addPopupView(pop);
        }
        else {//有其他面板打开时 不显示升级面板
            PopupViewController::getInstance()->pushPop(pop);
        }
    }
    
//    if (PopupViewController::getInstance()->pushPop(pop)) {
//        PopupViewController::getInstance()->addPopupView(pop);
//    }

    return;
    
    /////
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCENE_CHANGED);
    //CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(false);
    
    int heightFix = 80;
    
    auto tmpNode = CCNode::create();
    tmpNode->setTag(CONGRATULATION_TAG);
    CCDirector::sharedDirector()->getRunningScene()->addChild(tmpNode);
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    auto bg = CCLayerColor::create({0,0,0,1}, winSize.width, winSize.height);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setOpacity(80);
    tmpNode->addChild(bg);

    auto sp = CCLoadSprite::createSprite("congratulation.png");
    sp->setAnchorPoint(ccp(0.5,0.5));
    sp->setPosition(ccp(winSize.width/2, winSize.height/2 + 100));
    tmpNode->addChild(sp);
    
    Vector<FiniteTimeAction*> actionList;
    actionList.pushBack(CCCallFuncO::create(this, callfuncO_selector(GameController::decorateLevelUp), CCInteger::create(4)));
    actionList.pushBack(CCSpawn::createWithTwoActions(CCSequence::create(CCFadeIn::create(0.1),NULL),
                                                        CCSequence::create(CCDelayTime::create(0.05),
                                                                           CCCallFuncO::create(this, callfuncO_selector(GameController::decorateLevelUp), CCInteger::create(1)),
                                                                           CCDelayTime::create(0.1),
                                                                           CCCallFuncO::create(this, callfuncO_selector(GameController::decorateLevelUp), CCInteger::create(2)),
                                                                           CCDelayTime::create(0.05),
                                                                           CCCallFuncO::create(this, callfuncO_selector(GameController::decorateLevelUp), CCInteger::create(3)),
                                                                           NULL)
                                                        ));
    
    auto level = CCLabelIF::create(_lang_1("105239", CC_ITOA(GlobalData::shared()->playerInfo.level)),52);
    level->enableStroke(ccBLACK, 1.0);
    level->setAnchorPoint(ccp(0.5, 0.5));
    level->setPosition(ccp(winSize.width/2, winSize.height/2));
    tmpNode->addChild(level);
    level->setColor({255,207,61});
    level->setOpacity(0);
    
    actionList.pushBack(CCTargetedAction::create(level, CCFadeIn::create(1)));
    
    if (m_userLevelUpGift1 > 0) {
        auto gift1 = CCLabelIF::create(_lang_1("105240", CC_ITOA(m_userLevelUpGift1)),36);
        gift1->enableStroke(ccBLACK, 1.0);
        gift1->setAnchorPoint(ccp(0.5, 0.5));
        gift1->setPosition(ccp(winSize.width/2, winSize.height/2 - heightFix));
        tmpNode->addChild(gift1);
        gift1->setOpacity(0);
        gift1->setColor({255,207,61});
        heightFix += 50;
        
        actionList.pushBack(CCTargetedAction::create(gift1, CCFadeIn::create(1)));
    }
    if (m_userLevelUpGift2 > 0) {
        auto gift2 = CCLabelIF::create(_lang_1("105241", CC_ITOA(m_userLevelUpGift2)),36);
        gift2->enableStroke(ccBLACK, 1.0);
        gift2->setAnchorPoint(ccp(0.5, 0.5));
        gift2->setTag(AP_REWARD_TAG);
        gift2->setPosition(ccp(winSize.width/2, winSize.height/2 - heightFix));
        tmpNode->addChild(gift2);
        gift2->setOpacity(0);
        gift2->setColor({255,207,61});
        heightFix += 50;
        
        actionList.pushBack(CCTargetedAction::create(gift2, CCFadeIn::create(1)));
    }
    
    sp->runAction(CCSequence::create(actionList));
}

void GameController::addEnergyForLevelUpFinish(cocos2d::CCObject *obj) {
    //CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(true);
//    setTouchEnabled(false);
    CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(CONGRATULATION_TAG,true);
//    UIComponent::getInstance()->addEnergyUpdate();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_ENERGY_UPDATE,CCBoolean::create(true));
}

void GameController::decorateLevelUp(cocos2d::CCObject *obj) {
    int step = dynamic_cast<CCInteger*>(obj)->getValue();
    auto runningScene = CCDirector::sharedDirector()->getRunningScene();
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto particle = ParticleController::createFightingParticle("explode",0.03);
    switch (step) {
        case 1:{
            particle->setPosition(ccp(winSize.width/2 - 300, winSize.height/2 + 100));
            particle->setDuration(0.04);
        }
            break;
        case 2:{
            particle->setPosition(ccp(winSize.width/2 + 160, winSize.height/2 - 40));
            particle->setDuration(0.2);
        }
            break;
        case 3:{
            particle->setPosition(ccp(winSize.width/2 + 280, winSize.height/2 + 120));
            particle->setDuration(0.1);
        }
            break;
            
        default: {
            //CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(true);
            //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -127, true);
        }
            return;
    }
    runningScene->addChild(particle);
}

bool GameController::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    return true;
}

void GameController::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    auto tmpNode = CCDirector::sharedDirector()->getRunningScene()->getChildByTag(CONGRATULATION_TAG);
    if (tmpNode && tmpNode->getChildByTag(AP_REWARD_TAG)) {
        auto gift2 = tmpNode->getChildByTag(AP_REWARD_TAG);
        // add flyingBall effect
        auto ballPosition = gift2->getPosition();
        auto uiPosition = ccp(0,0);//UIComponent::getInstance()->getEnergyPosition();
        auto ball = CCLoadSprite::createSprite("upgrade_adding_effect.png");
        ball->setPosition(ballPosition);
        ball->setOpacity(0);
        tmpNode->addChild(ball);
        //CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(false);
        tmpNode->runAction(CCSequence::create(CCTargetedAction::create(gift2, CCFadeOut::create(0.3)),
                                              CCTargetedAction::create(ball, CCFadeIn::create(0.3)),
                                              CCTargetedAction::create(ball, CCMoveTo::create(0.3, uiPosition)),
                                              CCTargetedAction::create(ball, CCFadeOut::create(0.2)),
                                              CCCallFuncO::create(this, callfuncO_selector(GameController::addEnergyForLevelUpFinish), NULL),
                                              NULL));

    } else {
//        setTouchEnabled(false);
        CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(CONGRATULATION_TAG,true);
    }
}

void GameController::addMidnightObserver(float t) {
    // init midnight time
    time_t now = GlobalData::shared()->getWorldTime();
    time_t nowCache = now;
    
    tm *test_time = localtime(&now);
    nowCache -= test_time->tm_gmtoff;
    tm *time = localtime(&nowCache);
    time->tm_hour = 23;
    time->tm_min = 59;
    time->tm_sec = 59;
    time_t midnight = mktime(time);
    
    // init timeStamp
    auto timeStampForever = TimeStamp();
    timeStampForever.type = Stamp_MIDNIGHT_UPDATE;
    timeStampForever.finishTime = now + midnight - nowCache;
    GlobalData::shared()->timeStampMap[1] = timeStampForever;
    
    //init pvp fight times
//    ArenaControl::getInstance()->m_fightNum = ArenaControl::getInstance()->m_arena_base["k8"];
    
    // add observer
    GameController::getInstance()->addTimeObserver(Stamp_MIDNIGHT_UPDATE, GameController::getInstance(), NULL,timeFinish_selector(GameController::midnightUpdate));
}

void GameController::midnightUpdate(cocos2d::CCObject *obj) {
    
    // update for pve npc defeat count
//    auto &allCheckPoints = GlobalData::shared()->pveSceneInfo.checkPoints;
//    for (auto &sceneCheckPoints:allCheckPoints) {
//        for (auto &checkPoint:sceneCheckPoints.second) {
//            checkPoint.second.midnightUpdate = true;
//        }
//    }
    //每日抽将个数更新
    GlobalData::shared()->generalConfig.oneTimeFree = GlobalData::shared()->generalConfig.freePerDay;
//    // update for login reward
//    auto &loginReward = GlobalData::shared()->loginReward;
//    loginReward.canDoLottery = true;
//    if (SceneController::getInstance()->currentSceneId != SCENE_ID_BATTLE) {
//        UIComponent::getInstance()->setLoginRewardVisible(true);
//    }
    
    // todo : add other update here
    GlobalData::shared()->lordInfo.feed_count = 0;//feed分享次数每日清0
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameController::addMidnightObserver), this, 3.0f,0, 3.0f, false);
}

void GameController::onError(UpdateManager::UMErrorCode errorCode) {
    CCLog("$update error code:%d",errorCode);
    CCLog("$xml version:%s",GlobalData::shared()->xmlVersion.c_str());
    CC_SAFE_DELETE(m_manager);
}

void GameController::onProgress(int percent) {
//    CCLOG("$update progress:%d",percent);
}

void GameController::onSuccess() {
    CCLOG("$update success");
    CCLog("$xml version:%s",GlobalData::shared()->xmlVersion.c_str());
    CC_SAFE_DELETE(m_manager);
}
