//
//  KingsGiftView.cpp
//  IF
//
//  Created by 李锐奇 on 15/1/23.
//
//

#include "KingsGiftView.h"
#include "KingsGiftCommand.h"
#include "RewardController.h"
#include "AllianceInviteView.h"
#include "HelpshiftCocos2dx.h"
#include "ParticleFireAni.h"
#include "YesNoDialog.h"
#include "UIComponent.h"
const static int downH = 75;
const static int upH = 145;
const static int cellPerLine = 130;
const static int numPerLine = 4;

void KingsGiftCell::setData(KingsGiftInfo *info){
    m_info = info;
    refreshView();
}

void KingsGiftCell::refreshView(){
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(105, true);
    
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(105, false);
    });
    m_giftBtn->setEnabled(m_info->totalCount > m_info->usedCount);

    std::string itemName = _lang(m_info->dialogId);
    m_nameText->setString(itemName);
    std::string numStr = _lang("110127") + CC_ITOA(m_info->totalCount - m_info->usedCount) + "/" + CC_ITOA(m_info->totalCount);
    m_numText->setString(numStr);
    m_iconContainer->removeAllChildren();

    auto addItem = [](int num, int type, int value,CCNode *parent, int &startX, int &startY, int &count){
        if(count % numPerLine == 0){
            startY -= cellPerLine;
            startX = 70;
        }
        count++;
        
        std::string iconPath = RewardController::getInstance()->getPicByType(type, value);
        std::string itemName = CC_ITOA(num);
        auto bgSprite = CCLoadSprite::createSprite("Items_icon_kuang.png");
        parent->addChild(bgSprite);
        bgSprite->setScale(0.66);
        bgSprite->setPosition(ccp(startX, startY));

        auto sprite = CCLoadSprite::createSprite(iconPath.c_str());
        parent->addChild(sprite);
        CCCommonUtils::setSpriteMaxSize(sprite, 100);
        sprite->setPosition(bgSprite->getPosition());
        
        auto nameBG = CCLoadSprite::createSprite("skill_name_bg.png");
        parent->addChild(nameBG);
        nameBG->setScaleX(-0.57);
        nameBG->setScaleY(-0.7);
        nameBG->setPosition(ccp(bgSprite->getPositionX(), bgSprite->getPositionY() - bgSprite->getContentSize().height * bgSprite->getScale() / 2 + 15));
        auto label = CCLabelIFTTF::create(itemName.c_str());
        label->setFontSize(20);
        label->setAnchorPoint(ccp(1, 0.5));
        label->setColor(ccc3(249, 218, 180));
        parent->addChild(label);
        label->setPosition(ccp(bgSprite->getPositionX() + bgSprite->getContentSize().width / 2 * bgSprite->getScale() - 6, nameBG->getPositionY()));
        
        startX += 120;
    };
    
    int startX = 70;
    int startY = 70;
    int count = 0;
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(m_info->m_reward, obj) {
        auto dict = _dict(obj);
        int type = dict->valueForKey("type")->intValue();
        int num = 0;
        int value = 0;
        if(type == R_GOODS){
            auto item = _dict(dict->objectForKey("value"));
            num = item->valueForKey("num")->intValue();
            value = item->valueForKey("id")->intValue();
        }else{
            num = dict->valueForKey("value")->intValue();
        }
        addItem(num, type, value, m_iconContainer, startX, startY, count);
    }
    int totalH = ceil(count / numPerLine) * cellPerLine + downH + upH;

    addBG(totalH);
    m_titleNode->setPositionY(totalH - upH);
}

void KingsGiftCell::addBG(int totalH){
    std::string picPath = "Kingsreward_BG.png";
    m_cellBGNode->removeAllChildren();
    auto batchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource(picPath.c_str())->getTexture(), 10);
    m_cellBGNode->addChild(batchNode);
    int startY = downH;
    while (startY < totalH - upH) {
        auto sprite = CCLoadSprite::createSprite(picPath.c_str());
        sprite->setPosition(ccp(0, startY));
        sprite->setAnchorPoint(ccp(0, 0));
        batchNode->addChild(sprite);
        startY += sprite->getContentSize().height;
    }
    m_sprItemBG->setPreferredSize(CCSize(m_sprItemBG->getContentSize().width, totalH - upH - downH));
    m_iconContainer->setPositionY(totalH - upH);
    m_sprItemBG->setPositionY(totalH - upH + 10);
}

