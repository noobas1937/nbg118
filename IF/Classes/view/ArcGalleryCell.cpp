//
//  ArcGalleryCell.cpp
//  IF
//
//  Created by tao.yu on 15/10/16.
//
//

#include "ArcGalleryCell.hpp"

ArcGalleryCell* ArcGalleryCell::create()
{
    auto ret = new ArcGalleryCell();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool ArcGalleryCell::init()
{
    CCBLoadFile("ArcScrollCell",this,this);
    this->setContentSize(CCSize(170,210));//250,132
    
    
    return true;
}

bool ArcGalleryCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonTxt", CCLabelIF*, this->m_buttonTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonNode", CCNode*, this->m_buttonNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtNode", CCNode*, this->m_txtNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_LvNode", CCNode*, this->m_LvNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockNode", CCNode*, this->m_lockNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockLvNode", CCNode*, this->m_lockLvNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonLockTxt", CCLabelIF*, this->m_buttonLockTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button", CCSprite*, this->m_button);
    
    return false;
}