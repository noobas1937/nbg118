//
//  AllianceInfoMembersView.cpp
//  IF
//
//  Created by chenliang on 14-4-3.
//
//

#include "AllianceInfoMembersView.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "PopupBaseView.h"
#include "GetAllianceMembersCommand.h"
#include <algorithm>
#include "MailWritePopUpView.h"
#include "YesNoDialog.h"
#include "UserApplyListCommand.h"
#include "AllianceManager.h"
#include "RefuseAllianceApplyCommand.h"
#include "AcceptAllianceApplyCommand.h"
#include "AllianceApplylistCommand.h"
#include "AllianceKickMemberCommand.h"
#include "DismissAllianceCommand.h"
#include "LeaveAllianceCommand.h"
#include "AllianceTitleRankCell.h"
#include "AllianceRankAttrView.h"
#include "AllianceApplyView.h"
#include "AllianceManagerFunView.h"
#include "JoinAllianceView.h"
#include "ParticleController.h"
#include "ChatController.h"
#include "UIComponent.h"
#include "CountryChatCommand.h"
#include "MailController.h"
#include "AllianceIntroTip.h"
#include "ChatServiceCocos2dx.h"
#include "AllianceInviteView.h"
#include "AllianceTerritoryShowCommand.h"

#define ALLIANCE_RANK_TITLE_CLICK "alliance_rank_title_click"

static const CCSize CELL_SIZE_WITH_TITLE(640, 85);
static const CCSize CELL_SIZE_WITHOUT_TITLE(640, 230);

static const CCSize CELL_SIZE_WITH_TITLE_HD(1536, 170);
static const CCSize CELL_SIZE_WITHOUT_TITLE_HD(1536, 400);

static int status[6] = {1,1,1,1,1,1};
static bool tipFlag = false;

bool AllianceInfoMembersView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_background", CCScale9Sprite*, this->m_background);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, this->m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selfNode", CCNode*, this->m_selfNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAllianeNode", CCNode*, this->m_noAllianeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAllianceTip", CCLabelIF*, this->m_noAllianceTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnJoin", CCControlButton*, this->m_btnJoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_introNode", CCNode*, this->m_introNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_postest", CCNode*, this->m_postest);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleFlag", CCSprite*, this->m_titleFlag);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powertxt", CCLabelIF*, this->m_powertxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvtxt", CCLabelIF*, this->m_lvtxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nametxt", CCLabelIF*, this->m_nametxt);
    
    return true;
}

SEL_CCControlHandler AllianceInfoMembersView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpClick", AllianceInfoMembersView::onHelpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onApply", AllianceInfoMembersView::onApply);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "openJoinAlliance", AllianceInfoMembersView::openJoinAlliance);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMailBtn", AllianceInfoMembersView::onClickMailBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickInviteBtn", AllianceInfoMembersView::onClickInviteBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickExitBtn", AllianceInfoMembersView::onClickExitBtn);
    
    return NULL;
}

void AllianceInfoMembersView::openJoinAlliance(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
}

void AllianceInfoMembersView::onApply(CCObject *pSender, CCControlEvent event){
    
}

