//
//  TimeExchangeView.cpp
//  IF
//
//  Created by xxrdsg on 14-12-22.
//
//

#include "TimeExchangeView.h"
#include "ActivityController.h"
#include "FunBuildController.h"
#include "ToolController.h"
#include "RewardController.h"
#include "PopupViewController.h"
#include "TipsView.h"
#include "ParticleController.h"

TimeExchangeView* TimeExchangeView::create()
{
    TimeExchangeView* ret = new TimeExchangeView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TimeExchangeView::init()
{
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    
    auto temCCB = CCBLoadFile("TimeExchangeView", this, this);
    setContentSize(temCCB->getContentSize());
    auto size = CCDirector::sharedDirector()->getWinSize();
    float dh = size.height - 852 - 8;
    m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + dh));
    
    m_data = CCArray::create();
    m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
    m_tabView->setTouchPriority(Touch_Popup);
    m_infoList->addChild(m_tabView);
    
    string msg = _lang("105082");
    m_infoLabel->setString(msg);
    m_st = true;
    m_maxRwdLv = PortActController::getInstance()->getMaxRwdLv();
    
    generateData(NULL);
    onEnterFrame(0);
    return true;
}

void TimeExchangeView::onEnter()
{
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(TimeExchangeView::onEnterFrame), this, 1.0, kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TimeExchangeView::generateData), PORT_TIME_EXCHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TimeExchangeView::requestData), MSG_FUNBUILD_OPEN, NULL);
    resetTabviewPos();
}

void TimeExchangeView::onExit()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(TimeExchangeView::onEnterFrame), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PORT_TIME_EXCHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FUNBUILD_OPEN);
    CCNode::onExit();
}

void TimeExchangeView::resetTabviewPos()
{
    int level = PortActController::getInstance()->getLvByTime(PortActController::getInstance()->m_lastRwdTime);
    if (level * gridSizeForTable(NULL).height < m_tabView->getContentSize().height - m_infoList->getContentSize().height) {
        m_tabView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_tabView->getContentSize().height + level * gridSizeForTable(NULL).height));
    } else {
        m_tabView->setContentOffset(ccp(0, 0));
    }
}

void TimeExchangeView::requestData(CCObject* obj)
{
    m_st = false;
    PortActController::getInstance()->startGetExchangeData(1);
}

void TimeExchangeView::onEnterFrame(float dt)
{
    if (!m_st) {
        return;
    }
    
    int tmpMaxRwdLv = PortActController::getInstance()->getMaxRwdLv();
    if (tmpMaxRwdLv != m_maxRwdLv) {
        m_maxRwdLv = tmpMaxRwdLv;
        generateData(NULL);
    }
    
    if (PortActController::getInstance()->isNewDay()) {
        requestData(NULL);
    }
}
void TimeExchangeView::generateData(CCObject* p)
{
    m_st = true;
    int cnt = PortActController::getInstance()->m_timeExMap.size();
    m_data->removeAllObjects();
    for (int i = 1; i <= cnt; ++i) {
        m_data->addObject(CCInteger::create(i));
    }
    m_tabView->reloadData();
}

void TimeExchangeView::onClickTipBtn(CCObject* pSender, Control::EventType pCCControlEvent)
{
    string tips = _lang("105083") + "\n" + _lang("105084") + "\n" + _lang("105085");
    PopupViewController::getInstance()->addPopupView(TipsView::create(tips, kCCTextAlignmentLeft));
}

void TimeExchangeView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
    TimeExchangeCell* exchangeCell = dynamic_cast<TimeExchangeCell*>(cell);
    if (exchangeCell) {
        exchangeCell->cellTouchEnded(m_tabView->m_pCurTouch);
    }
}

cocos2d::CCSize TimeExchangeView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(600, 190);
}

CCTableViewCell* TimeExchangeView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    int itemId = dynamic_cast<CCInteger*>(m_data->objectAtIndex(idx))->getValue();
    
    TimeExchangeCell* cell = (TimeExchangeCell*)table->dequeueGrid();
    if(cell){
        cell->setData(itemId);
    }else{
        cell = TimeExchangeCell::create(itemId);
    }
    return cell;
}

ssize_t TimeExchangeView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return m_data->count();
}

unsigned int TimeExchangeView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}

SEL_CCControlHandler TimeExchangeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTipBtn", TimeExchangeView::onClickTipBtn);
    return NULL;
}

bool TimeExchangeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, this->m_infoLabel);
    return false;
}

