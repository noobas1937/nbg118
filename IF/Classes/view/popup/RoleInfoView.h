//
//  RoleInfoView.h
//  IF
//
//  Created by ganxiaohua on 14-3-21.
//
//

#ifndef __IF__RoleInfoView__
#define __IF__RoleInfoView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "VipUtil.h"
#include "CCEditLabel.h"
#include "DefaultTableViewDelegate.h"
#include "HFHeadImgNode.h"
#include "CCTableViewTouchIFCell.h"
typedef enum
{
    SELF_VIEW_TYPE,
    OTHER_VIEW_TYPE
} RoleViewType;

class RoleCellDelegate
{
public:
    virtual void showTipNode(string name,string des,CCTouch *pTouch)=0;
    virtual void hideTipNode()=0;
};

class RoleInfoView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//,public CCTouchDelegate
,public CCTableViewDataSource
,public DefaultTableViewDelegate
,public RoleCellDelegate
{
public:
    static RoleInfoView* create(PlayerInfo* info,int fromType=0, string achievePro="", string medalCntStr="",string medalIconId="");
    
    static void createInfoByUid(const string& uid);
    
    RoleInfoView(const string& uid):m_playerUid(uid),m_fromType(0) {};
    
    RoleInfoView(PlayerInfo* info,int fromType, string achievePro="", string medalCntStr="",string medalIconId=""):m_info(info),m_fromType(fromType),m_achievePro(achievePro),m_medalCntStr(medalCntStr),m_medalIconId(medalIconId),count(0){};
    ~RoleInfoView();
    void updateInfo();
    void hideButtons();
    
protected:
    // scrollview delegate
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
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onCloseClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onHeroCkick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onAllianceClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onMessageClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onGiftClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onBattle(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickPicBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void updatePlayerInfo(CCObject* obj);
    
    void onChangePic(CCObject* p);
    void updatePlayerName(CCObject* p);
    void enableScroll(CCObject* p);
    
    void inviteFunc(CCObject* param);
    void addMedalParticle(string medalId);
    
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<Label> m_nameTxt;
    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCLabelIF> m_killTxt;
    CCSafeObject<CCLabelIF> m_achieveTTF;
    CCSafeObject<CCLabelIF> m_medalTTF;
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCNode> m_midNode;
    CCSafeObject<CCNode> m_viewNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_otherNode;
//    CCSafeObject<CCScale9Sprite> m_expBar;
//    CCSafeObject<CCLabelIF> m_expTxt;
    CCSafeObject<CCLabelIF> m_levelTxt;
    CCSafeObject<CCLabelIF> m_serverTxt;
    CCSafeObject<CCSprite> m_medalIcon;
    CCSafeObject<CCNode> m_medalNode;
//    CCSafeObject<CCLabelIF> m_commonTTF;
    CCSafeObject<CCLabelIF> m_medalTitleTTF;
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCScrollView> m_scrollView;
    
    CCSafeObject<CCControlButton> m_picBtn;
    
    CCSafeObject<CCControlButton> m_btnAlliance;
    CCSafeObject<CCControlButton> m_msgBtn;
    CCSafeObject<CCControlButton> m_heroBtn;
    CCSafeObject<CCControlButton> m_btnBattle;
    CCSafeObject<PlayerInfo> m_info;
    
    CCSafeObject<CCNode> m_nameTouchNode;
    CCSafeObject<CCSprite> m_nameSpr;

    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<CCNode> m_starNode;
    CCSafeObject<CCNode> m_speStarNode;
    CCSafeObject<CCLabelIF> m_speStarTTF;
    
    CCSafeObject<CCNode> m_tipNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLabel;
    void showTipNode(string name,string des,CCTouch *pTouch);//显示提示
    void hideTipNode();//隐藏提示
    CCSafeObject<CCNode> m_medalUpNode;//隐藏徽章标题node
    CCSafeObject<CCNode> m_flgNode;//官职node
    CCSafeObject<CCControlButton> m_flgBtn;
    void onFlgBtnPressed(CCObject * pSender, CCControlEvent pCCControlEvent);
    string medalIconName;
    
    CCTableView* m_tabView;
    vector<int> m_data;
    
    string m_playerUid;
    string m_achievePro;
    string m_medalCntStr;
    string m_medalIconId;
    
    void onGetPlayerInfoCallback(CCObject* obj);

    int m_fromType;
    int m_cellHeight;
    void initPlayerStats();
    int count;
    string medalId;
    
};

class RoleCell :
public Layer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static RoleCell* create(std::string name,std::string value,int type);
    RoleCell(std::string name,std::string value,int type):m_name(name),m_value(value),m_type(type),m_isShowTip(false),m_touch(NULL),m_py(-1){ignoreAnchorPointForPosition(false);setAnchorPoint(Vec2(0,0));};
    
    std::string m_des;
    RoleCellDelegate* m_delegate;
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void showTip(float t);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_valueTxt;
    CCSafeObject<CCScale9Sprite> m_bg1;
    CCSafeObject<CCScale9Sprite> m_bg2;
    
    
    
    
     CCSafeObject<CCSprite> m_huaWen0;
     CCSafeObject<CCSprite> m_huaWen1;
//    CCSafeObject<CCNode> m_tipNode;
//    CCSafeObject<CCLabelIF> m_desName;
//    CCSafeObject<CCLabelIF> m_desLabel;
    
    int m_type;
    std::string m_name;
    std::string m_value;
    bool m_isShowTip;
    CCTouch *m_touch;
    int m_py;
};

class AttSiteCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
,public CCTableViewTouchIFCell
{
public:
    static AttSiteCell* create(int area, PlayerInfo* info);
    void setData(int area, PlayerInfo* info);
private:
    bool init(int area, PlayerInfo* info);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCScale9Sprite> m_itemBg;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_touchNode;
    
    int m_area;
    CCPoint m_startPoint;
    PlayerInfo* m_info;
};

class AttributeCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static AttributeCell* create(int attId, PlayerInfo* info);
    int m_attId;
private:
    bool init(int attId, PlayerInfo* info);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_valueLabel;
    
    PlayerInfo* m_info;
};

class AttInfoView :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
,public PopupBaseView
{
public:
    static AttInfoView* create(int attId);
    AttInfoView():m_attId(0),m_info(NULL){};
    int m_attId;
    
private:
    bool init(int attId);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCNode> m_picNode;
    
    PlayerInfo* m_info;
};

#endif /* defined(__IF__RoleInfoView__) */
