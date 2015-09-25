//
//  CCLabelIF.h
//  IF
//
//  Created by 邹 程 on 13-9-17.
//
//

#ifndef __IF__CCLabelIF__
#define __IF__CCLabelIF__

#include "cocos2d.h"
//#include "../../../extensions/CCBReader/CCNodeLoader.h"

#include "CCLabelIFBMFont.h"
#include "CCLabelIFTTF.h"
#include "CCRichLabel/CCRichLabelTTF.h"
#include "cocosextV3.h"
NS_CC_BEGIN

class CCLabelIF :public CCNode, public CCLabelProtocol
{
public:
    
    bool init();
    
    static CCLabelIF * create();
    static CCLabelIF * create(const char *str);
    static CCLabelIF * create(const char *str, float fontSize);
    static CCLabelIF * create(const char *str, const char *fntFile);
    static CCLabelIF * create(const char *str, const char *fontName, float fontSize);
    static CCLabelIF * create(const char *fontName, float fontSize, CCTextAlignment alignment);
    static CCLabelIF * create(const char *str, const char *fntFile, float width, CCTextAlignment alignment);
    static CCLabelIF * create(const char *str, const char *fntFile, float width, CCTextAlignment alignment, CCPoint imageOffset);
    
    // backward interface
    static CCLabelIF* create(const char *string, float fontSize,const CCSize &dimensions, CCTextAlignment hAlignment,CCVerticalTextAlignment vAlignment);
    void enableStroke(const ccColor3B &strokeColor, float strokeSize, bool mustUpdateTexture = true);
    const char* getFontName();
    float getFontSize();
    void setScale(float scale);
    void setScaleX(float scaleX);
    void setScaleY(float scaleY);
    float getScale();
    float getScaleX();
    float getScaleY();
    float getOriginScaleX();
    float getOriginScaleY();
    const CCSize& getContentSize();
    CCRect boundingBox(void);
    virtual const std::string& getString(void) const;
    CCTexture2D* getTexture(void);
    GLubyte getOpacity(void);
    
    const ccColor3B& getColor(void);
    const ccColor3B& getDisplayedColor(void);
    GLubyte getDisplayedOpacity(void);
    void setOpacityModifyRGB(bool bValue);
    bool isOpacityModifyRGB(void);
    bool isCascadeColorEnabled(void);
    void setCascadeColorEnabled(bool cascadeColorEnabled);
    void updateDisplayedColor(const ccColor3B& color);
    bool isCascadeOpacityEnabled(void);
    void setCascadeOpacityEnabled(bool cascadeOpacityEnabled);
    void updateDisplayedOpacity(GLubyte opacity);
   

    void setFontSize(float fontSize);
    void setVerticalAlignment(CCVerticalTextAlignment align);
    void setHorizontalAlignment(CCTextAlignment align);
    void setDimensions(const CCSize &dim);
    void setAlignment(CCTextAlignment align);
    virtual void setString(const std::string& str);
    void setString(const char *label);
    void setString(const char *format,bool isMultiParams,...);
    void setColor(const ccColor3B& color);
    void setFntFile(const char* fntFile);
    void setOpacity(GLubyte opacity);
    
    virtual void setAnchorPoint(const CCPoint& anchorPoint);
    
    static void enableBmFont(bool enabled);
    static void enableRichFont(bool enabled);
    static void initFont();
    static bool canBeSupportedWithBmpFont(std::string language);
    static bool canUseRich(std::string language);
    CCNode* getFontControl();
    CCNode* getParent();
    
    void setMaxScaleXByWidth(float width);
    
    std::string getIBMDescription(void) const;
private:
    bool initWithString(const char *str, const char *fntFile, float width = kCCLabelAutomaticWidth, CCTextAlignment alignment = kCCTextAlignmentLeft, CCPoint imageOffset = CCPointZero);
    
//    float m_fontSize;
//    std::string m_fontName;
//    float m_fontScaleX;
//    float m_fontScaleY;
    
    CCLabelIFBMFont* m_bmFont;
    CCLabelIFTTF* m_ttfFont;
    CCRichLabelTTF *m_richFont;
    
    std::string _originalUTF8String;
    
    static bool m_useBMFont;
    static bool m_useRichFont;
};

NS_CC_END

NS_CC_EXT_BEGIN

/* Forward declaration. */
//class CCBReader;

class CCLabelIFLoader : public CCNodeLoader {
public:
    virtual ~CCLabelIFLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCLabelIFLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCLabelIF);
    
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

#endif /* defined(__IF__CCLabelIF__) */
