//
//  FBAppRequestFriendView.h
//  IF
//
//  Created by ganxiaohua on 15/3/27.
//
//

#ifndef __IF__FBAppRequestFriendView__
#define __IF__FBAppRequestFriendView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"
class FBAppRequestFriendView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static FBAppRequestFriendView *create(CCArray* arr);
    FBAppRequestFriendView();
    ~FBAppRequestFriendView();
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
    virtual bool init(CCArray* arr);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

    void onSearchClick(CCObject *pSender, CCControlEvent event);
    
    void getInviteFriends(CCObject *data);
    void getRequestFriends(CCObject *data);
    void initData();
    void refreshData(float t);
    void delayShowData(float t);
    
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_srcData;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCLabelIF> m_tip;
    CCSafeObject<CCNode> m_searchNode;
    CCSafeObject<CCControlButton> m_searchBtn;
    CCSafeObject<CCEditBox> m_inputName;
    CCSafeObject<CCNode> m_container;
    string m_fbUids;
    WaitInterface* m_waitInterface1;

};

class RequestFriendCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner

{
public:
    static RequestFriendCell *create(CCDictionary* info,CCNode* clickArea);
    void setData(CCDictionary* info);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    RequestFriendCell(CCDictionary* info,CCNode* clickArea):m_info(info),m_clickArea(clickArea){};
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
    void serverCallBack(CCObject* data);
    void confirmFun();
    
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIFTTF> m_timeTxt;
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCControlButton> m_acceptBtn;
    CCDictionary* m_info;
    CCNode* m_clickArea;
    bool m_isDraging;
    int m_type;
    string m_fromId;
};
#endif /* defined(__IF__FBAppRequestFriendView__) */
