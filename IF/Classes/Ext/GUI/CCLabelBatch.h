//
//  CCLabelBatch.h
//  IF
//
//  Created by 邹 程 on 14-4-9.
//
//

#ifndef __IF__CCLabelBatch__
#define __IF__CCLabelBatch__

#include "cocos2d.h"

NS_CC_BEGIN

class CCLabelBatchNode : public CCNode
{
public:    
    CCLabelBatchNode(){}
    
    virtual ~CCLabelBatchNode(){m_sFntFile="";}

    static CCLabelBatchNode * create() {
        //        return create(str, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, node);
        auto ret = new CCLabelBatchNode();
        if(ret->init()){
            ret->autorelease();
        }
        return ret;
    }

    static CCLabelBatchNode * create(const char* str) {
        auto ret = new CCLabelBatchNode();
        if(ret->init()){
            ret->setFntFile(str);
            ret->autorelease();
        }
        return ret;
    }
    void setFntFile(std::string str){m_sFntFile=str;}
    std::string getFntFile(){return m_sFntFile;}
protected:
    std::string m_sFntFile;
};

class CCLabelBatch : public CCLabelBMFont
{
    CCLabelBatch(){}
    
    virtual ~CCLabelBatch(){}
public:
    static CCLabelBatch * create(const char *str,CCLabelBatchNode* node) {
        //        return create(str, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, node);
                auto ret = new CCLabelBatch();
                ret->setFntFile(node->getFntFile());
                ret->autorelease();
                ret->setString(str);
                node->addChild(ret);
                return ret;
            }
};

//class CCLabelBatchNode : public CCSpriteBatchNode
//{
//public:
//    CCLabelBatchNode();
//    
//    virtual ~CCLabelBatchNode();
//    
//    static void purgeCachedData();
//    
//    static CCLabelBatchNode * create(const char *fntFile) {
//        auto ret = new CCLabelBatchNode();
//        if (ret && ret->init(fntFile)) {
//            ret->autorelease();
//        } else {
//            CC_SAFE_DELETE(ret);
//        }
//        return ret;
//    }
//    
//    bool init(const char* fntFile);
//    
//    CCBMFontConfiguration* getConf() {
//        return m_pConfiguration;
//    }
//    
//    CCSprite* getReusedChar() {
//        return m_pReusedChar;
//    }
//    
//protected:
//    
//    // name of fntFile
//    std::string m_sFntFile;
//    
//    CCBMFontConfiguration *m_pConfiguration;
//    
//    // reused char
//    CCSprite *m_pReusedChar;
//};


//class CCLabelBatch : public CCSprite
//{
//public:
//    CCLabelBatch();
//    
//    virtual ~CCLabelBatch();
//    
//    static CCLabelBatch * create(const char *str,float width, CCTextAlignment alignment,CCLabelBatchNode* node) {
//        auto ret = new CCLabelBatch();
//        if (ret && ret->initWithBatchNode(str,node,width,alignment)) {
//            ret->autorelease();
//        } else {
//            CC_SAFE_DELETE(ret);
//        }
//        return ret;
//    }
//    
//    static CCLabelBatch * create(const char *str,float width, CCLabelBatchNode* node) {
//        return create(str, width, kCCTextAlignmentLeft, node);
//    }
//    
//    static CCLabelBatch * create(const char *str,CCLabelBatchNode* node) {
//        return create(str, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, node);
//    }
//    
//    static CCLabelBatch * create(const char *str,float width, CCTextAlignment alignment,CCLabelBatch* node) {
//        auto ret = new CCLabelBatch();
//        if (ret && ret->initWithBatch(str,node,width,alignment)) {
//            ret->autorelease();
//        } else {
//            CC_SAFE_DELETE(ret);
//        }
//        return ret;
//    }
//    
//    static CCLabelBatch * create(const char *str,float width, CCLabelBatch* node) {
//        return create(str, width, kCCTextAlignmentLeft, node);
//    }
//    
//    static CCLabelBatch * create(const char *str,CCLabelBatch* node) {
//        return create(str, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, node);
//    }
//    
//    bool initWithBatch(const char *str, CCLabelBatch* node , float width, CCTextAlignment alignment);
//    bool initWithBatchNode(const char *str, CCLabelBatchNode* node , float width, CCTextAlignment alignment);
//    
//    /** updates the font chars based on the string to render */
//    void createFontChars();
//    // super method
//    virtual void setString(const char *newString);
//    virtual void setString(const char *newString, bool needUpdateLabel);
//    
//    virtual const char* getString(void);
//    virtual void setAnchorPoint(const CCPoint& var);
//    virtual void updateLabel();
//    virtual void setAlignment(CCTextAlignment alignment);
//    virtual void setWidth(float width);
//    virtual void setLineBreakWithoutSpace(bool breakWithoutSpace);
//    virtual void setScale(float scale);
//    virtual void setScaleX(float scaleX);
//    virtual void setScaleY(float scaleY);
//    
//    // CCRGBAProtocol
//    virtual bool isOpacityModifyRGB();
//    virtual void setOpacityModifyRGB(bool isOpacityModifyRGB); virtual GLubyte getOpacity();
//    virtual GLubyte getDisplayedOpacity();
//    virtual void setOpacity(GLubyte opacity);
//    virtual void updateDisplayedOpacity(GLubyte parentOpacity);
//    virtual bool isCascadeOpacityEnabled();
//    virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled);
//    virtual const ccColor3B& getColor(void);
//    virtual const ccColor3B& getDisplayedColor();
//    virtual void setColor(const ccColor3B& color);
//    virtual void updateDisplayedColor(const ccColor3B& parentColor);
//    virtual bool isCascadeColorEnabled();
//    virtual void setCascadeColorEnabled(bool cascadeColorEnabled);
//    
//private:
//    int kerningAmountForFirst(unsigned short first, unsigned short second);
//    float getLetterPosXLeft( CCSprite* characterSprite );
//    float getLetterPosXRight( CCSprite* characterSprite );
//    
//protected:
//    virtual void setString(unsigned short *newString, bool needUpdateLabel);
//    // string to render
//    unsigned short* m_sString;
//    
//    // initial string without line breaks
//    unsigned short* m_sInitialString;
//    std::string m_sInitialStringUTF8;
//    
//    // alignment of all lines
//    CCTextAlignment m_pAlignment;
//    // max width until a line break is added
//    float m_fWidth;
//    
//    CCBMFontConfiguration *m_pConfiguration;
//    
//    // reused char
//    CCSprite *m_pReusedChar;
//    
//    CCLabelBatchNode *m_batchNode;
//
//    bool m_bLineBreakWithoutSpaces;
//    
//    // texture RGBA
//    GLubyte m_cDisplayedOpacity;
//    GLubyte m_cRealOpacity;
//    ccColor3B m_tDisplayedColor;
//    ccColor3B m_tRealColor;
//    bool m_bCascadeColorEnabled;
//    bool m_bCascadeOpacityEnabled;
//    /** conforms to CCRGBAProtocol protocol */
//    bool m_bIsOpacityModifyRGB;
//    
//};

NS_CC_END


#endif /* defined(__IF__CCLabelBatch__) */
