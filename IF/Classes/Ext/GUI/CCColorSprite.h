//
//  CCColorSprite.h
//  IF
//
//  Created by 邹 程 on 14-4-14.
//
//

#ifndef __IF__CCColorSprite__
#define __IF__CCColorSprite__

#include "cocos2d.h"

using namespace std;

NS_CC_BEGIN

class CCColorSpriteBatchNode : public CCSpriteBatchNode {
public:
    
    CCColorSpriteBatchNode(ccColor3B decorateColor) :
    m_decorateColor(decorateColor)
    ,CCSpriteBatchNode()
    {}

    CCColorSpriteBatchNode* create(const string& pic,ccColor3B decorateColor);
    
//    virtual void setTextureAtlas(CCTextureAtlas* textureAtlas);
//    virtual bool initWithTexture(CCTexture2D *tex, unsigned int capacity);
//    virtual void setTexture(CCTexture2D *texture);
//    virtual void draw(void);
    
private:
    ccColor3B m_decorateColor;

};

class CCColorSprite : public CCSprite {
public:
    
    CCColorSprite(ccColor3B decorateColor) :
    m_decorateColor(decorateColor)
    ,CCSprite()
    {}
    
    CCColorSprite* create(const string& pic,ccColor3B decorateColor);
    
//    virtual bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated);
//    
//    virtual void draw(void);
    
    ccColor3B getDecorateColor();
    void setDecorateColor(ccColor3B decorateColor);
private:
    ccColor3B m_decorateColor;
};

NS_CC_END

#endif /* defined(__IF__CCColorSprite__) */
