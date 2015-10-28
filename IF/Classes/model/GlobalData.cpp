//
//  GlobalData.cpp
//  IF
//
//  Created by 吴 凌江 on 13-8-28.
//
//

#include "GlobalData.h"
#include "CCCommonUtils.h"
#include "../Ext/CCDevice.h"

static GlobalData* _sharedInstance = NULL;

GlobalData* GlobalData::shared()
{
    if (_sharedInstance==NULL){
        _sharedInstance=new GlobalData();
        _sharedInstance->initRand();
        _sharedInstance->battleSpeedFactor = 1.0;
        _sharedInstance->m_harvestInfo = "";
        _sharedInstance->facebook_gift_k7 = 0;
        _sharedInstance->m_isFirstOpenDailyView = true;
        _sharedInstance->m_isFirstAskFor = false;
        _sharedInstance->isPressedHomeKey = false;
        _sharedInstance->isUploadPic = false;
        _sharedInstance->hd_switch = 0;
        _sharedInstance->isCrossService = false;
        _sharedInstance->alliance_territory_index = -1;//0哨塔 1超级矿 2箭塔 3仓库
        _sharedInstance->serverMax = 0;
        _sharedInstance->replayTime = 100000;
        _sharedInstance->cnReplayFlag = 0;
        _sharedInstance->sceneResourceStepLoad = 0;
        _sharedInstance->isChatOpen = true;
    }
    return _sharedInstance;
}

void GlobalData::purgeData() {
    delete _sharedInstance;
    _sharedInstance = NULL;
}



time_t GlobalData::getWorldTime()
{
    return worldTime + (time(NULL) - localTime) + timeZone*60*60;
}

time_t GlobalData::getTimeStamp() {
    return worldTime + (time(NULL) - localTime);
}

time_t GlobalData::changeTime(time_t t)
{
    if(t<=0) return 0;
    return t + timeZone*60*60;
}

double GlobalData::renewTime(double t)
{
    return t - timeZone*60*60*1000;
}

void GlobalData::setWorldTime(time_t t, int tz)
{
    worldTime=t;
    localTime=time(NULL);
    timeZone = tz;
}

int GlobalData::getTimeZone(){
    return timeZone;
}

void GlobalData::initRand(){
    srand((int)time(NULL)+11);//随机数的种子
    speedFactor = 1.0;
}

int GlobalData::getRand(int min,int max){
    return min+rand()%(max-min);
}

//判断是否走google play的支付
bool GlobalData::isGoogle(){
    if(GlobalData::shared()->analyticID == "market_global")
        return true;
//    if (GlobalData::shared()->platformUID != ""
//        || GlobalData::shared()->analyticID == "tstore"
//        || GlobalData::shared()->analyticID == "amazon"
//        )
//        return false;
    return false;
}

bool GlobalData::needLogin(){
    return isXiaoMiPlatForm();
}

bool GlobalData::needLogout(){
    if (analyticID == "cn_uc"
        || analyticID == "cn_360"
        ) {
        return true;
    }
    if (analyticID == "cn_mi"
        || analyticID == "cn_mihy"
        || analyticID == "cn_baidu"
        ) {
        return false;
    }
    if(analyticID.size() > 3){
        std::string sub = analyticID.substr(0, 3);
        if(sub == "cn_"){
            return cocos2d::extension::CCDevice::needPlatformQuit();
        }
    }
    return false;
}

bool GlobalData::isChinaPlatForm(){
    if(analyticID == "cn1" || analyticID == "tencent"){
        return true;
    }else{
        if(analyticID.size() > 3){
            std::string sub = analyticID.substr(0, 3);
            if(sub == "cn_"){
                return true;
            }
        }
    }
    return false;
}

bool GlobalData::isXiaoMiPlatForm(){
    bool flag = false;
    if(analyticID == "tencent"){
        flag = true;
    }else{
        if(analyticID.size() > 3){
            std::string sub = analyticID.substr(0, 3);
            if(sub == "cn_"){
                flag = true;
            }
        }
    }
    return flag;
}

bool GlobalData::cantSwtichAccount(){
    if (isXiaoMiPlatForm()
        ||analyticID == "common"
        ) {
        return true;
    }
    return false;    
}

bool GlobalData::isWeiboEnabled(){
    if(analyticID == "cn1" || (analyticID == "AppStore" && CCCommonUtils::getLanguage() == "zh_CN"))
        return true;
    return false;
}

bool GlobalData::isVKEnabled(){
    if(analyticID == "market_global" && CCCommonUtils::getLanguage() == "ru")
        return true;
    if(analyticID == "AppStore" && CCCommonUtils::getLanguage() == "ru")
        return true;
    return false;
}