//
//  CCLabelIF.cpp
//  IF
//
//  Created by 邹 程 on 13-9-17.
//
//

#include "CCLabelIF.h"
#include "LocalController.h"

NS_CC_BEGIN

#if CC_USE_LA88_LABELS
#define SHADER_PROGRAM_DE kCCShader_PositionTextureColor
#else
#define SHADER_PROGRAM_DE kCCShader_PositionTextureA8Color
#endif

static const float _defaultBMFontSize = 30.0;
#define BMFONT_NAME "Arial_Bold.fnt"

bool CCLabelIF::m_useBMFont = true;
bool CCLabelIF::m_useRichFont = false;

void CCLabelIF::setMaxScaleXByWidth(float width){
    float fw = getContentSize().width;
    if (fw>width) {//防爆框
        setScaleX(width/fw);
    }
}

bool CCLabelIF::canUseRich(std::string language){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    int length = language.length();
    return language.find("ar") == 0;
#endif
    return false;
}

bool CCLabelIF::canBeSupportedWithBmpFont(std::string language)
{
    int length = language.length();
    CCLOG("liudi support lan, %s, %d", language.c_str(),length);
    
    if(language.find("zh_TW") < length ||
       language.find("zh_CN") < length ||
       language.find("zh-Hans") < length ||
       language.find("ko")< length ||
       language.find("ja")< length ||
       language.find("th")< length ||
       language.find("pt")< length ||
       language.find("ar")< length ||
       language.find("nl")< length ||
       language.find("fr")< length ||
       language.find("it")< length ||
       language.find("pl")< length ||
       language.find("vi")< length ||
       language.find("ru")< length ||
       language.find("fa")< length ||
       language.find("tr")< length){
        return false;
    }
    else
    {
        return true;
    }
}

void CCLabelIF::initFont()
{
    std::string language = LocalController::shared()->getLanguageFileName();
    if (canBeSupportedWithBmpFont(language))
    {
        enableBmFont(true);
    }
    else
    {
        enableBmFont(false);
    }
    if (canUseRich(language)) {
        enableRichFont(true);
    }else{
        enableRichFont(false);
    }
}

void CCLabelIF::enableBmFont(bool enabled)
{
    m_useBMFont = enabled;
}

void CCLabelIF::enableRichFont(bool enabled)
{
    m_useRichFont = enabled;
}

void CCLabelIF::setAnchorPoint(const CCPoint& anchorPoint)
{
    
    if (m_bmFont)
    {
        return m_bmFont->setAnchorPoint(anchorPoint);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setAnchorPoint(anchorPoint);
    }
    else
    {
        return m_ttfFont->setAnchorPoint(anchorPoint);
    }
}
const ccColor3B& CCLabelIF::getColor()
{
    if (m_bmFont)
    {
        return m_bmFont->getColor();
    }
    else if(m_useRichFont)
    {
        return m_richFont->getColor();
    }
    else
    {
        return m_ttfFont->getColor();
    }
}

const ccColor3B& CCLabelIF::getDisplayedColor()
{
    if (m_bmFont)
    {
        return m_bmFont->getDisplayedColor();
    }
    else if(m_useRichFont)
    {
        return m_richFont->getDisplayedColor();
    }
    else
    {
        return m_ttfFont->getDisplayedColor();
    }
}

GLubyte CCLabelIF::getDisplayedOpacity()
{
    if (m_bmFont)
    {
        return m_bmFont->getDisplayedOpacity();
    }
    else if(m_useRichFont)
    {
        return m_richFont->getDisplayedOpacity();
    }
    else
    {
        return m_ttfFont->getDisplayedOpacity();
    }
}

void CCLabelIF::setOpacityModifyRGB(bool bValue)
{
    if (m_bmFont)
    {
        m_bmFont->setOpacityModifyRGB(bValue);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setOpacityModifyRGB(bValue);
    }
    else
    {
        m_ttfFont->setOpacityModifyRGB(bValue);
    }
}

bool CCLabelIF::isOpacityModifyRGB()
{
    if (m_bmFont)
    {
        return m_bmFont->isOpacityModifyRGB();
    }
    else if(m_useRichFont)
    {
        return m_richFont->isOpacityModifyRGB();
    }
    else
    {
        return m_ttfFont->isOpacityModifyRGB();
    }
}

