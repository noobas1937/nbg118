//
//  AllianceScienceDonateView.cpp
//  IF
//
//  Created by fubin on 14-4-9.
//
//

#include "AllianceScienceDonateView.h"
#include "YesNoDialog.h"
#include "PopupViewController.h"
#include "QueueController.h"
#include "ScienceController.h"
#include "FunBuildController.h"
#include "ParticleController.h"
#include "FunBuildView.h"
#include "UseResToolView.h"
#include "GoldExchangeView.h"

AllianceScienceDonateView* AllianceScienceDonateView::create(int scienceId) {
    auto ret = new AllianceScienceDonateView();
    if (ret && ret->init(scienceId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceScienceDonateView::init(int scienceId) {
    bool ret = false;
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        setModelLayerTouchPriority(Touch_Popup_Item);
        m_scienceId = scienceId;
        CCBLoadFile("AllianceScienceDonateView",this,this);//SciencePopupView
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        auto& info = ScienceController::getInstance()->allianceScienceMap[m_scienceId];
        m_nameLabel->setString(_lang("115801"));
        auto icon = CCLoadSprite::createSprite((info.icon+".png").c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 130, true);
        m_iconNode->removeAllChildren();
        m_iconNode->addChild(icon);
        
        m_noticeLabel->setString(_lang("115805"));
        m_get1Label->setString(_lang("102148"));
        m_get2Label->setString(_lang("102148"));
        m_get3Label->setString(_lang("102148"));
        
        float dx=80.0;
        if (m_get1Label->getContentSize().width>dx) {
            float sx=dx/m_get1Label->getContentSize().width;
            m_get1Label->setScaleX(sx);
        }
        if (m_get2Label->getContentSize().width>dx) {
            float sx=dx/m_get2Label->getContentSize().width;
            m_get2Label->setScaleX(sx);
        }
        if (m_get3Label->getContentSize().width>dx) {
            float sx=dx/m_get3Label->getContentSize().width;
            m_get3Label->setScaleX(sx);
        }
        
        if ((info.finishTime> GlobalData::shared()->getWorldTime()) || info.level==info.maxLv
            || (info.curStar==info.sumStars && info.currentPro>=info.needPro))
        {
            m_shortNode->setVisible(true);
            m_longNode->setVisible(false);
            m_nameLabel->setString(_lang(info.name));
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_mainNode->setPositionY(m_mainNode->getPositionY() - 224);
            } else {
                m_mainNode->setPositionY(m_mainNode->getPositionY()-150);
            }
        }
        else {
            m_shortNode->setVisible(false);
            m_longNode->setVisible(true);
        }
        
        m_curDonate = info.currentPro;
        m_sumDonate = info.needPro;
        m_endDonate = info.currentPro;
        m_preDonate = 1;
        
        string destip = CCCommonUtils::getPropById(CC_ITOA(info.scienceId), "destip");
        m_desLabel->setString(_lang(info.des));
        m_curLabel->setString(_lang("115803"));
        if (!CCCommonUtils::isIosAndroidPad())
        {
            m_curLabel->setMaxScaleXByWidth(200.0);
        } else {
            float fw = m_curLabel->getContentSize().width;
            if (fw>362.0) {//防爆框
                m_curLabel->setScaleX(362.0/fw);
            }
        }
        
        m_curNumLabel->setString(_lang_1(destip.c_str(), info.para2.c_str()));
        if (!CCCommonUtils::isIosAndroidPad())
        {
            m_curNumLabel->setMaxScaleXByWidth(55.0);
        } else {
            float fw = m_curNumLabel->getContentSize().width;
            if (fw>110.0) {//防爆框
                m_curNumLabel->setScaleX(110.0/fw);
            }
        }
        
        m_nextLabel->setString(_lang("115804"));
        if (!CCCommonUtils::isIosAndroidPad())
        {
            m_nextLabel->setMaxScaleXByWidth(200.0);
        } else {
            float fw = m_nextLabel->getContentSize().width;
            if (fw>362) {//防爆框
                m_nextLabel->setScaleX(362.0/fw);
            }
        }
        
        m_nextNumLabel->setString(_lang_1(destip.c_str(), info.nextPara2.c_str()));
        if (!CCCommonUtils::isIosAndroidPad())
        {
            m_nextNumLabel->setMaxScaleXByWidth(55.0);
        } else {
            float fw = m_nextNumLabel->getContentSize().width;
            if (fw>110.0) {//防爆框
                m_nextNumLabel->setScaleX(110.0/fw);
            }
        }
        
        
        m_starBar = CCStarBar::createStarBar(info.sumStars);
        m_starNode->addChild(m_starBar);
        
        m_clipNode = CCClipNode::create(CCSize(500,60));
        m_clipNode->setAnchorPoint(ccp(0,0));
        m_clipBarNode->addChild(m_clipNode);
        auto bar = CCLoadSprite::createSprite("VIP_jingdutiao.png");
        bar->setAnchorPoint(ccp(0, 0));
        bar->setPosition(ccp(0, 0));
        bar->setScaleX(0.8);
        m_clipNode->addChild(bar);
        auto bar1 = CCLoadSprite::createSprite("VIP_jingdutiao.png");
        bar1->setFlipX(true);
        bar1->setAnchorPoint(ccp(0, 0));
        bar1->setPosition(ccp(216, 0));
        bar1->setScaleX(0.8);
        m_clipNode->addChild(bar1);
        
        updateInfo(NULL);
        
        if (info.level == info.maxLv) {
            m_nextLabel->setString("");
            m_nextNumLabel->setString("");
            m_starBar->setProgress(info.sumStars, 1,true);
        }
        
        ret = true;
    }
    return ret;
}

void AllianceScienceDonateView::updateInfo(CCObject* obj)
{
    onRefreshData(NULL);
}

void AllianceScienceDonateView::onEnter() {
  
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceScienceDonateView::onRefreshData), MSG_SCIENCE_DONATE, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(AllianceScienceDonateView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    PopupBaseView::onEnter();
}

void AllianceScienceDonateView::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_DONATE);
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(AllianceScienceDonateView::onEnterFrame), this);
    PopupBaseView::onExit();
}

