//
//  C3DShowView.cpp
//  IF
//
//  Created by tao.yu on 15/9/29.
//
//

#include "C3DShowView.hpp"
#include "CCBExtension.h"
//begin a by ljf
#include "NBGRenderTarget.h"
//end a by ljf
C3DShowView* C3DShowView::create(const char* modelPath, const char* texPath)
{
    auto c3dspr = new (std::nothrow) C3DShowView();
    if (c3dspr && c3dspr->init(modelPath, texPath))
    {
        c3dspr->autorelease();
        return c3dspr;
    }
    CC_SAFE_DELETE(c3dspr);
    return nullptr;
}

bool C3DShowView::init(const char* modelPath, const char* texPath)
{
    if (!CCIFTouchNode::init()) {
        return false;
    }
    auto bg = CCBLoadFile("C3DShowView",this,this);
    m_Model = NBSprite3D::create(modelPath);
    m_Model->setTexture(texPath);
    std::string shaderTexPath = texPath;
    int pos = shaderTexPath.find(".");
    shaderTexPath.insert(pos, "_shader");
    
    
    //m_modelPos->addChild(m_Model); //d by ljf
    
    //begin a by ljf
    auto renderTexture = NBGRenderTarget::create(200, 200, cocos2d::Texture2D::PixelFormat::RGBA4444);
    auto pNode = CCNode::create();
    pNode->setRotation3D(Vec3(Vec3(22, 0, 0)));
    pNode->setScale(0.7);
    pNode->addChild(m_Model);
    renderTexture->addChild(pNode);
    m_modelPos->addChild(renderTexture);
    //end a by ljf
    return true;
    
}
bool C3DShowView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_modelPos", CCNode*, this->m_modelPos);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchLayer", CCLayerColor*, this->m_touchLayer);
    return false;
}

void C3DShowView::onEnter(){
    
    CCIFTouchNode::onEnter();
    setTouchEnabled(true);
}

void C3DShowView::onExit(){
    
    
    CCIFTouchNode::onExit();
}

bool C3DShowView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    if (!isTouchInside(m_touchLayer,pTouch)) {
        return false;
    }
    
    m_TouchMoved = false;
    m_TouchPoint = pTouch->getLocation();
    
    return true;
}

void C3DShowView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_Model) {
        return;
    }
    m_TouchMoved = true;
    Vec2 oldPoint = m_TouchPoint;
    m_TouchPoint = pTouch->getLocation();
    float dtx = (m_TouchPoint.x - oldPoint.x)/3;
    Vec3 r = m_Model->getRotation3D();
    m_Model->setRotation3D(Vec3(r.x, r.y+dtx, r.z));

    return;
}

void C3DShowView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
//    auto animation3d = Animation3D::create(m_info->getModelAniName().c_str());
//    if (animation3d) {
//        auto pAnim = Animate3D::createWithFrames(animation3d, 1, 100);
//        if (pAnim) {
//            Action* act = RepeatForever::create(pAnim);
//            pic->getModel().getObject()->stopAllActions();
//            pic->getModel().getObject()->runAction(act);
//        }
//    }

    if (m_TouchMoved) {
        m_TouchMoved = false;
        return;
    }
}


