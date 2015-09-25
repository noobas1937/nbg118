//
//  FunBuildInfo.cpp
//  IF
//
//  Created by fubin on 13-11-1.
//
//

#include "FunBuildInfo.h"
#include "FunBuildController.h"
#include "DragonBuildingController.h"
#include "CCCommonUtils.h"

FunBuildInfo::FunBuildInfo(CCDictionary* dict)
{
    SetInfoFromXml(dict);
}

void FunBuildInfo::SetInfoFromXml(CCDictionary* dict)
{
//    <ItemSpec id="402000" name="102002" description="102027" pic="pic406000" stone="100" food="100" silver="100" time="100" num="1"/>
    
    this->itemId = dict->valueForKey("id")->intValue();
    this->name = dict->valueForKey("name")->getCString();
    this->type = dict->valueForKey("id")->intValue();
//    this->plevel = dict->valueForKey("plevel")->intValue();
    
    this->exp = dict->valueForKey("exp")->intValue();
    
    std::string powerStr = dict->valueForKey("power")->getCString();
    if(powerStr!=""){
        std::vector<std::string> levelArr;
        CCCommonUtils::splitString(powerStr,";",levelArr);
        int num = levelArr.size();
        int last = 0;
        for(int i=0;i<num;i++){
            int levelPower = atoi(levelArr[i].c_str());
            int addPower = levelPower - last;
            addPower = MAX(addPower, 0);
            mapPower[i+1] = addPower;
            last = levelPower;
        }
    }
    
//    this->power = dict->valueForKey("power")->intValue();
    this->putId = dict->valueForKey("putId")->getCString();
    this->dragonGold_need = dict->valueForKey("resource")->intValue();
    this->silver_need = dict->valueForKey("silver")->intValue();
    this->wood_need = dict->valueForKey("wood")->intValue();
    this->time_need = dict->valueForKey("time")->intValue();
    this->stone_need = dict->valueForKey("stone")->intValue();
    this->limitNum = dict->valueForKey("num")->intValue();
    this->food_need = dict->valueForKey("food")->intValue();
    this->iron_need = dict->valueForKey("iron")->intValue();
    this->building = dict->valueForKey("building")->getCString();
    this->tool_need = dict->valueForKey("item")->getCString();
    
    this->description = dict->valueForKey("description")->getCString();
    this->destip = dict->valueForKey("destip")->getCString();
    this->tip = dict->valueForKey("tip")->getCString();
    this->information = dict->valueForKey("information")->getCString();
//    this->deno = dict->valueForKey("deno")->getCString();
//    this->introduce = dict->valueForKey("introduce")->getCString();
//    this->icon = dict->valueForKey("icon")->getCString();
//    this->button_txt = dict->valueForKey("button_text")->getCString();
    this->pic = dict->valueForKey("pic")->getCString();
    
    this->open = dict->valueForKey("open")->intValue();
    this->open_arms = dict->valueForKey("open_arms")->getCString();
    
    picVec.clear();
    string picOrder = dict->valueForKey("pic_order")->getCString();
    CCCommonUtils::splitString(picOrder, ";", picVec);
    
    this->level = 0;
    this->isError = 0;
    this->updateTime = 0;
    this->starNum = 0;
    this->starRate = 0;
    this->para1 = 0;
    this->para2 = 0;
    this->para3 = 0;
    this->para4 = 0;
    this->para5 = 0;
    this->refreshTime = 0;
    this->des_time = 0;
    this->effectTime = 0;
    this->oldExp = 0;
    
    this->nextLevelParas = "";
    this->is_Dirc = false;
}

