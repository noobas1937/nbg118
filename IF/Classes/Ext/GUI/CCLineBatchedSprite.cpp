//
//  CCLineBatchedSprite.cpp
//  IF
//
//  Created by Luojunfeng on 14/12/25.
//
//

#include "CCLineBatchedSprite.h"

NS_CC_BEGIN

#define STRINGIFY(A)  #A
const static GLchar* UVAnimationVert = STRINGIFY
(
#ifdef GL_ES
precision highp float;
#endif

attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
});

const static GLchar* UVAnimationFrag = STRINGIFY
(
#ifdef GL_ES
precision highp float;
#endif
 
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
//uniform sampler2D CC_Texture0;

void main()
{
    gl_FragColor =  v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
});

bool CCLineBatchedSprite::init(CCTexture2D* texture) {
    if (!texture) {
        return false;
    }

    if (!CCSprite::initWithTexture(texture)) {
        return false;
    }
    
    m_lineWidth = m_start.getDistance(m_end);
    
    if (m_lineHeight == FLT_MIN) {
        m_lineHeight = texture->getPixelsHigh();
    }
    
    Texture2D::TexParams texParams = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    texture->setTexParameters(&texParams);
    
    m_spScale = m_lineHeight / texture->getPixelsHigh();
    m_spHalfWidth = texture->getPixelsWide() / 2;
    CCPoint vs = ccpSub(m_start, m_end);
    this->setRotation(180.0f * (1 - atan2f(vs.y, vs.x)/ M_PI));
    this->setAnchorPoint(ccp(0,0.5));
    this->setTextureRect(CCRectZero);
    this->setScale(m_spScale);
    this->setPosition(m_start);
    
    unsigned int cellW = texture->getPixelsWide();
    int totalW = (int)(m_lineWidth / m_spScale);
    int currentW = 0;
    int w = 0;
    int cellX = 0;
    int cellY = 0;
    while (w < MAX_LENGTH_PER_LINE && currentW <= totalW) {
        w += cellW;
        currentW +=cellW;
        if(w >= MAX_LENGTH_PER_LINE){
            auto cell = CCLineBatchedSpriteCell::create(m_updatingSpeed, m_spHalfWidth, CCRect(0, 0, w, texture->getPixelsHigh()));
            cell->setAnchorPoint(ccp(0, 0.5));
            cell->setPosition(ccp(cellX, cellY));
            cellX += MAX_LENGTH_PER_LINE;
            this->addChild(cell);
            if(currentW > totalW){
                break;
            }
            w = 0;
        }
        if(currentW > totalW){
            float lineW = w - (currentW - totalW);
            auto cell = CCLineBatchedSpriteCell::create(m_updatingSpeed, m_spHalfWidth, CCRect(0, 0, lineW, texture->getPixelsHigh()));
            cell->setAnchorPoint(ccp(0, 0.5));
            cell->setPosition(ccp(cellX, cellY));
            this->addChild(cell);
            cellX += lineW;
            break;
        }
    }
    return true;
}

void CCLineBatchedSprite::onEnter() {
    CCNode::onEnter();
}

void CCLineBatchedSprite::onExit() {
    CCNode::onExit();
}

void CCLineBatchedSprite::setColor(const ccColor3B& color3){
    CCSprite::setColor(color3);
    auto arr = this->getChildren();
    for (auto child : arr)
    {
        auto cell = dynamic_cast<CCLineBatchedSpriteCell*>(child);
        if(cell){
            cell->setColor(color3);
        }
    }
}

void CCLineBatchedSprite::changeUpdatingSpeed(float speed) {
    if (fabsf(speed - m_updatingSpeed) < 0.001) {
        return;
    }
    m_updatingSpeed = speed;
    auto arr = this->getChildren();
    for (auto child : arr)
    {
        auto cell = dynamic_cast<CCLineBatchedSpriteCell*>(child);
        if(cell){
            cell->setSpeed(speed);
        }
    }
//
//    while (arr && i < arr->count()) {
//        auto cell = dynamic_cast<CCLineBatchedSpriteCell*>(arr->objectAtIndex(i));
//        if(cell){
//            cell->setSpeed(speed);
//        }
//        i++;
//    }
}

//-----
CCLineBatchedSpriteCell *CCLineBatchedSpriteCell::create(float updatingSpeed, float halfW, CCRect rect){
    CCLineBatchedSpriteCell *ret = new CCLineBatchedSpriteCell(updatingSpeed, halfW, rect);
    if(ret && ret->init()){
        
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CCLineBatchedSpriteCell::init(){
    return CCSprite::init();
}

void CCLineBatchedSpriteCell::setSpeed(float speed){
    m_updatingSpeed = speed;
}

void CCLineBatchedSpriteCell::onEnter(){
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void CCLineBatchedSpriteCell::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
    CCNode::onExit();
}

void CCLineBatchedSpriteCell::update(float speed){
    if (m_updatingSpeed == FLT_MIN) {
        return;
    }
    
    m_percentStack += speed * m_updatingSpeed / m_spHalfWidth;
    m_percentStack = m_percentStack >= 1.000 ? m_percentStack - 1.000 : m_percentStack;
    
    float offset = m_percentStack * m_spHalfWidth * 2;
    m_rect.origin.x = -(int)(offset);
    this->setTextureRect(m_rect);
}

//-----

bool CCLineBatchedLayer::init() {
    this->batchlayer = CCSpriteBatchNode::create(FOOTPRINTIMGPATH, 50);
    CCGLProgram* p = CCShaderCache::sharedShaderCache()->addCustomGLProgram("LineBatchedLayer", UVAnimationVert, UVAnimationFrag);
    this->batchlayer->setShaderProgram(p);
    CCNode::addChild(this->batchlayer);
#if CC_ENABLE_CACHE_TEXTURE_DATA
    // Need to listen the event only when not use batchnode, because it will use VBO
    auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
        /** listen the event that renderer was recreated on Android/WP8 */
        CCLOG("Tongyue LineBatchedLayer reload");
        CCGLProgram* p = CCShaderCache::sharedShaderCache()->reloadCustomGLProgram("LineBatchedLayer", UVAnimationVert, UVAnimationFrag);
        //this->batchlayer->setShaderProgram(p);
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
    return true;
}

void CCLineBatchedLayer::addChild(CCLineBatchedSprite* child) {
    batchlayer->addChild(child);
}

CCNode* CCLineBatchedLayer::getChildByTag(int tag) {
    return batchlayer->getChildByTag(tag);
}

void CCLineBatchedLayer::removeAllChildren() {
    batchlayer->removeAllChildren();
}

NS_CC_END