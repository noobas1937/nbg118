//
//  CCEditText.cpp
//  IF
//
//  Created by ganxiaohua on 14-5-4.
//
//

#include "CCEditText.h"
#include "Utf8Utils.h"
#include "CCLoadSprite.h"
#include "CCCommonUtils.h"
#include "CommonInclude.h"

NS_CC_EXT_BEGIN

static const int MARGIN_HEIGHT = 16; // 文字高度的基础上的扩充高度
static const int INDENT_WIDTH = 10; // 文字缩进宽度

enum {
    kAOEEditLabelTypeUseCocosLayout = 0,           //软键盘方案：cocos2d层布局
    kAOEEditLabelTypeUsePlatformLayout = 1,        //软键盘方案：平台提供布局
};

CCEditText * CCEditText::create(CCSize size, const char *str ,bool multiLine,bool needInputBox)
{
    CCEditText * pRet = new CCEditText();
    if (pRet && pRet->init(size, str,multiLine,needInputBox))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool CCEditText::init(CCSize size, const char *str ,bool multiLine,bool needInputBox)
{
    bool bRet = false;
    m_haveIME = false;
    m_isTextEnable = true;
    m_size = size;
    m_multiLine = multiLine;
    m_needInputBox = needInputBox;
    
    m_ccbNode = CCBLoadFile("EditTextView.ccbi",NULL,this);
    
    m_clickBg = CCLoadSprite::createScale9Sprite(str);
    m_clickBg->setPosition(0, 0);
    m_clickBg->setPreferredSize(CCSize(size.width, size.height+MARGIN_HEIGHT));
    m_clickBg->setAnchorPoint(ccp(0, 0.5));
    addChild(m_clickBg);
    
    m_label = CCLabelTTF::create();
    m_label->setFontName("Helvetica");
    m_label->setFontSize(24);
    m_label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    m_label->setHorizontalAlignment(kCCTextAlignmentLeft);
    m_label->setColor(ccBLACK);
    m_label->setAnchorPoint(ccp(0, 0.5));
    m_label->setPosition(ccp(INDENT_WIDTH/2, -3));
    addChild(m_label);
    m_nCharLimit = -1;

    CCScale9Sprite* spr = CCLoadSprite::createScale9Sprite(str);
    spr->setContentSize(size);
    m_contentBox = CCEditBox::create(size,spr);
    m_contentBox->setAnchorPoint(ccp(0, 0.5));
    this->addChild(m_contentBox);
    
    m_textField = CCTextFieldTTF::textFieldWithPlaceHolder(getString(),"Helvetica",24);
    m_textField->setString(getString());
    m_textField->setScale(0.5);
    this->addChild(m_textField);
    m_textField->setPosition(ccp(20,20));
    m_textField->setAnchorPoint(CCPointZero);
    m_textField->setSingleLine(true);
    m_textField->setDelegate(this);
    m_textField->setVisible(false);
    if (m_inputMode == kEditBoxInputModeNumeric) {
        m_textField->setInputType(0x02);
    }
    else{
        m_textField->setInputType(0x01);
    }
    
    if (m_isPasswordMode) {
        m_textField->setInputType(m_textField->getInputType()|0x90);//passwordMode
    }else{
        m_textField->setInputType(m_textField->getInputType()|0x90);
        //TODO::3.6
        //m_textField->setInputType(kKeyboardReturnTypeDone);
        //passwordMode
    }
    m_textField->setSingleLine(!multiLine);
    // Android 层提供了输入框的Layout布局
    m_editLabelType = kAOEEditLabelTypeUsePlatformLayout;
    
    if(multiLine){
        m_label->setAnchorPoint(ccp(0, 1));
        m_label->setPosition(ccp(INDENT_WIDTH/2, -size.height/2-3));
        m_clickBg->setAnchorPoint(ccp(0, 1));
        m_clickBg->setPosition(ccp(0, -size.height/2));
        m_label->setDimensions(CCSizeMake(size.width - INDENT_WIDTH, size.height));
    }else{
        m_label->setAnchorPoint(ccp(0, 0.5));
        m_label->setPosition(ccp(INDENT_WIDTH/2, 0));
        m_clickBg->setAnchorPoint(ccp(0, 0.5));
        m_clickBg->setPosition(ccp(0, 0));
    }
    m_label->setVisible(false);
    
    m_andriodEdit = CCLayer::create();
    
    //android need inputBox
    CCSize visibleSize = Director::sharedDirector()->getVisibleSize();
    CCScale9Sprite *sprite = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
    sprite->CCNode::setContentSize(CCSize(visibleSize.width-20, 55));
    sprite->setAnchorPoint(ccp(0, 0));
    sprite->setPosition(ccp(10,0));
    m_andriodEdit->addChild(sprite);
    
    inputTxt = CCLabelTTF::create();
    inputTxt->setFontName("Helvetica");
    inputTxt->setFontSize(30);
    inputTxt->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    inputTxt->setHorizontalAlignment(kCCTextAlignmentLeft);
    inputTxt->setColor(ccBLACK);
    inputTxt->setAnchorPoint(ccp(0, 0.5));
    inputTxt->setPosition(ccp(16, 28));
    m_andriodEdit->addChild(inputTxt);
    m_andriodEdit->setVisible(false);
    //this->setIsKeypadEnabled(true)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_contentBox->setVisible(false);
    m_label->setVisible(true);
    m_andriodEdit->setVisible(m_needInputBox);
#else
    if(multiLine){
        m_contentBox->setVisible(false);
        m_clickBg->setVisible(true);
        m_label->setVisible(true);
    }else{
        m_contentBox->setVisible(true);
        m_clickBg->setVisible(false);
    }
#endif
    
    this->setSingleLine(!multiLine);
    bRet = true;
    
    return bRet;
}

void CCEditText::destory(){
    if(m_editBox){
        m_editBox->removeFromParentAndCleanup(true);
    }
    if(m_editView){
        m_editView->removeFromParentAndCleanup(true);
    }
    if(m_textField){
        m_textField->removeFromParentAndCleanup(true);
    }
    if(m_andriodEdit){
        m_andriodEdit->removeFromParentAndCleanup(true);
    }
    if(inputTxt){
        inputTxt->removeFromParentAndCleanup(true);
    }
}

void CCEditText::setFontName(const char* pFontName){
    m_strFontName = pFontName;
    m_label->setFontName(pFontName);
    m_contentBox->setFontName(pFontName);
}

void CCEditText::setFontSize(int fontSize){
    m_nFontSize = fontSize;
    m_label->setFontSize(fontSize);
    m_contentBox->setFontSize(fontSize);
}

void CCEditText::setFontColor(const ccColor3B& color){
    m_colText = color;
    m_label->setColor(color);
    m_contentBox->setFontColor(color);
}

void CCEditText::setText(const char* pText)
{
    if (pText != NULL)
    {
        setString(pText);
        m_contentBox->setText(m_label->getString().c_str());
    }
}

void CCEditText::setInputFlag(EditBox::InputFlag inputFlag){
    m_contentBox->setInputFlag(inputFlag);
}

void CCEditText::setReturnType(EditBox::KeyboardReturnType returnType){
    m_contentBox->setReturnType(returnType);
}

const char* CCEditText::getText(void)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return m_label->getString().c_str();
#else
    if(m_multiLine){
        return m_label->getString().c_str();
    }else{
        return m_contentBox->getText();
    }
#endif
    return "";
}

