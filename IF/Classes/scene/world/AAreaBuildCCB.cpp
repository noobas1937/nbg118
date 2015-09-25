//
//  AAreaBuildCCB.cpp
//  IF
//
//  Created by wangdianzhen on 15/4/27.
//
//

#include "AAreaBuildCCB.h"

bool AAreaBuildCCB::init(){
    return true;
}
void AAreaBuildCCB::onEnter() {
    CCNode::onEnter();
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
}
AAreaBuildCCB* AAreaBuildCCB::setCCBName(string ccbName){
    CCBLoadFile(ccbName.c_str(),this,this,true);
    return this;
}
void AAreaBuildCCB::onExit() {
    CCNode::onExit();
}
void AAreaBuildCCB::OnPlayOver(){
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(AAreaBuildCCB::OnRemoveSelf));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
}
void AAreaBuildCCB::OnRemoveSelf(){
    if (this->getParent()) {
        this->removeFromParentAndCleanup(true);
    }
}
#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler AAreaBuildCCB::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler AAreaBuildCCB::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool AAreaBuildCCB::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    return false;
}