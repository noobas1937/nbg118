//
//  TitanInView.cpp
//  IF
//
//  Created by fusheng on 15-9-18.
//
//

#include "TitanInView.h"
#include "CCBExtension.h"
//begin a by ljf
#include "NBGRenderTarget.h"
#include "GlobalData.h"
//end a by ljf
TitanInView* TitanInView::create()
{
    auto titan = new (std::nothrow) TitanInView();
    if (titan && titan->init())
    {
        titan->autorelease();
        return titan;
    }
    CC_SAFE_DELETE(titan);
    return nullptr;
}

bool TitanInView::init()
{
    if (!CCIFTouchNode::init()) {
        return false;
    }
    auto bg = CCBLoadFile("Titan",this,this);
    m_Titan = Titan::create(GlobalData::shared()->titanInfo.tid);
    m_Titan->setScale(0.7);
    //m_titanPos->addChild(m_Titan); //d by ljf
    //begin a by ljf
    auto s = Director::getInstance()->getWinSize();
    auto renderTexture = NBGRenderTarget::create(s.width , s.height , cocos2d::Texture2D::PixelFormat::RGBA8888);
    auto titanParent = Node::create();
    titanParent->addChild(m_Titan);
    renderTexture->addChild(titanParent);
    titanParent->setRotation3D(Vec3(0, 40, 0));
    m_titanPos->addChild(renderTexture);
//    m_Titan->changeTitanState(Titan::eActState::Stand);
    //end a by ljf
    return true;
    
}
bool TitanInView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanPos", CCNode*, this->m_titanPos);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanTouchLayer", CCLayerColor*, this->m_titanTouchLayer);
    return false;
}

void TitanInView::onEnter(){
   
    CCIFTouchNode::onEnter();
    setTouchEnabled(true);
}

void TitanInView::onExit(){

    
    CCIFTouchNode::onExit();
}

bool TitanInView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    if (!isTouchInside(m_titanTouchLayer,pTouch)) {
        return false;
    }
    
    m_TouchMoved = false;
    m_TouchPoint = pTouch->getLocation();

    return true;
}

void TitanInView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    m_TouchMoved = true;
    Vec2 oldPoint = m_TouchPoint;
    m_TouchPoint = pTouch->getLocation();
    float dtx = (m_TouchPoint.x - oldPoint.x)/3;
    CCLOG("Titan touch dtx:%f",dtx);
    m_Titan->onRotationY(dtx);
    return;
}

void TitanInView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_TouchMoved) {
        m_TouchMoved = false;
        return;
    }
    
    

}