//class cell
TimeExchangeCell* TimeExchangeCell::create(int itemId)
{
    auto ret = new TimeExchangeCell();
    if (ret && ret->init(itemId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TimeExchangeCell::init(int itemId)
{
    CCBLoadFile("TimeExchangeCell", this, this);
    setContentSize(CCSize(600, 190));
    m_waitInterface = NULL;
    setData(itemId);
    return true;
}

void TimeExchangeCell::setData(int itemId)
{
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    m_itemId = itemId;
    m_picNode->removeAllChildren();
    m_picNode1->removeAllChildren();
    m_picNode2->removeAllChildren();
    m_numLabel1->setString("");
    m_numLabel2->setString("");
    m_numBG1->setVisible(false);
    m_numBG2->setVisible(false);
    m_picBG1->setVisible(false);
    m_picBG2->setVisible(false);
    m_rdMap.clear();
    CCCommonUtils::setButtonTitle(m_rwdBtn, _lang("105086").c_str());
    m_nameLabel->setString(_lang_1("105087", CC_ITOA(m_itemId)));
    m_nameLabel->setColor(ccc3(255, 233, 190));
    m_nameLabelBg->setPreferredSize(CCSize(m_nameLabel->getContentSize().width * m_nameLabel->getOriginScaleX() + 20, m_nameLabelBg->getContentSize().height));
    
    //上次已领奖的等级
    int level = PortActController::getInstance()->getLvByTime(PortActController::getInstance()->m_lastRwdTime);
    if ((level + 1) < m_itemId) {//未兑换且不可兑换
        m_picNode->removeAllChildren();
        auto spr = CCLoadSprite::createSprite("Items_baoxiang2.png");
        CCCommonUtils::setSpriteMaxSize(spr, 120,true);
        int targetSecs = PortActController::getInstance()->m_levelTime[m_itemId - 1] * 60;
        int onlineSecs = PortActController::getInstance()->m_timeOnline + GlobalData::shared()->getTimeStamp() - PortActController::getInstance()->m_startTime;
        if (targetSecs <= onlineSecs) {
            auto particle1 = ParticleController::createParticle("QueueGlow_1");
            auto particle2 = ParticleController::createParticle("QueueGlow_2");
            m_picNode->addChild(particle1);
            m_picNode->addChild(particle2);
        }
        m_picNode->addChild(spr);
        m_picNode->setVisible(true);
        m_rwdBtn->setVisible(false);
    } else if ((level + 1) == m_itemId) {//未兑换且即将兑换
        m_picNode->removeAllChildren();
        auto spr = CCLoadSprite::createSprite("Items_baoxiang2.png");
        CCCommonUtils::setSpriteMaxSize(spr, 120,true);
        auto particle1 = ParticleController::createParticle("QueueGlow_1");
        auto particle2 = ParticleController::createParticle("QueueGlow_2");
        m_picNode->addChild(particle1);
        m_picNode->addChild(particle2);
        m_picNode->addChild(spr);
        m_picNode->setVisible(true);
        m_rwdBtn->setVisible(true);
        if (PortActController::getInstance()->getMaxRwdLv() < m_itemId) {
            m_rwdBtn->setEnabled(false);
        } else {
            m_rwdBtn->setEnabled(true);
        }
    } else {//已经兑换
        m_picNode->removeAllChildren();
        auto spr = CCLoadSprite::createSprite("Items_baoxiang1.png");
        CCCommonUtils::setSpriteMaxSize(spr, 120,true);
        m_picNode->addChild(spr);
        m_picNode->setVisible(true);
        m_rwdBtn->setVisible(false);
        string str = "--:--:--";
        m_timeLabel->setString(str);
    }
    
    if (m_itemId > level) {
        this->unschedule(schedule_selector(TimeExchangeCell::onTimer));
        onTimer(0.0f);
        this->schedule(schedule_selector(TimeExchangeCell::onTimer), 1.0f);
    }
    
    auto& info = PortActController::getInstance()->m_timeExMap[m_itemId];
    for (int i = 0; i < info.rewardArr->count(); ++i) {
        
        auto dict = _dict(info.rewardArr->objectAtIndex(i));
        int type = dict->valueForKey("type")->intValue();
        int num = 0;
        
        string picStr = "";
        string bg = "";
        bool dPath = false;
        if (type == R_GOODS) {
            auto value = _dict(dict->objectForKey("value"));
            string rid = value->valueForKey("id")->getCString();
            num = value->valueForKey("num")->intValue();
            
            picStr = CCCommonUtils::getIcon(rid);
            auto& tool = ToolController::getInstance()->getToolInfoById(atoi(rid.c_str()));
            bg = CCCommonUtils::getToolBgByColor(tool.color);
            dPath = true;
            m_rdMap[i] = rid;
        } else {
            num = dict->valueForKey("value")->intValue();
            picStr = RewardController::getInstance()->getPicByType(type, 0);
            bg = CCCommonUtils::getToolBgByColor(PURPLE);
        }
        auto icon = dPath? CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_GOODS):CCLoadSprite::createSprite(picStr.c_str());
        auto bgIcon = CCLoadSprite::createSprite(bg.c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 100,true);
        CCCommonUtils::setSpriteMaxSize(bgIcon, 107,true);
        if (i == 0) {
            m_picNode1->removeAllChildren();
            if (level + 2 < m_itemId) {
                auto spr = CCLoadSprite::createSprite("no_iconFlag.png");
                CCCommonUtils::setSpriteMaxSize(spr, 107,true);
                m_picNode1->addChild(spr);
            } else {
                if (bg != "") {
                    m_picNode1->addChild(bgIcon);
                }
                m_picNode1->addChild(icon);
                m_numLabel1->setString(CC_CMDITOA(num));
                m_numBG1->setVisible(true);
            }
            m_picBG1->setVisible(true);
        } else if (i == 1) {
            m_picNode2->removeAllChildren();
            if (level + 2 < m_itemId) {
                auto spr = CCLoadSprite::createSprite("no_iconFlag.png");
                CCCommonUtils::setSpriteMaxSize(spr, 107,true);
                m_picNode2->addChild(spr);
            } else {
                if (bg != "") {
                    m_picNode2->addChild(bgIcon);
                }
                m_picNode2->addChild(icon);
                m_numLabel2->setString(CC_CMDITOA(num));
                m_numBG2->setVisible(true);
            }
            m_picBG2->setVisible(true);
        } else {
            break;
        }
    }
    
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
}

void TimeExchangeCell::onTimer(float dt)
{
    int targetSecs = PortActController::getInstance()->m_levelTime[m_itemId - 1] * 60;
    int onlineSecs = PortActController::getInstance()->m_timeOnline + GlobalData::shared()->getTimeStamp() - PortActController::getInstance()->m_startTime;
    if (targetSecs <= onlineSecs) {
        this->m_timeLabel->setString(CC_SECTOA(0));
        this->unschedule(schedule_selector(TimeExchangeCell::onTimer));
    } else {
        this->m_timeLabel->setString(CC_SECTOA(targetSecs - onlineSecs));
    }
}

void TimeExchangeCell::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TimeExchangeCell::hideDesNode), MSG_HIDE_RWDDES_NODE, NULL);
}