void AllianceInfoMembersView::onHelpClick(CCObject *pSender, CCControlEvent event){
    CCLOG("onHelpClick");
    PopupViewController::getInstance()->addPopupInView(AllianceRankAttrView::create());
    return ;
    if(!tipFlag){
        m_btnStatus = "";
        for (int i=0; i<6; i++) {
            m_btnStatus.append(CC_ITOA(status[i]));
            if(i!=5){
                m_btnStatus.append(",");
            }
        }
        vector<std::string> bvector;
        CCCommonUtils::splitString("1,0,0,0,0,0",",", bvector);
        int num = bvector.size();
        num = MIN(num,6);
        for (int i=0; i<num; i++) {
            status[i] = atoi(bvector[i].c_str());
        }
        m_first = true;
        this->resetTitleStatus();
        this->generalData();
        this->resetPosition();
        
        m_parVec.clear();
        m_introNode->removeFromParent();
        m_introNode->removeAllChildrenWithCleanup(true);
        m_tableView->getContainer()->addChild(m_introNode,100000);
        
        CCRenderTexture* layer = CCRenderTexture::create(640, 1500);
        ccBlendFunc ccb1 = {GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
        layer->getSprite()->setBlendFunc(ccb1);
        layer->clear(0.0f, 0.0f, 0.0f, 0.7f);//0.0f, 0.0f, 0.0f, 0.7f
        layer->setAnchorPoint(ccp(0.5, 0.5));
        layer->setPosition(ccp(320, 750));
        m_introNode->addChild(layer,-20);
        layer->removeAllChildren();
        layer->begin();
        
        CCPoint startPos = ccp(235, m_introNode->getContentSize().height+100);
        if(GlobalData::shared()->playerInfo.allianceInfo.rank>=4){
            startPos.y = startPos.y +80;
        }
        AllianceIntroTip* intro1 = AllianceIntroTip::create(true, ccp(321.0, 103.0),_lang("115245"));
        intro1->setPosition(ccp(235, startPos.y));
        m_introNode->addChild(intro1);
        
        string tmpStart = "ShowFire_";
        string tmpStart1 = "ShowFireUp_";
        CCPoint pos = ccp(560,startPos.y +110);
        CCSize size = m_helpBtn->getContentSize();
        CCNode* frieNode =  CCNode::create();
        frieNode->setAnchorPoint(ccp(0.5, 0.5));
        frieNode->setTag(999);
        m_helpBtn->addChild(frieNode);
        
        for (int i=1; i<=5; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
            particle->setPosition(ccp(size.width/2, -3));
            particle->setPosVar(ccp(size.width/2, 0));
            frieNode->addChild(particle);
            
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
            particle1->setPosition(ccp(size.width/2, size.height-3));
            particle1->setPosVar(ccp(size.width/2, 0));
            frieNode->addChild(particle1);
            
            auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
            particle2->setPosition(ccp(0, size.height/2));
            particle2->setPosVar(ccp(0, size.height/2));
            frieNode->addChild(particle2);
            
            auto particle3 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
            particle3->setPosition(ccp(size.width, size.height/2));
            particle3->setPosVar(ccp(0, size.height/2));
            frieNode->addChild(particle3);
        }
        
        pos = ccp(560,startPos.y +110);
        size = CCSize(60,60);
        CCLayerColor* intro1layer = CCLayerColor::create();
        intro1layer->setOpacity(255);
        intro1layer->setColor(ccc3(255, 255, 255));
        intro1layer->setContentSize(size);
        intro1layer->setPosition(pos);
        ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
        intro1layer->setBlendFunc(cbf);
        intro1layer->visit();
        
        for (int i=1; i<=5; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
            particle->setPosition(pos+ccp(size.width/2, -3));
            particle->setPosVar(ccp(size.width/2, 0));
            addParticleToBatch(particle);
            
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
            particle1->setPosition(pos+ccp(size.width/2, size.height-3));
            particle1->setPosVar(ccp(size.width/2, 0));
            addParticleToBatch(particle1);
            
            auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
            particle2->setPosition(pos+ccp(0, size.height/2));
            particle2->setPosVar(ccp(0, size.height/2));
            addParticleToBatch(particle2);
            
            auto particle3 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
            particle3->setPosition(pos+ccp(size.width, size.height/2));
            particle3->setPosVar(ccp(0, size.height/2));
            addParticleToBatch(particle3);
        }
        
        AllianceIntroTip* intro2 = AllianceIntroTip::create(true, ccp(105.0, 103),_lang("115246"));
        intro2->setPosition(ccp(90, startPos.y - 230));
        m_introNode->addChild(intro2);
        
        pos = ccp(165, startPos.y - 120);
        size = CCSize(150,220);
        
        for (int i=1; i<=5; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
            particle->setPosition(pos+ccp(size.width/2, -3));
            particle->setPosVar(ccp(size.width/2, 0));
            addParticleToBatch(particle);
            
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
            particle1->setPosition(pos+ccp(size.width/2, size.height-3));
            particle1->setPosVar(ccp(size.width/2, 0));
            addParticleToBatch(particle1);
            
            auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
            particle2->setPosition(pos+ccp(0, size.height/2));
            particle2->setPosVar(ccp(0, size.height/2));
            addParticleToBatch(particle2);
            
            auto particle3 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
            particle3->setPosition(pos+ccp(size.width, size.height/2));
            particle3->setPosVar(ccp(0, size.height/2));
            addParticleToBatch(particle3);
        }
        CCLayerColor* clayer = CCLayerColor::create();
        clayer->setOpacity(255);
        clayer->setColor(ccc3(255, 255, 255));
        clayer->setContentSize(size);
        clayer->setPosition(pos);
        cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
        clayer->setBlendFunc(cbf);
        clayer->visit();

        if(GlobalData::shared()->playerInfo.allianceInfo.rank>=4){
            AllianceIntroTip* intro3 = AllianceIntroTip::create(false, ccp(267.0, -17.0),_lang("115247"));
            intro3->setPosition(ccp(20, startPos.y - 580));
            m_introNode->addChild(intro3);
            
            pos = ccp(175, startPos.y - 690);
            size = CCSize(300,60);
            CCLayerColor* intro3layer = CCLayerColor::create();
            intro3layer->setOpacity(255);
            intro3layer->setColor(ccc3(255, 255, 255));
            intro3layer->setContentSize(size);
            intro3layer->setPosition(pos);
            cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
            intro3layer->setBlendFunc(cbf);
            intro3layer->visit();
            
            for (int i=1; i<=5; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
                particle->setPosition(pos+ccp(size.width/2, -3));
                particle->setPosVar(ccp(size.width/2, 0));
                addParticleToBatch(particle);
                
                auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
                particle1->setPosition(pos+ccp(size.width/2, size.height-3));
                particle1->setPosVar(ccp(size.width/2, 0));
                addParticleToBatch(particle1);
                
                auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
                particle2->setPosition(pos+ccp(0, size.height/2));
                particle2->setPosVar(ccp(0, size.height/2));
                addParticleToBatch(particle2);
                
                auto particle3 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
                particle3->setPosition(pos+ccp(size.width, size.height/2));
                particle3->setPosVar(ccp(0, size.height/2));
                addParticleToBatch(particle3);
            }
            
            AllianceIntroTip* intro4 = AllianceIntroTip::create(true, ccp(105.0, 103),_lang("115248"));
            intro4->setPosition(ccp(90, startPos.y - 940));
            m_introNode->addChild(intro4);
            
            pos = ccp(175, startPos.y - 830);
            size = CCSize(300,60);
            
            CCLayerColor* intro4layer = CCLayerColor::create();
            intro4layer->setOpacity(255);
            intro4layer->setColor(ccc3(255, 255, 255));
            intro4layer->setContentSize(size);
            intro4layer->setPosition(pos);
            cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
            intro4layer->setBlendFunc(cbf);
            intro4layer->visit();
            
            for (int i=1; i<=5; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
                particle->setPosition(pos+ccp(size.width/2, -3));
                particle->setPosVar(ccp(size.width/2, 0));
                addParticleToBatch(particle);
                
                auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
                particle1->setPosition(pos+ccp(size.width/2, size.height-3));
                particle1->setPosVar(ccp(size.width/2, 0));
                addParticleToBatch(particle1);
                
                auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
                particle2->setPosition(pos+ccp(0, size.height/2));
                particle2->setPosVar(ccp(0, size.height/2));
                addParticleToBatch(particle2);
                
                auto particle3 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
                particle3->setPosition(pos+ccp(size.width, size.height/2));
                particle3->setPosVar(ccp(0, size.height/2));
                addParticleToBatch(particle3);
            }
        }else{
            AllianceIntroTip* intro3 = AllianceIntroTip::create(false, ccp(267.0, -17.0),_lang("115247"));
            intro3->setPosition(ccp(20, startPos.y - 500));
            m_introNode->addChild(intro3);
            
            pos = ccp(175, startPos.y - 610);
            size = CCSize(300,60);
            CCLayerColor* intro3layer = CCLayerColor::create();
            intro3layer->setOpacity(255);
            intro3layer->setColor(ccc3(255, 255, 255));
            intro3layer->setContentSize(size);
            intro3layer->setPosition(pos);
            cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
            intro3layer->setBlendFunc(cbf);
            intro3layer->visit();
            
            for (int i=1; i<=5; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
                particle->setPosition(pos+ccp(size.width/2, -3));
                particle->setPosVar(ccp(size.width/2, 0));
                addParticleToBatch(particle);
                
                auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
                particle1->setPosition(pos+ccp(size.width/2, size.height-3));
                particle1->setPosVar(ccp(size.width/2, 0));
                addParticleToBatch(particle1);
                
                auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
                particle2->setPosition(pos+ccp(0, size.height/2));
                particle2->setPosVar(ccp(0, size.height/2));
                addParticleToBatch(particle2);
                
                auto particle3 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
                particle3->setPosition(pos+ccp(size.width, size.height/2));
                particle3->setPosVar(ccp(0, size.height/2));
                addParticleToBatch(particle3);
            }
            
            AllianceIntroTip* intro4 = AllianceIntroTip::create(true, ccp(105.0, 103),_lang("115248"));
            intro4->setPosition(ccp(90, startPos.y - 860));
            m_introNode->addChild(intro4);
            pos = ccp(175, startPos.y - 750);
            size = CCSize(300,60);
            
            CCLayerColor* intro4layer = CCLayerColor::create();
            intro4layer->setOpacity(255);
            intro4layer->setColor(ccc3(255, 255, 255));
            intro4layer->setContentSize(size);
            intro4layer->setPosition(pos);
            cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
            intro4layer->setBlendFunc(cbf);
            intro4layer->visit();
            
            for (int i=1; i<=5; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
                particle->setPosition(pos+ccp(size.width/2, -3));
                particle->setPosVar(ccp(size.width/2, 0));
                addParticleToBatch(particle);
                
                auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
                particle1->setPosition(pos+ccp(size.width/2, size.height-3));
                particle1->setPosVar(ccp(size.width/2, 0));
                addParticleToBatch(particle1);
                
                auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
                particle2->setPosition(pos+ccp(0, size.height/2));
                particle2->setPosVar(ccp(0, size.height/2));
                addParticleToBatch(particle2);
                
                auto particle3 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
                particle3->setPosition(pos+ccp(size.width, size.height/2));
                particle3->setPosVar(ccp(0, size.height/2));
                addParticleToBatch(particle3);
            }
        }
        layer->end();
        tipFlag = true;
    }else{
        tipFlag = false;
        m_introNode->removeFromParent();
        vector<std::string> bvector;
        CCCommonUtils::splitString(m_btnStatus,",", bvector);
        int num = bvector.size();
        num = MIN(num,6);
        for (int i=0; i<num; i++) {
            status[i] = atoi(bvector[i].c_str());
        }
        this->resetTitleStatus();
        this->generalData();
        this->resetPosition();
        m_helpBtn->removeChildByTag(999);
    }
}

void AllianceInfoMembersView::onClickMailBtn(CCObject *pSender, CCControlEvent event)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    MailController::getInstance()->openMailDialogViewFirst(_lang("105564"), GlobalData::shared()->playerInfo.uid);
}

void AllianceInfoMembersView::onClickInviteBtn(CCObject *pSender, CCControlEvent event)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->addPopupInView(AllianceInviteView::create());
}

