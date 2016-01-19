//
//  ActivityEventCommand.h
//  IF
//
//  Created by ganxiaohua on 14-10-24.
//
//

#ifndef __IF__ActivityEventCommand__
#define __IF__ActivityEventCommand__

#include "CommandBase.h"
#define ACTIVITY_EVENT_COMMAND "score.login"
#define ACTIVITY_SINGLESCORE_COMMAND "hero.event.info.get"
#define ACTIVITY_SINGLESCORE_RANK_COMMAND "hero.event.ranking.get"
#define ACTIVITY_SINGLES_RANK_HISTORY_COMMAND "hero.event.ranking.his.get"
#define ACTIVITY_AL_RANK_CON_COMMAND "hero.al.inside.ranking"
#define ACTIVITY_RANK_SCORE_COMMAND "hero.self.rank"

class ActivityEventCommand: public CommandBase {
public:
    ActivityEventCommand() : CommandBase(ACTIVITY_EVENT_COMMAND){

    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
class ActivitySingleScoreGetCommand: public CommandBase {
public:
    ActivitySingleScoreGetCommand(string activityId) : CommandBase(ACTIVITY_SINGLESCORE_COMMAND){
        putParam("activityId", CCString::create(activityId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

class GetSingleScoreRankCommand: public CommandBase {
public:
    GetSingleScoreRankCommand(string activityId, int type) : CommandBase(ACTIVITY_SINGLESCORE_RANK_COMMAND){
        putParam("actId", CCString::create(activityId));
        m_type = type;
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    int m_type;
};

class GetSingleHistoryRankCommand: public CommandBase {
public:
    GetSingleHistoryRankCommand(int type) : CommandBase(ACTIVITY_SINGLES_RANK_HISTORY_COMMAND){
        putParam("type", CCInteger::create(type));
        m_type = type;
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    int m_type;
};

class GetAllianceConRankCommand: public CommandBase {
public:
    GetAllianceConRankCommand(string actid) : CommandBase(ACTIVITY_AL_RANK_CON_COMMAND){
        putParam("actId", CCString::create(actid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

class GetScoreRankCommand: public CommandBase {
public:
    GetScoreRankCommand(string actid) : CommandBase(ACTIVITY_RANK_SCORE_COMMAND){
        putParam("activityId", CCString::create(actid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__ActivityEventCommand__) */
