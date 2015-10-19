//
//  GeneralsPopupView.h
//  IF
//
//  Created by ganxiaohua on 14-6-11.
//
//

#ifndef __IF__GeneralTitanPopupView__
#define __IF__GeneralTitanPopupView__

//#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "Titan.h"
//#include "DefaultTableViewDelegate.h"
//
//#define CHANGE_PIC_REMOVE "change.pic.remove"

//class ClickTipView : public CCNode{
//public:
//    CREATE_FUNC(ClickTipView);
//    void update(float time);
//    
//private:
//    bool init();
//    //    void onEnter();
//    //    void onExit();
//    CCSafeObject<CCLabelIF> m_timeText;
//    CCSafeObject<CCLabelIF> m_desText;
//};


class GeneralTitanPopupView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static GeneralTitanPopupView* create();
    void resetAttribute(CCObject* obj);
    
    CCNode* getGuideNode(string _key);
    
//    void fadeOut();
//    static void createInfoByUid(const string& uid);
    void update(float time);
//    CCNode* getGuideNode(string _key);
protected:

private:
    GeneralTitanPopupView():m_titanId(""),feedCD(0),isUpdating(false),upgradeCD(0),isUpgrading(false),queue_id(-1){}
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    void loadResource();
    void onGetPlayerInfoCallback(CCObject* obj);
//
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
//
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    bool isUpdating = false;
    
//    Vec2 m_TouchPoint;
//    bool m_TouchMoved;
    std::string m_titanId ;
//    long titanLevel = 0;
//    int needfood = 0;
//    int needexp = 0;
//    int currentExp = 0;
//    int maxManual = 0;
//    int exp = 0;
//    int titanid = 0;
//    int status = 0;
//    int feedNum = 0;
//    int feedMaxNum = 0;
//    int currentManual=0;
    float m_ProTitanExtMaxWidth;
    float m_ProTiTanAPMaxWidth;
    
    int queue_id ;
    
    
    float feedCD ;
    float upgradeCD ;
    
    
    bool isUpgrading ;
    
    bool isFoodEnough;
    
    
    TitanInfo m_titanInfo;

//
    void onSkillClick(CCObject * pSender, Control::EventType pCCControlEvent);

    void onTitanFeedClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onSpeedUpClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onCleanFeedCDClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onToolSpeedUpClick(CCObject * pSender, Control::EventType pCCControlEvent);

    void speedUpCallBack();
    
    void AccGrowthCallBack();
    
    void spdCallBack();
//    void onChangeName(CCObject * pSender, Control::EventType pCCControlEvent);
//    void onPicClick(CCObject * pSender, Control::EventType pCCControlEvent);
//    void onResetPicClick(CCObject * pSender, Control::EventType pCCControlEvent);
//    void onSkillBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
//    
//    void onAllianceClick(CCObject * pSender, CCControlEvent pCCControlEvent);
//    void onMessageClick(CCObject * pSender, CCControlEvent pCCControlEvent);
//
    void animationFunc();
//    void animationFunc1();
//    void onChangePic(CCObject* p);
//    void updatePlayerName(CCObject* p);
//    void refreshView();
//    void changePic();
//    void asyDelayLoad(CCObject* p);
//    
//    void resetRankPop(CCObject *obj = NULL);
//    
    void onRefreshEquip();
