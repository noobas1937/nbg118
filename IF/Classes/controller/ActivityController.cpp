//
//  ActivityController.cpp
//  IF
//
//  Created by 李锐奇 on 14-4-3.
//
//

#include "ActivityController.h"
#include "PopupViewController.h"
#include "ActivityTipView.h"
#include "ActivityBeginView.h"
#include "ActivityHistoryView.h"
#include "DataRecordCommand.h"
#include "FunBuildController.h"
#include "ActivityEventCommand.h"
#include "RewardController.h"
#include "WorldController.h"
#include "PortActCommand.h"
#include "ActivityEventObj.h"
#include "ActivityListView.h"
#include "GCMRewardController.h"
#include "SceneController.h"
#include "ToolController.h"
#include "FeedRewardCommand.h"
#include "BaoZangView.h"
#include "GuideController.h"
#include "PortActView.h"
#include "../Ext/CCDevice.h"
#include "SoundController.h"
#include "FBUtilies.h"
#include "ActExcCommand.h"
#include "CCMathUtils.h"
#include "ActivityExcView.h"
#include "CrossServiceActivityView.h"
#include "VipUtil.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "NetController.h"
#include "LogoutCommand.h"
#include "FlyHint.h"
#include "CCMathUtils.h"
#include "RechargeACTVCell.h"
#include "ActivityPopupView.h"
#include "QuestController.h"
#include "TrainingView.h"
#include "LuaController.h"

#define RECHARGE_ACTV_TOMORROW_TIME "recharge.actv.tomorrow.time.mark"
#define RECHARGE_ACTV_POPUP_VIEW_IDX "recharge.actv.popup.view.idx"

static ActivityController *_instance = NULL;

ActivityController::ActivityController(){
    allianceKill = CCArray::create();
    gole = CCArray::create();
    memberKill = CCArray::create();
    wb_condition_k1 = 0;
    wb_condition_k2 = 0;
    wb_condition_k3 = 0;
    wb_info_k2 = "";
    wb_info_k7 = 1;
    wb_info_k6 = 24;
    wb_info_k9 = 1;
    wb_info_k10 = 1.0;
    wb_info_k11 = 3;
    wb_timing_k1 = 600;
    wb_timing_k2 = 200;
    ckfNeedLeave = 0;
    m_userScore = 0;
    m_userRank = 0;
    m_kingdomScore = 0;
    m_kingdomRank = 0;
    ckfServerInfo = NULL;
    m_ckfMemberReward = CCArray::create();
    m_ckfKingdomReward = CCArray::create();
    popCntLimit = 0;
    ckfProtectTime = 0;
    
    //试炼场活动
    m_freeAtk = 0;
    m_screen.clear();
    m_blood = 0;
    m_monster = "";
    m_maxBlood = 0;
    m_freeTrial = 0;
    m_maxAtkCnt = 0;//每局玩家可攻击怪兽的次数
    m_atkHarm.clear();//三种攻击对怪物的伤害
    m_isTrialOpen = 0;//试炼场活动开关
    m_bigStrengthGold = 0;//蓄力攻击花费金币数量
    m_moreAtkGold = 0;//增加攻击次数花费金币数量
    m_addedAtkCnt = 0;//花金币增加的攻击次数数量
    m_newTrialGold = 0;//新挑战花费的金币数量
    m_isInAtkCmdSending = false;//正在发送攻击命令
    m_bigStrenMul = 0;
    m_monsterDiv.clear();
    baozangHelpArr = CCArray::create();
}
ActivityController::~ActivityController(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PORT_DATA_INIT);
}
ActivityController *ActivityController::getInstance(){
    if(_instance == NULL){
        _instance = new ActivityController();
    }
    return _instance;
}

void ActivityController::createRwdViewByActId(string actId)
{
    if (m_exc2RefInfo.find(actId) == m_exc2RefInfo.end()) {
        return;
    }
    CCObject* obj = nullptr;
    bool st = false;
    CCARRAY_FOREACH(activityArr, obj)
    {
        auto aObj = dynamic_cast<ActivityEventObj*>(obj);
        if (aObj->id == actId && aObj->exchange == "2" && aObj->type == 6) {
            st = true;
            break;
        }
    }
    if (st) {
        auto aObj = dynamic_cast<ActivityEventObj*>(obj);
        PopupViewController::getInstance()->addPopupView(ActivityExcNewView::create(aObj));
    }
}

bool ActivityController::isHasAct(string actId)
{
    CCObject* obj = nullptr;
    bool st = false;
    CCARRAY_FOREACH(activityArr, obj)
    {
        auto aObj = dynamic_cast<ActivityEventObj*>(obj);
        if (aObj->id == actId) {
            st = true;
            break;
        }
    }
    return st;
}

bool ActivityController::checkCanExchange2(string actId, int idx)
{
    if (m_exc2RefInfo.find(actId) == m_exc2RefInfo.end() || m_exc2Data.find(actId) == m_exc2Data.end())
    {
        return false;
    }
    if (m_exc2Data[actId].size() < idx) {
        return false;
    }
    map<string, int> cntMap;
    for (auto it = m_exc2Data[actId][idx].m_ids.begin(); it != m_exc2Data[actId][idx].m_ids.end(); ++it) {
        if (cntMap.find(*it) == cntMap.end()) {
            cntMap[*it] = 1;
        } else {
            cntMap[*it] = cntMap[*it] + 1;
        }
    }
    bool st = true;
    for (auto it = cntMap.begin(); it != cntMap.end(); ++it) {
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(atoi(it->first.c_str()));
        if (toolInfo.getCNT() < it->second) {
            st = false;
            break;
        }
    }
    return st;
}

bool ActivityController::checkIsToday(time_t time)
{
    if (GlobalData::shared()->getTimeStamp() - time > 24 * 60 * 60) {
        return false;
    }
    
    time_t time2 = GlobalData::shared()->getTimeStamp();
    struct tm tm;
    tm = *localtime(&time);
    struct tm tm2;
    tm2 = *localtime(&time2);
    if (tm.tm_mday != tm2.tm_mday) {
        return false;
    } else {
        return true;
    }
}

void ActivityController::startGetAct6Data()
{
    auto& arr = activityArr;
    if (arr->count() == 0) {
        return;
    }
    CCObject* item = nullptr;
    ActivityEventObj* obj = nullptr;
    std::vector<std::string> vec;
    bool st = false;
    CCARRAY_FOREACH(arr, item)
    {
        obj = dynamic_cast<ActivityEventObj*>(item);
        if (obj->type == 6)
        {
            if (obj->exchange == "2" || obj->exchange == "1") {
                vec.push_back(obj->id);
            }
        }
    }
    std::string ts("");
    if (vec.size()) {
        for (int i = 0; i < vec.size() - 1; ++i) {
            ts += vec[i];
            ts += "|";
        }
        ts += vec[vec.size() - 1];
        GetAct6ExcDataCommand* cmd = new GetAct6ExcDataCommand(ts);
        cmd->sendAndRelease();
    }
}

void ActivityController::endGetAct6Data(CCDictionary* params)
{
    if (params->objectForKey("errorCode")) {
        CCCommonUtils::flyText(_lang(params->valueForKey("errorCode")->getCString()));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GET_ACT6_DATA_FAIL);
    } else if (params->objectForKey("exchange")){
        auto arr = dynamic_cast<CCArray*>(params->objectForKey("exchange"));
        CCObject* obj = nullptr;
        CCDictionary* dic = nullptr;
        CCARRAY_FOREACH(arr, obj)
        {
            dic = dynamic_cast<CCDictionary*>(obj);
            string actId = dic->valueForKey("actId")->getCString();
            if (m_exc2RefInfo.find(actId) != m_exc2RefInfo.end()) {
                if (dic->objectForKey("freeNum")) {
                    m_exc2RefInfo[actId].m_freeNum = dic->valueForKey("freeNum")->intValue();
                }
                if (dic->objectForKey("refresh")) {
                    m_exc2RefInfo[actId].m_refresh = dic->valueForKey("refresh")->intValue();
                }
                if (dic->objectForKey("refreshCost")) {
                    m_exc2RefInfo[actId].m_refreshCost = dic->valueForKey("refreshCost")->intValue();
                }
                if (dic->objectForKey("exchangeId")) {
                    string exchangeId = dic->valueForKey("exchangeId")->getCString();
                    m_exc2RefInfo[actId].m_excIds.clear();
                    CCCommonUtils::splitString(exchangeId, ";", m_exc2RefInfo[actId].m_excIds);
                }
                m_exc2RefInfo[actId].m_refreshTime = GlobalData::shared()->getTimeStamp();
                if (dic->objectForKey("info")) {
                    CCArray* info = dynamic_cast<CCArray*>(dic->objectForKey("info"));
                    int num = info->count();
                    CCDictionary* infoDic = nullptr;
                    for (int i = 1; i <= num; ++i) {
                        infoDic = dynamic_cast<CCDictionary*>(info->objectAtIndex(i - 1));
                        m_exc2Data[actId][i] = ActExc2Info(infoDic);
                    }
                }
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GET_ACT6_DATA_SUCCESS);
    }
}

void ActivityController::startAct6Exchange(string actId, int idx)
{
    if (m_exc2Data.empty()) {
        return;
    }
    if (m_exc2Data.find(actId) == m_exc2Data.end()) {
        return;
    }
    if (m_exc2Data[actId].size() < idx) {
        return;
    }
    int type = m_exc2Data[actId][idx].m_type;
    auto cmd = new Act6ExcCommand(actId, type);
    cmd->sendAndRelease();
}

void ActivityController::endAct6Exchange(CCDictionary* params)
{
    int tIdx = 0;
    if (params->objectForKey("errorCode")) {
        CCCommonUtils::flyText(_lang(params->valueForKey("errorCode")->getCString()));
        string actId = params->valueForKey("tempActId")->getCString();
        int type = params->valueForKey("type")->intValue();
        if (actId == "") {
            return;
        }
        for (auto it = m_exc2Data[actId].begin(); it != m_exc2Data[actId].end(); ++it) {
            if (it->second.m_type == type) {
                tIdx = it->first;
                break;
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ACT6_EXC_FAIL, CCInteger::create(tIdx));
    } else {
        string actId = params->valueForKey("actId")->getCString();
        if (actId == "") {
            return;
        }
        if (params->objectForKey("exchange")){
            auto excDic = dynamic_cast<CCDictionary*>(params->objectForKey("exchange"));
            int type = excDic->valueForKey("type")->intValue();
            map<int, ActExc2Info>::iterator it;
            int idx = 0;
            for (it = m_exc2Data[actId].begin(); it != m_exc2Data[actId].end(); ++it) {
                if (it->second.m_type == type) {
                    idx = it->first;
                    tIdx = idx;
                    break;
                }
            }
            if (m_exc2Data[actId].find(idx) != m_exc2Data[actId].end()) {
                auto& ids = m_exc2Data[actId][idx].m_ids;
                for (int i = 0; i < ids.size(); ++i)
                {
                    auto& toolInfo = ToolController::getInstance()->getToolInfoById(atoi(ids[i].c_str()));
                    int tCnt = toolInfo.getCNT() - 1;
                    toolInfo.setCNT(tCnt);
                }
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
                m_exc2Data[actId][idx].update(excDic);
            }
        }
        if (params->objectForKey("reward")) {
            auto rwdArr = dynamic_cast<CCArray*>(params->objectForKey("reward"));
            if (rwdArr)
            {
                PortActController::getInstance()->flyReward(rwdArr, true);
                string rwdInfo = RewardController::getInstance()->retReward(rwdArr);
                CCCommonUtils::flyHint("", "", rwdInfo);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ACT6_EXC_SUCCESS, CCInteger::create(tIdx));
    }
}

void ActivityController::startAct6Refresh(string actId)
{
    CCLOGFUNC();
    auto cmd = new Act6RefreshCommand(actId);
    cmd->sendAndRelease();
}

void ActivityController::endAct6Refresh(cocos2d::CCDictionary * params)
{
    CCLOGFUNC();
    string actId = params->valueForKey("m_actId")->getCString();
    if (params->objectForKey("errorCode")) {
        CCCommonUtils::flyText(_lang(params->valueForKey("errorCode")->getCString()));
        CCLOG("POST MSG_ACT6_REFRESH_FAIL");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ACT6_REFRESH_FAIL, CCString::create(actId));
    } else {
        if (params->objectForKey("flush")) {
            CCLOG("---START FLUSH---");
            auto dic = dynamic_cast<CCDictionary*>(params->objectForKey("flush"));
            if (m_exc2RefInfo.find(actId) != m_exc2RefInfo.end()) {
                if (dic->objectForKey("freeNum")) {
                    m_exc2RefInfo[actId].m_freeNum = dic->valueForKey("freeNum")->intValue();
                }
                if (dic->objectForKey("refresh")) {
                    m_exc2RefInfo[actId].m_refresh = dic->valueForKey("refresh")->intValue();
                }
                if (dic->objectForKey("refreshCost")) {
                    m_exc2RefInfo[actId].m_refreshCost = dic->valueForKey("refreshCost")->intValue();
                }
                if (dic->objectForKey("exchangeId")) {
                    string exchangeId = dic->valueForKey("exchangeId")->getCString();
                    m_exc2RefInfo[actId].m_excIds.clear();
                    CCCommonUtils::splitString(exchangeId, ";", m_exc2RefInfo[actId].m_excIds);
                }
                m_exc2RefInfo[actId].m_refreshTime = GlobalData::shared()->getTimeStamp();
                if (dic->objectForKey("info")) {
                    CCArray* info = dynamic_cast<CCArray*>(dic->objectForKey("info"));
                    int num = info->count();
                    CCDictionary* infoDic = nullptr;
                    for (int i = 1; i <= num; ++i) {
                        infoDic = dynamic_cast<CCDictionary*>(info->objectAtIndex(i - 1));
                        m_exc2Data[actId][i] = ActExc2Info(infoDic);
                    }
                }
                CCLOG("POST MSG_ACT6_REFRESH_SUCCESS");
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ACT6_REFRESH_SUCCESS, CCString::create(actId));
            }
        } else {
            CCLOG("POST MSG_ACT6_REFRESH_FAIL");
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ACT6_REFRESH_FAIL, CCString::create(actId));
        }
    }
}

