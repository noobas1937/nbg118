//
//  AdRewardPush.cpp
//  IF
//
//  Created by chenliang on 14/12/31.
//
//

#include "AdRewardPush.h"
#include "YesNoDialog.h"
#include "GuideController.h"

void AdRewardPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    if(!GuideController::share()->isInTutorial()){
        YesNoDialog::showConfirmCloseAllView(_lang("114134").c_str(), _lang("confirm").c_str());
    }
}