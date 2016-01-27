//
//  ActivityBeginView.h
//  IF
//
//  Created by ganxiaohua on 14-9-2.
//
//

#ifndef __IF__ActivityBeginView__
#define __IF__ActivityBeginView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "PopupViewController.h"
#include "CCClipNode.h"
#include "UIComponent.h"

class ActivityBeginView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static ActivityBeginView *create();
    ActivityBeginView()
    {
        m_rankRewardForUI = nullptr;
        m_totalRankRewardForUI = nullptr;
    };
    ~ActivityBeginView()
    {
        CC_SAFE_RELEASE_NULL(m_rewards);
        CC_SAFE_RELEASE_NULL(m_reward1);
        CC_SAFE_RELEASE_NULL(m_reward2);
        CC_SAFE_RELEASE_NULL(m_reward3);
        CC_SAFE_RELEASE_NULL(m_rankRewardForUI);
        CC_SAFE_RELEASE_NULL(m_totalRankRewardForUI);
    };
    
    // TODO:
    int height_offset;
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickReward(CCObject *pSender, CCControlEvent event);
    void onClickRecord(CCObject *pSender, CCControlEvent event);
    void onClickRankReward(CCObject *pSender, CCControlEvent event);
    void onInfoBtnClick(CCObject *pSender, CCControlEvent event);
    void onInfoBtn2Click(CCObject *pSender, CCControlEvent event);
    void onInfoBtn3Click(CCObject *pSender, CCControlEvent event);
    void onClickHistory(CCObject *pSender, CCControlEvent event);
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void onCloseClick(CCObject *pSender);
    void onTab1(CCObject *pSender);
    void onTab2(CCObject *pSender);
    void onTab3(CCObject *pSender);
    
    void onStep1(CCObject *pSender, CCControlEvent event);
    void onStep2(CCObject *pSender, CCControlEvent event);
    void onStep3(CCObject *pSender, CCControlEvent event);
    
    void updateTime(float _time);
    void getServerData(CCObject* param);
    void updateEventData(CCObject* param);
    void addLoadingAni();
    void removeLoadingAni(CCObject* obj);
    void showRewards(int hBG);
    
    CCSafeObject<CCNode> m_viewBg;
    
    CCSafeObject<CCSprite> m_loadingIcon;
    
    CCSafeObject<CCSprite> m_step1;
    CCSafeObject<CCSprite> m_step2;
    CCSafeObject<CCSprite> m_step3;
    
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_sorceTxt;
    CCSafeObject<CCLabelIF> m_rankTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIF> m_sorceNum;
    CCSafeObject<CCLabelIF> m_rankNum;
    CCSafeObject<CCLabelIF> m_totalRankTxt;
    CCSafeObject<CCLabelIF> m_totalRankNum;
    CCSafeObject<CCLabelIF> m_desc;
    
    CCSafeObject<CCNode> m_targetNode;
//    CCSafeObject<CCNode> m_clipperNode;
    CCSafeObject<CCLabelIF> m_targetTxt3;
    CCSafeObject<CCLabelIF> m_targetTxt2;
    CCSafeObject<CCLabelIF> m_targetTxt1;
    CCSafeObject<CCLabelIF> m_trTxt3;
    CCSafeObject<CCLabelIF> m_trTxt2;
    CCSafeObject<CCLabelIF> m_trTxt1;
    CCSafeObject<CCLabelIF> m_tRewardTxt3;
    CCSafeObject<CCLabelIF> m_tRewardTxt2;
    CCSafeObject<CCLabelIF> m_tRewardTxt1;
    CCSafeObject<CCNode> m_targetIcon3;
    CCSafeObject<CCNode> m_targetIcon2;
    CCSafeObject<CCNode> m_targetIcon1;
    CCSafeObject<CCLabelIF> m_trNum3;
    CCSafeObject<CCLabelIF> m_trNum2;
    CCSafeObject<CCLabelIF> m_trNum1;
//    CCSafeObject<CCNode> m_targetTxtBg3;
//    CCSafeObject<CCNode> m_targetTxtBg2;
//    CCSafeObject<CCNode> m_targetTxtBg1;
    
    CCSafeObject<CCNode> m_earnNode;
    CCSafeObject<CCNode> m_eNode;
    CCSafeObject<CCScale9Sprite> m_earnBg;
    CCSafeObject<CCLabelIF> m_earnTxt;

    CCSafeObject<CCNode> m_rankNode;
    CCSafeObject<CCLabelIF> m_rankTitle1;
    CCSafeObject<CCLabelIF> m_rankTitle2;    
    CCSafeObject<CCNode> m_clickNode;

    CCSafeObject<CCNode> m_funList;
    CCSafeObject<CCNode> m_rewardNode;
    CCSafeObject<CCNode> m_bottomFix;
    CCSafeObject<CCScale9Sprite> m_rankBg;
    CCSafeObject<CCControlButton> m_btnReward;
    CCSafeObject<CCControlButton> m_btnRecord;
    CCSafeObject<CCControlButton> m_btnRankReward;
    CCSafeObject<CCControlButton> m_btnHistory;
    CCSafeObject<CCControlButton> m_infoBtn;
    CCSafeObject<CCControlButton> m_infoBtn2;
    CCSafeObject<CCControlButton> m_infoBtn3;
    CCSafeObject<CCNode> m_bottomL1;
    CCSafeObject<CCNode> m_bottomL2;
    CCSafeObject<CCScale9Sprite> m_scBG;
    CCSafeObject<CCLayerGradient> m_progress1;
    CCSafeObject<CCLabelIF> m_totalRankTip;
    CCSafeObject<CCNode> m_rewardListNode;
    CCSafeObject<CCNode> m_tab1;
    CCSafeObject<CCNode> m_tab2;
    CCSafeObject<CCNode> m_tab3;
    
    CCSafeObject<CCLabelIF> m_targetSorceTxt;
    CCSafeObject<CCLabelIF> m_lordTxt;
    CCSafeObject<CCControlButton> m_sorceTargetBtn;
    
    CCSafeObject<CCNode> m_titleNode2;
    CCSafeObject<CCNode> m_titleNode3;
    
    CCSafeObject<CCScrollView> m_scrollView;
//    CCSafeObject<CCClipNode> m_clipper;
//    CCSafeObject<CCProgressTimer> m_proTimer;
    CCArray* m_rankReward;
    CCArray* m_rankRewardForUI;
    CCArray* m_totalRankReward;
    CCArray* m_totalRankRewardForUI;
    CCArray* m_rewards;
    CCArray* m_reward1;
    CCArray* m_reward2;
    CCArray* m_reward3;
    
    double m_haveTime;
    bool m_moveFlag;
    CCPoint m_touchPos;
    std::vector<std::string> m_eventIds;
    int m_totalH;
    int m_tab2H;
    int m_tab3H;
};

class ActivityEarnPointCell : public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ActivityEarnPointCell *create(std::string id);
private:
    ActivityEarnPointCell(std::string id):m_id(id){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_pointTxt;
    CCSafeObject<CCNode> m_iconNode;
    std::string m_id;
};
#endif /* defined(__IF__ActivityBeginView__) */