void ActivityController::createGoodsIconWithGray(cocos2d::CCNode *parent, string itemId, float size)
{
    createGoodsIcon(parent, itemId, size);
    CCSprite* spr = nullptr;
    spr = dynamic_cast<CCSprite*>(parent->getChildByTag(888));
    if (!spr)
    {
        return;
    }
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
    if (info.getCNT() <= 0) {
        CCCommonUtils::setSpriteGray(spr, true);
    } else {
        CCCommonUtils::setSpriteGray(spr, false);
    }
}

void ActivityController::createGoodsIcon(cocos2d::CCNode *parent, string itemId, float size)
{
    auto icon = CCLoadSprite::createSprite(CCCommonUtils::getIcon(itemId).c_str(),true,CCLoadSpriteType_GOODS);
    CCCommonUtils::setSpriteMaxSize(icon, size, true);
    icon->setTag(888);
    parent->addChild(icon);
    auto& tool = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
    if(tool.type==3 && !tool.getPara().empty()){
        CCScale9Sprite* numBG = CCLoadSprite::createScale9Sprite("BG_quatnity.png");
        numBG->setColor(CCCommonUtils::getItemColor(tool.color));
        numBG->setOpacity(200);
        CCLabelBMFont* numIF = CCLabelBMFont::create(CC_ITOA_K(atol(tool.getPara().c_str())), "pve_fnt_boss.fnt");
        CCSize numSize = numIF->getContentSize();
        const float constScale = 0.3;
        float scale = size * constScale / numSize.height;
        if((numSize.width * scale) > size){
            scale = size/numSize.width;
        }
        numIF->setScale(scale);
        numSize.height *= scale;
        numBG->setPreferredSize(CCSize(size, size * constScale));
        parent->addChild(numBG);
        parent->addChild(numIF);
        numIF->setPositionY(size * constScale * 0.5);
        numBG->setPosition(numIF->getPosition());
    }
}

string ActivityController::createRandomGoodsId(string actId, int type)
{
    if (type == 2) {
        auto& ids = m_exc2RefInfo[actId].m_excIds;
        int idx = CCMathUtils::getRandomInt(0, ids.size() - 1);
        idx = CCMathUtils::getRandomInt(0, ids.size() - 1);
        return ids[idx];
    } else {
        return "";
    }
}

bool ActivityController::checkShengDianOpen(){
    bool flag = false;
    if (wb_switch_k3==0) {
        return false;
    }
    flag = checkOpenVersion(wb_switch_k4);
    return flag;
}

bool ActivityController::checkLianShengOpen(){
    bool flag = false;
    if (wb_switch_k1==0) {
        return flag;
    }
    flag = checkOpenVersion(wb_switch_k2);
    return flag;
}

bool ActivityController::checkOpenVersion(string version){
    bool flag = false;
    std::vector<std::string> _updateVersion;
    CCCommonUtils::splitString(version,".", _updateVersion);
    //当前app版本
    std::string _appVersion = cocos2d::extension::CCDevice::getVersionName();
    std::vector<std::string> _appXMLVersion;
    CCCommonUtils::splitString(_appVersion,".", _appXMLVersion);
    //如果app的版本大于成功下载xml版本，使用apk内xml
    if (_updateVersion.size() == 3 && _appXMLVersion.size()==3) {
        if(_appXMLVersion[0] > _updateVersion[0]){
            flag = true;
        }else if(_appXMLVersion[0] == _updateVersion[0]){
            if (_appXMLVersion[1]>_updateVersion[1]) {
                flag = true;
            }else if(_appXMLVersion[1]==_updateVersion[1]){
                if(_appXMLVersion[2]>=_updateVersion[2]){
                    flag = true;
                }
            }
        }
    }
    return flag;
}

bool ActivityController::checkUseCKFMassTime(){
    bool flag = true;
    if (wb_switch_k6==0) {
        return flag;
    }
    //flag = checkOpenVersion(wb_switch_k7);
    return flag;
}

bool ActivityController::checkOpenDragonBattle(){
    bool flag = false;

    return flag;
}

void ActExc2Info::update(CCDictionary* dict)
{
    if (dict->objectForKey("type"))
    {
        m_type = dict->valueForKey("type")->intValue();
    }
    if (dict->objectForKey("multiple"))
    {
        m_multiple = dict->valueForKey("multiple")->intValue();
    }
    if (dict->objectForKey("reward"))
    {
        m_reward = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
    }
    if (dict->objectForKey("id"))
    {
        m_ids.clear();
        CCObject* obj = nullptr;
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("id"));
        CCARRAY_FOREACH(arr, obj)
        {
            CCString* cs = dynamic_cast<CCString*>(obj);
            m_ids.push_back(cs->getCString());
        }
    }
    if (dict->objectForKey("allReward"))
    {
        m_rwdIds.clear();
        CCObject* obj = nullptr;
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("allReward"));
        CCARRAY_FOREACH(arr, obj)
        {
            auto tarr = dynamic_cast<CCArray*>(obj);
            auto d = _dict(tarr->objectAtIndex(0));
            if (d->objectForKey("type")) {
                if (7 == d->valueForKey("type")->intValue()) {
                    if (d->objectForKey("value")) {
                        auto valueDic = _dict(d->objectForKey("value"));
                        if (valueDic->objectForKey("id")) {
                            m_rwdIds.push_back(valueDic->valueForKey("id")->getCString());
                        }
                    }
                }
            }
        }
    }
}

ActExc2Info::ActExc2Info(CCDictionary* dict)
{
    m_type = dict->valueForKey("type")->intValue();
    m_multiple = dict->valueForKey("multiple")->intValue();
    m_reward = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
    CCObject* obj = nullptr;
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("id"));
    CCARRAY_FOREACH(arr, obj)
    {
        CCString* cs = dynamic_cast<CCString*>(obj);
        m_ids.push_back(cs->getCString());
    }
    arr = dynamic_cast<CCArray*>(dict->objectForKey("allReward"));
    CCARRAY_FOREACH(arr, obj)
    {
        auto tarr = dynamic_cast<CCArray*>(obj);
        auto d = _dict(tarr->objectAtIndex(0));
        if (d->objectForKey("type")) {
            if (7 == d->valueForKey("type")->intValue()) {
                if (d->objectForKey("value")) {
                    auto valueDic = _dict(d->objectForKey("value"));
                    if (valueDic->objectForKey("id")) {
                        m_rwdIds.push_back(valueDic->valueForKey("id")->getCString());
                    }
                }
            }
        }
    }
}

void ActivityController::addActivity(CCArray *arr){
    CCObject *obj;
    CCARRAY_FOREACH(arr, obj) {
        CCDictionary *dict = dynamic_cast<CCDictionary*>(obj);
        if(dict){
            addActivity(dict);
        }
    }
}

void ActivityController::paraseEventObj(CCArray *dict){
    if(activityArr==nullptr){
        activityArr = CCArray::create();
    }else{
        activityArr->removeAllObjects();
    }
    if(speActivityArr==nullptr){
        speActivityArr = CCArray::create();
    }else{
        speActivityArr->removeAllObjects();
    }
    m_exc2RefInfo.clear();
    m_exc2Data.clear();
    if(dict==NULL) return ;
    int num = dict->count();
    for (int i=0; i<num; i++) {
        auto dicObj = _dict(dict->objectAtIndex(i));
        ActivityEventObj* event = ActivityEventObj::create();
        event->parse(dicObj);
        if (event->type == 1)
        {
            activityArr->addObject(event);
        }
        else if (event->type == 2)
        {
            speActivityArr->addObject(event);
        }else if (event->type == 3)
        {
            activityArr->addObject(event);
        }else if (event->type == 4)
        {
            activityArr->addObject(event);
        }else if (event->type == 6)
        {
            if (event->exchange == "0")
            {
                activityArr->addObject(event);
            }
            if (event->exchange == "2")
            {
                activityArr->addObject(event);
                m_exc2RefInfo[event->id] = Act2RefrshInfo();
            }
        }else if (event->type == 7)
        {
            if (dicObj->objectForKey("firstEnter")) {
                bool firstEnter = dicObj->valueForKey("firstEnter")->boolValue();
                GlobalData::shared()->firstEnterCrossService = firstEnter;
            }
            activityArr->addObject(event);
        }
    }
    
    sortSpeActivityArr();
    sortActivityArr();
    
}

ActivityEventObj* ActivityController::getActObj(string itemId)
{
    CCObject* cobj = nullptr;
    ActivityEventObj* obj = nullptr;
    CCARRAY_FOREACH(activityArr, cobj)
    {
        obj = dynamic_cast<ActivityEventObj*>(cobj);
        if (obj && obj->id == itemId) {
            return obj;
        } else {
            obj = nullptr;
        }
    }
    return obj;
}

void ActivityController::sortSpeActivityArr()
{
    ActivityEventObj* eObj1 = NULL;
    ActivityEventObj* eObj2 = NULL;
    for (int i = 0; i < speActivityArr->count(); i++)
    {
        for (int j = speActivityArr->count()-1; j > i; j--)
        {
            eObj1 = dynamic_cast<ActivityEventObj*>(speActivityArr->objectAtIndex(j));
            eObj2 = dynamic_cast<ActivityEventObj*>(speActivityArr->objectAtIndex(j-1));
            if (eObj1 && eObj2 && eObj1->sort > eObj2->sort)
            {
                speActivityArr->exchangeObjectAtIndex(j, j-1);
            }
        }
    }
}

void ActivityController::sortActivityArr()
{
 /*
  1.开启的排前面
  2.开启中，最早结束的排前面
  3.未开启中，最早开始的排前面
  4.已结束的排最后
  */
    CCArray* arr1 = CCArray::create();
    CCArray* arr2 = CCArray::create();
    CCArray* arr3 = CCArray::create();
    double nowTime = GlobalData::shared()->getWorldTime();
    CCObject* obj = NULL;
    ActivityEventObj* eObj = NULL;
    CCARRAY_FOREACH(activityArr, obj)
    {
        eObj = dynamic_cast<ActivityEventObj*>(obj);
        if (eObj)
        {
            if (eObj->startTime > nowTime)
            {
                //未开始
                arr2->addObject(eObj);
            }
            else if (eObj->endTime < nowTime)
            {
                //已结束
                arr3->addObject(eObj);
            }
            else
            {
                //已开始，未结束
                arr1->addObject(eObj);
            }
        }
    }
    ActivityEventObj* eObj1 = NULL;
    ActivityEventObj* eObj2 = NULL;
    for (int i = 0; i < arr1->count(); i++)
    {
        for (int j = arr1->count()-1; j > i; j--)
        {
            eObj1 = dynamic_cast<ActivityEventObj*>(arr1->objectAtIndex(j));
            eObj2 = dynamic_cast<ActivityEventObj*>(arr1->objectAtIndex(j-1));
            if (eObj1 && eObj2 && (eObj1->endTime < eObj1->endTime))
            {
                arr1->exchangeObjectAtIndex(j, j-1);
            }
        }
    }

    for (int i = 0; i < arr2->count(); i++)
    {
        for (int j = arr2->count()-1; j > i; j--)
        {
            eObj1 = dynamic_cast<ActivityEventObj*>(arr2->objectAtIndex(j));
            eObj2 = dynamic_cast<ActivityEventObj*>(arr2->objectAtIndex(j-1));
            if (eObj1 && eObj2 && (eObj1->startTime < eObj1->startTime))
            {
                arr2->exchangeObjectAtIndex(j, j-1);
            }
        }
    }
    
    activityArr->removeAllObjects();
    activityArr->addObjectsFromArray(arr1);
    activityArr->addObjectsFromArray(arr2);
    activityArr->addObjectsFromArray(arr3);
}

