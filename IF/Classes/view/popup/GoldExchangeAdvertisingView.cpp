//
//  GoldExchangeAdvertisingView.cpp
//  IF
//
//  Created by lifangkai on 14-10-29.
//
//

#include "GoldExchangeAdvertisingView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "DataRecordCommand.h"
#include "ParticleController.h"
#include "ChangeNickNameView.h"
#include "RollingText.h"
#include "ChangePicView.h"
#include "ShieldUnlockPopUpView.h"
#include "SettingPopUpView.h"
#include "ToolController.h"
#include "YesNoDialog.h"
#include "ChangePicCommand.h"
#include "WorldController.h"
#include "RoleInfoView.h"
#include "GoldExchangeMoreListView.h"
#include "PaymentCommand.h"
#include "PayController.h"
#include "TipsView.h"
#include "GoldExchangeInfoCommand.h"
#include "RewardController.h"
#include "ButtonLightEffect.h"

#include "LuaController.h"

const int advertiseCellW = 640;//fusheng 540

GoldExchangeAdvertisingView* GoldExchangeAdvertisingView::create(){
    GoldExchangeAdvertisingView* ret = new GoldExchangeAdvertisingView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool GoldExchangeAdvertisingView::init()
{
    //begin a by ljf
    CCLoadSprite::doResourceByCommonIndex(800, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(800, false);
    });
    //end a by ljf

//    CCLoadSprite::doResourceByCommonIndex(100, true);
    m_data = CCArray::create();
    auto bg = CCBLoadFile("GoldAdvertisingView",this,this);
    setContentSize(bg->getContentSize());

    m_tabView = CCTableView::create(this, m_scrollContainer->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionHorizontal);//
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchMode(kCCTouchesOneByOne);
    m_tabView->setTouchPriority(-1);
    m_tabView->setDelegate(this);
     m_tabView->setTouchEnabled(false);
    m_getLabel->setString(_lang_1("115073",""));
    m_scrollContainer->addChild(m_tabView);

//    m_tabView->setTouchEnabled(false);
    
    //fusheng beign d
//    m_animNode = CCNode::create();
//    m_scrollContainer->getParent()->addChild(m_animNode);
//    m_animNode->setVisible(false);
    
    m_animNode = nullptr;
    //fusheng end

    refreshView(NULL);
    
    
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = [this](Touch* pTouch, Event* evt)
    {
        if(isTouchInside(m_touchLayer, pTouch)){
            this->unschedule(schedule_selector(GoldExchangeAdvertisingView::showButtonLight));
            this->unschedule(schedule_selector(GoldExchangeAdvertisingView::onPlayEnterFrame));
            this->schedule(schedule_selector(GoldExchangeAdvertisingView::onPlayEnterFrame),10.0);
            m_x = 1000;
            return true;
        }else{
            return false;
        }

    };
    
    listener->onTouchMoved = [this](Touch* pTouch, Event* evt)
    {
        if(isTouchInside(m_touchLayer, pTouch)){
            int dx = pTouch->getLocation().x - pTouch->getStartLocation().x;
            if(dx > 10 || dx < -10){
                addButtonLight(false);
                if(m_x == 1000){
                    m_x = m_tabView->getContentOffset().x;
                }
                int offX = m_x + dx;
                int min = m_tabView->minContainerOffset().x;
                int max = 200;
                if(offX > max){
                    offX = max;
                }
                if(offX < min){
                    offX = min;
                }
                m_tabView->setContentOffset(ccp(offX, 0), false);
            }
        }

        
    };

    listener->onTouchEnded = [this](Touch* pTouch, Event* evt)
    {
        int addX = -advertiseCellW / 2;
        int dx = pTouch->getStartLocation().x - pTouch->getLocation().x;
        if(dx > 20){
            addX = -advertiseCellW;
        }else if(dx < -20){
            addX = 0;
        }
        int offSetX = int((m_tabView->getContentOffset().x + addX) / advertiseCellW) * advertiseCellW;
        //    int minX  = m_tabView->minContainerOffset().x+20;
        int minX  = m_tabView->minContainerOffset().x;
        
        if(offSetX > 0){
            offSetX = 0;
        }
        if(offSetX < minX){
            offSetX = minX;
        }
        m_tabView->setContentOffset(ccp(offSetX, 0), true);
        setButtonState(offSetX);
        this->scheduleOnce(schedule_selector(GoldExchangeAdvertisingView::showButtonLight), 0.15);
        
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_touchLayer);
    
    
    
    return true;
}

