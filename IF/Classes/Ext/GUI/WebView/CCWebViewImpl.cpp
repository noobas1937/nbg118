//
//  CCWebViewImpl.mm
//  aoe
//
//  Created by 谢文杰 on 13-3-1.
//
//

#include "CCWebViewImpl.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;

CCWebViewImpl::CCWebViewImpl(const cocos2d::CCPoint& pos,const cocos2d::CCSize& size)
{
    GLView* eglView = cocos2d::Director::getInstance()->getOpenGLView();
    CCSize frameSize = eglView->getFrameSize();
    CCSize designSize = eglView->getDesignResolutionSize();
    
    // because of border , scaleX and scaleY should count manually
    float m_scaleX = frameSize.width/designSize.width;
    float m_scaleY = frameSize.height/designSize.height;
    int x = pos.x*m_scaleX + (frameSize.width-designSize.width*eglView->getScaleX())/4;
    int y = frameSize.height - pos.y*m_scaleY + (frameSize.height-designSize.height*eglView->getScaleY())/4;
    
//    int x = pos.x*eglView->getScaleX()+(frameSize.width-designSize.width*eglView->getScaleX())/2;
//    int y = frameSize.height - pos.y*eglView->getScaleY()+(frameSize.height-designSize.height*eglView->getScaleY())/2;
    int width = float(size.width)*eglView->getScaleX();
    int height = float(size.height)*eglView->getScaleY();
    createWebView(x,y,width,height);
}

CCWebViewImpl::~CCWebViewImpl()
{
    removeWebView();
}

void CCWebViewImpl::setVisible(bool bVisible)
{
}

void CCWebViewImpl::loadUrl(const char *urlString)
{
    showWebView(urlString);
}

void CCWebViewImpl::loadFile(const char *filePath)
{
    showWebView(CCString::createWithFormat("%s%s","file:///android_asset/",filePath)->getCString());
}

void CCWebViewImpl::createWebView(int x,int y,int width,int height)
{
    //1. 获取activity静态对象
    JniMethodInfo minfo;
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
    jobject activityObj;
    if(JniHelper::getStaticMethodInfo(minfo,"org/nbg/IF/IF","getInstance","()Lorg/nbg/IF/IF;"))
    {
        activityObj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    //2. 查找updateURL接口，并用jobj调用
    if(!JniHelper::getMethodInfo(minfo,"org/nbg/IF/IF","displayWebView", "(IIII)V"))
    {
        return;
    }
    else
    {
        //调用此函数
        minfo.env->CallVoidMethod(activityObj, minfo.methodID, x, y, width, height);
    }
}

void CCWebViewImpl::showWebView(const char* url)
{
    //1. 获取activity静态对象
    JniMethodInfo minfo;
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
    jobject activityObj;
    if(JniHelper::getStaticMethodInfo(minfo,"org/nbg/IF/IF","getInstance","()Lorg/nbg/IF/IF;"))
    {
        activityObj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    //2. 查找updateURL接口，并用jobj调用
    if(!JniHelper::getMethodInfo(minfo,"org/nbg/IF/IF","updateURL", "(Ljava/lang/String;)V"))
    {
        return;
    }
    else
    {
        //调用此函数
        jstring jmsg = minfo.env->NewStringUTF(url);
        minfo.env->CallVoidMethod(activityObj, minfo.methodID, jmsg);
    }
}

void CCWebViewImpl::removeWebView()
{
    //1. 获取activity静态对象
    JniMethodInfo minfo;
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
    jobject activityObj;
    if(JniHelper::getStaticMethodInfo(minfo,"org/nbg/IF/IF","getInstance","()Lorg/nbg/IF/IF;"))
    {
        activityObj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    //2. 查找updateURL接口，并用jobj调用
    if (!JniHelper::getMethodInfo(minfo,"org/nbg/IF/IF","removeWebView", "()V"))
    {
        return;
    }
    else
    {
        //调用此函数
        minfo.env->CallVoidMethod(activityObj, minfo.methodID);
    }
}