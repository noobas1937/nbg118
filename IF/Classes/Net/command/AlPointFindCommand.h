//
//  AlPointFindCommand.h
//  IF
//
//  Created by xxrdsg on 15-7-2.
//
//

#ifndef __IF__AlPointFindCommand__
#define __IF__AlPointFindCommand__

#include "CommandBase.h"

#define AL_POINT_FIND "al.point.find"

class AlPointFindCommand: public CommandBase
{
public:
    AlPointFindCommand(int op):CommandBase(AL_POINT_FIND), m_op(op)
    {
        putParam("op", CCInteger::create(op));
    }
protected:
    virtual bool handleRecieve(CCDictionary* dict);
private:
    int m_op;//0 盟主； 1 领地
};


#endif /* defined(__IF__AlPointFindCommand__) */
