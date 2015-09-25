//
//  CCEditLabel.cpp
//  aoe
//
//  Created by 岳文铎 on 13-3-18.
//
//

#include "CCEditLabel.h"
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

CCEditLabel * CCEditLabel::create(CCSize size, int numOfLines/*=1*/,CCPoint anchor/*=CCPointZero*/)
{
    CCEditLabel * pRet = new CCEditLabel();
    if (pRet && pRet->init(size, numOfLines,anchor))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool CCEditLabel::init(CCSize size, int numOfLines/*=1*/,CCPoint anchor/*=CCPointZero*/)
{
    bool bRet = false;
    m_size = size;
    
    m_ccbNode = CCBLoadFile("AOEEditView.ccbi",NULL,this);
    
    this->setContentSize(CCSizeMake(size.width+INDENT_WIDTH, size.height+MARGIN_HEIGHT));
    this->setScale(0.5);
    this->setSingleLine(true);
    
    bg = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
    bg->setPosition(ccp(-INDENT_WIDTH/2, 0));
    bg->setAnchorPoint(ccp(0.5, 0.5));
    bg->setPreferredSize(CCSizeMake(size.width+INDENT_WIDTH, size.height+MARGIN_HEIGHT));
    addChild(bg, -1);
    
    m_btn = CCControlButton::create(CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png"));
    m_btn->setPosition(-INDENT_WIDTH/2, 0)   ;
    m_btn->setPreferredSize(CCSize(size.width+INDENT_WIDTH, size.height+MARGIN_HEIGHT));//可点击区域变大
    m_btn->setAnchorPoint(ccp(0.5, 0.5));
    m_btn->setZoomOnTouchDown(false);
    m_btn->addTargetWithActionForControlEvents(this, cccontrol_selector(CCEditLabel::showEditView), CCControlEventTouchUpInside);
    addChild(m_btn);
    
    m_label = CCLabelTTF::create();
    m_label->setFontName("Helvetica");
    m_label->setFontSize(20);
    m_label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    m_label->setHorizontalAlignment(kCCTextAlignmentLeft);
    m_label->setColor(ccBLACK);
    m_label->setAnchorPoint(ccp(0.5, 0.5));
    m_label->setPositionY(bg->getPositionY());
    addChild(m_label);
    
    // set default char count to -1
    m_nCharLimit = -1;
    
    m_numOfLines = numOfLines;
    
    bRet = true;
    
    return bRet;
}

void CCEditLabel::onEnter()
{
    CCNode::onEnter();
//    CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);
}

void CCEditLabel::onExit()
{
    CC_SAFE_RELEASE_NULL(m_onFinish);
    CC_SAFE_RELEASE_NULL(m_onBeforeInput);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NativeSetString);
//    CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

void CCEditLabel::keyBackClicked(){
    if (m_textField)
        m_textField->detachWithIME();
}

void CCEditLabel::handleNativeSetString(cocos2d::CCObject *obj)
{
    CCString* str = (CCString*)obj;
	CCLOG(">>>>handleNativeSetString:%s",str->getCString());
    
    if(m_editLabelType == kAOEEditLabelTypeUsePlatformLayout) {
        // 若是平台提供了输入框布局，则这里直接对cocos2d层的label进行字符串赋值
        setString(str->getCString());
    }
    
	keypressedBtnOK(NULL,CCControlEventTouchUpInside);
	CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NativeSetString);
}

void CCEditLabel::setString(const char *label)
{
    if (strcmp(m_label->getString().c_str(),label) == 0){
        return;
    }
    
    int len = Utf8Utils::strlen(label);
    
    if (m_nCharLimit > 0 && len > m_nCharLimit) {
        CCLOG("input char count is exceed max length");
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
    
    if (m_isSingleLine || (strcmp("", str)==0)) {
        m_label->setString(str);
    } else {
        std::string str_temp = str;
        m_label->setString(str_temp.substr(0, 2048).c_str());
        
        // 字符串包含回车时，文字显示到框的外边，通过重置矩形大小控制。
        CCRect rect = CCRectZero;
        rect.size = m_size;
       // m_label->setTextureRect(rect);
    }
}

const char* CCEditLabel::getString(void)
{
    return m_inputStr.c_str();
}

void CCEditLabel::showEditView(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    if (m_editView) {
        m_editView->removeAllChildrenWithCleanup(true);
    }
    if (getBeforeInputCallBack()){
        getBeforeInputCallBack()->execute();
    }
    
    m_editView = CCLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(m_editView);
    m_editView->addChild(m_ccbNode);
    //    AOEUtilies::setButtonTitle(m_btnOK, _("confirm"));
    CCCommonUtils::setButtonTitle(m_OKBtn, "confirm");
    m_editView->setAnchorPoint(CCPoint(0,0));
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_bg->setPreferredSize(CCSize(winSize.width*2,winSize.height));
    m_bg->setOpacity(0);
    m_bg->setAnchorPoint(ccp(0,1));
    m_bg->setPosition(ccp(0,winSize.height));
    m_OKBtn->setPositionX(winSize.width-m_OKBtn->getPreferredSize().width*m_OKBtn->getScaleX()/2-15);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CCEditLabel::handleNativeSetString), MSG_NativeSetString, NULL);
    m_textField = CCTextFieldTTF::textFieldWithPlaceHolder(getString(), CCSize(660, 235),kCCTextAlignmentLeft , "Helvetica", 35);
    m_textField->setString(getString());
    m_textField->setScale(0.5);
    m_editView->addChild(m_textField);
    m_textField->setPosition(ccp(20,185));
    m_textField->setAnchorPoint(CCPointZero);
    m_textField->setDelegate(this);
    
    if (m_inputMode == kEditBoxInputModeNumeric) {
        m_textField->setInputType(0x02);
    }
    else{
        m_textField->setInputType(0x01);
    }
    
    if (m_isPasswordMode) {
        m_textField->setInputType(m_textField->getInputType()|0x90);//passwordMode
    }
    m_textField->setSingleLine(m_isSingleLine);
    m_textField->attachWithIME();
    
    // 注：Android 层提供了输入框的Layout布局
    m_editLabelType = kAOEEditLabelTypeUsePlatformLayout;
	m_editView->setVisible(false);
    
#else
    CCScale9Sprite *sprite = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
    m_editBox = CCEditBox::create(CCSize(380, 70), sprite);
//    m_editBox->setZoomOnTouchDown(false);
    m_editBox->setInputMode(m_inputMode);
    m_editBox->setAnchorPoint(CCPointZero);
    m_editBox->setPosition(ccp(15,210));
    m_editBox->setText(getString());
    m_editView->addChild(m_editBox);
    if (m_isPasswordMode) {
        m_editBox->setInputFlag(EditBox::InputFlag::PASSWORD);
    }
    m_editBox->touchDownAction(NULL,ui::Widget::TouchEventType::MOVED);
    
    m_editLabelType = kAOEEditLabelTypeUseCocosLayout;
#endif
}

