//
//  EnemyInfoController.cpp
//  IF
//
//  Created by fubin on 14-2-24.
//
//

#include "EnemyInfoController.h"
#include "FunBuildController.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "BattleManager.h"
#include "SceneController.h"
#include "WorldController.h"
static EnemyInfoController *_instance = NULL;

EnemyInfoController::EnemyInfoController(){
    m_curType = -1;
}

EnemyInfoController::~EnemyInfoController() {
}

EnemyInfoController* EnemyInfoController::getInstance() {
    if (!_instance) {
        _instance = new EnemyInfoController();
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(EnemyInfoController::onEnterFrame), _instance, 1.0,kCCRepeatForever, 0.0f, false);
    }
    return _instance;
}

void EnemyInfoController::purgeData() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(EnemyInfoController::onEnterFrame), _instance);
    delete _instance;
    _instance = NULL;
}

void EnemyInfoController::initEnemyData(CCDictionary* dict)
{
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("enemy"));
    if(arr==NULL) {
        return;
    }
    CCDictionary* item = NULL;
    m_enemyInfos.clear();
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        EnemyInfo info = EnemyInfo(item);
        m_enemyInfos.push_back(info);
    }
    m_ignorInfos.clear();
    std::string ignoreStr = GlobalData::shared()->playerInfo.uid + "WTIgnoreList";
    std::string ignoreList = CCUserDefault::sharedUserDefault()->getStringForKey(ignoreStr.c_str(), "");
    vector<std::string> vector1;
    CCCommonUtils::splitString(ignoreList, "|", vector1);
    int num = vector1.size();
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < m_enemyInfos.size(); j++) {
            if (m_enemyInfos[j].uuid == vector1[i]) {
                m_ignorInfos.push_back(m_enemyInfos[j]);
                break;
            }
        }
    }
    ignoreList = "";
    for (int i = 0; i<m_ignorInfos.size(); i++) {
        ignoreList.append(m_ignorInfos[i].uuid);
        if (i != m_ignorInfos.size() - 1) {
            ignoreList.append("|");
        }
    }
    m_ignorInfos.clear();
    CCUserDefault::sharedUserDefault()->setStringForKey(ignoreStr.c_str(), ignoreList.c_str());
    CCUserDefault::sharedUserDefault()->flush();
}

bool EnemyInfoController::checkMonsterWar(){
    bool flag = false;
    for (int j = 0; j < m_enemyInfos.size(); j++) {
        if (m_enemyInfos[j].monsterCount >0) {
            flag = true;
            break;
        }
    }
    return flag;
}

void EnemyInfoController::pushEnemyData(CCDictionary* dict)
{
    string uuid = dict->valueForKey("uuid")->getCString();
    EnemyInfo info = EnemyInfo(dict);
    if (info.allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
            if (info.defType== Trebuchet) {
                return;
            }
       
    }
    bool _tmpB = true;
    std::vector<EnemyInfo>::iterator it = m_enemyInfos.begin();
    for (int i=0; i<m_enemyInfos.size(); i++) {
        if (m_enemyInfos[i].uuid == info.uuid)
        {
            if (dict->objectForKey("et")) {
                m_enemyInfos[i].stampTime = dict->valueForKey("et")->doubleValue()/1000;
                m_enemyInfos[i].arrivalTime = m_enemyInfos[i].stampTime;
                if (m_enemyInfos[i].arrivalTime>0) {
                    m_enemyInfos[i].arrivalTime = GlobalData::shared()->changeTime(m_enemyInfos[i].arrivalTime);
                }
            }else {
                m_enemyInfos.erase(it);
                m_enemyInfos.push_back(info);
            }
            
//            m_enemyInfos[i].stampTime = info.stampTime;
//            m_enemyInfos[i].arrivalTime = info.arrivalTime;
            _tmpB = false;
            break;
        }
        it++;
    }
    if (_tmpB) {
        m_enemyInfos.push_back(info);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ENEMY_INFO_LIST_CHANGE);
}

