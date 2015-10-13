//
//  UIComponent.h
//  IF
//
//  Created by wangzhenlei on 13-9-10.
//
//

#ifndef __IF__UIComponent__
#define __IF__UIComponent__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "BuildQueueView.h"
#include "QuestInfo.h"
#include "BuildQueueCell.h"
#include "AllianceAni.h"
#include "FlyCBuildView.h"
#include "HFHeadImgNode.h"
#include "ChatInfo.h"

enum BtnIndex{
    BTN_GENERAL,
    BTN_UPDATE,
    BTN_FORMATION,
    BTN_ARENA,
    BTN_GAMBLE,
    BTN_REWARD,
};

enum OldTitleType
{
    UIComponentOldTitle_ONE_TYPE = 1,  // 任务
    UIComponentOldTitle_TWO_TYPE,   // 背包
    UIComponentOldTitle_THREE_TYPE, // 邮件
    UIComponentOldTitle_FOUR_TYPE, // 联盟
    UIComponentOldTitle_FIVE_TYPE,
    UIComponentOldTitle_SIX_TYPE,
    UIComponentOldTitle_SEVEN_TYPE,
};

class UIComponent;

class UIComponentOldTitle : public CCLayer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    UIComponentOldTitle():_targetDelegate(NULL),_oldTitleType(UIComponentOldTitle_ONE_TYPE) {}
    static UIComponentOldTitle* create(OldTitleType type);
    virtual bool init(OldTitleType type);
    void setDelegate(UIComponent* delegate) {_targetDelegate = delegate; }
    
    void updateShowUIComponentByType(OldTitleType type);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onPopupReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onEquipCrtBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onEquipBagBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onWriteMailBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onCancelDelMailClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onDelAllMailClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onDelMailOPClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onGoldBuyBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