bool KingsGiftCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numText", CCLabelIF*, m_numText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconContainer", CCNode*, m_iconContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNode", CCNode*, m_titleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBGNode", CCNode*, m_cellBGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_giftBtn", CCControlButton*, m_giftBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprItemBG", CCScale9Sprite*, m_sprItemBG);

    return false;
}

bool KingsGiftCell::init(){
    CCBLoadFile("KingsGiftCell", this, this);
    CCCommonUtils::setButtonTitle(m_giftBtn, _lang("110116").c_str());
    if(GlobalData::shared()->playerInfo.officer != KINGDOM_KING_ID){
        m_giftBtn->setVisible(false);
    }
    m_giftBtn->setSwallowsTouches(false);
    return true;
}

void KingsGiftCell::onEnter(){
    CCNode::onEnter();

    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void KingsGiftCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool KingsGiftCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(this->m_giftBtn, pTouch)){
        return true;
    }
    return false;
}

void KingsGiftCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void KingsGiftCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    if(m_info->totalCount <= m_info->usedCount || GlobalData::shared()->playerInfo.officer != KINGDOM_KING_ID){
        return;
    }
    if(isTouchInside(this->m_giftBtn, pTouch)){
        float dis = ccpDistance(pTouch->getLocation(), pTouch->getStartLocation());
        if(dis < 20){//小范围移动
            AllianceInfo *info = NULL;
            if(GlobalData::shared()->playerInfo.isInAlliance()){
                info = &GlobalData::shared()->playerInfo.allianceInfo;
            }
            auto pop = AllianceInviteView::create(0, KINGSGIFT, m_info->presentId, m_info->dialogId);
            PopupViewController::getInstance()->addPopupInView(pop);
        }
    }
}

//---

void KingsGiftView::scrollViewDidScroll(CCScrollView* view){
    
}

void KingsGiftView::tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell){

}

cocos2d::CCSize KingsGiftView::tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx){
    if(idx >= m_info->count()){
        return CCSize(0, 0);
    }
    auto info = dynamic_cast<KingsGiftInfo*>(m_info->objectAtIndex(idx));
    int totalH = 0;
    int total = info->m_reward->count();
    totalH += ceil(total * 1.0f / numPerLine) * cellPerLine + downH + upH;
    
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1536, totalH*2.3);
    }
    return CCSize(640, totalH);
}

cocos2d::CCSize KingsGiftView::cellSizeForTable(  cocos2d::extension::TableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1536, 322);
    }
    return CCSize(640, 140);
}

CCTableViewCell* KingsGiftView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx){
    if(idx >= m_info->count()){
        return NULL;
    }
    KingsGiftCell *cell = (KingsGiftCell*)table->dequeueCell();
    auto info = dynamic_cast<KingsGiftInfo*>(m_info->objectAtIndex(idx));
    if(cell == NULL){
        cell = KingsGiftCell::create();
    }
    cell->setData(info);
    return cell;
}

ssize_t KingsGiftView::numberOfCellsInTableView(  cocos2d::extension::TableView *table){
    int size = m_info->count();
    return size;
}

void KingsGiftView::getData(){
    KingsGiftGetInfoCommand *cmd = new KingsGiftGetInfoCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(KingsGiftView::getDataBack), NULL));
    cmd->sendAndRelease();
}

void KingsGiftView::getDataBack(CCObject *obj){
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : net error
        return;
    }
    m_info->removeAllObjects();
    auto params = _dict(ret->getData());
    if(params){
        auto arr = dynamic_cast<CCArray*>(params->objectForKey("List"));
        CCObject *obj;
        CCARRAY_FOREACH(arr, obj){
            auto dict = _dict(obj);
            auto info = KingsGiftInfo::create();
            info->presentId = dict->valueForKey("presentId")->getCString();
            info->dialogId = dict->valueForKey("pDialogId")->getCString();
            info->rewardId = dict->valueForKey("rewardId")->getCString();
            info->m_reward = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
            info->usedCount = dict->valueForKey("usedCount")->intValue();
            info->totalCount = dict->valueForKey("total")->intValue();
            m_info->addObject(info);
        }
    }
    refreshView();
}

void KingsGiftView::refreshView(){
    m_tabView->reloadData();
}

void KingsGiftView::onEnter(){
    if (CCCommonUtils::isIosAndroidPad()) {
        UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    }
    m_tabView->setTouchEnabled(true);
    PopupBaseView::onEnter();
    if (CCCommonUtils::isIosAndroidPad()) {
        m_titleTxt->setString(_lang("110122"));
    }
    else
        setTitleName(_lang("110122"));
    getData();
}

