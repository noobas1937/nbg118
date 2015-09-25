//
//  ActivityListView.cpp
//  IF
//
//  Created by chenfubi on 15/1/6.
//
//

#include "ActivityListView.h"
#include "ActivityController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "ActivityTipView.h"
#include "ActivityBeginView.h"
#include "GoldExchangeItemView.h"
#include "GoldExchangeInfoCommand.h"
#include "TipsView.h"
#include "ToolController.h"
#include "GetActivityDetailCommand.h"
#include "ActivityListAdView.h"
#include "SoundController.h"
#include "AllianceActivityView.h"
#include "AllianceActBossView.h"
#include "ActivityDetailView.h"
#include "PayController.h"
#include "LuaController.h"
#include "ActivityView.h"
#include "CrossServiceActivityView.h"

#define ACTIVITY_CELL_REFRESH "activity_cell_refresh"

ActivityListView* ActivityListView::create()
{
    ActivityListView* ret = new ActivityListView();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityListView::init()
{
	bool bRet = false;
	do
	{
        if (!PopupBaseView::init())
        {
            break;
        }
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
        CCLoadSprite::doResourceByCommonIndex(502, true);
        CCLoadSprite::doResourceByCommonIndex(506, true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(500, false);
            CCLoadSprite::doResourceByCommonIndex(502, false);
            CCLoadSprite::doResourceByCommonIndex(506, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
        });
        
        auto tmpCCB = CCBLoadFile("ActivityListView", this, this);
        this->setContentSize(tmpCCB->getContentSize());
        
        int prev = m_viewBg->getContentSize().height;
        this->changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height - prev;
        if (CCCommonUtils::isIosAndroidPad()) {
            add = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height + add));
        m_infoList->setPositionY(m_infoList->getPositionY() - add);
        
        if (CCCommonUtils::isIosAndroidPad()) {
            float cury =  -1024 - add / 2.0;
            auto tbg = CCLoadSprite::loadResource("technology_09.png");
            auto tbatch = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
            while (cury < 1024 + add / 2.0) {
                auto pic = CCLoadSprite::createSprite("technology_09.png");
                pic->setScaleX(1536.0 / 640.0);
                pic->setAnchorPoint(ccp(0.5, 0));
                pic->setPosition(ccp(0, cury));
                tbatch->addChild(pic);
                cury += pic->getContentSize().height;
            }
            m_bgNode->addChild(tbatch);
        } else {
            int bgcount = (m_infoList->getContentSize().height)/100+2;
            for (int i = 0; i<=bgcount; i++) {
                auto pic = CCLoadSprite::createSprite("technology_09.png");
                pic->setPositionY(-i*100+200);
                pic->setScale(1.03);
                m_bgNode->addChild(pic);
            }
        }
        
        ActivityController::getInstance()->sortActivityArr();
        m_data = CCArray::create();
        
        m_data->addObjectsFromArray(ActivityController::getInstance()->activityArr);
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        
        ActivityListAdView* adView = ActivityListAdView::create();
        if (CCCommonUtils::isIosAndroidPad())
        {
            adView->setPosition(768, -m_adNode->getPositionY());
        } else {
            adView->setPosition(-2, -408);
        }
        m_adNode->addChild(adView);
        
        GetActivityDetailCommand* cmd = new GetActivityDetailCommand();
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ActivityListView::getServerData), NULL));
        cmd->sendAndRelease();
        
		bRet = true;
	}while(0);
	return bRet;
}

void ActivityListView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("133000"));
}

void ActivityListView::onExit()
{
    CCNode::onExit();
}

bool ActivityListView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_adNode", CCNode*, this->m_adNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    return false;
}

CCSize ActivityListView::tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return CCSizeZero;
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1536, 370);
    } else {
        if (idx == 0)
        {
            return CCSize(640, 195);
        }
        return CCSize(640, 195);
    }
}

CCSize ActivityListView::cellSizeForTable(  cocos2d::extension::TableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536, 370);
    } else {
        return CCSize(640, 195);
    }
}

CCTableViewCell* ActivityListView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return NULL;
    }
    
    ActivityListNewCell* cell = (ActivityListNewCell*)table->dequeueCell();
    ActivityEventObj* obj = (ActivityEventObj*)m_data->objectAtIndex(idx);
    if (cell)
    {
        cell->setData(obj, m_infoList);
    }
    else
    {
        cell = ActivityListNewCell::create(obj, m_infoList);
    }
    cell->setZOrder(m_data->count()-idx);
    return cell;
}

