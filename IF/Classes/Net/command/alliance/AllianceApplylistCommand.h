//
//  AllianceApplylistCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-3.
//
//

#ifndef __IF__AllianceApplylistCommand__
#define __IF__AllianceApplylistCommand__

#include "CommandBase.h"

#define AL_APPLY_LIST "al.applylist"

class AllianceApplylistCommand: public CommandBase {
public:
    AllianceApplylistCommand(int page);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    int m_page;
};
#endif /* defined(__IF__AllianceApplylistCommand__) */
