//
//  CityDefenseView.cpp
//  IF
//
//  Created by fubin on 14-8-13.
//
//

#include "CityDefenseView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "ParticleController.h"
#include "QueueController.h"
#include "UserUpgradeView.h"
#include "CCThreadManager.h"
#include "WorldController.h"
#include "CommandBase.h"
#include "AddDefenseCommand.h"
#include "BuyCityDefCommand.h"
#include "TipsView.h"
#include "GameController.h"

CityDefenseView* CityDefenseView::create(int buildId){
    CityDefenseView* ret = new CityDefenseView();
    if(ret && ret->init(buildId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CityDefenseView::init(int buildId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        
    });
    m_buildId = buildId;
    m_st = 0;
    //m_smokeNode = CCLayer::create();
   CCNode* __tmpNode = CCBLoadFile("CityDefenseView",this,this);
    
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());

//    changeBGHeight(m_buildBG);
//    auto size = CCDirector::sharedDirector()->getWinSize();
//    m_smokeNode->setAnchorPoint(ccp(0.5, 0.5));
//    m_smokeNode->setPosition(0, 0);
//    m_mainNode->addChild(m_smokeNode);
//    this->addChild(m_smokeNode);
    
    int addHeight = getExtendHeight();
    m_mainNode->setPositionY(m_mainNode->getPositionY()+addHeight);
//    m_bNode->setPositionY(m_bNode->getPositionY()-addHeight);

    m_fullbar->setVisible(false);
    //m_defMsgLabel->setString(_lang("102307")+"\n"+_lang("102308")+"\n"+_lang("102309")+"\n"+_lang("102310"));
    m_defMsgLabel->setString(_lang("102341"));
    m_defLabel->setString(_lang("102303"));
    //CCCommonUtils::setButtonTitle(m_defBtn, _lang("102305").c_str());
    
    FunBuildInfo& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    setTitleName(_lang("102206"));
    
//    for (int i=1; i<=4; i++) {
//        auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
//        m_fireNode1->addChild(particle);
//        
//        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
//        m_fireNode2->addChild(particle1);
//    }
    string str = _lang("102335");
    m_statusTxt->setString(_lang("102335"));
    int per = 0;
    if (GlobalData::shared()->alliance_territory_negative_effect > 0) {//联盟领地减益
        double effect = CCCommonUtils::getTerritoryNegativeEffectValueByNum(122) * 1.0 / 100;
        per = GlobalData::shared()->worldConfig.fire1[0]*60*2*effect/GlobalData::shared()->worldConfig.fire[0];
    }
    else
        per = GlobalData::shared()->worldConfig.fire1[0]*60*2/GlobalData::shared()->worldConfig.fire[0];
    
    string temp = _lang_2("102337","1",CC_ITOA(per));
    m_statusDesc->setString(temp);
    m_fireLabel->setString(_lang("102339"));
    temp = CC_ITOA(GlobalData::shared()->worldConfig.fire1[2]);
    m_defLabel->setString(temp);
    
    m_timeTxt->setString(_lang("102336"));//
    temp = _lang_1("102338","");
    m_timeDesc->setString(temp);
    m_fuFeiBtnTxt->setString(_lang("102340"));
    temp = CC_ITOA(GlobalData::shared()->worldConfig.fire1[3]);
    m_fuFeiValue->setString(temp);
    
    updateInfo();
    return true;
}

void CityDefenseView::updateInfo()
{
    onEnterFrame(0);
}

void CityDefenseView::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_smokeNode->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void CityDefenseView::onEnter()
{
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CityDefenseView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCNode::onEnter();
}

void CityDefenseView::onExit()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(CityDefenseView::onEnterFrame), this);
    
    m_smokeNode->removeAllChildren();
    m_parVec.clear();
    CCNode::onExit();
}