void AllianceInfoMembersView::onClickExitBtn(CCObject *pSender, CCControlEvent event)
{
    if (GlobalData::shared()->isCrossService)
    {
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    if (GlobalData::shared()->playerInfo.allianceInfo.rank == 5 && GlobalData::shared()->playerInfo.allianceInfo.currentNum > 1)
    {
        CCCommonUtils::flyHint("", "", _lang("115249"));
    }
    else if (GlobalData::shared()->playerInfo.allianceInfo.currentNum <= 1 && GlobalData::shared()->playerInfo.allianceInfo.rank == 5)
    {
        YesNoDialog::showVariableTitle(_lang("115040").c_str(),CCCallFunc::create(this, callfunc_selector(AllianceInfoMembersView::dismissAlliance)),_lang("confirm").c_str());
    }else {
        YesNoDialog::showVariableTitle(_lang("115042").c_str(),CCCallFunc::create(this, callfunc_selector(AllianceInfoMembersView::secondConfirm)),_lang("confirm").c_str());
    }
}

void AllianceInfoMembersView::dismissAlliance()
{
    DismissAllianceCommand * command = new DismissAllianceCommand();
    command->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInfoMembersView::onDismissAlliance), NULL));
    command->sendAndRelease();
}

void AllianceInfoMembersView::onDismissAlliance(CCObject* obj)
{
    UIComponent::getInstance()->changeChatChannel(CHAT_COUNTRY);
    PopupViewController::getInstance()->removeAllPopupView();
    ChatController::getInstance()->m_chatAlliancePool.clear();
    ChatController::getInstance()->m_latestAllianceMsg.sequenceId=0;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::resetPlayerIsInAlliance();
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    ChatServiceCocos2dx::resetPlayerIsInAlliance();
#endif
    CCUserDefault::sharedUserDefault()->setStringForKey(ALLIANCE_MEMBER_BTN_STATUS, "");
}

void AllianceInfoMembersView::secondConfirm()
{
    AllianceTerritoryCheckIfStorageCommand* cmd = new AllianceTerritoryCheckIfStorageCommand(GlobalData::shared()->playerInfo.uid);
    cmd->sendAndRelease();
}

