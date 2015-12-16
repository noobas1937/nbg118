//
//  LoadingScene.cpp
//  IF
//
//  Created by 吴 凌江 on 13-8-28.
//
//

#include "LoadingScene.h"
#include "../../Ext/CCDevice.h"
#include "SceneController.h"
#include "SoundController.h"
#include "LoginCommand.h"
#include "PopupViewController.h"
#include "KoreaPermitPopUPView.h"
#include "AppLibHelper.h"
#include "NetController.h"
#include "YesNoDialog.h"
#include "ParticleController.h"
#include "InitCommand.h"
#include "HelpshiftCocos2dx.h"
#include "SuggestionView.h"
#include <spine/Json.h>
#include "IFLoadingSceneArmyNode.h"
#include "SaoGuangEffectSprite.h"
#include "FunBuildController.h"
#include "IFSkeletonDataManager.h"
#include "md5.h"
#include "NBDLCController.hpp"

#define MSG_MAIN_THREAD_XML 453
#define LOADING_STEP        80
#define LOADING_LOGO_TAG    666
#define LOADING_1 667
#define LOADING_2 668
#define LOADING_3 669
#define LOADING_4 670

LoadingScene::LoadingScene()
: m_steps(LOADING_STEP)
, m_platformLoginFlag(false)
{
}

LoadingScene::~LoadingScene()
{
}

#pragma mark * init functions

void LoadingScene::addLoadingBG(Point& addPt)
{
    Size size = Director::getInstance()->getWinSize();
    
    if (SceneController::getInstance()->showBG)
    {
        SceneController::getInstance()->showBG->setPosition(ccp(size.width/2, size.height/2));
        addChild(SceneController::getInstance()->showBG);
        addPt = ccp(0, size.height / 2);
        
        auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 100), size.width, size.height);
        colorLayer->setAnchorPoint(ccp(0, 0));
        addChild(colorLayer);
    }
    else
    {
        auto loadingBG = CCLoadSprite::createSprite("loading.png");
        loadingBG->setAnchorPoint(ccp(0.5, 0.5));
        loadingBG->setPositionX(size.width/2);
        loadingBG->setPositionY(size.height/2);
        
        this->addChild(loadingBG);

//        auto kingSpine = IFLoadingSceneArmyNode::create("Loading/Loading_3.atlas", "Spine/Loading/loading.json", "loop", 1);
//        this->addChild(kingSpine);
    }
}

void LoadingScene::addVersionLabel()
{
    if (true) {
        return;
    }
    string _uuid = cocos2d::extension::CCDevice::getDeviceUid();
    GlobalData::shared()->version = cocos2d::extension::CCDevice::getVersionName();
    
    // tao.yu 版本号暂时不显示
    string _version = "";//GlobalData::shared()->version.c_str();
    //        _version = _version.substr(0,_version.length()-3);
#if COCOS2D_DEBUG > 0
    CCLabelIF* label = CCLabelIF::create(CCString::createWithFormat("Inner-V: %s \n hudson Code: %s \n deviceID: %s",_version.c_str(),cocos2d::extension::CCDevice::getVersionCode().c_str(),_uuid.c_str())->getCString());
#else
    CCLabelIF* label = CCLabelIF::create(CCString::createWithFormat("V %s (%s)",_version.c_str(),cocos2d::extension::CCDevice::getVersionCode().c_str())->getCString());
#endif
    
    label->setScale(0.5f);
    if (CCCommonUtils::isIosAndroidPad())
    {
        label->setScale(0.8f);
    }
    label->setFontSize(40);
    label->setColor({0,255,0});
    label->setAnchorPoint(CCPoint(1,1));
    label->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width-10,CCDirector::sharedDirector()->getWinSize().height));
    
    addChild(label);
}

void LoadingScene::addLoadingTips()
{
    Size size = Director::getInstance()->getWinSize();
    
    Size tipsSize = CCSizeMake(640, 90);
    m_loadingTips = CCLabelIF::create("loading...");
    m_loadingTips->setColor({229,246,160});
    m_loadingTips->enableStroke(ccBLACK, 1.0);
    m_loadingTips->setFontSize(24);
    m_loadingTips->setDimensions(CCSizeMake(tipsSize.width*0.85, tipsSize.height*1.8));
    m_loadingTips->setHorizontalAlignment(kCCTextAlignmentCenter);
    m_loadingTips->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    m_loadingTips->setAnchorPoint(ccp(0.5, 0.5));
    m_loadingTips->setPosition(ccp(size.width/2, m_barClipNode->getPositionY()));
    addChild(m_loadingTips);
}

void LoadingScene::addPlatformLogo()
{
    if (CCFileUtils::sharedFileUtils()->isFileExist("platform_logo.png"))
    {
        // 渠道logo
        m_platformLogo = CCSprite::create("platform_logo.png");
        if (m_platformLogo)
        {
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            m_platformLogo->setPosition(ccp(size.width/2, size.height/2));
            addChild(m_platformLogo);
        }
    }
}

