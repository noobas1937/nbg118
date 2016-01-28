//
//  MailReadPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-11.
//
//

#include "MailReadPopUpView.h"
#include "MailController.h"
#include "PopupViewController.h"
#include "MailWritePopUpView.h"
#include "CCSafeNotificationCenter.h"
#include "MailController.h"
//#include "RewardList.h"
#include "MailGetRewardCommand.h"
//#include "CCScrollView.h"
#include "MailPopUpView.h"
#include "ChatLockCommand.h"
#include "BattleReportMailPopUpView.h"
#include "GeneralHeadPic.h"
#include "RewardController.h"
#include "WorldController.h"
#include "CCLabelIFTTF.h"
#include "MailSaveCommand.h"
#include "YesNoDialog.h"
#include "ParticleController.h"
#include "SceneController.h"
#include "AllianceAcceptInviteCommand.h"
#include "RefuseInviteCommand.h"
#include "AllianceManager.h"
#include "AllianceInfoView.h"
#include "JoinAllianceView.h"
#include "ChatController.h"
#include "CheckAllianceInfoView.h"
#include "SuggestionView.h"
#include "HelpshiftCocos2dx.h"
//#include "HtmlLabelTFNode.h"
#include "RefuseAllianceApplyCommand.h"
#include "AcceptAllianceApplyCommand.h"
#include "SoundController.h"
#include "ToolController.h"
#include "WorldMapView.h"
#include "InviteTeleportCommand.h"
#include "CCRichLabelTTF.h"
#include "RoleInfoView.h"
#include "FBUtilies.h"
#include "EquipmentController.h"
#include "AllianceScienceView.h"
#include "ChatServiceCocos2dx.h"
#include "GetAllianceListCommand.h"
#include "LuaController.h"
#include "UIComponent.h"
#include "ChatServiceCocos2dx.h"

#include "AllianceShopView.h"
#include "AllianceHelpView.h"
#include "AllianceTerritoryView.h"
#include "AllianceWarView.h"
#include "FunBuildController.h"
void MailReadPopUpView::onEnter(){
    PopupBaseView::onEnter();
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade,true);
    UIComponent::getInstance()->hideReturnBtn();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailReadPopUpView::refreshContent), MAIL_CONTENT_READ, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailReadPopUpView::refreshContent), MIAL_GET_REWARD_REFRESH, NULL);
    if(m_info->type==MAIL_ALLIANCEAPPLY){
//        setTitleName(m_info.title.c_str());
        this->m_mailTitle->setString(m_info->title.c_str());
    }else if(m_info->type==MAIL_ALLIANCEINVITE) {
        this->m_mailTitle->setString(_lang("102394"));
    }
    else{
//        setTitleName(m_info.fromName.c_str());
        this->m_mailTitle->setString(m_info->fromName.c_str());
    }
    if (m_info->type == MAIL_INVITE_TELEPORT) {
//        setTitleName(m_info.title.c_str());
        m_titleText->setString("");
        this->m_mailTitle->setString(_lang("115295"));/////////////
    }
    if (m_info->type == MAIL_ALLIANCE_KICKOUT || m_info->type == MAIL_REFUSE_ALL_APPLY) {
//        setTitleName(m_info.title.c_str());
        m_titleText->setString(m_info->title.c_str());
        this->m_mailTitle->setString(m_info->title.c_str());
    }
    if (m_info->type == MAIL_ALLIANCE_RANKCHANGE) {
        this->m_mailTitle->setString(m_info->title.c_str());
    }
    setTouchEnabled(true);
    //CCTransitionSlideInR action = CCTransitionSlideInR::transitionWithDuration();
//    float x = m_ccbNode->getPositionX();
//    float y = m_ccbNode->getPositionY();
//    this->m_ccbNode->setPositionX(m_ccbNode->getPositionX()+m_ccbNode->getContentSize().width);
//    CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
//    this->m_ccbNode->runAction(CCSequence::create(userMove,NULL));
}

void MailReadPopUpView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_CONTENT_READ);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MIAL_GET_REWARD_REFRESH);
    if(m_modelLayer){
        CCDirector::sharedDirector()->getRunningScene()->removeChild(m_modelLayer);
    }
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

MailReadPopUpView* MailReadPopUpView::create(MailInfo& info){
    MailReadPopUpView* ret = new MailReadPopUpView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailReadPopUpView::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    setIsHDPanel(true);
    setMailUuid(m_info->uid);
    m_listArr = CCArray::create();
    m_listAnimArr = CCArray::create();
//    auto cf = CCLoadSprite::getSF("Mail_diban.png");
    auto cf = CCLoadSprite::getSF("Mail_BG1.png");
    if (cf==NULL) {
        CCLoadSprite::doResourceByCommonIndex(6, true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(205, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::doResourceByCommonIndex(6, false);
            CCLoadSprite::doResourceByCommonIndex(205, false);
            
        });
    }
    else {
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(205, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::doResourceByCommonIndex(205, false);
            
        });
    }
    auto bg = CCBLoadFile("NEW_MailReadView", this, this);
    m_ccbNode = bg;
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(bg->getContentSize());

    this->m_mailTitle->setString(_lang("105513"));
    if (CCCommonUtils::isIosAndroidPad()) {
        int extH = getExtendHeight();
        this->m_contentContainer->setContentSize(CCSize(m_contentContainer->getContentSize().width, m_contentContainer->getContentSize().height + extH));
        m_downNode->setPositionY(m_downNode->getPositionY() - extH);
        m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
//        auto tbg = CCLoadSprite::loadResource("UI_paper_3.png");
//        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        auto picBg1 = CCLoadSprite::createSprite("UI_paper_3.png");
//        picBg1->setAnchorPoint(ccp(0, 0));
//        picBg1->setPosition(ccp(0, 0));
//        picBg1->setScaleX(2.4);
//        tBatchNode->addChild(picBg1);
//        int maxHeight = CCDirector::sharedDirector()->getWinSize().height;
//        int curHeight = picBg1->getContentSize().height;
////        while(curHeight < maxHeight)
////        {
////            auto picBg2 = CCLoadSprite::createSprite("UI_paper_3.png");
////            picBg2->setAnchorPoint(ccp(0, 0));
////            picBg2->setPosition(ccp(0, curHeight));
////            picBg2->setScaleX(2.4);
////            tBatchNode->addChild(picBg2);
////            curHeight += picBg2->getContentSize().height;
////        }
//        m_bgNode->addChild(tBatchNode);
        m_buildBG->setPositionY(m_buildBG->getPositionY() - extH);
    }
    else {
        int extH = getExtendHeight();
        this->m_contentContainer->setContentSize(CCSize(m_contentContainer->getContentSize().width, m_contentContainer->getContentSize().height + extH));
        m_downNode->setPositionY(m_downNode->getPositionY() - extH);
        m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
//        auto tbg = CCLoadSprite::loadResource("UI_paper_3.png");
//        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        auto picBg1 = CCLoadSprite::createSprite("UI_paper_3.png");
//        picBg1->setAnchorPoint(ccp(0, 0));
//        picBg1->setPosition(ccp(0, 0));
//        tBatchNode->addChild(picBg1);
//        int maxHeight = CCDirector::sharedDirector()->getWinSize().height;
//        int curHeight = picBg1->getContentSize().height;
////        while(curHeight < maxHeight)
////        {
////            auto picBg2 = CCLoadSprite::createSprite("UI_paper_3.png");
////            picBg2->setAnchorPoint(ccp(0, 0));
////            picBg2->setPosition(ccp(0, curHeight));
////            tBatchNode->addChild(picBg2);
////            curHeight += picBg2->getContentSize().height;
////        }
//        m_bgNode->addChild(tBatchNode);
        m_buildBG->setPositionY(m_buildBG->getPositionY() - extH);
    }
    
    changeBGHeight(m_buildBG);
//    this->m_totalNode->removeChild(this->m_rewardNode);
    this->m_rewardNode->removeFromParent();
    this->m_nameText->setFntFile("Arial_Bold.fnt");
    this->m_titleText->setFntFile("Arial_Bold_Regular.fnt");
    this->m_timeText->setFntFile("Arial_Bold_Regular.fnt");
    //this->m_guideTxt->setFntFile("Arial_Bold.fnt");
    this->m_nameText->setString(m_info->fromName.c_str());
    string nameExt = m_info->fromName;
    if(m_info->alliance!=""){
        nameExt = "("+m_info->alliance+")"+m_info->fromName;
        this->m_nameText->setString(nameExt);
    }
    if(m_info->type==MAIL_Alliance_ALL){
        this->m_nameText->setString(nameExt+_lang("105589"));
    }
    if(m_info->type==MAIL_INVITE_TELEPORT){
        this->m_nameText->setString(m_info->inviterName.c_str());
    }
    this->m_titleText->setString(m_info->title.c_str());
    this->m_timeText->setString(CCCommonUtils::timeStampToDate(m_info->createTime).c_str());
    int titleHeight = m_titleText->getContentSize().height * m_titleText->getOriginScaleY();
    int timeHeight = m_timeText->getContentSize().height * m_timeText->getOriginScaleY();
    if (titleHeight >= timeHeight) {
        m_underLineNode->setPositionY(m_underLineNode->getPositionY() - titleHeight + timeHeight);
    }
    int w = m_contentContainer->getContentSize().width;
    int h = m_contentContainer->getContentSize().height;
    m_scroll = CCScrollView::create(CCSize(w, h));
    m_scroll->setAnchorPoint(ccp(0, 1));
    m_scroll->setTouchPriority(1);
    m_scroll->setDirection(kCCScrollViewDirectionVertical);
    m_contentContainer->addChild(m_scroll);
    m_ListNode = CCNode::create();
    m_blendFlag = true;
    m_scroll->addChild(m_ListNode);
    if(!m_info->isReadContent){
        MailController::getInstance()->readMailFromServer(m_info->uid, CC_ITOA(m_info->type));
        GameController::getInstance()->showWaitInterface();
    }else{
        refreshContent(NULL);
    }
    m_headImgNode = HFHeadImgNode::create();
    bool showCustomPic = false;
    int picVer = 0;
    string uid = "";
    CCSprite* pic;
    if(m_info->type!=MAIL_USER&&m_info->type != MAIL_SELF_SEND&&m_info->type != MAIL_ALLIANCEINVITE&&m_info->type!=MAIL_MOD_SEND&&m_info->type != MAIL_MOD_PERSONAL && m_info->type != MAIL_ALLIANCEAPPLY && m_info->type != MAIL_INVITE_TELEPORT && m_info->type != MAIL_ALLIANCE_KICKOUT && m_info->type != MAIL_REFUSE_ALL_APPLY){
        if (m_info->fromName.compare(_lang("3140051")) == 0) {
            pic = CCLoadSprite::createSprite("mail_princessIcon.png");
        }
        else
            pic = CCLoadSprite::createSprite("guidePlayerIcon.png");
        pic->setScale(0.5);
    } else if (m_info->type == MAIL_ALLIANCEAPPLY) {
        showCustomPic = true;
        picVer = m_info->picVer;
        uid = m_info->fromUid;
        if(m_info->pic=="0"||m_info->pic==""){
            string mpic = "g044";
            mpic = mpic+".png";
            pic = CCLoadSprite::createSprite(mpic.c_str());
        }else{
            std::string head = m_info->pic;
            head.append(".png");
            pic = CCLoadSprite::createSprite(head.c_str());
        }
       
        CCCommonUtils::setSpriteMaxSize(pic, 70);
    }else{
        showCustomPic = true;
        if(m_info->pic=="0"||m_info->pic==""){
            string mpic = GlobalData::shared()->playerInfo.pic;
            mpic = mpic+".png";
            pic = CCLoadSprite::createSprite(mpic.c_str());
            picVer = GlobalData::shared()->playerInfo.picVer;
            uid = GlobalData::shared()->playerInfo.uid;
        }else{
            std::string head = m_info->pic;
            head.append(".png");
            pic = CCLoadSprite::createSprite(head.c_str());
            picVer = m_info->picVer;
            uid = m_info->fromUid;
        }
        CCCommonUtils::setSpriteMaxSize(pic, 70);
    }
