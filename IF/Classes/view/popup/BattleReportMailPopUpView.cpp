//
//  BattleReportMailPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-26.
//
//

#include "BattleReportMailPopUpView.h"
#include "PopupViewController.h"
#include "RoleInfoView.h"
#include "WorldController.h"
#include "SceneController.h"
#include "MailController.h"
#include "GeneralHeadPic.h"
#include "MailPopUpView.h"
#include "WorldMapView.h"
#include "WorldController.h"
#include "FunBuildController.h"
#include "RewardController.h"
#include "BattleManager.h"
#include "MailBattleDetailView.h"
#include "YesNoDialog.h"
#include "MailSaveCommand.h"
#include "DetectMailPopUpView.h"
#include "ChatController.h"
#include "CountryChatCommand.h"
#include "ChatServiceCocos2dx.h"
#include "ActivityController.h"

BattleReportMailPopUpView::~BattleReportMailPopUpView(){
    mAtkWarEff.clear();
    mDefWarEff.clear();
}
BattleReportMailPopUpView *BattleReportMailPopUpView::create(MailInfo *info){
    BattleReportMailPopUpView *ret = new BattleReportMailPopUpView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void BattleReportMailPopUpView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleReportMailPopUpView::refreshView), MAIL_CONTENT_READ, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    if(m_info->checkMail){
        m_mailTitle->setString(_lang("115281"));
        setTitleName(_lang("115281"));
    }
}

void BattleReportMailPopUpView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_CONTENT_READ);
    PopupBaseView::onExit();
}

bool BattleReportMailPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto bg = CCBLoadFile("BattleReportMailCCB", this, this);
        this->setContentSize(bg->getContentSize());
        setTitleName(_lang("105513"));
        //CCCommonUtils::setButtonTitle(m_reportBtn, _lang("105519").c_str());
        this->m_timeText->setString(CCCommonUtils::timeStampToDate(m_info->createTime).c_str());
        CCLoadSprite::doResourceByCommonIndex(105, true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
//        CCLoadSprite::doResourceByCommonIndex(6, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(105, false);
            CCLoadSprite::doResourceByCommonIndex(500, false);
//            CCLoadSprite::doResourceByCommonIndex(6, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
        });
        if(m_info->checkMail){
            CCLoadSprite::doResourceByCommonIndex(6, true);
            CCLoadSprite::doResourceByCommonIndex(8, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(6, false);
                CCLoadSprite::doResourceByCommonIndex(8, false);
            });
        }
        m_ListNode = CCNode::create();
        int preHeight = this->m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        //changeBGHeight(m_bg);
        int dh = m_buildBG->getContentSize().height - preHeight;
        if (CCCommonUtils::isIosAndroidPad())
        {
            dh = CCDirector::sharedDirector()->getWinSize().height - 2048.0;
        }
        m_bg->setContentSize(CCSize(m_bg->getContentSize().width,m_bg->getContentSize().height+dh));
        this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
        
        m_scrollView = CCScrollView::create(m_listContainer->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setAnchorPoint(ccp(0, 1));
        m_scrollView->setTouchPriority(Touch_Popup);
        m_listContainer->addChild(m_scrollView);
        m_downNode->setPositionY(m_downNode->getPositionY() - dh);

//        m_deleteBtnTitle->setString(_lang("108523").c_str());
        this->m_totalNode->removeChild(this->m_firstNode);
        this->m_totalNode->removeChild(this->m_upNode);
        this->m_totalNode->removeChild(this->m_failNode);
        this->m_scrollView->addChild(this->m_ListNode);
        this->m_ListNode->addChild(m_upNode);
        this->m_upNode->setPositionX(0);
        m_totalH = 0;
        this->m_upNode->setPositionY(m_totalH-300);
        m_totalH-=300;
        if (CCCommonUtils::isIosAndroidPad())
        {
            this->m_upNode->setPositionY(-625);
            m_totalH = -650;
        }
        isaddHeight = false;
        m_Listheight = 305;
        if (CCCommonUtils::isIosAndroidPad()) {
            m_Listheight = 630;
        }
        if(m_info->checkMail){
            ownerUid = m_info->shareUid;
        }else{
            ownerUid = GlobalData::shared()->playerInfo.uid;
        }
        if(!m_info->isReadContent){
            getData();
        }else{
            refreshView();
            this->m_ListNode->setPositionY(m_Listheight);
            m_scrollView->setContentSize(CCSize(m_listContainer->getContentSize().width, m_Listheight));
            m_scrollView->setContentOffset(ccp(0, m_listContainer->getContentSize().height - (m_Listheight)));
        }
        ret = true;
    }
    return ret;
}

