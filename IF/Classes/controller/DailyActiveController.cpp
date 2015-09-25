//
//  DailyActiveController.cpp
//  IF
//
//  Created by chenfubi on 15/4/10.
//
//

#include "DailyActiveController.h"
#include "FunBuildController.h"
#include "SceneController.h"
#include "PopupViewController.h"
#include "WorldMapView.h"
#include "ImperialScene.h"
#include "UIComponent.h"

static DailyActiveController* s_sharedDailyActiveController = NULL;

DailyActiveController* DailyActiveController::shared(void)
{
	if (!s_sharedDailyActiveController)
	{
		s_sharedDailyActiveController = new DailyActiveController();
	}
	return s_sharedDailyActiveController;
}

void DailyActiveController::purge()
{
	CC_SAFE_RELEASE_NULL(s_sharedDailyActiveController);
}

DailyActiveController::DailyActiveController()
:m_activePoints(0)
,m_tomorrowTime(0)
,m_dailyActiveOpenFlag(0)
,m_hasUnlockItem(false)
,m_hasInit(false)
,m_curReward(0)
,m_perReward(0)
,m_preActPoint(0)
{
	
}

DailyActiveController::~DailyActiveController()
{
    
}

void DailyActiveController::resetAllData()
{
    m_tomorrowTime+=86400;
    m_activePoints = 0;
    m_preActPoint = 0;
    map<string, DailyActiveItemInfo>::iterator l_it = m_itemInfo.begin();
    for (; l_it != m_itemInfo.end(); l_it++)
    {
        l_it->second.nowNum = 0;
        l_it->second.times = 0;
    }

    m_curReward = 0;
    updateBoxInfo(nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DAILYQEUST_INIT);
}

void DailyActiveController::getDataFormServer()
{
    if (m_dailyActiveOpenFlag != 1)
    {
        return;
    }
    m_itemInfo.clear();
    m_boxInfo.clear();
    m_activePoints = 0;
    m_tomorrowTime = GlobalData::shared()->tomorrow_time;
    m_hasUnlockItem = false;
    m_hasInit = false;
    m_curReward = 0;
    
    DailyAcitveInitCmd* cmd = new DailyAcitveInitCmd();
    cmd->sendAndRelease();
}

void DailyActiveController::init(CCDictionary* dict)
{
    m_tomorrowTime = GlobalData::shared()->tomorrow_time;
    CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("dailyQuest"));
    initItemInfo(arr);
    updateActivePoint();
    m_curReward = dict->valueForKey("curReward")->intValue();
    CCArray* reward = (CCArray*)dict->objectForKey("rewardList");
    initBoxInfo(reward);
    updateBoxInfo(nullptr);
    m_hasInit = true;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DAILYQEUST_INIT);
}

void DailyActiveController::updateActivePoint()
{
    m_activePoints = 0;
    map<string, DailyActiveItemInfo>::iterator l_it = m_itemInfo.begin();
    for (; l_it != m_itemInfo.end(); l_it++)
    {
        if (!l_it->second.isShow)
        {
            continue;
        }
        m_activePoints += l_it->second.point*l_it->second.times;
    }
    if (m_preActPoint == 0) {
        m_preActPoint = m_activePoints;
    }
}

void DailyActiveController::initItemInfo(cocos2d::CCArray *arr)
{
    m_itemInfo.clear();
    CCDictionary* dict = LocalController::shared()->DBXMLManager()->getGroupByKey("daily_quest");
    CCDictElement* pelem = NULL;
    CCDictionary* d = NULL;
    CCDICT_FOREACH(dict, pelem)
    {
        d = dynamic_cast<CCDictionary*>(pelem->getObject());
        m_itemInfo[pelem->getStrKey()] = DailyActiveItemInfo();
        m_itemInfo[pelem->getStrKey()].initData(d);
    }
    CCObject* obj = NULL;
    string key = "";
    CCARRAY_FOREACH(arr, obj)
    {
        d = dynamic_cast<CCDictionary*>(obj);
        key = d->valueForKey("id")->getCString();
        updateItemInfo(key, d);
    }
}

void DailyActiveController::updateItemInfo(string itemId, CCDictionary* dict, bool setIsNew/*=false*/)
{
    if (m_itemInfo.find(itemId) == m_itemInfo.end())
    {
        m_itemInfo[itemId] = DailyActiveItemInfo();
        m_itemInfo[itemId].initData(itemId);
    }
    m_itemInfo[itemId].updateData(dict, setIsNew);
}

