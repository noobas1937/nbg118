//
//  AllRankCommand.h
//  IF
//
//  Created by lifangkai on 14-9-16.
//
//

#ifndef __IF__AllRankCommand__
#define __IF__AllRankCommand__

#include "CommandBase.h"

#define ALLIANCE_RANK "alliance.rank"
#define ALLIANCE_HELP_RANK "reinforce.record.get"
#define ALLIANCE_KILL_RANK "kill.al.rank"
#define PLAYER_RANK  "server.rank"
#define PLAYER_KILL_RANK  "kill.rank"
#define PLAYER_BUILDING_RANK "building.rank"
#define PLAYER_LEVEL_RANK "level.rank"

class AllianceRankListCommand: public CommandBase {
public:
    AllianceRankListCommand() : CommandBase(ALLIANCE_RANK){
        // putParam("uuid", CCString::create(m_uid.c_str()));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
};

class AllianceKillRankListCommand: public CommandBase {
public:
    AllianceKillRankListCommand() : CommandBase(ALLIANCE_KILL_RANK){
        // putParam("uuid", CCString::create(m_uid.c_str()));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
};

class PlayerRankListCommand: public CommandBase {
public:
    PlayerRankListCommand() : CommandBase(PLAYER_RANK){
        // putParam("uuid", CCString::create(m_uid.c_str()));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
};
class PlayerKillRankListCommand: public CommandBase {
public:
    PlayerKillRankListCommand() : CommandBase(PLAYER_KILL_RANK){
        // putParam("uuid", CCString::create(m_uid.c_str()));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
};
class PlayerBuildingRankListCommand: public CommandBase {
public:
    PlayerBuildingRankListCommand() : CommandBase(PLAYER_BUILDING_RANK){
        // putParam("uuid", CCString::create(m_uid.c_str()));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
};
class PlayerLevelRankListCommand: public CommandBase {
public:
    PlayerLevelRankListCommand() : CommandBase(PLAYER_LEVEL_RANK){
        // putParam("uuid", CCString::create(m_uid.c_str()));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
};
class HelpReportCmd: public CommandBase {
public:
    HelpReportCmd() : CommandBase(ALLIANCE_HELP_RANK){
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__AllRankCommand__) */
