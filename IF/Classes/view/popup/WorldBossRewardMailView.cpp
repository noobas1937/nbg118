//
//  WorldBossRewardMailView.cpp
//  IF
//
//  Created by zhangjun on 15-4-28.
//
//

//#include "MailController.h"
//#include "MailWritePopUpView.h"
//#include "CCSafeNotificationCenter.h"
//#include "RewardList.h"
////#include "CCScrollView.h"
//#include "ChatLockCommand.h"
//#include "GeneralHeadPic.h"
//#include "WorldController.h"
//#include "CCLabelIFTTF.h"
//#include "ParticleController.h"
//#include "SceneController.h"
//#include "AllianceAcceptInviteCommand.h"
//#include "RefuseInviteCommand.h"
//#include "AllianceManager.h"
//#include "AllianceInfoView.h"
//#include "JoinAllianceView.h"
//#include "ChatController.h"
//#include "CheckAllianceInfoView.h"
//#include "SuggestionView.h"
//#include "HelpshiftCocos2dx.h"
//#include "RefuseAllianceApplyCommand.h"
//#include "AcceptAllianceApplyCommand.h"
//#include "WorldMapView.h"
//#include "InviteTeleportCommand.h"
//#include "RoleInfoView.h"

#include "WorldBossRewardMailView.h"
#include "PopupViewController.h"
#include "MailController.h"
#include "MailGetRewardCommand.h"
#include "MailPopUpView.h"
#include "BattleReportMailPopUpView.h"
#include "RewardController.h"
#include "MailSaveCommand.h"
#include "YesNoDialog.h"
#include "SoundController.h"
#include "ToolController.h"
#include "CCRichLabelTTF.h"
#include "FBUtilies.h"
#include "UIComponent.h"
#include "MailWritePopUpView.h"
void WorldBossRewardMailView::onEnter(){
    PopupBaseView::onEnter();
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade,true);//simon
    UIComponent::getInstance()->hideReturnBtn();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WorldBossRewardMailView::refreshContent), MAIL_CONTENT_READ, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WorldBossRewardMailView::refreshContent), MIAL_GET_REWARD_REFRESH, NULL);
    if(m_info->type==MAIL_ALLIANCEAPPLY || m_info->type==MAIL_ALLIANCEINVITE){
        setTitleName(m_info->title.c_str());
        m_mailTitle->setString(m_info->title.c_str());
    }else{
        setTitleName(m_info->fromName.c_str());
        m_mailTitle->setString(m_info->fromName.c_str());
    }
    if (m_info->type == MAIL_INVITE_TELEPORT) {
        setTitleName(m_info->title.c_str());
        m_mailTitle->setString(m_info->title.c_str());
        m_titleText->setString("");
    }
    if (m_info->type == MAIL_ALLIANCE_KICKOUT || m_info->type == MAIL_REFUSE_ALL_APPLY) {
        setTitleName(m_info->title.c_str());
        m_mailTitle->setString(m_info->title.c_str());
        m_titleText->setString(m_info->title.c_str());
    }
    setTouchEnabled(true);
}

void WorldBossRewardMailView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_CONTENT_READ);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MIAL_GET_REWARD_REFRESH);
    if(m_modelLayer){
        CCDirector::sharedDirector()->getRunningScene()->removeChild(m_modelLayer);
    }
   setTouchEnabled(false);
    PopupBaseView::onExit();
}

WorldBossRewardMailView* WorldBossRewardMailView::create(MailInfo& info){
    WorldBossRewardMailView* ret = new WorldBossRewardMailView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool WorldBossRewardMailView::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    setIsHDPanel(true);
    m_listArr = CCArray::create();
    m_listAnimArr = CCArray::create();
//    auto cf = CCLoadSprite::getSF("Mail_diban.png");
    auto cf = CCLoadSprite::getSF("Mail_BG1.png");
    if (cf==NULL) {
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(205, true);
        CCLoadSprite::doResourceByCommonIndex(206, true);
        CCLoadSprite::doResourceByCommonIndex(6, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::doResourceByCommonIndex(205, false);
            CCLoadSprite::doResourceByCommonIndex(206, false);
            CCLoadSprite::doResourceByCommonIndex(6, false);
        });
    }
    else {
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(205, true);
        CCLoadSprite::doResourceByCommonIndex(206, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::doResourceByCommonIndex(205, false);
            CCLoadSprite::doResourceByCommonIndex(206, false);
        });
    }
    
    auto bg = CCBLoadFile("NEW_WorldBossRewardMailView", this, this);
    m_ccbNode = bg;
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(bg->getContentSize());
    
