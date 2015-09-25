//
//  BattleArmy.h
//  IF
//
//  Created by ganxiaohua on 14-6-18.
//
//

#ifndef __IF__BattleArmy__
#define __IF__BattleArmy__

#include "CommonInclude.h"
#include "CCLoadSprite.h"
#include "BattleSoldier2.h"
#include "SpriteSheetAni.h"

class BattleArmy :public cocos2d::CCNode
{
public:
    static BattleArmy* create(CCSpriteBatchNode* batchNode, CCSpriteBatchNode* soldierNode,CCPoint pt, int type,int side,int key=0);
    
    BattleArmy(CCSpriteBatchNode* batchNode, CCSpriteBatchNode* soldierNode, CCPoint pt, int type, int side, int key):
    m_type(type)
    ,m_side(side)
    ,m_key(key)
    ,m_pos(pt)
    ,m_jianNode(batchNode)
    ,m_soldierNode(soldierNode){};
    
    bool init();
    
    ~BattleArmy(){};
    void playAnimation(ActionStatus status,float delayTime=0.0,CCCallFunc* completeFunc=NULL,int loopTimes=0, int moveType = 0);
    void changeDirect(std::string direct,bool replay=false);
    void destory();
    void hideSoldier(float dt);
    int getType();
    CCSafeObject<CCArray> m_soldiers;
    CCSafeObject<CCNode> m_particleNode;
protected:
    void handeDieSoldierHandle(float _time);
    
    std::string m_icon;
    int m_row;
    int m_col;
    int m_type;
    int m_side;
    int m_key;
    CCSpriteBatchNode* m_jianNode;
    CCSpriteBatchNode* m_soldierNode;
    CCPoint m_pos;
    std::string m_direct;
};
#endif /* defined(__IF__BattleArmy__) */
