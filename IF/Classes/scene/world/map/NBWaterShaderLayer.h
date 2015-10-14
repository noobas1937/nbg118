//
//  NBWaterShaderLayer.h
//
//

#ifndef __PetIsland__NBWaterShaderLayer__
#define __PetIsland__NBWaterShaderLayer__

#include "cocos2d.h"

USING_NS_CC;

#define CREATE_WATER_SHADER_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

class SpriteMap;

class NBWaterShaderLayer : public Layer
{
public:
	virtual bool init();
	CREATE_WATER_SHADER_FUNC(NBWaterShaderLayer);
	NBWaterShaderLayer();
	virtual ~NBWaterShaderLayer();
    
    void tick(float time);
    
    SpriteMap* getMapSprite() { return m_pMapSprite; }
    
protected:
    SpriteMap* m_pMapSprite;
};

/*
 * SpriteWater
 */

enum {
	kCCWaterUniformTime,
	kCCWaterUniformDivLevelWidth,
	kCCWaterUniformDivLevelHeight,
	kCCWaterUniformWAVE_HEIGHT,
	kCCWaterUniformWAVE_MOVEMENT,
	kCCWaterUniformSEA_DARK,
	kCCWaterUniformSHORE_DARK,
	kCCWaterUniformSHORE_LIGHT,
    kCCWaterUniformSEA_LIGHT,
    kCCWaterUniformlightPos,
    kCCWaterUniformNormal,
    kCCWaterUniformlightMap,
    kCCWaterUniformFoam,
    
	kCCWaterUniform_MAX,
};

#define kCCWaterUniformTime_s           "u_time"
#define kCCWaterUniformDivLevelWidth_s  "u_1DivLevelWidth"
#define kCCWaterUniformDivLevelHeight_s "u_1DivLevelHeight"
#define kCCWaterUniformWAVE_HEIGHT_s    "WAVE_HEIGHT"
#define kCCWaterUniformWAVE_MOVEMENT_s  "WAVE_MOVEMENT"
#define kCCWaterUniformSEA_DARK_s       "SEA_DARK"
#define kCCWaterUniformSHORE_DARK_s     "SHORE_DARK"
#define kCCWaterUniformSHORE_LIGHT_s    "SHORE_LIGHT"
#define kCCWaterUniformSEA_LIGHT_s      "SEA_LIGHT"
#define kCCWaterUniformlightPos_s       "u_lightPos"
#define kCCWaterUniformNormal_s         "normal0"
#define kCCWaterUniformlightMap_s       "lightmap"
#define kCCWaterUniformFoam_s           "foam"

#define MAP_GRID_COLUMN 32
#define MAP_GRID_ROWS	32

enum {
    kMapUniformUTime,
    kMapUniformTexture0,
    kMapUniformTexture1,
    kMapUniformNormal,
    kMapUniformNormalAlpha,
    
    kMapUniformMax
};

struct CC_DLL V2F_T2F
{
    //! vertices (2F)
    Vec2       vertices;
    //! tex coords (2F)
    Tex2F          texCoords;
};

#define kMapUniformUTime_s "u_time"
#define kMapUniformTexture0_s "texture0"
#define kMapUniformNormal_s "normal"

class SpriteMap : public Sprite
{
public:
    SpriteMap();
    ~SpriteMap();
    
    bool initWithTexture(Texture2D* texture, const Rect& rect);
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    virtual void update(float dt);
	void onDraw(const Mat4& transform, uint32_t flags);
    void initProgram();
    
    static SpriteMap* create(const char *pszFileName);
    static SpriteMap* create(const char *pszFileName, const Rect& rect);
    CC_SYNTHESIZE_RETAIN(Texture2D*, m_pTexture0, Texture0);
    
private:
    GLint    m_uUniforms[kMapUniformMax];
    V2F_T2F m_vertArr[(MAP_GRID_COLUMN + 1) * (MAP_GRID_ROWS + 1)];
    GLushort m_index[(MAP_GRID_COLUMN * MAP_GRID_ROWS) * 6];
    void initVertexData();
    GLuint  m_nVAOname;
    GLuint  m_arrBuffersVBO[2]; //0: vertex  1: indices
    void drawByVertexs();
    float m_fUtime;
	CustomCommand _customCommand;
    
    void setupVBOandVAO();
};

#endif /* defined(__PetIsland__NBWaterShaderLayer__) */