string ActivityController::getActivityInfo()
{
    string info = "";
    double displayTime = 0;
    string displayInfo = "";
    time_t nowTime = GlobalData::shared()->getWorldTime();
    if (activityArr->count() > 0)
    {
        int rand = GlobalData::shared()->getRand(0,activityArr->count());
        ActivityEventObj* eObj = dynamic_cast<ActivityEventObj*>(activityArr->objectAtIndex(rand));
        if (eObj->startTime > nowTime)
        {
            displayTime = eObj->startTime - nowTime;
            displayInfo = "133001";
        }
        else if (eObj->endTime > nowTime)
        {
            displayTime = eObj->endTime - nowTime;
            displayInfo = "133002";
        }
        if (eObj->type==5) {
            info = _lang("133232");
        }else if (displayInfo.length() > 0)
        {
            info = _lang_2(displayInfo.c_str(),eObj->name.c_str(), CC_SECTOA(displayTime));
        }
    }
    
    return info;
}

void ActivityController::addActivity(CCDictionary *dict){
    ActivityInfo *info = ActivityInfo::create();
    info->parse(dict);

    int dtime = info->endTime - GlobalData::shared()->getWorldTime();
    if (dtime < 0) {
        return;
    }

    info->retain();
    
    removeActivity(info->type);
    activity[info->type] = info;
    doWhenTimeOver();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACTIVITY_NUM_CHANGE);
}

void ActivityController::removeActivity(ActivityInfo *info){
    if (info) {
        removeActivity(info->type);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACTIVITY_NUM_CHANGE);
    }
}

void ActivityController::removeActivity(int type){
    map<int, ActivityInfo*>::iterator it = activity.find(type);
    if(it != activity.end()){
        auto info = it->second;
        activity.erase(it);
        CC_SAFE_RELEASE_NULL(info);
    }
}

bool ActivityController::hasActivity(int type){
    map<int, ActivityInfo*>::iterator it = activity.find(type);
    if(it != activity.end()){
        return !isActivityTimeOver(it->second);
    }
    return false;
}


bool ActivityController::isActivityTimeOver(ActivityInfo *info){
    if(info){
        if(info->endTime > GlobalData::shared()->getWorldTime()){
            return false;
        }
    }
    return true;
}

void ActivityController::doWhenTimeOver(float t){
    map<int, ActivityInfo*>::iterator it;
    time_t minTime = 0;
    
    CCArray *deleteArr = CCArray::create();
    for(it = activity.begin(); it != activity.end(); it++){
        if(isActivityTimeOver(it->second)){
            deleteArr->addObject(CCInteger::create(it->second->type));
        }else{
            if(it->second->endTime < minTime){
                minTime = it->second->endTime;
            }
        }
    }
    CCObject *obj;
    CCARRAY_FOREACH(deleteArr, obj) {
        int type = dynamic_cast<CCInteger*>(obj)->getValue();
        removeActivity(activity[type]);
    }
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    time_t dtime = minTime - GlobalData::shared()->getWorldTime();
    if(dtime > 0){
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ActivityController::doWhenTimeOver), this, 1.0, 1, dtime, false);
    }
}

void ActivityController::updateCurrentScore(int type, int score){
    map<int, ActivityInfo*>::iterator it = activity.find(type);
    if(it != activity.end()){
        it->second->currentScore = score;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACTIVITY_SCORE_CHANGE);
    }
}

void ActivityController::updateCurrentEvent(CCDictionary *dict){
    eventInfo.parse(dict);
    //CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_activity_EVENT_CHANGE);
}

void ActivityController::openActivityView(){
    PopupViewController::getInstance()->addPopupInView(ActivityListView::create());
    return;
    double tempTime = GlobalData::shared()->getWorldTime() - eventInfo.readyTime;
    //double readyTime = eventInfo.beginTime - GlobalData::shared()->getWorldTime();
    double endTime = eventInfo.endTime - GlobalData::shared()->getWorldTime();

    if(tempTime<=0){
        if(activityArr && activityArr->count()>0){
            ActivityEventObj* obj = (ActivityEventObj*)activityArr->objectAtIndex(0);
            PopupViewController::getInstance()->addPopupView(ActivityTipView::create(obj));
        }else{
            CCLog("error activity ");
        }
        
    }else if(endTime>0){
        PopupViewController::getInstance()->addPopupView(ActivityBeginView::create());
    }else{
        PopupViewController::getInstance()->addPopupView(ActivityHistoryView::create());
    }
}

void ActivityController::activityRecord(int type){
//    CCArray* p = CCArray::create();
//    p->addObject(CCInteger::create(POINT_ACTIVITY_OPEN));
//    p->addObject(CCInteger::create(type));
//    CCArray* d = CCArray::create();
//    d->addObject(CCInteger::create(FunBuildController::getInstance()->getMainCityLv()));
//    DataRecordCommand *cmd = new DataRecordCommand(OPEN_PANEL, p, d);
//    cmd->sendAndRelease();
}

bool ActivityController::activityIsOpen(){
    
    //return eventInfo.id != "";
    return true;//现在活动在主城的只是入口，一直常在
}

void ActivityController::rewardRecordHandle(){
    string isoLink = CCUserDefault::sharedUserDefault()->getStringForKey("iso_fbAPPLink","");
    string androidLink = CCUserDefault::sharedUserDefault()->getStringForKey("fbAPPLink","");
//    androidLink = 'fb713457855362204:///?al_applink_data={"target_url":"https:\/\/fb.me\/789290781112244?feed_key=755614521189659_feed","extras":[],"referer_app_link":{"url":"fb:\/\/\/?backtrack_id=g96pyKiVmzIk68c9","app_name":"Facebook"}}';
    //https://apps.facebook.com/clash-of-kings/?fb_source=notification&request_ids=784316265008949%2C426879690814991%2C436752513145361%2C1566301930312008%2C1587409761526906&ref=notif&app_request_type=user_to_user&content=send%3A739067812873207&fb_ref=no_ref_specified&notif_t=app_request
    //https://apps.facebook.com/clash-of-kings/?fb_source=notification&request_ids=340417592833858%2C554860414655455&ref=notif&app_request_type=user_to_user&content=askfor%3A426169294209352&notif_t=app_request
    
    CCLog("isoLink =%s",isoLink.c_str());
    if(androidLink!=""){
        isoLink = androidLink;
    }
    if(isoLink!=""){
        if(isoLink.find("request_ids")<isoLink.length()){
            int pos = isoLink.find("request_ids=");
            pos = pos+12;
            string str = isoLink.substr(pos);
            int pos2 = str.length();
            if(str.find("&ref")<str.length()){
                pos2 = str.find("&ref");
            }
            string str2 = str.substr(0,pos2);
            CCLog("request_ids =%s",str2.c_str());
            FeedRewardCommand* cmd = new FeedRewardCommand(str2,1);
            cmd->sendAndRelease();
        }else if(isoLink.find("feed_key")<isoLink.length()){
            int pos = isoLink.find("feed_key=");
            pos = pos+9;
            string str = isoLink.substr(pos);
            int pos2 = str.find("_feed");
            string str2 = str.substr(0,pos2);
            CCLog("feed_key=%s",str2.c_str());
            FeedRewardCommand* cmd = new FeedRewardCommand(str2,0);
            cmd->sendAndRelease();
        }
        if(isoLink.find("askfor")<isoLink.length()){
            //保箱请求
            GlobalData::shared()->m_isFirstAskFor = true;
        }
    }
    if (isoLink!="" && isoLink.find("request_ids")<isoLink.length()) {
        FBUtilies::appEventFBEntrance("request");
    }else if(isoLink!=""){
        FBUtilies::appEventFBEntrance("feed");
    }
    CCLog("androidLink feed_key=%s",androidLink.c_str());
//    if(androidLink!=""){
//        if(androidLink.find("_feed")<androidLink.length()){
//            int pos = androidLink.find("_feed");
//            string key = androidLink.substr(0,pos);
//
//            CCLog("feed_key=%s",key.c_str());
//            FeedRewardCommand* cmd = new FeedRewardCommand(key,0);
//            cmd->sendAndRelease();
//        }else{
//            FeedRewardCommand* cmd = new FeedRewardCommand(androidLink,1);
//            cmd->sendAndRelease();
//        }
//    }
    CCUserDefault::sharedUserDefault()->setStringForKey("iso_fbAPPLink", "");
    CCUserDefault::sharedUserDefault()->setStringForKey("fbAPPLink", "");
    CCUserDefault::sharedUserDefault()->flush();
}

void ActivityController::parseActConfig(CCDictionary* dict)
{
    if (dict->objectForKey("k3"))
    {
        m_maxAtkCnt = dict->valueForKey("k3")->intValue();
    }
    if (dict->objectForKey("k4"))
    {
        vector<string> vec;
        string s = dict->valueForKey("k4")->getCString();
        CCCommonUtils::splitString(s, ";", vec);
        m_atkHarm.clear();
        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            m_atkHarm.push_back(atoi((*it).c_str()));
        }
    }
    if (dict->objectForKey("k5"))
    {
        m_isTrialOpen = dict->valueForKey("k5")->intValue();
    }
    if (dict->objectForKey("k6"))
    {
        m_bigStrengthGold = dict->valueForKey("k6")->intValue();
    }
    if (dict->objectForKey("k7"))
    {
        m_moreAtkGold = dict->valueForKey("k7")->intValue();
    }
    if (dict->objectForKey("k8"))
    {
        m_addedAtkCnt = dict->valueForKey("k8")->intValue();
    }
    if (dict->objectForKey("k9")) {
        popCntLimit = dict->valueForKey("k9")->intValue();
    }
    if (dict->objectForKey("k10"))
    {
        m_newTrialGold = dict->valueForKey("k10")->intValue();
    }
    if (dict->objectForKey("k11"))
    {
        m_bigStrenMul = dict->valueForKey("k11")->intValue();
    }
    if (dict->objectForKey("k12"))
    {
        m_monsterDiv.clear();
        vector<string> vec;
        string s = dict->valueForKey("k12")->getCString();
        CCCommonUtils::splitString(s, ";", vec);
        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            m_monsterDiv.push_back(atoi((*it).c_str()));
        }
    }
}

string ActivityController::getRunningPopEvent()
{
    string ret = "";
    time_t nowTamp = GlobalData::shared()->getTimeStamp();
    time_t nowTime = GlobalData::shared()->changeTime(nowTamp);
    CCObject* obj = nullptr;
    ActivityEventObj* eObj = nullptr;
    CCARRAY_FOREACH(activityArr, obj)
    {
        eObj = dynamic_cast<ActivityEventObj*>(obj);
        if (eObj && eObj->popup != 0) {
            if (eObj->startTime <= nowTime && eObj->endTime >= nowTime) {
                ret += eObj->id;
                ret += ";";
            } else if (eObj->startTime == 0 && eObj->endTime == 0) {
                ret += eObj->id;
                ret += ";";
            }
        }
    }
    return ret;
}

string ActivityController::getRandomPopActId()
{
    string allIds = getRunningPopEvent();
    if (allIds == "")
    {
        return "";
    }
    vector<string> vec;
    CCCommonUtils::splitString(allIds, ";", vec);
    if (vec.size() <= 0)
    {
        return "";
    }
    if (vec.at(vec.size() - 1) == "")
    {
        vec.pop_back();
    }
    if (vec.size() <= 0)
    {
        return "";
    }
    srand (time(NULL));
    int randomIndex = rand() % vec.size();
    randomIndex = rand() % vec.size();
    if (randomIndex >= vec.size() || randomIndex < 0)
    {
        randomIndex = CCMathUtils::getRandomInt(0, vec.size() - 1);
    }
    return vec.at(randomIndex);
}

void ActivityController::loginEventData(){
    rewardRecordHandle();
    if(eventInfo.getRankReward()->count()<=0){
        ActivityEventCommand* cmd = new ActivityEventCommand();
        cmd->sendAndRelease();
    }
}

