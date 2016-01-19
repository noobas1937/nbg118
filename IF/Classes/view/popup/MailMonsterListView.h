//
//  MailMonsterListView.h
//  IF
//
//  Created by lifangkai on 14-11-3.
//
//

#ifndef __IF__MailMonsterListView__
#define __IF__MailMonsterListView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
//#include "CCScrollView.h"
#include "CCMultiColTableView.h"
#include "InputFieldMultiLine.h"
#include "DefaultTableViewDelegate.h"
#include "MailMonsterCellInfo.h"
#include "MailMonsterInfo.h"
#include "CCTableViewTouchIFCell.h"
class MailMonsterListView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
 
{
public:
    static MailMonsterListView* create(MailMonsterCellInfo* info);
    MailMonsterListView(MailMonsterCellInfo* info):m_mailInfo(info){};
    virtual ~MailMonsterListView();
    
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
    void onReturnClick(CCObject * pSender,CCControlEvent pCCControlEvent);
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
    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_mailTitle;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCControlButton> m_addSaveBtn;
//    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCControlButton> m_writeBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
    CCSafeObject<CCControlButton> m_deleteBtn;
    
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_data;
    
    CCSafeObject<MailMonsterCellInfo> m_mailInfo;
    bool m_isLoadMore;
    float m_currMinOffsetY;
    float m_currOffsetY;
    float m_newMinOffsetY;
};

class MailMonsterCell :
public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public CCTableViewTouchIFCell
{
public:
    static MailMonsterCell* create(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo, int idx);
    MailMonsterCell(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index):m_dialogInfo(dialogInfo),m_mailInfo(mailInfo),m_idx(index){};
    ~MailMonsterCell(){};
    void setData(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index);
private:
    virtual void onExit();
    virtual void onEnter();
    
    bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    int m_idx;
    CCSafeObject<MailMonsterInfo> m_dialogInfo;
    CCSafeObject<MailMonsterCellInfo> m_mailInfo;
    CCSafeObject<CCNode> m_cellNode;
    CCSafeObject<CCLabelIF>m_tipLoadingText;
};

//.first Kill cell
class MailMonsterFirstKillCell :
public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public CCTableViewTouchIFCell
{
public:
    static MailMonsterFirstKillCell* create(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo, int idx);
    MailMonsterFirstKillCell(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index):m_dialogInfo(dialogInfo),m_mailInfo(mailInfo),m_idx(index){};
    ~MailMonsterFirstKillCell(){};
    void setData(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index);
private:
    virtual void onExit();
    virtual void onEnter();
    
    bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    

    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCScale9Sprite> m_underlineSpr;
    CCSafeObject<CCScale9Sprite> m_rewardBG;
    CCSafeObject<CCNode> m_posBG;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_posTxt;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_rewardTitleText;
    CCSafeObject<CCLabelIF> m_firstkillTipText;
    
    CCSafeObject<CCLabelIF> m_powerTitleText;
    CCSafeObject<CCLabelIF> m_powerText;
    CCSafeObject<CCLabelIF> m_woundedTitleText;
    CCSafeObject<CCLabelIF> m_woundedText;
    CCSafeObject<CCLabelIF> m_numTitleText;
    CCSafeObject<CCLabelIF> m_nunText;
    CCSafeObject<CCLabelIF> m_killTitleText;
    CCSafeObject<CCLabelIF> m_killText;
    CCSafeObject<CCNode> m_armInfoNode;
    CCSafeObject<CCNode> m_rewardNode;
    CCSafeObject<CCNode> m_ccbNode;
    
    CCPoint m_touchPoint;
    int m_idx;
    CCSafeObject<MailMonsterInfo> m_dialogInfo;
    CCSafeObject<MailMonsterCellInfo> m_mailInfo;
};

//.fail cell
class MailMonsterFailCell :
public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public CCTableViewTouchIFCell
{
public:
    static MailMonsterFailCell* create(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo, int idx, bool miss);
    MailMonsterFailCell(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index):m_dialogInfo(dialogInfo),m_mailInfo(mailInfo),m_idx(index){};
    ~MailMonsterFailCell(){};
    void setData(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index,bool miss);
private:
    virtual void onExit();
    virtual void onEnter();
    
    bool init(bool miss);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCScale9Sprite> m_underlineSpr;
    CCSafeObject<CCNode> m_posBG;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_posTxt;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_tipText;
    
    CCSafeObject<CCLabelIF> m_powerTitleText;
    CCSafeObject<CCLabelIF> m_powerText;
    CCSafeObject<CCLabelIF> m_woundedTitleText;
    CCSafeObject<CCLabelIF> m_woundedText;
    CCSafeObject<CCLabelIF> m_numTitleText;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCLabelIF> m_killTitleText;
    CCSafeObject<CCLabelIF> m_killText;

    CCSafeObject<CCNode> m_armInfoNode;
    CCSafeObject<CCNode> m_rewardNode;
    CCSafeObject<CCNode> m_ccbNode;
    
    CCSafeObject<CCLabelIF> m_missText;
    CCSafeObject<CCNode> m_nodeMiss;
    CCSafeObject<CCNode> m_nodeFail;
    
    CCPoint m_touchPoint;
    int m_idx;
    CCSafeObject<MailMonsterInfo> m_dialogInfo;
    CCSafeObject<MailMonsterCellInfo> m_mailInfo;
    
};
//.victiory cell
class MailMonsterVictoryCell :
public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public CCTableViewTouchIFCell
{
public:
    static MailMonsterVictoryCell* create(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo, int idx);
    MailMonsterVictoryCell(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index):m_dialogInfo(dialogInfo),m_mailInfo(mailInfo),m_idx(index){};
    ~MailMonsterVictoryCell(){};
    void setData(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index);
private:
    virtual void onExit();
    virtual void onEnter();
    
    bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_rewardNode;
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCScale9Sprite> m_underlineSpr;
    CCSafeObject<CCScale9Sprite> m_rewardBG;
    CCSafeObject<CCNode> m_posBG;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_posTxt;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_tipText;
    CCSafeObject<CCLabelIF> m_rewardTitleText;
    
    CCSafeObject<CCLabelIF> m_powerTitleText;
    CCSafeObject<CCLabelIF> m_powerText;
    CCSafeObject<CCLabelIF> m_woundedTitleText;
    CCSafeObject<CCLabelIF> m_woundedText;
    CCSafeObject<CCLabelIF> m_numTitleText;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCLabelIF> m_killTitleText;
    CCSafeObject<CCLabelIF> m_killText;
    
    CCSafeObject<CCNode> m_armInfoNode;
    CCSafeObject<CCNode> m_ccbNode;
    
    CCPoint m_touchPoint;
    int m_idx;
    CCSafeObject<MailMonsterInfo> m_dialogInfo;
    CCSafeObject<MailMonsterCellInfo> m_mailInfo;
};
#endif /* defined(__IF__MailMonsterListView__) */
