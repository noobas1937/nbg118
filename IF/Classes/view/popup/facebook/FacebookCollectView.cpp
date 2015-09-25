//
//  FacebookCollectView.cpp
//  IF
//
//  Created by ganxiaohua on 14-10-15.
//
//

#include "FacebookCollectView.h"
#include "FBUtilies.h"
#include "UserBindCommand.h"
#include "FBGetGlodMineCommand.h"
#include "SocialController.h"
#include "PopupViewController.h"
#include "FacebookMineView.h"

FacebookCollectView::~FacebookCollectView()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBLoginSucess);
}

FacebookCollectView:: FacebookCollectView(){
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FacebookCollectView::FacebookCallback), MSG_FBLoginSucess, NULL);
}

bool FacebookCollectView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonTxt", CCLabelIF*, this->m_buttonTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardTxt", CCLabelIF*, this->m_rewardTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loginBtn", CCControlButton*, this->m_loginBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_collectBtn", CCControlButton*, this->m_collectBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_facebookNode", CCNode*, this->m_facebookNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNode", CCNode*, this->m_goldNode);
    return true;
}

SEL_CCControlHandler FacebookCollectView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFacebookClick", FacebookCollectView::onFacebookClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCollectClick", FacebookCollectView::onCollectClick);
    return NULL;
}

void FacebookCollectView::onFacebookClick(CCObject * pSender, Control::EventType pCCControlEvent){
    string localCus = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if (localCus != "") {
        //todo
    }else{
        if (FBUtilies::fbIsLogin())
        {
            FBUtilies::fbLogout();
        }
        GlobalData::shared()->isBind = true;
        FBUtilies::fbLogin();
    }
}

void FacebookCollectView::onCollectClick(CCObject * pSender, Control::EventType pCCControlEvent){
    FBGetGlodMineCommand* cmd = new FBGetGlodMineCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(FacebookCollectView::collectCallback), NULL));
    cmd->sendAndRelease();
}

void FacebookCollectView::collectCallback(CCObject* params){
    NetResult* result = dynamic_cast<NetResult*>(params);
    auto dic = _dict(result->getData());
    if(dic){
        int gold = dic->valueForKey("gold")->intValue();
        int goldReward = dic->valueForKey("goldReward")->intValue();
        GlobalData::shared()->playerInfo.gold = gold;
        SocialController::getInstance()->goldReward=goldReward;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        PopupViewController::getInstance()->removePopupView(this);
        PopupViewController::getInstance()->addPopupInView(FacebookMineView::create());
    }
}

bool FacebookCollectView::init(){
    bool ret = false;
    if(PopupBaseView::init())
    {
        CCLoadSprite::doResourceByCommonIndex(8, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
        });
        auto tbg = CCLoadSprite::loadResource("technology_09.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        int maxHight = CCDirector::sharedDirector()->getWinSize().height;
        int curHight = -500;
        while (curHight<maxHight) {
            auto bg = CCLoadSprite::createSprite("technology_09.png");
            bg->setAnchorPoint(ccp(0, 1));
            bg->setPosition(ccp(0, curHight));
            curHight += bg->getContentSize().height;
            tBatchNode->addChild(bg);
        }
        this->addChild(tBatchNode);
        
        auto tmpCCB = CCBLoadFile("FacebookCollectView", this, this);
        this->setContentSize(tmpCCB->getContentSize());
        changeBGHeight(m_viewBg);
        m_viewBg->setVisible(false);
        string _fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
        m_facebookNode->setVisible(_fbUid=="");
        m_collectBtn->setVisible(!m_facebookNode->isVisible());
        string _gpName = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERNAME,"");
        if(_gpName!=""){
            CCCommonUtils::setButtonTitle(m_collectBtn, _lang("107016").c_str());
        }else{
            m_buttonTxt->setString(_lang("105287").c_str());
        }
        string preuid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
        if (preuid=="") {
            m_descTxt->setString(_lang("107002").c_str());
            m_buttonTxt->setString(_lang("107001").c_str());
        }else if(preuid!="" && SocialController::getInstance()->goldReward==0){
            m_descTxt->setString(_lang("107051").c_str());
            m_buttonTxt->setString("");
            CCCommonUtils::setButtonTitle(m_collectBtn, _lang("107016").c_str());
        }else{
            m_descTxt->setString(_lang("107003").c_str());
            m_buttonTxt->setString(_lang("107001").c_str());
            m_goldNode->setVisible(false);
        }
        m_rewardTxt->setString(CC_ITOA(GlobalData::shared()->glodMine_fb_k2));
        ret = true;
    }
    return ret;
    
}

void FacebookCollectView::FacebookCallback(CCObject* params){
//    if(params==NULL){
//        return;
//    }
    
//    auto _tmpUid = dynamic_cast<CCString*>(params);
//    if (!_tmpUid) {
//        return;
//    }
    this->scheduleOnce(schedule_selector(FacebookCollectView::delayUseData), 0.2);
}

void FacebookCollectView::delayUseData(float _time){
    string m_facebookUid =CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_uid", "");
    if (m_facebookUid == "") {
        return;
    }
    string preuid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    string preuName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_Name","");
    GameController::getInstance()->showWaitInterface();
    string m_facebookName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_Name","");
    UserBindCommand* cmd = new UserBindCommand("",m_facebookUid,"","","",1,preuid,"",m_facebookName,preuName);
    cmd->sendAndRelease();
    if(SocialController::getInstance()->goldReward==0 && preuid!=""){
        m_facebookNode->setVisible(preuid=="");
        m_collectBtn->setVisible(!m_facebookNode->isVisible());
        if (preuid=="") {
            m_descTxt->setString(_lang("107051").c_str());
            m_buttonTxt->setString(_lang("107001").c_str());
        }else if(preuid!="" && SocialController::getInstance()->goldReward==0){
            m_descTxt->setString(_lang("107002").c_str());
            m_buttonTxt->setString("");
            CCCommonUtils::setButtonTitle(m_collectBtn, _lang("107016").c_str());
        }else{
            m_descTxt->setString(_lang("107003").c_str());
            m_buttonTxt->setString(_lang("107001").c_str());
            m_goldNode->setVisible(false);
        }
    }else{
        PopupViewController::getInstance()->removePopupView(this);
        PopupViewController::getInstance()->addPopupInView(FacebookMineView::create());
    }
}

void FacebookCollectView::onEnter(){
    CCNode::onEnter();
    this->setTitleName(_lang("107050"));
}

void FacebookCollectView::onExit(){
    GlobalData::shared()->isBind = false;
    CCNode::onExit();
}