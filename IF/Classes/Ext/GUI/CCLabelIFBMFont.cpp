//
//  CCLabelIFBMFont.cpp
//  IF
//
//  Created by chenliang on 14-3-5.
//
//

#include "CCLabelIFBMFont.h"

static const float _defaultBMFontSize = 30.0;
#define BMFONT_NAME "Arial_Bold.fnt"

USING_NS_CC;

const char* CCLabelIFBMFont::getString()
{
    return CCLabelBMFont::getString().c_str();
}

void CCLabelIFBMFont::setFntFile(const char* fntFile)
{
    CCLabelBMFont::setFntFile(fntFile);
//    CCLabelBMFont::getTexture()->setAntiAliasTexParameters();
}

void CCLabelIFBMFont::setFontSize(float fontSize) {
    m_fontSize = fontSize;
    float temp = m_fontSize*m_fontScaleX*1.0/_defaultBMFontSize;
    CCLabelBMFont::setScaleX(temp);
    CCLabelBMFont::setScaleY(m_fontSize*m_fontScaleY*1.0/_defaultBMFontSize);
}

float CCLabelIFBMFont::getFontSize() {
    return m_fontSize;
}

const char* CCLabelIFBMFont::getFontName() {
    return m_fontName.c_str();
}

void CCLabelIFBMFont::setHorizontalAlignment(CCTextAlignment align) {
    CCLabelBMFont::setAlignment(align);
}

void CCLabelIFBMFont::setVerticalAlignment(CCVerticalTextAlignment align) {
    // todo
}

void CCLabelIFBMFont::setDimensions(const cocos2d::CCSize &dim) {
   // CCLabelBMFont::setWidth(dim.width);
    CCLabelBMFont::setWidth(dim.width/m_fontSize*_defaultBMFontSize);
}

void CCLabelIFBMFont::setScale(float scale) {
    m_fontScaleX = m_fontScaleY = scale;
    CCLabelBMFont::setScale(scale*m_fontSize/_defaultBMFontSize);
}

void CCLabelIFBMFont::setScaleX(float scale) {
    m_fontScaleX = scale;
    CCLabelBMFont::setScaleX(scale*m_fontSize/_defaultBMFontSize);
}

void CCLabelIFBMFont::setScaleY(float scale) {
    m_fontScaleY = scale;
    CCLabelBMFont::setScaleY(scale*m_fontSize/_defaultBMFontSize);
}

float CCLabelIFBMFont::getScale() {
    CCAssert(m_fontScaleX == m_fontScaleY, "Don't know which to return!");
    return m_fontScaleX;
}

float CCLabelIFBMFont::getScaleX() {
    return m_fontScaleX;
}

float CCLabelIFBMFont::getScaleY() {
    return m_fontScaleY;
}

float CCLabelIFBMFont::getOriginScaleX() {
    return _scaleX;
}

float CCLabelIFBMFont::getOriginScaleY() {
    return _scaleY;
}


// backward interface
CCLabelIFBMFont* CCLabelIFBMFont::create(const char *string, float fontSize, const cocos2d::CCSize &dimensions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment) {
    auto label = CCLabelIFBMFont::create(string,BMFONT_NAME);
    label->setFontSize(fontSize);
    label->setDimensions(dimensions);
    label->setHorizontalAlignment(hAlignment);
    label->setVerticalAlignment(vAlignment);
    return label;
}

void CCLabelIFBMFont::enableStroke(const ccColor3B &strokeColor, float strokeSize,bool mustUpdateTexture) {
    // todo
}

CCLabelIFBMFont * CCLabelIFBMFont::create()
{
    return create("");
}

CCLabelIFBMFont * CCLabelIFBMFont::create(const char *str) {
    return CCLabelIFBMFont::create(str, 24);
}

CCLabelIFBMFont * CCLabelIFBMFont::create(const char *str, float fontSize) {
    auto label = CCLabelIFBMFont::create(str,BMFONT_NAME);
    label->setFontSize(fontSize);
    return label;
}

CCLabelIFBMFont * CCLabelIFBMFont::create(const char *str, const char *fntFile, float width, CCTextAlignment alignment)
{
    return CCLabelIFBMFont::create(str, fntFile, width, alignment, CCPointZero);
}

CCLabelIFBMFont * CCLabelIFBMFont::create(const char *str, const char *fontName, float fontSize)
{
    return CCLabelIFBMFont::create(str, fontSize);
}