bool AllianceScienceDonateView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_longNode->isVisible()) {
        if (m_selNode1->isVisible() && isTouchInside(m_touch1Node, pTouch)) {
//            m_btn1->onTouchBegan(pTouch, pEvent);
        }
        else if (m_selNode2->isVisible() && isTouchInside(m_touch2Node, pTouch)) {
//            m_btn2->onTouchBegan(pTouch, pEvent);
        }
        else if (m_selNode3->isVisible() && isTouchInside(m_touch3Node, pTouch)) {
//            m_btn3->onTouchBegan(pTouch, pEvent);
        }
    }
    return true;
}

void AllianceScienceDonateView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_longNode->isVisible()) {
        if (m_selNode1->isVisible() && isTouchInside(m_touch1Node, pTouch)) {
//            m_btn1->onTouchEnded(pTouch, pEvent);
//            onSendDonate(1);
            return;
        }
        else if (m_selNode2->isVisible() && isTouchInside(m_touch2Node, pTouch)) {
//            m_btn2->onTouchEnded(pTouch, pEvent);
//            onSendDonate(2);
            return;
        }
        else if (m_selNode3->isVisible() && isTouchInside(m_touch3Node, pTouch)) {
//            m_btn3->onTouchEnded(pTouch, pEvent);
//            onSendDonate(3);
            return;
        }
        else if (isTouchInside(m_longTouchNode, pTouch)) {
            return;
        }
    }
    if (m_shortNode->isVisible() && isTouchInside(m_shortTouchNode, pTouch)) {
        return;
    }
    
    PopupViewController::getInstance()->removePopupView(this);
}

