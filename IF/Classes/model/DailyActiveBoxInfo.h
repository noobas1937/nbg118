//
//  DailyActiveBoxInfo.h
//  IF
//
//  Created by chenfubi on 15/4/10.
//
//

#ifndef __IF__DailyActiveBoxInfo__
#define __IF__DailyActiveBoxInfo__

#include "CommonInclude.h"

enum boxState
{
    BOX_UN_COMPLETE,    //score is not enough, can't get reward
    BOX_COMPLETE,       //score is enough, can get reward
    BOX_GET_REWARD,     //has get reward
};

class DailyActiveBoxInfo : public CCObject
{
public:
    DailyActiveBoxInfo():state(0),point(0){ reward=CCArray::create(); }
    ~DailyActiveBoxInfo(){};
    
    void initData(string itemId);
    void initData(CCDictionary* dict);
    
    void updateData(CCDictionary* dict);
    
    int state;
    int point;
    CCSafeObject<CCArray> reward;
};

#endif /* defined(__IF__DailyActiveBoxInfo__) */
