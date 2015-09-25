//
//  ActivityListAdView.cpp
//  IF
//
//  Created by chenfubi on 15/1/9.
//
//

#include "ActivityListAdView.h"
#include "ActivityController.h"
#include "ParticleFireAni.h"
#include "PopupViewController.h"
#include "ActivityTipView.h"
#include "ActivityBeginView.h"
#include "SoundController.h"
#include "AllianceActivityView.h"
#include "ActivityListView.h"
#include "AllianceActBossView.h"
#include "ActivityDetailView.h"
#include "LuaController.h"
#include "ActivityView.h"
#include "CrossServiceActivityView.h"

const int tableCellW = 640;

float hd_ad_cell_width = 1156;
float hd_ad_cell_height = 645;
float hd_sc = hd_ad_cell_width / tableCellW;

ActivityListAdView* ActivityListAdView::create()
{
    ActivityListAdView* ret = new ActivityListAdView();
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

bool ActivityListAdView::init()
{
	bool bRet = false;
	do
	{
        ignoreAnchorPointForPosition(false);
        setAnchorPoint(Vec2(0,0));
        CCLoadSprite::doResourceByCommonIndex(500, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(500, false);
        });
        auto bg = CCBLoadFile("ActivityListAdView", this, this);
        setContentSize(bg->getContentSize());
        if (CCCommonUtils::isIosAndroidPad())
        {
            this->setAnchorPoint(ccp(0.5, 0.5));
        }
        
        m_data = ActivityController::getInstance()->activityArr;
        
        m_tabView = CCTableView::create(this, m_listNode->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionHorizontal);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchMode(kCCTouchesOneByOne);
        m_tabView->setTouchPriority(-1);
        m_tabView->setDelegate(this);
        m_tabView->setTouchEnabled(false);
        m_listNode->addChild(m_tabView);
        m_actTxt->setString(_lang("133103"));
        
        if (CCCommonUtils::isIosAndroidPad()) {
            float w = (1536.0 - m_listNode->getContentSize().width * m_listNode->getScaleX()) / 2.0;
            float h = hd_ad_cell_height;
            auto layer1 = CCModelLayerColor::create();
            layer1->setAnchorPoint(ccp(0, 0));
            layer1->setContentSize(CCSize(w, h));
            layer1->setPosition(-w, 0);
            layer1->setOpacity(255);
            layer1->setColor(ccBLACK);
            layer1->setTouchPriority(-999);
            m_layerNode->addChild(layer1);
            
            layer1 = CCModelLayerColor::create();
            layer1->setAnchorPoint(CCPointZero);
            layer1->setPosition(ccp(m_listNode->getContentSize().width * m_listNode->getScaleX(), 0));
            layer1->setContentSize(CCSize(w, h));
            layer1->setOpacity(255);
            layer1->setColor(ccBLACK);
            layer1->setTouchPriority(-999);
            m_layerNode->addChild(layer1);
        }
        
        initPageNode(0);
        
		bRet = true;
	}while(0);
	return bRet;
}