void EnemyInfoController::checkEnemyData()
{
    int curTime = GlobalData::shared()->getWorldTime();
    vector<EnemyInfo>::iterator it = m_enemyInfos.begin();
    for (int i=0; i<m_enemyInfos.size(); i++) {
        if (m_enemyInfos[i].arrivalTime<=curTime) {
            m_enemyInfos.erase(it);
            break;
        }

        //如果是同一个联盟就不是敌人
//        AllianceInfo *pInfo = &GlobalData::shared()->playerInfo.allianceInfo;
//        if(m_enemyInfos[i].abbr==pInfo->shortName){
//            m_enemyInSameAlliance.push_back(m_enemyInfos[i]);
//            m_enemyInfos.erase(it);
//            break;
//        }
        it++;
    }
//    vector<EnemyInfo>::iterator it2 = m_enemyInSameAlliance.begin();
//    for (int i=0; i<m_enemyInSameAlliance.size(); i++) {
//        //如果是同一个联盟就不是敌人
//        AllianceInfo *pInfo = &GlobalData::shared()->playerInfo.allianceInfo;
//        if(m_enemyInSameAlliance[i].abbr!=pInfo->shortName){
//            m_enemyInfos.push_back(m_enemyInSameAlliance[i]);
//            m_enemyInSameAlliance.erase(it2);
//            break;
//        }
//        it2++;
//    }
}

void EnemyInfoController::onEnterFrame(float dt)
{
    int curType = -1;
    int curTime = GlobalData::shared()->getWorldTime();
    vector<EnemyInfo>::iterator it = m_enemyInfos.begin();
    for (int i=0; i<m_enemyInfos.size(); i++) {
        if (m_enemyInfos[i].arrivalTime<=curTime) {
            m_enemyInfos.erase(it);
            std::string ignoreStr = GlobalData::shared()->playerInfo.uid + "WTIgnoreList";
            std::string ignoreList = "";
            for (int j = 0; j<m_enemyInfos.size(); j++) {
                ignoreList.append(m_enemyInfos[j].uuid);
                if (j != m_enemyInfos.size() - 1) {
                    ignoreList.append("|");
                }
            }
            CCUserDefault::sharedUserDefault()->setStringForKey(ignoreStr.c_str(), ignoreList.c_str());
            CCUserDefault::sharedUserDefault()->flush();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ENEMY_INFO_LIST_CHANGE);
            return;
        }
        else {
            std::string ignoreStr = GlobalData::shared()->playerInfo.uid + "WTIgnoreList";
            std::string ignoreList = CCUserDefault::sharedUserDefault()->getStringForKey(ignoreStr.c_str(), "");
            vector<std::string> vector1;
            bool flag = true;
            CCCommonUtils::splitString(ignoreList, "|", vector1);
            int num = vector1.size();
            for (int i = 0; i < num; i++) {
                if (it->uuid == vector1[i]) {
                    flag = false;
                    break;
                }
            }

            if (flag) {
                if (curType == ENEMY_TYPE_BATTLE || curType == ENEMY_TYPE_TEAM || curType == ENEMY_TYPE_MONSTER_ATTACK) {
                    break;
                }
                else if ((m_enemyInfos[i].defType == Tile_allianceArea || m_enemyInfos[i].defType == tile_superMine || m_enemyInfos[i].defType == tile_tower || m_enemyInfos[i].defType == tile_banner) && m_enemyInfos[i].allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                    break;
                }
                else {
                    curType = m_enemyInfos[i].type;
                }
            }
        }
        it++;
    }
    if (m_curType != curType) {
        m_curType = curType;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ENEMY_INFO_CHANGE);
    }
}

void EnemyInfoController::clearAllEnemy(){
    m_enemyInfos.clear();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ENEMY_INFO_CHANGE);
}
