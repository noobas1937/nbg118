//
//  ClippingNodeExt.h
//  IF
//
//  Created by zhaohf on 13-10-14.
//
//

#ifndef __IF__ClippingNodeExt__
#define __IF__ClippingNodeExt__

#include "cocos2d.h"
#include "cocos-ext.h"

//实例
//CCClippingNode* clipper = ClippingNodeExt::createCircle(CCSizeMake(100, 100),CCPointMake(0, 0));
//CCSprite *content = CCSprite::create(s_back2);
//content->setTag( kTagContentNode );
//content->setAnchorPoint(  ccp(0.5, 0.5) );
//content->setPosition( ccp(clipper->getContentSize().width / 2, clipper->getContentSize().height / 2) );
//clipper->addChild(content);
//this->addChild(clipper);

class ClippingNodeExt : public cocos2d::CCObject
{
    
    
public:
    virtual ~ClippingNodeExt();
    ClippingNodeExt();
    static cocos2d::CCClippingNode* createRect(cocos2d::CCSize contentSize,cocos2d::CCPoint pos);
    static cocos2d::CCClippingNode* createCircle(cocos2d::CCSize contentSize,cocos2d::CCPoint pos);
private:

};


#endif /* defined(__IF__ClippingNodeExt__) */