public:
    OldTitleType _oldTitleType;
    UIComponent* _targetDelegate;
    
    CCSafeObject<CCControlButton> m_popupReturnBtn;
    
    CCSafeObject<CCNode> m_collectNode;
    CCSafeObject<CCSpriteBatchNode> m_collectBatch;
    CCSafeObject<CCSprite> m_coordBG;
    CCSafeObject<CCControlButton> m_findIcon;
    CCSafeObject<CCSprite> m_faveBG;
    CCSafeObject<CCSprite> m_miniBG;
    CCSafeObject<CCSprite> m_goHomeBtn;
    CCSafeObject<CCNode> m_inUiTitle;
    
    CCSafeObject<CCSprite> m_woodIcon;
    CCSafeObject<CCSprite> m_ironIcon;
    CCSafeObject<CCSprite> m_foodIcon;
    CCSafeObject<CCSprite> m_dragonFoodIcon;
    CCSafeObject<CCSprite> m_dragonGoldIcon;
    CCSafeObject<CCSprite> m_stoneIcon;
    CCSafeObject<CCNode> m_watchNode;
    
    CCSafeObject<CCNode> m_bagTipNode;
    CCSafeObject<CCSprite> m_bagTipBG;
    CCSafeObject<CCLabelIF> m_bagTipNum;
    
    CCSafeObject<CCNode> m_googlePlayNode;
    
    CCSafeObject<CCSprite> m_howToPlay;
    CCSafeObject<CCSprite> m_world;
    CCSafeObject<CCSprite> m_general;
    CCSafeObject<CCSprite> m_alliance;
    CCSafeObject<CCSprite> m_homeBack;
    CCSafeObject<CCSprite> m_configSet;
    
    CCSafeObject<CCSprite> m_homeBG;
    CCSafeObject<CCSprite> m_storeBG;
    CCSafeObject<CCSprite> m_allianceBG;
    
    CCSafeObject<CCControlButton> m_marketBtn;
    CCSafeObject<CCControlButton> m_CountryBtn;
    
    CCSafeObject<CCControlButton> m_status;
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCControlButton> m_EquipBagBtn;
    CCSafeObject<CCControlButton> m_EquipCrtBtn;
    CCSafeObject<CCNode> m_EquipBagNode;
    CCSafeObject<CCNode> m_EquipCrtNode;
    CCSafeObject<CCNode> m_miniNode;
    CCSafeObject<CCNode> m_rechargeNode;
    CCSafeObject<CCNode> m_repayNode;
    CCSafeObject<CCNode> m_UIQuestNode;
    CCSafeObject<CCNode> m_configNode;
    CCSafeObject<CCControlButton> m_generalGambleBtn;
    CCSafeObject<CCControlButton> m_goldBuyBtn;
    
    CCSafeObject<CCControlButton> m_achievementBtn;
    
    CCSafeObject<CCLabelIF> m_allianceBtnTitle;
    CCSafeObject<CCLayerColor> m_generalGambleBtnHint;
    
    CCSafeObject<CCLabelIF> m_homeBackBtnTitle;
    CCSafeObject<CCLabelIF> m_worldBtnTitle;
    CCSafeObject<CCLabelIF> m_level;
    CCSafeObject<CCLabelIF> m_gold;
    CCSafeObject<CCNode>  m_nodeGoldIcon;
    //    CCSafeObject<CCLabelIF> m_goldNum;
    CCSafeObject<CCLabelIF> m_wood;
    CCSafeObject<CCLabelIF> m_iron;
    CCSafeObject<CCLabelIF> m_food;
    CCSafeObject<CCLabelIF> m_stone;
    CCSafeObject<CCLabelIF> m_dragonFood;
    CCSafeObject<CCLabelIF> m_dragonGold;
    CCSafeObject<CCLabelIFTTF> m_messageText;
    CCSafeObject<CCLabelIFBMFont> m_chatName;
    
    CCSafeObject<CCNode> m_floatingNode;
    CCSafeObject<CCLabelIF> m_floatingGold;
    CCSafeObject<CCLabelIF> m_floatingMoney;
    CCSafeObject<CCLabelIF> m_floatingWood;
    CCSafeObject<CCLabelIF> m_floatingFood;
    CCSafeObject<CCLabelIF> m_floatingIron;
    CCSafeObject<CCNode> m_questNameNode;
    CCSafeObject<CCNode> m_mainControlNode;
    CCSafeObject<CCNode> m_UserNode;
    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCNode> m_otherPartNode;
    CCSafeObject<CCNode> m_uiTitle;
    
    CCSafeObject<CCNode> m_woodNode;
    CCSafeObject<CCNode> m_goldNode;
    CCSafeObject<CCNode> m_goldNewNode;
    CCSafeObject<CCNode> m_moneyNode;
    CCSafeObject<CCNode> m_ironNode;
    CCSafeObject<CCNode> m_foodNode;
    CCSafeObject<CCNode> m_stoneNode;
    CCSafeObject<CCNode> m_woodBarNode;
    CCSafeObject<CCNode> m_ironBarNode;
    CCSafeObject<CCNode> m_foodBarNode;
    CCSafeObject<CCNode> m_stoneBarNode;
    
    CCSafeObject<CCNode> m_dragonFoodBarNode;
    CCSafeObject<CCNode> m_dragonGoldBarNode;
    
    CCSafeObject<CCNode> m_questTipNode;
    CCSafeObject<CCNode> m_questGoldNode;
    CCSize m_woodBarPos;
    CCSize m_ironBarPos;
    CCSize m_foodBarPos;
    CCSize m_stoneBarPos;
    CCSafeObject<CCSprite> m_sprEXPProgress;
    CCSafeObject<CCSprite> m_LvBG;
    CCSafeObject<CCSprite> m_pFlagL;
    CCSafeObject<CCSprite> m_pFlagR;
    CCSafeObject<CCSprite> m_playerBG;
    CCSafeObject<CCNode> m_vipNode;
    CCSafeObject<CCScale9Sprite> m_expBar;
    
    CCSafeObject<CCNode> m_UserResBg;
    CCSafeObject<CCScale9Sprite> m_rightResBg;
    
    CCSafeObject<CCScale9Sprite> m_questRecSpr;
    CCSafeObject<CCNode> m_questRecNode;
    CCSafeObject<CCSprite> m_chatBG;
    CCSafeObject<CCNode> m_questBG;
    CCSafeObject<CCNode> m_questBG2;
    CCSafeObject<CCSprite> m_popupTitleBg1;
    CCSafeObject<CCSprite> m_popupTitleBg2;
    CCSafeObject<CCSprite> m_countryIcon;
    CCSafeObject<CCSprite> m_allianceIcon;
    CCSafeObject<CCNode> m_achNumBg;
    CCSafeObject<CCLabelIF> m_achNum;
    CCSafeObject<CCNode> m_achNode;
    CCSafeObject<CCNode> m_queueNode;
    
    CCSafeObject<CCNode> m_worldUINode;
    
    CCSafeObject<CCLabelIF> m_configSetBtnTitle;
    CCSafeObject<CCLabelIF> m_storeBtnLabel;
    CCSafeObject<CCLabelIF> m_popupTitleName;
    CCSafeObject<CCLabelIF> m_goldName;
    
    CCSafeObject<CCNode> m_armyCDContainer;
    
    CCSafeObject<CCNode> m_frameEffNode;
    
    CCSafeObject<CCLabelIF> m_powerName;
    CCSafeObject<CCLabelIFBMFont> m_power;
    CCSafeObject<CCLabelIF> m_questName;
    CCSafeObject<CCLabelIF> m_questTitle;
    CCSafeObject<CCLabelIF> m_questTipNum;
    
    CCSafeObject<CCNode> m_queueCell;
    CCSafeObject<CCNode> m_expansionNode;
    CCSafeObject<CCLabelIF> m_lbExpansion;
    CCSafeObject<CCControlButton> m_cdExpansion;
    CCSafeObject<CCControlButton> m_wrireMailBtn;
    CCSafeObject<CCSprite> m_sprExpansion;
    CCSafeObject<CCSprite> m_sprPack;
    CCSafeObject<CCScale9Sprite> m_cdBG;
    CCSafeObject<CCScrollView> m_scrollQueueView;
    CCSafeObject<CCNode> m_rightNode;
    CCSafeObject<CCNode> m_fireNode;
    CCSafeObject<CCNode> m_kingsGiftNode;
    CCSafeObject<CCLabelIF> m_mailBtnTimes;
    CCSafeObject<CCLabelIF> m_mailBtnTitle;
