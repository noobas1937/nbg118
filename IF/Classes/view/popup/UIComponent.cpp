//

//  UIComponent.cpp

//  IF

//

//  Created by wangzhenlei on 13-9-10.

//

//
#include "../../Ext/CCDevice.h"

#include "UIComponent.h"

#include "SceneController.h"

#include "PopupViewController.h"

#include "ChatView.h"

#include "BattleManager.h"

#include "CCCommonUtils.h"

#include "IFShakeGuideLayer.h"

#include "GameController.h"

#include "SoundController.h"

#include "ChatController.h"

#include "SetAccountView.h"

#include "YesNoDialog.h"

#include "FunBuildController.h"

#include "GoldExchangeView.h"

#include "MailPopUpView.h"

#include "UserLevelCommand.h"

#include "DataRecordCommand.h"

#include "GuideController.h"

#include "SettingPopUpView.h"

#include "StoreView.h"

#include "WorldController.h"
#include "UseResToolView.h"
#include "WorldMapView.h"

#include "QuestController.h"

#include "RollingText.h"

#include "VipMainView.h"

#include "VipDetailView.h"

#include "RoleInfoView.h"

#include "ParticleController.h"

#include "MailController.h"

#include "AllianceInfoMembersView.h"

#include "ActivityController.h"

#include "ItemStatusView.h"

#include "SuggestionView.h"

#include "EnemyInfoController.h"

//#include "CCLuaEngine.h"

#include "GeneralsPopupView.h"

#include "MakingSoldierView.h"

#include "ScienceListView.h"

#include "QueueController.h"

#include "ImperialScene.h"

#include "ArmyController.h"

#include "JoinAllianceView.h"

#include "AllianceInfoView.h"

#include "AchievementNewPopUpView.h"

#include "AllianceMarketView.h"

#include "AllianceHelpView.h"

#include "BackpackView.h"

#include "HowToPlayView.h"

#include "CCShake.h"

#include "MailWritePopUpView.h"

#include "FavoriteList.h"

#include "MoveToView.h"

#include "ChatController.h"

#include "RechargeACTVCell.h"

#include "ActivityTipView.h"

#include "UseSkillPopUpView.h"

#include "CityDefenseView.h"

#include "EquipmentCreateView.h"
#include "EquipmentBagView.h"
#include "EquipSiteView.h"

#include "WatchtowerView.h"
#include "HelpshiftCocos2dx.h"
#include "ManagerKingdomView.h"
#include "ChatServiceCocos2dx.h"

#include "TestFeedBack/TestFeedBackView.h"
#include "TestFeedBackController.h"

#include "ToolController.h"
#include "LotteryActView.h"
#include "LotteryAct2View.h"
#include "LotteryController.h"

#include "WorldPopupOnceView.h"
#include "AchievementController.h"
#include "HFUrlImageCache.h"

#include "RepayView.h"
#include "CrossServiceTargetView.h"
#include "CSAWorldBtn.h"
#include "CKFNextRoundCommand.h"
#include "CSALeaveCommand.h"

#include "LuaController.h"
#include "ComposeView.h"

#include "EquipmentController.h"
#include "JoinRecAllianceTipView.h"

#include "GeneralTitanPopupView.h"//fusheng

//UIComponentOldTitle
UIComponentOldTitle* UIComponentOldTitle::create(OldTitleType type)
{
    auto ret = new UIComponentOldTitle();
    if (ret && ret->init(type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

bool UIComponentOldTitle::init(OldTitleType type) {
    bool ret = false;
    if (CCLayer::init()) {
        CCBLoadFile("gameUI_HD_title",this,this);
       
        ret = true;
    }
    
    
    updateShowUIComponentByType(type);
    
    return ret;
}

void UIComponentOldTitle::updateShowUIComponentByType(OldTitleType type)
{
//    switch (type) {
//        case <#constant#>:
//            <#statements#>
//            break;
//            
//        default:
//            break;
//    }
    this->m_uiTitle->setVisible(true);
    this->m_popupReturnBtn->setVisible(true);
    this->m_popupTitleName->setVisible(type==UIComponentOldTitle_SEVEN_TYPE?false:true);
    this->m_inUiTitle->setVisible(true);
    
    if(type==UIComponentOldTitle_SIX_TYPE || type == UIComponentOldTitle_SEVEN_TYPE)
    {
        m_popupTitleBg2->setVisible(false);
        this->m_UserNode->setVisible(true);
        this->m_powerAndBgNode->setVisible(false);
        m_otherPartNode->setVisible(false);
    }else{
        m_popupTitleBg2->setVisible(!true); // guo.jiang
        this->m_UserNode->setVisible(false);
        this->m_powerAndBgNode->setVisible(true);
        m_otherPartNode->setVisible(true);
        
    }
    
    this->m_goldNode->setVisible(false);
    
//    this->m_queueNode->setVisible(false);
//    
//    this->m_UIQuestNode->setVisible(false);
    
    //    this->m_mailNode->setVisible(false);
    
//    this->m_rightNode->setVisible(false);
//    
//    this->m_fireNode->setVisible(false);
    
    this->m_inUiTitle->setVisible(true);
    

}


SEL_CCControlHandler UIComponentOldTitle::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)

{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPopupReturnClick", UIComponentOldTitle::onPopupReturnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", UIComponentOldTitle::onCloseBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onEquipCrtBtnClick", UIComponentOldTitle::onEquipCrtBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onEquipBagBtnClick", UIComponentOldTitle::onEquipBagBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteMailBtnClick", UIComponentOldTitle::onWriteMailBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelDelMailClick", UIComponentOldTitle::onCancelDelMailClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDelAllMailClick", UIComponentOldTitle::onDelAllMailClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDelMailOPClick", UIComponentOldTitle::onDelMailOPClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoldBuyBtnClick", UIComponentOldTitle::onGoldBuyBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoldBtnClick", UIComponentOldTitle::onGoldBuyBtnClick);
    
    
    return NULL;
    
}

void UIComponentOldTitle::onPopupReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    _targetDelegate->onPopupReturnClick(pSender, pCCControlEvent);
//    _targetDelegate->m_titleOldTitle = NULL;
//    this->removeFromParentAndCleanup(true);
}

void UIComponentOldTitle::onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    
}

void UIComponentOldTitle::onEquipCrtBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    
}

void UIComponentOldTitle::onEquipBagBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    
}

void UIComponentOldTitle::onWriteMailBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    
}

void UIComponentOldTitle::onDelAllMailClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    
}

void UIComponentOldTitle::onDelMailOPClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    _targetDelegate->onDelMailOPClick(pSender, pCCControlEvent);
}

void UIComponentOldTitle::onGoldBuyBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    
}
void UIComponentOldTitle::onCancelDelMailClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    _targetDelegate->onCancelDelMailClick(pSender, pCCControlEvent);
}


bool UIComponentOldTitle::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    
    //    CCLOG("ccbi control name %s",pMemberVariableName);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironIcon", CCSprite*, this->m_ironIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneIcon", CCSprite*, this->m_stoneIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_UserResBg", CCNode*, this->m_UserResBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerAndBgNode", CCNode*, this->m_powerAndBgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerNode", CCNode*, this->m_powerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pFlagL", CCSprite*, this->m_pFlagL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pFlagR", CCSprite*, this->m_pFlagR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerBgNode", CCNode*, this->m_powerBgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_LvBG", CCSprite*, this->m_LvBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerName", CCLabelIF*, this->m_powerName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_power", CCLabelIFBMFont*, this->m_power);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerParticleNode", CCNode*, this->m_powerParticleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wood", CCLabelIF*, this->m_wood);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodIcon", CCSprite*, this->m_woodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodBarNode", CCNode*, this->m_foodBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodIcon", CCSprite*, this->m_foodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iron", CCLabelIF*, this->m_iron);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_food", CCLabelIF*, this->m_food);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stone", CCLabelIF*, this->m_stone);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gold", CCLabelIF*, this->m_gold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironBarNode", CCNode*, this->m_ironBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneBarNode", CCNode*, this->m_stoneBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonGoldBarNode", CCNode*, this->m_dragonGoldBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonGoldIcon", CCSprite*, this->m_dragonGoldIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonGold", CCLabelIF*, this->m_dragonGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonFoodBarNode", CCNode*, this->m_dragonFoodBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonFoodIcon", CCSprite*, this->m_dragonFoodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonFood", CCLabelIF*, this->m_dragonFood);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_otherPartNode", CCNode*, this->m_otherPartNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playerBG", CCSprite*, this->m_playerBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expBar", CCScale9Sprite*, this->m_expBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_uiTitle", CCNode*, this->m_uiTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inUiTitle", CCNode*, this->m_inUiTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popupTitleBg2", CCSprite*, this->m_popupTitleBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popupTitleName", CCLabelIF*, this->m_popupTitleName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popupReturnBtn", CCControlButton*, this->m_popupReturnBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_EquipCrtNode", CCNode*, m_EquipCrtNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_EquipCrtBtn", CCControlButton*, m_EquipCrtBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_EquipBagBtn", CCControlButton*, m_EquipBagBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_EquipBagNode", CCNode*, m_EquipBagNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBtnsNode", CCNode*, m_mailBtnsNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wrireMailBtn", CCControlButton*, this->m_wrireMailBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelDelMailBtn", CCControlButton*, m_cancelDelMailBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_delAllMailBtn", CCControlButton*, m_delAllMailBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_delMailOpBtn", CCControlButton*, m_delMailOpBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNode", CCNode*, this->m_goldNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldBuyBtn", CCControlButton*, this->m_goldBuyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gold", CCLabelIF*, this->m_gold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldName", CCLabelIF*, this->m_goldName);
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_googlePlayNode", CCNode*, m_googlePlayNode);
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNewNode", CCNode*, this->m_goldNewNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeGoldIcon", CCNode*, this->m_nodeGoldIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_UserNode", CCNode*, this->m_UserNode);
    
    return false;
    
}

//UIComponentGold
UIComponentGold* UIComponentGold::create(long goldValue) {
    auto ret = new UIComponentGold();
    if (ret && ret->init(goldValue)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UIComponentGold::init(long goldValue) {
    bool ret = false;
    if (CCLayer::init()) {
        CCBLoadFile("gameUICoin",this,this);
        mFinalValue = goldValue;
        mNowValue = goldValue;
        mInAnimation = false;
        m_goldNum->setString(CC_CMDITOAL(mNowValue));
//        this->getAnimationManager()->runAnimationsForSequenceNamed("0");
        ret = true;
    }
    return ret;
}
bool UIComponentGold::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum", CCLabelIF*, m_goldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldIcon", CCSprite*, m_goldIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldStar", CCSprite*, m_goldStar);
    return false;
}
void UIComponentGold::setGoldValue(long goldValue){
    mFinalValue = goldValue;
    if(mFinalValue>0 && mFinalValue!=mNowValue && !mInAnimation){
//        this->getAnimationManager()->runAnimationsForSequenceNamed("1");
        m_goldStar->setVisible(false);
        CCSequence* pSizeSeq = CCSequence::create(CCScaleTo::create(0.1f, 0.95f), CCScaleTo::create(0.1f, 1.15f), NULL);
        m_goldIcon->runAction(CCRepeatForever::create(pSizeSeq));
        CCSequence* pSizeSeq1 = CCSequence::create(CCScaleTo::create(0.1f, 0.95f), CCScaleTo::create(0.1f, 1.15f), NULL);
        m_goldNum->runAction(CCRepeatForever::create(pSizeSeq1));
        mInAnimation=true;
        schedule(schedule_selector(UIComponentGold::playAnimation), 0.05);
    }
}
void UIComponentGold::playAnimation(float dt){
    long value = mFinalValue-mNowValue;
    if (value == 0) {
        m_goldIcon->stopAllActions();
        m_goldIcon->setScale(1.0f);
        m_goldNum->stopAllActions();
        m_goldNum->setScale(1.0f);
        m_goldStar->setVisible(true);
//        this->getAnimationManager()->runAnimationsForSequenceNamed("0");
        mInAnimation=false;
        unschedule(schedule_selector(UIComponentGold::playAnimation));
    }else{
        if (abs(value) > 25) {
            mNowValue += value/2;
        }else if (abs(value) > 15){
            mNowValue += 5*value/abs(value);
        }else{
            mNowValue += value/abs(value);
        }
        m_goldNum->setString(CC_CMDITOAL(mNowValue));
    }
}

//龙晶UIComponentLongJing
UIComponentLongJing* UIComponentLongJing::create(long goldValue) {
    auto ret = new UIComponentLongJing();
    if (ret && ret->init(goldValue)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UIComponentLongJing::init(long goldValue) {
    bool ret = false;
    if (CCLayer::init()) {
        CCBLoadFile("gameUILongJing",this,this);
        mFinalValue = goldValue;
        mNowValue = goldValue;
        mInAnimation = false;
        m_goldNum->setString(CC_CMDITOAL(mNowValue));
        //        this->getAnimationManager()->runAnimationsForSequenceNamed("0");
        ret = true;
    }
    return ret;
}
bool UIComponentLongJing::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum", CCLabelIF*, m_goldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldIcon", CCSprite*, m_goldIcon);
    return false;
}
void UIComponentLongJing::setGoldValue(long goldValue){
    mFinalValue = goldValue;
    if(mFinalValue>0 && mFinalValue!=mNowValue && !mInAnimation){
        CCSequence* pSizeSeq = CCSequence::create(CCScaleTo::create(0.1f, 0.95f), CCScaleTo::create(0.1f, 1.15f), NULL);
        m_goldIcon->runAction(CCRepeatForever::create(pSizeSeq));
        CCSequence* pSizeSeq1 = CCSequence::create(CCScaleTo::create(0.1f, 0.95f), CCScaleTo::create(0.1f, 1.15f), NULL);
        m_goldNum->runAction(CCRepeatForever::create(pSizeSeq1));
        mInAnimation=true;
        schedule(schedule_selector(UIComponentLongJing::playAnimation), 0.05);
    }
}
void UIComponentLongJing::playAnimation(float dt){
    long value = mFinalValue-mNowValue;
    if (value == 0) {
        m_goldIcon->stopAllActions();
        m_goldIcon->setScale(1.0f);
        m_goldNum->stopAllActions();
        m_goldNum->setScale(1.0f);
        mInAnimation=false;
        unschedule(schedule_selector(UIComponentLongJing::playAnimation));
    }else{
        if (abs(value) > 25) {
            mNowValue += value/2;
        }else if (abs(value) > 15){
            mNowValue += 5*value/abs(value);
        }else{
            mNowValue += value/abs(value);
        }
        m_goldNum->setString(CC_CMDITOAL(mNowValue));
    }
}

//UIComponent
static UIComponent *_instance = NULL;

UIComponent* UIComponent::getInstance() {
    
    if (!_instance) {
        
        _instance = new UIComponent();
        
        if (_instance){
            
            _instance->init(CCDirector::sharedDirector()->getWinSize());
            
        }
        
        GlobalData::shared()->isUiInti = true;
        
    }
    
    return _instance;
    
}



UIComponent::~UIComponent(){
    
}



UIComponent::UIComponent():m_bUIShowFlag(true),m_goldNum(NULL){
    
    m_CanClickTarget = false;
    
    m_recommandQuest = NULL;
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    m_titleOldTitle = NULL;
}



void UIComponent::purgeData(){
    
    CC_SAFE_RELEASE_NULL(_instance);
    
    GlobalData::shared()->isUiInti = false;
    
}





bool UIComponent::init(CCSize size) 

{
    CCLOG("UI INIT");
    bool bRet = false;
    
    userLevelCommandCount = 0;
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        CCBLoadFile("gameUI_HD",this,this,true);
    }
    else
    {
//        CCBLoadFile("_gameUI",this,this,true);
        CCBLoadFile("_game_ui", this, this, true);
    }
    m_questRecSpr->setVisible(false);
    CanShowQuestPrc = false;
    
    this->setContentSize(CCSizeMake(size.width, size.height));
    
    m_power->setFntFile("Arial_Bold_Border.fnt");
    
    setFavoriteIconVisible(false);
    this->m_gold->setString(CC_CMDITOA(GlobalData::shared()->playerInfo.gold).c_str());
    if(!this->m_goldNum){
        m_goldNum = UIComponentGold::create(GlobalData::shared()->playerInfo.gold);
        m_nodeGoldIcon->addChild(m_goldNum);
    }else{
        this->m_goldNum->setGoldValue(GlobalData::shared()->playerInfo.gold);
    }
    //龙晶
    if(!m_longJingNum){
        m_longJingNum = UIComponentLongJing::create(GlobalData::shared()->playerInfo.longJing);
        m_sprNode->addChild(m_longJingNum);
    }else{
        this->m_longJingNum->setGoldValue(GlobalData::shared()->playerInfo.longJing);
    }
    
    
//    this->m_goldNum->setString(CC_CMDITOA(GlobalData::shared()->playerInfo.gold).c_str());
    
    //    this->m_money->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lMoney));
    
    this->m_wood->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lWood));
    
    this->m_iron->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lIron));
    
    this->m_food->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lFood));
    
    this->m_stone->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lStone));
    
    this->m_dragonGold->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lDragonGold));
    
    this->m_dragonFood->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lDragonFood));
    this->m_homeText->setString(_lang("108678"));
    this->setFoodColor();
    
    //    m_collectNode = CCNode::create();
    
    //    this->addChild(m_collectNode);
    
    m_collectBatch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("ui_wood.png")->getTexture());
    
    m_collectNode->addChild(m_collectBatch);
    
    
    m_dragonGoldBarNode->setVisible(false);
    m_dragonFoodBarNode->setVisible(false);
    showResBar();
    
    m_woodBarPos = this->m_woodBarNode->getPosition();
    
    m_ironBarPos = this->m_ironBarNode->getPosition();
    
    m_foodBarPos = this->m_foodBarNode->getPosition();
    
    m_stoneBarPos = this->m_stoneBarNode->getPosition();
    
    m_messageTextPt = this->m_messageText->getPosition();
    
    setOldResource();
    
    this->m_messageText->setString(_lang("105310").c_str());
    
    m_topModelLayer->setThroughBegan(std::bind(&UIComponent::onTouchBegan,this, std::placeholders::_1, std::placeholders::_2));
    m_topModelLayer->setThroughEnd(std::bind(&UIComponent::onTouchEnded,this, std::placeholders::_1, std::placeholders::_2));
    m_midModelLayer->setThroughBegan(std::bind(&UIComponent::onTouchBegan,this, std::placeholders::_1, std::placeholders::_2));
    m_midModelLayer->setThroughEnd(std::bind(&UIComponent::onTouchEnded,this, std::placeholders::_1, std::placeholders::_2));
    
    m_bomModelLayer->setThroughBegan(std::bind(&UIComponent::onTouchBegan,this, std::placeholders::_1, std::placeholders::_2));
    m_bomModelLayer->setThroughEnd(std::bind(&UIComponent::onTouchEnded,this, std::placeholders::_1, std::placeholders::_2));
    
    m_xCoordLabel->setString("X:");
    m_yCoordLabel->setString("Y:");
    m_zCoordLabel->setString("K:");
    //    this->m_PVEBtnTitle->setString(_lang("102159").c_str());
    
    this->m_storeBtnLabel->setString(_lang("102160").c_str());
    
    //    this->m_generalBtnTitle->setString(_lang("102161").c_str());
    
    this->m_allianceBtnTitle->setString(_lang("102161").c_str());
    
    //    this->m_levelName->setString(_lang("102203").c_str());
    
    //TODO 临时修改
    
    //    this->m_configSetBtnTitle->setString(_lang("102162").c_str());
    
    //    this->m_configSetBtnTitle->setString(_lang("101204").c_str());
    
    
    
//    this->m_powerName->setString(_lang("102163").c_str());
    
    this->m_goldName->setString(_lang("102153").c_str());
    
    this->m_mailBtnTitle->setString(_lang("101205").c_str());
    
    this->m_questName->setString(_lang("107500").c_str());
    
    this->m_questTitle->setString(_lang("107527").c_str());
    this->m_questTitle->setMaxScaleXByWidth(70.0);
    
    this->m_homeBackBtnTitle->setString(_lang("108538").c_str());
    
    this->m_worldBtnTitle->setString(_lang("105601").c_str());
    
    m_queueNode->setVisible(false);
    
    m_googlePlayNode->setVisible(false);
    
    int vipLv = VipUtil::getVipLevel(GlobalData::shared()->playerInfo.vipPoints);
    
    this->m_vipText->setFntFile("pve_fnt_title.fnt");
    
    this->m_vipText->setString(CC_ITOA(vipLv)); //_lang_1("103001", CC_ITOA(vipLv)));
    
    m_chatName->setFntFile("pve_fnt_title.fnt");
    
    
    
    m_ctrlNodePt = ccp(this->m_mainControlNode->getPositionX(),this->m_mainControlNode->getPositionY());
    
    //    m_chatNodePt = ccp(this->m_chatNode->getPositionX(), this->m_chatNode->getPositionY());
    
    
    
    //队列面板相关设置
    
    m_expansionNode->setVisible(false);
    
    m_cdBG->setVisible(false);
    
    m_homeBack->setVisible(false);
    
    
    
    m_flyTextNode = CCNode::create();
    
    m_flyTextNode->setPosition(ccp(0, size.height-40));
    
    this->addChild(m_flyTextNode);
    
    
    
    m_clickNode = CCNode::create();
    
    this->addChild(m_clickNode);
    
    
    
    updateResStatus();
    
    
    
    //User EXP
    
    setUserData();
    
    isPlayingPowerAni = false;
    
    bRet = true;
    
    
    
    onQuestStateUpdate(NULL);
//    refreshGoodsStatus(NULL);
    updateMailNum(NULL);
    
    //    this->m_generalSoulBtnTimes->setVisible(false);
    
    //    this->m_generalSoulBtnTimesBG->setVisible(false);
    
    this->m_fireNode->setVisible(false);
    
    m_upPt = m_upNode->getPosition();
    
    m_mainPt = m_mainControlNode->getPosition();
    
    m_UIQuestNodeStat = true;
    m_questPt = m_UIQuestNode->getPosition();
    
    updateHelpData(NULL);
    
    onUpdateStamine(NULL);
    
    goldExchangeType = 1;

//    resetGoldActivityBox(NULL);
    
    m_flyCBView = FlyCBuildView::create();
    m_flyCBView->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(m_flyCBView);
    m_wrireMailBtn->setVisible(false);
    
    auto particle1 = ParticleController::createParticle("QueueGlow_1");
    auto particle2 = ParticleController::createParticle("QueueGlow_2");
    m_giftNode2->addChild(particle1);
    m_giftNode2->addChild(particle2);

    this->schedule(schedule_selector(UIComponent::updateTime), 1);
    this->updateTime(0);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::refreshInit), MSG_INIT_FINISH, NULL);//init之后 如果ui已经创建修改ui属性

//    CCArray* uidArray=CCArray::create();
//    uidArray->addObject(CCString::create("132658621000001"));
    CCArray* picArray=CCArray::create();
//
//    MailController::getInstance()->createChatRoom(uidArray, picArray, picArray, "Third Room", "dadadada!");
//    CCArray* uidArray2=CCArray::create();
//    uidArray2->addObject(CCString::create("2693745000002"));
//    MailController::getInstance()->inviteChatRoomMembers("Empire01192", uidArray2, picArray, picArray, "401677000001");
//    MailController::getInstance()->kickChatRoomMembers("Empire010d2", uidArray2, picArray, picArray, "401677000001");
//    MailController::getInstance()->modifyChatRoomName("android chatroom", "388327000001");
//    MailController::getInstance()->quitChatRoom( "388327000001");
//    MailController::getInstance()->sendChatRoomMsg("hello everyone", "399237000001");

    m_flyArrow = CCLoadSprite::createSprite("UI_hand.png");//guide_arrow_new.png
    m_flyArrow->setAnchorPoint(ccp(0, 0));
    m_flyArrow->setVisible(false);
    m_csNode->removeAllChildrenWithCleanup(true);
    CSAWorldBtn* csBtn = CSAWorldBtn::create();
    csBtn->setPosition(ccp(35, 40));
    m_csNode->addChild(csBtn);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_rechargeNode->setVisible(false);
    }
    // tao.yu 第一版不开放vip功能
    // guo.jiang 等美术数字字体完成后设置数字
    m_vipText->setVisible(false);
