//
//  EquipSiteView.h
//  IF
//
//  Created by fubin on 14-12-29.
//
//

#ifndef __IF__EquipSiteView__
#define __IF__EquipSiteView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCClipNode.h"

class EquipSiteView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static EquipSiteView* create();
    CCNode* getGuideNode(string _key);
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickSite0Btn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSite1Btn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSite2Btn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSite3Btn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSite4Btn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSite5Btn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClearCDClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onRefreshStat();
    void onEnterFrame(float dt);
    
    void onPlayClick(int site);
    void onPlayClickEnd();
    
    void onPlaySound(float dt);
    
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_barNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCScale9Sprite> m_progrossBar;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCControlButton> m_btn;
    
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_guideNode;
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCControlButton> m_site0Btn;
    CCSafeObject<CCControlButton> m_site1Btn;
    CCSafeObject<CCControlButton> m_site2Btn;
    CCSafeObject<CCControlButton> m_site3Btn;
    CCSafeObject<CCControlButton> m_site4Btn;
    CCSafeObject<CCControlButton> m_site5Btn;
    
    CCSafeObject<CCSprite> m_book0Spr;
    CCSafeObject<CCSprite> m_book1Spr;
    CCSafeObject<CCSprite> m_book2Spr;
    CCSafeObject<CCSprite> m_book3Spr;
    CCSafeObject<CCSprite> m_book4Spr;
    CCSafeObject<CCSprite> m_book5Spr;
    
    CCSafeObject<CCLabelIF> m_equip0Label;
    CCSafeObject<CCLabelIF> m_equip1Label;
    CCSafeObject<CCLabelIF> m_equip2Label;
    CCSafeObject<CCLabelIF> m_equip3Label;
    CCSafeObject<CCLabelIF> m_equip4Label;
    CCSafeObject<CCLabelIF> m_equip5Label;
    
    CCSafeObject<CCSprite> m_flag0Spr;
    CCSafeObject<CCSprite> m_flag1Spr;
    CCSafeObject<CCSprite> m_flag2Spr;
    CCSafeObject<CCSprite> m_flag3Spr;
    CCSafeObject<CCSprite> m_flag4Spr;
    CCSafeObject<CCSprite> m_flag5Spr;
    
    CCSafeObject<CCNode> m_site0Node;
    CCSafeObject<CCNode> m_site1Node;
    CCSafeObject<CCNode> m_site2Node;
    CCSafeObject<CCNode> m_site3Node;
    CCSafeObject<CCNode> m_site4Node;
    CCSafeObject<CCNode> m_site5Node;
    
    CCSafeObject<CCNode> m_aniNode;
    
    CCSafeObject<CCLabelIF> m_msgLabel;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCClipNode>  m_barClipNode;
    
    CCSafeObject<CCSprite> m_leftBg;
    CCSafeObject<CCSprite> m_rightBg;
    
    CCSafeObject<CCNode> m_bookParent;
    
    int m_weapType;
    int m_mateType;
    int m_qid;
    int m_startY;
    int m_itemId;
    int m_curIdx;
    int m_willIdx;
    int m_moveSum;
    
    int m_bookDefineOrder[6];
    float m_bookDefineColor[6];
    float m_bookDefineScale[6];
    CCPoint m_bookDefinePos[6];
    
    int m_bookNodesMove2Posid[6]; //存放目标位置 [nodeid] = posid;
    
    int m_onAction = 0;
    CCPoint m_startTouchPoint;
    CCPoint m_nowTouchPoint;
    float m_movedPercent;
    float m_totalLength;
    
    bool m_CanMove;
    bool m_isInitEnd;
    
    int node2Nodeid(CCNode* node);
    CCControlButton* nodeid2Button(int nodeid);
    CCSprite* nodeid2Sprite(int nodeid);
    void moveend(CCNode *node);
    bool move(int distance);
    
    void startToMoveInit();
    void moveToIdx(float dt);
    void initToEnd(float dt);
};

#endif /* defined(__IF__EquipSiteView__) */
