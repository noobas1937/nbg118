//
//  UseItemStatusView.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-18.
//
//

#include "UseItemStatusView.h"
#include "PopupViewController.h"
#include "ToolController.h"
#include "YesNoDialog.h"
#include "WorldController.h"
#include "ParticleController.h"
#include "UIComponent.h"

UseItemStatusView* UseItemStatusView::create(int type,std::string title, std::string description){
    UseItemStatusView* ret = new UseItemStatusView(type,title, description);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool UseItemStatusView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(504, true);
    auto tmpCCB = CCBLoadFile("UseItemStatusView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    this->setTitleName(m_title.c_str());
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_description+=" ";//下面的setString在英文中有可能自动删除最后一个字符原因不明
        m_descriptionText->setString(m_description);
        int textH = m_descriptionText->getContentSize().height * m_descriptionText->getOriginScaleY();
        m_descriptionText->setPositionY(m_descriptionText->getPositionY() - textH / 2);
        
        m_viewBg2->setPositionY(m_descriptionText->getPositionY() - textH / 2 - 10);//350
        m_infoList->setPositionY(m_viewBg2->getPositionY() - 10);

        int listHeight = m_infoList->getPositionY() + 1024;
        m_infoList->setContentSize(CCSizeMake(m_infoList->getContentSize().width, listHeight));
        m_scrollView = CCScrollView::create(m_infoList->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_infoList->addChild(m_scrollView);
        m_listH = m_infoList->getContentSize().height;
        
        CCArray* array =ToolController::getInstance()->m_typeItems[m_type];
        int num = 0;
        int count = 0;
        int index = 0;
        if(array && array->count()>0){
            num = array->count();
            count = num;
            for (int i=0; i<num; i++) {
                CCDictionary* dictInfo = _dict(array->objectAtIndex(i));
                if(dictInfo->objectForKey("customskin")){
                    UseItemStatusCell* cell = UseItemStatusCell::create(0,"customskin",0);
                    cell->setAnchorPoint(ccp(0.5, 0.5));
                    cell->setPosition(ccp(0,index * 300));
                    m_scrollView->addChild(cell);
                }else{
                    int itemId = dictInfo->valueForKey("id")->intValue();
                    auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
                    if (toolInfo.price<=0 && toolInfo.getCNT()<=0) {
                        count--;
                        continue;
                    }
                    UseItemStatusCell* cell = UseItemStatusCell::create(itemId,"",0);
                    cell->setAnchorPoint(ccp(0.5, 0.5));
                    cell->setPosition(ccp(0,index * 300));
                    m_scrollView->addChild(cell);
                }
                ++index;
            }
        }
        
        int contentHeight = count*300;
        m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,contentHeight));
        m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - contentHeight));
        
        m_scrollH = contentHeight;
    }
    else {
        int oldBgHeight = m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        int newBgHeight = m_viewBg->getContentSize().height;
        int addHeight = newBgHeight - oldBgHeight;
        int oldWidth = m_infoList->getContentSize().width;
        int oldHeight = m_infoList->getContentSize().height;
        
        m_description+=" ";//下面的setString在英文中有可能自动删除最后一个字符原因不明
        m_descriptionText->setString(m_description);
        int textH = m_descriptionText->getContentSize().height * m_descriptionText->getOriginScaleY();
        m_descriptionText->setPositionY(m_descriptionText->getPositionY() - textH / 2);
        m_infoList->setPositionY(m_infoList->getPositionY()-addHeight-20);//
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight + addHeight - textH));
        
        m_viewBg2_py=m_infoList->getPositionY()+m_infoList->getContentSize().height+20;
        m_viewBg2->setPositionY(m_viewBg2_py);//350
        
        m_scrollView = CCScrollView::create(m_infoList->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_infoList->addChild(m_scrollView);
        
        m_listH = m_infoList->getContentSize().height;
        
        CCArray* array =ToolController::getInstance()->m_typeItems[m_type];
        int num = 0;
        int count = 0;
        int index = 0;
        if(array && array->count()>0){
            num = array->count();
            count = num;
            for (int i=0; i<num; i++) {
                CCDictionary* dictInfo = _dict(array->objectAtIndex(i));
                if(dictInfo->objectForKey("customskin")){
                    UseItemStatusCell* cell = UseItemStatusCell::create(0,"customskin",0);
                    cell->setAnchorPoint(ccp(0.5, 0.5));
                    cell->setPosition(ccp(0,index*160));
                    m_scrollView->addChild(cell);
                }else{
                    int itemId = dictInfo->valueForKey("id")->intValue();
                    auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
                    if (toolInfo.price<=0 && toolInfo.getCNT()<=0) {
                        count--;
                        continue;
                    }
                    UseItemStatusCell* cell = UseItemStatusCell::create(itemId,"",0);
                    cell->setAnchorPoint(ccp(0.5, 0.5));
                    cell->setPosition(ccp(0,index*160));
                    m_scrollView->addChild(cell);
                }
                ++index;
            }
        }
        
        int contentHeight = count*160;
        m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,contentHeight));
        m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - contentHeight));
        
        m_scrollH = contentHeight;
    }
    
    m_barSize = m_bar->getContentSize();
    //加进度条头上的光效
    m_headParticleNode = CCNode::create();
    for (int i=1; i<=3; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("Loading_%d",i)->getCString());
        m_headParticleNode->addChild(particle);
    }
    m_timeNode->addChild(m_headParticleNode);
    m_timeNode->setVisible(false);
    return true;
}

