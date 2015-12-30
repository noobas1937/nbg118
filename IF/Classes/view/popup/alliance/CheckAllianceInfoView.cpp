//
//  CheckAllianceInfoView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-22.
//
//

#include "CheckAllianceInfoView.h"
#include "PopupViewController.h"
#include "SoundController.h"
#include "MailWritePopUpView.h"
#include "AllianceInfoMembersView.h"
#include "FunBuildController.h"
#include "UIComponent.h"
#include "ApplyAllianceCommand.h"
#include "CancelAllianceApplyCommand.h"
#include "ParticleFireAni.h"
#include "AllianceFlagPar.h"
#include "AllianceManager.h"
#include "AllianceInfoView.h"
#include "ParticleController.h"
#include "ChatController.h"
#include "MailController.h"
#include "AllianceCommentView.h"
#include "AllianceCommentCheck.h"
#include "GameController.h"
#include "ChatView.h"
#include "ChatServiceCocos2dx.h"
#include "GetAllianceMembersCommand.h"
#include "YesNoDialog.h"
#include "EnemyInfoController.h"
#include "AllianceApplyInfoView.h"

CheckAllianceInfoView* CheckAllianceInfoView::create(AllianceInfo* info){
    CheckAllianceInfoView* ret = new CheckAllianceInfoView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool CheckAllianceInfoView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(205, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(8, false);
    });
