//
//  NBWaterSprite.hpp
//  IF
//
//  Created by gj on 10/23/15.
//
//

#ifndef NBWaterSprite_hpp
#define NBWaterSprite_hpp

#include "cocos2d.h"

USING_NS_CC;

enum
{
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
    kCCWaterUniformShape,
    
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
#define kCCWaterUniformShape_s           "shape"


#define WATER_GRID_COLUMN 64
#define WATER_GRID_ROWS	  64


#define WATER_NORMALS "shaders/water_normals.png"

class NBWaterSprite : public Sprite
{
public:
    NBWaterSprite();
    ~NBWaterSprite();
    
    bool initWithTexture(Texture2D* texture, const Rect& rect);
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    virtual void update(float dt);
    void onDraw(const Mat4& transform, uint32_t flags);
    void initProgram();
    void listenBackToForeground(Ref *obj);
    
    
    
    static NBWaterSprite* create(const char *pszFileName);
    static NBWaterSprite* create(const char *pszFileName, const CCRect&  rect);
    CC_SYNTHESIZE_RETAIN(Texture2D*, m_pNormal, NormalTexture);
    CC_SYNTHESIZE_RETAIN(Texture2D*, m_pFoam, FoamTexture);
    CC_SYNTHESIZE_RETAIN(Texture2D*, m_pShape, ShapeTexture);
    CC_SYNTHESIZE_RETAIN(Texture2D*, m_pLightMap, LightMapTexture);
    GLint m_uUniforms[kCCWaterUniform_MAX];
    CC_SYNTHESIZE_PASS_BY_REF(Color3B, m_shoreDarkColor, ShoreDark);
    CC_SYNTHESIZE_PASS_BY_REF(Color3B, m_shoreLightColor, ShoreLight);
    CC_SYNTHESIZE_PASS_BY_REF(Color3B, m_seaDarkColor, SeaDark);
    CC_SYNTHESIZE_PASS_BY_REF(Color3B, m_seaLightColor, SeaLight);
protected:
    ccV3F_C4B_T2F m_vertArr[(WATER_GRID_COLUMN + 1) * (WATER_GRID_ROWS + 1)];
    GLushort m_index[(WATER_GRID_COLUMN * WATER_GRID_ROWS) * 6];
    void initVertexData();
    void setUniforms();
    void drawByVertexs();
    
    CC_SYNTHESIZE(float, m_fUtime,      Utime);
    CC_SYNTHESIZE(float, m_fMovement,   WaveMovement);
    CC_SYNTHESIZE(float, m_fWaveHeight, WaveHeight);
    CC_SYNTHESIZE(float, m_fDivHeight,  DivHeight);
    CC_SYNTHESIZE(float, m_fDivWidth,   DivWidth);
    
    CC_SYNTHESIZE(float, m_shapeScaleX,   ShapeScaleX);
    CC_SYNTHESIZE(float, m_shapeScaleY,   ShapeScaleY);
    CustomCommand _customCommand;
};

#endif /* NBWaterSprite_hpp */