void AllianceInfoMembersView::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle){
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
    m_introNode->addChild(newBatch,-10);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void AllianceInfoMembersView::resetTitleStatus(){
    int num = m_titles.size();
    if(num>6) num = 6;
    for (int i=0; i<num; i++) {
        m_titles[i].setOpen(status[i]);
    }
}

AllianceInfoMember fakeMemberInfo()
{
    AllianceInfoMember member;
    member.setRank(rand()%5);
    member.setName("whatever");
    member.setKills(rand()%10000+500);
    member.setPoints(rand()%500+100);
    return member;
}

bool AllianceInfoMembersView::compareMember(const AllianceInfoMember member1, const AllianceInfoMember member2)
{
    return member1.getRank() > member2.getRank();
}

bool AllianceInfoMembersView::compareMember2(const AllianceInfoMember member1, const AllianceInfoMember member2)
{
    if(member1.getOnline()==member2.getOnline())
    {
        return member1.getPower() > member2.getPower();
    }
    else
    {
        return member1.getOnline()&&!member2.getOnline();
    }
}

float AllianceInfoMembersView::getContentHeight()
{
    map<int, int> rankMap;
    
    //return CELL_SIZE_WITH_TITLE.height*rankMap.size() + CELL_SIZE_WITHOUT_TITLE.height * (m_members.size() - rankMap.size());
    return 0.0 ;
}

bool AllianceInfoMembersView::init(){
    
    bool ret = false;
    if(PopupBaseView::init())
    {
        setIsHDPanel(true);
        
        CCLoadSprite::doResourceByGeneralIndex(1, true);
        CCLoadSprite::doResourceByGeneralIndex(2, true);
        CCLoadSprite::doResourceByGeneralIndex(3, true);
        
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(307, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(307, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
            
            CCLoadSprite::doResourceByGeneralIndex(1, false);
            CCLoadSprite::doResourceByGeneralIndex(2, false);
            CCLoadSprite::doResourceByGeneralIndex(3, false);
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
        if (CCCommonUtils::isIosAndroidPad())
        {
            tBatchNode->setScaleX(1536.0 / 640.0);
        }
        this->addChild(tBatchNode);
        auto node = CCBLoadFile("AllianceInfoMembersView", this, this);
        setContentSize(node->getContentSize());
        m_loadingIcon = NULL;
        //std::sort(m_members.begin(), m_members.end(), compareMember);
        int preHeight = this->m_background->getContentSize().height;
        changeBGHeight(m_background);
        int dh = m_background->getContentSize().height - preHeight;
        m_background->setVisible(false);

        m_lvtxt->setString("0");
        m_powertxt->setString("0");
        m_nametxt->setString("");
        m_postest->removeAllChildren();
        
        m_showData = CCArray::create();
        dh += 90;
        if (CCCommonUtils::isIosAndroidPad()) {
            dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
        m_listContainer->setPositionY(m_listContainer->getPositionY()-dh);
        
        m_introNode->setContentSize(m_listContainer->getContentSize());
        m_introNode->setPosition(m_listContainer->getPosition());
        
        m_tableView = CCTableView::create(this, m_listContainer->getContentSize());
        m_tableView->setDirection(kCCScrollViewDirectionVertical);
        m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tableView->setTouchPriority(Touch_Popup);
        m_listContainer->addChild(m_tableView);
        
        string statusBtn = CCUserDefault::sharedUserDefault()->getStringForKey(ALLIANCE_MEMBER_BTN_STATUS,"");
        if(statusBtn==""){
            statusBtn = "1,0,0,0,0,0";
        }
        if(m_allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4 && GlobalData::shared()->playerInfo.allianceInfo.applyNum > 0){
            statusBtn = "0,0,0,0,0,1";
        }
        vector<std::string> bvector;
        CCCommonUtils::splitString(statusBtn,",", bvector);
        int num = bvector.size();
        num = MIN(num,6);
        for (int i=0; i<num; i++) {
            status[i] = atoi(bvector[i].c_str());
        }
        AllianceInfoMember member;
        member.setRank(5);
        string rankTitle = _lang("115104");
        if(GlobalData::shared()->playerInfo.allianceInfo.strRank5!=""){
            rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank5;
        }
        member.setAllianceTitle(rankTitle);
        member.setIsTitle(true);
        member.setOpen(status[0]);
        m_titles.push_back(member);
        
        AllianceInfoMember member2;
        member2.setRank(4);
        rankTitle = _lang("115103");
        if(GlobalData::shared()->playerInfo.allianceInfo.strRank4!=""){
            rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank4;
        }
        member2.setAllianceTitle(rankTitle);
        member2.setIsTitle(true);
        member2.setOpen(status[1]);
        m_titles.push_back(member2);
        
        AllianceInfoMember member3;
        member3.setRank(3);
        rankTitle = _lang("115102");
        if(GlobalData::shared()->playerInfo.allianceInfo.strRank3!=""){
            rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank3;
        }
        member3.setAllianceTitle(rankTitle);
        member3.setIsTitle(true);
        member3.setOpen(status[2]);
        m_titles.push_back(member3);
        
        AllianceInfoMember member4;
        member4.setRank(2);
        rankTitle = _lang("115101");
        if(GlobalData::shared()->playerInfo.allianceInfo.strRank2!=""){
            rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank2;
        }
        member4.setAllianceTitle(rankTitle);
        member4.setIsTitle(true);
        member4.setOpen(status[3]);
        m_titles.push_back(member4);
        
        AllianceInfoMember member5;
        member5.setRank(1);
        rankTitle = _lang("115100");
        if(GlobalData::shared()->playerInfo.allianceInfo.strRank1!=""){
            rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank1;
        }
        member5.setAllianceTitle(rankTitle);
        member5.setIsTitle(true);
        member5.setOpen(status[4]);
        m_titles.push_back(member5);
        if(!m_isApply && GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.rank>=4 && m_clickType == 0){
            AllianceInfoMember member6;
            member6.setRank(0);
            member6.setAllianceTitle(_lang("115171"));
            member6.setIsTitle(true);
            member6.setOpen(status[5]);
            m_titles.push_back(member6);
        }
        if(!m_isApply && !GlobalData::shared()->playerInfo.isInAlliance()){
            m_noAllianeNode->setVisible(true);
            m_noAllianceTip->setString(_lang("115067"));
            CCCommonUtils::setButtonTitle(m_btnJoin, _lang("115068").c_str());
        }
        m_helpBtn->setVisible(!m_isApply);
        if (0 == m_clickType) {
            m_titleTxt->setString(_lang("115027"));
        } else {
            m_titleTxt->setString(_lang("115285"));
        }
        tipFlag = false;
        m_first = true;
        ret = true;
    }
    return ret;
}

void AllianceInfoMembersView::resetScrollPos(){

}

void AllianceInfoMembersView::getUseApplyList(){
    this->resetPosition();
}

void AllianceInfoMembersView::resetPosition(){
    m_tableView->setViewSize(CCSize(m_listContainer->getContentSize().width,m_listContainer->getContentSize().height));
    CCPoint curr = m_tableView->getContentOffset();
    CCPoint max = m_tableView->maxContainerOffset();
    CCPoint min = m_tableView->minContainerOffset();
    m_tableView->reloadData();
    if(!m_first){
//        if (curr.y > max.y) {
//            m_tableView->setContentOffset(m_tableView->maxContainerOffset());
//        }else
//        {
//            curr.y += m_tableView->minContainerOffset().y - min.y;
//            m_tableView->setContentOffset(curr);
//        }
        curr.y += m_tableView->minContainerOffset().y - min.y;
        m_tableView->setContentOffset(curr);
    }
    m_first = false;
}

void AllianceInfoMembersView::onGetAllianceMembers(CCObject* data)
{
    this->removeLoadingAni(NULL);
    m_showData->removeAllObjects();
    m_datas.clear();
//    NetResult* result = dynamic_cast<NetResult*>(data);
//    CCArray* members = dynamic_cast<CCArray*>(result->getData());
    NetResult* result = dynamic_cast<NetResult*>(data);
    CCDictionary* params = _dict(result->getData());
    if (params->objectForKey("rankName")) {
        m_tmpTitles.clear();
        CCDictionary* rankName = _dict(params->objectForKey("rankName"));
        if (rankName->objectForKey("5")) {
            m_tmpTitles.push_back(rankName->valueForKey("5")->getCString());
        } else {
            m_tmpTitles.push_back(_lang("115104"));
        }
        if (rankName->objectForKey("4")) {
            m_tmpTitles.push_back(rankName->valueForKey("4")->getCString());
        } else {
            m_tmpTitles.push_back(_lang("115103"));
        }
        if (rankName->objectForKey("3")) {
            m_tmpTitles.push_back(rankName->valueForKey("3")->getCString());
        } else {
            m_tmpTitles.push_back(_lang("115102"));
        }
        if (rankName->objectForKey("2")) {
            m_tmpTitles.push_back(rankName->valueForKey("2")->getCString());
        } else {
            m_tmpTitles.push_back(_lang("115101"));
        }
        if (rankName->objectForKey("1")) {
            m_tmpTitles.push_back(rankName->valueForKey("1")->getCString());
        } else {
            m_tmpTitles.push_back(_lang("115100"));
        }
        for (int i = 0; i < 5; ++i) {
            m_titles[i].setAllianceTitle(m_tmpTitles[i]);
        }
    }
    CCArray* members = dynamic_cast<CCArray*>(params->objectForKey("list"));
    if(members)
    {
        for (int i=0; i < members->count(); i++) {
            CCDictionary* member = (CCDictionary*)members->objectAtIndex(i);
            
            int rank = member->valueForKey("rank")->intValue();
            string name = member->valueForKey("name")->getCString();
            string pic = member->valueForKey("pic")->getCString();
            unsigned long power = member->valueForKey("power")->longValue();
            string uid = member->valueForKey("uid")->getCString();
            int kills = member->valueForKey("kills")->intValue();
            bool onLine = member->valueForKey("online")->boolValue();
            int pointId = member->valueForKey("pointId")->intValue();
            int mainCityLv = member->valueForKey("mainCityLv")->intValue();
            float offLineTime =  member->valueForKey("offLineTime")->floatValue();
            if(pic==""||pic=="0"){
                pic = "g044_middle.png";
            }else{
                pic.append("_middle.png");
            }
            int userLv = member->valueForKey("level")->intValue();
            
            AllianceInfoMember infoMember;
            infoMember.setRank(rank);
            infoMember.setName(name);
            infoMember.setPoints(power);
            infoMember.setKills(kills);
            infoMember.setUid(uid);
            infoMember.setAllianceId(m_allianceId);
            infoMember.setOffLineTime(offLineTime);
            infoMember.setPointId(pointId);
            infoMember.setHead(member->valueForKey("pic")->getCString());
            infoMember.setPic(pic);
            if(m_isApply){
                infoMember.setOnline(true);
            }else{
                infoMember.setOnline(onLine);
            }
            infoMember.setMainCityLv(mainCityLv);
            infoMember.setPower(power);
            infoMember.setIsManager(false);
            infoMember.setIsTitle(false);
            if(uid==GlobalData::shared()->playerInfo.uid){
                selfInfo = infoMember;
            }
            infoMember.setUserLevel(userLv);
            if (i == 0)
            {
                m_titleFlag->setVisible(infoMember.getRank()>1);
                if (infoMember.getRank() > 1)
                {
                    m_titleFlag->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("icon_Alliance_0%d.png",infoMember.getRank())->getCString()));
                }
                
                string lv = "LV."; lv.append(CC_ITOA(infoMember.getUserLevel()));
                m_lvtxt->setString(lv);
                m_powertxt->setString(CC_CMDITOAL(infoMember.getPower()));
                
                m_nametxt->setString(infoMember.getName());
                
                string t = infoMember.getHead() + "_bust.png";
//                m_postest->initWithSpriteFrame(CCLoadSprite::loadResource(t.c_str()));
                auto s = CCLoadSprite::createSprite(t.c_str());
                s->setAnchorPoint({0.5, 1});
                m_postest->removeAllChildren();
                m_postest->addChild(s);
            }

            m_datas.push_back(infoMember);
        }
    }
    std::sort(m_datas.begin(), m_datas.end(), compareMember2);
    this->generalData();
    this->resetPosition();
}

void AllianceInfoMembersView::onGetAppAllianceMembers(CCObject* data){
    map<string,PlayerInfo*> ::iterator it;
    m_appDatas.clear();
//    for(it = AllianceManager::getInstance()->applyUserList.begin(); it != AllianceManager::getInstance()->applyUserList.end(); it++){
//        AllianceInfoMember infoMember;
//        infoMember.setRank(0);
//        infoMember.setName(it->second->name);
//        infoMember.setPoints(0);
//        infoMember.setKills(0);
//        infoMember.setPointId(it->second->pointId);
//        infoMember.setMainCityLv(it->second->mainCityLv);
//        std::string pic = it->second->pic;
//        if(pic==""||pic=="0"){
//            pic = "g044_middle.png";
//        }else{
//            pic.append("_middle.png");
//        }
//        infoMember.setPic(pic);
//        infoMember.setUid(it->second->uid);
//        infoMember.setPower(it->second->power);
//        infoMember.setIsManager(true);
//        infoMember.setIsTitle(false);
//        
//    }
    NetResult* result = dynamic_cast<NetResult*>(data);
    CCDictionary* params = _dict(result->getData());
    CCArray* members = dynamic_cast<CCArray*>(params->objectForKey("list"));
    if(members)
    {
        for (int i=0; i < members->count(); i++) {
            CCDictionary* member = (CCDictionary*)members->objectAtIndex(i);
            std::string applyMessage = "";
            applyMessage = member->valueForKey("message")->getCString();
            string uid = member->valueForKey("playerId")->getCString();
            string name = member->valueForKey("playerName")->getCString();
            string pic = member->valueForKey("pic")->getCString();
            unsigned long power = member->valueForKey("power")->longValue();
            if(pic==""||pic=="0"){
                pic = "g044_middle.png";
            }else{
                pic.append("_middle.png");
            }
            int pointId = member->valueForKey("pointId")->intValue();
            AllianceInfoMember infoMember;
            infoMember.setRank(0);
            infoMember.setName(name);
            infoMember.setPoints(0);
            infoMember.setKills(0);
            infoMember.setUid(uid);
            infoMember.setAllianceId(m_allianceId);
            infoMember.setPointId(pointId);
            infoMember.setPic(pic);
            infoMember.setPower(power);
            infoMember.setApplyMessage(applyMessage);
            infoMember.setIsManager(true);
            infoMember.setIsTitle(false);
            m_appDatas.push_back(infoMember);
        }
    }
    
    this->generalData();
    this->resetPosition();
}

bool AllianceInfoMembersView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    return true;
}

void AllianceInfoMembersView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}
void AllianceInfoMembersView::onEnter(){
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    PopupBaseView::onEnter();
    this->setTitleName(_lang("115027"));
    CCLoadSprite::doResourceByCommonIndex(7, true);
    if((m_isApply || GlobalData::shared()->playerInfo.isInAlliance()) && m_datas.size()<=0){
        GetAllianceMembersCommand * command = new GetAllianceMembersCommand();
        command->putParam("allianceId", CCString::create(m_allianceId));
        command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInfoMembersView::onGetAllianceMembers), NULL));
        command->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInfoMembersView::removeLoadingAni), NULL));
        command->sendAndRelease();
        this->addLoadingAni();
    }
    if(!m_isApply && GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.rank>=4 && m_appDatas.size()<=0){
        AllianceApplylistCommand* cmd = new AllianceApplylistCommand(1);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInfoMembersView::onGetAppAllianceMembers), NULL));
        cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInfoMembersView::removeLoadingAni), NULL));
        cmd->sendAndRelease();
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceInfoMembersView::updateMemberList), MSG_ALLIANCE_MEMBER_DATA, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceInfoMembersView::updateShowData), ALLIANCE_RANK_TITLE_CLICK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceInfoMembersView::refresh), MSG_ALLIANCE_DATA_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceInfoMembersView::changeAllianceLeader), MSG_ALLIANCE_LEADER_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceInfoMembersView::acceptOnePlayer), MSG_ALLIANCE_ACCEPT_ONE_PLAYER, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceInfoMembersView::closeSelf), MSG_INVITE_TELEPORT_SEND_SUCCESS, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    ////CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -100, false);
}

