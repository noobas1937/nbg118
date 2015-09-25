//
//  MainCityView.h
//  IF
//
//  Created by fubin on 14-2-26.
//
//

#ifndef IF_MainCityView_h
#define IF_MainCityView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"

class ResInfoCell;

class MainCityView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static MainCityView* create(int buildId);
    MainCityView():m_buildId(0),m_dataCnt(0),m_isFree(false),m_isFull(true){};
    virtual ~MainCityView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int buildId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickAddDefense(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent);
    void onMoreInfoClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onDeleteClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickUpBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onUpdateDefenseCallback(cocos2d::CCObject *obj);
    void updateInfo();
    
    void onEnterFrame(float dt);
    void onYesCancel();
    
//    CCSafeObject<CCControlButton> m_upBtn;
//    CCSafeObject<CCControlButton> m_instantBtn;
    
    CCSafeObject<CCNode> m_infoList;
//    CCSafeObject<CCNode> m_picNode;
//    CCSafeObject<CCLabelIF> m_msg1Label;
//    CCSafeObject<CCLabelIF> m_msg2Label;
//    CCSafeObject<CCLabelIF> m_barLabel;
//    CCSafeObject<CCNode> m_upNode;
    
//    CCSafeObject<CCNode> m_inBtnNode;
//    CCSafeObject<CCLabelIF> m_btnMsgLabel;
//    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCScale9Sprite> m_bar;
//    CCSafeObject<CCControlButton> m_moreInfoBtn;
    
    CCSafeObject<CCNode> m_defArmyNode;
    CCSafeObject<CCLabelIF> m_nameDefLabel;
    CCSafeObject<CCControlButton> m_defBtn;
    CCSafeObject<CCLabelIF> m_defLabel;
    CCSafeObject<CCLabelIF> m_fireLabel;
    CCSafeObject<CCScale9Sprite> m_defbar;
    CCSafeObject<CCLabelIF> m_defbarLabel;
    
    CCSafeObject<CCScrollView> m_scrollView;
//    CCSafeObject<CCSprite> pic;
    
    int m_buildId;
    int m_dataCnt;
    bool m_isFree;
    bool m_isFull;
};

class ResInfoCell :
public Layer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
{
public:
    static ResInfoCell* create(int type, int buildId);
    ResInfoCell():m_type(0),m_buildId(0),m_curNum(-1),m_startY(0){};
    void setTouchNode(CCNode* touchNode);
    void updateInfo(CCObject* params);
private:
    bool init(int type, int buildId);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickMoreBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
//    CCSafeObject<CCControlButton> m_moreBtn;
//    CCSafeObject<CCLabelIF> m_nameLabel;
//    CCSafeObject<CCLabelIF> m_incomeLabel;
//    CCSafeObject<CCLabelIF> m_numLabel;
//    CCSafeObject<CCLabelIF> m_stroageLabel;
    CCSafeObject<CCNode> m_iconNode;
    
    CCSafeObject<CCNode> m_info1Node;
    CCSafeObject<CCNode> m_info2Node;
//    CCSafeObject<CCNode> m_info3Node;
//    CCSafeObject<CCNode> m_info4Node;
    
    CCSafeObject<CCNode> m_mianNode;
//    CCSafeObject<CCLabelIF> m_capLabel;
//    CCSafeObject<CCLabelIF> m_capNumLabel;
    
    CCSafeObject<CCLabelIF> m_info1TitleLabel;
    CCSafeObject<CCLabelIF> m_info2TitleLabel;
    CCSafeObject<CCLabelIF> m_info1Label;
    CCSafeObject<CCLabelIF> m_info2Label;
    CCSafeObject<CCLabelIF> m_info3Label;
    CCSafeObject<CCLabelIF> m_info4Label;
    CCSafeObject<CCLabelIF> m_info5Label;
    CCSafeObject<CCNode> m_lockNode;
    CCSafeObject<CCLabelIF> m_lockNameLabel;
    CCSafeObject<CCLabelIF> m_lockInfoLabel;
    
    CCNode* m_touchNode;
    
    int m_type;
    int m_buildId;
    int m_curNum;
    int m_startY;
};

#endif
