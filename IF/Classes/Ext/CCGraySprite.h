//
//  CCGraySprite.h
//  Cocos2dxExt
//
//  Created by zhaohf on 14-3-19.
//
//

#ifndef __Cocos2dxExt__CCGraySprite__
#define __Cocos2dxExt__CCGraySprite__

//#include "sprite_nodes/CCSprite.h"
//#include "textures/CCTexture2D.h"
//#include "cocoa/CCGeometry.h"
#include "cocos2d.h"
USING_NS_CC;

class CCGraySprite : public CCSprite
{
public:
	CCGraySprite();
	virtual ~CCGraySprite();
	static CCGraySprite* create(const char* pszFileName);
    static CCGraySprite* createWithSpriteFrameName(const char *pszSpriteFrameName);
    static CCGraySprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    
	bool initWithTexture(cocos2d::CCTexture2D* pTexture, const cocos2d::CCRect&  tRect);
    virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated);
};


#endif /* defined(__Cocos2dxExt__CCGraySprite__) */
