//
//  ChatCell.cpp
//  IF
//
//  Created by fubin on 13-10-30.
//
//

#include "ChatCell.h"
#include "PopupViewController.h"
#include "LocalController.h"
#include "CCCommonUtils.h"
#include "CCLabelIF.h"
#include "ChatController.h"
#include "YesNoDialog.h"
#include "ChatLockCommand.h"
#include "ChatView.h"
#include "TranslateCommand.h"
#include "ChatIsInAllanceCommand.h"
#include "ApplyAllianceCommand.h"
#include "AllianceFlagPar.h"
#include "BattleReportMailPopUpView.h"
#include "MailController.h"
#include "MailReadCommand.h"
#include "DetectMailPopUpView.h"
#include "WorldController.h"

ChatCell* ChatCell::create(int type, int idx, CCNode* touchNode,string allianceId){
    ChatCell* ret = new ChatCell();
    if(ret && ret->init(type, idx, touchNode,allianceId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ChatCell::init(int type, int idx, CCNode* touchNode,string allianceId)
{
    bool ret = true;
    CCBLoadFile("ChatCell",this,this);
    this->setContentSize(CCSizeMake(640, 130));
//    m_touchNode = touchNode;
    m_bgSize = m_chatBG->getContentSize();
    m_timeNodeY = m_timeNode->getPositionY();
    m_nameLabelPt = m_nameLabel->getPosition();
    m_vipLabelPt = m_vipLabel->getPosition();
    m_translatePt = m_translateNode->getPosition();
//    m_touchNode3Y = m_touchNode3->getPositionY();
    m_msgLabelY = m_msgLabel->getPositionY();
    m_msgDimensions = m_msgLabel->getDimensions();
    m_vipLabel->setFntFile("pve_fnt_title.fnt");
    m_isOriginal = false;
    m_vipLabel->setScale(0.7);
    m_mod = CCLoadSprite::createSprite("mod.png");
    m_mod->setAnchorPoint(ccp(0, 0.5));
    m_mod->setVisible(false);
    m_msgNode->addChild(m_mod);
    m_gm = CCLoadSprite::createSprite("gm.png");
    m_gm->setAnchorPoint(ccp(0, 0.5));
    m_gm->setVisible(false);
    m_msgNode->addChild(m_gm);
	m_translateBtn->setTouchPriority(Touch_Popup_Item);
	m_isCheckingInAllance = false;
	m_prevPopupCount = -1;
    m_joinBtn->setTouchPriority(Touch_Popup_Item);
    CCCommonUtils::setButtonTitle(m_joinBtn, _lang("115262").c_str());
    m_headImgNode = HFHeadImgNode::create();
    m_joinBtn->setSwallowsTouches(false);
    m_translateBtn->setSwallowsTouches(false);
    setData(type, idx,allianceId);

    return ret;
}

void ChatCell::setData(int type, int idx,string allianceId)
{
    m_idx = idx;
    m_type = type;
    m_allianceId = allianceId;
    m_picNode->removeAllChildren();
    m_chatBG->setContentSize(m_bgSize);
    m_sysBG->setContentSize(m_bgSize);
    m_noticeBG->setContentSize(m_bgSize);
    m_touchNode1->setContentSize(m_bgSize);
    m_timeNode->setPositionY(m_timeNodeY);
    m_nameLabel->setPosition(m_nameLabelPt);
    m_vipLabel->setPosition(m_vipLabelPt);
    m_translateNode->setPosition(m_translatePt);
//    m_touchNode3->setPositionY(m_touchNode3Y);
    m_msgLabel->setPositionY(m_msgLabelY);
    m_msgLabel->setDimensions(m_msgDimensions);
    m_timeLabel->setString("");
    m_vipLabel->setString("");
    m_mod->setVisible(false);
    m_gm->setVisible(false);
    m_joinBtn->setVisible(false);
    m_noticeBG->setVisible(false);
    m_noticeright->setVisible(false);
    m_noticeleft->setVisible(false);
//    int line = 0;
    float height = 0;
    if (m_type==CHAT_COUNTRY){
        auto& info = ChatController::getInstance()->m_chatCountryPool[idx];
        CCSprite* pic = NULL;
        if(info.senderPic=="" || info.senderPic=="0"){
            pic = CCLoadSprite::createSprite("g044.png");
        }else{
            std::string head = info.senderPic;
            head.append(".png");
            pic = CCLoadSprite::createSprite(head.c_str());
        }
        CCCommonUtils::setSpriteMaxSize(pic, 80);
        m_picNode->addChild(pic);
        if (showCustomPic(info.senderPicVer))
        {
            m_headImgNode->initHeadImgUrl2(m_picNode, CCCommonUtils::getCustomPicUrl(info.uid, info.senderPicVer), 1.0f, 80, true);
        }
//        line =ceil(info.msg.size() / 35);
//        height = m_bgSize.height;
//        if (line > 2) {
//            height += 25 * (line - 2);
//        }
        
        string _name = info.name;
        if (info.asn != "") {
            _name = "(" + info.asn + ") " + info.name;
        }
        if (info.vip > 0) {
            m_vipLabel->setString(_lang_1("103001", CC_ITOA(info.vip)));
            float infoX = m_vipLabel->getPositionX()+m_vipLabel->getContentSize().width*m_vipLabel->getOriginScaleX()+ 10;
            m_nameLabel->setPositionX(infoX);
        }
        m_nameLabel->setString(_name.c_str());
        if(info.gmFlag == 2 || info.gmFlag == 4 || info.gmFlag == 5)
        {
            float modX = m_nameLabel->getPositionX() + m_nameLabel->getContentSize().width + 10;
            m_mod->setPosition(ccp(modX, m_nameLabel->getPositionY()));
            m_gm->setVisible(false);
            m_mod->setVisible(true);
            if(info.gmFlag == 2){
                m_mod->setDisplayFrame(CCLoadSprite::loadResource("mod.png"));
            }else if(info.gmFlag == 5){
                m_mod->setDisplayFrame(CCLoadSprite::loadResource("tmod.png"));
            }else{
                m_mod->setDisplayFrame(CCLoadSprite::loadResource("smod.png"));
            }

            m_nameLabel->setColor({65,119,41});
        }else if(info.gmFlag == 3){
            float gmX = m_nameLabel->getPositionX() + m_nameLabel->getContentSize().width + 10;
            m_gm->setPosition(ccp(gmX, m_nameLabel->getPositionY()));
            m_mod->setVisible(false);
            m_gm->setVisible(true);
            m_nameLabel->setColor({33,111,169});
        }else{
            m_nameLabel->setColor({111,26,0});
        }
        
        bool flag = checkSameLangBySystem(info.originalLang);//本地语言和和 源语言一致，也不翻译
        //处理原文/译文  自己不翻译 /*&& info.originalLang != "en"cocos2d::extension::CCDevice::getLanguage()*/
        if(!flag && GlobalData::shared()->translation && info.uid!=GlobalData::shared()->playerInfo.uid && info.translationMsg!=info.msg && info.translationMsg!=""){
            m_translateNode->setVisible(true);
        }else{
            if(info.uid==GlobalData::shared()->playerInfo.uid){
                m_translateNode->setVisible(false);
            }else{
                m_translateNode->setVisible(true);
            }
        }
        string typeCache="";
        if(info.originalLang!=""){
            string typeCacheKey = "LanguageTranslation.set."+info.originalLang;
            typeCache= CCUserDefault::sharedUserDefault()->getStringForKey(typeCacheKey.c_str());
        }
        
        m_msgLabel->setDimensions(CCSizeMake(450, 0));
        bool bTrans = true;
        if(info.translationMsg.empty()){
            bTrans=false;
            if(!GlobalData::shared()->translation || typeCache!=""  || info.uid==GlobalData::shared()->playerInfo.uid || flag){
                m_translateLabel->setString(_lang("105322"));
                //CCCommonUtils::setButtonTitle(m_translateBtn, "translate");
                m_msgLabel->setString(info.msg.c_str());
                m_isOriginal = true;
            }
        }else{
            if(info.uid==GlobalData::shared()->playerInfo.uid){
                bTrans=false;
            }else{
                bTrans=true;
            }
        }
        if(bTrans==true){
            if(info.showOriginal)
            {
                m_translateLabel->setString(_lang("105322"));
                m_msgLabel->setString(info.msg.c_str());
                m_isOriginal = true;
            }else{
                m_translateLabel->setString(_lang_1("105321", info.originalLang.c_str()));
                //CCCommonUtils::setButtonTitle(m_translateBtn, "original");
                m_msgLabel->setString(info.translationMsg.c_str());
                m_isOriginal = false;
            }
        }
        
        height = m_bgSize.height;
        float _tmpHeight = MAX(getTextDimension(info.translationMsg.c_str()).height,getTextDimension(info.msg.c_str()).height);
        float offsetH = 0;
        if (_tmpHeight > 50) {
            offsetH = _tmpHeight - 45;
            height += offsetH;
            m_msgLabel->setPositionY(66+offsetH);
            m_nameLabel->setPositionY(75+offsetH);
            m_timeNode->setPositionY(124+offsetH);
            m_translateNode->setPositionY(76+offsetH);
            m_vipLabel->setPositionY(m_nameLabel->getPositionY());
        }
        m_uid = info.uid;
        info.isNew = false;
        if(info.isTimeShowed){
            m_timeLabel->setString(info.showedTime.c_str());
        }else if (idx ==0 ) {
            time_t infoTime = GlobalData::shared()->getTimeStamp();
            string lastTime = CCCommonUtils::timeStampToDHM(infoTime).substr(0,10);
            string nowTime = CCCommonUtils::timeStampToDHM(info.time).substr(0,10);
            if (lastTime == nowTime) {
                m_timeLabel->setString(CCCommonUtils::timeStampToHour(info.time).c_str());
                info.showedTime = CCCommonUtils::timeStampToHour(info.time);
                info.isTimeShowed = true;
            }
            else {
                m_timeLabel->setString(CCCommonUtils::timeStampToDHM(info.time).c_str());
                info.showedTime = CCCommonUtils::timeStampToDHM(info.time);
                info.isTimeShowed = true;
            }
        }
        else {
            time_t lt = ChatController::getInstance()->m_chatCountryPool[idx-1].time;
            if ((info.time-lt) < ChatController::getInstance()->chat_time) {
                m_timeLabel->setString("");
                info.showedTime = "";
                info.isTimeShowed = true;
            }
            else {
                string lastTime = CCCommonUtils::timeStampToDHM(lt).substr(0,10);
                string nowTime = CCCommonUtils::timeStampToDHM(info.time).substr(0,10);
                if (lastTime == nowTime) {
                    m_timeLabel->setString(CCCommonUtils::timeStampToHour(info.time).c_str());
                    info.showedTime = CCCommonUtils::timeStampToHour(info.time);
                    info.isTimeShowed = true;
                }
                else {
                    m_timeLabel->setString(CCCommonUtils::timeStampToDHM(info.time).c_str());
                    info.showedTime = CCCommonUtils::timeStampToDHM(info.time);
                    info.isTimeShowed = true;
                }
            }
        }
        
        if (info.uid == GlobalData::shared()->playerInfo.uid) {
            m_userNode->setPositionX(510);
            m_msgNode->setPositionX(-102);
            m_left->setVisible(false);
            if(info.post == CHAT_TYPE_NOTICE){
                m_noticeright->setVisible(true);
                m_right->setVisible(false);
            }else{
                m_right->setVisible(true);
            }
            
        }
        else {
            m_userNode->setPositionX(0);
            m_msgNode->setPositionX(0);
             m_right->setVisible(false);
            if(info.post == CHAT_TYPE_NOTICE){
                m_left->setVisible(false);
                m_noticeleft->setVisible(true);
            }else{
                m_left->setVisible(true);
            }
        }
//        m_chatBG->setVisible(true);
//        m_sysBG->setVisible(false);
        if(info.post==0){
            m_chatBG->setVisible(true);
            m_chatBG->setPreferredSize(CCSize(m_bgSize.width,height));
            m_sysBG->setVisible(false);
            m_noticeBG->setVisible(false);
        }else if(info.post == CHAT_TYPE_NOTICE){
            m_chatBG->setVisible(false);
            m_sysBG->setVisible(false);
            m_noticeBG->setVisible(true);
            m_noticeBG->setPreferredSize(CCSize(m_bgSize.width,height));
        }else{
            m_right->setVisible(false);
            m_left->setVisible(false);
            m_chatBG->setVisible(false);
            m_sysBG->setVisible(true);
            m_sysBG->setPreferredSize(CCSize(m_bgSize.width,height));
            m_noticeBG->setVisible(false);
        }
        if(info.post==CHAT_TYPE_INVITE && !GlobalData::shared()->playerInfo.isInAlliance()){
            m_joinBtn->setVisible(true);
        }
    }
    else if (m_type==CHAT_ALLIANCE){
        auto& info = ChatController::getInstance()->m_chatAlliancePool[idx];
        CCSprite* pic = NULL;
        if(info.senderPic=="" || info.senderPic=="0"){
            pic = CCLoadSprite::createSprite("hero_xiao.png");
        }else{
            std::string head = info.senderPic;
            head.append(".png");
            pic = CCLoadSprite::createSprite(head.c_str());
        }
        CCCommonUtils::setSpriteMaxSize(pic, 80);
        m_picNode->addChild(pic);
        if (showCustomPic(info.senderPicVer))
        {
            m_headImgNode->initHeadImgUrl2(m_picNode, CCCommonUtils::getCustomPicUrl(info.uid, info.senderPicVer), 1.0f, 80, true);
        }
        
        string _name = info.name;
        if (info.asn != "") {
            _name = "(" + info.asn + ") " + info.name;
        }
        if (info.vip > 0) {
            m_vipLabel->setString(_lang_1("103001", CC_ITOA(info.vip)));
            float infoX = m_vipLabel->getPositionX()+m_vipLabel->getContentSize().width*m_vipLabel->getOriginScaleX()+ 10;
            m_nameLabel->setPositionX(infoX);
        }
        m_nameLabel->setString(_name.c_str());
        if(info.gmFlag == 2 || info.gmFlag == 4 || info.gmFlag == 5)
        {
            float modX = m_nameLabel->getPositionX() + m_nameLabel->getContentSize().width +10;
            m_mod->setPosition(ccp(modX, m_nameLabel->getPositionY()));
            m_gm->setVisible(false);
            m_mod->setVisible(true);
            if(info.gmFlag == 2){
                m_mod->setDisplayFrame(CCLoadSprite::loadResource("mod.png"));
            }else if(info.gmFlag == 5){
                m_mod->setDisplayFrame(CCLoadSprite::loadResource("tmod.png"));
            }else{
                m_mod->setDisplayFrame(CCLoadSprite::loadResource("smod.png"));
            }

            m_nameLabel->setColor({65,119,41});
        }else if(info.gmFlag == 3){
            float gmX = m_nameLabel->getPositionX() + m_nameLabel->getContentSize().width + 10;
            m_gm->setPosition(ccp(gmX, m_nameLabel->getPositionY()));
            m_mod->setVisible(false);
            m_gm->setVisible(true);
            m_nameLabel->setColor({33,111,169});
        }else{
            m_nameLabel->setColor({111,26,0});
        }

//        m_timeLabel->setString(CC_ITOA(info.time) );
        
        bool flag = checkSameLangBySystem(info.originalLang);//本地语言和和 源语言一致，也不翻译
        //处理原文/译文  自己不翻译 /*&& info.originalLang != "en"cocos2d::extension::CCDevice::getLanguage()*/
        if(!flag && GlobalData::shared()->translation && info.uid!=GlobalData::shared()->playerInfo.uid && info.translationMsg!=info.msg && info.translationMsg!=""){
            m_translateNode->setVisible(true);
        }else{
            if(info.uid==GlobalData::shared()->playerInfo.uid){
                m_translateNode->setVisible(false);
            }else{
                m_translateNode->setVisible(true);
            }
        }
        string typeCache="";
        if(info.originalLang!=""){
            string typeCacheKey = "LanguageTranslation.set."+info.originalLang;
             typeCache= CCUserDefault::sharedUserDefault()->getStringForKey(typeCacheKey.c_str());
        }
        m_msgLabel->setDimensions(CCSizeMake(450, 0));
        if(info.translationMsg == "" || !GlobalData::shared()->translation||typeCache!="" || info.uid==GlobalData::shared()->playerInfo.uid || flag){
            m_translateLabel->setString(_lang("105322"));
            //CCCommonUtils::setButtonTitle(m_translateBtn, "translate");
            m_msgLabel->setString(info.msg.c_str());
            m_isOriginal = true;
        }else{
            if(info.showOriginal)
            {
                m_translateLabel->setString(_lang("105322"));
                m_msgLabel->setString(info.msg.c_str());
                m_isOriginal = true;
            }else{
                m_translateLabel->setString(_lang_1("105321", info.originalLang.c_str()));
                //CCCommonUtils::setButtonTitle(m_translateBtn, "original");
                m_msgLabel->setString(info.translationMsg.c_str());
                m_isOriginal = false;
            }
        }

        height = m_bgSize.height;
        float _tmpHeight = MAX(getTextDimension(info.translationMsg.c_str()).height,getTextDimension(info.msg.c_str()).height);
        float offsetH = 0;
        if (_tmpHeight > 50) {
            offsetH = _tmpHeight - 45;
            height +=offsetH;
            m_msgLabel->setPositionY(66+offsetH);
            m_nameLabel->setPositionY(75+offsetH);
            m_timeNode->setPositionY(124+offsetH);
            m_translateNode->setPositionY(76+offsetH);
            m_vipLabel->setPositionY(m_nameLabel->getPositionY());
        }
        
        m_uid = info.uid;
        info.isNew = false;
        if(info.isTimeShowed){
            m_timeLabel->setString(info.showedTime.c_str());
        }else if (idx ==0 ) {
            time_t infoTime = GlobalData::shared()->getTimeStamp();
            string lastTime = CCCommonUtils::timeStampToDHM(infoTime).substr(0,10);
            string nowTime = CCCommonUtils::timeStampToDHM(info.time).substr(0,10);
            if (lastTime == nowTime) {
                m_timeLabel->setString(CCCommonUtils::timeStampToHour(info.time).c_str());
                info.showedTime = CCCommonUtils::timeStampToHour(info.time);
                info.isTimeShowed = true;
            }
            else {
                m_timeLabel->setString(CCCommonUtils::timeStampToDHM(info.time).c_str());
                info.showedTime = CCCommonUtils::timeStampToDHM(info.time);
                info.isTimeShowed = true;
            }
        }
        else {
            time_t lt = ChatController::getInstance()->m_chatAlliancePool[idx-1].time;
            if ((info.time-lt) < ChatController::getInstance()->chat_time) {
                m_timeLabel->setString("");
                info.showedTime = "";
                info.isTimeShowed = true;
            }
            else {
                time_t infoTime = GlobalData::shared()->getTimeStamp();
                string lastTime = CCCommonUtils::timeStampToDHM(infoTime).substr(0,10);
                string nowTime = CCCommonUtils::timeStampToDHM(info.time).substr(0,10);
                if (lastTime == nowTime) {
                    m_timeLabel->setString(CCCommonUtils::timeStampToHour(info.time).c_str());
                    info.showedTime = CCCommonUtils::timeStampToHour(info.time);
                    info.isTimeShowed = true;
                }
                else {
                    m_timeLabel->setString(CCCommonUtils::timeStampToDHM(info.time).c_str());
                    info.showedTime = CCCommonUtils::timeStampToDHM(info.time);
                    info.isTimeShowed = true;
                }
            }
        }
        
        if (info.uid == GlobalData::shared()->playerInfo.uid) {
            m_userNode->setPositionX(510);
            m_msgNode->setPositionX(-102);
            m_left->setVisible(false);
            m_right->setVisible(true);
        }
        else {
            m_userNode->setPositionX(0);
            m_msgNode->setPositionX(0);
            m_left->setVisible(true);
            m_right->setVisible(false);
        }
        if(info.post==0){
            m_chatBG->setVisible(true);
            m_chatBG->setPreferredSize(CCSize(m_bgSize.width, height));
            m_sysBG->setVisible(false);
            m_noticeBG->setVisible(false);
        }else if(info.post!=0){
            m_right->setVisible(false);
            m_left->setVisible(false);
            m_chatBG->setVisible(false);
            m_sysBG->setVisible(true);
            m_sysBG->setPreferredSize(CCSize(m_bgSize.width, height));
            m_noticeBG->setVisible(false);
        }
        if((info.post==CHAT_TYPE_BATTLE_SHARE || info.post == CHAT_TYPE_DETECT_SHARE) && GlobalData::shared()->playerInfo.isInAlliance()){
            m_joinBtn->setVisible(true);
            CCCommonUtils::setButtonSprite(m_joinBtn, "mail_battlereport.png");
            m_joinBtn->setPreferredSize(CCSize(94, 49));
            m_joinBtn->setPosition(547.0, 49.0);
            m_msgLabel->setDimensions(CCSize(380.0,50));
            CCCommonUtils::setButtonTitle(m_joinBtn, "");
        }else{
            m_msgLabel->setDimensions(CCSize(450,0));
        }
    }else if (m_type==CHAT_COMMENT){
        CCLoadSprite::doResourceByCommonIndex(205, true);
        auto& info = ChatController::getInstance()->m_commentPool[idx];
        if (info.stype==2) {
            string banner = "Allance_flay.png";
            if(info.senderPic!=""){
                banner = info.senderPic;
                banner.append(".png");
            }
            AllianceFlagPar* flag = AllianceFlagPar::create(banner.c_str());
            flag->setScale(0.4);
            m_picNode->addChild(flag);
        }else{
            CCSprite* pic = NULL;
            if(info.senderPic=="" || info.senderPic=="0"){
                pic = CCLoadSprite::createSprite("g044.png");
            }else{
                std::string head = info.senderPic;
                head.append(".png");
                pic = CCLoadSprite::createSprite(head.c_str());
            }
            CCCommonUtils::setSpriteMaxSize(pic, 80);
            m_picNode->addChild(pic);
            if (showCustomPic(info.senderPicVer))
            {
                m_headImgNode->initHeadImgUrl2(m_picNode, CCCommonUtils::getCustomPicUrl(info.uid, info.senderPicVer), 1.0f, 80, true);
            }
        }
        string _name = info.name;
        if (info.asn != "") {
            _name = "(" + info.asn + ") " + info.name;
        }
        m_nameLabel->setString(_name.c_str());
        bool flag = checkSameLangBySystem(info.originalLang);//本地语言和和 源语言一致，也不翻译
        //处理原文/译文  自己不翻译 /*&& info.originalLang != "en"cocos2d::extension::CCDevice::getLanguage()*/
        if(!flag && GlobalData::shared()->translation && info.uid!=GlobalData::shared()->playerInfo.uid && info.translationMsg!=info.msg && info.translationMsg!=""){
            m_translateNode->setVisible(true);
        }else{
            m_translateNode->setVisible(false);
        }
        string typeCache="";
        if(info.originalLang!=""){
            string typeCacheKey = "LanguageTranslation.set."+info.originalLang;
            typeCache= CCUserDefault::sharedUserDefault()->getStringForKey(typeCacheKey.c_str());
        }
        m_msgLabel->setDimensions(CCSizeMake(450, 0));
        if(info.translationMsg == "" || !GlobalData::shared()->translation ||typeCache!="" || info.uid==GlobalData::shared()->playerInfo.uid || flag){
            m_translateLabel->setString(_lang("105322"));
            //CCCommonUtils::setButtonTitle(m_translateBtn, "translate");
            m_msgLabel->setString(info.msg.c_str());
            m_isOriginal = true;
        }else{
            if(info.showOriginal)
            {
                m_translateLabel->setString(_lang("105322"));
                m_msgLabel->setString(info.msg.c_str());
                m_isOriginal = true;
            }else{
                m_translateLabel->setString(_lang_1("105321", info.originalLang.c_str()));
                //CCCommonUtils::setButtonTitle(m_translateBtn, "original");
                m_msgLabel->setString(info.translationMsg.c_str());
                m_isOriginal = false;
            }
        }
        
        height = m_bgSize.height;
        int _tmpHeight = m_msgLabel->getContentSize().height;
        if (_tmpHeight > 50) {
            height += _tmpHeight - 45;
        }
        
        m_uid = info.uid;
        info.isNew = false;
        if(info.isTimeShowed){
            m_timeLabel->setString(info.showedTime.c_str());
        }else if (idx ==0 ) {
            time_t infoTime = GlobalData::shared()->getTimeStamp();
            string lastTime = CCCommonUtils::timeStampToDHM(infoTime).substr(0,10);
            string nowTime = CCCommonUtils::timeStampToDHM(info.time).substr(0,10);
            if (lastTime == nowTime) {
                m_timeLabel->setString(CCCommonUtils::timeStampToHour(info.time).c_str());
                info.showedTime = CCCommonUtils::timeStampToHour(info.time);
                info.isTimeShowed = true;
            }
            else {
                m_timeLabel->setString(CCCommonUtils::timeStampToDHM(info.time).c_str());
                info.showedTime = CCCommonUtils::timeStampToDHM(info.time);
                info.isTimeShowed = true;
            }
        }
        else {
            time_t lt = ChatController::getInstance()->m_commentPool[idx-1].time;
            if ((info.time-lt) < ChatController::getInstance()->chat_time) {
                m_timeLabel->setString("");
                info.showedTime = "";
                info.isTimeShowed = true;
            }
            else {
                time_t infoTime = GlobalData::shared()->getTimeStamp();
                string lastTime = CCCommonUtils::timeStampToDHM(infoTime).substr(0,10);
                string nowTime = CCCommonUtils::timeStampToDHM(info.time).substr(0,10);
                if (lastTime == nowTime) {
                    m_timeLabel->setString(CCCommonUtils::timeStampToHour(info.time).c_str());
                    info.showedTime = CCCommonUtils::timeStampToHour(info.time);
                    info.isTimeShowed = true;
                }
                else {
                    m_timeLabel->setString(CCCommonUtils::timeStampToDHM(info.time).c_str());
                    info.showedTime = CCCommonUtils::timeStampToDHM(info.time);
                    info.isTimeShowed = true;
                }
            }
        }
        
        if (GlobalData::shared()->playerInfo.isInAlliance() && info.senderAllianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
            m_userNode->setPositionX(510);
            m_msgNode->setPositionX(-102);
            m_left->setVisible(false);
            m_right->setVisible(true);
        }
        else {
            m_userNode->setPositionX(0);
            m_msgNode->setPositionX(0);
            m_left->setVisible(true);
            m_right->setVisible(false);
        }
        if(info.post==0){
            m_chatBG->setVisible(true);
            m_sysBG->setVisible(false);
            m_noticeBG->setVisible(false);
        }else if(info.post!=0){
            m_right->setVisible(false);
            m_left->setVisible(false);
            m_chatBG->setVisible(false);
            m_sysBG->setVisible(true);
            m_noticeBG->setVisible(false);
        }
    }
    
    std::string strTime = m_timeLabel->getString();
    if (height != m_bgSize.height)
    {
        float addHeight = height - m_bgSize.height;
        if (strTime!="") {
            m_timeNode->setPositionY(m_timeNodeY + addHeight);
        }
        
        m_chatBG->setContentSize(CCSizeMake(m_bgSize.width, height));
        m_sysBG->setContentSize(CCSizeMake(m_bgSize.width, height));
        m_noticeBG->setContentSize(CCSizeMake(m_bgSize.width, height));
        m_touchNode1->setContentSize(CCSizeMake(m_bgSize.width, height));
        m_nameLabel->setPositionY(m_nameLabelPt.y + addHeight);
        m_mod->setPositionY(m_nameLabelPt.y + addHeight);
        m_gm->setPositionY(m_nameLabelPt.y + addHeight);
        m_vipLabel->setPositionY(m_vipLabelPt.y + addHeight);
        m_translateNode->setPositionY(m_translatePt.y+addHeight);
//        m_touchNode3->setPositionY(m_touchNode3Y + addHeight);
        m_msgLabel->setPositionY(m_msgLabelY + addHeight);
        m_msgLabel->setDimensions(CCSizeMake(m_msgDimensions.width, m_msgDimensions.height+addHeight));
    }
    m_timeNode->setVisible(strTime!="");
}

bool ChatCell::checkSameLangBySystem(string oLang){
    bool flag = false;
    std::string sLang = LocalController::shared()->getLanguageFileName();
    if(oLang.find("zh")<oLang.length() && sLang.find("zh")<sLang.length()){
        flag = true;
    }else if(oLang==sLang || oLang==""){
        flag = true;
    }
    return flag;
}

void ChatCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
    //CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatCell::setTranslation),MSG_TRANSLATE, NULL);
}

void ChatCell::onExit(){
    setTouchEnabled(false);
    //CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TRANSLATE);
    CCNode::onExit();
}

void ChatCell::onShieldClick(cocos2d::CCObject *pTarget, CCControlEvent touchEvent){
    if(GlobalData::shared()->chatShieldInfo.isReachLimit()){
        CCCommonUtils::flyText(_lang("105314"));
        return;
    }
  
//    if(GlobalData::shared()->chatShieldInfo.isShield(_uid)){
//        return;
//    }
    YesNoDialog::show(_lang("105313").c_str(), CCCallFunc::create(this, callfunc_selector(ChatCell::onYesClick)));
}

void ChatCell::onYesClick(){
//    ChatLockCommand* cmd = new ChatLockCommand(_uid);
//    cmd->sendAndRelease();
}

SEL_CCControlHandler ChatCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
     //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTranslateClick", ChatCell::onTranslateClick);
    return NULL;
}

bool ChatCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_userNode", CCNode*, this->m_userNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgNode", CCNode*, this->m_msgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIFTTF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIFTTF*, this->m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLabel", CCLabelIFBMFont*, this->m_vipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIFTTF*, this->m_timeLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode1", CCNode*, this->m_touchNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode2", CCNode*, this->m_touchNode2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode3", CCNode*, this->m_touchNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeNode", CCNode*, this->m_timeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_left", CCSprite*, m_left);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_right", CCSprite*, m_right);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_chatBG", CCScale9Sprite*, m_chatBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sysBG", CCScale9Sprite*, m_sysBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translateNode", CCNode*, this->m_translateNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translateLabel", CCLabelIF*, this->m_translateLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translateBtn", CCControlButton*, this->m_translateBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_langNode", CCNode*, this->m_langNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_joinBtn", CCControlButton*, this->m_joinBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeBG", CCScale9Sprite*, m_noticeBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeleft", CCSprite*, m_noticeleft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeright", CCSprite*, m_noticeright);
    
    return false;
}

