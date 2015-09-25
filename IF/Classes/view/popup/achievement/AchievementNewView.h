//
//  AchievementNewView.h
//  IF
//
//  Created by chenfubi on 15/4/3.
//
//

#ifndef __IF__AchievementNewView__
#define __IF__AchievementNewView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "MedalNewView.h"
#include "CCTableViewTouchIFCell.h"
class AchieveNodeView;

class AchievementNewView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
	static AchievementNewView* create(int openType = 0);
	AchievementNewView(){};
    virtual ~AchievementNewView();
    CCSafeObject<AchieveNodeView> m_achieveNodeView;
    CCSafeObject<MedalNewNode> m_medalNodeView;

protected:
    
    
private:
	virtual bool init(int openType);
	virtual void onEnter();
    virtual void onExit();
    
//    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
//    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onSelectBtnClick(CCObject* pObj, CCControlEvent pEvent);
    
private:
    CCSafeObject<CCNode> m_achieveNode;
    CCSafeObject<CCControlButton> m_achieveBtn;
    CCSafeObject<CCControlButton> m_medalBtn;
    CCSafeObject<CCNode> m_viewNode;
    

};


//CLASS AchieveNodeView

class AchieveNodeView : public CCNode
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
	static AchieveNodeView* create();
	AchieveNodeView(){};
    CCSafeObject<CCNode> m_listNode;

protected:
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
private:
	virtual bool init();
	virtual void onEnter();
    virtual void onExit();
    
    void refreshView(CCObject* pObj);
    void getData();
    
private:
    CCSafeObject<CCArray> m_data;
    
    CCSafeObject<CCTableView> m_tableView;
};


//CLASS AchieveNodeViewCell
class AchieveNodeViewCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
{
public:
    static AchieveNodeViewCell* create(AchievementInfo* inf);
    AchieveNodeViewCell(){};
    void setData(AchievementInfo* info);
    virtual ~AchieveNodeViewCell(){
        CCLog("test");
    };
    

private:
    bool init(AchievementInfo* info);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
//    void onGoToBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onRewardClick(CCObject * pSender, Control::EventType pCCControlEvent);
//    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void resetStarNode();
    void displayReward(CCDictionary* dict, CCNode* rewardNode);
    void initNormalStarNode();
    void initSpeStarNode();
    CCNode *getParentTouchNode();
private:
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<CCNode> m_top2Node;
    CCSafeObject<CCNode> m_starNode;
    CCSafeObject<CCNode> m_proNode;
    CCSafeObject<CCNode> m_speStarNode;
    CCSafeObject<CCLabelIF> m_speStarTTF;
    CCSafeObject<CCLabelIF> m_rdMsgLabel;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_progressTxt;
    CCSafeObject<CCControlButton> m_rewardbBtn;
    CCSafeObject<CCNode> m_rewardNode0;
    CCSafeObject<CCNode> m_rewardNode1;
    CCSafeObject<CCLabelIF> m_comLabel;
    
    bool m_canClick;
    
    WaitInterface* m_waitInterface;
    AchievementInfo* m_achieveInfo;
    
};

#endif /* defined(__IF__AchievementNewView__) */
