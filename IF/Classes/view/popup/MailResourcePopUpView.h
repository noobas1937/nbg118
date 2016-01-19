//
//  MailResourcePopUpView.h
//  IF
//
//  Created by lifangkai on 14-10-9.
//
//

#ifndef __IF__MailResourcePopUpView__
#define __IF__MailResourcePopUpView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
//#include "CCScrollView.h"
#include "CCMultiColTableView.h"
#include "InputFieldMultiLine.h"
#include "DefaultTableViewDelegate.h"
#include "MailDialogInfo.h"
#include "MailResourceCellInfo.h"
#include "MailResourceInfo.h"
#include "CCTableViewTouchIFCell.h"
class MailResourcePopUpView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
 
{
public:
    static MailResourcePopUpView* create(MailResourceCellInfo* info);
    MailResourcePopUpView(MailResourceCellInfo* info):m_mailInfo(info){};
    virtual ~MailResourcePopUpView();
    
protected:

    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
//    virtual CCSize cellSizeForTable(CCTableView *table);
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
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
    CCSafeObject<CCControlButton> m_writeBtn;
    CCSafeObject<CCControlButton> m_deleteBtn;
    
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_data;
    
    CCSafeObject<MailResourceCellInfo> m_mailInfo;
    bool m_isLoadMore;      //如果当前数据数量小于总数量。置为true 可以下拉获取数据
    float m_currMinOffsetY;
    float m_currOffsetY;
    float m_newMinOffsetY;
};

class MailResourceCell :
public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public CCTableViewTouchIFCell
{
public:
    static MailResourceCell* create(MailResourceInfo* dialogInfo,MailResourceCellInfo* mailInfo, int idx);
    MailResourceCell(MailResourceInfo* dialogInfo,MailResourceCellInfo* mailInfo,int index):m_dialogInfo(dialogInfo),m_mailInfo(mailInfo),m_idx(index){};
    ~MailResourceCell(){};
    void setData(MailResourceInfo* dialogInfo,MailResourceCellInfo* mailInfo,int index);
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

    CCSafeObject<CCNode> m_PicNode;
    CCSafeObject<CCScale9Sprite> m_underlineSpr;
    CCSafeObject<CCNode> m_posBG;
    CCSafeObject<CCScale9Sprite> m_rewardBG;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_posTxt;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_tipLoadingText;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCNode> m_ccbNode;
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_node2;
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
    CCSafeObject<MailResourceInfo> m_dialogInfo;
    CCSafeObject<MailResourceCellInfo> m_mailInfo;
};

#endif /* defined(__IF__MailResourcePopUpView__) */