bool LoadingScene::init()
{
    CCLayer::init();

    ParticleController::initParticle();
    if (CCCommonUtils::isIosAndroidPad() && !CCCommonUtils::getIsHDViewPort())
    {
        CCCommonUtils::setIsHDViewPort(true);
    }
    m_getServerListRetryTime = 0;
    
    //TODO
    //xml init
    LocalController::shared();//765ms
    // load common resource
    // 1 2 3 308
    CCLoadSprite::loadCommonResource();//2200ms
    
    CCLabelIF::initFont();
    GameController::getInstance();
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "init");
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    m_isAdd = false;
    
    auto addPt = ccp(0, 0);
    addLoadingBG(addPt);
    addVersionLabel();
    
    // m_loadtips
    float _tipX = CCDirector::sharedDirector()->getWinSize().width/2-40;
    m_loadtips = CCLabelIF::create(_lang("E100074").c_str());
    m_loadtips->setFntFile(getNBFont(NB_FONT_Bold_Border));
    m_loadtips->setColor({140, 200, 200});
    m_loadtips->setFontSize(20);
    m_loadtips->setHorizontalAlignment(kCCTextAlignmentCenter);
    m_loadtips->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    m_loadtips->setAnchorPoint(ccp(0.5, 0.5));
    m_loadtips->setPosition(ccp(_tipX,30) + addPt);
    m_loadtips->setVisible(false);
    addChild(m_loadtips);
    
    //
    int __height = 30;
    std::string language = LocalController::shared()->getLanguageFileName();
    if (!CCLabelIF::canBeSupportedWithBmpFont(language))
    {
        __height = -20;
    }

    // m_FAQ
    m_FAQ = CCLabelIF::create(_lang("E100075").c_str());
    m_FAQ->setFntFile(getNBFont(NB_FONT_Bold_Border));
    m_FAQ->setColor(ccGREEN);
    m_FAQ->setFontSize(20);
    m_FAQ->setHorizontalAlignment(kCCTextAlignmentCenter);
    m_FAQ->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    m_FAQ->setAnchorPoint(ccp(0.5, 0.5));
    m_FAQ->setPosition(ccp(_tipX + m_loadtips->getContentSize().width/2 - __height,30) + addPt);
    m_FAQ->setVisible(false);
    addChild(m_FAQ);
    
    // m_line
    m_line = CCLoadSprite::createSprite("FAQ_line.png");
    m_line->setAnchorPoint(ccp(0.5, 0));
    m_line->setScaleX(m_FAQ->getContentSize().width/m_line->getContentSize().width-5);
    m_line->setPosition(ccp(_tipX + m_loadtips->getContentSize().width/2 - __height,33-m_FAQ->getContentSize().height/2) + addPt);
    m_line->setVisible(false);
    addChild(m_line);
    
    // m_FAQTouchNode
    m_FAQTouchNode = CCNode::create();
    m_FAQTouchNode->setContentSize(Size(m_FAQ->getContentSize().width +10, m_FAQ->getContentSize().height+10));
    m_FAQTouchNode->setAnchorPoint(ccp(0.5, 0.5));
    m_FAQTouchNode->setPosition(ccp(_tipX + m_loadtips->getContentSize().width/2,30) + addPt);
    m_FAQTouchNode->setScale(2);
    addChild(m_FAQTouchNode);
    
    //
    auto loadingBar = CCLoadSprite::createSprite("loading_bar.png");
    loadingBar->setTag(LOADING_2);
    CCSize lbBGsz = loadingBar->getContentSize();
    float baseHeight = lbBGsz.height*3;
    loadingBar->setPosition(ccp(size.width/2,baseHeight) + addPt);
    addChild(loadingBar);
    
    m_loadingProgress = CCLoadSprite::createSprite("loading_progress.png");
    CCSize lpsz = m_loadingProgress->getContentSize();
    m_barSize = lpsz+CCSizeMake(0, 30);

    m_loadingProgress->setAnchorPoint(ccp(0,0));
    m_loadingProgress->setPosition(ccp(0, lpsz.height/2));
    m_barClipNode = CCClipNode::create(m_barSize.width, m_barSize.height);
    m_barClipNode->addChild(m_loadingProgress);
    m_barClipNode->setPosition(ccp((size.width-lpsz.width)/2,baseHeight-lpsz.height) + addPt);
    m_loadingProgress->setContentSize(CCSizeMake(m_barSize.width*0.1, m_barSize.height));
    addChild(m_barClipNode);
    
    auto loadingBarBG = CCLoadSprite::createSprite("loading_bar_BG.png");
    loadingBarBG->setTag(LOADING_1);
    loadingBarBG->setPosition(ccp(size.width/2,baseHeight) + addPt);
    addChild(loadingBarBG);
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        loadingBarBG->setPosition(ccp(size.width/2,baseHeight*2.4) + addPt);
        loadingBarBG->setScale(2.f);
        
        m_barClipNode->setPosition(ccp((size.width-lpsz.width*2)/2.0,(baseHeight-lpsz.height)*2.4) + addPt);
        m_barClipNode->setScale(2.f);
        
        loadingBar->setPosition(ccp(size.width/2,baseHeight*2.2) + addPt);
        loadingBar->setScale(2.f);
    }
    
    // 加流动的光
    m_clipNode = CCClipNode::create(m_barClipNode->getContentSize().width, m_barClipNode->getContentSize().height);
    m_clipNode->setAnchorPoint(ccp(0, 0));
    m_clipNode->setPosition(ccp((size.width-lpsz.width)/2,baseHeight-30) + addPt);
    auto tmpSpr = CCLoadSprite::createSprite("Loading_bar_glow.png");
    tmpSpr->setAnchorPoint(ccp(0, 0));
    tmpSpr->setPosition(ccp(-tmpSpr->getContentSize().width, 8) + addPt);
    m_clipNode->addChild(tmpSpr);
    CCActionInterval * move1 = CCMoveTo::create(1.5, ccp(lpsz.width,8));
    CCActionInterval * move2 = CCMoveTo::create(0, ccp(-tmpSpr->getContentSize().width,8));
    tmpSpr->runAction(CCRepeatForever::create(CCSequence::create(move1,move2,NULL)));
    addChild(m_clipNode);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_clipNode->setPosition(ccp((size.width-lpsz.width*2)/2,(baseHeight-30)*2.4) + addPt);
        m_clipNode->setScale(2.f);
    }
    
    // 加进度条头上的光效
    m_headParticleNode = CCNode::create();
    m_headParticleNode->setPosition(ccp((size.width-lpsz.width)/2,baseHeight) + addPt);
    for (int i=1; i<=3; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("Loading_%d",i)->getCString());
        m_headParticleNode->addChild(particle);
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_headParticleNode->setPosition(ccp((size.width-lpsz.width)/2,baseHeight*2.35) + addPt);
        m_headParticleNode->setScale(2.f);
    }
    addChild(m_headParticleNode);
    
    
    auto logo = CCLoadSprite::createSprite("logo.png");
    logo->setAnchorPoint(ccp(0.5, 0));
    
    logo->setPosition(ccp(size.width * 0.5, size.height * 0.75));//
    logo->setTag(LOADING_LOGO_TAG);
    addChild(logo,10000);
    
    addLoadingTips();
    addPlatformLogo();
    
    setCleanFunction([](){
        CCLoadSprite::releaseResourceBySceneId(SCENE_ID_LOADING);
    });

    this->scheduleOnce(schedule_selector(LoadingScene::show45SecondAlert), 45);

    return true;
}

