
#include "MonthCardView.h"
#include "RewardController.h"
#include "PayController.h"
#include "PopupViewController.h"
#include "TipsView.h"
#include "GoldExchangeInfoCommand.h"
#include "ImperialScene.h"
#include "SceneController.h"
#include "ToolController.h"

#define MSG_REFRESH_MONTH_CARD_VIEW "msg.refresh.month.card.view"

MonthCardView* MonthCardView::create(){
    MonthCardView* ret = new MonthCardView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
MonthCardView::MonthCardView():m_rootNode(NULL){
    
}
MonthCardView::~MonthCardView(){
}
void MonthCardView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MonthCardView::refreshData), MSG_REFRESH_MONTH_CARD_VIEW, NULL);
}
void MonthCardView::onExit(){
    PopupBaseView::onExit();
}
bool MonthCardView::init(){
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(500, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    
    auto tmpCCB = CCBLoadFile("MonthCardView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        int newBgHeight = CCDirector::sharedDirector()->getWinSize().height;
        int bgcount = newBgHeight/100;
        for (int i = 0; i<=bgcount; i++) {
            auto pic = CCLoadSprite::createSprite("technology_09.png");
            pic->setPositionY(-i*100);
            pic->setScaleX(2.4);
            m_BGNode->addChild(pic);
        }
    }
    else {
        setContentSize(CCDirector::sharedDirector()->getWinSize());
    }

    m_lblDes->setString(_lang("101021"));
    auto iter = GlobalData::shared()->goldExchangeList.begin();
    while (iter!=GlobalData::shared()->goldExchangeList.end()) {
        if((*iter).second && (*iter).second->type.compare("5")==0){
            mCardID.push_back((*iter).second->id);
        }
        ++iter;
    }
    m_rootNode = Node::create();
    m_infoList->addChild(m_rootNode);
    setTitleName("");
    generateData();
    return true;
}
void MonthCardView::generateData(){
    if(m_rootNode->getChildrenCount()){
        m_rootNode->removeAllChildren();
    }
    float offsetY = 0;
    auto iter1 = mCardID.begin();
    while (iter1!=mCardID.end()) {
        bool isBuy = false;
        std::string cardid = (*iter1);
        if(GlobalData::shared()->monthCardInfoList.find(cardid)!=GlobalData::shared()->monthCardInfoList.end()){
            MonthCardInfo *info = GlobalData::shared()->monthCardInfoList[cardid];
            if(info->canReward()==true){
                isBuy = true;
            }
        }
        if(isBuy == true){
            auto *node = MonthCardRewardNode::create(cardid);
            m_rootNode->addChild(node);
        }else{
            auto *node = MonthCardBuyNode::create(cardid);
            m_rootNode->addChild(node);
        }
        ++iter1;
    }
}
bool MonthCardView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblDes", CCLabelIF*, this->m_lblDes);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BGNode", CCNode*, this->m_BGNode);
    return false;
}

SEL_CCControlHandler MonthCardView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickClose", MonthCardView::onClickClose);
    return NULL;
}

void MonthCardView::onClickClose(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    closeSelf();
}

void MonthCardView::refreshData(cocos2d::CCObject *ccObj){
    if(!ccObj)
        return;
    CCString* str = dynamic_cast<CCString*>(ccObj);
    if(!str || !str->getCString())
        return;
    if(find(mCardID.begin(), mCardID.end(), str->getCString())!=mCardID.end()){
        generateData();
    }
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if(layer){
        //layer->updateShipState("collect");
    }
}