void GoldExchangeAdvertisingView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeAdvertisingView::refreshView), PAYMENT_COMMAND_RETURN, NULL);
    this->schedule(schedule_selector(GoldExchangeAdvertisingView::onPlayEnterFrame),10.0);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeAdvertisingView::recoverScroll), GOLDEXCHANGE_ADVERTISING_SCROLL, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeAdvertisingView::stopScroll), GOLDEXCHANGE_ADVERTISING_STOP_SCROLL, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeAdvertisingView::errorPayment), MSG_PAY_FAILED, NULL);
}
void GoldExchangeAdvertisingView::errorPayment(cocos2d::CCObject *obj){
    this->schedule(schedule_selector(GoldExchangeAdvertisingView::onShowError),0.3,1,0);
}
void GoldExchangeAdvertisingView::onShowError(float dt){
    PayController::getInstance()->showErrorDialog();
    this->unschedule(schedule_selector(GoldExchangeAdvertisingView::onShowError));
}
void GoldExchangeAdvertisingView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PAYMENT_COMMAND_RETURN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GOLDEXCHANGE_ADVERTISING_STOP_SCROLL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GOLDEXCHANGE_ADVERTISING_SCROLL);
    this->unschedule(schedule_selector(GoldExchangeAdvertisingView::onPlayEnterFrame));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PAY_FAILED);
    CCNode::onExit();
}
void GoldExchangeAdvertisingView::onPlayEnterFrame(float dt){
    addButtonLight(false);
    int addX = -advertiseCellW;
    int offSetX = int((m_tabView->getContentOffset().x + addX) / advertiseCellW) * advertiseCellW;
//    int minX  = m_tabView->minContainerOffset().x+20;//fusheng 11.24
    int minX  = m_tabView->minContainerOffset().x;
    if(offSetX > 0){
        offSetX = 0;
    }else
        if(offSetX < minX){
            offSetX = minX;
        }
//    m_tabView->setContentOffset(ccp(offSetX, 0), true);
    m_tabView->setContentOffsetInDuration(ccp(offSetX, 0), 0.5);
    this->scheduleOnce(schedule_selector(GoldExchangeAdvertisingView::showButtonLight), 0.5);
    setButtonState(offSetX,false);
}

void GoldExchangeAdvertisingView::generateData(){
    m_data->removeAllObjects();
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        if(!it->second)
            continue;
        if(it->second->type == "5"){
            continue;
        }
        if(it->second->bought||(it->second->end<GlobalData::shared()->getWorldTime())){
            continue;
        }
        if(it->second->type == "3"&&it->second->popup_image!="close"){
            m_data->addObject(CCString::create(it->second->id));
        }
    }
    //排序
    int count = m_data->count();
    int i=0;
    while (i<count) {
        int j=0;
        string itemId1 = dynamic_cast<CCString*>(m_data->objectAtIndex(i))->getCString();
        map<string, GoldExchangeItem*>::iterator it1 = GlobalData::shared()->goldExchangeList.find(itemId1);
        while (j<i) {
            string itemId2 = dynamic_cast<CCString*>(m_data->objectAtIndex(j))->getCString();
            map<string, GoldExchangeItem*>::iterator it2 = GlobalData::shared()->goldExchangeList.find(itemId2);
            if(it1->second->popup>it2->second->popup){
                m_data->swap(i, j);
            }
            j++;
        }
        i++;
    }
    count = m_data->count();
    if(count>1){
        //将第一个插入到结尾
        string endItemId = dynamic_cast<CCString*>(m_data->objectAtIndex(0))->getCString();
        m_data->addObject(CCString::create(endItemId));
        
        //将最后一个插入到开始
        string startItemId = dynamic_cast<CCString*>(m_data->objectAtIndex(count-1))->getCString();
        m_data->insertObject(CCString::create(startItemId), 0);
    }
}
void GoldExchangeAdvertisingView::recoverScroll(CCObject* obj){
    this->unschedule(schedule_selector(GoldExchangeAdvertisingView::onPlayEnterFrame));
    this->schedule(schedule_selector(GoldExchangeAdvertisingView::onPlayEnterFrame),10.0);

}
void GoldExchangeAdvertisingView::stopScroll(CCObject* obj){
    this->unschedule(schedule_selector(GoldExchangeAdvertisingView::onPlayEnterFrame));
    
}
void GoldExchangeAdvertisingView::generalDataAgain(float _time){
//    m_tabView->setContentOffsetInDuration(ccp(0, 0), 0.20);
    if(m_data->count()>1){
        addButtonLight(false);
        m_tabView->setContentOffset(ccp(-advertiseCellW, 0));
        addButtonLight(true);
    }
}
void GoldExchangeAdvertisingView::generalEndDataAgain(float _time){
//    m_tabView->scrollToCellIndex(m_data->count()-2);
    addButtonLight(false);
    int dx =-(m_data->count()-2)*advertiseCellW;
    m_tabView->setContentOffset(ccp(dx,0));
    addButtonLight(true);
}
void GoldExchangeAdvertisingView::refreshView(CCObject* obj){
    addButtonLight(false);
    generateData();
    bool ishasChild = false;
    CCPoint pos;
    if(m_tabView->getContainer()->getChildrenCount()==0){
        ishasChild = true;
    }else{
        pos = m_tabView->getContentOffset();
    }
    m_tabView->reloadData();
    int count = (m_data->count()>0)? m_data->count():1;
    if(!ishasChild){
        CCPoint minPt = m_tabView->minContainerOffset();
        CCPoint maxPt = m_tabView->maxContainerOffset();
        if (pos.x > maxPt.x) {
            pos.x = maxPt.x;
        }
        if (pos.x < minPt.x) {
            pos.x = minPt.x;
        }
        m_tabView->setContentOffset(pos);
    }else if (count>1){
        m_tabView->setContentOffset(ccp(-advertiseCellW, 0));
    }
    m_pageNode->removeAllChildren();
    int width = 30;

    if(count>1){
        count-=2;
        
    }
    m_startX= 0;
    if(count%2==1){
        m_startX = -int(count/2)*width;
    }else{
        m_startX = -int(count/2)*width+width/2;
    }
    for (int i=0; i<count; i++) {
        auto spr = CCLoadSprite::createSprite("Recharge_fanye.png");
        m_pageNode->addChild(spr);
        spr->setPositionX(m_startX+width*i);
    }
    m_pageflag= CCLoadSprite::createSprite("Recharge_fanye2.png");
    m_pageNode->addChild(m_pageflag);
    m_pageflag->setPositionX(0);
    
    setButtonState(m_tabView->getContentOffset().x,false);
    addButtonLight(true);
}