SEL_CCControlHandler CityDefenseView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickAddDefense", CityDefenseView::onClickAddDefense);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpClick", CityDefenseView::onHelpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFuFeiClick", CityDefenseView::onFuFeiClick);
    return NULL;
}

bool CityDefenseView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bNode", CCNode*, this->m_bNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defLabel", CCLabelIF*, this->m_defLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defMsgLabel", CCLabelIF*, this->m_defMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireLabel", CCLabelIF*, this->m_fireLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defbarLabel", CCLabelIF*, this->m_defbarLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defBtn", CCControlButton*, this->m_defBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defbar", CCScale9Sprite*, this->m_defbar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fullbar", CCScale9Sprite*, this->m_fullbar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_middleNode", CCNode*, this->m_middleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fufeiBtn", CCControlButton*, this->m_fufeiBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_statusTxt", CCLabelIF*, this->m_statusTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_statusDesc", CCLabelIF*, this->m_statusDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeDesc", CCLabelIF*, this->m_timeDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fuFeiBtnTxt", CCLabelIF*, this->m_fuFeiBtnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fuFeiValue", CCLabelIF*, this->m_fuFeiValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fullTip", CCLabelIF*, this->m_fullTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_smokeNode", CCNode*, this->m_smokeNode);
    return false;
}

void CityDefenseView::onClickAddDefense(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_defBtn->setEnabled(false);
    m_defBtn->setColor(ccGRAY);
    
    auto cmd = new AddDefenseCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(CityDefenseView::onUpdateDefenseCallback), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void CityDefenseView::onHelpClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102333"),kCCTextAlignmentLeft));
}

void CityDefenseView::onFuFeiClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    YesNoDialog::showButtonAndGold(_lang("102334").c_str(), CCCallFunc::create(this, callfunc_selector(CityDefenseView::fuFeiHandle)), _lang("102340").c_str(), GlobalData::shared()->worldConfig.fire1[3]);
}

void CityDefenseView::fuFeiHandle(){
    if (GlobalData::shared()->worldConfig.fire1[3] > GlobalData::shared()->playerInfo.gold) {
        YesNoDialog::gotoPayTips();
        return ;
    }
    BuyCityDefCommand* cmd = new BuyCityDefCommand();
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(CityDefenseView::sendSuccess), NULL));
    cmd->sendAndRelease();
    m_fufeiBtn->setEnabled(false);
    GameController::getInstance()->showWaitInterface();
}

void CityDefenseView::sendSuccess(CCObject* data){
    auto ret = dynamic_cast<NetResult*>(data);
    if (!ret) {
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if(info->objectForKey("cityDef")){
        GlobalData::shared()->cityDefenseVal = info->valueForKey("cityDef")->intValue();
    }else{
        GlobalData::shared()->cityDefenseVal =  m_cityDefenseVal;
    }
    if(info->objectForKey("lastCityDefTime")){
        GlobalData::shared()->cityLastUpdateDefenseStamp = info->valueForKey("lastCityDefTime")->doubleValue();
    }
    GlobalData::shared()->cityStartFireStamp = WorldController::getInstance()->getTime();
    GlobalData::shared()->cityFireStamp = WorldController::getInstance()->getTime();
    
    onEnterFrame(0.0);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_CITY_FIRE);
}

