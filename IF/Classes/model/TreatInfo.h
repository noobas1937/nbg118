//
//  TreatInfo.h
//  IF
//
//  Created by 李锐奇 on 14-3-6.
//
//

#ifndef __IF__TreatInfo__
#define __IF__TreatInfo__

#include "cocos2d.h"

class TreatInfo : public cocos2d::CCObject{
public:
    TreatInfo(cocos2d::CCDictionary *dict);
    TreatInfo(){};
    int dead;
    int prepareToTreat;
    int heal;
    std::string armyId;
};

#endif /* defined(__IF__TreatInfo__) */