void BattleReportMailPopUpView::refreshView(CCObject* p){
    m_shareBtn->setVisible(false);
    if(m_info->save==0){
        m_unSaveBtn->setVisible(true);
        m_addSaveBtn->setVisible(false);
    }else{
        m_unSaveBtn->setVisible(false);
        m_addSaveBtn->setVisible(true);
    }
    this->m_mailTitle->setString(_lang_2("105518", CC_ITOA(m_info->warPoint.x), CC_ITOA(m_info->warPoint.y)));
    int h = 0;
    std::string attName = m_info->attUser->valueForKey("name")->getCString();
    if(m_info->attUser->objectForKey("alliance")){
        std::string attAlName = m_info->attUser->valueForKey("alliance")->getCString();
        if(!attAlName.empty()){
            attName = "(" + attAlName +")" + attName;
        }
    }
    attUid = m_info->attUser->valueForKey("uid")->getCString();
    
    std::string defName = m_info->defUser->valueForKey("name")->getCString();
    if(m_info->defUser->objectForKey("alliance")){
        std::string defAlName = m_info->defUser->valueForKey("alliance")->getCString();
        if(!defAlName.empty()){
            defName = "(" + defAlName +")" + defName;
        }
    }
    defUid = m_info->defUser->valueForKey("uid")->getCString();

    std::string npcId = m_info->defUser->valueForKey("npcId")->getCString();
    bool isBoss = npcId!="";
    bool isAtt = false;
    if(ownerUid==attUid){
        attName = _lang("102187");
        isAtt = true;
    }else if(ownerUid==defUid){
        if(m_info->battleType==3){
            defName = _lang("108678");
        }else if (m_info->battleType==6){
           
        }else{
            defName = _lang("102187");
        }
    }

    isatkHelp = false;
    if(m_info->atkHelper!=NULL){
        cocos2d::CCObject* obj;
        CCARRAY_FOREACH(m_info->atkHelper, obj){
            string helpUid = dynamic_cast<CCString*>(obj)->getCString();
            if(helpUid==ownerUid){
                isatkHelp = true;
                break;
            }
        }
    }
    if(m_info->atkGen == NULL && m_info->battleType!=6){
        isatkHelp = true;
    }
    isdefHelp = false;
    if(m_info->defHelper!=NULL){
        cocos2d::CCObject* obj;
        CCARRAY_FOREACH(m_info->defHelper, obj){
            string helpUid = dynamic_cast<CCString*>(obj)->getCString();
            if(helpUid==ownerUid){
                isdefHelp = true;
                break;
            }
        }
    }
    if(m_info->atkAlliance){
        std::string nameStr = m_info->attUser->valueForKey("name")->getCString();
        if(m_info->attUser->objectForKey("alliance")){
            std::string alNameStr = m_info->attUser->valueForKey("alliance")->getCString();
            if(!alNameStr.empty()){
                nameStr = "(" + alNameStr + ")" + nameStr;
            }
        }
        nameStr= _lang_1("105595", nameStr.c_str());

       // string leagueName = m_info->atkAlliance->valueForKey("allianceName")->getCString();
        if(nameStr!=""){
            attName =nameStr;
        }
    }
    if(m_info->defAlliance){
       // string leagueName = m_info->defAlliance->valueForKey("allianceName")->getCString();
        std::string nameStr = m_info->defUser->valueForKey("name")->getCString();
        if(m_info->defUser->objectForKey("alliance")){
            std::string alNameStr = m_info->defUser->valueForKey("alliance")->getCString();
            if(!alNameStr.empty()){
                nameStr = "(" + alNameStr + ")" + nameStr;
            }
        }
        nameStr= _lang_1("105595", nameStr.c_str());
        if(nameStr!=""){
            defName =nameStr;
        }
    }
    bool isMonster = false;
    if(m_info->attUser!=NULL && m_info->attUser->objectForKey("monster")){
        isMonster = m_info->attUser->valueForKey("monster")->boolValue();
        if(isMonster){
            string mid = m_info->attUser->valueForKey("uid")->getCString();
            attName = CCCommonUtils::getNameById(mid);
        }
    }
    if(npcId!=""){
        defName = CCCommonUtils::getNameById(npcId);
        m_attackText->setString(_lang_2("105547", attName.c_str(), defName.c_str()));
    }else{
        m_attackText->setString(_lang_2("105547", attName.c_str(), defName.c_str()));
    }
    //this->m_attackText->setString((attName + " " + _lang("105123") + " " + defName).c_str());
    
    int reportState = getReportResult();
    auto reportBg = CCLoadSprite::createSprite("battle_result_bg.png");
    reportBg->setAnchorPoint(ccp(0, 0.5));
    reportBg->setPositionX(-5);
    bool isbigLoss = false;
    if(m_info->atkGen == NULL && m_info->battleType!=6){
        this->m_hintText->setString(_lang("105535").c_str());
        isbigLoss = true;
        autoResizeHintNode();
    }else if(m_info->isResourceShieldState){
        this->m_hintText->setString(_lang("103765").c_str());
        autoResizeHintNode();
    }else{
        this->m_failNode->setVisible(false);
    }
    switch (reportState) {
        case WIN:{
            setTitleName(_lang("105117"));
            break;
        }
        case LOOSE:{
            setTitleName(_lang("105118"));
            break;
        }
        default:{
            setTitleName(_lang("105549"));
            break;
        }
    }
    if (m_info->msReport==1) {
        m_attackText->setString("");
        this->m_mailTitle->setString("");
        this->setTitleName(_lang("133053"));
    }
    if (m_info->ckfWarInfo!=NULL) {
        handleLianShengInfo(0);
    }
    if(reportState==WIN){
        if(m_info->battleType==2){//caiji
            auto battlePic = CCLoadSprite::createSprite("Mail_caiji.png");
            this->m_battlePicNode->addChild(battlePic);
            if(isAtt||isatkHelp){
                this->m_upTitle->setString(_lang("105117").c_str());
            }else{
                this->m_upTitle->setString(_lang("105117").c_str());
            }
        }else if(m_info->battleType==3){
            std::string picUrl = "";
            std::string name = "";
            if(isAtt||isatkHelp){
                switch (m_info->pointType) {
                    case Throne:{
                        name = _lang("105700");
                        picUrl = "Mail_darksoil.png";
                        break;
                    }
                    case Trebuchet:{
                        name = _lang("105704");
                        picUrl = "Mail_darksoil.png";
                        break;
                    }
                    default:{
                        name = _lang("105578");
                        picUrl = "Mail_gongcheng_succeed.png";
                        break;
                    }
                }
            }else{
                name = "";
                switch (m_info->pointType) {
                    case Throne:{
                        name = _lang("105702");
                        picUrl = "Mail_darksoil.png";
                        break;
                    }
                    case Trebuchet:{
                        name = _lang("105706");
                        picUrl = "Mail_darksoil.png";
                        break;
                    }
                    default:{
                        name = _lang("105579");
                        picUrl = "Mail_gongcheng_succeed.png";
                        break;
                    }
                }
            }
            this->m_upTitle->setString(name.c_str());
            auto battlePic = CCLoadSprite::createSprite(picUrl.c_str());
            this->m_battlePicNode->addChild(battlePic);
        }else if(m_info->battleType==4){//zhaying
            auto battlePic = CCLoadSprite::createSprite("Mail_zhencha_succeed.png");
            this->m_battlePicNode->addChild(battlePic);
            if(isAtt||isatkHelp){
                this->m_upTitle->setString(_lang("105117").c_str());
            }else{
                this->m_upTitle->setString(_lang("105117").c_str());
            }
        }else if(m_info->battleType==6){
            auto battlePic = CCLoadSprite::createSprite("Mail_monster.png");
            this->m_battlePicNode->addChild(battlePic);
            this->m_upTitle->setString(_lang("133053"));
            handleMonsterBattleInfo(0);
        }else if(m_info->battleType==7){
            auto battlePic = CCLoadSprite::createSprite("Mail_gongcheng_succeed.png");
            this->m_battlePicNode->addChild(battlePic);
            std::string titleStr = "";
            if (isAtt||isatkHelp) {
                if (m_info->pointType == Tile_allianceArea) {
                    titleStr = _lang("115337");
                }
                else if (m_info->pointType == tile_banner) {
                    titleStr = _lang("115540");
                }
            }
            else {
                if (m_info->pointType == Tile_allianceArea) {
                    titleStr = _lang("115339");
                }
                else if (m_info->pointType == tile_banner) {
                    titleStr = _lang("115542");
                }
            }
            this->m_upTitle->setString(titleStr.c_str());
        }else if(m_info->msReport==1){
            auto battlePic = CCLoadSprite::createSprite("Mail_monster.png");
            this->m_battlePicNode->addChild(battlePic);
            this->m_upTitle->setString(_lang("133083"));
            handleMonsterBattleInfo(0);
        }else{
            auto battlePic = CCLoadSprite::createSprite("Mail_gongcheng_succeed.png");
            this->m_battlePicNode->addChild(battlePic);
            this->m_upTitle->setString(_lang("105117").c_str());
        }
    }else{

        if(m_info->battleType==2){
            auto battlePic = CCLoadSprite::createSprite("Mail_gongcheng_Failure.png");
            this->m_battlePicNode->addChild(battlePic);
            if(isAtt||isatkHelp){
                this->m_upTitle->setString(_lang("105118").c_str());
            }else{
                this->m_upTitle->setString(_lang("105118").c_str());
            }
        }else if(m_info->battleType==3){
            this->m_battlePicNode->removeAllChildren();
            std::string picUrl = "";
            std::string name = "";
            if(isAtt||isatkHelp){
                if(isbigLoss){
                    switch (m_info->pointType) {
                        case Throne:{
                            name = _lang("105701");
                            picUrl = "Mail_darksoil.png";
                            break;
                        }
                        case Trebuchet:{
                            name = _lang("105705");
                            picUrl = "Mail_darksoil.png";
                            break;
                        }
                        default:{
                            name =_lang("105583");
                            picUrl = "Mail_gongcheng_Failure.png";
                            break;
                        }
                    }
                }
                else{
                    switch (m_info->pointType) {
                        case Throne:{
                            name = _lang("105701");
                            picUrl = "Mail_darksoil.png";
                            break;
                        }
                        case Trebuchet:{
                            name = _lang("105705");
                            picUrl = "Mail_darksoil.png";
                            break;
                        }
                        default:{
                            name = _lang("105582");
                            picUrl = "Mail_gongcheng_Failure.png";
                            break;
                        }
                    }
                }
            }else{
                if(isbigLoss){
                    switch (m_info->pointType) {//守城大败
                        case Throne:{
                            name = _lang("105703");
                            picUrl = "Mail_darksoil.png";
                            break;
                        }
                        case Trebuchet:{
                            name = _lang("105707");
                            picUrl = "Mail_darksoil.png";
                            break;
                        }
                        default:{
                            name =_lang("105581");
                            picUrl = "Mail_gongcheng_Failure.png";
                            break;
                        }
                    }
                }
                else{
                    switch (m_info->pointType) { // 守城失败
                        case Throne:{
                            name = _lang("105703");
                            picUrl = "Mail_darksoil.png";
                            break;
                        }
                        case Trebuchet:{
                            name = _lang("105707");
                            picUrl = "Mail_darksoil.png";
                            break;
                        }
                        default:{
                            name = _lang("105580");
                            picUrl = "Mail_gongcheng_Failure.png";
                            break;
                        }
                    }
                }
            }
            auto battlePic = CCLoadSprite::createSprite(picUrl.c_str());
            this->m_battlePicNode->addChild(battlePic);
            this->m_upTitle->setString(name.c_str());
        }else if(m_info->battleType==4){
            auto battlePic = CCLoadSprite::createSprite("Mail_gongcheng_Failure.png");
            this->m_battlePicNode->addChild(battlePic);
            if(isAtt||isatkHelp){
                this->m_upTitle->setString(_lang("105118").c_str());
            }else{
                this->m_upTitle->setString(_lang("105118").c_str());
            }
        }else if(m_info->battleType==6){
            auto battlePic = CCLoadSprite::createSprite("Mail_monster.png");
            this->m_battlePicNode->addChild(battlePic);
            this->m_upTitle->setString(_lang("133053"));
            handleMonsterBattleInfo(0);
        }else if(m_info->battleType==7){
            auto battlePic = CCLoadSprite::createSprite("Mail_gongcheng_Failure.png");
            this->m_battlePicNode->addChild(battlePic);
            std::string titleStr = "";
            if (isAtt||isatkHelp) {
                if (m_info->pointType == Tile_allianceArea) {
                    titleStr = _lang("115338");
                }
                else if (m_info->pointType == tile_banner) {
                    titleStr = _lang("115541");
                }
            }
            else {
                if (m_info->pointType == Tile_allianceArea) {
                    titleStr = _lang("115340");
                }
                else if (m_info->pointType == tile_banner) {
                    titleStr = _lang("115543");
                }
            }
            this->m_upTitle->setString(titleStr.c_str());
        }else if(m_info->msReport==1){
            auto battlePic = CCLoadSprite::createSprite("Mail_monster.png");
            this->m_battlePicNode->addChild(battlePic);
            this->m_upTitle->setString(_lang("133083"));
            handleMonsterBattleInfo(0);
        }else{
            auto battlePic = CCLoadSprite::createSprite("Mail_gongcheng_Failure.png");
            this->m_battlePicNode->addChild(battlePic);
            this->m_upTitle->setString(_lang("105118").c_str());
        }
    }
    if(m_info->battleType==6){//怪物攻城
        addMonsterAndUserInfo(0, h, m_info->defGen, m_info->defReport,isBoss);
    }else if(m_info->msReport!=1){
        if(isbigLoss){
            DetectBgCell* bgCell = DetectBgCell::create("");
            int cellH = m_hintBG->getContentSize().height + 60;
            if (CCCommonUtils::isIosAndroidPad())
            {
                cellH = m_hintBG->getContentSize().height * 2 + 120;
            }
            bgCell->setBgHeight(cellH);
            m_totalH -= cellH;
            bgCell->setPosition(0, m_totalH-10);
            if (CCCommonUtils::isIosAndroidPad())
            {
                bgCell->setPositionX(30);
            }
            this->m_ListNode->addChild(bgCell);
            bgCell->addChild(m_failNode);
            bgCell->m_kuangbg->setVisible(false);
            m_failNode->setPosition(310, cellH*0.5);
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_failNode->setPosition(710, m_hintBG->getContentSize().height + 18);
            }
            m_Listheight += cellH-30;
        }else{
            if(m_info->isResourceShieldState){
                DetectBgCell* bgCell = DetectBgCell::create("");
                int cellH = m_hintBG->getContentSize().height + 60;
                if (CCCommonUtils::isIosAndroidPad()) {
                    cellH = m_hintBG->getContentSize().height * m_hintBG->getScaleY() + 120;
                }
                bgCell->setBgHeight(cellH);
                m_totalH -= cellH;
                bgCell->setPosition(0, m_totalH-10);
                this->m_ListNode->addChild(bgCell);
                bgCell->addChild(m_failNode);
                bgCell->m_kuangbg->setVisible(false);
                if (CCCommonUtils::isIosAndroidPad())
                {
                    m_failNode->setPosition(710, cellH*0.5);
                    m_Listheight+=cellH;
                    m_totalH -= 50;
                } else {
                    m_failNode->setPosition(310, cellH*0.5);
                    m_Listheight+=cellH;
                    m_totalH -= 25;
                }
            }else{
                if (m_info->battleType!=7) {/////////
                    h = addRewards(0);
                }
            }
            generateWarEff();
            int h1 = 0;
            int h2 = 0;
            if(isatkHelp||attUid == ownerUid){
                h1 = addUserInfo(0, h, m_info->atkGen, m_info->attReport);
                h2 = addUserInfo(this->m_listContainer->getContentSize().width / 2, h, m_info->defGen, m_info->defReport,isBoss);
                int height1 = addUserEffect(0, 0, mAtkWarEff);
                int height2 = addUserEffect(this->m_listContainer->getContentSize().width / 2,0,mDefWarEff);
                int addH = MAX(height1, height2);
                if(addH>0){
                    m_Listheight += addH + 15;
                    m_totalH -= (addH + 30);
                }
            }else{
                h1 = addUserInfo(0, h, m_info->defGen, m_info->defReport,isBoss);
                h2 = addUserInfo(this->m_listContainer->getContentSize().width / 2, h, m_info->atkGen, m_info->attReport);
                int height1 =  addUserEffect(0, 0, mDefWarEff);
                int height2 = addUserEffect(this->m_listContainer->getContentSize().width / 2,0,mAtkWarEff);
                int addH = MAX(height1, height2);
                if(addH>0){
                    m_Listheight += addH + 15;
                    m_totalH -= (addH + 30);
                }
            }
            //vip 描述
            
//            CCLabelIF *tipLabel = CCLabelIF::create("", 20);
//            tipLabel->setDimensions(CCSize(550, 0));
//            tipLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
//            tipLabel->setColor(ccWHITE);
//            tipLabel->setString(_lang("103047"));
//            this->m_ListNode->addChild(tipLabel);
//            float tmpH = tipLabel->getContentSize().height*tipLabel->getOriginScaleY();
//            tipLabel->setPosition(320, m_totalH - tmpH * 0.5);
//            
//            m_totalH -= tmpH;
//            m_Listheight += tmpH;
            
            
            auto btnPic = CCLoadSprite::createScale9Sprite("Mail_btn04.png");
            m_detailBtn =CCControlButton::create(btnPic);
            m_detailBtn->setPreferredSize(CCSizeMake(300, 64));
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_detailBtn->setPreferredSize(CCSize(600, 128));
                m_detailBtn->setTitleTTFSizeForState(44, CCControlStateNormal);
                m_detailBtn->setTitleTTFSizeForState(44, CCControlStateHighlighted);
                m_detailBtn->setTitleTTFSizeForState(44, CCControlStateDisabled);
            }
            m_detailBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(BattleReportMailPopUpView::onBattleDetailClick), CCControlEventTouchUpInside);
            this->m_ListNode->addChild(m_detailBtn);
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_totalH -= 30;
                //m_detailBtn->setPosition(160, m_totalH-32);
                m_detailBtn->setPosition(740, m_totalH-64);
                m_totalH -= 128;
                m_Listheight+=190;
            } else {
                m_totalH -= 30;
                //m_detailBtn->setPosition(160, m_totalH-32);
                m_detailBtn->setPosition(320, m_totalH-32);
                m_totalH -= 64;
                m_Listheight+=74;
            }
            CCCommonUtils::setButtonTitle(m_detailBtn, _lang("114114").c_str());
            
            //        double gapTime = GlobalData::shared()->getWorldTime() - m_info->shareTime;
            //        string strIcon = "Mail_btn04.png";
            //        if(gapTime<10*60){
            //            strIcon = "Mail_btn06.png";
            //        }
            //        auto sharePic = CCLoadSprite::createScale9Sprite(strIcon.c_str());
            //        m_shareBtn =CCControlButton::create(sharePic);
            //        m_shareBtn->setPreferredSize(CCSizeMake(250, 64));
            //        //m_shareBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Mail_btn06.png"), CCControlStateDisabled);
            //        m_shareBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(BattleReportMailPopUpView::onShareClick), CCControlEventTouchUpInside);
            //        this->m_ListNode->addChild(m_shareBtn);
            //        m_shareBtn->setPosition(470, m_detailBtn->getPositionY());
            //        CCCommonUtils::setButtonTitle(m_shareBtn, _lang("115274").c_str());
            if(m_info->battleType==6){
                m_detailBtn->setVisible(false);
            }
        }
    }
    
    if(m_info->checkMail){
        m_addSaveBtn->setVisible(false);
        m_unSaveBtn->setVisible(false);
        m_deleteBtn->setVisible(false);
        m_mailTitle->setString(_lang("115281"));
        setTitleName(_lang("115281"));
    }
    if(GlobalData::shared()->playerInfo.isInAlliance() && !m_info->checkMail && m_info->battleType!=6 && m_info->msReport!=1){
        m_shareBtn->setVisible(true);
    }
}