//    void onClickEquip(int site);
//    void onPlayPutOnEquip();
//    void onYesGoToBuild();
//    void onSetBagTipNode();
//    
//    void onShowEquipTip(int site);
//    
//    void addMedalParticle(string medalId);
//    //提示可打造并穿戴装备
//    void updateEquipTips(CCObject* params);
//    void equipLoopGlowCallBack();
//    CCSafeObject<CCScrollView> m_scrollView;
//    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode>  m_titanExtNode;
    CCSafeObject<CCNode> titanFeedNode;
    CCSafeObject<CCNode> titanUpingNode;
    CCSafeObject<CCNode> m_titanFeedCDNode;
    CCSafeObject<CCNode> m_titanFeedBtnNode;
    CCSafeObject<CCNode> m_CleanFeedCDBtnNode;
    CCSafeObject<CCNode> m_needFoodNode;
    CCSafeObject<CCNode> m_needGoldNode;
    CCSafeObject<CCControlButton> m_titanFeedBtn;
    CCSafeObject<CCControlButton> m_speedUpBtn;
    CCSafeObject<CCControlButton> m_toolSpeedUpBtn;
    CCSafeObject<CCControlButton> m_CleanFeedCDBtn;

    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode>m_titanPosInView;
    CCSafeObject<CCLabelIF> m_titanAPTxt;
    CCSafeObject<CCLabelIF> m_titanAPTxtPre;
    CCSafeObject<CCLabelIF> m_titanFeedStatus;
    CCSafeObject<CCLabelIF> m_feedCDGoldTxt;
    CCSafeObject<CCLabelIF> m_needGlod;//fusheng 快速成长的Label
    CCSafeObject<CCLabelIF> m_feedCDBtnTxt;
    CCSafeObject<CCLabelIF> m_titanExtTxtPre;
    CCSafeObject<CCLabelIF> m_lvLabel;
    
    CCSafeObject<CCLabelIF> m_titanAttackTxt;
    CCSafeObject<CCLabelIF> m_titanMagicTxt;
    CCSafeObject<CCLabelIF> m_Txt1;
    CCSafeObject<CCLabelIF> m_Txt2;
    CCSafeObject<CCLabelIF> m_titanExtTxt;
    
    CCSafeObject<CCLabelIF> m_needFood;
    CCSafeObject<CCLabelIF> m_currentFoodNum;
    
    CCSafeObject<CCLabelIF> m_feedCDTxt;
    CCSafeObject<CCLabelIF> m_upgradeCDTxt;
    CCSafeObject<CCLabelIF> m_titanStatusDesc;
    CCSafeObject<CCLabelIF> m_titanFeedTxt;
    

    CCSafeObject<CCSprite> m_foodStatus;
    CCSafeObject<CCSprite> m_titanDetail;
    CCSafeObject<CCSprite> m_titanSkill;
    CCSafeObject<CCSprite> m_titanUngrade;
    CCSafeObject<CCSprite> m_titanFeedBtnSprite;

    
    CCSafeObject<CCScale9Sprite> m_ProTitanExt;
    CCSafeObject<CCScale9Sprite> m_ProTiTanAP;
    
////    CCSafeObject<CCControlButton> m_skillBtn;
//    
    CCSafeObject<CCNode> m_equipNode1;
    CCSafeObject<CCNode> m_equipNode2;
    CCSafeObject<CCNode> m_equipNode3;
    CCSafeObject<CCNode> m_equipNode4;
    CCSafeObject<CCNode> m_equipNode5;
    CCSafeObject<CCNode> m_equipNode6;
    CCSafeObject<CCNode> m_equipNode7;
    CCSafeObject<CCNode> m_equipNode8;
    
    CCSafeObject<CCSprite> m_eBgSpr1;
    CCSafeObject<CCSprite> m_eBgSpr2;
    CCSafeObject<CCSprite> m_eBgSpr3;
    CCSafeObject<CCSprite> m_eBgSpr4;
    CCSafeObject<CCSprite> m_eBgSpr5;
    CCSafeObject<CCSprite> m_eBgSpr6;
    CCSafeObject<CCSprite> m_eBgSpr7;
    CCSafeObject<CCSprite> m_eBgSpr8;
    CCSafeObject<CCNode> m_particle7;
    CCSafeObject<CCNode> m_particle8;

    CCSafeObject<CCSprite> m_red1Pt;
    CCSafeObject<CCSprite> m_red2Pt;
    CCSafeObject<CCSprite> m_red3Pt;
    CCSafeObject<CCSprite> m_red4Pt;
    CCSafeObject<CCSprite> m_red5Pt;
    CCSafeObject<CCSprite> m_red6Pt;
    CCSafeObject<CCSprite> m_red7Pt;
    CCSafeObject<CCSprite> m_red8Pt;
    
    CCSafeObject<CCScale9Sprite> m_eLvBg1;
    CCSafeObject<CCScale9Sprite> m_eLvBg2;
    CCSafeObject<CCScale9Sprite> m_eLvBg3;
    CCSafeObject<CCScale9Sprite> m_eLvBg4;
    CCSafeObject<CCScale9Sprite> m_eLvBg5;
    CCSafeObject<CCScale9Sprite> m_eLvBg6;
    CCSafeObject<CCScale9Sprite> m_eLvBg7;
    CCSafeObject<CCScale9Sprite> m_eLvBg8;
    
    CCSafeObject<CCLabelIF> m_equipLabel1;
    CCSafeObject<CCLabelIF> m_equipLabel2;
    CCSafeObject<CCLabelIF> m_equipLabel3;
    CCSafeObject<CCLabelIF> m_equipLabel4;
    CCSafeObject<CCLabelIF> m_equipLabel5;
    CCSafeObject<CCLabelIF> m_equipLabel6;
    CCSafeObject<CCLabelIF> m_equipLabel7;
    CCSafeObject<CCLabelIF> m_equipLabel8;
    
