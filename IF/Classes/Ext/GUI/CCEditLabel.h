//
//  CCEditLabel.h
//  aoe
//
//  Created by 岳文铎 on 13-3-18.
//
//

#ifndef __aoe__CCEditLabel__
#define __aoe__CCEditLabel__

#include "cocos-ext.h"
#include "CCBExtension.h"

#define  MSG_NativeSetString         "MNativeSetString"

NS_CC_EXT_BEGIN

class CCEditLabel
:
public CCNode,
public CCBSelectorResolver,
public CCBMemberVariableAssigner,
public CCIMEDelegate,
public CCTextFieldDelegate
//public CCKeypadDelegate
{
public:
    CCEditLabel():m_onFinish(NULL),m_onBeforeInput(NULL),m_inputMode(EditBox::InputMode::ANY),m_isPasswordMode(false),m_isSingleLine(true){};
    ~CCEditLabel(){CC_SAFE_RELEASE(m_onFinish);CC_SAFE_RELEASE(m_onBeforeInput);};
    static CCEditLabel * create(CCSize size, int numOfLines=1,CCPoint anchor=CCPointZero);
    virtual bool init(CCSize size, int numOfLines=1,CCPoint anchor=CCPointZero);
    void setInputMode(EditBox::InputMode mode);
    void setEnabled(bool isEnable);
    void enablePasswordMode(bool isEnable);
    void setSingleLine(bool isSingleLine);
    void setMaxLength(int length);      //设置最大字符长度
    virtual void setString(const char *label);
    virtual const char* getString(void);
    
    
    CCSafeObject<CCControlButton> m_btn;
    
    void setHorizontalAlignment(CCTextAlignment alignment);
    void setScale(float scale);
    CCScale9Sprite *bg;
    CCLabelTTF *m_label;
    CCSize m_size;
    
private:
    void handleNativeSetString(CCObject* obj);
    virtual void onEnter();
    virtual void onExit();
    virtual void keyBackClicked();//Android 返回键
    virtual void keyMenuClicked(){}//Android 菜单键
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info){};
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info){};
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info){};
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender){return false;}
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen){return false;}
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void updateTexture();
    void showEditView(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent);
    void keypressedBtnOK(CCObject * pSender, Control::EventType pCCControlEvent);
    CCSafeObject<CCControlButton> m_OKBtn;
    CCSafeObject<CCScale9Sprite> m_bg;
    
    CCSafeObject<CCEditBox> m_editBox;
    CCSafeObject<CCLayer> m_editView;
    CCSafeObject<CCNode> m_ccbNode;
    CCSafeObject<CCTextFieldTTF> m_textField;
    
    EditBoxInputMode m_inputMode;
    bool m_isPasswordMode;
    bool m_isSingleLine;
    std::string m_inputStr;
    CC_SYNTHESIZE_RETAIN(cocos2d::CCCallFuncO*, m_onFinish, FinishCallBack);
    // 显示输入框前的处理
    CC_SYNTHESIZE_RETAIN(cocos2d::CCCallFuncO*, m_onBeforeInput, BeforeInputCallBack);
    int m_editLabelType;
    int m_nCharLimit;   //文本长度限制
    int m_numOfLines;   //就0和1两种，0不限行数，1限一行
    
};

NS_CC_EXT_END


#endif /* defined(__aoe__CCEditLabel__) */
