//
//  SequenceResult.h
//  IF
//
//  Created by ganxiaohua on 14-3-13.
//
//

#ifndef __IF__SequenceResult__
#define __IF__SequenceResult__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTypesExt.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class SequenceResult : public cocos2d::CCObject {
public:
    SequenceResult():m_index(-1),m_type(std::string()),m_value(0){};
    ~SequenceResult(){};
    int m_index;
    std::string m_type;
    int m_value;
};
#endif /* defined(__IF__SequenceResult__) */
