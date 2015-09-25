/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-better
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "CCTexture2D_richlabel.h"
#include "VolatileTexture_richlabel.h"

NS_CC_BEGIN

CCTexture2D_richlabel::CCTexture2D_richlabel() :
m_shadowStrokePadding(CCPointZero) {
}

CCTexture2D_richlabel::~CCTexture2D_richlabel() {
}

const char* CCTexture2D_richlabel::description(void) {
    return CCString::createWithFormat("<CCTexture2D_richlabel | Name = %u | Dimensions = %u x %u | Coordinates = (%.2f, %.2f)>", _name, _pixelsWide, _pixelsHigh, _maxS, _maxT)->getCString();
}

bool CCTexture2D_richlabel::initWithRichString(const char *text, const char *fontName, float fontSize) {
    return initWithRichString(text,  fontName, fontSize, CCSizeMake(0,0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
}

bool CCTexture2D_richlabel::initWithRichString(const char *text, const char *fontName, float fontSize, const CCSize& dimensions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
	ccRichFontDefinition tempDef;
	
	tempDef._shadow._shadowEnabled = false;
	tempDef._stroke._strokeEnabled = false;
	
	
	tempDef._fontName      = std::string(fontName);
	tempDef._fontSize      = fontSize;
	tempDef._dimensions    = dimensions;
	tempDef._alignment     = hAlignment;
	tempDef._vertAlignment = vAlignment;
	tempDef._fontFillColor = ccWHITE;
    tempDef.m_shadowColor = 0;
    
	return initWithRichString(text, &tempDef);
	
#else
    CCLOGWARN("CCTexture2D_richlabel::initWithRichString only support iOS and Android");
    return false;
#endif
}

bool CCTexture2D_richlabel::initWithRichString(const char *text, ccRichFontDefinition *textDefinition) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
#if CC_ENABLE_CACHE_TEXTURE_DATA
	// cache the texture data
    VolatileTexture_richlabel::addRichStringTexture(this, text, *textDefinition);
#endif
	
	bool bRet = false;
    Device::TextAlign eAlign;
	
	if (kCCVerticalTextAlignmentTop == textDefinition->_vertAlignment)
	{
        eAlign = (kCCTextAlignmentCenter == textDefinition->_alignment) ? Device::TextAlign::TOP
		: (kCCTextAlignmentLeft == textDefinition->_alignment) ? Device::TextAlign::TOP_LEFT : Device::TextAlign::TOP_RIGHT;
	}
	else if (kCCVerticalTextAlignmentCenter == textDefinition->_vertAlignment)
	{
        eAlign = (kCCTextAlignmentCenter == textDefinition->_alignment) ? Device::TextAlign::CENTER
		: (kCCTextAlignmentLeft == textDefinition->_alignment) ? Device::TextAlign::LEFT : Device::TextAlign::RIGHT;
	}
	else if (kCCVerticalTextAlignmentBottom == textDefinition->_vertAlignment)
	{
		eAlign = (kCCTextAlignmentCenter == textDefinition->_alignment) ? Device::TextAlign::BOTTOM
		: (kCCTextAlignmentLeft == textDefinition->_alignment) ? Device::TextAlign::BOTTOM_LEFT : Device::TextAlign::BOTTOM_RIGHT;
	}
	else
	{
		CCAssert(false, "Not supported alignment format!");
		return false;
	}
	
	// handle shadow parameters
	bool  shadowEnabled =  false;
	float shadowDX      = 0.0;
	float shadowDY      = 0.0;
	float shadowBlur    = 0.0;
    int shadowColor     = 0;
	
	if ( textDefinition->_shadow._shadowEnabled )
	{
		shadowEnabled =  true;
		shadowDX      = textDefinition->_shadow._shadowOffset.width;
		shadowDY      = textDefinition->_shadow._shadowOffset.height;
		shadowBlur    = textDefinition->_shadow._shadowBlur;
        shadowColor   = textDefinition->m_shadowColor;
	}
	
	// handle stroke parameters
	bool strokeEnabled = false;
	float strokeColorR = 0.0;
	float strokeColorG = 0.0;
	float strokeColorB = 0.0;
	float strokeSize   = 0.0;
	
	if ( textDefinition->_stroke._strokeEnabled )
	{
		strokeEnabled = true;
		strokeColorR = textDefinition->_stroke._strokeColor.r / 255.0f;
		strokeColorG = textDefinition->_stroke._strokeColor.g / 255.0f;
		strokeColorB = textDefinition->_stroke._strokeColor.b / 255.0f;
		strokeSize   = textDefinition->_stroke._strokeSize;
	}
	
	CCImage_richlabel* pImage = new CCImage_richlabel();
	do
	{
		CC_BREAK_IF(NULL == pImage);
		
		bRet = pImage->initWithRichStringShadowStroke(text,
												  (int)textDefinition->_dimensions.width,
												  (int)textDefinition->_dimensions.height,
												  eAlign,
												  textDefinition->_fontName.c_str(),
												  textDefinition->_fontSize,
												  textDefinition->_fontFillColor.r / 255.0f,
												  textDefinition->_fontFillColor.g / 255.0f,
												  textDefinition->_fontFillColor.b / 255.0f,
												  shadowEnabled,
												  shadowDX,
												  shadowDY,
                                                  shadowColor,
												  shadowBlur,
												  strokeEnabled,
												  strokeColorR,
												  strokeColorG,
												  strokeColorB,
												  strokeSize);
		
		
		CC_BREAK_IF(!bRet);
		bRet = initWithImage(pImage);
		
		// save info needed by rich label
		m_shadowStrokePadding = pImage->getShadowStrokePadding();
		m_linkMetas = pImage->getLinkMetas();
	} while (0);
	
	CC_SAFE_RELEASE(pImage);
	
	return bRet;
    
    
#else
    
	CCAssert(false, "Currently only supported on iOS and Android!");
	return false;
    
#endif
}

NS_CC_END
