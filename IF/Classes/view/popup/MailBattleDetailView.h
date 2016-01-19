//
//  MailBattleDetailView.h
//  IF
//
//  Created by ganxiaohua on 14-6-9.
//
//

#ifndef __IF__MailBattleDetailView__
#define __IF__MailBattleDetailView__

#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "PopupBaseView.h"
#include "CCLoadSprite.h"
#include "GeneralManager.h"

class MailBattleDetailView: public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
{
public:
    static MailBattleDetailView* create(MailInfo *info);
    MailBattleDetailView(MailInfo *info):m_info(info){};
protected:
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void btnClose(CCObject *pSender, CCControlEvent event);
    void generateData();
    CCSafeObject<CCNode> m_infoContainer;
    CCSafeObject<CCLabelIF> m_upTitle;
    CCSafeObject<CCControlButton> m_btnClose;
    //CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_data;
    MailInfo *m_info;
};

class MailPlayerCell :public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static MailPlayerCell* create(CCDictionary *dict,bool attack=true,bool isTitle = false);
    MailPlayerCell(CCDictionary *info,bool attack,bool isTitle):m_dict(info),m_attack(attack),m_title(isTitle){};
    ~MailPlayerCell(){};
     void setData(CCDictionary *info,bool attack=true,bool isTitle =false);
    float getdh();
    int m_offY;
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void handleAsyRes();
    
    CCSafeObject<CCLabelIF> m_endTxt;
    CCSafeObject<CCNode> m_endNode;
    
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_playerNameTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_lostTxt;
    CCSafeObject<CCLabelIF> m_killTxt;
    CCSafeObject<CCLabelIF> m_hurtTxt;
    CCSafeObject<CCLabelIF> m_tipTitleText;
    CCSafeObject<CCScale9Sprite> m_dftFlag;
    CCSafeObject<CCScale9Sprite> m_atkFlag;
    CCSafeObject<CCScale9Sprite> m_kuangBG;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_cellNode;
    CCSafeObject<CCNode> m_cellTitleNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCNode> m_tipNode;
    bool m_attack;
    bool m_title;
    CCDictionary* m_dict;
    int m_index;
    int m_sumH;
};

class GeneralItemCell :
public CCNode
{
public:
    static GeneralItemCell* create(GeneralInfo *info,int kill);
    GeneralItemCell(GeneralInfo *info,int kill):m_info(info),m_kill(kill){};
    ~GeneralItemCell(){};
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    void handleAsyRes(CCObject* p);
    
    GeneralInfo* m_info;
    int m_kill;
};

class DetailBattleCellCCB : public CCNode
, public CCBMemberVariableAssigner

{
public:
    static DetailBattleCellCCB *create(std::string name,std::string num,std::string lost,std::string kill,std::string hurt,string icon="",string icon1="",string armyid = "",string playname = "",int star = 0);
private:
    DetailBattleCellCCB(std::string name,std::string num,std::string lost,std::string kill,std::string hurt,string icon,string icon1,string armyid,string playname,int starNum) : m_nameStr(name),
    m_numStr(num),
    m_lostStr(lost),
    m_killStr(kill),
    m_hurtStr(hurt),
    m_icon(icon),
    m_icon1(icon1),
    m_armyid(armyid),
    m_playername(playname),
    m_starNum(starNum)
    {};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    std::string m_nameStr;
    std::string m_numStr;
    std::string m_lostStr;
    std::string m_killStr;
    std::string m_hurtStr;
    string m_icon;
    string m_icon1;
    bool m_isTitle;
    string m_armyid;
    string m_playername;
    int m_starNum;
    
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_lostTxt;
    CCSafeObject<CCLabelIF> m_killTxt;
    CCSafeObject<CCLabelIF> m_hurtTxt;
    CCSafeObject<CCNode> m_levelSprNode;
    CCSafeObject<CCNode> m_picHead;
};
#endif /* defined(__IF__MailBattleDetailView__) */
