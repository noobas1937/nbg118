//
//  CCRegionMap.h
//  GameCore
//
//  Created by Chenfei on 13-11-4.
//
//

#ifndef __GameCore__CCRegionMap__
#define __GameCore__CCRegionMap__

#include "cocos2d.h"

NS_CC_BEGIN

typedef struct _RegionInfo
{
    uintptr_t start;
    uintptr_t end;
    uintptr_t exidx_start;
    uintptr_t exidx_end;
    char* name[49];
} RegionInfo;

typedef std::vector<RegionInfo> RegionInfoList;
typedef std::map<const char*,uintptr_t> RegionInfoCache;
class CCRegionMap
{
public:
    static CCRegionMap* sharedRegionMap();
    virtual ~CCRegionMap();
    uintptr_t getAddressOfLib(const char* libName);
    uintptr_t getOffsetInRegionMappingByAddr(uintptr_t addr);
    const RegionInfo* getRegionInfoByAddr(uintptr_t addr);
    std::string getAddressDesc(uintptr_t addr);
    inline void clear();
    void dump();
    void refresh();
protected:
    CCRegionMap();
    CC_SYNTHESIZE_READONLY(RegionInfoList, mRegionInfoList, RegionInfoList);
    RegionInfoCache mCache;
    static CCRegionMap* sRegionMapInstance;
};

NS_CC_END

#endif /* defined(__GameCore__CCRegionMap__) */