#pragma mark *

bool LoadingScene::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_FAQ->isVisible() && isTouchInside(m_FAQTouchNode, pTouch)) {
        onGoToHelpShift(NULL);
        //PopupViewController::getInstance()->addPopupInView(SuggestionView::create());
        return true;
    }
    return false;
}
void LoadingScene::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void LoadingScene::onGoToHelpShift(CCObject* p)
{
    cocos2d::CCArray *tags = new cocos2d::CCArray();
    tags->init();
    cocos2d::CCDictionary *config = new cocos2d::CCDictionary();
    cocos2d::CCDictionary *meta = new CCDictionary();
    tags->addObject(new CCString("loading"));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    tags->addObject(new CCString("ios"));
#else
    if(GlobalData::shared()->analyticID != "market_global")
        tags->addObject(new CCString(GlobalData::shared()->analyticID));
#endif
    meta->setObject(tags, HS_TAGS_KEY);
    meta->setObject(CCString::create(GameController::getInstance()->getLoadingLogString()), "loadingLog");
    config->setObject(meta, HS_META_DATA_KEY);
    GlobalData::shared()->isBind = true;
    HelpshiftCocos2dx::showConversation(config);
}

#pragma mark * update ini & xml

void LoadingScene::getDownloadContents()
{
    NBDLCController::clean();
    
    string language = LocalController::shared()->getLanguageFileNameBasedOnUserSystem();
    string manifest_file_path = "manifest/text_" + language + "_main.manifest";
    string version_filename = "text_" + language + "_version.manifest";
    string temp_manifest_filename = "text_" + language + "_project.manifest.temp";
    string manifest_filename = "text_" + language + "_project.manifest";
    auto cb_ini = [this, manifest_file_path](string manifest_file_path_as_key, EventAssetsManagerEx * event){
        
        // >>>>>>>
        auto dlc_xml = NBDLCController::create("manifest/local_xml_main.manifest", "local_xml_version.manifest", "local_xml_project.manifest.temp", "local_xml_project.manifest");
        dlc_xml->start("manifest/local_xml_main.manifest", [this](string manifest_file_path_as_key, EventAssetsManagerEx * event){
            LocalController::shared()->purgeData();
            LocalController::shared()->init();
            
            setLoadingTips();
            LoadingScene::showLoading();
            
            //为月卡做记录
            int loadCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("day_load_count");
            loadCount+=1;
            CCUserDefault::sharedUserDefault()->setIntegerForKey("day_load_count", loadCount);
            CCUserDefault::sharedUserDefault()->flush();
        });
        // <<<<<<
    };
    auto dlc_ini = NBDLCController::create(manifest_file_path, version_filename, temp_manifest_filename, manifest_filename);
    dlc_ini->start(manifest_file_path, cb_ini);
}

#pragma mark *

void LoadingScene::onEnter(){
    CCLayer::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::doLogin), MSG_GameStart, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::doLoginInitError), MSG_LoginInitError, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::doResourceUpdate), MSG_LoginSucess, NULL);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::doLoginInit), MSG_LoginInit, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::stepLoading), MSG_INIT_FINISH, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::onRelogin), MSG_RELOGIN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::onEnterFrame), MSG_SHOW_LOADINGS_FQA, NULL);
//    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(LoadingScene::onEnterFrame), this, 30.0,kCCRepeatForever, 0.0f, false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    //music
    
    GlobalData::shared()->analyticID = cocos2d::extension::CCDevice::getPublishRegion();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GlobalData::shared()->fromCountry = AppLibHelper::getLocalCountry();
