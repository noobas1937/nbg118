//
//  MainCityMoreView.h
//  IF
//
//  Created by fubin on 14-2-26.
//
//

#ifndef IF_MainCityMoreView_h
#define IF_MainCityMoreView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class ResMoreInfoCell;
class TmpInfoCell;
class TmpMsgCell;
class TmpBuildCell;

class MainCityMoreView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static MainCityMoreView* create(int buildId, int type);
    MainCityMoreView():m_buildId(0),m_type(0){};
    virtual ~MainCityMoreView(){};
    void updateInfo();
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int buildId, int type);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual void doWhenClose();
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_buildBG2;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScrollView> m_scrollView;
    
    int m_buildId;
    int m_type;
};

class ResMoreInfoCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ResMoreInfoCell* create(int restype, string msg, int value, int effValue);
    ResMoreInfoCell(){};
private:
    bool init(int restype, string msg, int value, int effValue);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_valueLabel;
    CCSafeObject<CCLabelIF> m_effLabel;
    CCSafeObject<CCLabelIF> m_hLabel;
    CCSafeObject<CCNode> m_picNode;
};

class TmpInfoCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TmpInfoCell* create(string msg1, string msg2, float value, float sum);
    TmpInfoCell(){};
private:
    bool init(string msg1, string msg2, float value, float sum);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_msg1Label;
    CCSafeObject<CCLabelIF> m_msg2Label;
    CCSafeObject<CCScale9Sprite> m_bar;
    CCSafeObject<CCScale9Sprite> m_bgFrame;
};

class TmpMsgCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TmpMsgCell* create(string msg1, string msg2);
    TmpMsgCell(){};
private:
    bool init(string msg1, string msg2);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_msg1Label;
    CCSafeObject<CCLabelIF> m_msg2Label;
};

class TmpBuildCell :
public Layer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
{
public:
    static TmpBuildCell* create(int buildItem, float effValue);
    void setTouchNode(CCNode* touchNode);
    TmpBuildCell():m_buildId(0),m_startY(0){ _ignoreAnchorPointForPosition = false;};
private:
    bool init(int buildItem, float effValue);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    CCSafeObject<CCNode> m_normalNode;
    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCNode> m_effNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCLabelIF> m_valueLabel;
    CCSafeObject<CCLabelIF> m_addHLabel;
    CCSafeObject<CCLabelIF> m_valueAddLabel;
    
    CCSafeObject<CCLabelIF> m_curLvLabel;
    CCSafeObject<CCLabelIF> m_nextLvLabel;
    CCSafeObject<CCLabelIF> m_curValueLabel;
    CCSafeObject<CCLabelIF> m_nextValueLabel;
    
    CCSafeObject<CCLabelIF> m_efflvLabel;
    CCSafeObject<CCLabelIF> m_effValueLabel;
    CCSafeObject<CCLabelIF> m_effValue2Label;
    CCSafeObject<CCLabelIF> m_tmp1Label;
    CCSafeObject<CCLabelIF> m_hLabel;
    
    CCSafeObject<CCScale9Sprite> m_bg;
    CCNode* m_touchNode;
    int m_buildId;
    int m_startY;
};

#endif