//    CCSafeObject<CCSprite> m_mailBtnTimesBG;
    CCSafeObject<CCSprite> m_mailBG;
    CCSafeObject<CCNode> m_mailBtnsNode;
    CCSafeObject<CCControlButton> m_cancelDelMailBtn;
    CCSafeObject<CCControlButton> m_delAllMailBtn;
    CCSafeObject<CCControlButton> m_delMailOpBtn;
    CCSafeObject<CCSprite> m_newFavoriteIcon;
    CCSafeObject<CCSprite> m_questTipBG;
    CCSafeObject<CCSprite> m_mailBGFlag2;
    CCSafeObject<CCSprite> m_chatNewAlert;
    CCSafeObject<CCNode> m_questIcon;
    CCSafeObject<CCNode> m_questAnimNode;
    CCSafeObject<CCScale9Sprite> m_questContextBG;
    CCSafeObject<CCScale9Sprite> m_questContextBG1;
    CCSafeObject<CCNode> m_questTipNpcNode;
    
    CCSafeObject<CCScale9Sprite> m_stamineBar;
    CCSafeObject<CCSprite> m_sprVip;
    CCSafeObject<CCSprite> m_sprVipHui;
    CCSafeObject<CCLabelIFBMFont> m_vipText;
    CCSafeObject<CCNode> m_testFeedBackNode;
    CCSafeObject<CCLabelIF> m_lblTestFeedBack;
    CCSafeObject<CCNode> m_lotteryNode;
    
    BuildQueueCell* m_buildQueueCell1;
    BuildQueueCell* m_buildQueueCell2;
    
    //help
    CCSafeObject<CCLabelIF> m_helpNum;
    CCSafeObject<CCNode> m_helpNumBg;
    CCSafeObject<CCNode> m_helpNode;
    CCSafeObject<CCNode> m_helpNumNode;
    CCSafeObject<CCLabelIF> m_helpNumTxt;
    
    CCSafeObject<CCNode> m_powerParticleNode;
    CCSafeObject<CCNode> m_expParticleNode;
    
    CCSafeObject<CCNode> m_powerNode;
    CCSafeObject<CCNode> m_powerAndBgNode;
    CCSafeObject<CCNode> m_giftNode1;
    CCSafeObject<CCNode> m_giftNode2;
    
    CCSafeObject<CCNode> m_flyTextNode;
    CCSafeObject<CCNode> m_pic;
    
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCNode> m_limitActNode;
    CCSafeObject<CCNode> m_limitActBg;
    
    CCSafeObject<CCNode> m_skillNode;
    CCSafeObject<CCNode> m_skillBG;
    
    CCSafeObject<CCNode> m_serverNode;
    CCSafeObject<CCLabelIF> m_homeText;
    CCSafeObject<CCLabelIF> m_serverIdText;
    
    CCSafeObject<FlyCBuildView> m_flyCBView;
    
    CCSafeObject<CCControlButton> m_closeNodeBtn;
    CCSafeObject<CCLabelIF> m_moveBuildLabel;
    CCSafeObject<CCNode> m_MoveBuildMsgNode;
    CCSafeObject<CCNode> m_powerBgNode;
};

