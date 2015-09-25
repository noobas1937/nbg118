//
//  PlayerInfoBtnPopUpView.cpp
//  IF
//
//  Created by lifangkai on 14-9-15.
//
//

#include "PlayerInfoBtnPopUpView.h"
#include "PopupViewController.h"
#include "ChatUnLockCommand.h"
#include "ChatLockCommand.h"
#include "YesNoDialog.h"
#include "MailWritePopUpView.h"
#include "RoleInfoView.h"
#include "MailWritePopUpView.h"
#include "GetUserInfoCommand.h"
#include "ChatController.h"
#include "AllianceKickMemberCommand.h"
#include "MailController.h"
#include "TipsView.h"
PlayerInfoBtnPopUpView* PlayerInfoBtnPopUpView::create(std::string checkName) {
    auto ret = new PlayerInfoBtnPopUpView();
    if (ret && ret->init(checkName)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PlayerInfoBtnPopUpView::init(std::string checkName) {
    bool ret = false;
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("PlayerInfoBtnView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        m_bCheckName = checkName;
        ret = true;

//        CCCommonUtils::setButtonTitle(m_copyBtn, _lang("105304").c_str());
//        CCCommonUtils::setButtonTitle(m_mailBtn, _lang("105308").c_str());
//        CCCommonUtils::setButtonTitle(m_viewBtn, _lang("105309").c_str());
//        CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("cancel_btn_label").c_str());
//        CCCommonUtils::setButtonTitle(m_inviteBtn, _lang("108584").c_str());
//        CCCommonUtils::setButtonTitle(m_joinBtn, _lang("115020").c_str());
    }
    return ret;
}
void PlayerInfoBtnPopUpView::InitBtns(AllianceDonateInfo* info){
    m_info = info;
    m_uid = m_info->uid;
    m_name = m_info->name;
    InitCommonBtns(m_name,m_uid);
//    dy-=70;
    if(GlobalData::shared()->playerInfo.allianceInfo.rank>=4&&m_info->rank<GlobalData::shared()->playerInfo.allianceInfo.rank) {
        m_dy-=70;
        m_kickOutBtn= CCControlButton::create(CCLoadSprite::createScale9Sprite("but_blue.png"));
        m_totalNode->addChild(m_kickOutBtn);
        m_kickOutBtn->setPreferredSize(CCSize(400, 60));
        m_kickOutBtn->setAnchorPoint(ccp(0.5, 0.5));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_dy-=168;
            m_kickOutBtn->setScale(2.4);
        }
        m_kickOutBtn->setPositionY(m_dy);
        m_kickOutBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(PlayerInfoBtnPopUpView::onKickOutBtnClick), CCControlEventTouchUpInside);
        CCCommonUtils::setButtonTitle(m_kickOutBtn, _lang("115111").c_str());
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        int hg = -m_dy+72;
        m_totalNode->setPositionY((960-hg)/2+hg);
    }
    else {
        int hg = -m_dy+30;
        m_totalNode->setPositionY((400-hg)/2+hg);
    }
}
void PlayerInfoBtnPopUpView::InitCommonBtns(string name,string uid,bool isMail){
    m_uid = uid;
    m_name = name;
    m_nameLabel->setString(m_name);
    m_dy = -30;
    if (CCCommonUtils::isIosAndroidPad()) {
        m_dy = -72;
    }
    if(isMail){
        m_mailBtn= CCControlButton::create(CCLoadSprite::createScale9Sprite("but_blue.png"));
        m_totalNode->addChild(m_mailBtn);
        m_mailBtn->setPreferredSize(CCSize(400, 60));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_mailBtn->setScale(2.4);
        }
        m_mailBtn->setAnchorPoint(ccp(0.5, 0.5));
        m_mailBtn->setPositionY(m_dy);
        m_mailBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(PlayerInfoBtnPopUpView::onMailBtnClick), CCControlEventTouchUpInside);

        CCCommonUtils::setButtonTitle(m_mailBtn, _lang("105308").c_str());
        m_dy-=70;
        if (CCCommonUtils::isIosAndroidPad()) {
            m_dy-=168;
        }
    }
    
    m_viewBtn= CCControlButton::create(CCLoadSprite::createScale9Sprite("but_blue.png"));
    m_totalNode->addChild(m_viewBtn);
    m_viewBtn->setPreferredSize(CCSize(400, 60));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_viewBtn->setScale(2.4);
    }
    m_viewBtn->setAnchorPoint(ccp(0.5, 0.5));
    m_viewBtn->setPositionY(m_dy);
    m_viewBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(PlayerInfoBtnPopUpView::onViewBtnClick), CCControlEventTouchUpInside);
    CCCommonUtils::setButtonTitle(m_viewBtn, _lang("105309").c_str());
    m_dy-=70;
    if (CCCommonUtils::isIosAndroidPad()) {
        m_dy-=168;
    }
    m_blockBtn= CCControlButton::create(CCLoadSprite::createScale9Sprite("but_blue.png"));
    m_totalNode->addChild(m_blockBtn);
    m_blockBtn->setPreferredSize(CCSize(400, 60));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_blockBtn->setScale(2.4);
    }
    m_blockBtn->setAnchorPoint(ccp(0.5, 0.5));
    m_blockBtn->setPositionY(m_dy);
    m_blockBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(PlayerInfoBtnPopUpView::onBlockBtnClick), CCControlEventTouchUpInside);

    // ShieldInfo* shieldInfo = GlobalData::shared()->chatShieldInfo.getShieldInfo(m_uid);m_kickOutBtn
    if (GlobalData::shared()->chatShieldInfo.isShield(m_uid)) {
        CCCommonUtils::setButtonTitle(m_blockBtn, _lang("105315").c_str());
    }else{
        CCCommonUtils::setButtonTitle(m_blockBtn, _lang("105312").c_str());
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        int hg = -m_dy+72;
        m_totalNode->setPositionY((960-hg)/2+hg);
    }
    else {
        int hg = -m_dy+30;
        m_totalNode->setPositionY((400-hg)/2+hg);
    }
}
void PlayerInfoBtnPopUpView::onEnter() {
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void PlayerInfoBtnPopUpView::onExit() {
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool PlayerInfoBtnPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_bg, pTouch)) {
        return false;
    }
    return true;
}

void PlayerInfoBtnPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    closeSelf();
    return;
}

SEL_CCControlHandler PlayerInfoBtnPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCopyBtnClick", ChatFunView::onCopyBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMailBtnClick", ChatFunView::onMailBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onViewBtnClick", ChatFunView::onViewBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBlockBtnClick", ChatFunView::onBlockBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelBtnClick", ChatFunView::onCancelBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInviteClick", ChatFunView::onInviteClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onJoinClick", ChatFunView::onJoinClick);
    return NULL;
}

bool PlayerInfoBtnPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_copyBtn", CCControlButton*, m_copyBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBtn", CCControlButton*, m_mailBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBtn", CCControlButton*, m_viewBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_blockBtn", CCControlButton*, m_blockBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn", CCControlButton*, m_cancelBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inviteBtn", CCControlButton*, m_inviteBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_joinBtn", CCControlButton*, m_joinBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleBg", CCNode*, m_titleBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, m_totalNode);
    return false;
}
void PlayerInfoBtnPopUpView::onJoinClick(CCObject * pSender, Control::EventType pCCControlEvent){
//    GetUserInfoCommand* cmd = new GetUserInfoCommand(m_uid);
//    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatFunView::joinFunc), NULL));
//    cmd->sendAndRelease();
}
void PlayerInfoBtnPopUpView::joinFunc(CCObject* param){

}
void PlayerInfoBtnPopUpView::onInviteClick(CCObject * pSender, Control::EventType pCCControlEvent){
//    ChatController::getInstance()->idx = m_idx;
//    auto& info = ChatController::getInstance()->m_chatCountryPool[m_idx];
//    InvitesAllianceCommand* cmd = new InvitesAllianceCommand(info.uid);
//    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatFunView::inviteFunc), NULL));
//    cmd->sendAndRelease();
//    PopupViewController::getInstance()->removePopupView(this);
}
void PlayerInfoBtnPopUpView::inviteFunc(){
//    int idx = ChatController::getInstance()->idx;
//    string name = ChatController::getInstance()->m_chatCountryPool[idx].name;
//    string msg = _lang_1("115182", name.c_str());
//    msg.append("  (").append(_lang("115181")).append(")");CCLOG("invite %s",msg.c_str());
//    ChatController::getInstance()->sendCountryChat(msg.c_str(), CHAT_ALLIANCE, 2);
}
void PlayerInfoBtnPopUpView::onKickOutBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    YesNoDialog::showVariableTitle(_lang_1("115041",m_info->name.c_str()),CCCallFunc::create(this, callfunc_selector(PlayerInfoBtnPopUpView::kickOutFun)),_lang("confirm").c_str());
}
void PlayerInfoBtnPopUpView::kickOutFun(){
    string msg = _lang_1("115187", m_name.c_str());
    msg.append("  (").append(_lang("115181")).append(")");
    ChatController::getInstance()->sendCountryChat(msg.c_str(), CHAT_ALLIANCE, 2);
    
    AllianceKickMemberCommand * command = new AllianceKickMemberCommand();
    command->putParam("playerId", CCString::create(m_info->uid));
    command->sendAndRelease();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_DATA_CHANGE,CCString::create(m_info->uid));
    PopupViewController::getInstance()->removePopupView(this);
}
void PlayerInfoBtnPopUpView::onMailBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(!m_bCheckName.empty() && m_bCheckName.compare(m_name)!=0){
        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("105717")));
    }else{
        std::string userName = m_name;
        PopupViewController::getInstance()->removePopupView(this);
        MailController::getInstance()->openMailDialogViewFirst(m_name, m_uid);
        //PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create(userName, ""));
    }
}

