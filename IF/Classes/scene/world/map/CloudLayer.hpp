//
//  CloudLayer.hpp
//  IF
//
//  Created by gj on 10/15/15.
//
//

#ifndef CloudLayer_hpp
#define CloudLayer_hpp

#include "cocos2d.h"

USING_NS_CC;

class CloudLayer : public CCParallaxNode {
public:
    static CloudLayer* create() {
        CloudLayer *pRet = new CloudLayer();
        pRet->autorelease();
        return pRet;
    }
    
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
};

#endif /* CloudLayer_hpp */
