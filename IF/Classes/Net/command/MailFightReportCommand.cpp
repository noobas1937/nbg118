//
//  MailFightReportCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-4.
//
//

#include "MailFightReportCommand.h"
#include "BattleManager.h"

MailFightReportCommand::MailFightReportCommand(std::string reportUid)
: CommandBase(BATTLE_FIGHT_REPORT),m_reportUid(reportUid)
{
    putParam("reportUid", CCString::create(reportUid.c_str()));
}

bool MailFightReportCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(BATTLE_FIGHT_REPORT) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        BattleManager::shared()->parseBattleReport(params);
        callSuccess(NetResult::create());
    }
    return true;
}