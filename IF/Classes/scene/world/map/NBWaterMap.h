//
//  NBWaterMap.h
//  IF
//
//  Created by 5ive on 15-10-28.
//
//

#ifndef __IF__NBWaterMap__
#define __IF__NBWaterMap__

#define MAP_GRID_COLUMN 64
#define MAP_GRID_ROWS	64

enum {
    kMapUniformUTime,
    kMapUniformTexture0,
    kMapUniformNormal,
    
    kMapUniformMax
};

#define kMapUniformUTime_s "u_time"
#define kMapUniformTexture0_s "texture0"
#define kMapUniformNormal_s "normal"

#include "cocos2d.h"

USING_NS_CC;

class NBWaterMap : public Sprite
{
public:
    NBWaterMap();
    ~NBWaterMap();
    
    bool initWithTexture(CCTexture2D* texture, const CCRect&  rect);
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    virtual void update(float dt);
    void onDraw(const Mat4& transform, uint32_t flags);
    void initProgram();
    void listenBackToForeground(CCObject *obj);
    
    static NBWaterMap* create(const char *pszFileName);
    static NBWaterMap* create(const char *pszFileName, const CCRect&  rect);
    CC_SYNTHESIZE_RETAIN(CCTexture2D*, m_pTexture0, Texture0);
    GLint    m_uUniforms[kMapUniformMax];
protected:
    V3F_T2F m_vertArr[(MAP_GRID_COLUMN + 1) * (MAP_GRID_ROWS + 1)];
    GLushort m_index[(MAP_GRID_COLUMN * MAP_GRID_ROWS) * 6];
    void initVertexData();
    void drawByVertexs();
    CC_SYNTHESIZE(float, m_fUtime,Utime);
    
    CustomCommand _customCommand;
};


#endif /* defined(__IF__NBWaterMap__) */
