//
//  GoldExchangeItemView.h
//  IF
//
//  Created by wangzhenlei on 13-12-17.
//
//

#ifndef __IF__GoldExchangeItemView__
#define __IF__GoldExchangeItemView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"

class GoldExchangeItemView : public Layer
, public CCBSelectorResolver
  
, public CCBMemberVariableAssigner
{
public:
    ~GoldExchangeItemView(){
    }
    GoldExchangeItemView(GoldExchangeItem *goldExchangeItem):m_currdataItem(NULL){
        m_dataItem = goldExchangeItem;
         _ignoreAnchorPointForPosition = false;
         setAnchorPoint(Vec2(0,0));
    }
    static GoldExchangeItemView* create(GoldExchangeItem *goldExchangeItem);
    
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
    
    void onSpeedBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void goToExchangeCommamand();
    void refreshData(CCObject* p = NULL);

    CCSafeObject<CCLabelIF> m_goldNum1;
    CCSafeObject<CCLabelIF> m_dollerNum1;
    CCSafeObject<CCNode> m_iconNode1;
    CCSafeObject<CCNode> m_cellNode1;
    
    CCSafeObject<CCLabelIF> m_goldNum2;
    CCSafeObject<CCLabelIF> m_dollerNum2;
    CCSafeObject<CCNode> m_iconNode2;
    CCSafeObject<CCNode> m_cellNode2;
    CCSafeObject<CCLabelIF> m_addFlagText2;
    CCSafeObject<CCLabelIF> m_desText2;
    CCSafeObject<CCLabelIF> m_saleText2;
    CCSafeObject<CCLabelIF> m_goldAddNum2;
    
    CCSafeObject<CCLabelIF> m_goldNum3;
    CCSafeObject<CCLabelIF> m_dollerNum3;
    CCSafeObject<CCNode> m_iconNode3;
    CCSafeObject<CCNode> m_cellNode3;
    CCSafeObject<CCLabelIF> m_addFlagText3;
    CCSafeObject<CCLabelIF> m_desText3;
    CCSafeObject<CCLabelIF> m_saleText3;
    CCSafeObject<CCLabelIF> m_goldAddNum3;
    
    CCSafeObject<CCNode> m_touchNode;
//    CCSafeObject<CCScale9Sprite> m_bg;
//    CCSafeObject<CCSprite> m_bg1;
 
    CCSafeObject<CCNode> m_percerNode;
    GoldExchangeItem *m_dataItem;
    GoldExchangeItem *m_currdataItem;
    CCPoint m_touchLocation;
};



#endif /* defined(__IF__GoldExchangeItemView__) */