//    m_userHeadContainer->addChild(pic);
//    pic->setPositionY(pic->getPositionY() - 3);
//    if (showCustomPic && CCCommonUtils::isUseCustomPic(picVer))
//    {
//        m_headImgNode->initHeadImgUrl2(m_userHeadContainer, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 70, true, ccp(0, -3));
//    }
    
    ////////融合底图
    if (m_blendFlag) {
        auto sizeLayer = CCSize(80, 80);
        m_selfModelLayer = CCRenderTexture::create(sizeLayer.width, sizeLayer.height);
        m_selfModelLayer->setAnchorPoint(ccp(0.5, 0.5));
        ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
        auto spr = CCLoadSprite::createSprite("Mail_headBack.png");
        spr->setScale(1);
        spr->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
        auto bgCircle = CCLoadSprite::createSprite("Mail_head_backBattle.png");
        bgCircle->setBlendFunc(cbf);
        bgCircle->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
        pic->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 3));
        //    pic->removeFromParent();
        m_selfModelLayer->begin();
        spr->visit();
        pic->visit();
        bgCircle->visit();
        m_selfModelLayer->end();
        m_userHeadContainer->addChild(m_selfModelLayer);
        if (showCustomPic && CCCommonUtils::isUseCustomPic(picVer) && uid != "")
        {
            string backImg = "Mail_headBack.png";
            string renderImg = "Mail_head_backBattle.png";
           m_headImgNode->initHeadImgUrl3(m_userHeadContainer, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 74, true, ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 2), sizeLayer, backImg, renderImg);//  simon
        }
    }
    
    return true;
}
void MailReadPopUpView::setAllBtnPosition(){
    
    auto btnPic = CCLoadSprite::createScale9Sprite("Mail_btn04.png");
    m_blockBtn =CCControlButton::create(btnPic);
    m_blockBtn->setPreferredSize(CCSizeMake(170, 64));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_blockBtn->setScale(2.4);
    }
    m_blockBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailReadPopUpView::onBlockClick), CCControlEventTouchUpInside);
    auto btngrayPic = CCLoadSprite::createScale9Sprite("Mail_btn06.png");
    m_blockBtn->setBackgroundSpriteForState(btngrayPic, CCControlStateDisabled);
    CCCommonUtils::setButtonTitle(m_blockBtn, _lang("105312").c_str());
    m_blockBtn->setVisible(false);
    
    auto btnPic1 = CCLoadSprite::createScale9Sprite("Mail_btn03.png");
    m_rewardBtn =CCControlButton::create(btnPic1);
    m_rewardBtn->setEffectStr(Music_Sfx_UI_collect_item);
    m_rewardBtn->setPreferredSize(CCSizeMake(170, 64));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_rewardBtn->setScale(2.4);
    }
    m_rewardBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailReadPopUpView::onRewardClick), CCControlEventTouchUpInside);
    auto btngrayPic1 = CCLoadSprite::createScale9Sprite("Mail_btn06.png");
    m_rewardBtn->setBackgroundSpriteForState(btngrayPic1, CCControlStateDisabled);
    CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("105572").c_str());
    m_rewardBtn->setVisible(false);
    
    auto btnPic2 = CCLoadSprite::createScale9Sprite("Mail_btn05.png");
    m_replyBtn =CCControlButton::create(btnPic2);
    m_replyBtn->setPreferredSize(CCSizeMake(170, 64));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_replyBtn->setScale(2.4);
    }
    m_replyBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailReadPopUpView::onReplyClick), CCControlEventTouchUpInside);
    auto btngrayPic2 = CCLoadSprite::createScale9Sprite("Mail_btn06.png");
    m_replyBtn->setBackgroundSpriteForState(btngrayPic2, CCControlStateDisabled);
    CCCommonUtils::setButtonTitle(m_replyBtn, _lang("105506").c_str());
    m_replyBtn->setVisible(false);
    
    auto btnPic4 = CCLoadSprite::createScale9Sprite("Mail_btn03.png");
    m_donateBtn =CCControlButton::create(btnPic4);
    m_donateBtn->setPreferredSize(CCSizeMake(170, 64));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_donateBtn->setScale(2.4);
    }
    m_donateBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailReadPopUpView::onDonateClick), CCControlEventTouchUpInside);
    CCCommonUtils::setButtonTitle(m_donateBtn, _lang("115495").c_str());
    m_donateBtn->setVisible(false);
    
    auto btnPic3 = CCLoadSprite::createScale9Sprite("Mail_btn03.png");
    auto btngrayPic3 = CCLoadSprite::createScale9Sprite("Mail_btn06.png");
    string fbIcon = "quest_facebook_icon1.png";
    string btnLang = "107113";
    if (GlobalData::shared()->analyticID == "cn1") {
        fbIcon = "weibo.png";
        btnLang = "4100010";
    }
    auto fb = CCLoadSprite::createSprite(fbIcon.c_str());
    fb->setAnchorPoint(ccp(0.5, 0.5));
    CCCommonUtils::setSpriteMaxSize(fb, 80,true);
    m_likeBtn =CCControlButton::create(btnPic3);
    m_likeBtn->setPreferredSize(CCSizeMake(170, 64));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_likeBtn->setScale(2.4);
    }
    m_likeBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailReadPopUpView::onFbLikeClick), CCControlEventTouchUpInside);
    m_likeBtn->setLabelAnchorPoint(ccp(0.12, 0.5));
    CCCommonUtils::setButtonTitle(m_likeBtn, _lang(btnLang).c_str());
    m_likeBtn->addChild(fb);
    m_likeBtn->setVisible(false);
    m_likeBtn->setSelected(false);
    fb->setPosition(ccp(30, m_likeBtn->getContentSize().height/2));
//    this->m_ListNode->addChild(m_blockBtn);
//    m_totalH -= 64;
//    m_detailBtn->setPosition(m_listContainer->getContentSize().width/2, m_totalH-10);
//    m_Listheight+=74;
   
    m_totalH-=10;
    CCArray* btnArr = CCArray::create();
    //1.blockBtn show conditions
    if(m_info->type ==MAIL_USER||m_info->type==MAIL_Alliance_ALL/*||m_info.type==MAIL_ALLIANCEINVITE*/){
        this->m_ListNode->addChild(m_blockBtn);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_blockBtn->setPosition(0, m_totalH-77);
        }
        else
            m_blockBtn->setPosition(0, m_totalH-32);
        m_blockBtn->setVisible(true);
        btnArr->addObject(m_blockBtn);
    }
    //2. rewardBtn show conditons
    
    if(m_info->rewardId!="" && checkRewardBtnVisible()){
        this->m_ListNode->addChild(m_rewardBtn);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_rewardBtn->setPosition(0, m_totalH-77);
        }
        else
            m_rewardBtn->setPosition(0, m_totalH-32);
        if(m_info->rewardStatus==0){
        
        }else{
            m_rewardBtn->setEnabled(false);
        }
       
        m_rewardBtn->setVisible(true);
        btnArr->addObject(m_rewardBtn);
    }
    if (m_info->like==1 && !GlobalData::shared()->isXiaoMiPlatForm()) {
        this->m_ListNode->addChild(m_likeBtn);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_likeBtn->setPosition(0, m_totalH-77);
        }
        else
            m_likeBtn->setPosition(0, m_totalH-32);
        m_likeBtn->setVisible(true);
        btnArr->addObject(m_likeBtn);
    }
    
    if (m_info->goToDonate == 1) {
        this->m_ListNode->addChild(m_donateBtn);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_donateBtn->setPosition(0, m_totalH-77);
        }
        else
            m_donateBtn->setPosition(0, m_totalH-32);
        m_donateBtn->setVisible(true);
        btnArr->addObject(m_donateBtn);
    }
    
    //3.replayBtn show conditions
    if(m_info->type ==MAIL_USER||m_info->type==MAIL_Alliance_ALL/*||m_info.type==MAIL_ALLIANCEINVITE*/||(m_info->type==ALL_SERVICE&&(m_info->reply==1))){
        this->m_ListNode->addChild(m_replyBtn);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_replyBtn->setPosition(0, m_totalH-77);
        }
        else
            m_replyBtn->setPosition(0, m_totalH-32);
        m_replyBtn->setVisible(true);
        btnArr->addObject(m_replyBtn);
    }else if(m_info->type == MAIL_ALLIANCE_PACKAGE){
        setButtonTitle(m_replyBtn, _lang("101386").c_str());
        this->m_ListNode->addChild(m_replyBtn);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_replyBtn->setPosition(0, m_totalH-77);
        }else{
            m_replyBtn->setPosition(0, m_totalH-32);
        }
        m_replyBtn->setVisible(true);
        m_replyBtn->setEnabled(!m_info->isThanks);
        btnArr->insertObject(m_replyBtn, 0);
//        btnArr->addObject(m_replyBtn);
    }
    int count = btnArr->count();
    int wh = m_contentContainer->getContentSize().width;

    if(count==1){
        auto btn =dynamic_cast<CCControlButton*>(btnArr->objectAtIndex(0));
        if (CCCommonUtils::isIosAndroidPad()) {
            btn->setPosition(wh/2, m_totalH-77);
        }
        else
            btn->setPosition(wh/2, m_totalH-32);
        
    }else if(count==2){
        auto btn =dynamic_cast<CCControlButton*>(btnArr->objectAtIndex(1));
        auto btn1 =dynamic_cast<CCControlButton*>(btnArr->objectAtIndex(0));
        if (CCCommonUtils::isIosAndroidPad()) {
            btn->setPosition(wh/2-10-408/2, m_totalH-77);
            btn1->setPosition(wh/2+10+408/2, m_totalH-77);
        }
        else {
            btn->setPosition(wh/2-10-170/2, m_totalH-32);
            btn1->setPosition(wh/2+10+170/2, m_totalH-32);
        }
    }else if(count==3){
        auto btn1 =dynamic_cast<CCControlButton*>(btnArr->objectAtIndex(0));
        auto btn =dynamic_cast<CCControlButton*>(btnArr->objectAtIndex(1));
        auto btn2 =dynamic_cast<CCControlButton*>(btnArr->objectAtIndex(2));
        if (CCCommonUtils::isIosAndroidPad()) {
            btn1->setPosition(wh/2 -504, m_totalH-77);
            btn->setPosition(wh/2, m_totalH-77);
            btn2->setPosition(wh/2+504, m_totalH-77);
        }
        else {
            btn1->setPosition(wh/2 -210, m_totalH-32);
            btn->setPosition(wh/2, m_totalH-32);
            btn2->setPosition(wh/2+210, m_totalH-32);
        }
    }
    btnArr->removeAllObjects();
    if(GlobalData::shared()->chatShieldInfo.isShield(m_info->fromUid)){
        m_blockBtn->setEnabled(false);
        CCCommonUtils::setButtonTitle(m_blockBtn, _lang("101219").c_str());
    }
    
    if(m_rewardBtn->isVisible()&&m_info->rewardStatus==0){
        auto sprAnim = CCLoadSprite::createSprite("Mail_btn00.png");
//        this->m_ListNode->addChild(sprAnim);
//        float wh =m_rewardBtn->getPreferredSize().width;
//        float dh = m_rewardBtn->getPreferredSize().height;
//        sprAnim->setScaleX((m_rewardBtn->getPreferredSize().width)/sprAnim->getContentSize().width);
//        sprAnim->setScaleY((m_rewardBtn->getPreferredSize().height)/sprAnim->getContentSize().height);
//        ccBlendFunc cbf = {GL_ONE,GL_ONE};
//        sprAnim->setBlendFunc(cbf);
//        sprAnim->setPosition(m_rewardBtn->getPosition());
//        sprAnim->setOpacity(0);
//        sprAnim->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(1,80),CCFadeTo::create(1,0),NULL)));
        
    }
//    if(m_info.type == MAIL_FRESHER || m_info.type == MAIL_DIGONG){
////        m_line->setVisible(false);
//        m_nameText->setString("");
//        m_guideNode->setVisible(true);
//        m_userHeadContainer->setVisible(false);
//    }
    if(count>0){
        if (CCCommonUtils::isIosAndroidPad()) {
            m_totalH-=154;
        }
        else
            m_totalH-=64;
    }
    //    m_deleteBtnTitle->setString(_lang("108523").c_str());
    
    
    if(m_info->save==0){
        m_addSaveBtn->setHighlighted(false);
        
    }else{
        m_addSaveBtn->setHighlighted(true);
    }
    
    if(m_info->type == MAIL_SELF_SEND || m_info->type == MAIL_MOD_PERSONAL){
        m_addSaveBtn->setVisible(false);
    }
}
bool MailReadPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_returnSpr, pTouch)) {
        return true;
    }
    return false;
}
void MailReadPopUpView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void MailReadPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_returnSpr, pTouch)) {
        PopupViewController::getInstance()->goBackPopupView();
    }
}
bool MailReadPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentContainer", CCNode*, this->m_contentContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_userHeadContainer", CCNode*, this->m_userHeadContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_userHeadBack", CCSprite*, this->m_userHeadBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailTitle", CCLabelIF*, this->m_mailTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_writeBtn", CCControlButton*, this->m_writeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnBtn", CCControlButton*, this->m_returnBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnSpr", CCSprite*, this->m_returnSpr);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardContainer", CCNode*, this->m_rewardContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listBG", CCScale9Sprite*, this->m_listBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kuangBG", CCScale9Sprite*, this->m_kuangBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_line", CCScale9Sprite*, this->m_line);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideNode", CCNode*, this->m_guideNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardNode", CCNode*, this->m_rewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardTitleText", CCLabelIF*, this->m_rewardTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_underLineNode", Node*, this->m_underLineNode);
    return false;
}

SEL_CCControlHandler MailReadPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", MailReadPopUpView::onDeleteClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBlockClick", MailReadPopUpView::onBlockClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReplyClick", MailReadPopUpView::onReplyClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardClick", MailReadPopUpView::onRewardClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", MailReadPopUpView::onAddSaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturnClick", MailReadPopUpView::onReturnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteClick", MailReadPopUpView::onWriteClick);
    return NULL;
}


void MailReadPopUpView::refrehsReward(){
    int w = this->m_contentContainer->getContentSize().width;
    if(m_info->rewardId != ""){
        m_totalH-=10;
        m_ListNode->addChild(this->m_rewardNode);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->m_rewardNode->setPosition(768, m_totalH);
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
//                m_kuangBG->setContentSize(CCSize(m_kuangBG->getContentSize().width, 384+(count-1)*ceHG));
            }
            else {
                m_listBG->setContentSize(CCSize(m_listBG->getContentSize().width, 120+(count-1)*ceHG));
//                m_kuangBG->setContentSize(CCSize(m_kuangBG->getContentSize().width, 160+(count-1)*ceHG));
            }
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
            //if(type == R_GOODS || type == R_GENERAL){
                //DetailRewardCell *cell = DetailRewardCell::create(type, value, num,0,gray);
               // resArr->addObject(cell);
            if(!gray){
               //  DetailRewardCell *cell1 = DetailRewardCell::create(type, value, num,0);
               // m_listAnimArr->addObject(cell1);
            }
            //}else{
             //   ReportNormalRewardCell *cell = ReportNormalRewardCell::create(type, num,num);
             //   resArr->addObject(cell);
            //}
            
 
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
            if(type == R_GOODS){
//                auto titer = ToolController::getInstance()->m_toolInfos.find(value);
//                if(titer!=ToolController::getInstance()->m_toolInfos.end()){
//                    nameStr = (*titer).second.getName();
//                }else{
                    nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
//                }
                CCCommonUtils::createGoodsIcon(value, icon0, CCSize(114, 114));
//                if(CCCommonUtils::isIosAndroidPad())
//                    cellNode->setPositionX(180);
//                else
//                    cellNode->setPositionX(10+57);
//                icon0->setPositionX(0);
//                icon0->setPositionY(0);
                if(m_info->rewardStatus==1 && icon0->getChildByTag(GOODS_ICON_TAG)){
                    CCSprite *icon = dynamic_cast<CCSprite*>(icon0->getChildByTag(GOODS_ICON_TAG));
                    if(icon){
                        icon->setColor({90,85,81});
                    }
                }
            }else if(type == R_GENERAL){
                nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
                picStr =CCCommonUtils::getIcon(CC_ITOA(value));
                //float sacle = 1.0;
                if(picStr==".png"){
                    picStr = "no_iconFlag.png";
                    sacle = 94/98;
                }
                auto icon  = CCLoadSprite::createSprite(picStr.c_str());
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
                    nameStr = info.getName();
                    auto iconBG = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info.color).c_str());
                    CCCommonUtils::setSpriteMaxSize(iconBG, 85);
                    cellNode->addChild(iconBG,0);
                    iconBG->setPosition(icon->getPosition());
                }
                
            }else if(type == R_HONOR || type == R_ALLIANCE_POINT || type == R_CRYSTAL){
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
            }else if(type == R_EQUIP){
                if(EquipmentController::getInstance()->EquipmentInfoMap.find(value)!=EquipmentController::getInstance()->EquipmentInfoMap.end()){
                    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[value];
                    nameStr = _lang(eInfo.name) + "   "+_lang("102203")+" " + CC_ITOA(eInfo.level);
                    string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
                    auto iconBg = CCLoadSprite::createSprite(bgPath.c_str());
                    iconBg->setPosition(ccp(40+49, 0+49));
                    CCCommonUtils::setSpriteMaxSize(iconBg, 85, true);
                    cellNode->addChild(iconBg);
                    picStr = CCCommonUtils::getIcon(CC_ITOA(value));
                    auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_EQUIP);
                    icon->setPosition(ccp(10+57, 57));
                    CCCommonUtils::setSpriteMaxSize(icon, 94, true);
                    cellNode->addChild(icon);
                    if(m_info->rewardStatus==1){
                        icon->setColor({90,85,81});
                    }
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
            
            cellNode->addChild(label);
            auto label1 = CCLabelIF::create();
            label1->setFntFile("Arial_Bold_Regular.fnt");
            label1->setFontSize(20);
            label1->setColor({156,18,0});
            label1->setAnchorPoint(ccp(1.0, 0.5));
            
            label1->setString(CC_CMDITOA(num));
            //int width =bg->getContentSize().width/2;
            
            cellNode->addChild(label1);
            
            
            if(m_info->rewardStatus==1){
                label->setColor({90,85,81});
                label1->setColor({90,85,81});
                
            }
            if (type == R_GOODS) {
                if (CCCommonUtils::isIosAndroidPad()) {
                    label->setPosition(120, 80);
                    label1->setPosition(620, 20);
                    cellNode->setPositionY(startY-i*ceHG);
                }
                else {
                    label->setPosition(120, 80);
                    label1->setPosition(620, 20);
                    cellNode->setPositionY(startY-i*ceHG);
                }
            }else{
                if (CCCommonUtils::isIosAndroidPad()) {
                    label->setPosition(120, 80);
                    label1->setPosition(620, 20);
                    cellNode->setPositionY(startY-i*ceHG);
                }
                else {
                    label->setPosition(120, 80);
                    label1->setPosition(620, 20);
                    cellNode->setPositionY(startY-i*ceHG);
                }
            }
            
            i++;
        }
        m_listArr = resArr;
    }
   //showRewardAnim();
    
}
void MailReadPopUpView::showRewardAnim(){

}

