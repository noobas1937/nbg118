//
//  TestFeedBackView.h
//  IF
//
//  Created by chenfubi on 15/2/9.
//
//

#ifndef __IF__TestFeedBackView__
#define __IF__TestFeedBackView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "WaitInterface.h"
#include "InputFieldMultiLine.h"
#include "ChatServiceMessageListener.h"
#include "CCTableViewTouchIFCell.h"
class TestFeedBackView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
, public CCTableViewDataSource
, public DefaultTableViewDelegate
, public CCEditBoxDelegate
{
public:
	virtual bool init();
	static TestFeedBackView* create();
    TestFeedBackView():m_waitInterFace(NULL){};
    virtual ~TestFeedBackView(){};
    
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    
protected:
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);

private:
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
//    CCSize getTextDimension(const char *text);
    void initData(CCObject* pObj);
    void onGameTick(float delay);
    void dataChange(CCObject* pObj);
    
    void onOkClick(CCObject* pSender, CCControlEvent event);
    void onCloseBtnClick(CCObject* pSender, CCControlEvent event);
    CCSize getTextDimension(const char *text);
    
private:
    CCSafeObject<CCNode> m_viewNode;
    CCSafeObject<CCNode> m_listNode;
//    CCSafeObject<CCNode> m_midBGNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_midNode;
    CCSafeObject<CCNode> m_serverNode;
    CCSafeObject<CCNode> m_rawNode;
    CCSafeObject<CCLabelIF> m_serverLabel;
    CCSafeObject<CCLabelIF> m_rawLabel;
    CCSafeObject<CCLabelIF> m_titleTTF;
    CCSafeObject<CCLabelIF> m_infoTTF;
    CCSafeObject<CCScale9Sprite> m_bg1;
    CCSafeObject<CCScale9Sprite> m_bg2;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCEditBox> m_serverEditBox;
    CCSafeObject<CCEditBox> m_rawEditBox;
    CCSafeObject<CCArray> m_data;
    
    WaitInterface* m_waitInterFace;
    
    int m_editBoxX;
};

//class TestFeedBackCell
class TestFeedBackCell
:public CCBSelectorResolver
,public CCBMemberVariableAssigner
//,public CCTouchDelegate
,public CCTableViewTouchIFCell
{
public:
    static TestFeedBackCell* create(CCDictionary* dict,CCNode* pNode);
    TestFeedBackCell():m_canTouch(true){};
    virtual ~TestFeedBackCell(){};
    
    void setData(CCDictionary* dict);
    void isEdit(bool isEdit);
private:
    bool init(CCDictionary* dict,CCNode* pNode);
    
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void notifyBack(CCObject* pObj);
    void dataChange(CCObject* pObj);
    void dataChangeBG(CCObject* pObj);
private:
    CCSafeObject<CCLabelIF> m_infoTTF1;
    CCSafeObject<CCLabelIF> m_infoTTF2;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_parentNode;
    CCSafeObject<CCScale9Sprite> m_sprBG;
    bool m_canTouch;
    int m_index;
    string m_content;
    string m_suggestion;
};

//class TestEditView
#include "InputFieldMultiLine.h"
class TestEditView
: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    virtual bool init(int index, string suggestion);
    static TestEditView* create(int index, string suggestion);
    TestEditView():m_index(-1),m_suggestion(""),m_oldSuggerstion(""){};
    virtual ~TestEditView(){};
    
protected:
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onOkClick(CCObject* pSender, CCControlEvent event);
    void onAddlineEvent(CCObject * params);
    void onInputFieldCloseEvent(CCObject * params);
    
private:
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCScale9Sprite> m_bg;
    
    CCSafeObject<InputFieldMultiLine> m_editBox;
    
    int m_index;
    string m_suggestion;
    string m_oldSuggerstion;
};
#endif /* defined(__IF__TestFeedBackView__) */
