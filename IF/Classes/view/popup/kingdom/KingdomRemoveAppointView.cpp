//
//  KingdomRemoveAppointView.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-10.
//
//

#include "KingdomRemoveAppointView.h"
#include "PopupViewController.h"
#include "GameController.h"
#include "ManagerKingdomView.h"
#include "WorldMapView.h"
#include "WorldController.h"
#include "SceneController.h"
#include "YesNoDialog.h"
#include "RoleInfoView.h"
#include "MailController.h"
#include "AllianceInviteView.h"

KingdomRemoveAppointView *KingdomRemoveAppointView::create(OfficeInfo* info,bool manager){
    KingdomRemoveAppointView *ret = new KingdomRemoveAppointView(info,manager);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void KingdomRemoveAppointView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void KingdomRemoveAppointView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool KingdomRemoveAppointView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void KingdomRemoveAppointView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_clickArea, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool KingdomRemoveAppointView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto cf = CCLoadSprite::getSF("KIng_frame_02.png");
        if (!cf) {
            CCLoadSprite::doResourceByCommonIndex(310, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(310, false);
            });
        }
        auto tmpCCB = CCBLoadFile("KingdomRemoveAppointView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        m_iconNode->removeAllChildrenWithCleanup(true);
        string iconStr = "";
        if(m_info->uid==""){
            m_nameTxt->setString("");
            iconStr = m_info->icon + ".png";
        }else{
            string temp = "";
            if(m_info->abbr!=""){
                temp.append("(");
                temp.append(m_info->abbr);
                temp.append(")");
            }
            temp.append(m_info->playerName);
            m_nameTxt->setString(temp.c_str());
            iconStr = m_info->playerIcon + ".png";
        }
        auto pic = CCLoadSprite::createSprite(iconStr.c_str());
        m_iconNode->addChild(pic);
        CCCommonUtils::setSpriteMaxSize(pic, 75);
        m_titleTxt->setString(m_info->name.c_str());
        int num = MIN(m_info->tips.size(),m_info->params.size());
        m_tipNode1->setVisible(false);
        m_tipNode2->setVisible(false);
        m_tipNode3->setVisible(false);
        if(m_info->isServant){
            m_txt1->setColor({255,38,0});
            m_txt2->setColor({255,38,0});
            m_txt3->setColor({255,38,0});
        }else{
            m_txt1->setColor({165,249,0});
            m_txt2->setColor({165,249,0});
            m_txt3->setColor({165,249,0});
        }
        switch (num) {
            case 1:
                m_txt1->setString(_lang_1(m_info->tips[0], m_info->params[0].c_str()));
                m_tipNode1->setVisible(true);
                break;
            case 2:
                m_txt1->setString(_lang_1(m_info->tips[0], m_info->params[0].c_str()));
                m_txt2->setString(_lang_1(m_info->tips[1], m_info->params[1].c_str()));
                m_tipNode1->setVisible(true);
                m_tipNode2->setVisible(true);
                break;
            case 3:
                m_txt1->setString(_lang_1(m_info->tips[0], m_info->params[0].c_str()));
                m_txt2->setString(_lang_1(m_info->tips[1], m_info->params[1].c_str()));
                m_txt3->setString(_lang_1(m_info->tips[2], m_info->params[2].c_str()));
                m_tipNode1->setVisible(true);
                m_tipNode2->setVisible(true);
                m_tipNode3->setVisible(true);
                break;
            default:
                break;
        }
         m_goBtn->setVisible(false);
        if(m_info->uid==""){
            m_btnNode->setVisible(false);
        }else{
            m_btnNode->setVisible(true);
        }
        if (m_manager) {
            m_introTxt->setString(_lang("110065"));
            m_introTxt->setVisible(m_info->uid=="");
            if(m_info->uid==""){
                double leftTime = GlobalData::shared()->getWorldTime() - m_info->appointTime;
                if (leftTime<=0 || m_info->appointTime <= 0) {
                    m_goBtn->setEnabled(true);
                    m_introTxt->setColor({255,255,255});
                }else{
                    m_goBtn->setEnabled(false);
                    m_introTxt->setColor({255,0,0});
                    m_introTxt->setString(_lang_1("138156",CC_SECTOA(leftTime)));
                    this->unschedule(schedule_selector(KingdomRemoveAppointView::freshTime));
                    this->schedule(schedule_selector(KingdomRemoveAppointView::freshTime), 1);
                }
                m_goBtn->setVisible(true);
            }
        }else{
            m_btnNode1->setPosition(-103.6, -289.0);
            m_btnNode2->setPosition(112.0, -289.0);
            m_btnNode3->setVisible(false);
        }

        m_btnTxt1->setString(_lang("108721"));
        m_btnTxt2->setString(_lang("101205"));
        m_btnTxt3->setString(_lang("110068"));
        CCCommonUtils::setButtonTitle(m_goBtn, _lang("110069").c_str());
        ret = true;
    }
    return ret;
}

