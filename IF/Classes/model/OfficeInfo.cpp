//
//  OfficeInfo.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-11.
//
//

#include "OfficeInfo.h"

void OfficeInfo::parse(CCDictionary *dict){
    if (dict==NULL) {
        return;
    }
    id = dict->valueForKey("id")->intValue();
    string dialogN = dict->valueForKey("name")->getCString();
    name = _lang(dialogN);
    string tempTips = dict->valueForKey("tips")->getCString();
    string tempParams = dict->valueForKey("param")->getCString();
    icon = dict->valueForKey("icon")->getCString();
    if (tempTips!="") {
        CCCommonUtils::splitString(tempTips,";",tips);
    }
    if (tempParams!="") {
        CCCommonUtils::splitString(tempParams,";",params);
    }
    if (dict->objectForKey("picVer"))
    {
        picVer = dict->valueForKey("picVer")->intValue();
    }
    if(dict->objectForKey("appointTime")){
        appointTime = dict->valueForKey("appointTime")->doubleValue() / 1000;
        appointTime = GlobalData::shared()->changeTime(appointTime);
    }
    isServant = id>216008;
}

string OfficeInfo::getPlayerIcon(){
    string pIcon = playerIcon + ".png";
    return pIcon;
}
