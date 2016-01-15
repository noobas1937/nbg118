//
//  CCLoadSprite.cpp
//  DragonEmpire
//
//  Created by 邹 程 on 13-8-22.
//
//

#include "CCLoadSprite.h"
#include "SceneController.h"
#include "DynamicResourceController.h"
#include "NBDLCController.hpp"
#include <map>

static std::map<std::string, bool> map_NBDLCController;
static std::map<std::string, int> MAP_TEXTURES_MANAGER;

void CCLoadSprite::retain_texture(const std::string& texture)
{
    auto it = MAP_TEXTURES_MANAGER.find(texture);
    if (it == MAP_TEXTURES_MANAGER.end())
    {
        MAP_TEXTURES_MANAGER[texture] = 1;
    }
    else
    {
        MAP_TEXTURES_MANAGER[texture] += 1;
    }
    
    CCLog("CCLoadSprite::retain_texture (%s, %d)", texture.c_str(), MAP_TEXTURES_MANAGER[texture]);
}

bool CCLoadSprite::release_texture(const std::string& texture)
{
    auto it = MAP_TEXTURES_MANAGER.find(texture);
    if (it == MAP_TEXTURES_MANAGER.end())
    {
        MAP_TEXTURES_MANAGER[texture] = 0;
    }
    else
    {
        MAP_TEXTURES_MANAGER[texture] -= 1;
    }
    
    CCLog("CCLoadSprite::release_texture (%s, %d)", texture.c_str(), MAP_TEXTURES_MANAGER[texture]);
    
    return MAP_TEXTURES_MANAGER[texture] <= 0;
}

#define CC_2x2_WHITE_IMAGE_KEY  "cc_2x2_white_image"
static unsigned char cc_2x2_white_image[] = {
    // RGBA8888
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
};

#define MEM_LIMIT 40

#define COMMON_COUNT 3
#define WORLD_COUNT 4

USING_NS_CC;

void CCLoadSprite::init()
{
    map_NBDLCController.clear();
}

void CCLoadSprite::doLoadCommonResourceAsync(){
//    CCLog("android_test_doLoadCommonResourceAsync");
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(DynamicResourceController::getInstance()->getInitedCommon()){
        DynamicResourceController::getInstance()->downLoadAllCommon();
    }else{
        DynamicResourceController::getInstance()->sendGetCommonCMD();
    }
//#endif
}
cocos2d::CCSprite* CCLoadSprite::createSprite(const char *fileName) {
    cocos2d::CCSpriteFrame *test = loadResource(fileName);
    if (!test) {
        return CCSprite::create();
    }
    return cocos2d::CCSprite::createWithSpriteFrame(test);
}
cocos2d::CCSprite* CCLoadSprite::createSprite(const char* fileName, bool useDefault,CCLoadSpriteType type) {
    auto ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
    if (ret == NULL && useDefault ==true){
        if(type == CCLoadSpriteType_GOODS){
            if(DynamicResourceController::getInstance()->checkGoodsResource()){
                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_GOODS_TEXTURE,false);
                ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
                if(!ret){
                    fileName = "no_iconFlag.png";
                }
            }else{
                fileName = "no_iconFlag.png";
            }
        }else if (type == CCLoadSpriteType_MONSTERLAYERBUST){
            if(DynamicResourceController::getInstance()->checkMonsterLayerRes()){
                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_MONSTER_TEXTURE_layer,false);
                ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
                if(!ret){
                    fileName = "ooo_bust.png";
                }
            }else{
                fileName = "ooo_bust.png";
            }
        }
        else if (type == CCLoadSpriteType_MONSTERLAYERLITTLE){
            if(DynamicResourceController::getInstance()->checkMonsterLayerRes()){
                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_MONSTER_TEXTURE_layer,false);
                ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
                if(!ret){
                    fileName = "ooo.png";
                }
            }else{
                fileName = "ooo.png";
            }
        }
        else{
            fileName = "no_iconFlag.png";
        }
    }
    return createSprite(fileName);
}

