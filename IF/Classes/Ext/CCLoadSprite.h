//
//  CCLoadSprite.h
//  DragonEmpire
//
//  Created by 邹 程 on 13-8-22.
//
//

#ifndef __CCLoadSprite__
#define __CCLoadSprite__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CommonInclude.h"

#define COMMON_PATH "Common/Common_%d.plist"

#define LOADING_PATH "Loading/Loading_%d.plist"
#define MAIN_PATH "Main/Main_%d.plist"
#define BATTLE_PATH "Battle/Battle_%d.plist"
#define PVE_PATH "PVE/PVE_%d.plist"
#define WORLD_PATH "World/World_%d.plist"
#define GENERAL_PATH "General/General_%d.plist"
#define IMPERIAL_PATH "Imperial/Imperial_%d.plist"
#define BATTLE_SOLDIER_PATH "Battle/Battle_%s.plist"

// hd
#define COMMON_PATH_HD "Common/Common_%d_hd.plist"
#define IMPERIAL_PATH_0_HD "Imperial/Imperial_0_hd.plist"
#define COMMON_PATH_1_HD "Common/Common_1_hd.plist"
#define COMMON_PATH_101_HD "Common/Common_101_hd.plist"
#define COMMON_PATH_200_HD "Common/Common_200_hd.plist"
#define COMMON_PATH_201_HD "Common/Common_201_hd.plist"
#define COMMON_PATH_202_HD "Common/Common_202_hd.plist"
#define COMMON_PATH_203_HD "Common/Common_203_hd.plist"

#define SPRITE_LOAD_COMPLETE "spriteLoadComplete"
#define SCENE_ID_GENERAL          987   

enum CCLoadSpriteType{
    CCLoadSpriteType_DEFAULT,
    CCLoadSpriteType_GOODS,
    CCLoadSpriteType_EQUIP,
    CCLoadSpriteType_MONSTER,
    CCLoadSpriteType_MAP,
    CCLoadSpriteType_MONSTERLAYERBUST,
    CCLoadSpriteType_MONSTERLAYERLITTLE,
    CCLoadSpriteType_HEAD_ICON,
    CCLoadSpriteType_HEAD_ICON_BUST,
    CCLoadSpriteType_HEAD_ICON_MIDDLE,
    CCLoadSpriteType_ACTIVITY_RES,
    CCLoadSpriteType_SCIENCE,
};
class CCLoadSprite {
public:
    static void init();
    
    static void retain_texture(const std::string& texture);
    static bool release_texture(const std::string& texture);
    
    static void loadResourceBySceneId(int sceneId,cocos2d::CCDictionary *dict = NULL);
    static void releaseResourceBySceneId(int sceneId,cocos2d::CCDictionary *dict = NULL);
    static void loadCommonResource();
    static void releaseDynamicResourceByType(CCLoadSpriteType type);
    static void releaseDynamicResourceByName(const char* name);
    static void releaseCommonResourceForBattle();
    static void releaseCommonResourceForWorld();
    static cocos2d::CCSprite* createSprite(const char* fileName);
    static cocos2d::CCSprite* createSprite(const char* fileName, bool useDefault,CCLoadSpriteType type);
    static CCScale9Sprite* createScale9Sprite(const char* fileName);
    static cocos2d::CCSpriteFrame* loadResource(const char* fileName);
    
    static void doResourceByCommonIndex(int commonIndex,bool isLoad);
    static void doResourceByImperialIndex(int imperialIndex,bool isLoad);
    static void doResourceByWorldIndex(int worldIndex,bool isLoad);
    static void doResourceByGeneralIndex(int commonIndex,bool isLoad);
    static void doResourceByPathIndex(string path,int commonIndex,bool isLoad);
    static long getCurrentTime();
    
    static cocos2d::CCSpriteFrame* getSF(const char* fileName);
    static void doLoadResourceAsync(const char *path, cocos2d::CCCallFuncO* selector, int index);
    static cocos2d::CCSize getSpriteSize(const char *path, int index, const char *fileName);
    static void doLoadCommonResourceAsync();
private:
    static cocos2d::CCSpriteFrame* testResource(const char* fileName,int sheetCount,const char *sheetPath);
    
    static void doLoadResource(const char* path,int index, bool frameLoad = false);
    static void doReleaseResource(const char* path,int index);
    static void parseLoadOrRelease(bool isLoad,int sceneId,cocos2d::CCDictionary *dict = NULL);
    
    static int m_currentResourceSceneId;
};

class ImageLoadAsync:public cocos2d::CCObject
{
public:
    void addImageCallback(cocos2d::CCObject* obj);
private:
    CC_SYNTHESIZE_SAFEOBJECT(cocos2d::CCCallFuncO, m_onSuccess, SuccessCallback);
    CC_SYNTHESIZE(std::string, m_plistPath, PlistPath);
    CC_SYNTHESIZE(CCDictionary*, m_dic, Dic);
    
};

class FrameSpriteLoader : public CCObject{
public:
    static FrameSpriteLoader *getInstance();
    void addToLoadArr(std::string path);
    void start();
    void removeAll();
    void update(float time);
private:
    void init();
    CCSafeObject<CCArray> m_loadSpriteArr;
};
#endif /* defined(__DragonEmpire__CCLoadSprite__) */
