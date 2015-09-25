//
//  ScienceCommand.h
//  IF
//
//  Created by 邹 程 on 13-11-4.
//
//

#ifndef __IF__ScienceCommand__
#define __IF__ScienceCommand__

#include "CommandBase.h"
#define SCIENCE_RESEARCH_FINISH "science.upgrade"
#define SCIENCE_DIRECTLY "science.directly"
#define SCIENCE_RESEARCH "science.research"
#define SCIENCE_INSTANT_FINISH "science.cd.clear"
#define SCIENCE_DATA_INIT "science.data.init"

#define MSG_SCIENCE_DATA_REFRESH "msg.science.data.refresh"

class ScienceResearchFinishCommand : public CommandBase {
public:
    ScienceResearchFinishCommand(string scienceId, string qUUid):
    CommandBase(SCIENCE_RESEARCH_FINISH)
    {
        putParam("itemId", CCString::create(scienceId));
        putParam("uuid", CCString::create(qUUid));
//        int trueScienceId = atoi(scienceId.c_str());
//        trueScienceId -= trueScienceId%100;
//        m_scienceId = CC_ITOA(trueScienceId);
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_scienceId;
};
    
    
class ScienceDirectlyCommand : public CommandBase {
public:
    ScienceDirectlyCommand(string scienceId):
    CommandBase(SCIENCE_DIRECTLY)
    {
        putParam("itemId", CCString::create(scienceId));
//        int trueScienceId = atoi(scienceId.c_str());
//        trueScienceId -= trueScienceId%100;
//        m_scienceId = CC_ITOA(trueScienceId);
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_scienceId;
};

class ScienceInstantFinishCommand : public CommandBase {
public:
    ScienceInstantFinishCommand(string scienceId):
    CommandBase(SCIENCE_INSTANT_FINISH)
    {
        putParam("itemId", CCString::create(scienceId));
        int trueScienceId = atoi(scienceId.c_str());
        trueScienceId -= trueScienceId%100;
        m_scienceId = CC_ITOA(trueScienceId);
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_scienceId;
};
    
    

class ScienceResearchCommand : public CommandBase {
public:
    ScienceResearchCommand(string scienceId, int gold=0):
    CommandBase(SCIENCE_RESEARCH)
    {
        putParam("itemId", CCString::create(scienceId));
        putParam("gold", CCInteger::create(gold));
        int trueScienceId = atoi(scienceId.c_str());
        trueScienceId -= trueScienceId%100;
        m_scienceId = CC_ITOA(trueScienceId);
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_scienceId;
};

class ScienceDataInitCommand : public CommandBase {
public:
    ScienceDataInitCommand():
    CommandBase(SCIENCE_DATA_INIT)
    {
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_scienceId;
};

#endif /* defined(__IF__ScienceCommand__) */
