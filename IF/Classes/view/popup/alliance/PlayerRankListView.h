//
//  PlayerRankListView.h
//  IF
//
//  Created by lifangkai on 14-9-16.
//
//

#ifndef __IF__PlayerRankListView__
#define __IF__PlayerRankListView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "BackpackView.h"
#include "ToolInfo.h"
#include "PlayerRankInfo.h"
#include "CCTableViewTouchIFCell.h"
class PlayerRankListCell;

class PlayerRankListView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static PlayerRankListView* create(int type=0);
    PlayerRankListView(int type):m_type(type){};
    ~PlayerRankListView(){};
    void updateInfo();
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    void refreashData(CCObject* obj);
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onTipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void getRankData();
    
    //
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCLabelIF> m_rankTitle;
    CCSafeObject<CCLabelIF> m_rankText;
    CCSafeObject<CCLabelIF> m_textTitle1;
    CCSafeObject<CCLabelIF> m_textTitle2;
    
    CCSafeObject<CCNode> m_totalNode;
    
    CCSafeObject<CCTableView> m_tabView;
    
    
    int m_type;
    bool m_isgetAllCommand;
    
    CCSafeObject<CCArray> m_data;
    vector<PlayerRankInfo*> m_allianceList;
    int m_cityLv;
    double m_time;
};

#include "HFHeadImgNode.h"

class PlayerRankListCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static PlayerRankListCell* create(PlayerRankInfo* info,int index,int type);
    PlayerRankListCell(PlayerRankInfo* info,int index,int type):m_info(info),m_index(index),m_type(type){};
    void setData(PlayerRankInfo* info,int index,int type);
private:
    bool init(PlayerRankInfo* info,int index);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<Label> m_text1;
    CCSafeObject<CCLabelIF> m_text2;
    CCSafeObject<CCLabelIF> m_text3;
    
    CCSafeObject<CCScale9Sprite> m_sprBG1;
    CCSafeObject<CCScale9Sprite> m_sprBG2;
    CCSafeObject<CCScale9Sprite> m_sprBG3;
    CCSafeObject<CCSprite> m_numspr1;
    CCSafeObject<CCSprite> m_numspr2;
    CCSafeObject<CCSprite> m_numspr3;
    CCSafeObject<CCScale9Sprite> m_sprBG4;
    CCSafeObject<CCSprite> m_selfheadSpr;
    CCSafeObject<CCSprite> m_otherheadSpr;
    
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCNode> m_rankNode;
    CCSafeObject<CCNode> m_hintBGNode;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    PlayerRankInfo* m_info;
    int m_index;
    CCPoint m_startPoint;
    int m_type;
    
};

#endif /* defined(__IF__PlayerRankListView__) */
