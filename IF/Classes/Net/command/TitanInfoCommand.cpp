//
//  TitanInfoCommand.cpp
//  IF
//
//  Created by Émilie.Jiang on 15/7/24.
//
//

#include "TitanInfoCommand.h"
//#include "PopupViewController.h"
//#include "GeneralTitanPopupView.h"
#include "TitanController.h"

bool TitanInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TitanInfoCMD) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
//    //test code
//    if(params->objectForKey("drconfig")){
//        DynamicResourceController::getInstance()->initCommonInfo(params->objectForKey("drconfig"));
//    }
//    PopupBaseView* view = PopupViewController::getInstance()->getCurrentPopupView();
//    
//    GeneralTitanPopupView* titanView = dynamic_cast<GeneralTitanPopupView*>(view);
//    
//    if (titanView) {
//        titanView->resetAttribute(params);
//    }

    TitanController::getInstance()->parse(params);
    
    return true;
}