void  AllianceInfoMembersView::onExit(){
    tipFlag = false;
    if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.uid == m_allianceId){
        string btnStatus = "";
        for (int i=0; i<6; i++) {
            btnStatus.append(CC_ITOA(status[i]));
            if(i!=5){
                btnStatus.append(",");
            }
        }
        CCUserDefault::sharedUserDefault()->setStringForKey(ALLIANCE_MEMBER_BTN_STATUS, btnStatus);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIANCE_MEMBER_DATA);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_RANK_TITLE_CLICK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIANCE_DATA_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIANCE_LEADER_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIANCE_ACCEPT_ONE_PLAYER);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INVITE_TELEPORT_SEND_SUCCESS);
    setTouchEnabled(false);
    this->removeLoadingAni(NULL);
    PopupBaseView::onExit();
}

void AllianceInfoMembersView::closeSelf(CCObject* obj) {
    if (m_clickType == 1) {
        PopupViewController::getInstance()->goBackPopupView();
    }
}

void AllianceInfoMembersView::removeLoadingAni(CCObject* obj){
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
    m_loadingIcon = NULL;
}

void AllianceInfoMembersView::changeAllianceLeader(CCObject* param){
    m_showData->removeAllObjects();
    auto dic = _dict(param);
    if(dic){
        int rank = dic->valueForKey("rank")->intValue();
        std::string playerId = dic->valueForKey("playerId")->getCString();
        for (int j=0; j<m_datas.size(); j++) {
            if(m_datas[j].getUid()==playerId){
                m_datas[j].setRank(rank);
            }else if(m_datas[j].getUid()==GlobalData::shared()->playerInfo.allianceInfo.leaderUid){
                m_datas[j].setRank(5);
            }
        }
    }
    this->generalData();
    this->resetPosition();
}

void AllianceInfoMembersView::refresh(CCObject* data){
    m_showData->removeAllObjects();
    CCString* uid = dynamic_cast<CCString*>(data);
    if(uid){
        std::vector<AllianceInfoMember> ::iterator it;
        for (it = m_datas.begin(); it!=m_datas.end(); it++) {
            if(it->getUid()==uid->getCString()){
                it->reset();
                break;
            }
        }
        std::vector<AllianceInfoMember> ::iterator appIt;
        for (appIt = m_appDatas.begin(); appIt!=m_appDatas.end(); appIt++) {
            if(appIt->getUid()==uid->getCString()){
                appIt->reset();
                break;
            }
        }
    }
    auto dic = dynamic_cast<CCDictionary*>(data);
    if(dic){
        int rank = dic->valueForKey("rank")->intValue();
        std::string playerId = dic->valueForKey("playerId")->getCString();
        for (int j=0; j<m_datas.size(); j++) {
            if(m_datas[j].getUid()==playerId){
                m_datas[j].setRank(rank);
                break;
            }
        }
    }
    this->generalData();
    this->resetPosition();
}

void AllianceInfoMembersView::acceptOnePlayer(CCObject* data){
    m_showData->removeAllObjects();
    CCString* uid = dynamic_cast<CCString*>(data);
    if(uid){
        std::vector<AllianceInfoMember> ::iterator it;
        int count = 0;
        for (it = m_appDatas.begin(); it!=m_appDatas.end(); it++) {
            if(it->getUid()==uid->getCString()){
                AllianceInfoMember infoMember;
                infoMember.setRank(1);
                infoMember.setName(it->getName());
                infoMember.setPoints(it->getPoints());
                infoMember.setKills(it->getKills());
                infoMember.setPointId(it->getPointId());
                infoMember.setMainCityLv(it->getMainCityLv());
                infoMember.setPic(it->getPic());
                infoMember.setUid(it->getUid());
                infoMember.setPower(it->getPower());
                infoMember.setAllianceId(m_allianceId);
                infoMember.setHead(it->getHead());
                infoMember.setIsManager(false);
                infoMember.setIsTitle(false);
                infoMember.setOnline(true);
                reverse(m_datas.begin(),m_datas.end());
                m_datas.push_back(infoMember);
                reverse(m_datas.begin(),m_datas.end());
                it->reset();
                break;
            }
            count+=1;
        }
    }
    this->generalData();
    this->resetPosition();
}

void AllianceInfoMembersView::updateShowData(CCObject* data){
    m_showData->removeAllObjects();
    auto dict = dynamic_cast<CCDictionary*>(data);
    if(dict){
        int rank = dict->valueForKey("rank")->intValue();
        int num = m_titles.size();
        for (int i=0; i<num; i++) {
            if(m_titles[i].getRank()==rank){
                bool open = m_titles[i].getOpen();
                m_titles[i].setOpen(!open);
                status[i] = m_titles[i].getOpen();
                break;
            }
        }
    }
    this->generalData();
    this->resetPosition();
}