void ActivityListAdView::onEnter()
{
    CCNode::onEnter();
    setSwallowsTouches(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    this->schedule(schedule_selector(ActivityListAdView::onGameTick),10.0);
}

void ActivityListAdView::onExit()
{
    setTouchEnabled(false);
    this->unschedule(schedule_selector(ActivityListAdView::onGameTick));
    CCNode::onExit();
}

void ActivityListAdView::onGameTick(float time)
{
    if (m_data->count() <= 1)
    {
        return;
    }
    int addX = 0;
    int offSetX = 0;
    if (!CCCommonUtils::isIosAndroidPad()) {
        addX = -tableCellW;
        offSetX = int((m_tabView->getContentOffset().x + addX) / tableCellW) * tableCellW;
    } else {
        addX = -hd_ad_cell_width;
        offSetX = int((m_tabView->getContentOffset().x + addX) / hd_ad_cell_width) * hd_ad_cell_width;
    }
    int minX  = m_tabView->minContainerOffset().x;
    if(offSetX > 0){
        offSetX = 0;
    }else
        if(offSetX < minX){
            offSetX = minX;
        }
    if (m_tabView->getContentOffset().x == m_tabView->minContainerOffset().x)
    {
        m_tabView->setContentOffsetInDuration(ccp(0, 0), 0.5);
        initPageNode(0);
    }
    else
    {
        m_tabView->setContentOffsetInDuration(ccp(offSetX, 0), 0.5);
        initPageNode(offSetX);
    }
}

void ActivityListAdView::initPageNode(int offSetX)
{
    m_pageNode->removeAllChildren();
    int width = 30;
    
    int count = (m_data->count()>0)? m_data->count():1;
    float m_startX = 0;
    if(count%2==1){
        m_startX = -int(count/2)*width;
    }else{
        m_startX = -int(count/2)*width+width/2;
    }
    int dataIndex = round(abs(offSetX) / tableCellW);
    if (CCCommonUtils::isIosAndroidPad()) {
        dataIndex = round(abs(offSetX) / hd_ad_cell_width);
    }
    for (int i=0; i<count; i++) {
        auto spr = CCLoadSprite::createSprite("Recharge_fanye.png");
        m_pageNode->addChild(spr);
        spr->setPositionX(m_startX+width*i);
        if (dataIndex == i)
        {
            CCSprite* sp = CCLoadSprite::createSprite("Recharge_fanye2.png");
            m_pageNode->addChild(sp);
            sp->setPositionX(m_startX+width*i);
        }
    }
}

bool ActivityListAdView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_listNode, pTouch))
    {
        this->unschedule(schedule_selector(ActivityListAdView::onGameTick));
        m_x = 9999;
        return true;
    }
    return false;
}

void ActivityListAdView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(isTouchInside(m_listNode, pTouch)){
        int dx = pTouch->getLocation().x - pTouch->getStartLocation().x;
        bool st = false;
        if (CCCommonUtils::isIosAndroidPad()) {
            if (dx > 24 || dx < -24) {
                st = true;
            }
        } else {
            if (dx > 10 || dx < -10) {
                st = true;
            }
        }
        if(st){
            if(m_x == 9999){
                m_x = m_tabView->getContentOffset().x;
            }
            int offX = m_x + dx;
            int min = m_tabView->minContainerOffset().x;
            int max = 0;
            if(offX > max){
                offX = max;
            }
            if(offX < min){
                offX = min;
            }
            m_tabView->setContentOffset(ccp(offX, 0), false);
        }
    }
}

void ActivityListAdView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    int addX = -tableCellW/2;
    if (CCCommonUtils::isIosAndroidPad()) {
        addX = -hd_ad_cell_width / 2;
    }
    int dx = pTouch->getStartLocation().x - pTouch->getLocation().x;
    if (CCCommonUtils::isIosAndroidPad()) {
        if(dx > 40){
            addX = -hd_ad_cell_width;
        }else if(dx < -40){
            addX = 0;
        }
    } else {
        if(dx > 20){
            addX = -tableCellW;
        }else if(dx < -20){
            addX = 0;
        }
    }
    int offSetX = int((m_tabView->getContentOffset().x + addX) / tableCellW) * tableCellW;
    if (CCCommonUtils::isIosAndroidPad()) {
        offSetX = int((m_tabView->getContentOffset().x + addX) / hd_ad_cell_width) * hd_ad_cell_width;
    }
    int minX  = m_tabView->minContainerOffset().x;
    if(offSetX > 0){
        offSetX = 0;
    }
    if(offSetX < minX){
        offSetX = minX;
    }
    m_tabView->setContentOffset(ccp(offSetX, 0), true);
    if (m_data->count() <= 1)
    {
        return;
    }
    initPageNode(offSetX);
    this->schedule(schedule_selector(ActivityListAdView::onGameTick), 10.0f);
    
}

SEL_CCControlHandler ActivityListAdView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    return NULL;
}

bool ActivityListAdView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pageNode", CCNode*, m_pageNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_actTxt", CCLabelIF*, m_actTxt);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_layerNode", CCNode*, m_layerNode);
    }
    return false;
}

CCSize ActivityListAdView::tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return CCSizeZero;
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1156, 645);
    }
    return CCSize(tableCellW, 357.0f);
}

