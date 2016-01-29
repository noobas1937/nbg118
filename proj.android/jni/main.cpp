#include "AppDelegate.h"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
//#include "platform/android/CCFileUtilsAndroid.h"
#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
//#include "android/asset_manager.h"
//#include "android/asset_manager_jni.h"
//#include <jni/JniHelper.h>

// tao.yu TalkingData
#include "TDGAJniHelper.h"

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

//extern "C"
//{
//
//jint JNI_OnLoad(JavaVM *vm, void *reserved)
//{
//    JniHelper::setJavaVM(vm);
//
//    return JNI_VERSION_1_4;
//}
//
//
//void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
//{
//
//    JniMethodInfo methodInfo;
//    if (! JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/lib/Cocos2dxHelper","getAssetManager", "()Landroid/content/res/AssetManager;"))
//    {
//        methodInfo.env->DeleteLocalRef(methodInfo.classID);
//        return;
//    }
//    jobject assetManager = methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
//    methodInfo.env->DeleteLocalRef(methodInfo.classID);
//    AAssetManager* mgr = AAssetManager_fromJava(methodInfo.env, assetManager);
//    CCFileUtilsAndroid::setassetmanager(mgr);
//
//    if (!CCDirector::sharedDirector()->getOpenGLView())
//    {
//        CCEGLView *view = CCEGLView::sharedOpenGLView();
//        view->setFrameSize(w, h);
//
//        AppDelegate *pAppDelegate = new AppDelegate();
//        CCApplication::sharedApplication()->run();
//    }
//    else
//    {
//        ccGLInvalidateStateCache();
//        CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
//        CCTextureCache::reloadAllTextures();
//        ccDrawInit();
//        //CCTextureCache::reloadAllTextures();
//        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_FOREGROUND, NULL);
//        CCDirector::sharedDirector()->setGLDefaultValues();
//    }
//}
//
//
//}
void cocos_android_app_init (JNIEnv* env) {
    LOGD("cocos_android_app_init");
    AppDelegate *pAppDelegate = new AppDelegate();
    TDGAJniHelper::setJavaVM(JniHelper::getJavaVM());
}


