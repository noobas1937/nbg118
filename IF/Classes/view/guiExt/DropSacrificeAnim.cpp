//
//  DropSacrificeAnim.cpp
//  IF
//
//  Created by lifangkai on 14-11-20.
//
//

#include "DropSacrificeAnim.h"
#include "SoundController.h"
#include "UIComponent.h"

using namespace cocos2d;

DropSacrificeAnim* DropSacrificeAnim::create(int type,int hintType)
{
    DropSacrificeAnim *pRet = new DropSacrificeAnim(type,hintType);
    if (pRet && pRet->initDropRdCCB(type,hintType))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool DropSacrificeAnim::initDropRdCCB(int type,int hintType)
{
    m_show_1_Nodes = CCArray::create();
    m_show_2_Nodes = CCArray::create();
    m_show_5_Nodes = CCArray::create();
    m_show_10_Nodes = CCArray::create();
    m_dropSprs = CCArray::create();
//    if(hintType==1){
//        m_dropNodes =m_show_1_Nodes;
//    }else if(hintType==2){
//        m_dropNodes =m_show_2_Nodes;
//    }else if(hintType==5){
//        m_dropNodes =m_show_5_Nodes;
//    }else if(hintType==10){
//        m_dropNodes =m_show_10_Nodes;
//    }
    m_dropNodes =m_show_10_Nodes;
    string ccbName = "Sacrifice";
    CCBLoadFile(ccbName.c_str(),this,this);
    
    string picName = "";
    musicName = "";
    int sprCnt = 5;
    float dt = 0.1;
    if (type == Wood) {
        picName = "The-fountain_icon1.png";
        sprCnt = 7;
        musicName = Music_Sfx_city_harvest_wood;
        dt = 0.05;
    }
    else if (type == Food) {
        picName = "The-fountain_icon2.png";
        musicName = Music_Sfx_city_harvest_farm;
    }
    else if (type == Stone) {
        picName = "The-fountain_icon4.png";
        musicName = Music_Sfx_city_harvest_mithril;
    }
    else if (type == Iron) {
        picName = "The-fountain_icon3.png";
        musicName = Music_Sfx_city_harvest_iron;
    }else if(type == Silver){
        picName = "The-fountain_icon5.png";
        musicName = Music_Sfx_city_harvest_mithril;
    }

    
    if (picName == "") {
        return true;
    }
    
    for (int i=0; i<m_dropNodes->count(); i++) {
        CCNode* curNode = dynamic_cast<CCNode*>(m_dropNodes->objectAtIndex(i));
//        auto pic = CCSprite::create();
//        CCCommonUtils::makeEffectSpr(pic, picName, sprCnt, dt, i%sprCnt);
//        CCCommonUtils::setSpriteMaxSize(pic, 70, true);
        auto pic = CCLoadSprite::createSprite(picName.c_str());
        m_dropSprs->addObject(pic);
        pic->setScale(0.5);
        curNode->addChild(pic);
    }
    
    return true;
}

void DropSacrificeAnim::onEnter() {
    CCNode::onEnter();
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(DropSacrificeAnim::onPlayEnd));
    string timeLine = "";
    if(m_hintType==1){
        timeLine="x1";
    }else if(m_hintType==2){
        timeLine="x2";
    }else if(m_hintType==5){
        timeLine="x5";
    }else if(m_hintType==10){
        timeLine="x10";
    }else{
        return;
    }
    this->getAnimationManager()->runAnimationsForSequenceNamed(timeLine.c_str());
    this->schedule(schedule_selector(DropSacrificeAnim::onPlayMusic), 0.2, 0, 0);
}

void DropSacrificeAnim::onExit() {
    CCNode::onExit();
}

