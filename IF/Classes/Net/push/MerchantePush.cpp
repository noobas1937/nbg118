#include "MerchantePush.h"
#include "ToolController.h"

void MerchantePush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    ToolController::getInstance()->initMerchanteInfo(params);
}