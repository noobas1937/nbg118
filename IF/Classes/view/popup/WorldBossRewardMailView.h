//
//  WorldBossRewardMailView.h
//  IF
//
//  Created by zhangjun on 15-4-28.
//
//

#ifndef IF_WorldBossRewardMailView_h
#define IF_WorldBossRewardMailView_h

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "MailInfo.h"
#include "HFHeadImgNode.h"

class WorldBossRewardMailView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static WorldBossRewardMailView* create(MailInfo& info);
protected:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    WorldBossRewardMailView(MailInfo& info) : m_info(&info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);

    void onDeleteClick(CCObject *pSender, CCControlEvent event);
    void onReplyClick(CCObject *pSender, CCControlEvent event);
    void onRewardClick(CCObject *pSender, CCControlEvent event);
    void onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onWriteClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onJoinAllianceBtnClick(CCObject *pSender, CCControlEvent event);
    void onRefuseAllianceBtnClick(CCObject *pSender, CCControlEvent event);
    void onShareBtnClick(CCObject *pSender, CCControlEvent event);
    void refreshContent(CCObject* p);
    void refrehsReward();
    void onOkDeleteMail();
    void showRewardAnim();
    void showJoinAllianceBtn();

    void setAllBtnPosition();
    void showShareBtn();
    void onLinkClicked(CCObject *ccObj = NULL);
    CCSafeObject<MailInfo> m_info;
    
//    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_mailTitle;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_rewardTitleText;
    CCSafeObject<CCNode> m_contentContainer;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_rewardNode;
//    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCScale9Sprite> m_kuangBG;
    CCSafeObject<CCScale9Sprite> m_listBG;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCControlButton> m_rewardBtn;
    CCSafeObject<CCControlButton> m_replyBtn;
//    CCSafeObject<CCScale9Sprite> m_line;
//    CCSafeObject<CCNode> m_guideNode;
    CCSafeObject<CCNode> m_rewardContainer;
    CCSafeObject<CCScrollView> m_scroll;
    CCSafeObject<CCControlButton> m_addSaveBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
    CCSafeObject<CCControlButton> m_writeBtn;
//    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCModelLayerColor>m_modelLayer;
    CCSafeObject<CCArray> m_listArr;
    CCSafeObject<CCArray> m_listAnimArr;
    CCSafeObject<CCNode> m_ccbNode;
    CCSafeObject<CCNode> m_ListNode;
    CCSafeObject<CCControlButton> m_joinAllianceBtn;
    CCSafeObject<CCControlButton> m_refuseAllianceBtn;
    CCSafeObject<CCNode> m_battlePicNode;
    
    int m_totalH;
    std::vector<std::string> m_linkUrls;
};

#endif