//    m_sprVip->setVisible(false);
    m_sprVipHui->setVisible(false);
    // ---------------
    return bRet;
}

void UIComponent::setOldResource()

{
    
    this->oldGold=GlobalData::shared()->playerInfo.gold;
    
    this->oldMoney=GlobalData::shared()->resourceInfo.lMoney;
    
    this->oldWood=GlobalData::shared()->resourceInfo.lWood;
    
    this->oldIron=GlobalData::shared()->resourceInfo.lIron;
    
    this->oldFood=GlobalData::shared()->resourceInfo.lFood;
    
    this->oldStone=GlobalData::shared()->resourceInfo.lStone;
    
}



void UIComponent::onEnter()

{
    
    CCLayer::onEnter();
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::onCityResoureceUpdate), MSG_CITY_RESOURCES_UPDATE, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::onSceneChanged), MSG_SCENE_CHANGED, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::onAddQueue), MSG_QUEUE_ADD, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::onAddQueue), MSG_QUEUE_REMOVE, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::onUpdateStamine), MSG_CURRENT_STAMINE, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::onQuestRemove), QUEST_NUM_CHANGE, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::onQuestStateUpdate), QUEST_STATE_UPDATE, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::updateMailNum), MAIL_LIST_CHANGE, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::updateMailNum), MAIL_SAVE_LIST_CHANGE, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::updateMailNum), MAIL_LIST_DELETE, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::updateMailNum), MAIL_CONTENT_READ, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::showCityFireBtn), MSG_SHOW_CITY_FIRE, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::onShowFrameEff), ENEMY_INFO_CHANGE, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::updateHelpData), MSG_ALLIACNE_HELP_NUM_CHANGE, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::updateHelpData), MSG_CHANGE_ALLIANCE_MILITARY_NUM, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::onChangePic), MSG_CHANGE_PLAYER_PIC, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::playAddPowerAni), MSG_COLLECT_SOLDIER_ADD_POWER, NULL);
    
    // tao.yu 第一版不开放vip功能
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::refreshVIPStatus), VIP_REFRESH, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::updateActivityEvent), MSG_UPDATE_ACTIVITY_EVENT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::resetGoldActivityBox), GOLDEXCHANGE_LIST_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::resetRepayActivity), MSG_REPAY_INFO_INIT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::onShowMoveBuild), MSG_MOVE_BUILD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::setTestFBFlame), TESTFEEDBACK_OPEN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::pushFeedSuccess), MSG_FBFeedDialogResult, NULL);
       //    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(UIComponent::refreshGoodsStatus),MSG_HAS_NEW_GOOD, NULL);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(UIComponent::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(UIComponent::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(UIComponent::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(Layer::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this); //addEventListenerWithFixedPriority(listener, -1);
    _touchListener = listener;
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::onPlayExpParticle), MSG_SHOW_EXP_PARTICLE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::onPlayPowerParticle), MSG_SHOW_POWER_PARTICLE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::leaveCKFServer), LEAVE_CKF_SERVER, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UIComponent::updateEquipTips), "showEquipTips", NULL);
    
    if (GlobalData::shared()->playerInfo.isInAlliance()) {
        m_countryIcon->setVisible(false);
        m_allianceIcon->setVisible(true);
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        if(ChatServiceCocos2dx::enableNativeChat){
            refreshChatInfo(CHAT_ALLIANCE);
        }
        else
        {
            updateChatInfo(CHAT_ALLIANCE);
        }
#else
        //IOS重写
        
        if(ChatServiceCocos2dx::enableNativeChat){
            updateChatInfoIOS(CHAT_ALLIANCE);
        }
        else
        {
            updateChatInfo(CHAT_ALLIANCE);
        }
        
//        if(ChatServiceCocos2dx::isChatShowing_fun()){
//            refreshChatInfo(CHANNEL_TYPE_ALLIANCE);
//        }
//        else
//        {
//            updateChatInfo(CHAT_ALLIANCE);
//        }
#endif
        

//        ChatController::getInstance()->sendRequestChat(CHAT_ALLIANCE);
    }
    else
    {
        m_countryIcon->setVisible(true);
        m_allianceIcon->setVisible(false);
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        if(ChatServiceCocos2dx::enableNativeChat){
            refreshChatInfo(CHAT_COUNTRY);
        }
        else
        {
            updateChatInfo(CHAT_COUNTRY);
        }
#else
        
        if(ChatServiceCocos2dx::enableNativeChat){
            updateChatInfoIOS(CHAT_COUNTRY);
        }
        else
        {
            updateChatInfo(CHAT_COUNTRY);
        }
        //IOS重写
//        if(ChatServiceCocos2dx::isChatShowing_fun()){
//            refreshChatInfo(CHANNEL_TYPE_COUNTRY);
//        }
//        else
//        {
//            updateChatInfo(CHAT_COUNTRY);
//        }
#endif

//        ChatController::getInstance()->sendRequestChat(CHAT_COUNTRY);
    }
    
    m_pic->removeAllChildrenWithCleanup(true);
    
    CCSprite* head = CCLoadSprite::createSprite(GlobalData::shared()->playerInfo.getPic().c_str());
    m_pic->addChild(head);
    
    CCCommonUtils::setSpriteMaxSize(head, 77,true);
    if (m_headImgNode == NULL)
    {
        m_headImgNode = HFHeadImgNode::create();
    }
    if (GlobalData::shared()->playerInfo.isUseCustomPic())
    {
        m_headImgNode->initHeadImgUrl2(m_pic, GlobalData::shared()->playerInfo.getCustomPicUrl(), 1.0f, 77, true);
    }
    
    onShowFrameEff(NULL);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(UIComponent::onEnterFrame), this, 1.0f,kCCRepeatForever, 0.0f, false);
    // tao.yu 第一版不开放vip功能
//    refreshVIPStatus(0.0);
    
    showFlygold();
    resetGoldActivityBox(NULL);
    resetRepayActivity(NULL);
    //提示可打造并穿戴装备
    updateEquipTips(NULL);
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::postUIShow();
#endif
}

void UIComponent::onEnterFrame(float dt)

{

    //    if(FunBuildController::getInstance()->curFoodOutPreSed < 0)
    
    //    {
    
    //        CCActionInterval * tinto = CCTintBy::create(0.5, 0, 255, 255);
    
    //        this->m_food->runAction(tinto);
    
    //    }
    
    if(true || (CanShowQuestPrc && !m_questRecSpr->isVisible())) {
        QuestPrcTimes ++;
        if (QuestPrcTimes>=7) {
            QuestPrcTimes = 0;
            playQuestRect();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TALK_OPEN);
        }
    }
    if(m_testFeedBackNode->isVisible()){
        double leftT = TestFeedBackController::shared()->getEndTime() - GlobalData::shared()->getTimeStamp();
        if(leftT>0){
            m_lblTestFeedBack->setString(CC_SECTOA(leftT));
        }else{
             m_lblTestFeedBack->setString("");
        }
    }
}



void UIComponent::showCityFireBtn(CCObject* p){
    this->updateTime(0);
}

void UIComponent::checkCityFire(float t){
    bool flag = false;
    string tmpStart = "BrokenCity_";
    unsigned int fireRate = 0;
    if (GlobalData::shared()->cityTileCountry == NEUTRALLY) {
        fireRate = GlobalData::shared()->worldConfig.fire[2];
        tmpStart = "BrokenCityFire_";
    }else {
        fireRate = GlobalData::shared()->worldConfig.fire[0];
    }
    auto now = WorldController::getInstance()->getTime();
    unsigned int fix = 0;
    if (GlobalData::shared()->cityStartFireStamp < GlobalData::shared()->cityFireStamp) {
        double start = MIN(now, GlobalData::shared()->cityFireStamp);
        double duration = start - GlobalData::shared()->cityStartFireStamp;
        fix = (unsigned int)(duration * (double)fireRate / 3600000.0);
    }
    unsigned int cityDefMax = GlobalData::shared()->imperialInfo[FUN_BUILD_WALL_ID].para2 + CCCommonUtils::getEffectValueByNum(125);
    unsigned int cityDefFix = GlobalData::shared()->cityDefenseVal - fix;
    if(now < GlobalData::shared()->cityFireStamp){
        flag = true;
    }
    if(flag && SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN){
        m_fireNode->setVisible(true);
        CCNode* node = m_fireNode->getChildByTag(99);
        if(node && node->getChildrenCount()<=0){
            //todo
            CCNode* nodeIcon = m_fireNode->getChildByTag(88);
            CCSprite* fireIcon = dynamic_cast<CCSprite*>(nodeIcon);
            if(fireIcon){
                if(GlobalData::shared()->cityTileCountry == NEUTRALLY){
                    fireIcon->initWithSpriteFrame(CCLoadSprite::loadResource("icon_burning2.png"));
                }else{
                    fireIcon->initWithSpriteFrame(CCLoadSprite::loadResource("icon_burning.png"));
                }
            }
            for (int i=1; i<5; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                if(GlobalData::shared()->cityTileCountry == NEUTRALLY){
                    particle->setPositionX(5);
                }
                node->addChild(particle);
            }
            string tmpFile = "CombustionUI_";
            for (int i=1; i<=2; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpFile.c_str(),i)->getCString());
                particle->setPosition(ccp(-320,0));
                node->addChild(particle);
            }
            CCScale9Sprite* node2 = (CCScale9Sprite*)m_fireNode->getChildByTag(199);
            if(node2){
                node2->setVisible(false);
//                auto winSize = CCDirector::sharedDirector()->getWinSize();
//                node2->setInsetBottom(10);
//                node2->setInsetLeft(2);
//                node2->setInsetRight(2);
//                node2->setInsetTop(2);
//                node2->setContentSize(CCSize(690,winSize.height-150));
//                node2->setAnchorPoint(ccp(1, 0));
//                node2->setPositionX(130);
            }
        }
    }else{
        m_fireNode->setVisible(false);
        CCNode* node = m_fireNode->getChildByTag(99);
        if(node && node->getChildrenCount()>0){
            node->removeAllChildrenWithCleanup(true);
        }
    }
}

//播放上下动画三次
void UIComponent::playQusetTiAnim(){
    float x = -398.8;
    float y = 75.4;
    if (CCCommonUtils::isIosAndroidPad())
    {
        x = -772.8;
        y = 119.3;
    }
    CCEaseExponentialIn* easeInAction = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCMoveTo::create(0.2, ccp(x, y+10))));
    CCFiniteTimeAction* easeInActionRevers =  CCMoveTo::create(0.2, ccp(x, y));
    
    CCEaseExponentialIn* easeInAction1 = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCMoveTo::create(0.2, ccp(x, y-10))));
    CCMoveTo* easeInAction1Revers = CCMoveTo::create(0.2, ccp(x, y));
    CCRepeat* repeatAction = CCRepeat::create(CCSequence::create(
                                                                 easeInAction
                                                                 , easeInActionRevers
                                                                 , easeInAction1
                                                                 ,easeInAction1Revers
                                                                 ,NULL)
                                              , 3);
    this->m_questTipNode->runAction(CCSequence::create(
                                                       repeatAction
                                                       ,CCDelayTime::create(5.0)
                                       , CCCallFunc::create(this, callfunc_selector(UIComponent::playQusetTiAnimFinish))
                                       , NULL
                                       ));

}
void UIComponent::playQusetTiAnimFinish(){
    this->m_questTipNode->setPosition(-398.8, 75.4);
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->m_questTipNode->setPosition(-772.8, 119.3);
    }
    playQusetTiAnim();

}

void UIComponent::onQuestStateUpdate(CCObject* p){
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(GlobalData::shared()->android_native_chat == 1 && ChatServiceCocos2dx::isChatShowing){
        return;
    }
#endif
    int completeNum = QuestController::getInstance()->getCompleteQuestNum();
    
    QuestInfo *info = QuestController::getInstance()->getRecommendQuest(false);
    
    bool isrefersh = false;
    
    if(m_recommandQuest!=NULL&&m_recommandQuest!=info){
        
        isrefersh = true;
        
    }
    
    m_recommandQuest = info;
    
    m_questNameNode->removeAllChildren();
    
    int dx =150;
    
    m_questGoldNode->removeAllChildren();
    if(info){
        
        auto arr = CCArray::create();
        
        std::string nameStr = info->name;
        
        arr->addObject(CCString::create(nameStr));
        
        auto rollText = RollingText::create(arr, 390);
        
        m_questNameNode->addChild(rollText);
        
        checkShowQuestPrc();
        
        bool isGold = false;
        for (int i=0; i<info->reward->count(); i++) {
            auto dic = dynamic_cast<CCDictionary*>(info->reward->objectAtIndex(i));
            int type = dic->valueForKey("type")->intValue();
            if (type == R_GOLD) {
                isGold = true;
                break;
            }
        }
        
        if (isGold) {
            auto goldAni = AllianceAni::create();
            goldAni->m_alliance->setVisible(false);
            m_questGoldNode->addChild(goldAni);
        }
        
        if (!m_questContextBG1->isVisible()) {
            playQuestRect();
        }
        
    }else{
        
        
        
    }
    m_questTipNode->stopAllActions();
    if(completeNum == 0){
        
        this->m_questTipBG->setVisible(false);
        
        this->m_questTipNum->setString("");
        
    }else{
        playQusetTiAnim();
        this->m_questTipBG->setVisible(true);
        
        this->m_questTipNum->setString(CC_ITOA(completeNum));
        
    }
    
    
    
    if(isrefersh){
        
        m_questAnimNode->removeAllChildren();
        
        ParticleController::initParticle();
        
        
        
        auto newBatch = ParticleController::createParticleBatch();
        
        auto praticle1 = ParticleController::createParticle("Capacity_1");
        
        newBatch->addChild(praticle1);
        
        auto praticle2 = ParticleController::createParticle("Capacity_2");
        
        newBatch->addChild(praticle2);
        
        auto praticle3= ParticleController::createParticle("Capacity_3");
        
        newBatch->addChild(praticle3);
        
        auto praticle4 = ParticleController::createParticle("Capacity_4");
        
        newBatch->addChild(praticle4);
        
        auto praticle5 = ParticleController::createParticle("Capacity_5");
        
        newBatch->addChild(praticle5);
        
        auto praticle6 = ParticleController::createParticle("Capacity_6");
        
        newBatch->addChild(praticle6);
        
        m_questAnimNode->addChild(newBatch);
        
        praticle1->setPosition(ccp(dx, 0));
        
        praticle1->setAnchorPoint(ccp(0, 0));
        
        praticle2->setPosition(ccp(dx, 0));
        
        praticle2->setAnchorPoint(ccp(0, 0));
        
        praticle3->setPosition(ccp(dx, 0));
        
        praticle3->setAnchorPoint(ccp(0, 0));
        
        praticle4->setPosition(ccp(dx, 0));
        
        praticle4->setAnchorPoint(ccp(0, 0));
        
        praticle5->setPosition(ccp(dx, 0));
        
        praticle5->setAnchorPoint(ccp(0, 0));
        
        praticle6->setPosition(ccp(dx, 0));
        
        praticle6->setAnchorPoint(ccp(0, 0));
        
        //questBGAction
        
        questBGHideAction();
        
    }
    
}



void UIComponent::onQuestRemove(CCObject* p){
    
    onQuestStateUpdate(NULL);
    
}



void UIComponent::onExit()

{
    
    onHideFrameEff(NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_DELETE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_SAVE_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REPAY_INFO_INIT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_CHANGE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_CONTENT_READ);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, QUEST_NUM_CHANGE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, QUEST_STATE_UPDATE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CITY_RESOURCES_UPDATE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCENE_CHANGED);
    
    //    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ACHIEVEMENT_DATA_CHANGE);
    
    //    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CITY_GCMREWARD_UPDATE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_QUEUE_ADD);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_QUEUE_REMOVE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CURRENT_STAMINE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ENEMY_INFO_CHANGE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIACNE_HELP_NUM_CHANGE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_ALLIANCE_MILITARY_NUM);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_PLAYER_PIC);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_COLLECT_SOLDIER_ADD_POWER);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHOW_CITY_FIRE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MOVE_BUILD);
    _eventDispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;

 //   setTouchEnabled(false);
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UIComponent::onEnterFrame), this);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHOW_EXP_PARTICLE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHOW_POWER_PARTICLE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, VIP_REFRESH);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UPDATE_ACTIVITY_EVENT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GOLDEXCHANGE_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TESTFEEDBACK_OPEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INIT_FINISH);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBFeedDialogResult);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, LEAVE_CKF_SERVER);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_HAS_NEW_GOOD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "showEquipTips");
    // tao.yu 第一版本不开放vip
//    this->unschedule(schedule_selector(UIComponent::refreshVIPStatus));
    
    CCLayer::onExit();
    
}

void UIComponent::UIHide()

{
    
    //    this->m_chatNode->setVisible(false);
    
    this->m_UserNode->setVisible(false);
    this->m_goldNewNode->setVisible(false);
    
    this->m_mainControlNode->setVisible(false);
    
    this->m_uiTitle->setVisible(false);
    if(CCCommonUtils::isIosAndroidPad() && m_titleOldTitle)
    {
        m_titleOldTitle->setVisible(false);
    }
    
    this->m_UIQuestNode->setVisible(false);
    
    this->m_queueNode->setVisible(false);
    setTestFBNodeVisible(false);
    setLotteryNodeVisible(false);
    setCrossServiceNodeVisible(false);
}

void UIComponent::UIMove(bool _bShow)

{

    if (_bShow == m_bUIShowFlag) {
        
        return;
        
    }
    this->setActivityStatus();
    m_upNode->setPosition(m_upPt.x,m_upPt.y+200);
    
    m_mainControlNode->setPosition(m_mainPt.x,m_mainPt.y -150);
    
    m_UIQuestNode->setPosition(m_questPt.x, m_questPt.y-220);
    
    if (_bShow) {
        
        CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(m_upPt.x,m_upPt.y));
        
        this->m_upNode->runAction(CCSequence::create(userMove,NULL));
        
        CCMoveTo* mainMove = CCMoveTo::create(0.3f, ccp(m_mainPt.x, m_mainPt.y));
        
        this->m_mainControlNode->runAction(CCSequence::create(mainMove,NULL));
        
        CCMoveTo* questMove = CCMoveTo::create(0.3f, ccp(m_questPt.x, m_questPt.y));
        
        this->m_UIQuestNode->runAction(CCSequence::create(questMove,CCCallFunc::create(this, callfunc_selector(UIComponent::questIconAction)),NULL));
        
        m_bUIShowFlag = true;
        
    }
    
    else
        
    {

//        m_upNode->setPosition(m_upPt.x,m_upPt.y+200);
//        
//        m_mainControlNode->setPosition(m_mainPt.x,m_mainPt.y -150);
//        
//        m_UIQuestNode->setPosition(m_questPt.x, m_questPt.y-220);
        
        m_bUIShowFlag = false;
        
    }
    
}



void UIComponent::questIconAction()

{
    m_upNode->setPosition(m_upPt.x,m_upPt.y);
    
    m_mainControlNode->setPosition(m_mainPt.x,m_mainPt.y);
    
    m_UIQuestNode->setPosition(m_questPt.x, m_questPt.y);
    
    m_questContextBG1->setVisible(false);
    m_questTipNpcNode->setVisible(false);
    m_questContextBG->setScaleX(0);
    
    m_questNameNode->setVisible(false);
    
    m_questIcon->setRotation(-90);
    
    CCRotateTo* iconRotate = CCRotateTo::create(0.3f, 0);
    
    this->m_questIcon->runAction(CCSequence::create(iconRotate,CCCallFunc::create(this, callfunc_selector(UIComponent::questBGAction)),NULL));

}

void UIComponent::questBGHideAction(){
    
    m_questNameNode->setVisible(false);
    
    CCScaleTo* bgScale = CCScaleTo::create(0.3f, 1, 0);
    
    this->m_questContextBG->runAction(CCSequence::create(bgScale,CCCallFunc::create(this, callfunc_selector(UIComponent::questBGAction)),NULL));
    
}

void UIComponent::questBGAction()

{
    
    CCScaleTo* bgScale = CCScaleTo::create(0.3f, 1, 1);
    
    this->m_questContextBG->runAction(CCSequence::create(bgScale,CCCallFunc::create(this, callfunc_selector(UIComponent::showQuestText)),NULL));
    
}



void UIComponent::showQuestText()

{
    
    m_questNameNode->setVisible(true);
    playQuestRect();
}



void UIComponent::hidePopupBG()

{
    
    if (this->m_popupTitleBg1->isVisible()) {
        
        this->m_goldNode->setVisible(false);
        
        this->m_popupTitleBg1->setVisible(false);
        
        this->m_popupTitleBg2->setVisible(false);
        
        this->m_popupTitleName->setVisible(false);
        
        //        this->m_line1->setVisible(false);
        
        //        this->m_line2->setVisible(false);
        
        this->m_mainControlNode->setVisible(false);
        
    }
    
    else
        
    {
        
        //        this->m_goldNode->setVisible(true);
        
        this->m_popupTitleBg1->setVisible(true);
        
        this->m_popupTitleBg2->setVisible(!true); // guo.jiang
        
        this->m_popupTitleName->setVisible(true);
        
        //        this->m_line1->setVisible(true);
        
        //        this->m_line2->setVisible(true);
        
        this->m_mainControlNode->setVisible(true);
        
    }
    
    
    
}



void UIComponent::hideReturnBtn()

{
    
    this->m_popupReturnBtn->setVisible(false);
    
}



void UIComponent::setPopupTitleName(string _name)

{
    
    this->m_popupTitleName->setString(_name.c_str());
    
}



void UIComponent::showResourceBar(bool _bShow)

{
    
    if (_bShow) {
        
        m_UserNode->setVisible(true);
        
        m_otherPartNode->setVisible(false);
        m_goldNewNode->setVisible(false);
        
        m_buildNode->setVisible(false);
        
        m_powerAndBgNode->setVisible(false);
        
    }
    
    else
        
    {
        
        m_UserNode->setVisible(false);
        
        m_otherPartNode->setVisible(true);
        m_goldNewNode->setVisible(false);
        
//        m_buildNode->setVisible(true);
        CheckGuideUIShow();
        
        m_powerAndBgNode->setVisible(true);
        
    }
    
}



