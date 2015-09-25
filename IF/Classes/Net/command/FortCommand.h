//
//  FortCommand.h
//  IF
//
//  Created by 李锐奇 on 14-3-4.
//
//

#ifndef __IF__FortCommand__
#define __IF__FortCommand__

#include "CommandBase.h"

#define FORT_START_COMMAND "fort.build"
#define FORT_END_COMMAND "fort.build.done"
#define FORT_START_USE_GOLD_COMMAND "fort.cd"
#define FORT_DISMISS_COMMAND "fort.destory"
#define FORT_CANCEL_COMMAND "fort.cancel"

class FortStartCommand : public CommandBase{
public:
    FortStartCommand(std::string itemId, int num,bool useGold):
    CommandBase(FORT_START_COMMAND)
    {
        m_id = itemId;
        m_num = num;
        m_useGold = useGold;
        putParam("id", CCString::create(itemId));
        putParam("num", CCInteger::create(num));
        putParam("gold", CCBoolean::create(useGold));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_id;
    int m_num;
    bool m_useGold;
};
/////

class FortEndCommand : public CommandBase{
public:
    FortEndCommand(std::string  itemId, std::string uuid):
    CommandBase(FORT_END_COMMAND)
    {
        m_id = itemId;
        putParam("id", CCString::create(itemId));
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_id;

};

///
class FortStartUseGoldCommand : public CommandBase{
public:
    FortStartUseGoldCommand(std::string itemId, int num, int max = 0):
    CommandBase(FORT_START_USE_GOLD_COMMAND)
    {
        m_id = itemId;
        putParam("id", CCString::create(itemId));
        putParam("num", CCInteger::create(num));
        if(max == 1){
            putParam("max", CCInteger::create(max));
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_id;
};

/////
class FortDismissCommand : public CommandBase{
public:
    FortDismissCommand(std::string armyId, int num):
    CommandBase(FORT_DISMISS_COMMAND)
    {
        m_id = armyId;
        putParam("id", CCString::create(armyId));
        putParam("num", CCInteger::create(num));
    }
    
protected:
    std::string m_id;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

/////
class FortCancelCommand : public CommandBase{
public:
    FortCancelCommand(std::string armyId, std::string uid):
    CommandBase(FORT_CANCEL_COMMAND)
    {
        m_id = armyId;
        putParam("id", CCString::create(armyId));
        putParam("uuid", CCString::create(uid));
    }
    
protected:
    std::string m_id;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
/////
class ResultParser{
public:
    static void parseResult(std::string cmd, std::string armyId, CCDictionary *dict);
};

//-----
#endif /* defined(__IF__FortCommand__) */
