//
//  CCLabelIFTTF.h
//  IF
//
//  Created by chenliang on 14-3-5.
//
//

#ifndef __IF__CCLabelIFTTF__
#define __IF__CCLabelIFTTF__

#include "cocos2d.h"
#include "cocosextV3.h"
//#include "../../../extensions/CCBReader/CCNodeLoader.h"

#define  FONT_COMMON   ""

static const float _defaultBMFontSize = 30.0;

USING_NS_CC;

class CCLabelIFTTF : public Label
{
public:
    static CCLabelIFTTF * create();
    static CCLabelIFTTF * create(const char *string);
    static CCLabelIFTTF * create(const char *string, float fontSize);
    static CCLabelIFTTF * create(const char *string, const char *fontName, float fontSize);
    static CCLabelIFTTF * create(const char *string, const char *fontName, float fontSize,
                              const CCSize &dimensions, CCTextAlignment hAlignment,
                              CCVerticalTextAlignment vAlignment);
    static CCLabelIFTTF* create(const char *string, float fontSize,
                             const CCSize &dimensions, CCTextAlignment hAlignment,
                             CCVerticalTextAlignment vAlignment);
    
    
    //static CCLabelIF * create();
    //static CCLabelIF * create(const char *str);
    //static CCLabelIF * create(const char *str, float fontSize);
    static CCLabelIFTTF * create(const char *str, const char *fntFile);
    //static CCLabelIF * create(const char *str, const char *fontName, float fontSize);
    static CCLabelIFTTF * create(const char *fontName, float fontSize, CCTextAlignment alignment);
    static CCLabelIFTTF * create(const char *str, const char *fntFile, float width, CCTextAlignment alignment);
    static CCLabelIFTTF * create(const char *str, const char *fntFile, float width, CCTextAlignment alignment, CCPoint imageOffset);
    //
    //    // backward interface
    //static CCLabelIF* create(const char *string, float fontSize,const CCSize &dimensions, CCTextAlignment hAlignment,CCVerticalTextAlignment vAlignment);
    
    virtual bool init();
    virtual void setString(const char *label);
    virtual void setColor(const ccColor3B& color);
    virtual const char* getString(void);
    virtual CCTexture2D* getTexture(void);
    virtual void setOpacity(GLubyte opacity);
    virtual void setFntFile(const char* file){}
    virtual float getOriginScaleX();
    virtual float getOriginScaleY();
    virtual void enableStroke(const ccColor3B &strokeColor, float strokeSize, bool mustUpdateTexture = true);
    void setFontSize(float fontSize);
    
    virtual const char* getFontName();
    virtual float getFontSize();
//    virtual void setScale(float scale);
//    virtual void setScaleX(float scaleX);
//    virtual void setScaleY(float scaleY);
//    virtual float getScale();
//    virtual float getScaleX();
//    virtual float getScaleY();
//    virtual void setFontSize(float fontSize);
    virtual void setDimensions(const CCSize &dim);
    virtual void setVerticalAlignment(CCVerticalTextAlignment align);
    virtual void setHorizontalAlignment(CCTextAlignment align);
    virtual void setAlignment(CCTextAlignment alignment);
    
    virtual ~CCLabelIFTTF(){}
    
private:
    bool initWithString(const char *string, const char *fontName, float fontSize,
                        const CCSize& dimensions, CCTextAlignment hAlignment,
                        CCVerticalTextAlignment vAlignment);
    
    float m_fontSize;
    std::string m_fontName;
    float m_fontScaleX;
    float m_fontScaleY;

};

NS_CC_EXT_BEGIN

/* Forward declaration. */
//class CCBReader;

class CCLabelIFTTFLoader : public CCNodeLoader {
public:
    virtual ~CCLabelIFTTFLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCLabelIFTTFLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCLabelIFTTF);
    
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

#endif /* defined(__IF__CCLabelIFTTF__) */