void UIComponent::showPopupView(int type, bool isHD)
{
    this->m_newuserHelpNode->setVisible(false);
    m_popupTitleBg2->setVisible(!true); // guo.jiang
    this->m_powerAndBgNode->setVisible(true);
     m_otherPartNode->setVisible(true);
    setTestFBNodeVisible(false);
    setLotteryNodeVisible(false);
    setCrossServiceNodeVisible(false);
    if(type == 5){
        this->m_uiTitle->setVisible(false);
        if (CCCommonUtils::isIosAndroidPad() && m_titleOldTitle)
        {
            m_titleOldTitle->setVisible(false);
        }
        this->m_popupReturnBtn->setVisible(false);
        this->m_popupTitleName->setVisible(false);
        this->m_UserNode->setVisible(false);
        this->m_queueNode->setVisible(false);
        this->m_goldNode->setVisible(false);
        this->m_UIQuestNode->setVisible(false);
        this->m_rightNode->setVisible(false);
        this->m_inUiTitle->setVisible(false);
        this->m_buildNode->setVisible(false);
        this->m_mainControlNode->setVisible(false);
        this->m_faveNode->setVisible(false);
        this->m_miniNode->setVisible(false);
        m_homeBack->setVisible(false);
        this->m_world->setVisible(false);
        m_homeBack->setVisible(false);
        this->m_world->setVisible(false);
        m_worldUINode->setVisible(false);
    }else if (type==0||type==3||type==4||type==6||type==7||type==8||type==9||type==10||type==11) {
        if (CCCommonUtils::isIosAndroidPad() && !CCCommonUtils::getIsHDViewPort())
        {
            if (!m_titleOldTitle)
            {
                m_titleOldTitle = UIComponentOldTitle::create(static_cast<OldTitleType>(type));
                m_titleOldTitle->setDelegate(this);
                this->addChild(m_titleOldTitle);
            }
            
            m_titleOldTitle->setVisible(true);
            
            this->m_uiTitle->setVisible(false);
            this->m_popupReturnBtn->setVisible(false);
            this->m_popupTitleName->setVisible(false);
        }
        else {
            if (m_titleOldTitle)
            {
                m_titleOldTitle->setVisible(false);
            }
            this->m_uiTitle->setVisible(true);
            this->m_popupReturnBtn->setVisible(true);
            this->m_popupTitleName->setVisible(type==7?false:true);
        }

        
        
        if(type==6||type==7){
            m_popupTitleBg2->setVisible(false);
            this->m_UserNode->setVisible(true);
            this->m_powerAndBgNode->setVisible(false);
            m_otherPartNode->setVisible(false);
        }else{
            m_popupTitleBg2->setVisible(!true); // guo.jiang
            this->m_UserNode->setVisible(false);
            this->m_powerAndBgNode->setVisible(true);
            m_otherPartNode->setVisible(true);

        }
        
        if (type == 11)
        {
            this->m_popupTitleBg2->setVisible(false);
            this->m_popupTitleName->setVisible(false);
        }
        
        //    this->m_leftControlNode->setVisible(false);
        
        this->m_goldNode->setVisible(false);
        
        this->m_queueNode->setVisible(false);
        
        
        
        this->m_UIQuestNode->setVisible(false);
        
        //    this->m_mailNode->setVisible(false);
        
        this->m_rightNode->setVisible(false);
        
        this->m_fireNode->setVisible(false);
        
        this->m_inUiTitle->setVisible(true);
        
        this->m_buildNode->setVisible(false);
        
        
        
        this->m_mainControlNode->setVisible(false);
        
        this->m_faveNode->setVisible(false);
        this->m_miniNode->setVisible(false);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN) {
            
            m_homeBack->setVisible(true);
            
            this->m_world->setVisible(false);
        }
        
        else
            
        {
            
            m_homeBack->setVisible(false);
            
            this->m_world->setVisible(true);
            
            m_worldUINode->setVisible(false);
        }
        
    }
    
    else if (type == 1){
        
        this->m_popupReturnBtn->setVisible(true);
        
        
        if (CCCommonUtils::isIosAndroidPad() && !CCCommonUtils::getIsHDViewPort())
        {
            if (!m_titleOldTitle)
            {
                m_titleOldTitle = UIComponentOldTitle::create(static_cast<OldTitleType>(type));
                m_titleOldTitle->setDelegate(this);
                this->addChild(m_titleOldTitle);
            }
            
            
            this->m_uiTitle->setVisible(false);
            this->m_popupReturnBtn->setVisible(false);
            this->m_popupTitleName->setVisible(false);
        }
        else {
            this->m_uiTitle->setVisible(true);
            this->m_popupReturnBtn->setVisible(true);
            this->m_popupTitleName->setVisible(false);
        }
        
        this->m_uiTitle->setVisible(true);
        this->m_popupTitleName->setVisible(false);
        
        this->m_UserNode->setVisible(false);
        
        this->m_buildNode->setVisible(false);
        
        this->m_queueNode->setVisible(false);
        
        this->m_goldNode->setVisible(false);
        
        this->m_UIQuestNode->setVisible(false);
        
        this->m_rightNode->setVisible(false);
        
        this->m_faveNode->setVisible(false);
        this->m_miniNode->setVisible(false);
        this->m_inUiTitle->setVisible(false);
        
        
        
        this->m_mainControlNode->setVisible(false);
        
    }else if (type == 2){
        
        this->m_popupReturnBtn->setVisible(false);
        
        this->m_uiTitle->setVisible(false);
        if(CCCommonUtils::isIosAndroidPad() && m_titleOldTitle)
        {
            m_titleOldTitle->setVisible(false);
        }
        
        this->m_popupTitleName->setVisible(false);
        
        this->m_UserNode->setVisible(false);
        
        this->m_buildNode->setVisible(false);
        
        this->m_queueNode->setVisible(false);
        
        this->m_goldNode->setVisible(false);
        
        this->m_rightNode->setVisible(false);
        
//        this->m_UIQuestNode->setVisible(true);
        CheckGuideUIShow();
        
        this->m_mainControlNode->setVisible(true);
        
        this->m_faveNode->setVisible(false);
        this->m_miniNode->setVisible(false);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN) {
            
            m_homeBack->setVisible(false);
            this->m_world->setVisible(true);
            
        }
        
        else
            
        {
            
            m_homeBack->setVisible(true);
            this->m_world->setVisible(false);
            
        }
        
    }
    this->m_goldNewNode->setVisible(false);
    if (CCCommonUtils::isIosAndroidPad() && m_titleOldTitle)
    {
        m_titleOldTitle->m_wrireMailBtn->setVisible(false);
        m_titleOldTitle->m_mailBtnsNode->setVisible(false);
    }
    this->m_wrireMailBtn->setVisible(false);
    this->m_mailBtnsNode->setVisible(false);
    
    this->m_LongJingNode->setVisible(false);
    if (type==10) {
        this->m_LongJingNode->setVisible(true);
    }
    
    this->m_googlePlayNode->setVisible(false);
    if(type==4||type==6||type==7||type==8){
        this->m_goldNewNode->setVisible(true);
    }else if(type==3){
        
        
        
        MailController::getInstance()->CanRemoveMail = false;
//        m_wrireMailBtn->setVisible(false);
        if (CCCommonUtils::isIosAndroidPad() && m_titleOldTitle)
        {
            m_titleOldTitle->m_wrireMailBtn->setVisible(false);
            m_titleOldTitle->m_mailBtnsNode->setVisible(true);
            m_titleOldTitle->m_cancelDelMailBtn->setVisible(false);
            m_titleOldTitle->m_delAllMailBtn->setVisible(false);
        }
        else
        {
            this->m_wrireMailBtn->setVisible(false);
            this->m_mailBtnsNode->setVisible(true);
            m_cancelDelMailBtn->setVisible(false);
            m_delAllMailBtn->setVisible(false);
        }
        
        
        
    }else if(type == 9)
    {
        m_googlePlayNode->setVisible((GlobalData::shared()->analyticID == "market_global"));
    }else{
        if (CCCommonUtils::isIosAndroidPad() && m_titleOldTitle)
        {
            m_titleOldTitle->m_wrireMailBtn->setVisible(false);
            
        }
        else
            this->m_wrireMailBtn->setVisible(false);

    }
    this->setActivityStatus();
}



void UIComponent::onSceneChanged(CCObject* params){
    
    //    m_flyTextNode->setPosition(ccp(0, this->getContentSize().height-40));
    
    int currentSceneId = SceneController::getInstance()->currentSceneId;
    
    float winHeight = CCDirector::sharedDirector()->getWinSize().height;
    
    // main scene as default value
    
    if (!this->isVisible()) {
        
        this->setVisible(true);
        
    }
    
    this->m_UserNode->setVisible(true);
    
    this->m_UserResBg->setVisible(true);
    
    this->m_otherPartNode->setVisible(true);
    this->m_goldNewNode->setVisible(true);
    
//    this->m_buildNode->setVisible(true);
    
    this->m_powerAndBgNode->setVisible(true);
    
    this->m_uiTitle->setVisible(false);
    if(CCCommonUtils::isIosAndroidPad() && m_titleOldTitle)
    {
        m_titleOldTitle->setVisible(false);
    }
    
//    this->m_UIQuestNode->setVisible(true);
    CheckGuideUIShow();
    
    this->m_homeBack->setVisible(false);
    
    this->m_homeBackBtnTitle->setVisible(false);
    
    this->m_world->setVisible(true);
    
    this->m_worldBtnTitle->setVisible(true);
    
    this->m_goldNode->setVisible(true);
    
    this->m_popupReturnBtn->setVisible(false);
    
    this->m_popupTitleName->setVisible(false);
    
    this->m_queueNode->setVisible(false);
    if(SceneController::getInstance()->currentSceneId!=SCENE_ID_WORLD){
        this->m_queueCell->removeAllChildren();
        m_cdBG->setVisible(false);//隐藏进入世界地图的时候会出现的空行军队列框
        m_expansionNode->setVisible(false);
    }
    
    this->m_queueNode->setPosition(ccp(0, winHeight - 189));
    
    if (CCCommonUtils::isIosAndroidPad())
    {
       this->m_queueNode->setPosition(ccp(0, winHeight - 189*2.1));
    }
    
    this->m_worldUINode->setVisible(false);
    this->m_rightNode->setVisible(false);
    
    this->m_status->setVisible(true);
    
    this->m_mainControlNode->setVisible(true);
    
    this->m_inUiTitle->setVisible(true);
    
    this->m_faveNode->setVisible(false);
    this->m_miniNode->setVisible(false);
    this->m_newuserHelpNode->setVisible(false);
    setLotteryNodeVisible(false);
    setTestFBNodeVisible(false);
    setCrossServiceNodeVisible(false);
    if(currentSceneId == SCENE_ID_MAIN){
        if(FunBuildController::getInstance()->getMainCityLv() < 10){
            std::string key = NEW_USER_HELP_ON + CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID);
            if(CCUserDefault::sharedUserDefault()->getBoolForKey(key.c_str())==false){
                m_newuserHelpNode->setVisible(true);
                m_newuserHelpNode->setPositionY(CCDirector::sharedDirector()->getWinSize().height*0.5);
            }
        }
        m_testFeedBackNode->setPositionY(CCDirector::sharedDirector()->getWinSize().height*0.5 - 120);
        setTestFBNodeVisible(true);
        setLotteryNodeVisible(true);
    }else if (currentSceneId==SCENE_ID_PVE){
        
        
        
        this->m_homeBack->setVisible(true);
        
        //        this->m_PVE->setVisible(false);
        
        //        this->m_PVEBtnTitle->setVisible(false);
        
        //        this->m_mailNode->setVisible(false);
        
        //        this->m_fireNode->setVisible(false);
        
        this->m_rightNode->setVisible(false);
        
        
        
    }else if(currentSceneId==SCENE_ID_CROP
             
             ||currentSceneId==SCENE_ID_WOOD
             
             ||currentSceneId==SCENE_ID_MARKET
             
             ||currentSceneId==SCENE_ID_IRON
             
             ||currentSceneId==SCENE_ID_BARRACKS){
        
        
        
        this->m_homeBack->setVisible(true);
        
        //        this->m_PVE->setVisible(false);
        
        //        this->m_PVEBtnTitle->setVisible(false);
        
        
        
    }else if(currentSceneId==SCENE_ID_BATTLE){
        
        this->m_UserNode->setVisible(false);
        this->m_goldNewNode->setVisible(false);
        
        this->m_mainControlNode->setVisible(false);
        
        this->m_UIQuestNode->setVisible(false);
        
        this->m_homeBack->setVisible(true);
        
        //        this->m_PVE->setVisible(false);
        
        //        this->m_PVEBtnTitle->setVisible(false);
        
        //        this->m_leftControlNode->setVisible(false);
        
        //        this->m_chatNode->setVisible(false);
        
        this->m_queueNode->setVisible(false);
        
        //        this->m_mailNode->setVisible(false);
        
        //        this->m_fireNode->setVisible(false);
        
        this->m_rightNode->setVisible(false);
        
        
        
    }else if(currentSceneId==SCENE_ID_IMPERIAL){
        
        this->m_homeBack->setVisible(true);
        
    } else if(currentSceneId == SCENE_ID_WORLD) {
        
        this->m_UserNode->setVisible(true);
        this->m_goldNewNode->setVisible(true);
        setCrossServiceNodeVisible(true);
        
        this->m_queueNode->setVisible(true);
        if(WorldMapView::instance() && !WorldMapView::instance()->isMiniMapModel()){
            this->m_faveNode->setVisible(true);
        }
        this->m_miniNode->setVisible(true);
        this->changeMiniMapIcon();
        int resTypeNum = 2;
        
        this->m_queueNode->setPosition(ccp(0, winHeight - 125));
        if (CCCommonUtils::isIosAndroidPad())
        {
            this->m_queueNode->setPosition(ccp(0, winHeight - 125*2));
        }
        
        this->m_worldUINode->setVisible(true);
        this->m_world->setVisible(false);
        
        this->m_worldBtnTitle->setVisible(false);
        
        this->m_homeBack->setVisible(true);
        
        this->m_homeBackBtnTitle->setVisible(true);
        
        this->m_status->setVisible(false);
        
        this->m_UIQuestNode->setVisible(false);
        
        //进入世界弹出的提示界面，要求只在新手引导结束后,且不在迁城动画进入世界时，第一次进入世界时弹出
        //从本地缓存读取点击进入世界的状态如为0则弹出界面且将标志置为1，如为1则不再弹出
        if (!GuideController::share()->isInTutorial()){
            std::string isFirstPopKey = GlobalData::shared()->playerInfo.uid + "isFirstPop";
                
            //CCUserDefault::sharedUserDefault()->setIntegerForKey(isFirstPopKey.c_str(), 0);
            
            int isFirstPop = CCUserDefault::sharedUserDefault()->getIntegerForKey(isFirstPopKey.c_str(),0);
                
            if (isFirstPop==0) {
                SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
                PopupViewController::getInstance()->addPopupView(WorldPopupOnceView::create());
                    
                isFirstPop = 1;
                CCUserDefault::sharedUserDefault()->setIntegerForKey(isFirstPopKey.c_str(), isFirstPop);
                CCUserDefault::sharedUserDefault()->flush();
            }
        }
        if (WorldController::getInstance()->firstInWorld==true){
            std::string isFirstPopKey = GlobalData::shared()->playerInfo.uid + "isFirstPop";
            CCUserDefault::sharedUserDefault()->setIntegerForKey(isFirstPopKey.c_str(), 0);
            CCUserDefault::sharedUserDefault()->flush();
            WorldController::getInstance()->firstInWorld = false;
        }
        
        
    }
    //m_skillNode->setVisible(currentSceneId==SCENE_ID_MAIN);
    showCityFireBtn(NULL);
    
}



void UIComponent::onUpdateStamine(CCObject *params)

{
    
    float scalex = WorldController::getInstance()->currentStamine*1.0f/GlobalData::shared()->worldConfig.stamineMax;
    
    scalex = scalex>1 ? 1:scalex;
    
    
    
    m_stamineBar->setScaleX(scalex);
    
}



void UIComponent::setUserData(){
    
    if(GlobalData::shared()->playerInfo.level < GlobalData::shared()->MaxPlayerLv){
        
        string lv= "";
        
        lv+=CC_ITOA(GlobalData::shared()->playerInfo.level);
        
        this->m_level->setString(lv.c_str());
        
    }else{
        
        this->m_level->setString(_lang("102183").c_str());
        
    }
    
    GlobalData::shared()->playerInfo.battlePower = GlobalData::shared()->playerInfo.getTotalPower();
    double total = GlobalData::shared()->playerInfo.battlePower;
    
    double lastBattlePower = GlobalData::shared()->playerInfo.lastBattlePower;
    
    double addPower = total - lastBattlePower;
    
    currentPower = atoi(m_power->getString());
    
    if(currentPower==0 || addPower<=0){
        isPlayingPowerAni = false;
        total = MAX(total, 0);
//        m_power->setString(CC_CMDITOAL(total));
        m_power->setString(CC_CMDITOAL(total).c_str());
        GlobalData::shared()->playerInfo.lastBattlePower = GlobalData::shared()->playerInfo.battlePower;
        
    }else{
        if(!isPlayingPowerAni && addPower>0){
            
            this->controllerPowerAni();
            
        }
        
    }
    
    string expStr = CC_ITOA(GlobalData::shared()->playerInfo.exp);
    
    string expMaxStr = expStr+"/"+CC_ITOA(GlobalData::shared()->playerInfo.maxExp);
    
    float scalex = GlobalData::shared()->playerInfo.exp*1.0/GlobalData::shared()->playerInfo.maxExp;
    
    scalex = scalex>1 ? 1:scalex;

    m_expBar->setScaleX(scalex);
    
    
    
    if(GlobalData::shared()->playerInfo.maxExp<GlobalData::shared()->playerInfo.exp){//请求后台更新错误等级
        
        if(userLevelCommandCount>3){
            
            return;
            
        }
        
        userLevelCommandCount++;
        
        UserLevelCommand *cmd = new UserLevelCommand();
        
        cmd->sendAndRelease();
        
    }
    
}

void UIComponent::updateGold(int gold){
    
    auto &playerInfo = GlobalData::shared()->playerInfo;
    
    playerInfo.gold = gold;
    
    
    
    updateResStatus();
    
}

void UIComponent::updateLongJing(int longJing){
    auto &playerInfo = GlobalData::shared()->playerInfo;
    playerInfo.longJing = longJing;
    updateResStatus();
}

void UIComponent::updateEquipTips(CCObject* params){
    m_equipGlowNode->setVisible(false);
    if (EquipmentController::getInstance()->isShowEquipTips(-1)) {
        //播放动画
        CCLog("ShowEquipTips(-1)");
        m_equipGlowNode->setVisible(true);
    }
}

void UIComponent::onCityResoureceUpdate(CCObject* params)

{
    
    if(params){
        CCBool* notFly = (dynamic_cast<CCBool*>(params));
        if(notFly){
            updateResStatus();
            return;
        }
    }
    
    FloatingTypeEnum floatType = floating_type_normal;
    
    ccColor3B color = ccGREEN;
    
    int addy = 0;
    
    
    
    int addMoney = -oldMoney+GlobalData::shared()->resourceInfo.lMoney;
    
    //    int addGold = -oldGold+GlobalData::shared()->playerInfo.gold;
    
    int addFood = -oldFood+GlobalData::shared()->resourceInfo.lFood;
    
    int addWood = -oldWood+GlobalData::shared()->resourceInfo.lWood;
    
    int addIron = -oldIron+GlobalData::shared()->resourceInfo.lIron;
    
    int addStone = -oldStone+GlobalData::shared()->resourceInfo.lStone;
    
    string temp ="";
    
    
    
    this->setFoodColor();
    
    
    
    if(addMoney==0){
        
        temp = "";
        
    }else{
        
    }
    
    
    
    if(addWood==0 && FunBuildController::getInstance()->lastAddWood==0){
        
        temp = "";
        
    }else {
        
        if(addWood>0 || (addWood==0 && FunBuildController::getInstance()->lastAddWood>0)){
            
            temp = CC_ITOA(addWood);
            
            temp = "+"+temp;
            
            color = ccGREEN;
            
            floatType = floating_type_ui1;
            
            addy = -30;
            
        }else{
            
            temp = CC_ITOA(addWood);
            
            color = ccRED;
            
            floatType = floating_type_ui2;
            
            addy = 0;
            
        }
        
        if(m_UserNode->isVisible()) {
            
            if(addWood==0 && FunBuildController::getInstance()->lastAddWood>0 && GlobalData::shared()->resourceInfo.lWood >= GlobalData::shared()->resourceInfo.lStorageWood) {
                
                temp = _lang("102286");
                
            }
            
            CCCommonUtils::flyUiResText(temp.c_str(), this->m_flyTextNode,ccp(160, addy) ,color, floatType);
            
        }
        
    }
    
    
    
    if(addFood==0 && FunBuildController::getInstance()->lastAddFood==0){
        
        temp = "";
        
    }else {
        
        if(addFood>0 || (addFood==0 && FunBuildController::getInstance()->lastAddFood>0)){
            
            temp = CC_ITOA(addFood);
            
            temp = "+"+temp;
            
            color = ccGREEN;
            
            floatType = floating_type_ui1;
            
            addy = -30;
            
        }else{
            
            if(addFood<0) {
                
                temp = CC_ITOA(addFood);
                
                color = ccRED;
                
                floatType = floating_type_ui2;
                
                addy = 0;
                
            }
            
        }
        
        if(m_UserNode->isVisible()) {
            
            if(addFood==0 && FunBuildController::getInstance()->lastAddFood>0 && GlobalData::shared()->resourceInfo.lFood >= GlobalData::shared()->resourceInfo.lStorageFood) {
                
                temp = _lang("102286");
                
            }
            
            
            
            if (addFood==0 && FunBuildController::getInstance()->lastAddFood<=0) {//当粮食减少到0的时候 就不再显示减少了
                
            }
            
            else {
                
                CCCommonUtils::flyUiResText(temp.c_str(), this->m_flyTextNode,ccp(280, addy) ,color, floatType);
                
            }
            
        }
        
        //        if(addFood < 0)
        
        //        {
        
        //            CCActionInterval * tinto = CCTintBy::create(0.5, 0, 255, 255);
        
        //            this->m_food->runAction(tinto);
        
        //        }
        
        //        CCScaleTo* scaleTo = CCScaleTo::create(0.3, 1.1);
        
        //        CCScaleTo* scaleTo1 = CCScaleTo::create(0.3, 1);
        
        //        this->m_foodIcon->stopAllActions();
        
        //        this->m_foodIcon->runAction(CCSequence::createWithTwoActions(scaleTo,scaleTo1));
        
        
        
    }
    
    
    
    if(addIron==0 && FunBuildController::getInstance()->lastAddIron==0){
        
        temp = "";
        
    }else {
        
        if(addIron>0 || (addIron==0 && FunBuildController::getInstance()->lastAddIron>0)){
            
            temp = CC_ITOA(addIron);
            
            temp = "+"+temp;
            
            color = ccGREEN;
            
            floatType = floating_type_ui1;
            
            addy = -30;
            
        }else{
            
            temp = CC_ITOA(addIron);
            
            color = ccRED;
            
            floatType = floating_type_ui2;
            
            addy = 0;
            
        }
        
        if(m_UserNode->isVisible()) {
            
            if(addIron==0 && FunBuildController::getInstance()->lastAddIron>0 && GlobalData::shared()->resourceInfo.lIron >= GlobalData::shared()->resourceInfo.lStorageIron) {
                
                temp = _lang("102286");
                
            }
            
            CCCommonUtils::flyUiResText(temp.c_str(), this->m_flyTextNode,ccp(390, addy) ,color, floatType);
            
        }
        
    }
    
    
    
    if(addStone==0 && FunBuildController::getInstance()->lastAddStone==0){
        
        temp = "";
        
    }else {
        
        if(addStone>0 || (addStone==0 && FunBuildController::getInstance()->lastAddStone>0)){
            
            temp = CC_ITOA(addStone);
            
            temp = "+"+temp;
            
            color = ccGREEN;
            
            floatType = floating_type_ui1;
            
            addy = -30;
            
        }else{
            
            temp = CC_ITOA(addStone);
            
            color = ccRED;
            
            floatType = floating_type_ui2;
            
            addy = 0;
            
        }
        
        if(m_UserNode->isVisible()) {
            
            if(addStone==0 && FunBuildController::getInstance()->lastAddStone>0 && GlobalData::shared()->resourceInfo.lStone >= GlobalData::shared()->resourceInfo.lStorageStone) {
                
                temp = _lang("102286");
                
            }
            
            CCCommonUtils::flyUiResText(temp.c_str(), this->m_flyTextNode,ccp(500, addy) ,color, floatType);
            
        }
        
    }
    
    
    
    updateResStatus();
    
}



void UIComponent::updateResStatus()

{
    
    if(GlobalData::shared()->playerInfo.level < GlobalData::shared()->MaxPlayerLv){
        
        string lv= "";
        
        lv+=CC_ITOA(GlobalData::shared()->playerInfo.level);
        
        this->m_level->setString(lv.c_str());
        
    }else{
        
        this->m_level->setString(_lang("102183").c_str());
        
    }
    
    this->m_gold->setString(CC_CMDITOA(GlobalData::shared()->playerInfo.gold).c_str());
    this->m_goldNum->setGoldValue(GlobalData::shared()->playerInfo.gold);
    //更新龙晶
    this->m_longJingNum->setGoldValue(GlobalData::shared()->playerInfo.longJing);
//    this->m_goldNum->setString(CC_CMDITOA(GlobalData::shared()->playerInfo.gold).c_str());
    
    //    this->m_money->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lMoney));
    
    this->m_wood->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lWood));
    
    this->m_iron->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lIron));
    
    this->m_food->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lFood));
    
    this->m_stone->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lStone));
    
    this->m_dragonGold->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lDragonGold));
    
    this->m_dragonFood->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lDragonFood));
    
    setOldResource();
}