ssize_t ActivityListView::numberOfCellsInTableView(  cocos2d::extension::TableView *table)
{
    return m_data->count();
}

void ActivityListView::getServerData(cocos2d::CCObject *obj)
{
    NetResult* result = dynamic_cast<NetResult*>(obj);
    auto dic = _dict(result->getData());
    if(dic)
    {
        string nameStr = _lang(dic->valueForKey("name")->getCString());
        if (nameStr.length() != 0)
        {
            ActivityController::getInstance()->eventInfo.currentName = _lang(dic->valueForKey("name")->getCString());
        }else{
            ActivityController::getInstance()->eventInfo.currentName = "";
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACTIVITY_CELL_REFRESH);
    }
}

// class ActivityListNewCell
ActivityListNewCell* ActivityListNewCell::create(ActivityEventObj *actObj, cocos2d::CCNode *clickArea)
{
    auto ret = new ActivityListNewCell();
    if (ret && ret->init(actObj, clickArea)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityListNewCell::init(ActivityEventObj *actObj, cocos2d::CCNode *clickArea)
{
    setData(actObj, clickArea);
    return true;
}

void ActivityListNewCell::setData(ActivityEventObj *actObj, cocos2d::CCNode *clickArea)
{
    this->removeAllChildrenWithCleanup(true);
    if (actObj->type != 6) {
        auto cell = ActivityListCell::create(actObj, clickArea);
        this->addChild(cell);
    } else {
        auto cell = ActivityListLuaCell::create(actObj, clickArea);
        this->addChild(cell);
    }
}

//class
ActivityListLuaCell* ActivityListLuaCell::create(ActivityEventObj *actObj, cocos2d::CCNode *clickArea)
{
    auto ret = new ActivityListLuaCell(actObj, clickArea);
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityListLuaCell::init()
{
    this->setContentSize(CCSize(640, 195));
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCSize(1536, 370));
    }
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCBLoadFile("ActivityListCell",this,this);
    m_cardNode->setVisible(false);
    m_cellNode->setVisible(true);
    m_nameLabel->setString(m_obj->name.c_str());
    m_infoTTF->setString(_lang(m_obj->desc_info).c_str());
    m_nameLabel->setColor({255,220,166});
    m_infoTTF->setColor({255,232,205});
    m_timeLabel1->setColor({255,232,205});
    if (LuaController::getInstance()->checkActLuaValid(m_obj))
    {
        CCNode* node = LuaController::getInstance()->createActivityListCellSprite(m_obj);
        if (node && node->getChildrenCount()) {
            m_aIcon->removeAllChildrenWithCleanup(true);
            m_aIcon->addChild(node);
        }
    }
    return true;
}

void ActivityListLuaCell::onGameTick(float time)
{
    if (!m_obj)
    {
        return;
    }
    double nowTime = GlobalData::shared()->getWorldTime();
    
    if (m_obj->startTime<nowTime && m_obj->endTime>nowTime)
    {
        double leftTime = m_obj->endTime - nowTime;
        m_timeLabel1->setString(_lang_1("105805", CC_SECTOA(leftTime)));
    }
    else if (m_obj->startTime > nowTime)
    {
        double leftTime = m_obj->startTime - nowTime;
        m_timeLabel1->setString(_lang_1("105804", CC_SECTOA(leftTime)));
    }
    else
    {
        m_timeLabel1->setString("");
        //如何处理待定
    }
}

void ActivityListLuaCell::onEnter()
{
    CCNode::onEnter();
    setSwallowsTouches(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    onGameTick(0);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ActivityListLuaCell::onGameTick), this, 1,kCCRepeatForever, 0.0f, false);
}

void ActivityListLuaCell::onExit()
{
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ActivityListLuaCell::onGameTick), this);
    CCNode::onExit();
}

bool ActivityListLuaCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_clickArea, pTouch)) {
        return false;
    }
    if (isTouchInside(this, pTouch))
    {
        this->setScale(0.98);
        return true;
    }
    return false;
}

void ActivityListLuaCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    this->setScale(1);
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<10)
    {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        PopupViewController::getInstance()->addPopupInView(ActivityView::create(m_obj));
//        PopupViewController::getInstance()->addPopupInView(ActivityDetailView::create(m_obj));
    }
}