void CCEditText::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -80, false);
    //CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);
}

void CCEditText::onExit()
{
    CC_SAFE_RELEASE_NULL(m_onFinish);
    CC_SAFE_RELEASE_NULL(m_onBeforeInput);
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate(this);
    if (m_textField){
        m_textField->detachWithIME();
    }
    if(m_andriodEdit){
        m_andriodEdit->removeFromParent();
    }
    CCNode::onExit();
}

//void CCEditText::keyBackClicked(){
//    CCLOG("keyBackClicked");
//    if (m_textField)
//        m_textField->detachWithIME();
//    if(m_andriodEdit){
//        m_andriodEdit->removeFromParent();
//    }
//}

static CCRect getRect(CCNode * pNode)
{
	CCSize contentSize = pNode->getContentSize();
	CCRect rect = CCRectMake(0, 0, contentSize.width, contentSize.height);
	return CCRectApplyAffineTransform(rect, pNode->nodeToWorldTransform());
}

void CCEditText::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    if (m_andriodEdit != NULL)
    {
        CCPoint visibleOrigin = Director::sharedDirector()->getVisibleOrigin();
        float scaleX = Director::sharedDirector()->getOpenGLView()->getScaleX();
        float scaleY = Director::sharedDirector()->getOpenGLView()->getScaleY();
        if(m_andriodEdit){
            CCRect rectTracked = getRect(m_andriodEdit);
            rectTracked.origin.y -= 4;
            if (!rectTracked.intersectsRect(info.end))
            {
                CCLOG("needn't to adjust view layout.");
                return;
            }
            m_keyBoardH = (info.end.getMaxY() - rectTracked.getMinY())*2;
            m_keyBoardH += m_andriodEdit->getPositionY();
            m_andriodEdit->setPosition(visibleOrigin.x,m_keyBoardH);
        }
    }
}

