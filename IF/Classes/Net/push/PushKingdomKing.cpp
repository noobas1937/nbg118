//
//  PushKingdomKing.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-15.
//
//

#include "PushKingdomKing.h"
#include "WorldController.h"

void PushKingdomKing::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    // 2 3
    string kingName = params->valueForKey("kingName")->getCString();
    string kingdomName = params->valueForKey("kingdomName")->getCString();
    string allianceName = params->valueForKey("allianceName")->getCString();
    string kingOldName = params->valueForKey("kingOldName")->getCString();
    std::string allianceIcon = params->valueForKey("allianceIcon")->getCString();
    int serverId = params->valueForKey("serverId")->intValue();
    
    for(auto it = WorldController::getInstance()->m_serverList.begin(); it != WorldController::getInstance()->m_serverList.end(); it++){
        if(it->serverId == serverId){
            it->serverName = kingdomName;
//            it->allianceName = allianceName;
//            it->allianceFlag = allianceIcon;
            it->kingName = kingName;
        }
    }
    
    double worldTime = GlobalData::shared()->getWorldTime();
    worldTime *= 1000;
    worldTime = GlobalData::shared()->renewTime(worldTime);
    auto time = worldTime+1000*60;
    double curTime = GlobalData::shared()->getWorldTime();
    auto m_countDown = GlobalData::shared()->changeTime(time/1000);
    auto m_time = (long)floor( m_countDown - curTime );
    string delayTime = CC_ITOA(m_time/60);
    string tip = "";
    if(serverId!=GlobalData::shared()->playerInfo.selfServerId){//跨服公告 1
        tip = _lang_2("110012",kingName.c_str(),kingdomName.c_str());
    }else if(allianceName!=""){//4 国王确定时 联盟
        tip = _lang_2("110009", allianceName.c_str(), kingName.c_str());
    }else{//4 国王确定时 没有联盟
        tip = _lang_1("110010",kingName.c_str());
    }
    CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_KING);
    
    // 4 最后都迁到这push.kingdom.officer.add，
}