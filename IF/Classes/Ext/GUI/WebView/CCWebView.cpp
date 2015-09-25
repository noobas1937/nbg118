//
//  CCWebView.cpp
//  aoe
//
//  Created by 谢文杰 on 13-3-4.
//
//

#include "CCWebView.h"
#include "CCWebViewImpl.h"

NS_CC_EXT_BEGIN

CCWebView::CCWebView(const CCPoint & pos,const CCSize &size,CCNode* parent)
{
    parent->addChild(this);
    CCNode::setPosition(pos);
    CCNode::setContentSize(size);
    m_webviewImpl = new CCWebViewImpl(convertToWorldSpace(ccp(0,size.height)),size);
}

CCWebView::~CCWebView(){
    CC_SAFE_DELETE(m_webviewImpl);
}

CCWebView * CCWebView::create(const CCPoint & pos,const CCSize &size,CCNode* parent)
{
    CCWebView *pRet = new CCWebView(pos,size,parent);
    if (pRet) {
        pRet->setContentSize(size);
        if (pRet->init())
            pRet->autorelease();
        else {
            delete pRet;
            pRet = NULL;
        }
    }
    return pRet;
}

/* override function */
void CCWebView::setVisible(bool bVisible)
{
    CCNode::setVisible(bVisible);
    m_webviewImpl->setVisible(bVisible);
}

void CCWebView::loadUrl(const std::string& url)
{
    m_webviewImpl->loadUrl(url.c_str());
}

void CCWebView::loadFile(const std::string& filename)
{
    m_webviewImpl->loadFile(filename.c_str());
}

#pragma mark -
#pragma mark Platform dependent
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
bool CCWebView::init()
{
    bool bRet=false;
    if (m_webviewImpl)
        bRet=true;
    return bRet;
}
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

NS_CC_EXT_END
