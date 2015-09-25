//
//  CancelAllianceApplyCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-2.
//
//

#ifndef __IF__CancelAllianceApplyCommand__
#define __IF__CancelAllianceApplyCommand__

#include "CommandBase.h"

#define CANCEL_APPLY "al.cancelapply"

class CancelAllianceApplyCommand: public CommandBase {
public:
    CancelAllianceApplyCommand(std::string allianceId);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_allianceId;
};
#endif /* defined(__IF__CancelAllianceApplyCommand__) */
