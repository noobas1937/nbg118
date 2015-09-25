//
//  Titan3DSprite.cpp
//  IF
//
//  Created by guojiang on 8/19/15.
//
//

#include "Titan3DSprite.h"

using namespace cocos2d;

// uniform names for shader
static const char * DURATION = "duration";
static const char * CAUSTICS = "caustics";
static const char * CAUSTICS_COLOR = "caustics_color";


Titan3DSprite* Titan3DSprite::create(const std::string &modelPath)
{
    if (modelPath.length() < 4)
        CCASSERT(false, "invalid filename for Sprite3D");
    
    auto sprite = new (std::nothrow) Titan3DSprite();
    if (sprite && sprite->initWithFile(modelPath))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Titan3DSprite::setEmittingShaderDuration(float speed, float min, float max)
{
    m_shining_duration_step = speed;
    m_min_duration = min;
    m_max_duration = max;
}

void Titan3DSprite::applyEmittingShader(const Vec3& caustics_color, const std::string& textureLight, const std::string& vShaderFilename, const std::string& fShaderFilename)
{
    //create and set our custom shader
    auto shader = GLProgram::createWithFilenames(vShaderFilename, fShaderFilename);
    auto glpstate = GLProgramState::create(shader);
    m_shining_duration = 0;
    glpstate->setUniformFloat(DURATION, m_shining_duration);
    
    //create the second texture
    releaseTex();
    m_shining_texture = Director::getInstance()->getTextureCache()->addImage(textureLight);
    m_shining_texture->retain();
    Texture2D::TexParams tRepeatParams;
    tRepeatParams.magFilter = GL_NEAREST;
    tRepeatParams.minFilter = GL_NEAREST;
    tRepeatParams.wrapS = GL_REPEAT;
    tRepeatParams.wrapT = GL_REPEAT;
    m_shining_texture->setTexParameters(tRepeatParams);
    //pass the texture sampler to our custom shader
    glpstate->setUniformTexture(CAUSTICS, m_shining_texture);
    Vec4 paramColor = Vec4(caustics_color.x / 255, caustics_color.y / 255, caustics_color.z / 255, 1);
    glpstate->setUniformVec4(CAUSTICS_COLOR, paramColor);
    
    long offset = 0;
    auto attributeCount = getMesh()->getMeshVertexAttribCount();
    for (auto k = 0; k < attributeCount; k++)
    {
        auto meshattribute = getMesh()->getMeshVertexAttribute(k);
        glpstate->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
                                           meshattribute.size,
                                           meshattribute.type,
                                           GL_FALSE,
                                           getMesh()->getVertexSizeInBytes(),
                                           (GLvoid*)offset);
        offset += meshattribute.attribSizeBytes;
    }
    
    setGLProgramState(glpstate);
    
    m_fade_in = true;
    schedule(schedule_selector(Titan3DSprite::titanUpdate));
}

void Titan3DSprite::titanUpdate(float dt)
{
    auto glprogramstate = getGLProgramState();
    if (!glprogramstate) return;
    
    if (m_fade_in)
    {
        m_shining_duration += m_shining_duration_step * dt;
        if (m_shining_duration > m_max_duration)
        {
            m_shining_duration = m_max_duration;
            m_fade_in = false;
        }
    }
    else
    {
        m_shining_duration -= m_shining_duration_step * dt;
        if (m_shining_duration < m_min_duration)
        {
            m_shining_duration = m_min_duration;
            m_fade_in = true;
        }
    }
    
    //pass the result to shader
    glprogramstate->setUniformFloat(DURATION, m_shining_duration);
}

#pragma make * test shader

void Titan3DSprite::applySeaShader(const std::string& texture1, const std::string& textureLight, const std::string& vShaderFilename,
                                   const std::string& fShaderFilename)
{
    auto glprogram = GLProgram::createWithFilenames(vShaderFilename, fShaderFilename);
    auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    setGLProgramState(glprogramstate);
    
    auto textrue1 = Director::getInstance()->getTextureCache()->addImage(texture1);
    glprogramstate->setUniformTexture("u_texture1", textrue1);
    
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
    
    long offset = 0;
    auto attributeCount = getMesh()->getMeshVertexAttribCount();
    for (auto k = 0; k < attributeCount; k++)
    {
        auto meshattribute = getMesh()->getMeshVertexAttribute(k);
        glprogramstate->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
                                               meshattribute.size,
                                               meshattribute.type,
                                               GL_FALSE,
                                               getMesh()->getVertexSizeInBytes(),
                                               (GLvoid*)offset);
        offset += meshattribute.attribSizeBytes;
    }
    
    m_LightAni.x = m_LightAni.y = 0;
    
    schedule(schedule_selector(Titan3DSprite::updateSeaShader));
}

void Titan3DSprite::updateSeaShader(float dt)
{
    auto glprogramstate = getGLProgramState();
    if (glprogramstate)
    {
        m_LightAni.x += 0.01;
        if (m_LightAni.x > 1.0)
        {
            m_LightAni.x -= 1.0;
        }
        m_LightAni.y += 0.01;
        if (m_LightAni.y > 1.0)
        {
            m_LightAni.y -= 1.0;
        }
        glprogramstate->setUniformVec2("v_animLight", m_LightAni);
    }
}