void ActivityController::enterGameOpenView(int type){
//    bool dailyFlag = false;
//    if(PortActController::getInstance()->m_isRdLoginDay == 0 && GlobalData::shared()->m_isFirstOpenDailyView && GlobalData::shared()->facebook_gift_k7==1 && !GuideController::share()->isInTutorial()){
//        PopupViewController::getInstance()->addPopupInView(PortActView::create(1,type));
//        dailyFlag = true;
//    }
//    if(!dailyFlag && !GuideController::share()->isInTutorial() && GlobalData::shared()->m_isFirstAskFor){
//        PopupViewController::getInstance()->addPopupInView(BaoZangView::create(type));
//        dailyFlag = true;
//    }
//    GlobalData::shared()->m_isFirstOpenDailyView = false;
//    GlobalData::shared()->m_isFirstAskFor = false;
    if(type==1 /*&& !dailyFlag*/){
        VipUtil::showVIPActiveView();
    }
}
void ActivityController::initViewConfig(CCDictionary *ccDict){
    mOpenViewConfig.clear();
    if(ccDict==NULL){
        mOpenViewConfig.push_back("gift_pop;10");
    }else{
        int index = 1;
        string key = string("k")+CC_ITOA(index);
        while (ccDict->objectForKey(key)) {
            string value = ccDict->valueForKey(key)->getCString();
            if(!value.empty()){
                mOpenViewConfig.push_back(value);
            }
            ++index;
            key=string("k")+CC_ITOA(index);
        }
        
        if(mOpenViewConfig.size() == 0){
             mOpenViewConfig.push_back("gift_pop;10");
        }
    }
}
void ActivityController::showEnterView(){
    double timeStamp = GlobalData::shared()->getTimeStamp();
    double tTime = GlobalData::shared()->tomorrow_time;
    if(tTime<timeStamp){
        GlobalData::shared()->tomorrow_time += 86400;
        tTime = GlobalData::shared()->tomorrow_time;
    }
    int idx = 0;
    std::string oldTTime = CCUserDefault::sharedUserDefault()->getStringForKey(RECHARGE_ACTV_TOMORROW_TIME);
    if(oldTTime.empty()){
        CCUserDefault::sharedUserDefault()->setStringForKey(RECHARGE_ACTV_TOMORROW_TIME, CC_ITOA(tTime));
        CCUserDefault::sharedUserDefault()->flush();
    }else{
        std::string newStr = CC_ITOA(tTime);
        if(newStr.compare(oldTTime) != 0){
            CCUserDefault::sharedUserDefault()->setStringForKey(RECHARGE_ACTV_TOMORROW_TIME, CC_ITOA(tTime));
            CCUserDefault::sharedUserDefault()->flush();
        }else{
            idx =  CCUserDefault::sharedUserDefault()->getIntegerForKey(RECHARGE_ACTV_POPUP_VIEW_IDX,0);
        }
    }
    //new rules
    bool bDaily = false;
    if(PortActController::getInstance()->m_isRdLoginDay == 0 && GlobalData::shared()->m_isFirstOpenDailyView && GlobalData::shared()->facebook_gift_k7==1 && !GuideController::share()->isInTutorial()){
        bDaily=true;
        if(CCCommonUtils::isTestPlatformAndServer("guide_3") && PortActController::getInstance()->m_newLoginDay==0 && PortActController::getInstance()->m_newPalyerRDMap.size() > 0){//新手7日奖励未激活不能弹出普通的7日签到奖励
            bDaily=false;
        }
    }
    if(PortActController::getInstance()->m_isRdLoginDay == 0)
    {
        bDaily = true;//fusheng  如果今天的7天没有领取  弹出领取界面
        idx = 0;//fusheng
    }

    bool bGuideDaily = false;
    if(PortActController::getInstance()->canPopNewRDView()){
        bGuideDaily=true;
    }
    GoldExchangeItem* giftItem = NULL;
    GoldExchangeItem* giftItemNormal = NULL;
    GoldExchangeItem* monthCard = NULL;
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        if(!it->second)
            continue;
        if(it->second->type=="5"){
            if(it->second && GlobalData::shared()->monthCardInfoList.find(it->second->id)!=GlobalData::shared()->monthCardInfoList.end()){
                MonthCardInfo *card =GlobalData::shared()->monthCardInfoList[it->second->id];
                if(card && card->canReward()==false){
                    monthCard = it->second;
                }
            }
        }else{
            if(it->second->type == "3"){
                if(it->second->bought||it->second->popup_image=="close"){
                    continue;
                }
                if(it->second->end>GlobalData::shared()->getWorldTime()){
                    if(giftItem==NULL ||giftItem->popup<it->second->popup){
                        giftItem = it->second;
                    }
                }
            }else if(it->second->type == "1"){
                if(it->second->bought|| it->second->popup_image=="hide" || it->second->popup_image.empty()){
                    continue;
                }
                if(it->second->end>GlobalData::shared()->getWorldTime()){
                    if(LuaController::getInstance()->showNoramlIcon(it->second->popup_image)){
                        if(giftItemNormal==NULL ||giftItemNormal->popup<it->second->popup){
                            giftItemNormal = it->second;
                        }
                    }
                }
            }
        }
    }
    bool bEvent = false;
    string runningEvent = getRunningPopEvent();
    if (runningEvent != "" && GlobalData::shared()->popupTimes <= popCntLimit) {
        bEvent = true;
    }
    idx = idx>(mOpenViewConfig.size()-1)?(mOpenViewConfig.size()-1):idx;
    string tmpString = mOpenViewConfig.at(idx);
    vector<string> vTmp;
    vector<vector<string>> vTmp0;
    int total=0;
    int rndValue = 0;
    CCCommonUtils::splitString(tmpString, "|", vTmp);
    auto itr = vTmp.begin();
    while (itr!= vTmp.end()) {
        if((bDaily==false && bGuideDaily==false) && (*itr).find("sign_pop") != string::npos){
            ++itr;
            continue;
        }else if(monthCard==NULL && (*itr).find("month_pop") != string::npos){
            ++itr;
            continue;
        }else if( (giftItem==NULL && giftItemNormal == NULL) && (*itr).find("gift_pop") != string::npos){
            ++itr;
            continue;
        }else if((*itr).find("event_pop") != string::npos && bEvent == false) {
            ++itr;
            continue;
        }
        vector<string> vTmp1;
        vector<string> vTmp2;
        CCCommonUtils::splitString((*itr), ";", vTmp1);
        if(vTmp1.size()>1){
            int rate = atoi(vTmp1.at(1).c_str());
            int rates = total;
            total+=rate;
            vTmp2.push_back(vTmp1.at(0));
            vTmp2.push_back(CC_ITOA(rates));
            vTmp2.push_back(CC_ITOA(total));
            vTmp0.push_back(vTmp2);
        }
        ++itr;
    }
    if(total>0){
        srand (time(NULL));
        int randomIndex = rand() % total;
        if( randomIndex >=0 && randomIndex <= total ){
            rndValue = randomIndex;
        }else{
            rndValue=CCMathUtils::getRandomInt(0, total);
        }
        
        auto iter = vTmp0.begin();
        while (iter!=vTmp0.end()) {
            vector<string> vTmp2 = (*iter);
            if(vTmp2.size()>2){
                int rates = atoi(vTmp2.at(1).c_str());
                int ratee = atoi(vTmp2.at(2).c_str());
                if(rndValue >= rates && rndValue < ratee){
                    string showPop = vTmp2.at(0);
                    if(showPop.compare("sign_pop") == 0 && bGuideDaily==true){
                        showEnterViewByType("sign_pop_guide");
                    }else{
                        showEnterViewByType(showPop);
                    }
                    
                    ++idx;
                    CCUserDefault::sharedUserDefault()->setIntegerForKey(RECHARGE_ACTV_POPUP_VIEW_IDX, idx);
                    CCUserDefault::sharedUserDefault()->flush();
                    return;
                }
            }
            ++iter;
        }
    }
    
    if(giftItem!=NULL){
        showEnterViewByType("gift_pop");
        ++idx;
        CCUserDefault::sharedUserDefault()->setIntegerForKey(RECHARGE_ACTV_POPUP_VIEW_IDX, idx);
        CCUserDefault::sharedUserDefault()->flush();
    }
}
void ActivityController::showEnterViewByType(string popup){
    //fusheng 修改为不弹出event_pop 和 month_pop
    
    if(popup.compare("sign_pop") != 0 && popup.compare("gift_pop") )
    {
        return;
    }

    if(popup.compare("sign_pop_guide") == 0){
        PopupViewController::getInstance()->addPopupView(RechargeACTVCell::createDailyGuideRwd(14));
    }else if(popup.compare("sign_pop") == 0){
        if (PortActController::getInstance()->m_loginDayMap.size() <= 0) {
            PortActController::getInstance()->startGetDailyRwdData();
            CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityController::showDailyRwd), PORT_DATA_INIT, NULL);
        }else{
            showDailyRwd(NULL);
        }
    }else if(popup.compare("month_pop") == 0){
        PopupViewController::getInstance()->addPopupView(RechargeACTVCell::createMonthCard(14));
    }else if(popup.compare("gift_pop") ==0){
//        if (!CCCommonUtils::isIosAndroidPad())
//        {
            PopupViewController::getInstance()->addPopupView(RechargeACTVCell::create(14));
//        }
        
    }else if(popup.compare("event_pop") == 0){
        showRandomActPop();
    }
}
void ActivityController::showRandomActPop()
{
    string actId = getRandomPopActId();
    PopupViewController::getInstance()->addPopupView(ActivityPopupView::create(actId));
}
void ActivityController::showDailyRwd(CCObject* ccobj){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PORT_DATA_INIT);
    PopupViewController::getInstance()->addPopupView(RechargeACTVCell::createDailyRwd(14));
    GlobalData::shared()->m_isFirstOpenDailyView = false;
    GlobalData::shared()->m_isFirstAskFor = false;
}

void ActivityController::facebookPostMethod(string key,CCArray* param){
    if (FBUtilies::fbIsLogin()) {
        double lastTime = 0.0;
        string timeKey = key;
        timeKey.append("_ShareDataTime");
        if(key=="wishing_five" || key=="wishing_ten"){
            timeKey = "facebookWishingTimeRecord";
        }
        string strTime = CCUserDefault::sharedUserDefault()->getStringForKey(timeKey.c_str(), "");
        if (strTime!="") {
            lastTime = atof(strTime.c_str());
        }
        std:: map<string, CCDictionary*>::iterator it = GlobalData::shared()->shareFbmap.find(key.c_str());
        auto ctime = GlobalData::shared()->getTimeStamp();
        if (it!=GlobalData::shared()->shareFbmap.end()) {
            CCDictionary* dict = it->second;
            if (dict) {
                int on = dict->valueForKey("ON")->intValue();
                double limit = dict->valueForKey("Limit")->doubleValue();
                string permission = dict->valueForKey("permissions")->getCString();
                string method = dict->valueForKey("method")->getCString();
                string object_type = dict->valueForKey("object_type")->getCString();
                string objectStr = dict->valueForKey("object")->getCString();
                string actions_name = dict->valueForKey("actions_name")->getCString();
                string ref = dict->valueForKey("ref")->getCString();
                if (lastTime > 0) {
                    lastTime = lastTime + limit;
                }
                bool flag = true;
                if (permission!="") {
                    flag = FBUtilies::fbHasGranted(permission);
                }
                if (on==1 && ctime > lastTime && flag) {
                    CCUserDefault::sharedUserDefault()->setStringForKey(timeKey.c_str(), CC_ITOA(ctime));
                    CCUserDefault::sharedUserDefault()->flush();
                    if (object_type=="" || objectStr=="") {
                        return ;
                    }
                    if (param!=NULL && param->count()>0) {
                        int pNum = param->count();
                        for (int k=0; k<pNum; k++) {
                            char strsrc[50]={0};
                            sprintf(strsrc,"{%d}",k);
                            CCString* tempP = dynamic_cast<CCString*>(param->objectAtIndex(k));
                            if (tempP!=NULL) {
                                objectStr = CCCommonUtils::StringReplace(objectStr, strsrc, tempP->getCString());
                            }
                        }
                    }
                    CCLog("fb achieveemnt method=%s  object_type=%s objectStr=%s",method.c_str(),object_type.c_str(),objectStr.c_str());
                    FBUtilies::fbPostMethod(method, object_type, objectStr,"","",ref);
                }
            }
        }
    }
}