CCLabelIFBMFont * CCLabelIFBMFont::create(const char *str, const char *fntFile)
{
    auto ret = CCLabelIFBMFont::create(str, fntFile, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
    ret->setFontSize(24);
    return ret;
}

CCLabelIFBMFont * CCLabelIFBMFont::create(const char *fontName, float fontSize, CCTextAlignment alignment){
    auto label = CCLabelIFBMFont::create("",fontName);
    label->setFontSize(fontSize);
    label->CCLabelBMFont::setAlignment(alignment);
    return NULL;
}

CCLabelIFBMFont *CCLabelIFBMFont::create(const char *str, const char *fntFile, float width/* = kCCLabelAutomaticWidth*/, CCTextAlignment alignment/* = kCCTextAlignmentLeft*/, CCPoint imageOffset/* = CCPointZero*/)
{
    CCLabelIFBMFont *pRet = new CCLabelIFBMFont();
    if(pRet && pRet->initWithString(str, fntFile, width, alignment, imageOffset))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCLabelIFBMFont::init()
{
    return initWithString("", BMFONT_NAME, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
}

bool CCLabelIFBMFont::initWithString(const char *theString, const char *fntFile, float width/* = kCCLabelAutomaticWidth*/, CCTextAlignment alignment/* = kCCTextAlignmentLeft*/, CCPoint imageOffset/* = CCPointZero*/)
{
    bool ret = false;
    if (CCLabelBMFont::initWithString(theString, fntFile,width,alignment,imageOffset)) {
        
        // todo: do CCLabelIFBMFont init here
        m_fontName = "Arial";
        m_fontSize = 24.0;
        m_fontScaleX = 1.0;
        m_fontScaleY = 1.0;
//        CCLabelBMFont::getTexture()->setAntiAliasTexParameters();
        ret = true;
    }
    return ret;
}

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

void CCLabelIFBMFontLoader::onHandlePropTypeColor3(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccColor3B pCCColor3B, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_COLOR) == 0) {
        ((CCLabelIFBMFont *)pNode)->setColor(pCCColor3B);
    } else {
        CCNodeLoader::onHandlePropTypeColor3(pNode, pParent, pPropertyName, pCCColor3B, pCCBReader);
    }
}

void CCLabelIFBMFontLoader::onHandlePropTypeByte(CCNode * pNode, CCNode * pParent, const char * pPropertyName, unsigned char pByte, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_OPACITY) == 0) {
        ((CCLabelIFBMFont *)pNode)->setOpacity(pByte);
    } else {
        CCNodeLoader::onHandlePropTypeByte(pNode, pParent, pPropertyName, pByte, pCCBReader);
    }
}

void CCLabelIFBMFontLoader::onHandlePropTypeBlendFunc(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccBlendFunc pCCBlendFunc, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_BLENDFUNC) == 0) {
        //        ((CCLabelIF *)pNode)->setBlendFunc(pCCBlendFunc);
    } else {
        CCNodeLoader::onHandlePropTypeBlendFunc(pNode, pParent, pPropertyName, pCCBlendFunc, pCCBReader);
    }
}

void CCLabelIFBMFontLoader::onHandlePropTypeFontTTF(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pFontTTF, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTNAME) == 0) {
        // do not change font
        //        ((CCLabelIF *)pNode)->setFontName(pFontTTF);
    } else {
        CCNodeLoader::onHandlePropTypeFontTTF(pNode, pParent, pPropertyName, pFontTTF, pCCBReader);
    }
}

void CCLabelIFBMFontLoader::onHandlePropTypeText(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pText, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_STRING) == 0) {
//        ((CCLabelIFBMFont *)pNode)->setString(pText);
    } else {
        CCNodeLoader::onHandlePropTypeText(pNode, pParent, pPropertyName, pText, pCCBReader);
    }
}

void CCLabelIFBMFontLoader::onHandlePropTypeFloatScale(CCNode * pNode, CCNode * pParent, const char * pPropertyName, float pFloatScale, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTSIZE) == 0) {
        ((CCLabelIFBMFont *)pNode)->setFontSize(pFloatScale);
    } else {
        CCNodeLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pFloatScale, pCCBReader);
    }
}

void CCLabelIFBMFontLoader::onHandlePropTypeIntegerLabeled(CCNode * pNode, CCNode * pParent, const char * pPropertyName, int pIntegerLabeled, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_HORIZONTALALIGNMENT) == 0) {
        ((CCLabelIFBMFont *)pNode)->setAlignment(CCTextAlignment(pIntegerLabeled));
    } else if(strcmp(pPropertyName, PROPERTY_VERTICALALIGNMENT) == 0) {
        //        ((CCLabelIF *)pNode)->setVerticalAlignment(CCVerticalTextAlignment(pIntegerLabeled));
    } else {
        CCNodeLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pIntegerLabeled, pCCBReader);
    }
}

void CCLabelIFBMFontLoader::onHandlePropTypeSize(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSize pSize, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_DIMENSIONS) == 0) {
        ((CCLabelIFBMFont *)pNode)->setDimensions(pSize);
    } else {
        CCNodeLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName, pSize, pCCBReader);
    }
}

NS_CC_EXT_END