//
//  DynamicTiledMap.hpp
//  IF
//
//  Created by gj on 10/12/15.
//
//

#ifndef DynamicTiledMap_h
#define DynamicTiledMap_h

#include "cocos2d.h"

USING_NS_CC;

class DynamicTiledMap : public TMXTiledMap {
public:
    
    static DynamicTiledMap* create(const char *tmxFile,const CCPoint& pos, int loopSize);
    
    ~DynamicTiledMap();
    
    void applySeaShader(const std::string& texture1 = "nb_ocean/sea.png",
                        const std::string& textureLight = "shaders/caustics.png",
                        const std::string& vShaderFilename = "shaders/water.vsh",
                        const std::string& fShaderFilename = "shaders/water.fsh");
    
    virtual void setPosition(const CCPoint &position);
    bool isNeedUpdate();
    void updateDynamicMap(CCPoint point = ccp(-1, -1));
    void updataBoderMap(CCPoint point,int serverId);
    CCPoint getViewPointByTilePoint(const CCPoint &tilePoint,int forceServerId = -1);
    CCPoint getTilePointByViewPoint(const CCPoint &viewPoint);
    
    CCPoint getTileMapPointByViewPoint(const CCPoint &viewPoint);
    
    int getServerIdByViewPoint(const CCPoint &viewPoint);
    void onShowMapMask();
    CCPoint lastDisplayTilePoint;
    CCPoint currentTilePoint;
    CCPoint centerTilePoint;
    CCPoint centerViewPoint;
    bool isSendCmd;
    bool isChangingServer;
    
    //    static Vector<DynamicTiledMap*> mZombieCaches;
    //    static void saveToCache(const std::string& key, DynamicTiledMap* );
    //    static DynamicTiledMap* loadFromCache(const std::string& key);
    
protected:
    GLProgramState* m_glpstate;
    Vec2 m_LightAni;
};

#endif /* DynamicTiledMap_h */