CCSize ActivityListAdView::cellSizeForTable(  cocos2d::extension::TableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1156, 645);
    }
    return CCSize(tableCellW, 357.0f);
}

CCTableViewCell* ActivityListAdView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return NULL;
    }
    
//    ActivityListAdCell* cell = (ActivityListAdCell*)table->dequeueCell();
    ActivityListAdNewCell* cell = (ActivityListAdNewCell*)table->dequeueCell();
    ActivityEventObj* obj = (ActivityEventObj*)m_data->objectAtIndex(idx);
    if (cell)
    {
        cell->setData(obj);
    }
    else
    {
        cell = ActivityListAdNewCell::create(obj);
//        cell = ActivityListAdCell::create(obj);
    }
    
    return cell;
}

ssize_t ActivityListAdView::numberOfCellsInTableView(  cocos2d::extension::TableView *table)
{
    return m_data->count();
}

void ActivityListAdView::tableCellWillRecycle(  cocos2d::extension::TableView *table,   cocos2d::extension::TableViewCell *cell)
{
    
}

void ActivityListAdView::scrollViewDidScroll(CCScrollView *view)
{
    
}

//class new cell
ActivityListAdNewCell* ActivityListAdNewCell::create(ActivityEventObj* obj)
{
    ActivityListAdNewCell* ret = new ActivityListAdNewCell(obj);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityListAdNewCell::init()
{
    setData(m_obj);
    return true;
}

void ActivityListAdNewCell::setData(ActivityEventObj* obj)
{
    this->removeAllChildrenWithCleanup(true);
    m_obj = obj;
    if (m_obj->type != 6) {
        auto cell = ActivityListAdCell::create(m_obj);
        this->addChild(cell);
    } else {
        auto cell = ActivityAdLuaCell::create(m_obj);
        this->addChild(cell);
    }
}


/////////
ActivityAdLuaCell* ActivityAdLuaCell::create(ActivityEventObj *obj)
{
    auto ret = new ActivityAdLuaCell(obj);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityAdLuaCell::init()
{
    this->setContentSize(CCSize(640, 357));
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCSize(1156, 645));
    }
    bool st = false;
    if (LuaController::getInstance()->checkActLuaValid(m_obj))
    {
        CCNode* node = LuaController::getInstance()->createActivityAdCell(m_obj);
        if (node && node->getChildrenCount()) {
            this->addChild(node);
            if (CCCommonUtils::isIosAndroidPad()) {
                node->setContentSize(CCSize(0, 0));
                node->setAnchorPoint(CCPointZero);
                node->setScaleX(1156.0 / 640.0);
                node->setScaleY(645.0 / 357);
            }
        } else {
            st = true;
        }
    }
    else
    {
        st = true;
    }
    
    if (st) {
        auto nameTTF = CCLabelIF::create(m_obj->name.c_str());
        auto infoTTF = CCLabelIF::create(_lang(m_obj->desc_info).c_str());
        auto bgImg = CCLoadSprite::createSprite("activity_ad_beiyong.png");
        nameTTF->setPosition(ccp(16,245));
        nameTTF->setAnchorPoint(ccp(0, 1));
        nameTTF->setFontSize(36);
        nameTTF->setColor({255,219,117});
        nameTTF->setHorizontalAlignment(kCCTextAlignmentLeft);
        nameTTF->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        nameTTF->setDimensions(CCSize(0, 0));
        
        infoTTF->setPosition(ccp(16,201));
        infoTTF->setAnchorPoint(ccp(0, 1));
        infoTTF->setColor({0, 249, 0});
        infoTTF->setHorizontalAlignment(kCCTextAlignmentLeft);
        infoTTF->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        infoTTF->setDimensions(CCSize(300, 0));
        
        
        bgImg->setAnchorPoint(ccp(0, 0));
        bgImg->setPosition(ccp(0, 0));
        if (CCCommonUtils::isIosAndroidPad()) {
            auto node = CCNode::create();
            node->addChild(bgImg);
            node->addChild(nameTTF);
            node->addChild(infoTTF);
            node->setScaleX(1156.0 / 640.0);
            node->setScaleY(645.0 / 357);
            this->addChild(node);
        } else {
            this->addChild(bgImg);
            this->addChild(nameTTF);
            this->addChild(infoTTF);
        }
    }
    
    return true;
}