//    auto tbg = CCLoadSprite::loadResource("technology_09.png");
//    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
//    int curHight = -500;
//    while (curHight<maxHight) {
//        auto bg = CCLoadSprite::createSprite("technology_09.png");
//        bg->setAnchorPoint(ccp(0, 1));
//        bg->setPosition(ccp(0, curHight));
//        curHight += bg->getContentSize().height;
//        tBatchNode->addChild(bg);
//    }
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        tBatchNode->setScaleX(1536.0 / 640.0);
//    }
//    this->addChild(tBatchNode);
    
    auto tmpCCB = CCBLoadFile("CheckNewAllianceInfoView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    int preH = m_viewBg->getContentSize().height;
    changeBGHeight(m_viewBg);
    int newH = m_viewBg->getContentSize().height;
    int addH = newH- preH;
    m_bottomNode->setVisible(false);
    m_viewBg->setVisible(false);
    if(!GlobalData::shared()->playerInfo.isInAlliance()){
        addH += 0;
        m_bottomNode->setVisible(true);
    }else{
        addH += 100;
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        addH = CCDirector::sharedDirector()->getWinSize().height - 2048;
        if (!GlobalData::shared()->playerInfo.isInAlliance()) {
            m_bottomNode->setVisible(true);
        } else {
            addH += 240;
        }
        m_funList->setContentSize(CCSize(m_funList->getContentSize().width,m_funList->getContentSize().height+addH));
        m_funList->setPositionY(m_funList->getPositionY() - addH);
    } else {
        int deltY = 0;
        if (addH >= 200)
        {
//            m_middleNode->setPositionY(m_middleNode->getPositionY() - 30);
//            m_renshuNode->setPositionY(m_renshuNode->getPositionY() - 15);
//            m_zhandouNode->setPositionY(m_zhandouNode->getPositionY() - 15);
            deltY = 30;
        }
        m_funList->setContentSize(CCSize(m_funList->getContentSize().width,m_funList->getContentSize().height+addH - deltY));
        m_funList->setPositionY(m_funList->getPositionY() - addH);
    }
    
    if(m_info!=NULL){
        std::string str = "(";
        str.append(m_info->shortName.c_str());
        str.append(") ");
        str.append(m_info->name.c_str());
        m_nameTxt->setString(str.c_str());
        if(GlobalData::shared()->playerInfo.isInAlliance() && m_info->uid==GlobalData::shared()->playerInfo.allianceInfo.uid){
            str = _lang_2("115016", CC_ITOA(m_info->currentNum),CC_ITOA(GlobalData::shared()->playerInfo.allianceInfo.getSelfAllianceNum()));
        }else{
            str = _lang_2("115016", CC_ITOA(m_info->currentNum),CC_ITOA(m_info->maxNum));
        }
        m_membersTxt->setString(str);
        str = _lang_1("115015", m_info->leader.c_str());
        m_leaderTxt->setString(str.c_str());
        str = _lang_1("115017", CC_CMDITOAL(m_info->totalForce).c_str());
        m_powerTxt->setString(str);
        
        string lv = "Lv.";
        lv.append(CC_CMDITOAL(m_info->level).c_str());
        m_lvTxt->setString(lv);
    }
    
    m_allianceIcon->removeAllChildrenWithCleanup(true);
    
    m_applyBtn->setVisible(false);
    m_joinAlliance->setVisible(false);
    m_rovkeBtn->setVisible(false);
//    addBtnPar(m_rovkeBtn);
    addBtnPar(m_joinAlliance);
    addBtnPar(m_applyBtn);
    m_bottomNode->setVisible(false);
    m_bottomNode->setPositionY(m_bottomNode->getPositionY()-addH + 20);
    
    CCCommonUtils::setButtonTitle(m_applyBtn, _lang("115019").c_str());
    CCCommonUtils::setButtonTitle(m_RestrictApply, _lang("115019").c_str());
    CCCommonUtils::setButtonTitle(m_joinAlliance, _lang("115020").c_str());
    CCCommonUtils::setButtonTitle(m_rovkeBtn, _lang("115021").c_str());
    
    AllianceFlagPar* flag = AllianceFlagPar::create(m_info->getAllianceIcon().c_str());
    m_allianceIcon->addChild(flag);
    
    m_scrollView = CCScrollView::create(m_funList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setTouchPriority(Touch_Popup);
    m_scrollView->setPosition({25, 30});
    m_funList->addChild(m_scrollView,10000);

    CCSize size = m_funList->getContentSize();
    m_textNode->setPositionY(0);
    m_textNode->removeFromParent();
    m_scrollView->addChild(m_textNode);
    if (CCCommonUtils::isIosAndroidPad()) {
        initFun();
        m_scrollView->setContentSize(CCSize(m_announceTxt->getContentSize().width * m_announceTxt->getOriginScaleX(), m_announceTxt->getContentSize().height * m_announceTxt->getOriginScaleY()));
        m_scrollView->setContentOffset(ccp(0, m_funList->getContentSize().height - m_scrollView->getContentSize().height));
    } else {
        m_scrollView->setContentSize(CCSize(m_funList->getContentSize().width,60));
        m_scrollView->setContentOffset(ccp(0, m_funList->getContentSize().height - 60));
    }
 
    if(!GlobalData::shared()->playerInfo.isInAlliance()){
        m_bottomNode->setVisible(true);
        if (m_info->isApplied) {
            m_rovkeBtn->setVisible(true);
        }else if(m_info->recruit==1){
            m_joinAlliance->setVisible(true);
        }else{
            m_applyBtn->setVisible(true);
        }
    }
    
    //TODO:
    m_RestrictApply->setVisible(false);
    if(!GlobalData::shared()->playerInfo.isInAlliance() && m_info->recruit == 0){
        int lv = FunBuildController::getInstance()->getMainCityLv();
        if ( lv < m_info->castleRestriction || GlobalData::shared()->playerInfo.getTotalPower() < m_info->powerRestriction) {
            m_RestrictApply->setVisible(true);
            removeBtnPar(m_joinAlliance);
            removeBtnPar(m_applyBtn);
        }
        else{
            m_RestrictApply->setVisible(false);
        }
    }
    if(!GlobalData::shared()->playerInfo.isInSelfServer()){
        m_joinAlliance->setVisible(false);
    }
    if (m_info->serverId != -1 && m_info->serverId != GlobalData::shared()->playerInfo.selfServerId) {
        m_rovkeBtn->setVisible(false);
        m_joinAlliance->setVisible(false);
        m_applyBtn->setVisible(false);
        m_bottomNode->setVisible(false);
        m_btnMember->setEnabled(false);
    }
    if (GlobalData::shared()->playerInfo.uid == m_info->leaderUid) {
        m_btnMail->setEnabled(false);
    }
    string lang = ( strcmp(m_info->language.c_str(), "")==0?"115600":m_info->language );
    m_language->setString(_lang(lang).c_str());
    return true;
}

void CheckAllianceInfoView::initFun(){
    std::string str = "(";
    str.append(m_info->shortName.c_str());
    str.append(") ");
    str.append(m_info->name.c_str());
    m_nameTxt->setString(str.c_str());
    str = CC_ITOA(m_info->currentNum);
    str.append("/");
    if(GlobalData::shared()->playerInfo.isInAlliance() && m_info->uid==GlobalData::shared()->playerInfo.allianceInfo.uid){
        str.append(CC_ITOA(GlobalData::shared()->playerInfo.allianceInfo.getSelfAllianceNum()));
    }else{
        str.append(CC_ITOA(m_info->maxNum));
    }
    m_membersTxt->setString(str);
    str = _lang_1("115015", m_info->leader.c_str());
    m_leaderTxt->setString(str.c_str());
    m_powerTxt->setString(CC_CMDITOAL(m_info->totalForce).c_str());
    string lv = "Lv.";
    lv.append(CC_CMDITOAL(m_info->level).c_str());
    m_lvTxt->setString(lv);
    //
    std::string strtemp;
    if (m_info->recruit == 0){
       if(m_info->powerRestriction!= 0 || m_info->castleRestriction != 0){
           std::string strCastle = CC_ITOA(m_info->castleRestriction);
           std::string strFight = CC_ITOA(m_info->powerRestriction);
           strtemp.append("\n");
           strtemp.append(_lang("115518"));
           strtemp.append("\n");
           strtemp.append(_lang("115509"));
           strtemp.append(strCastle);
           strtemp.append("\n");
           strtemp.append(_lang("115510"));
           strtemp.append(strFight);
           strtemp.append("\n");  //---
           }
    }
    else{
        strtemp = "";
    }
    //
    std::string annStr =  m_info->intro + strtemp;
    m_announceTxt->setAnchorPoint(ccp(0, 0));
    m_announceTxt->setPosition(ccp(0, 0));
    m_announceTxt->setAlignment(kCCTextAlignmentLeft);
    m_announceTxt->setVerticalAlignment(kCCVerticalTextAlignmentBottom);
//    int oneLineHeight = 1;
//    if(annStr.size() > 0){
//        std::string testStr = annStr.substr(0, 1);
//        m_announceTxt->setString(testStr.c_str());
//        oneLineHeight = m_announceTxt->getContentSize().height * m_announceTxt->getOriginScaleY();
//    }
//    auto orgSize = m_funList->getContentSize();
//    int h = int(orgSize.height / oneLineHeight) * oneLineHeight;
//    if(h == 0){
//        h = orgSize.height;
//    }
//    m_scrollView->setViewSize(CCSize(orgSize.width, h));
//    m_scrollView->setViewSize(CCSize(orgSize.width, orgSize.height));
    m_announceTxt->setString(annStr.c_str());
//    m_announceTxt->setString("annStr.c_str()xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
    m_scrollView->setContentSize(CCSize(m_announceTxt->getContentSize().width * m_announceTxt->getOriginScaleX(), m_announceTxt->getOriginScaleY() * m_announceTxt->getContentSize().height));
    int offY = m_announceTxt->getContentSize().height * m_announceTxt->getOriginScaleY() - m_funList->getContentSize().height;
    m_scrollView->setContentOffset(ccp(0, -offY));

    CCCommonUtils::setButtonTitle(m_btnMail, _lang("115026").c_str());
    CCCommonUtils::setButtonTitle(m_btnMember, _lang("115027").c_str());
    CCCommonUtils::setButtonTitle(m_btnComment, _lang("115929").c_str());
}

void CheckAllianceInfoView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, true);
    });
    this->setTitleName(_lang("115025"));
    initFun();
    GlobalData::shared()->playerInfo.isChatView = true;
}