void KingsGiftView::onExit(){
    m_tabView->setTouchEnabled(false);
    GlobalData::shared()->isBind = false;
    PopupBaseView::onExit();
}

bool KingsGiftView::init(){
    bool ret = false;
//    GlobalData::shared()->playerInfo.officer = KINGDOM_KING_ID;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(310, true);

        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(310, false);
        });
        setIsHDPanel(true);
        m_info = CCArray::create();

        auto bg = CCBLoadFile("KingsGiftView", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else {
            this->setContentSize(bg->getContentSize());
            int addH = CCDirector::sharedDirector()->getWinSize().height - 852;
            m_listContainer->setContentSize(m_listContainer->getContentSize() + CCSize(0, addH));
            m_bgContainer->setPositionY(m_bgContainer->getPositionY() - addH);
            m_downNode->setPositionY(m_downNode->getPositionY() - addH);
        }
        m_tabView = CCTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_listContainer->addChild(m_tabView);
        
        CCCommonUtils::setButtonTitle(m_introBtn, _lang("108730").c_str());
        CCCommonUtils::setButtonTitle(m_historyBtn, _lang("110110").c_str());
        
        ParticleFireAni* par = ParticleFireAni::create();
        m_fireNode1->addChild(par);
        
        ParticleFireAni* par2 = ParticleFireAni::create();
        m_fireNode2->addChild(par2);
        addBG();
        ret = true;
    }
    return true;
}
    
bool KingsGiftView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_introBtn", CCControlButton*, this->m_introBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_historyBtn", CCControlButton*, this->m_historyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, this->m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    if(CCCommonUtils::isIosAndroidPad()){
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, m_titleTxt);
    }
    return false;
}

void KingsGiftView::addBG(){
    auto tbg = CCLoadSprite::loadResource("technology_09.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    m_bgContainer->addChild(tBatchNode);
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int curHight = 0;
    while (curHight < maxHight) {
        auto bg = CCLoadSprite::createSprite("technology_09.png");
        bg->setAnchorPoint(ccp(0, 0));
        bg->setPosition(ccp(0, curHight));
        if (CCCommonUtils::isIosAndroidPad()) {
            bg->setScaleX(2.4);
        }
        curHight += bg->getContentSize().height;
        tBatchNode->addChild(bg);
    }
}

cocos2d::SEL_MenuHandler KingsGiftView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}

SEL_CCControlHandler KingsGiftView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onIntroClick", KingsGiftView::onIntroClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHistoryClick", KingsGiftView::onHistoryClick);

    return NULL;
}

void KingsGiftView::onIntroClick(CCObject * pSender, CCControlEvent pCCControlEvent){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftFAQ("45201");
//    HelpshiftCocos2dx::showFAQSection("466");
}

void KingsGiftView::onHistoryClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(GiftGiveHistoryView::create());
}

//--

bool KingsGiftHistoryCell::init(){
    CCBLoadFile("FightInfoCellCCB", this, this);
    m_node2->setVisible(false);
    return true;
}

void KingsGiftHistoryCell::onEnter(){
    CCNode::onEnter();
}

void KingsGiftHistoryCell::onExit(){
    CCNode::onExit();
}

void KingsGiftHistoryCell::setData(std::string content, std::string timeStr){
    m_contents = content;
    m_timeStr = timeStr;
    refresh();
}

void KingsGiftHistoryCell::refresh(){
    m_contentTxt->setString(m_contents.c_str());
    m_timeTxt->setString(m_timeStr);
    int msgHeight = m_contentTxt->getContentSize().height*m_contentTxt->getOriginScaleY() + 6;
    msgHeight = MAX(msgHeight, 40);
    m_cellBg->setContentSize(CCSizeMake(534.0, msgHeight));
    m_cellBg->setAnchorPoint(ccp(0.5,0));
    m_cellBg->setVisible(false);
}

bool KingsGiftHistoryCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBg", CCScale9Sprite*, this->m_cellBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentTxt", CCLabelIFTTF*, this->m_contentTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", CCNode*, this->m_node2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);

    return false;
}
//--

void GiftGiveHistoryView::scrollViewDidScroll(CCScrollView* view){
    
}
    
void GiftGiveHistoryView::tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize GiftGiveHistoryView::tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx){
    if(idx >= m_contents.size()){
        return CCSize(0, 0);
    }
    std::string str = m_contents[idx];
    m_testTxt->setString(str.c_str());
    int msgHeight = m_testTxt->getContentSize().height * m_testTxt->getOriginScaleY() + 20;
//    msgHeight = MAX(msgHeight, 40);
//    msgHeight = msgHeight + 10;
//    msgHeight += 40;

    return CCSize(640, msgHeight);
}

