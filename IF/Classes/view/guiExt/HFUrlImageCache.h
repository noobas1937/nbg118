//
//  HFUrlImageCache.h
//  HappyFarm
//
//  Created by 舒 圣林 on 14-2-25.
//
//

#ifndef __HappyFarm__HFUrlImageCache__
#define __HappyFarm__HFUrlImageCache__

#include "CommonInclude.h"
#include "HFUrlImage.h"

class HFUrlImageCache : public CCObject
{
public:
    typedef std::multimap<string, CCCallFuncO*> CallbacksMap;
    typedef std::map<string, HFUrlImage*> ImageLoaderMap;
	static HFUrlImageCache * shared();
	void purge();
    void loadImageAsync(const string& url, CCCallFuncO* callback);
    void cancelImageLoad(const string& url, CCObject* target);
    string getCachePath(const string& url);
    bool isHeadImgExist(const string& url);
    void downHeadImage(const string& url);
protected:
    bool isCacheExpired(const string& cachePath);
    CCString* initWithCache(const string& url);
    
    HFUrlImage* getLoader(const string& url);
    void doLoadCompletedCallback(const string& url, HFUrlImage* loader);
    void doLoadCompletedCallback(const string& url, CCObject* texture);
    void imageLoadCompletedCallback(CCObject* object);
    bool m_cacheEnable;
private:
    ImageLoaderMap m_imageLoaderCache;
    CallbacksMap m_callbacks;
private:
	HFUrlImageCache();
	virtual ~HFUrlImageCache();
	HFUrlImageCache(const HFUrlImageCache &);
    HFUrlImageCache &operator=(const HFUrlImageCache &);
};

#endif /* defined(__HappyFarm__HFUrlImageCache__) */
