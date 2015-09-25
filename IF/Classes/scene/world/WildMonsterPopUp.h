//
//  WildMonsterPopUp.h
//  IF
//
//  Created by 李锐奇 on 14-7-29.
//
//

#ifndef __IF__WildMonsterPopUp__
#define __IF__WildMonsterPopUp__

#include "CommonInclude.h"
#include "WorldController.h"
#include "PopupBaseView.h"

static const int maxnum = 4;

class WildMonsterPopUp : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver

{
public:
    WildMonsterPopUp(WorldCityInfo &info) : m_info(info){};
    
    static WildMonsterPopUp *create(WorldCityInfo &info);
    void onAttackButton(CCObject * pSender, Control::EventType pCCControlEvent);
private:
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void onExit();
    void onEnter();
    virtual bool init();
    WorldCityInfo &m_info;
    
//    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    

    CCSafeObject<CCLabelIF> m_text;
    CCSafeObject<CCControlButton> m_btn;
    CCSafeObject<CCScale9Sprite> m_tileBg;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_descriptionText;
    CCSafeObject<CCLabelIF> m_nameAndLvText;
    CCSafeObject<CCLabelIF> m_dropText;

    CCSafeObject<CCLabelIF> m_itemText[maxnum];
    CCSafeObject<CCNode> m_itemPicNode[maxnum];
    CCSafeObject<CCNode> m_itemTouchNode[maxnum];
};

#endif /* defined(__IF__WildMonsterPopUp__) */
