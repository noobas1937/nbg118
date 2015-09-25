//
//  CCEditText.h
//  IF
//
//  Created by ganxiaohua on 14-5-4.
//
//

#ifndef __IF__CCEditText__
#define __IF__CCEditText__

#include "cocos-ext.h"
#include "CCBExtension.h"

#define  MSG_NativeTextSetString         "MNativeSetTextString"

NS_CC_EXT_BEGIN

class CCEditText
:
public Layer,
public CCBSelectorResolver,
public CCBMemberVariableAssigner,
//public CCTargetedTouchDelegate,
public CCIMEDelegate,
public CCTextFieldDelegate,
//public CCKeypadDelegate,
public cocos2d::extension::EditBoxDelegate
{
public:
    CCEditText():m_onFinish(NULL),m_onBeforeInput(NULL),m_inputMode(EditBox::InputMode::ANY),m_isPasswordMode(false),m_editView(NULL),m_needInputBox(false),m_keyBoardH(220),m_iskeyBoardShow(false){_ignoreAnchorPointForPosition = false;};
    ~CCEditText(){
        CC_SAFE_RELEASE(m_onFinish);
        CC_SAFE_RELEASE(m_onBeforeInput);
    };
    static CCEditText * create(CCSize size, const char *str ,bool multiLine=false,bool needInputBox=false);
    virtual bool init(CCSize size, const char *str ,bool multiLine=false,bool needInputBox=false);
    void setInputMode(EditBoxInputMode mode);
    void setEnabled(bool isEnable);
    void enablePasswordMode(bool isEnable);
    void setSingleLine(bool isSingleLine);
    void setMaxLength(int length);      //设置最大字符长度
	void setFontName(const char* pFontName);
	void setFontSize(int fontSize);
    void setFontColor(const ccColor3B& color);
    void setInputFlag(EditBox::InputFlag inputFlag);
    void setReturnType(EditBox::KeyboardReturnType returnType);
    void destory();
    
    virtual void setString(const char *label);
    virtual const char* getString(void);
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCScale9Sprite> m_clickBg;
    
    void setHorizontalAlignment(CCTextAlignment alignment);
    void setScale(float scale);
    CCSize m_size;
    CCSafeObject<CCEditBox> m_contentBox;
    CCSafeObject<CCLabelTTF> m_label;
    CCSafeObject<CCScale9Sprite> bg;
    
    void setText(const char* pText);
    const char* getText(void);
private:
    void handleNativeSetString(CCObject* obj);
    virtual void onEnter();
    virtual void onExit();
 //   virtual void keyBackClicked();//Android 返回键
 //   virtual void keyMenuClicked(){}//Android 菜单键
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info){};
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info){};
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender){return false;}
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    
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
    CCSafeObject<CCLayer> m_andriodEdit;
    CCSafeObject<CCLabelTTF> inputTxt;
    
    EditBoxInputMode m_inputMode;
    bool m_isPasswordMode;
    bool m_multiLine;
    bool m_needInputBox;
    bool m_iskeyBoardShow;
    std::string m_inputStr;
    CC_SYNTHESIZE_RETAIN(cocos2d::CCCallFuncO*, m_onFinish, FinishCallBack);
    // 显示输入框前的处理
    CC_SYNTHESIZE_RETAIN(cocos2d::CCCallFuncO*, m_onBeforeInput, BeforeInputCallBack);
    int m_editLabelType;
    int m_nCharLimit;   //文本长度限制
    int m_numOfLines;   //就0和1两种，0不限行数，1限一行
    int m_keyBoardH;
    bool m_haveIME;
    bool m_isTextEnable;
    CCPoint m_beginPos;
    
    std::string m_strFontName;    
    int m_nFontSize;
    ccColor3B m_colText;
};

NS_CC_EXT_END
#endif /* defined(__IF__CCEditText__) */
