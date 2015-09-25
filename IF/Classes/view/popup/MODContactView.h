//
//  MODContactView.h
//  IF
//
//  Created by 李锐奇 on 14/11/19.
//
//

#ifndef __IF__MODContactView__
#define __IF__MODContactView__

#include "PopupBaseView.h"
#include "CommonInclude.h"
#include "CCTableViewTouchIFCell.h"
class MODContactView :
public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static MODContactView *create();
    
    virtual void changeFlagIcon(CCObject* lan);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void addBG();

    void onClickSendMail(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickIntro(CCObject * pSender, Control::EventType pCCControlEvent);
    void onGetModDataBack(CCObject *obj);
    CCSafeObject<CCControlButton> m_contactBtn;
    CCSafeObject<CCControlButton> m_howToBtn;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCLabelIF> m_introText;
    CCSafeObject<CCNode> m_bgContainer;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    
    CCSafeObject<CCNode> m_lanNode;
    CCSafeObject<CCControlButton> m_lanBtn;
    CCSafeObject<CCControlButton> m_mailBtn;
    void onClickLanBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickMailBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void getMODLanguageFromSever();
    void getMODLanguageFromSeverBack(CCObject *obj);
    CCArray *modLanArr;
};

class IntroPopUpView : public PopupBaseView
, public CCBMemberVariableAssigner
{
public:
    CREATE_FUNC(IntroPopUpView);
    
private:
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    void addText();
    void addBG();

    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCNode> m_container;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_bgContainer;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;

};

class MODLanguageView2 : public PopupBaseView
, public CCBMemberVariableAssigner
{
public:
    static MODLanguageView2 *create(CCArray *modLanArr);
    MODLanguageView2(CCArray *modLanArr):m_listData(modLanArr){};
    CCSafeObject<CCArray> m_listData;
private:
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    void addText();
    void addBG();
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCNode> m_container;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_bgContainer;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    
    CCSafeObject<CCLabelIF> m_subTitle;
    CCSafeObject<CCScale9Sprite> m_upSpr;
    CCSafeObject<CCScale9Sprite> m_libBg;
    CCSafeObject<CCNode> m_listNode;
};

class MODLanguageView:
public PopupBaseView,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
//,public CCTargetedTouchDelegate
{
public:
    static MODLanguageView* create();
    void setData();
    void touchEvent();
    MODLanguageView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void updateState();
    
    CCSafeObject<CCLabelIF> m_translationTitle;
    CCSafeObject<CCLabelIF> m_translationDes;
    CCSafeObject<CCScale9Sprite> m_listBG;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCArray> m_listData;
    
    unsigned int m_index;
    string m_languageType;
    CCPoint m_touchPoint;
};

class MODLanguageViewCell:
public CCTableViewTouchIFCell,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
//,public CCTargetedTouchDelegate
{
public:
    static MODLanguageViewCell* create(string str);
    void setData(string str);
//    string getStringByType(string str,int isName);
    void touchEvent();
    MODLanguageViewCell(string str):m_str(str){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void updateState();
    
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<Label> m_name;
    CCSafeObject<CCScale9Sprite> m_iconBG;
    
    unsigned int m_index;
    string m_str;
    string m_languageType;
    CCPoint m_touchPoint;
};

#endif /* defined(__IF__MODContactView__) */
