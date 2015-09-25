//
//  MailFightReportCommand.h
//  IF
//
//  Created by ganxiaohua on 14-6-4.
//
//

#ifndef __IF__MailFightReportCommand__
#define __IF__MailFightReportCommand__

#include "CommandBase.h"

#define BATTLE_FIGHT_REPORT "fight.report" 

class MailFightReportCommand: public CommandBase {
public:
    MailFightReportCommand(std::string reportUid);
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_reportUid;
};
#endif /* defined(__IF__MailFightReportCommand__) */
