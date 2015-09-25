//
//  EnemyInfoMoreView.h
//  IF
//
//  Created by fubin on 14-2-24.
//
//

#ifndef IF_EnemyInfoMoreView_h
#define IF_EnemyInfoMoreView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class EnemyMsgCell;
class ItemMsgCell;
class ResUpTitleCell;

class EnemyInfoMoreView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static EnemyInfoMoreView* create(int keyId, int buildId);
    EnemyInfoMoreView():m_keyId(0),m_buildId(0){};
    virtual ~EnemyInfoMoreView(){};
    void updateInfo();
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int keyId, int buildId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCScrollView> m_scrollView;
    
    int m_keyId;
    int m_buildId;
};

class EnemyMsgCell:
public Layer
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static EnemyMsgCell* create(int type, string msg, int idx);
    EnemyMsgCell():m_type(0),m_msg(""),m_idx(0){_ignoreAnchorPointForPosition = false;setAnchorPoint(Vec2(0,0));};
    void setTouchNode(CCNode* node);
private:
    bool init(int type, string msg, int idx);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    int m_type;
    string m_msg;
    int m_idx;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCSprite> pic;
};

class ItemMsgCell:
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ItemMsgCell* create(string picName, string msg, int type=0, int idx=0);
    ItemMsgCell():m_type(0),m_idx(0),m_ablityType(""){};
private:
    bool init(string picName, string msg, int type=0, int idx=0);
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onClickBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCControlButton> m_btn;
    int m_type;
    int m_idx;
    string m_ablityType;
};

class ItemMsgCell2:
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ItemMsgCell2* create(string picName, string msg, int type=0, int idx=0);
    ItemMsgCell2():m_type(0),m_idx(0),m_ablityType(""){};
private:
    bool init(string picName, string msg, int type=0, int idx=0);
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onClickBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCControlButton> m_btn;
    int m_type;
    int m_idx;
    string m_ablityType;
};

class ResUpTitleCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ResUpTitleCell* create(string msg);
    ResUpTitleCell(){};
private:
    bool init(string msg);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_titleLabel;
};

#endif
