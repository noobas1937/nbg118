//
//  MoveToView.h
//  IF
//
//  Created by 邹 程 on 14-3-20.
//
//

#ifndef __IF__MoveToView__
#define __IF__MoveToView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "WorldController.h"

class MoveToView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
,public CCEditBoxDelegate
{
public:
    static MoveToView* create();
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void keyPressedBtnGoto(CCObject * pSender, Control::EventType pCCControlEvent);

    virtual void editBoxReturn(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    
private:
    CCSafeObject<CCLabelIF> m_input;
    CCSafeObject<CCLabelIF> m_xLabel;
    CCSafeObject<CCLabelIF> m_yLabel;
    CCSafeObject<CCLabelIF> m_kingdom1;
    CCSafeObject<CCLabelIF> m_hintText;
    CCSafeObject<CCNode> m_xNode;
    CCSafeObject<CCNode> m_yNode;
    CCSafeObject<CCNode> m_zNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCControlButton> m_GotoBtn;
    CCEditBox* m_xEdit;
    CCEditBox* m_yEdit;
    CCEditBox* m_zEdit;
};
#endif /* defined(__IF__MoveToView__) */