#endif
    if (GlobalData::shared()->needLogin()) {
        if(GlobalData::shared()->analyticID != "cn_mi"
           && GlobalData::shared()->analyticID != "cn_mihy"
           ){
            GlobalData::shared()->platformUID = "";
            //重登时候清除掉悬浮窗等
            cocos2d::extension::CCDevice::doPlatformLoading();
        }
    }

    if(isChina() && !m_isAdd){
        CCSprite *logo = dynamic_cast<CCSprite*>(this->getChildByTag(LOADING_LOGO_TAG));
        if(logo){
            logo->setDisplayFrame(CCLoadSprite::loadResource("logo_only.png"));
            logo->setPositionY(logo->getPositionY() + 40);
        }
        auto layer = CCLayerColor::create(ccc4(0, 0, 0, 200), CCDirector::sharedDirector()->getWinSize().width, 70);
        this->addChild(layer);
        layer->setAnchorPoint(ccp(0.5, 0));

        auto label = CCLabelIF::create(_lang("4100009").c_str());
        this->addChild(label);
        label->setDimensions(CCSize(CCDirector::sharedDirector()->getWinSize().width - 16, 0));
        label->setColor(ccc3(141, 94, 75));
        label->setFontSize(22);
        label->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 35));
        
        
        auto addY = [](CCNode *node){
            if(node){
                int add = 30;
                if (CCCommonUtils::isIosAndroidPad())
                {
                    add *= 2.f;
                }
                node->setPositionY(node->getPositionY() + add);
            }
        };

        addY(m_loadtips);
        addY(m_FAQ);
        addY(m_line);
        addY(m_FAQTouchNode);
        addY(m_barClipNode);
        addY(m_clipNode);
        addY(m_headParticleNode);
        addY(m_loadingTips);
        addY(this->getChildByTag(LOADING_1));
        addY(this->getChildByTag(LOADING_2));
        addY(this->getChildByTag(LOADING_3));
        addY(this->getChildByTag(LOADING_4));
    }
    
    getDownloadContents();
}

void LoadingScene::onExit(){
    this->unschedule(schedule_selector(LoadingScene::show45SecondAlert));
    this->unschedule(schedule_selector(LoadingScene::resendGetServerList));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_LoginInitError);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_LoginSucess);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GameStart);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_LoginInit);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INIT_FINISH);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_RELOGIN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHOW_LOADINGS_FQA);
    setTouchEnabled(false);
//    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(LoadingScene::onEnterFrame), this);
    IFSkeletonDataManager::shared()->purge();
    CCLayer::onExit();
}

void LoadingScene::onEnterFrame(CCObject* p)
{
//    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(LoadingScene::onEnterFrame), this);
    m_loadtips->setVisible(true);
    m_FAQ->setVisible(true);
    m_line->setVisible(true);
    YesNoDialog::showVariableTitle(_lang("E100085").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::onGoToHelpShift), NULL),_lang("E100075").c_str(),false);
}

void LoadingScene::setLoadingTips(){
    // get tips content
    //int playLevel = CCUserDefault::sharedUserDefault()->getIntegerForKey(ACCOUNT_USER_LEVEL,0);
    int level = CCUserDefault::sharedUserDefault()->getIntegerForKey(FUN_BUILD_MAIN_LEVEL);
    if(level<=0){
        level = 1;
    }
    auto tipsDict = LocalController::shared()->DBXMLManager()->getGroupByKey("loading_tips");
    CCDictElement *pObj;
    string loadingTip = "";
    vector<string> tips;
    CCDICT_FOREACH(tipsDict, pObj) {
        auto infoDict = _dict(pObj->getObject());
        auto levelRange = string(infoDict->valueForKey("level_range")->getCString());
        auto id = string(infoDict->valueForKey("id")->getCString());
        auto description = string(infoDict->valueForKey("description")->getCString());
        
        vector<string> levels;
        CCCommonUtils::splitString(levelRange, ";", levels);
        if (level < atoi(levels[0].c_str()) || level > atoi(levels[1].c_str())) {
            continue;
        }
        CCCommonUtils::splitString(description, ";", tips);
    }
    srand(time(NULL));
    if(tips.size()>0){
        int index = rand()%tips.size();
        loadingTip = _lang(tips[index].c_str());
        CCLOG("index=%d",index);
        m_loadingTips->setString(loadingTip.c_str());
    }
}
void LoadingScene::doLoginInit(float t)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "doLoginInit");
    if (!GlobalData::shared()->isInitFlag && !GlobalData::shared()->isInDataParsing)
    {
        //loadingLog统计
        GameController::getInstance()->setLoadingLog("LoadingScene", "doLoginInit-false");
        InitCommand* cmd=new InitCommand();
        cmd->sendAndRelease();
    }
    else
    {
        unschedule(schedule_selector(LoadingScene::doLoginInit));
    }
}

void LoadingScene::doLoginInitError(CCObject* p){
    unschedule(schedule_selector(LoadingScene::doLoginInit));
//    YesNoDialog::show(_lang("114141").c_str(), NULL);
    YesNoDialog::showVariableTitle(_lang("114141").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::onGoToHelpShift), NULL),_lang("E100075").c_str(),false);
}

