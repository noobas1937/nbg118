//
//  RechargePopupView.cpp
//  IF
//
//  Created by 李锐奇 on 15/1/20.
//
//

#include "RechargePopupView.h"
#include "PayController.h"

RechargePopupView *RechargePopupView::create(GoldExchangeItem *item, std::string uid){
    RechargePopupView *ret = new RechargePopupView(item, uid);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
void RechargePopupView::scrollViewDidScroll(CCScrollView* view){
    
}

void RechargePopupView::tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell){
    Recharge *wCell = dynamic_cast<Recharge*>(cell);
    if(wCell){
        std::string channel = wCell->getChannel();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        PayController::getInstance()->pay337(m_item->id, m_item->dollar, m_item->gold_doller, m_uid, channel);
#endif
    }
}
    
cocos2d::CCSize RechargePopupView::tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx){
    return CCSize(604, 159);
}
    
cocos2d::CCSize RechargePopupView::cellSizeForTable(  cocos2d::extension::TableView *table){
    return CCSize(604, 159);
}
    
CCTableViewCell* RechargePopupView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx){
    int count = areas.size();
    if(idx >= count){
        return NULL;
    }
    
    Recharge *cell = dynamic_cast<Recharge*>(table->dequeueCell());

    if(idx < count){
        if(cell == NULL){
            cell = Recharge::create();
        }

        auto it = areas.find(idx + 1);
        if (it != areas.end()) {
            auto str = it->second;
            std::vector<std::string> vector;
            CCCommonUtils::splitString(str, ",", vector);
            cell->setData(vector[0], vector[1]);
        }
    }
    return cell;
}

ssize_t RechargePopupView::numberOfCellsInTableView(  cocos2d::extension::TableView *table){
    int size = areas.size();
    return size;
}
    
void RechargePopupView::onEnter(){
    PopupBaseView::onEnter();
}

void RechargePopupView::onExit(){
    PopupBaseView::onExit();
}

bool RechargePopupView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(6, true);
        CCLoadSprite::doResourceByCommonIndex(400, true);

        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(6, false);
            CCLoadSprite::doResourceByCommonIndex(400, true);
        });

        auto bg = CCBLoadFile("RechargeViewCCB", this, this);
        this->setContentSize(bg->getContentSize());

        int preHeight = this->m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int dh = m_buildBG->getContentSize().height - preHeight;
        this->m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height +dh));
        this->m_bg1->setContentSize(CCSize(m_bg1->getContentSize().width, m_bg1->getContentSize().height +dh));
        this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));

        m_tabView = CCTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setDelegate(this);
        m_tabView->setTouchPriority(1);
        m_tabView->setPositionX(0);
        
        m_listContainer->addChild(m_tabView);

        ret = true;
    }
    return ret;
}

bool RechargePopupView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg1", CCScale9Sprite*, this->m_bg1);
    return false;
}

//-------

void Recharge::setData(std::string channel, std::string name){
    m_channel = channel;
    m_name = name;
    refreshView();
}
    
void Recharge::refreshView(){
    CCLoadSprite::doResourceByCommonIndex(400, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(400, true);
    });
    this->removeAllChildren();
    std::string iconPath = m_channel + ".png";
    float s = 1.2;
    auto sprite = CCLoadSprite::createSprite(iconPath.c_str());
    if(m_channel == "Ycoin"){
        sprite->setScaleX(0.48 * s);
        sprite->setScaleY(0.75 * s);
    }else{
        sprite->setScale(s);
    }
    sprite->setPosition(ccp(110, 80));
    this->addChild(sprite);
    auto label = CCLabelIFBMFont::create(m_name.c_str());
    label->setFntFile("pve_fnt_title.fnt");
    label->setAnchorPoint(ccp(0, 0.5));
    label->setAlignment(kCCTextAlignmentLeft);
    label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    this->addChild(label);
    label->setPosition(ccp(230, 80));
    
    auto line = CCLoadSprite::createScale9Sprite("mail_separate3.png");
    this->addChild(line);
    line->setPreferredSize(CCSize(640, 4));
    line->setPosition(320, 3);
    auto arrow = CCLoadSprite::createSprite("UI_world_Arrow.png");
    arrow->setPosition(ccp(580, 80));
    this->addChild(arrow);
}

bool Recharge::init(){
    this->setContentSize(CCSize(640, 159));
    return true;
}

std::string Recharge::getChannel(){
    return m_channel;
}