//    setTitleName(_lang("105513"));
    m_mailTitle->setString(_lang("105513"));
    
    if (CCCommonUtils::isIosAndroidPad()) {
        int extH = getExtendHeight();
        this->m_contentContainer->setContentSize(CCSize(m_contentContainer->getContentSize().width, m_contentContainer->getContentSize().height + extH));
        m_downNode->setPositionY(m_downNode->getPositionY() - extH);
        m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
        auto tbg = CCLoadSprite::loadResource("Mail_diban.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        auto picBg1 = CCLoadSprite::createSprite("Mail_diban.png");
        picBg1->setAnchorPoint(ccp(0, 0));
        picBg1->setPosition(ccp(0, 0));
        picBg1->setScaleX(2.4);
        tBatchNode->addChild(picBg1);
        int maxHeight = CCDirector::sharedDirector()->getWinSize().height;
        int curHeight = picBg1->getContentSize().height;
        while(curHeight < maxHeight)
        {
            auto picBg2 = CCLoadSprite::createSprite("Mail_diban.png");
            picBg2->setAnchorPoint(ccp(0, 0));
            picBg2->setPosition(ccp(0, curHeight));
            picBg2->setScaleX(2.4);
            tBatchNode->addChild(picBg2);
            curHeight += picBg2->getContentSize().height;
        }
        m_bgNode->addChild(tBatchNode);
    }
    else {
        int extH = getExtendHeight();
        this->m_contentContainer->setContentSize(CCSize(m_contentContainer->getContentSize().width, m_contentContainer->getContentSize().height + extH));
        m_downNode->setPositionY(m_downNode->getPositionY() - extH);
        m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
        auto tbg = CCLoadSprite::loadResource("Mail_diban.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        auto picBg1 = CCLoadSprite::createSprite("Mail_diban.png");
        picBg1->setAnchorPoint(ccp(0, 0));
        picBg1->setPosition(ccp(0, 0));
        tBatchNode->addChild(picBg1);
        int maxHeight = CCDirector::sharedDirector()->getWinSize().height;
        int curHeight = picBg1->getContentSize().height;
        while(curHeight < maxHeight)
        {
            auto picBg2 = CCLoadSprite::createSprite("Mail_diban.png");
            picBg2->setAnchorPoint(ccp(0, 0));
            picBg2->setPosition(ccp(0, curHeight));
            tBatchNode->addChild(picBg2);
            curHeight += picBg2->getContentSize().height;
        }
        m_bgNode->addChild(tBatchNode);
    }

    this->m_titleText->setFntFile("Arial_Bold_Regular.fnt");
    this->m_timeText->setFntFile("Arial_Bold_Regular.fnt");
//    this->m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height + dh));
    this->m_totalNode->removeChild(this->m_rewardNode);
    this->m_titleText->setFntFile("Arial_Bold.fnt");
    this->m_titleText->setString(m_info->title.c_str());
    this->m_timeText->setString(CCCommonUtils::timeStampToDate(m_info->createTime).c_str());
    int w = m_contentContainer->getContentSize().width;
    int h = m_contentContainer->getContentSize().height;
    m_scroll = CCScrollView::create(CCSize(w, h));
    m_scroll->setAnchorPoint(ccp(0, 1));
    m_scroll->setTouchPriority(1);
    m_scroll->setDirection(kCCScrollViewDirectionVertical);
    m_contentContainer->addChild(m_scroll);
    m_ListNode = CCNode::create();
    m_scroll->addChild(m_ListNode);
    if(!m_info->isReadContent){
        MailController::getInstance()->readMailFromServer(m_info->uid, CC_ITOA(m_info->type));
        GameController::getInstance()->showWaitInterface();
    }else{
        refreshContent(NULL);
    }
    //图片
    auto battlePic = CCLoadSprite::createSprite("worldBossMail.png");
    m_battlePicNode->removeAllChildren();
    m_battlePicNode->addChild(battlePic);

    return true;
}
void WorldBossRewardMailView::setAllBtnPosition(){
    
    auto btnPic1 = CCLoadSprite::createScale9Sprite("Mail_btn03.png");
    m_rewardBtn =CCControlButton::create(btnPic1);
    m_rewardBtn->setEffectStr(Music_Sfx_UI_collect_item);
    m_rewardBtn->setPreferredSize(CCSizeMake(170, 64));
    m_rewardBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(WorldBossRewardMailView::onRewardClick), CCControlEventTouchUpInside);
    auto btngrayPic1 = CCLoadSprite::createScale9Sprite("Mail_btn06.png");
    m_rewardBtn->setBackgroundSpriteForState(btngrayPic1, CCControlStateDisabled);
    CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("105572").c_str());
    m_rewardBtn->setVisible(false);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_rewardBtn->setScale(2.4);
    }
    auto btnPic2 = CCLoadSprite::createScale9Sprite("Mail_btn05.png");
    m_replyBtn =CCControlButton::create(btnPic2);
    m_replyBtn->setPreferredSize(CCSizeMake(170, 64));
    m_replyBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(WorldBossRewardMailView::onReplyClick), CCControlEventTouchUpInside);
