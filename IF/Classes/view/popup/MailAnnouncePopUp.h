//
//  MailAnnouncePopUp.h
//  IF
//
//  Created by lifangkai on 14-7-24.
//
//

#ifndef __IF__MailAnnouncePopUp__
#define __IF__MailAnnouncePopUp__


#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"
#include "QuestController.h"

class MailAnnouncePopUp : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate

{
public:
    static MailAnnouncePopUp *create(MailInfo *info);
    
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
    MailAnnouncePopUp(MailInfo * info) : m_info(info){};
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onDeleteClick(CCObject *pSender, CCControlEvent event);
    //void onBtnPosClick(CCObject *pSender, CCControlEvent event);
    void onAddSaveClick(CCObject *pSender, CCControlEvent event);
    void onUpdateBtnClick(CCObject *pSender, CCControlEvent event);
    void refresh(bool isrefresh=false);
    void refreshRewardStatus();
    void getData();
    void onOKDeleteMail();
    
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_contentText;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_rewardTitleText;
    CCSafeObject<CCLabelIF> m_deleteBtnTitle;
    
    CCSafeObject<CCLabelIF> m_tipText;
    CCSafeObject<CCLabelIF> m_updateTitle;
    CCSafeObject<CCLabelIFTTF> m_updateContent;

    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_ListStartNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_moveDownListNode;

    CCSafeObject<CCScale9Sprite> m_listBG;
    CCSafeObject<CCScale9Sprite> m_kuangBG;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCControlButton> m_addSaveBtn;
    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCControlButton> m_updateBtn;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<MailInfo> m_info;
    CCSafeObject<CCSprite> m_sprAnim;

    int m_versionNum;

    int m_totalHg;
    
};

#endif /* defined(__IF__MailAnnouncePopUp__) */
