//
//  MonsterAttackPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-7-30.
//
//

#include "MonsterAttackPopUpView.h"
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
#include "MailGetRewardCommand.h"
#include "ParticleController.h"
#include "SceneController.h"

MonsterAttackPopUpView *MonsterAttackPopUpView::create(MailInfo *info){
    MonsterAttackPopUpView *ret = new MonsterAttackPopUpView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MonsterAttackPopUpView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MonsterAttackPopUpView::refreshView), MAIL_CONTENT_READ, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MonsterAttackPopUpView::refreshView), MIAL_GET_REWARD_REFRESH, NULL);

    setTitleName(m_info->fromName.c_str());

}

void MonsterAttackPopUpView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_CONTENT_READ);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MIAL_GET_REWARD_REFRESH);

    PopupBaseView::onExit();
}

bool MonsterAttackPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        m_total = 0;
        setMailUuid(m_info->uid);
        auto bg = CCBLoadFile("MonsterAttackReportCCB", this, this);
        this->setContentSize(bg->getContentSize());
        setTitleName(_lang("105513"));
        this->m_timeText->setString(CCCommonUtils::timeStampToDate(m_info->createTime).c_str());
        CCLoadSprite::doResourceByCommonIndex(105, true);
//        CCLoadSprite::doResourceByCommonIndex(6, true);

        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(105, false);
//            CCLoadSprite::doResourceByCommonIndex(6, false);
        });

        m_ListNode = CCNode::create();
        int preHeight = this->m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int dh = m_buildBG->getContentSize().height - preHeight;
        m_bg->setContentSize(CCSize(m_bg->getContentSize().width,m_bg->getContentSize().height+dh));
        this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
        m_scrollView = CCScrollView::create(m_listContainer->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setAnchorPoint(ccp(0, 1));
        m_scrollView->setTouchPriority(Touch_Popup);
        m_listContainer->addChild(m_scrollView);
        m_downNode->setPositionY(m_downNode->getPositionY() - dh);
      //  m_deleteBtnTitle->setString(_lang("108523").c_str());
        this->m_scrollView->addChild(this->m_ListNode);
        this->m_upNode->removeFromParent();
        this->m_ListNode->addChild(m_upNode);
        this->m_firstNode->removeFromParent();
        this->m_ListNode->addChild(m_firstNode);
        this->m_playerNode->removeFromParent();
        this->m_ListNode->addChild(m_playerNode);
        m_firstKillHint->setString(_lang("103759"));
        m_firstKillHint->setVisible(false);
        this->m_upNode->setPositionX(0);
        if(!m_info->isReadContent){
            getData();
        }else{
            refreshView(NULL);
        }
        m_battleBtn->setVisible(GlobalData::shared()->playerInfo.gmFlag==1);
        ret = true;
    }
    return ret;
}

void MonsterAttackPopUpView::getData(){
    MailController::getInstance()->readMailFromServer(m_info->uid, CC_ITOA(m_info->type));
}