void LoadingScene::show45SecondAlert(float time){
    this->unschedule(schedule_selector(LoadingScene::show45SecondAlert));
    auto _dialog = YesNoDialog::show(_lang("E100093").c_str()
                                     , CCCallFuncO::create(this, callfuncO_selector(LoadingScene::retry), NULL)
                                     , 0
                                     , true
                                     , CCCallFuncO::create(this, callfuncO_selector(LoadingScene::onGoToHelpShift), NULL)
                                     );
    _dialog->showCancelButton();
    _dialog->setYesButtonTitle(_lang("105248").c_str());
    _dialog->setNoButtonTitle(_lang("E100075").c_str());
}

void LoadingScene::retry(CCObject *obj){
    
}

void LoadingScene::doLogin(CCObject* p)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "doLogin");

    sendCmdLogin();
}

void LoadingScene::sendCmdLogin()
{
    // Guidance
    CCLoadSprite::doResourceByGeneralIndex(100, true);//450ms
    
    schedule(schedule_selector(LoadingScene::onLoginTimeout), 0.5);
    // password retry count reset
    GlobalData::shared()->isLoginFlag = true;
    NetController::shared()->clearRequestQueue();
    LoginCommand* cmd=new LoginCommand();
    cmd->setDefaultUserName();
    cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(LoadingScene::onConnectionLost), NULL));
    cmd->sendAndRelease();
}

void LoadingScene::onLoginTimeout(float t)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onLoginTimeout");
    unschedule(schedule_selector(LoadingScene::onLoginTimeout));
    CCHttpRequest* request = new CCHttpRequest();
    CCString* url;
    string _uuid = cocos2d::extension::CCDevice::getDeviceUid();
    string _platformUID = GlobalData::shared()->platformUID;
    string _Country = GlobalData::shared()->fromCountry;
    string _gameUid = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID,"");
    //string _gameUid = "145412096000001";
    string _lang = LocalController::shared()->getLanguageFileName();
    string _serverId = CCUserDefault::sharedUserDefault()->getStringForKey(SERVER_ID, "");
    string _serverIp = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP, "");
#if COCOS2D_DEBUG == 1
    url = CCString::createWithFormat("http://%s/probe.php?uuid=%s&loginFlag=%d&country=%s&gameuid=%s&lang=%s&serverId=%s&serverIp=%s",SERVERLIST_IP1,_uuid.c_str(),1,_Country.c_str(),_gameUid.c_str(),_lang.c_str(),_serverId.c_str(),_serverIp.c_str());
#else
#if COCOS2D_DEBUG == 2
    url = CCString::createWithFormat("http://%s/probe.php?uuid=%s&loginFlag=%d&country=%s&gameuid=%s&lang=%s&serverId=%s&serverIp=%s",SERVERLIST_IP1,_uuid.c_str(),1,_Country.c_str(),_gameUid.c_str(),_lang.c_str(),_serverId.c_str(),_serverIp.c_str());
#else
    url = CCString::createWithFormat("http://%s/probe.php?uuid=%s&loginFlag=%d&country=%s&gameuid=%s&lang=%s&serverId=%s&serverIp=%s",SERVERLIST_IP2,_uuid.c_str(),1,_Country.c_str(),_gameUid.c_str(),_lang.c_str(),_serverId.c_str(),_serverIp.c_str());
#endif
#endif
    CCLOG("get_server_status list URL: %s",url->getCString());
    
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(LoadingScene::onGetServerStatus));
    request->setTag("get_server_status");
    CCHttpClient::getInstance()->setTimeoutForConnect(10);
    CCHttpClient::getInstance()->send(request);
    request->release();
    
}

void LoadingScene::onGetServerStatus(CCHttpClient* client, CCHttpResponse* response)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onGetServerStatus");
    if (!response)
    {
        return;
    }

    m_responseCode = response->getResponseCode();
    CCLOG("response code: %d", m_responseCode);
    if (!response->isSucceed())
    {
        CCLOG("response failed!\nerror buffer: %s", response->getErrorBuffer());
//        YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::sendCmdGetServerList), NULL),_lang("105248").c_str(),true);
        return;
    }
    std::vector<char>* iter = response->getResponseData();
    std::string serverInfo(iter->begin(),iter->end());
    Json *c = Json_create(serverInfo.c_str());
    if(!c){
        return;
    }
    int _code =Json_getInt(c,"cokerrcode",0);//新版用这个cokerrcode为0 和1  代替code 防止http 200也进来了
    if (_code != 0) {
        std::string _message= Json_getString(c, "message", "");
        int time = Json_getInt(c, "timelist", 0);
        YesNoDialog::showVariableTitle(_message.c_str(),CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame), NULL),_lang("confirm").c_str(),true,time);
    }
    Json_dispose(c);
}

bool LoadingScene::showServiceTerms(){
    bool showServiceTerms = false;
    if (GlobalData::shared()->analyticID == "nstore"
        || GlobalData::shared()->analyticID == "tstore"){
        showServiceTerms = CCUserDefault::sharedUserDefault()->getStringForKey("service_terms_check","") == "";
    }
    return showServiceTerms;
}

bool LoadingScene::waitingForLogin(){
    if (GlobalData::shared()->needLogin() && GlobalData::shared()->platformUID == "") {
        return true;
    }
    return false;
}

