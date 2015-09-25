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
#include "CCImage_richlabel.h"
#include <jni.h>
#include "../../platform/android/jni/JniHelper.h"
#include "CLBitmapDC.h"

using namespace std;
NS_CC_BEGIN

// swap the alpha channel in an 32 bit image (from ARGB to RGBA)
static void swapAlphaChannel(unsigned int *pImageMemory, unsigned int numPixels) {
	for(int c = 0; c < numPixels; ++c, ++pImageMemory) {
		// copy the current pixel
		unsigned int currenPixel = (*pImageMemory);

		// swap channels and store back
		char *pSource = (char *) &currenPixel;
		*pImageMemory = (pSource[0] << 24) | (pSource[3] << 16) | (pSource[2] << 8) | pSource[1];
	}
}

CCImage_richlabel::CCImage_richlabel() {
}

CCImage_richlabel::~CCImage_richlabel() {
}

bool CCImage_richlabel::initWithRichStringShadowStroke(const char * pText, int nWidth, int nHeight, Device::TextAlign eAlignMask, const char * pFontName, int nSize,
		float textTintR, float textTintG, float textTintB, bool shadow, float shadowOffsetX, float shadowOffsetY, int shadowColor, float shadowBlur,
		bool stroke, float strokeR, float strokeG, float strokeB, float strokeSize) {
	bool bRet = false;
	do {
		CC_BREAK_IF(!pText);

		CLBitmapDC &dc = CLBitmapDC::sharedCLBitmapDC();

		CC_BREAK_IF(!dc.getBitmapFromJavaShadowStroke(pText,
                                                      nWidth,
                                                      nHeight,
                                                      eAlignMask,
                                                      pFontName,
                                                      nSize,
                                                      textTintR,
                                                      textTintG,
                                                      textTintB,
                                                      shadow,
                                                      shadowOffsetX,
                                                      shadowOffsetY,
                                                      shadowColor,
                                                      shadowBlur,
                                                      stroke,
                                                      strokeR,
                                                      strokeG,
                                                      strokeB,
                                                      strokeSize,
                                                      false));

		// assign the dc.m_pData to m_pData in order to save time
		_data = dc.m_pData;

		CC_BREAK_IF(!_data);

		_width = (short) dc.m_nWidth;
		_height = (short) dc.m_nHeight;
		// m_bHasAlpha = true;
		_hasPremultipliedAlpha = true;
		// m_nBitsPerComponent = 8;
		// swap the alpha channel (ARGB to RGBA)
		swapAlphaChannel((unsigned int *)_data, (_width * _height));
        _dataLen =_height*_width*4;
        _renderFormat = Texture2D::PixelFormat::RGBA8888;
		// ok
		bRet = true;

		// save info needed by label
		m_linkMetas = dc.m_linkMetas;
		m_shadowStrokePadding = dc.m_shadowStrokePadding;
	} while(0);

	return bRet;
}

CCSize CCImage_richlabel::measureRichString(const char* pText,
                                            const char* pFontName,
                                            int nSize,
                                            int maxWidth,
                                            float shadowOffsetX,
                                            float shadowOffsetY,
                                            float strokeSize) {
    CCSize size = CCSizeZero;
	do {
		CC_BREAK_IF(!pText);
        
		CLBitmapDC &dc = CLBitmapDC::sharedCLBitmapDC();
        
		CC_BREAK_IF(!dc.getBitmapFromJavaShadowStroke(pText,
                                                      maxWidth,
                                                      0,
                                                      Device::TextAlign::CENTER,
                                                      pFontName,
                                                      nSize,
                                                      0,
                                                      0,
                                                      0,
                                                      shadowOffsetX != 0 || shadowOffsetY != 0,
                                                      shadowOffsetX,
                                                      shadowOffsetY,
                                                      0,
                                                      0,
                                                      strokeSize != 0,
                                                      0,
                                                      0,
                                                      0,
                                                      strokeSize,
                                                      true));
		size.width = dc.m_nWidth;
		size.height = dc.m_nHeight;
	} while(0);
    
	return size;
}

NS_CC_END