//    CCCommonUtils::setButtonTitle(m_replyBtn, "查看战报-123");
    m_replyBtn->setVisible(false);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_replyBtn->setScale(2.4);
    }
    m_totalH-=10;
    CCArray* btnArr = CCArray::create();
    //查看战报btn
//    this->m_ListNode->addChild(m_replyBtn);
//    m_replyBtn->setPosition(0, m_totalH-32);
//    m_replyBtn->setVisible(true);
//    btnArr->addObject(m_replyBtn);
    //收取奖励btn
    if(m_info->rewardId!=""){
        this->m_ListNode->addChild(m_rewardBtn);
        m_rewardBtn->setPosition(0, m_totalH-32);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_rewardBtn->setPosition(0, m_totalH-77);
        }
        if(m_info->rewardStatus==0){
        
        }else{
            m_rewardBtn->setEnabled(false);
        }
        m_rewardBtn->setVisible(true);
        btnArr->addObject(m_rewardBtn);
    }

    int count = btnArr->count();
    int wh = m_contentContainer->getContentSize().width;

    if(count==1){
        auto btn =dynamic_cast<CCControlButton*>(btnArr->objectAtIndex(0));
        btn->setPosition(wh/2, m_totalH-32);
        if (CCCommonUtils::isIosAndroidPad()) {
            btn->setPosition(wh/2, m_totalH-77);
        }
    }else if(count==2){
        auto btn =dynamic_cast<CCControlButton*>(btnArr->objectAtIndex(1));
        btn->setPosition(wh/2-10-170/2, m_totalH-32);
        auto btn1 =dynamic_cast<CCControlButton*>(btnArr->objectAtIndex(0));
        btn1->setPosition(wh/2+10+170/2, m_totalH-32);
    }else if(count==3){
        auto btn =dynamic_cast<CCControlButton*>(btnArr->objectAtIndex(1));
        btn->setPosition(wh/2, m_totalH-64);
        auto btn1 =dynamic_cast<CCControlButton*>(btnArr->objectAtIndex(0));
        btn1->setPosition(wh/2-10-170, m_totalH-32);
        auto btn2 =dynamic_cast<CCControlButton*>(btnArr->objectAtIndex(2));
        btn2->setPosition(wh/2+10+170+170/2, m_totalH-32);
    }
    btnArr->removeAllObjects();
    
    if(m_rewardBtn->isVisible()&&m_info->rewardStatus==0){
        auto sprAnim = CCLoadSprite::createSprite("Mail_btn00.png");
    }
    if(count>0){
        m_totalH-=64;
    }
    
    if(m_info->save==0){
        m_addSaveBtn->setHighlighted(false);
        
    }else{
        m_addSaveBtn->setHighlighted(true);
    }
    
    if(m_info->type == MAIL_SELF_SEND || m_info->type == MAIL_MOD_PERSONAL){
        m_addSaveBtn->setVisible(false);
    }
}
bool WorldBossRewardMailView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentContainer", CCNode*, this->m_contentContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailTitle", CCLabelIF*, this->m_mailTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnBtn", CCControlButton*, this->m_returnBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnSpr", CCSprite*, this->m_returnSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_writeBtn", CCControlButton*, this->m_writeBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardContainer", CCNode*, this->m_rewardContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listBG", CCScale9Sprite*, this->m_listBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kuangBG", CCScale9Sprite*, this->m_kuangBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_line", CCScale9Sprite*, this->m_line);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideNode", CCNode*, this->m_guideNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardNode", CCNode*, this->m_rewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardTitleText", CCLabelIF*, this->m_rewardTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battlePicNode", CCNode*, this->m_battlePicNode);

    return false;
}

