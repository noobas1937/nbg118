//
//  ClippingNodeExt.cpp
//  IF
//
//  Created by zhaohf on 13-10-14.
//
//

#include "ClippingNodeExt.h"

USING_NS_CC;
USING_NS_CC_EXT;

ClippingNodeExt::~ClippingNodeExt()
{

}

ClippingNodeExt::ClippingNodeExt()
{
    
}

CCClippingNode* ClippingNodeExt::createRect(cocos2d::CCSize contentSize,cocos2d::CCPoint pos)
{
    CCClippingNode *clipper = CCClippingNode::create();
    clipper->setContentSize( contentSize );
    clipper->setAnchorPoint(  ccp(0.5, 0.5) );
    clipper->setPosition( pos );
    
    CCDrawNode *stencil = CCDrawNode::create();
    
    //绘制矩形区域
    CCPoint rectangle[4];
    rectangle[0] = ccp(0, 0);
    rectangle[1] = ccp(clipper->getContentSize().width, 0);
    rectangle[2] = ccp(clipper->getContentSize().width, clipper->getContentSize().height);
    rectangle[3] = ccp(0, clipper->getContentSize().height);
    
    ccColor4F green = {1, 1, 1, 1};
    
    stencil->drawPolygon(rectangle, 4, green, 0, green);//绘制这个多边形!
    clipper->setStencil(stencil);
    return clipper;
}

CCClippingNode* ClippingNodeExt::createCircle(cocos2d::CCSize contentSize,cocos2d::CCPoint pos)
{
    CCClippingNode *clipper = CCClippingNode::create();
    clipper->setContentSize( contentSize );
    clipper->setAnchorPoint(  ccp(0.5, 0.5) );
    clipper->setPosition( pos );

    CCDrawNode *stencil = CCDrawNode::create();

    //绘制圆形区域
    ccColor4F green = {0, 1, 0, 1};//顶点颜色,这里我们没有实质上没有绘制,所以看不出颜色
    const int nCount=100;//圆形其实可以看做正多边形,我们这里用正100边型来模拟园
    const float coef = 2.0f * (float)M_PI/nCount;//计算每两个相邻顶点与中心的夹角
    float fRadius = clipper->getContentSize().width/2;
    CCPoint circle[nCount];//顶点数组
    for(unsigned int i = 0;i <nCount; i++) {
        float rads = i*coef;//弧度
        circle[i].x = fRadius * cosf(rads);//对应顶点的x
        circle[i].y = fRadius * sinf(rads);//对应顶点的y
    }
    stencil->drawPolygon(circle, nCount, green, 0, green);//绘制这个多边形!
    clipper->setStencil(stencil);
    return clipper;
}