int BattleReportMailPopUpView::addMonsterAndUserInfo(int startX, int startY, CCArray *arr, CCArray *arr1,bool boss){
    MonsterInfoCell* cell1 = MonsterInfoCell::create(m_info->attUser,m_info->atkGen,m_info);
    this->m_ListNode->addChild(cell1);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_totalH -= 675;
    } else {
        m_totalH -= 300;
    }
    cell1->setPosition(0, m_totalH);
    if (CCCommonUtils::isIosAndroidPad())
    {
        cell1->setPositionX(30);
    }
    if (m_info->failTimes>0) {
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_Listheight += 580;
            m_totalH -= 580;
        } else {
            m_Listheight+=275;
            m_totalH -= 275;
        }
    }else{
        if (CCCommonUtils::isIosAndroidPad()) {
            m_Listheight+=560;
            m_totalH -= 560;
        } else {
            m_Listheight+=265;
            m_totalH -= 265;
        }
    }
    cell1 = MonsterInfoCell::create(m_info->defUser,m_info->defGen,m_info);
    this->m_ListNode->addChild(cell1);
    cell1->setPosition(0, m_totalH);
    if (CCCommonUtils::isIosAndroidPad()) {
        cell1->setPositionX(30);
    }
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_Listheight += 398;
        m_totalH -= 398;
    } else {
        m_Listheight+=184;
        m_totalH -= 184;
    }
    
    return startY;
}

int BattleReportMailPopUpView::getReportResult(){
    if(m_info->winner == ""){
        return DRAW;
    }
    if(m_info->winner == ownerUid){
        return WIN;
    }
    isatkHelp = false;
    if(m_info->atkHelper!=NULL){
        cocos2d::CCObject* obj;
        CCARRAY_FOREACH(m_info->atkHelper, obj){
            string helpUid = dynamic_cast<CCString*>(obj)->getCString();
            if(helpUid==ownerUid){
                isatkHelp = true;
                break;
            }
        }
    }
    attUid = m_info->attUser->valueForKey("uid")->getCString();
    defUid = m_info->defUser->valueForKey("uid")->getCString();
    if(attUid==ownerUid){
        isatkHelp = true;
    }
    if(m_info->atkGen == NULL && m_info->battleType!=6){
        isatkHelp = true;
    }
    if(isatkHelp){
        if(attUid==m_info->winner)
            return WIN;
        else
            return LOOSE;
        
    }else{
        if(defUid==m_info->winner)
            return WIN;
        else
            return LOOSE;
    }
    return LOOSE;
}

int BattleReportMailPopUpView::addRewards(int offY){
    if(m_info->reward->count()<=0){
        return 0;
    }
    float startY = offY;
    CCArray *arr = m_info->reward;
    int rowNum = arr->count();
    DetectBgCell* bgCell = DetectBgCell::create(_lang("104904").c_str());
    float preh =bgCell->getBgCellHeight();
    if(rowNum>2){
        int bgHeight = (rowNum-2)*40+bgCell->getBgCellHeight();
        if (CCCommonUtils::isIosAndroidPad())
        {
            bgHeight = (rowNum - 2) * 80 + bgCell->getBgCellHeight();
        }
        bgCell->setBgHeight(bgHeight);
    }
    m_totalH -= bgCell->getBgCellHeight() + 10;
    if (CCCommonUtils::isIosAndroidPad())
    {
//        m_totalH -= 10;
    }
    bgCell->setPosition(0, m_totalH);
    if (CCCommonUtils::isIosAndroidPad())
    {
        bgCell->setPositionX(30);
    }
    this->m_ListNode->addChild(bgCell);
    m_Listheight+=40;
    m_Listheight+=bgCell->getBgCellHeight();
    int i = 0;

    std::string defUid = m_info->defUser->valueForKey("uid")->getCString();
    while(i < m_info->reward->count()){
        CCDictionary *dict = dynamic_cast<CCDictionary*>(m_info->reward->objectAtIndex(i));
        int type = 0;
        int value = 0;
        int itemId = 0;
        if(offY<0){
            type = dict->valueForKey("t")->intValue();
            value = dict->valueForKey("v")->intValue();
            itemId = dict->valueForKey("id")->intValue();
        }else{
            type = dict->valueForKey("t")->intValue();
            value = dict->valueForKey("v")->intValue();
            itemId = dict->valueForKey("v")->intValue();
        }
        if(defUid == ownerUid){
        
            //value= -value;
        }
        auto cell = DetectRewardPreviewCell::create(type, value);
        cell->setAnchorPoint(ccp(0, 0));
        int posX = 60;
        cell->setPosition(ccp(posX,  40+i*40));
        if (CCCommonUtils::isIosAndroidPad()) {
//            posX = 120;
            cell->setPosition(ccp(posX, 65 + i * 80));
        }

        i++;
        if(rowNum==1){
            cell->setPosition(ccp(posX,  40+20));
            if (CCCommonUtils::isIosAndroidPad()) {
                cell->setPosition(ccp(posX, 65 + 40));
            }
        }
        bgCell->m_bg->addChild(cell);
        
    }
    m_totalH-=15;
    if (CCCommonUtils::isIosAndroidPad()) {
        m_totalH -= 15;
    }

    return startY;
}

