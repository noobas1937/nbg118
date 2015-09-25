//
//  AllianceApplyView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-12.
//
//

#include "AllianceApplyView.h"

bool AllianceApplyView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_background", CCScale9Sprite*, this->m_background);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showUserBtn", CCControlButton*, this->m_showUserBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_applyNode", CCNode*, this->m_applyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tip", CCLabelIF*, this->m_tip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    return true;
}

SEL_CCControlHandler AllianceApplyView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
}

bool AllianceApplyView::init(){
    
    bool ret = false;
    if(PopupBaseView::init())
    {
        auto node = CCBLoadFile("AllianceInfoMembersView", this, this);
        setContentSize(node->getContentSize());
        m_loadingIcon = NULL;
        
        int preHeight = this->m_background->getContentSize().height;
        changeBGHeight(m_background);
        int dh = m_background->getContentSize().height - preHeight;
        m_applyNode->setVisible(false);
        
        AllianceApplylistCommand* cmd = new AllianceApplylistCommand(1);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceApplyView::getUseApplyList), NULL));
        cmd->sendAndRelease();
        
        dh += 90;
        this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
        m_listContainer->setPositionY(m_listContainer->getPositionY()-dh);
        m_btnNode->setVisible(false);
        
        m_tableView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tableView->setDirection(kCCScrollViewDirectionVertical);
        m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tableView->setMultiColTableViewDelegate(this);
        m_tableView->setTouchPriority(Touch_Popup);
        m_listContainer->addChild(m_tableView);
        
        m_tip->setString(_lang("115172"));
        ret = true;
    }
    return ret;
}

void AllianceApplyView::getUseApplyList(CCObject* obj){
    updateApplyList(NULL);
    int num = m_members.size();
    m_tip->setVisible(num<=0);
    m_applyNode->setVisible(false);
    m_tableView->setViewSize(CCSize(m_listContainer->getContentSize().width,m_listContainer->getContentSize().height));
    m_tableView->reloadData();
}

bool AllianceApplyView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    return true;
}

void AllianceApplyView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}
void AllianceApplyView::onEnter(){
    PopupBaseView::onEnter();
    if(m_isApply){
        this->setTitleName(_lang("115043"));
    }else{
        this->setTitleName(_lang("115027"));
        this->getUseApplyList(NULL);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceApplyView::updateApplyList), MSG_INVITES_DATA, NULL);
}

void  AllianceApplyView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INVITES_DATA);
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

void AllianceApplyView::updateMemberList(CCObject* data){
    int num = m_members.size();
    CCString* strUid = dynamic_cast<CCString*>(data);
    if(strUid==NULL) return ;
    std::string uid = strUid->getCString();
    std::vector<AllianceInfoMember> ::iterator it;
    for (it = m_members.begin(); it!=m_members.end(); it++) {
        if(it->getUid()==uid){
            m_members.erase(it);
            break;
        }
    }
    m_tableView->reloadData();
}

void AllianceApplyView::updateApplyList(CCObject* data){
    map<string,PlayerInfo*> ::iterator it;
    m_members.clear();
    for(it = AllianceManager::getInstance()->applyUserList.begin(); it != AllianceManager::getInstance()->applyUserList.end(); it++){
        AllianceInfoMember infoMember;
        infoMember.setRank(it->second->rank);
        infoMember.setName(it->second->name);
        infoMember.setPoints(0);
        infoMember.setKills(0);
        infoMember.setPic(it->second->getPic());
        infoMember.setUid(it->second->uid);
        infoMember.setPower(it->second->power);
        infoMember.setIsManager(true);
        m_members.push_back(infoMember);
    }
    m_tableView->reloadData();
}

void AllianceApplyView::addLoadingAni(){
    m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
    m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
    auto cSize = m_listContainer->getContentSize();
    m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    m_listContainer->addChild(m_loadingIcon,1000000);
}

void AllianceApplyView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize AllianceApplyView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(640, 148);
}

CCTableViewCell* AllianceApplyView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_members.size()){
        return NULL;
    }
    AllianceApplyCell* cell = (AllianceApplyCell*)table->dequeueGrid();
    if(idx <m_members.size()){
        if(cell){
            cell->bind(&m_members[idx], "");
        }else{
            cell = AllianceApplyCell::create();
            cell->bind(&m_members[idx], "");
        }
    }
    return cell;
}

