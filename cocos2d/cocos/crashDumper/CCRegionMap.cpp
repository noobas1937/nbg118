//
//  CCRegionMap.cpp
//  GameCore
//
//  Created by Chenfei on 13-11-4.
//
//

#include "CCRegionMap.h"
#include <unistd.h>

#define LOG_REGION_MAP 0

NS_CC_BEGIN

CCRegionMap* CCRegionMap::sRegionMapInstance = NULL;
CCRegionMap* CCRegionMap::sharedRegionMap()
{
    if(!sRegionMapInstance)
    {
        sRegionMapInstance = new CCRegionMap();
        
    }
    return sRegionMapInstance;
}

CCRegionMap::CCRegionMap()
{
}

void CCRegionMap::clear()
{
    this->mRegionInfoList.clear();
}

CCRegionMap::~CCRegionMap()
{
    clear();
}

uintptr_t CCRegionMap::getAddressOfLib(const char* libName)
{
    RegionInfoCache::iterator cacheIter = mCache.find(libName);
    if( cacheIter != mCache.end() )
    {
        return (*cacheIter).second;
    }

    RegionInfoList::iterator listIter = mRegionInfoList.begin();
    while(listIter != mRegionInfoList.end())
    {
        if(strstr((char*)(*listIter).name, libName))
        {
            mCache[(char*)(*listIter).name] = (*listIter).start;
            return (*listIter).start;
        }
        listIter++;
    }
    return 0;
}

uintptr_t CCRegionMap::getOffsetInRegionMappingByAddr(uintptr_t addr)
{
    const RegionInfo* regionInfo = getRegionInfoByAddr(addr);
    if(regionInfo)
    {
        return addr - regionInfo->start;
    }
    return addr;
}

void CCRegionMap::dump()
{
    RegionInfoList::iterator listIter = mRegionInfoList.begin();
    CCLOG("Begin to dump map information:");
    while(listIter != mRegionInfoList.end())
    {
        CCLOG("    [0x%08lX, 0x%08lX] %s", (*listIter).start, (*listIter).end, (char*)((*listIter).name));
    }
    CCLOG("End to dump map information:");
}

void CCRegionMap::refresh()
{
    clear();
    char data[1024] = {0};
    // 取得pid tid 调用ptract取得stack
    sprintf(data, "/proc/%d/maps", getpid());
#if LOG_REGION_MAP > 0
    CCLOG("Begin to dump map information:");
#endif
    FILE* fp = fopen(data, "r");
    if(fp)
    {
        RegionInfo curMapInfo;
        while(fgets(data, 1024, fp))
        {
            //07001000-07015000 r-xp 00001000 07:01 132        /system/bin/linker
            int len = strlen(data);
            if(len < 50)
            {
                continue;
            }
            // Skip the data segments
            if(data[20] != 'x')
            {
                continue;
            }
            curMapInfo.start = strtoul(data, 0, 16);
            curMapInfo.end = strtoul((char*)data + 9, 0, 16);
            strcpy((char*)curMapInfo.name, (char*)data + 49);
            // Remove the last line wrapper
            char* lineWrapper = strrchr((char*)curMapInfo.name, '\n');
            if(lineWrapper)
            {
                *lineWrapper = 0;
            }

            this->mRegionInfoList.push_back(curMapInfo);
#if LOG_REGION_MAP > 0
            CCLOG("    [0x%08lX, 0x%08lX] %s", curMapInfo.start, curMapInfo.end, (char*)curMapInfo.name);
#endif
        }
        fclose(fp);
    }
#if LOG_REGION_MAP > 0
    CCLOG("End to dump map information:");
#endif
}

const RegionInfo* CCRegionMap::getRegionInfoByAddr(uintptr_t addr)
{
    RegionInfoList::iterator iter = this->mRegionInfoList.begin();
    while( iter != mRegionInfoList.end() )
    {
        if( (*iter).start <= addr && addr <= (*iter).end )
        {
            return iter.base();
        }
        iter++;
    }
    return NULL;
}

std::string CCRegionMap::getAddressDesc(uintptr_t addr)
{
    char buffer[256] = {0};
    const RegionInfo* regionInfo = getRegionInfoByAddr(addr);
//    if(regionInfo)
//    {
//        sprintf(buffer, "0x%08lX  %s", (unsigned long)(addr - regionInfo->start), (char*)(regionInfo->name));
//    }
//    else
//    {
//        sprintf(buffer, "0x%08lX", (unsigned long)addr);
//    }
    if(regionInfo)
    {
        sprintf(buffer, "0x%08lX", (unsigned long)(addr - regionInfo->start));
    }
    else
    {
        sprintf(buffer, "0x%08lX", (unsigned long)addr);
    }
    return buffer;
}

NS_CC_END