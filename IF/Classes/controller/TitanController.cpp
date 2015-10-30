//
//  TitanController.cpp
//  IF
//
//  Created by fusheng on 15-9-21.
//
//

#include "TitanController.h"
#include "GlobalData.h"
#include "CCSafeNotificationCenter.h"
#include "TitanInfoCommand.h"
#include <limits>
static TitanController *_instance = NULL;

TitanController* TitanController::getInstance() {
    if (!_instance) {
        _instance = new TitanController();
    }
    return _instance;
}

TitanController::TitanController()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TitanController::TitanComplete), MSG_TITAN_SPEED_UP_COMPLETE, NULL);
    
    isUpdate = false;
    
    if(GlobalData::shared()->titanInfo.lastCalTime == -1)//设置lastCalTime默认值
        GlobalData::shared()->titanInfo.resetLastCalTime();

    //this->scheduleUpdate();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

TitanController::~TitanController() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TITAN_SPEED_UP_COMPLETE);
    this->unscheduleUpdate();
}

void TitanController::TitanComplete(CCObject *obj){

//    if () {
//        
//    }
    
    CCString* str = dynamic_cast<CCString*>(obj);
    
    if (str->compare("feed")==0) {

        GlobalData::shared()->titanInfo.feedcd = 0;
        
        GlobalData::shared()->titanInfo.feedcdfix = 0;
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_INFORMATION_RESET,NULL );
    }
    else
    {
        TitanInfoCommand *ticmd = new TitanInfoCommand();
        ticmd->sendAndRelease();

    }
    
   

}

void TitanController::parse(CCDictionary *dict)
{
    
    int status = GlobalData::shared()->titanInfo.resetTitanInfo(dict);
    
    
    
    if (status&TITANVALUECHANGE) {//fusheng 泰坦数值改变
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_INFORMATION_RESET,dict );
        

        isUpdate = true;
        
//        if (this->isScheduled("calTitanManual"))
//        {
//            //Director::getInstance()->getScheduler()->unschedule("calTitanManual", this);
//            this->unschedule("calTitanManual");
//            
//       
//        }
//        unsigned int rep = (numeric_limits<unsigned int>::max)();
//        
//        //Director::getInstance()->getScheduler()->schedule(schedule_selector(TitanController::updateManual), this, 5, rep, false, "calTitanManual");
//        
//        this->schedule(
//        [](float time)
//        {int manual = GlobalData::shared()->titanInfo.currentManual+1;
//            if(GlobalData::shared()->titanInfo.maxManual>manual)
//            {
//                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_INFORMATION_RESET,NULL );
//                GlobalData::shared()->titanInfo.currentManual =  manual;
//            }
//            
//        }, 5, rep, 0, "calTitanManual");

    }
    
    if (status&TITANSTATUECHANGE) { //fusheng 泰坦状态改变
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_STATUS_CHANGE,dict );
    }
    
    if (status&TITANUPGRADE)
    {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_UPGRADE_COMPLETE,dict );
    }
    
}

void TitanController::handleErrorCode(int errorCode)
{
    Error code = (Error)errorCode;
    switch (code) {
        case Error::TITAN_NO_ENOUGH_TILI:
            
        {
            CCCommonUtils::flyHint("","","Insufficient Titan manual");
            TitanInfoCommand *ticmd = new TitanInfoCommand();
            ticmd->sendAndRelease();
            
            break;
        }
        case Error::TITAN_ALL_READLY_GO:
        {
//            CCCommonUtils::flyText("data error ");
            TitanInfoCommand *ticmd = new TitanInfoCommand();
            ticmd->sendAndRelease();
            break;
        }
        case Error::TITAN_EXP_FULL:
        {
            TitanInfoCommand *ticmd = new TitanInfoCommand();
            ticmd->sendAndRelease();
           // CCCommonUtils::flyText("data error ");
            
            break;
        }
        case Error::TITAN_NO_FEED_NUM :
        {
            // CCCommonUtils::flyText("data error ");
            TitanInfoCommand *ticmd = new TitanInfoCommand();
            ticmd->sendAndRelease();
            
            break;
        }
        case Error::TITAN_NEED_WAIT :
        {
            // CCCommonUtils::flyText("data error ");
//            CCCommonUtils::flyHint("","","feed CD error please refresh");
            TitanInfoCommand *ticmd = new TitanInfoCommand();
            ticmd->sendAndRelease();
            
            break;
        }
        case Error::TITAN_NOT_ENOUGH_FOOD :
        {
            
//            CCCommonUtils::flyText("data error food not enough");
            TitanInfoCommand *ticmd = new TitanInfoCommand();
            ticmd->sendAndRelease();
            
            break;
        }
        default:
        {
            
           // CCCommonUtils::flyText("data error ");
            TitanInfoCommand *ticmd = new TitanInfoCommand();
            ticmd->sendAndRelease();
            
            break;
        }
    }
}

void TitanController::update(float dt)
{
    if(!isUpdate)
        return;
    TitanInfo &ti =GlobalData::shared()->titanInfo;
    
    if(ti.currentManual == -1 || ti.lastCalTime == -1){
        return;
    }
    
    

    
    int recoverInterval = ti.recoverInterval;
    
    double dTime = (ti.lastCalTime + recoverInterval ) - GlobalData::shared()->getWorldTime();
    if(dTime < 0 && ti.currentManual < ti.maxManual){
        int t = (GlobalData::shared()->getWorldTime() - ti.lastCalTime) / recoverInterval;
        int num = ti.currentManual + ti.recoverymanual*t;
        if(num > ti.maxManual){
            num = ti.maxManual;
        }
        ti.resetLastCalTime();
        ti.currentManual = num;
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_INFORMATION_RESET,NULL );

    }
    if(ti.currentManual >= ti.maxManual){
        ti.resetLastCalTime();
    }
    
    
    
    
    
}