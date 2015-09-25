//
//  AllianceApplyView.h
//  IF
//
//  Created by ganxiaohua on 14-8-12.
//
//

#ifndef __IF__AllianceApplyView__
#define __IF__AllianceApplyView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include <vector>
#include "CCMultiColTableView.h"
#include "AllianceInfoMembersView.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "UserApplyListCommand.h"
#include "AllianceManager.h"
#include "RefuseAllianceApplyCommand.h"
#include "AcceptAllianceApplyCommand.h"
#include "AllianceApplylistCommand.h"

class AllianceApplyView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static AllianceApplyView* create(string allianceId,bool isApply=false)
    {
        AllianceApplyView *pRet = new AllianceApplyView();
        pRet->setAllianceId(allianceId);
        pRet->m_isApply = isApply;
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }
    virtual ~AllianceApplyView(){}
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    float getContentHeight();
    
    void onGetAllianceMembers(CCObject* data);
    void onShowApply(CCObject *pSender, CCControlEvent event);
    void onApply(CCObject *pSender, CCControlEvent event);
    
    void getUseApplyList(CCObject* obj);
    void updateApplyList(CCObject* data);
    void updateMemberList(CCObject* data);
    void addLoadingAni();
    
    CCSafeObject<CCControlButton> m_showUserBtn;
    CCSafeObject<CCNode> m_applyNode;
    CCSafeObject<CCScale9Sprite> m_background;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCSprite> m_titleBar;
    CCSafeObject<CCLabelIF> m_tip;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCNode> m_btnNode;
    
    std::vector<AllianceInfoMember> m_members;
    
    CC_SYNTHESIZE(CCMultiColTableView*, m_tableView, TableView);
    CC_SYNTHESIZE(string, m_allianceId, AllianceId);
    CCArray m_cells;
    bool m_isApply;
};

class AllianceApplyCell :
public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    CREATE_FUNC(AllianceApplyCell);
    
    AllianceApplyCell(){};
    
    ~AllianceApplyCell()
    {
        
    }
    
    void bind(AllianceInfoMember* member, bool showTitle);
private:
    bool init();
    
    CCSafeObject<CCControlButton> m_manageBtn;
    CCSafeObject<CCControlButton> m_mail;
    void onManageBtnClick(CCObject *pSender, CCControlEvent event);
    void onMailClick(CCObject *pSender, CCControlEvent event);
    void exitAlliance();
    void kickAlliance();
    void jieXianAlliance();
    void sendToServerSuccess();
    void leavelSuccess();
    
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCNode> m_iconPlaceHolder;
    CCSafeObject<CCNode> m_rankIconPlaceHolder;
    CCSafeObject<CCNode> m_titleRankIconPlaceHolder;
    CCSafeObject<CCNode> m_rankNode;
    CCSafeObject<CCNode> m_infoNode;
    
    CCSafeObject<CCLabelIF> m_powerLabel;
    CCSafeObject<CCLabelIF> m_nameLabel;
    AllianceInfoMember* m_info;
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
};
#endif /* defined(__IF__AllianceApplyView__) */
