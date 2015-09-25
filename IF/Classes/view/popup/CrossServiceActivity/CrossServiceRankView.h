//
//  CrossServiceRankView.h
//  IF
//
//  Created by ganxiaohua on 15/6/5.
//
//

#ifndef __IF__CrossServiceRankView__
#define __IF__CrossServiceRankView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ActivityEventObj.h"

class CrossServiceRankView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static CrossServiceRankView *create(ActivityEventObj* info);
    CrossServiceRankView(ActivityEventObj* info):m_info(info){};
    ~CrossServiceRankView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void onCheckClick(CCObject *pSender, CCControlEvent event);
    void onInfoClick(CCObject *pSender, CCControlEvent event);
    void getData2(CCObject *param);
    void updateTime(float _time);
    void refreshData(int tab);
    
    CCSafeObject<CCControlButton> m_btnClose;
    CCSafeObject<CCControlButton> m_infoBtn;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIF> m_tab1Txt1;
    CCSafeObject<CCLabelIF> m_tab2Txt1;
    CCSafeObject<CCLabelIF> m_tipTxt1;
    CCSafeObject<CCLabelIF> m_tipTxt2;
    CCSafeObject<CCNode> m_tab1;
    CCSafeObject<CCNode> m_tab2;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCControlButton> m_rewardBtn;
    CCSafeObject<CCControlButton> m_kingdomBtn;
    CCSafeObject<CCControlButton> m_playerBtn;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCNode> m_tabRewardNode1;
    CCSafeObject<CCNode> m_tabNode1;
    CCSafeObject<CCNode> m_scNode;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<WaitInterface> m_waitInterface;
    
    ActivityEventObj* m_info;
    double m_userScore;
    int m_userRank;
    double m_kingdomScore;
    int m_kingdomRank;
    int m_tab;
};

#endif /* defined(__IF__CrossServiceRankView__) */