void ActivityController::openCrossServerActivity(){
    if (activityArr && activityArr->count()>0) {
        int num = activityArr->count();
        for (int i=0; i<num; i++) {
            auto temp = dynamic_cast<ActivityEventObj*>(activityArr->objectAtIndex(i));
            double nowTime = GlobalData::shared()->getWorldTime();
            if (temp && temp->type==7) {
                auto view = dynamic_cast<CrossServiceActivityView*>(PopupViewController::getInstance()->getCurrentPopupView());
                if (view==NULL) {
                    PopupViewController::getInstance()->addPopupView(CrossServiceActivityView::create(temp));
                }
                break;
            }
        }
    }
}

void ActivityController::leaveCKFServer(CCDictionary *params){
    if (!params) {
        return ;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REMOVE_ALL_HINT);
    if(SceneController::getInstance()->showBG){
        CC_SAFE_RELEASE_NULL(SceneController::getInstance()->showBG);
        SceneController::getInstance()->showBG = NULL;
    }
    if (WorldMapView::instance()) {
        WorldMapView::instance()->removeCover();
    }
    auto scene = CCDirector::sharedDirector()->getRunningScene();
    auto texture = CCRenderTexture::create(CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height);
    texture->beginWithClear(0, 0, 0, 1.0);
    scene->visit();
    texture->end();
    SceneController::getInstance()->showBG = texture;
    SceneController::getInstance()->showBG->retain();
    
    auto serverInfo = _dict(params->objectForKey("serverInfo"));
    std::string ip = serverInfo->valueForKey("ip")->getCString();
    std::string zone = serverInfo->valueForKey("zone")->getCString();
    int port = serverInfo->valueForKey("port")->intValue();
    std::string gameUid = params->valueForKey("uid")->getCString();
    
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, port);
    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, gameUid);
    CCUserDefault::sharedUserDefault()->flush();
    
    NetController::shared()->setIp(ip);
    NetController::shared()->setZone(zone);
    NetController::shared()->setPort(port);
    NetController::shared()->setIsLogout(true);
    NetController::shared()->disconnect();
    LogoutCommand* cmd = new LogoutCommand();
    CCDictionary* _params = CCDictionary::create();
    _params->setObject(CCString::create(zone), "zoneName");
    CCDictionary* dict = CCDictionary::create();
    dict->setObject(CCString::create("logout"), "cmd");
    dict->setObject(_params, "params");
    cmd->handleRecieve(dict);
}
void ActivityController::parseTrialInfo(CCDictionary* dict)
{
    if (dict->objectForKey("freeAtk")) {
        m_freeAtk = dict->valueForKey("freeAtk")->intValue();
    }
    if (dict->objectForKey("screen")) {
        m_screen.clear();
        vector<string> vec;
        string screen = dict->valueForKey("screen")->getCString();
        CCCommonUtils::splitString(screen, "|", vec);
        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            m_screen.push_back(atoi((*it).c_str()));
        }
    }
    if (dict->objectForKey("blood"))
    {
        m_blood = dict->valueForKey("blood")->intValue();
    }
    if (dict->objectForKey("monster")) {
        m_monster = dict->valueForKey("monster")->getCString();
    }
    if (dict->objectForKey("maxBlood")) {
        m_maxBlood = dict->valueForKey("maxBlood")->intValue();
    }
    if (dict->objectForKey("freeTrial")) {
        m_freeTrial = dict->valueForKey("freeTrial")->intValue();
    }
    m_trialRefTime = GlobalData::shared()->getTimeStamp();
}

void ActivityController::endAtkCmd(cocos2d::CCDictionary * params)
{
    m_isInAtkCmdSending = false;
    if (params->objectForKey("errorCode"))
    {
        CCCommonUtils::flyText(_lang(params->valueForKey("errorCode")->getCString()));
    } else {
        int db = 0;
        if (params->objectForKey("trialField")) {
            auto dic = _dict(params->objectForKey("trialField"));
            int begin = m_blood;
            string bmonster = m_monster;
            parseTrialInfo(dic);
            string emonster = m_monster;
            int end = m_blood;
            if (bmonster != emonster) {
                end = 0;
            }
            if (begin > end) {
                db = begin - end;
            }
        }
        if (params->objectForKey("gold")) {
            GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->longValue();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GET_ATK_DATA, CCInteger::create(db));
    }
}

void ActivityController::endStartTrialCmd(cocos2d::CCDictionary * params)
{
    if (params->objectForKey("errorCode"))
    {
        CCCommonUtils::flyText(_lang(params->valueForKey("errorCode")->getCString()));
    } else {
        if (params->objectForKey("trialField")) {
            auto dic = _dict(params->objectForKey("trialField"));
            parseTrialInfo(dic);
        }
        if (params->objectForKey("type"))
        {
            int t = params->valueForKey("type")->intValue();
            if (t == 1) {
                auto& info = ToolController::getInstance()->getToolInfoById(ITEM_TRIAL_TICKET);
                int tCnt = info.getCNT() - 1;
                info.setCNT(tCnt);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TRIAL_START_SUC);
    }
}

void ActivityController::endTrialFinishCmd(cocos2d::CCDictionary * params)
{
    if (params->objectForKey("errorCode"))
    {
        CCCommonUtils::flyText(_lang(params->valueForKey("errorCode")->getCString()));
    } else {
        if (params->objectForKey("trialField")) {
            auto dic = _dict(params->objectForKey("trialField"));
            parseTrialInfo(dic);
        }
        if (params->objectForKey("reward")) {
            auto arr = dynamic_cast<CCArray*>(params->objectForKey("reward"));
//            if(arr){
//                string ardInfo = RewardController::getInstance()->retReward(arr);
//                CCCommonUtils::flyHint("", "", ardInfo);
//            }
            GCMRewardController::getInstance()->retReward2(arr, true);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TRIAL_FINISH_SUC);
    }
}

void ActivityController::endTrialBuyCmd(CCDictionary* params)
{
    if (params->objectForKey("errorCode"))
    {
        CCCommonUtils::flyText(_lang(params->valueForKey("errorCode")->getCString()));
    } else {
        if (params->objectForKey("gold")) {
            GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->longValue();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        }
        if (params->objectForKey("times")) {
            m_freeAtk = params->valueForKey("times")->intValue();
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TRIAL_BUY_SUC);
    }
}

void ActivityController::showTrainingView()
{
}

int ActivityController::getMyTrialState()
{
    //得到当前的活动进行程度  1 进行全新一轮的试炼 2 继续进行上一轮的攻击 3 攻击完要领奖但没有打完所有的怪物 4 打完所有的怪物要领奖 5 攻击完无法领奖
    int ret = 1;
    if (m_monster == "") {
        ret = 1;
    } else {
        if (m_blood > 0 && m_freeAtk > 0) {
            ret = 2;
        } else {
            if (m_blood == 0) {
                ret = 4;
            } else {
                int cnt = getKilledMonsterCnt();
                if (cnt == 0)
                {
                    ret = 5;
                } else {
                    ret = 3;
                }
            }
        }
    }
    return ret;
}

int ActivityController::getKilledMonsterCnt()
{
    vector<int> allids;
    CCDictElement* ele = nullptr;
    CCDictionary* group = LocalController::shared()->DBXMLManager()->getGroupByKey("trial");
    CCDICT_FOREACH(group, ele)
    {
        CCDictionary *dict = dynamic_cast<CCDictionary*>(ele->getObject());
        int id = dict->valueForKey("id")->intValue();
        allids.push_back(id);
    }
    int cnt = 0;
    int myId = atoi(m_monster.c_str());
    for (auto it = allids.begin(); it != allids.end(); ++it) {
        if (*it < myId) {
            cnt++;
        }
    }
    if (ActivityController::getInstance()->m_blood == 0) cnt++;
    return cnt;
}

int ActivityController::getAllMonsterCnt()
{
    CCDictionary* group = LocalController::shared()->DBXMLManager()->getGroupByKey("trial");
    return group->count();
}

//ChristmasActController
static ChristmasActController *_chrinstance = NULL;

ChristmasActController::ChristmasActController()
{
    m_loginDay = 0;
    m_exchangeTime = 0;
    m_level = 0;
    m_socksMsg = "";
    m_isRdLoginDay = 0;
    m_Today = 0;
    m_isNewDay = false;
    
    m_refreshTime = 0;//恢复一个雪球的时间戳
    m_maxThrowCount = 0;//一天最多扔多少雪球
    m_todayThrowCount = 0;//当天已经扔过多少雪球
    m_curLevel = 0;//当前雪球等级
    m_goldCostRecover = 0;//恢复全部雪球需要的金币数量。
    for (int i = 0; i < 5;  ++i) {
        m_expArr[i] = 0;
    }
    m_goldThrowMultiply = 0;//用金币扔雪球获得的经验倍数
    m_goldCostThrow = 0;//花费金币扔雪球消耗的金币数
    m_resourceType = 0;//扔雪球所耗费的资源种类
    m_resourceNum = 0;//扔雪球所耗费的资源的数量
    m_remainGold = 0;
    m_exp = 0;//玩家当前经验值
    m_isInit = 0;
    m_free = 0;
}

ChristmasActController::~ChristmasActController() {
}

void ChristmasActController::purgeData() {
    delete _chrinstance;
    _chrinstance = NULL;
}

ChristmasActController *ChristmasActController::getInstance(){
    if(_chrinstance == NULL){
        _chrinstance = new ChristmasActController();
    }
    return _chrinstance;
}

void ChristmasActController::startGetExchangeData(int type)
{
//    ChristmasExchangeInitCommand* cmd = new ChristmasExchangeInitCommand(type);
//    cmd->sendAndRelease();
}

void ChristmasActController::endGetExchangeData(CCDictionary *dict, int type)
{
    if (dict->objectForKey("sock")) {//袜子记录
        auto sock = _dict(dict->objectForKey("sock"));
        
        m_socExMap.clear();
        int tmpTime = sock->valueForKey("refreshTime")->doubleValue()/1000;
        m_exchangeTime = GlobalData::shared()->changeTime(tmpTime);
        m_level = sock->valueForKey("level")->intValue();
        
        auto arr = dynamic_cast<CCArray*>(sock->objectForKey("rewards"));
        if(arr){
            CCDictionary *item = NULL;
            for (int i=0; i<arr->count(); i++) {
                item = _dict(arr->objectAtIndex(i));
                auto info = SocksExInfo(item);
                m_socExMap[info.eId] = info;
            }
        }
    }
    
    m_socksMsg = "";
    if (dict->objectForKey("history")) {//袜子记录
        m_socksMsg = dict->valueForKey("history")->getCString();
    }
    
    if (type == 1) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHRISTMAS_SCOKS_EXCHANGE);
    }
}

void ChristmasActController::startExchange(int eid)
{
//    ChristmasExchangeCommand* cmd = new ChristmasExchangeCommand(eid);
//    cmd->sendAndRelease();
}

void ChristmasActController::endExchange(CCDictionary *dict)
{
    if (dict->objectForKey("errorCode")) {
        string code = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(code));
    }
    else {//袜子记录
        m_level = dict->valueForKey("level")->intValue();
        if (dict->objectForKey("sock")) {
            endGetExchangeData(dict);
        }
        
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        if(arr){
            string ardInfo = RewardController::getInstance()->retReward(arr);
            CCCommonUtils::flyHint("", "", ardInfo);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHRISTMAS_SCOKS_EXCHANGE);
    }
}

