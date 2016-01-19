//
//  ResourceMailPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-3-26.
//
//

#ifndef __IF__ResourceMailPopUpView__
#define __IF__ResourceMailPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"
#include "QuestController.h"

class ResourceMailPopUpView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//, public CCTouchDelegate

{
public:
    static ResourceMailPopUpView *create(MailInfo *info);
    
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual ssize_t numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    ResourceMailPopUpView(MailInfo * info) : m_info(info){};
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void doWhenClose();
    void onDeleteClick(CCObject *pSender, CCControlEvent event);
    void onPosClick(CCObject *pSender, CCControlEvent event);
    void onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onOkDeleteMail();
    void refresh(CCObject* p = NULL);
    void getData();
    
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_descriptionText;
    CCSafeObject<CCLabelIF> m_rewardTitle;
    CCSafeObject<CCLabelIF> m_playerTitle;
    
    CCSafeObject<CCNode> m_posBG;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_rewardNode;
    CCSafeObject<CCNode> m_playerNode;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<MailInfo> m_info;
    CCSafeObject<CCLabelIF> m_posTxt;
    CCSafeObject<CCControlButton> m_btnPos;
    CCSafeObject<CCLabelIF> m_deleteBtnTitle;
    CCSafeObject<CCSprite> m_savespr;
    CCSafeObject<CCSprite> m_unsavespr;
    CCSafeObject<CCControlButton> m_addSaveBtn;
    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCLabelIF> m_saveBtnTitle;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_battlePicNode;
    
    CCSafeObject<CCNode> m_rewardCellNode;
    int m_totalH;
    
};


class ResourceRewardPreviewCell : public CCTableViewCell
{
public:
    static ResourceRewardPreviewCell *create(int type, int num);
    void setData(int type, int num);
    
private:
    ResourceRewardPreviewCell(int type, int num) : m_type(type), m_num(num){};
    virtual bool init();
    void refreshView();
    int m_type;
    int m_num;
};

#endif /* defined(__IF__ResourceMailPopUpView__) */
