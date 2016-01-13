//
//  CCLineSprite.cpp
//  IF
//
//  Created by 邹 程 on 14-4-10.
//
//

#include "CCLineSprite.h"

static float _halfDisplayWidth = 800.0;
static float _halfDisplayEnd = 300.0;


NS_CC_BEGIN

bool CCLineSprite::init(CCSpriteFrame* spf) {
    if (!spf || !spf->getTexture()) {
        return false;
    }
    
    m_spRect = spf->getRect();
    m_spIsRotated = spf->isRotated();
    m_spHalfWidth = m_spRect.size.width / 2;
    auto texture = spf->getTexture();
    
    m_lineWidth = m_start.getDistance(m_end);
    
    if (m_lineHeight == FLT_MIN) {
        m_lineHeight = m_spRect.size.height;
    }
    
    m_atlasScale = m_lineHeight / m_spRect.size.height;
    auto perWidth = m_atlasScale * m_spHalfWidth;
    m_itemCount = m_lineWidth / perWidth;
    m_spRemainWidth = fmodf(m_lineWidth, perWidth) / m_atlasScale;
    
    m_atlas = new CCAtlasNode();
    if (!m_atlas->initWithTexture(texture, m_spHalfWidth, m_spRect.size.height, m_itemCount + 1)) {
        return false;
    }
    
    auto vs = ccpSub(m_start, m_end);
    m_atlas->setRotation(180.0f * (1 - atan2f(vs.y, vs.x)/ M_PI));
    m_atlas->setScale(m_atlasScale);
    
    this->addChild(m_atlas);
    this->setPosition(m_start);
    
    initAtlasQuad();
    return true;
}

void CCLineSprite::initAtlasQuad() {
    
    updateAtlasTexCoords();
    
    auto quadsToDraw = m_atlas->getTextureAtlas()->getQuads();
    
    ccColor4B tmpColor1 = { 255, 255, 255, 255 };
    ccColor4B tmpColor2 = { 255, 255, 255, 255 };
    bool needFixAlpha = false;
    if (m_isHalfDisplay && m_lineWidth > _halfDisplayWidth) {
        needFixAlpha = true;
    }
    
    for (int i=0; i<m_itemCount; ++i) {
        float x1 = i*m_spHalfWidth;
        float y1 = -m_spRect.size.height/2;
        float x2 = x1 + m_spHalfWidth;
        float y2 = y1 + m_spRect.size.height;
        
        quadsToDraw[i].bl.vertices = vertex3(x1, y1, 0);
        quadsToDraw[i].br.vertices = vertex3(x2, y1, 0);
        quadsToDraw[i].tl.vertices = vertex3(x1, y2, 0);
        quadsToDraw[i].tr.vertices = vertex3(x2, y2, 0);
        
        if (needFixAlpha) {
            float alphaPercent1 = FLT_MIN;
            float alphaPercent2 = FLT_MIN;
            if (x1 < _halfDisplayEnd) {
                alphaPercent1 = (_halfDisplayEnd - x1) / _halfDisplayEnd;
                alphaPercent2 = (_halfDisplayEnd - x1 - m_spHalfWidth) / _halfDisplayEnd;
            } else if (x1 > m_lineWidth - _halfDisplayEnd) {
                alphaPercent1 = (x1 - m_lineWidth + _halfDisplayEnd) / _halfDisplayEnd;
                alphaPercent2 = (x1 + m_spHalfWidth - m_lineWidth + _halfDisplayEnd) / _halfDisplayEnd;
            }
            
            alphaPercent1 = MIN(MAX(FLT_MIN, alphaPercent1),1.0);
            alphaPercent2 = MIN(MAX(FLT_MIN, alphaPercent2),1.0);
            GLubyte colorValue1 = alphaPercent1 * 255;
            GLubyte colorValue2 = alphaPercent2 * 255;
            tmpColor1 = {colorValue1,colorValue1,colorValue1,colorValue1};
            tmpColor2 = {colorValue2,colorValue2,colorValue2,colorValue2};
        }
        
        quadsToDraw[i].bl.colors = tmpColor1;
        quadsToDraw[i].br.colors = tmpColor2;
        quadsToDraw[i].tl.colors = tmpColor1;
        quadsToDraw[i].tr.colors = tmpColor2;
    }
    
    // final part quad
    float x1 = m_itemCount*m_spHalfWidth;
    float y1 = -m_spRect.size.height/2;
    float x2 = x1 + m_spRemainWidth;
    float y2 = y1 + m_spRect.size.height;
    
    quadsToDraw[m_itemCount].bl.vertices = vertex3(x1, y1, 0);
    quadsToDraw[m_itemCount].br.vertices = vertex3(x2, y1, 0);
    quadsToDraw[m_itemCount].tl.vertices = vertex3(x1, y2, 0);
    quadsToDraw[m_itemCount].tr.vertices = vertex3(x2, y2, 0);

    quadsToDraw[m_itemCount].bl.colors = tmpColor1;
    quadsToDraw[m_itemCount].br.colors = tmpColor2;
    quadsToDraw[m_itemCount].tl.colors = tmpColor1;
    quadsToDraw[m_itemCount].tr.colors = tmpColor2;

}

