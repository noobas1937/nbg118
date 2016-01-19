//
//  MailSystemListPopUp.h
//  IF
//
//  Created by lifangkai on 14-7-28.
//
//

#ifndef __IF__MailSystemListPopUp__
#define __IF__MailSystemListPopUp__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCBExtension.h"
#include "CheckBox.h"
#include "CCMultiColTableView.h"
#include "MailPopUpView.h"


class MailSystemListPopUp : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public CCTableViewDelegate
 
{
public:
    static MailSystemListPopUp* create(int panelType = 2);
    ~MailSystemListPopUp(){m_data->release();};
    
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx);
	virtual cocos2d::CCSize cellSizeForTable(  cocos2d::extension::TableView *table);
    virtual CCTableViewCell* tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(  cocos2d::extension::TableView *table);
    
private:
    MailSystemListPopUp(int panelType) : m_panelType(panelType){
        
    };
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void addLoadingAni();
    void removeLoadingAni();
    void onDeleteClick(CCObject *pSender, CCControlEvent event);
    void onWriteClick(CCObject *pSender, CCControlEvent event);
    void onUserMailClick(CCObject *pSender, CCControlEvent event);
    void onSystemMailClick(CCObject *pSender, CCControlEvent event);
    void onSaveMailClick(CCObject *pSender, CCControlEvent event);
    void onAllReadClick(CCObject *pSender, CCControlEvent event);
    
    void onAllSelected(CCObject *pSender, CCControlEvent event);
    void onDelete(CCObject *pSender, CCControlEvent event);
//    void onReward(CCObject *pSender, CCControlEvent event);
    void onWriteMailBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onEnterFrame(float dt);
    void deleteMailcellByIndex(CCObject* obj);
    
    void onEditAllMail(CCObject* obj);
    void onDeleteAllMail(CCObject* obj);
    void onOKDeleteAllMail();
    
    void onCheckBoxClick(CCObject* obj);
    void refreshView(CCObject* obj);
    void refreshAddList(CCObject* obj);
    void sendReloadMoreMail(float _time);
    void sendReloadMoreMail(CCObject* obj);
    void generateDataArr();
    int getMaxMailNum();
    void resetMailDeleteFlag();
    void sort();
    void setUnreadEffect();
    int  getOpenType();
    bool getDataContent();
    void refreshDataList();
    void onOkDeleteMail();
    void setIsLoadMoreFlag();
    
//    CCSafeObject<CCControlButton> m_deleteBtn;
//    CCSafeObject<CCControlButton> m_writeBtn;
//    CCSafeObject<CCControlButton> m_userMailBtn;
//    CCSafeObject<CCControlButton> m_systemMailBtn;
//    CCSafeObject<CCControlButton> m_saveMailBtn;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_bg1;
   // CCSafeObject<CCNode> m_checkBoxContainer;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CheckBox> m_checkBox;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<BGTouchLayer> m_BGTouchLayer;
    
    CCSafeObject<CCNode> m_BGNode;
//    CCSafeObject<CCNode> m_userUnreadNode;
//    CCSafeObject<CCNode> m_saveUnreadNode;
//    CCSafeObject<CCLabelIF> m_sysUnreadText;
//    CCSafeObject<CCLabelIF> m_userUnreadText;
//    CCSafeObject<CCLabelIF> m_saveUnreadText;
//    CCSafeObject<CCLabelIF> m_deleteBtnTitle;
    CCSafeObject<CCSprite> m_loadingIcon;
//    CCSafeObject<CCSprite> m_mailBGFlag2;
  //  CCSafeObject<CCModelLayerColor> m_ccLayerMode;
    CCSafeObject<CCModelLayerColor> m_ccLayerMode1;
    
    CCSafeObject<CCLabelIF> m_mailNum;
    CCSafeObject<CCLabelIF> m_noMail;
    
    CCSafeObject<CCNode> m_nodeEdit;
    CCSafeObject<CCControlButton> m_btnAll;
    CCSafeObject<CCControlButton> m_btnDel;
//    CCSafeObject<CCControlButton> m_btnReward;
    CCSafeObject<CCControlButton> m_wrireMailBtn;
    CCSafeObject<CCControlButton> m_saveBtn;
//    CCSafeObject<CCLabelIF> m_lblAll;
//    CCSafeObject<CCLabelIF> m_lblReward;
//    CCSafeObject<CCLabelIF> m_lblDel;
//    CCSafeObject<CCLabelIF> m_lblWrite;
//    CCSafeObject<CCLabelIF> m_lblSave;
    bool m_allSelected;
    int m_panelType;
    bool isInit;
    bool m_isChangeTab;
    int m_addNum;
    int m_deleteY;
    bool m_isLoadMore;
    float m_currMinOffsetY;
    float m_currOffsetY;
    float m_newMinOffsetY;
    CCPoint m_startPoint;
    int m_cellWith;
    int m_cell_HD_Width;
};

#endif /* defined(__IF__MailSystemListPopUp__) */
