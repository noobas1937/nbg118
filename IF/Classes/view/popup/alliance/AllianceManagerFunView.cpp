//
//  AllianceManagerFunView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-12.
//
//

#include "AllianceManagerFunView.h"
#include "PopupViewController.h"
#include "CCLoadSprite.h"
#include "PopupViewController.h"
#include "MailWritePopUpView.h"
#include "SetAllianceRankCommand.h"
#include "AllianceKickMemberCommand.h"
#include "AllianceLeaderTransCommand.h"
#include "AllianceResourceTradeView.h"
#include "PlayerInfo.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "ParticleController.h"
#include "ChatController.h"
#include "RefuseAllianceApplyCommand.h"
#include "AcceptAllianceApplyCommand.h"
#include "MailController.h"
#include "GameController.h"
#include "AllianceLeaderReplaceCommand.h"
#include "KingdomKingChooseCommand.h"
#include "WorldController.h"
#include "SceneController.h"
#include "InvitesAllianceCommand.h"
#include "ToolController.h"
#include "AllianceTerritoryShowCommand.h"
#include "RoleInfoView.h"
#include "AllianceApplyInfoView.h"
#include "AllianceApplyMessageView.h"
AllianceManagerFunView *AllianceManagerFunView::create(AllianceInfoMember* info,CCPoint pos,bool invite){
    AllianceManagerFunView *ret = new AllianceManagerFunView(info,pos,invite);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceManagerFunView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceManagerFunView::checkIfStorage), CHECKIFSTORAGE_DATA_BACK, NULL);
}

void AllianceManagerFunView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALERT_ALLIANCE_VIEW_CLOSE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CHECKIFSTORAGE_DATA_BACK);
    PopupBaseView::onExit();
}

bool AllianceManagerFunView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        this->setModelLayerOpacity(220);
        if(SceneController::getInstance()->currentSceneId!=SCENE_ID_WORLD){
            CCLoadSprite::doResourceByPathIndex(WORLD_PATH, 2,true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByPathIndex(WORLD_PATH, 2,false);
            });
        }
        auto node = CCBLoadFile("AllianceManagerFunView", this, this);
        this->setContentSize(node->getContentSize());
        
        m_bodyIcon = CCLoadSprite::createSprite(m_info->getPic().c_str());
        m_bodyIcon->setScale(0.8);
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            m_bodyIcon->setScale(1.6);
//        }
        CCPoint pos = m_icon->convertToNodeSpace(m_fromPos);
        m_bodyIcon->setPosition(pos);
        m_icon->addChild(m_bodyIcon);
    
        m_rankUp->setVisible(false);
        m_rankDown->setVisible(false);
        m_node1->setVisible(false);
        m_node2->setVisible(false);
        m_node3->setVisible(false);
        m_node4->setVisible(false);
        m_node5->setVisible(false);
        m_node6->setVisible(false);
        m_node7->setVisible(false);
        m_node8->setVisible(false);
        m_node11->setVisible(false);
        if(m_info->getRank()>0){
            m_rankUp->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("icon_Alliance_0%d.png",m_info->getRank())->getCString()));
            m_rankDown->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("icon_Alliance_0%d.png",m_info->getRank())->getCString()));
        }
        
        CCSequence* sc1 = CCSequence::create(CCDelayTime::create(0.1),CCMoveTo::create(0.35, ccp(0,0)),CCCallFuncO::create(this, callfuncO_selector(AllianceManagerFunView::playFadeIn), NULL),NULL);
        CCSequence* sc2 = CCSequence::create(CCDelayTime::create(0.05),CCScaleTo::create(0.4, 1.3),NULL);
        CCSpawn* sw = CCSpawn::create(sc1,sc2,NULL);
        m_bodyIcon->runAction(sw);

        m_playingFadeOut = false;
        ret = true;
    }
    return ret;
}

void AllianceManagerFunView::playFadeIn(CCObject* p){
    this->refresh();
    //this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(AllianceManagerFunView::playFadeInFinish));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
    //this->scheduleOnce(schedule_selector(AllianceManagerFunView::playFadeInFinish), 0.25);
}

void AllianceManagerFunView::playFadeInFinish(){
    this->stopAllActions();
    this->refresh();
}

void AllianceManagerFunView::playFadeOut(){
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
    this->scheduleOnce(schedule_selector(AllianceManagerFunView::playFadeOutFinish), 0.12);
}

void AllianceManagerFunView::playFadeOutFinish(float t){
    m_bodyIcon->stopAllActions();
    CCPoint pos = m_icon->convertToNodeSpace(m_fromPos);
    CCSequence* sc1 = CCSequence::create(CCDelayTime::create(0.05),CCMoveTo::create(0.4, pos),CCDelayTime::create(0.05),CCCallFuncO::create(this, callfuncO_selector(AllianceManagerFunView::close), NULL),NULL);
    CCSequence* sc2 = nullptr;
    if (!CCCommonUtils::isIosAndroidPad()) {
        sc2 = CCSequence::create(CCDelayTime::create(0.05),CCScaleTo::create(0.4, 0.4),NULL);
    } else {
        sc2 = CCSequence::create(CCDelayTime::create(0.05),CCScaleTo::create(0.4, 0.4),NULL);
    }
    CCSpawn* sw = CCSpawn::create(sc1,sc2,NULL);
    m_bodyIcon->runAction(sw);
}

