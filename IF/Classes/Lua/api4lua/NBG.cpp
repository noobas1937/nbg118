//
//  NBG.cpp
//  nbg
//
//  Created by gj on 9/7/15.
//
//

#include "NBG.h"
#include "NB_md5.h"

using namespace std;
USING_NS_CC;

// -------------------------------------------------------------------
#pragma mark -

int toMD5(char* dest, const char* src)
{
    md5_state_t state;
    md5_byte_t digest[16];
    md5_init(&state);
    md5_append(&state, (const md5_byte_t *)src, (int)strlen((char *)src));
    md5_finish(&state, digest);
    
    for (int di = 0; di < 16; ++di)
    {
        sprintf(dest + di * 2, "%02x", digest[di]);
    }
    return 0;
}

string NBG::md5(const char* in, string& out)
{
    char md5str[33] = {0,};
    toMD5(md5str, in);
    out = md5str;
    
    return out;
}

int NBG::get_COCOS2D_DEBUG() { return COCOS2D_DEBUG; }

long long NBG::getCurrentMillisecond()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    long long time = now.tv_sec;
    time = time * 1000 + now.tv_usec / 1000;
    
    return time;
}

long NBG::getCurrentSecond()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    
    return now.tv_sec;
}

// -------------------------------------------------------------------
#pragma mark - Android
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <jni.h>
#include "jni/JniHelper.h"

#pragma mark * native com.elex.tech.nbg.NBGNative

static string country_code = "";

#ifdef __cplusplus
extern "C" {
#endif

    void Java_com_elex_tech_nbg_NBGNative_nativeSetCountryCode(JNIEnv *env, jobject thisz, jstring country)
    {
        const char* k = env->GetStringUTFChars(country, 0);
        if (k) country_code = k;
        env->ReleaseStringUTFChars(country, k);
    }
    
    void Java_com_elex_tech_nbg_NBGNative_nativeSetAppInfo(JNIEnv *env, jobject thisz, jstring packageNames, jstring versionName, jstring versionCode)
    {
        const char* k = env->GetStringUTFChars(packageNames, 0);
        if (k)
        {
            UserDefault::getInstance()->setStringForKey("android_packageName", k);
        }
        env->ReleaseStringUTFChars(packageNames, k);
        
        k = env->GetStringUTFChars(versionName, 0);
        if (k)
        {
            UserDefault::getInstance()->setStringForKey("versionName", k);
        }
        env->ReleaseStringUTFChars(versionName, k);
        
        k = env->GetStringUTFChars(versionCode, 0);
        if (k)
        {
            UserDefault::getInstance()->setStringForKey("versionCode", k);
        }
        env->ReleaseStringUTFChars(versionCode, k);
    }
    
#ifdef __cplusplus
}
#endif


#pragma mark *

string NBG::getUDID()
{
    static string uid;
    if (uid.empty())
    {
        JniMethodInfo minfo;
        if (!JniHelper::getStaticMethodInfo(minfo, "com/elex/tech/nbg/Udid", "getUid", "()Ljava/lang/String;"))
        {
            return "";
        }
        jstring retFromJava = (jstring) minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
        const char* k = minfo.env->GetStringUTFChars(retFromJava, 0);
        if (k) uid = k;
        minfo.env->ReleaseStringUTFChars(retFromJava, k);
    }
    return uid;
}

string NBG::getOpenUDID(bool autosave)
{
    return "";
}

string NBG::getCountryCode()
{
    return country_code;
}

string NBG::getPublishRegion()
{
    return "";
}

string NBG::getPlatformUID()
{
    return "";
}

string NBG::getPlatformToken()
{
    return "";
}

string NBG::getHandSetInfo()
{
    static string ret;
    if (!ret.empty()) return ret;
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/elex/tech/nbg/NBGDevice", "getHandSetInfo", "()Ljava/lang/String;"))
    {
        return "";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    const char* k = minfo.env->GetStringUTFChars(retFromJava, 0);
    ret = k;
    minfo.env->ReleaseStringUTFChars(retFromJava, k);

    return ret;
}

string NBG::getVersionName()
{
    return UserDefault::getInstance()->getStringForKey("versionName", "");;
}

string NBG::getVersionCode()
{
    return UserDefault::getInstance()->getStringForKey("versionCode", "");
}

string NBG::getLanguage()
{
    static string ret;
    if (!ret.empty()) return ret;
    
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/elex/tech/nbg/NBGDevice", "getLanguage", "()Ljava/lang/String;"))
    {
        return "";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    ret = JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    return ret;
}

#endif