void PlayerInfoBtnPopUpView::onViewBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_uid=="") {
        PopupViewController::getInstance()->removeAllPopupView();
        return;
    }
    if(m_uid==GlobalData::shared()->playerInfo.uid){
        PopupViewController::getInstance()->removeAllPopupView();
        PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(&GlobalData::shared()->playerInfo,1));
    }else{
        if(!m_bCheckName.empty() && m_bCheckName.compare(m_name)!=0){
            PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("105716")));
        }else{
            PopupViewController::getInstance()->removePopupView(this);
            RoleInfoView::createInfoByUid(m_uid);
//            GetUserInfoCommand* cmd = new GetUserInfoCommand(m_uid);
//            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PlayerInfoBtnPopUpView::callFunc), NULL));
//            cmd->sendAndRelease();
        }
    }
}

void PlayerInfoBtnPopUpView::callFunc(CCObject* param){
    NetResult* net = dynamic_cast<NetResult*>(param);
    if(net==NULL) return ;
    CCDictionary* dic = dynamic_cast<CCDictionary*>(net->getData());
    if(dic==NULL) return ;
    PlayerInfo* info = new PlayerInfo();
    info->updateInfo(dic);
    CCDictionary* allianceInfo = _dict(dic->objectForKey("alliance"));
    CCDictionary* allianceInfo1 = _dict(allianceInfo->objectForKey("alliance"));
    if (info)
    {
        info->allianceInfo.updateAllianceInfo(allianceInfo1);
        info->setAllianceId(allianceInfo1->valueForKey("uid")->getCString());
    }
    PopupViewController::getInstance()->removePopupView(this);
    PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(info,1));
    info->release();
}

void PlayerInfoBtnPopUpView::onBlockBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    //    YesNoDialog::show(_lang("E100008").c_str(), [](){});
    //    return;
    if (m_uid=="") {
        PopupViewController::getInstance()->removePopupView(this);
        return;
    }
    string uuid= GlobalData::shared()->chatShieldInfo.getUuid(m_uid);
    if (GlobalData::shared()->chatShieldInfo.isShield(m_uid)) {
        ChatUnLockCommand* cmd = new ChatUnLockCommand(uuid);
        cmd->sendAndRelease();
        PopupViewController::getInstance()->removePopupView(this);
    }
    else {
        auto dialog = YesNoDialog::show( _lang_1("105313",m_name.c_str()) , CCCallFunc::create(this, callfunc_selector(PlayerInfoBtnPopUpView::onYesBlock)));
        dialog->showCancelButton();
    }
}

void PlayerInfoBtnPopUpView::onYesBlock()
{
    if(m_uid!="") {
       // ShieldInfo* info = GlobalData::shared()->chatShieldInfo.getShieldInfo(m_uid);
        ChatLockCommand* cmd = new ChatLockCommand(m_uid);
        cmd->sendAndRelease();
    }
    PopupViewController::getInstance()->removePopupView(this);
}

void PlayerInfoBtnPopUpView::onCancelBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}