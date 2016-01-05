//
//  NBCommonUtils.h
//  IF
//
//  Created by fusheng on 15-10-20.
//
//

#ifndef __IF__NBCommonUtils__
#define __IF__NBCommonUtils__

#include <stdio.h>
#include <string>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCLabelIF.h"
#include "CCCommonUtils.h"
#include "CCLoadSprite.h"
USING_NS_CC;
namespace NBCommonUtils {
    
    inline void setGrayNode(cocos2d::CCNode *node , bool gray)
    {
        auto trySpr = dynamic_cast<CCSprite*>(node);
        
        auto trySpr9 = dynamic_cast<CCScale9Sprite*>(node);
        
        if (trySpr) {
            CCCommonUtils::setSpriteGray(trySpr, gray);
        }
        
        if (trySpr9) {
            if(gray)
                trySpr9->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
            else
                trySpr9->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
        }
        
        
        
        for(auto child:node->getChildren())
        {
            setGrayNode(child,gray);
        }
        
        
    }
    
    inline void arrangeLabel(vector<CCLabelIF*> &labels,vector<float> &gaps )
    {
        size_t count = labels.size();
        
        size_t gaps_size = gaps.size();
        
        if (count == 0 || count == 1) {
            return;
        }
        
        for (int i = 1; i<count; i++) {
            
            CCLabelIF* nbPreLabel = labels.at(i-1);
            
            CCLabelIF* nbLabel = labels.at(i);
            
            nbLabel->setAnchorPoint(Vec2::ZERO);
            
            float gap = 0;
            
            if (i-1<gaps_size) {
                gap = gaps.at(i-1);
            }
            
            nbLabel->setPositionX(nbPreLabel->getContentSize().width * nbPreLabel->getOriginScaleX()+nbPreLabel->getPositionX()+gap);
            
        }
        
    }
    inline void arrangeLabel(vector<CCLabelIF*> &labels )
    {
        size_t count = labels.size();
        
        if (count == 0 || count == 1) {
            return;
        }
        
        for (int i = 1; i<count; i++) {
            
            CCLabelIF* nbPreLabel = labels.at(i-1);
            
            CCLabelIF* nbLabel = labels.at(i);
            
            nbLabel->setAnchorPoint(Vec2::ZERO);
            
            nbLabel->setPositionX(nbPreLabel->getContentSize().width * nbPreLabel->getOriginScaleX()+nbPreLabel->getPositionX());
            
        }
        
    }
    inline Sprite* convertNodeToSprite(Node* node)
    {
        
        Vec2 oldAP = node->getAnchorPoint();
        
        Vec2 oldPos = node->getPosition();
        
        node->setAnchorPoint(Vec2::ZERO);
        
        node->setPosition(Vec2::ZERO);
        
        auto renderTexture = RenderTexture::create(node->getContentSize().width, node->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
        
        renderTexture->beginWithClear(0, 0, 0, 0);
        
        node->visit();
        
        renderTexture->end();
        
        Director::getInstance()->getRenderer()->render();
        
        auto image = renderTexture->newImage();
        
        
        
        auto tex =TextureCache::getInstance()->addImage(image, "10086");
        
        auto sp = Sprite::createWithTexture(tex);
        
        
        node->setAnchorPoint(oldAP);
        
        node->setPosition(oldPos);
        
        return sp;
        
    }

    inline Node* createDashed(Size desingSize )
    {
        //    nb_xuxian.png
        //    nb_xuxiankuang
        
        CCSprite* sprDashed = CCLoadSprite::createSprite("nb_xuxian.png");
        
        float sprHeight = sprDashed->getContentSize().width;//fusheng 38
        
        CCSprite* sprDashedFrameBL = CCLoadSprite::createSprite("nb_xuxiankuang.png");
        
        float dfWidth = sprDashedFrameBL->getContentSize().width;
        
        float dfHeight = sprDashedFrameBL->getContentSize().height;
        
        
        
        int  numOfHor =(desingSize.width - dfWidth*2)/sprHeight;//横行需线条个数
        
        int  numOfVert =(desingSize.height - dfHeight*2)/sprHeight;//纵向需线条个数
        
        
        
        
        
        
        sprDashedFrameBL->setAnchorPoint(Vec2::ZERO);
        
        
        CCSprite* sprDashedFrameBR = CCLoadSprite::createSprite("nb_xuxiankuang.png");
        
        sprDashedFrameBR->setAnchorPoint(Vec2(1,0));
        
        sprDashedFrameBR->setFlippedX(true);
        
        CCSprite* sprDashedFrameTL = CCLoadSprite::createSprite("nb_xuxiankuang.png");
        
        sprDashedFrameTL->setAnchorPoint(Vec2(0,1));
        
        sprDashedFrameTL->setFlippedY(true);
        
        CCSprite* sprDashedFrameTR = CCLoadSprite::createSprite("nb_xuxiankuang.png");
        
        sprDashedFrameTR->setAnchorPoint(Vec2(1,1));
        
        sprDashedFrameTR->setFlippedX(true);
        
        sprDashedFrameTR->setFlippedY(true);
        
        //    Size size = desingSize;
        //
        //    size.width += spr->getContentSize().height*2;// 3像素*2
        //    size.height += spr->getContentSize().height*2;// 3像素*2
        
        Node* container = Node::create();
        
        
        sprDashedFrameBL->setPosition(Vec2::ZERO);
        
        
        sprDashedFrameBR->setPosition(Vec2(sprDashedFrameBL->getPosition().x+dfWidth*2+numOfHor*sprHeight,sprDashedFrameBL->getPosition().y));
        
        sprDashedFrameTL->setPosition(Vec2(sprDashedFrameBL->getPosition().x,sprDashedFrameBL->getPosition().y+dfHeight*2+numOfVert*sprHeight));
        
        sprDashedFrameTR->setPosition(Vec2(sprDashedFrameBR->getPosition().x,sprDashedFrameTL->getPosition().y));
        
        
        container->addChild(sprDashedFrameBL);//fusheng 使用左下角作为相对点
        
        container->addChild(sprDashedFrameBR);
        
        container->addChild(sprDashedFrameTL);
        
        container->addChild(sprDashedFrameTR);
        
        for (int i = 0; i < numOfHor; i++) {//fusheng 底部的虚线和顶部
            //底部
            auto sp1 = CCLoadSprite::createSprite("nb_xuxian.png");
            
            sp1->setAnchorPoint(Vec2::ZERO);
            
            sp1->setPosition(sprDashedFrameBL->getPosition().x+dfWidth+i*sprHeight,sprDashedFrameBL->getPosition().y);
            
            container->addChild(sp1);
            
            
            //顶部
            auto sp2 = CCLoadSprite::createSprite("nb_xuxian.png");
            
            sp2->setAnchorPoint(Vec2(0,1));
            
            sp2->setPosition(sprDashedFrameTL->getPosition().x+dfWidth+i*sprHeight,sprDashedFrameTL->getPosition().y);
            
            container->addChild(sp2);
        }
        
        for (int i = 0; i < numOfVert; i++) {//fusheng 左右的虚线
            //左
            auto sp1 = CCLoadSprite::createSprite("nb_xuxian.png");
            
            sp1->setAnchorPoint(Vec2(0,1));
            
            sp1->setRotation(-90);
            
            sp1->setPosition(sprDashedFrameBL->getPosition().x,sprDashedFrameBL->getPosition().y+dfHeight+i*sprHeight);
            
            container->addChild(sp1);
            
            
            
            //右
            auto sp2 = CCLoadSprite::createSprite("nb_xuxian.png");
            
            
            sp2->setAnchorPoint(Vec2(1,1));
            
            sp2->setRotation(90);
            
            sp2->setPosition(sprDashedFrameBR->getPosition().x,sprDashedFrameBR->getPosition().y+dfHeight+i*sprHeight);
            
            container->addChild(sp2);
        }
        
        
        
        float ratioX = desingSize.width/(dfWidth*2+sprHeight*numOfHor);
        
        float ratioY = desingSize.height/(dfHeight*2+sprHeight*numOfVert);
        
        container->setScale(ratioX, ratioY);
        
        return container;
        
        
    }

    
}

#endif /* defined(__IF__NBCommonUtils__) */