//
//    CCSafeObject<CCNode> m_icon;
//    CCSafeObject<CCNode> m_touchNode;
////    CCSafeObject<CCScale9Sprite> m_picCover;
//    CCSafeObject<CCScale9Sprite> m_stamineBG;
//    CCSafeObject<Label> m_nameTxt;
//    CCSafeObject<CCLabelIF> m_expTxt;
//    CCSafeObject<CCLabelIF> m_pointText;
//    CCSafeObject<CCScale9Sprite> m_expBar;
//    CCSafeObject<CCNode> m_bustPic;
//    CCSafeObject<CCSprite> m_pic;
//    CCSafeObject<CCSprite> m_stamineIcon;
//    CCSafeObject<CCNode> m_touchLayer;
//    CCSafeObject<CCNode> m_bNode;
    CCSafeObject<CCNode> m_bottomNode;
//    CCSafeObject<CCNode> m_particleNode;
//    CCSafeObject<CCSprite> m_leftBg;
//    CCSafeObject<CCSprite> m_rightBg;
//    CCSafeObject<CCControlButton> m_renameBtn;
//    CCSafeObject<CCControlButton> m_picBtn;
////    CCSafeObject<CCNode> m_buttonNode;
//    CCSafeObject<CCNode> m_nameTouch;
////    CCSafeObject<CCLabelIF> m_bTxt1;
////    CCSafeObject<CCLabelIF> m_bTxt2;
//    CCSafeObject<CCLabelIF> m_bTxt3;
////    CCSafeObject<CCLabelIF> m_bTxt4;
//    CCSafeObject<CCLabelIF> m_bTxt5;
//    CCSafeObject<CCLabelIF> m_bTxt6;
//
//    CCSafeObject<CCNode> m_clickNode1;
//    CCSafeObject<CCNode> m_clickNode2;
//    CCSafeObject<CCNode> m_clickNode3;
    CCSafeObject<CCNode> m_clickNode4;
//    CCSafeObject<CCNode> m_clickNode5;
//    CCSafeObject<CCNode> m_clickNode6;
//    CCSafeObject<CCNode> m_clickNode7;
//
//    CCSafeObject<CCNode> m_headIcon;
//    CCSafeObject<CCNode> m_scrollContainer;
//    
////    CCSafeObject<CCTableView> m_tabView;
//    CCSafeObject<CCArray> m_data;
//    CCSafeObject<CCNode> m_resetNode;
//    CCSafeObject<CCControlButton> m_resetPicBtn;
//    CCSafeObject<CCLabelIF> m_btnText;
//    CCSafeObject<CCLabelIF> m_goldNum;
//    CCSafeObject<CCSprite> m_goldIcon;
//    
//    CCSafeObject<CCLabelIF> m_stamineText;
////    CCSafeObject<CCLabelIF> m_timeText;
//    CCSafeObject<ClickTipView> m_tip;
//    CCSafeObject<CCScale9Sprite> m_staminePro;
//    CCSafeObject<CCNode> m_skillNumNode;
//    
//    CCSafeObject<CCNode> m_bagTipNode;
//    CCSafeObject<CCLabelIF> m_bagTipNum;
//    
//    CCSafeObject<CCNode> m_selfInfoNode;
//    CCSafeObject<CCNode> m_otherNode;
//    CCSafeObject<CCNode> m_medalNode;
//    CCSafeObject<CCScale9Sprite> m_expOthBar;
//    CCSafeObject<CCLabelIF> m_expOthTxt;
//    CCSafeObject<CCLabelIF> m_lvOthTxt;
//    CCSafeObject<CCLabelIF> m_medalLable;
//    CCSafeObject<CCControlButton> m_msgBtn;
//    CCSafeObject<CCControlButton> m_btnAlliance;
//    CCSafeObject<CCNode> m_medalParticleNode;
//    CCSafeObject<CCNode> m_starNode;
//    CCSafeObject<CCNode> m_speStarNode;
//    CCSafeObject<CCLabelIF> m_speStarTTF;
//    CCSafeObject<CCLabelIF> m_noMedalLable;
//    
//    CCSafeObject<CCSprite> m_lockL;
//    CCSafeObject<CCSprite> m_lockR;
//    
//    CCSafeObject<CCNode> m_equipTipNode;
//    CCSafeObject<CCLabelIF> m_equipTipLabel;
//    
//    CCSafeObject<CCScale9Sprite> m_button;
//    
    CCSafeObject<PlayerInfo> m_info;
