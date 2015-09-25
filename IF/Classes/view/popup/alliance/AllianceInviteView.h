//
//  AllianceInviteView.h
//  IF
//
//  Created by ganxiaohua on 14-12-31.
//
//

#ifndef __IF__AllianceInviteView__
#define __IF__AllianceInviteView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "AllianceInfoMembersView.h"
#include "CCTableViewTouchIFCell.h"
enum InvitePanelType{
    INVITE,
    OFFICIAL,
    KINGSGIFT,
    ITEM_GIFT,//火漆信送礼
    ITEM_DONATE //礼包赠送
};

class AllianceInviteView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static AllianceInviteView *create(int officeId=0, int panelType = INVITE, std::string giftId = "", std::string giftDialogId = "");
    AllianceInviteView(int officeId, int panelType, std::string giftId, std::string giftDialogId):m_officeId(officeId), m_panelType(panelType), m_giftId(giftId), m_dialogId(giftDialogId),m_backTo(0){};
    
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
protected:
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onSearchClick(CCObject *pSender, CCControlEvent event);
    void onInviteClick(CCObject *pSender, CCControlEvent event);
    void onChangeClick(CCObject *pSender, CCControlEvent event);
    void updateData(CCObject* param);
    void getAllianceMemberData(CCObject* param);
    void removeOneData(CCObject* param);
    void inviteFun();
    void callMore();
    void refresh(CCObject* param);
    
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCControlButton> onInviteBtn;
    CCSafeObject<CCControlButton> m_searchBtn;
    CCSafeObject<CCNode> m_searchNode;
    CCSafeObject<CCNode> m_inviteNode;
    CCSafeObject<CCEditBox> m_inputName;
    CCSafeObject<CCLabelIF> m_goldTxt;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_moreTxt;
    CCSafeObject<CCLabelIF> m_tipTxt;
    CCSafeObject<CCNode> m_waitNode;
    
    WaitInterface* m_waitInterface;
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCArray> m_data;
    string m_lang;
    string m_searchKey;
    string m_uids;
    bool m_first;
    int m_total;
    int m_currentPage;
    CCPoint m_startPos;
    CCPoint m_tabPos;
    int m_prevNum;
    float m_currMinOffsetY;
    float m_currOffsetY;
    float m_newMinOffsetY;
    int m_officeId;
    int m_panelType;
    std::string m_giftId;
    std::string m_dialogId;
    std::string m_allianeId;
    
    CC_SYNTHESIZE(int, m_backTo, BackTo);
};

class AllianceUserCell :
public CCTableViewTouchIFCell
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    CREATE_FUNC(AllianceUserCell);
    
    AllianceUserCell(){};
    ~AllianceUserCell(){}
    void setData(AllianceInfoMember* member,CCNode* clickArea,int officeId=0, int panelType = INVITE, std::string giftId = "", std::string didlogId = "");
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void sureToDonate();
    void inviteCallBack();
    void inviteSuccess(CCObject* param);
    void sendGiftSuccess(CCObject* param);
    void playInviteAni(CCObject* param);
    void postionMSG();
    void appointedSuccess(CCObject* param);
    
    CCSafeObject<CCNode> m_headIcon;
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_offLineTime;
    CCSafeObject<CCLabelIF> m_powerValue;
    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCSprite> m_titleFlag;
    CCSafeObject<CCSprite> m_flagBg;
    CCSafeObject<CCNode> m_clickNode1;
    CCSafeObject<CCNode> m_clickNode2;
    CCSafeObject<CCNode> m_onLineBg;
    CCSafeObject<CCNode> m_ccbNode;
    CCSafeObject<CCNode> m_nodeHide[2];
    CCNode* m_clickArea;
    AllianceInfoMember* m_info;
    CCPoint m_touchPos;
    int m_officeId;
    int m_panelType;
    std::string m_dialogId;
    std::string m_giftId;
    std::string m_inviteId;
};
#endif /* defined(__IF__AllianceInviteView__) */