bool ActivityListLuaCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cardNode", CCNode*, this->m_cardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellNode", CCNode*, this->m_cellNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTTF", CCLabelIF*, this->m_infoTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel1", CCLabelIF*, this->m_timeLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_aIcon", CCNode*, this->m_aIcon);
    return false;
}

// class ActivityListCell

ActivityListCell* ActivityListCell::create(ActivityEventObj* actObj,CCNode* clickArea)
{
    auto ret = new ActivityListCell();
    if (ret && ret->init(actObj,clickArea))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityListCell::init(ActivityEventObj* actObj,CCNode* clickArea)
{
    bool ret = true;
    m_btnReward=NULL;
    m_btnInfo=NULL;
    m_sprItemBG=NULL;
    m_waitInterface = NULL;
    m_bTick = false;
    std::string language = LocalController::shared()->getLanguageFileName();
    if (CCLabelIF::canBeSupportedWithBmpFont(language)) {
        CCLabelIF::enableBmFont(true);
    }
    auto ccb = CCBLoadFile("ActivityListCell",this,this);
    setContentSize(ccb->getContentSize());

    if (CCLabelIF::canBeSupportedWithBmpFont(language)) {
//        m_nameLabel->setFntFile("Arial_Bold_Border.fnt");
//        m_infoTTF->setFntFile("Arial_Bold_Border.fnt");
//        m_timeLabel1->setFntFile("Arial_Bold_Border.fnt");
//        m_desText1->setFntFile("Arial_Bold_Border.fnt");
//        m_lblItem1->setFntFile("Arial_Bold_Border.fnt");
//        m_lblItem2->setFntFile("Arial_Bold_Border.fnt");
//        m_moreLabel->setFntFile("Arial_Bold.fnt");
//        m_rewardName->setFntFile("Arial_Bold_Border.fnt");
//        m_timeLabel2->setFntFile("Arial_Bold_Border.fnt");
        
        m_nameLabel->setFntFile("Arial_Bold_Regular.fnt");
        m_infoTTF->setFntFile("Arial_Bold_Regular.fnt");
        m_timeLabel1->setFntFile("Arial_Bold_Regular.fnt");
        m_desText1->setFntFile("Arial_Bold_Regular.fnt");
        m_lblItem1->setFntFile("Arial_Bold_Regular.fnt");
        m_lblItem2->setFntFile("Arial_Bold_Regular.fnt");
        m_moreLabel->setFntFile("Arial_Bold.fnt");
        m_rewardName->setFntFile("Arial_Bold_Regular.fnt");
        m_timeLabel2->setFntFile("Arial_Bold_Regular.fnt");
    }
    if (CCLabelIF::canBeSupportedWithBmpFont(language)) {
        CCLabelIF::enableBmFont(false);
    }
    m_desText1->setColor({0,234,255});
    m_rewardName->setColor({0,234,255});
    m_nameLabel->setColor({255,220,166});
    
    m_infoTTF->setColor({255,232,205});
    m_timeLabel1->setColor({255,232,205});
    m_lblItem1->setColor({255,232,205});
    m_lblItem2->setColor({255,232,205});
    m_moreLabel->setColor({255,232,205});
    m_timeLabel2->setColor({255,232,205});
    
    m_clickArea = clickArea;
    setData(actObj);
    return ret;
}

void ActivityListCell::setData(ActivityEventObj* actObj)
{
    
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface=NULL;
    }
    m_actObj = actObj;
    m_bTick = true;
    m_nameLabel->setString(m_actObj->name.c_str());
    m_aIcon->removeAllChildrenWithCleanup(true);
    string logoStr = CCString::createWithFormat("Ativity_iconLogo_%d.png",m_actObj->type)->getCString();
    CCSprite* logo = logo = CCLoadSprite::createSprite(logoStr.c_str());
    m_aIcon->addChild(logo);
    m_cardNode->setVisible(false);
    m_cellNode->setVisible(false);
    cardRewardNode->setVisible(false);
    m_btnInfo->setVisible(m_actObj->type==2);
    switch (m_actObj->type) {
        case 1:
        case 3:
        {
            m_cellNode->setVisible(true);
            onGameTick(0);
            if (m_actObj->id == "57000")
            {
                refreshData(NULL);
            }else{
                m_infoTTF->setString(_lang(m_actObj->desc_info).c_str());
            }
        }
            break;
        case 2:
        {
        }
            break;
        case 4:
        {
            m_cellNode->setVisible(true);
            m_infoTTF->setString(_lang(m_actObj->desc_info).c_str());
        }
            break;
        case 7:
        {
            m_cellNode->setVisible(true);
            m_nameLabel->setString(m_actObj->name);
            m_infoTTF->setString(_lang(m_actObj->desc_info).c_str());
        }
            break;
    }
    int hTxt = m_infoTTF->getPositionY() - 20;
    hTxt -= m_infoTTF->getContentSize().height*m_infoTTF->getOriginScaleY()/2.0;
    m_timeLabel1->setPositionY(hTxt);
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_timeLabel1->setPositionY(m_infoTTF->getPositionY() - m_infoTTF->getContentSize().height * m_infoTTF->getOriginScaleY() / 2.0 - m_timeLabel1->getContentSize().height * m_timeLabel1->getOriginScaleY() / 2.0 - 25);
    }
}