void UseItemStatusView::updateTime(float t){
    map<int, CCDictionary* >::iterator it = ToolController::getInstance()->m_statusItems.find(m_type);
    m_time = 0;
    if(it!=ToolController::getInstance()->m_statusItems.end()){
        m_time =(it->second->valueForKey("endTime")->doubleValue() - WorldController::getInstance()->getTime())/1000;
    }
    if(m_time>0){
        int tmpEndTime = it->second->valueForKey("endTime")->doubleValue()/1000;
        int max = (it->second->valueForKey("endTime")->doubleValue() - it->second->valueForKey("startTime")->doubleValue())/1000;
        max = MAX(max, 1);
        float len = m_time*1.0/max;
        m_barRight->setVisible(len>0.99);
        if(len>1) len = 1;
//        m_bar->setContentSize(CCSize(m_barSize.width*len,m_barSize.height));
        m_bar->setScaleX(len);
        m_timeLabel->setString(_lang_1("105805", CC_SECTOA(m_time)));
        m_timeLabel->setVisible(true);
        
        if (!m_timeNode->isVisible() || m_endTime != tmpEndTime) {
            m_endTime = tmpEndTime;
            refeashData();
        }
        
        if(m_infoList->getContentSize().height!=m_listH){
            m_infoList->setContentSize(CCSizeMake(m_infoList->getContentSize().width, m_listH));
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollH));
            m_scrollView->setViewSize(m_infoList->getContentSize());
            if (CCCommonUtils::isIosAndroidPad()) {
                m_viewBg2_py=m_infoList->getPositionY()+10;
                m_viewBg2->setPositionY(m_viewBg2_py);//350
            }
            else {
                m_viewBg2_py=m_infoList->getPositionY()+m_infoList->getContentSize().height+20;
                m_viewBg2->setPositionY(m_viewBg2_py);//350
            }
        }
        m_timeNode->setVisible(true);
        float px=0;//进度条头上的光效位置
        px=m_bar->getPositionX()+(m_barSize.width*len);
        m_headParticleNode->setPositionX(px);

    }else{
//        m_bar->setContentSize(CCSize(0,m_barSize.height));
        m_timeLabel->setString(_lang_1("105805", CC_SECTOA(0)));
        this->unschedule(schedule_selector(UseItemStatusView::updateTime));
        m_infoList->setContentSize(CCSizeMake(m_infoList->getContentSize().width, m_listH+60));
        m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollH));
        m_scrollView->setViewSize(m_infoList->getContentSize());
        m_timeNode->setVisible(false);
        m_viewBg2_py=m_infoList->getPositionY()+m_infoList->getContentSize().height+20;
        m_viewBg2->setPositionY(m_viewBg2_py);//350
    }
    int textH = m_descriptionText->getContentSize().height * m_descriptionText->getOriginScaleY();
    if(m_timeNode->isVisible()){
        if (CCCommonUtils::isIosAndroidPad()) {
            m_descriptionText->setPositionY(m_timeNode->getPositionY() - textH / 2 - 50);
        }
        else
            m_descriptionText->setPositionY(m_timeNode->getPositionY() - textH / 2 - 30);
//        m_viewBg2->setPositionY(m_viewBg2_py-60);//350
    }else{
        if (CCCommonUtils::isIosAndroidPad()) {
            m_descriptionText->setPositionY(m_timeNode->getPositionY() - textH / 2);
            m_viewBg2->setPositionY(m_descriptionText->getPositionY() - textH / 2 - 50);//350
            m_infoList->setPositionY(m_viewBg2->getPositionY() - 10);
            
            int listHeight = m_infoList->getPositionY() + 1024;
            m_infoList->setContentSize(CCSizeMake(m_infoList->getContentSize().width, listHeight));
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollH));
            m_scrollView->setViewSize(m_infoList->getContentSize());
        }
        else
            m_descriptionText->setPositionY(m_timeNode->getPositionY() - textH / 2 + 30);
