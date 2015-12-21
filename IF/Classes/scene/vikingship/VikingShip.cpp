//
//  VikingShip.cpp
//  DragonClans
//
//  Created by 5ive on 15-11-30.
//
//

#include "VikingShip.h"

VikingShip::VikingShip(CCNode * pPosCCBNode, CCNode * pTouchCCBNode,  int seq, NBSprite3D * p3D, Node * pParticleNode, int level) : mPosCCBNode(pPosCCBNode), mTouchCCBNode(pTouchCCBNode), mSeq(seq),  mVikings3D(p3D), mParticleNode(pParticleNode), mLevel(level)
{
}

VikingShip::~VikingShip()
{
    
}

VikingShip * VikingShip::create(CCNode * pPosCCBNode, CCNode * pTouchCCBNode,  int seq, NBSprite3D * p3D, Node * pParticleNode, int level)
{
    VikingShip* ret = new VikingShip(pPosCCBNode, pTouchCCBNode,   seq, p3D, pParticleNode, level);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool VikingShip::init()
{
    mActionStatus = VikingShipActionStatus::VIKING_SHIP_ACTION_IDLE;
    return true;
}

void VikingShip::playIdle()
{
    //if (mActionStatus == VikingShipActionStatus::VIKING_SHIP_ACTION_IDLE)
        //return;
    //if (pShipInfo && pShipInfo->getViking3D())
    if(mVikings3D)
    {
        //NBSprite3D * pSprite3d = pShipInfo->getViking3D();
        int level = getModelLevel();
        char modelPath[256];
        sprintf(modelPath, "%s%d%s", "3d/ship/ship_", level, "_stand.c3b");
        //auto anim_stand = Animation3D::create("3d/ship/ship_3_stand.c3b");
        auto anim_stand = Animation3D::create(modelPath);
        if (anim_stand) {
            auto pAnim = Animate3D::create(anim_stand);
            if (pAnim) {
                auto act = RepeatForever::create(pAnim);
                mVikings3D->stopAllActions();
                mVikings3D->runAction(act);
                mActionStatus = VikingShipActionStatus::VIKING_SHIP_ACTION_IDLE;
            }
        }
    }

}

void VikingShip::playMove()
{
    //if (mActionStatus == VikingShipActionStatus::VIKING_SHIP_ACTION_MOVE)
        //return;
    if(mVikings3D)
    {
    int level = getModelLevel();
    char modelPath[256];
    sprintf(modelPath, "%s%d%s", "3d/ship/ship_", level, "_move.c3b");
    //auto anim_stand = Animation3D::create("3d/ship/ship_3_stand.c3b");
    auto anim_stand = Animation3D::create(modelPath);
    if (anim_stand) {
        auto pAnim = Animate3D::create(anim_stand);
        if (pAnim) {
            auto act = RepeatForever::create(pAnim);
            mVikings3D->stopAllActions();
            mVikings3D->runAction(act);
            mActionStatus = VikingShipActionStatus::VIKING_SHIP_ACTION_MOVE;
        }
    }
    }
}