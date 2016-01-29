//
//  DRConfigCommand.cpp
//  IF
//
//  Created by Émilie.Jiang on 15/7/24.
//
//

#include "TitanFeedCommand.h"
#include "TitanController.h"
//#include "PopupViewController.h"
//#include "GeneralTitanPopupView.h"

bool TitanFeedCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TitanFeed) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
//    //test code

//    PopupBaseView* view = PopupViewController::getInstance()->getCurrentPopupView();
//    
//    GeneralTitanPopupView* titanView = dynamic_cast<GeneralTitanPopupView*>(view);
//    
//    if (titanView) {
//        titanView->resetAttribute(params);
//    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")==0) {;
        
        TitanController::getInstance()->parse(params);
        
        auto &resourceInfo = GlobalData::shared()->resourceInfo;
        resourceInfo.setResourceData(params);//只是为了刷新food
        
        
//        TitanController::getInstance()->parse(_dict(params->objectForKey("params")));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Feed"));//fusheng 喂食成功
        
        return true;
    }
    else
    {
        TitanController::getInstance()->handleErrorCode(params->valueForKey("errorCode")->intValue());
    }
    return true;
    
}