void UIComponent::resourceFloatingAni(){
    
    this->m_floatingNode->setVisible(true);
    
    CCActionInterval*  actionBy = CCMoveBy::create(0.5, ccp(0,20));
    
    this->m_floatingNode->stopAllActions();
    
    this->m_floatingNode->runAction(CCSequence::create(actionBy,CCCallFuncN::create(this, callfuncN_selector(UIComponent::resetFloatingAni)),NULL));
    
    
    
}

void UIComponent::resetFloatingAni(CCNode* p){
    
    this->m_floatingNode->setPosition(this->m_floatingNode->getPositionX(), this->m_floatingNode->getPositionY()-20);
    
    this->m_floatingNode->setVisible(false);
    
    
    
}



void UIComponent::updateNewChatNews(int num){
    
    //    m_chatNewAlert->setVisible(num>0);
    
}


#pragma mark UIComponent CCB
SEL_CCControlHandler UIComponent::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAchievementClick", UIComponent::onAchievementClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLoginRewardBtnClick", UIComponent::onLoginRewardBtnClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onShowCountryView", UIComponent::onShowCountryView);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGeneralGambleClick", UIComponent::onGeneralGambleClick);
    
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChatBarClick", UIComponent::onChatBarClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGCMRewardClick", UIComponent::onGCMRewardClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoldBuyBtnClick", UIComponent::onGoldBuyBtnClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoldBtnClick", UIComponent::onGoldBuyBtnClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWorldFindClick", UIComponent::onWorldFindClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPopupReturnClick", UIComponent::onPopupReturnClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", UIComponent::onCloseBtnClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onEquipBagBtnClick", UIComponent::onEquipBagBtnClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onEquipCrtBtnClick", UIComponent::onEquipCrtBtnClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCDExpansion", UIComponent::onCDExpansion);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMailClick", UIComponent::onMailClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onActivityClick", UIComponent::onActivityClick);
    // tao.yu 第一版不开放vip
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onVipBtnClick", UIComponent::onVipBtnClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFaceClick", UIComponent::onFaceClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStatusClick", UIComponent::onStatusClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteMailBtnClick", UIComponent::onWriteMailBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelDelMailClick", UIComponent::onCancelDelMailClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDelAllMailClick", UIComponent::onDelAllMailClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDelMailOPClick", UIComponent::onDelMailOPClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseNodeClick", UIComponent::onCloseNodeClick);
    //龙晶
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLongJingBtnClick", UIComponent::onLongJingBtnClick);
    return NULL;
    
}

bool UIComponent::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
//    CCLOG("ccbi control name %s",pMemberVariableName);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_general", CCSprite*, this->m_general);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_homeBack", CCSprite*, this->m_homeBack);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_PVE", CCSprite*, this->m_PVE);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_world", CCSprite*, this->m_world);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alliance", CCSprite*, this->m_alliance);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_configSet", CCSprite*, this->m_configSet);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_store", CCSprite*, this->m_store);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_homeBG", CCSprite*, this->m_homeBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, this->m_upNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_status", CCControlButton*, m_status);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_xCoordLabel", CCLabelIF*, this->m_xCoordLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yCoordLabel", CCLabelIF*, this->m_yCoordLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_xCoordText", CCLabelIF*, this->m_xCoordText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yCoordText", CCLabelIF*, this->m_yCoordText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_zCoordLabel", CCLabelIF*, this->m_zCoordLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_zCoordText", CCLabelIF*, this->m_zCoordText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingdomBG", CCSprite*, this->m_kingdomBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_xBG", CCSprite*, this->m_xBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yBG", CCSprite*, this->m_yBG);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tileCoordinateNode", CCNode*, this->m_tileCoordinateNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coordBG", CCSprite*, this->m_coordBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_faveNode", CCNode*, this->m_faveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_miniNode", CCNode*, this->m_miniNode);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rechargeNode", CCNode*, this->m_rechargeNode);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelName", CCLabelIF*, this->m_levelName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_repayNode", CCNode*, this->m_repayNode);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_generalGambleBtn", CCControlButton*, this->m_generalGambleBtn);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loginRewardBtn", CCControlButton*, this->m_loginRewardBtn);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_generalBtnTitle", CCLabelIF*, this->m_generalBtnTitle);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_generalGambleBtnTitle", CCLabelIF*, this->m_generalGambleBtnTitle);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_generalSoulBtnTimes", CCLabelIF*, this->m_generalSoulBtnTimes);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceBtnTitle", CCLabelIF*, this->m_allianceBtnTitle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_generalGambleBtnHint", CCLayerColor*, this->m_generalGambleBtnHint);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, m_closeBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_EquipBagBtn", CCControlButton*, m_EquipBagBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_EquipCrtBtn", CCControlButton*, m_EquipCrtBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_EquipBagNode", CCNode*, m_EquipBagNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_EquipCrtNode", CCNode*, m_EquipCrtNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_chatBG", CCScale9Sprite*, this->m_chatBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questBG", CCNode*, this->m_questBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questBG2", CCNode*, this->m_questBG2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questRecSpr", CCScale9Sprite*, this->m_questRecSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questRecNode", CCNode*, this->m_questRecNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_storeBG", CCSprite*, this->m_storeBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceBG", CCSprite*, this->m_allianceBG);
    
    
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loginRewardBtnTitle", CCLabelIF*, this->m_loginRewardBtnTitle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_homeBackBtnTitle", CCLabelIF*, this->m_homeBackBtnTitle);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_PVEBtnTitle", CCLabelIF*, this->m_PVEBtnTitle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_worldBtnTitle", CCLabelIF*, this->m_worldBtnTitle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_level", CCLabelIF*, this->m_level);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_money", CCLabelIF*, this->m_money);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wood", CCLabelIF*, this->m_wood);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iron", CCLabelIF*, this->m_iron);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_food", CCLabelIF*, this->m_food);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stone", CCLabelIF*, this->m_stone);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gold", CCLabelIF*, this->m_gold);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum", CCLabelIF*, this->m_goldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeGoldIcon", CCNode*, this->m_nodeGoldIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_messageText", CCLabelIFTTF*, this->m_messageText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_chatName", CCLabelIFBMFont*, this->m_chatName);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_chatFlyNode", CCNode*, this->m_chatFlyNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stamineBar", CCScale9Sprite*, this->m_stamineBar);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprVip", CCSprite*, this->m_sprVip);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprVipHui", CCSprite*, this->m_sprVipHui);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipText", CCLabelIFBMFont*, this->m_vipText);
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_uiTitle", CCNode*, this->m_uiTitle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inUiTitle", CCNode*, this->m_inUiTitle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipNode", CCNode*, this->m_vipNode);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftControlNode", CCNode*, this->m_leftControlNode);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BuildingToolNode", CCNode*, this->m_BuildingToolNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_UIQuestNode", CCNode*, this->m_UIQuestNode);
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_worldUINode", CCNode*, this->m_worldUINode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questAnimNode", CCNode*, this->m_questAnimNode);
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_otherPartNode", CCNode*, this->m_otherPartNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerAndBgNode", CCNode*, this->m_powerAndBgNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainControlNode", CCNode*, this->m_mainControlNode);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resouceNode", CCNode*, this->m_resouceNode);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_generalSoulBtnTimesBG", CCSprite*, this->m_generalSoulBtnTimesBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_LvBG", CCSprite*, this->m_LvBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pFlagL", CCSprite*, this->m_pFlagL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pFlagR", CCSprite*, this->m_pFlagR);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playerBG", CCSprite*, this->m_playerBG);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_peopleBar", CCSprite*, this->m_peopleBar);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expBar", CCScale9Sprite*, this->m_expBar);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popupTitleBg1", CCSprite*, this->m_popupTitleBg1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popupTitleBg2", CCSprite*, this->m_popupTitleBg2);
    
    
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_PVEBG", CCSprite*, this->m_PVEBG);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_storeBG", CCSprite*, this->m_storeBG);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceBG", CCSprite*, this->m_allianceBG);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_generalBG", CCSprite*, this->m_generalBG);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_configSetBG", CCSprite*, this->m_configSetBG);
    
    
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moneyIcon", CCSprite*, this->m_moneyIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodIcon", CCSprite*, this->m_woodIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironIcon", CCSprite*, this->m_ironIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodIcon", CCSprite*, this->m_foodIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneIcon", CCSprite*, this->m_stoneIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_UserResBg", CCNode*, this->m_UserResBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightResBg", CCScale9Sprite*, this->m_rightResBg);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodBG", CCSprite*, this->m_woodBG);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironBG", CCSprite*, this->m_ironBG);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodBG", CCSprite*, this->m_foodBG);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneBG", CCSprite*, this->m_stoneBG);
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodBarNode", CCNode*, this->m_woodBarNode);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moneyBarNode", CCNode*, this->m_moneyBarNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironBarNode", CCNode*, this->m_ironBarNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodBarNode", CCNode*, this->m_foodBarNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneBarNode", CCNode*, this->m_stoneBarNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_UserNode", CCNode*, this->m_UserNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodNode", CCNode*, this->m_woodNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moneyNode", CCNode*, this->m_moneyNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironNode", CCNode*, this->m_ironNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodNode", CCNode*, this->m_foodNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneNode", CCNode*, this->m_stoneNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNode", CCNode*, this->m_goldNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildNode", CCNode*, this->m_buildNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questIcon", CCNode*, m_questIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questContextBG", CCScale9Sprite*, m_questContextBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questContextBG1", CCScale9Sprite*, m_questContextBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questTipNpcNode", CCNode*, m_questTipNpcNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNewNode", CCNode*, this->m_goldNewNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questTipNode", CCNode*, this->m_questTipNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newFavoriteIcon", CCSprite*, m_newFavoriteIcon);

    //邮件绑定
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBtnTimes", CCLabelIF*, this->m_mailBtnTimes);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBtnTitle", CCLabelIF*, this->m_mailBtnTitle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBtnTimesBG", CCSprite*, this->m_mailBtnTimesBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBG", CCSprite*, m_mailBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBtnsNode", CCNode*, m_mailBtnsNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_googlePlayNode", CCNode*, m_googlePlayNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelDelMailBtn", CCControlButton*, m_cancelDelMailBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_delAllMailBtn", CCControlButton*, m_delAllMailBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_delMailOpBtn", CCControlButton*, m_delMailOpBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_chatNewAlert", CCSprite*, this->m_chatNewAlert);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailText", CCLabelIF*, this->m_mailText);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailNode", CCNode*, this->m_mailNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightNode", CCNode*, this->m_rightNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode", CCNode*, this->m_fireNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingsGiftNode", CCNode*, this->m_kingsGiftNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_giftNode1", CCNode*, this->m_giftNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_giftNode2", CCNode*, this->m_giftNode2);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldBuyBtn", CCControlButton*, this->m_goldBuyBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_achievementBtn", CCControlButton*, this->m_achievementBtn);
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_configSetBtnTitle", CCLabelIF*, this->m_configSetBtnTitle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_storeBtnLabel", CCLabelIF*, this->m_storeBtnLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_achNumBg", CCNode*, this->m_achNumBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_achNum", CCLabelIF*, this->m_achNum);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_achNode", CCNode*, this->m_achNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questNameNode", CCNode*, this->m_questNameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questGoldNode", CCNode*, this->m_questGoldNode);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_configNode", CCNode*, this->m_configNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_frameEffNode", CCNode*, this->m_frameEffNode);
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popupTitleName", CCLabelIF*, this->m_popupTitleName);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popupReturnBtn", CCControlButton*, this->m_popupReturnBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldName", CCLabelIF*, this->m_goldName);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armyCDContainer", CCNode*, this->m_armyCDContainer);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_power", CCLabelIFBMFont*, this->m_power);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questName", CCLabelIF*, this->m_questName);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questTitle", CCLabelIF*, this->m_questTitle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questTipNum", CCLabelIF*, this->m_questTipNum);
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_queueNode", CCNode*, this->m_queueNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_queueCell", CCNode*, this->m_queueCell);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expansionNode", CCNode*, this->m_expansionNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lbExpansion", CCLabelIF*, this->m_lbExpansion);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cdExpansion", CCControlButton*, this->m_cdExpansion);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wrireMailBtn", CCControlButton*, this->m_wrireMailBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprExpansion", CCSprite*, this->m_sprExpansion);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprPack", CCSprite*, this->m_sprPack);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_questTipBG", CCSprite*, this->m_questTipBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cdBG", CCScale9Sprite*, this->m_cdBG);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_line1", CCSprite*, this->m_line1);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_line2", CCSprite*, this->m_line2);
    
    
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_queueBtnTimes", CCLabelIF*, m_queueBtnTimes);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_queueBtnTimesBG", CCSprite*, m_queueBtnTimesBG);
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBGFlag2", CCSprite*, this->m_mailBGFlag2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_countryIcon", CCSprite*, this->m_countryIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceIcon", CCSprite*, this->m_allianceIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_faveBG", CCSprite*, this->m_faveBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_miniBG", CCSprite*, this->m_miniBG);

    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpNum", CCLabelIF*, this->m_helpNum);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpNumBg", CCNode*, this->m_helpNumBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpNode", CCNode*, this->m_helpNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpNumNode", CCNode*, this->m_helpNumNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpNumTxt", CCLabelIF*, this->m_helpNumTxt);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pic", CCNode*, this->m_pic);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerNode", CCNode*, this->m_powerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerParticleNode", CCNode*, this->m_powerParticleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expParticleNode", CCNode*, this->m_expParticleNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_findIcon", CCControlButton*, this->m_findIcon);
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_collectNode", CCNode*, this->m_collectNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceAni", CCNode*, this->m_allianceAni);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_limitActNode", CCNode*, this->m_limitActNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_limitActBg", CCNode*, this->m_limitActBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillNode", CCNode*, this->m_skillNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillBG", CCNode*, this->m_skillBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverNode", CCNode*, this->m_serverNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_homeText", CCLabelIF*, this->m_homeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverIdText", CCLabelIF*, this->m_serverIdText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goHomeBtn", CCSprite*, this->m_goHomeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_watchNode", CCNode*, this->m_watchNode);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newuserHelpNode", CCNode*, this->m_newuserHelpNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newuserHelpBG", CCSprite*, this->m_newuserHelpBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeNodeBtn", CCControlButton*, this->m_closeNodeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveBuildLabel", CCLabelIF*, this->m_moveBuildLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_MoveBuildMsgNode", CCNode*, this->m_MoveBuildMsgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_testFeedBackNode", CCNode*, this->m_testFeedBackNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTestFeedBack", CCLabelIF*, this->m_lblTestFeedBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagTipNum", CCLabelIF*, this->m_bagTipNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagTipNode", CCNode*, this->m_bagTipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagTipBG", CCSprite*, this->m_bagTipBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lotteryNode", CCNode*, this->m_lotteryNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_csNode", CCNode*, this->m_csNode);

    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonFood", CCLabelIF*, this->m_dragonFood);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonGold", CCLabelIF*, this->m_dragonGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonFoodIcon", CCSprite*, this->m_dragonFoodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonGoldIcon", CCSprite*, this->m_dragonGoldIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonFoodBarNode", CCNode*, this->m_dragonFoodBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonGoldBarNode", CCNode*, this->m_dragonGoldBarNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerBgNode", CCNode*, this->m_powerBgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_miniIcon", CCSprite*, this->m_miniIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_faveIcon", CCSprite*, this->m_faveIcon);
    //装备提示特效
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipGlowNode", CCNode*, this->m_equipGlowNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topModelLayer", CCModelLayerColor*, this->m_topModelLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bomModelLayer", CCModelLayerColor*, this->m_bomModelLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midModelLayer", CCModelLayerColor*, this->m_midModelLayer);
    //龙晶
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_LongJingNode", CCNode*, this->m_LongJingNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode", CCNode*, this->m_sprNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceTipNode", CCNode*, this->m_allianceTipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shakeGuideNode", CCNode*, this->m_shakeGuideNode);
    return false;
    
}
//邮件点击入口
void UIComponent::onMailClick(CCObject *pSender, CCControlEvent pCCControlEvent)

{
    onCancelMoveBuild();
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);

#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(MailController::getInstance()->getIsNewMailListEnable())
        ChatServiceCocos2dx::showChannelListFrom2dx();
    else
    {
        PopupViewController::getInstance()->addPopupInView(MailPopUpView::create());
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("UI_mail"));
    }
#else
    if (ChatServiceCocos2dx::Mail_OC_Native_Enable){
       //原生邮件打开
        ChatServiceCocos2dx::creatingMailListWith_OC_Native();
    }else{
        PopupViewController::getInstance()->addPopupInView(MailPopUpView::create());
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("UI_mail"));
    }
 
    
#endif
   
    
}



void UIComponent::updateMailNum(CCObject* p){
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(GlobalData::shared()->android_native_chat == 1 && ChatServiceCocos2dx::isChatShowing){
        return;
    }
#endif
    //    this->m_mailNode->stopAllActions();
    
    //    this->m_mailNode->setPositionY(240);
    
    //    float px = m_mailNode->getPositionX();
    
    map<string, MailInfo*>::iterator it;
    
    int count = 0;
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(MailController::getInstance()->getIsNewMailListEnable())
        count =MailController::getInstance()->getNewMailUnreadNum();
    else
        count =GlobalData::shared()->mailCountObj.saveR +GlobalData::shared()->mailCountObj.perR +GlobalData::shared()->mailCountObj.sysR+GlobalData::shared()->mailCountObj.studioR+GlobalData::shared()->mailCountObj.fightR + GlobalData::shared()->mailCountObj.modR;
#else
    count =GlobalData::shared()->mailCountObj.saveR +GlobalData::shared()->mailCountObj.perR +GlobalData::shared()->mailCountObj.sysR+GlobalData::shared()->mailCountObj.studioR+GlobalData::shared()->mailCountObj.fightR + GlobalData::shared()->mailCountObj.modR;
#endif
    //    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
    
    
    //        if(it->second->status == 0){
    
    //            count++;
    
    //        }
    
    //    }
    
    //    if(GlobalData::shared()->serverVersion != GlobalData::shared()->version){
    
    //        count++;
    
    //    }
    
    m_mailBGFlag2->setVisible(false);
    
    if(count > 0){
        
        //this->m_mailBtnTimesBG->setVisible(true);
        
        this->m_mailBGFlag2->setVisible(true);
        
        this->m_mailBtnTimes->setVisible(true);
        
        this->m_mailBtnTimes->setString(CC_ITOA(count));
        
        if(GlobalData::shared()->mailCountObj.upR>0){
            
            m_mailBGFlag2->setVisible(true);
            
        }
        
        //        CCMoveTo* moveUp= CCMoveTo::create(0.1f, ccp(px,250));
        
        //        CCMoveTo* moveDown= CCMoveTo::create(0.1f, ccp(px,235));
        
        //        CCMoveTo* moveUp2= CCMoveTo::create(0.1f, ccp(px,245));
        
        //        this->m_mailNode->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(1),moveUp,moveDown,moveUp2,moveDown,NULL)));
        
    }else{
        
        this->m_mailBtnTimesBG->setVisible(false);
        
        this->m_mailBGFlag2->setVisible(false);
        
        this->m_mailBtnTimes->setVisible(false);
        
    }
    
}



bool UIComponent::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    m_curPt = pTouch->getLocation();
    
    hintType = 0;
    
    if( !this->isVisible()){
        
        return false;
        
    }
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD
       && WorldMapView::instance()
       && WorldMapView::instance()->m_directionIcon->isVisible()
       && isTouchInside(WorldMapView::instance()->m_directionIcon, pTouch)){
        return false;
    }
    if(this->m_uiTitle->isVisible() && this->m_goldNode->isVisible() && isTouchInside(this->m_goldBuyBtn, pTouch)){
        
        hintType = 1;
        
        return true;
        
    }
    
    else if (this->m_mainControlNode->isVisible() && isTouchInside(this->m_mailBG, pTouch))
        
    {
        
        hintType = 2;
        
        return true;
        
    }
    
    else if(m_UserNode->isVisible() && isTouchInside(this->m_vipNode, pTouch)){
        
        hintType = 4;
        
        return true;
        
    }
    
    else if(isTouchInside(this->m_generalGambleBtnHint, pTouch)&&this->m_mainControlNode->isVisible()){
        
        hintType = 5;
        
        return true;
        
    }else if(m_otherPartNode->isVisible() && m_UserNode->isVisible() && isTouchInside(this->m_playerBG, pTouch)){
        //(isTouchInside(this->m_LvBG, pTouch) ||
        hintType = 7;
        
        return true;
        
    }else if(m_mainControlNode->isVisible() &&  isTouchInside(this->m_chatBG, pTouch)){
        
        hintType = 6;
        
        return true;
        
    }else if(isTouchInside(this->m_questBG2, pTouch) && m_mainControlNode->isVisible()){
        
        hintType = 8;
        
        return true;
        
    }
    
    else if(isTouchInside(this->m_woodBarNode, pTouch) && m_UserNode->isVisible() && m_woodBarNode->isVisible()){
        
        hintType = 9;
        
        createLightSprite(this->m_woodBarNode);
        
        return true;
        
    }
    
    else if(isTouchInside(this->m_stoneBarNode, pTouch) && m_UserNode->isVisible() && m_stoneBarNode->isVisible()){
        
        hintType = 10;
        
        createLightSprite(this->m_stoneBarNode);
        
        return true;
        
    }
    
    else if(isTouchInside(this->m_foodBarNode, pTouch) && m_UserNode->isVisible() && m_foodBarNode->isVisible()){
        
        hintType = 11;
        
        createLightSprite(this->m_foodBarNode);
        
        return true;
        
    }
    
    else if(isTouchInside(this->m_ironBarNode, pTouch) && m_UserNode->isVisible() && m_ironBarNode->isVisible()){
        
        hintType = 12;
        
        createLightSprite(this->m_ironBarNode);
        
        return true;
        
    }
    
    //    else if(isTouchInside(this->m_moneyBarNode, pTouch) && m_UserNode->isVisible()){
    
    //        hintType = 13;
    
    //        createLightSprite(this->m_moneyBarNode);
    
    //        return true;
    
    //    }
    
    else if(m_mainControlNode->isVisible() && isTouchInside(this->m_homeBG, pTouch))
        
    {
        
        hintType = 14;
        
        return true;
        
    }
    
    //    else if(m_mainControlNode->isVisible() && isTouchInside(this->m_PVE, pTouch))
    
    //    {
    
    //        hintType = 15;
    
    //        return true;
    
    //    }
    
    else if(m_mainControlNode->isVisible() && isTouchInside(this->m_store, pTouch))
        
    {
        
        hintType = 16;
        
        return true;
        
    }
    
    else if(m_mainControlNode->isVisible() && isTouchInside(this->m_alliance, pTouch))
        
    {
        
        hintType = 17;
        
        return true;
        
    }
    
    else if(m_mainControlNode->isVisible() && isTouchInside(this->m_configSet, pTouch))
        
    {
        
        hintType = 18;
        
        return true;
        
    }
    
    else if(m_mainControlNode->isVisible() && isTouchInside(this->m_general, pTouch))
        
    {
        
        hintType = 19;
        
        return true;
        
    }
    
    else if(m_UIQuestNode->isVisible()&&isTouchInside(this->m_questBG, pTouch)){
        
        hintType = 20;
        
        return true;
        
    }
    
