//
//  AllianceInfoMembersView.h
//  IF
//
//  Created by chenliang on 14-4-3.
//
//

#ifndef __IF__AllianceInfoMembersView__
#define __IF__AllianceInfoMembersView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include <vector>
#include "CCMultiColTableView.h"
#include "ParticleFireAni.h"
#include "CCLabelIFTTF.h"
#include "CCTableViewTouchIFCell.h"
class AllianceInfoMembersCell;

class AllianceInfoMember:public CCObject
{
public:
    CC_SYNTHESIZE(int, m_rank, Rank);
    CC_SYNTHESIZE(string, m_name, Name);
    CC_SYNTHESIZE(string, m_uid, Uid);
    CC_SYNTHESIZE(string, m_pic, Pic);
    CC_SYNTHESIZE(int, m_picVer, PicVer);
    CC_SYNTHESIZE(int, m_kills, Kills);
    CC_SYNTHESIZE(float, m_offLineTime, OffLineTime);
    CC_SYNTHESIZE(int, m_pointId, PointId);
    CC_SYNTHESIZE(string, m_head, Head);
    CC_SYNTHESIZE(string, m_allianceId, AllianceId);
    CC_SYNTHESIZE(int, m_points, Points);
    CC_SYNTHESIZE(long, m_power, Power);
    CC_SYNTHESIZE(bool, m_open, Open);
    CC_SYNTHESIZE(bool, m_online, Online);
    CC_SYNTHESIZE(int, m_mainCityLv, MainCityLv);
    CC_SYNTHESIZE(bool, m_isManager, IsManager);
    CC_SYNTHESIZE(int, m_appNum, AppNum);
    CC_SYNTHESIZE(string, m_lang, Lang);
    CC_SYNTHESIZE(string, m_abbr, Abbr);
    CC_SYNTHESIZE(std::string, m_applyMessage, ApplyMessage);
    //同一个model 用于表示联盟阶级
    CC_SYNTHESIZE(string, m_allianceTitle, AllianceTitle);
    CC_SYNTHESIZE(bool, m_isTitle, IsTitle);
    
    void reset(){
        m_rank = -1;
        m_lang = "";
        m_name = "";
        m_uid = "";
        m_pic = "";
        m_kills = 0;
        m_pointId = 0;
        m_head = "";
        m_allianceId = "";
        m_points = 0;
        m_open = false;
        m_online = false;
        m_mainCityLv = 0;
        m_isManager = false;
    }
};

typedef std::vector<AllianceInfoMember> InfoMembers;

class AllianceInfoMembersView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static bool compareMember(const AllianceInfoMember member1, const AllianceInfoMember member2);
    static bool compareMember2(const AllianceInfoMember member1, const AllianceInfoMember member2);
    static AllianceInfoMembersView* create(string allianceId,bool isApply=false, int clickType = 0)
    { 
        AllianceInfoMembersView *pRet = new AllianceInfoMembersView();
        pRet->setAllianceId(allianceId);
        pRet->m_isApply = isApply;
        pRet->m_clickType = clickType;
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
    
    virtual ~AllianceInfoMembersView(){}
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
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
    void onGetAppAllianceMembers(CCObject* data);
    void onHelpClick(CCObject *pSender, CCControlEvent event);
    void onApply(CCObject *pSender, CCControlEvent event);
    void openJoinAlliance(CCObject *pSender, CCControlEvent event);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    
    void exitAlliance();
    void kickAlliance();
    void jieXianAlliance();
    void sendToServerSuccess();
    void leavelSuccess();
    
    void getUseApplyList();
    void updateApplyList(CCObject* data);
    void updateMemberList(CCObject* data);
    void addLoadingAni();
    void removeLoadingAni(CCObject* obj);
    void resetPosition();
    void resetScrollPos();
    void updateShowData(CCObject* data);
    void generalData();
    void refresh(CCObject* data);
    void changeAllianceLeader(CCObject* param);
    void acceptOnePlayer(CCObject* data);
    int getAppNumByRank(int rank);
    int getMemberNumByRank(int rank);
    void resetTitleStatus();
    void closeSelf(CCObject* obj);
    
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<CCScale9Sprite> m_background;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCSprite> m_titleBar;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCNode> m_selfNode;
    
    CCSafeObject<CCNode> m_noAllianeNode;
    CCSafeObject<CCLabelIF> m_noAllianceTip;
    CCSafeObject<CCControlButton> m_btnJoin;
    CCSafeObject<CCLabelIF> m_titleTxt;
    
    CCSafeObject<CCNode> m_introNode;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCArray> m_cell;
    
    InfoMembers m_datas;
    InfoMembers m_appDatas;
    InfoMembers m_titles;
    vector<string> m_tmpTitles;
    
    CCSafeObject<CCArray> m_showData;
    
    AllianceInfoMember selfInfo;
    vector<CCParticleBatchNode*> m_parVec;
    CC_SYNTHESIZE(CCTableView*, m_tableView, TableView);
    CC_SYNTHESIZE(string, m_allianceId, AllianceId);
    CCArray m_cells;
    bool m_isApply;
    bool m_first;
    string m_btnStatus;
    
    int m_clickType;//0 默认； 1 点击cell时发送迁城邀请
};

