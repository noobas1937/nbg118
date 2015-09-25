//
//  AchievementController.cpp
//  IF
//
//  Created by 李锐奇 on 15/3/18.
//
//

#include "AchievementController.h"
#include "AchieveGetCommand.h"
#include "AppLibHelper.h"
#include "FunBuildController.h"
#include <spine/Json.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

static AchievementController *_instance = NULL;

AchievementController *AchievementController::getInstance(){
    if(!_instance){
        _instance = new AchievementController();
        _instance->init();
    }
    return _instance;
}

void AchievementController::init(){
    purgeData();
    isNeedPostCompleteAchievementToGoogle = false;
    openFlag = FUNCTION_ON;
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AchievementController::postCompleteAchievement), MSG_RESPONSED_3RD_PLATFORM, NULL);
}

void AchievementController::purgeData(){
    isDataBack = false;
    firstOpen = true;
    m_infos.clear();
    allCompelete.clear();
    m_myMedalInfos.clear();
    m_otherMedalInfos.clear();
}

void AchievementController::updateAchievement(CCArray *arr){
    CCObject *obj;
    CCARRAY_FOREACH(arr, obj) {
        updateAchievement(_dict(obj), false);
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACHIEVEMENT_STATE_CHAGE);
}

void AchievementController::updateAchievement(CCDictionary *dict, bool postNotification){
    if(dict){
        std::string itemId = dict->valueForKey("id")->getCString();
        std::string key = getKeyByItemId(itemId);
        if (key.length() == 0)
        {
            return;
        }
        bool replaceFlag = false;
        if(m_infos.find(key) == m_infos.end()){
            //不存在，新加
            replaceFlag = true;
        }else{
            auto &info = m_infos[key];
            
            //理论上不存在一个系列2个未领奖的
            if(info.currentItemId == itemId){//相同id更新显示
                info.updateData(dict);
                if(m_infos[key].state == ACHIEVEMENT_REWARD_GET){
                    allCompelete[itemId] = 1;
                }

                if(postNotification){
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACHIEVEMENT_STATE_CHAGE);
                }
                return;
            }else{
                AchievementInfo newInfo = AchievementInfo();
                newInfo.initData(itemId);
                newInfo.updateData(dict);
                if(newInfo.state == ACHIEVEMENT_REWARD_GET){
                    allCompelete[itemId] = 1;
                }
                if(newInfo.isVisible){
                    if(!info.isVisible){//显示
                        replaceFlag = true;
                    }else{
                        if(newInfo.state != ACHIEVEMENT_REWARD_GET){//一个type只能显示一个
                            if(info.state != ACHIEVEMENT_REWARD_GET){
                                if(newInfo.showLevel < info.showLevel){//2个未领奖，低级显示取代高级
                                    replaceFlag = true;
                                }
                            }else{//未领奖取代已领奖
                                replaceFlag = true;
                            }
                        }else{
                            if(info.state == ACHIEVEMENT_REWARD_GET){//都领奖的高级取代低级
                                if(newInfo.showLevel >= info.showLevel){
                                    replaceFlag = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        if(replaceFlag){
            m_infos[key] = AchievementInfo();
            m_infos[key].initData(itemId);
            m_infos[key].updateData(dict);
            if(m_infos[key].state == ACHIEVEMENT_REWARD_GET){
                allCompelete[itemId] = 1;
            }

            if(postNotification){
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACHIEVEMENT_STATE_CHAGE);
            }
        }
    }
}

void AchievementController::refreshAllAchievementVisibleFlag(){
    bool isChange = false;
    for (auto it = m_infos.begin(); it != m_infos.end(); it++) {
        if(!isChange){
            isChange = it->second.refreshVisibleFlag();
        }else{
            it->second.refreshVisibleFlag();
        }
    }
    if(isChange){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACHIEVEMENT_VISIBLE_STATE_CHANGE);
    }
}

std::string AchievementController::getKeyByItemId(std::string itemId){
    return CCCommonUtils::getPropById(itemId, "series");
}

CCArray *AchievementController::getVisbleAchievement(){
    CCArray *arr = CCArray::create();
    double nowTime = GlobalData::shared()->getWorldTime();
    for (auto it = m_infos.begin(); it != m_infos.end(); it++) {
        if(it->second.isVisible){
            if (nowTime>=it->second.start && nowTime<=it->second.end) {//在时间范围内
                arr->addObject(CCString::create(it->first));
            }else if (it->second.state == ACHIEVEMENT_COMPELETE){//能领
                arr->addObject(CCString::create(it->first));
            }
        }
    }
    return arr;
}

void AchievementController::getReward(std::string itemId){
    AchievementController::getInstance()->doWhenComplete(itemId);
    AchieveGetRewardCommand *cmd = new AchieveGetRewardCommand(itemId);
    cmd->sendAndRelease();
}

void AchievementController::getDataFromServer(){
    AchieveGetCommand *cmd = new AchieveGetCommand();
    cmd->sendAndRelease();
}

void AchievementController::getMedalDataFormServer()
{
    MedalInitCommand* cmd = new MedalInitCommand();
    cmd->sendAndRelease();
}

void AchievementController::doWhenComplete(std::string itemId){
    if(GlobalData::shared()->analyticID != "market_global")
        return;
    std::string gp_id = CCCommonUtils::getPropById(itemId, "gp_id");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "com/clash/of/util/MyGooglePlusUtil", "unlockAchievements", "(Ljava/lang/String;)V") ) {
        CCLOGFUNC("jni: no method");
        return;
    }
    
    jstring jgp_id=minfo.env->NewStringUTF(gp_id.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jgp_id);
    minfo.env->DeleteLocalRef(jgp_id);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void AchievementController::doOpenGooglePlay(){
    if(GlobalData::shared()->analyticID != "market_global")
        return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/clash/of/util/MyGooglePlusUtil", "openLeaderBoards", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void AchievementController::doOpenGooglePlayAchievement(){
    if(GlobalData::shared()->analyticID != "market_global")
        return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/clash/of/util/MyGooglePlusUtil", "openAchievements", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void AchievementController::postCompleteAchievement(CCObject *obj){
    CCString* jsonCCStr=dynamic_cast<CCString*>(obj);
    string jsonStr=jsonCCStr->getCString();
    
    Json *jsonObj = Json_create(jsonStr.c_str());
    string _msgId=Json_getString(jsonObj, "msgId", "");
    string _userName=Json_getString(jsonObj, "userName", "");
    string _userId=Json_getString(jsonObj, "userId", "");
    Json_dispose(jsonObj);
    if (_msgId != ""){//有数据
        if (_msgId=="login_sucess_google") {
            postCompleteAchievementToGoogle();
        }
    }
}

void AchievementController::postCompleteAchievementToGoogle(){
    if(!isDataBack){
        isNeedPostCompleteAchievementToGoogle = true;
        return;
    }
    isNeedPostCompleteAchievementToGoogle = false;
    AppLibHelper::updatePlatformScore(FunBuildController::getInstance()->getMainCityLv());
    for (auto it = allCompelete.begin(); it != allCompelete.end(); it++) {
        doWhenComplete(it->first);
    }
}

void AchievementController::setOpenFlag(int i){
    openFlag = i;
}

void AchievementController::initMedalInfo(CCArray *arr)
{
    m_myMedalInfos.clear();
    CCDictionary* d = LocalController::shared()->DBXMLManager()->getGroupByKey("medal");
    CCDictElement* pelem = nullptr;
    CCDictionary* p = nullptr;
    CCDICT_FOREACH(d, pelem)
    {
        p = dynamic_cast<CCDictionary*>(pelem->getObject());
        m_myMedalInfos[pelem->getStrKey()] = MedalInfo();
        m_myMedalInfos[pelem->getStrKey()].initData(p);
    }
    if (arr == NULL)
    {
        updateMyMedalInfo();
        return;
    }
    CCObject* obj = NULL;
    CCDictionary* dict = NULL;
    string key = "";
    CCARRAY_FOREACH(arr, obj)
    {
        dict = dynamic_cast<CCDictionary*>(obj);
        if (dict)
        {
            key = dict->valueForKey("id")->getCString();
            if (key.length() == 0)
            {
                continue;
            }
            if (m_myMedalInfos.find(key) == m_myMedalInfos.end())
            {
                m_myMedalInfos[key] = MedalInfo();
                m_myMedalInfos[key].initData(key);
            }
            else
            {
                m_myMedalInfos[key].level = dict->valueForKey("level")->intValue();
                m_myMedalInfos[key].hasGet = true;
            }
        }
    }
}

void AchievementController::updateMyMedalInfo()
{
    map<string, AchievementInfo>::iterator it = m_infos.begin();
    string medalId = "";
    for (; it != m_infos.end(); it++)
    {
        medalId = it->second.medalId;
        if (m_myMedalInfos.find(medalId) == m_myMedalInfos.end())
        {
            m_myMedalInfos[medalId] = MedalInfo();
            m_myMedalInfos[medalId].initData(medalId);
        }
        CCLOG("%d,%d", it->second.state, it->second.showOrder);
        
        if (it->second.state == ACHIEVEMENT_REWARD_GET)
        {
            m_myMedalInfos[medalId].level = it->second.showOrder>=m_myMedalInfos[medalId].level?it->second.showOrder:m_myMedalInfos[medalId].level;
        }
        else
        {
            m_myMedalInfos[medalId].level = (it->second.showOrder-1)>=m_myMedalInfos[medalId].level?(it->second.showOrder-1):m_myMedalInfos[medalId].level;
        }
        if (m_myMedalInfos[medalId].level >= 3)
        {
            m_myMedalInfos[medalId].hasGet = true;
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACHIEVEMENT_MEDAL_GET);
}

void AchievementController::getOtherMedalInfo(CCArray* a)
{
    m_otherMedalInfos.clear();
    CCDictionary* d = LocalController::shared()->DBXMLManager()->getGroupByKey("medal");
    CCDictElement* pelem = nullptr;
    CCDictionary* p = nullptr;
    CCDICT_FOREACH(d, pelem)
    {
        p = dynamic_cast<CCDictionary*>(pelem->getObject());
        m_otherMedalInfos[pelem->getStrKey()] = MedalInfo();
        m_otherMedalInfos[pelem->getStrKey()].initData(p);
    }
    CCObject* obj = nullptr;
    string id = "";
    CCARRAY_FOREACH(a, obj)
    {
        p = dynamic_cast<CCDictionary*>(obj);
        id = p->valueForKey("id")->getCString();
        m_otherMedalInfos[id].hasGet = true;
        m_otherMedalInfos[id].level = p->valueForKey("level")->intValue();
    }
}

CCArray* AchievementController::getOtherMedalInfo()
{
    CCArray* a = CCArray::create();
    map<string, MedalInfo>::iterator lt = m_otherMedalInfos.begin();
    for (; lt!=m_otherMedalInfos.end(); lt++)
    {
        a->addObject(ccs(lt->first));
    }
    return a;
}

CCArray* AchievementController::getMyMedalInfo()
{
    CCArray* a = CCArray::create();
    map<string, MedalInfo>::iterator lt = m_myMedalInfos.begin();
    for (; lt!=m_myMedalInfos.end(); lt++)
    {
        if (lt->first.length()>0 && lt->second.medalId>0)
        {
            a->addObject(ccs(lt->first));
        }
    }
    return a;
}

float AchievementController::getAchieveProgress()
{
    int totalCnt = LocalController::shared()->DBXMLManager()->getGroupByKey("achievement")->count();
    int comCnt = 0;
    map<string, AchievementInfo>::iterator it = m_infos.begin();
    for (; it!=m_infos.end(); it++)
    {
        if (it->second.state==ACHIEVEMENT_COMPELETE)
        {
            comCnt++;
        }
    }
    comCnt += allCompelete.size();
    comCnt = comCnt>totalCnt?totalCnt:comCnt;
    float progress = (comCnt*1.0f)/(totalCnt*1.0f);
    
    return progress;
}

int AchievementController::getMedalComCnt()
{
    int totalCnt = 0;
    map<string, MedalInfo>::iterator it = m_myMedalInfos.begin();
    for (; it != m_myMedalInfos.end(); it++)
    {
        if (it->second.hasGet)
        {
            totalCnt++;
        }
    }
    return totalCnt;
}

string AchievementController::getMedalIconName(string medalId, int type)
{
    if (medalId == "")
    {
        return "";
    }
    string name = LocalController::shared()->DBXMLManager()->getObjectByKey(medalId)->valueForKey("icon_main")->getCString();
    name.append(".png");
    return name;
}

void AchievementController::changePlayerMedalId()
{
    MedalSaveCommand* cmd = new MedalSaveCommand(GlobalData::shared()->medalid);
    cmd->sendAndRelease();
}

void AchievementController::firstOpenPopup()
{
    if (firstOpen)
    {
        firstOpen = false;
        AchieveFindCmd* cmd = new AchieveFindCmd();
        cmd->sendAndRelease();
    }
}

int AchievementController::getRewardAchieve()
{
    int rewardNum = 0;
    map<string, AchievementInfo>::iterator it = m_infos.begin();
    for (; it != m_infos.end(); it++)
    {
        if (it->second.state == ACHIEVEMENT_COMPELETE)
        {
            rewardNum++;
        }
    }
    return rewardNum;
}

void AchievementController::getMedalIconColor(string medalId, map<int, vector<float>> &colorMap)
{
    colorMap.clear();
    if (m_myMedalInfos.find(medalId) == m_myMedalInfos.end())
    {
        return;
    }
    MedalInfo* medal = &m_myMedalInfos[medalId];
    string color1 = "";
    string color2 = "";
    switch (medal->icon_color)
    {
        case MEDAL_RED:
            color1 = "255,100,0,100,0,0,0,0";
            color2 = "200,50,0,100,5,0,10,0";
            break;
            
        case MEDAL_BULE:
            color1 = "0,150,255,100,0,0,5,0";
            color2 = "0,150,255,100,0,0,5,0";
            break;
            
        case MEDAL_PURPLE:
            color1 = "100,50,255,100,0,0,0,0";
            color2 = "55,35,255,100,0,0,10,0";
            break;
            
        default:
            color1 = "255,100,0,100,0,0,0,0";
            color2 = "200,50,0,100,5,0,10,0";
            break;
    }
    vector<string> strVec;
    CCCommonUtils::splitString(color1, ",", strVec);
    vector<float> floVec;
    for (int i = 0; i<strVec.size(); i++)
    {
        floVec.push_back(atof(strVec.at(i).c_str())/255.0f);
    }
    colorMap[1] = floVec;
    
    strVec.clear();
    CCCommonUtils::splitString(color1, ",", strVec);
    vector<float> floVec2;
    for (int i = 0; i<strVec.size(); i++)
    {
        floVec2.push_back(atof(strVec.at(i).c_str())/255);
    }
    colorMap[2] = floVec2;
}