//    else if(m_faveNode->isVisible()&&isTouchInside(this->m_faveBG, pTouch)){
//        
//        hintType = 23;
//        
//        return true;
//        
//    }
    
    //    else if(m_buildNode->isVisible() && isTouchInside(this->m_buildNode, pTouch))
    
    //    {
    
    //        hintType = 21;
    
    //        return true;
    
    //    }
    
    else if(m_mainControlNode->isVisible() && isTouchInside(this->m_helpNode, pTouch) && this->m_helpNode->isVisible() && GlobalData::shared()->playerInfo.isInAlliance())
        
    {
        
        hintType = 22;
        
        return true;
        
    }
    
    else if(m_otherPartNode->isVisible() && m_rechargeNode->isVisible()
            && PopupViewController::getInstance()->getCurrViewCount()==0 && isTouchInside(this->m_rechargeNode, pTouch)){
        
        hintType = 24;
        
        return true;
        
    }
    else if(m_limitActNode->isVisible() && isTouchInside(this->m_limitActBg, pTouch)){
        
        hintType = 25;
        
        return true;
        
    }
    else if(m_mainControlNode->isVisible() && m_skillNode->isVisible() && isTouchInside(this->m_skillBG, pTouch)){
        
        hintType = 26;
        
        return true;
        
    }
    else if(m_mainControlNode->isVisible() && m_fireNode->isVisible() && isTouchInside(this->m_fireNode, pTouch)){
        hintType = 27;
        return true;
	}else if(m_newuserHelpNode->isVisible() && isTouchInside(m_newuserHelpBG,pTouch)){
        hintType = 28; //新手help信息
        return true;
    }else if(m_mainControlNode->isVisible() && m_watchNode->isVisible() && isTouchInside(this->m_watchNode, pTouch)){
        hintType = 29;
        return true;
    }
    else if(m_mainControlNode->isVisible() && m_kingsGiftNode->isVisible() && isTouchInside(this->m_kingsGiftNode, pTouch)){
        hintType = 30;
        return true;
    }
    else if(m_testFeedBackNode->isVisible() && isTouchInside(this->m_testFeedBackNode, pTouch)){
        hintType = 31;
        return true;
    }
    else if(m_lotteryNode->isVisible() && isTouchInside(this->m_lotteryNode, pTouch)){
        hintType = 32;
        return true;
    }
    else if(m_googlePlayNode->isVisible() && isTouchInside(this->m_googlePlayNode, pTouch)){
        hintType = 33;
        return true;
    }
    else if(m_otherPartNode->isVisible() && m_repayNode->isVisible()
             && PopupViewController::getInstance()->getCurrViewCount()==0 && isTouchInside(this->m_repayNode, pTouch)){
        hintType = 34;
        return true;
    }else if(m_csNode->isVisible() && isTouchInside(this->m_csNode, pTouch)){
        hintType = 35;
        return true;
    }
    return false;
    
}



void UIComponent::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    
    
}



void UIComponent::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    onCancelMoveBuild();
    if (hintType != 6) {
        
        if (fabs(pTouch->getLocation().x-m_curPt.x) > 30 || fabs(pTouch->getLocation().y-m_curPt.y) > 30)
            
        {
            
            return;
            
        }
        
    }
    
    switch (hintType) {
            
        case 1:
            
            onGoldBuyBtnClick(NULL,Control::EventType::TOUCH_DOWN);
            
            break;
            
        case 2:
            
            onMailClick(NULL,Control::EventType::TOUCH_DOWN);
            
            break;
            
        case 4:
            
            onVipBtnClick(NULL,Control::EventType::TOUCH_DOWN);
            
            break;
            
        case 5:
            
            if(isTouchInside(this->m_generalGambleBtnHint, pTouch))
                
                onGeneralGambleClick(NULL,Control::EventType::TOUCH_DOWN);
            
            break;
            
        case 6:
            
            if(isTouchInside(this->m_chatBG, pTouch)) {
                
                float tmpX = pTouch->getLocation().x;
                
                if (tmpX>m_curPt.x && (tmpX-m_curPt.x)>20) {
                    
                     m_allianceIcon->setVisible(false);
                    m_countryIcon->setVisible(true);
                    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
                    ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_COUNTRY;
                    
                    if(ChatServiceCocos2dx::enableNativeChat){
                        refreshChatInfo(CHAT_COUNTRY);
                    }
                    else
                    {
                        updateChatInfo(CHAT_COUNTRY);
                    }
#else
                    if (ChatServiceCocos2dx::enableNativeChat) {
                        ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_COUNTRY;
                        //选择显示聊天频道
                        ChatServiceCocos2dx::selectAsnORCountries();
                        //IOS重新写
                        
                        if(ChatServiceCocos2dx::enableNativeChat){
                            updateChatInfoIOS(CHAT_COUNTRY);
                        }
                        else
                        {
                            updateChatInfo(CHAT_COUNTRY);
                        }
                    }else{
                        if(ChatController::getInstance()->isFirstRequestCountry)
                            ChatController::getInstance()->sendRequestChat(CHAT_COUNTRY);
                    }

#endif
                }
                else if (tmpX<m_curPt.x && (m_curPt.x-tmpX)>20) {
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
                    ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_ALLIANCE;
#else
                    ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_ALLIANCE;
                    //选择显示聊天频道
                    ChatServiceCocos2dx::selectAsnORCountries();
#endif
                    if (!GlobalData::shared()->playerInfo.isInAlliance()) {
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
                        CCLOG("GlobalData::shared()->android_native_chat = %d", GlobalData::shared()->android_native_chat);
                        if(GlobalData::shared()->android_native_chat == 1){
                            if(!ChatServiceCocos2dx::isChatShowing){
                                CCLOG("android showChatActivityFrom2dx");
//                                ChatServiceCocos2dx::m_isInMailDialog=false;
                                ChatServiceCocos2dx::m_isNoticItemUsed=false;
                                ChatServiceCocos2dx::showChatActivityFrom2dx();
                            }
                        }else{
                            PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_ALLIANCE),false);
                        }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
                        if(ChatServiceCocos2dx::enableNativeChat){
                            if(!ChatServiceCocos2dx::isChatShowing_fun()){
                                CCLOG("IOS showChatIOSFrom2dx");
                                ChatServiceCocos2dx::m_isInMailDialog=false;
                                ChatServiceCocos2dx::showChatIOSFrom2dx();
                            }
                        }else{
                            PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_ALLIANCE),false);
                        }
#endif
//                        YesNoDialog::show(_lang("E100068").c_str(), [](){});
                        return;
                    }
                    m_countryIcon->setVisible(false);
                    m_allianceIcon->setVisible(true);
                    
                    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
                    if(ChatServiceCocos2dx::enableNativeChat){
                        refreshChatInfo(CHAT_ALLIANCE);
                    }
                    else
                    {
                        updateChatInfo(CHAT_ALLIANCE);
                    }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
                    
                    if(ChatServiceCocos2dx::enableNativeChat){
                        updateChatInfoIOS(CHAT_ALLIANCE);
                    }
                    else
                    {
                        updateChatInfo(CHAT_ALLIANCE);
                    }
                    
#endif
                    moveChatInfo(1);
                    
                }
                
                else {
                    
                    onChatBtnClick(NULL,Control::EventType::TOUCH_DOWN);
                    
                }
                
            }
            
            break;
            
        case 7:{
            
            onFaceClick(NULL,Control::EventType::TOUCH_DOWN);
            
            break;
            
        }
            
        case 8:{
            
            showQuest();
            
            break;
            
        }
            
        case 9:{
            
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            
            PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Wood));
            
            this->m_woodBarNode->removeChildByTag(99995);
            
            break;
            
        }
            
        case 10:{
            
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            
            PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Stone));
            
            this->m_stoneBarNode->removeChildByTag(99995);
            
            break;
            
        }
            
        case 11:{
            
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            
            PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Food));
            
            this->m_foodBarNode->removeChildByTag(99995);
            
            break;
            
        }
            
        case 12:{
            
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            
            PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Iron));
            
            this->m_ironBarNode->removeChildByTag(99995);
            
            break;
            
        }
            
        case 13:{
            
            //            PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Silver));
            
            //            this->m_moneyBarNode->removeAllChildren();
            
            break;
            
        }
            
        case 14:{
            
            if (m_homeBack->isVisible())
                
            {
                
                OnHomeBackBtnClick(NULL,Control::EventType::TOUCH_DOWN);
                
            }
            
            else
                
            {
                
                onWorldBtnClick(NULL,Control::EventType::TOUCH_DOWN);
                
            }
            
            break;
            
        }
            
        case 15:{
            
            onPVEBtnClick(NULL,Control::EventType::TOUCH_DOWN);
            
            break;
            
        }
            
        case 16:{
            
            onStoreBtnClick(NULL,Control::EventType::TOUCH_DOWN);
            
            break;
            
        }
            
        case 17:{
            CCCommonUtils::flyHint("", "", _lang("E100008"));
//            onAllianceBtnClick(NULL,Control::EventType::TOUCH_DOWN);
            break;
            
        }
            
        case 18:{
            
            onConfigSetBtnClick(NULL,Control::EventType::TOUCH_DOWN);
            
            break;
            
        }
            
        case 19:{
            
            //onShowGeneralView(NULL,Control::EventType::TOUCH_DOWN);
            
            break;
            
        }
            
        case 20:{
            
            onClickTarget();
            
            break;
            
        }
            
        case 21:{
            
            onGoToBuilding();
            
            break;
            
        }
            
        case 22:{
            
            this->showAllianceHelp();
            
            break;
            
        }
            
        case 23:{
            
            this->onFaveBtnClick(NULL,Control::EventType::TOUCH_DOWN);
            
            break;
            
        }
            
        case 24:{
            
            this->onRechargeBtnClick(NULL,Control::EventType::TOUCH_DOWN);
            
            break;
            
        }
        case 25:{
            //PopupViewController::getInstance()->addPopupView(ActivityTipView::create());
            break;
        }
        case 26:{
//            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);  fusheng
//            PopupViewController::getInstance()->addPopupView(UseSkillPopUpView::create());
            PopupViewController::getInstance()->addPopupInView(GeneralTitanPopupView::create(),true);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("UI_titan"));
            
            break;
        }
        case 27:{
            PopupViewController::getInstance()->addPopupInView(CityDefenseView::create(FUN_BUILD_MAIN_CITY_ID));
            break;
        }
        case 28:{
//            HelpshiftCocos2dx::showFAQSection("8");
            HelpshiftCocos2dx::showFAQs();
            CCDelayTime *delayTime = CCDelayTime::create(0.5);
            m_newuserHelpNode->runAction(CCSequence::create(delayTime,CCCallFunc::create(this, callfunc_selector(UIComponent::showHideNewUserHelpDialog)),NULL));
            break;
        }
        case 29:{
            int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_WATCH_TOWER);
            if (buildId>0) {
                PopupViewController::getInstance()->addPopupInView(WatchtowerView::create(buildId));
            }
            break;
        }
        case 30:{
            PopupViewController::getInstance()->addPopupInView(ManagerKingdomView::create());
            break;
        }
        case 31:{
            double leftT = TestFeedBackController::shared()->getEndTime() - GlobalData::shared()->getTimeStamp();
            if(leftT>0){
                PopupViewController::getInstance()->addPopupView(TestFeedBackView::create());
            }else{
                YesNoDialog::showYesDialog(_lang("111096").c_str());
            }
            break;
        }
        case 32:{
            if (LotteryController::shared()->getOpenFlag() != 1)
            {
                CCCommonUtils::flyHint("", "", _lang("111123"));
            }
            else if (LotteryController::shared()->lotteryInfo.type == 2)
            {
                PopupViewController::getInstance()->addPopupInView(LotteryAct2View::create());
            }
            else
            {
                PopupViewController::getInstance()->addPopupInView(LotteryActView::create());
            }
            break;
        }
        case 33:
            if(GlobalData::shared()->analyticID == "market_global"){
                AchievementController::getInstance()->doOpenGooglePlayAchievement();
            }
            break;
        case 34:{
            PopupViewController::getInstance()->addPopupInView(RepayView::create());
            break;
        }
        case 35:{
            PopupViewController::getInstance()->addPopupInView(CrossServiceTargetView::create());
            break;
        }
        default:
            if (CCCommonUtils::isIosAndroidPad())
            {
                auto glview = Director::getInstance()->getOpenGLView();
                glview->setDesignResolutionSize(1536, 2048, kResolutionFixedWidth);
            }
            break;
            
    }

}
void UIComponent::showHideNewUserHelpDialog(){
    m_newuserHelpNode->stopAllActions();
    YesNoDialog::show(_lang("101202").c_str(), CCCallFunc::create(this, callfunc_selector(UIComponent::sureToHideNewUserHelp)));
}
void UIComponent::sureToHideNewUserHelp(){
    std::string key = NEW_USER_HELP_ON + CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID);
    CCUserDefault::sharedUserDefault()->setBoolForKey(key.c_str(), true);
    CCUserDefault::sharedUserDefault()->flush();
    m_newuserHelpNode->setVisible(false);
}

void UIComponent::showAllianceHelp(){
    
    PopupViewController::getInstance()->addPopupInView(AllianceHelpView::create());
    
}



void UIComponent::updateHelpData(CCObject* param){
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(GlobalData::shared()->android_native_chat == 1 && ChatServiceCocos2dx::isChatShowing){
        return;
    }
#endif
    if(m_helpNode==NULL) return ;
    int num = GlobalData::shared()->playerInfo.allianceInfo.helpcount + GlobalData::shared()->playerInfo.allianceInfo.militaryNum;

    if(GlobalData::shared()->playerInfo.isInAlliance() && num>0){
        m_helpNode->setVisible(false);
        m_helpNum->setString(CC_ITOA(num));
        m_helpNumTxt->setString(CC_ITOA(num));
        m_helpNumNode->setVisible(true);
        
    }else{
        m_helpNode->setVisible(false);
        m_helpNumNode->setVisible(false);
        
    }
    
}

void UIComponent::updateActivityEvent(CCObject* param){
    
}

void UIComponent::createLightSprite(CCNode* node)

{
    
    ParticleController::initParticle();
    
    
    
    auto newBatch = ParticleController::createParticleBatch();
    
    auto particle1 = ParticleController::createParticle("UIGlow_1");
    
    particle1->setPosition(ccp(22, 22));
    
    newBatch->addChild(particle1);
    
    auto particle2 = ParticleController::createParticle("UIGlow_2");
    
    particle2->setPosition(ccp(22, 22));
    
    newBatch->addChild(particle2);
    
    auto particle3 = ParticleController::createParticle("UIGlow_3");
    
    particle3->setPosition(ccp(22, 22));
    
    newBatch->addChild(particle3);
    
    newBatch->setTag(99995);
    
    node->addChild(newBatch);
    
}



void UIComponent::showClickEff(CCPoint pt)

{
    
    int tag = m_clickNode->getChildrenCount();
    
    
    
    auto newBatch = ParticleController::createParticleBatch();
    
    auto particle1 = ParticleController::createParticle("UIGlow_1");
    
    particle1->setPosition(pt);
    
    newBatch->addChild(particle1);
    
    auto particle2 = ParticleController::createParticle("UIGlow_2");
    
    particle2->setPosition(pt);
    
    newBatch->addChild(particle2);
    
    auto particle3 = ParticleController::createParticle("UIGlow_3");
    
    particle3->setPosition(pt);
    
    newBatch->addChild(particle3);
    
    newBatch->setTag(tag);
    
    
    
    m_clickNode->addChild(newBatch);
    
    
    
    CCActionInterval * delayT = CCDelayTime::create(0.2);
    
    auto func = CCCallFuncO::create(this, callfuncO_selector(UIComponent::delClickEff), CCInteger::create(tag));
    
    newBatch->runAction(CCSequence::create(delayT,func,NULL));
    
}



void UIComponent::delClickEff(CCObject *obj)

{
    
    int tag = dynamic_cast<CCInteger*>(obj)->getValue();
    
    m_clickNode->removeChildByTag(tag);
    
}


int willGo = 0;
void UIComponent::onWorldBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        CCCommonUtils::setIsHDViewPort(true);
    }
     willGo = FunBuildController::getInstance()->findBdRoPos();
    if (willGo > 0) {
        YesNoDialog::showVariableTitle(_lang("3000020").c_str(), CCCallFunc::create(this, callfunc_selector(UIComponent::onOKGotoBuild)), _lang("108525").c_str() );
        return;
    }
    
    if (QuestController::getInstance()->currentAchList.find("2102501") != QuestController::getInstance()->currentAchList.end()) {
        QuestInfo* qusetTmp = QuestController::getInstance()->currentAchList["2102501"];
        if(qusetTmp->state == ACCEPT) {
            CCCommonUtils::flyHint("", "", _lang("3000021"), 3, 200);
            return;
        }
    }
    
    
//    SoundController::sharedSound()->playEffects(Music_ui_world);
    
    PopupViewController::getInstance()->removeAllPopupView();
    
    SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
    
        
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           
                                                                           , CCString::createWithFormat("UI_world_go"));
    
//    std::string isFirstPopKey = GlobalData::shared()->playerInfo.uid + "isFirstPop";
//    
//    CCUserDefault::sharedUserDefault()->setIntegerForKey(isFirstPopKey.c_str(), 0);
}

void UIComponent::onOKGotoBuild() {
    CCLOG("test");
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if(layer) {
        layer->onMoveToBuildAndPlay(willGo);
    }
}



void UIComponent::onPVEBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)

{
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    //    if (m_PVEBG->isVisible()) {
    
    //        return;
    
    //    }
    
    PopupViewController::getInstance()->removeAllPopupView();
}



void UIComponent::onGeneralGambleClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    
}



void UIComponent::onFaveBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    
    PopupViewController::getInstance()->addPopupInView(FavoriteList::create(),false);
    
}

void UIComponent::onRechargeBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->addPopupView(RechargeACTVCell::create(13));
    
}

void UIComponent::onChatBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)

{
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    int chatType = CHAT_COUNTRY;
    
    if(m_allianceIcon->isVisible())
     {
        chatType = CHAT_ALLIANCE;
    }
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    CCLOG("GlobalData::shared()->android_native_chat = %d", GlobalData::shared()->android_native_chat);
    if(GlobalData::shared()->android_native_chat == 1){
        if(!ChatServiceCocos2dx::isChatShowing){
            CCLOG("onChatBtnClick 1 showChatActivityFrom2dx");
            if (chatType==CHAT_COUNTRY) {
                 ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_COUNTRY;
            }else if(chatType==CHAT_ALLIANCE) {
                ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_ALLIANCE;
            }
//            ChatServiceCocos2dx::m_isInMailDialog=false;
            ChatServiceCocos2dx::m_isNoticItemUsed=false;
            ChatServiceCocos2dx::showChatActivityFrom2dx();
        }
    }else{
        PopupViewController::getInstance()->addPopupInView(ChatView::create(chatType),false);
    }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if(ChatServiceCocos2dx::enableNativeChat){
        if (!ChatServiceCocos2dx::isChatShowing_fun()){
            //聊天入口
            CCLOG("onChatBtnClick 1 showChatIOSFrom2dx");
            if (chatType==CHAT_COUNTRY) {
                ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_COUNTRY;
            }else if(chatType==CHAT_ALLIANCE) {
                ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_ALLIANCE;
            }
            ChatServiceCocos2dx::m_isInMailDialog=false;
            ChatServiceCocos2dx::showChatIOSFrom2dx();
        }
        
    }else{
        PopupViewController::getInstance()->addPopupInView(ChatView::create(chatType),false);
    }
    
#endif
    
}



void UIComponent::showQuest(){
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->addPopupInView(AchievementNewPopUpView::create(), true, false, false, true);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           
                                                                           , CCString::createWithFormat("UI_achievement"));
    
}



void UIComponent::onClickTarget()

{
    
    m_CanClickTarget = false;
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           
                                                                           , CCString::createWithFormat("UI_target"));
    
    if(m_recommandQuest==NULL)
        
        return;
    
    QuestController::getInstance()->goToQuestTarget(m_recommandQuest);
    hideQuestRect();
}



void UIComponent::onLoginRewardBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)

{
//    onCancelMoveBuild();
//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
//    
//    LoginRewardView::create();
//    
}



void UIComponent::OnHomeBackBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)

{
    if (CCCommonUtils::isIosAndroidPad())
    {
        CCCommonUtils::setIsHDViewPort(true);
    }
//    SoundController::sharedSound()->playEffects(Music_ui_world);
    
    PopupViewController::getInstance()->removeAllPopupView();
    
    auto world = WorldMapView::instance();
    
    if (world) {
        
        world->leaveWorld();
        
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           
                                                                           , CCString::createWithFormat("UI_world_back"));
    
}

void UIComponent::onShowCountryView(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)

{
    onCancelMoveBuild();
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
}



void UIComponent::onShowGeneralView(CCObject * pSender, Control::EventType pCCControlEvent)

{
    
    //#if COCOS2D_DEBUG>0
    
    //    //    if (m_allianceBG->isVisible())
    
    //    //    {
    
    //    //        return;
    
    //    //    }
    
    //    PopupViewController::getInstance()->removeAllPopupView();
    
    //    bool alreadyInAlliance = GlobalData::shared()->playerInfo.isInAlliance();
    
    //    if (alreadyInAlliance)
    
    //    {
    
    //    }
    
    //    else
    
    //    {
    
    
    //    }
    
    //    //    m_allianceBG->setVisible(true);
    
    //#else
    
    YesNoDialog::show(_lang("E100008").c_str(), [](){});
    
    //#endif
    
    
    
    //    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    //    PopupViewController::getInstance()->removeAllPopupView();
    
    //    if(GlobalData::shared()->generals.size() == 0){
    
    //        YesNoDialog::showYesDialog(_lang("105406").c_str());
    
    //        return;
    
    //    }
    
    //
    
    //    PopupViewController::getInstance()->addPopupInView(GeneralsPopupView::create());
    
    //    resetWorldMap();
    
}

void UIComponent::loadMailResource()
{
    CCLoadSprite::doResourceByCommonIndex(6, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(6, false);
    });
}

void UIComponent::onActivityClick(CCObject * pSender, Control::EventType pCCControlEvent){
    onCancelMoveBuild();
    SettingPopUpView * popUp = SettingPopUpView::create();
    
    if (PopupViewController::getInstance()->addPopupInView(popUp) > 0)
        
    {
        
        //        m_configSetBG->setVisible(true);
        
    }
    
    return;
    
}



void UIComponent::onConfigSetBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)

{
    
    //    if (m_configSetBG->isVisible()) {
    
    //        return;
    
    //    }
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    PopupViewController::getInstance()->removeAllPopupView();
    
    //    SettingPopUpView * popUp = SettingPopUpView::create();
    
    SuggestionView* popUp = SuggestionView::create();
    
    if (PopupViewController::getInstance()->addPopupInView(popUp) > 0)
        
    {
        
        //        m_configSetBG->setVisible(true);
        
    }
    
}



void UIComponent::onWorldFindClick(CCObject * pSender, Control::EventType pCCControlEvent){
    onCancelMoveBuild();
    auto world = WorldMapView::instance();
    
    if (world) {
        
        PopupViewController::getInstance()->addPopupView(MoveToView::create());
        
    }
    
}



void UIComponent::onGoldBuyBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)

{
    if(GlobalData::shared()->analyticID == "common"){
        return;
    }
    onCancelMoveBuild();
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    //    // register lua engine
    
    //    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    
    //    //把资源路径传给lua，在lua中拼接资源所在路径
    
    //    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    
    //    string luaFolder = CCFileUtils::sharedFileUtils()->getWritablePath() + "luaScript/";
    
    //    string luaPath = luaFolder + "ActivityPanel.lua";
    
    //    //读取并编译lua脚本
    
    //    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    //        CCLuaLog(luaPath.c_str());
    
    //        CCString* pstrFileContent = CCString::createWithContentsOfFile(luaPath.c_str());
    
    //        if (pstrFileContent)
    
    //        {
    
    //            pEngine->executeString(pstrFileContent->getCString());
    
    //        }
    
    //    #else
    
    //        std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(luaPath.c_str());
    
    ////        pEngine->addSearchPath(path.substr(0, path.find_last_of("/")).c_str());
    
    //        pEngine->executeScriptFile(path.c_str());
    
    //    #endif
    
    //    //调用lua的function
    
    //    string luaFunction = "main";
    
    //    lua_getglobal(ls, luaFunction.c_str());
    
    //    lua_pushstring(ls, luaFolder.c_str());
    
    //    lua_pcall(ls, 1, 0, 0);
    
    showGoldBuyPanel();
    
}



