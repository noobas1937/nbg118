//
//  NewPlayerQianDaoView.h
//  IF
//
//  Created by 付彬 on 15/7/2.
//
//

#ifndef __IF__NewPlayerQianDaoView__
#define __IF__NewPlayerQianDaoView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "TimeExchangeView.h"
#include "DailyRwdView.h"

class NewPlayerQianDaoView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static NewPlayerQianDaoView* create();
    NewPlayerQianDaoView(){};
    virtual ~NewPlayerQianDaoView();
    CCNode* getGuideNode(string _key);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickRwdBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickTipBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void retData(CCObject* param);
    void refreshData();
    
    void showHaveGetRD(int day);
    void clickShowRD(int day);
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCControlButton> m_rwdBtn;
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCSprite> m_leftSpr;
    CCSafeObject<CCSprite> m_rightSpr;
    
    CCSafeObject<CCNode> m_rd1Node;
    CCSafeObject<CCNode> m_rd2Node;
    CCSafeObject<CCNode> m_rd3Node;
    CCSafeObject<CCNode> m_rd4Node;
    CCSafeObject<CCNode> m_rd5Node;
    CCSafeObject<CCNode> m_rd6Node;
    CCSafeObject<CCNode> m_rd7Node;
    
    CCSafeObject<CCSprite> m_del1Spr;
    CCSafeObject<CCSprite> m_del2Spr;
    CCSafeObject<CCSprite> m_del3Spr;
    CCSafeObject<CCSprite> m_del4Spr;
    CCSafeObject<CCSprite> m_del5Spr;
    CCSafeObject<CCSprite> m_del6Spr;
    CCSafeObject<CCSprite> m_del7Spr;
    
    CCSafeObject<CCLabelIF> m_rd1Label;
    CCSafeObject<CCLabelIF> m_rd2Label;
    CCSafeObject<CCLabelIF> m_rd3Label;
    CCSafeObject<CCLabelIF> m_rd4Label;
    CCSafeObject<CCLabelIF> m_rd5Label;
    CCSafeObject<CCLabelIF> m_rd6Label;
    CCSafeObject<CCLabelIF> m_rd7Label;
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_vipLabel;
    CCSafeObject<CCLabelIF> m_tipTxt1;
    CCSafeObject<CCLabelIF> m_tipTxt2;
    
    CCSafeObject<CCScrollView> m_scrollView;
    
    WaitInterface* m_waitInterface;
    int m_type;
    int m_panelType;
    int m_startY;
    string m_guideKey;
};



class NewPlayerQianDaoPopView: public Layer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static NewPlayerQianDaoPopView* create();
    NewPlayerQianDaoPopView(){ignoreAnchorPointForPosition(false);setAnchorPoint(Vec2(0,0));};
    virtual ~NewPlayerQianDaoPopView();
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickRwdBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
//    void onClickTipBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void retData(CCObject* param);
    void refreshData();
    
    void showHaveGetRD(int day);
    void clickShowRD(int day);
    
    CCSafeObject<CCNode> m_touchNode;
//    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCControlButton> m_rwdBtn;
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCSprite> m_leftSpr;
    CCSafeObject<CCSprite> m_rightSpr;
    
    CCSafeObject<CCNode> m_rd1Node;
    CCSafeObject<CCNode> m_rd2Node;
    CCSafeObject<CCNode> m_rd3Node;
    CCSafeObject<CCNode> m_rd4Node;
    CCSafeObject<CCNode> m_rd5Node;
    CCSafeObject<CCNode> m_rd6Node;
    CCSafeObject<CCNode> m_rd7Node;
    
    CCSafeObject<CCSprite> m_del1Spr;
    CCSafeObject<CCSprite> m_del2Spr;
    CCSafeObject<CCSprite> m_del3Spr;
    CCSafeObject<CCSprite> m_del4Spr;
    CCSafeObject<CCSprite> m_del5Spr;
    CCSafeObject<CCSprite> m_del6Spr;
    CCSafeObject<CCSprite> m_del7Spr;
    
    CCSafeObject<CCLabelIF> m_rd1Label;
    CCSafeObject<CCLabelIF> m_rd2Label;
    CCSafeObject<CCLabelIF> m_rd3Label;
    CCSafeObject<CCLabelIF> m_rd4Label;
    CCSafeObject<CCLabelIF> m_rd5Label;
    CCSafeObject<CCLabelIF> m_rd6Label;
    CCSafeObject<CCLabelIF> m_rd7Label;
    
    CCSafeObject<CCLabelIF> m_titleLabel;
//    CCSafeObject<CCLabelIF> m_vipLabel;
//    CCSafeObject<CCLabelIF> m_tipTxt1;
//    CCSafeObject<CCLabelIF> m_tipTxt2;
    
//    CCSafeObject<CCScrollView> m_scrollView;
    
    WaitInterface* m_waitInterface;
    int m_type;
    int m_panelType;
    int m_startY;
};
#endif /* defined(__IF__NewPlayerQianDaoView__) */
