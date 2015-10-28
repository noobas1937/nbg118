//
//  NewTroopsView.h
//  IF
//
//  Created by xxrdsg on 15-8-31.
//
//

#ifndef __IF__NewTroopsView__
#define __IF__NewTroopsView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class NewTroopsView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCScrollViewDelegate
{
public:
    static NewTroopsView* create();
    NewTroopsView(){};
    void refreshView(CCObject*);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
protected:
    
private:
    
    
    
    CCSafeObject<CCNode> m_listNode;
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCScrollView> m_scrollView;
};

class NewTroopsCell: public CCNode
, public CCBMemberVariableAssigner
{
public:
    static NewTroopsCell* create(string identity);
    NewTroopsCell(string identity): m_id(identity){};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
protected:
private:
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCLabelIF> m_levelLabel;
    CCSafeObject<CCNode> m_levelNode;
    CCSafeObject<CCLabelIF> m_soliderNameTxt;
    CCSafeObject<CCSprite> m_soliderBG;
    CCSafeObject<CCSprite> m_soliderTypeSprite;
    string m_id;
};

class NewTroopsBtn: public CCNode, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    CREATE_FUNC(NewTroopsBtn);
    virtual bool init();
    
    void onFormation1Click(CCObject *pSender, CCControlEvent event);
    void onFormation2Click(CCObject *pSender, CCControlEvent event);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
protected:
private:
};

class NewTroopsArmyTitle: public CCNode, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static NewTroopsArmyTitle* create();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
protected:
private:
    CCSafeObject<CCLabelIF> m_armyLabel;
    CCSafeObject<CCLabelIF> m_armyNum;
    CCSafeObject<CCLabelIF> m_marchLabel;
    CCSafeObject<CCLabelIF> m_marchNum;
    CCSafeObject<CCLabelIF> m_woundedLabel;
    CCSafeObject<CCLabelIF> m_woundedNum;
    CCSafeObject<CCLabelIF> m_foodSpdLabel;
    CCSafeObject<CCLabelIF> m_foodSpd;
    CCSafeObject<CCLabelIF> m_myTroopLabel;
};

class NewTroopsTrapTitle: public CCNode
, public CCBMemberVariableAssigner
{
public:
    static NewTroopsTrapTitle* create();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
protected:
private:
    CCSafeObject<CCLabelIF> m_tipLabel;
    CCSafeObject<CCLabelIF> m_trapLabel;
    CCSafeObject<CCLabelIF> m_trapNum;
};

#endif /* defined(__IF__NewTroopsView__) */