void GoldExchangeAdvertisingView::onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_dataItem==NULL){
        return;
    }
    PayController::getInstance()->callPayment(m_dataItem);
}

SEL_CCControlHandler GoldExchangeAdvertisingView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCostBtn", GoldExchangeAdvertisingView::onClickCostBtn);
    
    return NULL;
}

void GoldExchangeAdvertisingView::setButtonState(int offX,bool isreset){

    if(m_data->count()<=1){
        this->unschedule(schedule_selector(GoldExchangeAdvertisingView::onPlayEnterFrame));
    }
    if(m_data->count()<=0){
        m_titleLabel->setString(_lang("101242"));
        m_bgNode->setVisible(true);
//        auto spr = CCLoadSprite::createScale9Sprite("Recharge_bg.png");
//        spr->setContentSize(CCSize(520, 300));
//        spr->setInsetLeft(1.0);
//        spr->setInsetBottom(1.0);
//        spr->setInsetRight(1.0);
//        spr->setInsetTop(1.0);
//        m_bgNode->addChild(spr);
    }else{
        m_bgNode->setVisible(false);
    }

    int index = round(abs(offX) / advertiseCellW);
    m_dataItem = NULL;
    if(index < 0 || index >= m_data->count()){
        return;
    }else{
        std::string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(index))->getCString();
        map<string, GoldExchangeItem*>::iterator it =GlobalData::shared()->goldExchangeList.find(itemId.c_str());
        GoldExchangeItem* item = NULL;
        if ( it != GlobalData::shared()->goldExchangeList.end()){
            item = it->second;
            m_dataItem = item;
            m_titleNode->removeAllChildren();
            m_titleLabel->setString("");
            if(item->type == "5"){
                m_titleLabel->setString(_lang("101281"));
            }else{
                if(item->popup_image=="thanksgiving"){
//                string boxName =  "Recharge_biaoti.png";
//                CCSprite* sprite = CCLoadSprite::createSprite(boxName.c_str());
//                sprite->setScale(0.6);
//                m_titleNode->addChild(sprite);
                    m_titleLabel->setString(_lang(item->name).c_str());
                }else if(item->popup_image == "winter") {
                    m_titleLabel->setString(_lang(item->name).c_str());
                } else {
                    m_titleLabel->setString(_lang(item->name).c_str());
                }
            }
            string dollar = PayController::getInstance()->getDollarText(item->dollar,item->product_id);
            this->m_newPriceLabel->setString(dollar.c_str());
        }
//        if(index==0){
//            m_pageflag->setPositionX(m_startX+(m_data->count()-1-2)*30);
//            this->scheduleOnce(schedule_selector(GoldExchangeAdvertisingView::generalEndDataAgain),0.6);
//        }else
        if(m_data->count()<=1){
            return;
        }
        float delayTime = 0;
        if(isreset){
            delayTime = 0.2;
        }else{
            delayTime = 0.6;
        }
        if(index==m_data->count()-1){
            m_pageflag->setPositionX(m_startX);
            this->scheduleOnce(schedule_selector(GoldExchangeAdvertisingView::generalDataAgain),delayTime);
            
        }else if(index==0){
            m_pageflag->setPositionX(m_startX+30*(m_data->count()-2-1));
            this->scheduleOnce(schedule_selector(GoldExchangeAdvertisingView::generalEndDataAgain),delayTime);
        }else{
            m_pageflag->setPositionX(m_startX+30*(index-1));
        }
        
        
    }
}