bool CCLabelIF::isCascadeColorEnabled()
{
    if (m_bmFont)
    {
        return m_bmFont->isCascadeColorEnabled();
    }
    else if(m_useRichFont)
    {
        return m_richFont->isCascadeColorEnabled();
    }
    else
    {
        return m_ttfFont->isCascadeColorEnabled();
    }
}

void CCLabelIF::setCascadeColorEnabled(bool cascadeColorEnabled)
{
    if (m_bmFont)
    {
        m_bmFont->setOpacityModifyRGB(cascadeColorEnabled);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setOpacityModifyRGB(cascadeColorEnabled);
    }
    else
    {
        m_ttfFont->setOpacityModifyRGB(cascadeColorEnabled);
    }
}

void CCLabelIF::updateDisplayedColor(const ccColor3B& color)
{
    if (m_bmFont)
    {
        m_bmFont->updateDisplayedColor(color);
    }
    else if(m_useRichFont)
    {
        return m_richFont->updateDisplayedColor(color);
    }
    else
    {
        m_ttfFont->updateDisplayedColor(color);
    }
}

bool CCLabelIF::isCascadeOpacityEnabled()
{
    if (m_bmFont)
    {
        return m_bmFont->isCascadeOpacityEnabled();
    }
    else if(m_useRichFont)
    {
        return m_richFont->isCascadeOpacityEnabled();
    }
    else
    {
        return m_ttfFont->isCascadeOpacityEnabled();
    }
}

void CCLabelIF::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    if (m_bmFont)
    {
        m_bmFont->setCascadeOpacityEnabled(cascadeOpacityEnabled);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setCascadeOpacityEnabled(cascadeOpacityEnabled);
    }
    else
    {
        m_ttfFont->setCascadeOpacityEnabled(cascadeOpacityEnabled);
    }
}

void CCLabelIF::updateDisplayedOpacity(GLubyte opacity)
{
    if (m_bmFont)
    {
        m_bmFont->updateDisplayedOpacity(opacity);
    }
    else if(m_useRichFont)
    {
        m_richFont->updateDisplayedOpacity(opacity);
    }
    else
    {
        m_ttfFont->updateDisplayedOpacity(opacity);
    }
}

void CCLabelIF::setAlignment(CCTextAlignment align)
{
    if (m_bmFont)
    {
        m_bmFont->setAlignment(align);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setHorizontalAlignment(align);
    }
    else
    {
        m_ttfFont->setAlignment(align);
    }
}

void CCLabelIF::setFontSize(float fontSize) {
    if (m_bmFont)
    {
        m_bmFont->setFontSize(fontSize);
    }
    else if(m_useRichFont)
    {
        m_richFont->setFontSize(fontSize);
    }
    else
    {
        m_ttfFont->setFontSize(fontSize);
    }
}

float CCLabelIF::getFontSize() {
    if (m_bmFont)
    {
        return m_bmFont->getFontSize();
    }
    else if(m_useRichFont)
    {
        return m_richFont->getFontSize();
    }
    else
    {
        return m_ttfFont->getFontSize();
    }
}

const char* CCLabelIF::getFontName()
{
    if (m_bmFont)
    {
        return m_bmFont->getFontName();
    }
    else if(m_useRichFont)
    {
        return m_richFont->getFontName();
    }
    else
    {
        return m_ttfFont->getFontName();
    }
}

void CCLabelIF::setHorizontalAlignment(CCTextAlignment align)
{
    if (m_bmFont)
    {
        return m_bmFont->setHorizontalAlignment(align);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setHorizontalAlignment(align);
    }
    else
    {
        return m_ttfFont->setHorizontalAlignment(align);
    }
}

void CCLabelIF::setVerticalAlignment(CCVerticalTextAlignment align)
{
    if (m_bmFont)
    {
        return m_bmFont->setVerticalAlignment(align);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setVerticalAlignment(align);
    }
    else
    {
        return m_ttfFont->setVerticalAlignment(align);
    }
}

void CCLabelIF::setDimensions(const cocos2d::CCSize &dim)
{
    if (m_bmFont)
    {
        float scale = this->getScaleX();
        auto size = CCSizeMake(dim.width*scale, dim.height);
//        return m_bmFont->setDimensions(size);
        return  m_bmFont->setDimensions(dim);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setDimensions(dim);
    }
    else
    {
        return m_ttfFont->setDimensions(dim);
    }
}

void CCLabelIF::setScale(float scale) {
    if (m_bmFont)
    {
        return m_bmFont->setScale(scale);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setScale(scale);
    }
    else
    {
        return m_ttfFont->setScale(scale);
    }
}

