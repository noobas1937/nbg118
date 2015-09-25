//
//  RollingText.h
//  IF
//
//  Created by 李锐奇 on 13-12-23.
//
//

#ifndef __IF__RollingText__
#define __IF__RollingText__

#include "CommonInclude.h"
#include "CCClipNode.h"

class RollingText : public CCNode{
public:
    static RollingText* create(CCArray* arr, int w,const ccColor3B& color={255,255,255});
private:
    RollingText(CCArray* arr, int w,const ccColor3B& color) : m_arr(arr), m_width(w), m_currentIndex(0), m_scroll(false),m_colText(color){
        
    }
   
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    void enterFrame(float t);
    void setFontColor(const ccColor3B& color);
    
    CCClipNode *m_clipNode;

    CCSafeObject<CCArray> m_arr;
    int m_width;
    int m_currentIndex;
    bool m_scroll;
    ccColor3B m_colText;
};


class RollRender : public CCNode{
public:
    static RollRender* create(std::string str,const ccColor3B& color);
    int getW();
    
private:
    RollRender(std::string str,const ccColor3B& color) : m_str(str),m_colText(color){
        
    }
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    std::string m_str;
    ccColor3B m_colText;
    CCSafeObject<CCLabelIF> m_text;
};

#endif /* defined(__IF__RollingText__) */