bool GoldExchangeAdvertisingView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getLabel", CCLabelIF*, this->m_getLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newPriceLabel", CCLabelIF*, this->m_newPriceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollContainer", CCNode*, this->m_scrollContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNode", CCNode*, this->m_titleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pageNode", CCNode*, this->m_pageNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costBtn", CCControlButton*, this->m_costBtn);
    
    
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchLayer", CCLayerColor*, this->m_touchLayer);
    
    
    return false;
}

bool GoldExchangeAdvertisingView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    if(isTouchInside(m_scrollContainer, pTouch)){
//    if(isTouchInside(m_touchLayer, pTouch)){
//        this->unschedule(schedule_selector(GoldExchangeAdvertisingView::showButtonLight));
//        this->unschedule(schedule_selector(GoldExchangeAdvertisingView::onPlayEnterFrame));
//        this->schedule(schedule_selector(GoldExchangeAdvertisingView::onPlayEnterFrame),10.0);
//        m_x = 1000;
//        return true;
//    }else{
//        return false;
//    }
    return false;
    
}

void GoldExchangeAdvertisingView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
//    if(isTouchInside(m_scrollContainer, pTouch)){
    
}

void GoldExchangeAdvertisingView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

    
}

CCSize GoldExchangeAdvertisingView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    return CCSize(advertiseCellW, 245);
}

CCSize GoldExchangeAdvertisingView::cellSizeForTable(CCTableView *table)
{
    return CCSize(advertiseCellW, 245);
}

CCTableViewCell* GoldExchangeAdvertisingView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    map<string, GoldExchangeItem*>::iterator it =GlobalData::shared()->goldExchangeList.find(itemId.c_str());
    GoldExchangeItem* item = NULL;
    if ( it != GlobalData::shared()->goldExchangeList.end()){
         item = it->second;
    }
    GoldExchangeAdvertisingCell* cell = (GoldExchangeAdvertisingCell*)table->dequeueCell();
    
    if(cell){
        cell->setData(item);
    }else{
        cell = GoldExchangeAdvertisingCell::create(item);
    }
    return cell;
}

ssize_t GoldExchangeAdvertisingView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1.0f);
    return num;
}

void GoldExchangeAdvertisingView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}

void GoldExchangeAdvertisingView::scrollViewDidScroll(CCScrollView* view){
    CCLOG("%f", view->getContentOffset().x);
}
void GoldExchangeAdvertisingView::addButtonLight(bool bAdd){
    if(m_animNode){
        m_animNode->removeAllChildren();
        if(bAdd){
            CCPoint offset = m_tabView->getContentOffset();
            int index = ceil(-offset.x /advertiseCellW);
//            GoldExchangeAdvertisingCell *cell = dynamic_cast<GoldExchangeAdvertisingCell*>(tableCellAtIndex(m_tabView,index));
            GoldExchangeAdvertisingCell *cell = NULL;
            if(m_tabView && m_tabView->getContainer()){
                auto arr = m_tabView->getContainer()->getChildren();
                if( arr.size() > 0){
                    cell = dynamic_cast<GoldExchangeAdvertisingCell*>(arr.at(0));
                }
            }
            if(cell){
                CCRect rect = cell->getCostBtnRect();
                if(rect.equals(CCRectZero))
                    return;
                CCPoint pos = m_scrollContainer->getPosition() + rect.origin;
//                pos = m_animNode->getParent()->convertToNodeSpace(pos);
                m_animNode->setContentSize(rect.size);
                m_animNode->setPosition(pos);
                ButtonLightEffect* btnEffect = ButtonLightEffect::create(rect.size);
                m_animNode->addChild(btnEffect);
                btnEffect->setPosition(ccp(0, -10));
//                if (CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
//                {
//                    btnEffect->setPosition(ccp(80, -10));
//                }
                m_animNode->setVisible(true);
            }
        }
    }
}
void GoldExchangeAdvertisingView::showButtonLight(float _time){
    addButtonLight(true);
}

