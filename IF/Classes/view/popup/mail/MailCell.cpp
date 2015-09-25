//
//  MailCell.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-11.
//
//
#include "CCSafeNotificationCenter.h"
#include "MailCell.h"
#include "PopupViewController.h"
#include "MailReadPopUpView.h"
#include "MailController.h"
#include "MailGetRewardCommand.h"
#include "UIComponent.h"
#include "MailSaveCommand.h"
#include "BattleReportMailPopUpView.h"
#include "ResourceMailPopUpView.h"
#include "MailPopUpView.h"
#include "DetectMailPopUpView.h"
#include "OccupyMailPopUpView.h"
#include "ExplorePopUpView.h"
#include "MailAnnouncePopUp.h"
#include "WorldController.h"
#include "RewardController.h"
#include "MailSystemListPopUp.h"
#include "YesNoDialog.h"
#include "MonsterAttackPopUpView.h"
#include "MailDialogView.h"
#include "Utf8Utils.h"
#include "MailResourceCellInfo.h"
#include "MailResourceHelpCellInfo.h"
#include "MailResourcePopUpView.h"
#include "MailResourceHelpView.h"
#include "MailMonsterListView.h"
#include "MailMonsterCellInfo.h"
#include "SoundController.h"
#include "ChatServiceCocos2dx.h"
#include "MailGiftView.h"
#include "ChatMailInfo.h"
#include "WorldBossRewardMailView.h"
#include "WorldBossMailView.h"

static int predeleteNum = 0;
MailCell* MailCell::create(MailInfo *info,CCTableView *tableView){
    MailCell* ret = new MailCell(info,tableView);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailCell::init(){
    m_ccbNode = CCBLoadFile("MailRenderNew", this, this);
    this->setContentSize(m_ccbNode->getContentSize());
    m_headImgNode = HFHeadImgNode::create();
    
    m_readColor = ccc3(60,28,0);//ccc3(122,102,78);
    m_tipLoadingText = CCLabelIF::create();
    if (CCCommonUtils::isIosAndroidPad()) {
        m_tipLoadingText->setFontSize(44);
    }
    else
        m_tipLoadingText->setFontSize(22);
    m_tipLoadingText->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    m_tipLoadingText->setHorizontalAlignment(kCCTextAlignmentLeft);
    m_tipLoadingText->setColor({117,62,15});
    m_tipLoadingText->setString(_lang("115189").c_str());
    this->addChild(m_tipLoadingText,10001,10001);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_tipLoadingText->setPosition(768, 80);
    }
    else
        m_tipLoadingText->setPosition(320, 40);
    m_tipLoadingText->setVisible(false);
    
    m_removeBG->setVisible(false);
    this->m_deleteText->setString(_lang("108523"));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_rightPosX = getContentSize().width - 90;
    }
    else
        m_rightPosX = getContentSize().width - 45;
    m_editNode->setVisible(false);
    this->setData(m_mailInfo,m_tableView);
    m_modelLayer = CCLayerColor::create();
    m_modelLayer->setOpacity(30);
    m_modelLayer->setColor(ccBLACK);
    m_modelLayer->setContentSize(m_ccbNode->getContentSize());
    m_modelLayer->setAnchorPoint(ccp(0, 0));
    
    this->addChild(m_modelLayer);
    m_modelLayer->setVisible(false);
    
    //refreshView();
    return true;
}

void MailCell::onEnter(){
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailCell::onDeleteStatusFlag), MAIL_LIST_DELETERECOVER, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailCell::refreshRewardIcon), MIAL_GET_REWARD_REFRESH, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailCell::showDelState), MAIL_DEL_MSG, NULL);

    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, true);
}

void MailCell::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MIAL_GET_REWARD_REFRESH);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_DELETERECOVER);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_DEL_MSG);
    setTouchEnabled(false);
    CCNode::onExit();
}

bool MailCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailRwardIcon", CCNode*, this->m_mailRwardIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", Label*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);


    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_removeBG", CCSprite*, this->m_removeBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardNode1", CCNode*, this->m_rewardNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unreadNumBG", CCSprite*, this->m_unreadNumBG);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unreadNumText", CCLabelIF*, this->m_unreadNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBG", CCScale9Sprite*, this->m_deleteBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteText", CCLabelIF*, this->m_deleteText);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deletSpr", CCSprite*, this->m_deletSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprSel", CCSprite*, this->m_sprSel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, this->m_editNode);
    return false;
}

void MailCell::showDelState(CCObject* obj)
{
    if(m_mailInfo==NULL){
        return;
    }
//    if (m_mailInfo->tabType==2 || m_mailInfo->tabType==3|| m_mailInfo->tabType==4) {
//        return;
//    }
//    if (m_mailInfo->save==1 || (m_mailInfo->rewardId!=""&&m_mailInfo->rewardStatus==0)) {
//        return;
//    }
    if (MailController::getInstance()->CanRemoveMail) {
        m_editNode->setVisible(true);
        m_deletSpr->setVisible(true);
        m_sprSel->setVisible(false);
        m_editNode->setPositionX(m_rightPosX);
        m_mailRwardIcon->setPositionX(m_rightPosX - 60);
    }
    else {
        m_editNode->setVisible(false);
        m_mailRwardIcon->setPositionX(m_rightPosX);
        m_editNode->setPositionX(m_rightPosX - 60);
    }
}

SEL_CCControlHandler MailCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){

    return NULL;
}

