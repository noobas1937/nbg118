//
//  checkAllianceRestrictcommamd.h
//  IF
//
//  Created by 李朝辉 on 15-8-11.
//
//

#ifndef __IF__checkAllianceRestrictcommamd__
#define __IF__checkAllianceRestrictcommamd__

#include "CommandBase.h"

class checkAllianceRestrictcommamd: public CommandBase {
public:
    checkAllianceRestrictcommamd();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__checkAllianceRestrictcommamd__) */
