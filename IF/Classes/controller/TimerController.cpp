//
//  TimerController.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-18.
//
//

#include "TimerController.h"

static TimerController *_instance = NULL;

TimerController* TimerController::getInstance() {
    if (!_instance) {
        _instance = new TimerController();
    }
    return _instance;
}

void TimerController::purgeData() {
    delete _instance;
    _instance = NULL;
}

TimerController::TimerController()
{
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(TimerController::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

TimerController::~TimerController() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(TimerController::onEnterFrame), this);
}

void TimerController::onEnterFrame(float dt)
{
    this->checkWarTime();
}

void TimerController::checkWarTime(){
    auto it = WorldController::getInstance()->m_worldActivity.find(FIGHT_OF_KING);
    if(it != WorldController::getInstance()->m_worldActivity.end() && it->second.currentState == PeaceTime){
        double curTime = WorldController::getInstance()->getTime();
        auto time = curTime+1000*59;
        auto m_time = (long)floor( it->second.timeInfo.startTime - curTime );
        m_time = m_time/1000;
        if(m_time == 900 || m_time == 600 || m_time == 300){
            string delayTime = CC_ITOA(m_time/60);
            string tip = _lang_1("110003", delayTime.c_str());
            CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_KING);
        }else if(m_time == 59){
            string tip = _lang_1("110004", CC_ITOA(m_time));
            CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_KING);
        }
    }
}

void TimerController::showSystemNotice(WorldActivityState state){
    if (state==WarTime) {
        double curTime = WorldController::getInstance()->getTime();
        auto time = curTime+1000*59;
        CCCommonUtils::flySystemUpdateHint(time, true,_lang("110095"),FLY_HINT_KING);
    }
}
