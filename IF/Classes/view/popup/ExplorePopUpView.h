//
//  ExplorePopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-7-10.
//
//

#ifndef __IF__ExplorePopUpView__
#define __IF__ExplorePopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class ExplorePopUpView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ExplorePopUpView* create(MailInfo *info);
    ExplorePopUpView(MailInfo *info):m_info(info){};
private:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onDeleteClick(CCObject *pSender, CCControlEvent event);
    void onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onWriteClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void addGet();
    void addLost();
    void getData();
    void refreshView();
    void onOKDeleteMail();
    CCSafeObject<CCNode> m_infoContainer;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<MailInfo> m_info;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_mailTitle;
    CCSafeObject<CCLabelIF> m_contentText;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_totalText;
    CCSafeObject<CCNode> m_firstNode;
    CCSafeObject<CCNode> m_secondNode;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCNode> m_battlePicNode;
    CCSafeObject<CCNode> m_listNode;
//    CCSafeObject<CCLabelIF> m_troopText;
//    CCSafeObject<CCLabelIF> m_getTitle;
//    CCSafeObject<CCLabelIF> m_lostTitle;
//    CCSafeObject<CCLabelIF> m_lostLabel;
//    CCSafeObject<CCLabelIF> m_lostText;
    CCSafeObject<CCLabelIF> m_deleteBtnTitle;
   // CCSafeObject<CCNode> m_lostNode;
   // CCSafeObject<CCNode> m_getNode;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCScale9Sprite> m_bg;
//    CCSafeObject<CCScale9Sprite> m_lostBG;
//    CCSafeObject<CCScale9Sprite> m_getBG;
  //  CCSafeObject<CCSprite> m_savespr;
  //  CCSafeObject<CCSprite> m_unsavespr;
    CCSafeObject<CCControlButton> m_addSaveBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
    CCSafeObject<CCControlButton> m_writeBtn;
//    CCSafeObject<CCControlButton> m_unSaveBtn;
//    CCSafeObject<CCLabelIF> m_saveBtnTitle;
    int m_totalH;
};

class ExploreRewardPreviewCell : public CCTableViewCell
{
public:
    static ExploreRewardPreviewCell *create(int type, int num);
    void setData(int type, int num);
    
private:
    ExploreRewardPreviewCell(int type, int num) : m_type(type), m_num(num){};
    virtual bool init();
    void refreshView();
    int m_type;
    int m_num;
};


class ExploreLossPreviewCell : public CCTableViewCell
{
public:
    static ExploreLossPreviewCell *create(string type, int num);
    void setData(int type, int num);
    
private:
    ExploreLossPreviewCell(string type, int num) : m_type(type), m_num(num){};
    virtual bool init();
    void refreshView();
    string m_type;
    int m_num;
};

#endif /* defined(__IF__ExplorePopUpView__) */
