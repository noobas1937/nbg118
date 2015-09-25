//
//  TimerController.h
//  IF
//
//  Created by ganxiaohua on 14-12-18.
//
//

#ifndef __IF__TimerController__
#define __IF__TimerController__

#include "CommonInclude.h"
#include "UIComponent.h"
#include "WorldController.h"

class TimerController : public CCObject{
public:
    static TimerController* getInstance();
    static void purgeData();
    TimerController();
    ~TimerController();

    void onEnterFrame(float dt);
    void checkWarTime();
    void showSystemNotice(WorldActivityState state);
};

#endif /* defined(__IF__TimerController__) */