void ChatCell::onTranslateClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_type == CHAT_ALLIANCE){
        auto& info = ChatController::getInstance()->m_chatAlliancePool[m_idx];
        if(m_isOriginal){
            if(info.translationMsg != ""){
                m_msgLabel->setString(info.translationMsg.c_str());
                m_translateLabel->setString(_lang_1("105321", info.originalLang.c_str()));
                m_isOriginal = false;
                info.showOriginal = false;
            }else{
                ChatController::getInstance()->addLocalTranslate(&info);
                info.showOriginal=true;
                //调后台
//                TranslateCommand* cmd = new TranslateCommand(info.msg);
//                cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChatCell::setTranslation), NULL));
//                cmd->sendAndRelease();
            }
        }else{
            if(info.msg != ""){
                m_msgLabel->setString(info.msg.c_str());
            }else{
                //调后台
            }
            m_translateLabel->setString(_lang("105322"));
            m_isOriginal = true;
            info.showOriginal=true;
        }
        
    }else if(m_type == CHAT_COUNTRY){
        auto& info = ChatController::getInstance()->m_chatCountryPool[m_idx];
        if(m_isOriginal){
            if(info.translationMsg != ""){
                m_msgLabel->setString(info.translationMsg.c_str());
                m_translateLabel->setString(_lang_1("105321", info.originalLang.c_str()));
                m_isOriginal = false;
                info.showOriginal = false;
            }else{
                ChatController::getInstance()->addLocalTranslate(&info);
                info.showOriginal = true;
                //调后台
//                TranslateCommand* cmd = new TranslateCommand(info.msg);
//                cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChatCell::setTranslation), NULL));
//                cmd->sendAndRelease();
            }
        }else{
            if(info.msg != ""){
                m_msgLabel->setString(info.msg.c_str());
            }else{
                    //调后台
            }
            m_translateLabel->setString(_lang("105322"));
            m_isOriginal = true;
            info.showOriginal = true;
        }
        
    }else if(m_type == CHAT_COMMENT){
        auto& info = ChatController::getInstance()->m_commentPool[m_idx];
        if(m_isOriginal){
            if(info.translationMsg != ""){
                m_msgLabel->setString(info.translationMsg.c_str());
            }else{
                //调后台
                TranslateCommand* cmd = new TranslateCommand(info.msg);
                cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChatCell::setTranslation), NULL));
                cmd->sendAndRelease();
            }
            m_translateLabel->setString(_lang_1("105321", info.originalLang.c_str()));
            m_isOriginal = false;
        }else{
            if(info.msg != ""){
                m_msgLabel->setString(info.msg.c_str());
            }else{
                //调后台
            }
            m_translateLabel->setString(_lang("105322"));
            m_isOriginal = true;
        }
        info.showOriginal = m_isOriginal;
    }

}

