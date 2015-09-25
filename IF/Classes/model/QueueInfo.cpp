//
//  QueueInfo.cpp
//  IF
//
//  Created by fubin on 14-2-19.
//
//

#include "QueueInfo.h"
#include "FunBuildController.h"
#include "QueueController.h"
#include "WorldController.h"
#include "EquipmentController.h"
#include "DragonBuildingController.h"

QueueInfo::QueueInfo(CCDictionary* dict)
{
    this->qid = dict->valueForKey("qid")->intValue();
    this->type = dict->valueForKey("type")->intValue();
    this->uuid = dict->valueForKey("uuid")->getCString();
    this->qid = CCCommonUtils::getQueueSortByType(this->type) *1000 + (this->type + 1)*100+this->qid;
    
    auto tmp = dict->valueForKey("endTime")->doubleValue()/1000;
    if (tmp == 0) {
        this->endTime = 0;
    }
    else{
        this->endTime = GlobalData::shared()->changeTime(tmp);
    }
    
    tmp = dict->valueForKey("updateTime")->doubleValue()/1000;
    if (tmp == 0) {
        this->finishTime = 0;
    }
    else{
        this->finishTime = GlobalData::shared()->changeTime(tmp);
    }
    
    tmp = dict->valueForKey("startTime")->doubleValue()/1000;
    if (tmp == 0) {
        this->startTime = 0;
    }
    else{
        this->startTime = GlobalData::shared()->changeTime(tmp);
    }
    if(dict->objectForKey("isHelped")){
        canHelp = dict->valueForKey("isHelped")->intValue()==0;
    }
    auto obj = dict->objectForKey("itemObj");
    auto autoDict = _dict(obj);
    itemId = autoDict->valueForKey("itemId")->intValue();
    
    para=0;
    allId="";
    if (obj && itemId != 0) {
        if (this->type == TYPE_BUILDING) {
            int pos = autoDict->valueForKey("pos")->intValue();
            int tk = FunBuildController::getInstance()->getBuildKey(itemId, pos);
            this->key = CC_ITOA(tk);
            FunBuildController::getInstance()->pushBuildQueue(autoDict);
        }
        else if (type == TYPE_FORCE || type == TYPE_RIDE_SOLDIER || type == TYPE_BOW_SOLDIER || type == TYPE_CAR_SOLDIER) {
            std::string tk = QueueController::getInstance()->getKey(itemId, type);
            key = tk;
        }
        else if (type == TYPE_FORT) {
            std::string tk = QueueController::getInstance()->getKey(itemId, type);
            key = tk;
        }
        else if (type == TYPE_HOSPITAL) {
            std::string tk = QueueController::getInstance()->getKey(itemId, type);
            key = tk;
        }
        else if (type == TYPE_AFFAIRS) {
            std::string tk = QueueController::getInstance()->getKey(itemId, type);
            key = tk;
        }
        else if (type == TYPE_ALLIANCE_AFFAIRS) {
            std::string tk = QueueController::getInstance()->getKey(itemId, type);
            key = tk;
        }
        else if (type == TYPE_SCIENCE) {
            std::string tk = QueueController::getInstance()->getKey(itemId, type);
            key = tk;
        }
        else if (type == TYPE_FORGE) {
            std::string tk = QueueController::getInstance()->getKey(itemId, type);
            key = tk;
            if (autoDict->objectForKey("cost")) {
                string cost = autoDict->valueForKey("cost")->getCString();
                EquipmentController::getInstance()->m_curCreateCost = cost;
            }
        }
        else if (type == TYPE_MATE) {
            std::string tk = QueueController::getInstance()->getKey(itemId, type);
            key = tk;
            if(autoDict->valueForKey("para")){
                para = autoDict->valueForKey("para")->intValue();
            }
            if(autoDict->valueForKey("allId")){
                allId = autoDict->valueForKey("allId")->getCString();
            }
        }
        else if (type == TYPE_DRAGON_BUILD) {
            if (autoDict->objectForKey("pos")) {
                int pos = autoDict->valueForKey("pos")->intValue();
                int tk = FunBuildController::getInstance()->getBuildKey(itemId, pos);
                this->key = CC_ITOA(tk);
                DragonBuildingController::getInstance()->pushBuildQueue(autoDict);
            }
        }
    }
    else
    {
        this->key = "";
    }
}

QueueInfo::QueueInfo(const QueueInfo& info)
{
    this->qid = info.qid;
    this->type = info.type;
    this->itemId = info.itemId;
    this->canHelp = info.canHelp;
    this->endTime = info.endTime;
    this->finishTime = info.finishTime;
    this->startTime = info.startTime;
    this->key = info.key;
    this->uuid = info.uuid;
    this->para = info.para;
    this->allId = info.allId;
}

double QueueInfo::getTime() {
    if (type == TYPE_MARCH) {
        return WorldController::getInstance()->getTime();
    } else {
        return GlobalData::shared()->getWorldTime();
    }
}