void ChristmasActController::pushSocksMsg(CCDictionary *dict)
{
    if (dict->objectForKey("msg")) {
        string tmpMsg = dict->valueForKey("msg")->getCString();
        m_socksMsg = m_socksMsg+"|"+tmpMsg;
    }
    if(dict->objectForKey("rewards")){
        CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("rewards"));
        if(arr){
            GCMRewardController::getInstance()->retReward2(arr, false);
            CCObject *obj;
            CCARRAY_FOREACH(arr, obj){
                auto dict = dynamic_cast<CCDictionary*>(obj);
                int type = dict->valueForKey("type")->intValue();
                switch (type) {
                    case R_GOODS:{
                        CCDictionary *goodsDict = _dict(dict->objectForKey("value"));
                        int itemId = goodsDict->valueForKey("itemId")->intValue();
//                        int rewardAdd = goodsDict->valueForKey("rewardAdd")->intValue();
                        CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(itemId)), "", _lang_1("104913", CCCommonUtils::getNameById(CC_ITOA(itemId)).c_str()));
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }
}

void ChristmasActController::startGetLogin14Day()
{
    ChristmasActController::getInstance()->m_loginDayMap.clear();
//    ChristmasLogin14Command* cmd = new ChristmasLogin14Command();
//    cmd->sendAndRelease();
}
void ChristmasActController::endGetLogin14Day(CCDictionary *dict)
{
    CCLOG("test");
    if (dict) {
        m_loginDay = dict->valueForKey("currentDay")->intValue();
        m_isRdLoginDay = dict->valueForKey("isValid")->intValue();
        m_isNewDay = false;
        
        CCArray* loginReward = dynamic_cast<CCArray*>(dict->objectForKey("loginReward"));
        CCDictionary *item = NULL;
        for (int i=0; i<loginReward->count(); i++) {
            item = _dict(loginReward->objectAtIndex(i));
            int keyDay = item->valueForKey("day")->intValue();
            CCArray* reward = dynamic_cast<CCArray*>(item->objectForKey("reward"));
            
            m_loginDayMap[keyDay] = reward;
        }

    }
}

void ChristmasActController::startGetLogin14RD()
{
//    ChristmasLogin14CheckInCommand* cmd = new ChristmasLogin14CheckInCommand();
//    cmd->sendAndRelease();
}
void ChristmasActController::endGetLogin14RD(CCDictionary *dict)
{
    CCLOG("get login day rd");
    if (dict->objectForKey("errorCode")) {
        string code = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(code));
    }
    else {//获取奖励
        m_loginDay = dict->valueForKey("day")->intValue();
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        if(arr){
            string ardInfo = RewardController::getInstance()->retReward(arr);
            CCCommonUtils::flyHint("", "", ardInfo);
        }
        
        m_loginDay = MIN(m_loginDay, 14);
        m_isRdLoginDay = 1;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHRISTMAS_LOGINRD_END, CCInteger::create(m_loginDay));
    }
}

void ChristmasActController::startGetSnowActData(int isInit)
{
    m_isInit = isInit;
//    SnowBallActivityGetCommand* cmd = new SnowBallActivityGetCommand();
//    cmd->sendAndRelease();
}

void ChristmasActController::endGetSnowActData(CCDictionary* param)
{
//    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dict = param;
    if (dict->objectForKey("goldCostRecover")) {
        m_goldCostRecover = dict->valueForKey("goldCostRecover")->intValue();
    }
    if (dict->objectForKey("maxThrowCount")) {
        m_maxThrowCount = dict->valueForKey("maxThrowCount")->intValue();
    }
    if (dict->objectForKey("snowBall")) {
        auto dict1 = dynamic_cast<CCDictionary*>(dict->objectForKey("snowBall"));
        if (dict1->objectForKey("todayThrowCount")) {
            m_todayThrowCount = dict1->valueForKey("todayThrowCount")->intValue();
        }
        if (dict1->objectForKey("curLevel")) {
            m_curLevel = dict1->valueForKey("curLevel")->intValue();
        }
    }
    if (dict->objectForKey("refreshTime")) {
        m_refreshTime = dict->valueForKey("refreshTime")->doubleValue();
    }
    if (dict->objectForKey("goldThrowMultiply")) {
        m_goldThrowMultiply = dict->valueForKey("goldThrowMultiply")->intValue();
    }
    if (dict->objectForKey("goldCostThrow")) {
        m_goldCostThrow = dict->valueForKey("goldCostThrow")->intValue();
    }
    if (dict->objectForKey("resourceType")) {
        m_resourceType = dict->valueForKey("resourceType")->intValue();
    }
    if (dict->objectForKey("resourceNum")) {
        m_resourceNum = dict->valueForKey("resourceNum")->intValue();
    }
    if (dict->objectForKey("free")) {
        m_free = dict->valueForKey("free")->intValue();
    }
    if (dict->objectForKey("resource")) {
        auto resInfo = _dict(dict->objectForKey("resource"));
        GlobalData::shared()->resourceInfo.setResourceData(resInfo);
    }
    if (dict->objectForKey("remainGold")) {
        m_remainGold = dict->valueForKey("remainGold")->doubleValue();
        GlobalData::shared()->playerInfo.gold = dict->valueForKey("remainGold")->doubleValue();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    if (dict->objectForKey("m_exp")) {
        m_exp = dict->valueForKey("m_exp")->intValue();
    }
    if (dict->objectForKey("expArr")) {
        CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("expArr"));
        for (int i = 0; i < arr->count(); ++i) {
            m_expArr[i] = dynamic_cast<CCString*>(arr->objectAtIndex(4 - i))->intValue();
        }
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHRISTMAS_SNOW_END);
    if (m_isInit==1) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHRISTMAS_DATA_INIT, CCInteger::create(2));
    }
    m_isInit = 0;
}

void ChristmasActController::startGetActTime()
{
//    ChristmasActTimeCommand* cmd = new ChristmasActTimeCommand();
//    cmd->sendAndRelease();
}

void ChristmasActController::endGetActTime(CCDictionary* param)
{
    auto dict = param;
    int tmpTime = 0;
    if (dict->objectForKey("entireStartPreviewTime")) {
        tmpTime = dict->valueForKey("entireStartPreviewTime")->doubleValue()/1000;
        m_actTime["entireStartPreviewTime"] = GlobalData::shared()->changeTime(tmpTime);
    }
    if (dict->objectForKey("snowBallStartPreviewTime")) {
        tmpTime = dict->valueForKey("snowBallStartPreviewTime")->doubleValue()/1000;
        m_actTime["snowBallStartPreviewTime"] = GlobalData::shared()->changeTime(tmpTime);
    }
    if (dict->objectForKey("snowBallStartTime")) {
        tmpTime = dict->valueForKey("snowBallStartTime")->doubleValue()/1000;
        m_actTime["snowBallStartTime"] = GlobalData::shared()->changeTime(tmpTime);
    }
    if (dict->objectForKey("snowBallEndTime")) {
        tmpTime = dict->valueForKey("snowBallEndTime")->doubleValue()/1000;
        m_actTime["snowBallEndTime"] = GlobalData::shared()->changeTime(tmpTime);
    }
    if (dict->objectForKey("checkInStartTime")) {
        tmpTime = dict->valueForKey("checkInStartTime")->doubleValue()/1000;
        m_actTime["checkInStartTime"] = GlobalData::shared()->changeTime(tmpTime);
    }
    if (dict->objectForKey("checkInEndTime")) {
        tmpTime = dict->valueForKey("checkInEndTime")->doubleValue()/1000;
        m_actTime["checkInEndTime"] = GlobalData::shared()->changeTime(tmpTime);
    }
    if (dict->objectForKey("sockStartTime")) {
        tmpTime = dict->valueForKey("sockStartTime")->doubleValue()/1000;
        m_actTime["sockStartTime"] = GlobalData::shared()->changeTime(tmpTime);
    }
    if (dict->objectForKey("sockEndTime")) {
        tmpTime = dict->valueForKey("sockEndTime")->doubleValue()/1000;
        m_actTime["sockEndTime"] = GlobalData::shared()->changeTime(tmpTime);
    }
    if (dict->objectForKey("entireEndPreviewTime")) {
        tmpTime = dict->valueForKey("entireEndPreviewTime")->doubleValue()/1000;
        m_actTime["entireEndPreviewTime"] = GlobalData::shared()->changeTime(tmpTime);
    }
    CCLOG("test");
}

int ChristmasActController::checkActState()
{
    int state = 0;
    int nowTime = GlobalData::shared()->getWorldTime();
    if (nowTime < m_actTime["entireStartPreviewTime"]) {
        state = 1;
    } else if(nowTime < m_actTime["checkInStartTime"]) {
        state = 2;
    } else if(nowTime < m_actTime["snowBallStartPreviewTime"]) {
        state = 3;
    } else if (nowTime < m_actTime["snowBallStartTime"]) {
        state = 4;
    } else if (nowTime < m_actTime["entireEndPreviewTime"]) {
        state = 5;
    } else if (nowTime < m_actTime["checkInEndTime"]) {
        state = 6;
    } else {
        state = 7;
    }
    return state;
}

int ChristmasActController::getStartTime()
{
    return m_actTime["checkInStartTime"];
}

int ChristmasActController::getSnowTime()
{
    return m_actTime["snowBallStartTime"];
}

int ChristmasActController::getEndTime()
{
    return m_actTime["checkInEndTime"];
}

//SocksExInfo
SocksExInfo::SocksExInfo(CCDictionary* dict)
{
    this->eId = dict->valueForKey("level")->intValue();
    this->needNum = dict->valueForKey("needNum")->intValue();
    this->rewardId = dict->valueForKey("rewardId")->getCString();
    this->rewardArr = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
}

//TimeExInfo
TimeExInfo::TimeExInfo(CCArray* arr, int i)
{
    rewardArr = CCArray::create();
    this->eId = i;
    for (int i = 0; i < arr->count(); ++i) {
        this->rewardArr->addObject(arr->objectAtIndex(i));
    }
//    this->rewardArr = dynamic_cast<CCArray*>(dict->objectForKey(key));
}

//DailyRwdInfo
DailyRwdInfo::DailyRwdInfo(CCDictionary* dict)
{
    reward = CCArray::create();
    vipReward = CCArray::create();
    if (dict->objectForKey("day")) {
        this->day = dict->valueForKey("day")->intValue();
    }
    if (dict->objectForKey("showDay")) {
        this->showDay = dict->valueForKey("showDay")->intValue();
    }
    if (dict->objectForKey("multiple")) {
        this->multiple = dict->valueForKey("multiple")->intValue();
    }
    if (dict->objectForKey("vipMultiple")) {
        this->vipMultiple = dict->valueForKey("vipMultiple")->intValue();
    }
    if (dict->objectForKey("reward")) {
        auto arr1 = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        for (int i = 0; i < arr1->count(); ++i) {
            this->reward->addObject(arr1->objectAtIndex(i));
        }
    }
    if (dict->objectForKey("vipReward")) {
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("vipReward"));
        for (int i = 0; i < arr->count(); ++i) {
            this->vipReward->addObject(arr->objectAtIndex(i));
        }
    }
    if (this->vipReward==NULL) {
        this->vipReward = CCArray::create();
    }
    if (this->reward==NULL) {
        this->reward = CCArray::create();
    }
}

//PortActController
static PortActController* _portinstance = NULL;

PortActController::PortActController()
{
    //在线时长兑换数据
    //上一次领奖的级别的时长最大值
    m_lastRwdTime = 0;
    //更新时大本等级
    m_castleLevel = 0;
    //当前在线时长
    m_timeOnline = 0;
    //前台开始计时时间
    m_startTime = 0;
    //end
    
    //每日签到数据
    //登陆天数
    m_loginDay = 0;
    m_isRdLoginDay = 0;
    m_Today = 0;
    m_isNewDay = false;
    m_inCnt = 0;
    
    //新在线时长数据
    //下次可领奖时间
    m_nextRewardTime = 0;
    //奖励信息
    m_rewardInfo = "";
    //开关
    m_isNewTimeRwd = false;
    
    m_newLoginDay = 0;
    m_isGetNewRD = 0;
}

PortActController::~PortActController() {
}

void PortActController::purgeData() {
    delete _portinstance;
    _portinstance = NULL;
}

PortActController *PortActController::getInstance(){
    if(_portinstance == NULL){
        _portinstance = new PortActController();
    }
    return _portinstance;
}

int PortActController::getMaxRwdLv()
{
    long curTime = GlobalData::shared()->getTimeStamp();
    long startTime = m_startTime;
    long preOnlineTime = m_timeOnline;
    long onlineTime = preOnlineTime + curTime - startTime;
    return getLvByTime(onlineTime);
}

void PortActController::parseTimeDiv(CCDictionary* dict)
{
    m_levelTime.clear();
    string k = "k";
    for (int i = 1; i < 100; ++i) {
        string key = k + CC_ITOA(i);
        if (dict->objectForKey(key)) {
            m_levelTime.push_back(dict->valueForKey(key)->intValue());
        } else {
            break;
        }
    }
}

//获得时间time可以领奖的等级
int PortActController::getLvByTime(long time)
{
    vector<int>& vec = PortActController::getInstance()->m_levelTime;
    int i = 0;
    for (; i < vec.size(); ++i) {
        if (time >= vec[i] * 60) {
            continue;
        } else {
            break;
        }
    }
    return i;
}

//判断是否是新的一天
bool PortActController::isNewDay()
{
    bool st = false;
    
    if (GlobalData::shared()->getTimeStamp() - m_startTime > 24 * 60 * 60) {
        st = true;
    }
    
    long time1 = m_startTime;
    long time2 = GlobalData::shared()->getTimeStamp();
    struct tm tm1;
    tm1 = *localtime(&time1);
    struct tm tm2;
    tm2 = *localtime(&time2);
    if (tm1.tm_mday != tm2.tm_mday) {
        st = true;
    }
    
    return st;
}

