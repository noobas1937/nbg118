//
//  CCClipNode.h
//  ccgoe
//
//  Created by 谢文杰 on 12-12-7.
//
//

#ifndef __ccgoe__CCClipNode__
#define __ccgoe__CCClipNode__

#include "cocos-ext.h"
#include "CCProgressAppear.h"

NS_CC_EXT_BEGIN

class CCClipNode :public CCNode
{
public:
    static CCClipNode* create(int width, int height);
    static CCClipNode* create(const CCSize& size);
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
private:
    void beforeDraw();
    void onBeforeDraw();
    void afterDraw();
    void onAfterDraw();

    CustomCommand _beforeDrawCommand;
    CustomCommand _afterDrawCommand;
    Rect _parentScissorRect;
    bool _scissorRestored;
};

class CCClipSprite : public CCSprite
{
public:

    CCClipSprite():
    CCSprite()
    ,m_pDirection(NONE_DIRECTION)
    ,m_pX(0.0)
    ,m_pY(0.0)
    {}
    
    static CCClipSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    
    CC_SYNTHESIZE(ClipProgressDirection,m_pDirection,ProgressDirection);
    CC_SYNTHESIZE(float,m_pX,ProgressX);
    CC_SYNTHESIZE(float,m_pY,ProgressY);
    
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
};

NS_CC_EXT_END

#endif /* defined(__ccgoe__CCClipNode__) */
