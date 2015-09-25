//
//  CCLabelIFTTFTTF.cpp
//  IF
//
//  Created by chenliang on 14-3-5.
//
//

#include "CCLabelIFTTF.h"

USING_NS_CC;

#if CC_USE_LA88_LABELS
#define SHADER_PROGRAM_DE kCCShader_PositionTextureColor
#else
#define SHADER_PROGRAM_DE kCCShader_PositionTextureA8Color
#endif

void CCLabelIFTTF::setString(const char *label)
{
    Label::setString(label);
}

void CCLabelIFTTF::setColor(const ccColor3B& color)
{
    Label::setColor(color);
}

const char* CCLabelIFTTF::getString(void)
{
    return Label::getString().c_str();
}

CCTexture2D* CCLabelIFTTF::getTexture(void)
{
    return nullptr;// CCLabelTTF::getTexture();
}

void CCLabelIFTTF::setOpacity(GLubyte opacity)
{
    return Label::setOpacity(opacity);
}

float CCLabelIFTTF::getOriginScaleX()
{
    return _scaleX;
}

float CCLabelIFTTF::getOriginScaleY()
{
    return _scaleY;
}

void CCLabelIFTTF::enableStroke(const ccColor3B &strokeColor, float strokeSize, bool mustUpdateTexture)
{
}

const char* CCLabelIFTTF::getFontName()
{
    return "Arial";
}

float CCLabelIFTTF::getFontSize()
{
    return getSystemFontSize();
}

//void CCLabelIFTTF::setScale(float scale)
//{
//    m_fontScaleX = m_fontScaleY = scale;
//    CCLabelTTF::setScale(scale*m_fontSize/_defaultBMFontSize);
//}
//
//void CCLabelIFTTF::setScaleX(float scaleX)
//{
//    m_fontScaleX = scaleX;
//    CCLabelTTF::setScaleX(scaleX*m_fontSize/_defaultBMFontSize);
//}
//
//void CCLabelIFTTF::setScaleY(float scaleY)
//{
//    m_fontScaleY = scaleY;
//    CCLabelTTF::setScaleY(scaleY*m_fontSize/_defaultBMFontSize);
//}
//
//float CCLabelIFTTF::getScale()
//{
//    return m_fontScaleX;
//    //return m_fScaleX;
//}
//
//float CCLabelIFTTF::getScaleX()
//{
//    return m_fontScaleX;
//    //return m_fScaleX;
//}
//
//float CCLabelIFTTF::getScaleY()
//{
//    return m_fontScaleY;
//    //return m_fScaleY;
//}

void CCLabelIFTTF::setFontSize(float fontSize)
{
    m_fontSize = fontSize;
    Label::setSystemFontSize(fontSize);
    
    //Label::setScaleX(m_fontSize*m_fontScaleX/_defaultBMFontSize);
    //Label::setScaleY(m_fontSize*m_fontScaleY/_defaultBMFontSize);
}

void CCLabelIFTTF::setDimensions(const CCSize &dim)
{
    Label::setDimensions(dim.width,dim.height);
    
}

void CCLabelIFTTF::setVerticalAlignment(CCVerticalTextAlignment align)
{
    //CCLabelTTF::setVerticalAlignment(align);
}

void CCLabelIFTTF::setHorizontalAlignment(CCTextAlignment align)
{
    Label::setHorizontalAlignment(align);
}

void CCLabelIFTTF::setAlignment(CCTextAlignment alignment)
{
    Label::setHorizontalAlignment(alignment);
}


