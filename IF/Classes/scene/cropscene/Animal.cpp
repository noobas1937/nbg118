//
//  Animal.cpp
//  IF
//
//  Created by fubin on 14-7-28.
//
//

#include "Animal.h"
#include "SceneController.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "BattleView.h"
#include "QueueController.h"
#include "ToolController.h"
#include "WorldController.h"
#include "ImperialScene.h"
#include "GongJian2.h"

using namespace cocos2d;

Animal* Animal::create(int itemId, int type)
{
    Animal *pRet = new Animal();
    if (pRet && pRet->initAnimal(itemId, type))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool Animal::initAnimal(int itemId, int type)
{
    m_buildingKey = itemId;
    
    m_sprArray = CCArray::create();
    
    m_mainNode = CCNode::create();
    this->addChild(m_mainNode);
    
    string ccbName = "Animal";
    CCBLoadFile(ccbName.c_str(),m_mainNode,this);
    
    auto pt = m_touchNode->getPosition();
    mainWidth = (m_touchNode->getContentSize().width*m_touchNode->getScaleX()/2+pt.x)*2;
    mainHeight = (m_touchNode->getContentSize().height*m_touchNode->getScaleY()/2+pt.y)*2;
    
    this->setContentSize(CCSizeMake(mainWidth, mainHeight));
    this->setAnchorPoint(ccp(0,0));
    
    m_effectSpr = CCSprite::create();
    m_signNode = CCNode::create();
    m_upEffectNode = CCNode::create();
    return true;
}

void Animal::setNamePos(int x, int y, CCLayer* sginLayer, CCSpriteBatchNode* batchNode, int zOrder)
{
    parentX = x;
    parentY = y;
    this->setPosition(ccp(parentX, parentY));
    
    m_signLayer = sginLayer;
    m_batchNode = batchNode;
    
    m_signNode->setPosition(ccp(x,y));
    m_upEffectNode->setPosition(ccp(x+mainWidth/2,y+mainHeight/2));
    sginLayer->addChild(m_signNode);
    sginLayer->addChild(m_upEffectNode);
    
    int tmpOrd = 0;
    if(m_mainNode) {// m_buildingKey!=9995 &&
        auto array = m_mainNode->getChildren();
        if (array.size()>0) {
            CCNode* tmpNode = dynamic_cast<CCNode*>(array.at(0));//ccb 中的layer层
            if (tmpNode) {
                auto arr1 = tmpNode->getChildren();
                if (arr1.size()>0) {
                    CCNode* allSprNode = dynamic_cast<CCNode*>(arr1.at(0));//ccb 中的layer的 node
                    if (allSprNode) {
                        auto& arrSpr = allSprNode->getChildren();
                        if (true) {
                            for (int i=0; i<arrSpr.size(); i++) {
                                CCSprite* tmpSpr = dynamic_cast<CCSprite*>(arrSpr.at(i));
                                if (tmpSpr) {
                                    tmpSpr->removeFromParent();
                                    tmpSpr->getTexture()->setAntiAliasTexParameters();
                                    tmpSpr->setPosition(ccp(tmpSpr->getPositionX()+parentX, tmpSpr->getPositionY()+parentY));
                                    m_batchNode->addChild(tmpSpr);
                                    m_sprArray->addObject(tmpSpr);
                                    tmpSpr->setZOrder(zOrder*1000+tmpOrd);
                                    tmpOrd++;
                                    i--;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else {
    }
    
    drowEffectSpr(zOrder, tmpOrd);
}

void Animal::onAnimalDelete()
{
    if (m_signLayer) {
        m_signLayer->removeChild(m_signNode);
        m_signLayer->removeChild(m_upEffectNode);
        
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

void Animal::onEnter() {
    CCNode::onEnter();
}

void Animal::onExit() {
    CCNode::onExit();
}

bool Animal::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    bool ret = false;
    ret = isTouchInside(m_touchNode, pTouch);
    return ret;
}

void Animal::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isCanClick) {
        return;
    }
    isCanClick=false;
    scheduleOnce(schedule_selector(Animal::onClickThis), 0.1f);
}

void Animal::onClickThis(float _time)
{
//    auto startPt = ccp(1130, 710);//下塔
    auto startPt = ccp(2040, 1180);
    auto endPt = ccp(parentX+mainWidth/2, parentY+mainHeight/2);
    GongJian2* gong = GongJian2::create(m_batchNode, startPt, endPt, 1, "jian_3.png");
    
    scheduleOnce(schedule_selector(Animal::onCanClick), 0.2f);
}

void Animal::onCanClick(float _time)
{
    isCanClick = true;
}

void Animal::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void Animal::drowEffectSpr(int zOrder, int tmpOrd)
{
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler Animal::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler Animal::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool Animal::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    return false;
}

void Animal::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
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

void Animal::onShowParticle()
{
    if (true) {
        auto tmp_ptArray = CCPointArray::create(20);
        if (1) {
            tmp_ptArray->addControlPoint(ccp(parentX+170, parentY+210));
        }
        for (int j=0; j<tmp_ptArray->count(); j++) {
            auto particle = ParticleController::createParticle("Leaf");
            particle->setPosition(tmp_ptArray->getControlPointAtIndex(j));
            addParticleToBatch(particle);
        }
    }
}

void Animal::onHideParticle()
{
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        m_signLayer->removeChild(m_parVec[batchCount]);
    }
    m_parVec.clear();
}