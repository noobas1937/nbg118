//
//  MODView.cpp
//  IF
//
//  Created by 李锐奇 on 14/11/19.
//
//

#include "MODView.h"
#include "PopupViewController.h"
#include "ModController.h"
#include "WorldController.h"
#include "MODCommand.h"
#include "FunBuildController.h"
#include "ParticleController.h"

void MODView::onEnter(){
    PopupBaseView::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    setTitleName(_lang("132005"));
}

void MODView::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    PopupBaseView::onExit();
}

bool MODView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        auto ccb = CCBLoadFile("MODView", this, this);
        this->setContentSize(ccb->getContentSize());
        CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("132008").c_str());
        CCCommonUtils::setButtonTitle(m_managerBtn, _lang("132009").c_str());
        CCCommonUtils::setButtonTitle(m_listBtn, _lang("132024").c_str());

        if(GlobalData::shared()->playerInfo.gmFlag != 4){
            m_managerBtn->setVisible(false);
            m_listBtn->setVisible(false);
        }
        int addH = CCDirector::sharedDirector()->getWinSize().height - 852;
        isSendRequest = false;
        isGoldInfoBack = false;
        m_introText1->setString(_lang("132010"));
        int textH = m_introText1->getOriginScaleY() * m_introText1->getContentSize().height;
        m_introText2->setString(_lang("132011"));
        m_introText2->setPositionY(m_introText1->getPositionY() - textH - 40);
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - addH);
        m_introText1->setPositionY(m_introText1->getPositionY() - addH);
        m_introText2->setPositionY(m_introText2->getPositionY() - addH);
        m_bgContainer->setPositionY(m_bgContainer->getPositionY() - addH);
        addBG();
//        for (int i=1; i<=4; i++) {
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
//            m_fireNode1->addChild(particle);
//            
//            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
//            m_fireNode2->addChild(particle1);
//        }

        getRewardData();
        update(1.0f);
        ret = true;
    }
    return ret;
}

bool MODView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardBtn", CCControlButton*, m_rewardBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_managerBtn", CCControlButton*, m_managerBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listBtn", CCControlButton*, m_listBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_introText1", CCLabelIF*, m_introText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_introText2", CCLabelIF*, m_introText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, m_fireNode2);

    return false;
}

SEL_CCControlHandler MODView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickReward", MODView::onClickReward);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMODManager", MODView::onClickMODManager);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMODList", MODView::onClickMODList);

    return NULL;
}

void MODView::onClickReward(CCObject * pSender, Control::EventType pCCControlEvent){
    time_t dTime = ModController::getInstance()->nextRewardTime - WorldController::getInstance()->getTime();
    if(dTime < 0 && !isSendRequest){
        MODRewardCommand *cmd = new MODRewardCommand();
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(MODView::getRewardBack), NULL));
        cmd->sendAndRelease();
        isSendRequest = true;
    }
}

void MODView::onClickMODManager(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(MODManagerPopUpView::create(0));
}

void MODView::onClickMODList(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(MODManagerPopUpView::create(1));
}

void MODView::update(float time){
    m_rewardBtn->setEnabled(false);
    if(!isGoldInfoBack){
        return;
    }
    if(ModController::getInstance()->nextRewardTime == 0){
        m_timeText->setString("");
    }else{
        time_t dTime = ModController::getInstance()->nextRewardTime - WorldController::getInstance()->getTime();
        if (dTime <= 0) {
            m_rewardBtn->setEnabled(true);
            m_timeText->setString("");
        }else{
            m_timeText->setString(_lang_2("132006", CCCommonUtils::timeLeftToCountDown(dTime / 1000).c_str(), CC_ITOA(ModController::getInstance()->rewardGold)));
        }
    }
}

void MODView::addBG(){
//    std::string picName = "technology_09.png";
//    auto totalH = CCDirector::sharedDirector()->getWinSize().height;
//    int h = 0;
//    auto batch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource(picName.c_str())->getTexture());
//    m_bgContainer->addChild(batch);
//    while (h < totalH) {
//        auto sprite = CCLoadSprite::createSprite(picName.c_str());
//        sprite->setAnchorPoint(ccp(0, 0));
//        batch->addChild(sprite);
//        sprite->setPosition(ccp(0, h));
//        h += sprite->getContentSize().height;
//    }
}

void MODView::getRewardData(){
    MODRewardInfoCommand *cmd = new MODRewardInfoCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(MODView::getRewardDataBack), NULL));
    cmd->sendAndRelease();
}