CCScale9Sprite* CCLoadSprite::createScale9Sprite(const char *fileName) {
    cocos2d::CCSpriteFrame *test = loadResource(fileName);
    if (!test) {
        return CCScale9Sprite::create();
    }
    return CCScale9Sprite::createWithSpriteFrame(test);
}

cocos2d::CCSpriteFrame* CCLoadSprite::loadResource(const char *fileName) {
    auto ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
    if (ret == NULL) {
        CCLOG("CCLoadSprite loadResource: No file found in spriteFrames : %s. Possible missing file.", fileName);
        // Gets the texture by key firstly.
        auto texture = CCTextureCache::sharedTextureCache()->textureForKey(CC_2x2_WHITE_IMAGE_KEY);
        // If texture wasn't in cache, create it from RAW data.
        if (NULL == texture)
        {
            Image* image = new Image();
            bool isOK = image->initWithRawData(cc_2x2_white_image, sizeof(cc_2x2_white_image), 2, 2, 8);
            CC_UNUSED_PARAM(isOK);
            CCASSERT(isOK, "The 2x2 empty texture was created unsuccessfully.");
            
            texture = Director::getInstance()->getTextureCache()->addImage(image, CC_2x2_WHITE_IMAGE_KEY);
            CC_SAFE_RELEASE(image);
//            CCImage* image = new CCImage();
//            bool isOK = image->initWithImageData(cc_2x2_white_image, sizeof(cc_2x2_white_image));
//            //bool isOK = image->initWithImageData(cc_2x2_white_image, sizeof(cc_2x2_white_image), CCImage::Format::RAW_DATA, 2, 2, 8);
//            CCAssert(isOK, "The 2x2 empty texture was created unsuccessfully.");
//            texture = CCTextureCache::sharedTextureCache()->addUIImage(image, CC_2x2_WHITE_IMAGE_KEY);
//            CC_SAFE_RELEASE(image);
        }
        return CCSpriteFrame::createWithTexture(texture, CCRectMake(0, 0, 2, 2));
    }
    return ret;
}

cocos2d::CCSpriteFrame* CCLoadSprite::getSF(const char *fileName) {
    auto ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
    return ret;
}

void CCLoadSprite::loadCommonResource() {
    int generalIconIndex = 1;
    for (int i=generalIconIndex; i<=COMMON_COUNT; i++) {
        doLoadResource(COMMON_PATH, i);
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        std::string _path = CCFileUtils::sharedFileUtils()->getWritablePath()+DynamicResource_Folder + COMMON_PATH_1_HD;
        if (CCFileUtils::sharedFileUtils()->isFileExist(_path))
        {
            _path = CCFileUtils::sharedFileUtils()->getWritablePath()+DynamicResource_Folder + COMMON_PATH_HD;
            doLoadResource(_path.c_str(), 1); // 高清图标
        }
    }
    doLoadResource(COMMON_PATH, 308); // 装备图标
}
void CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType type){
    if(type == CCLoadSpriteType_GOODS){
        releaseDynamicResourceByName(DynamicResource_GOODS_TEXTURE);
    }else if (type == CCLoadSpriteType_MONSTERLAYERBUST || type == CCLoadSpriteType_MONSTERLAYERLITTLE){
        releaseDynamicResourceByName(DynamicResource_MONSTER_TEXTURE_layer);
    }
}
void CCLoadSprite::releaseDynamicResourceByName(const char* name){
    DynamicResourceController::getInstance()->loadNameTypeResource(name, true);
}
void CCLoadSprite::releaseCommonResourceForBattle() {
//    int res[5] = {7,11,13,14,15};
//    for (int i=0; i<5; i++) {
//        doReleaseResource(COMMON_PATH, res[i]);
//    }
}

