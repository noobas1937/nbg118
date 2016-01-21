//
//  FriendsView.h
//  IF
//
//  Created by xxrdsg on 15-9-29.
//
//

#ifndef __IF__FriendsView__
#define __IF__FriendsView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "HFHeadImgNode.h"
#include "InputFieldMultiLine.h"
#include "CCTableViewTouchIFCell.h"
#define FRIEND_TITLE_TOUCHED "FRIEND_TITLE_TOUCHED"
#define MAX_FRIEND_DESCRIPTION 30

class FriendsView: public PopupBaseView
, public CCTableViewDelegate
, public CCTableViewDataSource
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual cocos2d::CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onClickSearchBtn(CCObject *pSender, CCControlEvent event);
    void onAddFriendBtnClick(CCObject *pSender, CCControlEvent event);
    
    void onGetTitleTouchMsg(CCObject* obj);
    void onGetMsgDeleteFriend(CCObject* obj);
    void onGetMsgRejectApply(CCObject* obj);
    void onGetMsgAcceptApply(CCObject* obj);
    void onGetMsgFavoFriend(CCObject* obj);
    void onGetMsgCacelFavaoFriend(CCObject* obj);
    void onGetMsgChangeDesc(CCObject* obj);
    
    void onGetMsgDeletedPush(CCObject* obj);
    void onGetMsgAcceptedPush(CCObject* obj);
    void onGetMsgApplyMePush(CCObject* obj);
    
    void goToGroup(int type);
    
    static FriendsView* create();
    void afterGetFriendsInfo(CCObject* param);
    void checkFriendsData(float _time);
    
    bool init();
    virtual void onEnter();
    virtual void onExit();
    void refreshView();
    void refreshData();
    
    void getFBFriendList();
    
    void getFriendLastTalk(CCObject* obj);
    void RefreshFriendLastTalk(CCObject* obj);
    
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_searchNode;
    CCSafeObject<CCControlButton> m_searchBtn;
    CCSafeObject<CCLabelIF> m_label1;
    CCSafeObject<CCLabelIF> m_label2;
    CCSafeObject<CCLabelIF> m_label3;
    CCSafeObject<CCNode> m_touchNode1;
    CCSafeObject<CCNode> m_touchNode2;
    CCSafeObject<CCNode> m_touchNode3;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCLabelIF> m_tipLabel;
    
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCControlButton> m_addFriendBtn;
    
//    CCSafeObject<CCEditBox> m_editBox;
    CCSafeObject<CCTableView> m_tableView;
    
    vector<string> m_data;
    map<int, bool> m_bOpenMap;
    map< int, pair<int, int> > m_numMap;
};

class FriendsCell: public CCTableViewTouchIFCell
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    FriendsCell(string uid, CCNode* touchNode, int type): m_uid(uid), m_getTouchNode(touchNode),cellType(type){};
    static FriendsCell* create(string uid, CCNode* toucNode, int type);
    bool init();
    void setData(string uid, int type);
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onClickAcceptBtn(CCObject *pSender, CCControlEvent event);
    void onClickCancelBtn(CCObject *pSender, CCControlEvent event);
    void onClickApplyBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onGetMsgApplySuc(CCObject* obj);
    void onGetMsgChangeDesc(CCObject* obj);
    
    CCSafeObject<CCNode> m_iconTouchNode;
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCNode> m_touchNode1;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_vipLabel;
    CCSafeObject<CCLabelIF> m_fbNameLabel;
    CCSafeObject<CCLabelIFTTF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_levelTxt;
    CCSafeObject<CCLabelIFTTF> m_onlineLabel;
    CCSafeObject<CCLabelIFTTF> m_moreLabel;
    CCSafeObject<CCControlButton> m_acceptBtn;
    CCSafeObject<CCControlButton> m_cancelBtn;
    CCSafeObject<CCControlButton> m_applyBtn;
    CCSafeObject<CCSprite> m_newIcon;
    CCSafeObject<CCSprite> m_facebook_icon;
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCScale9Sprite> m_dise;
    CCSafeObject<CCNode> m_touchNode2;
    CCSafeObject<CCSprite> m_arrowSpr;
    CCSafeObject<CCLabelIF> m_groupLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    
    CCNode* m_getTouchNode;
    string m_uid;
    int cellType;
};

class FriendPopupView: public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static FriendPopupView* create(string uid);
    FriendPopupView(string uid): m_uid(uid){};
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void playCloseAni();
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameLabel1;
    CCSafeObject<CCLabelIF> m_nameLabel2;
    
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_node2;
    CCSafeObject<CCNode> m_node3;
    CCSafeObject<CCNode> m_node4;
    CCSafeObject<CCNode> m_node5;
    CCSafeObject<CCNode> m_iconNode1;
    CCSafeObject<CCNode> m_iconNode2;
    CCSafeObject<CCNode> m_iconNode3;
    CCSafeObject<CCNode> m_iconNode4;
    CCSafeObject<CCNode> m_iconNode5;
    CCSafeObject<CCLabelIF> m_label1;
    CCSafeObject<CCLabelIF> m_label2;
    CCSafeObject<CCLabelIF> m_label3;
    CCSafeObject<CCLabelIF> m_label4;
    CCSafeObject<CCLabelIF> m_label5;
    CCSafeObject<CCNode> m_nodeClick1;
    CCSafeObject<CCNode> m_nodeClick2;
    CCSafeObject<CCNode> m_nodeClick3;
    CCSafeObject<CCNode> m_nodeClick4;
    CCSafeObject<CCNode> m_nodeClick5;
    CCSafeObject<CCSprite> m_bg1;
    CCSafeObject<CCSprite> m_bg2;
    CCSafeObject<CCSprite> m_bg3;
    CCSafeObject<CCSprite> m_bg4;
    CCSafeObject<CCSprite> m_bg5;
    
    string m_uid;
};

class FriendChangeDesView: public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static FriendChangeDesView* create(string uid);
    FriendChangeDesView(string uid): m_uid(uid){};
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onOkBtnClick(CCObject *pSender, CCControlEvent event);
    void checkNameFun(float dt);
    
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<CCNode> m_nameNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<InputFieldMultiLine> m_editBox;
    
    string m_uid;
};

#endif /* defined(__IF__FriendsView__) */