void AllianceManagerFunView::refresh(){
    m_buttonIcons.clear();
    m_buttonNames.clear();
    m_buttonFuns.clear();
    m_buttonNodes.clear();
    m_buttonState.clear();
    if(m_info->getRank()>0){
        m_rankUp->setVisible(true);
        m_rankDown->setVisible(true);
    }else{
        m_rankUp->setVisible(false);
        m_rankDown->setVisible(false);
    }
    
    m_node1->setVisible(false);
    m_node2->setVisible(false);
    m_node3->setVisible(false);
    m_node4->setVisible(false);
    m_node5->setVisible(false);
    m_node6->setVisible(false);
    m_node7->setVisible(false);
    m_node8->setVisible(false);
    m_node11->setVisible(false);
    m_nameTxt->setString(m_info->getName().c_str());
//    m_node1->setPosition(ccp(170, 139));
//    m_node2->setPosition(ccp(218, -12));
//    m_node3->setPosition(ccp(189, -180));
    if(m_invite){
        m_node1->setVisible(false);
        m_node2->setVisible(false);
        m_node3->setVisible(false);
        m_node4->setVisible(false);
        m_node5->setVisible(false);
        m_node6->setVisible(false);
        m_node7->setVisible(false);
        m_node8->setVisible(false);
        m_node11->setVisible(false);
        CCSprite* spr = CCLoadSprite::createSprite("icon_person_invite.png");
        m_btnNode11->removeAllChildrenWithCleanup(true);
        m_btnNode11->addChild(spr);
        m_nameText11->setString(_lang("115178"));
        setButtonState(m_node11,ButtonInviteUser);
        m_node11->setVisible(true);
        return ;
    }else if(m_info->getRank()==0){
        m_node1->setVisible(true);
        m_node2->setVisible(true);
        m_node3->setVisible(true);
        m_node4->setVisible(true);
        m_node5->setVisible(false);
        m_node6->setVisible(false);
        m_node7->setVisible(true);
        m_node8->setVisible(false);
        m_node11->setVisible(false);
        
//        m_node2->setPosition(ccp(210, 67));
//        m_node3->setPosition(ccp(210, -67.0));
        auto spr = CCLoadSprite::createSprite("tile_pop_icon2.png");
        m_btnNode1->addChild(spr);
        m_nameText1->setString(_lang("108552"));
        setButtonState(m_node1, ButtonPlayerInfo);
        
        spr = CCLoadSprite::createSprite("mail_01.png");
        m_btnNode2->addChild(spr);
        m_nameText2->setString(_lang("105513"));
        setButtonState(m_node2, ButtonMail);
        
        spr = CCLoadSprite::createSprite("accpet_in_alliance.png");
        m_btnNode3->addChild(spr);
        m_nameText3->setString(_lang("115044"));
        setButtonState(m_node3,ButtonAccept);
        
        spr = CCLoadSprite::createSprite("alliance_refuse.png");
        m_btnNode4->addChild(spr);
        m_nameText4->setString(_lang("115045"));
        setButtonState(m_node4,ButtonRefuse);
        
        spr = CCLoadSprite::createSprite("icon_al_applymessage.png");
        if (m_info->getApplyMessage() == "") {
            CCCommonUtils::setSpriteGray(spr, true);
        }
        m_btnNode7->addChild(spr);
        m_nameText7->setString(_lang("115502"));
        setButtonState(m_node7, ButtonApplyMessage);
        return ;
    }

    int rank = GlobalData::shared()->playerInfo.allianceInfo.rank;
    if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.uid == m_info->getUid()){
        m_node1->setVisible(false);
        m_node2->setVisible(false);
        m_node3->setVisible(false);
        m_node4->setVisible(false);
        m_node5->setVisible(false);
        m_node6->setVisible(false);
        m_node7->setVisible(false);
        m_node8->setVisible(false);
        if(GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->getAllianceId() && GlobalData::shared()->playerInfo.allianceInfo.rank ==5 && GlobalData::shared()->playerInfo.relinquishEndTime > WorldController::getInstance()->getTime()){
            CCSprite* spr = CCLoadSprite::createSprite("icon_216000.png");
            m_btnNode9->removeAllChildrenWithCleanup(true);
            m_btnNode9->addChild(spr);
            m_nameText9->setString(_lang("110016"));
            setButtonState(m_node9,ButtonRelinquish);
            m_node9->setVisible(true);
        }else{
            m_node9->setVisible(false);
        }
        return ;
    }
    CCSprite* spr = NULL;
    if(!GlobalData::shared()->playerInfo.isInAlliance() || m_info->getAllianceId()!= GlobalData::shared()->playerInfo.allianceInfo.uid){
        spr = CCLoadSprite::createSprite("allianceFunMail.png");
        m_btnNode3->removeAllChildrenWithCleanup(true);
        m_btnNode3->addChild(spr);
        m_nameText3->setString(_lang("115107"));
        setButtonState(m_node3,ButtonMail);
        rank = -1;
    }else{
        spr = CCLoadSprite::createSprite("allianceFunMail.png");
        m_btnNode2->removeAllChildrenWithCleanup(true);
        m_btnNode2->addChild(spr);
        m_nameText2->setString(_lang("115107"));
        setButtonState(m_node2,ButtonMail);
        
        int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_TAVERN);
        bool haveMarket = false;
        if (buildId>0) {
            FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(buildId);
            if(&info!=NULL){
                haveMarket = true;
            }
        }
        spr = CCLoadSprite::createSprite("allianceTrade_icon.png");
        m_btnNode3->removeAllChildrenWithCleanup(true);
        m_btnNode3->addChild(spr);
        m_nameText3->setString(_lang("108729"));
        setButtonState(m_node3,ButtonTrade);
        if(!haveMarket){
            CCCommonUtils::setSpriteGray(spr, true);
        }
        
        spr = CCLoadSprite::createSprite("allianceYuanYun.png");
        m_btnNode4->removeAllChildrenWithCleanup(true);
        m_btnNode4->addChild(spr);
        m_nameText4->setString(_lang("115151"));
        setButtonState(m_node4,ButtonRallyState);
    }
    
    if (GlobalData::shared()->playerInfo.isInAlliance() && m_info->getAllianceId() == GlobalData::shared()->playerInfo.allianceInfo.uid && rank >= 3 && GlobalData::shared()->playerInfo.uid != m_info->getUid() && m_info->getRank() >= 1) {
        spr = CCLoadSprite::createSprite("tile_pop_icon22.png");
        m_btnNode1->removeAllChildrenWithCleanup(true);
        m_btnNode1->addChild(spr);
        m_nameText1->setString(_lang("115285"));
        setButtonState(m_node1, ButtonPopInviteTeleport);
    }
    
    if(m_info->getRank()==5 && rank!=5 && m_info->getAllianceId()==GlobalData::shared()->playerInfo.allianceInfo.uid && rank > 1){
        m_buttonIcons.push_back("allianceTranLeader.png");
        m_buttonNames.push_back(_lang("115251"));
        m_buttonFuns.push_back(ButtonReplaceLeader);
    }
    switch (rank) {
        case 5:
        {
            m_buttonIcons.push_back("allianceTranLeader.png");
            m_buttonNames.push_back(_lang("115108"));
            m_buttonFuns.push_back(ButtonTranLeader);

            
            if(m_info->getRank()!=4){
                addButton(PROMOTE_MEMBERS,rank);
            }
            if(m_info->getRank()!=1){
                addButton(DEMOTE_MEMBERS,rank);
            }
            addButton(KICK_MEMBERS,rank);
        }
            break;
        case 4:
        {
            if(m_info->getRank()<3){
                addButton(PROMOTE_MEMBERS,rank);
            }
            
            if(m_info->getRank()>1 && m_info->getRank()<=3){
                addButton(DEMOTE_MEMBERS,rank);
            }
            if (m_info->getRank()<=3) {
                addButton(KICK_MEMBERS,rank);
            }
        }
            break;
        case 3:
        {
            if(m_info->getRank()<2){
                addButton(PROMOTE_MEMBERS,rank);
            }
            
            if(m_info->getRank()>1 && m_info->getRank()<=2){
                addButton(DEMOTE_MEMBERS,rank);
            }
            if (m_info->getRank()<=2) {
                addButton(KICK_MEMBERS,rank);
            }
        }
            break;
        case 2:
        {
            if (m_info->getRank()<2) {
                 addButton(KICK_MEMBERS,rank);
            }
        }
            break;
        default:
            break;
    }
    int num = m_buttonIcons.size();
    switch (num) {
        case 4:
        {
            spr = CCLoadSprite::createSprite(m_buttonIcons[0].c_str());
            m_btnNode5->removeAllChildrenWithCleanup(true);
            m_btnNode5->addChild(spr);
            m_nameText5->setString(m_buttonNames[0]);
            setButtonState(m_node5,m_buttonFuns[0]);
            
            spr = CCLoadSprite::createSprite(m_buttonIcons[1].c_str());
            m_btnNode6->removeAllChildrenWithCleanup(true);
            m_btnNode6->addChild(spr);
            m_nameText6->setString(m_buttonNames[1]);
            setButtonState(m_node6,m_buttonFuns[1]);
            
            spr = CCLoadSprite::createSprite(m_buttonIcons[2].c_str());
            m_btnNode7->removeAllChildrenWithCleanup(true);
            m_btnNode7->addChild(spr);
            m_nameText7->setString(m_buttonNames[2]);
            setButtonState(m_node7,m_buttonFuns[2]);
            
            spr = CCLoadSprite::createSprite(m_buttonIcons[3].c_str());
            m_btnNode8->removeAllChildrenWithCleanup(true);
            m_btnNode8->addChild(spr);
            m_nameText8->setString(m_buttonNames[3]);
            setButtonState(m_node8,m_buttonFuns[3]);
        }
            break;
        case 3:
        {
            spr = CCLoadSprite::createSprite(m_buttonIcons[0].c_str());
            m_btnNode6->removeAllChildrenWithCleanup(true);
            m_btnNode6->addChild(spr);
            m_nameText6->setString(m_buttonNames[0]);
            setButtonState(m_node6,m_buttonFuns[0]);
            
            spr = CCLoadSprite::createSprite(m_buttonIcons[1].c_str());
            m_btnNode7->removeAllChildrenWithCleanup(true);
            m_btnNode7->addChild(spr);
            m_nameText7->setString(m_buttonNames[1]);
            setButtonState(m_node7,m_buttonFuns[1]);
            
            spr = CCLoadSprite::createSprite(m_buttonIcons[2].c_str());
            m_btnNode8->removeAllChildrenWithCleanup(true);
            m_btnNode8->addChild(spr);
            m_nameText8->setString(m_buttonNames[2]);
            setButtonState(m_node8,m_buttonFuns[2]);
        }
            break;
        case 2:
        {
            spr = CCLoadSprite::createSprite(m_buttonIcons[0].c_str());
            m_btnNode6->removeAllChildrenWithCleanup(true);
            m_btnNode6->addChild(spr);
            m_nameText6->setString(m_buttonNames[0]);
            setButtonState(m_node6,m_buttonFuns[0]);
            
            spr = CCLoadSprite::createSprite(m_buttonIcons[1].c_str());
            m_btnNode7->removeAllChildrenWithCleanup(true);
            m_btnNode7->addChild(spr);
            m_nameText7->setString(m_buttonNames[1]);
            setButtonState(m_node7,m_buttonFuns[1]);
        }
            break;
        case 1:
        {
            spr = CCLoadSprite::createSprite(m_buttonIcons[0].c_str());
            m_btnNode7->removeAllChildrenWithCleanup(true);
            m_btnNode7->addChild(spr);
            m_nameText7->setString(m_buttonNames[0]);
            setButtonState(m_node7,m_buttonFuns[0]);
        }
            break;
    }
    num = m_buttonNodes.size();
    for (int i=0; i<num; i++) {
         m_buttonNodes[i]->setVisible(true);
    }
    