void MODView::getRewardDataBack(CCObject *obj){
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        return;
    }
    isGoldInfoBack = true;
    auto params = _dict(ret->getData());
    double lastModGoldGetTime = params->valueForKey("lastModGoldGetTime")->doubleValue();
    double modGoldAmount = params->valueForKey("modGoldAmount")->doubleValue();
    double modGoldGetTimeInterval = params->valueForKey("modGoldGetTimeInterval")->doubleValue();
    ModController::getInstance()->rewardGold = modGoldAmount;
    ModController::getInstance()->nextRewardTime = lastModGoldGetTime + modGoldGetTimeInterval;
    update(1.0f);
}

void MODView::getRewardBack(CCObject *obj){
    getRewardData();
}

//---
MODManagerPopUpView *MODManagerPopUpView::create(int tab) {
    MODManagerPopUpView *ret = new MODManagerPopUpView();
    if(ret && ret->init(tab)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MODManagerPopUpView::onEnter(){
    PopupBaseView::onEnter();
    setTitleName(_lang("132009"));
}

void MODManagerPopUpView::onExit(){
    PopupBaseView::onExit();
}

bool MODManagerPopUpView::init(int tab){
    bool ret = false;
    if(PopupBaseView::init()){
        auto ccb = CCBLoadFile("MODMangerPopUpView", this, this);
        this->setContentSize(ccb->getContentSize());
        m_data = CCArray::create();
        ModController::getInstance()->m_currentSelectServer = -1;
        m_kingdomText->setString(_lang("132021"));
        m_nameText->setString(_lang("132022"));
        m_nameHintText->setString(_lang("132012"));
        CCCommonUtils::setButtonTitle(m_assignmentBtn, _lang("132009").c_str());
        CCCommonUtils::setButtonTitle(m_addBtn, _lang("132009").c_str());
        CCCommonUtils::setButtonTitle(m_listBtn, _lang("132024").c_str());
        int addH = CCDirector::sharedDirector()->getWinSize().height - 852;
//        m_tabNode1->setPositionY(m_tabNode1->getPositionY());
        m_tabNode2->setPositionY(m_tabNode2->getPositionY() - addH);
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - addH);
        m_bgContainer->setPositionY(m_bgContainer->getPositionY() - addH);
        m_infoNode->setVisible(false);
        m_assignmentBtn->setVisible(true);
        
        auto nSize = m_nameInputNode->getContentSize();
        m_nameEdit = CCEditBox::create(nSize, CCLoadSprite::createScale9Sprite("world_title_3.png"));
        m_nameEdit->setFontColor(ccc3(62, 37, 0));
        m_nameEdit->setMaxLength(25);
        m_nameEdit->setReturnType(kKeyboardReturnTypeDone);
        m_nameEdit->setPosition(ccp(nSize.width/2, nSize.height/2));
        m_nameInputNode->addChild(m_nameEdit);

        auto kSize = m_kingdomInputNode->getContentSize();
        m_serverEdit = CCEditBox::create(kSize, CCLoadSprite::createScale9Sprite("world_title_3.png"));
        m_serverEdit->setFontColor(ccc3(62, 37, 0));
        m_serverEdit->setMaxLength(25);
        m_serverEdit->setInputMode(kEditBoxInputModeNumeric);
        m_serverEdit->setReturnType(kKeyboardReturnTypeDone);
        m_serverEdit->setPosition(ccp(kSize.width/2, kSize.height/2));
        m_kingdomInputNode->addChild(m_serverEdit);
        
        m_listContainer->setContentSize(m_listContainer->getContentSize() + Size(0, addH));
        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(1);
        m_listContainer->addChild(m_tabView);

        m_scrollView = CCScrollView::create(m_modContainer->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_modContainer->addChild(m_scrollView);
//        m_serverListText->setString(_lang("132023"));
//        m_modListText->setString(_lang("132024"));
        addBG();
//        for (int i=1; i<=4; i++) {
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
//            m_fireNode1->addChild(particle);
//            
//            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
//            m_fireNode2->addChild(particle1);
//        }
        isFirstShow = true;
        showTab(tab);
        ret = true;
    }
    return ret;
}

bool MODManagerPopUpView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingdomText", CCLabelIF*, m_kingdomText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameHintText", CCLabelIF*, m_nameHintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingdomInputNode", CCNode*, m_kingdomInputNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameInputNode", CCNode*, m_nameInputNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_assignmentBtn", CCControlButton*, m_assignmentBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listBtn", CCControlButton*, m_listBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn", CCControlButton*, m_addBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, m_bottomNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverListText", CCLabelIF*, m_serverListText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_modListText", CCLabelIF*, m_modListText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_modContainer", CCNode*, m_modContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabNode1", CCNode*, m_tabNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabNode2", CCNode*, m_tabNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_searchNameText", CCLabelIF*, m_searchNameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverTxt", CCLabelIF*, m_serverTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelTxt", CCLabelIF*, m_levelTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expTxt", CCLabelIF*, m_expTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expBar", CCScale9Sprite*, m_expBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_head", CCNode*, m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, m_fireNode2);

    return false;
}