void MonsterAttackPopUpView::refreshView(CCObject* obj){
    m_total = 0;
    m_total -= 300;
    m_upNode->setPositionY(m_total);
    m_total -= 8;
    m_firstNode->setPosition(0, m_total);
    
    if(m_info->save==0){
        m_unSaveBtn->setVisible(true);
        m_addSaveBtn->setVisible(false);
    }else{
        m_unSaveBtn->setVisible(false);
        m_addSaveBtn->setVisible(true);
    }
    m_title1->setString(_lang("103702"));

    addNormalReward();
    addKillReward();
    this->m_playerNode->setPosition(0, m_total);
    
    m_total -= 300;
    m_total = -m_total;
    this->m_ListNode->setPositionY(m_total);
    m_scrollView->setContentSize(CCSize(m_listContainer->getContentSize().width, m_total));
    m_scrollView->setContentOffset(ccp(0, m_listContainer->getContentSize().height - (m_total)));
    
    m_totalText->setString(_lang_1("108600", ""));
    m_loseText->setString(_lang("105544").c_str());
    m_woundedText->setString(_lang("105545").c_str());
    m_surplusText->setString(_lang("105546").c_str());
//    m_totalNumText->setString(CC_CMDITOA(m_info->total));
//    m_loseNumText->setString(CC_CMDITOA(m_info->dead));
//    m_woundedNumText->setString(CC_CMDITOA(m_info->hurt));
//    m_surplusNumText->setString(CC_CMDITOA(m_info->survived));
//    m_selfName->setString(GlobalData::shared()->playerInfo.name);
//    
//    std::string name = CCCommonUtils::getNameById(m_info->monsterId);
//    int index = WorldController::getIndexByPoint(ccp(m_info->monsterX, m_info->monsterY));
//    if(index % 2 == 0){
//        name = _lang("103762");
//    }
//
//    m_attackText->setString(_lang_2("105547", _lang("102187").c_str(), name.c_str()));
//    this->m_mailTitle->setString(_lang_2("105518", CC_ITOA(m_info->monsterX), CC_ITOA(m_info->monsterY)));
//    m_title2->setString(_lang("103700"));
//    m_battlePicNode->removeAllChildren();
//    auto battlePic = CCLoadSprite::createSprite("Mail_monster.png");
//    this->m_battlePicNode->addChild(battlePic);
//
//    if(m_info->monsterCurrentHp == 0){
//        m_upTitle->setString(_lang("105117"));
//    }else{
//        CCCommonUtils::setSpriteGray(battlePic, true);
//        m_upTitle->setString(_lang("105118"));
//    }
//    if(m_info->rewardStatus != 0 || m_info->rewardId == ""){
//        m_chest1->setVisible(false);
//    }
//    
//    m_posText->setString(std::string("x:") + CC_ITOA(WorldController::getInstance()->selfPoint.x) + ", " + std::string("y:") + CC_ITOA(WorldController::getInstance()->selfPoint.y));
//    std::string powerStr = _lang("102163");
//    powerStr.append(":");
//    m_powerDown->setString((powerStr + "-" + CC_CMDITOA(m_info->selfPowerLost)).c_str());
    
    auto sprite = CCLoadSprite::createSprite((GlobalData::shared()->playerInfo.pic + ".png").c_str());
    CCCommonUtils::setSpriteMaxSize(sprite, 90);
    m_picNode1->addChild(sprite);
    this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
}

bool MonsterAttackPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, this->m_upNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_firstNode", CCNode*, this->m_firstNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtnTitle", CCLabelIF*, this->m_deleteBtnTitle);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_deleteBtnTitle", CCLabelIF*, this->m_deleteBtnTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_upTitle", CCLabelIF*, this->m_upTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battlePicNode", CCNode*, this->m_battlePicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_secondNode", CCNode*, this->m_secondNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_secondRewardContainer", CCNode*, this->m_secondRewardContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_firstRewardContainer", CCNode*, this->m_firstRewardContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playerNode", CCNode*, this->m_playerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_firstDec", CCScale9Sprite*, this->m_firstDec);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_secondDec", CCScale9Sprite*, this->m_secondDec);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_firstBG", CCScale9Sprite*, this->m_firstBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_secondBG", CCScale9Sprite*, this->m_secondBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackText", CCLabelIF*, this->m_attackText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailTitle", CCLabelIF*, this->m_mailTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalText", CCLabelIF*, this->m_totalText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loseText", CCLabelIF*, this->m_loseText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedText", CCLabelIF*, this->m_woundedText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_surplusText", CCLabelIF*, this->m_surplusText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expText", CCLabelIF*, this->m_expText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expNumText", CCLabelIF*, this->m_expNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNumText", CCLabelIF*, this->m_totalNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loseNumText", CCLabelIF*, this->m_loseNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedNumText", CCLabelIF*, this->m_woundedNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_surplusNumText", CCLabelIF*, this->m_surplusNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selfName", CCLabelIF*, this->m_selfName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posText", CCLabelIF*, this->m_posText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerDown", CCLabelIF*, this->m_powerDown);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title1", CCLabelIF*, this->m_title1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title2", CCLabelIF*, this->m_title2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_firstKillHint", CCLabelIF*, this->m_firstKillHint);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_chest1", CCSprite*, this->m_chest1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, this->m_picNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, this->m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selfCoord", CCNode*, this->m_selfCoord);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battleBtn", CCControlButton*, this->m_battleBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
    return false;
}

