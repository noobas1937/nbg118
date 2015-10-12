//
//  CCTMXTiledSprite.cpp
//  cocos2d_libs
//
//  Created by gj on 10/12/15.
//
//

#include "CCTMXTiledSprite.hpp"

#include <algorithm>

#include "CCFileUtils.h"

#include "2d/CCSpriteBatchNode.h"
#include "2d/CCAnimationCache.h"
#include "2d/CCSpriteFrame.h"
#include "2d/CCSpriteFrameCache.h"
#include "renderer/CCTextureCache.h"
#include "renderer/CCTexture2D.h"
#include "renderer/CCRenderer.h"
#include "base/CCDirector.h"
#include "renderer/ccShaders.h"
#include "renderer/ccGLStateCache.h"

#include "deprecated/CCString.h"

NS_CC_BEGIN

TMXTiledSprite* TMXTiledSprite::createWithTexture(Texture2D *texture)
{
    TMXTiledSprite *sprite = new (std::nothrow) TMXTiledSprite();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

TMXTiledSprite* TMXTiledSprite::createWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
{
    TMXTiledSprite *sprite = new (std::nothrow) TMXTiledSprite();
    if (sprite && sprite->initWithTexture(texture, rect, rotated))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

TMXTiledSprite* TMXTiledSprite::create(const std::string& filename)
{
    TMXTiledSprite *sprite = new (std::nothrow) TMXTiledSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

TMXTiledSprite* TMXTiledSprite::create(const PolygonInfo& info)
{
    TMXTiledSprite *sprite = new (std::nothrow) TMXTiledSprite();
    if(sprite && sprite->initWithPolygon(info))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

TMXTiledSprite* TMXTiledSprite::create(const std::string& filename, const Rect& rect)
{
    TMXTiledSprite *sprite = new (std::nothrow) TMXTiledSprite();
    if (sprite && sprite->initWithFile(filename, rect))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

TMXTiledSprite* TMXTiledSprite::createWithSpriteFrame(SpriteFrame *spriteFrame)
{
    TMXTiledSprite *sprite = new (std::nothrow) TMXTiledSprite();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

TMXTiledSprite* TMXTiledSprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    
    return createWithSpriteFrame(frame);
}

TMXTiledSprite* TMXTiledSprite::create()
{
    TMXTiledSprite *sprite = new (std::nothrow) TMXTiledSprite();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

TMXTiledSprite::TMXTiledSprite()
{

}

TMXTiledSprite::~TMXTiledSprite()
{
    
}

void TMXTiledSprite::applySeaShader(const std::string& textureLight,
                                     const std::string& fShaderFilename)
{
    auto fileUtiles = FileUtils::getInstance();
    auto fragmentFullPath = fileUtiles->fullPathForFilename(fShaderFilename);
    auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
    auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_vert, fragSource.c_str());
    auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    setGLProgramState(glprogramstate);
    
    auto textrue2 = Director::getInstance()->getTextureCache()->addImage(textureLight);
    glprogramstate->setUniformTexture("u_lightTexture", textrue2);
    
    Texture2D::TexParams tRepeatParams;
    tRepeatParams.magFilter = GL_LINEAR_MIPMAP_LINEAR;
    tRepeatParams.minFilter = GL_LINEAR;
    tRepeatParams.wrapS = GL_REPEAT;
    tRepeatParams.wrapT = GL_REPEAT;
    textrue2->setTexParameters(tRepeatParams);
    
    Vec4 tLightColor(1.0, 1.0, 1.0, 1.0);
    glprogramstate->setUniformVec4("v_LightColor", tLightColor);
}

void TMXTiledSprite::onEnter()
{
    Sprite::onEnter();
    scheduleUpdate();
}

void TMXTiledSprite::onExit()
{
    unscheduleUpdate();
    Sprite::onExit();
}

void TMXTiledSprite::update(float dt)
{
    Sprite::update(dt);
    
    //更新u
    if (_AutoScrollU)
    {
        _AutoScrollCountU += dt * _AutoScrollSpeedU;
    }
    
    //更新v
    if (_AutoScrollV)
    {
        _AutoScrollCountV += dt * _AutoScrollSpeedV;
    }
    
    //如果超出范围从0开始
    if (_AutoScrollCountU > 1.0 || _AutoScrollCountU < -1.0)
    {
        _AutoScrollCountU = 0;
    }
    
    if (_AutoScrollCountV > 1.0 || _AutoScrollCountV < -1.0)
    {
        _AutoScrollCountV = 0;
    }
}

void TMXTiledSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if (getGLProgramState())
    {
        _customCommand.init(_globalZOrder);
        _customCommand.func = CC_CALLBACK_0(TMXTiledSprite::onDraw, this, transform, flags);
        renderer->addCommand(&_customCommand);
    }
    else
    {
        Sprite::draw(renderer, transform, flags);
    }
}

void TMXTiledSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
    auto glProgramState = getGLProgramState();
    glProgramState->setUniformVec2("texOffset", cocos2d::Vec2(_AutoScrollCountU, _AutoScrollCountV));
    glProgramState->apply(transform);
    
    GL::blendFunc( _blendFunc.src, _blendFunc.dst );
    
    GL::bindTexture2D( _texture->getName() );
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX );
    
#define kQuadSize sizeof(_quad.bl)
    size_t offset = (size_t)&_quad;
    
    // vertex
    int diff = offsetof( V3F_C4B_T2F, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( V3F_C4B_T2F, texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    // color
    diff = offsetof( V3F_C4B_T2F, colors);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    CHECK_GL_ERROR_DEBUG();
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,4);
}

NS_CC_END