void CCLineSprite::updateAtlasTexCoords(float percent) {
    
    auto rect = CC_RECT_POINTS_TO_PIXELS(m_spRect);
    
    float atlasWidth = (float)m_atlas->getTexture()->getPixelsWide();
    float atlasHeight = (float)m_atlas->getTexture()->getPixelsHigh();
    
    auto getQuad = [&](bool isFinalQuad) {
        
        float left, right, top, bottom;
        
        float width = isFinalQuad ? m_spRemainWidth : m_spHalfWidth;
        
        ccV3F_C4B_T2F_Quad quad;
        
        if (m_spIsRotated) {
            left    = rect.origin.x / atlasWidth;
            right   = (rect.origin.x+rect.size.height) / atlasWidth;
            top     = (rect.origin.y + m_spHalfWidth * (1.0 - percent)) / atlasHeight;
            bottom  = (rect.origin.y + m_spHalfWidth * (1.0 - percent) + width) / atlasHeight;
            
            quad.bl.texCoords.u = left;
            quad.bl.texCoords.v = top;
            quad.br.texCoords.u = left;
            quad.br.texCoords.v = bottom;
            quad.tl.texCoords.u = right;
            quad.tl.texCoords.v = top;
            quad.tr.texCoords.u = right;
            quad.tr.texCoords.v = bottom;
            
        } else {
            left    = (rect.origin.x + m_spHalfWidth * (1.0 - percent)) / atlasWidth;
            right   = (rect.origin.x + m_spHalfWidth * (1.0 - percent) + width) / atlasWidth;
            top     = rect.origin.y / atlasHeight;
            bottom  = (rect.origin.y + rect.size.height) / atlasHeight;
            
            quad.bl.texCoords.u = left;
            quad.bl.texCoords.v = bottom;
            quad.br.texCoords.u = right;
            quad.br.texCoords.v = bottom;
            quad.tl.texCoords.u = left;
            quad.tl.texCoords.v = top;
            quad.tr.texCoords.u = right;
            quad.tr.texCoords.v = top;
        }
        
        return quad;
    };
    
    ccV3F_C4B_T2F_Quad quad = getQuad(false);
    
    auto atlasQuad = m_atlas->getTextureAtlas()->getQuads();
    
    for (int i=0; i<m_itemCount; ++i) {
        atlasQuad[i].bl.texCoords = quad.bl.texCoords;
        atlasQuad[i].br.texCoords = quad.br.texCoords;
        atlasQuad[i].tl.texCoords = quad.tl.texCoords;
        atlasQuad[i].tr.texCoords = quad.tr.texCoords;
    }
    
    // update final quad
    quad = getQuad(true);
    atlasQuad[m_itemCount].bl.texCoords = quad.bl.texCoords;
    atlasQuad[m_itemCount].br.texCoords = quad.br.texCoords;
    atlasQuad[m_itemCount].tl.texCoords = quad.tl.texCoords;
    atlasQuad[m_itemCount].tr.texCoords = quad.tr.texCoords;
}

void CCLineSprite::onEnter() {
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void CCLineSprite::onExit() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
    m_atlas->release();
    CCNode::onExit();
}

void CCLineSprite::changeUpdatingSpeed(float speed) {
    if (fabsf(speed - m_updatingSpeed) < 0.001) {
        return;
    }
    m_updatingSpeed = speed;
}

void CCLineSprite::update(float delta) {
    if (m_updatingSpeed == FLT_MIN) {
        return;
    }
    m_percentStack += delta * m_updatingSpeed / m_spHalfWidth;
    m_percentStack = m_percentStack >= 1.000 ? m_percentStack - 1.000 : m_percentStack;
    updateAtlasTexCoords(m_percentStack);
}

NS_CC_END