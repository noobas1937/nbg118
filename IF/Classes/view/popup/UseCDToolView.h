//
//  UseCDToolView.h
//  IF
//
//  Created by fubin on 14-2-24.
//
//

#ifndef IF_UseCDToolView_h
#define IF_UseCDToolView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "WaitInterface.h"

class UseToolCell;

class UseCDToolView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCMultiColTableViewDataSource
, public CCMultiColTableViewDelegate
{
public:
    static UseCDToolView* create(string itemId, int qid,bool timeFlag = false,double startTime = 0,double endTime = 0);
    UseCDToolView():m_itemId(""),m_qid(0),m_finishTime(0),m_sumTime(0),m_curTime(0),m_lastMaxTime(0){};
    virtual ~UseCDToolView(){};
    void updateInfo();
    
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual cocos2d::extension::TableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    void refreashData(CCObject* obj);
    void updateArriveTime(CCObject* data);
    virtual bool init(string itemId, int qid,bool timeFlag = false,double startTime = 0,double endTime = 0);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onEnterFrame(float dt);
    void onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent);
    std::string getNameStr();
    int getTotalTime();

    CCSafeObject<CCNode> m_progressBarNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCScale9Sprite> m_bar;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCMultiColTableView > m_tabView;
    
    std::string m_itemId;
    int m_qid;
    double m_finishTime;
    int m_sumTime;
    int m_curTime;
    int m_lastMaxTime;
    std::vector<int> m_curList;
    bool m_timeFlag;
    double m_startTime;
    double m_endTime;
};

class UseToolCell : public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static UseToolCell* create(int itemId, string objId, int qid);
    UseToolCell():m_itemId(0),m_objId(""),m_price(0){};
    void setData(int itemId, string objId, int qid);
private:
    bool init(int itemId, string objId, int qid);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickUseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onUseTool();
    void onYes();
    
    void onEnterFrame(float dt);
    
    CCSafeObject<CCControlButton> m_useBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_des2Label;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_lockNode;
    CCSafeObject<WaitInterface> m_waitInterFace;
    CCSafeObject<CCNode> m_buyNode;
    CCSafeObject<CCControlButton> m_buyBtn;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    
    int m_itemId;
    string m_objId;
    int m_qid;
    float m_price;
};

#endif
