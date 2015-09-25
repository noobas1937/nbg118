//
//  MarchDetailView.cpp
//  IF
//
//  Created by 邹 程 on 14-3-27.
//
//

#include "MarchDetailView.h"
#include "WorldMapView.h"
#include "PopupViewController.h"
#include "UseToolView.h"
#include "UseCDToolView.h"
#include "ToolController.h"

void MarchDetailInfo::setInfo(cocos2d::CCDictionary *dict) {
    if (!dict) {
        return;
    }
    
    if (dict->objectForKey("uid")) {
        userUid = dict->valueForKey("uid")->getCString();
    }
    if (dict->objectForKey("uuid")) {
        uuid = dict->valueForKey("uuid")->getCString();
    }
    if (dict->objectForKey("name")) {
        name = dict->valueForKey("name")->getCString();
    }
    if (dict->objectForKey("targetPointIndex")) {
        targetPointIndex = dict->valueForKey("targetPointIndex")->intValue();
    }
    if (dict->objectForKey("marchState")) {
        marchState = (MarchStateType)dict->valueForKey("marchState")->intValue();
    }
    if (dict->objectForKey("targetType")) {
        marchTarget = (WorldCityType)dict->valueForKey("targetType")->intValue();
    }
    if (dict->objectForKey("marchType")) {
        marchMethod = (MarchMethodType)dict->valueForKey("marchType")->intValue();
    }
    if (dict->objectForKey("startStamp")) {
        startStamp = dict->valueForKey("startStamp")->doubleValue();
    }
    if (dict->objectForKey("endStamp")) {
        endStamp = dict->valueForKey("endStamp")->doubleValue();
    }
    if (dict->objectForKey("actionMax")) {
        actionMax = dict->valueForKey("actionMax")->intValue();
    }
    soldiers.clear();
    int index = 2;
    bool isInMember = false;
    if (dict->objectForKey("members")) {
        auto memArr = dynamic_cast<CCArray*>(dict->objectForKey("members"));
        CCObject *obj = NULL;
        totalTroop = 0;
        CCARRAY_FOREACH(memArr, obj) {
            auto memDict = _dict(obj);
            std::string nameStr = memDict->valueForKey("name")->getCString();
            std::string memberUUid = "";
            if (memDict->objectForKey("uid")) {
                memberUUid = memDict->valueForKey("uid")->getCString();
            }
            if(memberUUid == userUid){
                isInMember = true;
            }
            string tempMonsterId = "";
            if (memDict->objectForKey("monsterId")) {
                tempMonsterId = memDict->valueForKey("monsterId")->getCString();
                isInMember = true;
            }
            CCArray *allSoldier = dynamic_cast<CCArray*>(memDict->objectForKey("soldiers"));
            CCObject* sobj;
            CCARRAY_FOREACH(allSoldier, sobj) {
                auto soldier = dynamic_cast<CCDictionary*>(sobj);
                if (soldier) {
                    auto soldierInfo = MarchSoldierDetail();
                    soldierInfo.armyId = soldier->valueForKey("armyId")->getCString();
                    soldierInfo.count = soldier->valueForKey("count")->intValue();
                    bool isLeader = false;
                    if(soldier->objectForKey("leader")){
                        isLeader = soldier->valueForKey("leader")->boolValue();
                    }
                    soldierInfo.isLeader = isLeader;
                    soldierInfo.playerName = nameStr;
                    soldierInfo.monsterId = tempMonsterId;
                    if(isLeader){
                        soldiers[1].push_back(soldierInfo);
                    }else{
                        soldiers[index].push_back(soldierInfo);
                    }
                    totalTroop += soldierInfo.count;
                }
            }
            index++;
        }
    }
    
    if(!isInMember){
        if (dict->objectForKey("totalTroops")) {
            totalTroop = dict->valueForKey("totalTroops")->intValue();
        }
        if (dict->objectForKey("soldiers")) {
            auto allSoldier = dynamic_cast<CCArray*>(dict->objectForKey("soldiers"));
            CCObject* obj;
            CCARRAY_FOREACH(allSoldier, obj) {
                auto soldier = dynamic_cast<CCDictionary*>(obj);
                if (soldier) {
                    auto soldierInfo = MarchSoldierDetail();
                    soldierInfo.armyId = soldier->valueForKey("armyId")->getCString();
                    soldierInfo.count = soldier->valueForKey("count")->intValue();
                    soldierInfo.playerName = name;
                    soldiers[index].push_back(soldierInfo);
                }
            }
            index++;
        }
    }
    
    if (dict->objectForKey("generals")) {
        generals.clear();
        auto allGeneral = dynamic_cast<CCArray*>(dict->objectForKey("generals"));
        CCObject* obj;
        CCARRAY_FOREACH(allGeneral, obj) {
            auto general = dynamic_cast<CCDictionary*>(obj);
            if (general) {
                auto generalInfo = MarchGeneralDetail();
                generalInfo.generalId = general->valueForKey("genId")->getCString();
                generalInfo.generalLv = general->valueForKey("level")->intValue();
                generals.push_back(generalInfo);
            }
        }
    }
    
    inUpdate = false;
}