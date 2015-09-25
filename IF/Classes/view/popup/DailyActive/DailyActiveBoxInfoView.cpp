//
//  DailyActiveBoxInfoView.cpp
//  IF
//
//  Created by chenfubi on 15/4/13.
//
//

#include "DailyActiveBoxInfoView.h"
#include "DailyActiveController.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "SoundController.h"

DailyActiveBoxInfoView* DailyActiveBoxInfoView::create(string itemId, int index)
{
	DailyActiveBoxInfoView* ret = new DailyActiveBoxInfoView();
    if (ret && ret->init(itemId, index))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DailyActiveBoxInfoView::init(string itemId, int index)
{
	bool bRet = false;
	do
	{
        if (!PopupBaseView::init())
        {
            break;
        }
        setIsHDPanel(true);
//        CCLoadSprite::doResourceByCommonIndex(502, true);
//        
//        setCleanFunction([](){
//            CCLoadSprite::doResourceByCommonIndex(502, false);
//        });
        
        CCBLoadFile("DailyActiveBoxInfoView", this, this);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winSize);
        
        int extH = this->getExtendHeight();
        if (!CCCommonUtils::isIosAndroidPad())
        {
            m_mainNode->setPositionY(m_mainNode->getPositionY() + extH/2);
        }
        
        
        m_itemId = itemId;
        m_index = index;
        initBoxInfo();
        
        m_tableView = CCTableView::create(this, m_listNode->getContentSize());
        m_tableView->setDirection(kCCScrollViewDirectionVertical);
        m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_listNode->addChild(m_tableView);
        
		bRet = true;
	}while(0);
	return bRet;
}

void DailyActiveBoxInfoView::initBoxInfo()
{
    DailyActiveBoxInfo info = DailyActiveController::shared()->m_boxInfo[atoi(m_itemId.c_str())];
    m_titleTTF->setVisible(true);
    m_congratureNode->setVisible(false);
    if (info.state == BOX_COMPLETE)
    {
        m_titleTTF->setVisible(false);
        m_congratuTTF->setString(_lang("107502"));
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("137403").c_str());
        m_congratureNode->setVisible(true);
        addParticle();
    }
    else if (info.state == BOX_UN_COMPLETE)
    {
        m_titleTTF->setString(_lang_1("133267", CC_ITOA(m_index)));
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("133268").c_str());
    }
    else if (info.state == BOX_GET_REWARD)
    {
        m_titleTTF->setString(_lang("133109"));
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("101312").c_str());
    }
    
    m_data = info.reward;
}

void DailyActiveBoxInfoView::onOkClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    DailyActiveBoxInfo info = DailyActiveController::shared()->m_boxInfo[atoi(m_itemId.c_str())];
    if (info.state == BOX_COMPLETE)
    {
        DailyActiveController::shared()->getBoxReward(m_itemId);
    }
    doCloseAction();
}

void DailyActiveBoxInfoView::doCloseAction()
{
    PopupViewController::getInstance()->removePopupView(this);
}

void DailyActiveBoxInfoView::addParticle()
{
    for (int i=1; i<4; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("DailyActivity_%d",i)->getCString());
//        particle->setPosition(CCPoint(40, 15));
        m_particleNode1->addChild(particle);
    }
    for (int i=4; i<7; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("DailyActivity_%d",i)->getCString());
        //        particle->setPosition(CCPoint(40, 15));
        m_particleNode2->addChild(particle);
    }
    
    this->schedule(schedule_selector(DailyActiveBoxInfoView::playGiftAnim), 0.2f, 5, 0.0f);
    
    SoundController::sharedSound()->playEffects(Music_Daily_Effect);
}

void DailyActiveBoxInfoView::playGiftAnim(float dt)
{
    auto newBatch = ParticleController::createParticleBatch();
    for (int i=1; i<5; i++) {
        auto praticle1 = ParticleController::createParticle(CCString::createWithFormat("Fireworks_%d",i)->getCString());
        newBatch->addChild(praticle1);
    }
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    int maxX = size.width;
    int maxY = size.height;
    int x = rand() % maxX;
    int y = rand() % maxY;
    this->addChild(newBatch);
    newBatch->setPosition(x, y);
}

