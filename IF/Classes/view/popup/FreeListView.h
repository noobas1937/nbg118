//
//  FreeListView.h
//  IF
//
//  Created by fubin on 14-8-29.
//
//

#ifndef IF_FreeListView_h
#define IF_FreeListView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "ArcPopupBaseView.h"
#include "CCLabelIF.h"

class RechargeLargeCell;

class FreeListView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static FreeListView* create(int itemId);
    FreeListView():m_itemId(0){};
    virtual ~FreeListView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int itemId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void updateInfo();
    
    void onClickCollectBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelBMFont> m_goldNumText;
    CCSafeObject<CCLabelBMFont> m_goldTitleText;
    
    int m_itemId;
    CCSafeObject<CCScrollView> m_scrollView;
};

class RechargeLargeCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static RechargeLargeCell* create(int itemId, int num, int type);
    RechargeLargeCell(){};
private:
    bool init(int itemId, int num, int type);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_iconNode;
};
#endif
