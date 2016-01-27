//
//  AchievementController.cpp
//  IF
//
//  Created by 李锐奇 on 15/3/18.
//
//

#include "RecommendAllianceController.h"
#include "AchieveGetCommand.h"
#include "AppLibHelper.h"
//begin a by ljf
#include "PopupViewController.h"
#include "CreateAllianceView.h"
#include "AlertAddAllianceView.h"
#include "CreateAllianceViewRecommend.h"
#include "AlertAddAllianceViewRecommend.h"
#include "RecommendAllianceCommand.h"
//end a by ljf

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

#endif

static RecommendAllianceController *_instance = NULL;

RecommendAllianceController *RecommendAllianceController::getInstance()
{
    if(!_instance){
        _instance = new RecommendAllianceController();
        _instance->init();
    }
    return _instance;
}

void RecommendAllianceController::init(){
    //purgeData();
    applyRecommendAllianceFailTimes = 0;
    isShowed = false;
    isCreate = -1;
    toShowCreateAfterFailThreeTimes = false;
    hasSendRequest = false;
    if(!allInfo)
        allInfo = new AllianceInfo();
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(_instance, callfuncO_selector(RecommendAllianceController::onPlayerLevelUp), MSG_PLAYER_LEVELUP, NULL);
}

void RecommendAllianceController::purgeData(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(_instance, MSG_PLAYER_LEVELUP);
    if(allInfo)
    {
        delete allInfo;
        allInfo = nullptr;
    }
    if(_instance)
    {
        delete _instance;
        _instance = NULL;
    }
}

void RecommendAllianceController::updateRecommendAllianceInfo(CCDictionary *pDict){
    /*
    CCObject *obj;
    CCARRAY_FOREACH(arr, obj) {
        updateAchievement(_dict(obj), false);
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACHIEVEMENT_STATE_CHAGE);
    */
    /*
     string allianceId;
     string allianceName;
     string language;
     string leaderName;
     int maxMember;
     int curMember;
     string allianceIcon;
     double fightPower;
     int isCreate;
    */
    if(!pDict)
        return;
    isCreate = pDict->valueForKey("isCreate")->intValue();
    if(isCreate == 0)
    {
        allianceId = pDict->valueForKey("allianceId")->getCString();
        allianceName = pDict->valueForKey("allianceName")->getCString();
        shortName = pDict->valueForKey("shortName")->getCString();
        language = pDict->valueForKey("language")->getCString();
        leaderName = pDict->valueForKey("leaderName")->getCString();
        maxMember = pDict->valueForKey("maxMember")->intValue();
        curMember = pDict->valueForKey("curMember")->intValue();
        allianceIcon = pDict->valueForKey("allianceIcon")->getCString();
        fightPower = pDict->valueForKey("fightPower")->doubleValue();
        isCreate = pDict->valueForKey("isCreate")->intValue();
        
        
        allInfo->uid = allianceId;
        allInfo->name = allianceName;
        allInfo->shortName = shortName;
        allInfo->language = language;
        allInfo->leader = leaderName;
        allInfo->maxNum = maxMember;
        allInfo->currentNum = curMember;
        allInfo->icon = allianceIcon;
        //allInfo->fightpower = fightPower;
        allInfo->totalForce = fightPower;
    }
    
    
}

void RecommendAllianceController::popRecommendAlliance(int createFlag)
{
    if(createFlag == 1)
    {
        PopupViewController::getInstance()->addPopupView(CreateAllianceViewRecommend::create(11));
        //PopupViewController::getInstance()->pushPop(AlertAddAllianceViewRecommend::create(allInfo), true);
    }
    if(createFlag == 0)
    {
        /*
         std::string name;//联盟名
         std::string shortName;//联盟简称
         int currentNum;//联盟当前人数
         int maxNum;//联盟最大人数
         int level;
         std::string language;
         unsigned long totalForce;//联盟总战斗力
         */
        CCCommonUtils::setIsHDViewPort(true);
        PopupViewController::getInstance()->addPopupView(AlertAddAllianceViewRecommend::create(allInfo));
        //PopupViewController::getInstance()->pushPop(AlertAddAllianceViewRecommend::create(allInfo), true);
    }
}

void RecommendAllianceController::checkToPopRecommendAlliance()
{
    if(applyRecommendAllianceFailTimes < 3)
    {
        if(isCreate == -1) //未发过请求的情况，适用于还没到4级，只要到4级，都不会是这种
        {
            return;
        }
        if(isShowed == false) //本次使用未展示过
        {
            if(PopupViewController::getInstance()->getCurrentPopupView() == nullptr)
            {
                popRecommendAlliance(isCreate);
                isShowed = true;
            }
        }
    }
    if(toShowCreateAfterFailThreeTimes)
    {
        if(PopupViewController::getInstance()->getCurrentPopupView() == nullptr)
        {
            popRecommendAlliance(1);
            toShowCreateAfterFailThreeTimes = false;
        }
    }
    /* 先不管这种情况
    else //展示过，但申请加入一直失败
    {
        if((applyAllianceTimes > 0) && (applyAllianceTimes <= 3))
        {
            //继续申请
            RecommendAllianceCommand* command = new RecommendAllianceCommand();
            command->sendAndRelease();
        }
        if(applyAllianceTimes > 3)
        {
            if(PopupViewController::getInstance()->getCurrentPopupView() == nullptr)
            {
                popRecommendAlliance(1);
                
            }
        }
    }
    */
}

void RecommendAllianceController::sendRecommendRequest()
{
    RecommendAllianceCommand* command = new RecommendAllianceCommand();
    command->sendAndRelease();
}

void RecommendAllianceController::checkToSendRecommendRequest()
{
    if(applyRecommendAllianceFailTimes < 3)
    {
        //tmp d by lj if(hasSendRequest)
            //return;
        if(GlobalData::shared()->playerInfo.level >= 4) //用户等级没有到4
        {
            //tmp d by ljf if(GlobalData::shared()->playerInfo.onceJoinedAlliance == 0) //没有加入过联盟
            {
                //if(GlobalData::shared()->playerInfo.isfirstJoin == 0)
                {
                    //if(hasSendRequest == false)
                    {
                        sendRecommendRequest();
                        //hasSendRequest = true;
                    }
                }
            }
        }
    }
    if(applyRecommendAllianceFailTimes == 3)
    {
        applyRecommendAllianceFailTimes = 4;
        toShowCreateAfterFailThreeTimes = true;
    }
    /*
    if(applyRecommendAllianceFailTimes < 3)
    {
        sendRecommendRequest();
    }
    if(applyRecommendAllianceFailTimes == 3) //满3次，弹创建窗口
    {
        applyRecommendAllianceFailTimes = 4;
        
    }
    */
    //等级是否到4
    //每次登陆
    //三次失败
}

void RecommendAllianceController::onPlayerLevelUp(CCObject* obj)
{
    int newLevel = GlobalData::shared()->playerInfo.level;
    if(newLevel == 4)
    {
        checkToSendRecommendRequest();
    }
}

void RecommendAllianceController::onApplyRecommendAllianceFail()
{
    applyRecommendAllianceFailTimes = applyRecommendAllianceFailTimes + 1;
    if(applyRecommendAllianceFailTimes < 3)
    {
        resetData();
        checkToSendRecommendRequest();
    }
}

void RecommendAllianceController::resetData()
{
    isShowed = false;
    isCreate = -1;
    hasSendRequest = false;
}