void CityDefenseView::onEnterFrame(float dt)
{
    unsigned int fireRate = 0;
    if (GlobalData::shared()->isCrossService) {
        fireRate = GlobalData::shared()->wb_fire_k1;
    }else if (GlobalData::shared()->cityTileCountry == NEUTRALLY) {
        fireRate = GlobalData::shared()->worldConfig.fire[2];
    }else{
        fireRate = GlobalData::shared()->worldConfig.fire[0];
    }
    
    auto now = WorldController::getInstance()->getTime();
    unsigned int fix = 0;
    if (GlobalData::shared()->cityStartFireStamp < GlobalData::shared()->cityFireStamp) {
        double start = MIN(now, GlobalData::shared()->cityFireStamp);
        double duration = start - GlobalData::shared()->cityStartFireStamp;
        fix = (unsigned int)(duration * (double)fireRate / 3600000.0);
    }
    
    auto lastUpdate = GlobalData::shared()->cityLastUpdateDefenseStamp;
    double delay = GlobalData::shared()->cityUpdateDefenseDelayMin * 60000.0;
    if (now > lastUpdate + delay) {
        m_defBtn->setEnabled(true);
        m_defBtn->setColor(ccWHITE);
        m_fireLabel->setString(_lang("102339"));
        string temp = CC_ITOA(GlobalData::shared()->worldConfig.fire1[2]);
        m_defLabel->setString(temp);
        CCCommonUtils::setButtonTitle(m_defBtn, "");
    } else {
        m_defBtn->setEnabled(false);
        m_defBtn->setColor(ccGRAY);
        int tmpTime = ((lastUpdate + delay)-now)/1000;
        CCCommonUtils::setButtonTitle(m_defBtn, CC_SECTOA(tmpTime));
        m_fireLabel->setString("");
        m_defLabel->setString("");
    }
    
    unsigned int cityDefMax = GlobalData::shared()->imperialInfo[FUN_BUILD_WALL_ID].para2 + CCCommonUtils::getEffectValueByNum(125);
    //unsigned int cityDefFix = GlobalData::shared()->cityDefenseVal - fix;
    unsigned int cityDefFix = GlobalData::shared()->cityDefenseVal - fix;
    if(now >= GlobalData::shared()->cityFireStamp){
        cityDefFix = GlobalData::shared()->cityDefenseVal;
    }
    m_cityDefenseVal = cityDefFix;
    m_fufeiBtn->setEnabled(false);
    m_middleNode->setVisible(false);
    if (cityDefFix >= cityDefMax) {
        changeST(0);
        m_fullbar->setVisible(true);
        m_defbar->setVisible(false);
        m_defBtn->setVisible(false);
        m_defBtn->setEnabled(false);
        m_defBtn->setColor(ccGRAY);
        m_defMsgLabel->setString("");
        m_fullTip->setString(_lang("102331"));
        m_defLabel->setVisible(true);
    }
    else {
        m_defBtn->setVisible(true);
        m_middleNode->setVisible(true);
        m_fullbar->setVisible(false);
        m_defbar->setVisible(true);
        m_fullTip->setString("");
        if (now < GlobalData::shared()->cityFireStamp) {
            changeST(1);
            m_fufeiBtn->setEnabled(true);
            m_defMsgLabel->setString(_lang("102332"));
            
            double dt = (GlobalData::shared()->cityFireStamp-now)/1000;
            string timestr = "";
            int timedt = 0;
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
        
            string str = _lang("102335");
            m_statusTxt->setString(_lang("102335"));
            string temp = "";
            int per = 0;
            if (GlobalData::shared()->alliance_territory_negative_effect >0) {//联盟领地减益
                double effect = CCCommonUtils::getTerritoryNegativeEffectValueByNum(122) * 1.0 / 100;
                if (GlobalData::shared()->isCrossService) {
                    int rate = GlobalData::shared()->wb_fire_k1;
                    int perRate = rate/3600;
                    temp = _lang_2("102337",CC_ITOA(perRate),"1");
                }else if (GlobalData::shared()->cityTileCountry == NEUTRALLY) {
                    per = GlobalData::shared()->worldConfig.fire[2]/(GlobalData::shared()->worldConfig.fire1[0]*60*2*effect);
                    temp = _lang_2("102337",CC_ITOA(per),"1");
                }else{
                    per = GlobalData::shared()->worldConfig.fire1[0]*60*2*effect/GlobalData::shared()->worldConfig.fire[0];
                    temp = _lang_2("102337","1",CC_ITOA(per));
                }
            }
            else {
                if (GlobalData::shared()->isCrossService) {
                    int rate = GlobalData::shared()->wb_fire_k1;
                    int perRate = rate/3600;
                    temp = _lang_2("102337",CC_ITOA(perRate),"1");
                }else if (GlobalData::shared()->cityTileCountry == NEUTRALLY) {
                    per = GlobalData::shared()->worldConfig.fire[2]/(GlobalData::shared()->worldConfig.fire1[0]*60*2);
                    temp = _lang_2("102337",CC_ITOA(per),"1");
                }else{
                    per = GlobalData::shared()->worldConfig.fire1[0]*60*2/GlobalData::shared()->worldConfig.fire[0];
                    temp = _lang_2("102337","1",CC_ITOA(per));
                }
            }
            m_statusDesc->setString(temp);
            m_timeTxt->setString(_lang("102336"));
            m_timeDesc->setString(_lang_1("102338",timestr.c_str()));
        }
        else {
            m_defMsgLabel->setString(_lang("102341"));
            m_timeTxt->setString(_lang("102336"));
            m_timeDesc->setString(_lang("102344"));
            m_statusTxt->setString(_lang("102342"));
            m_statusDesc->setString(_lang("102343"));
            changeST(0);
        }
    }
    float pro = cityDefFix*1.0/cityDefMax;
    pro = MIN(pro,1);
    pro = MAX(0, pro);
    m_defbar->setScaleX(pro);
    m_defbarLabel->setString(_lang_2("102170", CC_CMDITOA(cityDefFix).c_str(), CC_CMDITOA(cityDefMax).c_str()));
}

