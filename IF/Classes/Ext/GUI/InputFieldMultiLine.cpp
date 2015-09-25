//
//  InputFieldMultiLine.cpp
//  COK
//
//  Created by nosources on 14-4-23.
//
//

#include "InputFieldMultiLine.h"
#include "Utf8Utils.h"
#include "CCLoadSprite.h"
#include "CCCommonUtils.h"
#include "CommonInclude.h"
#include "SceneController.h"
#include "PopupViewController.h"

#define DEFAULT_FONT 24
#define POSITION_X_INDENT 10

InputFieldMultiLine* InputFieldMultiLine::create(CCSize size, std::string bgSpriteName, int fontSize){
    InputFieldMultiLine* input = new InputFieldMultiLine();
    CCLOG("create input field the address is %x",(long)input);
    if (input->init(size, bgSpriteName, fontSize)) {
        input->autorelease();
        return input;
    }else{
        CC_SAFE_RELEASE(input);
        return NULL;
    }
}

InputFieldMultiLine::InputFieldMultiLine():isEditing(false), adjustHeight(0.0f),mBeingEditLabel(NULL),m_touchPriority(-1000),m_enabled(true),m_bSwallowsTouches(false),m_moveFlag(false),m_showLineNum(0),m_calCharLen(false), maxChars(std::numeric_limits<int>::max()), m_onlySingleLine(false){
    
}
void InputFieldMultiLine::setEnabled(bool flag)
{
    m_enabled = flag;
    if (m_enabled) {
        bg->setColor(ccWHITE);
    }
    else
    {
        bg->setColor(ccGRAY);
    }
}

bool InputFieldMultiLine::getEnabled()
{
    return m_enabled;
}

int InputFieldMultiLine::getTouchPriority(){
    return m_touchPriority;
}

void InputFieldMultiLine::setTouchPriority(int var){
//    m_touchPriority = var;
//    setTouchEnabled(false);
//    setTouchEnabled(true);
}

void InputFieldMultiLine::setSwallowsTouches(bool _swall)
{
    cocos2d::Layer::setSwallowsTouches(_swall);
}
bool InputFieldMultiLine::init(CCSize size, std::string bgSpriteName, int fontSize){
    if (CCNode::init()) {
        this->setContentSize(size);
        mFontSize = fontSize;
        
//        //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_touchPriority, m_bSwallowsTouches);
        setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
        setTouchEnabled(true);
        bg = CCLoadSprite::createScale9Sprite(bgSpriteName.c_str());
        
        bg->setInsetLeft(13);
        bg->setInsetRight(13);
        bg->setInsetTop(13);
        bg->setInsetBottom(13);
        bg->setAnchorPoint(ccp(0, 0));
        bg->setPreferredSize(size);
        addChild(bg);
        
        
        mTextFieldTTF = cocos2d::CCTextFieldTTF::textFieldWithPlaceHolder(getText().c_str(),"Helvetica",mFontSize);
        mTextFieldTTF->setDelegate(this);

        mTextFieldTTF->setDimensions(size.width - POSITION_X_INDENT*2, size.height);

        mTextFieldTTF->setAnchorPoint(ccp(0,0.5));
        mTextFieldTTF->setPositionX(POSITION_X_INDENT);
        mTextFieldTTF->setPositionY(size.height/2);
        mTextFieldTTF->setColor(ccBLACK);
        mTextFieldTTF->setVisible(false);
        this->addChild(mTextFieldTTF, 10, 1);
        mTextFieldTTF->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        
        
        m_pCursorSprite = CCSprite::create();
        this->addChild(m_pCursorSprite);
        m_addH = -1;
        m_pCursorSprite->setVisible(false);
        
        m_defaultText = "";
        return true;
    }
    return false;
}
void InputFieldMultiLine::resize(cocos2d::CCSize size){
    this->setContentSize(size);
    bg->setPreferredSize(size);
    mTextFieldTTF->setDimensions(size.width - POSITION_X_INDENT*2, size.height);
    mTextFieldTTF->setPositionX(POSITION_X_INDENT);
    mTextFieldTTF->setPositionY(size.height/2);
}
void InputFieldMultiLine::changeBG(std::string bgImg){
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bgImg.c_str());
    if(frame){
        bg->setSpriteFrame(frame);
        bg->setPreferredSize(getContentSize());
    }
}
void InputFieldMultiLine::addTargetedDelegate(){
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_touchPriority, m_bSwallowsTouches);
    setTouchEnabled(true);
}
void InputFieldMultiLine::addLine()
{
    CCSize oldSize = this->getContentSize();
    int lingHeight = oldSize.height / mLineNumber;
    oldSize.height += lingHeight;
    this->setContentSize(oldSize);
    bg->setPreferredSize(oldSize);
    mTextFieldTTF->setDimensions(oldSize.width - POSITION_X_INDENT*2, oldSize.height);
    mTextFieldTTF->setPositionY(oldSize.height/2);
    mLineNumber ++;

    CCLabelTTF* label = CCLabelTTF::create("", "Helvetica", mFontSize);
    mLabels.push_back(label);
    this->addChild(label);
    label->setAnchorPoint(ccp(0,0));
    label->setPositionX(POSITION_X_INDENT);
    label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    label->setPositionY(oldSize.height / mLineNumber * (mLineNumber-1));
    label->setColor(mLabels[0]->getColor());
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INPUT_ADD_LINE,CCInteger::create(lingHeight));
}

