//
//  MemberInfo.h
//  IF
//
//  Created by chenliang on 14-4-4.
//
//

#ifndef __IF__MemberInfo__
#define __IF__MemberInfo__

#include "CommonInclude.h"

class MemberInfo
{
public:
    MemberInfo(string playerId, int value, int level, string name)
    :m_playerId(playerId),
    m_value(value),
    m_level(level),
    m_name(name)
    {}
    CC_SYNTHESIZE(string, m_playerId, PlayerId);
    CC_SYNTHESIZE(int, m_value, Value);
    CC_SYNTHESIZE(int, m_level, Level);
    CC_SYNTHESIZE(string, m_name, Name);
};

#endif /* defined(__IF__MemberInfo__) */
