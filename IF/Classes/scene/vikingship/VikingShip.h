//
//  VikingShip.h
//  DragonClans
//
//  Created by 5ive on 15-11-30.
//
//

#ifndef __DragonClans__VikingShip__
#define __DragonClans__VikingShip__

#include "CommonInclude.h"
#include "CCLoadSprite.h"
#include "SpriteSheetAni.h"
#include "CCMathUtils.h"
#include "NBSprite3D.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum VikingShipActionStatus
{
    VIKING_SHIP_ACTION_NONE,
    VIKING_SHIP_ACTION_IDLE,
    VIKING_SHIP_ACTION_MOVE,
};

class VikingShip : public CCNode
{
public:
    VikingShip(CCNode * pPosCCBNode, CCNode * pTouchCCBNode,  int seq, NBSprite3D * p3D, Node * pParticleNode, int level);
    virtual  ~VikingShip();
    
    static VikingShip * create(CCNode * pPosCCBNode, CCNode * pTouchCCBNode,  int seq, NBSprite3D * p3D, Node * pParticleNode, int level);
    
    bool init();
    
    Node * getPosCCBNode(){return mPosCCBNode; }
    Node * getTouchCCBNode(){return mTouchCCBNode;}
    int getSeq(){return mSeq;}
    NBSprite3D * getViking3D(){return mVikings3D;}
    void setViking3D(NBSprite3D * p3D){mVikings3D = p3D;}
    Node * getParticleNode(){return mParticleNode;}
    int getModelLevel(){return mLevel;}
    void  setModelLevel(int level){mLevel = level;}
    int getActionStatus(){return mActionStatus;}
    
    void playIdle();
    void playMove();
    
private:
    NBSprite3D * mVikings3D;
    CCNode * mPosCCBNode;
    CCNode * mTouchCCBNode;
    CCNode * mParticleNode;
    int mSeq;
    int mLevel;
    int mActionStatus;
};

#endif /* defined(__DragonClans__VikingShip__) */
