//
//  DragonUpgradeView.h
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#ifndef __IF__DragonUpgradeView__
#define __IF__DragonUpgradeView__

#include "CommonInclude.h"
#include "ArcPopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCClipNode.h"

class DragonUpgradeView: public ArcPopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static DragonUpgradeView* create();
    DragonUpgradeView(){};
    virtual ~DragonUpgradeView(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onOKClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCControlButton> m_OKBtn;
    CCSafeObject<CCTableView> m_tabView;
    
    CCSafeObject<CCLabelIF> m_attributeTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_levelTxt;
    CCSafeObject<CCLabelIF> m_outputTxt;
    CCSafeObject<CCLabelIF> m_outputValueTxt;
    CCSafeObject<CCLabelIF> m_consumemTxt;
    CCSafeObject<CCLabelIF> m_allFoodTxt;
    CCSafeObject<CCNode> m_starNode;
    CCSafeObject<CCNode> m_dragonNode;
    CCSafeObject<CCNode> m_btnNode1;
    CCSafeObject<CCNode> m_btnNode2;
    CCSafeObject<CCSprite> m_starSpr;
    CCSafeObject<CCSprite> m_foodSpr;
    CCSafeObject<CCControlButton> m_skillBtn;
    CCSafeObject<CCControlButton> m_sellBtn;
    CCSafeObject<CCControlButton> m_moveBtn;
    CCSafeObject<CCControlButton> m_feedBtn1;
    CCSafeObject<CCControlButton> m_feedBtn2;
    void onSkillClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    void onSellClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    void onMoveClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    void onFeed1Click(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    void onFeed2Click(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
};

#endif /* defined(__IF__DragonUpgradeView__) */