void CCLoadSprite::releaseCommonResourceForWorld() {
    for (int i=1; i<=COMMON_COUNT; i++) {
        doReleaseResource(COMMON_PATH, i);
    }
    doReleaseResource(COMMON_PATH, 308);
}

void CCLoadSprite::doResourceByCommonIndex(int commonIndex,bool isLoad) {
    if (isLoad) {
        doLoadResource(COMMON_PATH, commonIndex);
    } else {
        doReleaseResource(COMMON_PATH, commonIndex);
    }
}

void CCLoadSprite::doResourceByWorldIndex(int worldIndex,bool isLoad){
    if (isLoad) {
        doLoadResource(WORLD_PATH, worldIndex);
    } else {
        doReleaseResource(WORLD_PATH, worldIndex);
    }
}

void CCLoadSprite::doResourceByPathIndex(string path,int commonIndex,bool isLoad)
{
    if (isLoad) {
        doLoadResource(path.c_str(), commonIndex);
    } else {
        doReleaseResource(path.c_str(), commonIndex);
    }
}

void CCLoadSprite::doResourceByImperialIndex(int imperialIndex,bool isLoad){
    if (isLoad) {
        doLoadResource(IMPERIAL_PATH, imperialIndex);
    } else {
        doReleaseResource(IMPERIAL_PATH, imperialIndex);
    }
}

void CCLoadSprite::doResourceByGeneralIndex(int commonIndex,bool isLoad) {
    if (isLoad) {
        doLoadResource(GENERAL_PATH, commonIndex);
    } else {
        doReleaseResource(GENERAL_PATH, commonIndex);
    }
}

int CCLoadSprite::m_currentResourceSceneId = -1;

void CCLoadSprite::loadResourceBySceneId(int sceneId,CCDictionary *dict) {
    
    // right now we just clear every scene texture, so don't need this check
//    // check mem limit
//    int totalBytes = CCTextureCache::sharedTextureCache()->getTotalTextureBytes();
//    if (totalBytes > MEM_LIMIT*1024*1024 && sceneId != m_currentResourceSceneId && sceneId != SCENE_ID_GENERAL) {
//        // release last scene resource
//        releaseResourceBySceneId(m_currentResourceSceneId);
//    }
    
    
    if (sceneId != SCENE_ID_GENERAL) {
        m_currentResourceSceneId = sceneId;
    }
    // load resource
    parseLoadOrRelease(true, sceneId,dict);
}

void CCLoadSprite::releaseResourceBySceneId(int sceneId,CCDictionary *dict) {
    parseLoadOrRelease(false, sceneId,dict);
}

