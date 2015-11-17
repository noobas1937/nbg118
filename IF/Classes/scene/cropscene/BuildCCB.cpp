//
//  BuildCCB.cpp
//  IF
//
//  Created by fubin on 14-10-8.
//
//

#include "BuildCCB.h"
#include "SceneController.h"
#include "SoundController.h"
#include "FunBuildController.h"
#include "ImperialScene.h"

using namespace cocos2d;

BuildCCB* BuildCCB::create(int type)
{
    BuildCCB *pRet = new BuildCCB();
    if (pRet && pRet->initBuildCCB(type))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool BuildCCB::initBuildCCB(int type)
{
    m_sprArray = CCArray::create();
    m_famuNodes = CCArray::create();
    m_datieNodes = CCArray::create();
    
    if(true)
    {
        m_mainNode = CCNode::create();
        this->addChild(m_mainNode);
        string ccbName = "";
        if (type == 1) {
            ccbName = "CityBuild_L";
        }
        else if (type == 2) {
            ccbName = "CityBuild_Farm";
        }
        else if (type == 3) {
            ccbName = "CityBuild_Wall";
        }
        else
        {
            ccbName = "CityBuild_S";
        }
        CCBLoadFile(ccbName.c_str(),m_mainNode,this);
    }
    this->setAnchorPoint(ccp(0,0));
    
    return true;
}

void BuildCCB::setNamePos(int x, int y, CCLayer* sginLayer, CCSpriteBatchNode* batchNode, int zOrder)
{
    parentX = x;
    parentY = y;
    
    m_signLayer = sginLayer;
    m_batchNode = batchNode;
    
    m_zOrder = zOrder;
    int tmpOrd = 0;
    if(m_mainNode && m_rootNode) {

        auto& arrSpr = m_rootNode->getChildren();
        std::vector<Node*>::iterator itVec;
        for( itVec = arrSpr.begin(); itVec != arrSpr.end(); )
        {
            CCSprite* tmpSpr = dynamic_cast<CCSprite*>(*itVec);
            bool isFucked = tmpSpr
                         && tmpSpr->getTexture()
                         && tmpSpr->getTexture()->getPixelsWide() == 2
                         && tmpSpr->getTexture()->getPixelsHigh() == 2;// guojiang fuck this
            if (tmpSpr && !isFucked)
            {
                tmpSpr->removeFromParent();
                tmpSpr->getTexture()->setAntiAliasTexParameters();
                tmpSpr->setPosition(ccp(tmpSpr->getPositionX()+parentX, tmpSpr->getPositionY()+parentY));
                m_batchNode->addChild(tmpSpr, zOrder*1000+200+tmpOrd);
                m_sprArray->addObject(tmpSpr);
                tmpOrd++;
            }
            else
                itVec++;
        }
    }
    
}

void BuildCCB::onBuildDelete()
{
    if (m_signLayer) {
        auto batchCount = m_parVec.size();
        while (batchCount--) {
            m_signLayer->removeChild(m_parVec[batchCount]);
        }
        m_parVec.clear();
    }
    
    for (int i=0; i<m_sprArray->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_sprArray->objectAtIndex(i));
        m_batchNode->removeChild(spr, true);
    }
    m_sprArray->removeAllObjects();
    
}

void BuildCCB::onEnter() {
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(BuildCCB::onEnterFrame), this, 3.0,kCCRepeatForever, 0.0f, false);
}

void BuildCCB::onExit() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(BuildCCB::onEnterFrame), this);
    CCNode::onExit();
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler BuildCCB::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler BuildCCB::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool BuildCCB::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    if (pTarget == this && strncmp(pMemberVariableName, "m_famuNode",10) == 0) {
        m_famuNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_datieNode",11) == 0) {
        m_datieNodes->addObject(pNode);
        return true;
    }
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rootNode", CCNode*, this->m_rootNode);
    
    return false;
}

void BuildCCB::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_signLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void BuildCCB::onShowParticle()
{
    for (int i=0; i<m_famuNodes->count(); i++) {
        CCNode* node = dynamic_cast<CCNode*>( m_famuNodes->objectAtIndex(i) );
        if (node) {
            for (int j=1; j<=2; j++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("CityLumbering_%d",j)->getCString());
                particle->setPosition(ccp(parentX, parentY)+node->getPosition());
                addParticleToBatch(particle);
            }
        }
    }
    
    for (int m=0; m<m_datieNodes->count(); m++) {
        CCNode* node = dynamic_cast<CCNode*>( m_datieNodes->objectAtIndex(m) );
        if (node) {
            for (int n=1; n<=3; n++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("CityMining_%d",n)->getCString());
                particle->setPosition(ccp(parentX, parentY)+node->getPosition());
                addParticleToBatch(particle);
            }
        }
    }
}

void BuildCCB::onHideParticle()
{
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        m_signLayer->removeChild(m_parVec[batchCount]);
    }
    m_parVec.clear();
}

void BuildCCB::onEnterFrame(float dt)
{
//    onShowParticle();
}