void FunBuildInfo::SetInfoFromServer(CCDictionary* dict)
{
//    (utf_string) itemId: 409000
//    (int) pos: 0
//    (long) updateTime: 1392642147628
//    (int) time: 60
//    (int) stone: 1
//    (int) food: 4
//    (int) level: 9
//    (utf_string) building: 413010
//    (int) silver: 5
//    (int) wood: 2
//    (int) iron: 3
//    (utf_string) uuid: c15cddaf0b8940dc990c207f2a190007
    
    this->level = dict->valueForKey("level")->intValue();
    this->exp = dict->valueForKey("exp")->intValue();
//    this->power = dict->valueForKey("power")->intValue();
    this->putId = dict->valueForKey("putId")->getCString();
    this->dragonGold_need = dict->valueForKey("resource")->intValue();
    this->silver_need = dict->valueForKey("silver")->intValue();
    this->wood_need = dict->valueForKey("wood")->intValue();
    this->time_need = dict->valueForKey("time")->intValue();
    this->stone_need = dict->valueForKey("stone")->intValue();
    this->food_need = dict->valueForKey("food")->intValue();
    this->iron_need = dict->valueForKey("iron")->intValue();
    this->tool_need = dict->valueForKey("item")->getCString();
    this->building = dict->valueForKey("building")->getCString();
    this->plevel = 9999;//dict->valueForKey("plevel")->intValue();
    this->uuid = dict->valueForKey("uuid")->getCString();
    this->pos = dict->valueForKey("pos")->intValue();
    this->des_time = dict->valueForKey("destroy_time")->intValue();
    this->severST = dict->valueForKey("opType")->intValue();
    
//    CCLOG("level=%d power=%d",level,power);
    
    auto tmp = dict->valueForKey("updateTime")->doubleValue()/1000;
    if (tmp == 0) {
        this->updateTime = 0;
    }
    else{
        this->updateTime = GlobalData::shared()->changeTime(tmp);
    }
    auto tempTime = dict->valueForKey("effectTime")->doubleValue()/1000;
    if(tempTime==0){
        this->effectTime = 0;
    }else{
        this->effectTime = GlobalData::shared()->changeTime(tempTime);
    }
    
    this->starNum = dict->valueForKey("starNum")->intValue();
    this->starRate = dict->valueForKey("starRate")->floatValue();
    
    this->para1 = dict->valueForKey("para1")->intValue();
    this->para2 = dict->valueForKey("para2")->intValue();
    this->para3 = dict->valueForKey("para3")->intValue();
    this->para4 = dict->valueForKey("para4")->floatValue();
    this->para5 = dict->valueForKey("para5")->floatValue();
    this->nextLevelParas = dict->valueForKey("nextLevelParas")->getCString();
    
    auto ref = dict->valueForKey("refreshTime")->doubleValue()/1000;
    if (ref == 0) {
        this->refreshTime = 0;
    }
    else{
        this->refreshTime = GlobalData::shared()->changeTime(ref);
    }
    
    auto effTime = dict->valueForKey("effectTime")->doubleValue()/1000;
    if (effTime == 0) {
        this->effectTime = 0;
    }
    else{
        this->effectTime = GlobalData::shared()->changeTime(effTime);
    }
    
    if (dict->objectForKey("nextLevelInfo")) {
        CCDictionary* nextDic = _dict(dict->objectForKey("nextLevelInfo"));
        if (nextDic!=NULL) {
            FunBuildController::getInstance()->initSetUpBuildingInfo(nextDic);
        }
    }
    
    for (int i=1; ; i++) {
        string key = "para";
        key = key+CC_ITOA(i);
        if (dict->objectForKey(key)) {
            this->para[i] = dict->valueForKey(key)->getCString();
        }
        else{
            break;
        }
    }
    if(this->type== FUN_BUILD_MAIN) {
        CCUserDefault::sharedUserDefault()->setIntegerForKey(FUN_BUILD_MAIN_LEVEL, this->level);
        CCUserDefault::sharedUserDefault()->flush();
    }
}

FunBuildInfo::FunBuildInfo(FunBuildInfo* oldInfo)
{
    this->itemId = oldInfo->itemId;
    this->name = oldInfo->name;
    this->type = oldInfo->type;
    this->state = oldInfo->state;
    this->is_Dirc = oldInfo->is_Dirc;
    this->isError = oldInfo->isError;
    this->oldExp = oldInfo->oldExp;
    
    this->exp = oldInfo->exp;
    for(int i=1; ;i++) {
        if (oldInfo->mapPower.find(i) != oldInfo->mapPower.end()) {
            this->mapPower[i] = oldInfo->mapPower[i];
        }else {
            break;
        }
    }
    
    this->putId = oldInfo->putId;
    this->dragonGold_need = oldInfo->dragonGold_need;
    this->silver_need = oldInfo->silver_need;
    this->wood_need = oldInfo->wood_need;
    this->time_need = oldInfo->time_need;
    this->stone_need = oldInfo->stone_need;
    this->limitNum = oldInfo->limitNum;
    this->food_need = oldInfo->food_need;
    this->iron_need = oldInfo->iron_need;
    this->building = oldInfo->building;
    this->tool_need = oldInfo->tool_need;
    
    this->description = oldInfo->description;
    this->destip = oldInfo->destip;
    this->tip = oldInfo->tip;
    this->information = oldInfo->information;
    this->pic = oldInfo->pic;
    
    this->open = oldInfo->open;
    this->open_arms = oldInfo->open_arms;
    this->picVec = oldInfo->picVec;
    
    this->level = oldInfo->level;
    this->plevel = oldInfo->plevel;
    this->uuid = oldInfo->uuid;
    this->pos = oldInfo->pos;
    this->des_time = oldInfo->des_time;
    this->severST = oldInfo->severST;
    
    this->updateTime = oldInfo->updateTime;
    this->effectTime = oldInfo->effectTime;
    this->refreshTime = oldInfo->refreshTime;
    
    this->starRate = oldInfo->starRate;
    this->starNum = oldInfo->starNum;
    this->para1 = oldInfo->para1;
    this->para2 = oldInfo->para2;
    this->para3 = oldInfo->para3;
    this->para4 = oldInfo->para4;
    this->para5 = oldInfo->para5;
    this->nextLevelParas = oldInfo->nextLevelParas;
    
    for(int j=1; ;j++) {
        if (oldInfo->para.find(j) != oldInfo->para.end()) {
            this->para[j] = oldInfo->para[j];
        }else {
            break;
        }
    }
    if(this->level== FUN_BUILD_MAIN) {
        CCUserDefault::sharedUserDefault()->setIntegerForKey(FUN_BUILD_MAIN_LEVEL, this->level);
        CCUserDefault::sharedUserDefault()->flush();
    }
}

