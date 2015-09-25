//
//  DragonBattleJoinView.h
//  IF
//
//  Created by ganxiaohua on 15/8/25.
//
//

#ifndef __IF__DragonBattleJoinView__
#define __IF__DragonBattleJoinView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class DragonBattleJoinView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static DragonBattleJoinView *create();
    DragonBattleJoinView(){};
    ~DragonBattleJoinView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickJoin(CCObject *pSender, CCControlEvent event);
    void onClickInfo(CCObject *pSender, CCControlEvent event);
    void onHistoryClick(CCObject *pSender, CCControlEvent event);
    void onClickHelp(CCObject *pSender, CCControlEvent event);
    void onManageClick(CCObject *pSender, CCControlEvent event);
    void getData(CCObject *pSender);
    
    CCSafeObject<CCControlButton> m_joinBtn;
    CCSafeObject<CCControlButton> m_infoBtn;
    CCSafeObject<CCControlButton> m_historyBtn;
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<CCControlButton> m_manageBtn;
    
    CCSafeObject<CCLabelIF> m_battleTile;
    CCSafeObject<CCLabelIF> m_battleDesc;
    CCSafeObject<CCLabelIF> m_startTime;
    CCSafeObject<CCLabelIF> m_endTime;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCSprite> m_fixbg;
};

#endif /* defined(__IF__DragonBattleJoinView__) */