CCNode* CCLabelIF::getFontControl()
{
    if (m_bmFont)
    {
        return m_bmFont;
    }
    else if(m_useRichFont)
    {
        return m_richFont;
    }
    else
    {
        return m_ttfFont;
    }
}

CCNode* CCLabelIF::getParent()
{
    if (m_bmFont) {
        return m_bmFont->getParent();
    }
    else if(m_useRichFont)
    {
        return m_richFont->getParent();
    }
    else
    {
        return m_ttfFont->getParent();
    }
}

CCRect CCLabelIF::boundingBox(void)
{
    if (m_bmFont)
    {
        return m_bmFont->boundingBox();
    }
    else if(m_useRichFont)
    {
        return m_richFont->boundingBox();
    }
    else
    {
        return m_ttfFont->boundingBox();
    }
}

void CCLabelIF::setScaleX(float scale) {
    
    if (m_bmFont)
    {
        return m_bmFont->setScaleX(scale);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setScaleX(scale);
    }
    else
    {
        return m_ttfFont->setScaleX(scale);
    }
}

void CCLabelIF::setScaleY(float scale) {
    if (m_bmFont)
    {
        return m_bmFont->setScaleY(scale);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setScaleY(scale);
    }
    else
    {
        return m_ttfFont->setScaleY(scale);
    }
}

float CCLabelIF::getScale() {
    if (m_bmFont)
    {
        return m_bmFont->getScale()/2;
    }
    else if(m_useRichFont)
    {
        return m_richFont->getScale();
    }
    else
    {
        return m_ttfFont->getScale();
    }
}

float CCLabelIF::getScaleX() {
    if (m_bmFont)
    {
        return m_bmFont->getScaleX();
    }
    else if(m_useRichFont)
    {
        return m_richFont->getScaleX();
    }
    else
    {
        return m_ttfFont->getScaleX();
    }
}

float CCLabelIF::getScaleY() {
    if (m_bmFont)
    {
        return m_bmFont->getScaleY();
    }
    else if(m_useRichFont)
    {
        return m_richFont->getScaleY();
    }
    else
    {
        return m_ttfFont->getScaleY();
    }
}

float CCLabelIF::getOriginScaleX()
{
    if (m_bmFont)
    {
        return m_bmFont->getOriginScaleX();
    }
    else if(m_useRichFont)
    {
        return m_richFont->getScaleX();
    }
    else
    {
        return m_ttfFont->getOriginScaleX();
    }
}

float CCLabelIF::getOriginScaleY() {
    if (m_bmFont)
    {
        return m_bmFont->getOriginScaleY();
    }
    else if(m_useRichFont)
    {
        return m_richFont->getScaleY();
    }
    else
    {
        return m_ttfFont->getOriginScaleY();
    }
}

void CCLabelIF::setString(const char *label)
{
    if (m_bmFont)
    {
        m_bmFont->setString(label);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setString(label);
    }
    else
    {
        m_ttfFont->setString(label);
    }
    _originalUTF8String = label;
}

void CCLabelIF::setString(const std::string& str) {
    return setString(str.c_str());
}

void CCLabelIF::setString(const char *format,bool isMulti,...) {
    
    if (!isMulti) {
        setString(format);
        return;
    }
    
    std::string tmpStr;
    va_list ap;
    va_start(ap, isMulti);
    
    char* pBuf = (char*)malloc(1024000);
    vsnprintf(pBuf, 1024000, format, ap);
    tmpStr = pBuf;
    free(pBuf);
    
    va_end(ap);
    
    setString(tmpStr.c_str());
}

void CCLabelIF::enableStroke(const ccColor3B &strokeColor, float strokeSize,bool mustUpdateTexture)
{
    if (m_bmFont)
    {
        m_bmFont->enableStroke(strokeColor, strokeSize, mustUpdateTexture);
    }
    else if(m_useRichFont)
    {
        return m_richFont->enableStroke(strokeColor, strokeSize, mustUpdateTexture);
    }
    else
    {
        m_ttfFont->enableStroke(strokeColor, strokeSize, mustUpdateTexture);
    }
}

bool CCLabelIF::init()
{
    if (m_bmFont)
    {
        return m_bmFont->init();
    }
    else
    {
        return m_ttfFont->init();
    }
}

void CCLabelIF::setFntFile(const char* fntFile)
{
    if (m_bmFont)
    {
        return m_bmFont->setFntFile(fntFile);
    }
    else
    {
    }
}