void AllianceScienceDonateView::onClickBtn1(CCObject * pSender, Control::EventType pCCControlEvent){
    onSendDonate(1);
}
void AllianceScienceDonateView::onClickBtn2(CCObject * pSender, Control::EventType pCCControlEvent){
    onSendDonate(2);
}
void AllianceScienceDonateView::onClickBtn3(CCObject * pSender, Control::EventType pCCControlEvent){
    onSendDonate(3);
}

SEL_CCControlHandler AllianceScienceDonateView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn1", AllianceScienceDonateView::onClickBtn1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn2", AllianceScienceDonateView::onClickBtn2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn3", AllianceScienceDonateView::onClickBtn3);
    return NULL;
}

bool AllianceScienceDonateView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, m_bar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resLabel", CCLabelIF*, m_resLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_parNode", CCNode*, m_parNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touch1Node", CCNode*, m_touch1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resIcon1", CCNode*, m_resIcon1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touch2Node", CCNode*, m_touch2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resIcon2", CCNode*, m_resIcon2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touch3Node", CCNode*, m_touch3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resIcon3", CCNode*, m_resIcon3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_donate1Label", CCLabelIF*, m_donate1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_con1Label", CCLabelIF*, m_con1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pro1Label", CCLabelIF*, m_pro1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_donate2Label", CCLabelIF*, m_donate2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_con2Label", CCLabelIF*, m_con2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pro2Label", CCLabelIF*, m_pro2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_donate3Label", CCLabelIF*, m_donate3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_con3Label", CCLabelIF*, m_con3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pro3Label", CCLabelIF*, m_pro3Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resIcon23", CCSprite*, m_resIcon23);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resIcon21", CCSprite*, m_resIcon21);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resIcon22", CCSprite*, m_resIcon22);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_get1Label", CCLabelIF*, m_get1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_get2Label", CCLabelIF*, m_get2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_get3Label", CCLabelIF*, m_get3Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barNode", CCNode*, m_barNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barLabel", CCLabelIF*, m_barLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selNode1", CCNode*, m_selNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selNode2", CCNode*, m_selNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selNode3", CCNode*, m_selNode3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curLabel", CCLabelIF*, m_curLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curNumLabel", CCLabelIF*, m_curNumLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLabel", CCLabelIF*, m_nextLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextNumLabel", CCLabelIF*, m_nextNumLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starNode", CCNode*, m_starNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clipBarNode", CCNode*, m_clipBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jianTouSpr", CCSprite*, m_jianTouSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coolNode", CCNode*, m_coolNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coolTimeLabel", CCLabelIF*, m_coolTimeLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shortNode", CCNode*, m_shortNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_longNode", CCNode*, m_longNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeLabel", CCLabelIF*, m_noticeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shortTouchNode", CCNode*, m_shortTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_longTouchNode", CCNode*, m_longTouchNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1", CCControlButton*, m_btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2", CCControlButton*, m_btn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3", CCControlButton*, m_btn3);
    return false;
}

void AllianceScienceDonateView::onEnterFrame(float dt)
{
    int curTime = GlobalData::shared()->getWorldTime();
    int lastTime = GlobalData::shared()->playerInfo.allianceInfo.donateCDTime - curTime;
    if (lastTime>0) {
        m_coolNode->setVisible(true);
        m_coolTimeLabel->setString(CC_SECTOA(lastTime));
        if(lastTime >= GlobalData::shared()->alliance_science_k6 || GlobalData::shared()->playerInfo.allianceInfo.donateAvailable==0) {
            m_coolTimeLabel->setColor(ccRED);
        }
    }
    else {
        m_coolTimeLabel->setColor(ccc3(34, 252, 255));
        GlobalData::shared()->playerInfo.allianceInfo.donateAvailable = 1;
        m_coolNode->setVisible(false);
    }
}

