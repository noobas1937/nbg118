//
//  ItemPush.h
//  IF
//
//  Created by fubin on 14-4-24.
//
//

#ifndef IF_ItemPush_h
#define IF_ItemPush_h

#include "cocos2d.h"

class ItemAddPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class ItemDelPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class ItemUsePush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif
