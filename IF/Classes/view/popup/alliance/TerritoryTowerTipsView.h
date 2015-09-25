//
//  TerritoryTowerTipsView.h
//  IF
//
//  Created by 〜TIAN~ on 15/6/10.
//
//

#ifndef __IF__TerritoryTowerTipsView__
#define __IF__TerritoryTowerTipsView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "WorldController.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"

class TerritoryTowerTipsView : public PopupBaseView
////, public CCTouchDelegate
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static TerritoryTowerTipsView *create(WorldCityInfo& info,bool flag);
    
private:
    TerritoryTowerTipsView(WorldCityInfo& info,bool flag):m_cityInfo(info),m_flag(flag){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void onWithdrawBtnClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent);
    void withDraw();
    void secondConfirm();
    void showAllianceInfo(CCObject* obj);
    bool m_flag;
    void onTipBtnClick1(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onTipBtnClick2(CCObject * pSender, CCControlEvent pCCControlEvent);
    CCSafeObject<CCNode> m_buildBG;
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_node2;
    CCSafeObject<CCControlButton> m_withdrawBtn;
    CCSafeObject<CCLabelIF> m_TipLabel1;
    CCSafeObject<CCLabelIF> m_TipLabel2;
    CCSafeObject<CCLabelIF> m_withdrawLabel;
    CCSafeObject<CCLabelIF> m_TitleLabel1;
    CCSafeObject<CCLabelIF> m_TitleLabel2;
    CCSafeObject<CCControlButton> m_tipBtn1;
    CCSafeObject<CCControlButton> m_tipBtn2;
    WorldCityInfo& m_cityInfo;
};


#endif /* defined(__IF__TerritoryTowerTipsView__) */
