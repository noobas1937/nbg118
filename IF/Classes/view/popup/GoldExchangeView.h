//
//  GoldExchangeView.h
//  IF
//
//  Created by wangzhenlei on 13-12-13.
//
//

#ifndef __IF__GoldExchangeView__
#define __IF__GoldExchangeView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class GoldExchangeView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static GoldExchangeView* create();
    GoldExchangeView();
    ~GoldExchangeView();
    static bool sortExchangeItem(GoldExchangeItem *item1, GoldExchangeItem *item2);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void setGoldNum();
    void initList();
    void dataRecord();
    void refresh(CCObject* obj);
    void getData();
    bool isGetData();
    
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCLabelIF> m_title;
    CCSafeObject<CCLabelIF> m_goldText;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_BGNode;
    CCSafeObject<CCNode> m_advertisNode;
    CCSafeObject<CCScale9Sprite> m_bg;
    
    CCSafeObject<CCScrollView> m_scrollView;

};

#endif /* defined(__IF__GoldExchangeView__) */