void InputFieldMultiLine::delLine()
{
    CCSize oldSize = this->getContentSize();
    int lingHeight = oldSize.height / mLineNumber;
    oldSize.height -= lingHeight;
    this->setContentSize(oldSize);
    bg->setPreferredSize(oldSize);
    mTextFieldTTF->setDimensions(oldSize.width - POSITION_X_INDENT*2, oldSize.height);
    mTextFieldTTF->setPositionY(oldSize.height/2);
    mLineNumber --;

    CCLabelTTF * label = mLabels[mLineNumber];
    this->removeChild(label);
    mLabels.erase(mLabels.end()-1);

    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INPUT_ADD_LINE,CCInteger::create(-lingHeight));
}

int InputFieldMultiLine::getLineNumber(){
    return mLineNumber;
}

void InputFieldMultiLine::setFontColor(const ccColor3B& color)
{
    for (int i = 0; i < mLabels.size(); i++) {
        mLabels[i]->setColor(color);
    }
}

void InputFieldMultiLine::setShowLineNum(int num)
{
    m_showLineNum = num;
}

void InputFieldMultiLine::setLineNumber(int var){
    mLineNumber = var;
    m_bakLineNumber = mLineNumber;
    if (m_showLineNum == 0) {
        m_showLineNum = mLineNumber;
    }
    CCSize size = this->getContentSize();
    for (int i = 0; i < var; i++) {
        CCLabelTTF* label = CCLabelTTF::create("", "Helvetica", mFontSize);
        mLabels.push_back(label);
        this->addChild(label);
        label->setAnchorPoint(ccp(0,0));
        label->setPositionX(POSITION_X_INDENT);
        label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        label->setPositionY(size.height / mLineNumber * i);
    }
    mBeingEditLabel = mLabels[mLineNumber - 1];
    initCursorSprite(mFontSize);
}
void InputFieldMultiLine::initCursorSprite(int nHeight)
{
    // 初始化光标
    int column = 4;
    int pixels[nHeight][column];
    for (int i=0; i<nHeight; ++i) {
        for (int j=0; j<column; ++j) {
            pixels[i][j] = 0x00000000;
        }
    }
    
    CCTexture2D *texture = new CCTexture2D();
    texture->autorelease();
    texture->initWithData(pixels,4*nHeight*column, kCCTexture2DPixelFormat_RGB888, column, nHeight, CCSizeMake(column, nHeight));
    
    
    m_pCursorSprite->initWithTexture(texture);
    m_pCursorSprite->setAnchorPoint(ccp(0,0));

    resetCursorPosition();
    
    m_pCursorAction = CCRepeatForever::create((CCActionInterval *) CCSequence::create(CCFadeIn::create(0.1f),CCDelayTime::create(0.6f), CCFadeOut::create(0.1f),CCDelayTime::create(0.4f),NULL));
    m_pCursorSprite->runAction(m_pCursorAction);
}
bool InputFieldMultiLine::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCLOG("InputFieldMultiLine id %d",m_id);
    if (!m_enabled && isTouchInside(bg, pTouch)) {
        if (isEditing) {
            closeIME();
        }
        return false;
    }
    CCPoint touchLocation=this->getParent()->convertToNodeSpace(pTouch->getLocation());
    CCRect bBox=this->boundingBox();
    
    if(bBox.containsPoint(touchLocation) && isVisible()){
        CCLOG("InputFieldMultiLine openIME %d",m_id);
        if(m_defaultText!="")
        {
            setText(m_defaultText);
        }
        openIME();
        return true;
    }
    if (isEditing) {
        CCLOG("InputFieldMultiLine closeIME %d",m_id);
        closeIME();
        //key board is showing
        return true;
    }
    return false;
}
void InputFieldMultiLine::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