void CCLoadSprite::parseLoadOrRelease(bool isLoad,int sceneId, CCDictionary *dict) {
    auto actor = [&isLoad](const char* path,int index){
        if (isLoad) {
            if(GlobalData::shared()->sceneResourceStepLoad == 1){
                doLoadResource(path, index, true);
            }else{
                doLoadResource(path, index);
            }
        } else {
            doReleaseResource(path, index);
        }
    };
    if(isLoad){
        FrameSpriteLoader::getInstance()->removeAll();
    }
    switch (sceneId) {
        case SCENE_ID_LOADING:{
            const char* path = LOADING_PATH;
            actor(path, 2);
        }
            break;
        case SCENE_ID_WORLD:{
            const char* path = WORLD_PATH;
            if (!dict) {
                for (int i=1; i<=WORLD_COUNT; i++) {
                    actor(path,i);
                }
                actor("World/nb_ocean.plist", 0); // guo.jiang
                actor("World/World_5.plist", 0); // guo.jiang
//                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_SERVERFIGHT_TEXTURE,isLoad);
            } else {
            }
        }
            break;
        case SCENE_ID_IMPERIAL:
        case SCENE_ID_MAIN:{
            const char* path = IMPERIAL_PATH;
            unsigned long ulc = clock();
            if (isLoad) {
                for (int i=1; i<=7; i++) {
                    if (i!=2) {
                        actor(path,i+10*GlobalData::shared()->contryResType);
                    }
                }
            }
            else
            {
                for (int i=1; i<=7; i++) {
                    actor(path,i+10*GlobalData::shared()->contryResType);
                }
            }
            actor(path,0);
            actor(path,29);
            CCLOG("Imperial load resource %lu",clock()-ulc);
        }
            break;
        case SCENE_ID_BATTLE:{
            const char* path = BATTLE_PATH;
            if (!dict) {
                for (int i=1; i<=1; i++) {
                    actor(path,i);
                }
            } else {
                // todo: parse battle texture info
                CCDictElement* pElement = NULL;
                CCDICT_FOREACH(dict, pElement){
                    auto key = pElement->getStrKey();
                    std::string subKey = key;
                    subKey = subKey.substr(0,5);
                    if (strcmp(subKey.c_str(), "skill") == 0) {
                        auto param = (CCInteger*)pElement->getObject();
                        actor(path,param->getValue() + 1000);
                    } else if (strcmp(key, "bg") == 0) {
                        auto param = (CCInteger*)pElement->getObject();
                        actor(path,(param->getValue())*10 + 1);
                        actor(path,(param->getValue())*10 + 2);
                    } else if (strcmp(key, "soldier") == 0) {
                        auto param = (CCArray*)pElement->getObject();
                        CCObject* pObj;
                        const char* soldierPath = BATTLE_SOLDIER_PATH;
                        CCARRAY_FOREACH(param, pObj) {
                            CCString* str = (CCString*)pObj;
                            char buffer[256];
                            sprintf(buffer,soldierPath,str->getCString());
                            actor(buffer,0);
                        }
                    }
                }
            }
        }
            break;
            
        case SCENE_ID_PVE:{
            const char* path = PVE_PATH;
            if (!dict) {
                actor(path,3);
            } else {
                int pveSceneId = dynamic_cast<CCInteger*>(dict->objectForKey("pveSceneId"))->getValue();
                int pveIndex = pveSceneId - 99999;
                actor(path,pveIndex*10+1);
                actor(path,pveIndex*10+2);
            }
        }
            break;
            
        case SCENE_ID_CROP:
        case SCENE_ID_IRON:
        case SCENE_ID_BARRACKS:
        case SCENE_ID_WOOD:
        case SCENE_ID_MARKET:
            break;            
        case SCENE_ID_GENERAL: {
            const char* path = GENERAL_PATH;
            for (int i=1; i<=1; i++) {
                actor(path,i);
            }
        }
            break;
        case SCENE_ID_DRAGON:{
            const char* path = IMPERIAL_PATH;
            unsigned long ulc = clock();
            if (isLoad) {
                for (int i=1; i<=7; i++) {
                    if (i!=2) {
                        actor(path,i+10*GlobalData::shared()->contryResType);
                    }
                }
            }
            else
            {
                for (int i=1; i<=7; i++) {
                    actor(path,i+10*GlobalData::shared()->contryResType);
                }
            }
            actor(path,0);
            actor(path,29);
            CCLOG("dragon scene load resource %lu",clock()-ulc);
        }
            break;
        default:
            CCLOG("Unknown scene id : %d  Load resource failed!",sceneId);
            return;
    }
    if(isLoad){
        FrameSpriteLoader::getInstance()->start();
    }
}

