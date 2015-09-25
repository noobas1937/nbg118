//
//  TroopsCommand.h
//  IF
//
//  Created by fubin on 14-3-6.
//
//

#ifndef IF_TroopsCommand_h
#define IF_TroopsCommand_h

#include "CommandBase.h"
#define TROOPS_DEFENCE_COMMAND "wall.defence"
#define TROOPS_PREPARE_COMMAND "wall.prepare"
#define TROOPS_BATTLE_COMMAND "wall.battle"
#define TROOPS_AUTO_DEF_COMMAND "wall.auto.def"

class TroopsCommand: public CommandBase {
public:
    TroopsCommand(string cmd, string info, int autoType=-1):
    CommandBase(cmd){
        std::vector<string> items;
        CCCommonUtils::splitString(info, "|", items);
        for(int i=0; i<items.size(); i++)
        {
            std::vector<string> cells;
            CCCommonUtils::splitString(items[i], ":", cells);
            if(cells.size()>=2){
                putParam(cells[0], CCInteger::create(atoi(cells[1].c_str())));
            }
        }
        
        if(autoType>=0){
            putParam("autoType", CCInteger::create(autoType));
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class TroopsAutoCommand: public CommandBase {
public:
    TroopsAutoCommand(int autoType):
    CommandBase(TROOPS_AUTO_DEF_COMMAND){
        putParam("autoType", CCInteger::create(autoType));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif
