//
//  ScienceCommand.cpp
//  IF
//
//  Created by 邹 程 on 13-11-4.
//
//

#include "ScienceCommand.h"
#include "UIComponent.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "ScienceController.h"

bool ScienceResearchFinishCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SCIENCE_RESEARCH_FINISH) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    ScienceController::getInstance()->retFinishScience(params);
    
    return true;
    
    if (!params || strcmp(params->valueForKey("itemId")->getCString(), m_scienceId.c_str()) != 0) {
        return false;
    }
    
    if (!params->objectForKey("errorCode")) {
        GlobalData::shared()->scienceInfo.currentResearchId = 0;
        auto &info = GlobalData::shared()->scienceInfo.infoMap[atoi(m_scienceId.c_str())];
        info.initItemInfo(params);
        string key = params->valueForKey("effect")->getCString();
        int value = params->valueForKey("value")->intValue();
        auto &effectDict = GlobalData::shared()->scienceEffect;
        if (effectDict.find(key) != effectDict.end()) {
            // update effect
            effectDict[key] = value;
            
            if (key.compare("204")==0) {
                auto &buildsInfo = GlobalData::shared()->imperialInfo;
//                if (buildsInfo.find(FUN_BUILD_SILVER_STORGE)!=buildsInfo.end()) {
//                    FunBuildController::getInstance()->updateDataAboutBuild(FUN_BUILD_SILVER_STORGE);
//                }
//                else{
//                    float pro = GlobalData::shared()->scienceEffect["204"]*1.0;
//                    GlobalData::shared()->resourceInfo.lStorageMoney = GlobalData::shared()->resourceInfo.lStorageMoney * (1+pro/100);
//                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
//                }
            }
            else if (key.compare("205")==0) {
                auto &buildsInfo = GlobalData::shared()->imperialInfo;
                if (buildsInfo.find(FUN_BUILD_WOOD_STORGE)!=buildsInfo.end()) {
                    FunBuildController::getInstance()->updateDataAboutBuild(FUN_BUILD_WOOD_STORGE);
                }
                else{
                    float pro = GlobalData::shared()->scienceEffect["205"]*1.0;
                    GlobalData::shared()->resourceInfo.lStorageWood = GlobalData::shared()->resourceInfo.lStorageWood * (1+pro/100);
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
                }
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_RESEARCH_FINISH);
    } else {
        string errorCode = params->valueForKey("errorCode")->getCString();
        if (errorCode.compare("E100001") == 0) {
            YesNoDialog::gotoPayTips();
        } else {
            YesNoDialog::showYesDialog(_lang(errorCode).c_str());
        }
    }
    return true;
}



bool ScienceInstantFinishCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SCIENCE_INSTANT_FINISH) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params || strcmp(params->valueForKey("itemId")->getCString(), m_scienceId.c_str()) != 0) {
        return false;
    }
    
    if (!params->objectForKey("errorCode")) {
        UIComponent::getInstance()->updateGold(params->valueForKey("GOLD")->intValue());
        GlobalData::shared()->scienceInfo.currentResearchId = 0;
        auto &info = GlobalData::shared()->scienceInfo.infoMap[atoi(m_scienceId.c_str())];
        info.initItemInfo(params);
        string key = params->valueForKey("effect")->getCString();
        int value = params->valueForKey("value")->intValue();
        auto &effectDict = GlobalData::shared()->scienceEffect;
        if (effectDict.find(key) != effectDict.end()) {
            // update effect
            effectDict[key] = value;
            
            if (key.compare("204")==0) {
                auto &buildsInfo = GlobalData::shared()->imperialInfo;
//                if (buildsInfo.find(FUN_BUILD_SILVER_STORGE)!=buildsInfo.end()) {
//                    FunBuildController::getInstance()->updateDataAboutBuild(FUN_BUILD_SILVER_STORGE);
//                }
//                else{
//                    float pro = GlobalData::shared()->scienceEffect["204"]*1.0;
//                    GlobalData::shared()->resourceInfo.lStorageMoney = GlobalData::shared()->resourceInfo.lStorageMoney * (1+pro/100);
//                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
//                }
            }
            else if (key.compare("205")==0) {
                auto &buildsInfo = GlobalData::shared()->imperialInfo;
                if (buildsInfo.find(FUN_BUILD_WOOD_STORGE)!=buildsInfo.end()) {
                    FunBuildController::getInstance()->updateDataAboutBuild(FUN_BUILD_WOOD_STORGE);
                }
                else{
                    float pro = GlobalData::shared()->scienceEffect["205"]*1.0;
                    GlobalData::shared()->resourceInfo.lStorageWood = GlobalData::shared()->resourceInfo.lStorageWood * (1+pro/100);
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
                }
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_RESEARCH_FINISH);
    } else {
        string errorCode = params->valueForKey("errorCode")->getCString();
        if (errorCode.compare("E100001") == 0) {
            YesNoDialog::gotoPayTips();
        } else {
            YesNoDialog::showYesDialog(_lang(errorCode).c_str());
        }
    }
    return true;
}





bool ScienceDirectlyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SCIENCE_DIRECTLY) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    ScienceController::getInstance()->retDirResearchScience(params);
    return true;
    
    if (!params || strcmp(params->valueForKey("itemId")->getCString(), m_scienceId.c_str()) != 0) {
        return false;
    }
    
    if (!params->objectForKey("errorCode")) {
        // update resource
        auto &resourceInfo = GlobalData::shared()->resourceInfo;
        resourceInfo.setResourceData(_dict(params->objectForKey("usercity")));
        auto &info = GlobalData::shared()->scienceInfo.infoMap[atoi(m_scienceId.c_str())];
        info.investNum = params->valueForKey("injectNums")->intValue();
    }
    
    auto ret = NetResult::create();
    auto retDict = CCDictionary::create();
    retDict->setObject(CCInteger::create(1), "type");
    CCDictElement* pObj = NULL;
    CCDICT_FOREACH(params, pObj) {
        retDict->setObject(pObj->getObject(), pObj->getStrKey());
    }
    ret->setData(retDict);
    callSuccess(ret);
    return true;
}


bool ScienceResearchCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SCIENCE_RESEARCH) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    ScienceController::getInstance()->retResearchScience(params);
    return true;
}

bool ScienceDataInitCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SCIENCE_DATA_INIT) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (params) {
        auto scienceInfo = dynamic_cast<CCArray*>(params->objectForKey("science"));
        GlobalData::shared()->scienceInfo.initSelf();
        if (scienceInfo) {
            GlobalData::shared()->scienceInfo.initScienceState(scienceInfo);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_DATA_REFRESH);
        }
        
        CCArray *alScienceArr = dynamic_cast<CCArray*>(params->objectForKey("allianceScience"));
        if (alScienceArr) {
            ScienceController::getInstance()->initAllianceScienceData(alScienceArr);
        }
    }
    return true;
}

