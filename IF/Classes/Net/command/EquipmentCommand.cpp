//
//  EquipmentCommand.cpp
//  IF
//
//  Created by fubin on 14-12-3.
//
//

#include "EquipmentCommand.h"
#include "EquipmentController.h"

bool EquipmentCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(EQU_PUTON_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        EquipmentController::getInstance()->retPutOnEquipment(params1);
        return true;
    }
    else if (dict->valueForKey("cmd")->compare(EQU_TAKEOFF_COMMAND) == 0)
    {
        CCDictionary *params2=_dict(dict->objectForKey("params"));
        EquipmentController::getInstance()->retTakeOffEquipment(params2);
        return true;
    }
    else if (dict->valueForKey("cmd")->compare(EQU_DES_COMMAND) == 0)
    {
        CCDictionary *params3=_dict(dict->objectForKey("params"));
        EquipmentController::getInstance()->retDesEquipment(params3);
        return true;
    }
    else if (dict->valueForKey("cmd")->compare(EQU_SELL_LONG_COMMAND) == 0)
    {
        CCDictionary *params3=_dict(dict->objectForKey("params"));
        EquipmentController::getInstance()->retSellLongEquipment(params3);
        return true;
    }
    else
    {
        return false;
    }
}

bool EquipmentCreateCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(EQU_CT_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    else {
        EquipmentController::getInstance()->retCreateEquipment(params);
    }
    return true;
}

MaterialCreateCommand::MaterialCreateCommand(string itemId,int type):CommandBase(MATERIAL_CT_COMMAND),m_type(type){
    if(type == 1){
        auto iter = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
        std::string equips = "";
        while(iter!=EquipmentController::getInstance()->m_MyEquipmentMap.end()){
            if((*iter).second == atoi(itemId.c_str())){
                m_Uuids.push_back((*iter).first);
                equips = equips.empty()?(*iter).first:equips+","+(*iter).first;
            }
            if(m_Uuids.size()>=4){
                break;
            }
            ++iter;
        }
        if(m_Uuids.size()>=4){
            putParam("equips", CCString::create(equips));
        }
    }else{
        putParam("material", CCString::create(itemId));
    }
}
bool MaterialCreateCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MATERIAL_CT_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    else {
        if(m_type == 1){
            if(params->objectForKey("equipAdd")){
                CCArray* array = dynamic_cast<CCArray*>(params->objectForKey("equipAdd"));
                if(array){
                    EquipmentController::getInstance()->addEquip(array);
                }
                auto iter = m_Uuids.begin();
                while (iter!=m_Uuids.end()) {
                    auto iter1 = EquipmentController::getInstance()->m_MyEquipmentMap.find((*iter));
                    if(iter1!=EquipmentController::getInstance()->m_MyEquipmentMap.end()){
                        EquipmentController::getInstance()->m_MyEquipmentMap.erase(iter1);
                    }
                    ++iter;
                }
                m_Uuids.clear();
                
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MATE_CRT_END);
            }
        }else{
            EquipmentController::getInstance()->retMaterialCreate(params);
        }
    }
    return true;
}

bool HavestEquipCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(EQU_HAVEST_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    else {
        EquipmentController::getInstance()->retFinishCrtEquip(params);
    }
    return true;
}