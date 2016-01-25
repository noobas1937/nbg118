//
//  OutsideEnemy.cpp
//  IF
//
//  Created by 5ive on 15-11-18.
//
//

#include "Enemy.h"
#include "GongJian2.h"
#include "NBSprite3D.h"

CCPoint OutsideEnemy::PathBegin = Vec2(4477, 390);
CCPoint OutsideEnemy::PathEnd = Vec2(4117, 576);
//CCPoint OutsideEnemy::PathBegin1 = Vec2(4542, 533);
CCPoint OutsideEnemy::PathBegin1 = Vec2(4178, 236);
CCPoint OutsideEnemy::PathEnd1 = Vec2(4027, 512);

//CCPoint OutsideEnemy::PathBegin2 = Vec2(4387, 322);
CCPoint OutsideEnemy::PathBegin2 = Vec2(4357, 316);
CCPoint OutsideEnemy::PathEnd2 = Vec2(4208, 610);
CCPoint OutsideEnemy::ArrowBegin1 = Vec2(4120, 940);
CCPoint OutsideEnemy::ArrowBegin2 = Vec2(3734, 750);

OutsideEnemy::OutsideEnemy(CCNode * pBatchNode, CCNode * pArrowBatchNode,  int enemyType, bool isInGuide):mBatchNode(pBatchNode),mArrowBatchNode(pArrowBatchNode),mIcon(""),
mMoveSpeed(70), mIconSpr(nullptr),mActionStatus(ENEMY_ACTION_STATUS_IDLE),mspd(50), mEnemyType(enemyType), mShadowSpr(nullptr),mIsInGuide(isInGuide)
{
}

OutsideEnemy::~OutsideEnemy()
{
    
}

void OutsideEnemy::onEnter()
{
    CCNode::onEnter();
    scheduleUpdate();
}

void OutsideEnemy::onExit()
{
    CCNode::onExit();
    unscheduleUpdate();
}