GoldExchangeAdvertisingCell *GoldExchangeAdvertisingCell::create(GoldExchangeItem* item){
    GoldExchangeAdvertisingCell *ret = new GoldExchangeAdvertisingCell(item);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void GoldExchangeAdvertisingCell::setData(GoldExchangeItem* item){
    this->removeAllChildren();
    m_item = item;
    m_costBtnRect = CCRectZero;
    if(m_item->type == "5"){
        
    }else{
        if(m_item->popup_image=="thanksgiving" || m_item->popup_image=="winter" || m_item->popup_image=="christmas" || m_item->popup_image=="newyear" || m_item->popup_image == "silent" || m_item->popup_image == "throne" || m_item->popup_image == "equip" || m_item->popup_image =="lover"||m_item->popup_image== "chunjie"){
            auto cell = GoldExchangeHallweenCell::create(m_item);
            this->addChild(cell);
            cell->setPositionY(-66);
            m_costBtnRect = cell->getCostBtnRect();
        }else{
            if(LuaController::getInstance()->checkLuaValid(m_item->popup_image,m_item->md5,m_item->type)){
                std::string info = m_item->toString();
                m_costBtnRect = LuaController::getInstance()->createExchangeAdvCell(this,info);
            }else{
                auto cell = GoldExchangeAdvertisingCommCell::create(m_item);
                this->addChild(cell);
//                cell->setPositionY(-66);
                m_costBtnRect = cell->getCostBtnRect();
            }
        }
    }
}
void GoldExchangeAdvertisingCell::refreshView(){

    
}

bool GoldExchangeAdvertisingCell::init(){
    this->setContentSize(CCSize(640, 250));//fusheng 540
//    setCleanFunction([](){
//        CCLoadSprite::doResourceByCommonIndex(100, false);
//        CCLoadSprite::doResourceByGeneralIndex(1, false);
//        CCLoadSprite::doResourceByGeneralIndex(2, false);
//        CCLoadSprite::doResourceByGeneralIndex(3, false);
//    });
    
    setData(m_item);
    return true;
}

void GoldExchangeAdvertisingCell::onEnter(){
    CCTableViewCell::onEnter();
}

void GoldExchangeAdvertisingCell::onExit(){
    CCTableViewCell::onExit();
}
//......//


GoldExchangeHallweenCell* GoldExchangeHallweenCell::create(GoldExchangeItem *goldExchangeItem){
    GoldExchangeHallweenCell* ret = new GoldExchangeHallweenCell(goldExchangeItem);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldExchangeHallweenCell::init()
{
    if(m_dataItem->popup_image == "thanksgiving"){
        CCBLoadFile("GoldAdvertisingThanksGivingCell",this,this,false);
    } else if (m_dataItem->popup_image == "winter") {
        CCBLoadFile("GoldAdvertisingWintersCell", this, this, false);
    }else if(m_dataItem->popup_image == "christmas"){
        CCBLoadFile("GoldAdvertisingChristmassCell", this, this, false);
    }else if(m_dataItem->popup_image == "newyear"){
        CCBLoadFile("GoldAdvertisingNewYearCell", this, this, false);
    }else if(m_dataItem->popup_image == "silent"){
        CCBLoadFile("GoldAdvertisingChristmasEveCell", this, this, false);
    }else if(m_dataItem->popup_image == "throne" || m_dataItem->popup_image == "equip"||  m_dataItem->popup_image =="lover"|| m_dataItem->popup_image == "chunjie"){
        std::string tmp = m_dataItem->popup_image;
        char aa = tmp.at(0);
        aa = std::toupper(aa);
        std::string subStr = aa + tmp.substr(1);
        subStr = "GoldAdvertising"+subStr+"Cell";
        CCBLoadFile(subStr.c_str(), this, this, false);
    }
    this->setContentSize(CCSizeMake(640, 245));
    //    this->m_title->setString(_lang("102148").c_str());
    string dollar = PayController::getInstance()->getDollarText(m_dataItem->dollar,m_dataItem->product_id);
//    m_animNode = CCNode::create();
//    m_costBtn->getParent()->addChild(m_animNode);
//    m_animNode->setPosition(m_costBtn->getPosition());
//    CCSize btnSize = m_costBtn->getContentSize();
//    ButtonLightEffect* btnEffect = ButtonLightEffect::create(btnSize);
//    m_animNode->addChild(btnEffect);
//    CCSprite* Spr = CCSprite::create();
//    m_animNode->addChild(Spr);
//    string curPath = "ButtonLight_%d.png";
//    CCCommonUtils::makeInternalEffectSpr(Spr, curPath, 12, 0.07,0,2.0);
//    Spr->setAnchorPoint(ccp(0.5, 0.5));
//    CCSize size = m_costBtn->getContentSize();
//    CCSize size1 = Spr->getContentSize();
//    float scalex = m_costBtn->getContentSize().width/Spr->getContentSize().width;
//    float scaley = 1.0;
//    if(m_dataItem->popup_image=="thanksgiving" || m_dataItem->popup_image == "winter"){
//        scaley = (m_costBtn->getContentSize().height)/Spr->getContentSize().height;
//        Spr->setPositionY(0);
//    }else{
//        scaley =(m_costBtn->getContentSize().height+10)/Spr->getContentSize().height;
//        Spr->setPositionY(3);
//    }
//    Spr->setScaleX(scalex);
//    Spr->setScaleY(scaley);
//    CCSize size2 = Spr->getContentSize();
//    Spr->setOpacity(175);
    refreshData();
    return true;
}
void GoldExchangeHallweenCell::refreshData(){
    m_getGoldNumText->setString(CC_CMDITOA(atoi(m_dataItem->gold_doller.c_str())).c_str());
    string perStr = m_dataItem->percent+"%";
    m_percentLabel->setString(perStr);
    m_desLabel->setString(_lang("101237"));
    if(m_dataItem->item!=""){
        m_moreLabel->setString(_lang("102162"));
    }else{
        m_moreLabel->setString("");
    }

    m_getLabel->setString(_lang_1("115073",""));
    string dollar = PayController::getInstance()->getDollarText(m_dataItem->dollar,m_dataItem->product_id);
    this->m_newPriceLabel->setString(dollar.c_str());
    if(m_btnGivePackage){
        m_btnGivePackage->setVisible(m_dataItem->send_mail);
    }
    onEnterFrame(0.0);

}
void GoldExchangeHallweenCell::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GoldExchangeHallweenCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);

}

