//
//  EquipmentCommand.h
//  IF
//
//  Created by fubin on 14-12-3.
//
//

#ifndef IF_EquipmentCommand_h
#define IF_EquipmentCommand_h

#include "CommandBase.h"
#define EQU_CT_COMMAND "equipment.create"
#define EQU_PUTON_COMMAND "equip.putOn"
#define EQU_TAKEOFF_COMMAND "equip.takeOff"
#define EQU_DES_COMMAND "equipment.break"
#define MATERIAL_CT_COMMAND "material.create"
#define EQU_HAVEST_COMMAND "equip.q.finish"
#define EQU_SELL_LONG_COMMAND "dragonshard.sell"

class EquipmentCreateCommand;
class MaterialCreateCommand;

class EquipmentCommand: public CommandBase {
public:
    EquipmentCommand(string uuid, string cmd):
    CommandBase(cmd){
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class EquipmentCreateCommand: public CommandBase {
public:
    EquipmentCreateCommand(string uuid, string qItemId, vector<int> toolItemIds, int gold=0):
    CommandBase(EQU_CT_COMMAND){
        putParam("uuid", CCString::create(uuid));
        putParam("itemId", CCString::create(qItemId));
        auto arr = CCArray::create();
        for (int i=0; i<toolItemIds.size(); i++) {
            arr->addObject(CCInteger::create(toolItemIds[i]));
        }
        putParam("tools", arr);
        _gold = gold;
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int _gold;
};

class MaterialCreateCommand: public CommandBase {
public:
    MaterialCreateCommand(string itemId,int type);
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    vector<string> m_Uuids;
    int m_type; //0-材料 1-装备
};

class HavestEquipCommand: public CommandBase {
public:
    HavestEquipCommand(string quuid):
    CommandBase(EQU_HAVEST_COMMAND){
        putParam("qUUID", CCString::create(quuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif
