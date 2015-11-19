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
    pDirector->setAnimationInterval(1.0 / 15);
    
//    CCLOG("Time profiler: [%ld] ,line: [%d]",CCMathUtils::getCurrentTime(),__LINE__);
    
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
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCLOG("applicationWillEnterForeground");
    cocos2d::extension::CCDevice::cancelNotice(987);
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