//    spr = CCLoadSprite::createSprite("allianceTrade_icon.png");
//    m_btnNode9->removeAllChildrenWithCleanup(true);
//    m_btnNode9->addChild(spr);
//    m_nameText9->setString(_lang("110016"));
//    setButtonState(m_node9,ButtonRelinquish);
//    m_node9->setVisible(true);
    
    //&& GlobalData::shared()->playerInfo.uid != m_info->getUid() GlobalData::shared()->playerInfo.officer==KINGDOM_KING_ID
    //没有任命过，盟主可以任命为成员里的任何人
    if(!GlobalData::shared()->isCrossService && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->getAllianceId() && GlobalData::shared()->playerInfo.allianceInfo.rank==5 && GlobalData::shared()->playerInfo.relinquishEndTime > WorldController::getInstance()->getTime()){
        spr = CCLoadSprite::createSprite("icon_216000.png");
        m_btnNode9->removeAllChildrenWithCleanup(true);
        m_btnNode9->addChild(spr);
        m_nameText9->setString(_lang("110016"));
        setButtonState(m_node9,ButtonRelinquish);
        m_node9->setVisible(true);
    }else{
        m_node9->setVisible(false);
    }
    
    bool hasTool = false;
    auto iter= ToolController::getInstance()->m_toolInfos.find(ITEM_SEND_GIFT);
    if(iter != ToolController::getInstance()->m_toolInfos.end()){
        auto &tInfo = ToolController::getInstance()->getToolInfoById(ITEM_SEND_GIFT);
        if(tInfo.getCNT()>0){
            hasTool =true;
        }
    }
    if(hasTool){
        CCSprite *gSpr  = CCLoadSprite::createSprite("item012.png");
        CCCommonUtils::setSpriteMaxSize(gSpr, 80);
        m_btnNode10->removeAllChildrenWithCleanup(true);
        m_btnNode10->addChild(gSpr);
        m_nameText10->setString(_lang("104366"));
        setButtonState(m_node10,ButtonSendGift);
        m_node10->setVisible(true);
    }else{
        m_node10->setVisible(false);
    }
    
    //
    if(CCCommonUtils::isTestPlatformAndServer("al_order") && rank>=4 && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->getAllianceId()
       && GlobalData::shared()->playerInfo.uid != m_info->getUid() && m_info->getRank() <=4 ){
        CCSprite* spr = CCLoadSprite::createSprite("al_order_icon.png");
        m_btnNode11->removeAllChildrenWithCleanup(true);
        m_btnNode11->addChild(spr);
        m_nameText11->setString(_lang("115429"));
        setButtonState(m_node11,ButtonOrder);
        m_node11->setVisible(true);
    }else{
        m_node11->setVisible(false);
    }
}

