//
//  PortActView.cpp
//  IF
//
//  Created by xxrdsg on 14-12-22.
//
//

#include "PortActView.h"
#include "ActivityController.h"
#include "FunBuildController.h"
PortActView::~PortActView(){
    if(m_panelType==1){
        ActivityController::getInstance()->enterGameOpenView(m_panelType);
    }
}
PortActView* PortActView::create(int type,int panelType)
{
    auto ret = new PortActView(type,panelType);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool PortActView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    auto tmpCCB = CCBLoadFile("PortActView", this, this);
    this->setContentSize(tmpCCB->getContentSize());
    
    int addH = CCDirector::sharedDirector()->getWinSize().height - 852;
    if (CCCommonUtils::isIosAndroidPad())
    {
        addH = CCDirector::sharedDirector()->getWinSize().height - 2048;
    }
    m_bgContainer->setPositionY(m_bgContainer->getPositionY() - addH);
//    addBG();//fusheng 去掉背景
    if (PortActController::getInstance()->m_isNewTimeRwd) {
        setTitleName(_lang("105081"));
    } else {
        setTitleName(_lang("105079"));
    }
    CCCommonUtils::setButtonTitle(m_timeRwdBtn, _lang("105080").c_str());
    CCCommonUtils::setButtonTitle(m_dailyRwdBtn, _lang("105081").c_str());
    m_waitInterface = NULL;
    
    if (PortActController::getInstance()->m_isNewTimeRwd) {
        m_mainNode->setVisible(false);
        m_dailyRwdBtn->setEnabled(false);
        m_timeRwdBtn->setEnabled(false);
    }
    return true;
}

void PortActView::addBG()
{
    std::string picName = "technology_09.png";
    auto totalH = CCDirector::sharedDirector()->getWinSize().height;
    int h = 0;
    auto batch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource(picName.c_str())->getTexture());
    m_bgContainer->addChild(batch);
    while (h < totalH) {
        auto sprite = CCLoadSprite::createSprite(picName.c_str());
        sprite->setAnchorPoint(ccp(0, 0));
        if (CCCommonUtils::isIosAndroidPad()) {
//            sprite->setScaleX(2.4);
            sprite->setAnchorPoint(ccp(0, 0.5));
        }
        batch->addChild(sprite);
        sprite->setPosition(ccp(0, h));
        h += sprite->getContentSize().height;
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        batch->setScaleX(1536 * 1.0 / 640);
    }
}

void PortActView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PortActView::retData), PORT_DATA_INIT, NULL);
    if (PortActController::getInstance()->m_isNewTimeRwd) {
        if (!m_dailyRwdView) {
            requestData();
        }
        setTitleName(_lang("105081"));
    } else {
        if(!m_TimeExchangeView || !m_dailyRwdView) {
            requestData();
        }
        setTitleName(_lang("105079"));
    }
}

void PortActView::onExit()
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PORT_DATA_INIT);
    CCNode::onExit();
}

void PortActView::requestData()
{
    if (!PortActController::getInstance()->m_isNewTimeRwd) {
        bool st = false;
        
        if (PortActController::getInstance()->m_timeExMap.size() <= 0 || PortActController::getInstance()->m_loginDayMap.size() <= 0) {//没有在线时长或每日签到数据
            PortActController::getInstance()->startGetExchangeData(0);
            PortActController::getInstance()->startGetDailyRwdData();
            st = true;
        }
        
        if (st) {
            m_waitInterface = GameController::getInstance()->showWaitInterface(this);
            auto size = CCDirector::sharedDirector()->getWinSize();
            m_waitInterface->setPosition(ccp(size.width / 2, -size.height / 2));
        } else {
            refreshData();
        }
    } else {
        bool st = false;
        if (PortActController::getInstance()->m_loginDayMap.size() <= 0) {
            PortActController::getInstance()->startGetDailyRwdData();
            st = true;
        }
        if (st) {
            m_waitInterface = GameController::getInstance()->showWaitInterface(this);
            auto size = CCDirector::sharedDirector()->getWinSize();
            m_waitInterface->setPosition(ccp(size.width / 2, -size.height / 2));
        } else {
            refreshData();
        }
    }
    
}