class AllianceInfoMembersCell :
public CCTableViewTouchIFCell
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
//    CREATE_FUNC(AllianceInfoMembersCell);
    static AllianceInfoMembersCell* create(int clickType = 0)
    {
        AllianceInfoMembersCell* ret = new AllianceInfoMembersCell(clickType);
        if (ret && ret->init())
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    AllianceInfoMembersCell(int clickType = 0): m_clickType(clickType){};
    
    ~AllianceInfoMembersCell()
    {
        
    }
    
    void bind(AllianceInfoMember* oneInfo,AllianceInfoMember* twoInfo,bool buttonFlag=false);
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
        
    void onBtnClick1(CCObject *pSender, CCControlEvent event);
    void onBtnClick2(CCObject *pSender, CCControlEvent event);
    void onBtnClick3(CCObject *pSender, CCControlEvent event);
    void onBtnClick4(CCObject *pSender, CCControlEvent event);
    
    void exitAlliance(float _time);
    void kickAlliance();
    void jieXianAlliance();
    void sendToServerSuccess();
    void leavelSuccess(CCObject* obj);
    void sendChatFirst();
    
    CCSafeObject<CCNode> m_infoNode;
    
    CCSafeObject<CCNode> m_rankNode;
    CCSafeObject<CCLabelIFTTF> m_titleTxt;
    CCSafeObject<CCSprite> m_bg;
    CCSafeObject<CCNode> m_arrow;
    CCSafeObject<CCNode> m_rankIcon;
    
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCNode> m_rankSpr;
    CCSafeObject<CCControlButton> m_btn1;
    CCSafeObject<CCControlButton> m_btn2;
    CCSafeObject<CCControlButton> m_btn4;
    CCSafeObject<CCLabelIF> m_numTxt;
    
    AllianceInfoMember* m_oneInfo;
    AllianceInfoMember* m_twoInfo;
    AllianceInfoMember* m_info;
    CCPoint m_touchPos;
    
    int m_clickType;//0 默认； 1 点击cell时发送迁城邀请
};

class AllianceOneMembersCell :
public Layer
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
//    CREATE_FUNC(AllianceOneMembersCell);
    static AllianceOneMembersCell* create(int clickType = 0)
    {
        AllianceOneMembersCell* ret = new AllianceOneMembersCell(clickType);
        if (ret && ret->init())
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    AllianceOneMembersCell(int clickType = 0): m_clickType(clickType){};
    ~AllianceOneMembersCell(){}
    void setData(AllianceInfoMember* member);
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
    //邀请迁城
    void inviteTeleport();
    
    CCSafeObject<CCNode> m_headIcon;
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_offLineTime;
    CCSafeObject<CCLabelIF> m_powerValue;
    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCSprite> m_titleFlag;
    CCSafeObject<CCSprite> m_flagBg;
    CCSafeObject<CCNode> m_clickNode1;
    CCSafeObject<CCNode> m_clickNode2;
//    CCSafeObject<CCNode> m_onLineBg;
    AllianceInfoMember* m_info;
    CCPoint m_touchPos;
    
    int m_clickType;//0 默认； 1 点击cell时发送迁城邀请
};
#endif /* defined(__IF__AllianceInfoMembersView__) */
