//
//  TreatCommand.h
//  IF
//
//  Created by 李锐奇 on 14-3-6.
//
//

#ifndef __IF__TreatCommand__
#define __IF__TreatCommand__

#include "CommandBase.h"

#define TREAT_START_COMMAND "hospital.cure"
#define TREAT_END_COMMAND "hospital.cure.done"
#define TREAT_START_USE_GOLD_COMMAND "hospital.cd"
#define TREAT_CANCEL_COMMAND "hospital.cancel"

class TreatStartCommand : public CommandBase{
public:
    TreatStartCommand(CCDictionary *dict,int isBuyRes=0):
    CommandBase(TREAT_START_COMMAND)
    {
        m_dict = dict;
        CCDictElement *element;
        CCDICT_FOREACH(dict, element) {
            std::string armyId = element->getStrKey();
            int num = dynamic_cast<CCInteger*>(element->getObject())->getValue();
            putParam(armyId, CCInteger::create(num));
        }
        putParam("isBuyRes", CCInteger::create(isBuyRes));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    CCSafeObject<CCDictionary> m_dict;
};
/////

class TreatEndCommand : public CommandBase{
public:
    TreatEndCommand(std::string uuid):
    CommandBase(TREAT_END_COMMAND)
    {
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);    
};

///
class TreatStartUseGoldCommand : public CommandBase{
public:
    TreatStartUseGoldCommand(CCDictionary *dict):
    CommandBase(TREAT_START_USE_GOLD_COMMAND)
    {
        CCDictElement *element;
        CCDICT_FOREACH(dict, element) {
            std::string armyId = element->getStrKey();
            int num = dynamic_cast<CCInteger*>(element->getObject())->getValue();
            putParam(armyId, CCInteger::create(num));
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
//---
class TreatCancelCommand : public CommandBase{
public:
    TreatCancelCommand(std::string uuid):
    CommandBase(TREAT_CANCEL_COMMAND)
    {
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


class TreatResultParser{
public:
    static void parseResult(std::string cmd, CCDictionary *dict);
};

#endif /* defined(__IF__TreatCommand__) */