OutsideEnemy * OutsideEnemy::create(CCNode * pBatchNode, CCNode * pArrowBatchNode,  int enemyType, bool isInGuide)
{
    OutsideEnemy* ret = new OutsideEnemy(pBatchNode, pArrowBatchNode,   enemyType, isInGuide);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool OutsideEnemy::init()
{
    mIcon = "c010";
    if(mEnemyType == ENEMY_TYPE_WILDMAN)
    {
        mIcon = "c010";
    }
    CCString* filePath = CCString::createWithFormat("%s_%d_%s_move_0.png", mIcon.c_str(), 0, "NW"); // guo.jiang
    if(CCLoadSprite::getSF(filePath->getCString())==NULL){
        CCLOG("filePath=%s",filePath->getCString());
    }
    mShadowSpr = CCLoadSprite::createSprite("walker_shadow.png");
    mShadowSpr->setAnchorPoint(ccp(0.5, 0.5));
    mShadowSpr->setOpacity(127);
    mShadowSpr->setScale(0.5);
    mBatchNode->addChild(mShadowSpr);
    
    mIconSpr = CCLoadSprite::createSprite(filePath->getCString());
    mIconSpr->setAnchorPoint(ccp(0.5,0.5));
    mIconSpr->setScale(1.2);
    mBatchNode->addChild(mIconSpr);
    return true;
}

void OutsideEnemy::move()
{
    if(mActionStatus == ENEMY_ACTION_STATUS_MOVE)
        return;
    
    CCPoint fromPos = OutsideEnemy::PathBegin2;
    CCPoint toPos = OutsideEnemy::PathEnd2;
    int posNum = 10;
    float dxFrom = (OutsideEnemy::PathBegin1.x - OutsideEnemy::PathBegin2.x) / posNum;
    float dyFrom = (OutsideEnemy::PathBegin1.y - OutsideEnemy::PathBegin2.y) / posNum;
    float dxTo = (OutsideEnemy::PathEnd1.x - OutsideEnemy::PathEnd2.x) / posNum;
    float dyTo = (OutsideEnemy::PathEnd1.y - OutsideEnemy::PathEnd2.y) / posNum;
    int xRandomIndex = (rand() % posNum);
    int YRandomIndex = (rand() % posNum);
    /*
    if(randomIndex == 0)
    {
        fromPos = OutsideEnemy::PathBegin;
        toPos = OutsideEnemy::PathEnd;
    }
    if(randomIndex == 1)
    {
        fromPos = OutsideEnemy::PathBegin1;
        toPos = OutsideEnemy::PathEnd1;
    }
    */
    fromPos.x = fromPos.x + dxFrom * xRandomIndex;
    fromPos.y = fromPos.y + dyFrom * YRandomIndex;
    toPos.x = toPos.x + dxTo * xRandomIndex;
    toPos.y = toPos.y + dyTo * YRandomIndex;
    CCPoint gap = ccpSub(fromPos, toPos);
    mFromPos = fromPos;
    mToPos = toPos;
    
    float len = ccpLength(gap);
    float useTime = len / mMoveSpeed;//len/120.0;
    CCSequence* sc = nullptr;
    CCSpawn * arrowAction = nullptr;
    Action* action = nullptr;
    if(useTime != 0)
    {
        //sc = CCSequence::create(CCMoveTo::create(useTime, onePos),CCCallFunc::create(this, callfunc_selector(Walker::moveOnePoint)),NULL);
        //sc = CCSequence::create(CCMoveTo::create(useTime, toPos), CCCallFunc::create(this, callfunc_selector(OutsideEnemy::die)), NULL);
        sc = CCSequence::create(CCMoveTo::create(useTime, toPos), CCCallFunc::create(this, callfunc_selector(OutsideEnemy::scare)), NULL);
        CCSequence *arrowSc = CCSequence::create(CCDelayTime::create(useTime - 0.25), CCCallFunc::create(this, callfunc_selector(OutsideEnemy::shootArrow)), NULL);
        arrowAction = CCSpawn::create(sc, arrowSc, NULL);
        action = createAnimation(ENEMY_ACTION_STATUS_MOVE);
    }
    //Action* action = createAnimation();
    mIconSpr->setPosition(fromPos);
    if (action)
    {
        auto sp = Spawn::create(arrowAction, action, NULL);
        mIconSpr->stopAllActions();
        mIconSpr->runAction(sp);
    }
    else
    {
        mIconSpr->stopAllActions();
        mIconSpr->runAction(arrowAction);
    }

    mActionStatus = ENEMY_ACTION_STATUS_MOVE;
}

void OutsideEnemy::die()
{
    if(mActionStatus == ENEMY_ACTION_STATUS_DIE)
        return;
    CCSequence* action = createAnimation(ENEMY_ACTION_STATUS_DIE);
    CCFadeOut * fadeOut = FadeOut::create(0.5);
    CCSequence * sc = CCSequence::create(action, fadeOut,  CCCallFunc::create(this, callfunc_selector(OutsideEnemy::hideAndReleaseSelf)),NULL);
    mIconSpr->stopAllActions();
    mIconSpr->runAction(sc);
    mActionStatus = ENEMY_ACTION_STATUS_DIE;
}

void OutsideEnemy::scare()
{
    
    if(mActionStatus == ENEMY_ACTION_STATUS_SCARE)
        return;
    CCSequence* action = createAnimation(ENEMY_ACTION_STATUS_SCARE);
    CCSequence * sc = CCSequence::create(action, CCCallFunc::create(this, callfunc_selector(OutsideEnemy::retreat)), NULL);
    mIconSpr->stopAllActions();
    mIconSpr->runAction(sc);
    mActionStatus = ENEMY_ACTION_STATUS_SCARE;
}

void OutsideEnemy::retreat()
{
    if(mActionStatus == ENEMY_ACTION_STATUS_RETREAT)
        return;
    CCSequence* action = createAnimation(ENEMY_ACTION_STATUS_RETREAT);
    
    CCPoint gap = ccpSub(mToPos, mFromPos);
    float len = ccpLength(gap);
    float useTime = len / (mMoveSpeed + 30);//len/120.0;
   
    CCSpawn * spawnAction = nullptr;
    if(useTime > 0)
    {
        auto moveAction = CCMoveTo::create(useTime, mFromPos);
        CCSequence * sequanceAction = CCSequence::create(moveAction, CCCallFunc::create(this, callfunc_selector(OutsideEnemy::hideAndReleaseSelf)), NULL);
        spawnAction = Spawn::create(action, sequanceAction, NULL);
    }
   
    mIconSpr->stopAllActions();
    mIconSpr->runAction(spawnAction);
    mActionStatus = ENEMY_ACTION_STATUS_RETREAT;
}

CCSequence* OutsideEnemy::createAnimation(int enemyActionStatus)
{
    CCSequence* result = nullptr;
    int totalFrame = 8;
    string actionName = "move";
    float delayPerUnit = 0.1;
    mAnimationFrameArray.clear();
    int loopTimes;
    if(enemyActionStatus == ENEMY_ACTION_STATUS_MOVE)
    {
        totalFrame = 5;
        actionName = "move";
        delayPerUnit = 0.1;
        loopTimes = numeric_limits<int>::max();
    }
    else if(enemyActionStatus == ENEMY_ACTION_STATUS_DIE)
    {
        totalFrame = 5;
        actionName = "die";
        delayPerUnit = 0.15;
        loopTimes = 1;
    }
    else if(enemyActionStatus == ENEMY_ACTION_STATUS_SCARE)
    {
        totalFrame = 7;
        actionName = "scare";
        delayPerUnit = 0.1;
        loopTimes = 1;
    }
    else if(enemyActionStatus == ENEMY_ACTION_STATUS_RETREAT)
    {
        totalFrame = 5;
        actionName = "retreat";
        delayPerUnit = 0.15;
        loopTimes = numeric_limits<int>::max();
    }
    for (int j = 0; j < totalFrame; j++)
    {
        auto cf = CCLoadSprite::getSF(CCString::createWithFormat("%s_%d_%s_%s_%d.png", mIcon.c_str(), 0, "NW", actionName.c_str(), j)->getCString());
        CC_BREAK_IF(!cf);
        mAnimationFrameArray.pushBack(cf);
    }
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(mAnimationFrameArray, delayPerUnit);
    animation->setLoops(loopTimes);
    animation->setRestoreOriginalFrame(false);
    result = CCSequence::create(CCAnimate::create(animation), NULL);
    //m_iconSpr->stopAllActions();
    return result;
}

void OutsideEnemy::hideAndReleaseSelf()
{
    if(mIconSpr)
    {
        mIconSpr->removeFromParentAndCleanup(true);
        mIconSpr = nullptr;
    }
    if(mShadowSpr)
    {
        mShadowSpr->removeFromParentAndCleanup(true);
        mShadowSpr = nullptr;
    }
    unscheduleUpdate();
}

void OutsideEnemy::update(float delta)
{
    if (mShadowSpr && mIconSpr)
    {
        mShadowSpr->setPosition(mIconSpr->getPosition() + Vec2(-5, -14));
    }
}

void OutsideEnemy::shootArrow()
{
    CCPoint p1 = OutsideEnemy::ArrowBegin1;
    CCPoint p11 = OutsideEnemy::ArrowBegin2;
    CCPoint p2 = mToPos + Vec2(-80, 80);
    GongJian2* gong1 = GongJian2::create(mArrowBatchNode, p1, p2, 0, "jian_0.png", 1.5, 600);
    GongJian2* gong2 = GongJian2::create(mArrowBatchNode, p11, p2, 0, "jian_0.png", 1.5, 600);
}

void OutsideEnemy::start()
{
    move();
}

void OutsideEnemy::boatCome()
{
    /*
    char modelPath[256];
    sprintf(modelPath, "%s%d%s", "3d/ship/ship_", 1, "_skin.c3b");
    char texturePath[256];
    sprintf(texturePath, "%s%d%s", "3d/ship/ship_", 1, ".jpg");
    
      NBSprite3D * m_vikings3D = NBSprite3D::create(modelPath);
    m_vikings3D->setTexture(texturePath);
    m_vikings3D->setScale(1.1);
    
    
    auto vikingsRootNode = CCNode::create();
    vikingsRootNode->setRotation3D(Vec3(38, 39, -24));
    auto rotateNode = CCNode::create();
    rotateNode->addChild(m_vikings3D);
    rotateNode->setRotation3D(Vec3(0, -80, 0));
    vikingsRootNode->addChild(rotateNode);
    
    
    vikingsRootNode->setPosition(m_touchLayer->convertToNodeSpace(pPosCCBNode->convertToWorldSpace(Point(0, 0))));
    
    Node * m_vikingsParticleNode = Node::create();
    vikingsRootNode->addChild(m_vikingsParticleNode);
    */
}

void OutsideEnemy::boatGo()
{
    
}

void OutsideEnemy::ResumeEnemy()
{
    mIconSpr->resume();
}

void OutsideEnemy::PauseEnemy()
{
    mIconSpr->pause();
}