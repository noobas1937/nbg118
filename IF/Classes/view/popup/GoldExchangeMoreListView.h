//
//  GoldExchangeMoreListView.h
//  IF
//
//  Created by lifangkai on 14-9-10.
//
//

#ifndef __IF__GoldExchangeMoreListView__
#define __IF__GoldExchangeMoreListView__


#include "CommonInclude.h"
#include "CCBExtension.h"
#include "ArcPopupBaseView.h"
#include "CCLabelIF.h"

class GoldExchangeMoreListCell;

class GoldExchangeMoreListView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static GoldExchangeMoreListView* create(int itemId);
    GoldExchangeMoreListView():m_itemId(0){};
    virtual ~GoldExchangeMoreListView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int itemId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void updateInfo();
    
   // void onClickCollectBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelBMFont> m_goldNumText;
    CCSafeObject<CCLabelIF> m_oldPriceLabel;
    CCSafeObject<CCLabelIF> m_newPriceLabel;
    CCSafeObject<CCLabelIF> m_getLabel;
    CCSafeObject<CCControlButton> m_costBtn;
    CCSafeObject<CCScale9Sprite>m_buildBG;
    CCSafeObject<CCSprite> m_soleOutSpr;
    CCSafeObject<CCSprite> m_soleOutSpr1;
    CCSafeObject<CCSprite> m_arrowSpr;
    CCSafeObject<CCNode> m_animNode;
    
    int m_itemId;
    CCSafeObject<CCScrollView> m_scrollView;
};

class GoldExchangeMoreListCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static GoldExchangeMoreListCell* create(int itemId, int num,int type);
    GoldExchangeMoreListCell(){};
private:
    bool init(int itemId, int num,int type);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_iconNode;
};

#endif /* defined(__IF__GoldExchangeMoreListView__) */