void ActivityAdLuaCell::onEnter()
{
    CCNode::onEnter();
    setSwallowsTouches(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
}

void ActivityAdLuaCell::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

bool ActivityAdLuaCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(this, pTouch))
    {
        return true;
    }
    return false;
}

void ActivityAdLuaCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<10 || (CCCommonUtils::isIosAndroidPad() && ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) < 24))
    {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        PopupViewController::getInstance()->addPopupInView(ActivityView::create(m_obj));
//        PopupViewController::getInstance()->addPopupInView(ActivityDetailView::create(m_obj));
    }
}
//class cell

ActivityListAdCell* ActivityListAdCell::create(ActivityEventObj* obj)
{
    auto ret = new ActivityListAdCell();
    if (ret && ret->init(obj))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityListAdCell::init(ActivityEventObj* obj)
{
    bool bRet = false;
    do
    {
        auto bg = CCBLoadFile("ActivityListAdCell", this, this);
        setContentSize(m_bgNode->getContentSize());
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCSize(hd_ad_cell_width, hd_ad_cell_height));
            bg->setScaleX(hd_ad_cell_width / 640.0);
            bg->setScaleY(hd_ad_cell_height / 357.0);
        }
        
        m_clipNode = CCClipNode::create(m_bgNode->getContentSize().width, m_bgNode->getContentSize().height);
        m_bgNode->addChild(m_clipNode);
        
        bRet = true;
    } while (0);
    setData(obj);
    return bRet;
}

void ActivityListAdCell::setData(ActivityEventObj *obj)
{
    m_actObj = obj;
    onGameTick(0);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    string img = m_actObj->Advertise_pic + ".png";
    m_nameTTF->setString(m_actObj->name);
    m_infoTTF->setString(_lang(m_actObj->desc_info));
    m_nameTTF->setHorizontalAlignment(kCCTextAlignmentCenter);
    m_infoTTF->setHorizontalAlignment(kCCTextAlignmentCenter);
    m_nameTTF->setFontSize(36);
    m_infoTTF->setDimensions(CCSize(560.0,90));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_infoTTF->setFontSize(m_infoTTF->getFontSize() * hd_sc);
        m_infoTTF->setDimensions(CCSize(560 * hd_sc, 90 * hd_sc));
        m_infoTTF->setScale(m_infoTTF->getScale() / hd_sc);
        m_nameTTF->setScale(m_nameTTF->getScale() / hd_sc);
        m_nameTTF->setFontSize(m_nameTTF->getFontSize() * hd_sc);
        m_timeLabel1->setScale(m_timeLabel1->getScale() / hd_sc);
        m_timeLabel1->setFontSize(m_timeLabel1->getFontSize() * hd_sc);
    }
    auto bgImg = CCLoadSprite::createSprite(img.c_str());
    bgImg->setAnchorPoint(ccp(0, 0));
    if(m_actObj->type==3 || m_actObj->type==4){
        int bgx = bgImg->getContentSize().width*0.1;
        int bgy = bgImg->getContentSize().height*0.1;
        bgImg->setPosition(ccp(-bgx, 0));
        bgImg->setScaleX(1.2);
        bgImg->setScaleY(1.2);
        
        m_nameTTF->setPosition(ccp(40.0,259));
        m_nameTTF->setAnchorPoint(ccp(0, 0.5));
        m_infoTTF->setPosition(ccp(40.0,217));
        m_infoTTF->setAnchorPoint(ccp(0, 0.5));
        m_timeLabel1->setPosition(ccp(40.0,111));
        m_timeLabel1->setAnchorPoint(ccp(0, 0.5));
        m_nameTTF->setColor({255,219,117});
        m_infoTTF->setColor({0,249,0});
        m_timeLabel1->setColor({0,249,0});
        m_nameTTF->setHorizontalAlignment(kCCTextAlignmentLeft);
        m_infoTTF->setHorizontalAlignment(kCCTextAlignmentLeft);
    }else if(m_actObj->type==7){
        m_infoTTF->setDimensions(CCSize(340.0,100));
        m_nameTTF->setPosition(ccp(205,252.2));
        m_nameTTF->setAnchorPoint(ccp(0.5, 0.5));
        m_infoTTF->setPosition(ccp(205,180.9));
        m_infoTTF->setAnchorPoint(ccp(0.5, 0.5));
        m_timeLabel1->setPosition(ccp(205,109));
        m_timeLabel1->setAnchorPoint(ccp(0.5, 0.5));
        m_nameTTF->setColor({255,219,117});
        m_infoTTF->setColor({0,249,0});
        m_timeLabel1->setColor({0,249,0});
        m_nameTTF->setHorizontalAlignment(kCCTextAlignmentCenter);
        m_infoTTF->setHorizontalAlignment(kCCTextAlignmentCenter);
    }else{
        m_nameTTF->setPosition(ccp(324.0,280.2));
        m_nameTTF->setAnchorPoint(ccp(0.5, 0.5));
        m_infoTTF->setPosition(ccp(324.0,244.9));
        m_infoTTF->setAnchorPoint(ccp(0.5, 0.5));
        m_timeLabel1->setPosition(ccp(324.0,83.1));
        m_timeLabel1->setAnchorPoint(ccp(0.5, 0.5));
        m_nameTTF->setColor({0,0,0});
        m_infoTTF->setColor({0,0,0});
        m_timeLabel1->setColor({0,249,0});
        m_nameTTF->setHorizontalAlignment(kCCTextAlignmentCenter);
        m_infoTTF->setHorizontalAlignment(kCCTextAlignmentCenter);
    }
    m_clipNode->removeAllChildren();
    m_clipNode->addChild(bgImg);
}