//        m_viewBg2->setPositionY(m_viewBg2_py);//350
    }
}

void UseItemStatusView::refeashData()
{
    m_scrollView->getContainer()->removeAllChildren();
    CCArray* array =ToolController::getInstance()->m_typeItems[m_type];
    int num = 0;
    int count = 0;
    int index = 0;
    if(array && array->count()>0){
        num = array->count();
        count = num;
        for (int i=0; i<num; i++) {
            CCDictionary* dictInfo = _dict(array->objectAtIndex(i));
            if(dictInfo->objectForKey("customskin")){
                UseItemStatusCell* cell = UseItemStatusCell::create(0,"customskin",0);
                cell->setAnchorPoint(ccp(0.5, 0.5));
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(ccp(0,index*300));
                }
                else
                    cell->setPosition(ccp(0,index*160));
                m_scrollView->addChild(cell);
            }else{
                int itemId = dictInfo->valueForKey("id")->intValue();
                auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
                if (toolInfo.price<=0 && toolInfo.getCNT()<=0) {
                    count--;
                    continue;
                }
                
                UseItemStatusCell* cell = UseItemStatusCell::create(itemId,"",0);
                cell->setAnchorPoint(ccp(0.5, 0.5));
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(ccp(0,index*300));
                }
                else
                    cell->setPosition(ccp(0,index*160));
                m_scrollView->addChild(cell);
            }
            ++index;
        }
    }
    
    int contentHeight = 0;
    if (CCCommonUtils::isIosAndroidPad()) {
        contentHeight = count * 300;
    }
    else
        contentHeight = count * 160;
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,contentHeight));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - contentHeight));
}

void UseItemStatusView::resetTime(CCObject* param){
    CCInteger* integer = (CCInteger*)param;
    if(integer->getValue()==m_type){
        updateTime();
        this->schedule(schedule_selector(UseItemStatusView::updateTime), 1);
    }
}

void UseItemStatusView::onEnter()
{
    CCNode::onEnter();
     this->schedule(schedule_selector(UseItemStatusView::updateTime), 1);
    updateTime();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseItemStatusView::resetTime), MSG_ITME_STATUS_TIME_CHANGE, NULL);
}

void UseItemStatusView::onExit()
{
    this->unschedule(schedule_selector(UseItemStatusView::updateTime));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ITME_STATUS_TIME_CHANGE);
    CCNode::onExit();
}