int BattleReportMailPopUpView::handleMonsterBattleInfo(int offY){
    float startY = offY;
    int rowNum = 4;
    int result = getReportResult();
    DetectBgCell* bgCell = DetectBgCell::create(_lang("133094").c_str());
    if(rowNum>2){
        int bgHeight = (rowNum-2)*40+bgCell->getBgCellHeight();
        if (CCCommonUtils::isIosAndroidPad())
        {
            bgHeight = (rowNum - 2) * 80 + bgCell->getBgCellHeight();
        }
        bgCell->setBgHeight(bgHeight);
    }
    m_totalH -= bgCell->getBgCellHeight() + 10;
    bgCell->setPosition(0, m_totalH);
    if (CCCommonUtils::isIosAndroidPad()) {
        bgCell->setPositionX(30);
    }
    this->m_ListNode->addChild(bgCell);
    m_Listheight+=40;
    m_Listheight+=bgCell->getBgCellHeight();

    CCLabelIF* txt = NULL;
    string temp = "";

    
    if (m_info->msReport==1) {
        txt = CCLabelIF::create("", 20);
        txt->setAnchorPoint(ccp(0, 0.5));
        txt->setColor({130,99,56});
        txt->setPosition(ccp(30, 145));
        txt->setDimensions(CCSize(530,0));
        txt->setString(_lang("133081"));
        bgCell->m_bg->addChild(txt);
        if (CCCommonUtils::isIosAndroidPad()) {
            txt->setFontSize(40);
            txt->setDimensions(CCSize(1300, 0));
            txt->setPosition(30, 290);
        }
        
        int hTxt = 147;
        if (CCCommonUtils::isIosAndroidPad()) {
            hTxt = 294;
        }
        hTxt -= txt->getContentSize().height*txt->getOriginScaleY();
        if (CCCommonUtils::isIosAndroidPad()) {
            if(txt->getContentSize().height*txt->getOriginScaleY()<70){
                hTxt -= 10;
            }
        } else {
            if(txt->getContentSize().height*txt->getOriginScaleY()<35){
                hTxt -= 5;
            }
        }
        
        txt = CCLabelIF::create("", 20);
        txt->setAnchorPoint(ccp(0, 0.5));
        txt->setColor({130,99,56});
        txt->setPosition(ccp(30, hTxt));
        temp = _lang("133084");
        txt->setString(temp);
        bgCell->m_bg->addChild(txt);
        if (CCCommonUtils::isIosAndroidPad()) {
            txt->setFontSize(40);
            txt->setPositionX(30);
        }
        
        CCLabelIF* boTxt = CCLabelIF::create("", 20);
        boTxt->setAnchorPoint(ccp(0, 0.5));
        boTxt->setColor({125,35,29});
        boTxt->setPosition(ccp(30+txt->getContentSize().width*txt->getOriginScaleX()+10, hTxt));
        boTxt->setString(CC_ITOA(m_info->killRound));
        bgCell->m_bg->addChild(boTxt);
        if (CCCommonUtils::isIosAndroidPad()) {
            boTxt->setFontSize(40);
            boTxt->setPositionX(boTxt->getPositionX());
        }
        
        hTxt -= txt->getContentSize().height*txt->getOriginScaleY();
        hTxt -= 5;
        if (CCCommonUtils::isIosAndroidPad()) {
            hTxt -= 10;
        }
        txt = CCLabelIF::create("", 20);
        txt->setAnchorPoint(ccp(0, 0.5));
        txt->setColor({130,99,56});
        txt->setPosition(ccp(30, hTxt));
        temp = _lang("133095");
        txt->setString(temp);
        bgCell->m_bg->addChild(txt);
        if (CCCommonUtils::isIosAndroidPad()) {
            txt->setFontSize(40);
            txt->setPositionX(30);
        }
        
        CCLabelIF* killTxt = CCLabelIF::create("", 20);
        killTxt->setAnchorPoint(ccp(0, 0.5));
        killTxt->setColor({125,35,29});
        killTxt->setPosition(ccp(30+txt->getContentSize().width*txt->getOriginScaleX()+10, hTxt));
        killTxt->setString(CC_ITOA(m_info->userScore));
        bgCell->m_bg->addChild(killTxt);
        if (CCCommonUtils::isIosAndroidPad()) {
            killTxt->setFontSize(40);
            killTxt->setPositionX(killTxt->getPositionX());
        }
        
        hTxt -= txt->getContentSize().height*txt->getOriginScaleY();
        hTxt -= 5;
        if (CCCommonUtils::isIosAndroidPad()) {
            hTxt -= 5;
        }
        txt = CCLabelIF::create("", 20);
        txt->setAnchorPoint(ccp(0, 0.5));
        txt->setColor({130,99,56});
        txt->setPosition(ccp(30, hTxt));
        txt->setString(_lang("133056"));
        bgCell->m_bg->addChild(txt);
        if (CCCommonUtils::isIosAndroidPad()) {
            txt->setFontSize(40);
            txt->setDimensions(CCSize(1350, 0));
        }
    }else{
        txt = CCLabelIF::create("", 20);
        txt->setAnchorPoint(ccp(0, 0.5));
        txt->setColor({130,99,56});
        txt->setPosition(ccp(30, 130));
        temp = _lang("133095");
        txt->setString(temp);
        bgCell->m_bg->addChild(txt);
        if (CCCommonUtils::isIosAndroidPad()) {
            txt->setFontSize(40);
            txt->setPositionX(30);
            txt->setPositionY(260);
        }
        
        CCLabelIF* killTxt = CCLabelIF::create("", 20);
        killTxt->setAnchorPoint(ccp(0, 0.5));
        killTxt->setColor({125,35,29});
        killTxt->setPosition(ccp(30+txt->getContentSize().width*txt->getOriginScaleX()+10, 130));
        killTxt->setString(CC_ITOA(m_info->userScore));
        bgCell->m_bg->addChild(killTxt);
        if (CCCommonUtils::isIosAndroidPad()) {
            killTxt->setFontSize(40);
            killTxt->setPosition(ccp(30+txt->getContentSize().width*txt->getOriginScaleX()+10, 260));
        }
        
        txt = CCLabelIF::create("", 20);
        txt->setAnchorPoint(ccp(0, 0.5));
        txt->setColor({130,99,56});
        txt->setPosition(ccp(30, 100));
        txt->setString(_lang("133056"));
        bgCell->m_bg->addChild(txt);
        if (CCCommonUtils::isIosAndroidPad()) {
            txt->setFontSize(40);
            txt->setPosition(30, 200);
            txt->setAnchorPoint(ccp(0, 1));
            txt->setDimensions(CCSize(1300, 0));
        }
    }
    
    return startY;
}

int BattleReportMailPopUpView::handleLianShengInfo(int offY){
    float startY = offY;
    int reportState = getReportResult();
    int rowNum = 4;
    int result = getReportResult();
    DetectBgCell* bgCell = DetectBgCell::create(_lang("138116").c_str());
    if(rowNum>2){
        int bgHeight = (rowNum-2)*40+bgCell->getBgCellHeight();
        if (CCCommonUtils::isIosAndroidPad())
        {
            bgHeight = (rowNum - 2) * 73 + bgCell->getBgCellHeight();
        }
        bgCell->setBgHeight(bgHeight);
    }
    m_totalH -= bgCell->getBgCellHeight() + 10;
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_totalH -= 10;
    }
    bgCell->setPosition(0, m_totalH);
    this->m_ListNode->addChild(bgCell);
    m_Listheight+=40;
    m_Listheight+=bgCell->getBgCellHeight();
    
    CCLabelIF* txt = NULL;
    string temp = "";
    
    txt = CCLabelIF::create("", 20);
    txt->setAnchorPoint(ccp(0, 0.5));
    txt->setColor({130,99,56});
    txt->setPosition(ccp(30, 145));
    txt->setDimensions(CCSize(530,0));
    if (CCCommonUtils::isIosAndroidPad())
    {
        txt->setFontSize(44);
        txt->setPosition(60, 290);
        txt->setDimensions(CCSize(1360, 0));
    }
    temp = _lang_1("138117",m_info->ckfWarInfo->valueForKey("shengdian")->getCString());
    txt->setString(temp.c_str());
    bgCell->m_bg->addChild(txt);
    txt->setVisible(ActivityController::getInstance()->checkShengDianOpen());
    
    int hTxt = 147;
    hTxt -= txt->getContentSize().height*txt->getOriginScaleY();
    hTxt -= 5;
    if (CCCommonUtils::isIosAndroidPad()) {
        hTxt = 292 - txt->getContentSize().height * txt->getOriginScaleY() - 10;
    }
    txt = CCLabelIF::create("", 20);
    txt->setAnchorPoint(ccp(0, 0.5));
    txt->setColor({130,99,56});
    txt->setPosition(ccp(30, hTxt));
    
    if (CCCommonUtils::isIosAndroidPad()) {
        txt->setFontSize(44);
        txt->setPosition(ccp(60, hTxt));
    }
    
    temp = _lang_1("138118",m_info->ckfWarInfo->valueForKey("score")->getCString());
    txt->setString(temp);
    bgCell->m_bg->addChild(txt);
    
    hTxt -= txt->getContentSize().height*txt->getOriginScaleY();
    hTxt -= 5;
    if (CCCommonUtils::isIosAndroidPad()) {
        hTxt -= 5;
    }
    txt = CCLabelIF::create("", 20);
    txt->setAnchorPoint(ccp(0, 0.5));
    txt->setColor({130,99,56});
    txt->setPosition(ccp(30, hTxt));
    if (CCCommonUtils::isIosAndroidPad()) {
        txt->setPosition(ccp(60, hTxt));
    }
    bgCell->m_bg->addChild(txt);
    temp = "";
    if(reportState==WIN){
        if (defUid==m_info->winner) {
            temp = _lang("138154");
        }else{
            temp = _lang_2("138119",m_info->ckfWarInfo->valueForKey("winNum")->getCString(),m_info->ckfWarInfo->valueForKey("addScore")->getCString());
        }

    }else{
        int winNum = m_info->ckfWarInfo->valueForKey("winNum")->intValue();
        if(winNum>0){
            temp = _lang_1("138120",m_info->ckfWarInfo->valueForKey("winNum")->getCString());
        }
    }
    txt->setString(temp);
    return startY;
}

int BattleReportMailPopUpView::addUserInfo(int startX, int startY, CCArray *arr, CCArray *arr1,bool boss){
    PlayerInfoCell* playerCell = NULL;
    if(arr == m_info->atkGen){
        playerCell = PlayerInfoCell::create(m_info->attUser,m_info->atkGen,m_info);
    }else{
        playerCell = PlayerInfoCell::create(m_info->defUser,m_info->defGen,m_info);
    }
    this->m_ListNode->addChild(playerCell);
    int height =playerCell->getContentSize().height;
    if(!isaddHeight){
        isaddHeight = true;
        m_Listheight+=height+15;
        m_totalH -=(height -15);
    }
    playerCell->setPosition(startX + 8, m_totalH);
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        playerCell->setPositionX(playerCell->getPositionX() + 20);
    }

    return startY;
}
int BattleReportMailPopUpView::addUserEffect(int startX, int startY, std::vector<BattleReportWarEffect> &effArr){
    if(effArr.size()>0){
        PlayerEffectCell *pEffCell = PlayerEffectCell::create(effArr);
        this->m_ListNode->addChild(pEffCell);
        int height = pEffCell->getContentSize().height;
        pEffCell->setPosition(startX,m_totalH - (height + 20));
        if (CCCommonUtils::isIosAndroidPad()) {
            pEffCell->setPositionX(pEffCell->getPositionX() + 28);
        }
        return height;
    }
    return 0;
}
void BattleReportMailPopUpView::initBattleReportWarEffect(std::map<int,int> &effMap,bool attack){
    auto iter = GeneralManager::getInstance()->GeneralAttMap.begin();
    while (iter!=GeneralManager::getInstance()->GeneralAttMap.end()){
        if((*iter).second.bMail==false){
            ++iter;
            continue;
        }
        if(attack==true){
            if((*iter).second.bAttack==false){
                ++iter;
                continue;
            }
        }else{
            if((*iter).second.bDefence==false){
                ++iter;
                continue;
            }
        }
        vector<string> mVector;
        string effectStr = (*iter).second.effect;
        CCCommonUtils::splitString(effectStr, "|", mVector);
        int value = 0;
        vector<string>::iterator iter1 = mVector.begin();
        while (iter1!=mVector.end()) {
            auto pos1 = (*iter1).find(";");
            string effIDStr = "";
            if(pos1 != string::npos){
                effIDStr = (*iter1).substr(0,pos1);
            }else{
                effIDStr =(*iter1);
            }
            size_t iPos = effIDStr.find_first_not_of("1234567890");
            if(iPos==string::npos){
                int effID = atoi(effIDStr.c_str());
                auto iter2 = effMap.find(effID);
                if(iter2!=effMap.end()){
                    value += effMap[effID];
//                    effMap.erase(iter2);
                }
            }else{
                CCLOG("effect is string, not integer!");
            }
            ++iter1;
        }
        if(value>0){
            BattleReportWarEffect eff;
            eff.effectName = _lang((*iter).second.name);
            if(!eff.effectName.empty()){
                eff.effectOrder = (*iter).second.mail_order;
                eff.effectType = (*iter).second.show;
                eff.effectValue = value;
                if(attack){
                    mAtkWarEff.push_back(eff);
                }else{
                    mDefWarEff.push_back(eff);
                }
            }else{
                CCLOG("effect name is empty!");
            }
        }
        ++iter;
    }
    if(attack){
        sort(mAtkWarEff.begin(), mAtkWarEff.end(), sortByUseOder);
    }else{
        sort(mDefWarEff.begin(), mDefWarEff.end(), sortByUseOder);
    }
    effMap.clear();
}
bool BattleReportMailPopUpView::sortByUseOder(BattleReportWarEffect eff1, BattleReportWarEffect eff2){
    return (eff1.effectOrder>eff2.effectOrder);
}
void BattleReportMailPopUpView::generateWarEff(){
    mAtkWarEff.clear();
    mDefWarEff.clear();
    if(m_info->atkWarEffect){
        CCObject *ccObj = NULL;
        map<int,int> mAtkMap;
        CCARRAY_FOREACH(m_info->atkWarEffect, ccObj){
            CCDictionary* dict = _dict(ccObj);
            CCDictElement *ele = NULL;
            CCDICT_FOREACH(dict, ele){
                if(!ele->getObject())
                    continue;
                CCString *tmpStr = dynamic_cast<CCString*>(ele->getObject());
                if(!tmpStr || !tmpStr->getCString())
                    continue;
                int effectId = atoi(ele->getStrKey());
                float value = atof(tmpStr->getCString());
                if(value>0){
                    mAtkMap[effectId] = value;
                }
            }
        }
        initBattleReportWarEffect(mAtkMap, true);
    }
    if(m_info->dfWarEffect){
        CCObject *ccObj = NULL;
        map<int,int> mDefMap;
        CCARRAY_FOREACH(m_info->dfWarEffect, ccObj){
            CCDictionary* dict = _dict(ccObj);
            CCDictElement *ele = NULL;
            CCDICT_FOREACH(dict, ele){
                if(!ele->getObject())
                    continue;
                CCString *tmpStr = dynamic_cast<CCString*>(ele->getObject());
                if(!tmpStr || !tmpStr->getCString())
                    continue;
                int effectId = atoi(ele->getStrKey());
                float value = atof(tmpStr->getCString());
                if(value>0){
                    mDefMap[effectId] = value;
                }
            }
        }
        initBattleReportWarEffect(mDefMap, false);
    }
}
int BattleReportMailPopUpView::getTotalDead(CCArray *arr){
    CCObject *obj;
    int total = 0;
    CCARRAY_FOREACH(arr, obj) {
        CCDictionary *dict = dynamic_cast<CCDictionary*>(obj);
        int dead = dict->valueForKey("dead")->intValue();
        total += dead;
    }
    return total;
}