void MailReadPopUpView::showAllianceOrderBtn()//显示联盟指令跳转按钮
{
    _allianceOrderType = -1;
    if (m_info->contents == _lang("115437") ) {
        _allianceOrderType = 0;
    }else if (m_info->contents == _lang("115438") ) {
        _allianceOrderType = 1;
    }else if (m_info->contents == _lang("115439") ) {
        _allianceOrderType = 2;
    }else if (m_info->contents == _lang("115440") ) {
        _allianceOrderType = 3;
    }else if (m_info->contents == _lang("115441") ) {
        _allianceOrderType = 4;
    }else if (m_info->contents == _lang("115442") ) {
        _allianceOrderType = 5;
    }
    
    string btnMsg = "";
    if (_allianceOrderType == 0) {//去采集
        btnMsg = _lang("115554");
    }else if (_allianceOrderType == 1) {//打开联盟科技 联盟捐献
        btnMsg = _lang("115495");
    } else if (_allianceOrderType == 2) {//联盟帮助
        btnMsg = _lang("115555");
    } else if (_allianceOrderType == 3) {//建设联盟领地
        btnMsg = _lang("115558");
    } else if (_allianceOrderType == 4) {//参加联盟战争
        btnMsg = _lang("115556");
    } else if (_allianceOrderType == 5) {//联盟商店
        btnMsg = _lang("115557");
    } else {
        return;
    }
    
    auto btnPic = CCLoadSprite::createScale9Sprite("Mail_btn03.png");
    auto m_orderBtn =CCControlButton::create(btnPic);
    m_orderBtn->setPreferredSize(CCSizeMake(170, 64));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_orderBtn->setScale(2.4);
    }
    m_orderBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailReadPopUpView::onGoToAllianceOrder), CCControlEventTouchUpInside);
    CCCommonUtils::setButtonTitle(m_orderBtn, btnMsg.c_str());
    
    int wh = m_contentContainer->getContentSize().width;
    this->m_ListNode->addChild(m_orderBtn);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_orderBtn->setPosition(wh/2, m_totalH-77);
    }
    else {
        m_orderBtn->setPosition(wh/2, m_totalH-32);
    }
}

void MailReadPopUpView::onGoToAllianceOrder(cocos2d::CCObject *pSender, CCControlEvent event)
{
    
    AutoSafeRef temp(this);
    
    PopupViewController::getInstance()->removeAllPopupView();
    //联盟指令跳转功能按钮 事件
    if (_allianceOrderType == 0) {//去世界采集
        
        if (SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD) {
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
        }
        
    } else if (_allianceOrderType == 1) {//打开联盟科技 联盟捐献
        PopupViewController::getInstance()->addPopupInView(AllianceScienceView::create());
    } else if (_allianceOrderType == 2) {//联盟帮助
        PopupViewController::getInstance()->addPopupInView(AllianceHelpView::create());
    } else if (_allianceOrderType == 3) {//建设联盟领地
        unsigned int index = WorldController::getIndexByPoint(WorldController::getInstance()->selfPoint);
        PopupViewController::getInstance()->addPopupInView(AllianceTerritoryView::create(index,false));
    } else if (_allianceOrderType == 4) {//参加联盟战争
        PopupViewController::getInstance()->addPopupInView(AllianceWarView::create());
    } else if (_allianceOrderType == 5) {//联盟商店
//        auto view = AllianceShopView::create(); simon
//        view->showR4Store();
//        PopupViewController::getInstance()->addPopupInView(view);
    }
    
}

void MailReadPopUpView::onDeleteClick(cocos2d::CCObject *pSender, CCControlEvent event){
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
    YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(MailReadPopUpView::onOkDeleteMail)),false);

}

void MailReadPopUpView::onRewardClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    if(m_info->mbLevel>0 && m_info->mbLevel >= FunBuildController::getInstance()->getMainCityLv())
    {
        CCCommonUtils::flyText(_lang_1("105784", CC_ITOA(m_info->mbLevel)));
        return;
    }
    this->m_rewardBtn->setEnabled(false);
    MailGetRewardCommand* cmd = new MailGetRewardCommand(m_info->uid, m_info->type);
    cmd->sendAndRelease();
}
void MailReadPopUpView::onOkDeleteMail(){
    MailController::getInstance()->removeMail(m_info->uid, CC_ITOA(m_info->type));
    //    this->closeSelf();
    PopupViewController::getInstance()->goBackPopupView();
}
void MailReadPopUpView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
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
void MailReadPopUpView::onReturnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->goBackPopupView();
}

void MailReadPopUpView::onWriteClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create("", ""));
}
void MailReadPopUpView::onBlockClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    
    YesNoDialog::show(_lang_1("105313",m_info->fromName.c_str()), CCCallFunc::create(this, callfunc_selector(MailReadPopUpView::blockAction)),0,false);
}
void MailReadPopUpView::blockAction(){
    ChatLockCommand* cmd = new ChatLockCommand(m_info->fromUid);
    cmd->sendAndRelease();
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    PopupViewController::getInstance()->goBackPopupView();
}
void MailReadPopUpView::onReplyClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    if(m_info->type == MAIL_USER||m_info->type==MAIL_Alliance_ALL/*||m_info.type==MAIL_ALLIANCEINVITE*/){
//        this->closeSelf();
        //PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create(m_info.fromName, ""));
        this->retain();
        PopupViewController::getInstance()->removeAllPopupView();
        MailController::getInstance()->openMailDialogViewFirst(m_info->fromName, m_info->fromUid);
        this->release();
    }else if(m_info->type==ALL_SERVICE){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        GameController::getInstance()->platformCollectUserInfo(GlobalData::shared()->playerInfo.uid,GlobalData::shared()->playerInfo.name,CC_ITOA(GlobalData::shared()->playerInfo.level));
#else
        HelpshiftCocos2dx::setUserIdentifier(GlobalData::shared()->playerInfo.uid.c_str());
        HelpshiftCocos2dx::setNameAndEmail(GlobalData::shared()->playerInfo.name.c_str(),"");
#endif
        ValueMap meta;
        HelpshiftCocos2dx::addProperties(meta); //simon

        ValueVector tags;
        string server_id = CCUserDefault::sharedUserDefault()->getStringForKey(SERVER_ID,"");
        string timestr = CCCommonUtils::timeStampToYMD(m_info->createTime);
        tags.push_back(Value(timestr));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        tags.push_back(Value("ios"));
#else
        if(GlobalData::shared()->analyticID != "market_global")
            tags.push_back(Value(GlobalData::shared()->analyticID));
#endif
        if (server_id != "") {
            tags.push_back(Value(server_id));
        }
        //添加策划反馈标签
        string chfkStr = "策划反馈";
        tags.push_back(Value(chfkStr));
        if (m_info->isBanMail) {
            ////添加被ban邮件回复的标签
            string banStr = "ban";
            tags.push_back(Value(banStr));
        }
        meta[HS_TAGS_KEY] = tags;
        
        ValueMap config;
        config[HS_META_DATA_KEY] = meta;
        GlobalData::shared()->isBind = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        GlobalData::shared()->isBind = false;
#endif
        HelpshiftCocos2dx::showConversation(config); //simon
//                PopupViewController::getInstance()->addPopupInView(SuggestionView::create());
    }else if(m_info->type == MAIL_ALLIANCE_PACKAGE){
        string tmpStr = _lang_1("101046", m_info->crGroupId.c_str());
        if(!tmpStr.empty()){
            CCArray* array = CCArray::create();
            array->addObject(CCString::create(m_info->crGroupId));
//            ChatController::getInstance()->sendCountryChat(tmpStr.c_str(), CHAT_ALLIANCE,0,"","101046",array,m_info->uid); simon
            CCCommonUtils::flyHint("", "", _lang("101049").c_str());
            m_replyBtn->setEnabled(false);
            m_info->isThanks=true;
        }
    }
    CCLog("replay");
}

void MailReadPopUpView::onFbLikeClick(CCObject *pSender, CCControlEvent event){
    if (GlobalData::shared()->isXiaoMiPlatForm()|| GlobalData::shared()->analyticID == "cn1") {
        cocos2d::extension::CCDevice::updateVersion("http://weibo.com/liewangdefenzheng");
        return ;
    }
    std::string language = LocalController::shared()->getLanguageFileName();
    if(language=="es"){//西语关联网址
        cocos2d::extension::CCDevice::updateVersion("https://www.facebook.com/Clash-of-Kings-Espa%C3%B1ol-519397994874228/timeline/");
        return;
    }
    else if (language=="pt"){//葡语关联网址
        cocos2d::extension::CCDevice::updateVersion("https://m.facebook.com/Clash.Of.Kings.PT");
        return;
    }
    else if (language=="de"){//德语关联网址
        cocos2d::extension::CCDevice::updateVersion("https://www.facebook.com/Clash-of-Kings-Deutschland-1429065620745662/timeline/");
        return;
    }
    GlobalData::shared()->isBind = true;
    FBUtilies::fbLike();
}

void MailReadPopUpView::onDonateClick(CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    this->retain();
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(AllianceScienceView::create());
    this->release();
}

