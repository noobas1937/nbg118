//
//  MailResourceHelpView.h
//  IF
//
//  Created by lifangkai on 14-11-26.
//
//

#ifndef __IF__MailResourceHelpView__
#define __IF__MailResourceHelpView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
//#include "CCScrollView.h"
#include "CCMultiColTableView.h"
#include "InputFieldMultiLine.h"
#include "DefaultTableViewDelegate.h"
#include "MailResourceHelpCellInfo.h"
#include "MailResourceHelpInfo.h"
#include "CCTableViewTouchIFCell.h"
class MailResourceHelpView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
 
{
public:
    static MailResourceHelpView* create(MailResourceHelpCellInfo* info);
    MailResourceHelpView(MailResourceHelpCellInfo* info):m_mailInfo(info){};
    virtual ~MailResourceHelpView();
    
protected:
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){};
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
private:
    virtual void onEnter();
    virtual void onExit();
    void refresh(CCObject* obj);
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onPostBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onDeleteClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onWriteClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void readDialogContent();
    void addLoadingAni();
    void removeLoadingAni();
    void sendReloadMoreMail(float _time);
    void refreshAddList(cocos2d::CCObject *obj);
    void setData();
    void closeInputRefeshList();
    
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_battlePicNode;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCScale9Sprite> m_bg;
    
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCControlButton> m_addSaveBtn;
//    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
    CCSafeObject<CCControlButton> m_writeBtn;
    
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_data;
    
    CCSafeObject<MailResourceHelpCellInfo> m_mailInfo;
    bool m_isLoadMore;
    float m_currMinOffsetY;
    float m_currOffsetY;
    float m_newMinOffsetY;
};

class MailResourceHelpCell :
public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public CCTableViewTouchIFCell
{
public:
    static MailResourceHelpCell* create(MailResourceHelpInfo* dialogInfo,MailResourceHelpCellInfo* mailInfo, int idx);
    MailResourceHelpCell(MailResourceHelpInfo* dialogInfo,MailResourceHelpCellInfo* mailInfo,int index):m_dialogInfo(dialogInfo),m_mailInfo(mailInfo),m_idx(index){};
    ~MailResourceHelpCell(){};
    void setData(MailResourceHelpInfo* dialogInfo,MailResourceHelpCellInfo* mailInfo,int index);
private:
    virtual void onExit();
    virtual void onEnter();
    
    bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void initContents(string info, int lastWidth, int maxNum, int maxWidth);
    void onShieldClick(cocos2d::CCObject *pTarget, CCControlEvent touchEvent);
    void onYesClick();
    void setTranslation(CCObject* msg);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onDeleteBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void openChatFun();
    void setCellStatus(bool isHint);
    
    CCSafeObject<CCArray> m_rewardNodeArr;
    CCSafeObject<CCNode> m_PicNode;
    CCSafeObject<CCNode> m_PicNode1;
    CCSafeObject<CCNode> m_PicNode2;
    CCSafeObject<CCNode> m_PicNode3;
    CCSafeObject<CCNode> m_rewardNode;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCScale9Sprite> m_underlineSpr;
    CCSafeObject<CCScale9Sprite> m_rewardBG;
    CCSafeObject<CCNode> m_posBG;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_posTxt;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCLabelIF> m_tipLoadingText;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCNode> m_ccbNode;
    
    int m_type;
    
    string m_uid;
    CCSize m_bgSize;
    int m_timeNodeY;
    CCPoint m_nameLabelPt;
    CCPoint m_vipLabelPt;
    CCPoint m_translatePt;
    int m_touchNode3Y;
    int m_msgLabelY;
    CCSize m_msgDimensions;
    CCPoint m_touchPoint;
    
    int m_idx;
    CCSafeObject<MailResourceHelpInfo> m_dialogInfo;
    CCSafeObject<MailResourceHelpCellInfo> m_mailInfo;
};

#endif /* defined(__IF__MailResourceHelpView__) */
