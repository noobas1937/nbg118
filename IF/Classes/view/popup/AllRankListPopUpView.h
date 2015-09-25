//
//  AllRankListPopUpView.h
//  IF
//
//  Created by lifangkai on 14-9-16.
//
//

#ifndef __IF__AllRankListPopUpView__
#define __IF__AllRankListPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCBExtension.h"
#include "CheckBox.h"
#include "CCMultiColTableView.h"

class AllRankListPopUpView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
{
public:
    static AllRankListPopUpView* create();
    ~AllRankListPopUpView(){};
    
protected:
    
private:
    AllRankListPopUpView(){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onGameCenterClick(CCObject * pSender, Control::EventType pCCControlEvent);

    void refresh();
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_IOSNode;
    CCSafeObject<CCControlButton> m_gameCenterBtn;
    CCSafeObject<CCLabelIFTTF> m_hintText;
    CCSafeObject<CCSprite>  m_facebookIcon;
    void addIosShowBtn();
};

class AllRankListCell : public CCNode
, public CCBSelectorResolver
  
, public CCBMemberVariableAssigner
{
public:
    AllRankListCell(){
    }
    AllRankListCell( int parentType){
        m_parentType = parentType;
    }
    static AllRankListCell* create( int parentType);
    void setParentScrollView(CCScrollView* parentScrollView);
private:
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onMoreBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);

    

    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCControlButton> m_moreBtn;
    CCSafeObject<CCNode> m_nodeContent;
    CCSafeObject<CCNode> m_top1;
    CCSafeObject<CCNode> m_top2;
    CCSafeObject<CCNode> m_bgContainer;
    CCSafeObject<CCScale9Sprite>  m_bg1;
    CCSafeObject<CCScale9Sprite>  m_bg2;

    CCSafeObject<CCScrollView> m_scrollView;
    

    CCPoint m_startPoint;
    int m_parentType;
};

class AllRankListSmallCell :
public Layer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
  
{
public:
    static AllRankListSmallCell* create(int type);
    AllRankListSmallCell(int type):m_type(type){};
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCScale9Sprite> m_cellBG;
    CCSafeObject<CCScale9Sprite> m_cellBG2;
    int m_type;
    CCPoint m_startPoint;
};

#endif /* defined(__IF__AllRankListPopUpView__) */
