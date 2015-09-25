//
//  DTCursorTextField.cpp
//  IF
//
//  Created by fubin on 13-11-14.
//
//

#include "DTCursorTextField.h"

static int _calcCharCount(const char * pszText)
{
    int n = 0;
    char ch = 0;
    while ((ch = *pszText))
    {
        CC_BREAK_IF(! ch);
        
        if (0x80 != (0xC0 & ch))
        {
            ++n;
        }
        ++pszText;
    }
    return n;
}

const static float DELTA = 20.0f;

DTCursorTextField::DTCursorTextField()
{
    CCTextFieldTTF();
    
    m_pCursorSprite = NULL;
    m_pCursorAction = NULL;
    
    _inputText = "";
    isPsw = false;
    m_limitNum = 1024;
}

DTCursorTextField::~DTCursorTextField()
{
    //delete m_pInputText;
}

void DTCursorTextField::onEnter()
{
    CCTextFieldTTF::onEnter();
//TODO
//    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
//    setTouchEnabled(true);
    ////CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, false);
    this->setDelegate(this);
}
//静态生成函数
DTCursorTextField * DTCursorTextField::textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize)
{
    DTCursorTextField *pRet = new DTCursorTextField();
    
    if(pRet && pRet->initWithPlaceHolder("", fontName, fontSize))
    {
        pRet->autorelease();
        if (placeholder)
        {
            pRet->setPlaceHolder(placeholder);
        }
        pRet->initCursorSprite(fontSize);
        
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    
    return NULL;
}

void DTCursorTextField::initCursorSprite(int nHeight)
{
    // 初始化光标
    int column = 4;
    int pixels[nHeight][column];
    for (int i=0; i<nHeight; ++i) {
        for (int j=0; j<column; ++j) {
            pixels[i][j] = 0x00ff00ff;
        }
    }
    auto dataLen = column * nHeight * 4;
    CCTexture2D *texture = new CCTexture2D();
    texture->initWithData(pixels, dataLen,kCCTexture2DPixelFormat_RGB888, column, nHeight, CCSizeMake(column, nHeight));
//    CCTexture2D *texture = new CCTexture2D();
//    texture->initWithData(pixels, kCCTexture2DPixelFormat_RGB888, 1, 1, CCSizeMake(column, nHeight));
    
    m_pCursorSprite = CCSprite::create();
    m_pCursorSprite->initWithTexture(texture);
    CCSize winSize = getContentSize();
    m_cursorPos = ccp(0, winSize.height / 2);
    m_pCursorSprite->setPosition(m_cursorPos);
    this->addChild(m_pCursorSprite);
    
    m_pCursorAction = CCRepeatForever::create((CCActionInterval *) CCSequence::create(CCFadeOut::create(0.25f), CCFadeIn::create(0.25f), NULL));
    
    m_pCursorSprite->runAction(m_pCursorAction);
    
    _inputText = "";//new std::string();
}

bool DTCursorTextField::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_beginPos = pTouch->getLocationInView();
    m_beginPos = CCDirector::sharedDirector()->convertToGL(m_beginPos);
    
    return true;
}

void DTCursorTextField::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
{
    ccDrawColor4F( 255, 0, 0, 1 );
    //设置绘制线条的宽度
    glLineWidth(10);
    ccDrawLine(ccp(0, 0), ccp(this->getContentSize().width, 0));
    ccDrawLine(ccp(0, 0), ccp(0, this->getContentSize().height));
    ccDrawLine(ccp(0, this->getContentSize().height), ccp(this->getContentSize().width, this->getContentSize().height));
    ccDrawLine(ccp(this->getContentSize().width, 0), ccp(this->getContentSize().width, this->getContentSize().height));
    
    
}

CCRect DTCursorTextField::getRect()
{
    CCSize size;
//    if (&m_designedSize != NULL) {
//        size = m_designedSize;
//    }else {
//        size = getContentSize();
//    }
    
    size = getContentSize();
    
    CCRect rect = CCRectMake(0 - size.width * getAnchorPoint().x, 0 - size.height * getAnchorPoint().y, size.width, size.height);
    return  rect;
}
//设置触摸弹出输入法的区域大小
void DTCursorTextField::setDesignedSize(cocos2d::CCSize size)
{
    m_designedSize = size;
}

CCSize DTCursorTextField::getDesignedSize()
{
    return m_designedSize;
}