void BattleReportMailPopUpView::getData(){
    MailController::getInstance()->readMailFromServer(m_info->uid, CC_ITOA(m_info->type));
}

bool BattleReportMailPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailTitle", CCLabelIF*, this->m_mailTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackText", Label*, this->m_attackText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_failNode", CCNode*, this->m_failNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, this->m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upTitle", CCLabelIF*, this->m_upTitle);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_reportBtn", CCControlButton*, this->m_reportBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_firstNode", CCNode*, this->m_firstNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, this->m_upNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_detailBtn", CCControlButton*, this->m_detailBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battlePicNode", CCNode*, this->m_battlePicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_shareBtn", CCControlButton*, this->m_shareBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintBG", CCScale9Sprite*, this->m_hintBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintLine", CCSprite*, this->m_hintLine);

    return false;
}

SEL_CCControlHandler BattleReportMailPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReportClick", BattleReportMailPopUpView::onReportClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", BattleReportMailPopUpView::onDeleteClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBattleDetailClick", BattleReportMailPopUpView::onBattleDetailClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", BattleReportMailPopUpView::onAddSaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onShareClick", BattleReportMailPopUpView::onShareClick);
    return NULL;
}

void BattleReportMailPopUpView::onShareClick(CCObject *pSender, CCControlEvent event){
    double gapTime = m_info->shareTime -GlobalData::shared()->getWorldTime();
    if(gapTime<0){
        string reportLang = "115277";
        string msg = "";
        int result = this->getReportResult();
        std::string attName = "";
        if(m_info->attUser){
            attName = m_info->attUser->valueForKey("name")->getCString();
            if(m_info->attUser->objectForKey("alliance"))
            {
                std::string asn = m_info->attUser->valueForKey("alliance")->getCString();
                attName = std::string("(") + asn + ")" + m_info->attUser->valueForKey("name")->getCString();
            }
        }
        attUid = m_info->attUser->valueForKey("uid")->getCString();
        
        std::string defName = "";
        if(m_info->defUser){
            defName = m_info->defUser->valueForKey("name")->getCString();
            if (m_info->defUser->objectForKey("alliance")) {
                std::string asn = m_info->defUser->valueForKey("alliance")->getCString();
                defName = std::string("(") + asn + ")" + m_info->defUser->valueForKey("name")->getCString();
            }
        }
        defUid = m_info->defUser->valueForKey("uid")->getCString();
        string param1 = "";
        if(result==WIN){
            if(isatkHelp || ownerUid==attUid){
                reportLang = "115277";
                param1 = defName;
            }else{
                reportLang = "115279";
                param1 = attName;
            }
        }else if(result==DRAW){
            if(isatkHelp || ownerUid==attUid){
                reportLang = "115283";
                param1 = defName;
            }else{
                reportLang = "115284";
                param1 = attName;
            }
        
        }else{
            isdefHelp = false;
            if(m_info->defHelper!=NULL){
                cocos2d::CCObject* obj;
                CCARRAY_FOREACH(m_info->defHelper, obj){
                    string helpUid = dynamic_cast<CCString*>(obj)->getCString();
                    if(helpUid==ownerUid){
                        isdefHelp = true;
                        break;
                    }
                }
            }
            if(isdefHelp || ownerUid==defUid){
                reportLang = "115280";
                param1 = attName;
            }else{
                reportLang = "115278";
                param1 = defName;
            }
        }
        
        msg = _lang_1(reportLang, param1.c_str());
        CountryChatCommand * cmd = new CountryChatCommand(CHAT_STATE_ALLIANCE_COMMAND,msg.c_str(),CHAT_TYPE_BATTLE_SHARE);
        cmd->putParam("reportUid", CCString::create(m_info->uid));
        cmd->putParam("reportLang",CCString::create(reportLang));
        cmd->sendAndRelease();
        double shareTime = GlobalData::shared()->getWorldTime();
        shareTime = shareTime + 10*60 - 1;
        m_info->shareTime = shareTime;
    }else{
        int gap = gapTime/60 +1;
        if(gap<=0) gap = 1;
        CCCommonUtils::flyHint("", "", _lang_1("115276", CC_ITOA(gap)));
    }
}

void BattleReportMailPopUpView::shareSuccess(CCObject* param){
    CCCommonUtils::flyText(_lang("115275"));
}

void BattleReportMailPopUpView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(m_info->save ==1){
        MailCancelSaveCommand *cmd = new MailCancelSaveCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->save = 0;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105576"));
    }else{
        if(MailController::getInstance()->isMailFull(SAVEMAIL)){
            CCCommonUtils::flyText("full");
            return;
        }
        MailSaveCommand *cmd = new MailSaveCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->save = 1;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
    }
    if(m_info->save==0){
        m_unSaveBtn->setVisible(true);
        m_addSaveBtn->setVisible(false);
    }else{
        m_unSaveBtn->setVisible(false);
        m_addSaveBtn->setVisible(true);
    }
}
void BattleReportMailPopUpView::onBattleDetailClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupView(MailBattleDetailView::create(m_info), false);
}

void BattleReportMailPopUpView::onReportClick(CCObject *pSender, CCControlEvent event){
    CCCommonUtils::flyText(_lang("105534"));
    BattleManager::shared()->mailFightReport(m_info->reportUid);

}
void BattleReportMailPopUpView::onOkDeleteMail(){
    MailController::getInstance()->removeMail(m_info->uid, CC_ITOA(m_info->type));
    PopupViewController::getInstance()->goBackPopupView();
}
void BattleReportMailPopUpView::onDeleteClick(CCObject *pSender, CCControlEvent event){
    if(m_info->save!=0){
        CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(BattleReportMailPopUpView::onOkDeleteMail)),true);
}

bool BattleReportMailPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(this->m_listContainer, pTouch)){
        return true;
    }
    return false;
}

void BattleReportMailPopUpView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void BattleReportMailPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(this->m_listContainer, pTouch)){
        return;
    }
    int d = pTouch->getStartLocation().y - pTouch->getLocation().y;
    d = abs(d);
    if(d > 20){
        return;
    }
//    CCArray *arr = m_scrollView->getContainer()->getChildren();
//    CCArray *arr = m_ListNode->getChildren();
//    int i = 0;
    auto& arr = m_ListNode->getChildren();
    for (auto child : arr)
    {
//    while(arr && i < arr->count()){
//        GeneralInfoCell *generalCell = dynamic_cast<GeneralInfoCell*>(arr->objectAtIndex(i));
//        if(generalCell
//           && isTouchInside(generalCell, pTouch)){
//            generalCell->showGeneralInfo();
//        }else{
//            PlayerInfoCell *playerCell = dynamic_cast<PlayerInfoCell*>(arr->objectAtIndex(i));
//            if(playerCell
//               && isTouchInside(playerCell, pTouch)){
//                playerCell->cellTouch(pTouch);
//            }
//        }
        PlayerInfoCell *playerCell = dynamic_cast<PlayerInfoCell*>(child);
        if(playerCell
           && isTouchInside(playerCell, pTouch)){
            playerCell->cellTouch(pTouch);
            return;
        }
        MonsterInfoCell *mCell = dynamic_cast<MonsterInfoCell*>(child);
        if(mCell){
            mCell->cellTouch(pTouch);
            return;
        }
    }
}
std::string BattleReportMailPopUpView::getEffectDescByID(int effectID){
    switch (effectID) {
        case 56:
            return _lang("113923");
        case 1011:
            return _lang("113927");
        case 1031:
            return _lang("113928");
        case 1041:
            return _lang("113929");
        case 1013:
            return _lang("113930");
        case 1033:
            return _lang("113931");
        case 1035:
            return _lang("121068");
        case 1043:
            return _lang("113932");
        case 1015:
            return _lang("113933");
        case 1045:
            return _lang("113935");
        case 1017:
            return _lang("113936");
        case 1037:
            return _lang("113937");
        case 1047:
            return _lang("113938");
        case 1071:
            return _lang("130020");
        case 101010:
            return _lang("130024");
        case 1065:
            return _lang("130025");
        case 1207:
            return _lang("130026");
        case 1220:
            return _lang("113946");
        case 1230:
            return _lang("121052");
        case 1152:
            return _lang("113948");
        case 1153:
            return _lang("113949");
        case 1597:
            return _lang("113940");
        case 1591:
            return _lang("113943");
        case 1593:
            return _lang("113944");
        case 1595:
            return _lang("113945");
    }
    return "";
}
void BattleReportMailPopUpView::autoResizeHintNode(){
    CCSize hintSize = m_hintText->getContentSize();
    CCSize hintBGSize = m_hintBG->getContentSize();
    if (!CCCommonUtils::isIosAndroidPad())
    {
        int addH = hintBGSize.height - 40 - hintSize.height;
        if(addH<0){
            m_hintBG->setPreferredSize(CCSize(hintBGSize.width,hintSize.height+40));
            m_hintLine->setPositionY(m_hintLine->getPositionY() - addH*0.5);
            //        m_hintBG->setPositionY(m_hintBG->getPositionY() + addH*0.5);
            //        m_hintText->setPositionY(m_hintBG->getPositionY());
        }
    } else {
        int addH = hintBGSize.height * 2 - 60 - hintSize.height;
        if (addH < 0) {
            m_hintBG->setPreferredSize(CCSize(hintBGSize.width, (hintSize.height + 60)) / 2.0);
            m_hintLine->setPositionY(m_hintLine->getPositionY() - addH * 0.5);
        }
    }
}
//----------

