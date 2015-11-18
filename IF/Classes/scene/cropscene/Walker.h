//
//  Walker.h
//  aoe
//
//  Created by ganxiaohua on 13-8-27.
//
//

#ifndef __IF__Walker__
#define __IF__Walker__

#include "CommonInclude.h"
#include "CCLoadSprite.h"
#include "SpriteSheetAni.h"
#include "CCMathUtils.h"
#include "GongJian2.h"
#include "BattleManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum WalkerStatus{
    WALKER_S_STAND,
    WALKER_S_MOVE,
    WALKER_S_WAVE,
    WALKER_S_REST,
    WALKER_S_WORK,
};



enum WalkerActionStatus{
    WALKER_ACTION_STAND,
    WALKER_ACTION_MOVE,
    WALKER_ACTION_WAVE,
    WALKER_ACTION_REST,
    WALKER_ACTION_WORK,
};

enum PointType{
    POINT_TYPE_MOVE,
    POINT_TYPE_WORK,
    POINT_TYPE_REST,
    POINT_TYPE_WAVE,
};

class WalkerPathPoint
{
public:
    WalkerPathPoint(int px, int py, int seq1, int type1 = 0)
    {
        point.x = px;
        point.y = py;
        seq = seq1;
        type = type1;
    }
    CCPoint point; //位置坐标
    unsigned int seq; //图中的编号
    int type; //类别，如是否可以停下来做其他动作
    vector<int> connectedPointSeqs;
};

class Walker :public CCNode
{
public:
	~Walker(){
        //m_enemyList->removeAllObjects();
        //CCLOG("$test battle soldier$");
    }

    Walker(CCNode* batchNode,CCNode* pNode,int armType,int side,std::string icon,std::string direct,bool isHead):m_side(side),m_completeTarget(NULL),m_status(WALKER_ACTION_STAND)
    ,m_batchNode(batchNode),m_pNode(pNode),m_icon(icon),m_armType(armType)
    ,m_iconSpr(NULL)
    ,m_key(std::string())
    ,m_position(0)
    ,m_isHead(isHead)
    ,m_direct(direct)
    ,m_moveSpd(90)
    ,m_lastDirect(direct)
    {};
    
    static Walker* create(CCNode* batchNode,CCNode* pNode,int armType,int side,std::string icon,std::string direct,bool isHead=false);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void playAnimation(WalkerActionStatus status,float delayTime=0.0,CCCallFunc* completeFunc=NULL,int loopTimes=0);
    Action* createAnimation();
    void delayPlayAnimation();
    
    bool init() override;
    void changeDirect(std::string direct,bool replay=false);
    void setSoldierPosition(CCPoint p);
    CCPoint getSoldierPosition();
    void playAttack(float t);
    //void moveToPosition(int currentSeq, int targetSeq, vector<CCPoint> posV,float delayTime=0, float moveSpd=0);
    void moveToPosition(vector<int> seqs, vector<CCPoint> posV,float delayTime=0, float moveSpd=0);

    void removeFromBatchNode(float dt=-1);
    void moveOnePoint(float t);
    void moveOnePoint();

    std::string getDirection(CCPoint from,CCPoint to);
    void destory();
    void playDie();
    void setSprScale(float sc);
    
    void startMove();
    void nextMove(int currentSeq);
    void nextWork(int pointType);
    void onMoveFinished();
    void onWorkFinished();
    void initPath();
    void addPointToPathGraph(WalkerPathPoint & pathPoint);
    void addEdgeToPathGraph(int seq1, int seq2);
    
    int m_side;
    CCPoint m_point;
    int m_status;
    int m_armType;
    int m_position;
    std::string m_icon;
    bool m_isHead;//
    std::string m_key;
    std::string m_direct;
    std::string m_lastDirect;
    
    vector<CCPoint> m_movePositions;
    vector<int> m_moveSeqs;
    int m_moveIndex;
    float m_moveSpd;
    
    virtual void update(float delta) override;
    
private:
    vector<cocos2d::CCPoint> mPath1;
    vector<cocos2d::CCPoint> mPath2;
    map<int, WalkerPathPoint> mPointsInPathGraph;
    int mTargetSeq;
    int mCurrentSeq;
    //CC_SAFE_CALLFUNC(m_completeTarget, m_completeFunc, CompleteFun);
cocos2d::CCObject* m_completeTarget;
cocos2d::SEL_CallFunc m_completeFunc;
public: virtual cocos2d::CCCallFunc* getCompleteFun(void) const {
if (m_completeTarget && m_completeFunc) {
return cocos2d::CCCallFunc::create(m_completeTarget,m_completeFunc);
}
return NULL;
}
public: virtual void setCompleteFun(cocos2d::CCCallFunc* func){
if (func && func->getTargetCallback() && func->getCallFunc()) {
    m_completeTarget = func->getTargetCallback();
    m_completeFunc = func->getCallFunc();
}
else {
    m_completeTarget = NULL;
    m_completeFunc = NULL;
}
}
    CC_SYNTHESIZE(int, m_loopTimes, LoopTimes);
    CC_SYNTHESIZE(CCNode*, m_batchNode, BatchNode);
    CC_SYNTHESIZE(CCNode*, m_container, Container);
    CC_SYNTHESIZE(CCSprite*, m_iconSpr, IconSpr);
    CC_SYNTHESIZE(CCNode*, m_pNode, PNode);
    CC_SYNTHESIZE(CCSprite*, m_shadow, Shadow);
};
#endif /* defined(__IF__Walker__) */
