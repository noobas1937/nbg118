//
//  WorldPopupOnceView.h
//  IF
//
//  Created by wangchaoyi on 15/3/23.
//
//

#ifndef __IF__WorldPopupOnceView__
#define __IF__WorldPopupOnceView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
//#include "CCMultiColTableView.h"
//#include "DefaultTableViewDelegate.h"
//#include "BackpackView.h"
//#include "ToolInfo.h"

class WorldPopupOnceView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
{
public:
    static WorldPopupOnceView* create();
    WorldPopupOnceView();
    ~WorldPopupOnceView();
    
protected:
private:
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    CCSafeObject<CCLabelIF> m_infoLabel1;
    CCSafeObject<CCLabelIF> m_infoLabel2;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCNode> m_buildBG;
    CCSafeObject<CCLabelIF> m_infoLabel3;
    CCSafeObject<CCLabelIF> m_infoLabel4;
    CCSafeObject<CCLabelIF> m_infoLabel5;
    CCSafeObject<CCLabelIF> m_infoLabel6;
    CCSafeObject<CCLabelIF> m_infoLabel7;
    CCSafeObject<CCLabelIF> m_infoLabel8;
    CCSafeObject<CCLabelIF> m_infoLabel9;
    CCSafeObject<CCLabelIF> m_infoLabel10;
    CCSafeObject<CCLabelIF> m_infoLabel11;
    CCSafeObject<CCLabelIF> m_infoLabel12;
    CCSafeObject<CCLabelIF> m_infoLabel13;
    CCSafeObject<CCLabelIF> m_infoLabel14;
    CCSafeObject<CCNode> m_spriteNode1;
    CCSafeObject<CCNode> m_spriteNode2;
    CCSafeObject<CCNode> m_spriteNode3;
    CCSafeObject<CCNode> m_spriteNode4;
    CCSafeObject<CCNode> m_infoNode1;
    CCSafeObject<CCNode> m_infoNode2;
    CCSafeObject<CCNode> m_infoNode3;
    CCSafeObject<CCNode> m_infoNode4;

    int m_type;
    CCPoint m_touchPoint;
};

#endif /* defined(__IF__WorldPopupOnceView__) */