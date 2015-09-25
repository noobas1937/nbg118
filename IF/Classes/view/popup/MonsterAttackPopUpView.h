//
//  MonsterAttackPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-7-30.
//
//

#ifndef __IF__MonsterAttackPopUpView__
#define __IF__MonsterAttackPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class MonsterAttackPopUpView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
//, public CCTouchDelegate

{
public:
    static MonsterAttackPopUpView *create(MailInfo *info);
    
private:
    MonsterAttackPopUpView(MailInfo *info) : m_info(info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onOkDeleteMail();
    void onDeleteClick(CCObject *pSender, CCControlEvent event);
    void onReportClick(CCObject *pSender, CCControlEvent event);
    void onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event);

    void addNormalReward();
    void addKillReward();
    void removeParticle(CCObject* obj);
    void getData();
    void refreshView(CCObject* obj);
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCLabelIF> m_title1;
    CCSafeObject<CCLabelIF> m_firstKillHint;
    CCSafeObject<CCLabelIF> m_title2;
    CCSafeObject<CCLabelIF> m_upTitle;
    CCSafeObject<CCLabelIF> m_mailTitle;
    CCSafeObject<CCLabelIF> m_attackText;
    CCSafeObject<CCNode> m_battlePicNode;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCControlButton> m_battleBtn;
   // CCSafeObject<CCLabelIF> m_deleteBtnTitle;
    CCSafeObject<CCNode> m_ListNode;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCNode> m_firstNode;
    CCSafeObject<CCNode> m_secondNode;
    CCSafeObject<CCNode> m_secondRewardContainer;
    CCSafeObject<CCNode> m_firstRewardContainer;
    CCSafeObject<CCScale9Sprite> m_firstDec;
    CCSafeObject<CCScale9Sprite> m_secondDec;
    CCSafeObject<CCScale9Sprite> m_firstBG;
    CCSafeObject<CCScale9Sprite> m_secondBG;
    CCSafeObject<CCNode> m_playerNode;
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCSprite> m_chest1;
    CCSafeObject<CCLabelIF> m_totalText;
    CCSafeObject<CCLabelIF> m_loseText;
    CCSafeObject<CCLabelIF> m_woundedText;
    CCSafeObject<CCLabelIF> m_surplusText;
    CCSafeObject<CCLabelIF> m_expText;
    CCSafeObject<CCLabelIF> m_expNumText;
    CCSafeObject<CCLabelIF> m_totalNumText;
    CCSafeObject<CCLabelIF> m_loseNumText;
    CCSafeObject<CCLabelIF> m_woundedNumText;
    CCSafeObject<CCLabelIF> m_surplusNumText;
    CCSafeObject<CCLabelIF> m_selfName;
    CCSafeObject<CCLabelIF> m_posText;
    CCSafeObject<CCNode> m_selfCoord;
    CCSafeObject<CCLabelIF> m_powerDown;
    
    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCControlButton> m_addSaveBtn;

    MailInfo* m_info;
    int m_total;
};
#endif /* defined(__IF__MonsterAttackPopUpView__) */