//
    CCSafeObject<CCNode> m_receiveGlow;
//    std::vector<int> glowVect;
//    int curGlow;
//    
//    int m_generalIndex;
//    int m_x;
//    int m_count;
//    
//    int m_startY;
    map<int, string> m_EquipMap;
    map<int, int> m_EquipSiteMap;
//
//    bool m_addPopup;
//    
    string m_achievePro;
    string m_medalCntStr;
    string m_medalIconId;
    string m_playerUid;
    bool isOpenLongJing;//龙晶装备开关1
};

//class ChangePicPopupView: public PopupBaseView
//,public CCBSelectorResolver
//,public CCBMemberVariableAssigner
////, public CCTouchDelegate
//,public CCTableViewDataSource
//,public DefaultTableViewDelegate
//{
//public:
//    static ChangePicPopupView* create();
//    
//protected:
//    // scrollview delegate
//    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
//    virtual CCSize cellSizeForTable(CCTableView *table);
//    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
//    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
//    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
//    virtual void scrollViewDidScroll(CCScrollView* view);
//    
//private:
//    virtual void onEnter();
//    virtual void onExit();
//    virtual bool init();
//    
//    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
//	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
//	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
//    
//    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    
//    void refreshView();
//    void asyDelayLoad(CCObject* p);
//    void onResetPicClick(CCObject * pSender, Control::EventType pCCControlEvent);
//    void onSetUserPicClick(CCObject * pSender, Control::EventType pCCControlEvent);
//    void changePic();
//    void onOKCostGold();
//    bool haveUpload();
//    void onGetPicBack(cocos2d::CCObject *pObj);
//    void onGameTick(float time);
//    
//    CCSafeObject<CCTableView> m_tabView;
//    CCSafeObject<CCArray> m_data;
//    
//    CCSafeObject<CCNode> m_scrollContainer;
//    CCSafeObject<CCControlButton> m_resetPicBtn;
//    CCSafeObject<CCControlButton> m_setUserPicBtn;
//    CCSafeObject<CCLabelIF> m_btnText;
//    CCSafeObject<CCLabelIF> m_goldNum;
//    CCSafeObject<CCSprite> m_goldIcon;
//    CCSafeObject<CCSprite> m_pageFlag;
//    CCSafeObject<CCLayerColor> m_bgColor;
//    CCSafeObject<CCLabelIF> m_timeLb;
//    
//    
//    void generateData();
//    void setButtonState(int offX);
//    int m_generalIndex;
//    int m_x;
//    int m_count;
//};
//
//class GeneralPicCell :
//public CCBSelectorResolver
//,public CCBMemberVariableAssigner
//,public CCTableViewCell
//{
//public:
//    static GeneralPicCell *create(std::string generalId);
//    GeneralPicCell(std::string generalId) : m_generalId(generalId) {};
//    void setData(std::string generalId);
//private:
//    void refreshView();
//    bool init();
//    
//    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
//	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
//	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
//
//    CCSafeObject<CCNode> m_picNode;
//    
//    virtual void onEnter();
//    virtual void onExit();
//    std::string m_generalId;
//};
//
//class GeneralCustomPicView : public PopupBaseView
//,public CCBSelectorResolver
//,public CCBMemberVariableAssigner
////,public CCTouchDelegate
//{
//public:
//    static GeneralCustomPicView* create();
//    
//private:
//    virtual void onEnter();
//    virtual void onExit();
//    virtual bool init();
//    
//    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
//    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
//    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
//    
//    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    
//    void onGetCustomPicClick(CCObject * pSender, Control::EventType pCCControlEvent);
//    void onGetPicBack(CCObject* pObj);
//    void doGetPicAction();
//    
//private:
//    CCSafeObject<CCNode> m_touchNode;
//    CCSafeObject<CCNode> m_resetNode;
//    CCSafeObject<CCControlButton> m_pictureBtn;
//    CCSafeObject<CCControlButton> m_cemaraBtn;
//    CCSafeObject<CCLayerColor> m_bgColor;
//    CCSafeObject<CCLabelIF> m_alertTTF;
//    
//    int m_selectIndex;
//    
//};

#endif /* defined(__IF__GeneralTitanPopupView__) */
