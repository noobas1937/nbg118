//
//  FlyCell.h
//  IF
//
//  Created by fubin on 14-7-31.
//
//

#ifndef IF_FlyCell_h
#define IF_FlyCell_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"

class FlyCell: public CCNode
{
public:
    FlyCell():m_batchNode(NULL),m_type(0){};
    ~FlyCell(){};
    
    //资源收获效果
    static FlyCell* create(CCPoint spt, CCPoint ept, int type, string picPath, int picCnt, CCSpriteBatchNode* batchNode, float duration);
    //码头货船收金币效果
    static FlyCell* createCircle(CCPoint spt, CCPoint ept, string picPath, int picCnt, CCSpriteBatchNode* batchNode, float duration, float param1,float param2);
    //码头建筑光效
    static FlyCell* createCycleLight(string picPath,float height,int idx,int cnt,CCSpriteBatchNode* batchNode,ccColor3B &color,CCPoint spt);
    virtual void onEnter();
    virtual void onExit();
    void onFlyCellDelete();
    void onFlyCellCycle(CCObject *ccObj);
protected:
    bool initFlyCell(CCPoint spt, CCPoint ept, int type, string picPath, int picCnt, CCSpriteBatchNode* batchNode, float duration);
    bool initCircleFly(CCPoint spt, CCPoint ept, string picPath, int picCnt, CCSpriteBatchNode* batchNode, float duration, float param1,float param2);
    bool initCycleLight(string picPath,float height,int idx,int cnt,CCSpriteBatchNode* batchNode,ccColor3B &color,CCPoint spt);
    
    CCSpriteBatchNode* m_batchNode;
    CCSafeObject<CCSprite> m_Spr;
    int m_type;
private:
    float m_cycleH;
    int m_cycleIDX;
    int m_cycleCNT;
    CCPoint m_cyclePoint;
    
};

#endif
