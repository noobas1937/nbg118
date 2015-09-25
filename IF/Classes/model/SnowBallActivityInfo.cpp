//
//  SnowBallActivityInfo.cpp
//  IF
//
//  Created by lifangkai on 14-12-3.
//
//

#include "SnowBallActivityInfo.h"

void SnowBallActivityInfo::parse(CCDictionary *dict){
    
    //    int num;
    //    int hint;
    //    int resource;
    refreshTime = dict->valueForKey("refreshTime")->doubleValue() / 1000;
    if(dict->objectForKey("ballCount")){
        ballCount = dict->valueForKey("ballCount")->intValue();
    }
    if(dict->objectForKey("maxThrowCount")){
        maxThrowCount = dict->valueForKey("maxThrowCount")->intValue();
    }
    if(dict->objectForKey("todayThrowCount")){
        todayThrowCount = dict->valueForKey("todayThrowCount")->intValue();
    }
    if(dict->objectForKey("goldArr")){
        CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("goldArr"));
        int count = arr->count();
        count = count>10?10:count;
        int i=0;
        while (i<count) {
            goldArr[i]=dynamic_cast<CCString*>(arr->objectAtIndex(i))->intValue();
            i++;
        }
    }
    if(dict->objectForKey("expArr")){
        CCArray* exparr = dynamic_cast<CCArray*>(dict->objectForKey("expArr"));
        int count = exparr->count();
        count = count>10?10:count;
        int j=0;
        while (j<count) {
            goldArr[j]=dynamic_cast<CCString*>(exparr->objectAtIndex(j))->intValue();
            j++;
        }
    }

}