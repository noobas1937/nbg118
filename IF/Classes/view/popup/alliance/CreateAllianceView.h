//
//  CreateAllianceView.h
//  IF
//
//  Created by chenliang on 14-3-25.
//
//

#ifndef __IF__CreateAllianceView__
#define __IF__CreateAllianceView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class CreateAllianceView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCEditBoxDelegate
{
public:
    static CreateAllianceView* create(float operateAreaHeight);
    CreateAllianceView(float operateAreaHeight):m_operateAreaHeight(operateAreaHeight){};
    
    virtual ~CreateAllianceView(){}
    
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCreateAllianceBtnClick(CCObject *pSender, CCControlEvent event);
    void onChooseAllianceSymbolBtnClick(CCObject *pSender, CCControlEvent event);
    void onChooseAllianceLanguageBtnClick(CCObject *pSender, CCControlEvent event);
    void ontClickCreate(CCObject *pSender, CCControlEvent event);
    void onCancelClick(CCObject *pSender, CCControlEvent event);
    void onNameBgClick(CCObject *pSender, CCControlEvent event);
    
    void onCheckAllianceName(CCObject* data);
    void onCheckAllianceTag(CCObject* data);
    void onCreateAlliance(CCObject* data);
    void createSuccess();
    void createFail(CCObject* obj);
    void iconSelected(CCObject* data);
    void languageSelected(CCObject* data);
    void checkName(float _time);
    void addLoadingAni();
    string setDefaultAllianceLanguge();
    
    CCSafeObject<CCLabelIF> m_allianceNameAvailableLettersLabel;
    
    CCSafeObject<CCEditBox> m_allianceNameEditBox;
    CCSafeObject<CCEditBox> m_allianceIntroEditBox;
    
    CCSafeObject<CCLabelIF> m_step1TitleLabel;
    CCSafeObject<CCLabelIF> m_step1ContentLabel;
    CCSafeObject<CCLabelIF> m_allianceNameTagAvailableLettersLabel;

    CCSafeObject<CCEditBox> m_allianceNameTagEditBox;
    CCSafeObject<CCLabelIF> m_step2TitleLabel;
    CCSafeObject<CCLabelIF> m_step2ContentLabel;
    
    CCSafeObject<CCLabelIF> m_step3TitleLabel;
    CCSafeObject<CCLabelIF> m_browseSymbolBtnLabel;

    CCSafeObject<CCLabelIF> m_step4TitleLabel;
    CCSafeObject<CCLabelIF> m_step4CurrentLanguageLabel;
    CCSafeObject<CCLabelIF> m_chooseAllianceLanguageBtnLabel;
    CCSafeObject<CCLabelIF> m_step4ContentLabel;
    
    CCSafeObject<CCLabelIF> m_step5TitleLabel;
    CCSafeObject<CCLabelIF> m_createAllianceBtnLabel;
    CCSafeObject<CCLabelIF> m_step5ContentLabel;
    
    CCSafeObject<CCNode> m_nameOkNode;
    CCSafeObject<CCNode> m_nameNode;
    CCSafeObject<CCNode> m_tagOkNode;
    
    CCSafeObject<CCNode> m_symbolPlaceHolder;
    CCSafeObject<CCNode> m_step1Node;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCControlButton> m_btnCreate;
    CCSafeObject<CCLabelIF> m_allianceBtnTitle;
    CCSafeObject<CCLabelIF> m_createBtnCost;
    CCSafeObject<CCControlButton> m_btnCancel;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_unionTitleTxt;
    CCSafeObject<CCLabelIF> m_enterNameTxt;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCSprite> m_iconGold;
    CCSafeObject<CCLabelIF> m_cancleLabel;
    
    CC_SYNTHESIZE(string, m_name, Name);
    CC_SYNTHESIZE(string, m_tag, AllianceTag);
    CC_SYNTHESIZE(string, m_symbol, Symbol);
    CC_SYNTHESIZE(string, m_language, Language);
    
    std::string lastName;
    float m_operateAreaHeight;
};

#endif /* defined(__IF__CreateAllianceView__) */