SEL_CCControlHandler MODManagerPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickAssignment", MODManagerPopUpView::onClickAssignment);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSearch", MODManagerPopUpView::onClickSearch);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", MODManagerPopUpView::onAddClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onListClick", MODManagerPopUpView::onListClick);

    return NULL;
}

void MODManagerPopUpView::onClickAssignment(CCObject * pSender, Control::EventType pCCControlEvent){
    if(!m_currentInfo){
        return;
    }
    if(!checkServerAndName()){
        return;
    }

    std::string uid = m_currentInfo->uid;
    int kingdom = m_currentInfo->selfServerId;

    MODSetCommand *cmd = new MODSetCommand(kingdom, uid);
    cmd->sendAndRelease();
}

void MODManagerPopUpView::onClickSearch(CCObject * pSender, Control::EventType pCCControlEvent){
    if(!checkServerAndName()){
        return;
    }
    std::string nameStr = m_nameEdit->getText();
    std::string kingdomStr = m_serverEdit->getText();
    int kingdom = atoi(kingdomStr.c_str());

    MODGetUserInfoCommand *cmd = new MODGetUserInfoCommand(kingdom, nameStr);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(MODManagerPopUpView::searchBack), NULL));
    cmd->sendAndRelease();
}

bool MODManagerPopUpView::checkServerAndName(){
    std::string nameStr = m_nameEdit->getText();
    std::string kingdomStr = m_serverEdit->getText();
    if(nameStr == ""){
        CCCommonUtils::flyHint("", "", _lang("132014"));
        return false;
    }
    if(kingdomStr == ""){
        CCCommonUtils::flyHint("", "", _lang("132013"));
        return false;
    }
    int kingdom = atoi(kingdomStr.c_str());
    if(kingdom == 0){
        CCCommonUtils::flyHint("", "", _lang("132013"));
        return false;
    }
    return true;
}

void MODManagerPopUpView::onAddClick(CCObject * pSender, Control::EventType pCCControlEvent){
    showTab(0);
}

void MODManagerPopUpView::onListClick(CCObject * pSender, Control::EventType pCCControlEvent){
    showTab(1);
}

void MODManagerPopUpView::searchBack(CCObject *obj){
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        return;
    }
    
    m_infoNode->setVisible(true);
    m_assignmentBtn->setVisible(true);
    auto params = _dict(ret->getData());
    m_currentInfo = new PlayerInfo();
    m_currentInfo->updateInfo(params);
    
    std::string leagueShort = "";
    if(m_currentInfo->isInAlliance()){
        leagueShort = std::string("(") + m_currentInfo->allianceInfo.shortName + ")";
    }

    if(m_currentInfo->uid == GlobalData::shared()->playerInfo.uid){
        m_serverTxt->setString(_lang("108755") + CC_ITOA(GlobalData::shared()->playerInfo.selfServerId));
        m_searchNameText->setString(leagueShort + GlobalData::shared()->playerInfo.name.c_str());
    }else{
        m_searchNameText->setString(leagueShort + m_currentInfo->name.c_str());
        m_serverTxt->setString(_lang("108755") + CC_ITOA(m_currentInfo->selfServerId));
    }
    auto pic = CCLoadSprite::createSprite(m_currentInfo->getPic().c_str());
    if (pic) {
        m_head->addChild(pic);
        pic->setPosition(ccp(2,-2));
        pic->setScale(92/pic->getContentSize().width);
    }
    std::string str = _lang("102203")+" ";
    str.append(CC_ITOA(m_currentInfo->level));
    m_levelTxt->setString(str.c_str());
    
    str = "";
    str.append(CC_CMDITOA(m_currentInfo->exp));
    str.append("/");
    str.append(CC_CMDITOA(m_currentInfo->maxExp));
    m_expTxt->setString(str.c_str());

    int maxExp = m_currentInfo->maxExp;
    if(maxExp<=0) maxExp = 1;
    float len = m_currentInfo->exp*1.0/maxExp;
    if(len>1) len = 1;
    m_expBar->setContentSize(CCSize(len*435,23));

    CC_SAFE_RELEASE(m_currentInfo);
}

