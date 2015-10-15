//
//  NBGRenderTarget.cpp
//  IF
//
//  Created by 5ive on 15-10-14.
//
//

#include "NBGRenderTarget.h"

NBGRenderTarget::NBGRenderTarget(){}
NBGRenderTarget::~NBGRenderTarget(){}

NBGRenderTarget* NBGRenderTarget::create(int w, int h, cocos2d::Texture2D::PixelFormat format)
{
    NBGRenderTarget *ret = new (std::nothrow) NBGRenderTarget();
    
    if(ret && ret->initWithWidthAndHeight(w, h, format))
    {
        ret->autorelease();
        ret->setClearColor(cocos2d::Color4F(0, 0, 0, 0));
        ret->setClearFlags(GL_COLOR_BUFFER_BIT);
        ret->setAutoDraw(true);
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}