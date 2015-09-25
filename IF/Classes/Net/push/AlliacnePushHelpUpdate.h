//
//  AlliacnePushHelpUpdate.h
//  IF
//
//  Created by ganxiaohua on 14-7-17.
//
//

#ifndef __IF__AlliacnePushHelpUpdate__
#define __IF__AlliacnePushHelpUpdate__

#include "CommonInclude.h"

class AlliacnePushHelpUpdate{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class AlliacneMarkLinePush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__AlliacnePushHelpUpdate__) */
