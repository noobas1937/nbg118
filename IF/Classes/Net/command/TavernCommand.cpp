//
//  TavernCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-20.
//
//

#include "TavernCommand.h"
#include "GeneralManager.h"
#include "UIComponent.h"
#include "PopupViewController.h"

bool GeneralRecruitCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(GENERAL_RECRUIT) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    return true;

}

bool GamblePickCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(GENERAL_PICK_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));

//    if (!params) {
//        return false;
//    }
//    const CCString *pStr = params->valueForKey("errorCode");
//    if (pStr->compare("")!=0) {
//        GeneralGambleController::getInstance()->resetUnOpenGeneral(m_pos);
//        CCCommonUtils::flyText(_lang(pStr->getCString()));
//        return true;
//    }else{
//        pStr = params->valueForKey("gold");
//        if(pStr->compare("") != 0){
//            int gold = atoi(pStr->getCString());
//            UIComponent::getInstance()->updateGold(gold);
//        }
//
//        GeneralGambleController::getInstance()->setGeneralOpen(params, m_pos);
//    }
    return true;
}