void CCLoadSprite::doLoadResource(const char* path, int index, bool frameLoad)
{
    if (!path) return;
    
#ifdef USING_DLC
    // guojiang
    // NBDLCController::create 会增加下载目录到 SearchPath，所以在加载资源前调用
    if (strcmp(path, COMMON_PATH) == 0)
    {
        char buffer[32];
        sprintf(buffer, "%d", index);
        
        string idx = (const char*)buffer;
        string manifest_file_path = "manifest/Common_" + idx + "_main.manifest";
        string version_filename = "Common_" + idx + "_version.manifest";
        string temp_manifest_filename = "Common_" + idx + "_project.manifest.temp";
        string manifest_filename = "Common_" + idx + "_project.manifest";
        
        auto it = map_NBDLCController.find(manifest_file_path.c_str());
        if (it == map_NBDLCController.end())
        {
            auto dlc = NBDLCController::create(manifest_file_path, version_filename, temp_manifest_filename, manifest_filename);
            map_NBDLCController[manifest_file_path.c_str()] = true;
            dlc->start(manifest_file_path, [](string manifest_file_path_as_key, EventAssetsManagerEx * event){
                
            });
        }
    }
#endif
    
    if (CCTexture2D::useDownloadResource() == true)
    {
        char buffer[256];
        sprintf(buffer,path,index);
        string tmpFile(buffer);
        string tmpPath;
        auto iter = tmpFile.find_last_of(".");
        if (iter!=string::npos)
        {
            tmpFile = tmpFile.substr(0,iter);
        }
        iter = tmpFile.find_last_of("/");
        if (iter!=string::npos)
        {
            tmpPath = tmpFile.substr(0,iter);
            tmpFile = tmpFile.substr(iter+1);
        }
        if (DynamicResourceController::getInstance()->checkCommonResource(tmpFile))
        {
            DynamicResourceController::getInstance()->loadNameTypeResource(tmpFile, false);
        }
        else
        {
            cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(buffer);
            retain_texture(buffer);
        }
    }
    else
    {
        char buffer[256];
        sprintf(buffer,path,index);
        if (frameLoad)
        {
            FrameSpriteLoader::getInstance()->addToLoadArr(buffer);
        }
        else
        {
            cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(buffer);
            retain_texture(buffer);
        }
    }
}

void CCLoadSprite::doReleaseResource(const char* path, int index) {
    if (path)
    {
        char buffer[256];
        sprintf(buffer,path,index);
//        cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesAndTextureWithFile(buffer);
        if (release_texture(buffer))
        {
            cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(buffer);
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
        }
    }
    
    
    if (CCTexture2D::useDownloadResource() == true && path)
    {
        string path1 =  CCFileUtils::sharedFileUtils()->getWritablePath();
        path1.append(DynamicResource_Folder);
        string filePath = path;
        auto iter = filePath.find_last_of("/");
        if(iter!=string::npos){
            filePath = filePath.substr(iter+1);
            path1.append(filePath);
        }
        char buffer[256];
        sprintf(buffer,path1.c_str(),index);
        CCLog("android_test_doReleaseResource[%s]",buffer);
        if (CCFileUtils::sharedFileUtils()->isFileExist(buffer))
        {
            if (release_texture(buffer))
            {
                cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(buffer);
                CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            }
        }
    }
}

void CCLoadSprite::doLoadResourceAsync(const char *path, CCCallFuncO* selector, int index)
{
    char buffer[256];
    sprintf(buffer,path,index);
    if (cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->isSpriteFramesWithFileLoaded(buffer))
    {
        selector->retain();
        selector->setObject(NULL);
        selector->execute();
        CC_SAFE_RELEASE(selector);
        return;
    }
    if (buffer)
    {
        std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(buffer);
        ValueMap dict2 = FileUtils::getInstance()->getValueMapFromFile(fullPath);
        CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());
        
        if (dict == NULL) {
            return;
        }
        
        string texturePath("");
        
        CCDictionary* metadataDict = (CCDictionary*)dict->objectForKey("metadata");
        if (metadataDict)
        {
            // try to read  texture file name from meta data
            texturePath = metadataDict->valueForKey("textureFileName")->getCString();
        }
        
        if (! texturePath.empty())
        {
            // build texture path relative to plist file
            texturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(texturePath.c_str(), buffer);
        }
        else
        {
            // build texture path by replacing file extension
            texturePath = buffer;
            
            // remove .xxx
            size_t startPos = texturePath.find_last_of(".");
            texturePath = texturePath.erase(startPos);
            
            // append .png
            texturePath = texturePath.append(".png");
            
            CCLOG("cocos2d: CCSpriteFrameCache: Trying to use file %s as texture", texturePath.c_str());
        }
        auto callbackClass = new ImageLoadAsync();
        callbackClass->setSuccessCallback(selector);
        callbackClass->setPlistPath(buffer);
        //callbackClass->m_dic = dict2;
        //CCTextureCache::sharedTextureCache()->addImageAsync(texturePath.c_str(), callbackClass, callfuncO_selector(ImageLoadAsync::addImageCallback));
        CCTextureCache::sharedTextureCache()->addImageAsync(texturePath, CC_CALLBACK_1(ImageLoadAsync::addImageCallback,callbackClass));
    }
}

