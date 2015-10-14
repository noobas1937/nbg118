//
//  NBGRenderTarget.h
//  IF
//
//  Created by 5ive on 15-10-14.
//
//

#ifndef __IF__NBGRenderTarget__
#define __IF__NBGRenderTarget__

#include "cocos2d.h"

class NBGRenderTarget : public cocos2d::RenderTexture
{
public:
    static NBGRenderTarget* create(int w, int h, cocos2d::Texture2D::PixelFormat format);
    NBGRenderTarget();
    virtual ~NBGRenderTarget();
private:
    
};


#endif /* defined(__IF__NBGRenderTarget__) */