void AllianceManagerFunView::addButton(RankType type,int rank){
    switch (type) {
        case KICK_MEMBERS:
        {
            if (AllianceManager::getInstance()->checkRight(KICK_MEMBERS, rank)) {
                m_buttonIcons.push_back("alliacne_kickOut.png");
                m_buttonNames.push_back(_lang("115111"));
                m_buttonFuns.push_back(ButtonKickOut);
            }
        }
            break;
        case PROMOTE_MEMBERS:
        {
            if (AllianceManager::getInstance()->checkRight(PROMOTE_MEMBERS, rank)) {
                m_buttonIcons.push_back("AllianceUpRank.png");
                m_buttonNames.push_back(_lang("115109"));
                m_buttonFuns.push_back(ButtonUpRank);
            }
        }
            break;
        case DEMOTE_MEMBERS:
        {
            if (AllianceManager::getInstance()->checkRight(DEMOTE_MEMBERS, rank)) {
                m_buttonIcons.push_back("allianceDownRank.png");
                m_buttonNames.push_back(_lang("115110"));
                m_buttonFuns.push_back(ButtonDownRank);
            }
        }
            break;
    }
}

void AllianceManagerFunView::setButtonState(CCNode* node,AllianceButtonState state){
    m_buttonNodes.push_back(node);
    m_buttonState.push_back(state);
}

bool AllianceManagerFunView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", CCNode*, this->m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode1", CCNode*, this->m_btnNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText1", CCLabelIF*, this->m_nameText1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", CCNode*, this->m_node2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode2", CCNode*, this->m_btnNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText2", CCLabelIF*, this->m_nameText2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node3", CCNode*, this->m_node3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode3", CCNode*, this->m_btnNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText3", CCLabelIF*, this->m_nameText3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node4", CCNode*, this->m_node4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode4", CCNode*, this->m_btnNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText4", CCLabelIF*, this->m_nameText4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node5", CCNode*, this->m_node5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode5", CCNode*, this->m_btnNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText5", CCLabelIF*, this->m_nameText5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node6", CCNode*, this->m_node6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode6", CCNode*, this->m_btnNode6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText6", CCLabelIF*, this->m_nameText6);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node7", CCNode*, this->m_node7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode7", CCNode*, this->m_btnNode7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText7", CCLabelIF*, this->m_nameText7);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node8", CCNode*, this->m_node8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode8", CCNode*, this->m_btnNode8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText8", CCLabelIF*, this->m_nameText8);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node9", CCNode*, this->m_node9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode9", CCNode*, this->m_btnNode9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText9", CCLabelIF*, this->m_nameText9);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node10", CCNode*, this->m_node10);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode10", CCNode*, this->m_btnNode10);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText10", CCLabelIF*, this->m_nameText10);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node11", CCNode*, this->m_node11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode11", CCNode*, this->m_btnNode11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText11", CCLabelIF*, this->m_nameText11);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick1", CCNode*, this->m_nodeClick1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick2", CCNode*, this->m_nodeClick2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick3", CCNode*, this->m_nodeClick3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick4", CCNode*, this->m_nodeClick4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick5", CCNode*, this->m_nodeClick5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick6", CCNode*, this->m_nodeClick6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick7", CCNode*, this->m_nodeClick7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick8", CCNode*, this->m_nodeClick8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick9", CCNode*, this->m_nodeClick9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick10", CCNode*, this->m_nodeClick10);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick11", CCNode*, this->m_nodeClick11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_parNode", CCNode*, this->m_parNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_parNode2", CCNode*, this->m_parNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankUp", CCSprite*, this->m_rankUp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankDown", CCSprite*, this->m_rankDown);
    return false;
}