bool InputFieldMultiLine::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
    if (m_pInputText.empty()) {
        return false;
    }
    resetCursorPosition();
    
    return false;
}

CCSize InputFieldMultiLine::getTextDimension(const char *text)
{
    int len = (int)strlen(text);
    if ((len==0)  ||  (len==1 && text[0] == '\n')) {
        CCLabelTTF *label = CCLabelTTF::create( "sample" , "Helvetica", mFontSize );
        CCSize size = label->getContentSize();
        size.width = 0;
        return size;
    }
    CCLabelTTF *label = CCLabelTTF::create( text, "Helvetica", mFontSize);
	return label->getContentSize();
}
void InputFieldMultiLine::setSingleLineForLabel(cocos2d::CCLabelTTF *label, std::string tosetString){
    std::string labelStringToSet = tosetString;
    CCLOG("test setSingleLine %s",labelStringToSet.c_str());
    if (tosetString[0] == '\n') {
        labelStringToSet = tosetString.substr(1, tosetString.length() - 1);
    }
    CCLOG("test setSingleLine1 %s",labelStringToSet.c_str());
    label->setString(labelStringToSet.c_str());
}
void InputFieldMultiLine::setStringFromVectorOrFromInputString(){
    if (mVectorInput.size() == 0) {
        if(mLineNumber > m_bakLineNumber)
        {
            for (int i = mLineNumber-m_bakLineNumber; i > 0; i--) {
                delLine();
            }
        }
        mLabels[mLabels.size() - 1]->setString(m_pInputText.c_str());
        mBeingEditLabel = mLabels[mLabels.size() - 1];
    }else{
        int length = (int)mVectorInput.size();
        if (mVectorInput.size() >=m_showLineNum) {
            int j = 1;
            for (int i = length - 1; i > length - mLineNumber && j < mLineNumber; i--) {
                setSingleLineForLabel(mLabels[j], mVectorInput[i]);
                j++;
            }
            setSingleLineForLabel(mLabels[0], nowLine);
            mBeingEditLabel = mLabels[0];
            
        }else{
            if (mVectorInput.size() >= mLineNumber && mLineNumber < m_showLineNum) {
                addLine();
            }else if (mVectorInput.size() < mLineNumber-1 && mLineNumber > m_bakLineNumber)
            {
                delLine();
            }
            int j = mLineNumber;
            for (int i = 0; i < length; i++) {
                setSingleLineForLabel(mLabels[j - 1], mVectorInput[i]);
                j--;
            }
            setSingleLineForLabel(mLabels[j - 1], nowLine);
            mBeingEditLabel = mLabels[j - 1];
        }
    }
    mTextFieldTTF->setString(m_pInputText.c_str());
}
void InputFieldMultiLine::resetInitBG(){
    float hg =mTextFieldTTF->getContentSize().height;
    if(hg>0){
    
    }

}
bool InputFieldMultiLine::insertOneChar(const char *text, int nLen){
    std::string textString = text;
    if (text[0] == '\n'){
        m_pInputText.append(textString);
        mVectorInput.push_back(nowLine);
        nowLine = '\n';
        setStringFromVectorOrFromInputString();
        resetCursorPosition();
        
        return true;
    }
    
    m_pInputText.append(textString);
    //    mTextFieldTTF->setString(m_pInputText.c_str());
    nowLine.append(textString);
    CCLOG("input test m_pInputText %s",m_pInputText.c_str());
    CCLOG("input test nowline %s",nowLine.c_str());
    CCLOG("input test textString %s",textString.c_str());
    CCSize nowSize = getTextDimension(nowLine.c_str());
    float width =getContentSize().width - POSITION_X_INDENT*2;
    if (nowSize.width > width) {
        size_t pos = nowLine.find_last_of(" ");
        if (pos != std::string::npos) {
            mVectorInput.push_back(nowLine.substr(0,pos+1));
            nowLine = nowLine.substr(pos+1, nowLine.length());
        }else{
            char _charUTF8[255]={0};
            long _length= strlen( nowLine.c_str() );
            strncpy(_charUTF8,nowLine.c_str(), _length);
            CCLOG("input test _charUTF8 %s",_charUTF8);
            string tmpStr = Utf8Utils::utf8sub(_charUTF8,0,Utf8Utils::strlen(_charUTF8)  - nLen);
            mVectorInput.push_back(tmpStr);
            CCLOG("input test tmpStr %s",tmpStr.c_str());
            nowLine = textString;
        }
    }
    setStringFromVectorOrFromInputString();
    resetCursorPosition();
    
    
    
    return true;
}

