//
//  SuggestionView.h
//  IF
//
//  Created by fubin on 13-11-14.
//
//

#ifndef IF_SuggestionView_h
#define IF_SuggestionView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
////#include "CCScrollView.h"
#include "CCLabelIF.h"
#include "CCEditLabel.h"
#include "CCEditText.h"
#include "DTCursorTextField.h"

class SuggestionView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static SuggestionView* create();
    SuggestionView(){};
    virtual ~SuggestionView();

    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
//    void onEnterFrame(float dt);
    void setBeforeShowEditText();
    void onPostBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCloseView(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelIF> m_tips;
    CCSafeObject<CCLabelIF> m_titleName1;
    CCSafeObject<CCNode> m_editNode;
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCControlButton> m_sendBtn;
    
    CCSafeObject<CCEditText> m_editBox;
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCScale9Sprite> m_BG;
    CCSafeObject<CCScale9Sprite> m_infoBG;
    CCSafeObject<CCEditLabel> m_editLabel;
    
    CCSafeObject<CCTextFieldTTF> m_textFieldTTF;
    
    CCSafeObject<DTCursorTextField> m_dtTextField;
};

#endif /* defined(__IF__ChatView__) */