void DailyActiveBoxInfoView::reloadData(cocos2d::CCObject *pObj)
{
    initBoxInfo();
    m_tableView->reloadData();
}

CCSize DailyActiveBoxInfoView::tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return CCSizeZero;
    }
    return CCSizeMake(530, 100);
}

CCSize DailyActiveBoxInfoView::cellSizeForTable(  cocos2d::extension::TableView *table)
{
    return CCSizeMake(530, 100);
}

CCTableViewCell* DailyActiveBoxInfoView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return NULL;
    }
    CCDictionary* dict = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    BoxInfoCell* cell = dynamic_cast<BoxInfoCell*>(table->dequeueCell());
    if (cell)
    {
        cell->setData(dict);
    }
    else
    {
        cell = BoxInfoCell::create(dict);
    }
    return cell;
}

ssize_t DailyActiveBoxInfoView::numberOfCellsInTableView(  cocos2d::extension::TableView *table)
{
    return m_data->count();
}

void DailyActiveBoxInfoView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DailyActiveBoxInfoView::reloadData), DAILYQEUST_BOX_INIT, NULL);
}

void DailyActiveBoxInfoView::onExit()
{
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DAILYQEUST_BOX_INIT);
    this->unschedule(schedule_selector(DailyActiveBoxInfoView::playGiftAnim));
    CCNode::onExit();
}

bool DailyActiveBoxInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void DailyActiveBoxInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode, pTouch) && (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<20))
    {
        doCloseAction();
    }
}

bool DailyActiveBoxInfoView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_congratureNode", CCNode*, this->m_congratureNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode1", CCNode*, this->m_particleNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode2", CCNode*, this->m_particleNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTTF", CCLabelIF*, this->m_titleTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_congratuTTF", CCLabelIF*, this->m_congratuTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
    return false;
}

SEL_CCControlHandler DailyActiveBoxInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkClick", DailyActiveBoxInfoView::onOkClick);
    
    return NULL;
}


#include "ToolController.h"
#include "RewardController.h"
//CLASS BoxInfoCell
BoxInfoCell* BoxInfoCell::create(CCDictionary* dict)
{
    BoxInfoCell* ret = new BoxInfoCell();
    if (ret && ret->init(dict))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BoxInfoCell::init(CCDictionary* dict)
{
    bool ret = false;
    do {
        auto ccb = CCBLoadFile("BoxInfoCell", this, this);
        this->setContentSize(ccb->getContentSize());
        setData(dict);
        ret = true;
    } while (0);
    return ret;
}

void BoxInfoCell::setData(CCDictionary* dict)
{
    string num = "X";
    int type = dict->valueForKey("type")->intValue();
    string icon = "";
    string toolBgIcon = "";
    if (type == R_GOODS)
    {
        CCDictionary* value = (CCDictionary*)dict->objectForKey("value");
        string itemId = value->valueForKey("id")->getCString();
        ToolInfo toolInfo = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
        icon = toolInfo.icon;
        icon.append(".png");
        m_infoTTF->setString(toolInfo.getName());
        num.append(CC_CMDITOA(value->valueForKey("num")->intValue()));
        toolBgIcon = CCCommonUtils::getToolBgByColor(toolInfo.color);
    }
    else
    {
        num.append(CC_CMDITOA(dict->valueForKey("value")->intValue()));
        icon = RewardController::getInstance()->getPicByType(type, 0);
        toolBgIcon = CCCommonUtils::getToolBgByColor(4);
        m_infoTTF->setString(RewardController::getInstance()->getNameByType(type));
    }
    m_icon->setDisplayFrame(CCLoadSprite::loadResource(icon.c_str()));
    m_colorBg->setDisplayFrame(CCLoadSprite::loadResource(toolBgIcon.c_str()));
    m_numTTF->setString(num);
}

void BoxInfoCell::onEnter()
{
    CCNode::onEnter();
}

void BoxInfoCell::onExit()
{
    CCNode::onExit();
}

bool BoxInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCSprite*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_colorBg", CCSprite*, this->m_colorBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTTF", CCLabelIF*, this->m_infoTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTTF", CCLabelIF*, this->m_numTTF);
    return false;
}

