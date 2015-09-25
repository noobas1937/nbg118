//
//  SpriteBlur.h
//  IF
//
//  Created by ganxiaohua on 14-1-9.
//
//

#ifndef __IF__SpriteBlur__
#define __IF__SpriteBlur__

#include "CommonInclude.h"
#define EVNET_COME_TO_FOREGROUND    "event_come_to_foreground"

class SpriteBlur : public CCSprite
{
public:
    ~SpriteBlur();
    void setBlurSize(float f);
    bool initWithTexture(CCTexture2D* texture, const CCRect&  rect);
    virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated);
    void initProgram();
    void listenBackToForeground(CCObject *obj);
    
    static SpriteBlur* create(const char *pszFileName);
    
    CCPoint blur_;
    GLfloat    sub_[4];
    
    GLuint    blurLocation;
    GLuint    subLocation;
};

#endif /* defined(__IF__SpriteBlur__) */
