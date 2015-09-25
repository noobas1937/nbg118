//
//  SNSUtil.cpp
//  IF
//
//  Created by 王智圆 on 15/7/29.
//
//

#include "SNSUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#   include <jni.h>
#   include "platform/android/jni/JniHelper.h"

void SNSUtilies::doLogin(std::string pf){
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "com/clash/of/jni/Jni", "loginPlatform", "(Ljava/lang/String;)V") ) {
        CCLOG("JNI: method loginPlatform not found!");
        return;
    }
    jstring jpf = minfo.env->NewStringUTF(pf.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jpf);
    minfo.env->DeleteLocalRef(jpf);
    minfo.env->DeleteLocalRef(minfo.classID);
}

#endif