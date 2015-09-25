//
//  PlayerInfoPush.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-8.
//
//

#include "PlayerInfoPush.h"
#include "UIComponent.h"

void PlayerInfoPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    GlobalData::shared()->playerInfo.updateInfo(params);
    UIComponent::getInstance()->setUserData();
}