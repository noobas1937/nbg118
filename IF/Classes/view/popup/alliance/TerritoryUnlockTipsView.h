//
//  TerritoryUnlockTipsView.h
//  IF
//
//  Created by 〜TIAN~ on 15/6/1.
//
//

#ifndef __IF__TerritoryUnlockTipsView__
#define __IF__TerritoryUnlockTipsView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "WorldController.h"

class TerritoryUnlockTipsView : public PopupBaseView
,public CCBMemberVariableAssigner
 
{
public:
    static TerritoryUnlockTipsView* create(std::string name, int count);
    TerritoryUnlockTipsView(std::string name, int count):m_name(name),m_count(count){};
    ~TerritoryUnlockTipsView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
private:
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_lock1Label;
    CCSafeObject<CCLabelIF> m_lock2Label;
    CCSafeObject<CCLabelIF> m_lock3Label;
    CCSafeObject<CCLabelIF> m_condition1Label;
    CCSafeObject<CCLabelIF> m_condition2Label;
    CCSafeObject<CCLabelIF> m_condition3Label;
    CCSafeObject<CCLabelIF> m_tipLabel;
    CCSafeObject<CCScale9Sprite> m_bar1;
    CCSafeObject<CCScale9Sprite> m_bar2;
    CCSafeObject<CCScale9Sprite> m_bar3;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_icon1;
    CCSafeObject<CCNode> m_icon2;
    CCSafeObject<CCNode> m_icon3;
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_node2;
    CCSafeObject<CCNode> m_node3;
    CCSafeObject<CCSprite> m_lbar1;
    CCSafeObject<CCSprite> m_rbar1;
    CCSafeObject<CCSprite> m_lbar2;
    CCSafeObject<CCSprite> m_rbar2;
    CCSafeObject<CCSprite> m_lbar3;
    CCSafeObject<CCSprite> m_rbar3;
    std::string m_name;
    int m_count;
    double m_currentNum;
    double m_totalForce;
    double m_scienceLevel;
};

#endif /* defined(__IF__TerritoryUnlockTipsView__) */