GeneralInfoCell *GeneralInfoCell::create(GeneralInfo *info, CCDictionary *dict,bool boss){
    GeneralInfoCell *ret = new GeneralInfoCell(info, dict,boss);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GeneralInfoCell::init(){
    auto bg = CCBLoadFile("BattleReportGeneralCCB", this, this);
    this->setContentSize(bg->getContentSize());
    this->m_headContainer->removeAllChildren();
    this->m_headContainer->addChild(GeneralHeadPic::create(m_info->generalId));
    
    m_lvText->setString(_lang_1("103462", CC_ITOA(m_info->level)));
    m_lvText->setColor(CCCommonUtils::getItemColor(m_info->color));
    m_nameText->setString(CCCommonUtils::getNameById(m_info->generalId).c_str());
    m_nameText->setColor(CCCommonUtils::getItemColor(m_info->color));

    int exp = m_dict->valueForKey("exp")->intValue();
    int dead = m_dict->valueForKey("dead")->intValue();
    int kill = m_dict->valueForKey("kill")->intValue();
    int shangbing = m_dict->valueForKey("hurt")->intValue();
    int force = m_dict->valueForKey("num")->intValue();
    m_expText->setString(CC_ITOA(exp));
    m_killNumText->setString(CC_CMDITOA(kill));
    m_loseNumText->setString(CC_CMDITOA(dead));
    m_surplusNumText->setString(CC_CMDITOA(force));
    
    m_killText->setString(_lang("105543").c_str());
    m_loseText->setString(_lang("105544").c_str());
    m_surplusText->setString(_lang("105546").c_str());
    
    m_forceNode->setVisible(!m_isBoss);
    return true;
}

bool GeneralInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headContainer", CCNode*, this->m_headContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvText", CCLabelIF*, this->m_lvText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expText", CCLabelIF*, this->m_expText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_forceNode", CCNode*, this->m_forceNode);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killText", CCLabelIF*, this->m_killText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killNumText", CCLabelIF*, this->m_killNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loseText", CCLabelIF*, this->m_loseText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loseNumText", CCLabelIF*, this->m_loseNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_surplusText", CCLabelIF*, this->m_surplusText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_surplusNumText", CCLabelIF*, this->m_surplusNumText);
    
    return false;
}

void GeneralInfoCell::showGeneralInfo(){
//    PopupViewController::getInstance()->addPopupInView(GeneralDetailInfoPopUpView::create(m_info, OTHER_USER_TYPE));
}

//-----

PlayerInfoCell *PlayerInfoCell::create(CCDictionary *obj,CCArray* generals,MailInfo *mail){
    PlayerInfoCell *ret = new PlayerInfoCell(obj,generals,mail);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PlayerInfoCell::init(){
    auto bg = CCBLoadFile("BattleReportPlayerInfoCellCCB", this, this);
    this->setContentSize(bg->getContentSize());
    
    std::string nameStr = "";
    if(m_info->objectForKey("alliance")){
        std::string allianceStr =m_info->valueForKey("alliance")->getCString();
        nameStr.append("("+allianceStr+")");
    }
    bool isMonster = false;
    if(m_info->objectForKey("monster")){
        isMonster = m_info->valueForKey("monster")->boolValue();
    }
    nameStr.append(m_info->valueForKey("name")->getCString());
    m_pointID = m_info->valueForKey("pointId")->intValue();
    CCPoint pt = WorldController::getPointByIndex(m_pointID);
    if (m_mail->ckf==1) {
        pt = WorldController::getPointByMapTypeAndIndex(m_pointID,SERVERFIGHT_MAP);
    }
    m_posText->setString(_lang_2("105521", CC_ITOA(pt.x), CC_ITOA(pt.y)));
    string picstr = "";
    int picVer = 0;
    string uid = "";
    if(m_generals == m_mail->atkGen){
        picstr = m_mail->attUser->valueForKey("pic")->getCString();
        picVer = m_mail->attUser->valueForKey("picVer")->intValue();
        uid = m_mail->attUser->valueForKey("uid")->getCString();
        if(picstr==""){
            picstr ="g044.png";
        }else{
            picstr+=".png";
        }
    }else{
        picstr = m_mail->defUser->valueForKey("pic")->getCString();
        picVer = m_mail->defUser->valueForKey("picVer")->intValue();
        uid = m_mail->defUser->valueForKey("uid")->getCString();
        if(picstr==""){
            picstr ="g044.png";
        }else{
            picstr+=".png";
        }
    }
    if(isMonster){
        string mid = m_info->valueForKey("uid")->getCString();
        nameStr = CCCommonUtils::getNameById(mid);
        picstr = "tile_pop_icon21.png";
    }
    m_nameText->setString(nameStr.c_str());
    m_pic= CCLoadSprite::createSprite(picstr.c_str());
    CCCommonUtils::setSpriteMaxSize(m_pic, 94);
    
    if(nameStr==GlobalData::shared()->playerInfo.name){
        
    }else{
        // m_pic= CCLoadSprite::createSprite("g044.png");
    }
    std::string npc = m_info->valueForKey("npcId")->getCString();
    int dead = 0;
    int num = 0;
    int hurt = 0;
    int kill = 0;
    int total = 0;
    if(npc!=""){
        auto dic = LocalController::shared()->DBXMLManager()->getObjectByKey(m_info->valueForKey("npcId")->getCString());
        std::string strIcon = dic->valueForKey("icon_monster")->getCString();
        strIcon.append("_common.png");
        m_pic= CCLoadSprite::createSprite(strIcon.c_str());
        //m_powerText->setString("");
        m_nameText->setString(CCCommonUtils::getNameById(m_info->valueForKey("npcId")->getCString()).c_str());
        m_bossNode->setVisible(true);
        m_playerNode->setVisible(false);
        m_bossPos->setString(_lang_2("105521", CC_ITOA(pt.x), CC_ITOA(pt.y)));
        m_bossDesc->setString(_lang("105539").c_str());
        if(m_mail->defReport){
            int count = m_mail->defReport->count();
            for (int i=0; i<count; i++) {
                CCDictionary* dic = _dict(m_mail->defReport->objectAtIndex(i));
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
        float len = 1.0*num/total;
        m_expBar->setContentSize(CCSizeMake(len*266, 28));
        m_pic->setScale((m_pic->getContentSize().width-12)/105);
        m_bossPic->addChild(m_pic);
        std::string hpStr = "";
        hpStr.append(CC_ITOA(num));
        hpStr.append("/");
        hpStr.append(CC_ITOA(total));
        //m_bossNumTxt->setString(hpStr.c_str());
        m_bossNumTxt->setString("");
    }else{
        if(m_mail->defFortLost!=NULL){
          //  m_boxBg->setContentSize(CCSize(266,225));
            m_trapLossTxt->setString(_lang("105555").c_str());
            if(m_generals == m_mail->atkGen){
                m_trapLossNum->setString(CC_ITOA(0));
            }else{
                int total = 0;
                for (int i=0; i<m_mail->defFortLost->count(); i++) {
                    auto trapDic = _dict(m_mail->defFortLost->objectAtIndex(i));
                    if(trapDic){
                        total += trapDic->valueForKey("dead")->intValue();
                    }
                }
                m_trapLossNum->setString(CC_ITOA(total));
            }
        }
        if(m_generals == m_mail->atkGen){
            if(m_mail->attReport){
                int count = m_mail->attReport->count();
                for (int i=0; i<count; i++) {
                    CCDictionary* dic = _dict(m_mail->attReport->objectAtIndex(i));
                    if(dic){
                        dead += dic->valueForKey("dead")->intValue();
                        num += dic->valueForKey("num")->intValue();
                        hurt += dic->valueForKey("hurt")->intValue();
                        kill += dic->valueForKey("kill")->intValue();
                    }
                }
            }
            std::string powerStr = _lang("102163");
            powerStr.append(": ");
            std::string appendStr = m_mail->atkPowerLost == 0?_lang("105508"):CC_CMDITOA(-m_mail->atkPowerLost);
            powerStr.append(appendStr);
            m_powerText->setString(powerStr.c_str());
        }else{
            if(m_mail->defReport){
                int count = m_mail->defReport->count();
                for (int i=0; i<count; i++) {
                    CCDictionary* dic = _dict(m_mail->defReport->objectAtIndex(i));
                    if(dic){
                        dead += dic->valueForKey("dead")->intValue();
                        num += dic->valueForKey("num")->intValue();
                        hurt += dic->valueForKey("hurt")->intValue();
                        kill += dic->valueForKey("kill")->intValue();
                    }
                }
            }
            std::string powerStr = _lang("102163");
            powerStr.append(": ");
            std::string appendStr = m_mail->defPowerLost == 0?_lang("105508"):CC_CMDITOA(-m_mail->defPowerLost);
            powerStr.append(appendStr);
            m_powerText->setString(powerStr.c_str());
        }
        m_pic->setScale((94/m_pic->getContentSize().width));
        m_picNode->addChild(m_pic);
        
        if (CCCommonUtils::isUseCustomPic(picVer))
        {
            m_headImgNode = HFHeadImgNode::create();
            m_headImgNode->initHeadImgUrl2(m_picNode, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 85, true);
        }
    }
    total = dead+num+hurt;
    
    //m_totalText->setString(_lang("105542").c_str());
   // m_totalText->setString("");
    m_killText->setString(_lang("105543").c_str());
    m_loseText->setString(_lang("105544").c_str());
    m_woundedText->setString(_lang("105545").c_str());
    m_surplusText->setString(_lang("114113").c_str());
    
    //m_totalNumText->setString(CC_CMDITOA(total));
    //m_totalNumText->setString("");
    m_killNumText->setString(CC_CMDITOA(kill));
    m_loseNumText->setString(CC_CMDITOA(dead));
    m_woundedNumText->setString(CC_CMDITOA(hurt));
    m_surplusNumText->setString(CC_CMDITOA(num));
    addShowTeamInfo();
    return true;
}
void PlayerInfoCell::addShowTeamInfo(){
    int kill = 0;
    int dead = 0;
    int hurt = 0;
    int num = 0;
    if(m_generals == m_mail->atkGen){//攻方
        if(m_mail->atkArmyTotal){
            CCDictionary* dic = _dict(m_mail->atkArmyTotal);
            kill =dic->valueForKey("kill")->intValue();
            dead =dic->valueForKey("dead")->intValue();
            hurt =dic->valueForKey("hurt")->intValue();
            num =dic->valueForKey("num")->intValue();
        }
        std::string powerStr = _lang("102163");
        powerStr.append(": ");
        std::string appendStr = m_mail->atkPowerLost == 0?_lang("105508"):CC_CMDITOA(-m_mail->atkPowerLost);
        powerStr.append(appendStr);
        if(m_mail->atkAlliance){
           // m_picNode->removeAllChildren();
            CCDictionary* dic = _dict(m_mail->atkAlliance);
            string leagueName = dic->valueForKey("allianceName")->getCString();
            string leagueIcon = dic->valueForKey("allianceIcon")->getCString();
            if(leagueIcon==""){

                leagueIcon = "Allance_flay.png";
            }
//            m_pic= CCLoadSprite::createSprite(leagueIcon.c_str());
//            m_pic->setScale((94/m_pic->getContentSize().height));
//            m_picNode->addChild(m_pic);
           // m_nameText->setString(leagueName.c_str());
            std::string nameStr = m_info->valueForKey("name")->getCString();
            nameStr= _lang_1("105595", nameStr.c_str());
            m_nameText->setString(nameStr.c_str());
           // m_powerText->setString(nameStr.c_str());
           // m_powerText1->setString(powerStr.c_str());
        }else{

            //m_powerText->setString(powerStr.c_str());
        }
        CCArray *atkGenKilltArr = dynamic_cast<CCArray*>(m_mail->atkGenKill);
        CCObject *atkGenKilltobj;
        if(atkGenKilltArr){
            CCARRAY_FOREACH(atkGenKilltArr, atkGenKilltobj){
                kill+=dynamic_cast<CCString*>(atkGenKilltobj)->intValue();
            }
        }
    }else{
        if(m_mail->defArmyTotal){
            kill =m_mail->defArmyTotal->valueForKey("kill")->intValue();
            dead =m_mail->defArmyTotal->valueForKey("dead")->intValue();
            hurt =m_mail->defArmyTotal->valueForKey("hurt")->intValue();
            num =m_mail->defArmyTotal->valueForKey("num")->intValue();
        }
        CCArray *defGenKilltArr = dynamic_cast<CCArray*>(m_mail->defGenKill);
        CCObject *defGenKilltobj;
        if(defGenKilltArr){
            CCARRAY_FOREACH(defGenKilltArr, defGenKilltobj){
                kill+=dynamic_cast<CCString*>(defGenKilltobj)->intValue();
            }
        }
        if(m_mail->defTowerKill!=NULL){

            for (int i=0; i<m_mail->defTowerKill->count(); i++) {
                auto trapDic = _dict(m_mail->defTowerKill->objectAtIndex(i));
                if(trapDic){
                    kill += trapDic->valueForKey("kill")->intValue();
                }
            }
        }
        std::string powerStr = _lang("102163");
        powerStr.append(": ");
        std::string appendStr = m_mail->defPowerLost == 0?_lang("105508"):CC_CMDITOA(-m_mail->defPowerLost);
        powerStr.append(appendStr);
   
        if(m_mail->defAlliance){
           //m_picNode->removeAllChildren();
            CCDictionary* dic = _dict(m_mail->defAlliance);
            string leagueName = dic->valueForKey("allianceName")->getCString();
            string leagueIcon = dic->valueForKey("allianceIcon")->getCString();
//            if(leagueIcon==""){
// 
//                leagueIcon = "Allance_flay.png";
//            }
//            m_pic= CCLoadSprite::createSprite(leagueIcon.c_str());
//            m_pic->setScale((94/m_pic->getContentSize().height));
//            m_picNode->addChild(m_pic);
           // m_nameText->setString(leagueName.c_str());
            std::string nameStr = m_info->valueForKey("name")->getCString();
            nameStr= _lang_1("105595", nameStr.c_str());
            m_nameText->setString(nameStr.c_str());
           // m_powerText->setString(nameStr.c_str());
            //m_powerText1->setString(powerStr.c_str());
        }else{
            //m_powerText->setString(powerStr.c_str());
        }
    }
    if(m_mail->defFortLost!=NULL){
        //  m_boxBg->setContentSize(CCSize(266,225));
        m_trapLossTxt->setString(_lang("105555").c_str());
        if(m_generals == m_mail->atkGen){
            m_trapLossNum->setString(CC_ITOA(0));
        }else{
            int total = 0;
            for (int i=0; i<m_mail->defFortLost->count(); i++) {
                auto trapDic = _dict(m_mail->defFortLost->objectAtIndex(i));
                if(trapDic){
                    total += trapDic->valueForKey("dead")->intValue();
                    kill+=trapDic->valueForKey("kill")->intValue();
                }
            }
            m_trapLossNum->setString(CC_ITOA(total));
        }
    }

    m_killNumText->setString(CC_CMDITOA(kill));
    m_loseNumText->setString(CC_CMDITOA(dead));
    m_woundedNumText->setString(CC_CMDITOA(hurt));
    m_surplusNumText->setString(CC_CMDITOA(num));

}
void PlayerInfoCell::onBossPosClick(CCObject * pSender, Control::EventType pCCControlEvent){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::stopReturnToChat();
#endif
    CCPoint pt = WorldController::getPointByIndex(m_pointID);
    if (m_mail && m_mail->ckf==1) {
        pt = WorldController::getPointByMapTypeAndIndex(m_pointID,SERVERFIGHT_MAP);
    }
    WorldController::getInstance()->openTargetIndex = m_pointID;
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
    }else{
        int index = WorldController::getIndexByPoint(pt);
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
    PopupViewController::getInstance()->removeAllPopupView();
}

void PlayerInfoCell::onPosBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::stopReturnToChat();
#endif
    CCPoint pt = WorldController::getPointByIndex(m_pointID);
    if (m_mail && m_mail->ckf==1) {
        pt = WorldController::getPointByMapTypeAndIndex(m_pointID,SERVERFIGHT_MAP);
    }
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
    }else{
        int index = WorldController::getIndexByPoint(pt);
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
    PopupViewController::getInstance()->removeAllPopupView();
}

bool PlayerInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", Label*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posText", CCLabelIF*, this->m_posText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerText", CCLabelIF*, this->m_powerText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerText1", CCLabelIF*, this->m_powerText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalText", CCLabelIF*, this->m_totalText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNumText", CCLabelIF*, this->m_totalNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killText", CCLabelIF*, this->m_killText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killNumText", CCLabelIF*, this->m_killNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loseText", CCLabelIF*, this->m_loseText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loseNumText", CCLabelIF*, this->m_loseNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedText", CCLabelIF*, this->m_woundedText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedNumText", CCLabelIF*, this->m_woundedNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_surplusText", CCLabelIF*, this->m_surplusText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_surplusNumText", CCLabelIF*, this->m_surplusNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bossNumTxt", CCLabelIF*, this->m_bossNumTxt);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bossNode", CCNode*, this->m_bossNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playerNode", CCNode*, this->m_playerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bossPic", CCNode*, this->m_bossPic);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bossPos", CCLabelIF*, this->m_bossPos);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bossDesc", CCLabelIF*, this->m_bossDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expBar", CCScale9Sprite*, this->m_expBar);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_trapLossTxt", CCLabelIF*, this->m_trapLossTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_trapLossNum", CCLabelIF*, this->m_trapLossNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_boxBg", CCScale9Sprite*, this->m_boxBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posBG", CCNode*, this->m_posBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconBtn", CCControlButton*, this->m_iconBtn);
    
    return false;
}

