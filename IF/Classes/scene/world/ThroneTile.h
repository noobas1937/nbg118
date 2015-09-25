//
//  ThroneTile.h
//  IF
//
//  Created by 李锐奇 on 14/12/4.
//
//

#ifndef __IF__ThroneTile__
#define __IF__ThroneTile__

#include "NewBaseTileInfo.h"
#include "DefaultTableViewDelegate.h"
#include "HFHeadImgNode.h"

static int _loadPerTime = 40;

class ThroneTile : public NewBaseTileInfo {
public:
    
    ThroneTile(WorldCityInfo& info):
    NewBaseTileInfo(info)
    {}
    
    static ThroneTile* create(WorldCityInfo& info) {
        auto ret = new ThroneTile(info);
        if (ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
protected:
    virtual bool init();
    virtual std::string getShowText();
    virtual int getOwnerFlag();
    virtual void setPeaceTimeButtons();
    virtual void setNotOpenTimeButtons();
    virtual void setDefenderWarTimeButtons();
    virtual void setAttackerWarTimeButtons();
    virtual bool isSelfBuild();
    bool isInProtect();
    virtual void onInfoClick(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual void onClickKing(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual void onClickOccupy(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual void onClickRally(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual void onTroopClick(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual void onClickScout(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual void onClickSurpport(CCObject * pSender, Control::EventType pCCControlEvent);
};

class FightInfo : public CCObject{
public:
    time_t time;
    std::string content;
    int color;
    int type;
    
    std::string pic;
    int picVer;
    int kPeriodId;
    std::string abbr;
    std::string kName;
    std::string kUid;
};

class WarBuildInfoPopUpView : public PopupBaseView
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
,public CCTableViewDataSource
,public DefaultTableViewDelegate
//,public CCTouchDelegate
{
public:
    static WarBuildInfoPopUpView *create(WorldCityInfo &info);
    virtual void update(float time);
private:
    WarBuildInfoPopUpView(WorldCityInfo &info) : m_info(info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    void onInfoClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onHistoryClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onKingRecordClick(CCObject * pSender, Control::EventType pCCControlEvent);

    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

    void getData();
    void dataBack(CCObject *obj);
    void sortData();
    
    void getKingRecordData();
    void getHistoryData();
    void historyDataBack(CCObject *obj);
    void kingRecordCallBack(CCObject *obj);
    std::string getCellString(FightInfo *info);
    void showTab();
    WorldCityInfo &m_info;
    bool isDataBack;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_timeTitle;
    CCSafeObject<CCLabelIF> m_occupyTitle;
    CCSafeObject<CCLabelIF> m_soldierText;
    CCSafeObject<CCLabelIFTTF> m_testTxt;
    CCSafeObject<CCLabelIFTTF> m_allianceName;
    CCSafeObject<CCLabelIFTTF> m_occupyName;
    CCSafeObject<CCControlButton> m_infoBtn;
    CCSafeObject<CCControlButton> m_historyBtn;
    CCSafeObject<CCControlButton> m_kingRecordBtn;
    CCSafeObject<CCNode> m_iconContainer;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_allianceNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_soldierNode;
    CCSafeObject<CCNode> m_historyNode;
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCLabelIF> m_noRecordText;
    CCSafeObject<CCNode> m_kingNode;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<HFHeadImgNode> m_headImgNode;

    double nextRefreshTime;
    WorldActivityState type;
    CCSafeObject<CCArray> m_fightInfos;
    CCSafeObject<CCArray> m_recordKings;
    int m_currentTab;
    CCPoint m_touchPos;
    bool hasMore;
    int m_currentNum;
    string m_kTimes;
    int m_totalPage;
    int m_curPage;
};

class FightInfoCell :  public CCTableViewCell
, public CCBMemberVariableAssigner
{
public:
    static FightInfoCell *create();
    void setData(FightInfo* info,int idx=-1);
    int getMaxHeight();
    bool checkIsEventInSide(cocos2d::CCTouch *pTouch);
    CCSafeObject<CCScale9Sprite> m_clickBg;
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCScale9Sprite> m_cellBg;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIFTTF> m_contentTxt;
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_node2;
    // king record
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIFTTF> m_txt3;
    CCSafeObject<CCNode> m_historyNode;
    CCSafeObject<CCNode> m_kingNode;
    CCSafeObject<CCScale9Sprite> m_renderBg1;
    CCSafeObject<CCScale9Sprite> m_renderBg2;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    
    FightInfo* m_info;
    int m_idx;
};

class WarNoticeInfoView: public PopupBaseView
//, public CCTouchDelegate
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static WarNoticeInfoView* create();
protected:
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void onTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    void onClickHelpBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelIF> m_infoLabel1;
    CCSafeObject<CCLabelIF> m_infoLabel2;
    CCSafeObject<CCLabelIF> m_infoLabel3;
    CCSafeObject<CCLabelIF> m_infoLabel4;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<CCScrollView> m_scrollView;
    
};

#endif /* defined(__IF__ThroneTile__) */