void ChatCell::setTranslation(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic && dic->objectForKey("translationMsg")){
        string msg = dic->valueForKey("translationMsg")->getCString();
        m_msgLabel->setString(msg.c_str());
        if(m_type==CHAT_ALLIANCE){
            auto& info = ChatController::getInstance()->m_chatAlliancePool[m_idx];
            ChatController::getInstance()->m_chatAlliancePool[m_idx].translationMsg = msg;
            if(info.originalLang.c_str()){
                m_translateLabel->setString(_lang_1("105321", info.originalLang.c_str()));
            }
        }else if(m_type==CHAT_COMMENT){
            auto& info = ChatController::getInstance()->m_commentPool[m_idx];
            ChatController::getInstance()->m_commentPool[m_idx].translationMsg = msg;
            if(info.originalLang.c_str()){
                m_translateLabel->setString(_lang_1("105321", info.originalLang.c_str()));
            }
        }else
        {
            auto& info = ChatController::getInstance()->m_chatCountryPool[m_idx];
            ChatController::getInstance()->m_chatCountryPool[m_idx].translationMsg = msg;
            if(info.originalLang.c_str()){
                m_translateLabel->setString(_lang_1("105321", info.originalLang.c_str()));
            }
        }
    }
}

bool ChatCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(m_translateBtn && isTouchInside(m_translateBtn, pTouch)){
         m_translateBtn->setHighlighted(true);
    }
    m_touchPoint = pTouch->getLocation();
    if(isTouchInside(m_touchNode1, pTouch) || isTouchInside(m_touchNode2, pTouch)) {
        return true;
    }
    return false;
}

void ChatCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_translateBtn->setHighlighted(false);
    if (fabs( pTouch->getLocation().y - m_touchPoint.y) > 30) {
        return;
    }else if(isTouchInside(m_langNode, pTouch) && m_translateNode->isVisible()){
        onTranslateClick(NULL,Control::EventType::TOUCH_DOWN);
    }
//    if (isTouchInside(m_touchNode3, pTouch)) {
//        //复制成功
//        cocos2d::extension::CCDevice::clipboardSetText(m_msgLabel->getString());
//    }
    else if (isTouchInside(m_touchNode1, pTouch) || isTouchInside(m_touchNode2, pTouch) ) {
		if(m_prevPopupCount == -1){
			m_prevPopupCount = PopupViewController::getInstance()->getCurrViewCount();
		}
        if (isTouchInside(m_joinBtn, pTouch) && m_joinBtn->isVisible() && m_type==CHAT_COUNTRY){
            ApplyAllianceCommand* cmd = new ApplyAllianceCommand(ChatController::getInstance()->m_chatCountryPool[m_idx].allianceId);
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatCell::joinSuccess), NULL));
            cmd->sendAndRelease();
            GameController::getInstance()->showWaitInterface();
            return ;
        }else if (isTouchInside(m_joinBtn, pTouch) && m_joinBtn->isVisible() && m_type==CHAT_ALLIANCE){
            auto& info = ChatController::getInstance()->m_chatAlliancePool[m_idx];
            if (info.post == CHAT_TYPE_BATTLE_SHARE) {
                CCLOG("share mail");//reportUid
                string uid = ChatController::getInstance()->m_chatAlliancePool[m_idx].reportUid;
                MailReadCommand* cmd = new MailReadCommand(uid, CC_ITOA(MAIL_BATTLE_REPORT));
                cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChatCell::readMailSuccess), NULL));
                cmd->sendAndRelease();
                GameController::getInstance()->showWaitInterface();
                return ;
            } else if (info.post == CHAT_TYPE_DETECT_SHARE) {
                CCLOG("detect share mail");//scout
                CCLoadSprite::doResourceByCommonIndex(6, true);
                setCleanFunction([](){
                    CCLoadSprite::doResourceByCommonIndex(6, false);
                });
                string uid = ChatController::getInstance()->m_chatAlliancePool[m_idx].detectRepUid;
                MailReadCommand* cmd = new MailReadCommand(uid, CC_ITOA(MAIL_DETECT_REPORT));
                cmd->setFromChat();
                cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChatCell::readDetectRepMailSuccess), NULL));
                cmd->sendAndRelease();
                return;
            }
        }
		if(m_type==CHAT_COUNTRY){
			//检查是否有联盟
			if(!m_isCheckingInAllance && m_prevPopupCount == PopupViewController::getInstance()->getCurrViewCount()){
				ChatController::getInstance()->idx = m_idx;
				ChatIsInAllanceCommand* cmd = new ChatIsInAllanceCommand(ChatController::getInstance()->m_chatCountryPool[m_idx].uid);
				cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatCell::openChatFun), NULL));
				cmd->sendAndRelease();
				m_isCheckingInAllance = true;
			}
        }else if(m_type==CHAT_COMMENT && ChatController::getInstance()->m_commentPool.size()>m_idx){
            ChatController::getInstance()->idx = m_idx;
            if(ChatController::getInstance()->m_commentPool[m_idx].uid != GlobalData::shared()->playerInfo.uid && GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.uid == m_allianceId){
                PopupViewController::getInstance()->addPopupView(ChatFunView::create(ChatController::getInstance()->idx, CHAT_COMMENT), false);
            }
        }else{
			if(m_prevPopupCount == PopupViewController::getInstance()->getCurrViewCount()){
				PopupViewController::getInstance()->addPopupView(ChatFunView::create(m_idx, m_type), false);
			}
        }
        CCLOG("call0:idx:%d  type:%d",m_idx,m_type);
    }
}