void KingdomRemoveAppointView::freshTime(float _time){
    double leftTime = GlobalData::shared()->getWorldTime() - m_info->appointTime;
    if (leftTime<=0) {
        m_goBtn->setEnabled(true);
        m_introTxt->setColor({255,255,255});
        this->unschedule(schedule_selector(KingdomRemoveAppointView::freshTime));
    }else{
        m_goBtn->setEnabled(false);
        m_introTxt->setColor({255,0,0});
        m_introTxt->setString(_lang_1("138156",CC_SECTOA(leftTime)));
    }
}

bool KingdomRemoveAppointView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1Value", CCLabelIF*, this->m_txt1Value);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2Value", CCLabelIF*, this->m_txt2Value);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3Value", CCLabelIF*, this->m_txt3Value);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goBtn", CCControlButton*, this->m_goBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipNode1", CCNode*, this->m_tipNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipNode2", CCNode*, this->m_tipNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipNode3", CCNode*, this->m_tipNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_introTxt", CCLabelIF*, this->m_introTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnNode1", CCNode*, this->m_btnNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnNode2", CCNode*, this->m_btnNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnNode3", CCNode*, this->m_btnNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnTxt1", CCLabelIF*, this->m_btnTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnTxt2", CCLabelIF*, this->m_btnTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnTxt3", CCLabelIF*, this->m_btnTxt3);
    
    return false;
}

SEL_CCControlHandler KingdomRemoveAppointView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoWorldClick", KingdomRemoveAppointView::onGoWorldClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "btnClick1", KingdomRemoveAppointView::btnClick1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "btnClick2", KingdomRemoveAppointView::btnClick2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "btnClick3", KingdomRemoveAppointView::btnClick3);
    return NULL;
}

void KingdomRemoveAppointView::btnClick1(CCObject *pSender, CCControlEvent event){
    string uid = m_info->uid;
    PopupViewController::getInstance()->removePopupView(this);
    RoleInfoView::createInfoByUid(uid);
}

void KingdomRemoveAppointView::btnClick2(CCObject *pSender, CCControlEvent event){
    string uid = m_info->uid;
    string playerName = m_info->playerName;
    PopupViewController::getInstance()->removePopupView(this);
    MailController::getInstance()->openMailDialogViewFirst(playerName, uid);
}

void KingdomRemoveAppointView::btnClick3(CCObject *pSender, CCControlEvent event){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    YesNoDialog::show(_lang_2("110084",m_info->playerName.c_str(),m_info->name.c_str()), CCCallFunc::create(this, callfunc_selector(KingdomRemoveAppointView::removeAppointedFun)));
}

void KingdomRemoveAppointView::removeAppointedFun(){
    KingdomExecCommand* cmd = new KingdomExecCommand();
    cmd->putParam("type", CCInteger::create(FIRE));
    cmd->putParam("uid", CCString::create(m_info->uid));
    cmd->putParam("posId", CCString::create(CC_ITOA(m_info->id)));
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(KingdomRemoveAppointView::removeSuccess), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void KingdomRemoveAppointView::removeSuccess(CCObject* obj){
    m_info->uid = "";
    m_info->playerIcon = "";
    m_info->playerName = "";
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REMOVE_APPOINT_OFFICER,CCInteger::create(m_info->id));
    PopupViewController::getInstance()->removePopupView(this);
}

void KingdomRemoveAppointView::onGoWorldClick(CCObject *pSender, CCControlEvent event){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    int officeId = m_info->id;
    PopupViewController::getInstance()->removePopupView(this);
    PopupViewController::getInstance()->addPopupInView(AllianceInviteView::create(officeId,OFFICIAL));
}