SEL_CCControlHandler WorldBossRewardMailView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", WorldBossRewardMailView::onDeleteClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", WorldBossRewardMailView::onAddSaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturnClick", WorldBossRewardMailView::onReturnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteClick", WorldBossRewardMailView::onWriteClick);
    return NULL;
}

void WorldBossRewardMailView::refrehsReward(){
    int w = this->m_contentContainer->getContentSize().width;
    if(m_info->rewardId != ""){
        m_totalH-=10;
        m_ListNode->addChild(this->m_rewardNode);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->m_rewardNode->setPosition(768,m_totalH);
        }
        else
            this->m_rewardNode->setPosition(320,m_totalH);
        vector<std::string> vector1;
        vector<std::string> vector;

        CCCommonUtils::splitString(m_info->rewardId, "|", vector);
        int i = 0;
        CCArray *resArr = CCArray::create();
        CCArray *itemAndGeneralArr = CCArray::create();
        bool gray = (m_info->rewardStatus!=0);
        int count =vector.size();
        int ceHG = 120;
        if (CCCommonUtils::isIosAndroidPad()) {
            ceHG = 288;
        }
        
        if(count>1){
            if (CCCommonUtils::isIosAndroidPad()) {
                m_listBG->setContentSize(CCSize(m_listBG->getContentSize().width, 288+(count-1)*ceHG));
            }
            else
                m_listBG->setContentSize(CCSize(m_listBG->getContentSize().width, 120+(count-1)*ceHG));
//            m_kuangBG->setContentSize(CCSize(m_kuangBG->getContentSize().width, 160+(count-1)*ceHG));
        }
        m_totalH-=m_listBG->getContentSize().height;
        int startY = -120+m_rewardNode->getPositionY();
        if (CCCommonUtils::isIosAndroidPad()) {
            startY = -288+m_rewardNode->getPositionY();
        }
        while(i < count){
            std::string rewardStr = vector[i];
            vector1.clear();
            CCCommonUtils::splitString(rewardStr, ",", vector1);
            int type = 0;
            int value = 0;
            int num = 0;
            if(vector1.size()>=3){
                type = RewardController::getInstance()->getTypeByName(vector1[0]);
                value = atoi(vector1[1].c_str());
                num = atoi(vector1[2].c_str());
            }
 
            std::string nameStr;
            std::string picStr;
            auto cellNode = CCNode::create();
            if (CCCommonUtils::isIosAndroidPad()) {
                cellNode->setScale(2.4);
            }
            m_ListNode->addChild(cellNode);
            auto icon0  = CCLoadSprite::createSprite("icon_kuang.png");
            icon0->setPositionX(10+57);
            icon0->setPositionY(57);
            cellNode->addChild(icon0);
            icon0->setScale(98/icon0->getContentSize().width);
            float sacle = 1.0;
            if(type == R_GOODS || type == R_GENERAL){
                nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
                picStr =CCCommonUtils::getIcon(CC_ITOA(value));
                //float sacle = 1.0;
                if(picStr==".png"){
                    picStr = "no_iconFlag.png";
                    sacle = 94/98;
                }
                auto icon  = (type == R_GOODS)? CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_GOODS):CCLoadSprite::createSprite(picStr.c_str());
                sacle = 94/icon->getContentSize().width;
                cellNode->addChild(icon,1);
                icon->setScale(sacle);
                icon->setPositionX(10+57);
                icon->setPositionY(57);
                if(m_info->rewardStatus==1){
                    icon->setColor({90,85,81});
                }
                auto iter = ToolController::getInstance()->m_toolInfos.find(value);
                if(iter != ToolController::getInstance()->m_toolInfos.end()){
                    ToolInfo& info = ToolController::getInstance()->getToolInfoById(value);
                    auto iconBG = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info.color).c_str());
                    CCCommonUtils::setSpriteMaxSize(iconBG, 85);
                    cellNode->addChild(iconBG,0);
                    iconBG->setPosition(icon->getPosition());
                }
                
            }else if(type == R_HONOR || type == R_ALLIANCE_POINT){
                nameStr = RewardController::getInstance()->getNameByType(type,0);
                picStr = RewardController::getInstance()->getPicByType(type,0);
                auto icon  = CCLoadSprite::createSprite(picStr.c_str());
                sacle = 94/icon->getContentSize().width;
                cellNode->addChild(icon);
                icon->setScale(sacle);
                icon->setPositionX(10+57);
                icon->setPositionY(57);
                if(m_info->rewardStatus==1){
                    icon->setColor({90,85,81});
                }
            }else{
                nameStr = CCCommonUtils::getResourceNameByType(type);
                picStr =CCCommonUtils::getResourceIconByType(type);
                
                if(picStr==".png"){
                    picStr = "no_iconFlag.png";
                    sacle = 94/98;
                }
                
                auto icon  = CCLoadSprite::createSprite(picStr.c_str());
                sacle = 94/icon->getContentSize().width;
                cellNode->addChild(icon);
                icon->setScale(sacle);
                icon->setPositionX(10+57);
                icon->setPositionY(57);
                if(m_info->rewardStatus==1){
                    icon->setColor({90,85,81});
                }
                
            }
            
            auto label = CCLabelIF::create();
            label->setFntFile("Arial_Bold_Regular.fnt");
            label->setFontSize(22);
            label->setColor({125,98,63});
            
            label->setString(nameStr);
            label->setAnchorPoint(ccp(0, 0.5));
            //int width =bg->getContentSize().width/2;
            label->setPosition(120, 80);
            cellNode->addChild(label);
            auto label1 = CCLabelIF::create();
            label1->setFntFile("Arial_Bold_Regular.fnt");
            label1->setFontSize(20);
            label1->setColor({156,18,0});
            label1->setAnchorPoint(ccp(1.0, 0.5));
            
            label1->setString(CC_CMDITOA(num));
            //int width =bg->getContentSize().width/2;
            label1->setPosition(600, 20);
            cellNode->addChild(label1);
            
            cellNode->setPositionY(startY-i*ceHG);
            if(m_info->rewardStatus==1){
                label->setColor({90,85,81});
                label1->setColor({90,85,81});
                
            }
            
            i++;
        }
        m_listArr = resArr;
    }
   //showRewardAnim();
    
}
void WorldBossRewardMailView::showRewardAnim(){



}
void WorldBossRewardMailView::onDeleteClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    if(m_info==NULL){
        return;
    }
    if(m_info->rewardStatus==0&&m_info->rewardId!=""){
        CCCommonUtils::flyHint("", "", _lang("105512"));
        return;
    }
    if(m_info->save!=0){
         CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(WorldBossRewardMailView::onOkDeleteMail)),false);

}