void ImageLoadAsync::addImageCallback(CCObject* obj)
{
    CCTexture2D* pTexture = dynamic_cast<CCTexture2D*>(obj);
    if (pTexture)
    {
        //ValueMap dict = m_dic;
        //cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(dict, pTexture);
        //dict->release();
       // cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesName(getPlistPath().c_str());
        cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(getPlistPath(),pTexture);
        CCLoadSprite::retain_texture(getPlistPath());
        if (getSuccessCallback())
        {
            CCCallFuncO * func=getSuccessCallback();
            func->retain();
            setSuccessCallback(NULL);
            func->setObject(pTexture);
            func->execute();
            CC_SAFE_RELEASE(func);
        }
    }
    else
    {
        CCLOG("cocos2d: CCSpriteFrameCache: Couldn't load texture");
    }
    CC_SAFE_RELEASE(this);
}

CCSize CCLoadSprite::getSpriteSize(const char *path, int index, const char *fileName)
{
    char buffer[256];
    sprintf(buffer,path,index);

    if (buffer)
    {
        std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(buffer);
        CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());
        
        if (dict == NULL) {
            return CCSize(0,0);
        }
        
        string texturePath("");
        
        CCDictionary* framesDict = (CCDictionary*)dict->objectForKey("frames");
        if (framesDict)
        {
            CCDictionary* fileDict = (CCDictionary*)framesDict->objectForKey(fileName);
            if (fileDict) {
                string _strSize = fileDict->valueForKey("sourceSize")->getCString();
                CC_SAFE_RELEASE(dict);
                return cocos2d::SizeFromString(_strSize.c_str());
            }
        }
        CC_SAFE_RELEASE(dict);
    }
    return CCSize(0,0);
}

static FrameSpriteLoader *_instace = NULL;
FrameSpriteLoader *FrameSpriteLoader::getInstance(){
    if(_instace == NULL){
        _instace = new FrameSpriteLoader();
        _instace->init();
    }
    return _instace;
}

void FrameSpriteLoader::init(){
    m_loadSpriteArr = CCArray::create();
}

void FrameSpriteLoader::addToLoadArr(std::string path){
    m_loadSpriteArr->addObject(CCString::create(path));
}

void FrameSpriteLoader::start(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void FrameSpriteLoader::update(float time){
    if(m_loadSpriteArr && m_loadSpriteArr->count() > 0){
        std::string filePath = dynamic_cast<CCString*>(m_loadSpriteArr->objectAtIndex(0))->getCString();
        m_loadSpriteArr->removeObjectAtIndex(0);
        if(filePath != ""){
//            CCLoadSprite::loadResource(filePath.c_str());
            cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(filePath.c_str());
            CCLoadSprite::retain_texture(filePath);
        }
    }
    if(!m_loadSpriteArr || m_loadSpriteArr->count() == 0){
        CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(SPRITE_LOAD_COMPLETE);
    }
}

void FrameSpriteLoader::removeAll(){
    if(m_loadSpriteArr){
        m_loadSpriteArr->removeAllObjects();
    }
}