//金币
class UIComponentGold : public CCLayer
,public CCBMemberVariableAssigner
{
public:
    static UIComponentGold* create(long goldValue);
    virtual bool init(long goldValue);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void setGoldValue(long goldValue);
private:
    void playAnimation(float dt);
    CCSafeObject<CCLabelIF> m_goldNum;
    CCSafeObject<CCSprite> m_goldIcon;
    CCSafeObject<CCSprite> m_goldStar;
    long mFinalValue;
    long mNowValue;
    bool mInAnimation;
};
//龙晶
class UIComponentLongJing : public CCLayer
,public CCBMemberVariableAssigner
{
public:
    static UIComponentLongJing* create(long goldValue);
    virtual bool init(long goldValue);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void setGoldValue(long goldValue);
private:
    void playAnimation(float dt);
    CCSafeObject<CCLabelIF> m_goldNum;
    CCSafeObject<CCSprite> m_goldIcon;
    long mFinalValue;
    long mNowValue;
    bool mInAnimation;
};

class UIComponent: public CCLayer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    ~UIComponent();
    UIComponent();
    static UIComponent* getInstance();
    static void purgeData();
    void setUserData();
    void onCreateCropSceneUI();
    void updateChatInfo(int type);
    void updateChatInfoIOS(int type);
    void updateGold(int gold);
    void updateLongJing(int longJing);//更新龙晶
    void updateEquipTips(CCObject* params);
    void onCityResoureceUpdate(CCObject* params);
    void UIHide();
    void UIMove(bool _bShow);
    void hidePopupBG();
    void showPopupView(UIPopupViewType type=UIPopupViewType_NONE, bool isHD=true);
    void showResourceBar(bool _bShow);
    void setPopupTitleName(string _name);
    CCNode* getNodeByIndex(string _key);
    void updateMailNum(CCObject* p);
    void updateNewChatNews(int num);
    void updateBuildState(bool st=false, int qid=0);
    void loadMailResource();
    
    void refreshUIComponent();
    void refreshChatInfo(int type);
    void refreshChatInfoIOS(ChatInfo info);
    void playIconByType(int resType);
    
    void showCloseBtn(bool show);
    void showEquipOrBagBtn(int type);
    void hideReturnBtn();
    CCSafeObject<CCLabelIF> m_xCoordLabel;
    CCSafeObject<CCLabelIF> m_yCoordLabel;
    CCSafeObject<CCLabelIF> m_zCoordLabel;
    CCSafeObject<CCLabelIF> m_xCoordText;
    CCSafeObject<CCLabelIF> m_yCoordText;
    CCSafeObject<CCLabelIF> m_zCoordText;
    CCSafeObject<CCSprite> m_kingdomBG;
    CCSafeObject<CCSprite> m_xBG;
    CCSafeObject<CCSprite> m_yBG;

    CCSafeObject<CCNode> m_tileCoordinateNode;
    CCSafeObject<CCNode> m_faveNode;
    void showGoldBuyPanel();
    CCSafeObject<CCControlButton> m_popupReturnBtn;
    
    CCSafeObject<CCNode> m_collectNode;
    CCSafeObject<CCSpriteBatchNode> m_collectBatch;
    CCSafeObject<CCSprite> m_coordBG;
    CCSafeObject<CCControlButton> m_findIcon;
    CCSafeObject<CCSprite> m_faveBG;
    CCSafeObject<CCSprite> m_miniBG;
    CCSafeObject<CCSprite> m_goHomeBtn;
    CCSafeObject<CCNode> m_inUiTitle;
    
    CCSafeObject<CCSprite> m_woodIcon;
    CCSafeObject<CCSprite> m_ironIcon;
    CCSafeObject<CCSprite> m_foodIcon;
    CCSafeObject<CCSprite> m_dragonFoodIcon;
    CCSafeObject<CCSprite> m_dragonGoldIcon;
    CCSafeObject<CCSprite> m_stoneIcon;
    CCSafeObject<CCNode> m_watchNode;
    CCSafeObject<CCNode> m_skillBG;
    CCSafeObject<CCNode> m_bagTipNode;
    CCSafeObject<CCSprite> m_bagTipBG;
    CCSafeObject<CCLabelIF> m_bagTipNum;
    
    CCSafeObject<CCNode> m_googlePlayNode;
    
    UIComponentOldTitle* m_titleOldTitle;
    
    void showResBar();
    void onOpenShowBuildQueue2();
    void showClickEff(CCPoint pt);
    void showFlygold();
    CCSafeObject<CCSprite> m_store;
    CCSafeObject<CCNode> m_buildNode;
    
    void changeChatChannel(int type);
    void changeMailDelState();
    void showFlyCBView(int exp, int power);
    void showFlyCBViewPower(int power);
    
    void checkShowQuestPrc();
    int QuestPrcTimes;
    bool CanShowQuestPrc;
    void setSerchCoordState();
    
    void CheckGuideUIShow();
    void setFavoriteIconVisible(bool b);
    void setServerText(int serverId);
    void showCityFireBtn(CCObject* p);
    void showUIQuestNode(bool b);
    void onCancelMoveBuild();
    
    void showCountryIcon(bool isShow);
    void setTestFBFlame(CCObject* params = NULL);
    void onMoveAndPlay(int type);
    
    void onVipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onChangeShowRes(int type);
    void changeMiniMapIcon();
    void hideFlyArrow(float dt);
    void hideQuestRect();
    
    void showAllianceTip();
    void showShakeGuideLayer();
    CC_SYNTHESIZE(int, m_queueCount, queueCount);
    void removeShakeGuide();
