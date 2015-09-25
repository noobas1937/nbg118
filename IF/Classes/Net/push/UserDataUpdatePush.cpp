//
//  UserDataUpdatePush.cpp
//  IF
//
//  Created by wangzhenlei on 13-10-15.
//
//

#include "UserDataUpdatePush.h"
#include "UIComponent.h"
#include "YesNoDialog.h"
#include "GameController.h"

void UserDataUpdatePush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params)
        return;
    
    const CCString *pStr = params->valueForKey("level");
    auto &general = GlobalData::shared()->generals.begin()->second;
    
    if(GlobalData::shared()->playerInfo.level != pStr->intValue())
    {
        auto tmp = CCDictionary::create();
        if (params->objectForKey("gift_AP")) {
            int actionPoint = params->valueForKey("gift_AP")->intValue();
            auto &lordInfo = GlobalData::shared()->lordInfo;
            lordInfo.energy += actionPoint;
            if (lordInfo.energy > lordInfo.physical_k1) {
                lordInfo.energy = lordInfo.physical_k1;
            }
            
            if (params->objectForKey("has_gift")) {
                // hold Energy update animation for delay play
//                UIComponent::getInstance()->removeEnergyUpdate();
            }
            
            tmp->setObject(CCInteger::create(actionPoint), "gift2");
        }
        if (params->objectForKey("has_gift")) {
            tmp->setObject(CCBool::create(true), "has_gift");
        }
        GlobalData::shared()->playerInfo.level = pStr->intValue();
        general.level = GlobalData::shared()->playerInfo.level;

        pStr = params->valueForKey("att");
        general.attack = pStr->intValue();
        pStr = params->valueForKey("defence");
        general.defense = pStr->intValue();

        // client user level save
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_USER_LEVEL, pStr->intValue());
        if(GlobalData::shared()->playerInfo.level>1){
            int userLv = GlobalData::shared()->playerInfo.level;
            //if(userLv<GlobalData::shared()->playerInfo.level){
                int key = 100100 + userLv-1;
                auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(key));
                string item =  dict->valueForKey("level_item")->getCString();
                if(item!=""){
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAYER_LEVELUP,tmp);
                }else{
                    GlobalData::shared()->lordInfo.levelUp+=1;
                }
            //}
            
        }
        
    } else {
        GlobalData::shared()->playerInfo.level = pStr->intValue();
    }
    pStr = params->valueForKey("exp");
    GlobalData::shared()->playerInfo.exp = pStr->intValue();
    pStr = params->valueForKey("maxExp");
    GlobalData::shared()->playerInfo.maxExp = pStr->intValue();
    UIComponent::getInstance()->setUserData();
    
    general.currExp = GlobalData::shared()->playerInfo.exp;
    general.maxExp = GlobalData::shared()->playerInfo.maxExp;
    
    CCArray *generalArr = dynamic_cast<CCArray*>(params->objectForKey("userGenerals"));
    if(generalArr){
        map<string,GeneralInfo> *generals = &(GlobalData::shared()->generals);

        CCObject *generalItem;
        const CCString *pStrGeneral;
        CCARRAY_FOREACH(generalArr,generalItem){
            pStrGeneral = _dict(generalItem)->valueForKey("generalId");
            if(pStrGeneral->compare("")!=0){
                auto it = generals->find(pStrGeneral->getCString());
                it->second.updateGeneralData(_dict(generalItem),false);
            }
        }
    }
    
    CCDictionary *paramsResource=_dict(params->objectForKey("cityResource"));
    if(paramsResource){
        auto &resourceInfo = GlobalData::shared()->resourceInfo;
        resourceInfo.setResourceData(paramsResource);
    }
}

void ServerStop::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params)
        return;
    
    int _type = params->valueForKey("t")->intValue();
    switch (_type) {
        case 0:
            break;
        case 1:
        {
//            auto quitGame = [](){
//                GameController::getInstance()->quitGame();
//            };
//            YesNoDialog::show(_lang("E100069").c_str(), quitGame);
            YesNoDialog::showVariableTitle(_lang("E100069").c_str(),CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame), NULL),_lang("confirm").c_str(),true);
        }
            break;
            
        default:
            break;
    }
}