void MonsterAttackPopUpView::onOkDeleteMail(){
    MailController::getInstance()->removeMail(m_info->uid, CC_ITOA(m_info->type));
    PopupViewController::getInstance()->goBackPopupView();
}
void MonsterAttackPopUpView::onDeleteClick(CCObject *pSender, CCControlEvent event){
    if(m_info->save!=0){
        CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(MonsterAttackPopUpView::onOkDeleteMail)),true);
}

SEL_CCControlHandler MonsterAttackPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", MonsterAttackPopUpView::onDeleteClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", MonsterAttackPopUpView::onAddSaveClick);
    return NULL;
}

void MonsterAttackPopUpView::onReportClick(CCObject *pSender, CCControlEvent event){
    if(m_info->reportUid == ""){
        return;
    }
    BattleManager::shared()->mailFightReport(m_info->reportUid);
}

bool MonsterAttackPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_chest1, pTouch)){
        return true;
    }else if(isTouchInside(m_selfCoord, pTouch)){
        return true;
    }
    return false;
}

void MonsterAttackPopUpView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void MonsterAttackPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_chest1, pTouch) && m_chest1->isVisible()){
        MailGetRewardCommand* cmd = new MailGetRewardCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->rewardStatus = 1;
        string tmpStart = "Capacity_";
        int count = 6;
        CCParticleBatchNode *batch = ParticleController::createParticleBatch();

        for (int i=1; i<count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            batch->addChild(particle);
        }
        m_particleNode->addChild(batch);
        this->runAction(CCSequence::create(CCDelayTime::create(0.4)
                                               , CCCallFuncO::create(this, callfuncO_selector(MonsterAttackPopUpView::removeParticle), NULL)
                                               , NULL
                                               ));
        m_chest1->setVisible(false);
    }else if(isTouchInside(m_selfCoord, pTouch)){
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
    }
}

void MonsterAttackPopUpView::removeParticle(CCObject* obj){
    m_particleNode->removeAllChildren();
}

void MonsterAttackPopUpView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
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

