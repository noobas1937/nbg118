//
//  AllianceWarView.h
//  IF
//
//  Created by ganxiaohua on 14-4-15.
//
//

#ifndef __IF__AllianceWarView__
#define __IF__AllianceWarView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "AllianceTeamInfo.h"
#include "DefaultTableViewDelegate.h"
#include "CCClipNode.h"
#include "AllianceWarHeadCell.h"
#include "HFHeadImgNode.h"
#include "CCTableViewTouchIFCell.h"
class AllianceWarView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static AllianceWarView* create(int initTab=0);
    AllianceWarView(){};
    virtual ~AllianceWarView(){};
    void updateInfo(CCObject* data);
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int initTab);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onBtnAttClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtnDefClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void openJoinAlliance(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickResult(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickTab1(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickTab2(CCObject * pSender, Control::EventType pCCControlEvent);
    void refresh(CCObject* param);
    void addLoadingAni();
    void removeLoadingAni(CCObject* p = NULL);
    bool checkOpenMonster();
    void refreshTableView();
    void changeTableData();
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCControlButton> m_btnAtt;
    CCSafeObject<CCControlButton> m_btnDef;
    CCSafeObject<CCTableView> m_tableView;
    CCSafeObject<CCLabelIF> m_infoTxt;
    CCSafeObject<CCNode> m_noAllianeNode;
    CCSafeObject<CCLabelIF> m_noAllianceTip;
    CCSafeObject<CCControlButton> m_btnJoin;
    CCSafeObject<CCControlButton> m_btnResult;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCNode> m_tabNode;
    CCSafeObject<CCControlButton> m_tabBtn1;
    CCSafeObject<CCControlButton> m_tabBtn2;
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_srcData;
    int m_addHeight;
    int m_listH;
    int m_tabType;
    bool m_first;
};

class AllianceWarCell :
public CCTableViewTouchIFCell
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static AllianceWarCell* create(AllianceTeamInfo* info,int index);
    AllianceWarCell(AllianceTeamInfo* info,int index):m_info(info),m_index(index){};
    void setData(AllianceTeamInfo* info);
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void updateData();
    void updateTime(float t);
    void playScaleAniEnd();
    void goToWorld();
    
    void refresh(CCObject* pObj);
    
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<AllianceTeamInfo> m_info;
    CCSafeObject<CCNode> m_posNode2;
    
    CCSafeObject<CCNode> m_signNode;
    CCSafeObject<CCNode> m_sAttNode;
    CCSafeObject<CCNode> m_sDefNode;
    CCSafeObject<CCNode> m_head1;
    CCSafeObject<CCNode> m_head2;
    CCSafeObject<CCSprite> m_signSpr1;
    CCSafeObject<CCSprite> m_signSpr2;
    CCSafeObject<CCLabelIF> m_sStatusTxt;
    CCSafeObject<CCLabelIF> m_sTitleTxt;
    CCSafeObject<CCLabelIF> m_sTimeTxt;
    CCSafeObject<CCLabelIFTTF> m_sTxt1;
    CCSafeObject<CCLabelIFTTF> m_sTxt2;
    CCSafeObject<CCLabelIF> m_sTxt3;
    CCSafeObject<CCLabelIF> m_sTxt4;
    
    CCSafeObject<CCNode> m_teamNode2;
    CCSafeObject<CCLabelIF> m_teamTitleTxt;
    CCSafeObject<CCLabelIF> m_teamStatusTxt;
    CCSafeObject<CCLabelIF> m_teamTimeTxt;
    CCSafeObject<CCLabelIF> m_targetTitle;
    CCSafeObject<CCLabelIF> m_teamTargetPosTxt1;
    CCSafeObject<CCLabelIF> m_teamTargetTxt;
    CCSafeObject<CCNode> m_cityIconNode;
    CCSafeObject<CCSprite> m_teamAction1;
    CCSafeObject<CCSprite> m_teamAction2;
    CCSafeObject<CCLabelIFTTF> m_teamTxt1;
    CCSafeObject<CCLabelIF> m_teamTxt2;
    CCSafeObject<CCLabelIFTTF> m_teamTxt3;
    CCSafeObject<CCLabelIF> m_teamTxt4;
    CCSafeObject<CCNode> m_teamHeadNode1;
    CCSafeObject<CCNode> m_teamHeadNode2;
    CCSafeObject<CCNode> m_teamAttNode;
    CCSafeObject<CCNode> m_teamDefNode;
    CCSafeObject<CCNode> m_teamAttNode2;
    CCSafeObject<CCNode> m_teamDefNode2;
    CCSafeObject<CCNode> m_signClick1;
    CCSafeObject<CCNode> m_signClick2;
    CCSafeObject<CCNode> m_signClick3;
    CCSafeObject<CCNode> m_signClick4;
    CCSafeObject<CCNode> m_ccbNode;
    
    CCSafeObject<CCNode> m_territoryNode;
    CCSafeObject<CCLabelIF> m_tTargetTxt;
    CCSafeObject<CCLabelIF> m_tCoordTxt;
    CCSafeObject<CCLabelIF> m_tNameTxt;
    CCSafeObject<CCNode> m_tHeadNode2;
    CCSafeObject<CCNode> m_tPicNode;
    CCSafeObject<CCLabelIF> m_tNumTxt;
    CCSafeObject<CCLabelIF> m_tTitleTxt;
    CCSafeObject<CCLabelIF> m_tStatusTxt;
    CCSafeObject<CCLabelIF> m_tTimeTxt;
    CCSafeObject<CCNode> m_tPosNode;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    int m_index;
    CCPoint m_touchPos;
    bool m_freshRally;
    int m_worldIndex;
    bool m_isMove;
    bool m_isClickMosnter;
};
#endif /* defined(__IF__AllianceWarView__) */