//根据不同情况选择登录方式
void LoadingScene::selectLogin(float t){
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "selectLogin");
    //根据平台判断是否需要先登录平台账号
    bool loadingWait = waitingForLogin();
    //服务条款
    bool serviceTerms = showServiceTerms();
    if (loadingWait || serviceTerms){
        CCLOG("select login wait");
        if (!m_platformLoginFlag) {
            CCLOG("select login strat schedule");
            m_platformLoginFlag = true;
            schedule(schedule_selector(LoadingScene::selectLogin), 0.5f);
            if(serviceTerms){
                vector<std::string> serviceTermsStr = AppLibHelper::getServiceTerms(1);
                vector<std::string> personalTermsStr = AppLibHelper::getServiceTerms(2);
                PopupViewController::getInstance()->addPopupView(KoreaPermitPopUPView::create("", "", serviceTermsStr, "", personalTermsStr));
            }
        }
        if(loadingWait){
            //解决百度初始化异步问题，由java控制是否调用登陆
            cocos2d::extension::CCDevice::doPlatformLogin();
        }
        return;
    }
    else if (m_platformLoginFlag)
    {
        m_platformLoginFlag = false;
        CCLOG("select login end schedule");
        unschedule(schedule_selector(LoadingScene::selectLogin));
    }
    //1.判断本地缓存
    string gameUID = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID,"");
    string _uuid = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_UUID,"");
    if(gameUID == "" && _uuid == "")
    {
        //2.根据URL请求，选择登录方式
        sendCmdGetServerList(NULL);
    }
    else
    {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GameStart);
    }
}

void LoadingScene::onRelogin(CCObject* p)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onRelogin");
    selectLogin(0.0);
}

void LoadingScene::sendCmdGetServerList(CCObject* p){
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "sendCmdGetServerList");

    CCHttpRequest* request = new CCHttpRequest();
    CCString* url;
    string _uuid = cocos2d::extension::CCDevice::getDeviceUid();
    string _platformUID = GlobalData::shared()->platformUID;
    string _platformToken = GlobalData::shared()->platformToken;
    string _Country = GlobalData::shared()->fromCountry;
    string param = "";
    
    auto currentTime = time(NULL);
    std::string timeStr = CC_ITOA(double(currentTime));

    std::string key = _uuid + timeStr + "@z4LugUwrDN0MqOJtzmHPK7Ss-k4taAUOZh1M3Yu9";
    MD5 md5(key);
    std::string md5Str = md5.toString();
    
    bool isChinaPhone = isChina();
    
    if (_platformUID != "") {
        string _PublishRegion = cocos2d::extension::CCDevice::getPublishRegion();
        param = CCString::createWithFormat("uuid=%s&loginFlag=%d&pfId=%s&pf=%s&token=%s&country=%s&t=%s&sig=%s",_uuid.c_str(),1,_platformUID.c_str(),_PublishRegion.c_str(),_platformToken.c_str(),_Country.c_str(),timeStr.c_str(),md5Str.c_str())->getCString();
    }
    else
    {
        param = CCString::createWithFormat("uuid=%s&loginFlag=%d&country=%s&t=%s&sig=%s",_uuid.c_str(),1,_Country.c_str(),timeStr.c_str(),md5Str.c_str())->getCString();
    }
#if COCOS2D_DEBUG == 1
    url = CCString::createWithFormat("http://%s/gameservice/getserverlist.php?%s", SERVERLIST_IP1, param.c_str());
#elif COCOS2D_DEBUG == 2
    url = CCString::createWithFormat("http://%s/gameservice/getserverlist.php?%s", SERVERLIST_IP1, param.c_str());
#else
    if (isChinaPhone && m_getServerListRetryTime % 2 == 0) {
        url = CCString::createWithFormat("http://%s/gameservice/getserverlist.php?%s", SERVERLIST_IP1, param.c_str());
    } else {
        url = CCString::createWithFormat("http://%s/gameservice/getserverlist.php?%s", SERVERLIST_IP2, param.c_str());
    }
#endif
    
    CCLOG("COK server list URL: %s",url->getCString());
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(LoadingScene::onGetServerList));
    request->setTag("get_server_list");
    CCHttpClient::getInstance()->setTimeoutForConnect(30);
    CCHttpClient::getInstance()->send(request);
    request->release();
    m_getServerListRetryTime++;
    int maxTime = 3;
    if(m_getServerListRetryTime <= maxTime){
        this->scheduleOnce(schedule_selector(LoadingScene::resendGetServerList), 30);
    }
}

void LoadingScene::resendGetServerList(float time){
    sendCmdGetServerList(NULL);
}