void WorldBossRewardMailView::onRewardClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    
    this->m_rewardBtn->setEnabled(false);
    MailGetRewardCommand* cmd = new MailGetRewardCommand(m_info->uid, m_info->type);
    cmd->sendAndRelease();
}
void WorldBossRewardMailView::onOkDeleteMail(){
    MailController::getInstance()->removeMail(m_info->uid, CC_ITOA(m_info->type));
    //    this->closeSelf();
    PopupViewController::getInstance()->goBackPopupView();
}
void WorldBossRewardMailView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    
//    showRewardAnim();
//    return;
    if(m_info->save ==1){
        MailCancelSaveCommand *cmd = new MailCancelSaveCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->save = 0;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105576"));
        m_addSaveBtn->setHighlighted(false);
    }else{
        if(MailController::getInstance()->isMailFull(SAVEMAIL)){
            CCCommonUtils::flyText("full");
            return;
        }
        MailSaveCommand *cmd = new MailSaveCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->save = 1;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
        m_addSaveBtn->setHighlighted(true);
    }

}
void WorldBossRewardMailView::onReturnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->goBackPopupView();
}

void WorldBossRewardMailView::onWriteClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create("", ""));
}
void WorldBossRewardMailView::onReplyClick(cocos2d::CCObject *pSender, CCControlEvent event){
    CCLOGFUNC();
//    std::string str = "fc4fc1bc5a244d8c8d49feb0370aa0bd";
//    MailInfo *info = GlobalData::shared()->mailList[str];
//    PopupViewController::getInstance()->addPopupInViewWithAnim(BattleReportMailPopUpView::create(info));
}