void DropSacrificeAnim::onPlayEnd()
{
    auto node = UIComponent::getInstance();
    CCPoint p;
    CCPoint p2;
    if (m_type == Wood) {
        p = node->m_woodIcon->getPosition();
        CCDirector::sharedDirector()->convertToGL(p);
         p2 = node->m_woodIcon->getParent()->convertToWorldSpace(p);
    }
    else if (m_type == Food) {
        p = node->m_foodIcon->getPosition();
        CCDirector::sharedDirector()->convertToGL(p);
        p2 = node->m_foodIcon->getParent()->convertToWorldSpace(p);
    }
    else if (m_type == Stone) {
        p = node->m_stoneIcon->getPosition();
        CCDirector::sharedDirector()->convertToGL(p);
        p2 = node->m_stoneIcon->getParent()->convertToWorldSpace(p);
    }
    else if (m_type == Iron) {
        p = node->m_ironIcon->getPosition();
        CCDirector::sharedDirector()->convertToGL(p);
        p2 = node->m_ironIcon->getParent()->convertToWorldSpace(p);
    }else{
        for (int i=0; i<m_dropSprs->count(); i++) {
            CCSprite* spr = dynamic_cast<CCSprite*>(m_dropSprs->objectAtIndex(i));
            if (spr) {
                CCDelayTime* delay = CCDelayTime::create(0.25);
                spr->runAction(CCSequence::create(delay, CCFadeOut::create(0.25),NULL));
            }
        }
        this->scheduleOnce(schedule_selector(DropSacrificeAnim::onDelete), 0.5);
        return;
    }

    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(DropSacrificeAnim::onPlayFlyEnd));
    int count =m_dropSprs->count();
    for (int i=0; i<m_dropSprs->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_dropSprs->objectAtIndex(i));
        if (spr) {
            CCPoint m_endpoint = spr->getParent()->convertToNodeSpace(p2);
            CCMoveTo* moveToAction = CCMoveTo::create(0.5f/count*(i+1), m_endpoint);
            CCDelayTime* delay = CCDelayTime::create(0.25);
////            spr->runAction(CCFadeOut::create(0.2));
//            CCSequence* action  =CCSequence::create(moveToAction,NULL);
            spr->runAction(CCSequence::create(delay,moveToAction,  CCFadeOut::create(0.2),funcall,NULL));
        }
    }
    this->scheduleOnce(schedule_selector(DropSacrificeAnim::onDelete), 1);
}

void DropSacrificeAnim::onPlayFlyEnd(){
    UIComponent::getInstance()->playIconByType(m_type);
}
void DropSacrificeAnim::onPlayMusic(float _time)
{
    if (musicName != "") {
        SoundController::sharedSound()->playEffects(musicName.c_str());
    }
}

void DropSacrificeAnim::onDelete(float _time)
{
    m_dropSprs->removeAllObjects();
    this->removeFromParent();
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler DropSacrificeAnim::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler DropSacrificeAnim::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool DropSacrificeAnim::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    if (pTarget == this && strncmp(pMemberVariableName, "m_dropNode10_",13) == 0) {
        pNode->removeAllChildren();
        if((m_hintType==1||m_hintType==2||m_hintType==5)&&strncmp(pMemberVariableName, "m_dropNode10_1_",15)== 0){
            m_show_10_Nodes->addObject(pNode);
        }else if((m_hintType==2||m_hintType==5)&&strncmp(pMemberVariableName, "m_dropNode10_2_",15)== 0){
            m_show_10_Nodes->addObject(pNode);
        }else if(m_hintType==5&&strncmp(pMemberVariableName, "m_dropNode10_5_",15)== 0){
            m_show_10_Nodes->addObject(pNode);
        }else if(m_hintType==10){
          m_show_10_Nodes->addObject(pNode);
        }
        
        return true;
    }
    if (pTarget == this && strncmp(pMemberVariableName, "m_dropNode1_",12) == 0) {
        pNode->removeAllChildren();
        m_show_1_Nodes->addObject(pNode);
        return true;
    }
    if (pTarget == this && strncmp(pMemberVariableName, "m_dropNode2_",12) == 0) {
        pNode->removeAllChildren();
        m_show_2_Nodes->addObject(pNode);
        return true;
    }
    if (pTarget == this && strncmp(pMemberVariableName, "m_dropNode5_",12) == 0) {
        pNode->removeAllChildren();
        m_show_5_Nodes->addObject(pNode);
        return true;
    }
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_show_1_Node", CCNode*, this->m_show_1_Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_show_2_Node", CCNode*, this->m_show_2_Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_show_5_Node", CCNode*, this->m_show_5_Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_show_10_Node", CCNode*, this->m_show_10_Node);

    return false;
}