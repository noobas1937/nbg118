//
//  CCLineSprite.h
//  IF
//
//  Created by 邹 程 on 14-4-10.
//
//

#ifndef __IF__CCLineSprite__
#define __IF__CCLineSprite__

#include "cocos2d.h"

NS_CC_BEGIN

class CCLineSprite : public CCNode {
public:
    CCLineSprite(const CCPoint& start,const CCPoint& end,float lineHeight,float updatingSpeed,bool ishalfDisplay) :
    m_atlas(NULL)
    ,m_start(start)
    ,m_end(end)
    ,m_lineHeight(lineHeight)
    ,m_updatingSpeed(updatingSpeed)
    ,m_itemCount(0)
    ,m_atlasScale(1.0)
    ,m_percentStack(0.0)
    ,m_isHalfDisplay(ishalfDisplay)
    {}
    
    virtual ~CCLineSprite() {}
    
    static CCLineSprite* create(CCSpriteFrame* spf,const CCPoint& start,const CCPoint& end,float lineHeight = FLT_MIN,float updatingSpeed = FLT_MIN,bool ishalfDisplay = false) {
        auto ret = new CCLineSprite(start,end,lineHeight,updatingSpeed,ishalfDisplay);
        if (ret && ret->init(spf)) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    bool init(CCSpriteFrame* sprite);
    
    void changeUpdatingSpeed(float speed);
    
    CCAtlasNode* getAtlasNode() {
        return m_atlas;
    }
    
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float delta);
    
protected:
    void initAtlasQuad();
    void updateAtlasTexCoords(float percent = 0.0);
    
private:
    CCRect m_spRect;
    float m_spRemainWidth;
    bool m_spIsRotated;
    float m_spHalfWidth;
    CCAtlasNode* m_atlas;
    CCPoint m_start;
    CCPoint m_end;
    float m_lineWidth;
    float m_lineHeight;
    float m_updatingSpeed;
    unsigned int m_itemCount;
    float m_atlasScale;
    
    bool m_isHalfDisplay;
    
    float m_percentStack;
};

NS_CC_END

#endif /* defined(__IF__CCLineSprite__) */