void DailyActiveController::initBoxInfo(CCArray* arr)
{
    CCDictionary* dict = nullptr;
    CCObject* obj = nullptr;
    int id = 0;
    CCARRAY_FOREACH(arr, obj)
    {
        dict = dynamic_cast<CCDictionary*>(obj);
        if (dict)
        {
            id = dict->valueForKey("point")->intValue();
            if (id == 0)
            {
                continue;
            }
            if (m_boxInfo.find(id) == m_boxInfo.end())
            {
                m_boxInfo[id] = DailyActiveBoxInfo();
                m_boxInfo[id].initData(dict);
            }
        }
    }
}

void DailyActiveController::updateBoxInfo(CCDictionary* dict)
{
    map<int, DailyActiveBoxInfo>::iterator l_it = m_boxInfo.begin();
    for (; l_it != m_boxInfo.end(); l_it++)
    {
        l_it->second.state = BOX_UN_COMPLETE;
        if (m_curReward >= l_it->second.point)
        {
            l_it->second.state = BOX_GET_REWARD;
        }
        else if (m_activePoints >= l_it->second.point)
        {
            l_it->second.state = BOX_COMPLETE;
            break;
        }
    }
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DAILYQEUST_REWARD);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DAILYQEUST_BOX_INIT);
}

CCArray* DailyActiveController::getSortItemArr()
{
    /*
     1.刚解锁项目 2.可进行项目 3.已完成项目 4.锁住项目
     */
    CCArray* itemArr = CCArray::create();
    CCArray* unlockArr = CCArray::create();
    CCArray* lockArr = CCArray::create();
    int mainLevel = FunBuildController::getInstance()->getMainCityLv();
    map<string, DailyActiveItemInfo>::iterator l_it = m_itemInfo.begin();
    for (; l_it != m_itemInfo.end(); l_it++)
    {
        l_it->second.unlock = false;
        l_it->second.playAni = false;
        
        if (!l_it->second.isShow)
        {
            continue;
        }
        if (l_it->second.unlockLevel > mainLevel)
        {
            l_it->second.unlock = true;
            lockArr->addObject(ccs(l_it->first));
        }
        else
        {
            unlockArr->addObject(ccs(l_it->first));
        }
    }
    CCArray* comArr = CCArray::create();
    CCArray* unComArr = CCArray::create();
    string key = "";
    for (int i = 0; i < unlockArr->count(); i++)
    {
        key = dynamic_cast<CCString*>(unlockArr->objectAtIndex(i))->getCString();
        if (m_itemInfo[key].hasComplete)
        {
            comArr->addObject(ccs(key));
        }
        else
        {
            unComArr->addObject(ccs(key));
        }
    }
    
    CCArray* comPlayArr = CCArray::create();
    CCArray* comUnplayArr = CCArray::create();
    for (int i = 0; i < comArr->count(); i++)
    {
        key = dynamic_cast<CCString*>(comArr->objectAtIndex(i))->getCString();
        if (m_itemInfo[key].isNew)
        {
            comPlayArr->addObject(ccs(key));
        }
        else
        {
            comUnplayArr->addObject(ccs(key));
        }
    }
    
    if (m_hasUnlockItem && comPlayArr->count()==0)
    {
        CCArray* playArr = CCArray::create();
        CCArray* unPlayArr = CCArray::create();
        for (int i = 0; i < unComArr->count(); i++)
        {
            key = dynamic_cast<CCString*>(unComArr->objectAtIndex(i))->getCString();
            if (m_itemInfo[key].unlockLevel == mainLevel)
            {
                m_itemInfo[key].playAni = true;
                playArr->addObject(ccs(key));
            }
            else
            {
                unPlayArr->addObject(ccs(key));
            }
        }
        itemArr->addObjectsFromArray(playArr);
        sortArray(unPlayArr);
        itemArr->addObjectsFromArray(unPlayArr);
    }
    else
    {
        itemArr->addObjectsFromArray(comPlayArr);
        sortArray(unComArr);
        itemArr->addObjectsFromArray(unComArr);
    }
    itemArr->addObjectsFromArray(comUnplayArr);
    itemArr->addObjectsFromArray(lockArr);
    DailyActiveController::shared()->m_hasUnlockItem = false;
    
    return itemArr;
}