void CheckAllianceInfoView::onExit()
{
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    setTouchEnabled(false);
    CCNode::onExit();
}

bool CheckAllianceInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void CheckAllianceInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

}


void CheckAllianceInfoView::revokeAlliacne(CCObject * pSender, Control::EventType pCCControlEvent){
    CancelAllianceApplyCommand* cmd = new CancelAllianceApplyCommand(m_info->uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(CheckAllianceInfoView::cancelApply), NULL));
    cmd->sendAndRelease();
    m_rovkeBtn->setEnabled(false);
}

void CheckAllianceInfoView::applyAlliacne(CCObject * pSender, Control::EventType pCCControlEvent){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    int lv = FunBuildController::getInstance()->getMainCityLv();
    if ( lv < m_info->castleRestriction || GlobalData::shared()->playerInfo.getTotalPower() < m_info->powerRestriction) {
        if (m_info->recruit == 0) {
            YesNoDialog::show(_lang_1("115511", m_info->name.c_str()), NULL);  //--sun dialog
            return;
        }
    }
    ApplyAllianceCommand* cmd = new ApplyAllianceCommand(m_info->uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(CheckAllianceInfoView::success), NULL));
    cmd->sendAndRelease();
    m_applyBtn->setEnabled(false);
    removeBtnPar(m_applyBtn);
}

