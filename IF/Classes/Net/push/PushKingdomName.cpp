//
//  PushKingdomName.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-15.
//
//

#include "PushKingdomName.h"

void PushKingdomName::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    string kingName = params->valueForKey("kingName")->getCString();//国王的名字
    string name = params->valueForKey("name")->getCString();//王国的名字
    string serverId = "Kingdom#";
    int sid = params->valueForKey("serverId")->intValue();
    serverId += params->valueForKey("serverId")->getCString();
    
    double worldTime = GlobalData::shared()->getWorldTime();
    worldTime *= 1000;
    worldTime = GlobalData::shared()->renewTime(worldTime);
    auto time = worldTime+1000*60;
    double curTime = GlobalData::shared()->getWorldTime();
    auto m_countDown = GlobalData::shared()->changeTime(time/1000);
    auto m_time = (long)floor( m_countDown - curTime );
    string delayTime = CC_ITOA(m_time/60);
    string tip = "";
    if(sid==GlobalData::shared()->playerInfo.selfServerId){
        tip = _lang_2("110011", kingName.c_str(),name.c_str());
    }else{
       tip = CCString::createWithFormat("%s",_lang_3("110013", kingName.c_str(),serverId.c_str(),name.c_str()))->getCString();
    }
    CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_KING);
}