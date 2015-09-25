//
//  WorldMarchPush.h
//  IF
//
//  Created by 邹 程 on 14-2-28.
//
//

#ifndef __IF__WorldMarchPush__
#define __IF__WorldMarchPush__

#include "cocos2d.h"

class WorldMarchPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldMarchWinPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldMarchLosePush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldMarchSpeedUpPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldMarchCancelPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldTeleportPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldAllianceAreaPush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDict);
};
class WolrdSuperMinePush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDict);
};
class WolrdSuperMineCancelPush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDict);
};
class WolrdTowerBuildPush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDic);
};
class WolrdTowerBuildCancelPush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDic);
};
class WolrdWareHousePush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDict);
};
class WolrdWareHouseCancelPush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDict);
};
class WorldWareHouseTakeBackPush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDict);
};
class WorldWareHouseStorePush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDict);
};
class WorldWareHouseRetrievePush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDict);
};
class WolrdBannerBuildPush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDic);
};
class WolrdBannerBuildCancelPush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDic);
};
class WolrdBannerBuildCrachPush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDic);
};
class WorldAllianceAreaCancelPush{
public:
    static void handleResponse(cocos2d::CCDictionary* pDict);
};

class WorldCrashPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldRefreshPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldRelicUpdatePush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldRelicFinishPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldTowerPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldProtectUpdatePush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldResourceProtectUpdatePush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldAddItemPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__WorldMarchPush__) */