CCLabelIFTTF * CCLabelIFTTF::create()
{
    CCLabelIFTTF * pRet = new CCLabelIFTTF();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool CCLabelIFTTF::init(){
    return this->initWithString("", FONT_COMMON, _defaultBMFontSize,CCSizeZero,kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
}

CCLabelIFTTF * CCLabelIFTTF::create(const char *string)
{
    return CCLabelIFTTF::create(string, FONT_COMMON, _defaultBMFontSize,
                             CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
}

CCLabelIFTTF * CCLabelIFTTF::create(const char *string, float fontSize)
{
    return CCLabelIFTTF::create(string, FONT_COMMON, fontSize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
}

CCLabelIFTTF * CCLabelIFTTF::create(const char *string, const char *fontName, float fontSize)
{
    return CCLabelIFTTF::create(string, FONT_COMMON, fontSize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
}

CCLabelIFTTF* CCLabelIFTTF::create(const char *string, const char *fontName, float fontSize,
                             const CCSize &dimensions, CCTextAlignment hAlignment,
                             CCVerticalTextAlignment vAlignment)
{
    CCLabelIFTTF *pRet = new CCLabelIFTTF();
    if(pRet && pRet->initWithString(string, fontName, fontSize, dimensions, hAlignment, vAlignment))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CCLabelIFTTF* CCLabelIFTTF::create(const char *string, float fontSize,
                             const CCSize &dimensions, CCTextAlignment hAlignment,
                             CCVerticalTextAlignment vAlignment)
{
    return CCLabelIFTTF::create(string, FONT_COMMON, fontSize, dimensions, hAlignment, vAlignment);
}

CCLabelIFTTF* CCLabelIFTTF::create(const char *str, const char *fntFile, float width, CCTextAlignment alignment, CCPoint imageOffset)
{
    //this functions needs to be tuned
    return CCLabelIFTTF::create(str, FONT_COMMON, _defaultBMFontSize, CCSizeZero, alignment, kCCVerticalTextAlignmentCenter);
}

bool CCLabelIFTTF::initWithString(const char *string, const char *fontName, float fontSize,
                               const cocos2d::CCSize &dimensions, CCTextAlignment hAlignment,
                               CCVerticalTextAlignment vAlignment)
{
    if (Node::init())
    {
        // shader program
        Label::setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(SHADER_PROGRAM_DE));
        
        // init font def
        ccFontDefinition texDef;
        texDef._fontSize                       =  fontSize;
        texDef._fontName                       =  std::string(FONT_COMMON);
        texDef._alignment                      =  kCCTextAlignmentCenter;
        texDef._vertAlignment                  =  kCCVerticalTextAlignmentTop;
        texDef._dimensions                     =  CCSizeZero;
        texDef._stroke._strokeEnabled         = false;
        texDef._stroke._strokeColor           = ccBLACK;
        texDef._stroke._strokeSize            = 0.5;
//        texDef.m_shadow.m_shadowEnabled         = true;
//        texDef.m_shadow.m_shadowBlur            = 1;
//        texDef.m_shadow.m_shadowOpacity         = 1;
        
        
        texDef._shadow._shadowOffset          = CCSizeMake(0,-2);
        texDef._fontFillColor                  = ccWHITE;
        
//        _updateWithTextDefinition(texDef, false);
//        
        setDimensions(CCSizeMake(dimensions.width, dimensions.height));
        _hAlignment  = hAlignment;
        _vAlignment  = vAlignment;
//        delete m_pFontName;
        _systemFont   =  fontName;
        _systemFontSize   = fontSize;
//        
        m_fontScaleX = 1.0;
        m_fontScaleY = 1.0;
        
        Label::setString(string);
        
        return true;
    }
    
    return false;
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

void CCLabelIFTTFLoader::onHandlePropTypeColor3(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccColor3B pCCColor3B, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_COLOR) == 0) {
        ((CCLabelIFTTF *)pNode)->setColor(pCCColor3B);
    } else {
        CCNodeLoader::onHandlePropTypeColor3(pNode, pParent, pPropertyName, pCCColor3B, pCCBReader);
    }
}

void CCLabelIFTTFLoader::onHandlePropTypeByte(CCNode * pNode, CCNode * pParent, const char * pPropertyName, unsigned char pByte, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_OPACITY) == 0) {
        ((CCLabelIFTTF *)pNode)->setOpacity(pByte);
    } else {
        CCNodeLoader::onHandlePropTypeByte(pNode, pParent, pPropertyName, pByte, pCCBReader);
    }
}

void CCLabelIFTTFLoader::onHandlePropTypeBlendFunc(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccBlendFunc pCCBlendFunc, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_BLENDFUNC) == 0) {
        //        ((CCLabelIF *)pNode)->setBlendFunc(pCCBlendFunc);
    } else {
        CCNodeLoader::onHandlePropTypeBlendFunc(pNode, pParent, pPropertyName, pCCBlendFunc, pCCBReader);
    }
}

void CCLabelIFTTFLoader::onHandlePropTypeFontTTF(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pFontTTF, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTNAME) == 0) {
        // do not change font
        //        ((CCLabelIF *)pNode)->setFontName(pFontTTF);
    } else {
        CCNodeLoader::onHandlePropTypeFontTTF(pNode, pParent, pPropertyName, pFontTTF, pCCBReader);
    }
}

void CCLabelIFTTFLoader::onHandlePropTypeText(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pText, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_STRING) == 0) {
        //        ((CCLabelIF *)pNode)->setString(pText);
    } else {
        CCNodeLoader::onHandlePropTypeText(pNode, pParent, pPropertyName, pText, pCCBReader);
    }
}

void CCLabelIFTTFLoader::onHandlePropTypeFloatScale(CCNode * pNode, CCNode * pParent, const char * pPropertyName, float pFloatScale, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTSIZE) == 0) {
        ((CCLabelIFTTF *)pNode)->setFontSize(pFloatScale);
    } else {
        CCNodeLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pFloatScale, pCCBReader);
    }
}

void CCLabelIFTTFLoader::onHandlePropTypeIntegerLabeled(CCNode * pNode, CCNode * pParent, const char * pPropertyName, int pIntegerLabeled, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_HORIZONTALALIGNMENT) == 0) {
        ((CCLabelIFTTF *)pNode)->setAlignment(CCTextAlignment(pIntegerLabeled));
    } else if(strcmp(pPropertyName, PROPERTY_VERTICALALIGNMENT) == 0) {
        //        ((CCLabelIF *)pNode)->setVerticalAlignment(CCVerticalTextAlignment(pIntegerLabeled));
    } else {
        CCNodeLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pIntegerLabeled, pCCBReader);
    }
}

void CCLabelIFTTFLoader::onHandlePropTypeSize(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSize pSize, cocosbuilder::CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_DIMENSIONS) == 0) {
        ((CCLabelIFTTF *)pNode)->setDimensions(pSize);
    } else {
        CCNodeLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName, pSize, pCCBReader);
    }
}

NS_CC_EXT_END