void AllianceScienceDonateView::onRefreshData(CCObject* obj)
{
    if(!m_canDonate) {
        return;
    }
    m_donate1Label->setColor(ccc3(169, 149, 101));
    m_donate2Label->setColor(ccc3(169, 149, 101));
    m_donate3Label->setColor(ccc3(169, 149, 101));
    m_btn1->setEnabled(true);
    m_btn2->setEnabled(true);
    m_btn3->setEnabled(true);
    
    auto& info = ScienceController::getInstance()->allianceScienceMap[m_scienceId];
    m_donate1Label->setString(CC_CMDITOA(info.lowDonateNum));
    m_con1Label->setString(CC_ITOA(info.lowContribute));
    m_pro1Label->setString(CC_CMDITOA(info.lowProgress));
    m_resIcon1->removeAllChildren();
    auto icon1 = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(info.lowRes).c_str());
    m_resIcon1->addChild(icon1);
    if (!CCCommonUtils::isEnoughResourceByType(info.lowRes, info.lowDonateNum)) {
        m_donate1Label->setColor(ccRED);
    }
    
    m_donate2Label->setString(CC_CMDITOA(info.midDonateNum));
    m_con2Label->setString(CC_ITOA(info.midContribute));
    m_pro2Label->setString(CC_CMDITOA(info.midProgress));
    m_resIcon2->removeAllChildren();
    auto icon2 = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(info.midRes).c_str());
    m_resIcon2->addChild(icon2);
    
    m_donate3Label->setString(CC_CMDITOA(info.highDonateNum));
    m_con3Label->setString(CC_ITOA(info.highContribute));
    m_pro3Label->setString(CC_CMDITOA(info.highProgress));
    m_resIcon3->removeAllChildren();
    auto icon3 = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(info.highRes).c_str());
    m_resIcon3->addChild(icon3);

    
    int tmpWidth = info.currentPro*1.0*436/info.needPro;
    m_clipNode->setContentSize(CCSize(tmpWidth,60));
    if (!CCCommonUtils::isIosAndroidPad())
    {
        m_jianTouSpr->setPositionX(-145+(info.curStar-1)*65);
    } else {
        m_jianTouSpr->setPositionX(-310 + (info.curStar - 1) * 130);
    }
    m_barLabel->setString(CC_CMDITOA(info.currentPro)+"/"+CC_CMDITOA(info.needPro));
    
    if (info.level != info.maxLv) {
        if(info.currentPro >= info.needPro && info.curStar == info.sumStars)
            m_starBar->setProgress(info.curStar, info.currentPro,true);
        else
            m_starBar->setProgress(info.curStar, info.currentPro);
    }
    