bool AllianceManagerFunView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void AllianceManagerFunView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

    if(m_playingFadeOut) return ;
    int index = 99;
    int num = m_buttonNodes.size();
    for (int i=0; i<num; i++) {
        CCNode* clickNode = m_buttonNodes[i]->getChildByTag(88);
        if (isTouchInside(clickNode, pTouch) && clickNode->isVisible() && m_buttonNodes[i]->isVisible()) {
            index = i;
            break;
        }
    }
    if(index==99 && !isTouchInside(m_clickNode, pTouch)){
        this->playFadeOut();
        m_playingFadeOut = true;
        m_node9->setVisible(false);
        m_node10->setVisible(false);
        return ;
    }
    int rank = 0;
    SetAllianceRankCommand* cmdRank = NULL;
    if (index!=99) {
        switch (m_buttonState[index]) {
            case ButtonMail:
                //PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create(m_info->getName(), ""));
                PopupViewController::getInstance()->removePopupView(this);
                MailController::getInstance()->openMailDialogViewFirst(m_info->getName(), m_info->getUid());

                break;
            case ButtonTrade:
            {
                int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_TAVERN);
                bool haveMarket = false;
                if (buildId>0) {
                    FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(buildId);
                    if(&info!=NULL){
                        haveMarket = true;
                    }
                }
                if(haveMarket){
                    PlayerInfo* playerInfo = new PlayerInfo();
                    playerInfo->name = m_info->getName();
                    playerInfo->uid = m_info->getUid();
                    playerInfo->power = m_info->getPower();
                    playerInfo->pointId = m_info->getPointId();
                    playerInfo->mainCityLv = m_info->getMainCityLv();
                    playerInfo->pic = m_info->getHead();
                    
                    double now = GlobalData::shared()->getTimeStamp();
                    double playingTime = (now - GlobalData::shared()->playerInfo.registerTime) / 3600;
                    if (GlobalData::shared()->cnt_forbidden_swith == 1) {
                        if (playingTime <= GlobalData::shared()->cnt_forbidden_playingtime) {
                            YesNoDialog::showVariableTitle(_lang("108793").c_str(),CCCallFunc::create(this, callfunc_selector(YesNoDialog::closeSelf)),_lang("confirm").c_str());
                        }
                        else {
                            AllianceManager::getInstance()->openAllianceTradeView(m_info->getUid(),m_info->getMainCityLv(),m_info->getPointId());
//                            PopupViewController::getInstance()->addPopupInView(AllianceResourceTradeView::create(playerInfo));
                        }
                    }
                    else {
                        AllianceManager::getInstance()->openAllianceTradeView(m_info->getUid(),m_info->getMainCityLv(),m_info->getPointId());
//                        PopupViewController::getInstance()->addPopupInView(AllianceResourceTradeView::create(playerInfo));
                    }
                    
                    //PopupViewController::getInstance()->addPopupInView(AllianceResourceTradeView::create(playerInfo));
                    playerInfo->release();
                }else{
                    CCCommonUtils::flyHint("", "", _lang("115075"));
                }
                PopupViewController::getInstance()->removePopupView(this);
            }
                break;
            case ButtonRallyState:
                AllianceManager::getInstance()->openYuanYunView(m_info->getPointId());
                PopupViewController::getInstance()->removePopupView(this);
                break;
            case ButtonTranLeader:
            {
                YesNoDialog::showVariableTitle(_lang_1("115112",m_info->getName().c_str()),CCCallFunc::create(this, callfunc_selector(AllianceManagerFunView::leaderTransFun)),_lang("confirm").c_str());
            }
                break;
            case ButtonUpRank:
            {
                rank = m_info->getRank() + 1;
                YesNoDialog::showVariableTitle(_lang_3("115114", m_info->getName().c_str(), CC_ITOA(m_info->getRank()),CC_ITOA(rank)),CCCallFunc::create(this, callfunc_selector(AllianceManagerFunView::upRankFun)),_lang("confirm").c_str());
            }
                break;
            case ButtonDownRank:
            {
                rank = m_info->getRank() - 1;
                YesNoDialog::showVariableTitle(_lang_3("115114", m_info->getName().c_str(), CC_ITOA(m_info->getRank()),CC_ITOA(rank)),CCCallFunc::create(this, callfunc_selector(AllianceManagerFunView::downRankFun)),_lang("confirm").c_str());
            }
                break;
            case ButtonKickOut:
            {
                if(GlobalData::shared()->isCrossService){
                    YesNoDialog::show(_lang("138064").c_str(), NULL);
                }else{
                    AllianceTerritoryCheckIfStorageCommand* cmd = new AllianceTerritoryCheckIfStorageCommand(m_info->getUid());
                    cmd->sendAndRelease();
                }
            }
                break;
            case ButtonAccept:
            {
                AcceptAllianceApplyCommand* cmd = new AcceptAllianceApplyCommand(m_info->getUid());
                cmd->sendAndRelease();
                PopupViewController::getInstance()->removePopupView(this);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_ACCEPT_ONE_PLAYER,CCString::create(m_info->getUid()));
            }
                break;
            case ButtonRefuse:
            {
                RefuseAllianceApplyCommand* cmd = new RefuseAllianceApplyCommand(m_info->getUid());
                cmd->sendAndRelease();
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_DATA_CHANGE,CCString::create(m_info->getUid()));
                PopupViewController::getInstance()->removePopupView(this);
            }
                break;
            case ButtonReplaceLeader:
            {
                auto dt = GlobalData::shared()->getWorldTime()-GlobalData::shared()->changeTime(m_info->getOffLineTime()/1000);
                dt = dt;
                int timedt = 0;
                if(dt>=24*60*60){
                    timedt =dt/(24*60*60);
                }
                string tipStr = _lang_1("115252",CC_ITOA(GlobalData::shared()->alliance_cost_k7));
                if(timedt>=7){
                    tipStr = _lang_1("115254",CC_ITOA(GlobalData::shared()->alliance_cost_k7));
                }
                YesNoDialog* dialog = YesNoDialog::showButtonAndGold(tipStr.c_str(),CCCallFunc::create(this, callfunc_selector(AllianceManagerFunView::replaceLeaderFun)),_lang("115251").c_str(),GlobalData::shared()->alliance_cost_k7);
                
                if(timedt>=7){
                    dialog->m_btnOk->setEnabled(true);
                }else{
                    dialog->m_btnOk->setEnabled(false);
                }
            }
                break;
            case ButtonRelinquish:
            {
                YesNoDialog::showVariableTitle(_lang_1("110017", m_info->getName().c_str()),CCCallFunc::create(this, callfunc_selector(AllianceManagerFunView::shanRang1)),_lang("confirm").c_str());
            }
                break;
            case ButtonInviteUser:
            {
                InvitesAllianceCommand* cmd = new InvitesAllianceCommand(m_info->getUid());
                cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceManagerFunView::inviteCallBack), NULL));
                cmd->sendAndRelease();
                GameController::getInstance()->showWaitInterface();
            }
                break;
            case ButtonPopInviteTeleport:
                YesNoDialog::show(_lang("115287").c_str(), CCCallFunc::create(this, callfunc_selector(AllianceManagerFunView::inviteTeleport)));
                break;
            case ButtonSendGift:{
                std::string toName = m_info->getName();
                PopupViewController::getInstance()->removePopupView(this);
                PopupViewController::getInstance()->addPopupInViewWithAnim(MailWritePopUpView::createWithGift(toName));
                break;
            }
            case ButtonPlayerInfo:{
                RoleInfoView::createInfoByUid(m_info->getUid());
                PopupViewController::getInstance()->removePopupView(this);
            }
                break;
            case ButtonApplyMessage:{
                if (m_info->getApplyMessage() == "") {
                    CCCommonUtils::flyHint("", "", _lang("115503"), 3, 0, true);
                }
                else {
                    PopupViewController::getInstance()->addPopupView(AllianceApplyMessageView::create(m_info->getApplyMessage()));
                }
            }
                break;
            case ButtonOrder:{
                GameController::getInstance()->callXCApi("action=alOrder_open_m");
                PopupViewController::getInstance()->addPopupView(AllianceOrderView::create(m_info->getUid()));
            }
                break;
        }
    }
}