void ActivityListCell::refreshData(cocos2d::CCObject *obj)
{
    if(m_actObj && m_actObj->type==1)
    {
        if (ActivityController::getInstance()->eventInfo.currentName.length() == 0)
        {
            m_infoTTF->setString(_lang("105800").c_str());
            return;
        }
        int current = ActivityController::getInstance()->eventInfo.curPhase;
        if (current > ActivityController::getInstance()->eventInfo.maxPhase) {
            current = ActivityController::getInstance()->eventInfo.maxPhase;
        }
        m_infoStr = _lang_2("105835",CC_ITOA(current),CC_ITOA(ActivityController::getInstance()->eventInfo.maxPhase));
        m_infoStr += " ";
        m_infoStr += ActivityController::getInstance()->eventInfo.currentName;
        m_infoTTF->setString(m_infoStr.c_str());
    }
}

void ActivityListCell::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityListCell::buySuccess), PAYMENT_COMMAND_RETURN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityListCell::errorPayment), MSG_PAY_FAILED, NULL);
    setTouchEnabled(true);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ActivityListCell::onGameTick), this, 1,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityListCell::rewardSpe), GOLDEXCHANGE_SHOW_COMMAND, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityListCell::refreshData), ACTIVITY_CELL_REFRESH, NULL);
    onGameTick(0.0);
}

void ActivityListCell::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PAYMENT_COMMAND_RETURN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PAY_FAILED);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GOLDEXCHANGE_SHOW_COMMAND);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ACTIVITY_CELL_REFRESH);
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ActivityListCell::onGameTick), this);
    CCNode::onExit();
}

void ActivityListCell::onGameTick(float time)
{
    if(m_actObj!=NULL && m_actObj->type==5){
        m_timeLabel1->setString("");
        return;
    }
    if (m_actObj->type == 2)
    {
        return;
    }
    double nowTime = GlobalData::shared()->getWorldTime();
    
    if (m_actObj->startTime<nowTime && m_actObj->endTime>nowTime)
    {
        double leftTime = m_actObj->endTime - nowTime;
        m_timeLabel1->setString(_lang_1("105805", CC_SECTOA(leftTime)));
    }
    else if (m_actObj->startTime > nowTime)
    {
        double leftTime = m_actObj->startTime - nowTime;
        m_timeLabel1->setString(_lang_1("105804", CC_SECTOA(leftTime)));
    }
    else
    {
        m_timeLabel1->setString("");
        //如何处理待定
    }
}

bool ActivityListCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_clickArea, pTouch)) {
        return false;
    }
    if (m_touchNode && isTouchInside(m_touchNode, pTouch))
    {
        this->setScale(0.98);
        return true;
    }
    return false;
}

void ActivityListCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    this->setScale(1);
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) > 10)
    {
        return;
    }
    else
    {
        if(m_actObj->type == 1)
        {
            onGeneralClick();
        }
        else if (m_actObj->type == 2)
        {
        }else if(m_actObj->type == 3){
            PopupViewController::getInstance()->addPopupView(AllianceActivityView::create(m_actObj));
        }else if(m_actObj->type==4){
            PopupViewController::getInstance()->addPopupView(AllianceActBossView::create(m_actObj));
        }else if(m_actObj->type==7){
            PopupViewController::getInstance()->addPopupView(CrossServiceActivityView::create(m_actObj));
        }
    }
}