void CheckAllianceInfoView::joinAlliacne(CCObject * pSender, Control::EventType pCCControlEvent){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    ApplyAllianceCommand* cmd = new ApplyAllianceCommand(m_info->uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(CheckAllianceInfoView::joinSuccess), NULL));
    cmd->sendAndRelease();
    m_joinAlliance->setEnabled(false);
    removeBtnPar(m_joinAlliance);
}

void CheckAllianceInfoView::success(CCObject* obj){
    m_applyBtn->setVisible(false);
    m_rovkeBtn->setVisible(true);
    m_rovkeBtn->setEnabled(true);
    AllianceManager::getInstance()->needRefresh = false;
    PopupViewController::getInstance()->addPopupView(AllianceApplyInfoView::create(m_info->uid));
}

void CheckAllianceInfoView::joinSuccess(CCObject* obj){
     m_joinAlliance->setEnabled(true);
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        CCCommonUtils::setIsHDViewPort(true);
        PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
        
//        if(MailController::getInstance()->getChatRoomEnable()&& MailController::getInstance()->m_chatRoomMemberArr->count()<=0){
//            GetAllianceMembersCommand * command = new GetAllianceMembersCommand();
//            command->putParam("allianceId", CCString::create(GlobalData::shared()->playerInfo.getAllianceId()));
//            command->sendAndRelease();
//        }
    }
    //发聊天消息，加入联盟
    ChatController::getInstance()->sendCountryChat(_lang("115188").append("  (").append(_lang("115181")).append(")").c_str(), CHAT_ALLIANCE, 2);
    
}

void CheckAllianceInfoView::cancelApply(CCObject* obj){
    m_info->isApplied = false;
    m_rovkeBtn->setVisible(false);
    if (m_info->isApplied) {
        m_rovkeBtn->setVisible(true);
        m_rovkeBtn->setEnabled(true);
    }else if(m_info->recruit==1){
        m_joinAlliance->setVisible(true);
        m_joinAlliance->setEnabled(true);
    }else{
        m_applyBtn->setVisible(true);
        m_applyBtn->setEnabled(true);
    }
    AllianceManager::getInstance()->needRefresh = false;
}

void CheckAllianceInfoView::onMailClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
   // PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create(m_info->leader, ""));
    MailController::getInstance()->openMailDialogViewFirst(m_info->leader, m_info->leaderUid);
}

void CheckAllianceInfoView::onMemberClick(CCObject * pSender, Control::EventType pCCControlEvent){
    bool isApply = GlobalData::shared()->playerInfo.allianceInfo.uid !=m_info->uid;
    if (m_info->serverId == GlobalData::shared()->playerInfo.selfServerId || m_info->serverId == -1) {
        PopupViewController::getInstance()->addPopupInView(AllianceInfoMembersView::create(m_info->uid,isApply));
    } else {
        return;
    }
}

void CheckAllianceInfoView::onCommentClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->uid){
        PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_COMMENT,m_info->uid),false);
    }else{
        AllianceCommentCheck* cmd = new AllianceCommentCheck(m_info->uid);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(CheckAllianceInfoView::checkComment), NULL));
        cmd->sendAndRelease();
        if(m_waitInterface){
            m_waitInterface->remove();
            m_waitInterface = NULL;
        }
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_btnComment);
    }
}

