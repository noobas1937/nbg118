//
//  SearchUserAllianceCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-3.
//
//

#include "SearchUserAllianceCommand.h"
#include "NetController.h"
#include "AllianceInfo.h"
#include "AllianceManager.h"

SearchUserAllianceCommand::SearchUserAllianceCommand(std::string key,string lang,int page)
: CommandBase(SEARCH_USER),m_key(key),m_lang(lang)
{
    putParam("key", CCString::create(m_key.c_str()));
    putParam("lang", CCString::create(m_lang.c_str()));
    putParam("page", CCInteger::create(page));
}

bool SearchUserAllianceCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SEARCH_USER) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        callSuccess(NetResult::create(Error_OK, (CCObject*)params));
    }
    return true;
}