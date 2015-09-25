//
//  InputFieldMultiLineMultiLine.h
//  COK
//
//  Created by nosources on 14-4-23.
//
//

#ifndef __COK__InputFieldMultiLine__
#define __COK__InputFieldMultiLine__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;
#define INPUTFIELD_OPEN_EVENT "InputFieldMultiLine.open"
#define INPUTFIELD_CLOSE_EVENT "InputFieldMultiLine.close"
class InputFieldMultiLine : public cocos2d::Layer
, public CCTextFieldDelegate
////, public CCTouchDelegate
, public CCIMEDelegate
{
public:
    InputFieldMultiLine();
    virtual ~InputFieldMultiLine();
    static InputFieldMultiLine*  create(CCSize size, std::string bgSpriteName, int fontSize);
    bool init(CCSize size, std::string bgSpriteName, int fontSize);
    void resize(CCSize size);
    void changeBG(std::string bgImg);
    void initCursorSprite(int nHeight);
    int getStringNum();
    
    // CCTextFieldDelegate
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF *pSender);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, size_t nLen);

    bool insertOneChar(const char* text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, size_t nLen);
    
//    virtual void keyBackClicked();//Android 返回键
//    virtual void keyMenuClicked(){}//Android 菜单键
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info){};
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info){};
    virtual void onExit();
    virtual void onEnter();
    // CCLayer Touch
    bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CC_SYNTHESIZE(int, m_id, TestId);
    CC_SYNTHESIZE(bool, m_moveFlag, MoveFlag);
    CC_SYNTHESIZE(std::string, m_defaultText, DefaultText);
    CC_SYNTHESIZE(bool, m_calCharLen, calCharLen);
    CC_SYNTHESIZE(bool, m_onlySingleLine, OnlySingleLine);
    void setSwallowsTouches(bool _swall);
    bool m_bSwallowsTouches;
    
    bool m_enabled;
    void setEnabled(bool flag);
    bool getEnabled();
    
    void setStringFromVectorOrFromInputString();
    void resetCursorPosition();
    CCRect getViewRect();
    void beforeDraw();
    void afterDraw();
//    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags);
    
    // 打开输入法
    void openIME();
    // 关闭输入法
    void closeIME();
    // 检查输入法开启状态
    void checkIME();
    
    void setFontColor(const ccColor3B& color);
    CCSize getTextDimension(const char* pText);
    CC_PROPERTY(int, mLineNumber, LineNumber);
    void setShowLineNum(int num);
    std::string getText();
    void addTargetedDelegate();
    void setSingleLineForLabel(CCLabelTTF* label , std::string tosetString);
    void setMaxChars(int i);
    void setText(std::string text);
    void setPlaceHolder(std::string text);
    void addLine();
    void delLine();
    
    CC_PROPERTY(int, m_touchPriority, TouchPriority);
    void resetInitBG();
    bool getIsEditing();
    void setAddH(int h);
    void setBgVisible(bool v);
private:
    bool isEditing;
    int m_bakLineNumber;
    int maxChars;
    int m_showLineNum;
    float adjustHeight;
    CCSprite *m_pCursorSprite;
    CCAction *m_pCursorAction;
    CCTextFieldTTF* mTextFieldTTF;
    Scale9Sprite* bg;
    std::string m_pInputText;
    bool m_bScissorRestored;
    CCRect m_tParentScissorRect;
    
    int mFontSize;
    std::string nowLine;
    std::vector<std::string> mVectorInput;
    std::vector<cocos2d::CCLabelTTF*> mLabels;
    cocos2d::CCLabelTTF* mBeingEditLabel;
    int m_addH;

//    bool m_bTouchEnabled;
//    void setTouchEnabled(bool enable, bool force = false);
//    void registerWithTouchDispatcher();
};

#endif /* defined(__COK__InputFieldMultiLine__) */