/**邮件cell 点击触发方法*/
void MailCell::showReadPopUp(){
    bool issaveRead = false;
    if(m_mailInfo==NULL){
        return;
    }
    if(m_mailInfo->status==0){
        issaveRead = true;
        if(m_mailInfo->tabType==3||m_mailInfo->tabType==4){
        
        }else if(m_mailInfo->type== MAIL_FRESHER||m_mailInfo->type==MAIL_SYSNOTICE||m_mailInfo->type==MAIL_SYSUPDATE){
            //GlobalData::shared()->mailCountObj.saveR -=1;
        }else if(m_mailInfo->tabType==USERMAIL
                 ||m_mailInfo->type==MAIL_RESOURCE
                 ||m_mailInfo->type==MAIL_ATTACKMONSTER
                 ||m_mailInfo->type==MAIL_RESOURCE_HELP
                 || m_mailInfo->type==MAIL_MOD_PERSONAL
                 || m_mailInfo->type==MAIL_MOD_SEND
                 ){
//                GlobalData::shared()->mailCountObj.perR -=1;
//                MailController::getInstance()->notyfyReadMail(m_mailInfo->uid, m_mailInfo->type);
        }else{
            GlobalData::shared()->mailCountObj.sysR -=1;
            MailController::getInstance()->notyfyReadMail(m_mailInfo->uid, m_mailInfo->type);
        }
        if(m_mailInfo->type==MAIL_SYSUPDATE){
            GlobalData::shared()->mailCountObj.upR-=1;
        }
    }
    
    this->refreshView();
    if(m_mailInfo->tabType == 1){
        m_mailInfo->status = READ;
        if(m_mailInfo->type == MAIL_BATTLE_REPORT){
            PopupViewController::getInstance()->addPopupInViewWithAnim(BattleReportMailPopUpView::create(m_mailInfo));
        }
        else if(m_mailInfo->type == MAIL_RESOURCE){
            PopupViewController::getInstance()->addPopupInViewWithAnim(MailResourcePopUpView::create(dynamic_cast<MailResourceCellInfo*>(m_mailInfo.getObject())));
        }else if(m_mailInfo->type == MAIL_RESOURCE_HELP){
            PopupViewController::getInstance()->addPopupInViewWithAnim(MailResourceHelpView::create(dynamic_cast<MailResourceHelpCellInfo*>(m_mailInfo.getObject())));
        }
        else if(m_mailInfo->type == MAIL_DETECT_REPORT){
            if(m_mailInfo->pointType == MAIL_DETECT_REPORT_PROTECT){
                PopupViewController::getInstance()->addPopupInViewWithAnim(MailReadPopUpView::create(*m_mailInfo), true);
            }else{
                PopupViewController::getInstance()->addPopupInViewWithAnim(DetectMailPopUpView::create(m_mailInfo));
            }
        }else if(m_mailInfo->type==MAIL_DETECT){
            if(m_mailInfo->itemIdFlag == 1){ //反侦察
                PopupViewController::getInstance()->addPopupInViewWithAnim(MailReadPopUpView::create(*m_mailInfo), true);
            }else{
                PopupViewController::getInstance()->addPopupInViewWithAnim(DetectMailPopUpView::create(m_mailInfo));
            }
        }
        else if(m_mailInfo->type == MAIL_ENCAMP){
            PopupViewController::getInstance()->addPopupInViewWithAnim(OccupyMailPopUpView::create(m_mailInfo));
        }
        else if(m_mailInfo->type == WORLD_NEW_EXPLORE){
            
            PopupViewController::getInstance()->addPopupInViewWithAnim(ExplorePopUpView::create(m_mailInfo));
        }else if(m_mailInfo->type==MAIL_SYSUPDATE){
            PopupViewController::getInstance()->addPopupInViewWithAnim(MailAnnouncePopUp::create(m_mailInfo));
        }else if(m_mailInfo->type == MAIL_ATTACKMONSTER){
            PopupViewController::getInstance()->addPopupInViewWithAnim(MailMonsterListView::create(dynamic_cast<MailMonsterCellInfo*>(m_mailInfo.getObject())));
        }else if(m_mailInfo->type == MAIL_GIFT){
            PopupViewController::getInstance()->addPopupInViewWithAnim(MailGiftReadPopUpView::create(*m_mailInfo));
        }else if(m_mailInfo->type == MAIL_WORLD_BOSS){
            PopupViewController::getInstance()->addPopupInViewWithAnim(WorldBossRewardMailView::create(*m_mailInfo), true);
        }else{
            PopupViewController::getInstance()->addPopupInViewWithAnim(MailReadPopUpView::create(*m_mailInfo), true);
//            PopupViewController::getInstance()->addPopupInViewWithAnim(WorldBossRewardMailView::create(*m_mailInfo), true);
//            PopupViewController::getInstance()->addPopupInViewWithAnim(WorldBossMailView::create(*m_mailInfo));
        }
    }
    else if(m_mailInfo->tabType == 2){
        PopupViewController::getInstance()->addPopupInViewWithAnim(MailSystemListPopUp::create(2),true);
    }else if(m_mailInfo->tabType == 3){
        PopupViewController::getInstance()->addPopupInViewWithAnim(MailSystemListPopUp::create(3),true);
    }else if(m_mailInfo->tabType == 4){
        PopupViewController::getInstance()->addPopupInViewWithAnim(MailSystemListPopUp::create(4),true);
    }else if(m_mailInfo->tabType == MAILTAB5){
        PopupViewController::getInstance()->addPopupInViewWithAnim(MailSystemListPopUp::create(MAILTAB5),true);
    }else if(m_mailInfo->tabType==USERMAIL){
        
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        if(ChatServiceCocos2dx::enableNativeMail && GlobalData::shared()->android_native_chat == 1){
            CCLOGFUNCF("m_mailInfo->type %d",m_mailInfo->type);
            if (m_mailInfo->type ==CHAT_ROOM) {
                ChatServiceCocos2dx::setMailInfo(m_mailInfo->crGroupId.c_str(),m_mailInfo->uid.c_str(),m_mailInfo->fromName.c_str(),m_mailInfo->type);
//                if (m_mailInfo->dialogs!=NULL&& m_mailInfo->dialogs->count()<=1){
//                     CCLOGFUNCF("!getHasRequestDataBefore m_mailInfo->fromName.c_str() %s",m_mailInfo->fromName.c_str());
//                    MailController::getInstance()->getChatRoomMsgRecord(false, m_mailInfo->crGroupId);
//                }
                if(!ChatServiceCocos2dx::isChatShowing){
                    CCLOG("!ChatServiceCocos2dx::isChatShowing");
                    ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_CHATROOM;
//                    ChatServiceCocos2dx::m_isInMailDialog=true;
                    ChatServiceCocos2dx::m_isNoticItemUsed=false;
                    MailController::getInstance()->setOldOpenMailInfo(m_mailInfo);
                    ChatServiceCocos2dx::showChatActivityFrom2dx();
                }
            }
            else
            {
                CCArray* mailInfoArr=CCArray::create();
                CCObject* obj;
                CCARRAY_FOREACH(m_mailInfo->dialogs, obj){
                    MailDialogInfo* tempDialogInfo = dynamic_cast<MailDialogInfo*>(obj);
                    ChatMailInfo* info=ChatMailInfo::create(m_mailInfo,tempDialogInfo,true);
                    mailInfoArr->addObject(info);
                }
                CCLOG("showReadPopUp USERMAIL %d,ChatServiceCocos2dx::m_curSendMailIndex:%d",m_mailInfo->dialogs->count(),ChatServiceCocos2dx::m_curSendMailIndex);
                
                ChatServiceCocos2dx::setMailInfo(m_mailInfo->fromUid.c_str(),m_mailInfo->uid.c_str(),m_mailInfo->fromName.c_str(),m_mailInfo->type);
//                if (!ChatServiceCocos2dx::getHasRequestDataBefore(m_mailInfo->fromUid)) {
//                    
//                }
                
                CCLOG("!ChatServiceCocos2dx::getHasRequestDataBefore");
                if(mailInfoArr->count()>20)
                {
                    CCArray* mailArr=CCArray::create();
                    int count=0;
                    for (int i=0; i<mailInfoArr->count(); i++) {
                        if(count==20)
                        {
                            MailController::getInstance()->m_mailInfoSendDic->setObject(mailArr, ChatServiceCocos2dx::m_curSendMailIndex);
                            ChatServiceCocos2dx::notifyMessageIndex(ChatServiceCocos2dx::m_curSendMailIndex,m_mailInfo->fromUid,m_mailInfo->fromName);
                            ChatServiceCocos2dx::m_curSendMailIndex++;
                            mailArr->removeAllObjects();
                            count=0;
                        }
                        mailArr->addObject(mailInfoArr->objectAtIndex(i));
                        count++;
                    }
                    
                    if(mailArr->count()>0)
                    {
                        MailController::getInstance()->m_mailInfoSendDic->setObject(mailArr, ChatServiceCocos2dx::m_curSendMailIndex);
                        ChatServiceCocos2dx::notifyMessageIndex(ChatServiceCocos2dx::m_curSendMailIndex,m_mailInfo->fromUid,m_mailInfo->fromName);
                        ChatServiceCocos2dx::m_curSendMailIndex++;
                    }
                    
                }
                else
                {
                    CCLOG("mailInfoArr->count():%d",mailInfoArr->count());
                    MailController::getInstance()->m_mailInfoSendDic->setObject(mailInfoArr, ChatServiceCocos2dx::m_curSendMailIndex);
                    ChatServiceCocos2dx::notifyMessageIndex(ChatServiceCocos2dx::m_curSendMailIndex,m_mailInfo->fromUid,m_mailInfo->fromName);
                    ChatServiceCocos2dx::m_curSendMailIndex++;
                }
                
                if(!ChatServiceCocos2dx::isChatShowing){
                    CCLOG("!ChatServiceCocos2dx::isChatShowing");
//                    ChatServiceCocos2dx::m_isInMailDialog=true;
                    ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_USER;
                    ChatServiceCocos2dx::m_isNoticItemUsed=false;
                    MailController::getInstance()->setOldOpenMailInfo(m_mailInfo);
                    ChatServiceCocos2dx::showChatActivityFrom2dx();
                }
            }
        }
        else
        {
            if (m_mailInfo->type ==CHAT_ROOM) {
                MailController::getInstance()->getChatRoomMsgRecord(false,m_mailInfo->crGroupId);
                PopupViewController::getInstance()->addPopupInView(MailDialogView::create(m_mailInfo));
            }
            else
            {
                PopupViewController::getInstance()->addPopupInView(MailDialogView::create(m_mailInfo));
            }
        }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        if(ChatServiceCocos2dx::enableNativeChat ){
            CCLOGFUNCF("m_mailInfo->type %d",m_mailInfo->type);
            int channelType = -1;
            if (m_mailInfo->type ==CHAT_ROOM) {
                channelType = CHANNEL_TYPE_CHATROOM;
            }else{
                channelType = CHANNEL_TYPE_USER;
            }
            if (m_mailInfo->type ==CHAT_ROOM) {
                ChatServiceCocos2dx::setMailInfo(m_mailInfo->crGroupId.c_str(),m_mailInfo->uid.c_str(),m_mailInfo->fromName.c_str(),m_mailInfo->type);
                //                if (m_mailInfo->dialogs!=NULL&& m_mailInfo->dialogs->count()<=1){
                //                     CCLOGFUNCF("!getHasRequestDataBefore m_mailInfo->fromName.c_str() %s",m_mailInfo->fromName.c_str());
                //                    MailController::getInstance()->getChatRoomMsgRecord(false, m_mailInfo->crGroupId);
                //                }
                 ChatServiceCocos2dx:: settingGroupChatMailVCChatChannel();
                if(!ChatServiceCocos2dx::isChatShowing_fun()){
                    CCLOG("!ChatServiceCocos2dx::isChatShowing");
                   
                    
                    ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_CHATROOM;
                    //                    ChatServiceCocos2dx::m_isInMailDialog=true;
                    ChatServiceCocos2dx::m_isNoticItemUsed=false;
                    MailController::getInstance()->setOldOpenMailInfo(m_mailInfo);
                    ChatServiceCocos2dx::showChatIOSFrom2dx();
                }
            }
            else
            {
                CCArray* mailInfoArr=CCArray::create();
                CCObject* obj;
                CCARRAY_FOREACH(m_mailInfo->dialogs, obj){
                    MailDialogInfo* tempDialogInfo = dynamic_cast<MailDialogInfo*>(obj);
                    ChatMailInfo* info=ChatMailInfo::create(m_mailInfo,tempDialogInfo,true);
                    mailInfoArr->addObject(info);
                }
                CCLOG("showReadPopUp USERMAIL %d,ChatServiceCocos2dx::m_curSendMailIndex:%d",m_mailInfo->dialogs->count(),ChatServiceCocos2dx::m_curSendMailIndex);
                
                ChatServiceCocos2dx::setMailInfo(m_mailInfo->fromUid.c_str(),m_mailInfo->uid.c_str(),m_mailInfo->fromName.c_str(),m_mailInfo->type);
//                if (!ChatServiceCocos2dx::getHasRequestDataBefore(m_mailInfo->fromUid)) {
                ChatServiceCocos2dx:: settingGroupChatMailVCChatChannel();
                if(mailInfoArr->count()>0)
                {
                    if(!ChatServiceCocos2dx::isFirstOpenMailToPlayer(m_mailInfo->fromUid.c_str()))
                        ChatServiceCocos2dx::handleChatPush(mailInfoArr, channelType ,m_mailInfo->fromUid.c_str());
//                    ChatServiceCocos2dx::m_curSendMailIndex++;
                }
//                }
                
                if(!ChatServiceCocos2dx::isChatShowing_fun()){
                    CCLOG("!ChatServiceCocos2dx::isChatShowing");
                    //                    ChatServiceCocos2dx::m_isInMailDialog=true;
                    ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_USER;
                    ChatServiceCocos2dx::m_isNoticItemUsed=false;
                    MailController::getInstance()->setOldOpenMailInfo(m_mailInfo);
                
                    ChatServiceCocos2dx::showChatIOSFrom2dx();
                }
            }
        }
        else
        {
            if (m_mailInfo->type ==CHAT_ROOM) {
                MailController::getInstance()->getChatRoomMsgRecord(false,m_mailInfo->crGroupId);
                PopupViewController::getInstance()->addPopupInView(MailDialogView::create(m_mailInfo));
            }
            else
            {
                PopupViewController::getInstance()->addPopupInView(MailDialogView::create(m_mailInfo));
            }
        }
#endif
    }
    else{//个人邮件
        m_mailInfo->status = READ;
        PopupViewController::getInstance()->addPopupInViewWithAnim(MailReadPopUpView::create(*m_mailInfo), true);
    }
    if(issaveRead){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    }
}