//void UseItemStatusView::onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent){
//    
//}

SEL_CCControlHandler UseItemStatusView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn", UseItemStatusView::onClickBuyBtn);
    return NULL;
}

bool UseItemStatusView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descriptionText", CCLabelIF*, this->m_descriptionText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, this->m_bar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeNode", CCNode*, this->m_timeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barRight", CCSprite*, this->m_barRight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg2", CCNode*, this->m_viewBg2);
    return false;
}

UseItemStatusCell* UseItemStatusCell::create(int itemId, string objId, int qid)
{
    auto ret = new UseItemStatusCell();
    if (ret && ret->init(itemId, objId, qid)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UseItemStatusCell::init(int itemId, string objId, int qid)
{
    bool ret = true;
    CCBLoadFile("UseItemStatusCell",this,this);
    setData(itemId, objId, qid);
    return ret;
}

void UseItemStatusCell::setData(int itemId, string objId, int qid)
{
    m_itemId = itemId;
    m_objId = objId;
    m_qid = qid;
    
    m_picNode->removeAllChildren();
    
    m_des2Label->setString("");
    if (m_itemId == 0) {
        if(objId == "customskin"){
            m_nameLabel->setString(_lang("101458").c_str());
            m_desLabel->setString(_lang("101460").c_str());
            m_numLabel->setString("");
            m_useBtn->setVisible(true);
            m_buyNode->setVisible(false);
            m_buyBtn->setEnabled(false);
            m_useBtn->setEnabled(true);
            CCCommonUtils::setButtonTitle(m_useBtn, _lang("108532").c_str());
            auto pic = CCLoadSprite::createSprite("uncityskin_icon.png");
            if (CCCommonUtils::isIosAndroidPad()) {
                pic->setScale(2);
            }
            m_picNode->addChild(pic);
        }else{
            m_numLabel->setString("");
            m_nameLabel->setString(_lang("103669").c_str());
            m_des2Label->setString(_lang("103671").c_str());
            m_buyNode->setVisible(false);
            CCCommonUtils::setButtonTitle(m_useBtn, _lang("103672").c_str());
            CCCommonUtils::setButtonSprite(m_useBtn, "btn_green4.png");
            
            m_useBtn->setVisible(true);
            m_buyNode->setVisible(false);
            m_buyBtn->setEnabled(false);
            int curTime = GlobalData::shared()->getWorldTime();
            int m_finishTime = GlobalData::shared()->allQueuesInfo[m_qid].finishTime;
            int tmpTime = m_finishTime-curTime;
            if (tmpTime < GlobalData::shared()->freeSpdT) {
                m_useBtn->setEnabled(true);
                m_desLabel->setString("");
            }
            else {
                m_lockNode->removeAllChildren();
                auto picLock = CCLoadSprite::createSprite("iron_lock.png");
                if (!CCCommonUtils::isIosAndroidPad()) {
                    picLock->setScale(0.5);
                }
                m_lockNode->addChild(picLock);
                m_useBtn->setEnabled(false);
                m_desLabel->setString(_lang_1("103670", CC_SECTOA(tmpTime-GlobalData::shared()->freeSpdT)));
            }
            
            auto pic = CCLoadSprite::createSprite("iron_lock.png");
            if (CCCommonUtils::isIosAndroidPad()) {
                pic->setScale(2);
            }
            m_picNode->addChild(pic);
        }
    }
    else {
        ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
        
        m_nameLabel->setString(toolInfo.getName().c_str());
        m_desLabel->setString(_lang(toolInfo.des).c_str());
        m_numLabel->setString(CC_ITOA(toolInfo.getCNT()));
        m_price = toolInfo.price;
        
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::createGoodsIcon(toolInfo.itemId, m_picNode, CCSize(180, 180));
        }
        else
            CCCommonUtils::createGoodsIcon(toolInfo.itemId, m_picNode, CCSize(92, 92));
        
//        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(toolInfo.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//        if (pic) {
//            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, 92);
//            m_picNode->addChild(iconBg);
//            CCCommonUtils::setSpriteMaxSize(pic, 92);
//            m_picNode->addChild(pic);
//        }
        
        m_inBtnGoldNum->setString(CC_CMDITOA(m_price));
        CCCommonUtils::setButtonTitle(m_useBtn, _lang("102137").c_str());
        m_btnMsgLabel->setString(_lang("104906").c_str());
        
        if (toolInfo.getCNT()>0) {
            m_buyNode->setVisible(false);
            m_buyBtn->setEnabled(false);
            m_useBtn->setVisible(true);
            m_useBtn->setEnabled(true);
        }
        else {
            m_buyNode->setVisible(true);
            m_buyBtn->setEnabled(true);
            m_useBtn->setVisible(false);
            m_useBtn->setEnabled(false);
        }
//        if(toolInfo.type2>=9){
//            m_buyNode->setVisible(false);
//            m_buyBtn->setEnabled(false);
//            m_useBtn->setVisible(true);
//            m_useBtn->setEnabled(toolInfo.cnt>0);
//        }
    }
}

void UseItemStatusCell::onEnter() {
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(UseItemStatusCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

void UseItemStatusCell::onExit() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UseItemStatusCell::onEnterFrame), this);
    CCNode::onExit();
}

