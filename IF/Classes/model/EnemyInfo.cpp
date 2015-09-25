//
//  EnemyInfo.cpp
//  IF
//
//  Created by fubin on 14-2-24.
//
//

#include "EnemyInfo.h"
#include "WorldController.h"

EnemyInfo::EnemyInfo(CCDictionary* dict)
{
    uuid = dict->valueForKey("uuid")->getCString();
    uid = dict->valueForKey("uid")->getCString();
    stampTime = dict->valueForKey("arrivalTime")->doubleValue()/1000;
    arrivalTime = stampTime;
    if (arrivalTime>0) {
        arrivalTime = GlobalData::shared()->changeTime(arrivalTime);
    }
    if (dict->objectForKey("name") == NULL) {
        return;
    }
    createTime = dict->valueForKey("createTime")->doubleValue()/1000;
    if (createTime>0) {
        createTime = GlobalData::shared()->changeTime(createTime);
    }
    
    name = dict->valueForKey("name")->getCString();
    //出发地坐标
    int posId = dict->valueForKey("xy")->intValue();
    auto pt = WorldController::getPointByIndex(posId);
    index = posId;
    xy = " ";
    xy = xy + "x:" + CC_ITOA(pt.x) + ", y:"+CC_ITOA(pt.y);
    if (dict->valueForKey("attXy")->intValue()) {
        int fromPosId = dict->valueForKey("attXy")->intValue();
        auto fromPt = WorldController::getPointByIndex(fromPosId);
        indexFrom = fromPosId;
        xyFrom = " ";
        xyFrom = xyFrom + "x:" + CC_ITOA(fromPt.x) + ", y:"+CC_ITOA(fromPt.y);
    }else{
        indexFrom = posId;
        xyFrom = " ";
        xyFrom = xyFrom + "x:" + CC_ITOA(pt.x) + ", y:"+CC_ITOA(pt.y);
    }
    //目的地坐标
    if (dict->valueForKey("defXy")->intValue()) {
        int toPosId = dict->valueForKey("defXy")->intValue();
        auto toPt = WorldController::getPointByIndex(toPosId);
        indexTo = toPosId;
        xyTo = " ";
        xyTo = xyTo + "x:" + CC_ITOA(toPt.x) + ", y:"+CC_ITOA(toPt.y);
    }else{
        indexTo = posId;
        xyTo = " ";
        xyTo = xyTo + "x:" + CC_ITOA(pt.x) + ", y:"+CC_ITOA(pt.y);
    }
    
    //目的地类型
    if (dict->valueForKey("defType")->intValue()) {
        defType = dict->valueForKey("defType")->intValue();
    }
    
    type = dict->valueForKey("type")->intValue();
    level = dict->valueForKey("level")->intValue();
    soldiers = dict->valueForKey("totalTroops")->intValue();
    power = dict->valueForKey("power")->intValue();
    abbr = dict->valueForKey("abbr")->getCString();
    pic = dict->valueForKey("pic")->getCString();
    allianceId = dict->valueForKey("allianceId")->getCString();
    picVer = dict->valueForKey("picVer")->intValue();
    marchTime = dict->valueForKey("marchTime")->intValue()/1000;
    monsterCount = dict->valueForKey("monsterCount")->intValue();
    monsterArmyInfo = dict->valueForKey("monsterArmyInfo")->getCString();
    if (dict->objectForKey("monsterLevel")) {
        level = dict->valueForKey("monsterLevel")->intValue();
    }
    if(type==8){
        pic = "tile_pop_icon21";
    }
    teamId = dict->valueForKey("teamId")->getCString();
    CCDictionary *item = NULL;
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("soldiers"));
    if (arr)
    {
        for (int i=0; i<arr->count(); i++)
        {
            item = _dict(arr->objectAtIndex(i));
            int key = item->valueForKey("armyId")->intValue();
            int value = item->valueForKey("count")->intValue();
            army[key] = value;
        }
    }
    
    auto arr1 = dynamic_cast<CCArray*>(dict->objectForKey("generals"));
    if (arr1)
    {
        for (int i=0; i<arr1->count(); i++)
        {
            item = _dict(arr1->objectAtIndex(i));
            string key = item->valueForKey("genId")->getCString();
            int value = item->valueForKey("level")->intValue();
            hero[key] = value;
        }
    }
    
    auto arr2 = dynamic_cast<CCArray*>(dict->objectForKey("science"));
    if (arr2)
    {
        for (int i=0; i<arr2->count(); i++)
        {
            item = _dict(arr2->objectAtIndex(i));
            int key = item->valueForKey("id")->intValue();
            int value = item->valueForKey("level")->intValue();
            science[key] = value;
        }
    }
    
    auto arr3 = dynamic_cast<CCArray*>(dict->objectForKey("talent"));
    if (arr3)
    {
        for (int i=0; i<arr3->count(); i++)
        {
            item = _dict(arr3->objectAtIndex(i));
            int key = item->valueForKey("id")->intValue();
            int value = item->valueForKey("level")->intValue();
            talent[key] = value;
        }
    }
    
    auto arr4 = dynamic_cast<CCArray*>(dict->objectForKey("ability"));
    if (arr4)
    {
        for (int i=0; i<arr4->count(); i++)
        {
            item = _dict(arr4->objectAtIndex(i));
            int key = item->valueForKey("id")->intValue();
            ablityTypes.push_back(key);
        }
    }
    
    genDic = dict;
}