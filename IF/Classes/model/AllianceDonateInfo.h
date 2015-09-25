//
//  AllianceDonateInfo.h
//  IF
//
//  Created by lifangkai on 14-9-2.
//
//

#ifndef __IF__AllianceDonateInfo__
#define __IF__AllianceDonateInfo__
#include "cocos2d.h"
//#include "PlayerInfo.h"
USING_NS_CC;
class AllianceDonateInfo:public CCNode{
public:
    static AllianceDonateInfo* create();
    AllianceDonateInfo():name(""),uid(""),donation(0),alliance_honor(0),x(0),y(0),refreshTime(0),picVer(0){};
    ~AllianceDonateInfo(){};
    void update(CCDictionary* dict,double time);

    std::string name;
    std::string uid;
    int donation;
    int alliance_honor;
    int x;
    int y;
    int rank;
    std::string pic;
    int picVer;
    double refreshTime;
    
};

#endif /* defined(__IF__AllianceDonateInfo__) */