void UseItemStatusCell::onEnterFrame(float dt)
{
    if(m_itemId == 0) {
        if(!m_objId.empty()){
            return;
        }
        int curTime = GlobalData::shared()->getWorldTime();
        int m_finishTime = GlobalData::shared()->allQueuesInfo[m_qid].finishTime;
        int tmpTime = m_finishTime-curTime;
        if (tmpTime <= GlobalData::shared()->freeSpdT) {
            m_desLabel->setString("");
            m_lockNode->removeAllChildren();
            m_useBtn->setEnabled(true);
        }
        else {
            m_desLabel->setString(_lang_1("103670", CC_SECTOA(tmpTime-GlobalData::shared()->freeSpdT)));
        }
    }
}

SEL_CCControlHandler UseItemStatusCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUseBtn", UseItemStatusCell::onClickUseBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn", UseItemStatusCell::onClickBuyBtn);
    return NULL;
}

void UseItemStatusCell::onClickUseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_isByBuy = false;
    if(m_itemId == 0 && m_objId == "customskin"){
        YesNoDialog::show(_lang("101459").c_str(), CCCallFunc::create(this, callfunc_selector(UseItemStatusCell::sureToCancel)));
    }else{
        onUseTool();
    }
}
void UseItemStatusCell::sureToCancel(){
    ToolController::getInstance()->cancelCustomSkin();
    UIComponent::getInstance()->onPopupReturnClick(NULL,CCControlEvent::TOUCH_DOWN);
}
void UseItemStatusCell::onYes()
{
    ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(UseItemStatusCell::onUseTool)),0,true,false,"UseItemStatusView");
}

void UseItemStatusCell::onUseTool()
{
    if(m_itemId == 0 && m_objId == "customskin"){
        sureToCancel();
    }else{
        ToolController::getInstance()->useTool(m_itemId,1,true,m_isByBuy);
    }
}

void UseItemStatusCell::onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_isByBuy = false;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    if (info.type == 4 && ToolController::getInstance()->checkUseStateTool(m_itemId,CCCallFunc::create(this, callfunc_selector(UseItemStatusCell::onYes))) )
    {
        m_isByBuy = true;
    }
    else
    {
        YesNoDialog::showButtonAndGold( _lang("104919").c_str() , CCCallFunc::create(this, callfunc_selector(UseItemStatusCell::onYes)), _lang("104906").c_str(), info.price);
    }
}

bool UseItemStatusCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useBtn", CCControlButton*, this->m_useBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_des2Label", CCLabelIF*, m_des2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockNode", CCNode*, m_lockNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtn", CCControlButton*, this->m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyNode", CCNode*, m_buyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, m_inBtnGoldNum);
    return false;
}