//
//  IFSkeletonDataManager.h
//  IF
//
//  Created by 舒 圣林 on 13-12-2.
//
//

#ifndef __IF__IFSkeletonDataManager__
#define __IF__IFSkeletonDataManager__

#include "CommonInclude.h"
#include <map>

using namespace std;
using namespace spine;

#define GET_SKELETON_DATA(SKELETON_NAME) IFSkeletonDataManager::shared()->getSkeletonData(SKELETON_NAME)
#define GET_SKELETON_DATA2(SKELETON_DATA_FILE,SKELETON_ATLAS_FILE,SCALE) IFSkeletonDataManager::shared()->getSkeletonData(SKELETON_DATA_FILE,SKELETON_ATLAS_FILE, SCALE)
#define PRELOAD_SKELETON(SKELETON_NAME) GET_SKELETON_DATA(SKELETON_NAME)

typedef std::map<string,spSkeletonData*> SkeletonDataMap;
typedef std::map<string,spAtlas*> AtlasMap;

class IFSkeletonDataManager : public CCObject
{
public:
    spSkeletonData* getSkeletonData(const string& skeletonNamePrefix, float scale = 1.0f);
    
    spSkeletonData* getSkeletonData(const char* skeletonDataFiel, const char* atlasFile, float scale = 1.0f);
    
private:
    SkeletonDataMap m_skeletonDataCache;
    AtlasMap m_atlasCache;
public:
	static IFSkeletonDataManager * shared();
	void purge();

private:
	IFSkeletonDataManager();
	virtual ~IFSkeletonDataManager();
	IFSkeletonDataManager(const IFSkeletonDataManager &);
    IFSkeletonDataManager &operator=(const IFSkeletonDataManager &);
};


#endif /* defined(__HappyFarm__IFSkeletonDataManager__) */
