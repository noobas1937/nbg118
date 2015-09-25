//
//  LogoutCommand.cpp
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#include "LogoutCommand.h"
#include "NetController.h"
#include "SceneController.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "DragonBuildingController.h"
#include "GuideController.h"
#include "WorldController.h"
#include "CCThreadManager.h"
#include "ToolController.h"
#include "ChatController.h"
#include "ScienceController.h"
#include "QueueController.h"
#include "EnemyInfoController.h"
#include "QuestController.h"
#include "ActivityController.h"
#include "TimerController.h"
#include "FBUtilies.h"
#include "AchievementController.h"
#include "../../Ext/CCDevice.h"
#include "UserBindCommand.h"
#include "ChatServiceCocos2dx.h"
#include "ShakeController.h"
LogoutCommand::LogoutCommand()
: CommandBase(LOGOUT_COMMAND)
{
    
}

bool LogoutCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare("logout") != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (params->objectForKey("zoneName")) {
        CCLOG("logout success");
        m_dic = params;
        this->retain();
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(LogoutCommand::doReload), this, 0.0f,0, 0.02f, false);
        return true;
    }
    
    return true;
}

void LogoutCommand::doReload(float f)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::isAccountChanged = true;
#endif
    
    callSuccess(NetResult::create());
    NetController::shared()->disconnect();
    NetController::shared()->setIsChangingServer(true);
    NetController::shared()->setIsLogout(true);
    bool clearFlag = false;//国内渠道切换账号，会重新创建loading
    if(m_dic->objectForKey("xiaomi")){
        clearFlag = true;
    }
    SceneController::getInstance()->gotoLoading(clearFlag);
    
    // record login success
    GlobalData::shared()->playerInfo.allianceInfo.reset();
    ActivityController::getInstance()->eventInfo.reset();
    PopupViewController::getInstance()->removeAllPopupView();
    SceneController::getInstance()->clearLayerByLevelAndSceneId(LEVEL_GUI,SCENE_ID_GUI);
    FunBuildController::purgeData();
    DragonBuildingController::purgeData();
    ToolController::purgeData();
    ChatController::purgeData();
    ChristmasActController::purgeData();
    ScienceController::purgeData();
    QueueController::purgeData();
    QuestController::purgeData();
    TimerController::purgeData();
    string gcmID = GlobalData::shared()->gcmRegisterId;
    int country = GlobalData::shared()->loginServerInfo.recommandCountry;
    string fromCountry = GlobalData::shared()->fromCountry;
    int resType = GlobalData::shared()->contryResType;
    std::string analyId = GlobalData::shared()->analyticID;
    std::string version = GlobalData::shared()->version;
    bool loginFlag = GlobalData::shared()->isLoginFlag;
    std::string platformUid = GlobalData::shared()->platformUID;
    
    GlobalData::purgeData();
    GlobalData::shared()->loginServerInfo.recommandCountry = country;
    GlobalData::shared()->gcmRegisterId = gcmID;
    GlobalData::shared()->contryResType = resType;
    GlobalData::shared()->fromCountry = fromCountry;
    GlobalData::shared()->analyticID = analyId;
    GlobalData::shared()->version = version;
    GlobalData::shared()->isLoginFlag = loginFlag;
    GlobalData::shared()->platformUID = platformUid;
    ShakeController::getInstance()->resetShakeDate();
    GuideController::share()->purgeData();
    GameController::purgeData();
    UIComponent::purgeData();
    AchievementController::getInstance()->purgeData();
    PortActController::purgeData();
    WorldController::getInstance()->doWhenBackToGame(NULL, true);
    EnemyInfoController::getInstance()->purgeData();
    //        CCThreadManager::getInstance()->runInMainThread(SceneController::getInstance(), callfuncO_selector(SceneController::gotoLoading),NULL);
    
    //防止输入键盘开着的时候，重登游戏，没有恢复popup层
    auto popupLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP_IN);
    popupLayer->setPositionY(0);
    //        CCUserDefault::sharedUserDefault()->setStringForKey(GUIDE_STEP, "");
}
/////
bool NewACCCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(NEWACCOUNT_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if(params && params->objectForKey("errorCode")) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_RET_WARING_ACC);
        return true;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::isAccountChanged = true;
#endif
    
    if(FBUtilies::fbIsLogin()){//如果fb是登录状态的token 要把它清掉
        FBUtilies::fbLogout();
        CCUserDefault::sharedUserDefault()->setIntegerForKey(FUN_BUILD_MAIN_LEVEL, 0);
    }
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "");
    CCUserDefault::sharedUserDefault()->setStringForKey(SERVER_ID, "");
    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "");
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, "");
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, "");
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,0);
    CCUserDefault::sharedUserDefault()->setBoolForKey("TASK_TIPS_ON", true);
    
    CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","");
    CCUserDefault::sharedUserDefault()->flush();
    
    cocos2d::extension::CCDevice::cancelNotice(-1);
    CCLog("cancel all notice , new account !");
    
    UserCleanPostCommand* cleanCmd = new UserCleanPostCommand();
    cleanCmd->sendAndRelease();
    
    NetController::shared()->disconnect();
    LogoutCommand* cmd = new LogoutCommand();
    CCDictionary* _params = CCDictionary::create();
    _params->setObject(CCString::create("cok"), "zoneName");
//    _params->setObject(CCString::create("true"), "isReLogin");
    CCDictionary* dict1 = CCDictionary::create();
    dict1->setObject(CCString::create("logout"), "cmd");
    dict1->setObject(_params, "params");
    cmd->handleRecieve(dict1);
    
    return true;
}
