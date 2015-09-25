//
//  TerritoryResourceInfoView.h
//  IF
//
//  Created by 〜TIAN~ on 15/5/27.
//
//

#ifndef __IF__TerritoryResourceInfoView__
#define __IF__TerritoryResourceInfoView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "WorldController.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"

class TerritoryResourceInfoView : public PopupBaseView
//, public CCTouchDelegate
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static TerritoryResourceInfoView *create(WorldCityInfo &info);
    virtual void update(float time);
private:
    TerritoryResourceInfoView(WorldCityInfo &info) : m_info(info){};
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
    void refreshView();
    
    WorldCityInfo &m_info;
    CCSafeObject<CCLabelIF> m_nameAndLevelText;
    CCSafeObject<CCLabelIF> m_capText;
    CCSafeObject<CCLabelIF> m_speedText;
    CCSafeObject<CCLabelIF> m_alreadyGetText;
    CCSafeObject<CCLabelIF> m_capLabel;
    CCSafeObject<CCLabelIF> m_defLabel;
    CCSafeObject<CCLabelIF> m_timeLabel;
    
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_buildBG;
    CCSafeObject<CCControlButton> m_withdrawBtn;
    CCSafeObject<CCLabelIF> m_smalIntoTxt;
    CCSafeObject<CCLabelIF> m_withdrawLabel;
};


#endif /* defined(__IF__TerritoryResourceInfoView__) */
