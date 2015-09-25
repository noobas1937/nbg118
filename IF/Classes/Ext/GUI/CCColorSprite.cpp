//
//  CCColorSprite.cpp
//  IF
//
//  Created by 邹 程 on 14-4-14.
//
//

#include "CCColorSprite.h"

NS_CC_BEGIN


//CCColorSprite* CCColorSprite::create(const string &pic, ccColor3B decorateColor) {
//    auto sf = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pic.c_str());
//    auto ret = new CCColorSprite(decorateColor);
//    if (sf && ret && ret->initWithSpriteFrame(sf))
//    {
//        ret->autorelease();
//        return ret;
//    }
//    CC_SAFE_DELETE(ret);
//    return NULL;
//}
//
//bool CCColorSprite::initWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect &rect, bool rotated) {
//    if (CCNodeRGBA::init())
//    {
//        m_pobBatchNode = NULL;
//        
//        m_bRecursiveDirty = false;
//        setDirty(false);
//        
//        m_bOpacityModifyRGB = true;
//        
//        m_sBlendFunc.src = CC_BLEND_SRC;
//        m_sBlendFunc.dst = CC_BLEND_DST;
//        
//        m_bFlipX = m_bFlipY = false;
//        
//        // default transform anchor: center
//        setAnchorPoint(ccp(0.5f, 0.5f));
//        
//        // zwoptex default values
//        m_obOffsetPosition = CCPointZero;
//        
//        m_bHasChildren = false;
//        
//        // clean the Quad
//        memset(&m_sQuad, 0, sizeof(m_sQuad));
//        
//        // Atlas: Color
//        ccColor4B tmpColor = { 255, 255, 255, 255 };
//        m_sQuad.bl.colors = tmpColor;
//        m_sQuad.br.colors = tmpColor;
//        m_sQuad.tl.colors = tmpColor;
//        m_sQuad.tr.colors = tmpColor;
//        
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        // shader program
//        if (pTexture && pTexture->getAlphaName()) {
//            setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor_ETC));
//        } else {
//            setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
//        }
//#else
//        // shader program
//        setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
//#endif
//        
//        // update texture (calls updateBlendFunc)
//        setTexture(pTexture);
//        setTextureRect(rect, rotated, rect.size);
//        
//        // by default use "Self Render".
//        // if the sprite is added to a batchnode, then it will automatically switch to "batchnode Render"
//        setBatchNode(NULL);
//        
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//
//}

NS_CC_END
