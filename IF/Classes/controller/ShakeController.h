//
//  ShakeController.h
//  IF
//
//  Created by 童悦 on 15/8/7.
//
//

#ifndef __IF__ShakeController__
#define __IF__ShakeController__
#define COCOS2D_V3
#include "cocosextV3.h"
#include "base/CCEventListenerAcceleration.h"
#include "AllianceInfo.h"
class ShakeController : public CCObject
#ifndef COCOS2D_V3
,public CCAccelerometerDelegate
#endif
{
public:
    static ShakeController *getInstance();
    void initialize();
    void addAccelerometer();
    void removeAccelerometer();
    CC_SYNTHESIZE(bool, m_canShake, canshake);
    void resetShakeMonsterData(CCDictionary* pDic);
    std::vector <std::pair<int,int>> m_shakeDataVec;//index monsterId
    Vector<AllianceInfo*> m_shakeAllianceVec;//shake
    void resetShakeDate();
private:
    void setAccelerometerEnabled(bool enabled);
#if defined(COCOS2D_V3)
    void onAcceleration(Acceleration* acc, Event* unused_event);
    EventListenerAcceleration* _accelerationListener;
#else
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
#endif
    bool _accelerometerEnabled;
    int m_shakeCnt;
    int m_waitcount;
    int m_usecount;
    int m_changeDirectCount;
    int tmpx;
    int tmpy;
    int tmpz;
};
#endif
 /* defined(__IF__ShakeController__) */