void MailReadPopUpView::refreshContent(CCObject* p){
    int txtW = 580;
    if (CCCommonUtils::isIosAndroidPad()) {
        txtW = 1392;
    }
//    if (m_info.type == MAIL_FRESHER || m_info.type == MAIL_DIGONG) {
//        if (CCCommonUtils::isIosAndroidPad()) {
//            m_contentContainer->setContentSize(CCSizeMake(912, m_contentContainer->getContentSize().height-30));
//            m_contentContainer->setPosition(ccp(-216, m_contentContainer->getPositionY()+192));
//            txtW = 912;
//        }
//        else {
//            m_contentContainer->setContentSize(CCSizeMake(380, m_contentContainer->getContentSize().height-30));
//            m_contentContainer->setPosition(ccp(-90, m_contentContainer->getPositionY()+80));
//            txtW = 380;
//        }
//    }
    m_totalH = 0;
    string littleStr="";
    if(m_info->rewardId!=""&&m_info->mbLevel>0 && m_info->mbLevel >= FunBuildController::getInstance()->getMainCityLv())
    {
        littleStr=_lang_1("105784", CC_ITOA(m_info->mbLevel));
        littleStr+="\r\n \r\n";
    }
    if (littleStr.empty())
        littleStr = "";
    string tempstr = littleStr+m_info->contents;
//    tempstr = "亲爱的领主们，\n我们需要您们帮助我们与Wyne The Poo的家人取得联系，去年由癌症去世的那个马来西亚玩家！如果您知道谁是她的父母，或如何达到联系他们，请您在我们Facebook粉丝页上 <a href=\"http://dwz.cn/DbPmU/\">http://dwz.cn/DbPmU</a>和我们联系。\n我们知道，送礼是春节在马来西亚的一个重要组成部分，我们希望捐赠一万美元给她的父母作为一个农历新年礼物，像她的家人表示诚挚的慰问。让我们一起来做这件事，我们所有的人，因为我们是《列王的纷争》社会。\n非常感谢！";
//    tempstr = tempstr + "\n";
//    tempstr = tempstr + "亲爱的领主们，\n<a href=\"https://www.facebook.com/\">facebook</a>we are all excepted that you will like it. Thanks. <a href=\"https://www.facebook.com/Clash.Of.Kings.Game/\">https://www.facebook.com/Clash.Of.Kings.Game/</a>";

//    auto label = CCLabelIFTTF::create();
//    label->setFontSize(22);
//    label->setAlignment(kCCTextAlignmentLeft);
//    label->setDimensions(CCSize(txtW-30, 0));
//    label->setColor(ccc3(56, 8, 0));
//    int w = m_contentContainer->getContentSize().width;
//    int h = m_contentContainer->getContentSize().height;
//    if(m_info.type == MAIL_DIGONG){
//        std::string m_str = "";
//        if(m_info.monsters && m_info.monsters->count()>0){
//            for (int i=0; i<m_info.monsters->count();i++){
//                m_str.append(_lang_1("108674", CC_ITOA(i+1)));
//                MonsterSearchResultInfo *info = dynamic_cast<MonsterSearchResultInfo*>(m_info.monsters->objectAtIndex(i));
//                if(info==NULL) continue;
//                if(info->type == FIGHT){
//                    if(info->fType == BATTLE_WIN){
//                        m_str += _lang("108661") + _lang_2("108663", CCCommonUtils::getNameById(info->npcId).c_str(), CC_ITOA(info->exp));
//                    }else{
//                        m_str += _lang("108662") + _lang_2("108663", CCCommonUtils::getNameById(info->npcId).c_str(), CC_ITOA(info->exp));
//                    }
//                }else if(info->type == CHEST){
//                    m_str += _lang_1("108664", CCCommonUtils::getNameById(info->itemId).c_str());
//                }else if(info->type == LOST_SOLDIER){
//                    CCDictElement *element;
//                    std::string addStr = "";
//                    auto dic = info->dead;
//                    CCDICT_FOREACH(dic, element){
//                        std::string armyId = element->getStrKey();
//                        int value = info->dead->valueForKey(armyId)->intValue();
//                        if(value>0){
//                            addStr = addStr + CCCommonUtils::getNameById(armyId) + " * " + CC_ITOA(value) + " ";
//                        }
//                    }
//                    m_str += _lang_1("108665", addStr.c_str());
//                }else if(info->type == RESOURCE){
//                    vector<std::string> vector1;
//                    vector<std::string> vector;
//                    CCCommonUtils::splitString(info->resType, "|", vector);
//                    CCCommonUtils::splitString(info->resValue, "|", vector1);
//                    int i = 0;
//                    std::string addStr = "";
//                    while(i < vector.size()){
//                        int type = atoi(vector[i].c_str());
//                        int value = atoi(vector1[i].c_str());
//                        if(value>0){
//                            std::string typeStr = CCCommonUtils::getResourceNameByType(type);
//                            addStr = addStr + typeStr + " * " + CC_ITOA(value) + " ";
//                        }
//                        i++;
//                    }
//                    m_str += _lang_1("108664", addStr.c_str());
//                }
//                m_str += "\n";
//            }
//            
//            label->setString(m_str.c_str());
//        }
//    }
//    // this->m_scroll->removeAllChildren();
//    
//    float scale =label->getOriginScaleY();
//    
//    //    m_scroll = CCScrollView::create(CCSize(w, h));
//    //    m_scroll->setAnchorPoint(ccp(0, 1));
//    //    m_scroll->setTouchPriority(1);
//    //    m_scroll->setDirection(kCCScrollViewDirectionVertical);
//    //    m_contentContainer->addChild(m_scroll);
    
    int fontSize = 22;
    if (CCCommonUtils::isIosAndroidPad()) {
        fontSize = 53;
    }
    
    CCRichLabelTTF *richLabel= CCRichLabelTTF::create("", "Helvetica", fontSize,CCSize(txtW-30,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
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
                richLabel->setLinkTarget(index-1, CCCallFuncO::create(this, callfuncO_selector(MailReadPopUpView::onLinkClicked), NULL));
                --index;
            }
        }
        
//        if(posStart!=-1&&posMiddle!=-1&&posEnd!=-1){
//            if(posEnd>posMiddle&&posMiddle>posStart){
//                HtmlLabelTFNode* htmltext = HtmlLabelTFNode::create("", 22, txtW-30);
//                htmltext->setTextStr(tempstr);
//                htmltext->setPositionY(-htmltext->getContentSize().height);
//                this->m_ListNode->addChild(htmltext);
//                m_totalH-=htmltext->getContentSize().height;
//                label->setString("");
//            }else{
//                m_totalH -= label->getContentSize().height * scale;
//            }
//        }else{
//            m_totalH -= label->getContentSize().height * scale;
//        }
        // richLabel
        int contentWidth = txtW-30;
        if (CCCommonUtils::isIosAndroidPad()) {
            richLabel->setPosition(ccp(768 - contentWidth / 2,0));
        }
        else {
            richLabel->setPosition(ccp(320 - contentWidth / 2, 0));
        }
        m_totalH -= richLabel->getContentSize().height;
    }
    
    refrehsReward();
    showJoinAllianceBtn();
    showInviteTeleportBtn();
    showKickOutInterface();
    showRefuseApplyInterface();
    if(m_info->type==MAIL_ALLIANCEAPPLY){
        richLabel->setString("");
    }
    setAllBtnPosition();
    showShareBtn();
//    int i = 0;
//    while(i < arr->count()){
//        auto node = dynamic_cast<CCNode*>(arr->objectAtIndex(i++));
//        node->setPositionY(node->getPositionY() + m_totalH);
//    }
    //m_totalH += 30;
    this->m_ListNode->setPositionY(-m_totalH);
    m_scroll->setContentSize(CCSize(m_contentContainer->getContentSize().width, -m_totalH));
    m_scroll->setContentOffset(ccp(0, m_contentContainer->getContentSize().height - (-m_totalH)));
    
//    m_scroll->setContentSize(CCSize(m_contentContainer->getContentSize().width, m_totalH));
//    m_scroll->setContentOffset(ccp(0, -m_totalH + m_contentContainer->getContentSize().height));
    ckfRewardHandle();
    
    //添加联盟指令跳转按钮
    showAllianceOrderBtn();
}

void MailReadPopUpView::ckfRewardHandle(){
    if (m_info->ckfContents!="") {
        m_userHeadContainer->removeAllChildrenWithCleanup(true);
        m_blendFlag = false;
//        m_line->setVisible(false);
        m_nameText->setString("");
        m_guideNode->setVisible(false);
//        auto picBG = CCLoadSprite::createScale9Sprite("chuzheng_frame01.png");
//        picBG->setAnchorPoint(ccp(0.5, 0.5));
//        picBG->setContentSize(CCSize(640,250));
//        picBG->setPosition(ccp(0,0));
//        this->m_userHeadContainer->addChild(picBG);
        auto battlePic = CCLoadSprite::createSprite("Mail_monster.png");
        battlePic->setPosition(ccp(0,-60));
        this->m_userHeadContainer->addChild(battlePic);
        this->m_userHeadBack->setVisible(false);
        if (CCCommonUtils::isIosAndroidPad()) {
//            picBG->setPosition(ccp(0, -40));
//            battlePic->setPosition(ccp(0, -120));
//            m_contentContainer->setContentSize(CCSize(m_contentContainer->getContentSize().width, m_contentContainer->getContentSize().height - 120));
//            m_contentContainer->setPositionY(m_contentContainer->getPositionY() - 120);
        }
        m_userHeadContainer->setVisible(true);
        int w = m_contentContainer->getContentSize().width;
        int h = m_contentContainer->getContentSize().height - 100;
        if (CCCommonUtils::isIosAndroidPad()) {
            h = m_contentContainer->getContentSize().height - 240;
        }
        m_scroll->setViewSize(CCSize(w, h));
        m_scroll->setContentSize(CCSize(m_contentContainer->getContentSize().width, -m_totalH));
        m_scroll->setContentOffset(ccp(0, h - (-m_totalH)));
    }
}

void MailReadPopUpView::onLinkClicked(CCObject *ccObj) {
    CCLOG("on link clicked");
    if(ccObj){
        CCInteger *integer = dynamic_cast<CCInteger*>(ccObj);
        if(integer){
            int index = integer->getValue();
            if(index < m_linkUrls.size()){
                std::string url = m_linkUrls.at(index);
                if(!url.empty()){
                    url = LuaController::getInstance()->encodeUrl(url);
                    CCLOG("link url [%s]",url.c_str());
                    GameController::getInstance()->goTurntoUrl(url);
                }
            }
        }
    }
}
void MailReadPopUpView::showInviteTeleportBtn() {
    if (m_info->type == MAIL_INVITE_TELEPORT) {
//        m_totalH-=20;
        MailInviteTeleCell* cell = MailInviteTeleCell::create(m_info);
        m_ListNode->addChild(cell);
        if (CCCommonUtils::isIosAndroidPad()) {
            cell->setPosition(0, m_totalH - 1080);
            m_totalH -= 1080;
        }
        else {
            cell->setPosition(0, m_totalH - 450);
            m_totalH -= 450;
        }
    }
}

void MailReadPopUpView::showKickOutInterface() {
    if (m_info->type == MAIL_ALLIANCE_KICKOUT) {
//        m_totalH -= 20;
        MailAllianceKickCell* cell = MailAllianceKickCell::create(m_info);
        m_ListNode->addChild(cell);
        if (CCCommonUtils::isIosAndroidPad()) {
            cell->setPosition(0, m_totalH - 1440);
            m_totalH -= 1440;
        }
        else {
            cell->setPosition(0, m_totalH - 600);
            m_totalH -= 600;
        }
    }
}

void MailReadPopUpView::showRefuseApplyInterface()
{
    if (m_info->type == MAIL_REFUSE_ALL_APPLY) {
//        m_totalH -= 20;
        MailAllianceKickCell* cell = MailAllianceKickCell::create(m_info);
        m_ListNode->addChild(cell);
        if (CCCommonUtils::isIosAndroidPad()) {
            cell->setPosition(0, m_totalH - 1440);
            m_totalH -= 600;
        }
        else {
            cell->setPosition(0, m_totalH - 1440);
            m_totalH -= 600;
        }
    }
}

