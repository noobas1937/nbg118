//
//  AllianceDismissPush.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-21.
//
//

#include "AllianceDismissPush.h"
#include "YesNoDialog.h"

void AllianceDismissPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    GlobalData::shared()->playerInfo.setAllianceId("");
    GlobalData::shared()->playerInfo.allianceInfo.reset();
    YesNoDialog::showConfirmCloseAllView(_lang("115097").c_str(), _lang("confirm").c_str());
}