void WorldBossRewardMailView::refreshContent(CCObject* p){
    int txtW = 580;
//    if (CCCommonUtils::isIosAndroidPad()) {
//        txtW = 1300;
//    }
    if (m_info->type == MAIL_FRESHER || m_info->type == MAIL_DIGONG) {
        m_contentContainer->setContentSize(CCSizeMake(380, m_contentContainer->getContentSize().height-30));
        m_contentContainer->setPosition(ccp(-90, m_contentContainer->getPositionY()+80));
        txtW = 380;
    }
    m_totalH = 0;
    string tempstr = m_info->contents;
    if (CCCommonUtils::isIosAndroidPad()) {
        txtW = 1200;
    }
    CCRichLabelTTF *richLabel= CCRichLabelTTF::create("", "Helvetica", 22,CCSize(txtW-30,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        richLabel->setFontSize(44);
//    }
    if (CCCommonUtils::isIosAndroidPad()) {
        richLabel->setFontSize(53);
    }
    this->m_ListNode->removeAllChildren();
    this->m_ListNode->addChild(richLabel);
    richLabel->setAnchorPoint(ccp(0, 1));
    if(m_info->type==MAIL_ALLIANCEINVITE || m_info->type == MAIL_INVITE_TELEPORT || m_info->type == MAIL_ALLIANCE_KICKOUT || m_info->type == MAIL_REFUSE_ALL_APPLY){
        richLabel->setString("");
        m_totalH = 0;
    }else{
        bool hasLink = false;
        std::string findString1 = "<a href=\"";
        std::string findString2 = "\">";
        std::string findString3 = "</a>";
        int posStart = tempstr.find(findString1);
        int posMiddle = tempstr.find(findString2);
        int posEnd= tempstr.find(findString3);
        int index = 0;
        std::string showStr = "";
        while(posStart!=string::npos && posMiddle!=string::npos && posEnd!=string::npos){
            hasLink=true;
            std::string subStr1 = tempstr.substr(0,posStart);
            posStart = posStart + findString1.length();
            std::string subStr2 = tempstr.substr(posStart,posMiddle - posStart);
            m_linkUrls.push_back(subStr2);
            posStart = posMiddle + findString2.length();
            std::string subStr3 = tempstr.substr(posStart,posEnd - posStart);
            posStart = posEnd + findString3.length();
            tempstr= tempstr.substr(posStart);
            showStr += subStr1 + "[u][color=ff0000ff][link bg=00000000 bg_click=00000000]"+subStr3 +"[/link][/color][/u]";
            ++index;
            posStart = tempstr.find(findString1);
            posMiddle = tempstr.find(findString2);
            posEnd= tempstr.find(findString3);
        }
        if(hasLink==false){
            tempstr ="[color=ff380800]" + tempstr + "[/color]";
            richLabel->setString(tempstr.c_str());
        }else{
            if(!tempstr.empty()){
                showStr += tempstr;
            }
            showStr = "[color=ff380800]" + showStr  + "[/color]" + "\n";
            richLabel->setString(showStr.c_str());
            while (index>0) {
                richLabel->setLinkTarget(index-1, CCCallFuncO::create(this, callfuncO_selector(WorldBossRewardMailView::onLinkClicked), NULL));
                --index;
            }
        }
        int contentWidth = txtW - 30;
        if (CCCommonUtils::isIosAndroidPad()) {
            richLabel->setPosition(ccp(768 - contentWidth / 2,0));
        }
        else
            richLabel->setPosition(ccp(320 - contentWidth / 2,0));
        m_totalH -= richLabel->getContentSize().height;
    }
    
    refrehsReward();
    if(m_info->type==MAIL_ALLIANCEAPPLY){
        richLabel->setString("");
    }
    setAllBtnPosition();
    showShareBtn();
    
    this->m_ListNode->setPositionY(-m_totalH);
    m_scroll->setContentSize(CCSize(m_contentContainer->getContentSize().width, -m_totalH));
    m_scroll->setContentOffset(ccp(0, m_contentContainer->getContentSize().height - (-m_totalH)));
    
}
void WorldBossRewardMailView::onLinkClicked(CCObject *ccObj) {
    CCLOG("on link clicked");
    if(ccObj){
        CCInteger *integer = dynamic_cast<CCInteger*>(ccObj);
        if(integer){
            int index = integer->getValue();
            if(index < m_linkUrls.size()){
                std::string url = m_linkUrls.at(index);
                if(!url.empty()){
                    GameController::getInstance()->goTurntoUrl(url);
                }
            }
        }
    }
}

void WorldBossRewardMailView::showShareBtn(){
    if(m_info->isShare){
        CCNode *node = CCNode::create();
        auto spr1 = CCLoadSprite::createScale9Sprite("btn_green3.png");
        CCControlButton *shareBtn = CCControlButton::create(spr1);
        node->addChild(shareBtn);
        shareBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(WorldBossRewardMailView::onShareBtnClick), CCControlEventTouchUpInside);
        CCSprite* sprIcon = CCLoadSprite::createSprite("facebook_icon.png");
        CCSize sprSize = sprIcon->getContentSize();
        node->addChild(sprIcon);
        sprIcon->setScale(0.7);
    
        CCLabelIF* label = CCLabelIF::create();
        label->setAnchorPoint(ccp(0, 0.5));
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setFontSize(24);
        label->setString(_lang("110145"));
        node->addChild(label);
        float lblSizeW = label->getContentSize().width * label->getOriginScaleX();
        float totalW = lblSizeW + 10 + sprSize.width*sprIcon->getScale() + 50;
        shareBtn->setPreferredSize(CCSize(totalW, 88));
        sprIcon->setPosition(ccp(-totalW*0.5 + sprSize.width * 0.35 + 25, 0));
        label->setPosition(ccp(sprIcon->getPositionX() + sprSize.width * 0.35 +10, 0));
        m_totalH -= 60;
        this->m_ListNode->addChild(node);
        node->setPosition(m_contentContainer->getContentSize().width*0.5,m_totalH);
        m_totalH-=50;
    }
}
void WorldBossRewardMailView::onShareBtnClick(CCObject *pSender, CCControlEvent event){
    GlobalData::shared()->isBind = true;
    string link = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    link = "https://fb.me/789279541113368?from_feed=android_king";
#else
    link = "https://fb.me/789290781112244?from_feed=ios_king";
#endif
    int rand = GlobalData::shared()->getRand(0,5);
    int randL = GlobalData::shared()->getRand(0,4);
    int randFeed = GlobalData::shared()->getRand(0,10000);
    string fbIcon = CCString::createWithFormat("http://cok.eleximg.com/cok/img/%s","King_Feed_1.png")->getCString();
    if(randFeed%2==0){
        string temp = _lang_2("110157", GlobalData::shared()->playerInfo.name.c_str(), CC_ITOA(GlobalData::shared()->playerInfo.selfServerId));
        FBUtilies::fbPublishFeedDialog("Clash Of Kings",_lang("110146"),temp,link,fbIcon,1);//shareLang[randL]
    }else{
        FBUtilies::fbPublishFeedDialog("Clash Of Kings",_lang("110147"),_lang("110158"),link,fbIcon,1);//shareLang[randL]
    }
}
bool WorldBossRewardMailView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_returnSpr, pTouch)) {
        return true;
    }
    return false;
}
void WorldBossRewardMailView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void WorldBossRewardMailView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_returnSpr, pTouch)) {
        PopupViewController::getInstance()->goBackPopupView();
    }
}