void CCEditText::setString(const char *label)
{
    if (strcmp(m_label->getString().c_str(),label) == 0){
        return;
    }
    
    int len = Utf8Utils::strlen(label);
    
    if (m_nCharLimit > 0 && len > m_nCharLimit) {
        //CCLOG("input char count is exceed max length");
        label = Utf8Utils::utf8sub(label, 0, 12);
    }
    
    char* str="";
    m_inputStr = label;
    if (m_isPasswordMode) {
        for (int i=0; i<len; i++) {
            str = (char*)CCString::createWithFormat("%s%s",str,"*")->getCString();
        }
    } else {
        str = (char*)label;
    }
    
    if (!m_multiLine || (strcmp("", str)==0)) {
        m_label->setString(str);
        inputTxt->setString(str);
    } else {
        std::string str_temp = str;
        m_label->setString(str_temp.substr(0, 2048).c_str());
        inputTxt->setString(str);
        // 字符串包含回车时，文字显示到框的外边，通过重置矩形大小控制。
        CCRect rect = CCRectZero;
        rect.size = m_size;
      //  m_label->setTextureRect(rect);
    }
}

const char* CCEditText::getString(void)
{
    return m_inputStr.c_str();
}

void CCEditText::showEditView(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    if (m_editView) {
        m_editView->removeAllChildrenWithCleanup(true);
    }
    
    if (getBeforeInputCallBack()){
        getBeforeInputCallBack()->execute();
    }
    m_editView = CCLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(m_editView);
    CCCommonUtils::setButtonTitle(m_OKBtn, "confirm");
    m_editView->setAnchorPoint(CCPoint(0,0));
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_bg->setPreferredSize(CCSize(winSize.width*2,winSize.height));
    m_bg->setOpacity(0);
    m_bg->setAnchorPoint(ccp(0,1));
    m_bg->setPosition(ccp(0,winSize.height));
    m_OKBtn->setPositionX(winSize.width-m_OKBtn->getPreferredSize().width*m_OKBtn->getScaleX()/2-15);
    m_OKBtn->setVisible(false);
    //CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
    CCSize visibleSize = Director::sharedDirector()->getVisibleSize();
    CCScale9Sprite *sprite = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
    m_OKBtn->setVisible(false);
    m_editBox = CCEditBox::create(CCSize(visibleSize.width-30, 55), sprite);
//    m_editBox->setZoomOnTouchDown(false);
    m_editBox->setInputMode(m_inputMode);
    m_editBox->setAnchorPoint(CCPointZero);
    m_editBox->setPosition(ccp(15,210));
    m_editBox->setText(m_label->getString().c_str());
    m_editBox->setFontColor(ccBLACK);
    m_editBox->setMaxLength(m_nCharLimit);
    m_editView->addChild(m_editBox);
    m_editBox->setDelegate(this);
    m_editBox->setVisible(false);
    m_editBox->setPosition(ccp(-20,-210));
    if (m_isPasswordMode) {
        m_editBox->setInputFlag(EditBox::InputFlag::PASSWORD);
    }
    m_editBox->touchDownAction(NULL,ui::Widget::TouchEventType::ENDED);
    m_editBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_editLabelType = kAOEEditLabelTypeUseCocosLayout;
}

void CCEditText::setInputMode(EditBoxInputMode mode)
{
    m_inputMode = mode;
}

#pragma mark -
#pragma mark CocosBuilder Part

SEL_CCControlHandler CCEditText::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnOk", CCEditText::keypressedBtnOK);
    return NULL;
}

bool CCEditText::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_OKBtn", CCControlButton*, this->m_OKBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    return false;
}

void CCEditText::keypressedBtnOK(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    //CCLOG("keypressedBtnOK");
    if(m_andriodEdit){
        m_andriodEdit->removeFromParent();
    }
    if (!m_editView || !m_editView->getParent()) {
        return;
    }
    if (m_editBox) {
        if(m_editLabelType == kAOEEditLabelTypeUseCocosLayout) {
            std::string str = m_editBox->getText();
            setString(str.substr(0, 2048).c_str());
        }
        m_editView->removeFromParentAndCleanup(true);
    }
    if (getFinishCallBack()){
        getFinishCallBack()->execute();
    }
}