SEL_CCControlHandler PlayerInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPosBtnClick", PlayerInfoCell::onPosBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBossPosClick", PlayerInfoCell::onBossPosClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "playHeadClick", PlayerInfoCell::playHeadClick);
    return NULL;
}

void PlayerInfoCell::playHeadClick(CCObject *pSender, CCControlEvent event){
//    std::string npc = m_info->valueForKey("npcId")->getCString();
//    if(npc!="")return ;
    std::string uid = m_info->valueForKey("uid")->getCString();
    if (uid == GlobalData::shared()->playerInfo.uid) {
        PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(&(GlobalData::shared()->playerInfo)));
    }
    else {
        RoleInfoView::createInfoByUid(uid);
    }
}

void PlayerInfoCell::cellTouch(CCTouch *pTouch){
    if(isTouchInside(m_posBG, pTouch)){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::stopReturnToChat();
#endif
        int pos = m_info->valueForKey("pointId")->intValue();
        WorldController::getInstance()->openTargetIndex = pos;
        CCPoint pt = WorldController::getPointByIndex(pos);
        if (m_mail && m_mail->ckf==1) {
            pt = WorldController::getPointByMapTypeAndIndex(m_pointID,SERVERFIGHT_MAP);
        }
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            WorldMapView::instance()->gotoTilePoint(pt);
        }else{
            int index = WorldController::getIndexByPoint(pt);
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
        }
        PopupViewController::getInstance()->removeAllPopupView();
    }else if(isTouchInside(m_iconBtn, pTouch)){
        this->playHeadClick(NULL,Control::EventType::TOUCH_DOWN);
    }
    
//    else if(isTouchInside(m_pic, pTouch)){
//        std::string npc = m_info->valueForKey("npcId")->getCString();
//        if(npc!="") return ;
//        PlayerInfo *info = new PlayerInfo();
//        info->updateInfo(m_info);
//        info->parseGenerals(m_generals);
//        PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(info));
//    }
}

