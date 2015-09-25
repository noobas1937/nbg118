//
//  ChooseUserView.h
//  IF
//
//  Created by Émilie.Jiang on 15/6/5.
//
//

#ifndef __IF__ChooseUserView__
#define __IF__ChooseUserView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "AllianceInfoMembersView.h"
#include "HFHeadImgNode.h"
#include "CCTableViewTouchIFCell.h"
#define MSG_USER_CHOOSE "msg.user.choose"

enum USER_SOURCE{
    USER_SOURCE_ALLIANCE
};

struct ChooseUserNodeItem{
    int rank;
    string title;
    std::vector<AllianceInfoMember> mUsers;
    bool isOpen;
};

class ChooseUserView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate{
public:
    static void removeFromChooseUsers(std::string uid);
    static bool addToChooseUsers(std::string uid);
    static bool isInChooseList(std::string uid);
    static ChooseUserView* create(USER_SOURCE source,int limiteNum,string limiteDialog,string &users);
    ChooseUserView();
    ~ChooseUserView();
protected:
    
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onGetAllianceMembers(CCObject* data);
    void updateList(CCObject* data);
    void updateButtonState(CCObject* data);
private:
    static vector<string> ChooseUsers;
    static int m_maxCount;
    static string m_maxDialog;
    
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(USER_SOURCE source,int limiteNum,string limiteDialog,string &users);
    
    void generateData();
    void refreshView();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    
    void onCancelClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onOKClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void addLoadingAni();
    void removeLoadingAni(CCObject* obj);
    
    CCTableView* m_tabList;
    int m_listSize;
    USER_SOURCE mSource;
    
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCScale9Sprite> m_sprBG;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCControlButton> m_btnCancel;
    CCSafeObject<CCControlButton> m_btnOK;
    
    CCSafeObject<CCSprite> m_loadingIcon;
    map<int,ChooseUserNodeItem> mDatas;
    
    
};


class ChooseUserNode:
//public CCTouchDelegate
public CCTableViewTouchIFCell
{
public:
    static ChooseUserNode* create(ChooseUserNodeItem &data,CCNode *touchNode);
    void setData(ChooseUserNodeItem &data);
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    bool init(ChooseUserNodeItem &data,CCNode *touchNode);
    CCNode *mTotalNode;
    CCNode *mNode;
    CCScale9Sprite *mSprTitle;
    CCLabelTTF *mlblTitle;
    CCNode *mTouchNode;
    
    ChooseUserNodeItem mItem;
    float mWidth;
//    float mMaxHeight;
//    float mMinHeight;
};


class ChooseUserCell :
//public CCTouchDelegate
public Layer{
public:
    static ChooseUserCell* create(AllianceInfoMember &ccObj,bool showLine,CCNode *touchNode);
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    bool init(AllianceInfoMember &ccObj,bool showLine,CCNode *touchNode);
    
    HFHeadImgNode *mHeadImgNode;
    CCNode *mNodeHead;
    CCLabelTTF *mlblName;
    CCSprite *mSprMarkBG;
    CCSprite *mSprMark;
    CCScale9Sprite *mSprLine;
    CCNode *mTouchNode;
    AllianceInfoMember ccData;
};

#endif /* defined(__IF__ChooseUserView__) */
