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

#include "deprecated/CCString.h"

NS_CC_BEGIN

bool Effect::initGLProgramState(const std::string &fragmentFilename)
{
    auto fileUtiles = FileUtils::getInstance();
    auto fragmentFullPath = fileUtiles->fullPathForFilename(fragmentFilename);
    auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
    auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _fragSource = fragSource;
#endif
    
    _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _glprogramstate->retain();
    
    return _glprogramstate != nullptr;
}

Effect::Effect()
: _glprogramstate(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
                                                      [this](EventCustom*)
                                                      {
                                                          auto glProgram = _glprogramstate->getGLProgram();
                                                          glProgram->reset();
                                                          glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
                                                          glProgram->link();
                                                          glProgram->updateUniforms();
                                                      }
                                                      );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
}

Effect::~Effect()
{
    CC_SAFE_RELEASE_NULL(_glprogramstate);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}

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
: _defaultEffect(nullptr)
{
    _effects.reserve(2);
}

TMXTiledSprite::~TMXTiledSprite()
{
    for(auto &tuple : _effects)
    {
        std::get<1>(tuple)->release();
    }
    CC_SAFE_RELEASE(_defaultEffect);
}

void TMXTiledSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
#if CC_USE_CULLING
    // Don't do calculate the culling if the transform was not updated
    _insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
    
    if(_insideBounds)
#endif
    {
        // negative effects: order < 0
        int idx = 0;
        for (auto &effect : _effects)
        {
            if (std::get<0>(effect) >=0)
                break;
            QuadCommand &q = std::get<2>(effect);
            q.init(_globalZOrder, _texture->getName(), std::get<1>(effect)->getGLProgramState(), _blendFunc, &_quad, 1, transform, flags);
            renderer->addCommand(&q);
            idx++;
        }
        
        // normal effect: order == 0
        _trianglesCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, _polyInfo.triangles, transform, flags);
        renderer->addCommand(&_trianglesCommand);
        
        // postive effects: oder >= 0
        for (auto it = std::begin(_effects)+idx; it != std::end(_effects); ++it)
        {
            QuadCommand &q = std::get<2>(*it);
            q.init(_globalZOrder, _texture->getName(), std::get<1>(*it)->getGLProgramState(), _blendFunc, &_quad, 1, transform, flags);
            renderer->addCommand(&q);
            idx++;
        }
    }
    
//#if CC_USE_CULLING
//    // Don't do calculate the culling if the transform was not updated
//    _insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
//    
//    if(_insideBounds)
//#endif
//    {
//        _trianglesCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, _polyInfo.triangles, transform, flags);
//        _trianglesCommand.setTextureID2(_texture->getAlphaName());
//        renderer->addCommand(&_trianglesCommand);
//    }
}

NS_CC_END