bool InputFieldMultiLine::onTextFieldInsertText(cocos2d::CCTextFieldTTF *pSender, const char *text, size_t nLen)
{
    CCLOG("text field insert %s length %d", text, nLen);
    
    // 不允许换行
    if(m_onlySingleLine)
    {
        if(text[0] == '\n')
        {
            return true;
        }
    }
    
    //连续换行不允许
    if (nowLine.size() == 1 && nowLine[0] == '\n' && text[0] == '\n') {
        CCLOG("text field now line %s length %d", nowLine.c_str(), (int)nowLine.length());
        return true;
    }
    CCLOG("text field now line %s length %d", nowLine.c_str(), (int)nowLine.length());

    char _charUTF8[1000]={0};
    long _length= strlen( text );

    strncpy(_charUTF8,text, _length);
    //对于换行符的处理
    int a = Utf8Utils::strlen(_charUTF8);
    CCLOG("text field input text %s len %d, text Len %ld",_charUTF8,a,_length);
    if (a > 1) {
        for (int i = 0; i < a; i++) {
            auto charUTF8 = Utf8Utils::utf8sub(_charUTF8, i, 1);
            CCLOG("text field insert the %s",charUTF8);
            int len = Utf8Utils::strlen(m_pInputText.c_str());
            
            if (maxChars > 0 && len >= maxChars) {
                CCLOG("input char count is exceed max length");
                break;
            }
            if(m_calCharLen)
            {
                len = m_pInputText.length();
                if(len > 0 && len>=maxChars)
                {
                    CCLOG("input char count is exceed max length");
                    break;
                }
            }
            insertOneChar(charUTF8,Utf8Utils::strlen(charUTF8));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INPUT_TEXT_EVENT,CCInteger::create(len));
        }
    
        return true;
    }else{
        int len = Utf8Utils::strlen(m_pInputText.c_str());
        
        if (maxChars > 0 && len >= maxChars) {
            CCLOG("input char count is exceed max length");
            return true;
        }
        
        if(m_calCharLen)
        {
            len = m_pInputText.length();
            if(len > 0 && len>=maxChars)
            {
                CCLOG("input char count is exceed max length");
                return true;
            }
        }
        insertOneChar(text, Utf8Utils::strlen(text));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INPUT_TEXT_EVENT,CCInteger::create(len));
        return true;
    }
    
}
void InputFieldMultiLine::resetCursorPosition(){
    if (mVectorInput.size() < mLineNumber - 1) {
        m_pCursorSprite->setPositionY(mLabels[mLineNumber - 1 - mVectorInput.size()]->getPositionY() + 4);
    }else{
        m_pCursorSprite->setPositionY(4);
    }
    m_pCursorSprite->setPositionX(mBeingEditLabel->getContentSize().width  + POSITION_X_INDENT);
}
bool InputFieldMultiLine::onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *pSender, const char *delText, size_t nLen)
{
    CCLOG("test onTextFieldDeleteBackward %s, %d",delText,nLen);
    m_pInputText.resize(m_pInputText.size() - nLen);
    int len = Utf8Utils::strlen(m_pInputText.c_str());
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INPUT_TEXT_EVENT,CCInteger::create(len));
    if (nowLine.size() < nLen) {
        //如果一下子删除了很多个字符，已经删除到上一行了都
        while (nowLine.size() < nLen) {
            if (mVectorInput.size()>0) {
                std::string lastLine = mVectorInput.back();
                mVectorInput.pop_back();
                nowLine = lastLine + nowLine;
            }else{
                break;
            }//基本上不可能一下子删除太多，删除到上上行
        }
    }
    nowLine.resize(nowLine.size() - nLen);
    if (nowLine.length() == 0) {
        if (mVectorInput.size() > 0) {
            nowLine = mVectorInput.back();
            mVectorInput.pop_back();
            mBeingEditLabel->setString("");
        }
    }else{
        if (mVectorInput.size()>0) {
            std::string lastLine = mVectorInput.back();
            if (nowLine[0] != '\n') {
                std::string lastLineNew = lastLine + nowLine;
                CCSize lastLineNewSize = getTextDimension(lastLineNew.c_str());
                float width = getContentSize().width - POSITION_X_INDENT*2;
                if (lastLineNewSize.width <= width) {
                    nowLine = lastLineNew;
                    mVectorInput.pop_back();
                    mBeingEditLabel->setString("");
                }
            }
        }
    }
    setStringFromVectorOrFromInputString();
    resetCursorPosition();
    
    return true;
}
std::string InputFieldMultiLine::getText(){
    return  m_pInputText;
}
void InputFieldMultiLine::setText(std::string text){
    for (int i = 0; i < mLabels.size(); i++) {
        setSingleLineForLabel(mLabels[i], "");
    }
    mVectorInput.clear();
    nowLine.clear();
    m_pInputText.clear();
    onTextFieldInsertText(NULL, text.c_str(), text.size());
}
void InputFieldMultiLine::setPlaceHolder(std::string text){
//    for (int i = 0; i < mLabels.size(); i++) {
//        setSingleLineForLabel(mLabels[i], "");
//    }
    setSingleLineForLabel(mLabels[mLabels.size() - 1], text);
}
void InputFieldMultiLine::setMaxChars(int i){
    maxChars = i;
}
bool InputFieldMultiLine::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
    return false;
}
bool InputFieldMultiLine::getIsEditing(){
    return isEditing;
}

