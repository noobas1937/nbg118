//
//  AchievementDetailPopUpView.h
//  IF
//
//  Created by lifangkai on 14-7-9.
//
//

#ifndef __IF__AchievementDetailPopUpView__
#define __IF__AchievementDetailPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "CCMultiColTableView.h"
#include "QuestInfo.h"

class AchievementDetailPopUpView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static AchievementDetailPopUpView *create(QuestInfo* quest);
    CCNode* getGuideNode(string _key);
    int getScrollPos();
    AchievementDetailPopUpView(QuestInfo* info):m_info(info){};
    
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void refreshView(CCObject* obj);
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_buildBG1;
    CCSafeObject<CCScale9Sprite> m_buildBG2;
   // CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCArray> m_cells;
    
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCLabelIF> m_progressTxt;
    CCSafeObject<CCLabelIF> m_progressTotalTxt;
    CCSafeObject<CCLabelIF> m_rewardTitle;
    CCSafeObject<CCControlButton> m_rewardBtn;
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<CCScale9Sprite> m_rewardListBG;
    CCSafeObject<CCNode> m_rewardListNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_headNode;
   // CCSafeObject<CCNode> m_leftBianNode;
   // CCSafeObject<CCNode> m_rightBianNode;
    QuestInfo* m_info;
};

class AchievementDetailRewardCell : public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static AchievementDetailRewardCell *create(QuestInfo* info,int index);
    void setData(QuestInfo* info, int index);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    AchievementDetailRewardCell(QuestInfo* info,int index):m_info(info),m_index(index){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<LayerColor> m_bg_color;
    //CCSafeObject<CCControlButton> m_btn;
    QuestInfo* m_info;
    int m_index;
};
#endif /* defined(__IF__AchievementDetailPopUpView__) */