void CCEditLabel::setInputMode(EditBoxInputMode mode)
{
    m_inputMode = mode;
}

#pragma mark -
#pragma mark CocosBuilder Part

SEL_CCControlHandler CCEditLabel::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnOk", CCEditLabel::keypressedBtnOK);
    return NULL;
}

bool CCEditLabel::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_OKBtn", CCControlButton*, this->m_OKBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    return false;
}

void CCEditLabel::keypressedBtnOK(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    CCLOG("CCEditLabel::keypressedBtnOK");
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
    if (m_textField) {
        if(m_editLabelType == kAOEEditLabelTypeUseCocosLayout) {
            std::string str = m_textField->getString();
            setString(str.substr(0, 2048).c_str());
        }
        m_editView->removeFromParentAndCleanup(true);
		m_textField->detachWithIME();
    }
    if (getFinishCallBack()){
        getFinishCallBack()->execute();
    }
}

void CCEditLabel::keyboardWillHide(CCIMEKeyboardNotificationInfo& info)
{
    keypressedBtnOK(NULL,CCControlEventTouchUpInside);
}

bool CCEditLabel::onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
    keypressedBtnOK(NULL,CCControlEventTouchUpInside);
	if (!m_editView || !m_editView->getParent()) {
        m_editView->removeFromParentAndCleanup(true);
	}
    return false;
}

void CCEditLabel::setMaxLength(int length)
{
    m_nCharLimit = length;
}

bool CCEditLabel::onTextFieldInsertText(cocos2d::CCTextFieldTTF *sender, const char *text, int nLen)
{
	if (!m_editView || !m_editView->getParent()) {
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
    if (m_textField) {
        setString(m_textField->getString().c_str());
    }
    return false;
}

void CCEditLabel::setEnabled(bool isEnable)
{
    m_btn->setEnabled(isEnable);
}

void CCEditLabel::enablePasswordMode(bool isEnable)
{
    m_isPasswordMode = isEnable;
}
void CCEditLabel::setHorizontalAlignment(CCTextAlignment alignment)
{
    m_label->setDimensions(CCSizeMake(m_size.width-5, 0));
    m_label->setHorizontalAlignment(alignment);
}
void CCEditLabel::setScale(float scale)
{
    CCNode::setScale(1);
}

void CCEditLabel::setSingleLine(bool isSingleLine)
{
    m_isSingleLine = isSingleLine;// (for android only)true:click enter to finish/false:click enter to change line
    if (!m_isSingleLine) {
        m_label->setDimensions(CCSizeMake(m_size.width, m_size.height));
        m_label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    }
}
//
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#   include <jni.h>
//#   include "platform/android/jni/JniHelper.h"
//#ifdef __cplusplus
//extern "C" {
//#endif
//    void Java_org_cocos2dx_lib_Cocos2dxGLSurfaceView_nativeSetString(JNIEnv* env, jobject thiz, jstring str)
//    {
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_NativeSetString,CCString::create(JniHelper::jstring2string(str).c_str()));
//    }
//#ifdef __cplusplus
//}
//#endif
//#endif
NS_CC_EXT_END