public:
    void onPopupReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onCancelDelMailClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onDelMailOPClick(CCObject * pSender, CCControlEvent pCCControlEvent);

private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(CCSize size);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onFaveBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onRechargeBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onLoginRewardBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void OnHomeBackBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onPVEBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onWorldBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onShowGeneralView(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onShowCountryView(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onChatBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onAllianceBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onConfigSetBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onGeneralGambleClick(CCObject * pSender, CCControlEvent pCCControlEvent);
//    void onChatBarClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onGoldBuyBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onWorldFindClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onAchievementClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onGCMRewardClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onStoreBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void onMailClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onActivityClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void onFaceClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onStatusClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCloseBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onEquipBagBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onEquipCrtBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onWriteMailBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onSceneChanged(CCObject* params);
    void resourceFloatingAni();
    void resetFloatingAni(CCNode* p);
    void updateResStatus();
    void onGoToBuilding();
    void refreshVIPStatus(CCObject* p);
    void refreshVIPStatus(float t);
    void checkCityFire(float t);
    void checkWatchInfo(float t);
    void updateTime(float t);//以后如果要时间检测的，都放到这里来进行
    
    void onDelAllMailClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void onCloseNodeClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onShowMoveBuild(CCObject* params);
    
    void onAddQueue(CCObject* params);
    void onCDExpansion(CCObject* params, Control::EventType pCCControlEvent);
    void onUpdateStamine(CCObject* params);
    void setOldResource();
    void showQuest();
    void onClickTarget();
    void onQuestStateUpdate(CCObject* p);
    void onQuestRemove(CCObject* p);
    void createLightSprite(CCNode* node);
    
    void delClickEff(CCObject *obj);
    
    void onShowFrameEff(CCObject* params);
    void onHideFrameEff(CCObject* params);
    void setFoodColor();
    
    void moveChatInfo(int type);
    
    void questIconAction();
    void questBGAction();
    void questBGHideAction();
    void showQuestText();
    void showAllianceHelp();
    void updateHelpData(CCObject* param);
    void onEnterFrame(float dt);
    void onChangePic(CCObject* params);
    void playPowerAni(float _time);
    void playPowerAniEnd(CCObject* p);
    void playPowerAniEnd2(CCObject* p);
    void playAddPowerAni(CCObject* param);
    void controllerPowerAni();
    void playQusetTiAnim();
    void playQusetTiAnimFinish();
    void updateActivityEvent(CCObject* param);
    void setActivityStatus();
    
    void onOKGotoBuild();
    void resetGoldActivityBox(CCObject* p);
    void resetRepayActivity(CCObject* p);
    void onPlayExpParticle(CCObject* params);
    void onPlayPowerParticle(CCObject* params);
    void onPlayParticleEnd(CCObject *obj);
    
    void playQuestRect();
    
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void showHideNewUserHelpDialog();
    void sureToHideNewUserHelp();
    void resetKingsGiftNodePos();
    void setTestFBNodeVisible(bool visible);
    void setLotteryNodeVisible(bool visible);
    void setCrossServiceNodeVisible(bool visible);
    
    void refreshGoodsStatus(CCObject *ccObj);
    void pushFeedSuccess(CCObject* params);
    void refreshInit(CCObject *ccObj);
    void delayShowTip(float _time);
    
    void addAllianceTip(float _time);
    void crossServerTip();
    void continueNextRound();
    void cancelTipNextRound();
    void leaveCKFServer(CCObject* params);
    void leaveCKFconfirm();
    void leaveCKF();
    CCPoint m_curPt;
    //UI
    
//    CCSafeObject<CCSprite> m_PVE;
    CCSafeObject<CCSprite> m_howToPlay;
    CCSafeObject<CCSprite> m_world;
    CCSafeObject<CCSprite> m_general;
    CCSafeObject<CCSprite> m_alliance;
    CCSafeObject<CCSprite> m_homeBack;
    CCSafeObject<CCSprite> m_configSet;

    CCSafeObject<CCSprite> m_homeBG;
    CCSafeObject<CCSprite> m_storeBG;
    CCSafeObject<CCSprite> m_allianceBG;
    
    CCSafeObject<CCControlButton> m_marketBtn;
    CCSafeObject<CCControlButton> m_CountryBtn;
    
    CCSafeObject<CCControlButton> m_status;
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCControlButton> m_EquipBagBtn;
    CCSafeObject<CCControlButton> m_EquipCrtBtn;
    CCSafeObject<CCNode> m_EquipBagNode;
    CCSafeObject<CCNode> m_EquipCrtNode;
    CCSafeObject<CCNode> m_miniNode;
    CCSafeObject<CCNode> m_rechargeNode;
    CCSafeObject<CCNode> m_repayNode;
    CCSafeObject<CCNode> m_UIQuestNode;
    CCSafeObject<CCNode> m_configNode;
    CCSafeObject<CCControlButton> m_generalGambleBtn;
    CCSafeObject<CCControlButton> m_goldBuyBtn;
    
    CCSafeObject<CCControlButton> m_achievementBtn;
    
    CCSafeObject<CCLabelIF> m_allianceBtnTitle;
    CCSafeObject<CCLayerColor> m_generalGambleBtnHint;

    CCSafeObject<CCLabelIF> m_homeBackBtnTitle;
    CCSafeObject<CCLabelIF> m_worldBtnTitle;
    CCSafeObject<CCLabelIF> m_level;
    CCSafeObject<CCLabelIF> m_gold;
    CCSafeObject<UIComponentGold> m_goldNum;
    CCSafeObject<CCNode>  m_nodeGoldIcon;
//    CCSafeObject<CCLabelIF> m_goldNum;
    CCSafeObject<CCLabelIF> m_wood;
    CCSafeObject<CCLabelIF> m_iron;
    CCSafeObject<CCLabelIF> m_food;
    CCSafeObject<CCLabelIF> m_stone;
    CCSafeObject<CCLabelIF> m_dragonFood;
    CCSafeObject<CCLabelIF> m_dragonGold;
    CCSafeObject<CCLabelIFTTF> m_messageText;
    CCSafeObject<CCLabelIFBMFont> m_chatName;
    
    CCSafeObject<CCNode> m_floatingNode;
    CCSafeObject<CCLabelIF> m_floatingGold;
    CCSafeObject<CCLabelIF> m_floatingMoney;
    CCSafeObject<CCLabelIF> m_floatingWood;
    CCSafeObject<CCLabelIF> m_floatingFood;
    CCSafeObject<CCLabelIF> m_floatingIron;
    CCSafeObject<CCNode> m_questNameNode;
    CCSafeObject<CCNode> m_mainControlNode;
    CCSafeObject<CCNode> m_UserNode;
    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCNode> m_otherPartNode;
    CCSafeObject<CCNode> m_uiTitle;

    CCSafeObject<CCNode> m_woodNode;
    CCSafeObject<CCNode> m_goldNode;
    CCSafeObject<CCNode> m_goldNewNode;
    CCSafeObject<CCNode> m_moneyNode;
    CCSafeObject<CCNode> m_ironNode;
    CCSafeObject<CCNode> m_foodNode;
    CCSafeObject<CCNode> m_stoneNode;
    CCSafeObject<CCNode> m_woodBarNode;
    CCSafeObject<CCNode> m_ironBarNode;
    CCSafeObject<CCNode> m_foodBarNode;
    CCSafeObject<CCNode> m_stoneBarNode;
    
    CCSafeObject<CCNode> m_dragonFoodBarNode;
    CCSafeObject<CCNode> m_dragonGoldBarNode;

    CCSafeObject<CCNode> m_questTipNode;
    CCSafeObject<CCNode> m_questGoldNode;
    CCSize m_woodBarPos;
    CCSize m_ironBarPos;
    CCSize m_foodBarPos;
    CCSize m_stoneBarPos;
    CCSafeObject<CCSprite> m_sprEXPProgress;
    CCSafeObject<CCSprite> m_LvBG;
    CCSafeObject<CCSprite> m_pFlagL;
    CCSafeObject<CCSprite> m_pFlagR;
    CCSafeObject<CCSprite> m_playerBG;
    CCSafeObject<CCNode> m_vipNode;
    CCSafeObject<CCScale9Sprite> m_expBar;

    CCSafeObject<CCNode> m_UserResBg;
    CCSafeObject<CCScale9Sprite> m_rightResBg;
    
    CCSafeObject<CCScale9Sprite> m_questRecSpr;
    CCSafeObject<CCNode> m_questRecNode;
    CCSafeObject<CCScale9Sprite> m_chatBG;
    CCSafeObject<CCNode> m_questBG;
    CCSafeObject<CCNode> m_questBG2;
    CCSafeObject<CCSprite> m_popupTitleBg1;
    CCSafeObject<CCSprite> m_popupTitleBg2;
    CCSafeObject<CCSprite> m_countryIcon;
    CCSafeObject<CCSprite> m_allianceIcon;
    CCSafeObject<CCNode> m_achNumBg;
    CCSafeObject<CCLabelIF> m_achNum;
    CCSafeObject<CCNode> m_achNode;
    CCSafeObject<CCNode> m_queueNode;
    
    CCSafeObject<CCNode> m_worldUINode;
    
    CCSafeObject<CCLabelIF> m_configSetBtnTitle;
    CCSafeObject<CCLabelIF> m_storeBtnLabel;
    CCSafeObject<CCLabelIF> m_popupTitleName;
    CCSafeObject<CCLabelIF> m_goldName;
    
    CCSafeObject<CCNode> m_armyCDContainer;
    
    CCSafeObject<CCNode> m_frameEffNode;
    
    CCSafeObject<CCLabelIFBMFont> m_power;
    CCSafeObject<CCLabelIF> m_questName;
    CCSafeObject<CCLabelIF> m_questTitle;
    CCSafeObject<CCLabelIF> m_questTipNum;

    CCSafeObject<CCNode> m_queueCell;
    CCSafeObject<CCNode> m_expansionNode;
    CCSafeObject<CCLabelIF> m_lbExpansion;
    CCSafeObject<CCControlButton> m_cdExpansion;
    CCSafeObject<CCControlButton> m_wrireMailBtn;
    CCSafeObject<CCSprite> m_sprExpansion;
    CCSafeObject<CCSprite> m_sprPack;
    CCSafeObject<CCScale9Sprite> m_cdBG;
    CCSafeObject<CCScrollView> m_scrollQueueView;
    CCSafeObject<CCNode> m_rightNode;
    CCSafeObject<CCNode> m_fireNode;
    CCSafeObject<CCNode> m_kingsGiftNode;
    CCSafeObject<CCLabelIF> m_mailBtnTimes;
    CCSafeObject<CCLabelIF> m_mailBtnTitle;
//    CCSafeObject<CCSprite> m_mailBtnTimesBG;
    CCSafeObject<CCSprite> m_mailBG;
    CCSafeObject<CCNode> m_mailBtnsNode;
    CCSafeObject<CCControlButton> m_cancelDelMailBtn;
    CCSafeObject<CCControlButton> m_delAllMailBtn;
    CCSafeObject<CCControlButton> m_delMailOpBtn;
    CCSafeObject<CCSprite> m_newFavoriteIcon;
    CCSafeObject<CCSprite> m_questTipBG;
    CCSafeObject<CCSprite> m_mailBGFlag2;
    CCSafeObject<CCSprite> m_chatNewAlert;
    CCSafeObject<CCNode> m_questIcon;
    CCSafeObject<CCNode> m_questAnimNode;
    CCSafeObject<CCScale9Sprite> m_questContextBG;
    CCSafeObject<CCScale9Sprite> m_questContextBG1;
    CCSafeObject<CCNode> m_questTipNpcNode;
    
    CCSafeObject<CCScale9Sprite> m_stamineBar;
    CCSafeObject<CCSprite> m_sprVip;
    CCSafeObject<CCSprite> m_sprVipHui;
    CCSafeObject<CCLabelIFBMFont> m_vipText;
    CCSafeObject<CCNode> m_testFeedBackNode;
    CCSafeObject<CCLabelIF> m_lblTestFeedBack;
    CCSafeObject<CCNode> m_lotteryNode;
    
    CCSafeObject<CCNode> m_allianceTipNode;
    CCSafeObject<CCNode> m_shakeGuideNode;
    
    BuildQueueCell* m_buildQueueCell1;
    BuildQueueCell* m_buildQueueCell2;
    
    //help
    CCSafeObject<CCLabelIF> m_helpNum;
    CCSafeObject<CCNode> m_helpNumBg;
    CCSafeObject<CCNode> m_helpNode;
    CCSafeObject<CCNode> m_helpNumNode;
    CCSafeObject<CCLabelIF> m_helpNumTxt;
    
    CCSafeObject<CCNode> m_powerParticleNode;
    CCSafeObject<CCNode> m_expParticleNode;
    
    CCSafeObject<CCNode> m_powerNode;
    CCSafeObject<CCNode> m_powerAndBgNode;
    CCSafeObject<CCNode> m_giftNode1;
    CCSafeObject<CCNode> m_giftNode2;
    
    CCSafeObject<CCNode> m_flyTextNode;
    CCSafeObject<CCNode> m_pic;
    
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCNode> m_limitActNode;
    CCSafeObject<CCNode> m_limitActBg;
    
    CCSafeObject<CCNode> m_skillNode;
    
    CCSafeObject<CCNode> m_serverNode;
    CCSafeObject<CCLabelIF> m_homeText;
    CCSafeObject<CCLabelIF> m_serverIdText;
    CCSafeObject<CCModelLayerColor> m_bomModelLayer;
    CCSafeObject<CCModelLayerColor> m_topModelLayer;
    CCSafeObject<CCModelLayerColor> m_midModelLayer;
    CCSafeObject<FlyCBuildView> m_flyCBView;
    
    CCSafeObject<CCControlButton> m_closeNodeBtn;
    CCSafeObject<CCLabelIF> m_moveBuildLabel;
    CCSafeObject<CCNode> m_MoveBuildMsgNode;
    CCSafeObject<CCNode> m_powerBgNode;
    
    //新玩家help Icon
    CCSafeObject<CCNode> m_newuserHelpNode;
    CCSafeObject<CCSprite> m_newuserHelpBG;
    CCSafeObject<CCSprite> m_miniIcon;
    CCSafeObject<CCSprite> m_faveIcon;
    
    //装备提示特效
    CCSafeObject<CCNode> m_equipGlowNode;
    //龙晶
    CCSafeObject<CCNode> m_LongJingNode;
    CCSafeObject<CCNode> m_sprNode;
    CCSafeObject<UIComponentLongJing> m_longJingNum;
    void onLongJingBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    CCPoint m_ctrlNodePt;
    CCPoint m_upCtrlLyPt;
    CCPoint m_questPt;
    CCPoint m_mainPt;
    CCPoint m_upPt;
    CCPoint m_messageTextPt;
    
    //Alliance
    AllianceAni* m_allianceAni1;
    CCSafeObject<CCNode> m_allianceAni;
    
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<CCSprite> m_flyArrow;
    
    CCSafeObject<CCNode> m_csNode;
    
    long oldGold;
    long oldMoney;
    long oldWood;
    long oldIron;
    long oldFood;
    long oldStone;

    float m_bloodMaxHeight;
    float m_bloodMaxWidth;
    int count;
    long perPower;
    double currentPower;
    double endPower;
    int hintType;
    int userLevelCommandCount;
    bool m_bUIShowFlag;
    bool m_CanClickTarget;
    bool isPlayingPowerAni;
    QuestInfo* m_recommandQuest;
    int goldExchangeType;
    bool m_UIQuestNodeStat;
    int m_showPower;
    vector<CCParticleBatchNode*> m_parVec;
};

#endif /* defined(__IF__UIComponent__) */
