//
//  GetAllianceCommentsCommand.h
//  IF
//
//  Created by chenliang on 14-4-11.
//
//

#ifndef __IF__GetAllianceCommentsCommand__
#define __IF__GetAllianceCommentsCommand__

#include "CommandBase.h"

class GetAllianceCommentsCommand: public CommandBase {
public:
    GetAllianceCommentsCommand(std::string allianceId,int page,std::string lang);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};



#endif /* defined(__IF__GetAllianceCommentsCommand__) */
