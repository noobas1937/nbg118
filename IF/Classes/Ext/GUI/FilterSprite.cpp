//
//  FilterSprite.cpp
//  IF
//
//  Created by 李锐奇 on 15/2/4.
//
//

#include "FilterSprite.h"
#include "renderer/CCGLProgram.h"

FilterSprite::~FilterSprite()
{
    
}

FilterSprite* FilterSprite::create(const std::string& filename)
{
    FilterSprite *sprite = new (std::nothrow) FilterSprite();
//    auto texture = CCLoadSprite::loadResource(filename.c_str())->getTexture();
//    CCRect rect = CCRectZero;
//    
//    rect.size = texture->getContentSize();

    if (sprite && sprite->initWithFile(filename.c_str()))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

FilterSprite* FilterSprite::create(const std::string& filename, const CCRect& rect)
{
    FilterSprite *sprite = new (std::nothrow) FilterSprite();
    if (sprite && sprite->initWithFile(filename.c_str(), rect))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

//FilterSprite* FilterSprite::createWithTexture(Texture2D *pTexture)
//{
//    FilterSprite *sprite = new (std::nothrow) FilterSprite();
//    Rect rect = Rect::ZERO;
//    rect.size = pTexture->getContentSize();
//    if (sprite && sprite->initWithTexture(pTexture,rect))
//    {
//        sprite->autorelease();
//        return sprite;
//    }
//    CC_SAFE_DELETE(sprite);
//    return nullptr;
//}
//
//FilterSprite* FilterSprite::createWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
//{
//    FilterSprite *sprite = new (std::nothrow) FilterSprite();
//    if (sprite && sprite->initWithTexture(texture, rect))
//    {
//        sprite->autorelease();
//        return sprite;
//    }
//    CC_SAFE_DELETE(sprite);
//    return nullptr;
//}
//
FilterSprite* FilterSprite::createWithSpriteFrame(CCSpriteFrame *spriteFrame)
{
    FilterSprite *sprite = new (std::nothrow) FilterSprite();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

FilterSprite* FilterSprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteFrameName.c_str());
    return createWithSpriteFrame(frame);
}

bool FilterSprite::initWithTexture(CCTexture2D* pTexture, const CCRect& tRect){
    do{
        CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, tRect));
        bool isETC = false;
        CCGLProgram* pProgram = new CCGLProgram();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (pTexture && pTexture->getAlphaName()) {
            isETC = true;
            GLchar* pszFragSource =
            "#ifdef GL_ES \n \
            precision mediump float; \n \
            #endif \n \
            varying vec2 v_texCoord; \n \
            varying vec4 v_fragmentColor; \n \
            uniform mat4 filterMat; \n \
            uniform float filterVec; \n \
            void main(void) \n \
            { \n \
            vec4 color = texture2D(CC_Texture0, v_texCoord);        \n\
            color.a = texture2D(CC_Texture1, v_texCoord).r;         \n\
            vec4 value = v_fragmentColor * color;			        \n\
            value = filterMat*value; \n \
            if(value.a <= 0.0) discard; \n \
            gl_FragColor = value + vec4(filterVec, filterVec, filterVec, 0); \n \
            }";

            pProgram->initWithVertexShaderByteArray(ccPositionETC1A_vert, pszFragSource);
        }
        else
        {
            GLchar* pszFragSource =
            "#ifdef GL_ES \n \
            precision mediump float; \n \
            #endif \n \
            varying vec2 v_texCoord; \n \
            varying vec4 v_fragmentColor; \n \
            uniform mat4 filterMat; \n \
            uniform float filterVec; \n \
            void main(void) \n \
            { \n \
            vec4 value = v_fragmentColor*texture2D(u_texture, v_texCoord); \n \
            value = filterMat*value; \n \
            if(value.a <= 0.0) discard; \n \
            gl_FragColor = value + vec4(filterVec, filterVec, filterVec, 0); \n \
            }";
            pProgram->initWithVertexShaderByteArray(ccPositionETC1A_vert, pszFragSource);
        }
        
#else
        GLchar* pszFragSource =
        "#ifdef GL_ES \n \
        precision mediump float; \n \
        #endif \n \
        uniform sampler2D u_texture; \n \
        varying vec2 v_texCoord; \n \
        varying vec4 v_fragmentColor; \n \
        uniform mat4 filterMat; \n \
        uniform float filterVec; \n \
        void main(void) \n \
        { \n \
        vec4 value = v_fragmentColor*texture2D(u_texture, v_texCoord); \n \
        value = filterMat*value; \n \
        if(value.a <= 0.0) discard; \n \
        gl_FragColor = value + vec4(filterVec, filterVec, filterVec, 0); \n \
        }";
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
#endif
        this->setShaderProgram(pProgram);
        CC_SAFE_RELEASE_NULL(pProgram);
        CHECK_GL_ERROR_DEBUG();
        
        this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        CHECK_GL_ERROR_DEBUG();
        
        this->getShaderProgram()->link();
        CHECK_GL_ERROR_DEBUG();
        
        this->getShaderProgram()->updateUniforms(isETC);
        CHECK_GL_ERROR_DEBUG();
        
        return true;
    } while (0);
    return false;

