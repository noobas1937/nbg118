//
//  GoldFirstPayView.h
//  IF
//
//  Created by lifangkai on 14-10-17.
//
//

#ifndef __IF__GoldFirstPayView__
#define __IF__GoldFirstPayView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"

class GoldFirstPayView : public Layer
, public CCBSelectorResolver
  
, public CCBMemberVariableAssigner
{
public:
    GoldFirstPayView(){
    }
    GoldFirstPayView(int parentType){
        m_parentType = parentType;
        m_parentScrollView = NULL;
    }
    static GoldFirstPayView* create( int parentType);
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
    
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_moreLabel;
    CCSafeObject<CCNode> m_listNode;

    CCSafeObject<CCNode> m_touchMoreNode;

    
    CCSafeObject<CCScrollView> m_scrollView;
    
    CCPoint m_startPoint;
    //GoldExchangeItem *m_dataItem;
    CCScrollView* m_parentScrollView;
    int m_parentType;
};

class GoldFirstPayRewardCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static GoldFirstPayRewardCell* create(int itemId, int num,int type);
    GoldFirstPayRewardCell(){};
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

#endif /* defined(__IF__GoldFirstPayView__) */