void AllianceInfoMembersView::generalData(){
    m_showData->removeAllObjects();
    int titleNum = m_titles.size();
    for (int j=titleNum; j>0; j--) {
        CCArray* titleV = CCArray::create();
        int rank = m_titles[titleNum-j].getRank();
        string rankTitle = "";
//        switch (rank) {
//            case 1:
//                rankTitle = _lang("115100");
//                if(GlobalData::shared()->playerInfo.allianceInfo.strRank1!=""){
//                    rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank1;
//                }
//                m_titles[titleNum-j].setAllianceTitle(rankTitle);
//                break;
//            case 2:
//                rankTitle = _lang("115101");
//                if(GlobalData::shared()->playerInfo.allianceInfo.strRank2!=""){
//                    rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank2;
//                }
//                m_titles[titleNum-j].setAllianceTitle(rankTitle);
//                break;
//            case 3:
//                rankTitle = _lang("115102");
//                if(GlobalData::shared()->playerInfo.allianceInfo.strRank3!=""){
//                    rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank3;
//                }
//                m_titles[titleNum-j].setAllianceTitle(rankTitle);
//                break;
//            case 4:
//                rankTitle = _lang("115103");
//                if(GlobalData::shared()->playerInfo.allianceInfo.strRank4!=""){
//                    rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank4;
//                }
//                m_titles[titleNum-j].setAllianceTitle(rankTitle);
//                break;
//            case 5:
//                rankTitle = _lang("115104");
//                if(GlobalData::shared()->playerInfo.allianceInfo.strRank5!=""){
//                    rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank5;
//                }
//                m_titles[titleNum-j].setAllianceTitle(rankTitle);
//                break;
//            default:
//                break;
//        }
        int memberNum = getMemberNumByRank(rank);
        m_titles[titleNum-j].setAppNum(memberNum);
        titleV->addObject(&m_titles[titleNum-j]);
        m_showData->addObject(titleV);
        if(m_titles[titleNum-j].getOpen()){
            int num = m_datas.size();
            for (int i=0; i<num; i++) {
                int index = j;
                if (titleNum==6) {
                    index = index - 1;
                }
                AllianceInfoMember oneMember = m_datas[i];
                if(oneMember.getRank()==index){
                    CCArray* oneData = dynamic_cast<CCArray*>(m_showData->objectAtIndex(m_showData->count()-1));
                    if(oneData->count()==2){
                        CCArray* oneData = CCArray::create();
                        oneData->addObject(&m_datas[i]);
                        m_showData->addObject(oneData);
                    }else{
                        AllianceInfoMember* info = dynamic_cast<AllianceInfoMember*>(oneData->objectAtIndex(0));
                        if (info) {
                            if(info->getIsTitle()){
                                CCArray* newData = CCArray::create();
                                newData->addObject(&m_datas[i]);
                                m_showData->addObject(newData);
                            }else{
                                oneData->addObject(&m_datas[i]);
                                if(m_datas[i].getOnline() && !info->getOnline()){
                                    oneData->swap(0, 1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(titleNum==6){
        int appNum = this->getAppNumByRank(0);
        m_titles[5].setAppNum(appNum);
        if(m_titles[5].getOpen()){
            int num = m_appDatas.size();
            for (int i=0; i<num; i++) {
                if(m_appDatas[i].getRank()==0){
                    CCArray* oneData = dynamic_cast<CCArray*>(m_showData->objectAtIndex(m_showData->count()-1));
                    if(oneData->count()==2){
                        CCArray* oneData = CCArray::create();
                        oneData->addObject(&m_appDatas[i]);
                        m_showData->addObject(oneData);
                    }else{
                        AllianceInfoMember* info = dynamic_cast<AllianceInfoMember*>(oneData->objectAtIndex(0));
                        if (info) {
                            if(info->getIsTitle()){
                                CCArray* newData = CCArray::create();
                                newData->addObject(&m_appDatas[i]);
                                m_showData->addObject(newData);
                            }else{
                                oneData->addObject(&m_appDatas[i]);
                            }
                        }
                    }
                }
            }
        }
    }
//    if(GlobalData::shared()->playerInfo.isInAlliance() && m_allianceId==GlobalData::shared()->playerInfo.allianceInfo.uid){
//        AllianceInfoMember* buttonInfo = new AllianceInfoMember();
//        buttonInfo->setRank(-99);
//        buttonInfo->setAllianceTitle("");
//        buttonInfo->setIsTitle(true);
//        buttonInfo->setOpen(true);
//        
//        CCArray* buttonArray = CCArray::create();
//        buttonArray->addObject(buttonInfo);
//        m_showData->addObject(buttonArray);
//        buttonInfo->release();
//    }
}

int AllianceInfoMembersView::getMemberNumByRank(int rank){
    int num = 0;
    for (int i=0; i<m_datas.size(); i++) {
        if (m_datas[i].getRank()==rank) {
            num += 1;
        }
    }
    return num;
}

int AllianceInfoMembersView::getAppNumByRank(int rank){
    int num = 0;
    for (int i=0; i<m_appDatas.size(); i++) {
        if (m_appDatas[i].getRank()==rank) {
            num += 1;
        }
    }
    return num;
}

void AllianceInfoMembersView::updateMemberList(CCObject* data){
    //int num = m_members.size();
//    CCString* strUid = dynamic_cast<CCString*>(data);
//    if(strUid==NULL) return ;
//    std::string uid = strUid->getCString();
//    std::vector<AllianceInfoMember> ::iterator it;
//    for (it = m_members.begin(); it!=m_members.end(); it++) {
//        if(it->getUid()==uid){
//            m_members.erase(it);
//            break;
//        }
//    }
    m_tableView->reloadData();
}

void AllianceInfoMembersView::updateApplyList(CCObject* data){

}

void AllianceInfoMembersView::addLoadingAni(){
    if (m_loadingIcon==NULL) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_listContainer->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_listContainer->addChild(m_loadingIcon,1000000);
    }
}


CCSize AllianceInfoMembersView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_showData->count()){
        return CCSizeZero;
    }
    CCArray* arr = dynamic_cast<CCArray*>(m_showData->objectAtIndex(idx));
    if(arr && arr->count()>0){
        if(arr->count()==2){
            if (CCCommonUtils::isIosAndroidPad()) {
                return CELL_SIZE_WITHOUT_TITLE_HD;
            } else {
                return CELL_SIZE_WITHOUT_TITLE;
            }
        }else if(arr->count()==1){
            auto info = dynamic_cast<AllianceInfoMember*>(arr->objectAtIndex(0));
            if(info && info->getRank()==-99){
                return CCSize(640,390);
            }else if(info && info->getIsTitle()){
                if (CCCommonUtils::isIosAndroidPad()) {
                    return CELL_SIZE_WITH_TITLE_HD;
                } else {
                    return CELL_SIZE_WITH_TITLE;
                }
            }
            if (CCCommonUtils::isIosAndroidPad()) {
                return CELL_SIZE_WITHOUT_TITLE_HD;
            }else {
                return CELL_SIZE_WITHOUT_TITLE;
            }
        }
    }
    return CCSizeZero;
}

CCSize AllianceInfoMembersView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CELL_SIZE_WITHOUT_TITLE_HD;
    } else {
        return CELL_SIZE_WITHOUT_TITLE;
    }
}

CCTableViewCell* AllianceInfoMembersView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_showData->count()){
        return NULL;
    }
    AllianceInfoMembersCell* cell = (AllianceInfoMembersCell*)table->dequeueCell();
    CCArray* arr = dynamic_cast<CCArray*>(m_showData->objectAtIndex(idx));
    if(arr){
        if(cell==NULL){
            cell = AllianceInfoMembersCell::create(m_clickType);
        }
        if(arr->count()==2){
            auto info = dynamic_cast<AllianceInfoMember*>(arr->objectAtIndex(0));
            auto info2 = dynamic_cast<AllianceInfoMember*>(arr->objectAtIndex(1));
            cell->bind(info,info2);
        }else  if(arr->count()==1){
            auto info = dynamic_cast<AllianceInfoMember*>(arr->objectAtIndex(0));
            cell->bind(info,NULL);
        }
    }
    return cell;
}

ssize_t AllianceInfoMembersView::numberOfCellsInTableView(CCTableView *table)
{
    return m_showData->count();
}

void AllianceInfoMembersView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

//
bool AllianceInfoMembersCell::init()
{
    bool ret = false;
    
    if (CCTableViewCell::init()) {
        CCNode* node = CCBLoadFile("AllianceInfoMembersCell", this, this);
        m_rankSpr->setVisible(false);
        CCSize size = node->getContentSize();
        setContentSize(size);
        CCCommonUtils::setButtonTitle(m_btn1, _lang("115031").c_str());
        CCCommonUtils::setButtonTitle(m_btn2, _lang("115904").c_str());
        return true;
    }
    return ret;
}

void AllianceInfoMembersCell::onEnter(){
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
}

void AllianceInfoMembersCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool AllianceInfoMembersCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    if(isTouchInside(m_bg, pTouch)){
        return true;
    }
    return false;
}

void AllianceInfoMembersCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint end = pTouch->getLocation();
    if(tipFlag) return ;
    if(!isTouchInside(m_bg, pTouch)) return ;
    m_bg->stopAllActions();
    if(m_oneInfo && fabs(end.y - m_touchPos.y)<30 && m_oneInfo && m_oneInfo->getIsTitle() && m_oneInfo->getRank()!=-99){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        int rank = m_oneInfo->getRank();
        auto dic = CCDictionary::create();
        dic->setObject(CCString::createWithFormat("%ld",(long)end.x), "posX");
        dic->setObject(CCString::createWithFormat("%ld",(long)end.y), "posY");
        dic->setObject(CCString::create(CC_ITOA(rank)), "rank");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ALLIANCE_RANK_TITLE_CLICK,dic);
    }
}

void AllianceInfoMembersCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void AllianceInfoMembersCell::onBtnClick1(CCObject *pSender, CCControlEvent event){
    if(tipFlag) return ;
    MailController::getInstance()->openMailDialogViewFirst(_lang("105564"), GlobalData::shared()->playerInfo.uid);
}

void AllianceInfoMembersCell::onBtnClick2(CCObject *pSender, CCControlEvent event){
    if(tipFlag) return ;
    PopupViewController::getInstance()->addPopupInView(AllianceRankAttrView::create());
}

void AllianceInfoMembersCell::onBtnClick3(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(AllianceApplyView::create(GlobalData::shared()->playerInfo.allianceInfo.uid,true));
}

void AllianceInfoMembersCell::onBtnClick4(CCObject *pSender, CCControlEvent event){
    if(tipFlag) return ;
//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    if(GlobalData::shared()->playerInfo.allianceInfo.rank==5 && GlobalData::shared()->playerInfo.allianceInfo.currentNum>1){
        CCCommonUtils::flyHint("", "", _lang("115249"));
    }else if(GlobalData::shared()->playerInfo.allianceInfo.currentNum<=1 && GlobalData::shared()->playerInfo.allianceInfo.rank==5){
        YesNoDialog::showVariableTitle(_lang("115040").c_str(),CCCallFunc::create(this, callfunc_selector(AllianceInfoMembersCell::jieXianAlliance)),_lang("confirm").c_str());
    }else {
        YesNoDialog::showVariableTitle(_lang("115042").c_str(),CCCallFunc::create(this, callfunc_selector(AllianceInfoMembersCell::sendChatFirst)),_lang("confirm").c_str());
    }
}
void AllianceInfoMembersCell::sendChatFirst(){
//    CountryChatCommand * cmd = new CountryChatCommand(CHAT_STATE_ALLIANCE_COMMAND,_lang("115186").append("  (").append(_lang("115181")).append(")").c_str(),2);
//    cmd->sendAndRelease();
//    this->scheduleOnce(schedule_selector(AllianceInfoMembersCell::exitAlliance),0.3);
    exitAlliance(0.0f);
    m_btn4->setEnabled(false);
}
void AllianceInfoMembersCell::exitAlliance(float _time){
//    ChatController::getInstance()->sendCountryChat(_lang("115186").append("  (").append(_lang("115181")).append(")").c_str(), CHAT_ALLIANCE, 2);
    LeaveAllianceCommand * command = new LeaveAllianceCommand(_lang("115186").append("  (").append(_lang("115181")).append(")").c_str(),2);
    command->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInfoMembersCell::leavelSuccess), NULL));
    command->sendAndRelease();
}

void AllianceInfoMembersCell::leavelSuccess(CCObject* obj){
    UIComponent::getInstance()->changeChatChannel(CHAT_COUNTRY);
    PopupViewController::getInstance()->removeAllPopupView();
    ChatController::getInstance()->m_chatAlliancePool.clear();
   ChatController::getInstance()->m_latestAllianceMsg.sequenceId=0;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::resetPlayerIsInAlliance();
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    ChatServiceCocos2dx::resetPlayerIsInAlliance();
#endif
    CCUserDefault::sharedUserDefault()->setStringForKey(ALLIANCE_MEMBER_BTN_STATUS, "");
}

void AllianceInfoMembersCell::kickAlliance(){
    //    AllianceKickMemberCommand * command = new AllianceKickMemberCommand();
    //    command->putParam("playerId", CCString::create(m_info->getUid()));
    //    command->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInfoMembersCell::sendToServerSuccess), NULL));
    //    command->sendAndRelease();
}

void AllianceInfoMembersCell::jieXianAlliance(){
    DismissAllianceCommand * command = new DismissAllianceCommand();
    command->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInfoMembersCell::leavelSuccess), NULL));
    command->sendAndRelease();
    m_btn4->setEnabled(false);
}

SEL_CCControlHandler AllianceInfoMembersCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnClick1", AllianceInfoMembersCell::onBtnClick1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnClick2", AllianceInfoMembersCell::onBtnClick2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnClick4", AllianceInfoMembersCell::onBtnClick4);
    
    return NULL;
}

void AllianceInfoMembersCell::bind(AllianceInfoMember* oneInfo,AllianceInfoMember* twoInfo,bool buttonFlag)
{
    m_oneInfo = oneInfo;
    m_twoInfo = twoInfo;
    m_infoNode->removeAllChildrenWithCleanup(true);
    m_rankNode->setVisible(false);
    m_infoNode->setVisible(false);
    m_btnNode->setVisible(false);
    m_rankSpr->setVisible(false);
    if (m_oneInfo == NULL && m_twoInfo==NULL) {
        return ;
    }
    if(oneInfo->getRank()==-99){
        m_btnNode->setVisible(true);
        if(GlobalData::shared()->playerInfo.allianceInfo.currentNum<=1 && GlobalData::shared()->playerInfo.allianceInfo.rank==5){
            CCCommonUtils::setButtonTitle(m_btn4, _lang("115037").c_str());
        }else{
            CCCommonUtils::setButtonTitle(m_btn4, _lang("115039").c_str());
        }
        return ;
    }
    if(m_oneInfo->getIsTitle()){
        m_rankNode->setVisible(true);
        m_infoNode->setVisible(false);
        m_arrow->setRotation(m_oneInfo->getOpen()?90:0);
        m_titleTxt->setString(m_oneInfo->getAllianceTitle().c_str());
        m_numTxt->setString(CC_ITOA(m_oneInfo->getAppNum()));
        m_rankIcon->removeAllChildrenWithCleanup(true);
        CCSprite* spr = NULL;
        if(m_oneInfo->getRank()==5){
           spr = CCLoadSprite::createSprite(CCString::createWithFormat("icon_Alliance_0%d.png",m_oneInfo->getRank())->getCString());
//           spr->setScale(0.5);
        }else if(m_oneInfo->getRank()==0){
            std::string str = m_oneInfo->getAllianceTitle();
//            str.append("(");
//            str.append(CC_ITOA(m_oneInfo->getAppNum()));
//            str.append(")");
            m_titleTxt->setString(str.c_str());
            spr = CCLoadSprite::createSprite(CCString::createWithFormat("nb_0032.png")->getCString());
        }
        else
        {
            m_rankSpr->setVisible(true);
            spr = CCLoadSprite::createSprite(CCString::createWithFormat("nb_al_R_n%d.png",m_oneInfo->getRank())->getCString());
            spr->setPositionX(6);
        }
        if (spr) m_rankIcon->addChild(spr);
    }else if(m_oneInfo->getRank()==5){
        m_rankNode->setVisible(false);
        m_infoNode->setVisible(true);
        CCScale9Sprite* spr = CCLoadSprite::createScale9Sprite("nb_al_members_bg.png");//
        spr->setAnchorPoint(ccp(0, 0));
        spr->setPosition(CCPoint(0 + 10, -5));
        spr->setContentSize(CCSize(640 - 20, 235));
        m_infoNode->addChild(spr);
        AllianceOneMembersCell* oneCell = AllianceOneMembersCell::create(m_clickType);
//        oneCell->setScale(1.15);
        oneCell->setPosition(ccp(130, 0));
        if (CCCommonUtils::isIosAndroidPad()) {
            spr->setPreferredSize(CCSize(1536 - 20, 430 - 10));
            spr->setPosition(0 + 10, -25 + 5);
//            oneCell->setScale(1.1);
            oneCell->setPosition(ccp(371, 0));
        }
        oneCell->setData(m_oneInfo);
        m_infoNode->addChild(oneCell);
//        CCParticleBatchNode *batch = ParticleController::createParticleBatch();
//        batch->setPosition(ccp(130+260,100));
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            batch->setScale(2);
//            batch->setPosition(ccp(931, 153));
//        }
//        m_infoNode->addChild(batch);
//        string tmpStart = "ScienceGlow_";
//        int count = 4;
//        for (int i=2; i<count; i++) {
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
//            batch->addChild(particle);
//        }
        
    }else{
        m_rankNode->setVisible(false);
        m_infoNode->setVisible(true);
        CCScale9Sprite* spr = CCLoadSprite::createScale9Sprite("nb_al_members_bg.png");
        spr->setAnchorPoint(ccp(0, 0));
        spr->setPosition(CCPoint(0 + 10, -5));
        spr->setContentSize(CCSize(640 - 20, 235));
        if (CCCommonUtils::isIosAndroidPad()) {
            spr->setPosition(CCPoint(0, -25));
            spr->setPreferredSize(CCSize(1536, 430));
        }
        m_infoNode->addChild(spr);
        
        AllianceOneMembersCell* oneCell = AllianceOneMembersCell::create(m_clickType);
        oneCell->setData(m_oneInfo);
        oneCell->setPosition(ccp(10, 10));
        m_infoNode->addChild(oneCell);
        if (CCCommonUtils::isIosAndroidPad()) {
            oneCell->setPosition(ccp(95, 20));
        }
        if(m_twoInfo){
            AllianceOneMembersCell* TwoCell = AllianceOneMembersCell::create(m_clickType);
            TwoCell->setPosition(ccp(320,10));
            if (CCCommonUtils::isIosAndroidPad()) {
                TwoCell->setPosition(ccp(783, 20));
            }
            TwoCell->setData(m_twoInfo);
            m_infoNode->addChild(TwoCell);
        }
    }
}

