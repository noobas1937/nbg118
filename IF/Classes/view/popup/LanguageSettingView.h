//
//  LanguageSettingView.h
//  IF
//
//  Created by lifangkai on 14-9-28.
//
//

#ifndef __IF__LanguageSettingView__
#define __IF__LanguageSettingView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
class LanguageSettingView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static LanguageSettingView* create();
    static string getLanguageIconByType(string type);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual ~LanguageSettingView();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
//    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell cell);
//	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
//    virtual CCTableViewCell gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
//    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
//    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
//    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    void onTimer(float dt);
    void setDataList();
    unsigned int Setting_num_notGM = 5;
    CCSafeObject<CCLabelIF> m_tips;
    CCSafeObject<CCLabelIF> m_timeTitleText;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCScale9Sprite> m_popupBG;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCTableView> m_tabView;
    
    CCSafeObject<CCArray>m_data;
};

class LanguageSettingCell:
public CCTableViewTouchIFCell,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
//,public CCTargetedTouchDelegate
{
public:
    static LanguageSettingCell* create(string str);
    void setData(string str);
    void touchEvent();
    LanguageSettingCell(string str):m_str(str){};
    void cancelChangeLan();
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
    string m_languageCode;
    CCPoint m_touchPoint;
};

class LanguageSettingAndTranslationCell:
public CCTableViewTouchIFCell,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
//,public CCTargetedTouchDelegate
{
public:
    static LanguageSettingAndTranslationCell* create(string str);
    void setData(string str);
    void touchEvent();
    LanguageSettingAndTranslationCell(string str):m_str(str){};
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
    string m_str;
    string m_languageType;
    CCPoint m_touchPoint;
};

class LanguageTranslationCell:
public CCTableViewTouchIFCell,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
//,public CCTargetedTouchDelegate
{
public:
    static LanguageTranslationCell* create(CCDictionary* dict);
    void setData(CCDictionary* dict);
    void touchEvent();
    LanguageTranslationCell(CCDictionary* dict):m_itemDict(dict){};
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
    
    CCSafeObject<Label> m_nameText;
    CCSafeObject<CCSprite> m_touchBtnOpenBg;
    CCSafeObject<CCSprite> m_touchBtnCloseBg;
    CCSafeObject<CCSprite> m_touchBtn;
    CCSafeObject<CCDictionary> m_itemDict;
    
    unsigned int m_index;
    string m_str;
    string m_languageType;
    CCPoint m_touchPoint;
};

#endif /* defined(__IF__LanguageSettingView__) */