bool FunBuildInfo::isUnLock()
{
//    if(this->plevel == 0)
//    {
//        return true;
//    }
//    else
//    {
//        map<int, FunBuildInfo>* tmpData = FunBuildController::getInstance()->curBuildsInfo;
//        if (tmpData->find(FUN_BUILD_MAIN) != tmpData->end()) {
//            auto &info = tmpData->at(FUN_BUILD_MAIN);
//            if(this->plevel <= info.level){
//                return true;
//            }
//            else {
//                return false;
//            }
//        }
//        else {
//            return false;
//        }
//    }
    
    bool ret = true;
    std::vector<std::string> lockItems;
    if(this->building != "") {
        CCCommonUtils::splitString(this->building, "|", lockItems);
        
        if (type == DBUILDING_HATCHER || type == DBUILDING_CAVE || type == DBUILDING_FARM || type == DBUILDING_HABITAT_1 || type == DBUILDING_HABITAT_2 || type == DBUILDING_HABITAT_3 || type == DBUILDING_HABITAT_4 || type == DBUILDING_HABITAT_5 || type == DBUILDING_HABITAT_6 || type == DBUILDING_HABITAT_7) {
            for(int i=0; i<lockItems.size(); i++)
            {
                std::vector<std::string> tinyItems;
                CCCommonUtils::splitString(lockItems[i], ";", tinyItems);
                int tmpType = atoi(tinyItems[0].c_str());
                int tmLv = atoi(tinyItems[1].c_str());
                if(!DragonBuildingController::getInstance()->isExistBuildByTypeLv(tmpType, tmLv) && !FunBuildController::getInstance()->isExistBuildByTypeLv(tmpType, tmLv))
                {
                    ret = false;
                    break;
                }
            }
//            if (ret == true && type != DBUILDING_HATCHER && type != DBUILDING_CAVE) {
//                int limit = DragonBuildingController::getInstance()->getLimitNum(type);
//                int nowCnt = DragonBuildingController::getInstance()->getBuildNumByType(type);
//                if (nowCnt >= limit) {
//                    ret = false;
//                }
//            }
        } else {
            for(int i=0; i<lockItems.size(); i++)
            {
                std::vector<std::string> tinyItems;
                CCCommonUtils::splitString(lockItems[i], ";", tinyItems);
                int tmpType = atoi(tinyItems[0].c_str());
                int tmLv = atoi(tinyItems[1].c_str());
                if(!FunBuildController::getInstance()->isExistBuildByTypeLv(tmpType, tmLv))
                {
                    ret = false;
                    break;
                }
            }
        }
    }
    return ret;
}

string FunBuildInfo::getInfomationByLv(int lv)
{
    string ret = "";
    
    int state = 0;
    string resInfo = information;
    std::vector<std::string> lineItems;
    CCCommonUtils::splitString(resInfo, "|", lineItems);
    for (int i=0; i<lineItems.size(); i++)
    {
        string lineInfo = lineItems[i];
        std::vector<std::string> cellItems;
        CCCommonUtils::splitString(lineInfo, ";", cellItems);
        
        for (int j=0; j<cellItems.size(); j++)
        {
            string cellInfo = cellItems[j];
            std::vector<std::string> tinyItems;
            CCCommonUtils::splitString(cellInfo, ",", tinyItems);
            if (j==0 && tinyItems.size()==2) {
                int tmpLv = atoi(tinyItems[1].c_str());
                if (tmpLv == lv) {
                    state = 1;
                }
                else {
                    break;
                }
            }
            else if(state == 1){
                string cellMsg = "";
                if (tinyItems.size()==1)
                {
                    cellMsg = _lang(tinyItems[0]);
                }
                else if(tinyItems.size()==2)
                {
                    cellMsg = _lang_1(tinyItems[0],CC_CMDITOA(atoi(tinyItems[1].c_str())).c_str()  );
                }
                else if(tinyItems.size()==3)
                {
                    cellMsg = _lang_2(tinyItems[0],CC_CMDITOA(atoi(tinyItems[1].c_str())).c_str() ,CC_CMDITOA(atoi(tinyItems[2].c_str())).c_str());
                }
                ret = cellMsg;
            }
            else {
                break;
            }
        }
    }
    
    return ret;
}