cocos2d::CCSize GiftGiveHistoryView::cellSizeForTable(  cocos2d::extension::TableView *table){
    return CCSize(640, 60);
}

CCTableViewCell* GiftGiveHistoryView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx){
    if(idx >= m_contents.size()){
        return NULL;
    }
    KingsGiftHistoryCell* cell = (KingsGiftHistoryCell*)table->dequeueCell();
    std::string contentStr = m_contents[idx];
    std::string timeStr = m_times[idx];
    if(cell == NULL){
        cell = KingsGiftHistoryCell::create();
    }
    cell->setData(contentStr, timeStr);
    return cell;
}

ssize_t GiftGiveHistoryView::numberOfCellsInTableView(  cocos2d::extension::TableView *table){
    int size = m_contents.size();
    return size;
}

bool GiftGiveHistoryView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(208, true);
        
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(208, false);
        });
        setIsHDPanel(true);
        auto ccb = CCBLoadFile("WarBuildInfo", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
            m_testTxt = CCLabelIFTTF::create("", 36);
            m_testTxt->setAlignment(kCCTextAlignmentLeft);
            m_testTxt->setVerticalAlignment(kCCVerticalTextAlignmentTop);
            m_testTxt->setDimensions(CCSize(882,0));
            m_testTxt->setAnchorPoint(ccp(0, 1));
        }
        else {
            this->setContentSize(ccb->getContentSize());
            m_testTxt = CCLabelIFTTF::create("", 20);
            m_testTxt->setAlignment(kCCTextAlignmentLeft);
            m_testTxt->setVerticalAlignment(kCCVerticalTextAlignmentTop);
            m_testTxt->setDimensions(CCSize(490,0));
            m_testTxt->setAnchorPoint(ccp(0, 1));
        }
        
        m_tabView = CCTableView::create(this, m_listNode->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_listNode->addChild(m_tabView);
        m_titleText->setString(_lang("110122"));
        this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
            if (! isTouchInside(m_listNode, pTouch)){
                this->closeSelf();
            }
        });
        m_infoNode->setVisible(false);
        m_historyNode->setVisible(true);
        m_historyBtn->setVisible(false);
        m_infoBtn->setVisible(false);
        
        getData();
        ret = true;
    }
    return ret;
}

void GiftGiveHistoryView::getData(){
    GetKingsGiftHistoryCommand *cmd = new GetKingsGiftHistoryCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(GiftGiveHistoryView::getDataBack), NULL));
    cmd->sendAndRelease();
}

void GiftGiveHistoryView::getDataBack(CCObject *obj){
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : net error
        return;
    }
    m_contents.clear();
    m_times.clear();
    auto params = _dict(ret->getData());
    if(params){
        std::string kingName = "";
        if(params->objectForKey("kingName")){
            kingName = params->valueForKey("kingName")->getCString();
        }

        std::string abbr = "";
        if(params->objectForKey("kingAbbr")){
            abbr = params->valueForKey("kingAbbr")->getCString();
        }
        if(abbr != ""){
            kingName = std::string("(") + abbr + ")" + kingName;
        }

        auto arr = dynamic_cast<CCArray*>(params->objectForKey("list"));
        CCObject *obj;
        CCARRAY_FOREACH(arr, obj){
            auto dict = _dict(obj);
            std::string abbr = "";
            if(dict->objectForKey("abbr")){
                abbr = dict->valueForKey("abbr")->getCString();
            }
            std::string toName = dict->valueForKey("toUserName")->getCString();
            if(abbr != ""){
                toName = std::string("(") + abbr + ")" + toName;
            }
            std::string rewardName = _lang(dict->valueForKey("pDialogId")->getCString());
            std::string str = _lang_3("110126", kingName.c_str(), toName.c_str(), rewardName.c_str());
            time_t time = time_t(dict->valueForKey("sendTime")->intValue());
            std::string timeStr = CCCommonUtils::timeStampToDHM(time).c_str();
            m_contents.push_back(str);
            m_times.push_back(timeStr);
        }
    }
    refreshView();
}

void GiftGiveHistoryView::refreshView(){
    m_tabView->reloadData();
}

bool GiftGiveHistoryView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_historyNode", CCNode*, this->m_historyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_historyBtn", CCControlButton*, this->m_historyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBtn", CCControlButton*, this->m_infoBtn);
    return false;
}
