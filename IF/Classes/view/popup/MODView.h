//
//  MODView.h
//  IF
//
//  Created by 李锐奇 on 14/11/19.
//
//

#ifndef __IF__MODView__
#define __IF__MODView__

#define SERVER_PER_LINE 3
#define REFRESH_BUTTON_STATE "refreshBtnStates"

#include "PopupBaseView.h"
#include "CommonInclude.h"
#include "WorldController.h"
#include "CCMultiColTableView.h"

class MODView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver

{
public:
    CREATE_FUNC(MODView);
    virtual void update(float time);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onClickReward(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickMODManager(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickMODList(CCObject * pSender, Control::EventType pCCControlEvent);
 //   void update(float time);
    void addBG();
    void getRewardData();
    void getRewardDataBack(CCObject *obj);
    void getRewardBack(CCObject *obj);
    CCSafeObject<CCNode> m_bgContainer;

    CCSafeObject<CCControlButton> m_rewardBtn;
    CCSafeObject<CCControlButton> m_managerBtn;
    CCSafeObject<CCControlButton> m_listBtn;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_introText1;
    CCSafeObject<CCLabelIF> m_introText2;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;

    bool isSendRequest;
    bool isGoldInfoBack;
};

class MODManagerPopUpView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate

{
public:
    static MODManagerPopUpView *create(int tab);
    void getDataBack(CCObject *obj);

protected:
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);

private:
    MODManagerPopUpView() : m_currentTab(-1){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int tab);
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onClickAssignment(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSearch(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAddClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onListClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void showTab(int index);//0, 1
    void searchBack(CCObject *obj);
    void getData();
    bool checkServerAndName();
    void generateData();
//    CCSafeObject<CCLabelIF> m_serverListText;
//    CCSafeObject<CCLabelIF> m_modListText;
    void addBG();
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCNode> m_bgContainer;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_modContainer;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCLabelIF> m_kingdomText;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_nameHintText;
    CCSafeObject<CCLabelIF> m_searchNameText;
    CCSafeObject<CCLabelIF> m_serverTxt;
    CCSafeObject<CCLabelIF> m_levelTxt;
    CCSafeObject<CCLabelIF> m_expTxt;
    CCSafeObject<CCNode> m_kingdomInputNode;
    CCSafeObject<CCNode> m_nameInputNode;
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCControlButton> m_assignmentBtn;
    CCSafeObject<CCControlButton> m_addBtn;
    CCSafeObject<CCControlButton> m_listBtn;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_tabNode1;
    CCSafeObject<CCNode> m_tabNode2;
    CCSafeObject<CCScale9Sprite> m_expBar;
    CCSafeObject<CCEditBox> m_nameEdit;
    CCSafeObject<CCEditBox> m_serverEdit;
    CCSafeObject<CCArray> m_modArr;
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<PlayerInfo> m_currentInfo;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    bool isFirstShow;
    int m_currentTab;
};

class ServerCell : public CCTableViewCell{
public:
    static ServerCell *create(ServerListInfo *info);
    void setData(ServerListInfo *info);
    void onClick(cocos2d::CCObject *pTarget, CCControlEvent touchEvent);
    void getData(MODManagerPopUpView *panel);
private:
    ServerCell(ServerListInfo *info) : m_info(info){};
    void onEnter();
    void onExit();
    bool init();
    void refreshView();
    void resetBtnState(CCObject* obj);
    ServerListInfo *m_info;
    CCSafeObject<CCControlButton> m_btn;
};

class MODCell : public CCNode
, public CCBMemberVariableAssigner

{
public:
    static MODCell *create(PlayerInfo *info);
private:
    MODCell(PlayerInfo *info) : m_info(info){};
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    void onEnter();
    void onExit();
    bool init();
    CCSafeObject<PlayerInfo> m_info;
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCLabelIF> m_searchNameText;
    CCSafeObject<CCLabelIF> m_serverTxt;
    CCSafeObject<CCLabelIF> m_levelTxt;
    CCSafeObject<CCLabelIF> m_expTxt;
    CCSafeObject<CCScale9Sprite> m_expBar;
};
#endif /* defined(__IF__MODView__) */