//    m_selNode1->setPositionY(-110);
//    m_selNode2->setPositionY(-200);
//    m_selNode3->setPositionY(-290);
//    m_selNode1->setVisible(false);
//    m_selNode2->setVisible(false);
//    m_selNode3->setVisible(false);
    
    m_btn2->setEnabled(true);
    m_donate2Label->setColor({169,149,101});
    m_pro2Label->setColor({169,149,101});
    m_resIcon22->setColor(ccWHITE);
    icon2->setColor(ccWHITE);
    
    m_btn3->setEnabled(true);
    m_donate3Label->setColor({169,149,101});
    m_pro3Label->setColor({169,149,101});
    m_resIcon23->setColor(ccWHITE);
    icon3->setColor(ccWHITE);
    if (!CCCommonUtils::isEnoughResourceByType(info.midRes, info.midDonateNum)) {
        m_donate2Label->setColor(ccRED);
    }
    if (!CCCommonUtils::isEnoughResourceByType(info.highRes, info.highDonateNum)) {
        m_donate3Label->setColor(ccRED);
    }
    if (info.midAvailable ==0 && info.highAvailable ==0) {//只显示1
        m_selNode1->setVisible(true);
       // m_selNode1->setPositionY(-200);
        m_btn2->setEnabled(false);
        m_donate2Label->setColor(ccGRAY);
        m_donate2Label->setString(_lang("115831"));

        //m_pro2Label->setColor(ccGRAY);
        //m_resIcon22->setColor(ccGRAY);
        icon2->setColor(ccGRAY);
        icon2->setVisible(false);
        m_btn3->setEnabled(false);
        m_donate3Label->setColor(ccGRAY);
        m_donate3Label->setString(_lang("115831"));
       // m_pro3Label->setColor(ccGRAY);
        //m_resIcon23->setColor(ccGRAY);
        icon3->setColor(ccGRAY);
        icon3->setVisible(false);
    }
    else if (info.midAvailable ==0 && info.highAvailable ==1) {//显示1，3
        m_selNode1->setVisible(true);
       // m_selNode1->setPositionY(-155);
        m_selNode3->setVisible(true);
        m_btn2->setEnabled(false);
       // m_selNode3->setPositionY(-245);
        m_btn2->setEnabled(false);
        m_donate2Label->setColor(ccGRAY);
        m_donate2Label->setString(_lang("115831"));
        //m_pro2Label->setColor(ccGRAY);
       // m_resIcon22->setColor(ccGRAY);
        icon2->setColor(ccGRAY);
        icon2->setVisible(false);
    }
    else if (info.midAvailable ==1 && info.highAvailable ==0) {//显示1，2
        m_selNode1->setVisible(true);
       // m_selNode1->setPositionY(-155);
        m_selNode2->setVisible(true);
      //  m_selNode2->setPositionY(-245);
        m_btn3->setEnabled(false);
        m_btn3->setEnabled(false);
        m_donate3Label->setColor(ccGRAY);
        m_donate3Label->setString(_lang("115831"));
       // m_pro3Label->setColor(ccGRAY);
       // m_resIcon23->setColor(ccGRAY);
        icon3->setColor(ccGRAY);
        icon3->setVisible(false);
    }
    else {
        m_selNode1->setVisible(true);
        m_selNode2->setVisible(true);
        m_selNode3->setVisible(true);
    }
    onEnterFrame(0);
}

void AllianceScienceDonateView::onCostCDDonate()
{
    int lastTime = GlobalData::shared()->playerInfo.allianceInfo.donateCDTime - GlobalData::shared()->getWorldTime();
    int gold = MAX(lastTime/GlobalData::shared()->alliance_science_k4, 1);
    if (gold <= GlobalData::shared()->playerInfo.gold) {
        ScienceController::getInstance()->costCDDonate();
    }
    else {
        YesNoDialog::gotoPayTips();
    }
}