void PortActView::retData(CCObject* param)
{
    int dataType = dynamic_cast<CCInteger*>(param)->getValue();
    if (dataType == 1 && (!PortActController::getInstance()->m_isNewTimeRwd)) {
        if (!m_TimeExchangeView) {
            m_TimeExchangeView = TimeExchangeView::create();
            m_TimeExchangeView->setPositionY(-682);
            m_TimeExchangeView->setVisible(false);
            m_actNode->addChild(m_TimeExchangeView);
        }
    }
    if (dataType == 2) {
        if (!m_dailyRwdView) {
            m_dailyRwdView = DailyRwdView::create();
            m_dailyRwdView->setPositionY(-682);
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_dailyRwdView->setPositionY(-1713);
            }
            
            m_dailyRwdView->setVisible(false);
            m_dailyRwdView->setListNodeVisible(false);
            m_actNode->addChild(m_dailyRwdView);
        }
    }
    
    if (PortActController::getInstance()->m_isNewTimeRwd) {
        if (m_dailyRwdView) {
            if (m_waitInterface != NULL) {
                m_waitInterface->remove();
                m_waitInterface = NULL;
            }
            onClickDailyRwdBtn(NULL, Control::EventType::TOUCH_DOWN);
        }
    } else {
        if (m_TimeExchangeView && m_dailyRwdView) {
            if (m_waitInterface != NULL) {
                m_waitInterface->remove();
                m_waitInterface = NULL;
            }
            if(m_type!=0){
                onClickDailyRwdBtn(NULL, Control::EventType::TOUCH_DOWN);
                m_type = 0;
            }else if (PortActController::getInstance()->m_isNewTimeRwd){
                onClickDailyRwdBtn(NULL, Control::EventType::TOUCH_DOWN);
            } else {
                onClickTimeRwdBtn(NULL, Control::EventType::TOUCH_DOWN);
            }
        }
    }
}

void PortActView::refreshData()
{
    if (PortActController::getInstance()->m_isNewTimeRwd) {
        if (PortActController::getInstance()->m_isNewDay) {
            PortActController::getInstance()->startGetDailyRwdData();
            m_waitInterface = GameController::getInstance()->showWaitInterface(this);
            auto size = CCDirector::sharedDirector()->getWinSize();
            m_waitInterface->setPosition(ccp(size.width / 2, -size.height / 2));
        } else {
            if (!m_dailyRwdView) {
                m_dailyRwdView = DailyRwdView::create();
                m_dailyRwdView->setPositionY(-682);
                if (CCCommonUtils::isIosAndroidPad())
                {
                    m_dailyRwdView->setPositionY(-1713);
                }
                
                m_dailyRwdView->setVisible(false);
                m_dailyRwdView->setListNodeVisible(false);
                m_actNode->addChild(m_dailyRwdView);
            }
            onClickDailyRwdBtn(NULL, Control::EventType::TOUCH_DOWN);
        }
    } else {
        bool st = false;
        
        if (PortActController::getInstance()->isNewDay() || PortActController::getInstance()->m_castleLevel != FunBuildController::getInstance()->getMainCityLv()) {
            PortActController::getInstance()->startGetExchangeData(0);
            st = true;
        } else {
            if (!m_TimeExchangeView) {
                m_TimeExchangeView = TimeExchangeView::create();
                m_TimeExchangeView->setPositionY(-682);
                m_TimeExchangeView->setVisible(false);
                m_actNode->addChild(m_TimeExchangeView);
            }
        }
        
        if (PortActController::getInstance()->m_isNewDay) {
            PortActController::getInstance()->startGetDailyRwdData();
            st = true;
        } else {
            if (!m_dailyRwdView) {
                m_dailyRwdView = DailyRwdView::create();
                m_dailyRwdView->setPositionY(-682);
                if (CCCommonUtils::isIosAndroidPad())
                {
                    m_dailyRwdView->setPositionY(-1713);
                }
                m_dailyRwdView->setVisible(false);
                m_dailyRwdView->setListNodeVisible(false);
                m_actNode->addChild(m_dailyRwdView);
            }
        }
        
        if (st) {
            m_waitInterface = GameController::getInstance()->showWaitInterface(this);
            auto size = CCDirector::sharedDirector()->getWinSize();
            m_waitInterface->setPosition(ccp(size.width / 2, -size.height / 2));
        } else {
            if (PortActController::getInstance()->m_isNewTimeRwd) {
                onClickDailyRwdBtn(NULL, Control::EventType::TOUCH_DOWN);
            } else {
                onClickTimeRwdBtn(NULL, Control::EventType::TOUCH_DOWN);
            }
            
        }
    }
}