void GoldExchangeHallweenCell::onExit()
{

    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GoldExchangeHallweenCell::onEnterFrame), this);
    CCNode::onExit();
}

SEL_CCControlHandler GoldExchangeHallweenCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCostBtn", GoldExchangeHallweenCell::onClickCostBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickGivePackageBtn", GoldExchangeHallweenCell::onClickGivePackageBtn);
    return NULL;
}
void GoldExchangeHallweenCell::onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_dataItem==NULL){
        return;
    }
    if(m_dataItem->type=="3"&&m_dataItem->isPayCallBack){
        return;
    }
    PayController::getInstance()->callPayment(m_dataItem);
}
void GoldExchangeHallweenCell::onClickGivePackageBtn(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    if(m_dataItem==NULL){
        return;
    }
    LuaController::toSelectUser(m_dataItem->id, true,2);
}
bool GoldExchangeHallweenCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getGoldNumText", CCLabelIF*, this->m_getGoldNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_percentLabel", CCLabelIF*, this->m_percentLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreLabel", CCLabelIF*, this->m_moreLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getLabel", CCLabelIF*, this->m_getLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newPriceLabel", CCLabelIF*, this->m_newPriceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchMoreNode1", CCNode*, this->m_touchMoreNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchMoreNode2", CCNode*, this->m_touchMoreNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costBtn", CCControlButton*, this->m_costBtn);
  CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnGivePackage", CCControlButton*, this->m_btnGivePackage);
    return false;
}
void GoldExchangeHallweenCell::onEnterFrame(float dt)
{
    int curTime = GlobalData::shared()->getWorldTime();
    int lastTime = 0;
    if (m_dataItem->time>0) {
        int gapTime =(m_dataItem->end - curTime);
        int count =gapTime/(m_dataItem->time*3600);
        lastTime = m_dataItem->end - (m_dataItem->time*3600)*count-curTime;
    }else{
        lastTime = m_dataItem->end - curTime;
    }
    
    m_timeLabel->setString(CC_SECTOA(lastTime));
    if (m_dataItem->end - curTime<=0) {
        //        ToolController::getInstance()->m_lastTime += 3600*24;
        //        ToolController::getInstance()->m_canBuy = 1;
        PopupViewController::getInstance()->removeAllPopupView();
    }
}
bool GoldExchangeHallweenCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_touchLocation = pTouch->getLocation();
    if(isTouchInside(this->m_touchMoreNode1, pTouch)||isTouchInside(this->m_touchMoreNode2, pTouch) ) {
        return true;
    }
    return false;
}
void GoldExchangeHallweenCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (fabs(m_touchLocation.y - pTouch->getLocation().y) > 10 || fabs(m_touchLocation.x - pTouch->getLocation().x) > 10) {
        return;
    }
    if((isTouchInside(m_touchMoreNode1, pTouch)||isTouchInside(m_touchMoreNode2, pTouch))&&(m_dataItem->item != "")){
        PopupViewController::getInstance()->addPopupView(GoldExchangeMoreListView::create(atoi(m_dataItem->id.c_str())));
         CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_ADVERTISING_STOP_SCROLL);
    }
}
void GoldExchangeHallweenCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}
CCRect GoldExchangeHallweenCell::getCostBtnRect(){
    CCSize size = m_costBtn->getContentSize();
    CCPoint pos = m_costBtn->getPosition();
    pos = m_costBtn->getParent()->convertToWorldSpace(pos);
    pos = this->convertToNodeSpace(pos);
    return CCRect(pos.x, pos.y, size.width, size.height);
}
//GoldExchangeAdvertisingCommCell class//