void CCLabelIF::setColor(const ccColor3B& color)
{
    if (m_bmFont)
    {
        return m_bmFont->setColor(color);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setFontFillColor(color);
    }
    else
    {
        return m_ttfFont->setColor(color);
    }
}

const std::string& CCLabelIF::getString() const
{
    return _originalUTF8String;
//    if (m_bmFont)
//    {
//        return std::string(m_bmFont->getString());
//    }
//    else if(m_useRichFont)
//    {
//        return std::string(m_richFont->getString().c_str());
//    }
//    else
//    {
//        return std::string(m_ttfFont->getString());
//    }
}

std::string CCLabelIF::getIBMDescription() const
{
    if( m_bmFont)
        return m_bmFont->getDescription();
    return "NULL";
}

const CCSize& CCLabelIF::getContentSize()
{
    if (m_bmFont)
    {
        return m_bmFont->getContentSize();
    }
    else if(m_useRichFont)
    {
        return m_richFont->getContentSize();
    }
    else
    {
        return m_ttfFont->getContentSize();
    }
}

CCTexture2D* CCLabelIF::getTexture(void)
{
    if (m_bmFont)
    {
        return NULL; //m_bmFont->getTexture();  //TODO getTexture
    }
    else if(m_useRichFont)
    {
        return m_richFont->getTexture();
    }
    else
    {
        return m_ttfFont->getTexture();
    }
}

GLubyte CCLabelIF::getOpacity(void)
{
    if (m_bmFont)
    {
        return m_bmFont->getOpacity();
    }
    else if(m_useRichFont)
    {
        return m_richFont->getOpacity();
    }
    else
    {
        return m_ttfFont->getOpacity();
    }
}

void CCLabelIF::setOpacity(GLubyte opacity)
{
    if (m_bmFont)
    {
        return m_bmFont->setOpacity(opacity);
    }
    else if(m_useRichFont)
    {
        return m_richFont->setOpacity(opacity);
    }
    else
    {
        return m_ttfFont->setOpacity(opacity);
    }
}

// backward interface
CCLabelIF* CCLabelIF::create(const char *string, float fontSize, const cocos2d::CCSize &dimensions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment) {
    auto label = CCLabelIF::create(string,BMFONT_NAME);
    label->setFontSize(fontSize);
    label->setDimensions(dimensions);
    label->setHorizontalAlignment(hAlignment);
    label->setVerticalAlignment(vAlignment);
    return label;
}

CCLabelIF * CCLabelIF::create()
{
    return create("");
    
    
}

CCLabelIF * CCLabelIF::create(const char *str) {
    return CCLabelIF::create(str, 24);
}

CCLabelIF * CCLabelIF::create(const char *str, float fontSize) {
    auto label = CCLabelIF::create(str,BMFONT_NAME);
    label->setFontSize(fontSize);
    return label;
}

CCLabelIF * CCLabelIF::create(const char *str, const char *fntFile, float width, CCTextAlignment alignment)
{
    return CCLabelIF::create(str, fntFile, width, alignment, CCPointZero);
}

CCLabelIF * CCLabelIF::create(const char *str, const char *fontName, float fontSize)
{
    return CCLabelIF::create(str, fontSize);
}

