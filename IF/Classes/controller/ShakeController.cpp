//
//  ShakeController.cpp
//  IF
//
//  Created by 童悦 on 15/8/7.
//
//

#include "ShakeController.h"
#include "CCSafeNotificationCenter.h"
#include "TypeDefinition.h"
#include "CCCommonUtils.h"
#include "WorldController.h"
static ShakeController *_instance = NULL;

ShakeController* ShakeController::getInstance() {
    if (!_instance) {
        _instance = new ShakeController();
        _instance->initialize();
    }
    return _instance;
}


void ShakeController::initialize()
{
#if defined(COCOS2D_V3)
    _accelerationListener = EventListenerAcceleration::create(CC_CALLBACK_2(ShakeController::onAcceleration, this));
    _accelerometerEnabled = false;
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_accelerationListener, -1);
#endif
    m_usecount = 0;
    m_shakeCnt = 0;
    m_waitcount = 0;
    m_changeDirectCount = 0;
    tmpx = 0;
    tmpy = 0;
    tmpz = 0;
    m_canShake = true;
}

void ShakeController::addAccelerometer()
{
    m_usecount++;
    if(!_accelerometerEnabled)
    {
        setAccelerometerEnabled(true);
    }
}
void ShakeController::removeAccelerometer()
{
    m_usecount--;
    if(m_usecount<=0&&_accelerometerEnabled)
    {
        m_usecount =0;
        setAccelerometerEnabled(false);
    }
}

void ShakeController::setAccelerometerEnabled(bool enabled)
{
    if (enabled != _accelerometerEnabled)
    {
        _accelerometerEnabled = enabled;
#if defined(COCOS2D_V3)
        Device::setAccelerometerEnabled(enabled);
        Device::setAccelerometerInterval(1.0f/30.0f);
#else
        if (!enabled) {
            CCDirector::sharedDirector()->getAccelerometer()->setDelegate(nullptr);
        }
        else{
            CCDirector::sharedDirector()->getAccelerometer()->setDelegate(this);
            CCDirector::sharedDirector()->getAccelerometer()->setAccelerometerInterval(1.0f/30.0f);
        }
#endif
    }
}
#if defined(COCOS2D_V3)
void ShakeController::onAcceleration(Acceleration* acc, Event* unused_event)
{
    float x = acc->x,
    y = acc->y,
    z = acc->z;
    float all = x*x + y*y + z*z;
    if(all < 1.2) {
        if(m_waitcount>5&&m_shakeCnt > 5&&m_changeDirectCount>2){
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ON_SHAKE);
            m_shakeCnt = 0;
            m_waitcount = 0;
            tmpx = 0;
            tmpy = 0;
            tmpz = 0;
            m_changeDirectCount = 0;
        }
        if(m_waitcount>8)
        {
            m_shakeCnt = 0;
            tmpx = 0;
            tmpy = 0;
            tmpz = 0;
            m_changeDirectCount = 0;
        }
        m_waitcount ++;
    }
    else if(all > 2) {
        float dir = tmpx*(x-tmpx)+tmpy*(y-tmpy)+tmpz*(z-tmpz);
        if(dir<0)
        {
            m_changeDirectCount++;
        }
        tmpx = x;
        tmpy = y;
        tmpz = z;
        m_shakeCnt++;
        m_waitcount = 0;
    }
}
#else
void ShakeController::didAccelerate(CCAcceleration* pAccelerationValue)
{
    float x = pAccelerationValue->x,
    y = pAccelerationValue->y,
    z = pAccelerationValue->z;
    float all = x*x + y*y + z*z;
    if(all < 1.2) {
        if(m_waitcount>5&&m_shakeCnt > 5&&m_changeDirectCount>2){
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ON_SHAKE);
            m_shakeCnt = 0;
            m_waitcount = 0;
            tmpx = 0;
            tmpy = 0;
            tmpz = 0;
            m_changeDirectCount = 0;
        }
        if(m_waitcount>8)
        {
            m_shakeCnt = 0;
            tmpx = 0;
            tmpy = 0;
            tmpz = 0;
            m_changeDirectCount = 0;
        }
        m_waitcount ++;
    }
    else if(all > 2) {
        float dir = tmpx*(x-tmpx)+tmpy*(y-tmpy)+tmpz*(z-tmpz);
        if(dir<0)
        {
            m_changeDirectCount++;
        }
        tmpx = x;
        tmpy = y;
        tmpz = z;
        m_shakeCnt++;
        m_waitcount = 0;
    }
}
#endif
void ShakeController::resetShakeMonsterData(CCDictionary* pDic){
    if (!pDic) {
        return;
    }
    m_shakeDataVec.clear();
    if (pDic->objectForKey("monsters")) {
        auto dataArr = dynamic_cast<CCArray*>(pDic->objectForKey("monsters"));
        if (dataArr->count() > 0) {
            CCObject* pEle = NULL;
            CCARRAY_FOREACH(dataArr, pEle){
                auto dic = _dict(pEle);
                int index = dic->valueForKey("id")->intValue();
                int monsterId = dic->valueForKey("pointItem")->intValue();
                m_shakeDataVec.push_back(pair<int, int>(index,monsterId));
            }
        }
    }
    if (m_shakeDataVec.size() > 1) {
       int myIndex = WorldController::getInstance()->getIndexByPoint(WorldController::getInstance()->selfPoint);
        map<int ,pair<int,int>>tempMap;
        auto item = m_shakeDataVec.begin();
        while (item != m_shakeDataVec.end()) {
            int def = abs(item->first - myIndex) ;
            tempMap.insert(make_pair(def, make_pair(item->first, item->second)));
            item++;
        }
        m_shakeDataVec.clear();
        
        auto tempIt = tempMap.begin();
        while (tempIt != tempMap.end()) {
            m_shakeDataVec.push_back(tempIt->second);
            tempIt++;
        }
    }
//    CCLOG("shakeLog: data size %d",m_shakeDataVec.size());
//    auto tempVec = m_shakeDataVec.begin();
//    while (tempVec != m_shakeDataVec.end()) {
//        CCLOG("testwdz   index %d,  id  %d",tempVec->first,tempVec->second);
//        tempVec++;
//    }
}
void ShakeController::resetShakeDate(){
    m_shakeAllianceVec.clear();
    m_shakeAllianceVec.clear();
}