void ActivityListAdCell::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
    this->schedule(schedule_selector(ActivityListAdCell::onGameTick), 1, kCCRepeatForever, 0.0f);
}

void ActivityListAdCell::onExit()
{
    setTouchEnabled(false);
    this->unschedule(schedule_selector(ActivityListAdCell::onGameTick));
    CCNode::onExit();
}

void ActivityListAdCell::onGameTick(float time)
{
    if (m_actObj == nullptr || m_actObj->Advertise_pic.length() == 0)
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
        string temp = _lang("105800");
        if (m_actObj->type==3) {
            temp = "";
        }
        m_timeLabel1->setString(temp);
    }
}

bool ActivityListAdCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_activityAdImg, pTouch))
    {
        return true;
    }
    return false;
}

void ActivityListAdCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<10 || (CCCommonUtils::isIosAndroidPad() && ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) < 24))
    {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        if (m_actObj)
        {
            double nowTime = GlobalData::shared()->getWorldTime();
            ActivityListView* view = dynamic_cast<ActivityListView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(m_actObj->type==3){
                if(view!=NULL){
                    PopupViewController::getInstance()->addPopupView(AllianceActivityView::create(m_actObj));
                }
            }
            else if (m_actObj->startTime<nowTime && m_actObj->endTime>nowTime)
            {
                if(m_actObj->type==4){
                    if(view!=NULL){
                        PopupViewController::getInstance()->addPopupView(AllianceActBossView::create(m_actObj));
                    }
//                }else if(m_actObj->type==5){
                    
                }else if(m_actObj->type==7){
                    if(view!=NULL){
                        PopupViewController::getInstance()->addPopupView(CrossServiceActivityView::create(m_actObj));
                    }
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
            }else
            {
                if(m_actObj->type==7){
                    PopupViewController::getInstance()->addPopupView(CrossServiceActivityView::create(m_actObj));
                }else{
                    PopupViewController::getInstance()->addPopupView(ActivityTipView::create(m_actObj));
                }
            }
        }
    }
}

bool ActivityListAdCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_activityAdImg", CCSprite*, this->m_activityAdImg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel1", CCLabelIF*, this->m_timeLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTTF", CCLabelIF*, this->m_nameTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTTF", CCLabelIF*, this->m_infoTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    return false;
}

SEL_CCControlHandler ActivityListAdCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    return NULL;
}



