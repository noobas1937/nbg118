//
//  GeneralPush.cpp
//  IF
//
//  Created by 李锐奇 on 14-4-23.
//
//

#include "GeneralPush.h"
#include "CommonInclude.h"

void GeneralPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = _dict(dict->objectForKey("params"));
    CCArray *arr = dynamic_cast<CCArray*>(result->objectForKey("general"));
    if(arr){
        CCObject *obj;
        CCARRAY_FOREACH(arr, obj){
            auto dic = dynamic_cast<CCDictionary*>(obj);
            std::string uid = dic->valueForKey("uuid")->getCString();
            int level = dic->valueForKey("level")->intValue();
            int exp = dic->valueForKey("exp")->intValue();
            map<std::string, GeneralInfo>::iterator it = GlobalData::shared()->generals.find(uid);
            if(it != GlobalData::shared()->generals.end()){
                it->second.level = level;
                it->second.currExp = exp;
            }
        }
    }
    return;
}