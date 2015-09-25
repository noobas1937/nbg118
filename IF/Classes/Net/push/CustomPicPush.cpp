//
//  CustomPicPush.cpp
//  IF
//
//  Created by chenfubi on 15/4/27.
//
//

#include "CustomPicPush.h"

void CustomPicPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    if (params->objectForKey("picVer"))
    {
        string uid = params->valueForKey("uid")->getCString();
        if (uid != GlobalData::shared()->playerInfo.uid)
        {
            return;
        }
        int picVerI = params->valueForKey("picVer")->intValue();
        GlobalData::shared()->playerInfo.picVer = picVerI;
        if (picVerI > 3000000)
        {
            CCCommonUtils::flyHint("", "", _lang("4100005"));
        }
        else if (CCCommonUtils::isUseCustomPic(picVerI))
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_PLAYER_PIC);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("MSG_UPLOADIMAGE_BACK");
        }
    }
}