void MODManagerPopUpView::getData(){
    
}

void MODManagerPopUpView::getDataBack(CCObject *obj){
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        return;
    }
    
    auto params = _dict(ret->getData());
    m_scrollView->getContainer()->removeAllChildren();
    auto arr = dynamic_cast<CCArray*>(params->objectForKey("result"));
    int startY = 0;

    if(arr){
        CCObject *obj;
        CCARRAY_FOREACH(arr, obj) {
            PlayerInfo *info = new PlayerInfo();
            info->updateInfo(_dict(obj));
            MODCell *cell = MODCell::create(info);
            m_scrollView->addChild(cell);
            cell->setPosition(0, startY);
            startY += 145;
            CC_SAFE_RELEASE(info);
        }
    }
    auto size = this->m_modContainer->getContentSize();
    m_scrollView->setContentSize(CCSize(size.width, startY));
    m_scrollView->setContentOffset(ccp(0, size.height) - ccp(0, startY));
}

void MODManagerPopUpView::showTab(int tab){
    if(m_currentTab == tab){
        return;
    }
    m_currentTab = tab;
    if(m_currentTab == 0){//添加tab
        m_tabNode1->setVisible(true);
        m_tabNode2->setVisible(false);
        m_addBtn->setEnabled(false);
        m_listBtn->setEnabled(true);
        m_modContainer->setVisible(false);
        m_listContainer->setVisible(false);
    }else{//列表tab
        m_tabNode1->setVisible(false);
        m_tabNode2->setVisible(true);
        m_addBtn->setEnabled(true);
        m_listBtn->setEnabled(false);
        m_listContainer->setVisible(true);
        m_modContainer->setVisible(true);
        m_tabView->reloadData();
    }
}

void MODManagerPopUpView::addBG(){
//    std::string picName = "technology_09.png";
//    auto totalH = CCDirector::sharedDirector()->getWinSize().height;
//    int h = 0;
//    auto batch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource(picName.c_str())->getTexture());
//    m_bgContainer->addChild(batch);
//    while (h < totalH) {
//        auto sprite = CCLoadSprite::createSprite(picName.c_str());
//        sprite->setAnchorPoint(ccp(0, 0));
//        batch->addChild(sprite);
//        sprite->setPosition(ccp(0, h));
//        h += sprite->getContentSize().height;
//    }
}

void MODManagerPopUpView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    if(m_currentTab == 1){
        ServerCell* wCell = dynamic_cast<ServerCell*>(cell);
        if(wCell == NULL){
            return;
        }
        wCell->getData(this);
    }
}

cocos2d::CCSize MODManagerPopUpView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSizeMake(210, 60);
}

CCTableViewCell* MODManagerPopUpView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    auto &serverList = WorldController::getInstance()->m_serverList;
    if(idx >= serverList.size()){
        return NULL;
    }
    ServerCell* cell = (ServerCell*)table->dequeueGrid();
    if(idx < serverList.size()){
        ServerListInfo *info = &serverList[idx];
        if(cell){
            cell->setData(info);
        }else{
            cell = ServerCell::create(info);
            if(idx == 0 && isFirstShow){
                isFirstShow = false;
                cell->getData(this);
            }
        }
    }
    return cell;
}

ssize_t MODManagerPopUpView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = ceil(WorldController::getInstance()->m_serverList.size() * 1.0f / SERVER_PER_LINE);
    return num;
}

unsigned int MODManagerPopUpView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return SERVER_PER_LINE;
}

void MODManagerPopUpView::generateData(){
    m_data->removeAllObjects();
}

//----
ServerCell *ServerCell::create(ServerListInfo *info){
    ServerCell *ret = new ServerCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ServerCell::setData(ServerListInfo *info){
    m_info = info;
    refreshView();
}

void ServerCell::onEnter(){
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ServerCell::resetBtnState), REFRESH_BUTTON_STATE, NULL);
}

