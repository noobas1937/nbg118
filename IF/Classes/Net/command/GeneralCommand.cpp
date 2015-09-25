//
//  GeneralCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-10.
//
//

#include "GeneralCommand.h"
#include "GeneralManager.h"
#include "PopupViewController.h"
#include "UIComponent.h"
#include "ToolController.h"

bool GeneralLevelUpCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(GENERAL_LEVEL_UP_COMMAND) != 0){
        return false;
    }
    return true;
}

bool GeneralFireCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(GENERAL_FIRE_COMMAND) != 0){
        return false;
    }
    return true;
}

bool GeneralAbilityChooseCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(GENERAL_ABILITY_CHOOSE_COMMAND) != 0){
        return false;
    }
    return true;
}

bool GeneralAbilityResetCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(GENERAL_ABILITY_RESET_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        GeneralManager::getInstance()->isSendingResetCommand = false;
        auto &info = GlobalData::shared()->generals[m_uid];
        if(params->objectForKey("ids")){
            GeneralManager::getInstance()->resetGeneralAbility(info, params->valueForKey("ids")->getCString());
        }
        if(m_index == -1){
//            PopupViewController::getInstance()->addPopupView(GeneralChooseAbilityPanel::create(info.abilityToChoose[0], info.abilityToChoose[1], &info, true), false);
        }else{
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GENERAL_ABILITY_RESET);
        }
        if(params->objectForKey("gold")){
            GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->intValue();
            UIComponent::getInstance()->updateGold(GlobalData::shared()->playerInfo.gold);
        }
    }

    return true;
}

bool GeneralSkillSaveCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(GENERAL_SKILL_SAVE_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
        if(m_flag){//单一的一次点存
            GeneralManager::getInstance()->saveSkillFail(m_abilityId,m_skillId);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(SAVE_SKILL_FAIL,CCString::create(m_skillId));
        }
        callFail(NetResult::create(Error_OK, params));
    }else{
        //todo
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(SAVE_SKILL_SUCCESS,CCString::create(m_skillId));
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}

bool GeneralSkillResetCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(GENERAL_SKILL_RESET) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        if(params->objectForKey("gold")){
            GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->intValue();
            UIComponent::getInstance()->updateGold(GlobalData::shared()->playerInfo.gold);
        }
        if(m_itemId!=""){
            auto &tool = ToolController::getInstance()->getToolInfoById(200040);
            int tCnt = MAX(tool.getCNT() - 1, 0);
            tool.setCNT(tCnt);
        }
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}