GoldExchangeAdvertisingCommCell* GoldExchangeAdvertisingCommCell::create(GoldExchangeItem *goldExchangeItem){
    GoldExchangeAdvertisingCommCell* ret = new GoldExchangeAdvertisingCommCell(goldExchangeItem);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldExchangeAdvertisingCommCell::init()
{
   
    CCBLoadFile("GoldAdvertisingCommonCell",this,this,false);
    this->setContentSize(CCSizeMake(640, 250));
    //    this->m_title->setString(_lang("102148").c_str());
    string dollar = PayController::getInstance()->getDollarText(m_dataItem->dollar,m_dataItem->product_id);
    //    this->m_dollerNum->setString(dollar.c_str());
    //    this->m_goldNum->setString(CC_CMDITOA(atoi(m_dataItem->gold_doller.c_str())).c_str());
    //this->m_goldNum->setFntFile("pve_fnt_title.fnt");
    
//    m_animNode = CCNode::create();
//    m_costBtn->getParent()->addChild(m_animNode);
//    m_animNode->setPosition(m_costBtn->getPosition());
    
//    CCSize btnSize = m_costBtn->getContentSize();
//    ButtonLightEffect* btnEffect = ButtonLightEffect::create(btnSize);
//    m_animNode->addChild(btnEffect);
    
//    CCSprite* Spr = CCSprite::create();
//    m_animNode->addChild(Spr);
//    string curPath = "ButtonLight_%d.png";
//    CCCommonUtils::makeInternalEffectSpr(Spr, curPath, 12, 0.07,0,2.0);
//    //Spr->setContentSize(CCSizeMake(m_costBtn->getContentSize().width, 10));
//    // Spr->setPreferredSize(CCSizeMake(m_costBtn->getContentSize().width, m_costBtn->getContentSize().height));
//    Spr->setAnchorPoint(ccp(0.5, 0.5));
//    CCSize size = m_costBtn->getContentSize();
//    CCSize size1 = Spr->getContentSize();
//    float scalex = m_costBtn->getContentSize().width/Spr->getContentSize().width;
//    float scaley = 1.0;
//    if(m_dataItem->popup_image=="30"){
//        scaley = (m_costBtn->getContentSize().height)/Spr->getContentSize().height;
//        Spr->setPositionY(0);
//    }else{
//        scaley =(m_costBtn->getContentSize().height+10)/Spr->getContentSize().height;
//        Spr->setPositionY(3);
//    }
//    Spr->setScaleX(scalex);
//    Spr->setScaleY(scaley);
//    CCSize size2 = Spr->getContentSize();
//    Spr->setOpacity(175);
    
    int id = atoi(m_dataItem->id.c_str()) - 8999;
    string strID = CC_ITOA(id);
    string boxName = "gift00" + strID + ".png";
    CCSprite* sprite = CCLoadSprite::createSprite(boxName.c_str());
    //    this->m_goldIconNode->addChild(sprite);
    
    if (m_dataItem->gift != "") {
        //        auto xmlLocal = LocalController::shared()->DBXMLManager()->getObjectByKey(m_dataItem->gift);
        //        this->m_itemDec->setString(_lang(xmlLocal->valueForKey("description")->getCString()));
    }
    else
    {
        //        this->m_itemDec->setString("");
    }
    
    m_getLabel->setVisible(false);
    m_newPriceLabel->setPositionX(m_costBtn->getPositionX());
    //begin a by ljf
    m_showMoreNodeOrignPos.x = m_showMoneyNode->getPositionX();
    m_showMoreNodeOrignPos.y = m_showMoneyNode->getPositionY();
    //end a by ljf
    refreshData();
    
    return true;
}
void GoldExchangeAdvertisingCommCell::refreshData(){
    string str = CC_CMDITOA(atoi(m_dataItem->gold_doller.c_str())).c_str();//fusheng
    if(str.length()!=0)
    {
        str = "+"+str;
        m_getGoldNumText->setString(str);
    }
    string perStr = m_dataItem->percent;
    m_percentLabel->setString(perStr);
    m_percentLabel2->setString("%");
    m_desLabel->setString(_lang("101237"));
    if(m_dataItem->item!=""){
        m_moreLabel->setString(_lang("102162"));
        m_showMoreNode->setVisible(true);
//        m_showMoneyNode->setPositionY(0);//fusheng 11.25 d
//        m_desLabel->setPositionY(0.2);

    }else{
        m_moreLabel->setString("");
        m_showMoreNode->setVisible(false);
//        m_showMoneyNode->setPositionY(-20);//fusheng 11.25 d
//        m_desLabel->setPositionY(-50);

    }
//    m_getLabel->setString(_lang_1("115073",""));
    string dollar = PayController::getInstance()->getDollarText(m_dataItem->dollar,m_dataItem->product_id);
    this->m_newPriceLabel->setString(dollar.c_str());
    m_btnGivePackage->setVisible(m_dataItem->send_mail);
    
    //begin ljf
    string spriteName = "nb_libaoBG.png";
    Vec2 addPos(0, 0);
    if(m_dataItem->popup_image == "Build")
    {
        addPos.x = 150;
        spriteName = "abox_citybuild_ad.png";
    }
    if(m_dataItem->popup_image == "Newbie_Hot")
    {
        addPos.x = 150;
        spriteName = "abox_newuser_ad.png";
    }
    m_showMoneyNode->setPosition(m_showMoreNodeOrignPos + addPos);
    CCSpriteFrame* newSp = CCLoadSprite::getSF(spriteName.c_str());
    m_liebiaoBg->setDisplayFrame(newSp);
    //end ljf
    onEnterFrame(0.0);
    
}
CCRect GoldExchangeAdvertisingCommCell::getCostBtnRect(){
    CCSize size = m_costBtn->getContentSize();
    CCPoint pos = m_costBtn->getPosition();
    pos = m_costBtn->getParent()->convertToWorldSpace(pos);
    pos = this->convertToNodeSpace(pos);
    return CCRect(pos.x, pos.y, size.width, size.height);
}
void GoldExchangeAdvertisingCommCell::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GoldExchangeAdvertisingCommCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    
}