void ChatCell::readMailSuccess(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    if(result==NULL) return;
    auto dictInfo = _dict(result->getData());
    CCArray* arr = NULL;
    if(dictInfo && dictInfo->objectForKey("result")){
        arr = dynamic_cast<CCArray*>(dictInfo->objectForKey("result"));
    }
    if(arr && arr->count()>0){
        auto dic = _dict(arr->objectAtIndex(0));
        MailInfo* m_mailInfo = MailInfo::create();
        m_mailInfo->uid = ChatController::getInstance()->m_chatAlliancePool[m_idx].reportUid;
        m_mailInfo->type = MAIL_BATTLE_REPORT;
        m_mailInfo->isReadContent = true;
        m_mailInfo->shareUid = ChatController::getInstance()->m_chatAlliancePool[m_idx].uid;
        m_mailInfo->parseBattleMail(dic);
        PopupViewController::getInstance()->addPopupInViewWithAnim(BattleReportMailPopUpView::create(m_mailInfo));
    }else{
        CCCommonUtils::flyHint("", "", _lang("115282"));
    }
}

void ChatCell::readDetectRepMailSuccess(CCObject* param) {
    NetResult* result = dynamic_cast<NetResult*>(param);
    if(result==NULL) return;
    auto dictInfo = _dict(result->getData());
    CCArray* arr = NULL;
    if(dictInfo && dictInfo->objectForKey("result")){
        arr = dynamic_cast<CCArray*>(dictInfo->objectForKey("result"));
    }
    if(arr && arr->count()>0){
        auto dic = _dict(arr->objectAtIndex(0));
        MailInfo* m_mailInfo = MailInfo::create();
        m_mailInfo->uid = ChatController::getInstance()->m_chatAlliancePool[m_idx].detectRepUid;
        m_mailInfo->type = MAIL_DETECT_REPORT;
        m_mailInfo->fromName = _lang("105522");
        m_mailInfo->isReadContent = true;
        m_mailInfo->detectReport = dic;
        m_mailInfo->detectReport->retain();
        CCString *str = dynamic_cast<CCString*>(dic->objectForKey("pointType"));
        if(str && str->getCString()){
            m_mailInfo->pointType =  str->intValue();
        }
        m_mailInfo->isFromChat = true;
        m_mailInfo->createTime = dic->valueForKey("createTime")->doubleValue() / 1000;
        m_mailInfo->retain();
        PopupViewController::getInstance()->addPopupInViewWithAnim(DetectMailPopUpView::create(m_mailInfo));
    }else{
        CCCommonUtils::flyHint("", "", _lang("115299"));
    }
}

