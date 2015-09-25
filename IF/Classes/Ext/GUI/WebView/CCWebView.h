//
//  CCWebView.h
//  aoe
//
//  Created by 谢文杰 on 13-3-1.
//
//

#ifndef __aoe__CCWebView__
#define __aoe__CCWebView__

#include "cocos-ext.h"
#include "cocosextV3.h"
class CCWebViewImpl;

NS_CC_EXT_BEGIN

class CCWebView : public CCNode{
public:
    CCWebView(const CCPoint & pos,const CCSize &size,CCNode* parent);
    ~CCWebView();

    virtual bool init();
    
    static CCWebView* create(const CCPoint & pos, const CCSize & size,CCNode* parent);
    
    virtual void setVisible(bool bVisible);

    void loadUrl(const std::string& url);
    void loadFile(const std::string& filename);
    
//    void webViewDidFinishLoad();
//    void onBackbuttonClick();
    
private:
//    int mWebViewLoadCounter;
    CCWebViewImpl * m_webviewImpl;
};

NS_CC_EXT_END

#endif /* defined(__aoe__CCWebView__) */