void MailReadPopUpView::showJoinAllianceBtn(){
    if(m_info->type==MAIL_ALLIANCEINVITE || m_info->type==MAIL_ALLIANCEAPPLY){
//        m_totalH-=20;
        MailAllianceInviteCell* cell = MailAllianceInviteCell::create(m_info);
        this->m_ListNode->addChild(cell);
        if (CCCommonUtils::isIosAndroidPad()) {
            cell->setPosition(0, m_totalH-1680);
            m_totalH-=1680;
        }
        else {
            cell->setPosition(0, m_totalH-700);
            m_totalH-=700;
        }
//        auto spr = CCLoadSprite::createScale9Sprite("mail_red_btn.png");
//        m_joinAllianceBtn = CCControlButton::create(spr);
//        m_ListNode->addChild(m_joinAllianceBtn);
//        m_joinAllianceBtn->setPreferredSize(CCSize(170, 64));
//        m_joinAllianceBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailReadPopUpView::onJoinAllianceBtnClick), CCControlEventTouchUpInside);
//        m_joinAllianceBtn->setPosition(ccp(m_contentContainer->getContentSize().width/2+85+12,m_totalH));
//        CCCommonUtils::setButtonTitle(m_joinAllianceBtn, _lang("103741").c_str());
//        auto spr1 = CCLoadSprite::createScale9Sprite("btn_yellow.png");
//        m_refuseAllianceBtn = CCControlButton::create(spr1);
//        m_ListNode->addChild(m_refuseAllianceBtn);
//        m_refuseAllianceBtn->setPreferredSize(CCSize(170, 64));
//        m_refuseAllianceBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailReadPopUpView::onRefuseAllianceBtnClick), CCControlEventTouchUpInside);
//        m_refuseAllianceBtn->setPosition(ccp(m_contentContainer->getContentSize().width/2-85-12,m_totalH));
//        CCCommonUtils::setButtonTitle(m_refuseAllianceBtn, _lang("103742").c_str());
    }
}
void MailReadPopUpView::showShareBtn(){
    if(m_info->isShare){
        CCNode *node = CCNode::create();
        auto spr1 = CCLoadSprite::createScale9Sprite("btn_green3.png");
        CCControlButton *shareBtn = CCControlButton::create(spr1);
        node->addChild(shareBtn);
        shareBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailReadPopUpView::onShareBtnClick), CCControlEventTouchUpInside);
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
        if (CCCommonUtils::isIosAndroidPad()) {
            node->setScale(2.4);
            m_totalH -= 144;
        }
        else {
            m_totalH -= 60;
        }
        this->m_ListNode->addChild(node);
        node->setPosition(m_contentContainer->getContentSize().width*0.5,m_totalH);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_totalH-=120;
        }
        else
            m_totalH-=50;
    }
}
void MailReadPopUpView::onShareBtnClick(CCObject *pSender, CCControlEvent event){
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
void MailReadPopUpView::onJoinAllianceBtnClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        CCCommonUtils::flyHint("", "", _lang("103742"));
        return;
    }
    AllianceAcceptInviteCommand* cmd = new AllianceAcceptInviteCommand(m_info->uid);
//    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(InvitesAlliaceCell::joinAllianceSuccess), NULL));
    cmd->sendAndRelease();
    m_info->deal = 1;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (MailController::getInstance()->getIsNewMailListEnable()) {
        ChatServiceCocos2dx::postMailDealStatus(m_info->uid);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
//simon        ChatServiceCocos2dx::postMailDealStatus(m_info->uid);
    }
#endif
    m_joinAllianceBtn->setEnabled(false);
    PopupViewController::getInstance()->goBackPopupView();
    
}
void MailReadPopUpView::onRefuseAllianceBtnClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    
    RefuseInviteCommand* cmd = new RefuseInviteCommand(m_info->uid);
 //   cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(InvitesAlliaceCell::success), NULL));
    cmd->sendAndRelease();
    m_info->deal = 1;
    m_refuseAllianceBtn->setEnabled(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (MailController::getInstance()->getIsNewMailListEnable()) {
        ChatServiceCocos2dx::postMailDealStatus(m_info->uid);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
//simon         ChatServiceCocos2dx::postMailDealStatus(m_info->uid);
    }
#endif
    PopupViewController::getInstance()->goBackPopupView();
}

bool MailReadPopUpView::checkRewardBtnVisible()
{
    bool ret = true;
    if (m_info->title.compare(_lang("101098")) == 0) {
        ret = false;
    }
    return ret;
}
//----class MailAllianceInviteCell
MailAllianceInviteCell* MailAllianceInviteCell::create(MailInfo* info){
    MailAllianceInviteCell *ret = new MailAllianceInviteCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;

}
void MailAllianceInviteCell::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
     //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1,false);

}
void MailAllianceInviteCell::onExit(){
    CCNode::onExit();
    setTouchEnabled(false);

}
bool MailAllianceInviteCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceDesText", CCLabelIF*, this->m_allianceDesText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceLeaderText", CCLabelIF*, this->m_allianceLeaderText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_alliancenameText", CCLabelIF*, this->m_alliancenameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceNumText", CCLabelIF*, this->m_allianceNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerText", CCLabelIF*, this->m_powerText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_refuseAllianceBtn", CCControlButton*, this->m_refuseAllianceBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_joinAllianceBtn", CCControlButton*, this->m_joinAllianceBtn)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sendMailBtn", CCControlButton*, this->m_sendMailBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lineFlag", CCScale9Sprite*, m_lineFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagNode", CCNode*, this->m_flagNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_langText", CCLabelIF*, this->m_langText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCSprite*, m_bg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inviteNode", CCNode*, this->m_inviteNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_applyNode", CCNode*, this->m_applyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headIcon", CCNode*, this->m_headIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_playerNameTxt", CCLabelIFTTF*, this->m_playerNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerValue", CCLabelIF*, this->m_powerValue);
    return true;
}

SEL_CCControlHandler MailAllianceInviteCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRefuseAllianceBtnClick", MailAllianceInviteCell::onRefuseAllianceBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onJoinAllianceBtnClick", MailAllianceInviteCell::onJoinAllianceBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSendMailBtnClick", MailAllianceInviteCell::onSendMailBtnClick);
    return NULL;
}
bool MailAllianceInviteCell::init(){
    if(m_mailInfo->type==MAIL_ALLIANCEAPPLY){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(307, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(307, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
        });
    }
    auto bg = CCBLoadFile("NEW_MailAllianceInvite", this, this);
    this->setContentSize(bg->getContentSize());
    setData();
    return true;
}

