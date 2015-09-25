//
//  MainCityArmy.h
//  IF
//
//  Created by ganxiaohua on 14-7-4.
//
//

#ifndef __IF__MainCityArmy__
#define __IF__MainCityArmy__

#include "CommonInclude.h"
#include "HFViewport.h"
#include "BattleArmy.h"
#include "BattleSoldier2.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


class MainCityArmy :public CCLayer
{
public:
    MainCityArmy(){};
    ~MainCityArmy(){
    };
    static MainCityArmy* create(CCLayer* parent, CCSpriteBatchNode* soldierBatch, int buildId);

protected:
    bool init(CCLayer* parent, CCSpriteBatchNode* soldierBatch, int buildId);
    void onEnter();
    void onExit();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    bool MackSoldier(int type, int idx);
    void checkZOrder(CCObject* obj);
    void soldierStop(CCObject* obj);
    
    void changeBatchNode();
    void restart();
private:
    CCSafeObject<CCArray> m_soldiers;
    
    CCSpriteBatchNode* m_curBatch;
    CCSpriteBatchNode* m_soldierBatch;
    vector<cocos2d::CCPoint> m_posV;
    vector<int> m_zOrders;
    int m_soldierCnt;
    int m_stopCnt;
    bool m_isInGuide;
};
#endif /* defined(__IF__MainCityArmy__) */
