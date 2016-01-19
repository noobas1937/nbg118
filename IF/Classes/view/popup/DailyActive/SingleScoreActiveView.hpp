//
//  SingleScoreActiveView.hpp
//  IF
//
//  Created by 付彬 on 15/11/16.
//
//

#ifndef SingleScoreActiveView_hpp
#define SingleScoreActiveView_hpp

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "TimeExchangeView.h"
#include "DailyRwdView.h"
#include "ActivityEventObj.h"
#include "ActivityInfo.h"

class SingleScoreActiveView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static SingleScoreActiveView* create(ActivityEventObj* info, bool isInner=false);
    SingleScoreActiveView(){};
    virtual ~SingleScoreActiveView();
    CCNode* getGuideNode(string _key);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(ActivityEventObj* info, bool isInner);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void OnTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void retData(CCObject* param);
    void retRankData(CCObject* param);
    void refreshData();
    void clickShowRD(int day);
    void onGameTick(float time);
    
    CCSafeObject<CCNode> m_topNode;
    CCSafeObject<CCNode> m_midNode;
    
    CCSafeObject<CCNode> m_tabList;
    CCSafeObject<CCNode> m_infoList;
    
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCNode> m_bgNode;
//    CCSafeObject<CCNode> m_particleNode;
    
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCLabelIF> m_subTitleLabel;
    CCSafeObject<CCLabelIF> m_sub2TitleLabel;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCLabelIF> m_ttLabel;
    CCSafeObject<CCLabelIF> m_scoreTLabel;
    CCSafeObject<CCLabelIF> m_scoreLabel;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCScale9Sprite> m_bar;
    CCSafeObject<CCScale9Sprite> m_bar2;
    
    CCSafeObject<CCSprite> m_scoreIcon;
    
    CCSafeObject<CCSprite> m_box1OpenIcon;
    
    CCSafeObject<CCSprite> m_box2OpenIcon;
    
    CCSafeObject<CCSprite> m_box3OpenIcon;
    
    CCSafeObject<CCLabelIF> m_cup1Label;
    CCSafeObject<CCLabelIF> m_cup2Label;
    CCSafeObject<CCLabelIF> m_cup3Label;
    
    CCSafeObject<CCNode> m_touch1Node;
    CCSafeObject<CCNode> m_touch2Node;
    CCSafeObject<CCNode> m_touch3Node;
    CCSafeObject<CCNode> m_touch4Node;
    
    CCSafeObject<CCSprite> m_arrowSpr;
    CCSafeObject<CCLabelIF> m_pt1Label;
    CCSafeObject<CCLabelIF> m_pt2Label;
    CCSafeObject<CCLabelIF> m_pt3Label;
    CCSafeObject<CCLabelIF> m_pt4Label;
    
    CCSafeObject<CCScrollView> m_topScrollView;
    CCSafeObject<CCScrollView> m_scrollView;
    
    WaitInterface* m_waitInterface;
    int m_type;
    int m_panelType;
    int m_startY;
    string m_guideKey;
    ActivityEventObj* m_actEventObj;
    ActivityInfo* m_actInfo;
};

class SingleScoreCell: public Node
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static SingleScoreCell* create(string scoreId);
    SingleScoreCell():m_scoreId(""){};
    virtual ~SingleScoreCell(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(string scoreId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_scoreLabel;
    
    string m_scoreId;
};


class SingleScoreRwdPopView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static SingleScoreRwdPopView* create(CCArray* data, int score, int price, bool hasGet, string title);
    SingleScoreRwdPopView(){ignoreAnchorPointForPosition(false);setAnchorPoint(Vec2(0,0));};
    virtual ~SingleScoreRwdPopView(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(CCArray* data, int score, int price, bool hasGet, string title);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void refreshData();
    
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCNode> m_topNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_mainNode;
    
    CCSafeObject<CCScale9Sprite> m_bg;
    
    CCSafeObject<CCNode> m_particleNode;
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_info1Label;
    CCSafeObject<CCLabelIF> m_info2Label;
    CCSafeObject<CCLabelIF> m_info3Label;
    CCSafeObject<CCLabelIF> m_rwdLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCLabelIF> m_stateLabel;
    CCSafeObject<CCLabelIF> m_priceLabel;
    CCSafeObject<CCLabelIF> m_noticeLabel;
    
    CCSafeObject<CCSprite> m_scoreIcon;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCArray* m_data;
    
    int m_type;
    int m_panelType;
    int m_startY;
    
    int m_score;
    int m_price;
    bool m_bHasGet;
};

class SingleScoreRwdCell: public Node
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static SingleScoreRwdCell* create(CCDictionary* dict, bool isGet);
    SingleScoreRwdCell(){};
    virtual ~SingleScoreRwdCell(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(CCDictionary* dict, bool isGet);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void setData(int itemId,string nameStr,int countStr,int itemType);
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCLabelIF> m_stateLabel;
    
    CCDictionary* m_dict;
    bool m_hasGet;
};

#endif /* SingleScoreActiveView_hpp */
