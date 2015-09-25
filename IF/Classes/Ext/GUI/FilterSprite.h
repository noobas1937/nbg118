//
//  FilterSprite.h
//  IF
//
//  Created by 李锐奇 on 15/2/4.
//
//

#ifndef __IF__FilterSprite__
#define __IF__FilterSprite__

#include "CommonInclude.h"
//#include "kazmath/kazmath.h"

USING_NS_CC;

#include "CommonInclude.h"

class FilterSprite : public CCSprite{
public:
    FilterSprite() :
    m_bright(0.0f)
    ,m_contrast(1.0f)
    ,m_saturation(1.0f)
    {}
    ;
    virtual ~FilterSprite();
    
//    static FilterSprite* create();
    static FilterSprite* create(const std::string& filename);
    static FilterSprite* create(const std::string& filename, const CCRect& rect);
//
//    
//    static FilterSprite* createWithTexture(CCTexture2D *pTexture);
//    static FilterSprite* createWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated=false);
    static FilterSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    static FilterSprite* createWithSpriteFrameName(const std::string& spriteFrameName);
    
    bool initWithTexture(CCTexture2D* pTexture, const CCRect& tRect);
    virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated);
    void setFilterMat(kmMat4 matrixArray);
    void setParam(const ccColor3B &color, float bright = 0.0f, float contrast = 1.0f, float saturation = 1.0f);
    //to-do 提供一个设置滤镜的方法
private:
    float m_bright;
    float m_contrast;
    float m_saturation;
    ccColor3B m_color;
    kmMat4 m_uSpriteFilter;
};

#endif /* defined(__IF__FilterSprite__) */
