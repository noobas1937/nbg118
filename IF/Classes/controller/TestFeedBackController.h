//
//  TestFeedBackController.h
//  IF
//
//  Created by chenfubi on 15/2/10.
//
//

#ifndef __IF__TestFeedBackController__
#define __IF__TestFeedBackController__

#include "CommonInclude.h"

#define TESTFEEDBACK_CONTROL TestFeedBackController::shared()
#define TESTFEEDBACK_CMDBACK "TESTFEEDBACK_CMDBACK"
#define TESTFEEDBACK_OPEN "TESTFEEDBACK_OPEN"
#define TESTFEEDBACK_DATACHANGE "TESTFEEDBACK_DATACHANGE"

class TestFeedBackController : public CCObject
{
public:
	static TestFeedBackController * shared();
	void purge();
    
    CCArray* getSuggestArr(){return m_suggestArr;}
    void sendCmd();
    
    void test();
    void initFeedBack();
private:
	TestFeedBackController();
	virtual ~TestFeedBackController();
	TestFeedBackController(const TestFeedBackController &);
    TestFeedBackController &operator=(const TestFeedBackController &);
    
    void parseTestData(CCObject* pObj);
    void dataChange(CCObject* pObj);
    
public:
    CC_SYNTHESIZE_READONLY(string, m_version, Version);
    CC_SYNTHESIZE(string, m_name, Name);
    CC_SYNTHESIZE(string, m_serverId, ServerId);
    CC_SYNTHESIZE(bool, m_hasInit, HasInit);
    CC_SYNTHESIZE(bool, m_hasAtLeastOne, HasAtLeastOne);
    CC_SYNTHESIZE_READONLY(double, m_endtime, EndTime);
private:
    CCSafeObject<CCArray> m_suggestArr;
    
};

#endif /* defined(__IF__TestFeedBackController__) */
