//
//  ReportCustomHeadPicCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-9-1.
//
//

#include "ReportCustomHeadPicCommand.h"

bool ReportCustomHeadPicCommand::handleRecieve(cocos2d::CCDictionary *dict){
    
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(REPORT_PICVER) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        return false;
    }
    
    return true;
}