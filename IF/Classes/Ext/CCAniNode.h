//
//  CCAniNode.h
//  IF
//
//  Created by fubin on 14-6-6.
//
//

#ifndef IF_CCAniNode_h
#define IF_CCAniNode_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"
#include "CCIFTouchNode.h"
NS_CC_EXT_BEGIN

class CCAniNode : public CCIFTouchNode {
public:
    CCAniNode():mActionManager(NULL){};
    virtual ~CCAniNode(){}
    void setAnimationManager(cocosbuilder::CCBAnimationManager *pAnimationManager);
    cocosbuilder::CCBAnimationManager * getAnimationManager();
protected:
    cocosbuilder::CCBAnimationManager *mActionManager;
};

NS_CC_EXT_END

#endif