void MailCell::refreshStatus(){
    if(m_mailInfo==NULL)
        return;
    this->setScale(1.0);

    int count = 0;
    if(m_mailInfo->tabType ==4){//战斗邮件
        m_nameText->setString(_lang("105519").c_str());
        count =GlobalData::shared()->mailCountObj.fightR;
    }else if(m_mailInfo->tabType == MAILTAB5){//mod邮件
        m_nameText->setString(_lang("105519").c_str());
        count =GlobalData::shared()->mailCountObj.modR;
    }else if(m_mailInfo->tabType ==3){//系统邮件
        m_nameText->setString(_lang("103731").c_str());
        count =GlobalData::shared()->mailCountObj.studioR;
    }else if(m_mailInfo->tabType ==2){//系统邮件
        m_nameText->setString(_lang("105569").c_str());
        count =GlobalData::shared()->mailCountObj.saveR;

    }else{
        if(m_mailInfo->tabType==0){
            if(m_mailInfo->unreadDialogNum<=0)
                m_mailInfo->status=READ;
            else
                m_mailInfo->status=UNREAD;
            count = m_mailInfo->unreadDialogNum;
        }else if(m_mailInfo->type==MAIL_RESOURCE){
            MailResourceCellInfo* tempInfo = dynamic_cast<MailResourceCellInfo*>(m_mailInfo.getObject());
            if(tempInfo->unread<=0)
                m_mailInfo->status=READ;
            else
                m_mailInfo->status=UNREAD;
            count = tempInfo->unread;
        }else if(m_mailInfo->type==MAIL_RESOURCE_HELP){
            MailResourceHelpCellInfo* tempInfo = dynamic_cast<MailResourceHelpCellInfo*>(m_mailInfo.getObject());
            if(tempInfo->unread<=0)
                m_mailInfo->status=READ;
            else
                m_mailInfo->status=UNREAD;
            count = tempInfo->unread;
        }else if(m_mailInfo->type==MAIL_ATTACKMONSTER){
            MailMonsterCellInfo* tempInfo = dynamic_cast<MailMonsterCellInfo*>(m_mailInfo.getObject());
            if(tempInfo->unread<=0)
                m_mailInfo->status=READ;
            else
                m_mailInfo->status=UNREAD;
            count = tempInfo->unread;
        }
        
    }
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_unreadNumBG->setScale(2.0);
    }
    else
        m_unreadNumBG->setScale(1.0);
    if(count > 0){
        this->m_unreadNumBG->setVisible(true);
        this->m_unreadNumText->setVisible(true);
        if(count >= 100){
            this->m_unreadNumText->setString("99+");
        }else{
            this->m_unreadNumText->setString(CC_ITOA(count));
        }
    }else{
        this->m_unreadNumBG->setVisible(false);
        this->m_unreadNumText->setVisible(false);
        if(m_mailInfo->status==UNREAD){
            m_unreadNumBG->setVisible(true);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_unreadNumBG->setScale(1.0);
            }
            else
                m_unreadNumBG->setScale(0.5);
        }
    }

    if(m_mailInfo->prepareToDelete&&m_mailInfo->save==0&&m_mailInfo->tabType!=2){
        auto sprite = CCLoadSprite::createScale9Sprite("Mail_frame03.png");
        sprite->setPreferredSize(CCSize(604, 148));
        sprite->setAnchorPoint(ccp(0, 0));

    }else if(m_mailInfo->status==READ&&m_mailInfo->tabType!=2){
        auto sprite = CCLoadSprite::createScale9Sprite("chuzheng_frame02.png");
        sprite->setOpacity(50);
        sprite->setPreferredSize(CCSize(604, 148));
        sprite->setAnchorPoint(ccp(0, 0));
    }
    if(m_mailInfo->rewardStatus==0&&m_mailInfo->tabType !=2&&m_mailInfo->tabType !=3){
        this->m_mailRwardIcon->setVisible(true);
        this->m_mailRwardIcon->setPositionX(m_rightPosX);
    }else{
        this->m_mailRwardIcon->setVisible(false);
    }
    if(m_mailInfo->status==UNREAD){
        m_nameText->setColor({60,28,0});
        if (CCCommonUtils::isIosAndroidPad()) {
            m_nameText->setFontSize(48);
        }
        else
            m_nameText->setFontSize(24);
        m_timeText->setColor({70,58,44});
    }else{
        m_nameText->setColor(m_readColor);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_nameText->setFontSize(48);
        }
        else
            m_nameText->setFontSize(24);
        m_timeText->setColor(m_readColor);
    }
}

void MailCell::refreshRewardIcon(CCObject* obj){
    if(m_mailInfo.getObject()==NULL){
        return;
    }
    CCString* str = dynamic_cast<CCString*>(obj);
    if(str->getCString() == m_mailInfo->uid){
        this->m_mailRwardIcon->removeAllChildren();
    }
} 
void MailCell::onDeleteStatusFlag(cocos2d::CCObject *p)
{
    setDeleteStatusFlag();
}
void MailCell::setDeleteStatusFlag(bool isNeedAnim ){
    if(!isNeedAnim){
        CCActionInterval * easeSineOut = CCEaseBackOut::create(CCMoveTo::create(0.4,ccp(0,this->m_moveNode->getPositionY()+0)));
        this->m_moveNode->runAction(easeSineOut);
    }else{
        this->m_moveNode->setPositionX(0);
    }
    //
    predeleteNum = 0;
    m_predelete = false;

}
void MailCell::showLoadingMoreAnim(){
    if(m_mailInfo==NULL){
        m_tipLoadingText->setVisible(true);
    }
}

void MailCell::setData(MailInfo *mailInfo,CCTableView *tableView){
    //predeleteNum = 0;
    this->m_mailInfo = mailInfo;
    this->m_tableView =tableView;
    m_tipLoadingText->setVisible(false);
    if(this->m_mailInfo==NULL){
        m_tipLoadingText->setVisible(true);
        m_ccbNode->setVisible(false);
        return ;
    }
    
    m_ccbNode->setVisible(true);
    refreshView();
    setDeleteStatusFlag();
}

