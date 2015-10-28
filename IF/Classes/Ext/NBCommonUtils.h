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

    
}

#endif /* defined(__IF__NBCommonUtils__) */
