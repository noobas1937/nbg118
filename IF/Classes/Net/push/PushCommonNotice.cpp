//
//  PushCommonNotice.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/21.
//
//

#include "PushCommonNotice.h"

void PushCommonNotice::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    string msgId = params->valueForKey("msgId")->getCString();
    CCArray* array = dynamic_cast<CCArray*>(params->objectForKey("para"));
    string tip = _lang(msgId);
    if (array && array->count()>0) {
        int num = array->count();
        CCString* temp = NULL;
        std::vector<string> vArr(num,"");
        for (int i=0; i<num; i++) {
            temp = dynamic_cast<CCString*>(array->objectAtIndex(i));
            if (temp) {
                vArr[i] = temp->getCString();
            }
        }
        switch (num) {
            case 1:
                tip = _lang_1(msgId,vArr[0].c_str());
                break;
            case 2:
                tip = _lang_2(msgId,vArr[0].c_str(),vArr[1].c_str());
                break;
            case 3:
                tip = _lang_3(msgId,vArr[0].c_str(),vArr[1].c_str(),vArr[2].c_str());
                break;
            case 4:
                tip = _lang_4(msgId,vArr[0].c_str(),vArr[1].c_str(),vArr[2].c_str(),vArr[3].c_str());
                break;
            case 5:
                tip = _lang_5(msgId,vArr[0].c_str(),vArr[1].c_str(),vArr[2].c_str(),vArr[3].c_str(),vArr[4].c_str());
                break;
        }
    }
    if (tip!="") {
        double worldTime = GlobalData::shared()->getWorldTime();
        worldTime *= 1000;
        worldTime = GlobalData::shared()->renewTime(worldTime);
        auto time = worldTime+1000*60;
        CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_SYSTEM);
    }
}