//
//  BuildUpgradeView.h
//  IF
//
//  Created by fubin on 14-5-16.
//
//

#ifndef IF_BuildUpgradeView_h
#define IF_BuildUpgradeView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "ArcPopupBaseView.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"
#include "CCIFTouchNode.h"
class UpgradeCell;
class UpgradeTitleCell;

class BuildUpgradeView: public ArcPopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static BuildUpgradeView* create(int buildId, int pos=0);
    BuildUpgradeView():m_buildId(0),m_pos(0),_tmpGold(0),_resGold(0),_tmpTime(0),m_openNum(0),m_qid(QID_MAX),m_rentTime(0){};
    virtual ~BuildUpgradeView(){ m_itemScrollView->getContainer()->removeAllChildren(); };
    CCNode *getGuideNode(int itemId);
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool init(int buildId, int pos=0);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCreateOrUpClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onInstantClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onOkUp();
    void onLastUp();
    void showWarningInstantUp();
    void onOkInstantUp();
    void onGotoSpdView();
    
    void updateInfo(CCObject* obj);
    void updateParasInfo();
    
    void onShowNextUnlockItem();
    
    void onYesRent();
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCControlButton> m_instantBtn;
    CCSafeObject<CCControlButton> m_upBtn;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCLabelIF> m_demandLabel;
    
    CCSafeObject<CCNode> m_inBtnNode;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_nextLabel;
    CCSafeObject<CCLabelIF> m_nextLvLabel;
    CCSafeObject<CCLabelIF> m_nextMsgLabel;
    CCSafeObject<CCLabelIF> m_upBtnMsgLabel;
    
    CCSafeObject<CCNode> m_para1Node;
    CCSafeObject<CCLabelIF> m_para1Name;
    CCSafeObject<CCLabelIF> m_para1Num;
    CCSafeObject<CCLabelIF> m_next1Num;
    
    CCSafeObject<CCNode> m_para2Node;
    CCSafeObject<CCLabelIF> m_para2Name;
    CCSafeObject<CCLabelIF> m_para2Num;
    CCSafeObject<CCLabelIF> m_next2Num;
    
    CCSafeObject<CCNode> m_para3Node;
    CCSafeObject<CCLabelIF> m_para3Name;
    CCSafeObject<CCLabelIF> m_para3Num;
    CCSafeObject<CCLabelIF> m_next3Num;
    
    CCSafeObject<CCNode> m_para4Node;
    CCSafeObject<CCLabelIF> m_para4Name;
    CCSafeObject<CCLabelIF> m_para4Num;
    CCSafeObject<CCLabelIF> m_next4Num;
    
    CCSafeObject<CCNode> m_iconList;
    CCSafeObject<CCLabelIF> m_unlockLable;
    CCSafeObject<CCNode> m_arrowNode;
    CCSafeObject<CCSprite> m_rArrow;
    CCSafeObject<CCSprite> m_lArrow;
    
    int m_buildId;
    int m_pos;
    bool m_isReturn;
    int _tmpGold;
    int _resGold;
    int _tmpTime;
    CCSafeObject<CCScrollView> m_scrollView;
    CCScrollView* m_itemScrollView;
    int m_openNum;
    int m_qid;
    int m_rentTime;
    
    int m_lfood;
    int m_lwood;
    int m_liron;
    int m_lstone;
    
    int m_unlockItemWidth;
    int m_sumItemWidth;
    int m_x;
    
    vector<string> willUnlockItems;
};

class UpgradeCell :
public Layer
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static UpgradeCell* create(int type, int itemId, int value, bool isShow=false);
    UpgradeCell():m_isOk(true),m_type(0),m_itemId(0),m_value(0)
    ,m_buildId(0),m_gotoBuildId(0),m_parentType(0),m_createInfo(""),tmpNum(0),tmpTime(0),m_tmpName(""),m_isShow(false){_ignoreAnchorPointForPosition = false; setAnchorPoint(Vec2(0,0));};
    void setTouchNode(CCNode* node, int buildId, int parentType=0);
    void setCreateBuildInfo(int buildId, int pos);
    bool m_isOk;
private:
    bool init(int type, int itemId, int value, bool isShow=false);
    virtual void onEnter();
    virtual void onExit();
    
    void refresh();
    void onEnterFrame(float dt);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void spdCallBack();
    void spdCallBackDragon();
    void rectCallBack(CCObject* obj);
    
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_valueLabel;
    CCSafeObject<CCLabelIF> m_value2Label;
    CCSafeObject<CCSprite> m_yesSprite;
    CCSafeObject<CCSprite> m_noSprite;
    
    CCSafeObject<CCScale9Sprite> m_btn;
    CCSafeObject<CCScale9Sprite> m_rectPic;
    CCSafeObject<CCNode> m_touchBtn;
    CCSafeObject<CCLabelIF> m_btnLabel;
    
    int m_type;
    int m_itemId;
    int m_value;
    int m_buildId;
    int m_gotoBuildId;
    int m_parentType;
    string m_createInfo;
    CCNode* m_touchNode;
    int tmpNum;
    int tmpTime;
    string m_tmpName;
    bool m_isShow;
};

class UpgradeTitleCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static UpgradeTitleCell* create(string msg);
    UpgradeTitleCell(){};
private:
    bool init(string msg);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_titleLabel;
};

class UnLockItemCell :
public CCIFTouchNode
{
public:
    static UnLockItemCell* create(int type, string itemId, CCNode* node);
    UnLockItemCell():m_type(0),m_itemId(""),m_touchNode(NULL){};
private:
    bool init(int type, string itemId, CCNode* node);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    int m_type;
    string m_itemId;
    string m_iconPath;
    int startX;
    CCNode* m_touchNode;
};

class UnlockItemInfoView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static UnlockItemInfoView* create(string name, string des, string iconPath);
    UnlockItemInfoView(){};
    ~UnlockItemInfoView(){};
private:
    bool init(string name, string des, string iconPath);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler  onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_bcNode;
    CCSafeObject<CCNode> m_bottomNode;
};

#endif
