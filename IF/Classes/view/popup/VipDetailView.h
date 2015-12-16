//
//  VipDetailView.h
//  IF
//
//  Created by chenliang on 14-3-18.
//
//

#ifndef __IF__VipDetailView__
#define __IF__VipDetailView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCClipNode.h"

//begin a by ljf
#include "NBProgressBar.h"
//end a by ljf

#define VIP_REFRESH "vip.refresh"

class VipDetailView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public CCTableViewDataSource
{
public:
    static VipDetailView* create();
    
    VipDetailView();
    
    static const float CELL_MARGIN;
    static const int MAX_CELL_NEEDED;
    void refresh();
    
protected:
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onAddPointBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onActivityPointBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTimer(float dt);
    void PlayComeInFromRightAnim();
    void PlayComeInFromLeftAnim();
    void ComeInAnimFinish();
    void PlayComeOutFromRightAnim();
    void PlayComeOutFromLeftAnim();
    void ComeOutAnimFinish();
    void refreshPageFlag();
    string getTimeStr(int secs);
    void setBtnState();
    void setVipProgress(CCObject* obj);
    void onAddVipAnim();
    
    //begin a by ljf
    void updateAvatarFrame(bool isActivated);
    //end a by ljf
    
    CCSafeObject<CCScrollView> m_scroll;
    
    CCSafeObject<CCControlButton> m_tipBtn;
    
    CCSafeObject<CCLabelBMFont> m_vipLvText;
    CCSafeObject<CCLabelIF> m_vipLeftTimeText;
    CCSafeObject<CCNode> m_vipAnimNode;
    CCSafeObject<CCNode> m_picHeadNode;
    CCSafeObject<CCScale9Sprite> m_listBG;
    CCSafeObject<CCSprite> m_pageFlag;
    
    CCSafeObject<CCLabelIF> m_currentVIPText;
    CCSafeObject<CCLabelIF> m_NextVIPText;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_progressBarNode;
    CCSafeObject<CCLabelIF> m_loginDayText;
    
    CCSafeObject<CCLabelIF> m_lblVIP[4];
    
    
    CCSafeObject<CCLabelIF> m_vipNextPointNumText;
    CCSafeObject<CCLabelIF> m_vipLastPointNumText;
    CCSafeObject<CCLabelIF> m_vipCurrentPointNumText;
    CCSafeObject<CCLabelIF> m_desText;
    CCClipNode* m_nodeprogressBar;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCSprite> m_sprVip;
    CCSafeObject<CCSprite> m_sprActive[2];
    CCSafeObject<CCNode> m_barTouchNode;
    CCSafeObject<CCSprite> m_sprBar[4];
    
    CCSize listContentSize;
    CCPoint m_minPoint;
    CCPoint m_maxPoint;
    
    CCTableView* m_tableView;
    CCPoint m_touchPoint;
    CCPoint m_ListPoint;
    bool m_ismoveX;
    bool m_ismoving;
    CCSafeObject<CCArray> m_data;
    CCDictionary* m_vipGroup;
    int m_vipLevel;
    
    int m_intialIndex;
    int m_movedh;
    
    int m_touchState; //0-null,1-active vip, 2 - buy point
    
    CCArray m_freeVipPointsCells;
    CCArray m_usedVipPointsCells;
    
    //begin a by ljf
    CCSafeObject<CCLabelIF> m_vipSuperTitle;
    CCSafeObject<CCLabelIF> m_leftBranch;
    CCSafeObject<CCControlButton> m_activateBtn;
    CCSafeObject<CCScale9Sprite> m_progressFg;
    CCSafeObject<CCScale9Sprite> m_progressBg;
    int m_currentVipLevel;
    CCSafeObject<CCSprite> m_avatarFrame;
    CCSafeObject<CCSprite> m_avatarFlowerLeft;
    CCSafeObject<CCSprite> m_avatarFlowerRight;
    //end a by ljf
};

#endif /* defined(__IF__VipDetailView__) */