void AllianceManagerFunView::checkIfStorage(CCObject* param)
{
    auto dic = _dict(param);
    if(dic==NULL || dic->objectForKey("errorCode")) return;
    if (dic->objectForKey("ret")) {
        int ret = dic->valueForKey("ret")->intValue();
        if (ret == 1) {
            YesNoDialog::showVariableTitle(_lang_1("115041",m_info->getName().c_str()),CCCallFunc::create(this, callfunc_selector(AllianceManagerFunView::secondConfirm)),_lang("confirm").c_str());
        }
        else if (ret == 0) {
            YesNoDialog::showVariableTitle(_lang_1("115041",m_info->getName().c_str()),CCCallFunc::create(this, callfunc_selector(AllianceManagerFunView::kickOutFun)),_lang("confirm").c_str());
        }
    }
}
void AllianceManagerFunView::secondConfirm()
{
    YesNoDialog::showVariableTitle(_lang("115462").c_str(),CCCallFunc::create(this, callfunc_selector(AllianceManagerFunView::kickOutFun)),_lang("confirm").c_str());
}

void AllianceManagerFunView::inviteTeleport()
{
    AllianceManager::getInstance()->startInviteTeleport(m_info->getUid(), m_info->getAllianceId());
    PopupViewController::getInstance()->removePopupView(this);
}

void AllianceManagerFunView::inviteCallBack(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
    NetResult* result = dynamic_cast<NetResult*>(param);
    if(result==NULL) return;
    CCDictionary* dict = dynamic_cast<CCDictionary*>(result->getData());
    if(dict==NULL || dict->objectForKey("errorCode")) return;
    CCCommonUtils::flyHint("", "", _lang("115259"));
    m_node9->setVisible(false);
}

void AllianceManagerFunView::shanRang1(){
    YesNoDialog::showVariableTitle(_lang_1("110018", m_info->getName().c_str()),CCCallFunc::create(this, callfunc_selector(AllianceManagerFunView::shanRang2)),_lang("confirm").c_str());
}

void AllianceManagerFunView::shanRang2(){
    KingdomKingChooseCommand* cmd = new KingdomKingChooseCommand(m_info->getUid());
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceManagerFunView::shanRangSuccess), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void AllianceManagerFunView::shanRangSuccess(CCObject* param){
    m_node9->setVisible(false);
    GlobalData::shared()->playerInfo.relinquishEndTime = 0;//不可再任命
}

void AllianceManagerFunView::replaceLeaderFun(){
    if (GlobalData::shared()->alliance_cost_k7 > GlobalData::shared()->playerInfo.gold) {
        YesNoDialog::gotoPayTips();
        return ;
    }
    GameController::getInstance()->showWaitInterface();
    AllianceLeaderReplaceCommand* cmd = new AllianceLeaderReplaceCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceManagerFunView::replaceSuccess), NULL));
    cmd->sendAndRelease();
    
}

void AllianceManagerFunView::replaceSuccess(CCObject* data){
    GameController::getInstance()->removeWaitInterface();
    auto params = _dict(data);
    if(params){
        const CCString *pStr = params->valueForKey("errorCode");
        if (pStr->compare("")!=0) {
            CCCommonUtils::flyText(pStr->getCString());
        }else{
            string tipStr = _lang("115255");//成功后的提示
            CCCommonUtils::flyHint("", "", tipStr);
            m_info->setRank(1);
            auto dicInfo = _dict(params->objectForKey("alliance"));
            GlobalData::shared()->playerInfo.allianceInfo.updateAllianceInfo(dicInfo);
            if(params->objectForKey("gold")){
                auto gold = params->valueForKey("gold")->intValue();
                GlobalData::shared()->playerInfo.gold = gold;
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
            }
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(CCInteger::create(1), "rank");
            dic->setObject(CCString::create(m_info->getUid()), "playerId");
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_LEADER_CHANGE,dic);
            PopupViewController::getInstance()->removePopupView(this);
        }
    }
}

void AllianceManagerFunView::upRankFun(){
    int lastRank = m_info->getRank();
    int rank = m_info->getRank() + 1;
    SetAllianceRankCommand* cmdRank = new SetAllianceRankCommand(m_info->getUid(),rank);
    cmdRank->sendAndRelease();
    m_info->setRank(rank);
    m_rankUp->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("icon_Alliance_0%d.png",rank)->getCString()));
    m_rankDown->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("icon_Alliance_0%d.png",lastRank)->getCString()));
    refresh();
    addParticle(true);
    this->getAnimationManager()->runAnimationsForSequenceNamed("Up");
    this->unschedule(schedule_selector(AllianceManagerFunView::rUpAni));
    this->schedule(schedule_selector(AllianceManagerFunView::rUpAni), 0.55);
    //发聊天信息
    string msg = _lang_3("115184", m_info->getName().c_str(), CC_ITOA(rank-1), CC_ITOA(rank));
    msg.append("  (").append(_lang("115181")).append(")");
    ChatController::getInstance()->sendCountryChat(msg.c_str(), CHAT_ALLIANCE, 2);
}