//初始化兑换数据
void PortActController::startGetExchangeData(int type)
{
    TimeExchangeInitCommand* cmd = new TimeExchangeInitCommand(type);
    cmd->sendAndRelease();
}
void PortActController::endGetExchangeData(CCDictionary *dict, int type)
{
    m_timeExMap.clear();
    if (dict->objectForKey("onlineReward")) {
        auto onlineInfo = _dict(dict->objectForKey("onlineReward"));
        m_lastRwdTime = onlineInfo->valueForKey("use")->intValue();
        m_timeOnline = onlineInfo->valueForKey("online")->intValue();
        m_startTime = GlobalData::shared()->getTimeStamp();
    }
    if (dict->objectForKey("xml")) {
        auto xml = _dict(dict->objectForKey("xml"));
        auto cnt = m_levelTime.size();
        for (int i = 1; i <= cnt; ++i) {
            string tmp = "rewardID";
            tmp += CC_ITOA(i);
            if (xml->objectForKey(tmp)) {
                auto arr = dynamic_cast<CCArray*>(xml->objectForKey(tmp));
                auto info = TimeExInfo(arr, i);
                m_timeExMap[i] = info;
            }
        }
        if (xml->objectForKey("castleLV")) {
            m_castleLevel = xml->valueForKey("vastleLV")->intValue();
        } else {
            m_castleLevel = FunBuildController::getInstance()->getMainCityLv();
        }
    }
    if (type == 1) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PORT_TIME_EXCHANGE);
    } else {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PORT_DATA_INIT, CCInteger::create(1));
    }
}

//兑换
void PortActController::startExchange(int eId)
{
    int onlineSecs = PortActController::getInstance()->m_timeOnline + GlobalData::shared()->getTimeStamp() - PortActController::getInstance()->m_startTime;
    TimeExchangeCommand* cmd = new TimeExchangeCommand(m_levelTime[eId - 1], onlineSecs);
    cmd->sendAndRelease();
}
void PortActController::endExchange(CCDictionary* dict)
{
    if (dict->objectForKey("errorCode")) {
        string code = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(code));
        if (dict->objectForKey("online") && dict->objectForKey("use")) {
            m_lastRwdTime = dict->valueForKey("use")->intValue();
            m_timeOnline = dict->valueForKey("online")->intValue();
            m_startTime = GlobalData::shared()->getTimeStamp();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PORT_TIME_EXCHANGE);
        }
    } else {
        if (dict->objectForKey("onlineReward")) {
            auto onlineInfo = _dict(dict->objectForKey("onlineReward"));
            m_lastRwdTime = onlineInfo->valueForKey("use")->intValue();
            m_timeOnline = onlineInfo->valueForKey("online")->intValue();
            m_startTime = GlobalData::shared()->getTimeStamp();
        }
        if (dict->objectForKey("reward")) {
            auto arr = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
            string ardInfo = RewardController::getInstance()->retReward(arr);
            CCCommonUtils::flyHint("", "", ardInfo);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PORT_TIME_EXCHANGE);
    }
}

//初始化每日签到数据
void PortActController::startGetDailyRwdData()
{
    DailyRewardInitCommand* cmd = new DailyRewardInitCommand();
    cmd->sendAndRelease();
}
void PortActController::endGetDailyRwdData(CCDictionary* dict)
{
    if (dict) {
        m_loginDayMap.clear();
        m_loginDay = dict->valueForKey("currentDay")->intValue();
        m_isRdLoginDay = dict->valueForKey("isValid")->intValue();
        m_isVipRdLoginDay = dict->valueForKey("isVipValid")->intValue();
        m_isNewDay = false;
        
        CCArray* loginReward = dynamic_cast<CCArray*>(dict->objectForKey("loginReward"));
        CCDictionary *item = NULL;
        for (int i=0; i<loginReward->count(); i++) {
            item = _dict(loginReward->objectAtIndex(i));
            int keyDay = item->valueForKey("day")->intValue();
            m_loginDayMap[keyDay] = DailyRwdInfo(item);
        }
        if (m_loginDayMap.size() > 0) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PORT_DATA_INIT, CCInteger::create(2));
            m_inCnt = 0;
        }
    }
}

void PortActController::initNewPlayerDailyRwdData(CCDictionary* dict)
{
    if (dict) {
        m_newPalyerRDMap.clear();
        
        m_newLoginDay = dict->valueForKey("day")->intValue();
        m_isGetNewRD = dict->valueForKey("signIn")->intValue();
        
        CCArray* rewardInfo = dynamic_cast<CCArray*>(dict->objectForKey("rewardInfo"));
        if (rewardInfo) {
            for (int i=0; i<rewardInfo->count(); i++)
            {
                CCDictionary* rewardDict = dynamic_cast<CCDictionary*>(rewardInfo->objectAtIndex(i));
                if (rewardDict) {
                    if (rewardDict->objectForKey("day") && rewardDict->objectForKey("reward")) {
                        int day = rewardDict->valueForKey("day")->intValue();
                        m_newPalyerRDMap[day] = dynamic_cast<CCArray*>(rewardDict->objectForKey("reward"));
                    }
                }
            }
        }
    }
}

//签到
void PortActController::startGetCheckInRwd(bool isVip,int num,int newNum,int oldNum)
{
    //1 vip领奖 0 普通领奖
    int loginDay = PortActController::getInstance()->m_loginDay;
    string xcKey = "action=loginDay_7rwd_";
    xcKey = xcKey+CC_ITOA(loginDay);
    GameController::getInstance()->callXCApi(xcKey);
    DailyRewardCheckInCommand* cmd = new DailyRewardCheckInCommand(isVip,num,newNum,oldNum);
    cmd->sendAndRelease();
}
void PortActController::endGetCheckInRwd(CCDictionary *dict, bool isVip)
{
    if (dict->objectForKey("errorCode")) {
        string code = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(code));
    }
    else {//获取奖励
        m_loginDay = dict->valueForKey("day")->intValue();
        m_loginDay = MIN(m_loginDay, m_loginDayMap.size());
        if (isVip) {
            m_isVipRdLoginDay = 1;
        } else {
            m_isRdLoginDay = 1;
        }
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        if(arr){
            PortActController::getInstance()->flyReward(arr, true);//true isVip
            RewardController::getInstance()->retReward(arr);

            m_loginDay = MIN(m_loginDay, m_loginDayMap.size());
            if ((m_loginDay == m_loginDayMap.size()) && m_isRdLoginDay) {
                if (!isVip) {
                    CCCommonUtils::flyHint("", "", _lang("111056"));
                }
            } else {
                if(!isVip){
                    CCCommonUtils::flyHint("", "", _lang("111055"));
                }
            }
        }
    }
    if (isVip) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PORT_VIP_LOGINRD_END, CCInteger::create(m_loginDay));;
    } else {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PORT_LOGINRD_END, CCInteger::create(m_loginDay));
    }
}

void PortActController::startGetNewPlayerRD(string key)
{
    int loginDay = PortActController::getInstance()->m_newLoginDay;
    string xcKey = "action=newplayer_7rwd_";
    xcKey = xcKey+CC_ITOA(loginDay);
    GameController::getInstance()->callXCApi(xcKey);
    NewPlayerRewardCommand* cmd = new NewPlayerRewardCommand();
    if (key != "") {
        cmd->putParam("guideStep", CCString::create(key));
    }
    cmd->sendAndRelease();
}

void PortActController::endGetNewPlayerRD(CCDictionary* dict)
{
    if (dict->objectForKey("errorCode")) {
        string code = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(code));
    }
    else {//获取奖励
        CCDictionary* signIn = dynamic_cast<CCDictionary*>(dict->objectForKey("signIn"));
        if (signIn) {
            m_newLoginDay = signIn->valueForKey("day")->intValue();
            m_isGetNewRD = signIn->valueForKey("signIn")->intValue();
        }
        CCArray* reward = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        GCMRewardController::getInstance()->retReward2(reward, true);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_NEW_PLAYER_RD);
    }
}

void PortActController::flyReward(CCArray* arr, bool isVip)
{
    auto toolArr = CCArray::create();
    auto resArr = CCArray::create();
    for (int i = 0 ; i < arr->count(); ++i) {
        auto dict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
        int type = dict->valueForKey("type")->intValue();
        if (type == R_GOODS) {
            toolArr->addObject(arr->objectAtIndex(i));
        } else {
            resArr->addObject(arr->objectAtIndex(i));
        }
    }
    if (toolArr->count()) {
        flyToolReward(toolArr, isVip);
    }
}

void PortActController::flyToolReward(CCArray *arr, bool isVip)
{
    int dyCnt = 0;
    if (isVip) {
        dyCnt = 1;
    } else {
        dyCnt = -1;
    }
    
    auto topLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP);
    int centerX = CCDirector::sharedDirector()->getWinSize().width / 2;
    int centerY = CCDirector::sharedDirector()->getWinSize().height / 2;
    //数组元素数量为1  即只有一种奖励
    auto dict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(0));
    auto value = dynamic_cast<CCDictionary*>(dict->objectForKey("value"));
    std::string itemId = value->valueForKey("itemId")->getCString();
    auto &item = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
    int num = 0;
    if (value->objectForKey("rewardAdd")) {
        num = value->valueForKey("rewardAdd")->intValue();
    } else if (value->objectForKey("count")){
        num = value->valueForKey("count")->intValue() - item.getCNT();
    } else if (value->objectForKey("countString")) {
        string snum = value->valueForKey("countString")->getCString();
        num = atoi(snum.c_str()) - item.getCNT();
    }
    if (num <= 0) {
        return;
    }
    
    auto modelLayer = CCModelLayerColor::create();
    modelLayer->setOpacity(125);
    modelLayer->setColor(ccBLACK);
    modelLayer->setContentSize(CCDirector::sharedDirector()->getWinSize());
    modelLayer->setAnchorPoint(ccp(0.5, 0.5));
    modelLayer->setZOrder(100);
    topLayer->addChild(modelLayer);
    
    auto node = CCNode::create();
    node->setZOrder(200);
    if (CCCommonUtils::isIosAndroidPad())
    {
        node->setScale(1.6);
    }
    if (isVip) {
        auto particle1 = ParticleController::createParticle("MallBag_1");
        auto particle2 = ParticleController::createParticle("MallBag_2");
        auto particle3 = ParticleController::createParticle("MallBag_3");
        particle1->setPosition(0, 65);
        particle2->setPosition(0, 65);
        particle3->setPosition(0, 65);
        node->addChild(particle1);
        node->addChild(particle2);
        node->addChild(particle3);
    }
    
//    auto sprite = CCLoadSprite::createSprite(CCCommonUtils::getIcon(itemId.c_str()).c_str(),true,CCLoadSpriteType_GOODS);
//    sprite->setAnchorPoint(ccp(0.5, 0));
//    CCCommonUtils::setSpriteMaxSize(sprite, 130, true);
//    node->addChild(sprite);
    auto node2 = CCNode::create();
    node2->setPositionY(65);
    node->addChild(node2);
    ActivityController::getInstance()->createGoodsIcon(node2, itemId, 130);
    
    auto label = CCLabelIF::create();
    label->setColor(ccGREEN);
    std::string valueStr = "";
    valueStr.append("+ ").append(CC_ITOA(num));
    label->setString(valueStr.c_str());
    label->setFontSize(24);
    label->setAnchorPoint(ccp(0.5, 0));
    if (CCCommonUtils::isIosAndroidPad()) {
        label->setScale(0.5);
        label->setFontSize(48);
    }
    node->addChild(label);
    
    int endY = -80;//node->getPositionY() + 150;
    int endX = centerX;//node->getPositionX();
    
    CCActionInterval * scaleto = CCScaleTo ::create(0.5, 1);
    if (CCCommonUtils::isIosAndroidPad())
    {
        scaleto = CCScaleTo::create(0.5, 1.6);
    }
    CCActionInterval * easeBackOut1 = CCEaseBackOut ::create(scaleto);
    CCActionInterval * moveto = CCMoveTo ::create(0.5, ccp(centerX, dyCnt * 90 + centerY));
    CCActionInterval * easeBackOut2 = CCEaseBackOut ::create(moveto);
    CCFiniteTimeAction * spawn1 =CCSpawn::create(easeBackOut1,easeBackOut2,NULL);
    
    CCActionInterval * delayT = CCDelayTime ::create(1.25);
    
    CCActionInterval * scaleto2 = CCScaleTo ::create(0.5, 0);
    CCActionInterval * CCEaseBackIn1 = CCEaseBackIn ::create(scaleto2);
    CCActionInterval * moveto2 = CCMoveTo ::create(0.5, ccp(endX, endY));
    CCActionInterval * CCEaseBackIn2 = CCEaseBackIn ::create(moveto2);
    CCFiniteTimeAction * spawn2 =CCSpawn::create(CCEaseBackIn1,CCEaseBackIn2,NULL);
    
    node->setScale(0);
    node->setPosition(centerX, centerY);
    node->setZOrder(200);
    topLayer->addChild(node);
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_loteryrwd);
    node->runAction(CCSequence::create(spawn1,delayT,spawn2,CCCallFuncO::create(this, callfuncO_selector(PortActController::aniComplete),node), CCCallFuncO::create(this, callfuncO_selector(PortActController::removeLayerColor),modelLayer), NULL));
}

