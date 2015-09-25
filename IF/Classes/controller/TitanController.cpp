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

static TitanController *_instance = NULL;

TitanController* TitanController::getInstance() {
    if (!_instance) {
        _instance = new TitanController();
    }
    return _instance;
}

TitanController::TitanController()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TitanController::resetFeedCD), MSG_TITAN_FEED_SPEED_UP_COMPLETE, NULL);
 
}

TitanController::~TitanController() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TITAN_FEED_SPEED_UP_COMPLETE);
}

void TitanController::resetFeedCD(CCObject *obj){

//    if () {
//        
//    }
    
    GlobalData::shared()->titanInfo.feedcd = 0;
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_INFORMATION_RESET,NULL );

}

void TitanController::parse(CCDictionary *dict)
{
    
    int status = GlobalData::shared()->titanInfo.resetTitanInfo(dict);
    
    if (status&1) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_INFORMATION_RESET,dict );

    }
    
    if (status&2) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_STATUS_CHANGE,dict );
    }
    
}

void TitanController::handleErrorCode(int errorCode)
{
    Error code = (Error)errorCode;
    switch (code) {
        case Error::TITAN_NO_ENOUGH_TILI:
            
        {
            CCCommonUtils::flyText("Insufficient Titan manual");
            TitanInfoCommand *ticmd = new TitanInfoCommand();
            ticmd->sendAndRelease();
            
            break;
        }
        case Error::TITAN_ALL_READLY_GO:
        {
            CCCommonUtils::flyText("data error ");

            break;
        }
        case Error::TITAN_EXP_FULL:
        {
            
           // CCCommonUtils::flyText("data error ");
            
            break;
        }
        case Error::TITAN_NO_FEED_NUM :
        {
            // CCCommonUtils::flyText("data error ");
            
            break;
        }
        case Error::TITAN_NEED_WAIT :
        {
            // CCCommonUtils::flyText("data error ");
            
            break;
        }
        case Error::TITAN_NOT_ENOUGH_FOOD :
        {
            
            CCCommonUtils::flyText("data error food not enough");
            
            break;
        }
        default:
        {
            
           // CCCommonUtils::flyText("data error ");
            
            break;
        }
    }
}