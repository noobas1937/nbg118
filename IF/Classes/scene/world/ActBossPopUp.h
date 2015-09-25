//
//  ActBossPopUp.h
//  IF
//
//  Created by 付彬 on 15/4/27.
//
//

#ifndef __IF__ActBossPopUp__
#define __IF__ActBossPopUp__

#include "CommonInclude.h"
#include "WorldController.h"
#include "PopupBaseView.h"

static const int maxnum2 = 4;

class ActBossPopUp : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver

{
public:
    ActBossPopUp(WorldCityInfo &info) : m_info(info){};
    
    static ActBossPopUp *create(WorldCityInfo &info);
    void onAttackButton(CCObject * pSender, Control::EventType pCCControlEvent);
private:
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void onExit();
    void onEnter();
    virtual bool init();
    WorldCityInfo &m_info;
    
    //bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    //void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onDetailCallback(cocos2d::CCObject *obj);
    
    
    CCSafeObject<CCLabelIF> m_text;
    CCSafeObject<CCControlButton> m_btn;
    CCSafeObject<CCScale9Sprite> m_tileBg;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_descriptionText;
    CCSafeObject<CCLabelIF> m_nameAndLvText;
    CCSafeObject<CCLabelIF> m_dropText;
    
    CCSafeObject<CCScale9Sprite> m_bar;
    CCSafeObject<CCLabelIF> m_hpLabel;
    
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    
    CCSafeObject<CCLabelIF> m_itemText[maxnum2];
    CCSafeObject<CCNode> m_itemPicNode[maxnum2];
    CCSafeObject<CCNode> m_itemTouchNode[maxnum2];
};

#endif /* defined(__IF__ActBossPopUp__) */
