//
//  TestFeedBackController.cpp
//  IF
//
//  Created by chenfubi on 15/2/10.
//
//

#include "TestFeedBackController.h"
#include "TestFeedBackCommand.h"
#include "UIComponent.h"

static TestFeedBackController* s_sharedTestFeedBackController = NULL;

TestFeedBackController* TestFeedBackController::shared(void)
{
	if (!s_sharedTestFeedBackController)
	{
		s_sharedTestFeedBackController = new TestFeedBackController();
	}
	return s_sharedTestFeedBackController;
}

void TestFeedBackController::purge()
{
	CC_SAFE_RELEASE_NULL(s_sharedTestFeedBackController);
}

TestFeedBackController::TestFeedBackController()
:m_hasInit(false)
,m_version("")
,m_name("")
,m_serverId("")
,m_hasAtLeastOne(false)
{
    m_suggestArr = CCArray::create();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TestFeedBackController::dataChange), TESTFEEDBACK_DATACHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TestFeedBackController::parseTestData), TESTFEEDBACK_CMDBACK, NULL);
}

TestFeedBackController::~TestFeedBackController()
{
    m_suggestArr = NULL;
    m_hasInit = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TESTFEEDBACK_DATACHANGE);
}
void TestFeedBackController::initFeedBack(){
    TestFeedBackCommand* cmd = new TestFeedBackCommand();
    cmd->sendAndRelease();
}
void TestFeedBackController::test()
{
    m_hasInit = true;
    for (int i = 0; i < 11; i++)
    {
            CCDictionary* dict = CCDictionary::create();
        dict->setObject(CCString::createWithFormat("%d",i), "index");
        dict->setObject(ccs("全服更新，赶紧去更新全服更新，赶紧去更新全服更新，赶紧去更新全服更新，赶紧去更新全服更新，赶紧去更新全服更新，赶紧去更新"), "content");
        dict->setObject(ccs("这版很好"), "suggestion");
        dict->setObject(ccs("0"), "fixFlag");
        m_suggestArr->addObject(dict);
     
    }
}

void TestFeedBackController::parseTestData(cocos2d::CCObject *pObj)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TESTFEEDBACK_CMDBACK);
    CCDictionary* dataDict = _dict(pObj);
    m_version = dataDict->valueForKey("version")->getCString();
    m_name = dataDict->valueForKey("name")->getCString();
    m_serverId = dataDict->valueForKey("serverId")->getCString();
    m_endtime = dataDict->valueForKey("endTime")->doubleValue() / 1000;
    CCArray* arr = (CCArray*)dataDict->objectForKey("suggestion");
    CCObject* obj = NULL;
    CCDictionary* pelem = NULL;
    CCARRAY_FOREACH(arr, obj)
    {
        pelem = dynamic_cast<CCDictionary*>(obj);
        if (pelem)
        {
            CCDictionary* dict = CCDictionary::create();
            dict->setObject(pelem->objectForKey("index"), "index");
            dict->setObject(pelem->objectForKey("content"), "content");
            dict->setObject(pelem->objectForKey("suggestion"), "suggestion");
            if(pelem->objectForKey("suggestion") && pelem->valueForKey("suggestion")){
                if(m_hasAtLeastOne == false && pelem->valueForKey("suggestion")->getCString()){
                    std::string s = pelem->valueForKey("suggestion")->getCString();
                    if(!s.empty()){
                        m_hasAtLeastOne = true;
                    }
                }
            }
            dict->setObject(ccs("0"), "fixFlag");
            m_suggestArr->addObject(dict);
        }
    }
    m_hasInit = true;
    CCLOGFUNCF("parseTestData:%s,%s,%s",m_version.c_str(),m_name.c_str(),m_serverId.c_str());
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TESTFEEDBACK_OPEN);
}

void TestFeedBackController::dataChange(cocos2d::CCObject *pObj)
{
    CCDictionary* dict = _dict(pObj);
    int index = dict->valueForKey("index")->intValue();
    CCObject* obj = NULL;
    CCDictionary* pelem = NULL;
    CCARRAY_FOREACH(m_suggestArr, obj)
    {
        pelem = (CCDictionary*)obj;
        if (pelem->valueForKey("index")->intValue() == index)
        {
            pelem->setObject(dict->objectForKey("suggestion"), "suggestion");
            pelem->setObject(ccs("1"), "fixFlag");
            m_hasAtLeastOne = true;
            break;
        }
    }
}

void TestFeedBackController::sendCmd()
{
    CCObject* obj = NULL;
    CCDictionary* pelem = NULL;
    bool sendCmd = false;
    CCARRAY_FOREACH(m_suggestArr, obj)
    {
        pelem = (CCDictionary*)obj;
        if (pelem->valueForKey("fixFlag")->intValue() == 1)
        {
            pelem->setObject(ccs("0"), "fixFlag");
            TestFeedBackSaveCommand* cmd = new TestFeedBackSaveCommand(m_version,
                                                                       pelem->valueForKey("index")->intValue(),
                                                                       pelem->valueForKey("suggestion")->getCString(),
                                                                       m_name,
                                                                       atoi(m_serverId.c_str()));
            cmd->sendAndRelease();
            sendCmd = true;
        }
    }
    if (sendCmd)
    {
        TestFeedBackController::shared()->setHasAtLeastOne(true);
        UIComponent::getInstance()->setTestFBFlame();
    }
}
