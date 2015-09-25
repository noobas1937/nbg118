//
//  CCClipNode.cpp
//  ccgoe
//
//  Created by 谢文杰 on 12-12-7.
//
//
#include "CCClipNode.h"

NS_CC_EXT_BEGIN

CCClipNode * CCClipNode::create(int width, int height)
{
    return create(CCSizeMake(width, height));
}

CCClipNode * CCClipNode::create(const CCSize& size)
{
	CCClipNode * pRet = new CCClipNode();
    if (pRet) {
        pRet->autorelease();
        pRet->setContentSize(size);
    }
	return pRet;
}

void CCClipNode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    
    // IMPORTANT:
    // To ease the migration to v3.0, we still support the Mat4 stack,
    // but it is deprecated and your code should not rely on it
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when seting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
    
    this->beforeDraw();
    bool visibleByCamera = isVisitableByVisitingCamera();
    
    if (!_children.empty())
    {
        int i=0;
        
        // draw children zOrder < 0
        for( ; i < _children.size(); i++ )
        {
            Node *child = _children.at(i);
            if ( child->getLocalZOrder() < 0 )
            {
                child->visit(renderer, _modelViewTransform, flags);
            }
            else
            {
                break;
            }
        }
        
        // this draw
        if (visibleByCamera)
            this->draw(renderer, _modelViewTransform, flags);
        
        // draw children zOrder >= 0
        for( ; i < _children.size(); i++ )
        {
            Node *child = _children.at(i);
            child->visit(renderer, _modelViewTransform, flags);
        }
    }
    else if (visibleByCamera)
    {
        this->draw(renderer, _modelViewTransform, flags);
    }
    this->afterDraw();
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
void CCClipNode::beforeDraw()
{
    _beforeDrawCommand.init(_globalZOrder);
    _beforeDrawCommand.func = CC_CALLBACK_0(CCClipNode::onBeforeDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_beforeDrawCommand);
}
void CCClipNode::onBeforeDraw(){
    auto glview = Director::getInstance()->getOpenGLView();
    _scissorRestored =false;
    if (glview->isScissorEnabled()){
        _scissorRestored = true;
        _parentScissorRect = glview->getScissorRect();
    }
    else
    {
        glEnable(GL_SCISSOR_TEST);
    }
    CCPoint posLB=convertToWorldSpace(CCPointZero);
    CCPoint posRT=convertToWorldSpace(CCPoint(getContentSize().width, getContentSize().height));
    Director::getInstance()->getOpenGLView()->setScissorInPoints(    // scissorRect is the rectangle you want to show.
                                                                     posLB.x, posLB.y, posRT.x-posLB.x, posRT.y-posLB.y);
    
}


void CCClipNode::afterDraw(){
    _afterDrawCommand.init(_globalZOrder);
    _afterDrawCommand.func = CC_CALLBACK_0(CCClipNode::onAfterDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_afterDrawCommand);
    
}

void CCClipNode::onAfterDraw()
{
    if (_scissorRestored) {//restore the parent's scissor rect
        auto glview = Director::getInstance()->getOpenGLView();
            
        glview->setScissorInPoints(_parentScissorRect.origin.x, _parentScissorRect.origin.y, _parentScissorRect.size.width, _parentScissorRect.size.height);
    }
    else {
        glDisable(GL_SCISSOR_TEST);
    }
    
}
CCClipSprite* CCClipSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    CCClipSprite *pobSprite = new CCClipSprite();
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

void CCClipSprite::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags) {
    
    if (m_pDirection == NONE_DIRECTION) {
        // quick return
        CCSprite::visit();
        return;
    }
    
    CCPoint posLB = convertToWorldSpace(CCPointZero);
    CCPoint posRT = convertToWorldSpace(CCPoint(getContentSize().width, getContentSize().height));
    
    CCPoint toShowLB,toShowRT;
    switch (m_pDirection) {
        case WEST_TO_EAST:{
            toShowLB = posLB;
            toShowRT = ccp(m_pX*(posRT.x-posLB.x)+posLB.x,posRT.y);
        }
            break;
        case EAST_TO_WEST:{
            toShowLB = ccp((1-m_pX)*(posRT.x-posLB.x)+posLB.x,posLB.y);
            toShowRT = posRT;
        }
            break;
        case SOUTH_TO_NORTH:{
            toShowLB = posLB;
            toShowRT = ccp(posRT.x, m_pY*(posRT.y-posLB.y)+posLB.y);
        }
            break;
        case NORTH_TO_SOUTH:{
            toShowLB = ccp(posLB.x, (1-m_pY)*(posRT.y-posLB.y)+posLB.y);
            toShowRT = posRT;
        }
            break;
            
        default:{
            CCLOG("CCClipSprite error: error direction!");
            return;
        }
    }
    
    kmGLPushMatrix();
    
    glEnable(GL_SCISSOR_TEST);
    Director::getInstance()->getOpenGLView()->setScissorInPoints(toShowLB.x, toShowLB.y, toShowRT.x-toShowLB.x, toShowRT.y-toShowLB.y);
    CCSprite::visit();
    
    glDisable(GL_SCISSOR_TEST);
    
    kmGLPopMatrix();
}

NS_CC_EXT_END