void AllianceManagerFunView::downRankFun(){
    int lastRank = m_info->getRank();
    int rank = m_info->getRank() - 1;
    SetAllianceRankCommand* cmdRank = new SetAllianceRankCommand(m_info->getUid(),rank);
    cmdRank->sendAndRelease();
    m_info->setRank(rank);
    //addParticle(false);
    refresh();
    m_rankUp->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("icon_Alliance_0%d.png",lastRank)->getCString()));
    m_rankDown->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("icon_Alliance_0%d.png",rank)->getCString()));
    this->getAnimationManager()->runAnimationsForSequenceNamed("Down");
    this->unschedule(schedule_selector(AllianceManagerFunView::rDownAni));
    this->schedule(schedule_selector(AllianceManagerFunView::rDownAni), 0.5);
    //发聊天信息
    string msg = _lang_3("115185", m_info->getName().c_str(), CC_ITOA(rank+1), CC_ITOA(rank));
    msg.append("  (").append(_lang("115181")).append(")");
    ChatController::getInstance()->sendCountryChat(msg.c_str(), CHAT_ALLIANCE, 2);
}

void AllianceManagerFunView::removeParticle(float t){
    this->unschedule(schedule_selector(AllianceManagerFunView::removeParticle));
    m_parVec.clear();
    m_parNode->removeAllChildrenWithCleanup(true);
    m_parNode2->removeAllChildrenWithCleanup(true);
}

void AllianceManagerFunView::rUpAni(float t){
    this->unschedule(schedule_selector(AllianceManagerFunView::rUpAni));
    for (int i=1; i<9; i++) {
        string tmpStart = "CrownUp_";
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(20,0));
        m_parNode2->addChild(particle);
    }
}

void AllianceManagerFunView::rDownAni(float t){
    this->unschedule(schedule_selector(AllianceManagerFunView::rDownAni));
    for (int i=1; i<5; i++) {
        string tmpStart = "Crown_";
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(0,-35));
        m_parNode2->addChild(particle);
    }
}

void AllianceManagerFunView::addParticle(bool up){
    m_parVec.clear();
    m_parNode->removeAllChildrenWithCleanup(true);
    int count = 3;
    for (int i=2; i<count; i++) {
        string tmpStart = "Spotlight_";
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        if(i==2 && !up){
            particle->setStartColor({10,10,10,100});
            particle->setEndColor({10,10,10,100});
        }
        particle->setPosition(ccp(0,200));
        addParticleToBatch(particle);
        tmpStart = "Spotlight_Glow_";
        particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        if(i==2 && !up){
            particle->setStartColor({10,10,10,100});
            particle->setEndColor({10,10,10,100});
        }
        particle->setPosition(ccp(0,-15));
        addParticleToBatch(particle);
        tmpStart = "Spotlight_L_1_";
        particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        if(i==2 && !up){
            particle->setStartColor({10,10,10,100});
            particle->setEndColor({10,10,10,100});
        }
        particle->setPosition(ccp(-320,200));
        addParticleToBatch(particle);
        tmpStart = "Spotlight_L_2_";
        particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        if(i==2 && !up){
            particle->setStartColor({10,10,10,100});
            particle->setEndColor({10,10,10,100});
        }
        particle->setPosition(ccp(-160,200));
        addParticleToBatch(particle);
        tmpStart = "Spotlight_R_1_";
        particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        if(i==2 && !up){
            particle->setStartColor({10,10,10,100});
            particle->setEndColor({10,10,10,100});
        }
        particle->setPosition(ccp(320,200));
        addParticleToBatch(particle);
        tmpStart = "Spotlight_R_2_";
        particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        if(i==2 && !up){
            particle->setStartColor({10,10,10,100});
            particle->setEndColor({10,10,10,100});
        }
        particle->setPosition(ccp(160,200));
        addParticleToBatch(particle);
    }
    
    this->unschedule(schedule_selector(AllianceManagerFunView::removeParticle));
    this->schedule(schedule_selector(AllianceManagerFunView::removeParticle), 2);
}

void AllianceManagerFunView::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle){
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
    m_parNode->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void AllianceManagerFunView::kickOutFun(){
    //发聊天信息
    string msg = _lang_1("115187", m_info->getName().c_str());
    msg.append("  (").append(_lang("115181")).append(")");
    ChatController::getInstance()->sendCountryChat(msg.c_str(), CHAT_ALLIANCE, 2);
    
    AllianceKickMemberCommand * command = new AllianceKickMemberCommand();
    command->putParam("playerId", CCString::create(m_info->getUid()));
    command->sendAndRelease();
    int num = GlobalData::shared()->playerInfo.allianceInfo.currentNum - 1;
    if(num<=1) num = 1;
    GlobalData::shared()->playerInfo.allianceInfo.currentNum = num;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_DATA_CHANGE,CCString::create(m_info->getUid()));
    PopupViewController::getInstance()->removePopupView(this);
}

void AllianceManagerFunView::leaderTransFun(){
    int lastRank = m_info->getRank();
    int rank = 5;
    m_rankUp->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("icon_Alliance_0%d.png",rank)->getCString()));
    m_rankDown->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("icon_Alliance_0%d.png",lastRank)->getCString()));
    this->getAnimationManager()->runAnimationsForSequenceNamed("Up");
    addParticle(true);
    this->unschedule(schedule_selector(AllianceManagerFunView::rUpAni));
    this->schedule(schedule_selector(AllianceManagerFunView::rUpAni), 0.5);
    
    AllianceLeaderTransCommand* cmd = new AllianceLeaderTransCommand(m_info->getUid());
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceManagerFunView::leaderTransSuccess), NULL));
    cmd->sendAndRelease();
    //PopupViewController::getInstance()->removePopupView(this);
    
}

void AllianceManagerFunView::leaderTransSuccess(CCObject* p){
    if (this->getParent() && m_info) {
        m_info->setRank(5);
        refresh();
    }
}

