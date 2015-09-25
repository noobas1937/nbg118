//
//  CCLabelIFBMFont.h
//  IF
//
//  Created by chenliang on 14-3-5.
//
//

#ifndef __IF__CCLabelIFBMFont__
#define __IF__CCLabelIFBMFont__

#include "cocos2d.h"
#include "cocosextV3.h"
//#include "../../../extensions/CCBReader/CCNodeLoader.h"

USING_NS_CC;
using namespace cocosbuilder;
class CCLabelIFBMFont : public CCLabelBMFont
{
public:
    static CCLabelIFBMFont * create();
    static CCLabelIFBMFont * create(const char *str);
    static CCLabelIFBMFont * create(const char *str, float fontSize);
    static CCLabelIFBMFont * create(const char *str, const char *fntFile);
    static CCLabelIFBMFont * create(const char *str, const char *fontName, float fontSize);
    static CCLabelIFBMFont * create(const char *fontName, float fontSize, CCTextAlignment alignment);
    static CCLabelIFBMFont * create(const char *str, const char *fntFile, float width, CCTextAlignment alignment);
    static CCLabelIFBMFont * create(const char *str, const char *fntFile, float width, CCTextAlignment alignment, CCPoint imageOffset);
    
    // backward interface
    static CCLabelIFBMFont* create(const char *string, float fontSize,const CCSize &dimensions, CCTextAlignment hAlignment,CCVerticalTextAlignment vAlignment);
    
    virtual bool init();
    virtual void enableStroke(const ccColor3B &strokeColor, float strokeSize, bool mustUpdateTexture = true);
    virtual const char* getFontName();
    virtual float getFontSize();
    virtual void setScale(float scale);
    virtual void setScaleX(float scaleX);
    virtual void setScaleY(float scaleY);
    virtual float getScale();
    virtual float getScaleX();
    virtual float getScaleY();
    virtual float getOriginScaleX();
    virtual float getOriginScaleY();
    virtual const char* getString();
    
    virtual void setFontSize(float fontSize);
    virtual void setVerticalAlignment(CCVerticalTextAlignment align);
    virtual void setHorizontalAlignment(CCTextAlignment align);
    virtual void setDimensions(const CCSize &dim);
    
    virtual void setFntFile(const char* fntFile);
    
    virtual void setString(const char *label)
    {
        CCLabelBMFont::setString(label);
    }
    virtual void setColor(const ccColor3B& color)
    {
        CCLabelBMFont::setColor(color);
    }
    //TODO::getTexture
//    virtual CCTexture2D* getTexture(void)
//    {
//        return CCLabelBMFont::getTexture();
//    }
    virtual void setOpacity(GLubyte opacity)
    {
        CCLabelBMFont::setOpacity(opacity);
    }
    virtual void setAlignment(CCTextAlignment alignment)
    {
        CCLabelBMFont::setAlignment(alignment);
    }
    
protected:
    bool initWithString(const char *str, const char *fntFile, float width = kCCLabelAutomaticWidth, CCTextAlignment alignment = kCCTextAlignmentLeft, CCPoint imageOffset = CCPointZero);
    
    float m_fontSize;
    std::string m_fontName;
    float m_fontScaleX;
    float m_fontScaleY;
};

NS_CC_EXT_BEGIN

/* Forward declaration. */
//class CCBReader;

class CCLabelIFBMFontLoader : public CCNodeLoader {
public:
    virtual ~CCLabelIFBMFontLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCLabelIFBMFontLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCLabelIFBMFont);
    
    virtual void onHandlePropTypeColor3(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccColor3B pCCColor3B, cocosbuilder::CCBReader * pCCBReader);
    virtual void onHandlePropTypeByte(CCNode * pNode, CCNode * pParent, const char * pPropertyName, unsigned char pByte, cocosbuilder::CCBReader * pCCBReader);
    virtual void onHandlePropTypeBlendFunc(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccBlendFunc pCCBlendFunc, cocosbuilder::CCBReader * pCCBReader);
    virtual void onHandlePropTypeFontTTF(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pFontTTF, cocosbuilder::CCBReader * pCCBReader);
    virtual void onHandlePropTypeText(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pText, cocosbuilder::CCBReader * pCCBReader);
    virtual void onHandlePropTypeFloatScale(CCNode * pNode, CCNode * pParent, const char * pPropertyName, float pFloatScale, cocosbuilder::CCBReader * pCCBReader);
    virtual void onHandlePropTypeIntegerLabeled(CCNode * pNode, CCNode * pParent, const char * pPropertyName, int pIntegerLabeled, cocosbuilder::CCBReader * pCCBReader);
    virtual void onHandlePropTypeSize(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSize pSize, cocosbuilder::CCBReader * pCCBReader);
};

NS_CC_EXT_END

#endif /* defined(__IF__CCLabelIFBMFont__) */
