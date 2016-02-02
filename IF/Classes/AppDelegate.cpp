//
//  IFAppDelegate.cpp
//  IF
//
//  Created by 吴 凌江 on 13-8-28.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "CCMathUtils.h"
#include "GlobalData.h"
#include "SceneContainer.h"
#include "SceneController.h"
#include "CCSafeNotificationCenter.h"
//#include "CCLuaEngine.h"
//#include "Lua_extensions_CCB.h" //TODO lua
#include "Ext/CCDevice.h"
#include "ChatServiceCocos2dx.h"
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
//    #include "NetControllerSFSC.h"
//#endif
#include "WorldController.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "AppLibHelper.h"
    #include "TestinCrashHelper.h"
#endif

#include "CCLuaEngine.h"
#include "lua_module_register.h"
// tao.yu TalkingData
#include "../../cocos2d/external/TalkingDataGameAnalytics/include/TDCCTalkingDataGA.h"

#define IS_HD 1  // 1 是HD版本 0 不是

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    
#if (COCOS2D_DEBUG == 0 || COCOS2D_DEBUG == 2)
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // crash dumper init
    CCCrashHandler::sharedHandler()->init();  // -- added by Zouc
    #else
    TestinCrashHelper::initTestinAgent("f1d4256db620d787f78a1e28c415c5fd", "AppStore");
    #endif
#endif
    // tao.yu TalkingData
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    TDCCTalkingDataGA::onStart("E020286589120C45394E3994E90F44D8", "AppStore");
#endif
    
    // initialize director
    Director* pDirector = Director::getInstance();
    auto glview = pDirector->getOpenGLView();
    if (!glview)
    {
        glview = GLViewImpl::create("Dragon Clans");
    }
    pDirector->setOpenGLView(glview);
    CCTexture2D::PVRImagesHavePremultipliedAlpha(false);
    
    if (CCCommonUtils::isIOSPad() || CCCommonUtils::isAdriodPad())
    {
        CCCommonUtils::setIsUseHD(IS_HD); // 客户端开关
        CCCommonUtils::setServerHDSwitch();// 设置上次登录服务器给的开关
    }
    
    CCCommonUtils::setIsHDViewPort(CCCommonUtils::isIosAndroidPad());
    
#if COCOS2D_DEBUG > 0
    // 打开这个测多分辨率
    //    radio=1.78; // iphone5
    //    radio=1.67; // WVGA
    //    radio=1.33; // Ipad
    //    radio=1.5; //Iphone4/4s
    CCSize winSize = pDirector->getWinSize();
    CCSize screenSize = glview->getFrameSize();
    float radio = winSize.width/winSize.height;
    float scale = 852/screenSize.width;
    CCLOG("screen width: %d, height: %d \n resolution width: %d, height: %d \n radio:%.2f, scale:%.2f",
          (int)screenSize.width,(int)screenSize.height,
          (int)winSize.width,(int)winSize.height,
          radio,scale);
    
    // turn on display FPS
    pDirector->setDisplayStats(true);
#endif
    
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 30);
    
//    CCLOG("Time profiler: [%ld] ,line: [%d]",CCMathUtils::getCurrentTime(),__LINE__);
    
// ------------------------------------------------------------------------------------
#ifdef USING_LUA
    
    // tao.yu TalkingData game start
    GA_ON_EVENT("OPEN_GAME");
    
    // register lua module
    auto engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    lua_State* L = engine->getLuaStack()->getLuaState();
    lua_module_register(L);
    
    LuaStack* stack = engine->getLuaStack();
    stack->setXXTEAKeyAndSign("n_77wDDolqz", strlen("n_77wDDolqz"), "i900_gFymA", strlen("i900_gFymA"));
    
    if (engine->executeScriptFile("src/main.lua"))
    {
        CCLOG("Lua init Error");
    }
#endif
    
// ------------------------------------------------------------------------------------
    // run
    CCLoadSprite::init();
    pDirector->runWithScene(SceneContainer::create());
    SceneController::getInstance()->gotoLoading();//2300ms
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AppLibHelper::initLibs();
#endif
    
    CCLOG("WritablePath:[%s]", FileUtils::getInstance()->getWritablePath().c_str());
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCLOG("applicationDidEnterBackground");
    cocos2d::CCDirector::sharedDirector()->pause();
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
    GlobalData::shared()->isPause = true;
    GlobalData::shared()->pauseTime = GlobalData::shared()->getWorldTime();
    if (SceneController::getInstance()->currentSceneId != SCENE_ID_LOADING)
    {
        cocos2d::extension::CCDevice::pushNotice(987, 60);
        
        struct tm *tm = localtime(&(GlobalData::shared()->pauseTime));
        // char date[20];
        // strftime(date, sizeof(date), "%Y-%m-%d", tm);
        
        int h = 23 - tm->tm_hour;
        h = h > 0 ? h : 0;
        int m = 60 - tm->tm_min;
        int s = 60 - tm->tm_sec;
        int left = h * 60 * 60 + m * 60 + s;
        
        const char* idx[] = {"105675", "105676", "105677", "105696"};
        int b = 3;
        int a = 0;
        // [0, 3]
        srand((unsigned)time(NULL));
        int id_0 = (rand() % (b-a+1))+ a;
        int id_1 = (rand() % (b-a+1))+ a;
        
        // 最后下线时间次日12:30以及20:00各推一条推送，以当地时间（国家所属时区）为准。
#if COCOS2D_DEBUG > 0
        cocos2d::extension::CCDevice::pushNotice(988, 3, _lang(idx[id_0]));
        cocos2d::extension::CCDevice::pushNotice(989, 6, _lang(idx[id_1]));
#else
        cocos2d::extension::CCDevice::pushNotice(988, left + 12.5 * 60 * 60, _lang(idx[id_0]));
        cocos2d::extension::CCDevice::pushNotice(989, left + 20 * 60 * 60, _lang(idx[id_1]));
#endif
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCLOG("applicationWillEnterForeground");
    
    cocos2d::extension::CCDevice::cancelNotice(987);
    cocos2d::extension::CCDevice::cancelNotice(988);
    cocos2d::extension::CCDevice::cancelNotice(989);
    
    cocos2d::CCDirector::sharedDirector()->resume();
    CCDirector::sharedDirector()->startAnimation();
    if (CCUserDefault::sharedUserDefault()->getBoolForKey(BG_MUSIC_ON, true))
    {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    SimpleAudioEngine::getInstance()->resumeAllEffects();
    GlobalData::shared()->isPause = false;
    GlobalData::shared()->isPressedHomeKey = true;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_APP_FOREGROUND);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChatServiceCocos2dx::resetResume();
#endif
    WorldController::getInstance()->doWhenBackToGame(NULL);
}

void AppDelegate::applicationDidBecomeActive()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AppLibHelper::triggerEventActivate();
#endif
}