//
//PlayerEffectCell
//
PlayerEffectCell *PlayerEffectCell::create(vector<BattleReportWarEffect> &effArr){
    PlayerEffectCell *ret = new PlayerEffectCell();
    if(ret && ret->init(effArr)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PlayerEffectCell::init(vector<BattleReportWarEffect> &effArr){
    if(CCNode::init()){
        CCBLoadFile("BattleReportPlayerInfoCellCCB_1", this, this);
        m_nameText->setString(_lang("114140"));
        auto iter = effArr.begin();
        int iIndex = 0;
        float tmpH = 0;
        while (iter!=effArr.end()) {
            std::string effecDes = (*iter).effectName;
            float value = (*iter).effectValue;
            std::string valueDes = "";
            if((*iter).effectType == 0){
                valueDes = std::string("+") + CC_ITOA(round(value)) + "%";
            }else{
                valueDes = std::string("+") + CC_ITOA(round(value));
            }
            tmpH = createContent(effecDes,valueDes,tmpH);
            ++iIndex;
            ++iter;
        }
        if (CCCommonUtils::isIosAndroidPad())
        {
            tmpH = tmpH + 120;
        } else {
            tmpH = tmpH + 60;
        }
        float tmpW = m_sprBG->getContentSize().width;
        float oldH = m_sprBG->getContentSize().height;
        m_sprBG->setPreferredSize(CCSize(tmpW,tmpH));
        float changeH = tmpH - oldH;
        m_sprBG->setPositionY(m_sprBG->getPositionY() + changeH*0.5);
        m_sprTitleBG->setPositionY(m_sprTitleBG->getPositionY()+changeH);
        m_nameText->setPositionY(m_sprTitleBG->getPositionY());
        CCSize oldNodeSize = m_nodeList->getContentSize();
        m_nodeList->setContentSize(CCSize(oldNodeSize.width, oldNodeSize.height + changeH));
        setContentSize(CCSize(tmpW, tmpH));
        return true;
    }
    return false;
}
bool PlayerEffectCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG", CCScale9Sprite*, this->m_sprBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeList", CCNode*, this->m_nodeList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprTitleBG", CCScale9Sprite*, this->m_sprTitleBG);
    return false;
}
float PlayerEffectCell::createContent(std::string desc,std::string percent,float nowH){
    ccColor3B tColor = {130,99,56};
    ccColor3B pColor = {127,35,29};
    CCLabelIF *lable1 = CCLabelIF::create("", 20);
    lable1->setAnchorPoint(ccp(0, 0.5));
    lable1->setAlignment(kCCTextAlignmentLeft);
    lable1->setColor(tColor);
    lable1->setDimensions(CCSize(CCSize(210,0)));
    lable1->setString(desc);
    if (CCCommonUtils::isIosAndroidPad())
    {
        lable1->setFontSize(40);
        lable1->setDimensions(CCSize(500, 0));
    }
    m_nodeList->addChild(lable1);
    float labelH = lable1->getContentSize().height * lable1->getOriginScaleY();
    lable1->setPosition(ccp(20, nowH + labelH*0.5));
    if (CCCommonUtils::isIosAndroidPad())
    {
        lable1->setPosition(30, nowH + labelH * 0.5);
    }
    
    CCLabelIF *lable2 = CCLabelIF::create("", 20);
    lable2->setAnchorPoint(ccp(1, 0.5));
    lable2->setAlignment(kCCTextAlignmentRight);
    lable2->setColor(pColor);
    lable2->setDimensions(CCSize(CCSize(120,0)));
    lable2->setString(percent);
    m_nodeList->addChild(lable2);
    lable2->setPosition(ccp(280, lable1->getPositionY()));
    if (CCCommonUtils::isIosAndroidPad()) {
        lable2->setFontSize(40);
        lable2->setDimensions(CCSize(240, 0));
        lable2->setPositionX(620);
    }
    
    nowH += labelH + 5;
    if (CCCommonUtils::isIosAndroidPad())
    {
        nowH += 5;
    }
    return nowH;
}
//----

ReportNormalRewardCell *ReportNormalRewardCell::create(int type, int num,int itemId,int flag){
    ReportNormalRewardCell *ret = new ReportNormalRewardCell();
    if(ret && ret->init(type, num,itemId,flag)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ReportNormalRewardCell::init(int type, int num,int itemId,int flag){
    auto sprite = CCLoadSprite::createScale9Sprite("affair_reward_bg.png");
    sprite->setPreferredSize(CCSize(200, 40));
    sprite->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(sprite);
    
    auto resIcon = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(type, itemId).c_str());
    this->addChild(resIcon);
    resIcon->setPosition(ccp(-sprite->getContentSize().width / 2, 0));
    
    CCLabelIF *label = CCLabelIF::create();
    label->setAnchorPoint(ccp(0.5, 0.5));
    label->setPosition(ccp(0, 0));
    label->setColor(ccc3(253, 255, 103));
    label->setFontSize(22);
    this->addChild(label);
    if(flag==2){
        label->setString((CC_CMDITOA(num)).c_str());
    }else{
        label->setString((std::string("+") + CC_CMDITOA(num)).c_str());
    }
    return true;
}

// monster

MonsterInfoCell *MonsterInfoCell::create(CCDictionary *obj,CCArray* generals,MailInfo *mail){
    MonsterInfoCell *ret = new MonsterInfoCell(obj,generals,mail);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MonsterInfoCell::init(){
    auto bg = CCBLoadFile("BattleMonsterInfoCell", this, this);
    this->setContentSize(bg->getContentSize());
    
    std::string nameStr = "";
    if(m_info->objectForKey("alliance")){
        std::string allianceStr =m_info->valueForKey("alliance")->getCString();
        nameStr.append("("+allianceStr+")");
    }
    bool isMonster = false;
    if(m_info->objectForKey("monster")){
        isMonster = m_info->valueForKey("monster")->boolValue();
    }
    nameStr.append(m_info->valueForKey("name")->getCString());
    m_pointID = m_info->valueForKey("pointId")->intValue();
    CCPoint pt = WorldController::getPointByIndex(m_pointID);
    m_posText->setString(_lang_2("105521", CC_ITOA(pt.x), CC_ITOA(pt.y)));
    string picstr = "";
    int picVer = 0;
    string uid = "";
    if(m_generals == m_mail->atkGen){
        picstr = m_mail->attUser->valueForKey("pic")->getCString();
        picVer = m_mail->attUser->valueForKey("picVer")->intValue();
        uid = m_mail->attUser->valueForKey("uid")->getCString();
        if(picstr==""){
            picstr ="g044.png";
        }else{
            picstr+=".png";
        }
    }else{
        picstr = m_mail->defUser->valueForKey("pic")->getCString();
        picVer = m_mail->defUser->valueForKey("picVer")->intValue();
        uid = m_mail->defUser->valueForKey("uid")->getCString();
        if(picstr==""){
            picstr ="g044.png";
        }else{
            picstr+=".png";
        }
    }
    string mid = "";
    if(isMonster){
        mid = m_info->valueForKey("uid")->getCString();
        nameStr = CCCommonUtils::getNameById(mid);
        picstr = "tile_pop_icon21.png";
    }

    auto pic = CCLoadSprite::createSprite(picstr.c_str());
    CCCommonUtils::setSpriteMaxSize(pic, 90);
    m_picNode->addChild(pic);
    if (!isMonster && CCCommonUtils::isUseCustomPic(picVer))
    {
        m_headImgNode = HFHeadImgNode::create();
        m_headImgNode->initHeadImgUrl2(m_picNode, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 90, true);
    }
    
    if (isMonster) {
        int mType = 0;
        int color = 1;
        string monsterStr = CCCommonUtils::getPropById(mid, "monster");
        if(monsterStr=="heiqishi_1"){
            mType = 3;
            color = 0;

        }else if (monsterStr=="heiqishi_2"){
            mType = 3;
            color = 1;
        }else{
            mType = 1;
            color = 3;
        }
        if(mType!=2){
            auto preBg = CCLoadSprite::createSprite(CCString::createWithFormat("Activty_icon_BG%d.png",mType)->getCString());
            CCCommonUtils::setSpriteMaxSize(preBg, 92,true);
            m_picNode->addChild(preBg);
        }
        auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
        CCCommonUtils::setSpriteMaxSize(iconBg, 90,true);
        m_picNode->addChild(iconBg,-100);
    }
    int dead = 0;
    int num = 0;
    int hurt = 0;
    int kill = 0;
    int total = 0;
    if(m_mail->defFortLost!=NULL){
        if(m_generals == m_mail->atkGen){
        }else{
            int total = 0;
            for (int i=0; i<m_mail->defFortLost->count(); i++) {
                auto trapDic = _dict(m_mail->defFortLost->objectAtIndex(i));
                if(trapDic){
                    total += trapDic->valueForKey("dead")->intValue();
                }
            }
        }
    }
    if(m_generals == m_mail->atkGen){
        string percentStr = CC_ITOA(m_mail->winPercent);
        percentStr.append("%");
        string temp = _lang_1("133093", percentStr.c_str());
        if (m_mail->failTimes>0) {
            temp.append("\n");
            temp.append(_lang_1("133057",CC_ITOA(m_mail->failTimes)));
            m_txtTip->setString(temp);
            m_bg->setContentSize(CCSize(624.0,230.0));
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_bg->setContentSize(CCSize(1420, 460));
            }
        }else{
            m_txtTip->setString(temp);
            m_bg->setContentSize(CCSize(624.0,200.0));
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_bg->setContentSize(CCSize(1420, 400));
            }
        }

        if(m_mail->attReport){
            int count = m_mail->attReport->count();
            for (int i=0; i<count; i++) {
                CCDictionary* dic = _dict(m_mail->attReport->objectAtIndex(i));
                if(dic){
                    dead += dic->valueForKey("dead")->intValue();
                    num += dic->valueForKey("num")->intValue();
                    hurt += dic->valueForKey("hurt")->intValue();
                    kill += dic->valueForKey("kill")->intValue();
                }
            }
        }
    }else{
        if(m_mail->defReport){
            int count = m_mail->defReport->count();
            for (int i=0; i<count; i++) {
                CCDictionary* dic = _dict(m_mail->defReport->objectAtIndex(i));
                if(dic){
                    dead += dic->valueForKey("dead")->intValue();
                    num += dic->valueForKey("num")->intValue();
                    hurt += dic->valueForKey("hurt")->intValue();
                    kill += dic->valueForKey("kill")->intValue();
                }
            }
        }
    }
    total = dead+num+hurt;
    
    if(m_generals == m_mail->atkGen){
        m_txt1->setString(_lang("133096").c_str());
        m_txt2->setString(_lang("133097").c_str());
        m_txt3->setString(_lang("133098").c_str());
        
        m_txtv1->setString(CC_CMDITOA(total));
        m_txtv2->setString(CC_CMDITOA(dead));
        double percent = dead*1.0/total;
        m_txtv3->setString(CC_ITOA_Percent(percent));
        string temp = "";
        if (m_mail->attUser) {
            string mid = m_mail->attUser->valueForKey("uid")->getCString();
            temp = CCCommonUtils::getNameById(mid);
        }
        temp.append("  ");
        temp.append(_lang_1("102272", CC_ITOA(m_mail->monsterLevel)));
        m_title->setString(temp.c_str());
    }else{
        m_txt1->setString(_lang("133101").c_str());
        m_txt2->setString(_lang("105545").c_str());
        m_txt3->setString(_lang("105544").c_str());
        
        m_txtv1->setString(CC_CMDITOA(-m_mail->defPowerLost));
        m_txtv2->setString(CC_CMDITOA(hurt));
        m_txtv3->setString(CC_CMDITOA(dead));
        
        if (m_mail->defUser) {
            std::string defName = m_mail->defUser->valueForKey("name")->getCString();
            m_title->setString(defName.c_str());
        }
    }

    return true;
}

void MonsterInfoCell::onPosBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::stopReturnToChat();
#endif
    CCPoint pt = WorldController::getPointByIndex(m_pointID);
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
    }else{
        int index = WorldController::getIndexByPoint(pt);
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
    //PopupViewController::getInstance()->removeAllPopupView();
}

bool MonsterInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title", CCLabelIFTTF*, this->m_title);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posText", CCLabelIF*, this->m_posText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtv1", CCLabelIF*, this->m_txtv1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtv2", CCLabelIF*, this->m_txtv2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtv3", CCLabelIF*, this->m_txtv3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtTip", CCLabelIF*, this->m_txtTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posBG", CCNode*, this->m_posBG);
    return false;
}

SEL_CCControlHandler MonsterInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

void MonsterInfoCell::cellTouch(CCTouch *pTouch){
    if(isTouchInside(m_posBG, pTouch)){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::stopReturnToChat();
#endif
        int pos = m_info->valueForKey("pointId")->intValue();
        WorldController::getInstance()->openTargetIndex = pos;
        CCPoint pt = WorldController::getPointByIndex(pos);
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            WorldMapView::instance()->gotoTilePoint(pt);
        }else{
            int index = WorldController::getIndexByPoint(pt);
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
        }
        PopupViewController::getInstance()->removeAllPopupView();
    }
}
