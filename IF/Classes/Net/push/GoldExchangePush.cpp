//
//  GoldExchangePush.cpp
//  IF
//
//  Created by lifangkai on 14-9-11.
//
//

#include "GoldExchangePush.h"
#include "RechargeACTVCell.h"
void GoldExchangePush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    CCArray *items = dynamic_cast<CCArray*>(params->objectForKey("exchange"));
    CCObject *obj;
    CCARRAY_FOREACH(items, obj){
        
        CCDictionary* dict = _dict(obj);
        GoldExchangeItem* goldExchangeItem = new GoldExchangeItem(dict);
        map<string, GoldExchangeItem*>::iterator it =GlobalData::shared()->goldExchangeList.find(goldExchangeItem->id);
        if ( it!= GlobalData::shared()->goldExchangeList.end()){
            GoldExchangeItem* goldExchangeItemtemp =it->second;
            delete goldExchangeItemtemp;
            goldExchangeItemtemp = NULL;
        }
        GlobalData::shared()->goldExchangeList[goldExchangeItem->id] = goldExchangeItem;
    
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_LIST_CHANGE);
}