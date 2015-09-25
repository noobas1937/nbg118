//
//  UseSkillCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-11-3.
//
//

#include "UseSkillCommand.h"
#include "GeneralManager.h"
#include "GlobalData.h"

bool UseSkillCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(USE_SKILL_COMMAND) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        callFail(NetResult::create());
    }else{
        map<std::string, SkillCDInfo*>::iterator it = GeneralManager::getInstance()->SkillCDMap.find(m_skillId);
        if(it != GeneralManager::getInstance()->SkillCDMap.end()){
            it->second->parse(params);
        }else{
            SkillCDInfo* cdInfo = SkillCDInfo::create();
            cdInfo->parse(params);
            cdInfo->retain();
            GeneralManager::getInstance()->SkillCDMap[m_skillId] = cdInfo;
        }
        auto effectState = _dict(params->objectForKey("effectState"));
        if(effectState){
            if(effectState->objectForKey(CC_ITOA(LORD_SKILL_PROTECTED))){
                auto protectTime = effectState->valueForKey(CC_ITOA(LORD_SKILL_PROTECTED))->doubleValue();
                GlobalData::shared()->playerInfo.resourceProtectTimeStamp = protectTime;
            }
        }
        
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}