void PortActView::onClickTimeRwdBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (PortActController::getInstance()->m_isNewTimeRwd) {
        m_timeRwdBtn->setEnabled(false);
        return;
    } else {
        if (!m_TimeExchangeView || !m_dailyRwdView) {
            return;
        }
        
        m_timeRwdBtn->setEnabled(false);
        m_dailyRwdBtn->setEnabled(true);
        
        if (m_TimeExchangeView){
            m_TimeExchangeView->setVisible(true);
        }
        if (m_dailyRwdView) {
            m_dailyRwdView->setVisible(false);
            m_dailyRwdView->setListNodeVisible(false);
        }
    }
}

void PortActView::onClickDailyRwdBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (PortActController::getInstance()->m_isNewTimeRwd) {
        if(GlobalData::shared()->facebook_gift_k7!=1){
            CCCommonUtils::flyText(_lang("102192"));
            return;
        }
        if (!m_dailyRwdView) {
            return;
        }
        m_timeRwdBtn->setEnabled(false);
        m_dailyRwdBtn->setEnabled(false);
        if (m_dailyRwdView) {
            m_dailyRwdView->setVisible(true);
            m_dailyRwdView->setListNodeVisible(true);
            m_dailyRwdView->resetTabViewPos();
            if (PortActController::getInstance()->isLastDayFirstIn()) {
                CCCommonUtils::flyHint("", "", _lang("111056"));
            }
            PortActController::getInstance()->m_inCnt++;
        }
    } else {
        if(GlobalData::shared()->facebook_gift_k7!=1){
            CCCommonUtils::flyText(_lang("102192"));
            return;
        }
        if (!m_TimeExchangeView || !m_dailyRwdView) {
            return;
        }
        
        m_timeRwdBtn->setEnabled(true);
        m_dailyRwdBtn->setEnabled(false);
        
        if (m_TimeExchangeView) {
            m_TimeExchangeView->setVisible(false);
        }
        if (m_dailyRwdView) {
            m_dailyRwdView->setVisible(true);
            m_dailyRwdView->setListNodeVisible(true);
            m_dailyRwdView->resetTabViewPos();
            if (PortActController::getInstance()->isLastDayFirstIn()) {
                CCCommonUtils::flyHint("", "", _lang("111056"));
            }
            PortActController::getInstance()->m_inCnt++;
        }
    }
}

SEL_CCControlHandler PortActView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTimeRwdBtn", PortActView::onClickTimeRwdBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickDailyRwdBtn", PortActView::onClickDailyRwdBtn);
    return NULL;
}

bool PortActView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeRwdBtn", CCControlButton*, this->m_timeRwdBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dailyRwdBtn", CCControlButton*, this->m_dailyRwdBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_actNode", CCNode*, this->m_actNode);
    return false;
}