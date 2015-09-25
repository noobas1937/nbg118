//
//  IFSkeletonBatchLayer.h
//  IF
//
//  Created by 李锐奇 on 15/3/16.
//
//

#ifndef __IF__IFSkeletonBatchLayer__
#define __IF__IFSkeletonBatchLayer__

#include "CCCommonUtils.h"
#include "IFSkeletonNode.h"
#include "spine/PolygonBatch.h"

class IFSkeletonBatchLayer : public CCNode{
public:
    CREATE_FUNC(IFSkeletonBatchLayer);
//    void addChild(CCNode *render);
//    void addChild(CCNode *render, int zOrder);
//    void removeChild(CCNode *node, bool cleanUp = true);
//    ~IFSkeletonBatchLayer();
//    virtual void removeAllChildrenWithCleanup(bool b = true);
private:
//    virtual void update(float delayTime);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
//    CCSafeObject<CCArray> m_allRender;
    CCSafeObject<spine::PolygonBatch> m_batch;
    virtual void draw(Renderer *renderer, const kmMat4& transform, uint32_t transformFlags);
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
    virtual void drawSkeleton (const cocos2d::Mat4& transform, uint32_t transformFlags);
    int m_additive;
    CustomCommand _drawCommand;
    
};

#endif /* defined(__IF__IFSkeletonBatchLayer__) */