void PortActController::aniComplete(CCObject* p){
    auto node = dynamic_cast<CCNode*>(p);
    if(node->getParent()){
        node->removeFromParent();
    }
}

void PortActController::removeLayerColor(CCObject* p){
    auto layerColor = dynamic_cast<CCModelLayerColor*>(p);
    if(layerColor->getParent()){
        layerColor->removeFromParent();
    }
}

void PortActController::showRwdParticle()
{
    
}

bool PortActController::isLastDayFirstIn()
{
    bool ret = false;
    
    if ((m_loginDay == m_loginDayMap.size()) && m_isRdLoginDay && (m_inCnt == 0)) {
        ret = true;
    }

    return ret;
}

void PortActController::startTimeRwd()
{
    TimeRwdCommand* cmd = new TimeRwdCommand();
    cmd->sendAndRelease();
}

void PortActController::endTimeRwd(CCDictionary* params)
{
    const CCString* pstr = params->valueForKey("errorCode");
    if (pstr->compare("") != 0) {
//        CCCommonUtils::flyHint("", "", _lang(pstr->getCString()));
        CCDictionary* cargo = _dict(params->objectForKey("cargo"));
        if (cargo) {
            if (cargo->objectForKey("rewardTime")) {
                m_nextRewardTime = cargo->valueForKey("rewardTime")->doubleValue() / 1000;
                addPush();
            }
            if (cargo->objectForKey("rewardInfo")) {
                m_rewardInfo = cargo->valueForKey("rewardInfo")->getCString();
            }
        }
    } else {
        CCDictionary* cargo = _dict(params->objectForKey("cargo"));
        if (cargo) {
            if (cargo->objectForKey("rewardTime")) {
                m_nextRewardTime = cargo->valueForKey("rewardTime")->doubleValue() / 1000;
                addPush();
            }
            if (cargo->objectForKey("rewardInfo")) {
                m_rewardInfo = cargo->valueForKey("rewardInfo")->getCString();
            }
        }
        if (params->objectForKey("reward")) {
            CCDictionary* rewardDic = _dict(params->objectForKey("reward"));
            auto arr = dynamic_cast<CCArray*>(rewardDic->objectForKey("goods"));
            
            if (arr && arr->count()) {
                CCDictionary *item = NULL;
                auto toolArr = CCArray::create();
                for (int i=0; i<arr->count(); i++) {
                    item = _dict(arr->objectAtIndex(i));
                    string itemId = item->valueForKey("itemId")->getCString();
                    int num = item->valueForKey("count")->intValue();
                    string uuid = item->valueForKey("uuid")->getCString();
                    CCDictionary* value = new CCDictionary();
                    value->setObject(CCString::create(itemId), "itemId");
                    value->setObject(CCString::create(CC_ITOA(num)), "countString");
                    CCDictionary* dic = new CCDictionary();
                    dic->setObject(value, "value");
                    toolArr->addObject(dic);
                }
                if (toolArr->count()) {
                    flyToolReward(toolArr, true);
                }
                for (int i=0; i<arr->count(); i++) {
                    item = _dict(arr->objectAtIndex(i));
                    string itemId = item->valueForKey("itemId")->getCString();
                    int num = item->valueForKey("count")->intValue();
                    string uuid = item->valueForKey("uuid")->getCString();
                    ToolController::getInstance()->addTool(atoi(itemId.c_str()), num, uuid);
                }
                
            }
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_NEW_TIME_RWD_END, params);
}

bool PortActController::isCanRwd()
{
    if (!m_isNewTimeRwd) {
        return false;
    }
    if (m_nextRewardTime <= 0) {
        return false;
    }
    if (m_nextRewardTime > GlobalData::shared()->getTimeStamp()) {
        return false;
    }
    return true;
}

void PortActController::addPush()
{
    if (!m_isNewTimeRwd) {
        return;
    }
    if (m_nextRewardTime <= 0) {
        return;
    }
    if (m_nextRewardTime <= GlobalData::shared()->getTimeStamp()) {
        return;
    }
    
    cocos2d::extension::CCDevice::cancelNotice(89757);
    if (CCCommonUtils::isPushNotify(9)) {
        cocos2d::extension::CCDevice::pushNotice(89757, (m_nextRewardTime - GlobalData::shared()->getTimeStamp()),  _lang("111090"), "0", "16");
    }
}

bool PortActController::canPopNewRDView()
{
    if (QuestController::getInstance()->currentAchList.find("2102501") != QuestController::getInstance()->currentAchList.end()) {
        QuestInfo* qusetTmp = QuestController::getInstance()->currentAchList["2102501"];
        if(qusetTmp->state == ACCEPT) {
            return false;
        }
    }
    
    if(CCCommonUtils::isTestPlatformAndServer("guide_3") && PortActController::getInstance()->m_isGetNewRD==0 && PortActController::getInstance()->m_newLoginDay<7 && PortActController::getInstance()->m_newPalyerRDMap.size() > 0){
        return true;
    }
    else {
        return false;
    }
}


//
//累计充值回馈
//
static RepayController* _repayInstance = NULL;
RepayController* RepayController::getInstance() {
    if (!_repayInstance) {
        _repayInstance = new RepayController();
    }
    return _repayInstance;
}
RepayController::RepayController():m_oldPayPoint(0),m_newPayPoint(0),m_endTime(0),m_startTime(0),m_changeLV(false){
    
}
RepayController::~RepayController(){
    repayRewards.clear();
}
bool RepayController::sortRepayItem(const RepayRewardStruct& itemId1,const RepayRewardStruct& itemId2){
    return (itemId1.point < itemId2.point);
}
bool RepayController::isBegin(){
    if(GlobalData::shared()->analyticID == "common"){
        return false;
    }
    
    if(m_startTime > 0 && m_endTime > 0){
        double time =  GlobalData::shared()->getWorldTime();
        double sT = GlobalData::shared()->changeTime(m_startTime);
        double eT = GlobalData::shared()->changeTime(m_endTime);
        auto tmp = time - sT;
        auto tmp1 = eT - time;
        if(tmp>0 && tmp1>0){
            return true;
        }
    }
    
    return false;
}
void RepayController::addPoint(int addv){
    if(!isBegin() || addv <= m_newPayPoint)
        return;
//    int maxPoint = repayRewards.at(repayRewards.size()-1).point;
//    if(m_newPayPoint>=maxPoint)
//        return;
//    m_newPayPoint = MIN(addv, maxPoint);
    
    m_newPayPoint = addv;
    if(this->getLVIndexByPoint(m_newPayPoint)!=this->getLVIndexByPoint(m_oldPayPoint)){
        m_changeLV=true;
    }else{
        m_changeLV = false;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REPAY_INFO_INIT);
}
void RepayController::resetChangeLV(){
    m_oldPayPoint = m_newPayPoint;
    m_changeLV=false;
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REPAY_INFO_INIT);
}
void RepayController::testInfoInit(){
    return;
    m_startTime = 1431187200; //1431485339
    m_endTime = 1432051200;
    m_newPayPoint = 3;
    m_oldPayPoint = m_newPayPoint;
    repayRewards.clear();
    for (int index = 0; index < 10; ++index) {
        RepayRewardStruct rrStruct;
        rrStruct.point = index * 5;
        int reward = MAX(index * 5 / 2,1);
        for(int index0 = 0; index0<reward;++index0){
            RepayRewardStruct1 struct1;
            struct1.type = R_GOODS;
            struct1.itemid = CC_ITOA(200300 + index0);
            struct1.count = index0+1;
            struct1.orderid = 1;
            rrStruct.rewards.push_back(struct1);
        }
        repayRewards.push_back(rrStruct);
    }
    if(this->isBegin()){
        sort(repayRewards.begin(), repayRewards.end(), sortRepayItem);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REPAY_INFO_INIT);
    }
}
void RepayController::initRepayInfo(CCDictionary *ccDict){
    repayRewards.clear();
    if(!ccDict){
        m_startTime = 0;
        m_endTime = 0;
    }else{
        if(ccDict->objectForKey("startTime")){
            m_startTime = ccDict->valueForKey("startTime")->doubleValue()/1000;
        }
        if(ccDict->objectForKey("endTime")){
            m_endTime = ccDict->valueForKey("endTime")->doubleValue()/1000;
        }
        if(ccDict->objectForKey("payPoint")){
            m_newPayPoint = ccDict->valueForKey("payPoint")->intValue();
            m_oldPayPoint = m_newPayPoint;
        }
        if(ccDict->objectForKey("payRewards")){
            CCArray* rewardArr = dynamic_cast<CCArray*>(ccDict->objectForKey("payRewards"));
            CCObject* rewardObj = NULL;
            CCARRAY_FOREACH(rewardArr, rewardObj){
                CCDictionary *tmpDict = _dict(rewardObj);
                if(!tmpDict)
                    continue;
                RepayRewardStruct rrStruct;
                if(tmpDict->objectForKey("point")){
                    rrStruct.point = tmpDict->valueForKey("point")->intValue();
                }
                if(tmpDict->objectForKey("reward")){
                    CCArray *tmpArr = dynamic_cast<CCArray*>(tmpDict->objectForKey("reward"));
                    if(!tmpArr)
                        continue;
                    CCObject *tmpObj = NULL;
                    CCARRAY_FOREACH(tmpArr, tmpObj){
                        CCDictionary *tmpDict1 = _dict(tmpObj);
                        if(!tmpDict1)
                            continue;
                        if(!tmpDict1->objectForKey("type"))
                            continue;
                        RepayRewardStruct1 struct1;
                        struct1.type = tmpDict1->valueForKey("type")->intValue();
                        switch (struct1.type) {
                            case R_EXP:{
                                int addValue = tmpDict1->valueForKey("value")->intValue();
                                struct1.count = addValue;
                                struct1.itemid = CC_ITOA(struct1.type);
                                struct1.orderid = 5;
                                break;
                            }
                            case R_POWER:{
                                int addValue = tmpDict1->valueForKey("value")->intValue();
                                struct1.count = addValue;
                                struct1.itemid = CC_ITOA(struct1.type);
                                struct1.orderid = 4;
                                break;
                            }
                            case R_WOOD:
                            case R_STONE:
                            case R_IRON:
                            case R_FOOD:
                            case R_SILVER:
                            case R_GOLD:
                            case R_CHIP:
                            case R_DIAMOND:{
                                int addValue = tmpDict1->valueForKey("value")->intValue();
                                struct1.count = addValue;
                                struct1.itemid = CC_ITOA(struct1.type);
                                struct1.orderid = 3;
                                break;
                            }
                            case R_GENERAL:{
                                break;
                            }
                            case R_GOODS:{
                                CCDictionary *goodsDict = _dict(tmpDict1->objectForKey("value"));
                                if(!goodsDict)
                                    continue;
                                struct1.itemid = goodsDict->valueForKey("id")->getCString();
                                struct1.count = goodsDict->valueForKey("num")->intValue();
                                struct1.orderid = 1;
                                break;
                            }
                            case R_EQUIP:{
                                CCDictionary *equipDict = _dict(tmpDict1->objectForKey("value"));
                                if(!equipDict)
                                    continue;
                                struct1.itemid = equipDict->valueForKey("id")->getCString();
                                struct1.count = equipDict->valueForKey("num")->intValue();
                                struct1.orderid = 2;
                                break;
                            }
                            default:
                                break;
                        }
                        rrStruct.rewards.push_back(struct1);
                    }
                }
                repayRewards.push_back(rrStruct);
            }
        }
    }
    
    if(this->isBegin()){
        sort(repayRewards.begin(), repayRewards.end(), sortRepayItem);
        RepayRewardStruct rrStruct;
        rrStruct.point = 0;
        repayRewards.insert(repayRewards.begin(),rrStruct);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REPAY_INFO_INIT);
    }else{
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REPAY_INFO_INIT);
    }
}
int RepayController::getLVIndexByPoint(int point){
    int max = repayRewards.size();
    for (int index = 0; index < max; index++) {
        auto value = repayRewards.at(index);
        if(point < value.point){
            return index;
        }
    }
    return max-1;
}