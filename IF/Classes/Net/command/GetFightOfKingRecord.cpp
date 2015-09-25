//
//  GetFightOfKingRecord.cpp
//  IF
//
//  Created by ganxiaohua on 15/1/29.
//
//

#include "GetFightOfKingRecord.h"

bool GetFightOfKingRecord::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_FIGHT_OF_KING) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        callFail(NetResult::create(Error_OK, params));
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}