void MailCell::refreshView()
{
    string fromName=m_mailInfo->fromName;
    if (m_mailInfo->type==CHAT_ROOM && fromName.length()>20) {
        fromName=CCCommonUtils::subStrByUtf8(fromName,0,20);
        fromName.append("...");
    }
    this->m_nameText->setString(fromName.c_str());
    string nameExt =m_mailInfo->fromName;

    if(m_mailInfo->type==MAIL_RESOURCE_HELP){
    
    }else if(m_mailInfo->alliance!=""){
        nameExt = "("+m_mailInfo->alliance+")"+m_mailInfo->fromName;
        this->m_nameText->setString(nameExt.c_str());
    }
    if(m_mailInfo->type == MAIL_MOD_SEND || m_mailInfo->type == MAIL_MOD_PERSONAL){
        nameExt = "[MOD]" + nameExt;
        this->m_nameText->setString(nameExt.c_str());
    }
    if(m_mailInfo->type==MAIL_Alliance_ALL){
        this->m_nameText->setString(nameExt.c_str());
    }else if(m_mailInfo->type==MAIL_ALLIANCEINVITE){
        this->m_nameText->setString(_lang("103738").c_str());
    } else if(m_mailInfo->type == MAIL_ALLIANCE_KICKOUT){
        this->m_nameText->setString(_lang("103783").c_str());
    } else if (m_mailInfo->type == MAIL_REFUSE_ALL_APPLY) {
        this->m_nameText->setString(_lang("115464").c_str());
    }
    
    //将时间转换为day hour min
    auto dt = GlobalData::shared()->getWorldTime()-GlobalData::shared()->changeTime(m_mailInfo->createTime);
    string timestr = "";
    int timedt = 0;
    
    if (dt>=24*3600*2) {
        time_t timeT = m_mailInfo->createTime;
        timestr = CCCommonUtils::timeStampToMD(timeT);
    }
    else {
        if(dt>=24*60*60){
            timedt =dt/(24*60*60);
            timestr = CC_ITOA(timedt);
            timestr.append(_lang("105592"));
        }else if(dt>=60*60){
            timedt =dt/(60*60);
            timestr = CC_ITOA(timedt);
            timestr.append(_lang("105591"));
        }else if(dt>=60){
            timedt =dt/60;
            timestr = CC_ITOA(timedt);
            timestr.append(_lang("105590"));
        }else{
            timestr ="1";
            timestr.append(_lang("105590"));
        }
        timestr.append(" ");
        timestr.append(_lang("105593"));
    }
    this->m_timeText->setString(timestr);
    
    this->m_mailRwardIcon->removeAllChildren();
    if(m_mailInfo->rewardId != "" && m_mailInfo->rewardStatus==0){
        auto rewardIcon = CCLoadSprite::createSprite("mail_reward.png");
        rewardIcon->setScale(0.6);
        this->m_mailRwardIcon->addChild(rewardIcon);
        this->m_mailRwardIcon->setPositionX(m_rightPosX);
    }

//    this->m_removeBG->setVisible(!(m_mailInfo->save == 0));
    refreshStatus();
    addRewardIcon();

    if(m_mailInfo->tabType ==2){//系统邮件
        m_nameText->setString(_lang("105569").c_str());
    }else if(m_mailInfo->tabType ==3){
        m_nameText->setString(_lang("103731").c_str());
    }else if(m_mailInfo->tabType == 4){
        m_nameText->setString(_lang("105519").c_str());
    }else if(m_mailInfo->tabType == MAILTAB5){
        m_nameText->setString(_lang("132000").c_str());
    }
//    if (MailController::getInstance()->CanRemoveMail && m_mailInfo->tabType!=2 && m_mailInfo->tabType!=3&& m_mailInfo->tabType!=4) {//开启删除状态
//        if (m_mailInfo->save==1 || (m_mailInfo->rewardId!=""&&m_mailInfo->rewardStatus==0)) {
//            m_deletSpr->setVisible(false);
//        }
//        else {
//            if(MailController::getInstance()->isInOpMails(m_mailInfo->uid)){
//                m_sprSel->setVisible(true);
//            }
//            m_deletSpr->setVisible(true);
//        }
//    }
    if(MailController::getInstance()->CanRemoveMail){
        m_editNode->setVisible(true);
        m_deletSpr->setVisible(true);
        string uid="";
        if(m_mailInfo->type==CHAT_ROOM)
            uid=m_mailInfo->crGroupId;
        else
            uid=m_mailInfo->uid;
        if(MailController::getInstance()->isInOpMails(uid,m_mailInfo->tabType)){
            m_sprSel->setVisible(true);
        }else{
            m_sprSel->setVisible(false);
        }
        m_editNode->setPositionX(m_rightPosX);
        m_mailRwardIcon->setPositionX(m_rightPosX - 60);
    }else {
        m_editNode->setVisible(false);
        m_mailRwardIcon->setPositionX(m_rightPosX);
        m_editNode->setPositionX(m_rightPosX - 60);
    }
    if(m_mailInfo->status==UNREAD){
        m_nameText->setColor({60,28,0});
        string name = m_nameText->getString();
        resizeString(name,23);
        m_nameText->setString(name.c_str());
    }else{

    }
}
void MailCell::addPicFlag(){
    this->m_picNode->removeAllChildren();
    string path="";
    bool usePersonalPic = false;
    switch (m_mailInfo->type) {
        case 0:
        case 1:
        case MAIL_MOD_SEND:
        case MAIL_MOD_PERSONAL:
            path=m_mailInfo->pic;
            usePersonalPic = true;
            break;
        case 2:
            path="mail_pic_flag_2";
            break;
        case 3:
            path=m_mailInfo->pic;
            usePersonalPic = true;
            break;
        case 4:{
            if(m_isAtk){
                path="mail_pic_flag_4_1";
            }else{
                path="mail_pic_flag_4";
            }
            
            break;
        }
        case 5:
            path="mail_pic_flag_5";
            break;
        case 6:
            path="mail_pic_flag_8";
            break;
        case 7:
            path=m_mailInfo->pic;
            usePersonalPic = true;
            break;
        case 8:
            path="mail_pic_flag_8";
            break;
        case 9:
            path="mail_pic_flag_9";
            break;
        case 10:
            path="mail_pic_flag_2";
            break;
        case 11:
            path=m_mailInfo->pic;
            usePersonalPic = true;
            break;
        case 12:
            path=m_mailInfo->pic;
            usePersonalPic = true;
            break;
        case 13:
            path="mail_pic_flag_2";
            break;
        case 14:
            path="mail_pic_flag_14";
            break;
        case 15:
            path="mail_pic_flag_2";
            break;
        case 16:
            path="mail_pic_flag_16";
            break;
        case 17:
            path=m_mailInfo->pic;
            usePersonalPic = true;
            break;
        case MAIL_ATTACKMONSTER:
            path="mail_pic_flag_18";
            break;
        case 19:
            path=m_mailInfo->pic;
            usePersonalPic = true;
            break;
        case 20:
            path=m_mailInfo->pic;
            usePersonalPic = true;
            break;
        case 26:
            path = m_mailInfo->pic;
            usePersonalPic = true;
            break;
        case 27:
        case MAIL_REFUSE_ALL_APPLY:
            path = m_mailInfo->pic;
            usePersonalPic = true;
            break;
        case CHAT_ROOM:
            path="mail_pic_flag_31";
            break;
        default:
            break;
    }
    if(m_mailInfo->tabType == MAILTAB5){
        path = "mange-mod";
    }
    if(path!=""){
        path+=".png";
    }else{
        path="mail_pic_flag_2.png";
    }
    auto spr = CCLoadSprite::createSprite(path.c_str());
    this->m_picNode->addChild(spr);
    float scale = 1.0;
    scale =94/spr->getContentSize().width;
    scale = scale>1.0?1.0:scale;
    if(m_mailInfo->tabType==USERMAIL){
        spr->setScale(scale*0.9);
    }else{
        spr->setScale(scale);
    }
    if (usePersonalPic && CCCommonUtils::isUseCustomPic(m_mailInfo->picVer))
    {
        m_headImgNode->initHeadImgUrl2(m_picNode, CCCommonUtils::getCustomPicUrl(m_mailInfo->fromUid, m_mailInfo->picVer), 1.0f, 80, true);
    }
}
void MailCell::addRewardIcon(){

    m_rewardNode1->removeAllChildren();

    std::string content= m_mailInfo->title;
    content = CCCommonUtils::replaceString(content, "\n", " ");
//    auto checklbl = CCLabelTTF::create();
//    checklbl->setFontSize(24);
//    checklbl->setAnchorPoint(ccp(0, 0.5));
//    checklbl->setDimensions(CCSize(400, 0));
//    checklbl->setString(content.c_str());
//    CCSize checkSize = checklbl->getContentSize();
//    if(checkSize.height>60){
//        int tmpInt = ceil(checkSize.height / 60);
//        int nowSize = Utf8Utils::strlen(m_mailInfo->title.c_str());
//        nowSize = ceil(nowSize/tmpInt);
//        std::string title = CCCommonUtils::subStrByUtf8(m_mailInfo->title,0,nowSize);
//        title.append("...");
//        content = title;
//    }
    
    //4个文本框
    auto label = CCLabelTTF::create();
//    label->setFntFile("Arial_Bold_Regular.fnt");
    if (CCCommonUtils::isIosAndroidPad()) {
        label->setFontSize(48);
    }
    else
        label->setFontSize(24);
    label->setAnchorPoint(ccp(0, 0.5));
    if (CCCommonUtils::isIosAndroidPad()) {
        label->setDimensions(CCSize(760, 0));
    }
    else
        label->setDimensions(CCSize(380, 0));
    m_rewardNode1->addChild(label);
    
    auto label1 = CCLabelIF::create();
    label1->setFntFile("Arial_Bold_Regular.fnt");
    if (CCCommonUtils::isIosAndroidPad()) {
        label1->setFontSize(48);
    }
    else
        label1->setFontSize(24);
    label1->setAnchorPoint(ccp(0, 0.5));
    m_rewardNode1->addChild(label1);
    
    auto label2 = CCLabelIF::create();
    label2->setFntFile("Arial_Bold_Regular.fnt");
    if (CCCommonUtils::isIosAndroidPad()) {
        label2->setFontSize(48);
    }
    else
        label2->setFontSize(24);
    label2->setAnchorPoint(ccp(0, 0.5));
    m_rewardNode1->addChild(label2);
    
    auto label3 = CCLabelIF::create();
    label3->setFntFile("Arial_Bold_Regular.fnt");
    if (CCCommonUtils::isIosAndroidPad()) {
        label3->setFontSize(48);
    }
    else
        label3->setFontSize(24);
    label3->setAnchorPoint(ccp(0, 0.5));
    m_rewardNode1->addChild(label3);
    
    label->setString(content.c_str());
    if(m_mailInfo->status==UNREAD){
        label->setColor({60,28,0});
        label1->setColor({60,28,0});
        label2->setColor({60,28,0});
        label3->setColor({60,28,0});
        
    }else{
        label->setColor(m_readColor);
        label1->setColor(m_readColor);
        label2->setColor(m_readColor);
        label3->setColor(m_readColor);
    }
    
    bool isCreateContent = false;
    if(m_mailInfo->type == MAIL_ATTACKMONSTER){
        MailMonsterCellInfo* monstInfo = dynamic_cast<MailMonsterCellInfo*>(m_mailInfo.getObject());
        MailMonsterInfo* monsterInfo1 = dynamic_cast<MailMonsterInfo*>(monstInfo->monster->objectAtIndex(0));
        std::string name = CCCommonUtils::getNameById(monsterInfo1->monsterId);
        string level = CCCommonUtils::getPropById(monsterInfo1->monsterId,"level");
        name+=" Lv.";
        name+=level;
        label->setString(name.c_str());
        label1->setAlignment(kCCTextAlignmentRight);
        label1->setAnchorPoint(ccp(1.0, 0.5));
        label1->setPosition(ccp(425, 0));
        
        if(monsterInfo1->monsterResult==1){
//        _lang("105117")//
            label1->setString(_lang("103758"));
            
        }else if(monsterInfo1->monsterResult==2){
            label1->setString(_lang("105118"));
        }else if(monsterInfo1->monsterResult == 4){
            label1->setString(_lang("103786"));
        }else{
            label1->setString(_lang("105117"));
        }
        
   
    }
    if(m_mailInfo->type == MAIL_BATTLE_REPORT){
        bool isbigLoss = false;
        if(m_mailInfo->atkGen == NULL){
            isbigLoss = true;
        }else{
//            label->setDimensions(CCSizeZero);
            calculateKillandLoss();
        }
        bool isAtt = false;
        std::string attUid;
        std::string attName;
        std::string defName;
        if (m_mailInfo->attUser){
              attUid = m_mailInfo->attUser->valueForKey("uid")->getCString();
             attName = m_mailInfo->attUser->valueForKey("name")->getCString();
              defName = m_mailInfo->defUser->valueForKey("name")->getCString();
        }
        
        if(m_mailInfo->atkHelper!=NULL){
            cocos2d::CCObject* obj;
            CCARRAY_FOREACH(m_mailInfo->atkHelper, obj){
                string helpUid = dynamic_cast<CCString*>(obj)->getCString();
                if(helpUid==GlobalData::shared()->playerInfo.uid){
                    isAtt = true;
                    break;
                }
            }
        }

        if(GlobalData::shared()->playerInfo.uid==attUid){
            isAtt = true;
        }
        int reportState;
        if(m_mailInfo->winner == ""){
            reportState= DRAW;
        }
        if(m_mailInfo->winner == attUid){
            if(isAtt){
                reportState= WIN;
            }else{
                reportState=LOOSE;
            }
        }else{
            if(isAtt){
                reportState= LOOSE;
            }else{
                reportState=WIN;
            }
        }
        m_isAtk =isAtt;
        if(isAtt){
            m_mailInfo->title = _lang_2("105547",_lang("102187").c_str(),defName.c_str());
        }else{
            if(m_mailInfo->battleType!=3){
                m_mailInfo->title = _lang_2("105547",attName.c_str(),_lang("102187").c_str());
            }else{
                m_mailInfo->title = _lang_2("105547",attName.c_str(),_lang("108678").c_str());
            }
        }
        label->setString(m_mailInfo->title.c_str());
        if(reportState==WIN){
            if(m_mailInfo->battleType==2){//caiji
                
                if(isAtt){

                }else{

                }
            }else if(m_mailInfo->battleType==3){
                isCreateContent = true;
                string description  = "";
               // if(isAtt){
                string name = "";
                    if(isAtt){ //攻城胜利
                        switch (m_mailInfo->pointType) {
                            case Throne:{
                                name = _lang("105700");
                                break;
                            }
                            case Trebuchet:{
                                name = _lang("105704");
                                break;
                            }
                            default:{
                                name = _lang("105578");
                                break;
                            }
                        }
                    }else{ //守城胜利
                        switch (m_mailInfo->pointType) {
                            case Throne:{
                                name = _lang("105702");
                                break;
                            }
                            case Trebuchet:{
                                name = _lang("105706");
                                break;
                            }
                            default:{
                                name = _lang("105579");
                                break;
                            }
                        }
                    }
                    description = _lang("108554");
                    this->m_nameText->setString(name.c_str());
                    description.append(CC_CMDITOA(m_kill));
                    description.append(" ");
                    description.append(_lang("105019"));
                    string temp =CC_CMDITOA(m_loss);
                    temp.append(" ");
                    description.append(temp);
                    resizeString(description,40);
                    label->setString(description.c_str());
//                    label1->setString(CC_CMDITOA(m_kill));
//                    label1->setPosition(label->getContentSize().width+5, 0);
//                    label2->setString(_lang("105019"));
//                    label2->setPosition(label1->getPositionX()+label1->getContentSize().width+5, 0);
//                    string temp =CC_CMDITOA(m_loss);
//                    temp.append("  ...");
//                    label3->setString(temp);
//                    label3->setPosition(label2->getPositionX()+label2->getContentSize().width+5, 0);
            }else if(m_mailInfo->battleType==4){//zhaying

                if(isAtt){

                }else{

                }
            }else if(m_mailInfo->battleType==6){
                this->m_nameText->setString(_lang("133053").c_str());
                string description1 = _lang("105579");
                resizeString(description1,40);
                label->setString(description1.c_str());
            }else if (m_mailInfo->battleType==7){//联盟领地战斗报告
                if(isAtt){
                    if (m_mailInfo->pointType == Tile_allianceArea) {
                        this->m_nameText->setString(_lang("115337").c_str());
                    }
                    else if (m_mailInfo->pointType == tile_banner) {
                        this->m_nameText->setString(_lang("115540").c_str());
                    }
                    string description = _lang_2("115341", CC_ITOA(m_kill), CC_ITOA(m_loss));
                    resizeString(description,40);
                    label->setString(description.c_str());
                }else{
                    if (m_mailInfo->pointType == Tile_allianceArea) {
                        this->m_nameText->setString(_lang("115339").c_str());
                    }
                    else if (m_mailInfo->pointType == tile_banner) {
                        this->m_nameText->setString(_lang("115542").c_str());
                    }
                    string description = _lang_2("115341", CC_ITOA(m_kill), CC_ITOA(m_loss));
                    resizeString(description,40);
                    label->setString(description.c_str());
                }
            }else if(m_mailInfo->msReport==1){
                this->m_nameText->setString(_lang("133053").c_str());
                string description1 = _lang("133083");
                resizeString(description1,40);
                label->setString(description1.c_str());
            }else{

            }
        }else{
            
            if(m_mailInfo->battleType==2){

                if(isAtt){

                }else{

                }
            }else if(m_mailInfo->battleType==3){
                    isCreateContent = true;
                //if(isAtt){
                    if(isbigLoss){
                        string  name = "";
                        string  description = "";
                        if(isAtt){ // 攻城大败
                            switch (m_mailInfo->pointType) {
                                case Throne:{
                                    name = _lang("105701");
                                    break;
                                }
                                case Trebuchet:{
                                    name = _lang("105705");
                                    break;
                                }
                                default:{
                                    name =_lang("105583");
                                    break;
                                }
                            }
                        }else{
                            switch (m_mailInfo->pointType) {//守城大败
                                case Throne:{
                                    name = _lang("105703");
                                    break;
                                }
                                case Trebuchet:{
                                    name = _lang("105707");
                                    break;
                                }
                                default:{
                                    name =_lang("105581");
                                    break;
                                }
                            }
                        }
                        description =_lang("105535");
                        this->m_nameText->setString(m_mailInfo->fromName.c_str());
                        resizeString(description, 40);
                        label->setString(description.c_str());
                    }
                    else{
                        string  name1 = "";
                        string  description1 = "";
                        if(isAtt){ // 攻城失败
                            switch (m_mailInfo->pointType) {
                                case Throne:{
                                    name1 = _lang("105701");
                                    break;
                                }
                                case Trebuchet:{
                                    name1 = _lang("105705");
                                    break;
                                }
                                default:{
                                    name1 = _lang("105582");
                                    break;
                                }
                            }
                        }else{
                            switch (m_mailInfo->pointType) { // 守城失败
                                case Throne:{
                                    name1 = _lang("105703");
                                    break;
                                }
                                case Trebuchet:{
                                    name1 = _lang("105707");
                                    break;
                                }
                                default:{
                                    name1 = _lang("105580");
                                    break;
                                }
                            }
      
                        }
                         this->m_nameText->setString(name1.c_str());
                        description1 = _lang("108554");
                        description1.append(CC_CMDITOA(m_kill));
                        description1.append(" ");
                        description1.append(_lang("105019"));
                        string temp =CC_CMDITOA(m_loss);
                        temp.append(" ");
                        description1.append(temp);
                        resizeString(description1,40);
                        label->setString(description1.c_str());
//                        label1->setString(CC_CMDITOA(m_kill));
//                        label1->setPosition(label->getContentSize().width+5, 0);
//                        label2->setString(_lang("105019"));
//                        label2->setPosition(label1->getPositionX()+ label1->getContentSize().width+5, 0);
//
//                        string temp =CC_CMDITOA(m_loss);
//                        temp.append("  ...");
//                        label3->setString(temp);
//                        label3->setAnchorPoint(ccp(0, 0.5));
//                        label3->setPosition(label2->getPositionX()+label2->getContentSize().width+5, 0);
                        
                    }
            }else if(m_mailInfo->battleType==4){
                if(isAtt){

                }else{

                }
            }else if(m_mailInfo->battleType==6){
                this->m_nameText->setString(_lang("133053").c_str());
                string description1 = _lang("105580");
                resizeString(description1,40);
                label->setString(description1.c_str());
            }else if (m_mailInfo->battleType==7){
                if(isAtt){
                    if (m_mailInfo->pointType == Tile_allianceArea) {
                        this->m_nameText->setString(_lang("115338").c_str());
                    }
                    else if (m_mailInfo->pointType == tile_banner) {
                        this->m_nameText->setString(_lang("115541").c_str());
                    }
                    string description = _lang_2("115341", CC_ITOA(m_kill), CC_ITOA(m_loss));
                    resizeString(description,40);
                    label->setString(description.c_str());
                }else{
                    if (m_mailInfo->pointType == Tile_allianceArea) {
                        this->m_nameText->setString(_lang("115340").c_str());
                    }
                    else if (m_mailInfo->pointType == tile_banner) {
                        this->m_nameText->setString(_lang("115543").c_str());
                    }
                    string description = _lang_2("115341", CC_ITOA(m_kill), CC_ITOA(m_loss));
                    resizeString(description,40);
                    label->setString(description.c_str());
                }
            }else if(m_mailInfo->msReport==1){
                this->m_nameText->setString(_lang("133053").c_str());
                string description1 = _lang("133083");
                resizeString(description1,40);
                label->setString(description1.c_str());
            }else{

            }
        }
        
    }
    else if(m_mailInfo->type == MAIL_RESOURCE){
        MailResourceCellInfo* tempInfo = dynamic_cast<MailResourceCellInfo*>(m_mailInfo.getObject());
        bool flag = tempInfo->resourceResult;
        if (flag == true) {
            int type = tempInfo->resourceType;
            int value = tempInfo->resourceValue;
            auto icon = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(type, value).c_str());
            this->m_rewardNode1->addChild(icon);
            if(CCCommonUtils::isIosAndroidPad()){
                icon->setScale(2);
            }
            icon->setPositionX(icon->getContentSize().width/2);
            std:: string numstr ="+ ";
            numstr.append(CC_CMDITOA(value));
            label->setString(numstr.c_str());
            if (CCCommonUtils::isIosAndroidPad()) {
                label->setPositionX(80);
            }
            else
                label->setPositionX(50);
        }
        else{
            std::string numstr = _lang("108896");
            label->setString(numstr.c_str());
            label->setPositionX(0);
        }
    }else if(m_mailInfo->type == MAIL_RESOURCE_HELP){
        label->setDimensions(CCSizeZero);
        MailResourceHelpCellInfo* tempInfo = dynamic_cast<MailResourceHelpCellInfo*>(m_mailInfo.getObject());
        CCArray* arr = tempInfo->collect;
        if(arr){
            MailResourceHelpInfo* first = dynamic_cast<MailResourceHelpInfo*>(arr->objectAtIndex(0));
            bool outofrange = false;
            if(first && first->reward->count()>0){
                CCArray* firstReward = first->reward;
                int arrSize = firstReward->count();
                CCDictionary* dictHelp = dynamic_cast<CCDictionary*>(firstReward->objectAtIndex(0));
                int type = dictHelp->valueForKey("t")->intValue();
                int value = dictHelp->valueForKey("v")->intValue();
                CCSprite* icon = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(type, value).c_str());
                this->m_rewardNode1->addChild(icon);
                if (CCCommonUtils::isIosAndroidPad()) {
                    icon->setScale(2);
                    icon->setPositionX(icon->getContentSize().width * icon->getScaleX() /2);
                }
                else
                    icon->setPositionX(icon->getContentSize().width/2);
                std:: string numstr ="+ ";
                numstr.append(CC_CMDITOA(value));
                label->setString(numstr.c_str());
                label->setPositionX(icon->getContentSize().width + 5);
                float tmpW = label->getPositionX() + label->getContentSize().width + 10;
                if(arrSize>1){
                    dictHelp = dynamic_cast<CCDictionary*>(firstReward->objectAtIndex(1));
                    type = dictHelp->valueForKey("t")->intValue();
                    value = dictHelp->valueForKey("v")->intValue();
                    icon = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(type, value).c_str());
                    if (CCCommonUtils::isIosAndroidPad()) {
                        icon->setScale(2);
                        icon->setPositionX(tmpW + icon->getContentSize().width * icon->getScaleX() /2);
                    }
                    else
                        icon->setPositionX(tmpW + icon->getContentSize().width/2);
                    numstr ="+ ";
                    numstr.append(CC_CMDITOA(value));
                    label1->setString(numstr.c_str());
                    label1->setPositionX(tmpW + icon->getContentSize().width + 5);
                    float ntmpW = label1->getPositionX() + label1->getContentSize().width + 10;
                    if(ntmpW>420){
                        outofrange = true;
                        label1->setString("...");
                        label1->setPositionX(tmpW + 20);
                    }else{
                        this->m_rewardNode1->addChild(icon);
                    }
                    tmpW = ntmpW;
                }
                if(outofrange==false && arrSize>2){
                    dictHelp = dynamic_cast<CCDictionary*>(firstReward->objectAtIndex(2));
                    type = dictHelp->valueForKey("t")->intValue();
                    value = dictHelp->valueForKey("v")->intValue();
                    icon = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(type, value).c_str());
                    if (CCCommonUtils::isIosAndroidPad()) {
                        icon->setScale(2);
                        icon->setPositionX(tmpW + icon->getContentSize().width * icon->getScaleX() /2);
                    }
                    else
                        icon->setPositionX(tmpW + icon->getContentSize().width/2);
                    numstr ="+ ";
                    numstr.append(CC_CMDITOA(value));
                    label2->setString(numstr.c_str());
                    label2->setPositionX(tmpW + icon->getContentSize().width + 5);
                    float ntmpW  = label2->getPositionX() + label2->getContentSize().width + 10;
                    if(ntmpW>420){
                        outofrange=true;
                        label2->setString("...");
                        label2->setPositionX(tmpW + 20);
                    }else{
                        this->m_rewardNode1->addChild(icon);
                    }
                    tmpW = ntmpW;
                }
                if(outofrange==false && arrSize>3){
                    label3->setString("...");
                    label3->setPositionX(tmpW + 20);
                }
            }
        }
    }
    else if(m_mailInfo->type == MAIL_GENERAL_TRAIN){

    }
    else if(m_mailInfo->type == MAIL_DETECT_REPORT){
        isCreateContent = true;
        if(m_mailInfo->pointType == MAIL_DETECT_REPORT_PROTECT){
            this->m_nameText->setString(m_mailInfo->title.c_str());
            label->setString(m_mailInfo->contents.c_str());
        }else{
            int pointType = m_mailInfo->pointType;
            CCDictionary *dict = dynamic_cast<CCDictionary*>(m_mailInfo->detectReport->objectForKey("user"));
            std::string titile = "";
            switch (pointType) {
                case Throne:{ //王座
                    titile = _lang("105708");
                    break;
                }
                case Trebuchet:{//投石机
                    titile = _lang("105709");
                    break;
                }
                case Tile_allianceArea:{
                    titile = _lang("115356");
                    break;
                }
                case tile_banner:{
                    titile = _lang("115539");
                    break;
                }
                default:{
                    titile = _lang("105527");
                    break;
                }
            }
            this->m_nameText->setString(titile.c_str());
            string  description1 = _lang("114101");
            if(dict){
                if (dict->objectForKey("noDef") && dict->valueForKey("noDef")->intValue()==1) {
                    std::string nameStr = "";
                    if(dict->objectForKey("abbr")){
                        std::string asn = dict->valueForKey("abbr")->getCString();
                        if(asn != ""){
                            nameStr.append("(");
                            nameStr.append(asn);
                            nameStr.append(")");
                        }
                    }
                    std::string nameTemp = "";
                    if (dict->objectForKey("name")) {
                        nameTemp = dict->valueForKey("name")->getCString();
                    }
                    
                    if (nameTemp != "") {
                        nameStr.append(nameTemp);
                    }
                    else {
                        if (m_mailInfo->pointType == Tile_allianceArea) {
                            if (dict->objectForKey("count")) {
                                int num = dict->valueForKey("count")->intValue();
                                nameStr.append(_lang_1("115312", CC_ITOA(num)));
                            }
                            else
                                nameStr.append(_lang_1("115312", CC_ITOA(1)));
                        }
                        else if (m_mailInfo->pointType == tile_banner) {
                            nameStr.append(_lang("115534"));
                        }
                    }
                    description1 = _lang_1("114101",nameStr.c_str());
                }
                else {
                    std::string nameStr = dict->valueForKey("name")->getCString();
                    if(dict->objectForKey("abbr")){
                        std::string asn = dict->valueForKey("abbr")->getCString();
                        if(asn != ""){
                            nameStr = std::string("(") + asn + ")" + dict->valueForKey("name")->getCString();
                        }
                    }
                    description1 = _lang_1("114101",nameStr.c_str());
                }
            }
            resizeString(description1, 40);
            label->setString(description1.c_str());
        }
    }
    else if(m_mailInfo->type == MAIL_ALLIANCE_KICKOUT){
        this->m_nameText->setString(_lang("103783").c_str());
//        label->setString(_lang_1("", <#_param1#>);
    }
    else if (m_mailInfo->type == MAIL_REFUSE_ALL_APPLY){
        this->m_nameText->setString(_lang("115464").c_str());
    }
    else if(m_mailInfo->type == MAIL_ENCAMP){
        CCPoint pt = WorldController::getPointByIndex(m_mailInfo->occupyPointId);
        if (m_mailInfo->ckf==1) {
            pt = WorldController::getPointByMapTypeAndIndex(m_mailInfo->occupyPointId,SERVERFIGHT_MAP);
        }
        isCreateContent = true;
        std::string title = "";
        string  description1 = "";
        switch (m_mailInfo->pointType) {
            case Throne:{
                title = _lang("105710");
                description1 = _lang("105712");
                break;
            }
            case Trebuchet:{
                title = _lang("105711");
                description1 = _lang("105713");
                break;
            }
            default:{
                if(m_mailInfo->isTreasureMap){
                    title = _lang("111504");
                    description1 = _lang_3("111506",  "",CC_ITOA(int(pt.x)),CC_ITOA(int(pt.y)));
                }else{
                    title = _lang("105537");
                    description1 = _lang_3("105538","",CC_ITOA(int(pt.x)),CC_ITOA(int(pt.y)));
                }
                break;
            }
        }
        this->m_nameText->setString(title.c_str());
        resizeString(description1, 40);
        label->setString(description1.c_str());
    }
    else if(m_mailInfo->type == WORLD_NEW_EXPLORE){
        this->m_nameText->setString(_lang("108675").c_str());
        isCreateContent = true;
        bool isExp = false;
        bool isloss = false;
           CCObject *obj=NULL;
        string exptem = "+";
        CCARRAY_FOREACH(m_mailInfo->mazeReward, obj){
            CCDictionary *reward = dynamic_cast<CCDictionary*>(obj);
            int value = atoi(reward->valueForKey("value")->getCString());
            int type = atoi(reward->valueForKey("type")->getCString());
            if(type==6){
                isExp = true;
                exptem.append(CC_CMDITOA(value));
                break;
            }
        }
        CCDictElement *element = NULL;
        
        CCDictionary *dict = m_mailInfo->mazeSodiler;
        int numLoss;
        CCDICT_FOREACH(dict, element){
            std::string armyId = element->getStrKey();
            numLoss  = dict->valueForKey(armyId)->intValue();
            isloss = true;
        }
        label->setDimensions(CCSizeZero);
        label1->setDimensions(CCSizeZero);
        label2->setDimensions(CCSizeZero);
        label3->setDimensions(CCSizeZero);
        if(isExp&&isloss){
            label->setString("EXP");
            label1->setPosition(label->getContentSize().width+5, 0);
            label1->setString(exptem);
            label2->setPosition(label1->getPositionX()+ label1->getContentSize().width+5, 0);
            label2->setString(_lang("105019"));
            label3->setPosition(label2->getPositionX()+label2->getContentSize().width+5, 0);
            label3->setString(CC_CMDITOA(numLoss));
            
        }else if(isExp){
            label->setString("EXP");
            label1->setPosition(label->getContentSize().width+5, 0);
            label1->setString(exptem);
        
        }else if(isloss){
            label->setString(_lang("105019").c_str());
            label1->setPosition(label->getContentSize().width+5, 0);
            label1->setString(CC_CMDITOA(numLoss));
        }
        
    }else if(m_mailInfo->type ==MAIL_DETECT){
        isCreateContent = true;
        this->m_nameText->setString(m_mailInfo->title.c_str());
        string  description1 = m_mailInfo->contents;
        resizeString(description1, 40);
        label->setString(description1.c_str());
    }else if(m_mailInfo->type == MAIL_GIFT){
        if(m_mailInfo->alliance.empty()){
            this->m_nameText->setString(m_mailInfo->fromName.c_str());
        }else{
            std::string show = "("+ m_mailInfo->alliance +")"+m_mailInfo->fromName;
            this->m_nameText->setString(show.c_str());
        }
        string  description1 = m_mailInfo->contents;
        resizeString(description1, 40);
        label->setString(description1.c_str());
    }else if(m_mailInfo->type == MAIL_DONATE){
        this->m_nameText->setString(m_mailInfo->fromName.c_str());
        string  description1 = m_mailInfo->title;
        resizeString(description1, 40);
        label->setString(description1.c_str());
    }else if(m_mailInfo->type == MAIL_WORLD_BOSS){
        string  description1 = m_mailInfo->contents;
        resizeString(description1, 40);
        label->setString(description1.c_str());
    }
    std::string str1 = label1->getString();
    std::string str2 = label2->getString();
    std::string str3 = label3->getString();
    
    if(str1.empty() && str2.empty() && str3.empty() && label->getContentSize().height>60){
        std::string newStr = label->getString();
        int count = Utf8Utils::strlen(label->getString().c_str());
        int tmpInt = ceil(label->getContentSize().height/60);
        count = ceil(count / tmpInt);
        newStr = CCCommonUtils::subStrByUtf8(newStr, 0, count);
        newStr.append("...");
        label->setString(newStr.c_str());
    }
    addPicFlag();
}
void MailCell::calculateKillandLoss(){
    bool isAtt = false;
    std::string attUid = m_mailInfo->attUser->valueForKey("uid")->getCString();
    if(GlobalData::shared()->playerInfo.uid==attUid){
        isAtt = true;
    }else{
        if(m_mailInfo->atkHelper!=NULL){
            cocos2d::CCObject* obj;
            CCARRAY_FOREACH(m_mailInfo->atkHelper, obj){
                string helpUid = dynamic_cast<CCString*>(obj)->getCString();
                if(helpUid==GlobalData::shared()->playerInfo.uid){
                    isAtt = true;
                    break;
                }
            }
        }
    }
    CCDictionary *attUserInfo;
    if (isAtt) {
        attUserInfo = m_mailInfo->attUser;
    }else{
        attUserInfo = m_mailInfo->defUser;
    }
    std::string npc = attUserInfo->valueForKey("npcId")->getCString();
    int dead = 0;
    int num = 0;
    int hurt = 0;
    int kill = 0;
    int total = 0;
    if(npc!=""){
        if(m_mailInfo->defReport){
            int count = m_mailInfo->defReport->count();
            for (int i=0; i<count; i++) {
                CCDictionary* dic = _dict(m_mailInfo->defReport->objectAtIndex(i));
                if(dic){
                    dead += dic->valueForKey("dead")->intValue();
                    num += dic->valueForKey("num")->intValue();
                    hurt += dic->valueForKey("hurt")->intValue();
                    kill += dic->valueForKey("kill")->intValue();
                }
            }
        }
        total = dead+num+hurt;
        if(total<=0) total = 1;
    }else{
        if(isAtt==true){
            if(m_mailInfo->atkArmyTotal){
                CCDictionary* dic = _dict(m_mailInfo->atkArmyTotal);
                kill +=dic->valueForKey("kill")->intValue();
                dead +=dic->valueForKey("dead")->intValue();
                hurt +=dic->valueForKey("hurt")->intValue();
                num +=dic->valueForKey("num")->intValue();
            }
            CCArray *atkGenKilltArr = dynamic_cast<CCArray*>(m_mailInfo->atkGenKill);
            CCObject *atkGenKilltobj;
            if(atkGenKilltArr){
                CCARRAY_FOREACH(atkGenKilltArr, atkGenKilltobj){
                    kill+=dynamic_cast<CCString*>(atkGenKilltobj)->intValue();
                }
            }
        }else{
            if(m_mailInfo->defArmyTotal){
                kill =m_mailInfo->defArmyTotal->valueForKey("kill")->intValue();
                dead =m_mailInfo->defArmyTotal->valueForKey("dead")->intValue();
                hurt =m_mailInfo->defArmyTotal->valueForKey("hurt")->intValue();
                num =m_mailInfo->defArmyTotal->valueForKey("num")->intValue();
            }
            CCArray *defGenKilltArr = dynamic_cast<CCArray*>(m_mailInfo->defGenKill);
            CCObject *defGenKilltobj;
            if(defGenKilltArr){
                CCARRAY_FOREACH(defGenKilltArr, defGenKilltobj){
                    kill+=dynamic_cast<CCString*>(defGenKilltobj)->intValue();
                }
            }
            if(m_mailInfo->defTowerKill!=NULL){
                
                for (int i=0; i<m_mailInfo->defTowerKill->count(); i++) {
                    auto trapDic = _dict(m_mailInfo->defTowerKill->objectAtIndex(i));
                    if(trapDic){
                        kill += trapDic->valueForKey("kill")->intValue();
                    }
                }
            }
            if(m_mailInfo->defFortLost!=NULL){
                int total = 0;
                for (int i=0; i<m_mailInfo->defFortLost->count(); i++) {
                    auto trapDic = _dict(m_mailInfo->defFortLost->objectAtIndex(i));
                    if(trapDic){
                        total += trapDic->valueForKey("dead")->intValue();
                        kill+=trapDic->valueForKey("kill")->intValue();
                    }
                }
            }
        }
    }
    total = dead+num+hurt;
    
    m_kill = kill;
    m_loss = dead;
}
bool MailCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    MailPopUpView *popUP = dynamic_cast<MailPopUpView*>(PopupViewController::getInstance()->getCurrentPopupView());
    if(popUP == NULL){//由于有过渡动画会导致动画过程中邮件可以删除。造成崩溃
        return false;
    }
    if(m_mailInfo==NULL){
        return false;
    }
    if(m_tableView->isTouchMoved()){
        return false;
    }
