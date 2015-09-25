//
//  CCWebView.mm
//  aoe
//
//  Created by 谢文杰 on 13-3-1.
//
//

#include "CCWebView.h"
#import "CCEAGLView-ios.h"
#import "CCWebViewImpl.h"

NS_CC_EXT_BEGIN

bool CCWebView::init()
{
    bool bRet=false;
    
//    CCEGLViewProtocol* eglView = CCEGLView::sharedOpenGLView();
//    CGRect rect = CGRectMake(0, 0, getContentSize().width * eglView->getScaleX(), getContentSize().height * eglView->getScaleY());
//    
//    if ([[EAGLView sharedEGLView] contentScaleFactor] == 2.0f)
//    {
//        rect.size.width /= 2.0f;
//        rect.size.height /= 2.0f;
//    }
//    m_webviewImpl = new CCWebViewImpl();
//    m_webviewImpl->initWithFrame(rect);
//    if (m_webviewImpl)
        bRet=true;
    return bRet;
}

NS_CC_EXT_END