//
//  BuildingAddOutPutCommand.h
//  IF
//
//  Created by ganxiaohua on 14-7-29.
//
//

#ifndef __IF__BuildingAddOutPutCommand__
#define __IF__BuildingAddOutPutCommand__

#include "CommandBase.h"

#define BUILDING_ADD_OUTPUT "building.addOutPut"

class BuildingAddOutPutCommand: public CommandBase {
public:
    BuildingAddOutPutCommand(std::string uid,int buildingKey, std::string toolUUID="") : CommandBase(BUILDING_ADD_OUTPUT),m_uid(uid),m_buildingKey(buildingKey){
        putParam("uuid", CCString::create(m_uid.c_str()));
        if(toolUUID!="") {
            putParam("goods", CCString::create(toolUUID.c_str()));
        }
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
    int m_buildingKey;
};
#endif /* defined(__IF__BuildingAddOutPutCommand__) */
