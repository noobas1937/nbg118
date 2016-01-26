//
//  OutsideEnemy.h
//  IF
//
//  Created by 5ive on 15-11-18.
//
//

#ifndef __IF__Enemy__
#define __IF__Enemy__

#include "CommonInclude.h"
#include "CCLoadSprite.h"
#include "SpriteSheetAni.h"
#include "CCMathUtils.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum EnemyActionStatus
{
    ENEMY_ACTION_STATUS_DIE, //死亡
    ENEMY_ACTION_STATUS_MOVE, //走
    ENEMY_ACTION_STATUS_IDLE,
    ENEMY_ACTION_STATUS_RETREAT, //往回跑
    ENEMY_ACTION_STATUS_SCARE, //被吓到
};

enum EnemyType
{
    ENEMY_TYPE_WILDMAN, //野人
};

class OutsideEnemy : public CCNode
{
public:
    OutsideEnemy(CCNode * pBatchNode, CCNode * pArrowBatchNode,  int enemyType, bool isInGuide = false);
    virtual  ~OutsideEnemy();
    
    static OutsideEnemy * create(CCNode * pBatchNode, CCNode * pArrowBatchNode,  int enemyType, bool isInGuide = false);
    
    void start();
    
    void ResumeEnemy();
    
    void PauseEnemy();
    
    void update(float delta);
    
    void onEnter();
    
    void onExit();
    
    
    
    static CCPoint PathBegin;
    static CCPoint PathEnd;
    static CCPoint PathBegin1;
    static CCPoint PathEnd1;
    static CCPoint PathBegin2;
    static CCPoint PathEnd2;
    static CCPoint ArrowBegin1;
    static CCPoint ArrowBegin2;
    
private:
    bool init();
    void move();
    void die();
    void shootArrow();
    void retreat();
    void scare();
    CCSequence * createAnimation(int enemyActionStatus);
    void hideAndReleaseSelf();
    string mIcon;
    CCNode * mBatchNode;
    CCNode * mArrowBatchNode;
    //CCNode * mParent;
    CCSprite * mIconSpr;
    CCSprite * mShadowSpr;
    int mEnemyType;
    int mActionStatus;
    float mMoveSpeed;
    const float mspd;
    CCPoint mFromPos;
    CCPoint mToPos;
    bool mIsInGuide;
    Vector<SpriteFrame*> mAnimationFrameArray;
    void boatCome();
    void boatGo();
};
#endif /* defined(__IF__Enemy__) */
