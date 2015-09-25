//
//  AchievementNewPopUpView.h
//  IF
//
//  Created by lifangkai on 14-7-8.
//
//

#ifndef __IF__AchievementNewPopUpView__
#define __IF__AchievementNewPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
//#include "CCMultiColTableView.h"
#include "QuestInfo.h"
#include "DefaultTableViewDelegate.h"

#define MSG_PLAY_DROP_RD "msg.play.drop.rd"

class AchievementNewPopUpView : public PopupBaseView
, public CCBSelectorResolver
  
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static AchievementNewPopUpView *create();
    ~AchievementNewPopUpView();
    CCNode* getGuideNode(string _key);
    int getScrollPos();
    
protected:
    // scrollview delegate
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void scrollViewDidScroll(CCScrollView* view);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onGoInfoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void refreshView(CCObject* obj);
    void setRecommendTaskInfo(float dt=0);
    void updateRecommendTask();
    int getContentOffSety();
    
    void autoGuideNext(float dt=0);
    
    void onPlayDropRD(CCObject* obj);
    void onDropOneCell(CCObject* obj);
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_cells;
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCNode> m_recommandBG;
    
    CCSafeObject<CCLabelIF> m_noQuestText;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCLabelIF> m_progressTxt;
    CCSafeObject<CCControlButton> m_goInfoBtn;
    CCSafeObject<CCControlButton> m_btnReward;
    CCSafeObject<CCScale9Sprite> m_progress;
    
    CCSafeObject<CCNode> m_rewardIcon;
    CCSafeObject<CCLabelIFBMFont> m_rewardNum;
    
    CCSafeObject<CCNode> m_dropNode;
    
    CCPoint m_touchPoint;
    QuestInfo* m_recommendinfo;
    WaitInterface* m_waitInterface;
    int r_type;
    int m_dropNum;
    int m_guideAutoSt;
};

class AchievementNewTaskCell :public CCAniNode
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
{
public:
    static AchievementNewTaskCell* create(QuestInfo* info,int index);
    AchievementNewTaskCell(QuestInfo* info,int index):m_info(info),m_index(index),m_rewardClick(false),m_playAnimation(false){};
    CCNode* getGuideNode();
    void setData(QuestInfo* info,int index);
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onGoToBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
  //  void onGoToBtnClick1(CCObject * pSender, Control::EventType pCCControlEvent);
    void animationCallBack();
    
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_descTxt;
   // CCSafeObject<CCLabelIF> m_btnLabel;
    CCSafeObject<CCNode> m_cellBG;
    CCSafeObject<CCNode> m_picHead;
    CCSafeObject<CCControlButton> m_btnGoTo;
    CCSafeObject<CCControlButton> m_btnReward;
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<CCLabelIF> m_progressTxt;
    WaitInterface* m_waitInterface;
    CCPoint m_touchPoint;
    QuestInfo* m_info;
    int m_index;
    bool m_rewardClick;
    bool m_playAnimation;
};

class AchievementNewTaskTableCell : public CCTableViewCell
{
public:
    static AchievementNewTaskTableCell* create(QuestInfo* info,int index);
    AchievementNewTaskTableCell():mCell(NULL){};
    ~AchievementNewTaskTableCell(){};
    void setData(QuestInfo* info,int index);
    CCNode* getGuideNode();
    
private:
    bool init(QuestInfo* info,int index);
    AchievementNewTaskCell* mCell;
};
#endif /* defined(__IF__AchievementNewPopUpView__) */