void InputFieldMultiLine::setAddH(int h){
    m_addH = h;
}

void InputFieldMultiLine::setBgVisible(bool v){
    bg->setVisible(v);
}

void InputFieldMultiLine::openIME()
{
    isEditing = true;
    m_pCursorSprite->setVisible(true);
    bool _isOpenIME = mTextFieldTTF->attachWithIME();
    //优先级调高,暂时不用setPriority方法，因为会触发重新排列优先级，这样可能造成面板上面的button优先级低
//    //CCDirector::sharedDirector()->getTouchDispatcher()->setPriority(-2000, this);
//    setTouchEnabled(false);
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -2000,true);
    if (m_pInputText.length() == 0) {
        setSingleLineForLabel(mLabels[mLabels.size() - 1], "");
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (_isOpenIME && m_moveFlag) {
//        CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
//        if (scene->getPositionY() == 0) {
//            CCSize winsize = CCDirector::sharedDirector()->getWinSize();
//            scene->setPositionY(scene->getPositionY() + winsize.height/2-30);
//            
////            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(InputFieldMultiLine::checkIME), this);
////            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(InputFieldMultiLine::checkIME), this, 1.0,kCCRepeatForever, 0.0f, false);
//        }
        CCSize winsize = CCDirector::sharedDirector()->getWinSize();
        int addH = winsize.height/2-30;
        if(m_addH != -1){
            addH = m_addH;
        }
        if(PopupViewController::getInstance()->getCurrViewCount() >0){
            auto popupLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP_IN);
            
            if(popupLayer->getPositionY()==0){
                popupLayer->setPositionY(popupLayer->getPositionY() + addH);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(INPUTFIELD_OPEN_EVENT);
            }
        }else{
            CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
            if (scene->getPositionY() == 0) {
                scene->setPositionY(scene->getPositionY() + addH);
            }
        }
    }
#endif
}

void InputFieldMultiLine::closeIME()
{
    isEditing = false;
    m_pCursorSprite->setVisible(false);
    bool _isCloseIME = mTextFieldTTF->detachWithIME();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (_isCloseIME && m_moveFlag) {
//        CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
//        scene->setPositionY(0);
        if(PopupViewController::getInstance()->getCurrViewCount() >0){
            auto popupLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP_IN);
            popupLayer->setPositionY(0);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(INPUTFIELD_CLOSE_EVENT);
        }else{
            CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
            scene->setPositionY(0.0f);
        }
    }
#endif
    //优先级调高
//    setTouchEnabled(false);
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,m_touchPriority, true);
//    //CCDirector::sharedDirector()->getTouchDispatcher()->setPriority(m_touchPriority, this);
}

void InputFieldMultiLine::checkIME()
{
    
}