ssize_t AllianceApplyView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return m_members.size();
}

unsigned int AllianceApplyView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}

//
bool AllianceApplyCell::init()
{
    bool ret = true;
    
    if (CCTableViewCell::init()) {
        CCNode* node = CCBLoadFile("AllianceInfoMembersCell", this, this);
        CCSize size = node->getContentSize();
        setContentSize(size);
        
        return true;
    }
    
    return ret;
}

SEL_CCControlHandler AllianceApplyCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onManageBtnClick", AllianceApplyCell::onManageBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMailClick", AllianceApplyCell::onMailClick);
    return NULL;
}


void AllianceApplyCell::bind(AllianceInfoMember* member, bool showTitle)
{
    m_info = member;
    CCSprite* portrait = NULL;
    m_rankNode->setVisible(false);
    m_infoNode->setVisible(true);
    
    m_iconPlaceHolder->removeAllChildrenWithCleanup(true);
    portrait = CCLoadSprite::createSprite(member->getPic().c_str());
    CCCommonUtils::setSpriteMaxSize(portrait, 92);
    m_iconPlaceHolder->addChild(portrait);
    CCCommonUtils::setButtonTitle(m_mail, _lang("115030").c_str());
    std::string str = "";
    m_nameLabel->setString(member->getName().c_str());
    str = _lang_1("115029", CC_ITOA(member->getPower()));
    m_powerLabel->setString(str);
    m_manageBtn->setVisible(true);
    m_manageBtn->setEnabled(true);
    m_mail->setVisible(true);
    m_mail->setEnabled(true);
    if(member->getUid()==GlobalData::shared()->playerInfo.uid){
        m_mail->setVisible(false);
    }
    if(member->getUid()==GlobalData::shared()->playerInfo.uid && GlobalData::shared()->playerInfo.allianceInfo.rank!=5){
        CCCommonUtils::setButtonTitle(m_manageBtn, _lang("115039").c_str());
    }else if(member->getUid()==GlobalData::shared()->playerInfo.uid && GlobalData::shared()->playerInfo.allianceInfo.rank==5){
        CCCommonUtils::setButtonTitle(m_manageBtn, _lang("115037").c_str());
    }else if(GlobalData::shared()->playerInfo.allianceInfo.rank==5){
        CCCommonUtils::setButtonTitle(m_manageBtn, _lang("115038").c_str());
    }else{
        m_manageBtn->setVisible(false);
    }
    if(m_info->getIsManager()){
        m_manageBtn->setVisible(true);
        CCCommonUtils::setButtonTitle(m_manageBtn, _lang("115044").c_str());
        CCCommonUtils::setButtonTitle(m_mail, _lang("115045").c_str());
        CCCommonUtils::setButtonTitleColor(m_mail, ccRED);
    }
}

void AllianceApplyCell::onManageBtnClick(CCObject *pSender, CCControlEvent event)
{
//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    if(m_info->getIsManager()){
        AcceptAllianceApplyCommand* cmd = new AcceptAllianceApplyCommand(m_info->getUid());
        //cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceApplyCell::sendToServerSuccess), NULL));
        cmd->sendAndRelease();
        m_manageBtn->setEnabled(false);
        return ;
    }
}



void AllianceApplyCell::leavelSuccess(){
    PopupViewController::getInstance()->removeAllPopupView();
}


void AllianceApplyCell::onMailClick(CCObject *pSender, CCControlEvent event)
{
//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    if(m_info->getIsManager()){
        RefuseAllianceApplyCommand* cmd = new RefuseAllianceApplyCommand(m_info->getUid());
        cmd->sendAndRelease();
        m_mail->setEnabled(false);
        return ;
    }
}

void AllianceApplyCell::sendToServerSuccess(){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_MEMBER_DATA,CCString::create(m_info->getUid()));
}

bool AllianceApplyCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconPlaceHolder", CCNode*, this->m_iconPlaceHolder);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerLabel", CCLabelIF*, this->m_powerLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_manageBtn", CCControlButton*, this->m_manageBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mail", CCControlButton*, this->m_mail);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankNode", CCNode*, this->m_rankNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
    return false;
}