void LoadingScene::onGetServerList(CCHttpClient* client, CCHttpResponse* response)
{
    this->unschedule(schedule_selector(LoadingScene::resendGetServerList));
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onGetServerList");
    
    if (!response)
    {
        return;
    }
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLOG("%s completed", response->getHttpRequest()->getTag());
        if (strcmp("get_server_list",response->getHttpRequest()->getTag())!=0) {
            // return;
        }
    }
    m_responseCode = response->getResponseCode();
    CCLOG("response code: %d", m_responseCode);
    if (!response->isSucceed())
    {
        CCLOG("response failed!\nerror buffer: %s", response->getErrorBuffer());
//        LocalController::shared()->init();
        YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::sendCmdGetServerList), NULL),_lang("105248").c_str(),true);
        return;
    }
    std::vector<char>* iter = response->getResponseData();
    std::string serverInfo(iter->begin(),iter->end());
    if(serverInfo == "")
    {
        CCLOG("response data is null!");
        YesNoDialog::showVariableTitle(_lang("E100092").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::onGoToHelpShift), NULL),_lang("E100075").c_str(),false);
        return;
    }
    Json *c = Json_create(serverInfo.c_str());
    Json *serverList=Json_getItem(c,"serverList");
    int country=Json_getInt(c,"country",0);
    //get serverInfo
    Json *c2 = Json_getItemAt(serverList, 0);
    string id = Json_getString(c2,"id","-1");
    string port = Json_getString(c2, "port","-1");
    string zone = Json_getString(c2, "zone","-1");
    string ip = Json_getString(c2, "ip","-1");
    string gameUid = Json_getString(c2, "gameUid", "");
    string _uuid = Json_getString(c2, "uuid", "");
    Json_dispose(c);
    
    GlobalData::shared()->loginServerInfo.recommandCountry = country;
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, _uuid);
    CCUserDefault::sharedUserDefault()->setStringForKey(SERVER_ID, id);
    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, gameUid);
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
    

#if COCOS2D_DEBUG != 0
    //测试用切换服务器
//        ip = "10.1.6.72";
//    zone = "COK1";
//         CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//         CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//         CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
//         CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "");
#endif
    CCUserDefault::sharedUserDefault()->flush();
    
    NetController::shared()->setIp(ip);
    
    NetController::shared()->setZone(zone);

    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GameStart);
}

bool LoadingScene::isJP(){
    std::string country = GlobalData::shared()->fromCountry;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    country = AppLibHelper::getLocalCountry();
#endif
    if(country == "JA" || country == "ja"){
        return true;
    }
    return false;
}

void LoadingScene::showLoading()
{
    schedule(schedule_selector(LoadingScene::loadingAni), 0.167f);
    selectLogin(0.0);
}

void LoadingScene::loadingAni(float t)
{
    if (m_steps == LOADING_STEP) {
        SoundController::sharedSound()->playBGMusic(Music_Sfx_logo_loading);
    }
    if(m_steps == 75 && (showServiceTerms() || waitingForLogin()))
        return;
    if (m_steps <= 10)
    {
        unschedule(schedule_selector(LoadingScene::loadingAni));
    } else {
        float pro = (LOADING_STEP-m_steps+1)*1.0/(LOADING_STEP*1.0);
        string proInfo = CC_ITOA(pro*100);

        float curX = m_barSize.width*pro;
        m_clipNode->setContentSize(CCSizeMake(curX, m_barSize.height));
        m_barClipNode->setContentSize(CCSizeMake(curX, m_barSize.height));
        if (CCCommonUtils::isIosAndroidPad())
        {
            curX *= 2.f;
        }
        m_headParticleNode->setPositionX(m_clipNode->getPositionX()+curX);
        if (m_steps == 65)
        {
            unschedule(schedule_selector(LoadingScene::loadingAni));
            schedule(schedule_selector(LoadingScene::loadingAni), 0.167f);
        }
        --m_steps;
        if(m_steps == 18 && m_platformLogo){
            m_platformLogo->removeFromParent();
            m_platformLogo = NULL;
        }
    }
}

void LoadingScene::stepLoading(CCObject* p)
{
    //loadingLog统计
    time_t worldTime = GlobalData::shared()->getWorldTime();
    GameController::getInstance()->setLoadingLog("LoadingScene", CCString::createWithFormat("stepLoading:serverTime:%s ",CCCommonUtils::timeStampToDate(worldTime).c_str())->getCString());
    unschedule(schedule_selector(LoadingScene::doLoginInit));
    //提示更新版本
    std::string str;
    GlobalData::shared()->lang = cocos2d::extension::CCDevice::getLanguage();
    GlobalData::shared()->platform = "ios";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    GlobalData::shared()->platform = "android";
    //根据平台获得提示语
    if(GlobalData::shared()->analyticID == "market_global"){
        str = "GOOGLE";
    }else{
        /**
         tstore
         nstore
         amazon
         cafebazaar
         **/
        str = GlobalData::shared()->analyticID;
    }
#else
#ifdef Channel_91
    str = "91Store";
    GlobalData::shared()->analyticID = "91Store";
#else
    str = "AppStore";
    GlobalData::shared()->analyticID = "AppStore";
#endif
#endif
    
    CCLOG("------- Dragon Clans : GlobalData::shared()->analyticID : %s", GlobalData::shared()->analyticID.c_str());
    
    if (GlobalData::shared()->updateType==1)
    {
        auto _dialog = YesNoDialog::show(CCString::createWithFormat(_lang("login_error_client_ver_must").c_str(), str.c_str())->getCString(),
                                         CCCallFuncO::create(this, callfuncO_selector(LoadingScene::forceUpdate), NULL),0,
                                         true,
                                         CCCallFuncO::create(this, callfuncO_selector(LoadingScene::offerUpdate), NULL));
        _dialog->showCancelButton();
        return;
    }
    if (GlobalData::shared()->updateType==2)
    {
        YesNoDialog::showVariableTitle(CCString::createWithFormat(_lang("login_error_client_ver_must").c_str(), str.c_str())->getCString(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::forceUpdate),NULL), _lang("confirm").c_str(),true);
        return;
    }
    gotoMainScene(0.1);
}