void DailyActiveController::sortArray(cocos2d::CCArray *arr)
{
    string key1 = "";
    string key2 = "";
    float per1 = 0.0f;
    float per2 = 0.0f;
    for (int i = 0; i < arr->count(); i++)
    {
        for (int j = arr->count()-1; j > i; j--)
        {
            key1 = dynamic_cast<CCString*>(arr->objectAtIndex(i))->getCString();
            key2 = dynamic_cast<CCString*>(arr->objectAtIndex(j))->getCString();
            per1 = m_itemInfo[key1].times/(m_itemInfo[key1].totalTimes*1.0f);
            per2 = m_itemInfo[key2].times/(m_itemInfo[key2].totalTimes*1.0f);
            if (per1 < per2)
            {
                arr->exchangeObjectAtIndex(i, j);
            }
        }
    }
}

bool DailyActiveController::canGetReward()
{
    bool ret = false;
    if (m_dailyActiveOpenFlag != 1)
    {
        return ret;
    }
    map<int, DailyActiveBoxInfo>::iterator l_it = m_boxInfo.begin();
    for (; l_it != m_boxInfo.end(); l_it++)
    {
        if (l_it->second.state == BOX_COMPLETE)
        {
            ret = true;
            break;
        }
    }
    return ret;
}

bool DailyActiveController::needShow()
{
    bool ret = false;
    if (m_dailyActiveOpenFlag != 1)
    {
        return ret;
    }
    
    map<string, DailyActiveItemInfo>::iterator l_it = m_itemInfo.begin();
    for (; l_it != m_itemInfo.end(); l_it++)
    {
        if (!l_it->second.hasComplete && l_it->second.isShow)
        {
            ret = true;
            break;
        }
    }
    return ret;
}

void DailyActiveController::getBoxReward(string itemId)
{
    int id = atoi(itemId.c_str());
    if (m_boxInfo.find(id) != m_boxInfo.end())
    {
        m_boxInfo[id].state = BOX_GET_REWARD;
        m_perReward = id;
        DailyActiveRewardCmd* cmd = new DailyActiveRewardCmd();
        cmd->sendAndRelease();
    }
}

void DailyActiveController::updateCurReward()
{
    m_curReward = m_perReward;
}

int DailyActiveController::getTotalPoint()
{
    int point = m_boxInfo.rbegin()->second.point;
    return point;
}

void DailyActiveController::goActTarget(string itemId)
{
    if (itemId.length() <= 0)
    {
        return;
    }
    DailyActiveItemInfo* info = &m_itemInfo[itemId];
    if(info == NULL){
        return;
    }
    int type = info->gotype;
    int goItemId = info->go;
    switch (type) {
        case 1:{
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN)
            {
                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                if(layer)
                {
                    int bid = FunBuildController::getInstance()->getMaxLvBuildByType(goItemId);
                    if(bid>0)
                    {
                        layer->onMoveToBuildAndPlay(bid);
                    }
                    else
                    {
                        CCCommonUtils::flyHint("", "", _lang("133250"));
                    }
                }
            }
            break;
        }
        case 2://UIComponent
            UIComponent::getInstance()->onMoveAndPlay(goItemId);
            break;
        case 3://world
            UIComponent::getInstance()->onMoveAndPlay(goItemId);
            break;
        case 4://spebuild
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN)
            {
                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                if(layer)
                {
                    layer->onMoveToSpeBuildAndPlay(goItemId);
                }
            }
            break;
        default:
            break;
    }
}

#include "GCMRewardController.h"
//CLASS DailyActiveRewardCmd
bool DailyActiveRewardCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILYACTIVEREWARDCMD) != 0)
    {
        return false;
    }
    
    DailyActiveController::shared()->updateBoxInfo(nullptr);
    CCDictionary* params = _dict(dict->objectForKey("params"));
    if(params==NULL) return false;
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }
    else
    {
        if (params->objectForKey("award"))
        {
            GCMRewardController::getInstance()->retReward2(dynamic_cast<CCArray*>(params->objectForKey("award")), true);
        }
        DailyActiveController::shared()->updateCurReward();
        DailyActiveController::shared()->updateBoxInfo(nullptr);
    }
    return true;
}


//CLASS DailyAcitveInitCmd
bool DailyAcitveInitCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILYACTIVEINITCMD) != 0)
    {
        return false;
    }
    
    CCDictionary* params = _dict(dict->objectForKey("params"));
    if(params==NULL) return false;
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }
    else
    {
       DailyActiveController::shared()->init(params);
    }
    return true;
}

//CLASS DailyActiveFirstCmd
bool DailyActiveFirstCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILYACTIVEFIRST) != 0)
    {
        return false;
    }
    
    CCDictionary* params = _dict(dict->objectForKey("params"));
    if(params==NULL) return false;
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }
    else
    {
        DailyActiveController::shared()->updateBoxInfo(params);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DAILYQEUST_BOX_INIT);
    }
    
    return true;
}
