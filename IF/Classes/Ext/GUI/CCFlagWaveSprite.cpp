//
//  CCFlagWaveSprite.cpp
//  IF
//
//  Created by Luojunfeng on 15/1/9.
//
//

#include "CCFlagWaveSprite.h"

NS_CC_BEGIN

#define STRINGIFY(A)  #A
const static GLchar* WaveVert = STRINGIFY
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
 varying highp float v_time;
 uniform highp vec2 uniformT;
 
 void main()
{
    v_time = fract(0.1 * CC_Time.g);
    gl_Position = CC_PMatrix * (a_position + vec4(-(a_texCoord.x - uniformT.x) * 20.0, -(a_texCoord.x - uniformT.x) * 30.0 - (a_texCoord.x - uniformT.x) * (a_texCoord.y - uniformT.y) * 400.0, 0.0, 0.0));
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
});

const static GLchar* WaveFrag = STRINGIFY
(
#ifdef GL_ES
 precision mediump float;
#endif
 
 varying vec4 v_fragmentColor;
 varying vec2 v_texCoord;
 varying highp float v_time;
 
// uniform sampler2D CC_Texture0;
 uniform highp float sinWidth;
 uniform highp float sinHeight;
 uniform highp vec2 uniformT;

 void main()
{
    vec2 pos;
    float time = v_time * 50.0;
    time = mod(time, 360.0);
    pos.x = v_texCoord.x;
    float s = cos(sinWidth *(v_texCoord.x) - time);
    pos.y = v_texCoord.y + s * sinHeight * (v_texCoord.x - uniformT.x) * 20.0;
//    gl_FragColor =  v_fragmentColor * texture2D(CC_Texture0, pos) * (1.0 + pow(s * 0.3, 1.5));
    gl_FragColor =  v_fragmentColor * texture2D(CC_Texture0, pos);
});

const static GLchar* WaveFragETC = STRINGIFY
(
#ifdef GL_ES
 precision mediump float;
#endif
 
 varying vec4 v_fragmentColor;
 varying vec2 v_texCoord;
 varying highp float v_time;
 
// uniform sampler2D CC_Texture0;
// uniform sampler2D CC_Texture1;
 uniform highp float sinWidth;
 uniform highp float sinHeight;
 uniform highp vec2 uniformT;

 void main()
{
    vec2 pos;
    float time = v_time * 100.0;
    time = mod(time, 360.0);
    pos.x = v_texCoord.x;
    float s = cos(sinWidth *(v_texCoord.x) - time);
    pos.y = v_texCoord.y + s * sinHeight * (v_texCoord.x - uniformT.x) * 10.0;
    vec4 color = texture2D(CC_Texture0, pos);
    color.a = texture2D(CC_Texture1, pos).r;
//    gl_FragColor =  v_fragmentColor * color * (1.0 + pow(s * 0.3, 1.5));
    gl_FragColor =  v_fragmentColor * color;
});

CCFlagWaveSprite* CCFlagWaveSprite::create(CCSpriteFrame *spf) {
    auto ret = new CCFlagWaveSprite();
    if (ret && ret->initWithSpriteFrame(spf)) {
        float scaleX = spf->getRectInPixels().size.width / spf->getTexture()->getContentSizeInPixels().width;
        float scaleY = spf->getRectInPixels().size.height / spf->getTexture()->getContentSizeInPixels().height;
        float sinWidth = 7.0 / scaleX;
        float sinHeight = scaleY * 0.12;
        CCRect r = ret->getTextureRect();
        r.origin.y = r.origin.y - r.size.height * 0.1;
        r.size.height *= 1.2;
        ret->setTextureRect(r);
        CCGLProgram* p = new CCGLProgram();
        bool isETC = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (spf->getTexture()->getAlphaName()) {
            p->initWithVertexShaderByteArray(WaveVert, WaveFragETC);
            isETC = true;
        } else {
            p->initWithVertexShaderByteArray(WaveVert, WaveFrag);
        }
#else
        p->initWithVertexShaderByteArray(WaveVert, WaveFrag);
#endif
//        p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
//        p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
//        p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

        ret->setShaderProgram(p);
        p->release();
        p->link();
        p->updateUniforms(isETC);

        GLuint m_uniformX = p->getUniformLocationForName("sinWidth");
        p->setUniformLocationWith1f(m_uniformX, sinWidth);
        GLuint m_uniformY = p->getUniformLocationForName("sinHeight");
        p->setUniformLocationWith1f(m_uniformY, sinHeight);
        GLuint m_uniformT = p->getUniformLocationForName("uniformT");
        p->setUniformLocationWith2f(m_uniformT, ret->_quad.tl.texCoords.u, ret->_quad.tl.texCoords.v);


        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void CCFlagWaveSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
    if(this->getParent()){
        CCPoint p = this->getPosition();
        CCPoint p2 = this->getParent()->convertToWorldSpace(p);
        auto size = CCDirector::sharedDirector()->getWinSize();
        
        if(p2.x < -200 || p2.x > size.width + 200 || p2.y < -100 || p2.y > size.height + 100){
            
        }else{   
            CCSprite::draw(renderer,transform,flags);
        }
    }

}

NS_CC_END