void UIComponent::showGoldBuyPanel(){
    
    //#if COCOS2D_DEBUG>0
    
    GoldExchangeView * popUp = GoldExchangeView::create();
    
    PopupViewController::getInstance()->addPopupView(popUp);
    
    //#else
    
    //    YesNoDialog::show(_lang("101223").c_str(), [](){});
    
    //    return;
    
    //#endif
    
    
    
}



void UIComponent::onStatusClick(CCObject * pSender, Control::EventType pCCControlEvent){
    onCancelMoveBuild();
    PopupViewController::getInstance()->addPopupInView(ItemStatusView::create());
    
}



void UIComponent::onGCMRewardClick(CCObject * pSender, Control::EventType pCCControlEvent)

{
//    onCancelMoveBuild();
//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
//    
//    PopupViewController::getInstance()->addPopupView(GCMRewardView::create());
    
}



void UIComponent::onAllianceBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    
    //    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    //    PopupViewController::getInstance()->addPopupInView(AllianceInfoMembersView::create());
    
    //    return;
    
    //currently we do not have alliance info from server but we need to test
    
    //alliance functions
    
    
    
    
    
    //    if (m_allianceBG->isVisible())
    
    //    {
    
    //        return;
    
    //    }
    
    PopupViewController::getInstance()->removeAllPopupView();
    
    bool alreadyInAlliance = GlobalData::shared()->playerInfo.isInAlliance();
    
    if (alreadyInAlliance)
        
    {
        
        PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));//
        
        //PopupViewController::getInstance()->addPopupInView(AllianceMarketView::create());//
        
    }
    
    else
        
    {
        
        PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
        
        //PopupViewController::getInstance()->addPopupInView(AllianceMarketView::create());//
        
    }
    
    //    PopupViewController::getInstance()->removeAllPopupView();
    
    
}



//void UIComponent::onChatBarClick(CCObject * pSender, Control::EventType pCCControlEvent)

//{

//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);

//    PopupViewController::getInstance()->addPopupView(ChatView::create());

//}



void UIComponent::onAchievementClick(CCObject * pSender, Control::EventType pCCControlEvent)

{
    onCancelMoveBuild();
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    //    PopupViewController::getInstance()->addPopupView(AchievementView::create());
    
    
    
    BattleManager::shared()->requestBattleInfo(0,0);
    
}



void UIComponent::onStoreBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)

{
    //    if (m_storeBG->isVisible())
    
    //    {
    
    //        return;
    
    //    }
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    PopupViewController::getInstance()->removeAllPopupView();
    
    
    //            lzy  高分辨率和低分辨率切换
//    CCCommonUtils::setIsHDViewPort(false);
    if (PopupViewController::getInstance()->addPopupInView(StoreView::create()) > 0 )//StoreView BackpackView
        
    {
        
        //        m_storeBG->setVisible(true);
        
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           
                                                                           , CCString::createWithFormat("UI_goods"));
    
    
    
}



void UIComponent::onVipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)

{
    // tao.yu 第一版不开放vip功能
    if (true) {
        return;
    }
    onCancelMoveBuild();
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    PopupViewController::getInstance()->addPopupInView(VipDetailView::create());
    
}



void UIComponent::onFaceClick(CCObject * pSender, Control::EventType pCCControlEvent)

{
    onCancelMoveBuild();
    //临时屏蔽这个面板
    if (GuideController::share()->isInTutorial() && GuideController::share()->getCurGuideID()!="3150200") {//引导过程中不能打开
        return;
    }
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    //    PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(&GlobalData::shared()->playerInfo));
    
    PopupViewController::getInstance()->addPopupInView(GeneralsPopupView::create());
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("UI_head"));
}



void UIComponent::onPopupReturnClick(CCObject * pSender, Control::EventType pCCControlEvent)

{
//    m_titleOldTitle = NULL;
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    GeneralsPopupView* generalView = dynamic_cast<GeneralsPopupView*>(PopupViewController::getInstance()->getCurrentPopupView());
    
    if(generalView){
        
        generalView->fadeOut();
        
        return;
        
    }
    
    MailPopUpView* mailView = dynamic_cast<MailPopUpView*>(PopupViewController::getInstance()->getCurrentPopupView());
    
    if(mailView){
        
        PopupViewController::getInstance()->removeAllPopupView();
        
        return;
        
    }
    ChatView* chatView = dynamic_cast<ChatView*>(PopupViewController::getInstance()->getCurrentPopupView());
    
    if(chatView){
        
        chatView->onClose();
        return;
        
    }
    EquipmentCreateView* equipView = dynamic_cast<EquipmentCreateView*>(PopupViewController::getInstance()->getCurrentPopupView());
    if(equipView){
        equipView->playClose();
        return;
    }
    
    MakingSoldierView* view = dynamic_cast<MakingSoldierView*>(PopupViewController::getInstance()->getCurrentPopupView());
    
    if(view){
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               
                                                                               , CCString::createWithFormat("SO_back"));

        PopupViewController::getInstance()->removeAllPopupView();
        
    }else{
        
        ScienceListView* sView = dynamic_cast<ScienceListView*>(PopupViewController::getInstance()->getCurrentPopupView());
        
        if (sView) {
            
            sView->playClose(0, true);
            
        }
        
        else {
            
            AchievementNewPopUpView* acView = dynamic_cast<AchievementNewPopUpView*>(PopupViewController::getInstance()->getCurrentPopupView());
            
            if (acView) {
                
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                       
                                                                                       , CCString::createWithFormat("AC_back"));
                
            }
            
            PopupViewController::getInstance()->goBackPopupView();
        }
        
    }
    
    
    
}

void UIComponent::showCountryIcon(bool isShow)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(GlobalData::shared()->android_native_chat == 1 && ChatServiceCocos2dx::isChatShowing){
        return;
    }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if(!ChatServiceCocos2dx::isChatShowing_fun()){
        return;
    }
#endif
    
    if (isShow) {
        m_countryIcon->setVisible(true);
        m_allianceIcon->setVisible(false);
    }
    else
    {
        m_countryIcon->setVisible(false);
        m_allianceIcon->setVisible(true);
    }
}

//android当原生聊天开的时候调用，IOS另外写
void UIComponent::refreshChatInfo(int type)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::enableNativeChat && !ChatServiceCocos2dx::isChatShowing){
        
        CCLOGFUNC("");
        string name="";
        
        string msg="";
        
        int _vip = 0;
        
        m_chatName->setString("");
        
        m_messageText->setPosition(m_messageTextPt);
        
        bool countryIconFlg = m_countryIcon->isVisible();
        
        bool allianceIconFlg = m_allianceIcon->isVisible();
        
        if (countryIconFlg && type==CHAT_COUNTRY && ChatController::getInstance()->m_latestCountryMsg.sequenceId>0) {
            
            CCLOG("m_countryIcon->isVisible() ");
            auto& info = ChatController::getInstance()->m_latestCountryMsg;
            name = info.name;
            
            if (info.asn != "") {
                
                name = "(" + info.asn + ") " + info.name;
                
            }
            
            msg = info.msg;
            
            _vip = info.vip;
            
            if (name=="" && msg=="") {
                CCLOGFUNC("name NULL msg NULL");
                return;
            }
            
        }
        else if (allianceIconFlg && type==CHAT_ALLIANCE && ChatController::getInstance()->m_latestAllianceMsg.sequenceId>0 && GlobalData::shared()->playerInfo.isInAlliance()) {
            
            CCLOG("m_allianceIcon->isVisible() ");
            auto& info = ChatController::getInstance()->m_latestAllianceMsg;
            name = info.name;
            msg = info.msg;
            
            _vip = info.vip;
            
        }
        else if(m_allianceIcon->isVisible() && type==CHAT_ALLIANCE && (ChatController::getInstance()->m_latestAllianceMsg.sequenceId == 0 || !GlobalData::shared()->playerInfo.isInAlliance())){
            
            this->m_messageText->setString(_lang("105310").c_str());
            
            return;
            
        }
        else if(m_countryIcon->isVisible() && type==CHAT_COUNTRY && ChatController::getInstance()->m_latestCountryMsg.sequenceId== 0){
            
            this->m_messageText->setString(_lang("105310").c_str());
            
            return;
            
        }
        else{
            return;
        }
        
        if (_vip > 0) {
            
            m_chatName->setString(_lang_1("103001", CC_ITOA(_vip)));
            
            float infoX = m_chatName->getPositionX()+m_chatName->getContentSize().width*m_chatName->getOriginScaleX()+ 10;
            
            m_messageText->setPositionX(infoX);
            
        }
        
        
        
        int lastStrl = 50 ;//- namel;
        
        string tmp = name + ": " + msg;
        
        if (lastStrl < tmp.length() ) {
            tmp=CCCommonUtils::subStrByUtf8(tmp, 0 , lastStrl);
            tmp = tmp+"...";
        }
        
        CCLOG("m_messageText %s",tmp.c_str());
        
        m_messageText->setString(tmp.c_str());
    }
    
#endif
}

void UIComponent::refreshChatInfoIOS(ChatInfo info)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if(!ChatServiceCocos2dx::enableNativeChat){
        return;
    }

    CCLOGFUNC("");
    string name="";
    
    string msg="";
    
    int _vip = 0;
    
    m_chatName->setString("");
    
    m_messageText->setPosition(m_messageTextPt);
    
    bool countryIconFlg = m_countryIcon->isVisible();
    bool allianceIconFlg = m_allianceIcon->isVisible();
    
    if (countryIconFlg && info.type == 0) {
        
        CCLOG("m_countryIcon->isVisible() ");

        name = info.name;
        
        if (info.asn != "") {
            
            name = "(" + info.asn + ") " + info.name;
            
        }
        
        msg = info.msg;
        
        _vip = info.vip;
        
        if (name=="" && msg=="") {
            CCLOGFUNC("name NULL msg NULL");
            return;
        }
        
    }
    
    else if (allianceIconFlg && info.type == 1 && GlobalData::shared()->playerInfo.isInAlliance()) {
        
        CCLOG("m_allianceIcon->isVisible() ");
        
        name = info.name;
        
        if (info.asn != "") {
            
            name = "(" + info.asn + ") " + info.name;
            
        }
        
        msg = info.msg;
        
        _vip = info.vip;
        
    }
    
    else if(allianceIconFlg && info.type == 1 && !GlobalData::shared()->playerInfo.isInAlliance()){
        
        this->m_messageText->setString(_lang("105310").c_str());
        
        return;
        
    }
    
    else if(countryIconFlg && info.type == 0 ){
        
        this->m_messageText->setString(_lang("105310").c_str());
        
        return;
        
    }
    
    else{
        return;
    }
    
    //    int namel = name.length();
    
    if (_vip > 0) {
        
        m_chatName->setString(_lang_1("103001", CC_ITOA(_vip)));
        
        float infoX = m_chatName->getPositionX()+m_chatName->getContentSize().width*m_chatName->getOriginScaleX()+ 10;
        
        m_messageText->setPositionX(infoX);
        
    }
    
    
    
    int lastStrl = 50 ;//- namel;
    
    string tmp = name + ": " + msg;
    
    if (lastStrl < tmp.length() ) {
        tmp=CCCommonUtils::subStrByUtf8(tmp, 0 , lastStrl);
        
        tmp = tmp+"...";
        
    }
    
    CCLOG("m_messageText %s",tmp.c_str());
    
    m_messageText->setString(tmp.c_str());
#endif
}

void UIComponent::updateChatInfoIOS(int type)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    
    if (type == CHAT_COUNTRY) {
        ChatServiceCocos2dx::refreshChatInfo(CHANNEL_TYPE_COUNTRY);
    }else{
        ChatServiceCocos2dx::refreshChatInfo(CHANNEL_TYPE_ALLIANCE);
    }
    
#endif
}

void UIComponent::updateChatInfo(int type)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(GlobalData::shared()->android_native_chat == 1){
        return;
    }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if(ChatServiceCocos2dx::enableNativeChat){
        return;
    }
#endif
    
    CCLOG("UIComponent::updateChatInfo");
    string name="";
    string msg="";
    int _vip = 0;
    m_chatName->setString("");
    m_messageText->setPosition(m_messageTextPt);
    if (m_countryIcon->isVisible() && type==CHAT_COUNTRY && ChatController::getInstance()->m_chatCountryPool.size()>0) {
        
        CCLOG("m_countryIcon->isVisible() ");
        auto& info = ChatController::getInstance()->m_chatCountryPool.back();
        name = info.name;
        if (info.asn != "") {
            name = "(" + info.asn + ") " + info.name;
        }
        msg = info.msg;
        _vip = info.vip;
        if (name=="" && msg=="") {
            CCLOGFUNC("name NULL msg NULL");
            return;
        }
        
    }
    else if (m_allianceIcon->isVisible() && type==CHAT_ALLIANCE && ChatController::getInstance()->m_chatAlliancePool.size()>0 && GlobalData::shared()->playerInfo.isInAlliance()) {
        
        CCLOG("m_allianceIcon->isVisible() ");
        auto& info = ChatController::getInstance()->m_chatAlliancePool.back();
        name = info.name;
        
        if (info.asn != "") {
            
            name = "(" + info.asn + ") " + info.name;
            
        }
        
        msg = info.msg;
        
        _vip = info.vip;
        
    }
    else if(m_allianceIcon->isVisible() && type==CHAT_ALLIANCE && (ChatController::getInstance()->m_chatAlliancePool.size() == 0 || !GlobalData::shared()->playerInfo.isInAlliance())){
        
        this->m_messageText->setString(_lang("105310").c_str());
        
        return;
        
    }
    else if(m_countryIcon->isVisible() && type==CHAT_COUNTRY && ChatController::getInstance()->m_chatCountryPool.size() == 0){
        
        this->m_messageText->setString(_lang("105310").c_str());
        
        return;
        
    }
    
    else{
        return;
    }
    
    //    int namel = name.length();
    
    if (_vip > 0) {
        
        m_chatName->setString(_lang_1("103001", CC_ITOA(_vip)));
        
        float infoX = m_chatName->getPositionX()+m_chatName->getContentSize().width*m_chatName->getOriginScaleX()+ 10;
        
        m_messageText->setPositionX(infoX);
        
    }
    
    
    
    int lastStrl = 50 ;//- namel;
    
    string tmp = name + ": " + msg;
    
    if (lastStrl < tmp.length() ) {
         tmp=CCCommonUtils::subStrByUtf8(tmp, 0 , lastStrl);
         tmp = tmp+"...";
    }
    
    CCLOG("m_messageText %s",tmp.c_str());
    
    CCCommonUtils::StringReplace(tmp, "\n", "");
    
    m_messageText->setString(tmp.c_str());
    
}



void UIComponent::moveChatInfo(int type)

{
    
    int dis = 200*type;
    
    CCActionInterval* fadeIn = CCFadeIn::create(0);
    
    CCActionInterval* fadeOut = CCFadeOut::create(0.2);
    
    CCSequence* fadeAction = CCSequence::create(fadeIn,fadeOut,NULL);
    
    
    
    CCActionInterval* moveBy1 = CCMoveBy::create(0, ccp(-dis, 0));
    
    CCActionInterval* moveBy2 = CCMoveBy::create(0.2, ccp(dis, 0));
    
    CCSequence* moveAction = CCSequence::create(moveBy1,moveBy2,NULL);
    
    
    
    CCActionInterval*  action = CCSpawn::create(moveAction,fadeAction,NULL);
    
    //    m_chatFlyNode->runAction(action);
    
}



void UIComponent::onCreateCropSceneUI()

{
    
    m_queueCell->removeAllChildren();
    
    int queueCount = 0;
    
    int tmpY = 0;
    
    map<int, QueueInfo>::iterator it;
    
    map<int, QueueInfo> tempQueuesInfo;

    if (m_sprExpansion->isVisible()) {
        
        int i = 0;
        
        for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
            
            auto curTime = WorldController::getInstance()->getTime();
            
            if((it->second).finishTime >= curTime + 1000 && ((it->second).type == TYPE_MARCH
                                                      
                                                      || (it->second).type == TYPE_OCCUPY_RESOURCE
                                                      
                                                      || (it->second).type == TYPE_OCCUPY_ALLIANCE
                                                      
                                                      || (it->second).type == TYPE_OCCUPY_MAZE
                                                      || (it->second).type == TYPE_ARMY_MASS
                                                      ||(it->second).type == TYPE_OCCUPY_CAMP
                                                      || (it->second).type == TYPE_BUILDING_TERRITORY
                                                      || (it->second).type == TYPE_REPAIR_TERRITORY
                                                      || (it->second).type == TYPE_STATION_TERRITORY
                                                      || (it->second).type == TYPE_DESTROY_TERRITORY
                                                      || (it->second).type == TYPE_OCCUPY_TERRITORY_RESOURCE))
                
            {
                if (WorldController::getInstance()->m_marchInfo.find(it->second.uuid) != WorldController::getInstance()->m_marchInfo.end()) {
                    if (WorldController::getInstance()->m_marchInfo[it->second.uuid].marchType == MethodHeiqishi) {
                        continue;
                    }
                }
                tempQueuesInfo[it->first] = (it->second);
            }
//            if (it->second.type == TYPE_MARCH) {
//                CCLog("logInfo===========================QueueFinishTime %s", CC_ITOA(it->second.finishTime));
//                CCLog("logInfo===========================QueueCurentTime %s", CC_ITOA(curTime));
//            }
        }
        
    }
    
    else
        
    {
        
        for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
            
            if((it->second).finishTime >= WorldController::getInstance()->getTime() + 1000 && ((it->second).type == TYPE_MARCH
                                                                     
                                                                     || (it->second).type == TYPE_OCCUPY_RESOURCE
                                                                     
                                                                     || (it->second).type == TYPE_OCCUPY_ALLIANCE
                                                                     
                                                                     || (it->second).type == TYPE_OCCUPY_MAZE
                                                                     || (it->second).type == TYPE_ARMY_MASS
                                                                     ||(it->second).type == TYPE_OCCUPY_CAMP
                                                                     || (it->second).type == TYPE_BUILDING_TERRITORY
                                                                     || (it->second).type == TYPE_REPAIR_TERRITORY
                                                                     || (it->second).type == TYPE_STATION_TERRITORY
                                                                     || (it->second).type == TYPE_DESTROY_TERRITORY
                                                                     || (it->second).type == TYPE_OCCUPY_TERRITORY_RESOURCE))
                
            {
                if (WorldController::getInstance()->m_marchInfo.find(it->second.uuid) != WorldController::getInstance()->m_marchInfo.end()) {
                    if (WorldController::getInstance()->m_marchInfo[it->second.uuid].marchType == MethodHeiqishi) {
                        continue;
                    }
                }
                tempQueuesInfo[it->first] = (it->second);
            }
            
        }
        
    }
    
    
    
    for (it = tempQueuesInfo.begin(); it != tempQueuesInfo.end(); it++) {
        
        
        
        queueCount++;
        
        if (m_sprExpansion->isVisible() && queueCount > 2) {
            
            continue;
            
        }
        
        else
            
        {
            
            auto queueCell = BuildQueueView::create((it->second).key, (it->second).qid, (it->second).type);
//            auto queueCell = WorldController::getInstance()->getWorldQueue((it->second).qid);
            queueCell->setTag((it->second).qid);
            
            queueCell->setPosition(ccp(0,tmpY));
            
            tmpY -= 59;
            
            this->m_queueCell->addChild(queueCell);
        
        }
        
        
        
    }
    
    if (queueCount == 0) {
        
        m_cdBG->setVisible(false);
        
        m_expansionNode->setVisible(false);
        
        return;
        
    }
    
    m_cdBG->setVisible(true);
    
    if (queueCount <= 2) {
        
        m_expansionNode->setVisible(false);
        
        m_cdBG->setContentSize(CCSizeMake(m_cdBG->getContentSize().width, queueCount * (118/2)));
    }
    
    else
        
    {
        
        int _btnPos = queueCount - 2;
        
        m_expansionNode->setVisible(true);
        
        if (m_sprExpansion->isVisible())
            
        {
            
            string btnTitle = CC_ITOA(queueCount-2);
            
            btnTitle += " ";
            
            btnTitle += _lang("102162");
            
            m_lbExpansion->setString(btnTitle.c_str());
            
            _btnPos = 0;
            
            queueCount = 2;
            
        }
        
        else
            
        {
            
            m_lbExpansion->setString(_lang("103667").c_str());
            
        }
        
//        m_expansionNode->setPosition(0, m_cdBG->getPositionY()-50*_btnPos - 90);
        m_expansionNode->setPosition(0, m_cdBG->getPositionY()-59*_btnPos);
        m_cdBG->setContentSize(CCSizeMake(m_cdBG->getContentSize().width, queueCount * (118/2)));
        
    }
    setqueueCount(queueCount);
    CCLOG("shakelog: count %d",queueCount);
    
}



//void UIComponent::onDeleteCropSceneUI()

//{

//}



void UIComponent::onAddQueue(CCObject* params)

{
    
    onCreateCropSceneUI();
    
}



void UIComponent::onCDExpansion(CCObject * pSender, Control::EventType pCCControlEvent)

{
    onCancelMoveBuild();
    if (m_sprExpansion->isVisible()) {
        
        m_sprExpansion->setVisible(false);
        
        m_sprPack->setVisible(true);
        
    }
    
    else
        
    {
        
        m_sprPack->setVisible(false);
        
        m_sprExpansion->setVisible(true);
        
    }
    
    onCreateCropSceneUI();
    
}



CCNode* UIComponent::getNodeByIndex(string _key){
    
    //    if(index == 0){
    
    //        auto arr = m_queueCell->getChildren();
    
    //        if(arr && arr->count() > 0){
    
    //            BuildQueueView *cell = dynamic_cast<BuildQueueView*>(arr->objectAtIndex(0));
    
    //            return cell->getGuideNode();
    
    //        }
    
    //    }else
    
    if(_key == "UI_world_go"){
        
        return m_homeBG;
        
    }else if(_key == "UI_world_back"){
        
        return m_homeBG;
        
    }else if(_key == "UI_achievement"){
        
        return m_questBG2;
        
    }else if(_key == "UI_goods"){
        
        return m_store;
        
    }else if(_key == "UI_mail"){
        
        return m_allianceBG;
        
    }else if(_key == "UI_target"){
        
        m_CanClickTarget = true;
        
        return m_questBG;
        
    }else if(_key == "UI_target2"){
        
        return m_questBG2;
        
    }else if(_key == "SC2_out"){
        
        return m_closeBtn;
        
    }else if(_key == "UI_head"){
        
        return m_playerBG;
        
    }else if(_key == "UI_titan"){
        
        return m_skillBG;
        
    }
    
    return NULL;
    
}



void UIComponent::onShowFrameEff(CCObject* params)