void ChatCell::joinSuccess(CCObject* p){
    m_joinBtn->setVisible(false);
}

void ChatCell::openChatFun(CCObject* p){
    PopupViewController::getInstance()->addPopupView(ChatFunView::create(ChatController::getInstance()->idx, CHAT_COUNTRY), false);
    m_isCheckingInAllance = false;
}
void ChatCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(m_translateBtn && isTouchInside(m_translateBtn, pTouch)){
        m_translateBtn->setHighlighted(true);
    }
}

bool ChatCell::showCustomPic(int senderPicVer)
{
    bool ret = false;
    if (senderPicVer <= 999999 && senderPicVer >=1)
    {
        return true;
    }
    else
    {
        return false;
    }
    return ret;
}
CCSize ChatCell::getTextDimension(const char *text)
{
    int len = (int)strlen(text);
    if ((len==0)  ||  (len==1 && text[0] == '\n')) {
        CCLabelIFTTF *label = CCLabelIFTTF::create( "sample" , "Helvetica", 20 );
        label->setDimensions(CCSizeMake(450, 0));
        CCSize size = label->getContentSize();
        size.width = 0;
        return size;
    }
    CCLabelIFTTF *label = CCLabelIFTTF::create( text, "Helvetica", 20);
    label->setDimensions(CCSizeMake(450, 0));
    return label->getContentSize();
}
void ChatCell::initContents(string info, int lastWidth, int maxNum, int maxWidth)
{
//    int curX = 0;
//    int curY = 0;
//    int heightPreLine = 30;
//    int limitWidth = lastWidth;
//    int numPreLine = lastWidth*maxNum/maxWidth;
//    m_infoNode = CCNode::create();
//    
//    std::vector<std::string> infoItems;
//    CCCommonUtils::splitString(info, "|", infoItems);
//    for(int i=0; i<infoItems.size(); i++)
//    {
//        string tmpInfo = infoItems[i];
//        std::vector<std::string> cellItems;
//        CCCommonUtils::splitString(tmpInfo, "#", cellItems);
//        
//        auto color = ccWHITE;
//        if (cellItems.size()>=2) {
//            color = CCCommonUtils::getItemColor(atoi(cellItems[1].c_str()));
//        }
//        
//        int startPos = 0;
//        do {
//            
//            int curCanWriteNum = (limitWidth-curX)*numPreLine/limitWidth;
//            if ((cellItems[0].length()-startPos)<curCanWriteNum)
//            {
//                string tmpStr = cellItems[0].substr(startPos, cellItems[0].length()-startPos);
//                auto autoLabel = CCLabelIF::create(tmpStr.c_str(), 20, CCSizeMake(0, heightPreLine), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
//                autoLabel->setColor(color);
//                autoLabel->setAnchorPoint(ccp(0,1));
//                int curLabelwidth = autoLabel->getContentSize().width*autoLabel->getOriginScaleX();
//                autoLabel->setPosition(ccp(curX,curY));
//                m_infoNode->addChild(autoLabel);
//                curX = curX + curLabelwidth;
//                
//                break;
//            }
//            else
//            {
//                string tmpStr1 = cellItems[0].substr(startPos, curCanWriteNum);
//                string::size_type pos = tmpStr1.rfind(" ");
//                if (pos != string::npos && pos!=0) {
//                    //找到
//                    string subWriteStr = tmpStr1.substr(0,pos);
//                    startPos += pos+1;
//                    
//                    auto autoLabel = CCLabelIF::create(subWriteStr.c_str(), 20, CCSizeMake(0, heightPreLine), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
//                    autoLabel->setColor(color);
//                    autoLabel->setAnchorPoint(ccp(0,1));
//                    autoLabel->setPosition(ccp(curX,curY));
//                    m_infoNode->addChild(autoLabel);
//                }
//                else
//                {
//                    //没找到
//                }
//                curX = 0;
//                curY -= heightPreLine;
//            }
//            
//        } while (1);
//        
//    }
//    
//    this->setContentSize(CCSize(720,-(curY-heightPreLine/2)));
}