void AllianceManagerFunView::onClickConfirm(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

void AllianceManagerFunView::close(cocos2d::CCObject *p)
{
    close(0.0);
}
void AllianceManagerFunView::close(float t){
    PopupViewController::getInstance()->removePopupView(this);
}

void AllianceManagerFunView::setButtonStatus(){
    m_rankUp->setVisible(false);
    m_rankDown->setVisible(false);
    m_node1->setVisible(false);
    m_node2->setVisible(false);
    m_node3->setVisible(false);
    m_node4->setVisible(false);
    m_node5->setVisible(false);
    m_node6->setVisible(false);
    m_node7->setVisible(false);
    m_node8->setVisible(false);
    m_node11->setVisible(false);
    m_parNode->setVisible(false);
    m_nameTxt->setVisible(false);
}

void AllianceManagerFunView::onClickCancel(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

SEL_CCControlHandler AllianceManagerFunView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickConfirm", AllianceManagerFunView::onClickConfirm);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCancel", AllianceManagerFunView::onClickCancel);
    
    return NULL;
}


///

AllianceOrderView *AllianceOrderView::create(string uid){
    AllianceOrderView *ret = new AllianceOrderView();
    if(ret && ret->init(uid)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceOrderView::init(string uid)
{
    bool ret = false;
    if(PopupBaseView::init())
    {
        CCLoadSprite::doResourceByCommonIndex(502, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(502, false);
        });
        
        setIsHDPanel(true);
//        this->setModelLayerOpacity(220);
        auto node = CCBLoadFile("AllianceOrderView", this, this);
        this->setContentSize(node->getContentSize());
        
        m_titleLabel->setString(_lang("115429").c_str());
        m_btn1Label->setString(_lang("115430"));
        m_btn2Label->setString(_lang("115431"));
        m_btn3Label->setString(_lang("115432"));
        m_btn4Label->setString(_lang("115433"));
        m_btn5Label->setString(_lang("115434"));
        m_btn6Label->setString(_lang("115435"));
        m_uid = uid;
        ret = true;
    }
    return ret;
}

void AllianceOrderView::onEnter(){
    PopupBaseView::onEnter();
    setTouchEnabled(true);
}

void AllianceOrderView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool AllianceOrderView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (isTouchInside(m_btn1, pTouch)) {
        m_btn1->onTouchBegan(pTouch, pEvent);
    }else if (isTouchInside(m_btn2, pTouch)) {
        m_btn2->onTouchBegan(pTouch, pEvent);
    }else if (isTouchInside(m_btn3, pTouch)) {
        m_btn3->onTouchBegan(pTouch, pEvent);
    }else if (isTouchInside(m_btn4, pTouch)) {
        m_btn4->onTouchBegan(pTouch, pEvent);
    }else if (isTouchInside(m_btn5, pTouch)) {
        m_btn5->onTouchBegan(pTouch, pEvent);
    }else if (isTouchInside(m_btn6, pTouch)) {
        m_btn6->onTouchBegan(pTouch, pEvent);
    }
    return true;
}

void AllianceOrderView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (isTouchInside(m_btn1, pTouch)) {
        m_btn1->onTouchEnded(pTouch, pEvent);
//        onClickBtn1(NULL, NULL);
    }else if (isTouchInside(m_btn2, pTouch)) {
        m_btn2->onTouchEnded(pTouch, pEvent);
//        onClickBtn2(NULL, NULL);
    }else if (isTouchInside(m_btn3, pTouch)) {
        m_btn3->onTouchEnded(pTouch, pEvent);
//        onClickBtn3(NULL, NULL);
    }else if (isTouchInside(m_btn4, pTouch)) {
        m_btn4->onTouchEnded(pTouch, pEvent);
//        onClickBtn4(NULL, NULL);
    }else if (isTouchInside(m_btn5, pTouch)) {
        m_btn5->onTouchEnded(pTouch, pEvent);
//        onClickBtn5(NULL, NULL);
    }else if (isTouchInside(m_btn6, pTouch)) {
        m_btn6->onTouchEnded(pTouch, pEvent);
//        onClickBtn6(NULL, NULL);
    }else if (isTouchInside(m_touchNode, pTouch)){
        return;
    }else {
        closeSelf();
    }
}

SEL_CCControlHandler AllianceOrderView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn1", AllianceOrderView::onClickBtn1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn2", AllianceOrderView::onClickBtn2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn3", AllianceOrderView::onClickBtn3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn4", AllianceOrderView::onClickBtn4);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn5", AllianceOrderView::onClickBtn5);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn6", AllianceOrderView::onClickBtn6);
    return NULL;
}

bool AllianceOrderView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1", CCControlButton*, this->m_btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2", CCControlButton*, this->m_btn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3", CCControlButton*, this->m_btn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn4", CCControlButton*, this->m_btn4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn5", CCControlButton*, this->m_btn5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn6", CCControlButton*, this->m_btn6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1Label", CCLabelIF*, this->m_btn1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2Label", CCLabelIF*, this->m_btn2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3Label", CCLabelIF*, this->m_btn3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn4Label", CCLabelIF*, this->m_btn4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn5Label", CCLabelIF*, this->m_btn5Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn6Label", CCLabelIF*, this->m_btn6Label);
    return false;
}

void AllianceOrderView::onClickBtn1(CCObject *pSender, CCControlEvent event){
    onSendOrder(ORDER_COLLECT);
}
void AllianceOrderView::onClickBtn2(CCObject *pSender, CCControlEvent event){
    onSendOrder(ORDER_SCIENCE);
}
void AllianceOrderView::onClickBtn3(CCObject *pSender, CCControlEvent event){
    onSendOrder(ORDER_HELP);
}
void AllianceOrderView::onClickBtn4(CCObject *pSender, CCControlEvent event){
    onSendOrder(ORDER_DEFEND);
}
void AllianceOrderView::onClickBtn5(CCObject *pSender, CCControlEvent event){
    onSendOrder(ORDER_FIGHT);
}
void AllianceOrderView::onClickBtn6(CCObject *pSender, CCControlEvent event){
    onSendOrder(ORDER_BUY);
}

void AllianceOrderView::onSendOrder(int cmd)
{
    string xcloudAct = "action=alOrder_cmd_";
    xcloudAct = xcloudAct + CC_ITOA(cmd);
    GameController::getInstance()->callXCApi(xcloudAct);
    AllianceOrderCommand* command = new AllianceOrderCommand(m_uid, cmd);
    command->sendAndRelease();
    closeSelf();
}