void TimeExchangeCell::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_HIDE_RWDDES_NODE);
    CCNode::onExit();
}

void TimeExchangeCell::cellTouchEnded(CCTouch* pTouch)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_HIDE_RWDDES_NODE);
    int level = PortActController::getInstance()->getLvByTime(PortActController::getInstance()->m_lastRwdTime);
    if ((level + 2) < m_itemId) {
        return;
    }
    if (isTouchInside(m_touchNode1, pTouch)) {
        setDesNode(0);
    } else if (isTouchInside(m_touchNode2, pTouch)) {
        setDesNode(1);
    }
}

void TimeExchangeCell::hideDesNode(CCObject* param)
{
    m_desNode->setVisible(false);
}

void TimeExchangeCell::setDesNode(int pos)
{
    if (m_rdMap.find(pos) != m_rdMap.end()) {
        if (pos == 0) {
            m_desNode->setPositionX(160);
        } else if(pos == 1) {
            m_desNode->setPositionX(300);
        } else {
            return;
        }
        int toolID = atoi(m_rdMap[pos].c_str());
        if (toolID) {
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(toolID);
            m_desName->setString(info.getName().c_str());
            m_desLabel->setString(_lang(info.des).c_str());
            m_desNode->setVisible(true);
        }
    }
}

void TimeExchangeCell::onClickRwdBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    auto& info = PortActController::getInstance()->m_timeExMap[m_itemId];
    if (m_itemId > PortActController::getInstance()->getMaxRwdLv()) {
        return;
    } else {
        PortActController::getInstance()->startExchange(info.eId);
    }
}

void TimeExchangeCell::refreashData(CCObject* obj)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
}

void  TimeExchangeCell::setSprite()
{

}

bool TimeExchangeCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel1", CCLabelIF*, m_numLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel2", CCLabelIF*, m_numLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG1", CCSprite*, m_picBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG2", CCSprite*, m_picBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numBG1", CCSprite*, m_numBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numBG2", CCSprite*, m_numBG2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, m_picNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", CCNode*, m_picNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode1", CCNode*, m_touchNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode2", CCNode*, m_touchNode2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabelBg", CCScale9Sprite*, m_nameLabelBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdBtn", CCControlButton*, m_rwdBtn);
    return false;
}

SEL_CCControlHandler TimeExchangeCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRwdBtn", TimeExchangeCell::onClickRwdBtn);
    return NULL;
}