//void InputFieldMultiLine::keyBackClicked(){
//    CCLOG("keyBackClicked");
//
//}
void InputFieldMultiLine::keyboardWillShow(CCIMEKeyboardNotificationInfo &info){
    CCLOG("will show");
    if (!isEditing) {
        return;
    }
    CCLOG("will show 2");
    CCPoint pos = this->convertToWorldSpace(this->getPosition());
    if (pos.y > info.end.origin.y + info.end.size.height) {
        return;
    }
    CCLOG("will show 3");
    adjustHeight = info.end.origin.y + info.end.size.height - pos.y;


    if(PopupViewController::getInstance()->getCurrViewCount() >0){
        auto popupLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP_IN);
        popupLayer->setPositionY(popupLayer->getPositionY() + adjustHeight);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(INPUTFIELD_OPEN_EVENT);
    }else{
        CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
        scene->setPositionY(scene->getPositionY() + adjustHeight);
    }
}
void InputFieldMultiLine::keyboardWillHide(CCIMEKeyboardNotificationInfo& info){
    CCLOG("will hide");
    if(PopupViewController::getInstance()->getCurrViewCount() >0){
        auto popupLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP_IN);
        popupLayer->setPositionY(0);
        adjustHeight = 0.0f;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(INPUTFIELD_CLOSE_EVENT);
    }else{
        CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
        scene->setPositionY(0.0f);
        adjustHeight = 0.0f;
    }
}
void InputFieldMultiLine::onExit(){
    mTextFieldTTF->detachWithIME();
    if( _touchEnabled ) {
        setTouchEnabled(false);
    }
    CCNode::onExit();
}

void InputFieldMultiLine::onEnter() {
    //对应onExit, 恢复touch；


    //if(m_bTouchEnabled)
    //    this->registerWithTouchDispatcher();
    CCNode::onEnter();


}

//void InputFieldMultiLine::visit(){
////    beforeDraw();
//    CCNode::visit();
////    afterDraw();
//}
CCRect InputFieldMultiLine::getViewRect()
{
    CCPoint screenPos = this->convertToWorldSpace(CCPointZero);
    
    float scaleX = this->getScaleX();
    float scaleY = this->getScaleY();
    
    for (CCNode *p = _parent; p != NULL; p = p->getParent()) {
        scaleX *= p->getScaleX();
        scaleY *= p->getScaleY();
    }
    
    // Support negative scaling. Not doing so causes intersectsRect calls
    // (eg: to check if the touch was within the bounds) to return false.
    // Note, CCNode::getScale will assert if X and Y scales are different.
    if(scaleX<0.f) {
        screenPos.x += getContentSize().width*scaleX;
        scaleX = -scaleX;
    }
    if(scaleY<0.f) {
        screenPos.y += getContentSize().height*scaleY;
        scaleY = -scaleY;
    }
    
    return CCRectMake(screenPos.x, screenPos.y, getContentSize().width*scaleX, getContentSize().height*scaleY);
}
/**
 * clip this view so that outside of the visible bounds can be hidden.
 */
void InputFieldMultiLine::beforeDraw()
{
    m_bScissorRestored = false;
    CCRect frame = getViewRect();
    if (Director::sharedDirector()->getOpenGLView()->isScissorEnabled()) {
        m_bScissorRestored = true;
        m_tParentScissorRect = Director::sharedDirector()->getOpenGLView()->getScissorRect();
        //set the intersection of m_tParentScissorRect and frame as the new scissor rect
        if (frame.intersectsRect(m_tParentScissorRect)) {
            float x = MAX(frame.origin.x, m_tParentScissorRect.origin.x);
            float y = MAX(frame.origin.y, m_tParentScissorRect.origin.y);
            float xx = MIN(frame.origin.x+frame.size.width, m_tParentScissorRect.origin.x+m_tParentScissorRect.size.width);
            float yy = MIN(frame.origin.y+frame.size.height, m_tParentScissorRect.origin.y+m_tParentScissorRect.size.height);
            Director::sharedDirector()->getOpenGLView()->setScissorInPoints(x, y, xx-x, yy-y);
        }
    }
    else {
        glEnable(GL_SCISSOR_TEST);
        Director::sharedDirector()->getOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
    }
    
}

/**
 * retract what's done in beforeDraw so that there's no side effect to
 * other nodes.
 */
void InputFieldMultiLine::afterDraw()
{
    if (m_bScissorRestored) {//restore the parent's scissor rect
        Director::sharedDirector()->getOpenGLView()->setScissorInPoints(m_tParentScissorRect.origin.x, m_tParentScissorRect.origin.y, m_tParentScissorRect.size.width, m_tParentScissorRect.size.height);
    }
    else {
        glDisable(GL_SCISSOR_TEST);
    }
    
}

InputFieldMultiLine::~InputFieldMultiLine(){
    CCLOG("delete input field the address is %x",(long)this);
    this->ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0,0));
    mTextFieldTTF->detachWithIME();
}


