//
//  DTCursorTextField.h
//  IF
//
//  Created by fubin on 13-11-14.
//
//

#ifndef IF_DTCursorTextField_h
#define IF_DTCursorTextField_h

#include "cocos2d.h"

USING_NS_CC;

class DTCursorTextField: public CCTextFieldTTF////, public CCTouchDelegate
,public CCTextFieldDelegate
{
private:
    // 点击开始位置
    CCPoint m_beginPos;
    
    // 光标精灵
    CCSprite *m_pCursorSprite;
    
    // 光标动画
    CCAction *m_pCursorAction;
    
    // 光标坐标
    CCPoint m_cursorPos;
    
    // 是否加密显示
    bool isPsw;
    int m_limitNum;
    // 输入框内容
    //std::string *m_pInputText;
    CCSize m_designedSize;
    
public:
    DTCursorTextField();
    ~DTCursorTextField();
    
    // static，暂时不能使用
    //    static DTCursorTextField * textFieldWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
    
    /** creates a CCLabelTTF from a fontname and font size */
    static DTCursorTextField * textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);
    
    // CCLayer
    void onEnter();
    void onExit();
    
    virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated);
    
    // 初始化光标精灵
    void initCursorSprite(int nHeight);
    void setCursorColor(const ccColor3B& color3);
    // CCTextFieldDelegate
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF *pSender);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);
    
    // CCLayer Touch
    bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    // 判断是否点击在TextField处
    bool isInTextField(CCTouch *pTouch);
    // 得到TextField矩形
    CCRect getRect();
    
    // 打开输入法
    void openIME();
    // 关闭输入法
    void closeIME();
    
    //设置是否星号显示
    bool getIsPsw();
    void setIsPsw(bool bFlag);
    //设置字符长度限制，一个汉字三个字符
    void setLimitNum(int limitNum);
    int getLimitNum();
    //重载原函数，用来显示星号
    void setString(const char* displayTx, const char* inputTx);
    //点击弹出输入法的尺寸范围
    void setDesignedSize(CCSize size);
    CCSize getDesignedSize();
};

#endif
