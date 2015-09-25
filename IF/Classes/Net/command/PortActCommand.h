//
//  PortActCommand.h
//  IF
//
//  Created by xxrdsg on 14-12-22.
//
//

#ifndef __IF__PortActCommand__
#define __IF__PortActCommand__

#include "CommandBase.h"
#define TIME_EX_INIT_COMMAND "online.info"
#define TIME_EX_COMMAND "online.reward"
#define DAILY_REWARD_INIT_COMMAND "signIn.info"
#define DAILY_REWARD_CHECKIN_COMMAND "signIn.reward"

//新的在线时长兑换命令
#define TIME_RWD_COMMAND "cargo.reward"
//新手7日登陆领奖
#define NEW_PLAYER_RWD_COMMAND "fresh.signIn"

class TimeRwdCommand : public CommandBase {
public:
    TimeRwdCommand(): CommandBase(TIME_RWD_COMMAND){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class TimeExchangeInitCommand: public CommandBase {
public:
    TimeExchangeInitCommand(int type = 0): CommandBase(TIME_EX_INIT_COMMAND){
        m_type = type;
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int m_type;
};

class TimeExchangeCommand: public CommandBase {
public:
    TimeExchangeCommand(int min, int secs): CommandBase(TIME_EX_COMMAND){
        putParam("online", CCInteger::create(min));
        putParam("time", CCInteger::create(secs));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class DailyRewardInitCommand: public CommandBase {
public:
    DailyRewardInitCommand(): CommandBase(DAILY_REWARD_INIT_COMMAND){
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class DailyRewardCheckInCommand: public CommandBase {
public:
    DailyRewardCheckInCommand(bool isVip,int num,int newNum,int oldNum): CommandBase(DAILY_REWARD_CHECKIN_COMMAND){
        putParam("vip", CCInteger::create(isVip));
        putParam("num", CCInteger::create(num));
        putParam("newNum", CCInteger::create(newNum));
        putParam("oldNum", CCInteger::create(oldNum));
        m_isVip = isVip;
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    bool m_isVip;
};

class NewPlayerRewardCommand: public CommandBase {
public:
    NewPlayerRewardCommand(): CommandBase(NEW_PLAYER_RWD_COMMAND){
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
#endif /* defined(__IF__PortActCommand__) */