//
//
//MonthCardRewardNode
//
//
MonthCardRewardNode* MonthCardRewardNode::create(string cardid){
    MonthCardRewardNode* ret = new MonthCardRewardNode();
    if(ret && ret->init(cardid)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
MonthCardRewardNode::MonthCardRewardNode():m_bTick(false),m_waitInterface(NULL){
    
}
MonthCardRewardNode::~MonthCardRewardNode(){
}
void MonthCardRewardNode::onEnter(){
    CCNode::onEnter();
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MonthCardRewardNode::rewardCard), GOLDEXCHANGE_SHOW_COMMAND, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(MonthCardRewardNode::onGameTick), this, 1,kCCRepeatForever, 0.0f, false);
}
void MonthCardRewardNode::onExit(){
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GOLDEXCHANGE_SHOW_COMMAND);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(MonthCardRewardNode::onGameTick), this);
    CCNode::onExit();
}
bool MonthCardRewardNode::init(string cardid){
    if (!CCNode::init()) {
        return false;
    }
    auto tmpCCB = CCBLoadFile("MonthCardRewardCell",this,this,true);
    setContentSize(tmpCCB->getContentSize());
    this->mCardID = cardid;
    refreshData();
    return true;
}
bool MonthCardRewardNode::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle1", CCLabelIF*, this->m_lblTitle1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle2", CCLabelIF*, this->m_lblTitle2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnReward", CCControlButton*, this->m_btnReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeTime", CCNode*, this->m_nodeTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblDes", CCLabelIF*, this->m_lblDes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTime", CCLabelIF*, this->m_lblTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, this->m_picNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel1", CCLabelIF*, this->m_numLabel[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", CCNode*, this->m_picNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel2", CCLabelIF*, this->m_numLabel[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemSpr1", CCSprite*, m_itemSpr[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemSpr2", CCSprite*, m_itemSpr[1]);
    return false;
}
SEL_CCControlHandler MonthCardRewardNode::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickReward", MonthCardRewardNode::onClickReward);
    return NULL;
}
bool MonthCardRewardNode::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    int idx = 0;
    while (idx<2) {
        if(isTouchInside(m_itemSpr[idx], pTouch)){
            showDes(idx);
            return true;
        }
        ++idx;
    }
    return false;
}
void MonthCardRewardNode::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}
void MonthCardRewardNode::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    showDes(-1);
}
void MonthCardRewardNode::showDes(int idx){
    if(idx < 0 ||  idx > 1){
        m_desNode->setVisible(false);
        return;
    }
    auto iter = GlobalData::shared()->monthCardInfoList.find(mCardID);
    if(iter!=GlobalData::shared()->monthCardInfoList.end()){
        MonthCardInfo *card =(*iter).second;
        CCArray *rewardArr  = card->rewards;
        CCDictionary* reward = _dict(rewardArr->objectAtIndex(idx));
        int type = reward->valueForKey("type")->intValue();
        if (type == R_GOODS){
             CCDictionary* value = _dict(reward->objectForKey("value"));
            if (value){
                int toolid = value->valueForKey("id")->intValue();
                auto &tool = ToolController::getInstance()->getToolInfoById(toolid);
                CCLabelIF* lbl1 = dynamic_cast<CCLabelIF*>(m_desNode->getChildByTag(2));
                if(lbl1){
                    lbl1->setString(tool.getName());
                }
                CCLabelIF* lbl2 = dynamic_cast<CCLabelIF*>(m_desNode->getChildByTag(1));
                if(lbl2){
                    lbl2->setString(_lang(tool.des));
                }
            }
        }else{
            CCLabelIF* lbl1 = dynamic_cast<CCLabelIF*>(m_desNode->getChildByTag(2));
            if(lbl1){
                lbl1->setString(CCCommonUtils::getResourceNameByType(type));
            }
            CCLabelIF* lbl2 = dynamic_cast<CCLabelIF*>(m_desNode->getChildByTag(1));
            if(lbl2){
                lbl2->setString("");
            }
        }
        m_desNode->setVisible(true);
    }
}
void MonthCardRewardNode::refreshData(){
    auto iter = GlobalData::shared()->monthCardInfoList.find(mCardID);
    if(iter!=GlobalData::shared()->monthCardInfoList.end()){
        MonthCardInfo *card =(*iter).second;
        CCArray *rewardArr  = card->rewards;
        CCDictionary* reward = NULL;
        CCDictionary* value = NULL;
        int maxCount = 2;
        int count = MIN(rewardArr->count(), maxCount);
        int iconW = 120;
        int type = 0;
        for (int i = 0; i < count; i++){
            reward = _dict(rewardArr->objectAtIndex(i));
            type = reward->valueForKey("type")->intValue();
            if (type == R_GOODS){
                value = _dict(reward->objectForKey("value"));
                if (value){
                    int toolid = value->valueForKey("id")->intValue();
                    CCCommonUtils::createGoodsIcon(toolid, m_picNode[i], CCSize(iconW, iconW));
                    m_numLabel[i]->setString(std::string("x ") + value->valueForKey("num")->getCString());
                }
            }else{
                auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(BLUE).c_str());
                CCCommonUtils::setSpriteMaxSize(iconBg, iconW-10,true);
                m_picNode[i]->addChild(iconBg);
                string nextIcon = RewardController::getInstance()->getPicByType(type,0);
                auto pic = CCLoadSprite::createSprite(nextIcon.c_str());
                m_picNode[i]->addChild(pic);
                CCCommonUtils::setSpriteMaxSize(pic, iconW-10,true);
                m_numLabel[i]->setString(std::string("x ") + reward->valueForKey("value")->getCString());
            }
        }
        m_lblTitle1->setString( _lang("101281"));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_lblTitle1->setMaxScaleXByWidth(1100.0);
        }
        else
            m_lblTitle1->setMaxScaleXByWidth(550.0);
        string title = _lang("101024") +": " + CC_ITOA(card->getCount()-card->getTimes());
        m_lblTitle2->setString(title.c_str());
        m_lblTime->setString("");
        m_lblDes->setString(_lang("101312"));
        setButtonTitle(m_btnReward, _lang("107516").c_str());
        if(card->getAccepted()==true){
            m_btnReward->setVisible(false);
            m_lblDes->setVisible(true);
            m_nodeTime->setVisible(true);
        }else{
            m_btnReward->setVisible(true);
            m_lblDes->setVisible(false);
            m_nodeTime->setVisible(false);
        }
        onGameTick(0.0);
    }
}
void MonthCardRewardNode::onGameTick(float time){
    auto iter = GlobalData::shared()->monthCardInfoList.find(mCardID);
    if(iter != GlobalData::shared()->monthCardInfoList.end()){
        auto card = (*iter).second;
        if(card->canReward()){
            double timeStamp = GlobalData::shared()->getTimeStamp();
            if(GlobalData::shared()->tomorrow_time <= timeStamp){
                GlobalData::shared()->tomorrow_time += 86400;
                card->resetCard();
                m_btnReward->setVisible(true);
                m_lblDes->setVisible(false);
            }
            
            double leftTime = GlobalData::shared()->tomorrow_time - timeStamp;
//            string str = _lang("101285");
//            str.append("  ");
//            str.append(CC_SECTOA(leftTime));
            m_lblTime->setString(CC_SECTOA(leftTime));
            m_nodeTime->setVisible(!m_btnReward->isVisible());
        }
    }
}
void MonthCardRewardNode::onClickReward(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    if(m_waitInterface)
        return;
    m_waitInterface= GameController::getInstance()->showWaitInterface(m_btnReward);
    MonthCardRewardCommand* cmd = new MonthCardRewardCommand(mCardID);
    cmd->sendAndRelease();
}
void MonthCardRewardNode::rewardCard(CCObject *ccObj){
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface=NULL;
    }
    CCString *strVal = dynamic_cast<CCString*>(ccObj);
    if(strVal && strVal->getCString() && mCardID.compare(strVal->getCString()) == 0 ){
        this->refreshData();
        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        if(layer){
           // layer->updateShipState("leave");
        }
    }
}