void CheckAllianceInfoView::checkComment(CCObject* obj){
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    NetResult* result = dynamic_cast<NetResult*>(obj);
    auto dic = _dict(result->getData());
    if(dic){
        bool enable = dic->valueForKey("enable")->boolValue();
        if(enable){
            PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_COMMENT,m_info->uid),false);
        }else{
            CCCommonUtils::flyText(_lang("E100080"));
        }
    }else{
       PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_COMMENT,m_info->uid),false);
    }
}

SEL_CCControlHandler CheckAllianceInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "joinAlliacne", CheckAllianceInfoView::joinAlliacne);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "applyAlliacne", CheckAllianceInfoView::applyAlliacne);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "revokeAlliacne", CheckAllianceInfoView::revokeAlliacne);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMailClick", CheckAllianceInfoView::onMailClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMemberClick", CheckAllianceInfoView::onMemberClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCommentClick", CheckAllianceInfoView::onCommentClick);
    return NULL;
}

bool CheckAllianceInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leaderTxt", CCLabelIFTTF*, this->m_leaderTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_membersTxt", CCLabelIF*, this->m_membersTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_announceTxt", CCLabelIFTTF*, this->m_announceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_language", CCLabelIF*, this->m_language);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textNode", CCNode*, this->m_textNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_announceNode", CCNode*, this->m_announceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceIcon", CCNode*, this->m_allianceIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funList", CCNode*, this->m_funList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rovkeBtn", CCControlButton*, this->m_rovkeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_applyBtn", CCControlButton*, this->m_applyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_joinAlliance", CCControlButton*, this->m_joinAlliance);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_RestrictApply", CCControlButton*, this->m_RestrictApply);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMail", CCControlButton*, this->m_btnMail);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMember", CCControlButton*, this->m_btnMember);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnComment", CCControlButton*, this->m_btnComment);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_enterStrict", CCLabelIFTTF*, this->m_enterStrict);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ConNumShow", CCLabelIFTTF*, this->m_ConNumShow);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_zhandouNode", CCNode*, this->m_zhandouNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_renshuNode", CCNode*, this->m_renshuNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_middleNode", CCNode*, this->m_middleNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvTxt", CCLabelIF*, this->m_lvTxt);
    
    return false;
}

void CheckAllianceInfoView::addBtnPar(CCControlButton* btn){
    string tmpStart = "ShowFire_";
    string tmpStart1 = "ShowFireUp_";
    CCScale9Sprite* s9 = CCLoadSprite::createScale9Sprite("sel_general.png");
    CCSize size = btn->getPreferredSize();
    s9->setPreferredSize(size+CCSize(30,30));
    s9->setPosition(size/2);
    s9->setTag(8000);
    btn->addChild(s9);
    for (int i=1; i<=5; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
        particle->setPosition(ccp(size.width/2, -3));
        particle->setPosVar(ccp(size.width/2, 0));
        particle->setTag(8000 + i * 10 + 1);
        btn->addChild(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
        particle1->setPosition(ccp(size.width/2, size.height-3));
        particle1->setPosVar(ccp(size.width/2, 0));
        particle1->setTag(8000 + i * 10 + 2);
        btn->addChild(particle1);
        
        auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
        particle2->setPosition(ccp(0, size.height/2));
        particle2->setPosVar(ccp(0, size.height/2));
        particle2->setTag(8000 + i * 10 + 3);
        btn->addChild(particle2);
        
        auto particle3 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
        particle3->setPosition(ccp(size.width, size.height/2));
        particle3->setPosVar(ccp(0, size.height/2));
        particle3->setTag(8000 + i * 10 + 4);
        btn->addChild(particle3);
    }
}

void CheckAllianceInfoView::removeBtnPar(CCControlButton* btn)
{
    CCNode* node = nullptr;
    node = btn->getChildByTag(8000);
    if (node) {
        btn->removeChild(node, true);
    }
    node = nullptr;
    for (int i = 1; i <= 5; ++i) {
        for (int j = 1; j <= 4; ++j) {
            node = btn->getChildByTag(8000 + i * 10 + j);
            if (node) {
                btn->removeChild(node, true);
            }
            node = nullptr;
        }
    }
}