bool DTCursorTextField::isInTextField(cocos2d::CCTouch *pTouch)
{
    if (this==NULL || this->getParent()==NULL) {
        return false;
    }
//    CCPoint pToushPos = (this->getParent())->convertToNodeSpace(pTouch->getLocation());
    
    CCPoint pToushPos = this->convertToNodeSpace(pTouch->getLocation());
    
//    CCPoint pToushPos = convertTouchToNodeSpaceAR(pTouch);
    bool ret = getRect().containsPoint(pToushPos);
    return ret;
}

void DTCursorTextField::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint endPos = pTouch->getLocationInView();
    endPos = CCDirector::sharedDirector()->convertToGL(endPos);
    
    // 判断是否为点击事件
    if (::abs(endPos.x - m_beginPos.x) > DELTA ||
        ::abs(endPos.y - m_beginPos.y) > DELTA)
    {
        // 不是点击事件
        m_beginPos.x = m_beginPos.y = -1;
        
        return;
    }
    
    CCLOG("width: %f, height: %f.", getContentSize().width, getContentSize().height);
    
    // 判断是打开输入法还是关闭输入法
    isInTextField(pTouch) ? openIME() : closeIME();
}

bool DTCursorTextField::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
    if (_inputText=="") {
        return false;
    }
    
    m_pCursorSprite->setPositionX(getContentSize().width);
    
    return false;
}

bool DTCursorTextField::onTextFieldInsertText(cocos2d::CCTextFieldTTF *pSender, const char *text, int nLen)
{
    CCLOG("Width: %f", pSender->getContentSize().width);
    CCLOG("Text: %s", text);
    CCLOG("Length: %d", nLen);
    
    std::string tempStr = _inputText;// m_pInputText->substr();
    tempStr.append(text);
    if (tempStr.length() > m_limitNum) {
        return true;
    }
    
    _inputText.append(tempStr);
    
    
    if (isPsw) {
        std::string tempStr;
        for (int i = 0; i < _inputText.size(); i++) {
            tempStr.append("*");
        }
        setString(tempStr.c_str(), _inputText.c_str());
    }else {
        setString(_inputText.c_str(), _inputText.c_str());
    }
    
    m_pCursorSprite->setPositionX(getContentSize().width);
    
    return true;
}

bool DTCursorTextField::onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *pSender, const char *delText, int nLen)
{
    _inputText.resize(_inputText.size() - nLen);
    CCLOG("%s",_inputText.c_str());
    
    if (isPsw) {
        std::string tempStr;
        for (int i = 0; i < _inputText.size(); i++) {
            tempStr.append("*");
        }
        setString(tempStr.c_str(), _inputText.c_str());
    }else {
        setString(_inputText.c_str(), _inputText.c_str());
    }
    
    m_pCursorSprite->setPositionX(getContentSize().width);
    
    if (_inputText=="") {
        m_pCursorSprite->setPositionX(0);
    }
    
    return true;
}

bool DTCursorTextField::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
    return false;
}

void DTCursorTextField::openIME()
{
    m_pCursorSprite->setVisible(true);
    this->attachWithIME();
}

void DTCursorTextField::closeIME()
{
    m_pCursorSprite->setVisible(false);
    this->detachWithIME();
}

void DTCursorTextField::onExit()
{
    this->detachWithIME();
//    setTouchEnabled(false);
    CCTextFieldTTF::onExit();
}

bool DTCursorTextField::getIsPsw()
{
    return isPsw;
}
//设置星号显示否
void DTCursorTextField::setIsPsw( bool bFlag)
{
    isPsw = bFlag;
}

int DTCursorTextField::getLimitNum()
{
    return m_limitNum;
}
//设置字符长度
void DTCursorTextField::setLimitNum(int limitNum)
{
    m_limitNum = limitNum;
}

void DTCursorTextField::setString(const char *displayTx, const char* inputTx)
{
    //CC_SAFE_DELETE(_inputText);
    _inputText = inputTx;
//    if (inputTx)
//    {
//        m_pInputText = new std::string(inputTx);
//    }
//    else
//    {
//        m_pInputText = new std::string;
//    }
    
    // if there is no input text, display placeholder instead
    if (_inputText == "")
    {
        Label::setString(_placeHolder);
    }
    else
    {
        Label::setString(displayTx);
    }
    _charCount = _calcCharCount(_inputText.c_str());
}

void DTCursorTextField::setCursorColor(const ccColor3B& color3)
{
//    m_sColor = m_sColorUnmodified = color3;
//    
//    if (m_bOpacityModifyRGB)
//    {
//        m_sColor.r = color3.r * m_nOpacity/255.0f;
//        m_sColor.g = color3.g * m_nOpacity/255.0f;
//        m_sColor.b = color3.b * m_nOpacity/255.0f;
//    }
    
    updateColor();
    m_pCursorSprite->setColor(color3);
}