//
//  CCLineBatchedSprite.h
//  IF
//
//  Created by Luojunfeng on 14/12/25.
//
//

#ifndef __IF__CCLineBatchedSprite__
#define __IF__CCLineBatchedSprite__

#include "cocos2d.h"

NS_CC_BEGIN

#define FOOTPRINTIMGPATH "World/footprintsingle.png"
#define MAX_LENGTH_PER_LINE 128 * 120

class CCLineBatchedSpriteCell : public CCSprite{
public:
    static CCLineBatchedSpriteCell *create(float updatingSpeed, float halfW, CCRect rect);
    
    CCLineBatchedSpriteCell(float updatingSpeed, float halfW, CCRect rect)
    : m_rect(rect)
    , m_percentStack(0.0f)
    , m_updatingSpeed(updatingSpeed)
    , m_spHalfWidth(halfW)
    {
        
    }
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float speed);
    void setSpeed(float speed);
private:
    CCRect m_rect;
    float m_percentStack;
    float m_updatingSpeed;
    float m_spHalfWidth;
};

class CCLineBatchedSprite : public CCSprite {
public:
    CCLineBatchedSprite(const CCPoint& start,const CCPoint& end,float lineHeight,float updatingSpeed):
    m_start(start)
    ,m_end(end)
    ,m_lineHeight(lineHeight)
    ,m_updatingSpeed(updatingSpeed)
    ,m_percentStack(0)
    {
    }
    
    virtual ~CCLineBatchedSprite() {}
    
    static CCLineBatchedSprite* create(const CCPoint& start, const CCPoint &end, float lineHeight = FLT_MIN, float uploadingSpeed = FLT_MIN) {
        auto ret = new CCLineBatchedSprite(start, end, lineHeight, uploadingSpeed);
        if (ret && ret->init(CCTextureCache::sharedTextureCache()->addImage(FOOTPRINTIMGPATH))) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    bool init(CCTexture2D *texture);
    
    void changeUpdatingSpeed(float speed);
    virtual void setColor(const ccColor3B& color3);
    virtual void onEnter();
    virtual void onExit();
//    virtual void update(float delta);
private:
    CCPoint m_start;
    CCPoint m_end;
    float m_lineWidth;
    float m_lineHeight;
    float m_spHalfWidth;
    float m_percentStack;
    float m_updatingSpeed;
    float m_spScale;
    CCRect m_rect;
};

class CCLineBatchedLayer : public CCNode {
public:
    CCLineBatchedLayer():CCNode() {}
    virtual ~CCLineBatchedLayer() {}
    
    static CCLineBatchedLayer* create(void) {
        CCLineBatchedLayer * pRet = new CCLineBatchedLayer();
        if (pRet && pRet->init()) {
            pRet->autorelease();
        } else {
            CC_SAFE_DELETE(pRet);
        }
        return pRet;
    }
    
    bool init();
    
    virtual void addChild(CCLineBatchedSprite* child);
    
    virtual CCNode * getChildByTag(int tag);
    
    virtual void removeAllChildren();
    
private:
    CCSpriteBatchNode* batchlayer;
};

NS_CC_END

#endif /* defined(__IF__CCLineBatchedSprite__) */
