//
//  SkillHurtResult.h
//  IF
//
//  Created by ganxiaohua on 14-5-28.
//
//

#ifndef __IF__SkillHurtResult__
#define __IF__SkillHurtResult__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class SkillHurtResult : public cocos2d::CCObject {
public:
    SkillHurtResult(int side,int armyType,int position,std::string type,int value):m_side(side)
    ,m_armyType(armyType)
    ,m_position(position)
    ,m_type(type)
    ,m_value(value)
    {};
    
    int m_side;
    int m_armyType;
    int m_position;
    std::string m_type;
    int m_value;
protected:
    
};
#endif /* defined(__IF__SkillHurtResult__) */
