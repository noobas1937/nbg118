//
//  AllianceShopPush.cpp
//  IF
//
//  Created by lifangkai on 14-9-1.
//
//

#include "AllianceShopPush.h"
#include "ToolController.h"

void AllianceShopPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    string allianceId = params->valueForKey("allianceId")->getCString();
    if(GlobalData::shared()->playerInfo.allianceInfo.uid!=allianceId){
        return;
    }
    string goodsId = params->valueForKey("goodsId")->getCString();
    int count = params->valueForKey("num")->intValue();
    int alliancePoint = params->valueForKey("alliancePoint")->intValue();
    auto& toolInfo = ToolController::getInstance()->getToolInfoById(atoi(goodsId.c_str()));
    toolInfo.allianceCnt = count;
    GlobalData::shared()->playerInfo.allianceInfo.alliancepoint = alliancePoint;

}
void AlliancePointPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;

    int alliancePoint = params->valueForKey("alliancePoint")->intValue();
    GlobalData::shared()->playerInfo.allianceInfo.alliancepoint = alliancePoint;
    
}