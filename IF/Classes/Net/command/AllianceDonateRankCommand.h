//
//  AllianceDonateRankCommand.h
//  IF
//
//  Created by lifangkai on 14-9-3.
//
//

#ifndef __IF__AllianceDonateRankCommand__
#define __IF__AllianceDonateRankCommand__

#include "CommandBase.h"

#define ALLIANCE_DONATE_RANK_ALL "al.donate.rank.all"
#define ALLIANCE_DONATE_RANK_TODAY  "al.donate.rank.today"
#define ALLIANCE_DONATE_RANK_WEEK "al.donate.rank.week"

class AllianceDonateAllCommand: public CommandBase {
public:
    AllianceDonateAllCommand() : CommandBase(ALLIANCE_DONATE_RANK_ALL){
        // putParam("uuid", CCString::create(m_uid.c_str()));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
};

class AllianceDonateTodayCommand: public CommandBase {
public:
    AllianceDonateTodayCommand() : CommandBase(ALLIANCE_DONATE_RANK_TODAY){
        // putParam("uuid", CCString::create(m_uid.c_str()));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
};

class AllianceDonateWeekCommand: public CommandBase {
public:
    AllianceDonateWeekCommand(): CommandBase(ALLIANCE_DONATE_RANK_WEEK) {
        
    };
protected:
    virtual bool handleRecieve(CCDictionary* dict);
};

#endif /* defined(__IF__AllianceDonateRankView__) */
