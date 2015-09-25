//
//  FlyRollingText.h
//  IF
//
//  Created by liweiyu on 14-9-16.
//
//

#ifndef __IF__FlyRollingText__
#define __IF__FlyRollingText__

#include "CommonInclude.h"
#include "CCClipNode.h"

class FlyRollingText : public CCNode
{
public:
    //参数: 文字内容，滚动宽度，字体大小，字体颜色，滚动速度(px/s),重复次数(0表示一直重复).滚动结束后将自动删除.
    static FlyRollingText *create(string text, float width, float fontSize, ccColor3B color, float speed, int repeat,string icon="");
    //返回0表示一直重复.
    float getRollingTime();
    void rollText();
private:
    FlyRollingText(string text, float width, float fontSize, ccColor3B color, float speed, int repeat,string icon) :
    m_text(text)
    ,m_width(width)
    ,m_fontSize(fontSize)
    ,m_color(color)
    ,m_speed(speed)
    ,m_repeat(repeat)
    ,m_repeatCnt(0)
    ,m_icon(icon)
    {};
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    void stopAction();

    string m_text;
    float m_width;
    float m_fontSize;
    ccColor3B m_color;
    float m_speed;
    int m_repeat;
    int m_repeatCnt;
    string m_icon;
    CCSize m_textContentSize;
    CCSafeObject<CCLabelIFTTF> m_label;
    CCSafeObject<CCNode> m_contentNode;
    CCSafeObject<CCClipNode> m_clipNode;
};

#endif /* defined(__IF__FlyRollingText__) */