void CityDefenseView::onUpdateDefenseCallback(cocos2d::CCObject *obj) {
    GameController::getInstance()->removeWaitInterface();
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    GlobalData::shared()->cityDefenseVal = info->valueForKey("cityDefValue")->intValue();
    GlobalData::shared()->cityLastUpdateDefenseStamp = info->valueForKey("lastCityDefTime")->doubleValue();
    GlobalData::shared()->cityStartFireStamp = WorldController::getInstance()->getTime();
}

void CityDefenseView::changeST(int type)
{
    if(type==1 && m_st!=type) {
        m_st = type;
        if(GlobalData::shared()->cityTileCountry == NEUTRALLY){
            auto particle1 = ParticleController::createParticle("BrokenCityUIFire_1");
            particle1->setPosition(ccp(-260, 0));
            addParticleToBatch(particle1);
            
            auto particle2 = ParticleController::createParticle("BrokenCityUIFire_1");
            particle2->setPosition(ccp(260, 0));
            addParticleToBatch(particle2);
            
            auto particle3 = ParticleController::createParticle("BrokenCityUIFire_2");
            particle3->setPosition(ccp(0, 0));
            addParticleToBatch(particle3);
            
            auto particle4 = ParticleController::createParticle("BrokenCityUIFire_3");
            particle4->setPosition(ccp(0, 0));
            addParticleToBatch(particle4);
            
            auto particle5 = ParticleController::createParticle("BrokenCityUIFire_4");
            particle5->setPosition(ccp(0, 0));
            addParticleToBatch(particle5);
            
        }else{
            auto tmp_ptArray = CCPointArray::create(20);
            tmp_ptArray->addControlPoint(ccp(0, -10));
            tmp_ptArray->addControlPoint(ccp(90, 100));
            tmp_ptArray->addControlPoint(ccp(-150, 50));
            tmp_ptArray->addControlPoint(ccp(-50, 160));
            string fireName = "WarFire_";
            for (int i=0; i<tmp_ptArray->count(); i++) {
                for (int j=1; j<=5; j++) {
                    auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
                    particle1->setPosition(tmp_ptArray->getControlPointAtIndex(i));
                    addParticleToBatch(particle1);
                }
            }
        }

    }
    else if (type==0 && m_st!=type) {
        m_smokeNode->removeAllChildren();
        m_parVec.clear();
    }
}