//
//
//MonthCardBuyNode
//
//
MonthCardBuyNode* MonthCardBuyNode::create(string cardid){
    MonthCardBuyNode* ret = new MonthCardBuyNode();
    if(ret && ret->init(cardid)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
MonthCardBuyNode::MonthCardBuyNode()/*:m_isBuy(false)*/{
    
}
MonthCardBuyNode::~MonthCardBuyNode(){
}
void MonthCardBuyNode::onEnter(){
    CCNode::onEnter();
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MonthCardBuyNode::buySuccess), PAYMENT_COMMAND_RETURN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MonthCardBuyNode::errorPayment), MSG_PAY_FAILED, NULL);
}
void MonthCardBuyNode::onExit(){
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PAYMENT_COMMAND_RETURN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_PAY_FAILED);
    CCNode::onExit();
}
bool MonthCardBuyNode::init(string cardid){
    if (!CCNode::init()) {
        return false;
    }
    auto tmpCCB = CCBLoadFile("MonthCardBuyCell",this,this,true);
    setContentSize(tmpCCB->getContentSize());
    this->mCardID = cardid;
    m_lblTitle1->setString(_lang("101281"));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_lblTitle1->setMaxScaleXByWidth(1100.0);
    }
    else
        m_lblTitle1->setMaxScaleXByWidth(550.0);
    m_lblTitle2->setString(_lang("101287"));
    auto iter0 = GlobalData::shared()->goldExchangeList.find(mCardID);
    if(iter0 != GlobalData::shared()->goldExchangeList.end()){
       string dollar = PayController::getInstance()->getDollarText((*iter0).second->dollar,(*iter0).second->product_id);
        setButtonTitle(m_btnBuy, _lang_1("101245",dollar.c_str()));
    }
    
    auto iter = GlobalData::shared()->monthCardInfoList.find(mCardID);
    if(iter!=GlobalData::shared()->monthCardInfoList.end()){
        MonthCardInfo *card =GlobalData::shared()->monthCardInfoList[mCardID];
        CCArray *rewardArr  = card->rewards;
        CCDictionary* reward = NULL;
        CCDictionary* value = NULL;
        int maxCount = 2;
        int count = MIN(rewardArr->count(), maxCount);
        int iconW = 120;
        int type = 0;
        for (int i = 0; i < count; i++){
            reward = _dict(rewardArr->objectAtIndex(i));
            type = reward->valueForKey("type")->intValue();
            if (type == R_GOODS){
                value = _dict(reward->objectForKey("value"));
                if (value){
                    int toolid = value->valueForKey("id")->intValue();
                    CCCommonUtils::createGoodsIcon(toolid, m_picNode[i], CCSize(iconW, iconW));
                    m_numLabel[i]->setString(std::string("x ") + value->valueForKey("num")->getCString());
                }
            }else{
                auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(BLUE).c_str());
                CCCommonUtils::setSpriteMaxSize(iconBg, iconW-10,true);
                m_picNode[i]->addChild(iconBg);
                string nextIcon = RewardController::getInstance()->getPicByType(type,0);
                auto pic = CCLoadSprite::createSprite(nextIcon.c_str());
                m_picNode[i]->addChild(pic);
                CCCommonUtils::setSpriteMaxSize(pic, iconW-10,true);
                m_numLabel[i]->setString(std::string("x ") + reward->valueForKey("value")->getCString());
            }
        }
    }
    
    return true;
}
bool MonthCardBuyNode::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle1", CCLabelIF*, this->m_lblTitle1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle2", CCLabelIF*, this->m_lblTitle2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnBuy", CCControlButton*, m_btnBuy);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, this->m_picNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel1", CCLabelIF*, this->m_numLabel[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", CCNode*, this->m_picNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel2", CCLabelIF*, this->m_numLabel[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemSpr1", CCSprite*, m_itemSpr[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemSpr2", CCSprite*, m_itemSpr[1]);
    return false;
}
SEL_CCControlHandler MonthCardBuyNode::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuy", MonthCardBuyNode::onClickBuy);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTip", MonthCardBuyNode::onClickTip);
    return NULL;
}
bool MonthCardBuyNode::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    int idx = 0;
    while (idx<2) {
        if(isTouchInside(m_itemSpr[idx], pTouch)){
            showDes(idx);
            return true;
        }
        ++idx;
    }
    return false;
}
void MonthCardBuyNode::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}
void MonthCardBuyNode::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    showDes(-1);
}
void MonthCardBuyNode::showDes(int idx){
    if(idx < 0 ||  idx > 1){
        m_desNode->setVisible(false);
        return;
    }
    auto iter = GlobalData::shared()->monthCardInfoList.find(mCardID);
    if(iter!=GlobalData::shared()->monthCardInfoList.end()){
        MonthCardInfo *card =(*iter).second;
        CCArray *rewardArr  = card->rewards;
        CCDictionary* reward = _dict(rewardArr->objectAtIndex(idx));
        int type = reward->valueForKey("type")->intValue();
        if (type == R_GOODS){
            CCDictionary* value = _dict(reward->objectForKey("value"));
            if (value){
                int toolid = value->valueForKey("id")->intValue();
                auto &tool = ToolController::getInstance()->getToolInfoById(toolid);
                CCLabelIF* lbl1 = dynamic_cast<CCLabelIF*>(m_desNode->getChildByTag(2));
                if(lbl1){
                    lbl1->setString(tool.getName());
                }
                CCLabelIF* lbl2 = dynamic_cast<CCLabelIF*>(m_desNode->getChildByTag(1));
                if(lbl2){
                    lbl2->setString(_lang(tool.des));
                }
            }
        }else{
            CCLabelIF* lbl1 = dynamic_cast<CCLabelIF*>(m_desNode->getChildByTag(2));
            if(lbl1){
                lbl1->setString(CCCommonUtils::getResourceNameByType(type));
            }
            CCLabelIF* lbl2 = dynamic_cast<CCLabelIF*>(m_desNode->getChildByTag(1));
            if(lbl2){
                lbl2->setString("");
            }
        }
        m_desNode->setVisible(true);
    }
}
void MonthCardBuyNode::onClickTip(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    auto iter0 = GlobalData::shared()->goldExchangeList.find(mCardID);
    if(iter0 != GlobalData::shared()->goldExchangeList.end()){
        std::string tipB = _lang("101289");
        string doller = PayController::getInstance()->getDollarText((*iter0).second->dollar, (*iter0).second->product_id);
        string showTip = _lang_1("101286",doller.c_str());

        if(!tipB.empty()){
            showTip.append("\n");
            showTip.append(tipB);
        }
        PopupViewController::getInstance()->addPopupView(TipsView::create(showTip));
    }
}
void MonthCardBuyNode::onClickBuy(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
//    if(m_isBuy==true)
//        return;
//    
    auto iter0 = GlobalData::shared()->goldExchangeList.find(mCardID);
    if(iter0 != GlobalData::shared()->goldExchangeList.end()){
//        m_isBuy=true;
        PayController::getInstance()->callPayment((*iter0).second);
    }
}
void MonthCardBuyNode::buySuccess(CCObject *ccObj){
    bool pay = false;
    if(ccObj != NULL){
        CCString* str = dynamic_cast<CCString*>(ccObj);
        auto iter = GlobalData::shared()->goldExchangeList.find(mCardID);
        if (iter!=GlobalData::shared()->goldExchangeList.end()) {
            auto  dataItem = (*iter).second;
            if(str && str->getCString() && std::string(str->getCString()).compare(dataItem->id) == 0 ){
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFRESH_MONTH_CARD_VIEW,CCString::create(dataItem->id));
            }
        }
    }
}
void MonthCardBuyNode::errorPayment(cocos2d::CCObject *obj){
    this->unschedule(schedule_selector(MonthCardBuyNode::onShowError));
    this->schedule(schedule_selector(MonthCardBuyNode::onShowError),0.3,1,0);
}
void MonthCardBuyNode::onShowError(float dt){
    PayController::getInstance()->showErrorDialog();
    this->unschedule(schedule_selector(MonthCardBuyNode::onShowError));
}