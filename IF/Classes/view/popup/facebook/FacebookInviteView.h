//
//  FacebookInviteView.h
//  IF
//
//  Created by ganxiaohua on 14-10-29.
//
//

#ifndef __IF__FacebookInviteView__
#define __IF__FacebookInviteView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"
class FacebookInviteView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static FacebookInviteView *create(CCArray* fdata=NULL,string ftype="2");
    FacebookInviteView();
    ~FacebookInviteView();
    CCNode* getGuideNode(string _key);
    int getScrollPos();
    
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
    virtual bool init(CCArray* fdata,string ftype);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void onSendClick(CCObject *pSender, CCControlEvent event);
    void onSellectClick(CCObject *pSender, CCControlEvent event);
    void onSearchClick(CCObject *pSender, CCControlEvent event);
    void onCellClickHandle(CCObject *data);

    void getInviteFriends(CCObject *data);
    void getRequestFriends(CCObject *data);
    void addLoadingAni();
    void removeLoadingAni();
    void inviteSuccess(CCObject *data);
    void initData();
    void refreshData(float t);
    void delayShowData(float t);
    
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_srcData;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCControlButton> m_sendBtn;
    CCSafeObject<CCControlButton> m_selectBtn;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCLabelIF> m_tip;
    CCSafeObject<CCNode> m_searchNode;
    CCSafeObject<CCControlButton> m_searchBtn;
    CCSafeObject<CCEditBox> m_inputName;
    
    int m_inviteNum;
    bool m_selectAll;
    string m_ftype;

};

class FriendPicCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner

{
public:
    static FriendPicCell *create(CCDictionary* info,CCNode* clickArea);
    void setData(CCDictionary* info);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    FriendPicCell(CCDictionary* info,CCNode* clickArea):m_info(info),m_clickArea(clickArea){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void changePic();
    void changeSuccess();
    void clickHead(CCObject* data);

    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCNode> m_selectNode;
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCDictionary> m_info;
    CCNode* m_clickArea;
    bool m_isDraging;
    int m_type;
};

#endif /* defined(__IF__FacebookInviteView__) */
