//
//  AllianceScienceCommand.h
//  IF
//
//  Created by fubin on 14-4-9.
//
//

#ifndef IF_AllianceScienceCommand_h
#define IF_AllianceScienceCommand_h

#include "CommandBase.h"
#define ALL_SCIENCE_RESEARCH "al.science.research"
#define ALL_SCIENCE_DONATE "al.science.donate"
#define ALL_SCIENCE_DONATE_REFRESH "al.science.fresh"
#define ALL_SCIENCE_DONATE_CD "al.science.donate.cd"

//class ScienceResearchFinishCommand : public CommandBase {
//public:
//    ScienceResearchFinishCommand(string scienceId, string qUUid):
//    CommandBase(SCIENCE_RESEARCH_FINISH)
//    {
//        putParam("itemId", CCString::create(scienceId));
//        putParam("uuid", CCString::create(qUUid));
//    }
//    
//protected:
//    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
//private:
//    string m_scienceId;
//};

class AllScienceResearchCommand : public CommandBase {
public:
    AllScienceResearchCommand(int scienceId):
    CommandBase(ALL_SCIENCE_RESEARCH)
    {
        putParam("scienceId", CCInteger::create(scienceId));
        m_scienceId = scienceId;
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_scienceId;
};

class AllScienceDonateCommand : public CommandBase {
public:
    AllScienceDonateCommand(int scienceId, int type):
    CommandBase(ALL_SCIENCE_DONATE)
    {
        putParam("scienceId", CCInteger::create(scienceId));
        putParam("option", CCInteger::create(type));
        m_scienceId = scienceId;
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_scienceId;
};

class AllScienceDonateRefreshCommand : public CommandBase {
public:
    AllScienceDonateRefreshCommand(int scienceId):
    CommandBase(ALL_SCIENCE_DONATE_REFRESH)
    {
        putParam("scienceId", CCInteger::create(scienceId));
        m_scienceId = scienceId;
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_scienceId;
};

class AllScienceDonateCDCommand : public CommandBase {
public:
    AllScienceDonateCDCommand():
    CommandBase(ALL_SCIENCE_DONATE_CD)
    {
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif
