//
//  CCFlagWaveSprite.h
//  IF
//
//  Created by Luojunfeng on 15/1/9.
//
//

#ifndef __IF__CCFlagWaveSprite__
#define __IF__CCFlagWaveSprite__

#include "cocos2d.h"

NS_CC_BEGIN

class CCFlagWaveSprite : public CCSprite {
public:
    static CCFlagWaveSprite* create(CCSpriteFrame *spf);
    
private:
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
};

NS_CC_END

#endif /* defined(__IF__CCFlagWaveSprite__) */