void MonsterAttackPopUpView::addNormalReward(){
//    if(!m_info->normalReward){
//        return;
//    }
//    CCArray *arr = m_info->normalReward;
//    int rowNum = arr->count();
//    int i = 0;
//    m_firstBG->setPreferredSize(CCSize(m_firstBG->getContentSize().width, rowNum * 50 + 100));
//    m_firstDec->setPreferredSize(CCSize(m_firstDec->getContentSize().width, rowNum * 50 + 30));
//    
//    int up = 70;
//    int down = 35;
//
//    if(rowNum == 0){
//        m_firstBG->setPreferredSize(CCSize(m_firstBG->getContentSize().width, 1 * 50 + 100));
//        m_firstDec->setPreferredSize(CCSize(m_firstDec->getContentSize().width, 1 * 50 + 30));
//
//        m_title2->setString("");
//        int level = atoi(CCCommonUtils::getPropById(m_info->monsterId, "level").c_str());
//        std::string name = CCCommonUtils::getNameById(m_info->monsterId);
//        int index = WorldController::getIndexByPoint(ccp(m_info->monsterX, m_info->monsterY));
//        if(index % 2 == 0){
//            name = _lang("103762");
//        }
//
//        auto label = CCLabelIF::create(_lang_2("103757", CC_ITOA(level), name.c_str()));
//        label->setDimensions(CCSize(580, 0));
//        m_firstRewardContainer->addChild(label);
//        label->setPosition(ccp(290, -50));
//        m_total -= (1 * 50 + up + down);
//        m_title1->setString("");
//        return;
//    }
//    m_total -= (rowNum * 50 + up + down);
//
//    while(i < arr->count()){
//        CCDictionary *dict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
//        int type = 0;
//        int value = 0;
//        int num = 0;
//        type = dict->valueForKey("type")->intValue();
//        if(type != R_GOODS){
//            value = dict->valueForKey("value")->intValue();
//        }else{
//            auto good = _dict(dict->objectForKey("value"));
//            value = atoi(good->valueForKey("itemId")->getCString());
//            num = good->valueForKey("count")->intValue();
//            if(good->objectForKey("rewardAdd")){
//                num = good->valueForKey("rewardAdd")->intValue();
//            }
//        }
//        
//        auto cell = DetectRewardPreviewCell::create(type, value, num);
//        cell->setAnchorPoint(ccp(0, 0));
//        cell->setPosition(ccp(60, -50 * (i + 1)));
//        m_firstRewardContainer->addChild(cell);
//        i++;
//    }
//    if(m_info->firstKill){
//        m_title1->setString(_lang("103758"));
//        int level = atoi(CCCommonUtils::getPropById(m_info->monsterId, "level").c_str());
//        m_firstKillHint->setString(_lang_2("103759", CC_ITOA(level), CC_ITOA(level + 1)));
//        int addH = m_firstKillHint->getContentSize().height * m_firstKillHint->getOriginScaleY();
//        m_firstBG->setPreferredSize(CCSize(m_firstBG->getContentSize().width, m_firstBG->getContentSize().height + addH));
//        m_firstKillHint->setVisible(true);
//        m_firstKillHint->setPositionY(-(rowNum * 50 + up + down));
//        m_total -= addH;
//    }
}

void MonsterAttackPopUpView::addKillReward(){
//    this->m_secondNode->removeFromParent();
//
//    if((!m_info->killReward || m_info->killReward->count()<=0) && (m_info->rewardStatus != 0 || m_info->rewardId == "")){
//        return;
//    }
//    m_secondNode->setPosition(0, m_total);
//    this->m_ListNode->addChild(m_secondNode);
//    if(m_info->rewardStatus == 0 && m_info->rewardId != ""){
//        m_total -= 328;
//        return;
//    }
//
//    CCArray *arr = m_info->killReward;
//    int rowNum = arr->count();
//    m_secondBG->setPreferredSize(CCSize(m_firstBG->getContentSize().width, rowNum * 50 + 100));
//    m_secondDec->setPreferredSize(CCSize(m_firstDec->getContentSize().width, rowNum * 50 + 30));
//    int i = 0;
//    
//    while(i < arr->count()){
//        CCDictionary *dict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
//        int type = 0;
//        int value = 0;
//        int num = 0;
//        type = dict->valueForKey("type")->intValue();
//        if(type != R_GOODS){
//            value = dict->valueForKey("value")->intValue();
//        }else{
//            auto good = _dict(dict->objectForKey("value"));
//            value = atoi(good->valueForKey("itemId")->getCString());
//            num = good->valueForKey("count")->intValue();
//            if(good->objectForKey("rewardAdd")){
//                num = good->valueForKey("rewardAdd")->intValue();
//            }
//        }
//        
//        auto cell = DetectRewardPreviewCell::create(type, value, num);
//        cell->setAnchorPoint(ccp(0, 0));
//        cell->setPosition(ccp(60, -50 * (i + 1)));
//        m_secondRewardContainer->addChild(cell);
//        i++;
//    }
//    int up = 70;
//    int down = 35;
//    m_total -= (rowNum * 50 + up + down);
}