void MailAllianceInviteCell::setData(){

    if (m_mailInfo->type==MAIL_ALLIANCEINVITE) {
        m_allianceNumText->setFntFile("Arial_Bold_Regular.fnt");
        m_powerText->setFntFile("Arial_Bold_Regular.fnt");
        m_langText->setFntFile("Arial_Bold_Regular.fnt");
        m_alliancenameText->setFntFile("Arial_Bold_Regular.fnt");
        m_allianceLeaderText->setFntFile("Arial_Bold_Regular.fnt");
        m_allianceDesText->setFntFile("Arial_Bold_Regular.fnt");
        m_inviteNode->setVisible(true);
        m_applyNode->setVisible(false);
        string numstr = CC_ITOA(m_mailInfo->curMember);
        numstr.append("/");
        numstr+=CC_ITOA(m_mailInfo->maxMember);
        this->m_allianceNumText->setString(numstr);
        std::string alName = "";
        if(m_mailInfo->alliance!="")
            alName.append("(").append(m_mailInfo->alliance).append(")");
        alName.append(m_mailInfo->alliancename);
        this->m_alliancenameText->setString(alName);
//        this->m_lineFlag->setContentSize(CCSize(this->m_alliancenameText->getContentSize().width*this->m_alliancenameText->getOriginScaleX(), m_lineFlag->getContentSize().height));
//        this->m_lineFlag->setVisible(false);
        this->m_allianceLeaderText->setString(m_mailInfo->learderName);
        this->m_powerText->setString(CC_ITOA(m_mailInfo->fightpower));
        this->m_allianceDesText->setString(m_mailInfo->contents);
        string lang = ( strcmp(m_mailInfo->allianceLang.c_str(), "")==0?"115600":m_mailInfo->allianceLang);
        this->m_langText->setString(_lang(lang).c_str());
        CCCommonUtils::setButtonTitle(m_refuseAllianceBtn, _lang("103742").c_str());
        CCCommonUtils::setButtonTitle(m_joinAllianceBtn, _lang("103741").c_str());
        CCCommonUtils::setButtonTitle(m_sendMailBtn, _lang("105048").c_str());
        string leagueIcon = m_mailInfo->iconAlliance;
        if(leagueIcon==""){
            leagueIcon = "Allance_flay.png";
        }else{
            leagueIcon+=".png";
        }
        m_flagSpr = CCLoadSprite::createSprite(leagueIcon.c_str());
        m_flagSpr->setScale(1);
        m_flagNode->removeAllChildrenWithCleanup(true);
        this->m_flagNode->addChild(m_flagSpr);
        if(m_mailInfo->deal!=0){
            m_refuseAllianceBtn->setVisible(false);
            m_joinAllianceBtn->setVisible(false);
            m_sendMailBtn->setVisible(false);
        }
    }else{
        m_powerText->setFntFile("Arial_Bold_Regular.fnt");
        m_powerValue->setFntFile("Arial_Bold_Regular.fnt");
        m_playerNameTxt->setFntFile("Arial_Bold_Regular.fnt");
        m_allianceDesText->setFntFile("Arial_Bold_Regular.fnt");
        m_inviteNode->setVisible(false);
        m_applyNode->setVisible(true);
        m_powerTxt->setString(_lang("102163").c_str());
        m_powerValue->setString(CC_ITOA(m_mailInfo->applicantPower));
        m_playerNameTxt->setString(m_mailInfo->applicantName.c_str());
        this->m_allianceDesText->setString(m_mailInfo->contents);
        string tempIcon = m_mailInfo->applicantPic;
        if(tempIcon==""){
            tempIcon = "g044_middle.png";
        }else{
            tempIcon.append("_middle.png");
        }
        m_headIcon->removeAllChildrenWithCleanup(true);
        auto pic = CCLoadSprite::createSprite(tempIcon.c_str());
        pic->setScale(0.8);
        m_headIcon->addChild(pic);
        
        m_sendMailBtn->setVisible(false);
        CCCommonUtils::setButtonTitle(m_refuseAllianceBtn, _lang("115045").c_str());
        CCCommonUtils::setButtonTitle(m_joinAllianceBtn, _lang("115044").c_str());
        if(m_mailInfo->deal!=0 || !GlobalData::shared()->playerInfo.isInAlliance() || GlobalData::shared()->playerInfo.allianceInfo.rank<4){
            m_refuseAllianceBtn->setVisible(false);
            m_joinAllianceBtn->setVisible(false);
        }
    }

}
void MailAllianceInviteCell::onJoinAllianceBtnClick(cocos2d::CCObject *pSender, CCControlEvent event){
    m_mailInfo->deal = 1;
    m_joinAllianceBtn->setEnabled(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (MailController::getInstance()->getIsNewMailListEnable()) {
        ChatServiceCocos2dx::postMailDealStatus(m_mailInfo->uid);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
//simon         ChatServiceCocos2dx::postMailDealStatus(m_mailInfo->uid);
    }
#endif
//    if(GlobalData::shared()->serverType>=SERVER_BATTLE_FIELD){ simon
//        YesNoDialog::show(_lang("138064").c_str(), NULL);
//        return ;
//    }
//    if(CCCommonUtils::isKuaFuWangZhan()){
//        YesNoDialog::show(_lang("170048").c_str(), NULL);
//        return ;
//    }
    if(m_mailInfo->type==MAIL_ALLIANCEINVITE){
        if(GlobalData::shared()->playerInfo.isInAlliance()){
            CCCommonUtils::flyHint("", "", _lang("E100081"));
            return;
        }
        AllianceAcceptInviteCommand* cmd = new AllianceAcceptInviteCommand(m_mailInfo->uid);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(MailAllianceInviteCell::joinSuccess), NULL));
        cmd->sendAndRelease();
        GameController::getInstance()->showWaitInterface();
    }else{
        AcceptAllianceApplyCommand* cmd = new AcceptAllianceApplyCommand(m_mailInfo->applicantId,m_mailInfo->uid);
        cmd->sendAndRelease();
        PopupViewController::getInstance()->goBackPopupView();
    }
}
void MailAllianceInviteCell::onSendMailBtnClick(cocos2d::CCObject *pSender, CCControlEvent event){
//    m_mailInfo.deal = 1;
//    if(GlobalData::shared()->serverType>=SERVER_BATTLE_FIELD){ simon
//        YesNoDialog::show(_lang("138064").c_str(), NULL);
//        return ;
//    }
//    if(CCCommonUtils::isKuaFuWangZhan()){
//        YesNoDialog::show(_lang("170048").c_str(), NULL);
//        return ;
//    }
    m_sendMailBtn->setEnabled(false);
    if(m_mailInfo->type==MAIL_ALLIANCEINVITE){
        this->retain();
        PopupViewController::getInstance()->removeAllPopupView();
        MailController::getInstance()->openMailDialogViewFirst(m_mailInfo->fromName, m_mailInfo->fromUid);
        this->release();
    }
}
void MailAllianceInviteCell::joinSuccess(CCObject* p){
    string dialog = "115188";
//    ChatController::getInstance()->sendCountryChat(_lang("115188").append("  (").append(_lang("115181")).append(")").c_str(), CHAT_ALLIANCE, 2, "", dialog.c_str(), NULL); simon
    
    AutoSafeRef temp(this);
    
    if(GlobalData::shared()->playerInfo.isInAlliance()){
 
        PopupViewController::getInstance()->removeAllPopupView();
        PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
 
    }
//    if(CCCommonUtils::isFirstJoinAlliance()){  simon
//        GameController::getInstance()->callXCApi("action=joinAlliance_2");
//        CCLOGFUNC("firstJoin_style2");
//    }
}
void MailAllianceInviteCell::onRefuseAllianceBtnClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(m_mailInfo->type==MAIL_ALLIANCEINVITE){
        RefuseInviteCommand* cmd = new RefuseInviteCommand(m_mailInfo->uid);
        cmd->sendAndRelease();
        m_mailInfo->deal = 1;
        m_refuseAllianceBtn->setEnabled(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (MailController::getInstance()->getIsNewMailListEnable()) {
            ChatServiceCocos2dx::postMailDealStatus(m_mailInfo->uid);
        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
//simon             ChatServiceCocos2dx::postMailDealStatus(m_mailInfo->uid);
        }
#endif
    }else{
        RefuseAllianceApplyCommand* cmd = new RefuseAllianceApplyCommand(m_mailInfo->applicantId,m_mailInfo->uid);
        cmd->sendAndRelease();
        m_mailInfo->deal = 1;
        m_refuseAllianceBtn->setEnabled(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (MailController::getInstance()->getIsNewMailListEnable()) {
            ChatServiceCocos2dx::postMailDealStatus(m_mailInfo->uid);
        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
//simon             ChatServiceCocos2dx::postMailDealStatus(m_mailInfo->uid);
        }
#endif
    }
    PopupViewController::getInstance()->goBackPopupView();
}
bool MailAllianceInviteCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_mailInfo->type == MAIL_ALLIANCEINVITE) {
        bool alreadyInAlliance = GlobalData::shared()->playerInfo.isInAlliance();
        
        if (alreadyInAlliance)
            
        {
            return false;
        }
        
        if(isTouchInside(m_flagSpr, pTouch)){
            return true;
        }
        return false;
    } else if (m_mailInfo->type == MAIL_ALLIANCEAPPLY) {
        if (isTouchInside(m_bg, pTouch)) {
            return true;
        }
    }
    
}
void MailAllianceInviteCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

    
    return;
    
}
void MailAllianceInviteCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (m_mailInfo->type == MAIL_ALLIANCEINVITE) {
        if(isTouchInside(m_flagSpr, pTouch)){
//            bool ishasInfo = false;
//            map<string, AllianceInfo* >::iterator it;
//            for(it = AllianceManager::getInstance()->allianceList.begin();it != AllianceManager::getInstance()->allianceList.end();it++)
//            {
//                AllianceInfo* alliance = it->second;
//                if(alliance->uid==m_mailInfo.allianceId){
//                    ishasInfo = true;
//                    if (GlobalData::shared()->playerInfo.isInAlliance() && alliance->uid==GlobalData::shared()->playerInfo.allianceInfo.uid) {
//                        PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(alliance));
//                    }else{
//                        PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(alliance));
//                    }
//                    break;
//                }
//                //allianceArray->addObject(alliance);
//            }
//            //  PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
//            if(!ishasInfo){
//                PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
//            }
            GetAllianceListCommand* cmd = new GetAllianceListCommand(m_mailInfo->alliancename,1,1);
            cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(MailAllianceInviteCell::showAllianceInfo), NULL));
            cmd->sendAndRelease();
        }
    } else if (m_mailInfo->type == MAIL_ALLIANCEAPPLY) {
        if (isTouchInside(m_bg, pTouch)) {
            if (m_mailInfo->applicantId != "") {
                RoleInfoView::createInfoByUid(m_mailInfo->applicantId);
            }
        }
    }
}

void MailAllianceInviteCell::showAllianceInfo(CCObject* obj)
{
    NetResult* result = dynamic_cast<NetResult*>(obj);
    auto params = _dict(result->getData());
    CCArray* arr =  (CCArray*)params->objectForKey("list");
    int num = arr->count();
    for (int i=0; i<num; i++) {
        CCDictionary* dicAlliance = (CCDictionary*)arr->objectAtIndex(i);
        AllianceInfo* alliance = new AllianceInfo();
        alliance->updateAllianceInfo(dicAlliance);
        if(alliance->uid==m_mailInfo->allianceId){
            PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(alliance));
            alliance->release();
            break;
        }
        alliance->release();
    }
}
//------cl

//-----class MailKickOutCell
MailAllianceKickCell* MailAllianceKickCell::create(MailInfo* info)
{
    MailAllianceKickCell* ret = new MailAllianceKickCell(info);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MailAllianceKickCell::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1,false);
    
}
void MailAllianceKickCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}
bool MailAllianceKickCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceDesText", CCLabelIF*, this->m_allianceDesText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceLeaderText", CCLabelIF*, this->m_allianceLeaderText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_alliancenameText", CCLabelIF*, this->m_alliancenameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceNumText", CCLabelIF*, this->m_allianceNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerText", CCLabelIF*, this->m_powerText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lineFlag", CCScale9Sprite*, m_lineFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagNode", CCNode*, this->m_flagNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_langText", CCLabelIF*, this->m_langText);
    return true;
}

SEL_CCControlHandler MailAllianceKickCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

bool MailAllianceKickCell::init(){
    if(m_mailInfo->type==MAIL_ALLIANCEAPPLY){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(307, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(307, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
        });
    }
    auto bg = CCBLoadFile("NEW_MailAllianceKick", this, this);
    this->setContentSize(bg->getContentSize());
    setData();
    return true;
}

void MailAllianceKickCell::setData(){
    m_allianceNumText->setFntFile("Arial_Bold_Regular.fnt");
    m_powerText->setFntFile("Arial_Bold_Regular.fnt");
    m_langText->setFntFile("Arial_Bold_Regular.fnt");
    m_alliancenameText->setFntFile("Arial_Bold_Regular.fnt");
    m_allianceLeaderText->setFntFile("Arial_Bold_Regular.fnt");
    m_allianceDesText->setFntFile("Arial_Bold_Regular.fnt");
    string numstr = CC_ITOA(m_mailInfo->curMember);
    numstr.append("/");
    numstr+=CC_ITOA(m_mailInfo->maxMember);
    this->m_allianceNumText->setString(numstr);
    std::string alName = "";
    if(m_mailInfo->alliance!="")
        alName.append("(").append(m_mailInfo->alliance).append(")");
    alName.append(m_mailInfo->alliancename);
    this->m_alliancenameText->setString(alName);
//    this->m_lineFlag->setContentSize(CCSize(this->m_alliancenameText->getContentSize().width*this->m_alliancenameText->getOriginScaleX(), m_lineFlag->getContentSize().height));
//    this->m_lineFlag->setVisible(false);
    this->m_allianceLeaderText->setString(m_mailInfo->learderName);
    this->m_powerText->setString(CC_ITOA(m_mailInfo->fightpower));
    this->m_allianceDesText->setString(m_mailInfo->contents);
    if (m_mailInfo->type == MAIL_REFUSE_ALL_APPLY) {
        auto btnPic1 = CCLoadSprite::createScale9Sprite("Mail_btn03.png");
        auto btn =CCControlButton::create(btnPic1);
        btn->setAnchorPoint(ccp(0.5, 0));
        btn->setPreferredSize(CCSizeMake(250, 80));
        btn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailAllianceKickCell::onViewOtherAllBtnClick), CCControlEventTouchUpInside);
        auto btngrayPic1 = CCLoadSprite::createScale9Sprite("Mail_btn06.png");
        btn->setBackgroundSpriteForState(btngrayPic1, CCControlStateDisabled);
        CCCommonUtils::setButtonTitle(btn, _lang("115211").c_str());
        btn->setVisible(true);
        btn->setPosition(280 , 0);
        this->addChild(btn);
    }
    string lang = ( strcmp(m_mailInfo->allianceLang.c_str(), "")==0?"115600":m_mailInfo->allianceLang);
    this->m_langText->setString(_lang(lang).c_str());
    string leagueIcon = m_mailInfo->iconAlliance;
    if(leagueIcon==""){
        leagueIcon = "Allance_flay.png";
    }else{
        leagueIcon+=".png";
    }
    m_flagSpr = CCLoadSprite::createSprite(leagueIcon.c_str());
    m_flagSpr->setScale(1);
    m_flagNode->removeAllChildrenWithCleanup(true);
    this->m_flagNode->addChild(m_flagSpr);
}

void MailAllianceKickCell::onViewOtherAllBtnClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
}