void AllianceScienceDonateView::onSendDonate(int type)
{
    if(!m_canDonate) {
        return;
    }
    int joinTime = GlobalData::shared()->getWorldTime()-GlobalData::shared()->playerInfo.allianceInfo.joinTime;
    if (joinTime < GlobalData::shared()->alliance_science_k3&&(GlobalData::shared()->playerInfo.isfirstJoin==0)) {//加入联盟的时间小于24小时，不能捐献
        CCCommonUtils::flyHint("", "", _lang("115164"));
        return;
    }
    
    if (GlobalData::shared()->playerInfo.allianceInfo.donateAvailable==0) {//不可以捐
        int lastTime = GlobalData::shared()->playerInfo.allianceInfo.donateCDTime - GlobalData::shared()->getWorldTime();
        int gold = MAX(lastTime/GlobalData::shared()->alliance_science_k4, 1);
        YesNoDialog::showButtonAndGold( _lang("115802").c_str() , CCCallFunc::create(this, callfunc_selector(AllianceScienceDonateView::onCostCDDonate)),_lang("115809").c_str(), gold);
        return;
    }
    
    auto& info = ScienceController::getInstance()->allianceScienceMap[m_scienceId];
    int donateNum = 0;
    int resType = -1;
    int pro = 0;
    int contrb = 0;
    if (type == 1) {
        donateNum = info.lowDonateNum;
        resType = info.lowRes;
        pro = info.lowProgress;
        contrb = info.lowContribute;
    }
    else if (type == 2) {
        donateNum = info.midDonateNum;
        resType = info.midRes;
        pro = info.midProgress;
        contrb = info.midContribute;
    }
    else if (type == 3) {
        donateNum = info.highDonateNum;
        resType = info.highRes;
        pro = info.highProgress;
        contrb = info.highContribute;
    }
    
    m_curDonate = info.currentPro;
    m_endDonate = m_curDonate + pro;
    m_sumDonate = info.needPro;
    m_preDonate = MAX(ceil(pro*1.0/10), 1);
    
    if (donateNum <= 0) {
        return;
    }
    else {
        if ( CCCommonUtils::isEnoughResourceByType(resType, donateNum) ) {
            m_btn1->setEnabled(false);
            m_btn2->setEnabled(false);
            m_btn3->setEnabled(false);
            
            onPlayDonate();
            ScienceController::getInstance()->startDonate(m_scienceId, type);
            if(type==1){
                CCCommonUtils::flyHint("Contribution_icon.png", "", _lang_2("115808", CC_ITOA(contrb),CC_ITOA(contrb)),3,100);
            }else
                CCCommonUtils::flyHint("Contribution_icon.png", "", _lang_2("115808", CC_ITOA(contrb),CC_ITOA(contrb)),3,250);
            if (info.currentPro+pro >= info.needPro) {//捐满了
                PopupViewController::getInstance()->removePopupView(this);
            }
        }
        else {
//            CCCommonUtils::flyHint("", "", _lang("115163"));
            if (resType == Wood || resType == Food || resType == Stone || resType == Iron) {
                PopupViewController::getInstance()->removePopupView(this);
                PopupViewController::getInstance()->addPopupInView(UseResToolView::create(resType));
            } else if (resType == Gold) {
                PopupViewController::getInstance()->removeAllPopupView();
//                PopupViewController::getInstance()->addPopupView(GoldExchangeView::create(),false);
                YesNoDialog::gotoPayTips();
            } else {
                CCCommonUtils::flyHint("", "", _lang("115163"));
            }
        }
    }
}

void AllianceScienceDonateView::onPlayDonate()
{
    m_canDonate = false;
//    m_barNode->setScale(0);
//    m_barNode->setVisible(true);
//    CCActionInterval * scaleTo = CCScaleTo::create(0.5, 1);
//    CCActionInterval *easeBackOut = CCEaseBackOut::create(scaleTo);
//    m_barNode->runAction(easeBackOut);
    
//    CCActionInterval * delayT = CCDelayTime::create(0.5);
//    CCActionInterval * scaleTo1 = CCScaleTo::create(1.0, 4,1);
//    m_bar->runAction(CCSequence::create(delayT, scaleTo1, NULL));
    
    this->unschedule(schedule_selector(AllianceScienceDonateView::onPlayText));
    this->schedule(schedule_selector(AllianceScienceDonateView::onPlayText),0);
    
    this->scheduleOnce(schedule_selector(AllianceScienceDonateView::onHideDonate), 0.6);//1.2
}

void AllianceScienceDonateView::onPlayText(float _time){
    if(m_curDonate>=m_endDonate){
        m_curDonate = m_endDonate;
        this->unschedule(schedule_selector(AllianceScienceDonateView::onPlayText));
    }else{
        m_curDonate += m_preDonate;
    }
    m_barLabel->setString(CC_CMDITOA(m_curDonate)+"/"+CC_CMDITOA(m_sumDonate));
    int tmpWidth = m_curDonate*1.0*436/m_sumDonate;
    m_clipNode->setContentSize(CCSize(tmpWidth,60));
}

void AllianceScienceDonateView::onHideDonate(float _time){
//    m_barNode->setVisible(false);
//    m_barNode->setScale(0);
//    m_bar->setScaleX(1);
    m_canDonate = true;
    onRefreshData(NULL);
}