//    do{
//        CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, tRect));
//        
//        GLchar* pszFragSource =
//        "#ifdef GL_ES \n \
//        precision mediump float; \n \
//        #endif \n \
//        uniform sampler2D u_texture; \n \
//        varying vec2 v_texCoord; \n \
//        varying vec4 v_fragmentColor; \n \
//        uniform mat4 filterMat; \n \
//        uniform float filterVec; \n \
//        void main(void) \n \
//        { \n \
//        vec4 value = v_fragmentColor*texture2D(u_texture, v_texCoord); \n \
//        value = filterMat*value; \n \
//        if(value.a <= 0.0) discard; \n \
//        gl_FragColor = value + vec4(filterVec, filterVec, filterVec, 0); \n \
//        }";
//        
//        auto program = new CCGLProgram();
//        program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
//        this->setShaderProgram(program);
//        program->release();
//        CHECK_GL_ERROR_DEBUG();
//        
//        this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
//        this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
//        this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
//        CHECK_GL_ERROR_DEBUG();
//        
//        this->getShaderProgram()->link();
//        CHECK_GL_ERROR_DEBUG();
//        
//        this->getShaderProgram()->updateUniforms(isETC);
//        CHECK_GL_ERROR_DEBUG();
//        
//        return true;
//    } while (0);
//    return false;
}

void  FilterSprite::setFilterMat(kmMat4 matrixArray)
{
    m_uSpriteFilter = matrixArray;
}

void FilterSprite::setParam(const ccColor3B &color, float bright, float contrast, float saturation){
    this->setColor(color);
    m_bright = bright;
    m_contrast = contrast;
    m_saturation = saturation;
    float lumR = 0.3086;
    float lumG = 0.6094;
    float lumB = 0.0820;
    float sr = (1 - m_saturation) * lumR;
    float sg = (1 - m_saturation) * lumG;
    float sb = (1 - m_saturation) * lumB;
    GLfloat  filterMat[16]= {
        m_contrast * (sr + m_saturation),  m_contrast * (sr),  m_contrast * sr,  0.0f,
        m_contrast * sg, m_contrast * (sg + m_saturation), m_contrast * sg, 0.0f,
        m_contrast * sb, m_contrast * sb, m_contrast * (sb + m_saturation), 0.0f,
        0.0f,  0.0f,  0.0f,  1.0f,
    };
    
    kmMat4Fill(&m_uSpriteFilter, filterMat);
}

void FilterSprite::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
{
//    auto glProgramState = getGLProgramState();
//    this->getShaderProgram()->setUniformMat4("fiterMat",m_uSpriteFilter);
    this->getShaderProgram()->use();
    this->getShaderProgram()->setUniformsForBuiltins();
    GLint matLocation = glGetUniformLocation(this->getShaderProgram()->getProgram(), "filterMat");
    this->getShaderProgram()->setUniformLocationWithMatrix4fv(matLocation, m_uSpriteFilter.m, 1);
//    float addValue = 1.0 - (m_bright + (1.0f - m_contrast) / 2.0f);
    float addValue = (m_bright + (1.0f - m_contrast) / 2.0f);
    GLint vecLocation = glGetUniformLocation(this->getShaderProgram()->getProgram(), "filterVec");
    this->getShaderProgram()->setUniformLocationWith1f(vecLocation, addValue);
    
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
    ccGLBlendFunc( _blendFunc.src, _blendFunc.dst );
    
    GL::bindTexture2D(_texture->getName());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (_texture->getAlphaName()) {
//        ccGLBindTexture2DN(0, _texture->getName());
//        ccGLBindTexture2DN(1, _texture->getAlphaName());
        GL::bindTexture2DN(1, _texture->getAlphaName());
    } else {
//        ccGLBindTexture2D( _texture->getName() );
    }
//#else
//    ccGLBindTexture2D( _texture->getName() );
#endif
    
#define kQuadSize sizeof(_quad.bl)
    long offset = (long)&_quad;
    
    // vertex
    int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    // color
    diff = offsetof( ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CC_INCREMENT_GL_DRAWS(1);
}
