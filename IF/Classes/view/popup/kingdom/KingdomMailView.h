//
//  KingdomMailView.h
//  IF
//
//  Created by 张军 on 13-10-11.
//
//

#ifndef IF_KingdomMailView_h
#define IF_KingdomMailView_h

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
//#include "CCEditText.h"
#include "InputFieldMultiLine.h"

class KingdomMailView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static KingdomMailView* create(std::string picStr="", std::string allStr="",std::string nameStr="");
private:
    KingdomMailView(std::string picStr, std::string allStr, std::string nameStr) : m_picStr(picStr), m_allStr(allStr),m_nameStr(nameStr),mInited(false){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);

    void onCloseEvent(CCObject* p);
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void onSendClick(CCObject *pSender, CCControlEvent event);
    void onUpdateStatusEvent(CCObject* p);
    void updateBtnStatus();
    std::string m_picStr;
    std::string m_allStr;
    std::string m_nameStr;
    CCSafeObject<CCControlButton> m_sendBtn;
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCNode> m_contentContainer;
    CCSafeObject<CCLabelIF> m_contentText;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<InputFieldMultiLine> m_titleEditBox;
    CCSafeObject<InputFieldMultiLine> m_contentEditBox;
    
    CCSafeObject<CCSprite> m_sprMailIcon;
    
    CCSafeObject<CCNode> m_listNode;
    bool mInited;
    
    CCSafeObject<CCScale9Sprite> m_sprDownBorder;
    
    CCSafeObject<CCNode> m_kingNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_allianceTxt;
    CCSafeObject<Label> m_playerTxt;
    
    std::string defaultText;
};

#endif