{
    
    m_frameEffNode->stopAllActions();
    
    m_frameEffNode->removeAllChildren();

    if (EnemyInfoController::getInstance()->m_curType==ENEMY_TYPE_BATTLE || EnemyInfoController::getInstance()->m_curType==ENEMY_TYPE_TEAM || EnemyInfoController::getInstance()->m_curType==ENEMY_TYPE_MONSTER_ATTACK) {
//        auto spr_frame = CCLoadSprite::createScale9Sprite("red_frame.png");
        auto spr_frame = CCScale9Sprite::create("Spine/Single/red_frame_test.png");

        spr_frame->setAnchorPoint(ccp(0.5, 0.5));
        
        spr_frame->setScale(20);
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        spr_frame->setContentSize(winSize/spr_frame->getScale());
        
        m_frameEffNode->addChild(spr_frame);
        
        spr_frame->setOpacity(0);
        CCActionInterval * fadein = CCFadeTo::create(1,200);
        
        CCActionInterval * fadeout = CCFadeTo::create(1,0);
        
        CCSequence* fadeAction = CCSequence::create(fadein,fadeout,NULL);
        
        CCActionInterval * fadeForever =CCRepeatForever::create((CCActionInterval* )fadeAction);
        
        spr_frame->runAction(fadeForever);
//        auto winSize = CCDirector::sharedDirector()->getWinSize();
//        auto enemySpr = CCLoadSprite::createScale9Sprite("enemy_attack_eff.png");
//        enemySpr->setOpacity(128);
//        enemySpr->setAnchorPoint(ccp(0.5, 0));
//        enemySpr->setContentSize(winSize);
//        enemySpr->setPositionY(-winSize.height/2);
//        m_frameEffNode->addChild(enemySpr,-100);
//        
//        CCActionInterval * fIN = CCFadeTo::create(0.5, 100);
//        CCActionInterval * fOut = CCFadeTo::create(0.5, 0);
//        CCSequence* fAction = CCSequence::create(fIN,fOut,NULL);
//        CCActionInterval * fForever =CCRepeatForever::create((CCActionInterval* )fAction);
//        //enemySpr->runAction(fForever);
//        
//        string tmpFile = "CombustionUI_";
//        for (int i=1; i<=2; i++) {
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpFile.c_str(),i)->getCString());
//            particle->setPosition(ccp(160,-426));
//            m_frameEffNode->addChild(particle);
//        }
    }
    
}



void UIComponent::onHideFrameEff(CCObject* params)

{
    
    m_frameEffNode->stopAllActions();
    
    m_frameEffNode->removeAllChildren();
    
}



void UIComponent::onGoToBuilding()

{
    
    int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
    
    if (qid != QID_MAX) {
        
        string key = GlobalData::shared()->allQueuesInfo[qid].key;
        
        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        
        if (layer) {
            
            layer->onMoveToBuildAndOpen(atoi(key.c_str()), TYPE_POS_CLICK);
            
        }
        
    }
    
}



void UIComponent::updateBuildState(bool st, int qid)

{
    
    m_buildNode->removeAllChildren();
    
    m_buildQueueCell1 = NULL;
    
    m_buildQueueCell2 = NULL;
    
    if (st) {
        
        m_buildQueueCell1 = BuildQueueCell::create(BUILD_QUEUE_1ST);
        
        m_buildQueueCell1->setPosition(ccp(50, 50));
        
        m_buildNode->addChild(m_buildQueueCell1);
        
        
        if (FunBuildController::getInstance()->getMainCityLv() >= 3) {
            m_buildQueueCell2 = BuildQueueCell::create(BUILD_QUEUE_2ND);
            
            m_buildQueueCell2->setPosition(ccp(50, -70));
            
            m_buildNode->addChild(m_buildQueueCell2);
        }
    }
    
}

void UIComponent::onOpenShowBuildQueue2()
{
    if (m_buildQueueCell1!=NULL && m_buildQueueCell2==NULL && FunBuildController::getInstance()->getMainCityLv() == 3) {
        m_buildQueueCell2 = BuildQueueCell::create(BUILD_QUEUE_2ND);
        
        m_buildQueueCell2->setPosition(ccp(50, -70));
        
        m_buildNode->addChild(m_buildQueueCell2);
    }
}



void UIComponent::showCloseBtn(bool show)

{
    
    m_closeBtn->setVisible(show);
    
}



void UIComponent::onCloseBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)

{
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           
                                                                           , CCString::createWithFormat("SC2_out"));
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    PopupViewController::getInstance()->removeAllPopupView();
    
}

void UIComponent::showEquipOrBagBtn(int type)
{
    m_EquipBagNode->setVisible(false);
    m_EquipCrtNode->setVisible(false);
    if (type == 1) {
        m_EquipBagNode->setVisible(true);
    }else if (type == 2) {
        m_EquipCrtNode->setVisible(true);
    }
}

void UIComponent::onEquipBagBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(EquipmentBagView::create());
}

void UIComponent::onEquipCrtBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(EquipSiteView::create());
}


void UIComponent::setFoodColor(){
    
    return;
    
    int oneHourOutput = FunBuildController::getInstance()->FoodOutPreSed*3600;
    
    int oneHourUse = ArmyController::getInstance()->getTotalUpKeep();
    
    if(oneHourOutput>=oneHourUse){
        
        m_food->setColor({255,241,202});
        
    }else{
        
        m_food->setColor(ccRED);
        
    }
    
}



void UIComponent::showResBar()

{
    
    int mlv = FunBuildController::getInstance()->getMainCityLv();
    
    if (mlv>=FunBuildController::getInstance()->building_base_k4) {
        
        m_stoneBarNode->setVisible(true);
        
    }
    
    else {
        
        m_stoneBarNode->setVisible(false);
        
    }
    
    
    
    if (mlv>=FunBuildController::getInstance()->building_base_k3) {
        
        m_ironBarNode->setVisible(true);
        
    }
    
    else {
        
        m_ironBarNode->setVisible(false);
        
    }

}

void UIComponent::onChangePic(CCObject* params){
    
    m_pic->removeAllChildrenWithCleanup(true);
    
    CCSprite* head = CCLoadSprite::createSprite(GlobalData::shared()->playerInfo.getPic().c_str());
    
    m_pic->addChild(head);
    
    CCCommonUtils::setSpriteMaxSize(head, 77,true);
    
    if (GlobalData::shared()->playerInfo.isUseCustomPic())
    {
        m_headImgNode->initHeadImgUrl2(m_pic, GlobalData::shared()->playerInfo.getCustomPicUrl(), 1.0f, 77.0f, true);
    }
    
}



void UIComponent::playPowerAni(float _time){
    if(currentPower>=endPower){
        endPower = MAX(endPower,GlobalData::shared()->playerInfo.getTotalPower());
        currentPower = endPower;
//        m_power->setString(CC_CMDITOAL(currentPower));
        m_power->setString(CC_CMDITOAL(currentPower).c_str());
        this->unschedule(schedule_selector(UIComponent::playPowerAni));
        m_showPower = 0;
    }else{
        currentPower += perPower;
//        m_power->setString(CC_CMDITOAL(currentPower));
        m_power->setString(CC_CMDITOAL(currentPower).c_str());
        
    }
    
}

void UIComponent::onWriteMailBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    
    PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create("", ""));
    CCArray* p = CCArray::create();
    p->addObject(CCInteger::create(MAIL_OPEN));
    CCArray* d = CCArray::create();
    d->addObject(CCInteger::create(1));
    DataRecordCommand *cmd = new DataRecordCommand(OPEN_PANEL, p, d);
    cmd->sendAndRelease();
}

void UIComponent::onDelMailOPClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
//    m_wrireMailBtn->setVisible(false);
//    m_cancelDelMailBtn->setVisible(true);
//    m_delAllMailBtn->setVisible(true);
//    MailController::getInstance()->CanRemoveMail=true;
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_DEL_MSG);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_EDIT_MSG,CCInteger::create(1));
}

void UIComponent::onCloseNodeClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onCancelMoveBuild();
}

void UIComponent::onLongJingBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    CCLOGFUNC();
}

void UIComponent::onCancelMoveBuild()
{
    if (m_MoveBuildMsgNode->isVisible()) {
        FunBuildController::getInstance()->CancelMoveBuild();
        m_MoveBuildMsgNode->setVisible(false);
    }
}

void UIComponent::onShowMoveBuild(CCObject* params)
{
    m_MoveBuildMsgNode->setVisible(true);
    int cnt = FunBuildController::getInstance()->WillMovePos.size();
    if (cnt == 0) {
        m_moveBuildLabel->setString(_lang("102366"));
    }else {
        m_moveBuildLabel->setString(_lang("102368"));
    }
    
    CCString* posInfo = dynamic_cast<CCString*>(params);
    if (posInfo) {
        int pos = posInfo->intValue();
    }
}

void UIComponent::changeMailDelState()
{
    
//    m_delAllMailBtn->setVisible(true);
    if (CCCommonUtils::isIosAndroidPad() && m_titleOldTitle)
    {
        m_titleOldTitle->m_wrireMailBtn->setVisible(false);
        m_titleOldTitle->m_cancelDelMailBtn->setVisible(true);
    }
    else
    {
        m_wrireMailBtn->setVisible(false);
        m_cancelDelMailBtn->setVisible(true);
    }
    MailController::getInstance()->CanRemoveMail=true;
    MailController::getInstance()->clearOpMails();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_DEL_MSG);
}

void UIComponent::onCancelDelMailClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    if (CCCommonUtils::isIosAndroidPad() && this->m_titleOldTitle)
    {
        m_titleOldTitle->m_wrireMailBtn->setVisible(false);
        m_titleOldTitle->m_cancelDelMailBtn->setVisible(false);
        m_titleOldTitle->m_delAllMailBtn->setVisible(false);
    }
    else
    {
        m_wrireMailBtn->setVisible(false);
        m_cancelDelMailBtn->setVisible(false);
        m_delAllMailBtn->setVisible(false);
    }
    MailController::getInstance()->CanRemoveMail=false;
    MailController::getInstance()->clearOpMails();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_DEL_MSG);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_EDIT_MSG,CCInteger::create(0));
}

void UIComponent::onDelAllMailClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_DEL_ALL_MSG);
}


void UIComponent::playPowerAniEnd(CCObject* p){
    double battlePower = GlobalData::shared()->playerInfo.getTotalPower();
    battlePower = MAX(battlePower, 0);
    m_power->setString(CC_CMDITOAL(battlePower).c_str());
    m_power->setFontSize(75);
    this->unschedule(schedule_selector(UIComponent::playPowerAni));
    m_showPower = 0;
}



void UIComponent::playPowerAniEnd2(CCObject* p){
    
    double battlePower = GlobalData::shared()->playerInfo.getTotalPower();
    battlePower = MAX(battlePower, 0);
    m_power->setString(CC_CMDITOAL(battlePower).c_str());
    m_power->setFontSize(40);
    isPlayingPowerAni = false;
    GlobalData::shared()->playerInfo.lastBattlePower = battlePower;
    
}



void UIComponent::playAddPowerAni(CCObject* param){
    if(isPlayingPowerAni) return ;
    GlobalData::shared()->playerInfo.battlePower = GlobalData::shared()->playerInfo.getTotalPower();
    this->controllerPowerAni();
    
}

void UIComponent::controllerPowerAni(){
    
    isPlayingPowerAni = true;
    m_power->setFontSize(60);
    GlobalData::shared()->playerInfo.battlePower = GlobalData::shared()->playerInfo.getTotalPower();
    double total = GlobalData::shared()->playerInfo.battlePower;
    double lastBattlePower = GlobalData::shared()->playerInfo.lastBattlePower;
    
    endPower = total;
    currentPower = lastBattlePower;
    if(currentPower<1 && m_showPower>0){
        currentPower = total - m_showPower;
        currentPower = MAX(currentPower, 1);
    }
    CCCallFuncO* fun = CCCallFuncO::create(this, callfuncO_selector(UIComponent::playPowerAniEnd), NULL);
    
    CCCallFuncO* fun2 = CCCallFuncO::create(this, callfuncO_selector(UIComponent::playPowerAniEnd2), NULL);
    
    int tmpP = total - lastBattlePower;
    int repT = 9;
    if (tmpP<(repT*2)) {
        repT = ceil(tmpP*1.0/2);
        repT = MAX(1, repT);
    }
    
//    CCActionInterval * delayL1 = CCDelayTime::create(0.05);
//    CCActionInterval * scaleToL1 = CCScaleTo::create(0.1, 0.85,1.0);
//    CCActionInterval * scaleToL2 = CCScaleTo::create(0.1, 1.0,1.0);
//    CCActionInterval * seqL = CCSequence::create(scaleToL1, scaleToL2, NULL);
//    CCActionInterval * seqL_RP = CCRepeat::create(seqL, repT);
//    m_pFlagL->runAction(CCSequence::create(delayL1, seqL_RP, NULL));
//    
//    CCActionInterval * delayR1 = CCDelayTime::create(0.05);
//    CCActionInterval * scaleToR1 = CCScaleTo::create(0.1, 0.85,1.0);
//    CCActionInterval * scaleToR2 = CCScaleTo::create(0.1, 1.0,1.0);
//    CCActionInterval * seqR = CCSequence::create(scaleToR1, scaleToR2, NULL);
//    CCActionInterval * seqR_RP = CCRepeat::create(seqR, repT);
//    m_pFlagR->runAction(CCSequence::create(delayR1, seqR_RP, NULL));
    
    CCActionInterval * delayM1 = CCDelayTime::create(0.05);
    CCActionInterval * scaleToM1 = CCScaleTo::create(0.1, 1.2,1.1);
    CCActionInterval * scaleToM2 = CCScaleTo::create(0.1, 1.0,1.0);
    CCActionInterval * seqM = CCSequence::create(scaleToM1, scaleToM2, NULL);
    CCActionInterval * seqM_RP = CCRepeat::create(seqM, repT);
//    m_LvBG->runAction(CCSequence::create(delayM1, seqM_RP, fun, CCDelayTime::create(0.3), fun2, NULL));
    m_powerBgNode->runAction(CCSequence::create(delayM1, seqM_RP, fun, CCDelayTime::create(0.3), fun2, NULL));
    
    perPower = (long)(total - lastBattlePower)/(repT*2);
    perPower = MAX(perPower, 1);
    
    this->unschedule(schedule_selector(UIComponent::playPowerAni));
    
    this->schedule(schedule_selector(UIComponent::playPowerAni),0.05);
    
}

void UIComponent::showFlyCBView(int exp, int power)
{
    m_showPower = power;
    m_flyCBView->onShowInfo(exp, power);
}

void UIComponent::showFlyCBViewPower(int power)
{
    m_flyCBView->onShowPowerInfo(power);
}

void UIComponent::playIconByType(int resType)

{
    
    //    CCActionInterval * fadeto1 = CCFadeTo::create(0.1, 200);
    
    //    CCActionInterval * fadeto2 = CCFadeTo::create(0.1, 255);
    
    CCActionInterval * scaleto1 = CCScaleTo ::create(0.1, 0.9);
    
    CCActionInterval * scaleto2 = CCScaleTo ::create(0.1, 0.7);
    
    CCSequence * seq1 = CCSequence::create(scaleto1, scaleto2, NULL);
    
    //    CCSequence * seq2 = CCSequence::create(fadeto1, fadeto2, NULL);
    
    //    CCFiniteTimeAction * spawn =CCSpawn::create(seq1,seq2,NULL);
    
    if(resType == Food) {
        
        m_foodIcon->runAction(seq1);
        
    }
    
    else if(resType == Wood) {
        
        m_woodIcon->runAction(seq1);
        
    }
    
    else if(resType == Iron) {
        
        m_ironIcon->runAction(seq1);
        
    }
    
    else if(resType == Stone) {
        
        m_stoneIcon->runAction(seq1);
        
    }
    
    else if(resType == DragonFood) {
        
        m_dragonFoodIcon->runAction(seq1);
        
    }
    
    else if(resType == DragonGold) {
        
        m_dragonGoldIcon->runAction(seq1);
        
    }
    
}
void UIComponent::refreshVIPStatus(CCObject* p)
{
    refreshVIPStatus(0.0);
}

void UIComponent::refreshVIPStatus(float t){
    
    int dtime = GlobalData::shared()->playerInfo.vipEndTime - GlobalData::shared()->getWorldTime();
    
    int vipLv = VipUtil::getVipLevel(GlobalData::shared()->playerInfo.vipPoints);
    FunBuildController::getInstance()->checkFreeTime(true);
    
    this->m_vipText->setString(_lang_1("103001", CC_ITOA(vipLv)));
    
    if(dtime>0){
        
        //m_vipText->setColor({255,255,255});
        this->unschedule(schedule_selector(UIComponent::refreshVIPStatus));
        this->scheduleOnce(schedule_selector(UIComponent::refreshVIPStatus),dtime+2);
        m_sprVipHui->setVisible(false);
        
        m_sprVip->setVisible(true);
        
    }else{
        
        m_sprVipHui->setVisible(true);
        
        m_sprVip->setVisible(false);
        
        // m_vipText->setColor({90,85,81});
        
    }
    
    
    
}

void  UIComponent::showFlygold(){
    m_allianceAni->removeAllChildren();
    m_allianceAni->setVisible(false);
    if(GlobalData::shared()->playerInfo.isfirstJoin==1&&(!GlobalData::shared()->playerInfo.isInAlliance()) && FunBuildController::getInstance()->getMainCityLv()>=3) {
        
        m_allianceAni->setVisible(true);
        
        m_allianceAni1 = AllianceAni::create();
        
        m_allianceAni->addChild(m_allianceAni1);
        
    }
    
    else {
        
        m_allianceAni->removeAllChildren();
        
        m_allianceAni->setVisible(false);
        
    }
    
}
void UIComponent::changeChatChannel(int type){
    if(type == CHAT_ALLIANCE){
        m_countryIcon->setVisible(false);
        m_allianceIcon->setVisible(true);
    }
    else if(type == CHAT_COUNTRY){
        m_countryIcon->setVisible(true);
        m_allianceIcon->setVisible(false);
    }
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::enableNativeChat){
        if (!ChatServiceCocos2dx::isChatShowing) {
            refreshChatInfo(type);
        }
    }
    else
    {
        updateChatInfo(type);
    }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if(ChatServiceCocos2dx::enableNativeChat){
        if(!ChatServiceCocos2dx::isChatShowing_fun())
            UIComponent::getInstance()->updateChatInfoIOS(type);
    }
    else
    {
        UIComponent::getInstance()->updateChatInfo(type);
    }
#endif
    
//    refreshChatInfo(type);
}

void UIComponent::setActivityStatus(){
     m_limitActNode->setVisible(false);
//    if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN){
//        m_limitActNode->setVisible(true);
//    }else{
//        m_limitActNode->setVisible(false);
//    }
}
void UIComponent::resetGoldActivityBox(CCObject* p){
    // tao.yu 第一个版本关闭充值
    if (true) {
        this->m_rechargeNode->setVisible(false);
        return;
    }
    this->m_rechargeNode->removeAllChildren();
    if(GlobalData::shared()->analyticID == "common"){
        this->m_rechargeNode->setVisible(false);
        return;
    }
    if(goldExchangeType==0){
        goldExchangeType = 1;
    }else{
        goldExchangeType = 0;
    }
    bool isEnd = true;
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        if(!it->second)
            continue;
        if(it->second->type == "5"){
        }else{
            if(it->second->type == "3"){
                if(it->second->bought||it->second->popup_image=="close"){
                    continue;
                }
                if(it->second->end>GlobalData::shared()->getWorldTime()){
                    isEnd = false;
                    break;
                }
            }else if(it->second->type == "1"){
                if(it->second->bought|| it->second->popup_image=="hide" || it->second->popup_image.empty()){
                    continue;
                }
                if(it->second->end>GlobalData::shared()->getWorldTime()){
                    if(LuaController::getInstance()->showNoramlIcon(it->second->popup_image)){
                        isEnd = false;
                        break;
                    }
                }
            }
        }
    }
    if(isEnd){
        this->m_rechargeNode->setVisible(false);
    }else{
        this->m_rechargeNode->setVisible(true);
    }
    auto activityBox = ActivityBox::create(goldExchangeType);
    if(activityBox) {
        activityBox->setPosition(ccp(43, 43));
        this->m_rechargeNode->addChild(activityBox);
    }
//    if (CCCommonUtils::isIosAndroidPad()) {
//        this->m_rechargeNode->setVisible(false);
//    }
}
void UIComponent::resetRepayActivity(cocos2d::CCObject *p){
    if(RepayController::getInstance()->isBegin()){
        RepayIcon *rIcon = NULL;
        if(m_repayNode->getChildByTag(999)){
            rIcon = dynamic_cast<RepayIcon*>(m_repayNode->getChildByTag(999));
        }else{
            rIcon = RepayIcon::create();
            this->m_repayNode->addChild(rIcon);
            rIcon->setTag(999);
        }
        if(rIcon){
            if(RepayController::getInstance()->getChangeLV()){
                rIcon->showPartical(true);
            }else{
                rIcon->showPartical(false);
            }
            this->m_repayNode->setVisible(true);
        }else{
            this->m_repayNode->setVisible(false);
        }
//        rIcon->setPosition(ccp(45,45));
    }else{
        this->m_repayNode->removeAllChildren();
        this->m_repayNode->setVisible(false);
    }
}

void UIComponent::checkShowQuestPrc()
{
    CanShowQuestPrc = false;
    QuestPrcTimes = 0;
    m_questRecSpr->setVisible(false);
    if (m_recommandQuest==NULL) {
        return;
    }
    
    int CityLv = FunBuildController::getInstance()->getMainCityLv();
    if (CityLv>=5) {
        return;
    }
    
    int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_BARRACK1);
    if (m_recommandQuest->itemId == "2200101" || bid>0) {//建造步兵营任务
        CanShowQuestPrc = true;
    }
    
}

void UIComponent::playQuestRect()
{
    if (m_recommandQuest && !GuideController::share()->isInTutorial() && m_questContextBG->getScaleX()==1 && (m_recommandQuest->isShow==1||m_recommandQuest->isShow==2)) {
        m_questContextBG1->stopAllActions();
        m_questContextBG1->setVisible(!true); // guo.jiang
        
        if (m_recommandQuest->isShow==2) {
            m_questTipNpcNode->setVisible(true);
            m_questTipNpcNode->stopAllActions();
            m_questTipNpcNode->setPosition(ccp(-77, 127));
            CCActionInterval * moveTo1 = CCMoveTo::create(0.5, ccp(-93, 118));
            CCActionInterval * moveTo2 = CCMoveTo::create(0.5, ccp(-77, 127));
            CCActionInterval * repeat = CCRepeat::create(CCSequence::create(moveTo1, moveTo2, NULL), 7);
            m_questTipNpcNode->runAction(repeat);
        }
        
        CCActionInterval * fadein = CCFadeIn::create(0.7);
        CCActionInterval * fadeout = CCFadeOut::create(0.7);
        CCActionInterval * sequence = CCSequence::create(fadein, fadeout, NULL);
        CCActionInterval * repeat = CCRepeat::create(sequence, 5);
        CCCallFunc * endFunCall= CCCallFunc::create(this, callfunc_selector(UIComponent::hideQuestRect));
        m_questContextBG1->runAction(CCSequence::create(repeat, endFunCall, NULL));
    }
    return;
    m_questRecSpr->setVisible(true);
//    CCActionInterval * fadein = CCFadeIn::create(0.5);
//    CCActionInterval * fadeout = CCFadeOut::create(0.5);
//    CCActionInterval * repeat = CCRepeat::create(CCSequence::create(fadein, fadeout, NULL), 10);
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(UIComponent::hideQuestRect));
    m_questRecSpr->runAction(CCSequence::create(CCDelayTime::create(10), funcall, NULL) );
    
    CCRect tmp_rect = cocos2d::CCRect(0, 0, 520, 80);
    string tmpStart = "ShowFire_";
    int maxP = tmp_rect.size.width/3;
    for (int i=1; i<=5; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,maxP);
        particle->setPosVar(ccp(tmp_rect.size.width/2, 0));
        particle->setPosition(ccp(tmp_rect.origin.x+tmp_rect.size.width/2, tmp_rect.origin.y));
        addParticleToBatch(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(),CCPointZero,maxP);
        particle1->setPosVar(ccp(tmp_rect.size.width/2, 0));
        particle1->setPosition(ccp(tmp_rect.origin.x+tmp_rect.size.width/2, tmp_rect.origin.y+tmp_rect.size.height-7.5));
        addParticleToBatch(particle1);
    }
    
    tmpStart = "ShowFireUp_";
    maxP = tmp_rect.size.height/3;
    for (int i=1; i<=5; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(),CCPointZero,maxP);
        particle->setPosVar(ccp(0, tmp_rect.size.height/2));
        particle->setPosition(ccp(tmp_rect.origin.x, tmp_rect.origin.y+tmp_rect.size.height/2));
        addParticleToBatch(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(),CCPointZero,maxP);
        particle1->setPosVar(ccp(0, tmp_rect.size.height/2));
        particle1->setPosition(ccp(tmp_rect.origin.x+tmp_rect.size.width, tmp_rect.origin.y+tmp_rect.size.height/2));
        addParticleToBatch(particle1);
    }
}