CCLabelIF * CCLabelIF::create(const char *str, const char *fntFile)
{
    auto ret = CCLabelIF::create(str, fntFile, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
    ret->setFontSize(24);
    return ret;
}

CCLabelIF * CCLabelIF::create(const char *fontName, float fontSize, CCTextAlignment alignment){
    auto label = CCLabelIF::create("",fontName);
    label->setFontSize(fontSize);
    label->setAlignment(alignment);
    return NULL;
}

CCLabelIF *CCLabelIF::create(const char *str, const char *fntFile, float width/* = kCCLabelAutomaticWidth*/, CCTextAlignment alignment/* = kCCTextAlignmentLeft*/, CCPoint imageOffset/* = CCPointZero*/)
{
    bool created = false;
    
    CCLabelIF *pRet = new CCLabelIF();
    if (m_useBMFont)
    {
        pRet->m_richFont = NULL;
        pRet->m_ttfFont = NULL;
        pRet->m_bmFont = CCLabelIFBMFont::create(str, fntFile, width, alignment, imageOffset);
        if (pRet && pRet->m_bmFont)
        {
            pRet->addChild(pRet->m_bmFont);
            created = true;
        }
    }
    else if (m_useRichFont)
    {
        pRet->m_bmFont = NULL;
        pRet->m_ttfFont = NULL;
        
        pRet->m_richFont = CCRichLabelTTF::create(str, "Helvetica", 20,CCSize(width , 0), alignment);
        if (pRet && pRet->m_richFont)
        {
            pRet->addChild(pRet->m_richFont);
            created = true;
        }
    }
    else
    {
        pRet->m_richFont = NULL;
        pRet->m_bmFont = NULL;
        pRet->m_ttfFont = CCLabelIFTTF::create(str, fntFile, width, alignment, imageOffset);
        if (pRet && pRet->m_ttfFont)
        {
            pRet->addChild(pRet->m_ttfFont);
            created = true;
        }
    }
    
    if (created)
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
    
}



NS_CC_END

#define PROPERTY_COLOR "color"
#define PROPERTY_OPACITY "opacity"
#define PROPERTY_BLENDFUNC "blendFunc"
#define PROPERTY_FONTNAME "fontName"
#define PROPERTY_FONTSIZE "fontSize"
#define PROPERTY_HORIZONTALALIGNMENT "horizontalAlignment"
#define PROPERTY_VERTICALALIGNMENT "verticalAlignment"
#define PROPERTY_STRING "string"
#define PROPERTY_DIMENSIONS "dimensions"

NS_CC_EXT_BEGIN
void CCLabelIFLoader::onHandlePropTypeColor3(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccColor3B pCCColor3B, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_COLOR) == 0) {
        ((CCLabelIF *)pNode)->setColor(pCCColor3B);
    } else {
        CCNodeLoader::onHandlePropTypeColor3(pNode, pParent, pPropertyName, pCCColor3B, pCCBReader);
    }
}

void CCLabelIFLoader::onHandlePropTypeByte(CCNode * pNode, CCNode * pParent, const char * pPropertyName, unsigned char pByte, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_OPACITY) == 0) {
        ((CCLabelIF *)pNode)->setOpacity(pByte);
    } else {
        CCNodeLoader::onHandlePropTypeByte(pNode, pParent, pPropertyName, pByte, pCCBReader);
    }
}

void CCLabelIFLoader::onHandlePropTypeBlendFunc(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccBlendFunc pCCBlendFunc, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_BLENDFUNC) == 0) {
//        ((CCLabelIF *)pNode)->setBlendFunc(pCCBlendFunc);
    } else {
        CCNodeLoader::onHandlePropTypeBlendFunc(pNode, pParent, pPropertyName, pCCBlendFunc, pCCBReader);
    }
}

void CCLabelIFLoader::onHandlePropTypeFontTTF(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pFontTTF, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTNAME) == 0) {
        // do not change font
        //        ((CCLabelIF *)pNode)->setFontName(pFontTTF);
    } else {
        CCNodeLoader::onHandlePropTypeFontTTF(pNode, pParent, pPropertyName, pFontTTF, pCCBReader);
    }
}

void CCLabelIFLoader::onHandlePropTypeText(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pText, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_STRING) == 0) {
//        ((CCLabelIF *)pNode)->setString(pText);
    } else {
        CCNodeLoader::onHandlePropTypeText(pNode, pParent, pPropertyName, pText, pCCBReader);
    }
}

void CCLabelIFLoader::onHandlePropTypeFloatScale(CCNode * pNode, CCNode * pParent, const char * pPropertyName, float pFloatScale, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTSIZE) == 0) {
        ((CCLabelIF *)pNode)->setFontSize(pFloatScale);
    } else {
        CCNodeLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pFloatScale, pCCBReader);
    }
}

void CCLabelIFLoader::onHandlePropTypeIntegerLabeled(CCNode * pNode, CCNode * pParent, const char * pPropertyName, int pIntegerLabeled, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_HORIZONTALALIGNMENT) == 0) {
        ((CCLabelIF *)pNode)->setAlignment(CCTextAlignment(pIntegerLabeled));
    } else if(strcmp(pPropertyName, PROPERTY_VERTICALALIGNMENT) == 0) {
//        ((CCLabelIF *)pNode)->setVerticalAlignment(CCVerticalTextAlignment(pIntegerLabeled));
    } else {
        CCNodeLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pIntegerLabeled, pCCBReader);
    }
}

void CCLabelIFLoader::onHandlePropTypeSize(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSize pSize, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_DIMENSIONS) == 0) {
        ((CCLabelIF *)pNode)->setDimensions(pSize);
    } else {
        CCNodeLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName, pSize, pCCBReader);
    }
}

NS_CC_EXT_END