bool MailAllianceKickCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(isTouchInside(m_flagSpr, pTouch)){
        return true;
    }
    return false;
}
void MailAllianceKickCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    
    return;
    
}
void MailAllianceKickCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(isTouchInside(m_flagSpr, pTouch)){
        bool ishasInfo = false;
        map<string, AllianceInfo* >::iterator it;
        for(it = AllianceManager::getInstance()->allianceList.begin();it != AllianceManager::getInstance()->allianceList.end();it++)
        {
            AllianceInfo* alliance = it->second;
            if(alliance->uid==m_mailInfo->allianceId){
                ishasInfo = true;
//                if (GlobalData::shared()->playerInfo.isInAlliance() && alliance->uid==GlobalData::shared()->playerInfo.allianceInfo.uid) {
//                    PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(alliance, true));
//                }else{
//                    AllianceInfo* pData = alliance->copySimpeAlliance();
//                    PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(pData));
//                    pData->release();
//                } simon
                break;
            }
            //allianceArray->addObject(alliance);
        }
        //  PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
        if(!ishasInfo){
            PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
        }
    }
}

//-----class MailInviteTeleCell
MailInviteTeleCell* MailInviteTeleCell::create(MailInfo* info)
{
    MailInviteTeleCell* ret = new MailInviteTeleCell(info);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailInviteTeleCell::init() {
    auto bg = CCBLoadFile("NEW_MailInviteTeleport", this, this);
    this->setContentSize(bg->getContentSize());
    m_waitInterFace1 = NULL;
    m_waitInterFace2 = NULL;
    setData();
    return true;
}

void MailInviteTeleCell::setData() {
    m_desText->setFntFile("Arial_Bold_Regular.fnt");
    std::string invitername = m_mailInfo->inviterName;
    m_desText->setString(_lang_1(m_mailInfo->message, invitername.c_str()));
//    m_btnNode->setPosition(ccp(m_btnNode->getPositionX(), -m_desText->getContentSize().height - 50));
    CCCommonUtils::setButtonTitle(m_refuseBtn, _lang("103742").c_str());
    CCCommonUtils::setButtonTitle(m_acceptBtn, _lang("115290").c_str());
    if (m_mailInfo->inviteTeleDeal == 0) {
        m_refuseBtn->setEnabled(true);
        m_acceptBtn->setEnabled(true);
    } else {
        m_refuseBtn->setEnabled(false);
        m_acceptBtn->setEnabled(false);
    }
}

void MailInviteTeleCell::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailInviteTeleCell::afterGetNearestPoint), MSG_GET_NEAREST_POINT_END, NULL);
}

void MailInviteTeleCell::onExit() {
    removeWaitInterface();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GET_NEAREST_POINT_END);
    CCNode::onExit();
}

void MailInviteTeleCell::onRefuseBtnClick(CCObject *pSender, CCControlEvent event) {
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    string mailid = m_mailInfo->uid;
    RefuseTeleportCommand* cmd = new RefuseTeleportCommand(mailid);
    cmd->sendAndRelease();
    CCCommonUtils::flyHint("", "", _lang("115292"));
    m_mailInfo->inviteTeleDeal = 1;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (MailController::getInstance()->getIsNewMailListEnable()) {
        ChatServiceCocos2dx::postMailDealStatus(m_mailInfo->uid);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
//simon         ChatServiceCocos2dx::postMailDealStatus(m_mailInfo->uid);
    }
#endif
    m_refuseBtn->setEnabled(false);
    m_acceptBtn->setEnabled(false);
    PopupViewController::getInstance()->goBackPopupView();
}

void MailInviteTeleCell::onAcceptBtnClick(CCObject *pSender, CCControlEvent event) {
    removeWaitInterface();
//    m_waitInterFace1 = GameController::getInstance()->showWaitInterface(m_refuseBtn);
//    m_waitInterFace2 = GameController::getInstance()->showWaitInterface(m_acceptBtn);
//    GetNearestPointCommand* cmd = new GetNearestPointCommand(m_mailInfo.targetPoint, m_mailInfo.uid);
//    cmd->sendAndRelease();
//    return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::stopReturnToChat();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//simon    ChatServiceCocos2dx::stopReturnToChat();
#endif
    AllianceManager::getInstance()->goToWorldType = 1;
    AllianceManager::getInstance()->tmpMailUid = m_mailInfo->uid;
    int pos = m_mailInfo->targetPoint;
    WorldController::getInstance()->openTargetIndex = pos;
    CCPoint pt = WorldController::getPointByIndex(pos);
    std::string isFirstPopKey = GlobalData::shared()->playerInfo.uid + "isFirstPop";
    int isFirstPop = CCUserDefault::sharedUserDefault()->getIntegerForKey(isFirstPopKey.c_str(),0);
    
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
    }else{
        if (isFirstPop==0) {
            WorldController::getInstance()->firstInWorld = true;
            isFirstPop = 1;
            CCUserDefault::sharedUserDefault()->setIntegerForKey(isFirstPopKey.c_str(), isFirstPop);
            CCUserDefault::sharedUserDefault()->flush();
        }

        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, pos);
    }
    //zym 2015.12.11
    PopupViewController::getInstance()->forceClearAll(true); //simon
   // PopupViewController::getInstance()->removeAllPopupView();
}

void MailInviteTeleCell::afterGetNearestPoint(CCObject* obj)
{
    CCDictionary* dic = _dict(obj);
    string uid = dic->valueForKey("mailUid")->getCString();
    if (strcmp(uid.c_str(), m_mailInfo->uid.c_str()) != 0)
    {
        return;
    }
    removeWaitInterface();
    CCDictionary* params = dynamic_cast<CCDictionary*>(dic->objectForKey("params"));
    if (params->valueForKey("errorCode")->compare("") != 0)
    {
        CCCommonUtils::flyText(_lang(params->valueForKey("errorCode")->getCString()));
        return;
    }
    int pos = 1;
    if (params->objectForKey("point"))
    {
        pos = params->valueForKey("point")->intValue();
    }
    else {
        pos = m_mailInfo->targetPoint;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::stopReturnToChat();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//simon    ChatServiceCocos2dx::stopReturnToChat();
#endif
    CCPoint pt = WorldController::getPointByIndex(pos);
    WorldController::getInstance()->openTargetIndex = pos;
    AllianceManager::getInstance()->goToWorldType = 1;
    AllianceManager::getInstance()->tmpMailUid = m_mailInfo->uid;
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
    }else{
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, pos);
    }
    PopupViewController::getInstance()->removeAllPopupView();
    //zym 2015.12.11
    PopupViewController::getInstance()->forceClearAll(true);// simon
}

void MailInviteTeleCell::removeWaitInterface()
{
    if (m_waitInterFace1 != NULL)
    {
       m_waitInterFace1->remove();
       m_waitInterFace1 = NULL;
    }
    if (m_waitInterFace2 != NULL)
    {
        m_waitInterFace2->remove();
        m_waitInterFace2 = NULL;
    }
}

bool MailInviteTeleCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_desText", CCLabelIF*, m_desText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnNode", CCNode*, m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_acceptBtn", CCControlButton*, m_acceptBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_refuseBtn", CCControlButton*, m_refuseBtn);
    return true;
}

SEL_CCControlHandler MailInviteTeleCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRefuseBtnClick", MailInviteTeleCell::onRefuseBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAcceptBtnClick", MailInviteTeleCell::onAcceptBtnClick);
    return NULL;
}

//-----
ItemAndGeneralCell *ItemAndGeneralCell::create(int type, int value, int num){
    ItemAndGeneralCell *ret = new ItemAndGeneralCell();
    if(ret && ret->init(type, value, num)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ItemAndGeneralCell::init(int type, int value, int num){
    int w = 100;
    int h = 150;
    GeneralHeadPic *head;
    std::string nameStr;
    if(type == R_GOODS){
        head = GeneralHeadPic::create(CC_ITOA(value));
//        auto titer = ToolController::getInstance()->m_toolInfos.find(value);
//        if(titer!=ToolController::getInstance()->m_toolInfos.end()){
//            nameStr = (*titer).second.getName();
//        }else{
            nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
//        }
    }else if(type == R_GENERAL){
        head = GeneralHeadPic::create(CC_ITOA(value));
        nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
    }else{
        head = GeneralHeadPic::create(CC_ITOA(value),-1,type);
        nameStr = CCCommonUtils::getResourceProductByType(type);

    }
    this->setContentSize(CCSize(w, h));
    this->addChild(head);
    head->setPosition(0, h - head->getContentSize().height / 2 - 5);
    
    auto numText = CCLabelIF::create(CC_CMDITOA(num).c_str());
    this->addChild(numText);
    numText->setFontSize(20);
    numText->setColor(ccc3(56, 8, 0));
    numText->setPosition(0, 25);
    
    auto nameText = CCLabelIF::create();
    nameText->setColor(ccc3(56, 8, 0));
    nameText->setFontSize(20);
    
    this->addChild(nameText);
    nameText->setPosition(0, numText->getPositionY() - 20);
    nameText->setString(nameStr.c_str());
    return true;
}

DetailRewardCell *DetailRewardCell::create(int type,int value,int num,int color,bool gray){
    DetailRewardCell *ret = new DetailRewardCell(type,value,num,color,gray);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
CCNode* DetailRewardCell::getPicNode(){
    return m_picNode;
}
void DetailRewardCell::setData(int type,int value,int num,int color,bool gay){

    std::string nameStr;
    CCSprite icon;
    if(type == R_GOODS){
//        auto titer = ToolController::getInstance()->m_toolInfos.find(value);
//        if(titer!=ToolController::getInstance()->m_toolInfos.end()){
//            nameStr = (*titer).second.getName();
//        }else{
            nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
//        }
    }else if(type == R_GENERAL){
        nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
    }else{
        nameStr = CCCommonUtils::getResourceNameByType(type);
        
    }
    if(type == R_GOODS || type == R_GENERAL){
//        bool dPath = (type == R_GOODS)?true:false;
        auto icon  =(type == R_GOODS)? CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(value)).c_str(),true,CCLoadSpriteType_GOODS) : CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(value)).c_str());
        m_picNode->addChild(icon);
        icon->setAnchorPoint(ccp(0.5, 0.5));
        if(gay){
            icon->setColor({90,85,81});
        }
    }else{
        auto icon1 = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(type).c_str());
        m_picNode->addChild(icon1);
        icon1->setAnchorPoint(ccp(0.5, 0.5));
        if(gay){
            icon1->setColor({90,85,81});
        }
    }

    std::string str = CC_ITOA(num);
    
    this->m_nameText->setString(nameStr);
    this->m_numText->setString(str);
    if(gay){
        m_numText->setColor({90,85,81});
        m_nameText->setColor({90,85,81});
    }
    
}

void DetailRewardCell::onEnter(){
    CCNode::onEnter();
}

void DetailRewardCell::onExit(){
    CCNode::onExit();
}

bool DetailRewardCell::init(){
    auto bg = CCBLoadFile("QuestCellInfoTaskReward", this, this);
    this->setContentSize(bg->getContentSize());
    this->setData(m_type,m_value,m_num,m_color,m_gay);
    return true;
}

bool DetailRewardCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numText", CCLabelIF*, this->m_numText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_picNode", CCNode*, this->m_picNode);
    
    return false;
}

SEL_CCControlHandler DetailRewardCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}