void UIComponent::hideQuestRect()
{
    QuestPrcTimes = 0;
    m_parVec.clear();
    m_questRecSpr->stopAllActions();
    m_questRecSpr->setVisible(false);
    m_questRecNode->removeAllChildren();
    m_questTipNpcNode->setVisible(false);
}

void UIComponent::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
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
    m_questRecNode->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void UIComponent::setSerchCoordState(){
    if(GlobalData::shared()->playerInfo.isInSelfServer()){
        m_xCoordLabel->setPositionX(-78);
        m_xCoordText->setPositionX(-38);
        m_xBG->setPositionX(-38);
        
        m_yCoordLabel->setPositionX(20);
        m_yCoordText->setPositionX(58);
        m_yBG->setPositionX(58);

        m_findIcon->setPositionX(m_xCoordLabel->getPositionX() - 50);
        m_zCoordLabel->setVisible(false);
        m_zCoordText->setVisible(false);
        m_kingdomBG->setVisible(false);
        m_serverNode->setVisible(false);
    }else{
        m_findIcon->setPositionX(m_zCoordLabel->getPositionX() - 50);
        m_zCoordLabel->setVisible(true);
        m_zCoordText->setVisible(true);
        m_kingdomBG->setVisible(false);
        m_xCoordLabel->setPositionX(-22);
        m_xCoordText->setPositionX(14);
        m_xBG->setPositionX(14);
        
        m_yCoordLabel->setPositionX(76);
        m_yCoordText->setPositionX(114);
        m_yBG->setPositionX(114);
        m_serverNode->setVisible(true);
        setServerText(GlobalData::shared()->playerInfo.currentServerId);
    }
}

void UIComponent::CheckGuideUIShow()
{
    if (!m_UIQuestNodeStat) {
        return;
    }
    
    if (GlobalData::shared()->playerInfo.level<=2 && QuestController::getInstance()->currentAchList.find("2100101") != QuestController::getInstance()->currentAchList.end()) {
        m_UIQuestNode->setVisible(false);
        m_buildNode->setVisible(false);
    }else {
        m_UIQuestNode->setVisible(true);
        m_buildNode->setVisible(true);
    }
    bool _Tips = CCUserDefault::sharedUserDefault()->getBoolForKey("TASK_TIPS_ON", true);
    if (!_Tips && GuideController::share()->isInTutorial()) {
        _Tips = true;
    }
    m_UIQuestNode->setVisible(_Tips);
}

void UIComponent::setFavoriteIconVisible(bool b){
    m_newFavoriteIcon->setVisible(b);
}

void UIComponent::setServerText(int serverId){
//    std::string str = CC_ITOA(serverId);
//    for(auto it = WorldController::getInstance()->m_serverList.begin(); it != WorldController::getInstance()->m_serverList.end(); it++){
//        if(it->serverId == serverId){
//            str = it->serverName;
//            break;
//        }
//    }
    string aShortName = GlobalData::shared()->playerInfo.allianceInfo.shortName;
    if (aShortName != "") {
        aShortName = string("(") + aShortName + string(")");
    }
    string playerName = GlobalData::shared()->playerInfo.name;
    string lbStr =aShortName + playerName + string("#") + CC_ITOA(serverId);
    this->m_serverIdText->setString("");
    if(GlobalData::shared()->isCrossService && serverId<CROSS_SERVER_START_INDEX){
        string temp = _lang("138000");
//        temp.append("(");
//        temp.append(CC_ITOA(WorldController::getInstance()->selfPoint.x));
//        temp.append(",");
//        temp.append(CC_ITOA(WorldController::getInstance()->selfPoint.y));
//        temp.append(")");
        this->m_homeText->setString(temp);
    }else{
        this->m_homeText->setString(_lang("108678"));
    }
}

void UIComponent::updateTime(float t){
    checkCityFire(t);
    checkWatchInfo(t);
    resetKingsGiftNodePos();
    crossServerTip();
}

void UIComponent::checkWatchInfo(float t){
    bool flag = true;
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN || SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        int num = EnemyInfoController::getInstance()->m_enemyInfos.size();
        int sameNum = EnemyInfoController::getInstance()->m_enemyInSameAlliance.size();
        if(num>0||sameNum>0){
            EnemyInfoController::getInstance()->checkEnemyData();
        }
        num = EnemyInfoController::getInstance()->m_enemyInfos.size();
        double gapEnd = ActivityController::getInstance()->aActivityInfo.siegeET - GlobalData::shared()->getWorldTime();
        if(num>0 || (gapEnd>0 && ActivityController::getInstance()->aActivityInfo.siegeST>0)){
            m_watchNode->setVisible(true);
            CCSprite* watchIcon = dynamic_cast<CCSprite*>(m_watchNode->getChildByTag(88));
            CCSpriteFrame* cf = NULL;
            if(gapEnd>0){
                flag = false;
                cf = CCLoadSprite::getSF("icon_heiqishi.png");//放暗黑骑士图，这时临时的
            }else{
                cf = CCLoadSprite::getSF("icon_watchtower.png");
            }
            if (watchIcon!=NULL && cf!=NULL) {
                watchIcon->initWithSpriteFrame(cf);
            }
            if(m_fireNode->isVisible()){
                m_watchNode->setPosition(m_fireNode->getPosition()+ccp(0, 80));
            }else{
                m_watchNode->setPosition(m_fireNode->getPosition());
            }
        }else{
            m_watchNode->setVisible(false);
        }
    }else{
        m_watchNode->setVisible(false);
    }
    CCNode* node = m_watchNode->getChildByTag(99);
    if(m_watchNode->isVisible() && flag){
        if(node && node->getChildrenCount()<=0){
            auto particle = ParticleController::createParticle("Watchtower");
            particle->setPosition(ccp(10, 0));
            node->addChild(particle);
        }
    }else if (node){
        node->removeAllChildrenWithCleanup(true);
    }

}

void UIComponent::showUIQuestNode(bool b)
{
    if(!b) {
        m_UIQuestNodeStat = false;
        m_UIQuestNode->setVisible(b);
    }else {
        m_UIQuestNodeStat = true;
        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        if(layer && m_buildNode->isVisible()) {
            CheckGuideUIShow();
        }
    }
}

void UIComponent::resetKingsGiftNodePos(){
    int count = 0;
    if(m_fireNode->isVisible()){
        count++;
    }
    if(m_watchNode->isVisible()){
        count++;
    }
    m_kingsGiftNode->setPositionY(count * 80 + m_fireNode->getPositionY());
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_kingsGiftNode->setPositionY(count * 160 + m_fireNode->getPositionY());
    }
    m_kingsGiftNode->setVisible(GlobalData::shared()->playerInfo.officer == KINGDOM_KING_ID && !GlobalData::shared()->isCrossService);
}

void UIComponent::setLotteryNodeVisible(bool visible)
{
    if (FunBuildController::getInstance()->getMainCityLv()<2 || LotteryController::shared()->getOpenFlag()!=1)
    {
        m_lotteryNode->setVisible(false);
    }
    else
    {
        m_lotteryNode->setVisible(visible);
        if (visible && m_lotteryNode->getChildrenCount() == 0)
        {
            CCSize size = m_lotteryNode->getContentSize();
            auto ccb = CCBLoadFile("ActivitiesLuckDraw.ccbi", m_lotteryNode);
            ccb->setPosition(ccp(size.width/2, size.height/2));
            for (int i=1; i<=2; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("QueueGlow_%d",i)->getCString());
                m_lotteryNode->addChild(particle);
                particle->setPosition(ccp(size.width/2, size.height/2));
            }
        }
    }
}

void UIComponent::setTestFBNodeVisible(bool visible)
{
    if (!visible)
    {
        m_testFeedBackNode->setVisible(false);
    }
    else
    {
//        m_testFeedBackNode->setVisible(true);return;
        if (GlobalData::shared()->playerInfo.selfServerId == 0)
        {
            if(TestFeedBackController::shared()->getHasInit()){
                setTestFBFlame();
            }else{
                TestFeedBackController::shared()->initFeedBack();
            }
            m_testFeedBackNode->setVisible(true);
        }
        else
        {
            m_testFeedBackNode->setVisible(false);
        }
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        std::string versionCode = cocos2d::extension::CCDevice::getVersionCode();
        if(versionCode != ""){
            int version = atoi(versionCode.c_str());
            if(version >= 100){
                m_testFeedBackNode->setVisible(true);
            }
        }
#endif
    }
}

void UIComponent::setTestFBFlame(CCObject* params)
{
    int type = TestFeedBackController::shared()->getHasAtLeastOne() == true?2:1;
    CCSprite* sp = (CCSprite*)m_testFeedBackNode->getChildByTag(0);
    if (type == 1)
    {
        sp->setDisplayFrame(CCLoadSprite::loadResource("testFBIcon_g.png"));
    }
    else if (type == 2)
    {
        sp->setDisplayFrame(CCLoadSprite::loadResource("testFBIcon_h.png"));
    }
}

void UIComponent::refreshGoodsStatus(CCObject *ccObj){
    m_bagTipNode->setVisible(false);
    return;
    if(ToolController::getInstance()->m_newGoodsList.size()>0){
        if(!m_bagTipNode->isVisible()){
            m_bagTipNum->setString("...");
            m_bagTipNode->setVisible(true);
            CCPoint bagTipPos = m_bagTipNode->getPosition();
            CCEaseExponentialIn* easeInAction = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCMoveTo::create(0.2, ccp(bagTipPos.x,bagTipPos.y+10))));
            CCFiniteTimeAction* easeInActionRevers = CCMoveTo::create(0.2, ccp(bagTipPos.x,bagTipPos.y));
            
            CCEaseExponentialIn* easeInAction1 = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCMoveTo::create(0.2, ccp(bagTipPos.x, bagTipPos.y-10))));
            CCMoveTo* easeInAction1Revers = CCMoveTo::create(0.2, ccp(bagTipPos.x, bagTipPos.y));
            CCRepeat* repeatAction = CCRepeat::create(CCSequence::create(
                                                                         easeInAction
                                                                         , easeInActionRevers
                                                                         , easeInAction1
                                                                         ,easeInAction1Revers
                                                                         ,NULL)
                                                      , 3);
            CCSequence* ccseq = CCSequence::create(repeatAction,CCDelayTime::create(5),NULL);
            this->m_bagTipNode->runAction(CCRepeatForever::create(ccseq));
        }
    }else{
        if(m_bagTipNode->isVisible()){
            m_bagTipNode->stopAllActions();
            m_bagTipNode->setVisible(false);
            m_bagTipNode->setPosition(ccp(-285.0,75.4));
        }
    }
}

void UIComponent::refreshUIComponent()
{
    CCLOG("UIComponent::refreshUIComponent");
    refreshInit(NULL);
}

void UIComponent::refreshInit(CCObject *ccObj)  //init之后 如果ui已经创建修改ui属性
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(GlobalData::shared()->android_native_chat == 1 && ChatServiceCocos2dx::isChatShowing){
        return;
    }
#endif
    CCLOG("UIComponent::refreshInit");
    showResBar();
    updateResStatus();
    setUserData();
    onQuestStateUpdate(NULL);
    updateMailNum(NULL);
    updateHelpData(NULL);
}

void UIComponent::pushFeedSuccess(CCObject *param)
{
    this->scheduleOnce(schedule_selector(UIComponent::delayShowTip), 0.1);
}

void UIComponent::delayShowTip(float _time){
    CCCommonUtils::flyHint("", "", _lang("105552"));
    string data = "action=fbShare";
    GameController::getInstance()->callXCApi(data);
}

void UIComponent::onPlayExpParticle(CCObject* params)
{
    auto expNode = CCNode::create();
    m_expParticleNode->addChild(expNode);
    
    string tmpStart = "Collection_exp_";
    int count = 5;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPositionType(kCCPositionTypeRelative);
        expNode->addChild(particle);
        
    }
    CCActionInterval * delT = CCDelayTime::create(0.5);
    auto func = CCCallFuncO::create(this, callfuncO_selector(UIComponent::onPlayParticleEnd), expNode);
    expNode->runAction(CCSequence::create(delT, func, NULL));
}

void UIComponent::onPlayPowerParticle(CCObject* params)
{
    auto powerNode = CCNode::create();
    m_powerParticleNode->addChild(powerNode);
    string tmpStart = "Collection_fig_";
    int count = 5;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPositionType(kCCPositionTypeRelative);
        powerNode->addChild(particle);
    }
    CCActionInterval * delT = CCDelayTime::create(0.5);
    auto func = CCCallFuncO::create(this, callfuncO_selector(UIComponent::onPlayParticleEnd), powerNode);
    powerNode->runAction(CCSequence::create(delT, func, NULL));
}


void UIComponent::onPlayParticleEnd(CCObject *obj)
{
    CCNode* node = dynamic_cast<CCNode*>(obj);
    if (node) {
        node->stopAllActions();
        node->removeFromParent();
    }
}

void UIComponent::onMoveAndPlay(int type)
{
    if (m_flyArrow->getParent() == NULL)
    {
        m_mainControlNode->addChild(m_flyArrow, 100);
    }
    float posX = 0;
    float posY = 0;
    switch (type)
    {
        case 2://引导去世界
            posX = m_world->getPositionX();
            posY = m_world->getPositionY();
            break;
            
        case 1://引导去使用道具
            posX = m_store->getPositionX();
            posY = m_store->getPositionY();
            
            break;
        case 0://引导打开联盟
            posX = m_alliance->getPositionX();
            posY = m_alliance->getPositionY();
            
            break;
        default:
            break;
    }
    
    m_flyArrow->setPosition(ccp(posX, posY));
    m_flyArrow->setVisible(true);
    CCActionInterval * moveBy = CCMoveBy::create(0.3,ccp(10, 20));
    CCActionInterval * moveRBy = CCMoveBy::create(0.3,ccp(-10, -20));
    CCSequence* fadeAction = CCSequence::create(moveBy,moveRBy,NULL);
    CCActionInterval * fadeForever =CCRepeatForever::create((CCActionInterval* )fadeAction);
    m_flyArrow->runAction(fadeForever);
    scheduleOnce(schedule_selector(UIComponent::hideFlyArrow), 5.0f);
}

void UIComponent::hideFlyArrow(float dt)
{
    m_flyArrow->stopAllActions();
    m_flyArrow->setVisible(false);
}

void UIComponent::setCrossServiceNodeVisible(bool visible){
    if (!visible)
    {
        m_csNode->setVisible(false);
    }
    else
    {
        m_csNode->setVisible(GlobalData::shared()->isCrossService);
        m_csNode->setPositionY(CCDirector::sharedDirector()->getWinSize().height - 430);
    }
}

void UIComponent::onChangeShowRes(int type)
{
    if (type == 1) {
        m_dragonFoodBarNode->setVisible(true);
        m_dragonGoldBarNode->setVisible(true);
        
        m_woodBarNode->setVisible(false);
        m_foodBarNode->setVisible(false);
        m_ironBarNode->setVisible(false);
        m_stoneBarNode->setVisible(false);
    }else {
        m_dragonFoodBarNode->setVisible(false);
        m_dragonGoldBarNode->setVisible(false);
        
        m_woodBarNode->setVisible(true);
        m_foodBarNode->setVisible(true);
        showResBar();
    }
}

void UIComponent::changeMiniMapIcon(){
    if (m_miniIcon) {
        CCSpriteFrame* cf = NULL;
        if (GlobalData::shared()->isCrossService && GlobalData::shared()->playerInfo.currentServerId >CROSS_SERVER_START_INDEX) {
            cf = CCLoadSprite::getSF("icon_cs_return.png");
        }else{
            cf = CCLoadSprite::getSF("icon_resource_belt.png");
        }
        if (cf) {
            m_miniIcon->initWithSpriteFrame(cf);
        }
    }
    if (m_faveIcon) {
        //
        CCSpriteFrame* cf = NULL;
        if (GlobalData::shared()->isCrossService && GlobalData::shared()->playerInfo.currentServerId >CROSS_SERVER_START_INDEX && ActivityController::getInstance()->checkShengDianOpen()) {
            cf = CCLoadSprite::getSF("icon_liansheng.png");
        }else{
            cf = CCLoadSprite::getSF("world_favorite_icon.png");
        }
        if (cf) {
            m_faveIcon->initWithSpriteFrame(cf);
        }
    }
}

static int countFlag = 0;
void UIComponent::crossServerTip(){
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING) return;
    if (!GlobalData::shared()->isCrossService) {
        return;
    }
    if (GlobalData::shared()->isCrossService && ActivityController::getInstance()->ckfNeedLeave==1 && countFlag==1) {
        countFlag = 2;
        YesNoDialog* dialog = YesNoDialog::show(_lang("138072").c_str(),CCCallFunc::create(this, callfunc_selector(UIComponent::leaveCKF)));
        CCCommonUtils::setButtonTitle(dialog->m_btnOk, _lang("138078").c_str());
        dialog->setCloseCallback(CCCallFunc::create(this, callfunc_selector(UIComponent::leaveCKF)));
        return ;
    }
    if (countFlag<=0) {
        countFlag = 1;
    }
    auto view = dynamic_cast<YesNoDialog*>(PopupViewController::getInstance()->getCurrentPopupView());
    auto view2 = dynamic_cast<RechargeACTVCell*>(PopupViewController::getInstance()->getCurrentPopupView());
    auto view3 = dynamic_cast<UserUpgradeView*>(PopupViewController::getInstance()->getCurrentPopupView());
    
    if (!view && !view2 && !view3) {
        double nowTime = GlobalData::shared()->getWorldTime();
        bool flag = false;
        bool endFlag = false;
        int jieduanTime = 0;
        if (ActivityController::getInstance()->activityArr && ActivityController::getInstance()->activityArr->count()>0) {
            int num = ActivityController::getInstance()->activityArr->count();
            for (int i=0; i<num; i++) {
                auto temp = dynamic_cast<ActivityEventObj*>(ActivityController::getInstance()->activityArr->objectAtIndex(i));
                if (temp && temp->type==7) {
                    if (temp->startTime<nowTime && temp->endTime>nowTime)
                    {
                        int leftTime = temp->endTime - nowTime;
                        int k1 = ActivityController::getInstance()->wb_timing_k1;
                        int k2 = ActivityController::getInstance()->wb_timing_k2;
                        double lastTime = 0.0;
                        string strTime = CCUserDefault::sharedUserDefault()->getStringForKey("crossServerRecordTime", "");
                        if (strTime!="") {
                            lastTime = atof(strTime.c_str());
                        }
                        if (nowTime >lastTime) {//比上一个不让提示的时间大，才提示
                            jieduanTime = leftTime%(ActivityController::getInstance()->wb_info_k7*3600);
                            if (jieduanTime<k1) {
                                int fTime = leftTime%k2;
                                int gap = temp->endTime - temp->startTime;
                                int countNum = leftTime/(ActivityController::getInstance()->wb_info_k7*3600);
                                bool tempFlag = true;//如果小于30秒，阶段不再提示
                                if (jieduanTime<30) {
                                    tempFlag = false;
                                }
                                if(countNum ==0 && leftTime <= k1){
                                    endFlag = true;//是最后一个阶段
                                    tempFlag = true;
                                }
                                if (fTime<2 && tempFlag) {
                                    flag = true;
                                    double m_tipTime= nowTime + (ActivityController::getInstance()->wb_info_k6/ActivityController::getInstance()->wb_info_k7)*3600 + ActivityController::getInstance()->wb_timing_k2;
                                    CCUserDefault::sharedUserDefault()->setStringForKey("crossServerRecordTime", CC_ITOA(m_tipTime));
                                    CCUserDefault::sharedUserDefault()->flush();
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
        if (flag) {
            string temp = _lang("138077");
            if (endFlag) {
                temp = _lang("138092");
            }else{
                int jTime = jieduanTime/60;
                if (jTime==0) {
                    jTime = 1;
                }
                string tip = _lang_1("138046",CC_ITOA(jTime));
                double worldTime = GlobalData::shared()->getWorldTime();
                worldTime *= 1000;
                worldTime = GlobalData::shared()->renewTime(worldTime);
                auto time = worldTime+1000*60;
                CCCommonUtils::flySystemUpdateHint(time, true,tip.c_str(),FLY_HINT_SYSTEM);
            }
            YesNoDialog* dialog = YesNoDialog::show(temp.c_str(),CCCallFunc::create(this, callfunc_selector(UIComponent::continueNextRound)),0,false,CCCallFunc::create(this, callfunc_selector(UIComponent::cancelTipNextRound)),NULL);
            CCCommonUtils::setButtonTitle(dialog->m_btnCancel, _lang("138079").c_str());
            CCCommonUtils::setButtonTitle(dialog->m_btnOk, _lang("138078").c_str());
            dialog->showCancelButton();
        }
    }
}

void UIComponent::continueNextRound(){
    CKFNextRoundCommand* cmd = new CKFNextRoundCommand();
    cmd->sendAndRelease();
    double nowTime = GlobalData::shared()->getWorldTime();
    double tempTime = nowTime + (ActivityController::getInstance()->wb_info_k6/ActivityController::getInstance()->wb_info_k7)*3600 + ActivityController::getInstance()->wb_timing_k2;
    CCUserDefault::sharedUserDefault()->setStringForKey("crossServerRecordTime", CC_ITOA(tempTime));
    CCUserDefault::sharedUserDefault()->flush();
}

void UIComponent::cancelTipNextRound(){
    double nowTime = GlobalData::shared()->getWorldTime();
    double tempTime = nowTime + (ActivityController::getInstance()->wb_info_k6/ActivityController::getInstance()->wb_info_k7)*3600 +ActivityController::getInstance()->wb_timing_k2;
    CCUserDefault::sharedUserDefault()->setStringForKey("crossServerRecordTime", CC_ITOA(tempTime));
    CCUserDefault::sharedUserDefault()->flush();
}

void UIComponent::leaveCKFServer(CCObject* params){
    double nowTime = GlobalData::shared()->getWorldTime();
    string tip = _lang("138072");
    if (ActivityController::getInstance()->activityArr && ActivityController::getInstance()->activityArr->count()>0) {
        int num = ActivityController::getInstance()->activityArr->count();
        for (int i=0; i<num; i++) {
            auto temp = dynamic_cast<ActivityEventObj*>(ActivityController::getInstance()->activityArr->objectAtIndex(i));
            if (temp && temp->type==7) {
                if (nowTime > temp->endTime)
                {
                    tip = _lang("138092");
                    break;
                }
            }
        }
    }
    ActivityController::getInstance()->ckfServerInfo = dynamic_cast<CCDictionary*>(params);
    YesNoDialog* dialog = YesNoDialog::show(tip.c_str(),CCCallFunc::create(this, callfunc_selector(UIComponent::leaveCKFconfirm)));
     dialog->setCloseCallback(CCCallFunc::create(this, callfunc_selector(UIComponent::leaveCKFconfirm)));
}

void UIComponent::leaveCKFconfirm(){
    ActivityController::getInstance()->leaveCKFServer(ActivityController::getInstance()->ckfServerInfo);
    ActivityController::getInstance()->ckfServerInfo = NULL;
}

void UIComponent::leaveCKF(){
    CSALeaveCommand* cmd = new CSALeaveCommand();
    cmd->sendAndRelease();
}

void UIComponent::showAllianceTip()
{
    this->scheduleOnce(schedule_selector(UIComponent::addAllianceTip),3);
}
void UIComponent::addAllianceTip(float _time)
{
    if (m_allianceTipNode) {
        m_allianceTipNode->removeAllChildren();
        auto view = JoinRecAllianceTipView::create();
        if (view) {
            m_allianceTipNode->addChild(view);
        }
    }
}
void UIComponent::showShakeGuideLayer(){
    CCLOGFUNC("showshakeGuideLayer!");
    m_shakeGuideNode->removeAllChildrenWithCleanup(true);
    m_shakeGuideNode->addChild(IFShakeGuideLayer::create());
}
void UIComponent::removeShakeGuide(){
    m_shakeGuideNode->removeAllChildrenWithCleanup(true);
}