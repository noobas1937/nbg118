//
//  MailHeiqishiListView.hpp
//  IF
//
//  Created by 〜TIAN~ on 15/10/28.
//
//

#ifndef MailHeiqishiListView_hpp
#define MailHeiqishiListView_hpp

enum HeiqishiResult{
    HWIN,
    HLOOSE
};

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"
#include "InputFieldMultiLine.h"
#include "DefaultTableViewDelegate.h"
#include "MailHeiqishiCellInfo.hpp"
#include "MailHeiqishiInfo.hpp"
#include "CCTableViewTouchIFCell.h"
#include "HFHeadImgNode.h"
class MailHeiqishiListView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate

{
public:
    static MailHeiqishiListView* create(MailHeiqishiCellInfo* info);
    MailHeiqishiListView(MailHeiqishiCellInfo* info):m_mailInfo(info){};
    virtual ~MailHeiqishiListView();
    
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
    
    CCSafeObject<MailHeiqishiCellInfo> m_mailInfo;
    bool m_isLoadMore;
    float m_currMinOffsetY;
    float m_currOffsetY;
    float m_newMinOffsetY;
};

//cell
class MailHeiqishiCell :
public CCBSelectorResolver
, public CCBMemberVariableAssigner

, public CCTableViewTouchIFCell
{
public:
    static MailHeiqishiCell* create(MailHeiqishiInfo* dialogInfo,MailHeiqishiCellInfo* mailInfo, int idx);
    MailHeiqishiCell(MailHeiqishiInfo* dialogInfo,MailHeiqishiCellInfo* mailInfo,int index):m_dialogInfo(dialogInfo),m_mailInfo(mailInfo),m_idx(index){};
    ~MailHeiqishiCell(){};
    void setData(MailHeiqishiInfo* dialogInfo,MailHeiqishiCellInfo* mailInfo,int index);
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
    HeiqishiResult getReportResult();
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCScale9Sprite> m_underlineSpr;
    CCSafeObject<CCScale9Sprite> m_underlineSpr1;
    CCSafeObject<CCNode> m_posBG;
    CCSafeObject<CCNode> m_posBG1;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_posTxt;
    CCSafeObject<CCLabelIF> m_posTxt1;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_rewardTitleText;
    
    CCSafeObject<CCLabelIF> m_powerTitleText;
    CCSafeObject<CCLabelIF> m_powerText;
    CCSafeObject<CCLabelIF> m_woundedTitleText;
    CCSafeObject<CCLabelIF> m_woundedText;
    CCSafeObject<CCLabelIF> m_numTitleText;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCLabelIF> m_pointTitleText;
    CCSafeObject<CCLabelIF> m_pointText;
    
    CCSafeObject<CCNode> m_hqsPicNode;
    CCSafeObject<CCLabelIF> m_totalTitleText;
    CCSafeObject<CCLabelIF> m_totalText;
    CCSafeObject<CCLabelIF> m_killTotalText;
    CCSafeObject<CCLabelIF> m_killText;
    CCSafeObject<CCLabelIF> m_perTitleText;
    CCSafeObject<CCLabelIF> m_perText;
    CCSafeObject<CCLabelIF> m_tipText;
    CCSafeObject<CCRenderTexture> m_selfModelLayer;
    CCSafeObject<CCRenderTexture> m_selfModelLayer1;
    CCSafeObject<CCNode> m_ccbNode;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<CCLabelIF> m_tipLoadingText;
    CCPoint m_touchPoint;
    int m_idx;
    CCSafeObject<MailHeiqishiInfo> m_dialogInfo;
    CCSafeObject<MailHeiqishiCellInfo> m_mailInfo;
    std::string attUid;
    std::string defUid;
    bool isatkHelp;
    bool isdefHelp;
    std::string ownerUid;//为了区分是查邮件的人，还是自己就是邮件的拥有者
};


#endif /* MailHeiqishiListView_hpp */
