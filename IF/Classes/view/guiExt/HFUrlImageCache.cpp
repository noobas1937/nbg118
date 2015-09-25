//
//  HFUrlImageCache.cpp
//  HappyFarm
//
//  Created by 舒 圣林 on 14-2-25.
//
//

#include "HFUrlImageCache.h"
#include "md5.h"
#include "../../Ext/CCDevice.h"
#include <sys/stat.h>

static HFUrlImageCache* s_sharedHFUrlImageCache = NULL;

HFUrlImageCache* HFUrlImageCache::shared(void)
{
	if (!s_sharedHFUrlImageCache)
	{
		s_sharedHFUrlImageCache = new HFUrlImageCache();
	}
	return s_sharedHFUrlImageCache;
}

void HFUrlImageCache::purge()
{
	CC_SAFE_RELEASE_NULL(s_sharedHFUrlImageCache);
}

bool HFUrlImageCache::isHeadImgExist(const string& url)
{
    return CCFileUtils::sharedFileUtils()->isFileExist(url);
}

void HFUrlImageCache::loadImageAsync(const string& url, CCCallFuncO* callback)
{
    CCAssert(callback != NULL, "loadImageAsync callback is null");
    if (callback == NULL) {
        return;
    }
    if (url.length() < 1) {
        callback->execute();
        return;
    }
//    CCLOGFUNCF("start load image: %s",url.c_str());
    CCString* textureFile = initWithCache(url);
    if (textureFile != NULL) {
        CCLOG("loadImageAsync textureFile != NULL");
        callback->setObject(textureFile);
        callback->execute();
        return;
    }
    HFUrlImage* loader = getLoader(url);
    if (loader == NULL) {
        loader = new HFUrlImage();
        m_imageLoaderCache[url] = loader;
        CCLOG("loader == NULL");
        loader->startLoadImageAsync(url.c_str(), CCCallFuncO::create(this, callfuncO_selector(HFUrlImageCache::imageLoadCompletedCallback), NULL));
    }
    CC_SAFE_RETAIN(callback);
    m_callbacks.insert(pair<string,CCCallFuncO*> (url, callback));
}

void HFUrlImageCache::imageLoadCompletedCallback(CCObject* object)
{
    CCLOG("imageLoadCompletedCallback");
    HFUrlImage* loader = dynamic_cast<HFUrlImage*>(object);
    if (loader == NULL) {
//        CCLOGFUNCF("image load competed, cast fail with %p",object);
        return;
    }
    doLoadCompletedCallback(loader->getImageUrl(), loader);
}

CCString* HFUrlImageCache::initWithCache(const string& url)
{
    if (m_cacheEnable == false) {
        return NULL;
    }
    string cacheFile = getCachePath(url);
    if (CCFileUtils::sharedFileUtils()->isFileExist(cacheFile) && isCacheExpired(cacheFile) == false) {
        CCString* result = CCString::create(cacheFile);
//        CCLOGFUNCF("get image cache for url %s: %s", url.c_str(),cacheFile.c_str());
        return result;
    }
    return NULL;
}

string HFUrlImageCache::getCachePath(const string& url)
{
    string file = CCFileUtils::sharedFileUtils()->getWritablePath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string path = cocos2d::extension::CCDevice::getSDcardPath();
    if (path.length() > 0)
    {
        file = path;
    }
#endif
    MD5 md5;
    md5.update(url.c_str(), url.length());
    file.append("cache_").append(md5.toString());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    file.append(".png");
#endif
//    CCLOGFUNCF("get cache file for url %s: %s",url.c_str(),file.c_str());
    return file;
}

bool HFUrlImageCache::isCacheExpired(const string& cachePath)
{
    struct stat cacheStat;
    lstat(cachePath.c_str(),&cacheStat);
    int now = (int) time(NULL);
    // cache three days
//    CCLOGFUNCF("now timestamp: %ld, file %s mtime: %ld", now, cachePath.c_str(), cacheStat.st_mtime);
    if (now - cacheStat.st_mtime > 86400 * 3) {
        return true;
    }
    return false;
}

HFUrlImage* HFUrlImageCache::getLoader(const string& url)
{
    ImageLoaderMap::iterator it = m_imageLoaderCache.find(url);
    if (it != m_imageLoaderCache.end()) {
        return it->second;
    }
    return NULL;
}
void HFUrlImageCache::cancelImageLoad(const string& url, CCObject* target)
{
    if (url.length() < 1) {
        return;
    }
//    CCLOGFUNCF("try to remove image load callback with target: %p url: %s match",target, url.c_str());
    const pair<CallbacksMap::iterator,CallbacksMap::iterator> &range = m_callbacks.equal_range(url);
    for (CallbacksMap::iterator it2 = range.first; it2 != range.second; ++it2) {
//        CCLOGFUNCF("get for loader %s : %p", it2->first.c_str(),it2->second->getTargetCallback());
        if (it2->second->getTargetCallback() == target) {
            it2->second->release();
            m_callbacks.erase(it2);
//            CCLOGFUNCF("remove image load callback with target: %p url: %s",target, url.c_str());
            return;
        }
    }
}
void HFUrlImageCache::doLoadCompletedCallback(const string& url, CCObject* object)
{
    CCLOG("doLoadCompletedCallback");
    const pair<CallbacksMap::const_iterator,CallbacksMap::const_iterator> &range = m_callbacks.equal_range(url);
    for (CallbacksMap::const_iterator it2 = range.first; it2 != range.second; ++it2) {
        CCCallFuncO* callback = it2->second;
        if (callback) {
            callback->setObject(object);
            callback->execute();
            callback->release();
        }
    }
    m_callbacks.erase(url);
    m_imageLoaderCache.erase(url);
}
void HFUrlImageCache::doLoadCompletedCallback(const string& url, HFUrlImage* loader)
{
    CCTexture2D* texture = NULL;
    if (loader != NULL) {
        string cacheFile = getCachePath(url);
        if (loader->getImageLoadStat() == kImageLoadOK) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            loader->saveToFile(cacheFile.c_str(),false);
#else
            loader->saveToFile(cacheFile.c_str(),false);
#endif
            doLoadCompletedCallback(url, CCString::create(cacheFile));
            loader->release();
            return;
        }else{
            // load image fail, release the loader
            loader->release();
        }
    }
    doLoadCompletedCallback(url, texture);
}

HFUrlImageCache::HFUrlImageCache():
m_cacheEnable(true)
{
	
}

HFUrlImageCache::~HFUrlImageCache()
{
	
}

void HFUrlImageCache::downHeadImage(const string &url)
{
    HFUrlImage* loader = getLoader(url);
    if (loader == NULL) {
        loader = new HFUrlImage();
        m_imageLoaderCache[url] = loader;
        CCLOG("loader == NULL");
        loader->startLoadImageAsync(url.c_str(), CCCallFuncO::create(this, callfuncO_selector(HFUrlImageCache::imageLoadCompletedCallback), NULL));
    }
}