void ActivityListCell::onClickQuickBtn(cocos2d::CCObject *pSender, CCControlEvent event)
{
    onGeneralClick();
}

void ActivityListCell::onGeneralClick()
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    double nowTime = GlobalData::shared()->getWorldTime();
    
    if(m_actObj->type == 3){
        PopupViewController::getInstance()->addPopupView(AllianceActivityView::create(m_actObj));
    }
    else if (m_actObj->startTime<nowTime && m_actObj->endTime>nowTime)
    {
        if(m_actObj->type == 4){
            PopupViewController::getInstance()->addPopupView(AllianceActBossView::create(m_actObj));
        }else{
            //活动已开始
            switch (atoi(m_actObj->id.c_str()))
            {
                case 57000:
                    PopupViewController::getInstance()->addPopupView(ActivityBeginView::create());
                    break;
                    
                default:
                    break;
            }
        }
    }
    else
    {
        PopupViewController::getInstance()->addPopupView(ActivityTipView::create(m_actObj));
    }
}

void ActivityListCell::onSpeClick()
{
    //todobyjiangshan
}
void ActivityListCell::onTipClick(CCObject *pSender, CCControlEvent event){
    std::string tipB = _lang("101289");
    string showTip = _lang("101286");
    if(!tipB.empty()){
        showTip.append("\n");
        showTip.append(tipB);
    }
    showTip.append("\n");
    showTip.append(_lang("101288"));
    PopupViewController::getInstance()->addPopupView(TipsView::create(showTip) );
}

void ActivityListCell::rewardSpe(CCObject *ccObj){
}

void ActivityListCell::buySuccess(CCObject *ccObj){
    bool pay = false;
    if(ccObj != NULL){
        CCString* str = dynamic_cast<CCString*>(ccObj);
        if (GlobalData::shared()->goldExchangeList.find(m_actObj->activityid)!=GlobalData::shared()->goldExchangeList.end()) {
            auto  m_dataItem = GlobalData::shared()->goldExchangeList[m_actObj->activityid];
            if(str && str->getCString() && std::string(str->getCString()).compare(m_dataItem->id) == 0 ){
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_SHOW_COMMAND,CCString::create(m_dataItem->id));
            }
        }
    }
}
    
void ActivityListCell::errorPayment(cocos2d::CCObject *obj){
    if(m_actObj && m_actObj->type==2){
         this->unschedule(schedule_selector(ActivityListCell::onShowError));
         this->schedule(schedule_selector(ActivityListCell::onShowError),0.3,1,0);
    }
}

void ActivityListCell::onShowError(float dt){
    PayController::getInstance()->showErrorDialog();
    this->unschedule(schedule_selector(ActivityListCell::onShowError));
}

void ActivityListCell::onRewardClick(CCObject *pSender, CCControlEvent event){
    if(m_waitInterface)
        return; 
    m_waitInterface= GameController::getInstance()->showWaitInterface(m_btnReward);
    MonthCardRewardCommand* cmd = new MonthCardRewardCommand(m_actObj->activityid);
//    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ActivityListCell::rewardSpe), NULL));
    cmd->sendAndRelease();
}
SEL_CCControlHandler ActivityListCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
//    if(m_actObj->type == 2){
//        CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipClick", ActivityListCell::onTipClick);
//        CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardClick", ActivityListCell::onRewardClick);
//    }
    return NULL;
}

bool ActivityListCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTTF", CCLabelIF*, m_infoTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel1", CCLabelIF*, m_timeLabel1);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprItemBG", CCScale9Sprite*, m_sprItemBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnReward", CCControlButton*, m_btnReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnInfo", CCControlButton*, m_btnInfo);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblItem1", CCLabelIF*, this->m_lblItem1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblItem2", CCLabelIF*, this->m_lblItem2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreLabel", CCLabelIF*, this->m_moreLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desText1", CCLabelIF*, this->m_desText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeItem1", CCNode*, this->m_nodeItem1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeItem2", CCNode*, this->m_nodeItem2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_aIcon", CCNode*, this->m_aIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cardNode", CCNode*, this->m_cardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellNode", CCNode*, this->m_cellNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "cardRewardNode", CCNode*, this->cardRewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardItemNode", CCNode*, this->m_rewardItemNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardName", CCLabelIF*, this->m_rewardName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel2", CCLabelIF*, this->m_timeLabel2);
    return false;
}

