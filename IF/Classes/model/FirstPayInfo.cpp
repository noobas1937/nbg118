//
//  firstPayInfo.cpp
//  IF
//
//  Created by lifangkai on 14-10-17.
//
//

#include "firstPayInfo.h"

void FirstPayInfo::parse(CCDictionary *dict){

    rewardStatus = 0;
    firstPayReward = dynamic_cast<CCArray*>(dict->objectForKey("firstPayReward"));
    if(firstPayReward){
        firstPayReward->retain();
    }
}