void GoldExchangeAdvertisingCommCell::onExit()
{
    
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GoldExchangeAdvertisingCommCell::onEnterFrame), this);
    CCNode::onExit();
}

SEL_CCControlHandler GoldExchangeAdvertisingCommCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCostBtn", GoldExchangeAdvertisingCommCell::onClickCostBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickGivePackageBtn", GoldExchangeAdvertisingCommCell::onClickGivePackageBtn);
    return NULL;
}

bool GoldExchangeAdvertisingCommCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getGoldNumText", CCLabelBMFont*, this->m_getGoldNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_percentLabel", CCLabelIF*, this->m_percentLabel);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_percentLabel2", CCLabelIF*, this->m_percentLabel2);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreLabel", CCLabelIF*, this->m_moreLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getLabel", CCLabelIF*, this->m_getLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newPriceLabel", CCLabelIF*, this->m_newPriceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchMoreNode1", CCNode*, this->m_touchMoreNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchMoreNode2", CCNode*, this->m_touchMoreNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showMoreNode", CCNode*, this->m_showMoreNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costBtn", CCControlButton*, this->m_costBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showMoneyNode", CCNode*, this->m_showMoneyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnGivePackage", CCControlButton*, this->m_btnGivePackage);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_liebiaoBg", CCSprite*, this->m_liebiaoBg);
    
    return false;
}
void GoldExchangeAdvertisingCommCell::onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_dataItem==NULL){
        return;
    }
    if(m_dataItem->type=="3"&&m_dataItem->isPayCallBack){
        return;
    }
    PayController::getInstance()->callPayment(m_dataItem);
}
void GoldExchangeAdvertisingCommCell::onClickGivePackageBtn(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    if(m_dataItem==NULL){
        return;
    }
    LuaController::toSelectUser(m_dataItem->id, true,2);
}
void GoldExchangeAdvertisingCommCell::onEnterFrame(float dt)
{
    int curTime = GlobalData::shared()->getWorldTime();
    int lastTime = 0;
    if (m_dataItem->time>0) {
        int gapTime =(m_dataItem->end - curTime);
        int count =gapTime/(m_dataItem->time*3600);
        lastTime = m_dataItem->end - (m_dataItem->time*3600)*count-curTime;
    }else{
        lastTime = m_dataItem->end - curTime;
    }
    
    m_timeLabel->setString(CC_SECTOA(lastTime));
    if (m_dataItem->end - curTime<=0) {
        PopupViewController::getInstance()->removeAllPopupView();
    }
}
bool GoldExchangeAdvertisingCommCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_touchLocation = pTouch->getLocation();
    if(isTouchInside(this->m_touchMoreNode1, pTouch)||isTouchInside(this->m_touchMoreNode2, pTouch) ) {
        return true;
    }
    return false;
}
void GoldExchangeAdvertisingCommCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (fabs(m_touchLocation.y - pTouch->getLocation().y) > 10 || fabs(m_touchLocation.x - pTouch->getLocation().x) > 10) {
        return;
    }
    if((isTouchInside(m_touchMoreNode1, pTouch)||isTouchInside(m_touchMoreNode2, pTouch))&&(m_dataItem->item != "")){
        PopupViewController::getInstance()->addPopupView(GoldExchangeMoreListView::create(atoi(m_dataItem->id.c_str())));
         CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_ADVERTISING_STOP_SCROLL);
        
    }
}
void GoldExchangeAdvertisingCommCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}