//    this->m_moveNode->set;
    m_touchPoint = pTouch->getLocation();
   
    if(isTouchInside(this, pTouch)){
        m_modelLayer->setVisible(true);
        return true;
    }
    return false;
}
void MailCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    float dx =pTouch->getLocation().x-m_touchPoint.x;
    float dy =pTouch->getLocation().y-m_touchPoint.y;
    if(fabs(dy)>=fabs(dx)){//竖向
        m_ismoveX = false;
    }else{
        m_ismoveX = true;
    }
    if(!m_predelete&&predeleteNum==1){
        //CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_DELETERECOVER);
        return;
    }
    if(m_mailInfo->tabType == 2||m_mailInfo->tabType == 3||m_mailInfo->tabType == 4||m_mailInfo->tabType == MAILTAB5){
        if(!m_ismoveX){
            m_modelLayer->setVisible(false);
        }
        return ;
    }
    if(m_tableView->isTouchMoved()){
        m_modelLayer->setVisible(false);
        return ;
    }

    if(fabs(dy)>=fabs(dx)){//竖向
        m_modelLayer->setVisible(false);
        return;
    }else{
        if(m_tableView!=NULL){
            m_tableView->setTouchStop(false);
        }
    }
    if(fabs(dy)>20){
        m_modelLayer->setVisible(false);
        return;
    }
    if(dx<0){
        float currX =this->m_moveNode->getPositionX();
        if(currX<=-155 && !CCCommonUtils::isIosAndroidPad()){
            return;
        }else if (currX<=-305 && CCCommonUtils::isIosAndroidPad()){
            return;
        }
        if(dx<-155 && !CCCommonUtils::isIosAndroidPad()){
            dx = -155;
        }else if (dx<-305 && CCCommonUtils::isIosAndroidPad()) {
            dx = -305;
        }
        this->m_moveNode->setPositionX(dx);
    }else{
        float currX =this->m_moveNode->getPositionX();
        if(currX>=0){
            return;
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            dx = -305+dx;
        }
        else
            dx = -155+dx;
        if(dx>0){
            dx = 0;
        }
        this->m_moveNode->setPositionX(dx);
    }

        return;

}
void MailCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_modelLayer->setVisible(false);
    if(m_tableView!=NULL){
        m_tableView->setTouchStop(true);
    }
    if(!m_predelete&&predeleteNum==1){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_DELETERECOVER);
        return;
    }
    bool isTouchDelete = true;
