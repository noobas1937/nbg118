//
//  CCWebViewImpl.h
//  aoe
//
//  Created by 谢文杰 on 13-3-1.
//
//

#ifdef __OBJC__
#import <UIKit/UIKit.h>

@interface CCWebViewImplIOS : NSObject<UIWebViewDelegate,UIAlertViewDelegate>{
    UIWebView *mWebView_;
}

@property(nonatomic, retain)UIWebView* mWebView;

-(id) initWithFrame: (CGRect) frameRect;

@end
#endif

#include "cocos2d.h"

class CCWebViewImpl {
public:
    CCWebViewImpl(const cocos2d::CCPoint& pos,const cocos2d::CCSize& size);
    ~CCWebViewImpl();
    
    virtual void setVisible(bool bVisible);
    
    void loadUrl(const char *urlString);
    void loadFile(const char *filePath);
    
#ifdef __OBJC__
public:
    bool initWithFrame(const CGRect &frameRect);
private:
    CCWebViewImplIOS * m_impl;
    CGPoint m_screenPos;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void createWebView(int x,int y,int width,int height);
    void showWebView(const char* url);
    void removeWebView();
#endif
};
