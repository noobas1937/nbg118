//
//  ProductionSoldiersView.h
//  IF
//
//  Created by ganxiaohua on 14-5-16.
//
//

#ifndef __IF__ProductionSoldiersView__
#define __IF__ProductionSoldiersView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "PopupBaseView.h"
#include "ArcScrollView.h"
#include "CCSliderBar.h"
#include "CCClipNode.h"
#include "CCGallery.h"

class ProductionSoldiersView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCEditBoxDelegate
//,public CCTouchDelegate
{
public:
    static ProductionSoldiersView* create(int buildingId=-1);
    ProductionSoldiersView(int buildingId);
    ~ProductionSoldiersView();
    virtual void editBoxReturn(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    CCNode* getGuideNode(string _key);
    virtual void update(float time);
protected:
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void refreshGame(CCObject * pSender);
    void startRefreshGame(float time=0);
    void refreshTest(CCObject * pSender);
    
    void immediatelyHarvestFinish(CCObject* param = NULL);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void arcButtonClick();
    void onImmediateBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTrainBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onInfoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onFortHelpClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void moveSlider(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAddClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onReduceClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClearCDClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void cmdStringToNormal(std::string &text);
    int getTrainMax();
    int getTrainTime(int num);
    void refresh(CCObject* p = NULL);
    void refreshResource(CCObject* p = NULL);
    void refreshTrainNumText(int num);
    void train(int type=0);
    bool checkEmptyQuene();
    void gotoCDPanel(CCObject* p);
    void maxInstantCallBack();
    void instantCallBack();
    void addSoldierIcon();
    void confirmCallBack();
    int getMaxFort();
    void immediatelyFinishHandle(CCObject* p);
    void AsyLoadRes(CCObject* p);
    void AsyLoadRes2(CCObject* p);
    int getFortResourceTrainMax();
    int getFortRecommandCount();
    int getFortCountByHour(int hourCount);
    void writeLastGoldBuyCount(int count);
    int getLastGoldBuyCount(int defCount);
    
    
    void ClearCD();
    
    ArmyInfo* getCurArmy();
    
    CCSafeObject<CCSliderBar> m_slider;
    CCSafeObject<ArcScrollView> m_arcScroll;
    CCSafeObject<CCGallery> m_arcGallery;
    CCSafeObject<CCControlButton> m_immediateBtn;
    CCSafeObject<CCControlButton> m_trainBtn;
    CCSafeObject<CCControlButton> m_infoBtn;
    CCSafeObject<CCNode> m_arcNode;
    CCSafeObject<CCNode> m_resNode;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_numValueTxt;
    CCSafeObject<CCLabelIF> m_woodTxt;
    CCSafeObject<CCLabelIF> m_useWoodTxt;
    CCSafeObject<CCLabelIF> m_stoneTxt;
    CCSafeObject<CCLabelIF> m_useStoneTxt;
    CCSafeObject<CCLabelIF> m_ironTxt;
    CCSafeObject<CCLabelIF> m_useIronTxt;
    CCSafeObject<CCLabelIF> m_foodTxt;
    CCSafeObject<CCLabelIF> m_useFoodTxt;
    CCSafeObject<CCLabelIF> m_moneyTxt;
    CCSafeObject<CCLabelIF> m_useMoneyTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIF> m_btnTitle1;
    CCSafeObject<CCLabelIF> m_btnUseGoldTxt;
    CCSafeObject<CCLabelIF> m_btnTitle2;
    CCSafeObject<CCNode> m_useEditNode;
    CCSafeObject<CCNode> m_resBGNode;
    CCSafeObject<CCEditBox> m_useEditTxt;
//    CCSafeObject<ArmyInfo> m_info;
//    ArmyInfo* m_info;
//    CCSafeObject<CCArray> m_armys;
    
    string m_armyId;
    vector<string> m_armyIds;
    
    
    CCSafeObject<CCScale9Sprite> m_colorBg;
    CCSafeObject<CCNode> m_soldierNode;
    CCSafeObject<CCLabelIF> m_lockTxt;
    CCSafeObject<CCNode> m_makeResNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_soldierIconNode;
    CCSafeObject<CCScale9Sprite> m_soldierBg;
    CCSafeObject<CCLabelIF> m_fortLimitTxt;
    CCSafeObject<CCNode> m_woodNode;
    CCSafeObject<CCNode> m_foodNode;
    CCSafeObject<CCNode> m_ironNode;
    CCSafeObject<CCNode> m_stoneNode;
    CCSafeObject<CCNode> m_sliderNode;
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCControlButton> m_fortHelpBtn;
    
    CCSafeObject<CCNode> m_cdBarNode;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCScale9Sprite> m_progrossBar;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCControlButton> m_clearCDBtn;
    CCSafeObject<CCClipNode>  m_barClipNode;
    CCSafeObject<CCLabelIF> m_cdBtnTxt;
    CCSafeObject<CCLabelIF> m_cdGoldTxt;
    
    CCSafeObject<CCSprite> m_newIcon;
    CCSafeObject<CCNode> m_soldierLight;
    int m_pos;
    int m_resIndex;
    int m_buildingId;
    int m_buildingLevel;
    int m_trainSoldier;
    bool m_isFort;
    std::string m_buildingName;
    bool m_notEnoughRes;
    bool m_invalideSlider;//
    bool haveClick;
    int m_qid;
    int m_tmpGold;
    WaitInterface* m_waitInterface;
    
    bool m_isWaitingSeverRes;
    bool m_isShowRefresh;
};
#endif /* defined(__IF__ProductionSoldiersView__) */
