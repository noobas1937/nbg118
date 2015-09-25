//
//  GCMRewardInfo.h
//  IF
//
//  Created by fubin on 13-12-25.
//
//

#ifndef IF_GCMRewardInfo_h
#define IF_GCMRewardInfo_h

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class GCMRewardInfo{
    
public:
    GCMRewardInfo(CCDictionary* dict);
    GCMRewardInfo():
    actId(""),
    title(""),
    contents(""),
    reward(""){}
    
    string actId;
    string title;
    string contents;
    string reward;
};
#endif /* defined(__IF__GCMRewardInfo__) */
