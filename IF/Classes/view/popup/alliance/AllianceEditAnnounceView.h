//
//  AllianceEditAnnounceView.h
//  IF
//
//  Created by ganxiaohua on 14-8-20.
//
//

#ifndef __IF__AllianceEditAnnounceView__
#define __IF__AllianceEditAnnounceView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "InputFieldMultiLine.h"

class AllianceEditAnnounceView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    ~AllianceEditAnnounceView(){};
    AllianceEditAnnounceView(std::string content,bool edit):m_content(content),m_canEdit(edit){};
    static AllianceEditAnnounceView *create(std::string content,bool edit=false);
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickClose(CCObject * pSender, Control::EventType pCCControlEvent);
    void success(CCObject* p);
    
    CCSafeObject<CCControlButton> m_btnEdit;
    CCSafeObject<CCControlButton> m_btnSave;
    CCSafeObject<CCNode> m_textNode;
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<CCLabelIFTTF> m_announceTxt;
    CCSafeObject<InputFieldMultiLine> m_contentEditBox;
    std::string m_content;
    bool m_canEdit;
    bool m_first;
};
#endif /* defined(__IF__AllianceEditAnnounceView__) */
