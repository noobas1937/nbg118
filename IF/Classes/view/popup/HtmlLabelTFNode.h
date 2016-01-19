//
//  HtmlLabelTFNode.h
//  IF
//
//  Created by lifangkai on 14-11-13.
//
//

#ifndef __IF__HtmlLabelTFNode__
#define __IF__HtmlLabelTFNode__
#include "CommonInclude.h"
#include "CCBExtension.h"

class HtmlLabelTFNode :public CCLayer
{
public:
    HtmlLabelTFNode(string str,int fontSize,float wd):m_str(str),mFontSize(fontSize),m_width(wd),m_url(""){};
    ~HtmlLabelTFNode(){
    };
    static HtmlLabelTFNode* create(string str,int fontSize,float wd);
    void setTextStr(string);
    CCSize getContentSize();
protected:
    bool init();
    void onEnter();
    void onExit();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    // virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSize getTextDimension(const char *text);

    void showText();
    void showHtmlLine();
    void addUnderLine(int index,float dx,float wd,float dh);
    CCSize m_size;
    float m_width;
    float m_height;
    string m_str;
    string m_url;
    int mFontSize;
    std::vector<std::string> m_VectorStr;
    std::vector<std::string> m_showVectorStr;
    CCSafeObject<CCNode>m_textNode;
};
class HtmlBGTouchLayer :public CCLayer
{
public:
    HtmlBGTouchLayer(CCSize size,string url):m_size(size),m_url(url){};
    ~HtmlBGTouchLayer(){
    };
    static HtmlBGTouchLayer* create(CCSize size,string url);
protected:
    bool init();
    void onEnter();
    void onExit();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    // virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    CCSize m_size;
    CCPoint m_startPoint;
    string m_url;
};
#endif /* defined(__IF__HtmlLabelTFNode__) */