void CCEditText::keyboardWillHide(CCIMEKeyboardNotificationInfo& info)
{
    keypressedBtnOK(NULL,CCControlEventTouchUpInside);
}

bool CCEditText::onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
    keypressedBtnOK(NULL,CCControlEventTouchUpInside);
    return false;
}

bool CCEditText::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_isTextEnable && m_clickBg && (isTouchInside(m_clickBg,pTouch))){
        return true;
    }
    return false;
}

void CCEditText::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(!m_iskeyBoardShow&&m_clickBg && (isTouchInside(m_clickBg,pTouch))){
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        m_textField->detachWithIME();
        m_textField->attachWithIME();
        //CCLOG(" CCEditText onTouchEnded");
        if(m_andriodEdit){
//            CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
            CCPoint visibleOrigin = cocos2d::Director::getInstance()->getOpenGLView()->getVisibleOrigin();
            m_andriodEdit->setPosition(visibleOrigin.x,m_keyBoardH);
            if(m_andriodEdit->getParent()==NULL){
                int tagIndex = 40000000000;
                CCNode* node = CCDirector::sharedDirector()->getRunningScene()->getChildByTag(tagIndex);
                if(node!=NULL){
                    node->removeFromParentAndCleanup(true);
                }
                m_andriodEdit->setTag(tagIndex);
                //this->addChild(m_andriodEdit);
                CCDirector::sharedDirector()->getRunningScene()->addChild(m_andriodEdit);
            }
        }
        if (getBeforeInputCallBack()){
            getBeforeInputCallBack()->execute();
        }
    #else
        if(m_multiLine){
         this->showEditView(NULL,Control::EventType::TOUCH_DOWN);   
        }
    #endif
        m_iskeyBoardShow = true;
    }else{
        m_iskeyBoardShow = false;
        m_textField->detachWithIME();
        if(m_andriodEdit){
            m_andriodEdit->removeFromParent();   
        }
    }
}

void CCEditText::setMaxLength(int length)
{
    m_nCharLimit = length;
    m_contentBox->setMaxLength(length);
}

bool CCEditText::onTextFieldInsertText(cocos2d::CCTextFieldTTF *sender, const char *text, int nLen)
{
    if ('\n' == *text)
        
    {
        return false;
    }
    // if the textfield's char count more than m_nCharLimit, doesn't insert text anymore.
    if (m_nCharLimit > 0 && sender->getCharCount() >= m_nCharLimit)
    {
        CCLOG("input char count is exceed max length");
        return true;
    }
    if (m_editBox) {
        setString(m_editBox->getText());
    }
    if(m_textField!=sender) return  false;
    if (m_textField) {
        std::string str = this->getString();
        str += text;
        setString(str.c_str());
    }
    return false;
}

bool CCEditText::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen){
    if (m_editBox) {
        setString(m_editBox->getText());
    }
    if(m_textField!=sender) return  false;
    if (m_textField) {
        std::string str = this->getString();
        if(str.size()<=0) return false;
        str = str.substr(0,str.size()-nLen);
        setString(str.c_str());
    }
    return false;
}

void CCEditText::setEnabled(bool isEnable)
{
    m_isTextEnable = isEnable;
    if (m_textField && !isEnable){
        m_textField->detachWithIME();
    }else if(m_textField && isEnable){
        m_textField->detachWithIME();
        m_textField->attachWithIME();
    }
    if(isEnable){
        m_contentBox->setEnabled(true);
    }else{
        m_contentBox->setEnabled(false);
    }
}

void CCEditText::enablePasswordMode(bool isEnable)
{
    m_isPasswordMode = isEnable;
}
void CCEditText::setHorizontalAlignment(CCTextAlignment alignment)
{
    m_label->setDimensions(CCSizeMake(m_size.width-5, 0));
    m_label->setHorizontalAlignment(alignment);
}
void CCEditText::setScale(float scale)
{
    CCNode::setScale(1);
}

void CCEditText::setSingleLine(bool isSingleLine)
{
    if (!isSingleLine) {
        m_label->setDimensions(CCSizeMake(m_size.width-INDENT_WIDTH*2, m_size.height));
        m_label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    }
}
void CCEditText::editBoxEditingDidBegin(CCEditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

void CCEditText::editBoxEditingDidEnd(CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void CCEditText::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#else
    //CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
    m_label->setString(text.c_str());
#endif
}

void CCEditText::editBoxReturn(CCEditBox* editBox)
{
    //m_pTTFShowEditReturn->setString(m_pEditEmail->getText());
}
NS_CC_EXT_END