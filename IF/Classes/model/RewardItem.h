//
//  RewardItem.h
//  IF
//
//  Created by ganxiaohua on 13-11-27.
//
//

#ifndef __IF__RewardItem__
#define __IF__RewardItem__

#include "CommonInclude.h"

class RewardItem : public cocos2d::CCObject {
public:
    ~RewardItem(){};
    RewardItem():m_type(0),m_value(0),m_total(0){};
    std::string getItemName();
    
    CC_SYNTHESIZE(int,m_type,Type);
    CC_SYNTHESIZE(int,m_value,Value);
    CC_SYNTHESIZE(int,m_total,Total);
};
#endif /* defined(__IF__RewardItem__) */