void ServerCell::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, REFRESH_BUTTON_STATE);
    CCNode::onExit();
}

bool ServerCell::init(){
    auto bg = CCLoadSprite::createScale9Sprite("btn_04grey.png");
    m_btn = CCControlButton::create(bg);
    auto selectBG = CCLoadSprite::createScale9Sprite("btn_04.png");
    m_btn->setBackgroundSpriteForState(selectBG, CCControlStateDisabled);
    this->addChild(m_btn);
    auto size = CCSize(200, 60);
    m_btn->setAnchorPoint(ccp(0.5, 0.5));
    m_btn->setPreferredSize(size);
    m_btn->setPosition(size / 2 + Size(10, 0));
    this->setContentSize(size);
    m_btn->setTouchEnabled(false);
//    m_btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ServerCell::onClick), CCControlEventTouchUpInside);

    refreshView();
    return true;
}

void ServerCell::getData(MODManagerPopUpView *panel){
    MODGetServerModInfoCommand *cmd = new MODGetServerModInfoCommand(m_info->serverId);
    cmd->setCallback(CCCallFuncO::create(panel, callfuncO_selector(MODManagerPopUpView::getDataBack), NULL));
    cmd->sendAndRelease();
    ModController::getInstance()->m_currentSelectServer = m_info->serverId;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REFRESH_BUTTON_STATE);
    this->m_btn->setEnabled(false);
}

void ServerCell::onClick(cocos2d::CCObject *pTarget, CCControlEvent touchEvent){
    CCNode *node = this;
    MODManagerPopUpView *p = NULL;
    while (node) {
        if(dynamic_cast<MODManagerPopUpView*>(node)){
            p = dynamic_cast<MODManagerPopUpView*>(node);
            break;
        }
        node = node->getParent();
    }
    if(!p){
        return;
    }
    getData(p);
}

void ServerCell::refreshView(){
    CCCommonUtils::setButtonTitle(m_btn, m_info->serverName.c_str());
    resetBtnState(NULL);
}

void ServerCell::resetBtnState(CCObject* obj){
    if(ModController::getInstance()->m_currentSelectServer == m_info->serverId){
        m_btn->setEnabled(false);
    }else{
        m_btn->setEnabled(true);
    }
}

//-----

MODCell *MODCell::create(PlayerInfo *info){
    MODCell *ret = new MODCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MODCell::onEnter(){
    CCNode::onEnter();
}

void MODCell::onExit(){
    CCNode::onExit();
}

bool MODCell::init(){
    CCBLoadFile("MODCellCCB", this, this);
    
    std::string leagueShort = "";
    if(m_info->isInAlliance()){
        leagueShort = std::string("(") + m_info->allianceInfo.shortName + ")";
    }
    
    if(m_info->uid == GlobalData::shared()->playerInfo.uid){
        m_serverTxt->setString(_lang("108755") + CC_ITOA(GlobalData::shared()->playerInfo.selfServerId));
        m_searchNameText->setString(leagueShort + GlobalData::shared()->playerInfo.name.c_str());
    }else{
        m_searchNameText->setString(leagueShort + m_info->name.c_str());
        m_serverTxt->setString(_lang("108755") + CC_ITOA(m_info->selfServerId));
    }
    auto pic = CCLoadSprite::createSprite(m_info->getPic().c_str());
    if (pic) {
        m_head->addChild(pic);
        pic->setPosition(ccp(2,-2));
        pic->setScale(92/pic->getContentSize().width);
    }
    std::string str = _lang("102203")+" ";
    str.append(CC_ITOA(m_info->level));
    m_levelTxt->setString(str.c_str());
    
    str = "";
    str.append(CC_CMDITOA(m_info->exp));
    str.append("/");
    str.append(CC_CMDITOA(m_info->maxExp));
    m_expTxt->setString(str.c_str());
    
    int maxExp = m_info->maxExp;
    if(maxExp<=0) maxExp = 1;
    float len = m_info->exp*1.0/maxExp;
    if(len>1) len = 1;
    m_expBar->setContentSize(CCSize(len*435,23));

    return true;
}

bool MODCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_head", CCNode*, m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_searchNameText", CCLabelIF*, m_searchNameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverTxt", CCLabelIF*, m_serverTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelTxt", CCLabelIF*, m_levelTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expTxt", CCLabelIF*, m_expTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expBar", CCScale9Sprite*, m_expBar);
    return false;
}