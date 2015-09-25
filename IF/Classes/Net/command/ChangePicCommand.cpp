//
//  ChangePicCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-23.
//
//

#include "ChangePicCommand.h"
#include "UIComponent.h"

bool ChangePicCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PIC_CHANGE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
    }else{
        GlobalData::shared()->playerInfo.pic = m_pic;
        if (params->objectForKey("picVer"))
        {
            GlobalData::shared()->playerInfo.picVer = params->valueForKey("picVer")->intValue();
        }
        else
        {
            GlobalData::shared()->playerInfo.picVer += 1000000;
        }
        
        if(params->objectForKey("gold")){
            GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->intValue();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_PLAYER_PIC);
    }
    return true;
}