bool AllianceInfoMembersCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankNode", CCNode*, this->m_rankNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIFTTF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCSprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrow", CCNode*, this->m_arrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankIcon", CCNode*, this->m_rankIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankSpr", CCNode*, this->m_rankSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1", CCControlButton*, this->m_btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2", CCControlButton*, this->m_btn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn4", CCControlButton*, this->m_btn4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTxt", CCLabelIF*, this->m_numTxt);
    
    return false;
}
//
bool AllianceOneMembersCell::init()
{
    bool ret = true;
    
    if (CCNode::init()) {
        CCNode* node = CCBLoadFile("NewAllianceMemberCell", this, this);
        CCSize size = node->getContentSize();
        setContentSize(size);
        
        m_nodeHide[0]->setVisible(false);
        
        std::string lang = cocos2d::extension::CCDevice::getLanguage();
//        if (lang == "en" || lang == "en-US")
//        {
//            m_powerTxt->setFntFile("pve_fnt_title.fnt");
//        }
        return true;
    }
    
    return ret;
}

void AllianceOneMembersCell::onEnter(){
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
}

void AllianceOneMembersCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool AllianceOneMembersCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    if(isTouchInside(m_clickNode1, pTouch) || isTouchInside(m_clickNode2, pTouch)){
        return true;
    }
    return false;
}

void AllianceOneMembersCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint end = pTouch->getLocation();
    if(tipFlag) return ;
    if((isTouchInside(m_clickNode1, pTouch) || isTouchInside(m_clickNode2, pTouch)) && fabs(end.y - m_touchPos.y)<30){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        //PopupViewController::getInstance()->addPopupView(ManagerMemberView::create(m_info));
        if (m_clickType == 0) {
            CCPoint pos = m_headIcon->getParent()->convertToWorldSpace(m_headIcon->getPosition());
            PopupViewController::getInstance()->addPopupView(AllianceManagerFunView::create(m_info,pos),false,false);
        } else {
            if (m_info->getUid() == GlobalData::shared()->playerInfo.uid) {
                CCCommonUtils::flyHint("", "", _lang("115294"));
                return;
            }
            YesNoDialog::show(_lang("115287").c_str(), CCCallFunc::create(this, callfunc_selector(AllianceOneMembersCell::inviteTeleport)));
        }
    }
}

void AllianceOneMembersCell::inviteTeleport()
{
    AllianceManager::getInstance()->startInviteTeleport(m_info->getUid(), m_info->getAllianceId(), AllianceManager::getInstance()->tmpCityIndex);
}

void AllianceOneMembersCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

SEL_CCControlHandler AllianceOneMembersCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

void AllianceOneMembersCell::setData(AllianceInfoMember* member)
{
    m_info = member;
    if(m_info==NULL) return ;
    m_headIcon->removeAllChildrenWithCleanup(true);
    CCSprite* spr = CCLoadSprite::createSprite(m_info->getPic().c_str());
    spr->setAnchorPoint({0.5, 0});
    spr->setScale(.5);
    m_headIcon->addChild(spr);
    m_nameTxt->setString(m_info->getName().c_str());
//    m_powerTxt->setString(_lang("102163").c_str());
    string lv = "LV."; lv.append(CC_ITOA(m_info->getUserLevel()));
    m_powerTxt->setString(lv);
    m_powerValue->setString(CC_CMDITOAL(m_info->getPower()));
//    if(m_info->getOnline()){
//        m_powerValue->setColor({255,235,191});
//        std::string lang = cocos2d::extension::CCDevice::getLanguage();
//        if (lang=="en") {
//            m_powerTxt->setColor({255,255,255});
//        }else{
//            m_powerTxt->setColor({239,211,0});
//        }
//    }else{
//        m_powerTxt->setColor({172,172,172});
//        m_powerValue->setColor({172,172,172});
//    }
    
    m_titleFlag->setVisible(m_info->getRank()>1);
    if(m_info->getRank()>1){
        m_titleFlag->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("icon_Alliance_0%d.png",m_info->getRank())->getCString()));
    }
//    m_flagBg->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("Alliance_Flag_0%d.png",m_info->getRank())->getCString()));
    m_offLineTime->setString("");
//    m_onLineBg->setVisible(false);
    if(m_info->getOnline()|| m_info->getRank()==0){
        int rank = m_info->getRank();
        rank = MAX(rank, 1);
//        m_flagBg->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("Alliance_Flag_0%d.png",rank)->getCString()));
        if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->getAllianceId()){
//            m_onLineBg->setVisible(true);
            m_offLineTime->setString(_lang("115242"));
//            m_offLineTime->setColor({110,222,0});
        }else{
//            m_onLineBg->setVisible(false);
            m_offLineTime->setString("");
//            m_offLineTime->setColor({110,222,0});
        }
    }else{
        if (m_info->getRank()==5) {
//            m_flagBg->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("Alliance_Flag_Grey_05.png")->getCString()));
        }else{
//            m_flagBg->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("Alliance_Flag_Gre.png")->getCString()));
        }
        auto dt = GlobalData::shared()->getWorldTime()-GlobalData::shared()->changeTime(m_info->getOffLineTime()/1000);
        dt = dt;
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
        timestr.append(" ");
        timestr.append(_lang("105593"));
        this->m_offLineTime->setString(timestr);
//        m_offLineTime->setColor({172,172,172});
    }
    
}

bool AllianceOneMembersCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headIcon", CCNode*, this->m_headIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_offLineTime", CCLabelIF*, this->m_offLineTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerValue", CCLabelIF*, this->m_powerValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleFlag", CCSprite*, this->m_titleFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagBg", CCSprite*, this->m_flagBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode1", CCNode*, this->m_clickNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode2", CCNode*, this->m_clickNode2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_onLineBg", CCNode*, this->m_onLineBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeHide1", CCNode*, this->m_nodeHide[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeHide2", CCNode*, this->m_nodeHide[1]);
    
    return false;
}