void LoadingScene::gotoMainScene(float t)
{
    unschedule(schedule_selector(LoadingScene::loadingAni));
    if (m_steps > 1) {
        float pro = (LOADING_STEP-m_steps+1)*1.0/(LOADING_STEP*1.0);
        string proInfo = CC_ITOA(pro*100);
        
        float curX = m_barSize.width*pro;
        m_clipNode->setContentSize(CCSizeMake(curX, m_barSize.height+30));
        m_barClipNode->setContentSize(CCSizeMake(curX, m_barSize.height+30));
        if (CCCommonUtils::isIosAndroidPad())
        {
            curX *= 2.f;
        }
        m_headParticleNode->setPositionX(m_clipNode->getPositionX()+curX);
        
        //--m_steps;
        m_steps -= 10;
        if ( m_steps <= 1 ) m_steps = 1;
        schedule(schedule_selector(LoadingScene::gotoMainScene), 0.005f);
    }
    else if(!GlobalData::shared()->isInitFlag){
        unschedule(schedule_selector(LoadingScene::gotoMainScene));
        schedule(schedule_selector(LoadingScene::gotoMainScene), 0.01f);
    }
    else {
        //loadingLog统计
        GameController::getInstance()->setLoadingLog("LoadingScene", "gotoMainScene");
        unschedule(schedule_selector(LoadingScene::doLoginInit));
        unschedule(schedule_selector(LoadingScene::gotoMainScene));
        m_barClipNode->setContentSize(m_barSize);
        m_clipNode->setContentSize(m_barSize);

        m_headParticleNode->setPositionX(m_clipNode->getPositionX()+m_barSize.width);
        
        //行云log
        GameController::getInstance()->trackVisit();
        SceneController::getInstance()->addGUI();
        if(SceneController::getInstance()->showBG){
            SceneController::getInstance()->showBG->release();
            SceneController::getInstance()->showBG = NULL;
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
        }else{
            SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
        }
        CCCommonUtils::recordStep("5");
    }
}

void LoadingScene::forceUpdate(CCObject* p) {
    //服务器下发地址为空
    bool isUrlNull =  GlobalData::shared()->downloadUrl.empty() || GlobalData::shared()->downloadUrl=="null";
    
    if (isUrlNull)
        GameController::getInstance()->quitGame(NULL);
    else
        GameController::getInstance()->updateVersion();
}

void LoadingScene::onConnectionLost(cocos2d::CCObject *obj)
{
    CCLOG("ConnectionLost");
    auto ret = dynamic_cast<NetResult*>(obj);
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onConnectionLost");
    
    if (Error_Network_Lost==ret->getErrorCode() && !GlobalData::shared()->isPause) {
//        LocalController::shared()->init();
//        YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::doLogin), NULL),_lang("105248").c_str(),true);
        NetController::shared()->checkNetWork("login 1");
    };
}

void LoadingScene::doResourceUpdate(CCObject* p) {
    unschedule(schedule_selector(LoadingScene::onLoginTimeout));
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "doResourceUpdate");
    
    schedule(schedule_selector(LoadingScene::doLoginInit), 5.0f);

}

void LoadingScene::offerUpdate(CCObject* p)
{
    GlobalData::shared()->updateType=0;
    stepLoading(NULL);
}

bool LoadingScene::isChina(){
    bool isChinaPlatForm = (GlobalData::shared()->isChinaPlatForm() || GlobalData::shared()->analyticID == "common");
    bool isChinaLan = (CCCommonUtils::getLanguage() == "zh_CN");
    return isChinaPlatForm || isChinaLan;
}

//---------

RollingSprite *RollingSprite::create(std::string picName, float speed){
    RollingSprite *ret = new RollingSprite();
    if(ret && ret->init(picName, speed)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void RollingSprite::onEnter(){
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void RollingSprite::onExit(){
    this->removeAllChildren();
    m_batch = NULL;
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    CCNode::onExit();
}

bool RollingSprite::init(std::string picName, float speed){
    m_speed = speed;
    m_picName = picName;
    m_batch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource(m_picName.c_str())->getTexture(), 5);
    this->addChild(m_batch);
    auto size = CCDirector::sharedDirector()->getWinSize();
    int w = 0;
    
    auto addSprite = [&](){
        auto sprite = CCLoadSprite::createSprite(m_picName.c_str());
        m_batch->addChild(sprite);
        sprite->setAnchorPoint(ccp(0, 1));
        sprite->setPosition(ccp(w, 0));
        w += sprite->getContentSize().width;
    };
    
    while(w < size.width){
        addSprite();
        if(w > size.width){
            addSprite();
        }
    }
    return true;
}

void RollingSprite::update(float time){
    if(!m_batch){
        return;
    }
    auto &arr = m_batch->getChildren();
    if( arr.size() > 0){
        auto node = dynamic_cast<CCNode*>(arr.at(0));
        if(node){
            int cellW = node->getContentSize().width;
            float batchX = m_batch->getPositionX();
            float nextX = batchX - time * m_speed;
            if(nextX < -cellW){
                nextX += int(abs(nextX) / cellW) * cellW;
            }
            m_batch->setPositionX(nextX);
        }
    }
}

