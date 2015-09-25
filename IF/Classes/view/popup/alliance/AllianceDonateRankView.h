//
//  AllianceDonateRankView.h
//  IF
//
//  Created by lifangkai on 14-9-3.
//
//

#ifndef __IF__AllianceDonateRankView__
#define __IF__AllianceDonateRankView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "BackpackView.h"
#include "ToolInfo.h"
#include "AllianceDonateInfo.h"
#include "HFHeadImgNode.h"
#include "CCTableViewTouchIFCell.h"
class AllianceDonateRankCell;

class AllianceDonateRankView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static AllianceDonateRankView* create(int type=0);
    AllianceDonateRankView(int type):m_type(type){};
    virtual ~AllianceDonateRankView();
    void updateInfo();
    static bool sortByUse(int itemId1, int itemId2);
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
    void refreshDataByUid(CCObject* data);
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickHistoryBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickTodayBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickWeekBtn(CCObject * pSender, Control::EventType pCCControlEvent);

    void sortData();
    void getAllRankData();
    void getTodayRankData();
    void getWeekRandData();
    void onTimer(float dt);
    string getTimeStr(int des);
    //
    
    CCSafeObject<CCControlButton> m_todayBtn;
    CCSafeObject<CCControlButton> m_historyBtn;
    CCSafeObject<CCControlButton> m_weekBtn;

    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCLabelIF> m_timeTitleText;
    CCSafeObject<CCLabelIF> m_timeLeftText;
    CCSafeObject<CCLabelIF> m_textTitle1;
    CCSafeObject<CCLabelIF> m_textTitle2;
    CCSafeObject<CCLabelIF> m_textTitle3;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_timeNode;
    
    CCSafeObject<CCTableView> m_tabView;
    
    
    int m_type;
    bool m_isgetAllCommand;
    bool m_isgetWeekCommand;
    //std::vector<int> m_curList;
    CCSafeObject<CCArray> m_data;
    vector<AllianceDonateInfo*> m_allianceDonateList;
    int m_cityLv;
    double m_time;
};

class AllianceDonateRankCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static AllianceDonateRankCell* create(AllianceDonateInfo* m_info,int index);
    AllianceDonateRankCell(AllianceDonateInfo* m_info,int index):m_info(m_info),m_index(index){};
    void setData(AllianceDonateInfo* m_info,int index);
private:
    bool init(AllianceDonateInfo* m_info,int index);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    

    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCLabelIFTTF> m_text1;
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
    AllianceDonateInfo* m_info;
    CCPoint m_startPoint;
    int m_index;

};

#endif /* defined(__IF__AllianceDonateRankView__) */