SEL_CCControlHandler BoxInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoClick", DailyActiveItemInfoView::onGoClick);
    return NULL;
}



//CLASS DailyActiveItemInfoView

DailyActiveItemInfoView* DailyActiveItemInfoView::create(string itemId)
{
	DailyActiveItemInfoView* ret = new DailyActiveItemInfoView();
    if (ret && ret->init(itemId))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DailyActiveItemInfoView::init(string itemId)
{
	bool bRet = false;
	do
	{
        if (!PopupBaseView::init())
        {
            break;
        }
        setIsHDPanel(true);
//        CCLoadSprite::doResourceByCommonIndex(502, true);
//        setCleanFunction([](){
//            CCLoadSprite::doResourceByCommonIndex(502, false);
//        });
        
        CCBLoadFile("DailyActiveItemInfoView", this, this);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winSize);
        int extH = this->getExtendHeight();
//        if(!CCCommonUtils::isIosAndroidPad())
        m_mainNode->setPositionY(m_mainNode->getPositionY() + extH/2);
        
        m_itemId = itemId;
        DailyActiveItemInfo info = DailyActiveController::shared()->m_itemInfo[m_itemId];
        m_titleTTF->setString(info.name);
        m_descripTTF->setString(_lang(info.desc));
        
        string infoTTFstr = _lang_1(info.target, CC_ITOA(info.totalTimes));
        if (info.desc2 == 1)
        {
            int total = info.totalNum*info.totalTimes;
            infoTTFstr = _lang_2(info.target, CC_ITOA(total), CC_ITOA(info.totalNum));
        }
        else if (info.desc2 == 2)
        {
            infoTTFstr = _lang_1(info.target, CC_ITOA(info.totalNum));
        }
        m_infoTTF1->setString(infoTTFstr);
        
        string a = CC_ITOA(info.times);
        a.append("/").append(CC_ITOA(info.totalTimes));
        string achieve = _lang_2("133252", CC_ITOA(info.times), CC_ITOA(info.totalTimes));
        m_infoTTF11->setString(achieve);
        
        m_jinduTTF->setString(_lang_2("133253", CC_ITOA(info.point*info.times), CC_ITOA(info.point*info.totalTimes)));
        CCCommonUtils::setButtonTitle(m_goBtn, _lang("108531").c_str());
        if (m_itemId == "5000004")
        {
            m_goBtn->setVisible(false);
        }
        m_infoImg->setDisplayFrame(CCLoadSprite::loadResource(info.pic.c_str()));
        
		bRet = true;
	}while(0);
	return bRet;
}

void DailyActiveItemInfoView::onGoClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removeAllPopupView();
    DailyActiveController::shared()->goActTarget(m_itemId);
}

void DailyActiveItemInfoView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void DailyActiveItemInfoView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

bool DailyActiveItemInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void DailyActiveItemInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode, pTouch) && (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<20))
    {
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool DailyActiveItemInfoView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTTF", CCLabelIF*, this->m_titleTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descripTTF", CCLabelIF*, this->m_descripTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTTF1", CCLabelIF*, this->m_infoTTF1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTTF11", CCLabelIF*, this->m_infoTTF11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jinduTTF", CCLabelIF*, this->m_jinduTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goBtn", CCControlButton*, this->m_goBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoImg", CCSprite*, this->m_infoImg);
    return false;
}

SEL_CCControlHandler DailyActiveItemInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoClick", DailyActiveItemInfoView::onGoClick);
    return NULL;
}