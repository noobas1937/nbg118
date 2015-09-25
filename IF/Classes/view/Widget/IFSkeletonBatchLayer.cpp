//
//  IFSkeletonBatchLayer.cpp
//  IF
//
//  Created by 李锐奇 on 15/3/16.
//
//

#include "IFSkeletonBatchLayer.h"

bool IFSkeletonBatchLayer::init(){
    m_batch = spine::PolygonBatch::createWithCapacity(2000);
    m_additive = -1;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionETC1A));
    setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_ETC1_A_NO_MVP));
#else
//    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
    setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
#endif

    return true;
}

void IFSkeletonBatchLayer::onEnter(){
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void IFSkeletonBatchLayer::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    CCNode::onExit();
}

void IFSkeletonBatchLayer::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags){
//    CCNode::visit(renderer,parentTransform,parentFlags);
    draw(renderer,parentTransform,parentFlags);
}

void IFSkeletonBatchLayer::draw(Renderer *renderer, const kmMat4& transform, uint32_t transformFlags){
//    CCNode::draw(renderer, transform, transformFlags);
//    return;
    _drawCommand.init(_globalZOrder);
    _drawCommand.func = CC_CALLBACK_0(IFSkeletonBatchLayer::drawSkeleton, this, transform, transformFlags);
    _drawCommand.set3D(true);
    renderer->addCommand(&_drawCommand);
    return;
    
}

void IFSkeletonBatchLayer::drawSkeleton (const cocos2d::Mat4& transform, uint32_t transformFlags)
{
    if(_children.size()==0){
        return;
    }
    CC_NODE_DRAW_SETUP();
    ccGLBindVAO(0);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#else
    ccGLBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#endif
    
    for(auto child:_children) {
        auto node = dynamic_cast<IFSkeletonNode*>(child);
        if(node && node->isVisible() && node->animationObj){
            auto pt = this->getParent()->convertToWorldSpace(node->getPosition());
            auto size = CCDirector::sharedDirector()->getWinSize();
            int addX = 300;
            int addY = 200;
            if(pt.x < -addX || pt.x > size.width + addX || pt.y < -addY || pt.y > size.height + addY){
                
            }else{
                node->animationObj->setScaleX(node->getScaleX());
                node->animationObj->setScaleY(node->getScaleY());
                //TODO batch node
                node->animationObj->updateInBatch(transform,m_batch, node->getPosition(), m_additive);
            }
        }
    }
    m_batch->flush();
}