//    if(m_mailInfo->tabType == 2||m_mailInfo->tabType == 3||m_mailInfo->tabType == 4|| m_mailInfo->tabType == MAILTAB5){
//        if(fabs(m_touchPoint.y - pTouch->getLocation().y)>10||fabs(m_touchPoint.x - pTouch->getLocation().x)>10)
//        {
//            return;
//        }
//        if(isTouchInside(m_moveNode, pTouch)){
//            if(predeleteNum==0){
//                showReadPopUp();
//                return;
//            }
//        }
//    }
    
    if(fabs(m_touchPoint.y - pTouch->getLocation().y)>30)
    {
        isTouchDelete = false;
    }
    else if (fabs(m_touchPoint.x - pTouch->getLocation().x)> 30)
    {
        isTouchDelete = false;
    }
    else if (m_editNode->isVisible() && isTouchInside(m_editNode, pTouch))
    {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        isTouchDelete = false;
        deleteMailByOp();
    }
    else if(isTouchInside(m_moveNode, pTouch))
    {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        if(predeleteNum==0){
            showReadPopUp();
        }else if(m_predelete){
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_DELETERECOVER);
        }
    }
    else if (isTouchInside(m_deleteBG, pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        isTouchDelete = false;
        onDeleteMail();
    }
    
    
    bool isMoveself = false;
    int x = this->m_moveNode->getPositionX();
    if(x==0){
        isMoveself = true;
    }
    int dx = -m_touchPoint.x + pTouch->getLocation().x;
    if(dx>0){//右移
        if (CCCommonUtils::isIosAndroidPad()) {
            if(x>-305){
                CCActionInterval * easeSineOut = CCEaseBackOut::create(CCMoveTo::create(0.4,ccp(0,this->m_moveNode->getPositionY()+0)));
                this->m_moveNode->runAction(easeSineOut);
            }else{
                CCActionInterval * easeSineOut = CCEaseBackOut::create(CCMoveTo::create(0.4,ccp(-305,this->m_moveNode->getPositionY()+0)));
                this->m_moveNode->runAction(easeSineOut);
                isTouchDelete = false;
                predeleteNum = 1;
                m_predelete = true;
            }
        }
        else {
            if(x>-115){
                CCActionInterval * easeSineOut = CCEaseBackOut::create(CCMoveTo::create(0.4,ccp(0,this->m_moveNode->getPositionY()+0)));
                this->m_moveNode->runAction(easeSineOut);
            }else{
                CCActionInterval * easeSineOut = CCEaseBackOut::create(CCMoveTo::create(0.4,ccp(-155,this->m_moveNode->getPositionY()+0)));
                this->m_moveNode->runAction(easeSineOut);
                isTouchDelete = false;
                predeleteNum = 1;
                m_predelete = true;
            }
        }
    }else{
        if (CCCommonUtils::isIosAndroidPad()) {
            if(x<-80){
                CCActionInterval * easeSineOut = CCEaseBackOut::create(CCMoveTo::create(0.4,ccp(-305,this->m_moveNode->getPositionY()+0)));
                this->m_moveNode->runAction(easeSineOut);
                isTouchDelete = false;
                predeleteNum = 1;
                m_predelete = true;
            }else{
                CCActionInterval * easeSineOut = CCEaseBackOut::create(CCMoveTo::create(0.4,ccp(0,this->m_moveNode->getPositionY()+0)));
                this->m_moveNode->runAction(easeSineOut);
            }
        }
        else {
            if(x<-40){
                CCActionInterval * easeSineOut = CCEaseBackOut::create(CCMoveTo::create(0.4,ccp(-155,this->m_moveNode->getPositionY()+0)));
                this->m_moveNode->runAction(easeSineOut);
                isTouchDelete = false;
                predeleteNum = 1;
                m_predelete = true;
            }else{
                CCActionInterval * easeSineOut = CCEaseBackOut::create(CCMoveTo::create(0.4,ccp(0,this->m_moveNode->getPositionY()+0)));
                this->m_moveNode->runAction(easeSineOut);
            }
        }
    }
    
}
void MailCell::deleteMailByOp(){
    if(m_sprSel->isVisible()==false){
        if(m_mailInfo->tabType == SAVEMAIL || m_mailInfo->tabType == MAILTAB3 || m_mailInfo->tabType == MAILTAB4 || m_mailInfo->tabType == MAILTAB5){
            MailController::getInstance()->addToOpMails("",m_mailInfo->tabType);
        }
        else{
            if (m_mailInfo->type==CHAT_ROOM) {
                MailController::getInstance()->addToOpMails(m_mailInfo->crGroupId);
            }
            else
            {
                MailController::getInstance()->addToOpMails(m_mailInfo->uid);
            }
            if (m_mailInfo->tabType == USERMAIL) {
                MailController::getInstance()->addFromUidsToOpMail(m_mailInfo->fromUid);
            }
        }
        m_sprSel->setVisible(true);
    }else{
        if(m_mailInfo->tabType == SAVEMAIL || m_mailInfo->tabType == MAILTAB3 || m_mailInfo->tabType == MAILTAB4 || m_mailInfo->tabType == MAILTAB5){
            MailController::getInstance()->removeFromOpMails("",m_mailInfo->tabType);
        }else{
            MailController::getInstance()->removeFromOpMails(m_mailInfo->uid);
            if (m_mailInfo->tabType == USERMAIL) {
                MailController::getInstance()->removeFromUidFromOpMails(m_mailInfo->fromUid);
            }
        }
        m_sprSel->setVisible(false);
    }
}
void MailCell::onDeleteMail(){
    if(m_mailInfo==NULL){
        return;
    }
    if(m_mailInfo->rewardStatus==0&&m_mailInfo->rewardId!=""){
        CCCommonUtils::flyHint("","",_lang("105512"));
        return;
    }
    if(m_mailInfo->save!=0){
        CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    setDeleteStatusFlag(true);
    onDeleteMailCommand();
}
void MailCell::onDeleteMailCommand(){
    if(m_mailInfo==NULL){
        return;
    }
    int index = getIdx();
    if(m_mailInfo->tabType==0){
        if(m_mailInfo->type!=CHAT_ROOM)
            MailController::getInstance()->removeDialogMail(m_mailInfo->fromUid,m_mailInfo->uid,index);
        else{
             MailController::getInstance()->removeChatRoomDialog(m_mailInfo->crGroupId,index);
        }
    }else if(m_mailInfo->type==MAIL_RESOURCE_HELP||m_mailInfo->type==MAIL_RESOURCE||m_mailInfo->type==MAIL_ATTACKMONSTER){
        MailController::getInstance()->removeResourceMail(m_mailInfo->uid,index);
    }else{
        MailController::getInstance()->removeMail(m_mailInfo->uid, CC_ITOA(m_mailInfo->type),index);
    }
}

void MailCell::onPlayRemove()
{
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(MailCell::onDeleteMailCommand));
    CCActionInterval * scaleTo = CCEaseElasticIn::create(CCScaleTo::create(0.3, 0));
    this->runAction(CCSequence::create(scaleTo, funcall, NULL));
}
void MailCell::resizeString(std::string &str,int len){
    //    std::string language = LocalController::shared()->getLanguageFileName();
    //    len= CCLabelIF::canBeSupportedWithBmpFont(language)?len:len*0.5;
    
    int totalLen = Utf8Utils::strlen(str.c_str());
    if(totalLen>(len+3)){
        std::string title = CCCommonUtils::subStrByUtf8(str,0,len);
        title.append("...");
        str = title;
    }
}