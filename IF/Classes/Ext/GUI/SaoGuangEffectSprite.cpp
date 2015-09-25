//
//  SaoGuangEffectSprite.cpp
//  IF
//
//  Created by 李锐奇 on 15/3/3.
//
//

#include "SaoGuangEffectSprite.h"
NS_CC_BEGIN

#define STRINGIFY(A)  #A
const static GLchar* SaoGuangVert = STRINGIFY
(
 attribute vec4 a_position;
 attribute vec2 a_texCoord;
 attribute vec4 a_color;
 
#ifdef GL_ES
 varying lowp vec4 v_fragmentColor;
 varying mediump vec2 v_texCoord;
#else
 varying vec4 v_fragmentColor;
 varying vec2 v_texCoord;
#endif
 
 void main()
{
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
});

const static GLchar* SaoGuangFrag = STRINGIFY
(
#ifdef GL_ES
 precision mediump float;
#endif
 varying vec2 v_texCoord;
 varying vec4 v_fragmentColor;
 
// uniform sampler2D CC_Texture0;
 uniform float totalW;
 uniform float startX;
 
 void main()
{
    float time = CC_Time[1];
    float speed = 0.4;
    float radius = 0.02;
    float realTotalW = totalW * 1.5 + 2.0 * radius;
    float realStartX = startX - radius;
    float currentCenter = mod(time * speed - realTotalW, realTotalW) + realStartX;
    float dis = abs(v_texCoord.x - currentCenter);
    vec4 color = texture2D(CC_Texture0, v_texCoord);
    float alpha = 1.0;
    if(dis > radius){
        alpha = 0.0;
    }else{
        alpha = sqrt((radius - dis) / radius);
    }
    color = color * alpha;
    gl_FragColor = v_fragmentColor * color;
});

const static GLchar* SaoGuangFragETC = STRINGIFY
(
#ifdef GL_ES
 precision mediump float;
#endif
 varying vec2 v_texCoord;
 varying vec4 v_fragmentColor;
 
// uniform sampler2D CC_Texture0;
// uniform sampler2D CC_Texture1;

 uniform float totalW;
 uniform float startX;
 
 void main()
{
    float time = CC_Time[1];
    float speed = 0.4;
    float radius = 0.02;
    float realTotalW = totalW * 1.5 + 2.0 * radius;
    float realStartX = startX - radius;
    float currentCenter = mod(time * speed - realTotalW, realTotalW) + realStartX;
    float dis = abs(v_texCoord.x - currentCenter);
    vec4 color = texture2D(CC_Texture0, v_texCoord);
    color.a = texture2D(CC_Texture1, v_texCoord).r;
    float alpha = 1.0;
    if(dis > radius){
        alpha = 0.0;
    }else{
        alpha = sqrt((radius - dis) / radius);
    }
    color = color * alpha;
    gl_FragColor = v_fragmentColor * color;
});


SaoGuangEffectSprite *SaoGuangEffectSprite::create(CCSpriteFrame *frame){
    SaoGuangEffectSprite *ret = new SaoGuangEffectSprite();
    if(ret && ret->initWithSpriteFrame(frame)){
        ret->autorelease();
        
        CCGLProgram *p = new CCGLProgram();
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if (frame->getTexture()->getAlphaName()) {
                p = CCShaderCache::sharedShaderCache()->addCustomGLProgram("saoguang", SaoGuangVert, SaoGuangFragETC, true);
            } else {
                p = CCShaderCache::sharedShaderCache()->addCustomGLProgram("saoguang", SaoGuangVert, SaoGuangFrag);
            }
        #else
            p = CCShaderCache::sharedShaderCache()->addCustomGLProgram("saoguang", SaoGuangVert, SaoGuangFrag);
        #endif
        float totalW = ret->_quad.tr.texCoords.u - ret->_quad.tl.texCoords.u;
        float startX = ret->_quad.tl.texCoords.u;
        p->setUniformLocationWith1f(p->getUniformLocationForName("totalW"), totalW);
        p->setUniformLocationWith1f(p->getUniformLocationForName("startX"), startX);
        
        ret->setShaderProgram(p);
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

NS_CC_END