#include <string>
#include "CCDevice.h"
#include "CCTypesExt.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include <spine/Json.h>
#endif  // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

using namespace std;

NS_CC_EXT_BEGIN

void CCDevice::rateApp()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "com/elextech/ccgoe/RateUtil", "rate", "()V") ) {
        CCLOG("jni: no method");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif  // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
}

string CCDevice::getDeviceModel()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxHelper", "getDeviceModel", "()Ljava/lang/String;")) {
        CCLOGFUNC("jni: no method");
        return "unknown";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    string ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(minfo.classID);
    minfo.env->DeleteLocalRef(retFromJava);
    return ret;
#else
	return "unknown";
#endif
}

string CCDevice::getLanguage()
{
    static string ret;
    if (!ret.empty())
        return ret;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Device", "getLanguage", "()Ljava/lang/String;")) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    return ret;
#else
	return "zh";
#endif
}

string CCDevice::getChannel()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Device", "getChannel", "()Ljava/lang/String;")) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    string ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    return ret;
#else
	return "unimplemented";
#endif
}

string CCDevice::getDeviceUid()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Device", "getUid", "()Ljava/lang/String;")) {
        CCLOGFUNC("CCDevice::getDeviceUid -> jni: no method");
        return "";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    string ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    CCLOG("CCDevice::getDeviceUid -> %s", ret.c_str());
    return ret;
#else
	return "unimplemented";
#endif
}

string CCDevice::base64Encode(unsigned char* bytes_to_encode, unsigned int in_len){
    return "";
}

string CCDevice::getOSVersion()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxHelper", "getOSVersion", "()Ljava/lang/String;")) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    string _ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(minfo.classID);
    minfo.env->DeleteLocalRef(retFromJava);
    // add "Android" prefix
    string ret = "Android";
    ret.append(_ret);
    return ret;
#else
	return "unknown";
#endif
}

void CCDevice::sendMail(const string &address,const string &title,const string &content)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "cocos2dx/ext/CppCallJava", "sendMail", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    jstring jaddress=minfo.env->NewStringUTF(address.c_str());
    jstring jtitle=minfo.env->NewStringUTF(title.c_str());
    jstring jcontent=minfo.env->NewStringUTF(content.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jaddress,jtitle,jcontent);
    minfo.env->DeleteLocalRef(minfo.classID);
    minfo.env->DeleteLocalRef(jaddress);
    minfo.env->DeleteLocalRef(jtitle);
    minfo.env->DeleteLocalRef(jcontent);
#else
#endif
}

string CCDevice::getVersionName()
{
    static string ret;
    if (!ret.empty())
        return ret;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Device", "getVersionName", "()Ljava/lang/String;")) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    return ret;
#else
	return "unimplemented";
#endif
}

string CCDevice::getDLCVersionName()
{
    static string ret;
    if (!ret.empty())
        return ret;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Device", "getDLCVersionName", "()Ljava/lang/String;")) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    return ret;
#else
    return "unimplemented";
#endif
}

string CCDevice::getHandSetInfo()
{
    static string ret;
    if (!ret.empty())
        return ret;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Device", "getHandSetInfo", "()Ljava/lang/String;")) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    return ret;
#else
	return "unimplemented";
#endif
}

string CCDevice::getVersionCode()
{
    static string ret;
    if (!ret.empty())
        return ret;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Device", "getVersionCode", "()Ljava/lang/String;")) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    ret=JniHelper::jstring2string(retFromJava);
    return ret;
#else
	return "unimplemented";
#endif
}

void CCDevice::openURL(const string &url)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Device", "showWebView", "(Ljava/lang/String;)V") ) {
        CCLOGFUNC("jni: no method");
        return;
    }
    
    jstring jevent=minfo.env->NewStringUTF(url.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jevent);
    minfo.env->DeleteLocalRef(jevent);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

string CCDevice::clipboardGetText()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Device", "clipboardGetText", "()Ljava/lang/String;") ) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    string ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    return ret;
#else
    return "";
#endif
}

void CCDevice::clipboardSetText(const string &text)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Device", "clipboardSetText", "(Ljava/lang/String;)V") ) {
        CCLOGFUNC("jni: no method");
        return;
    }
    
    jstring jevent=minfo.env->NewStringUTF(text.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jevent);
    minfo.env->DeleteLocalRef(jevent);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void CCDevice::updateVersion(const std::string &msg)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //1. 获取activity静态对象
    JniMethodInfo minfo;
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
    jobject activityObj;
    if(JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/ext/CppCallJava","gotoMarket","(Ljava/lang/String;)V"))
    {
        jstring jurl = minfo.env->NewStringUTF(msg.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jurl);
        minfo.env->DeleteLocalRef(jurl);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
#endif
}

string CCDevice::getAccountInfo()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Device", "getAccountInfo", "()Ljava/lang/String;") ) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    string ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    return ret;
#else
    return "";
#endif
}

string CCDevice::getGaid()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/CppCallJava", "getGaid", "()Ljava/lang/String;") ) {
        CCLOGFUNC("jni: no method getGaid");
        return "";
    }
    
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    string ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    return ret;
#endif
}

void CCDevice::pushNotice(int type,int time, std::string body,std::string soundKey,std::string pushType)
{
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Jni", "pushNotification", "(IILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V") ) {
        CCLOGFUNC("jni: no method");
        return;
    }
    
    jstring jbody=minfo.env->NewStringUTF(body.c_str());
    jstring jsoundKey=minfo.env->NewStringUTF(soundKey.c_str());
    jstring jpushType=minfo.env->NewStringUTF(pushType.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, type, time, jbody, jsoundKey, jpushType);
    minfo.env->DeleteLocalRef(jbody);
    minfo.env->DeleteLocalRef(jsoundKey);
    minfo.env->DeleteLocalRef(jpushType);
    minfo.env->DeleteLocalRef(minfo.classID);
}

void CCDevice::cancelNotice(int type)
{
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Jni", "cancelNotification", "(I)V") ) {
        CCLOGFUNC("jni: no method");
        return;
    }
    
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, type);
    minfo.env->DeleteLocalRef(minfo.classID);
}

string CCDevice::getPushRecordData(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Jni", "getPushRecordData", "()Ljava/lang/String;") ) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    string ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    return ret;
#endif
}


string CCDevice::getPushClickData(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Jni", "getPushClickData", "()Ljava/lang/String;") ) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    string ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    return ret;
#endif
}

void CCDevice::clearPushCache(){
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Jni", "clearPushCache", "()V") ) {
        CCLOGFUNC("jni: no method");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
}

void CCDevice::finishGame(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Jni", "finishGame", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
#endif
}


void CCDevice::setTestinUser(std::string userInfo)
{
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Jni", "setTestinUser", "(Ljava/lang/String;)V") ) {
        CCLOGFUNC("jni: no method");
        return;
    }
    
    jstring juserInfo=minfo.env->NewStringUTF(userInfo.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, juserInfo);
    minfo.env->DeleteLocalRef(minfo.classID);
}

void CCDevice::doPlatformLoading(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Jni", "doPlatformLoading", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void CCDevice::doPlatformLogin(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/clash/of/jni/Jni", "doPlatformLogin", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

bool CCDevice::needPlatformQuit(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/clash/of/jni/Jni", "needPlatformQuit", "()Z")) {
        CCLOGFUNC("jni: no method");
        return false;
    }
    jboolean isNeeded =  minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    return isNeeded;
#endif
}

void CCDevice::doPlatformQuit(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/clash/of/jni/Jni", "doPlatformQuit", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void CCDevice::doPlatformCollectUserInfo(const string &userId,const string &userName,const string &userLevel){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // return;
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Jni", "doSetHelpShiftUserInfo", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    jstring juserId=minfo.env->NewStringUTF(userId.c_str());
    jstring juserName=minfo.env->NewStringUTF(userName.c_str());
    jstring juserLevel=minfo.env->NewStringUTF(userLevel.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,juserId,juserName,juserLevel);
    minfo.env->DeleteLocalRef(minfo.classID);
    minfo.env->DeleteLocalRef(juserId);
    minfo.env->DeleteLocalRef(juserName);
    minfo.env->DeleteLocalRef(juserLevel);
#else
#endif
}

void CCDevice::GooglePlusLogin(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/clash/of/util/MyGooglePlusUtil", "GooglePlusLogin", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void CCDevice::GooglePlusLogout(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/clash/of/util/MyGooglePlusUtil", "GooglePlusLogout", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

bool CCDevice::GooglePlusIsLogin(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/clash/of/util/MyGooglePlusUtil", "GooglePlusIsLogin", "()Z")) {
        CCLOGFUNC("jni: no method");
        return false;
    }
	jboolean isLogin =  minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
    if(isLogin){
        CCLOG("google login sucess!");
        return true;
    }
    CCLOG("google login fail!");
    return false;
#endif
}

string CCDevice::getPublishRegion()
{
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/CppCallJava", "getPublishRegion", "()Ljava/lang/String;")) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    string ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    
//    CCLOG("<<<<<<<<<<< publish Region = [%s] >>>>>>>>>>>", ret.c_str());
    
    return ret;
    
}

void CCDevice::login_Platform(int platform)
{
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/CppCallJava", "login_Platform", "(I)V") ) {
        CCLOGFUNC("jni: no method");
        return;
    }
    
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, platform);
    minfo.env->DeleteLocalRef(minfo.classID);
}
void CCDevice::logout_Platform(int platform)
{
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/CppCallJava", "logout_Platform", "(I)V") ) {
        CCLOGFUNC("jni: no method");
        return;
    }
    
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, platform);
    minfo.env->DeleteLocalRef(minfo.classID);
}

bool CCDevice::isAvailable_Platform(int platform)
{
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/CppCallJava", "isAvailable_Platform", "(I)Z") ) {
        CCLOGFUNC("jni: no method");
        return false;
    }
    
    jboolean ret = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID,platform);
    minfo.env->DeleteLocalRef(minfo.classID);
    return ret;
    
}
bool CCDevice::isConnected_Platform(int platform)
{
    CCLOG("call isConnected_Platform:%d",platform);
    
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/CppCallJava", "isConnected_Platform", "(I)Z") ) {
        CCLOGFUNC("jni: no method");
        return false;
    }
    
    jboolean ret = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID,platform);
    minfo.env->DeleteLocalRef(minfo.classID);
    if (ret) {
        CCLOG("isConnected_Platform:true");
    }else{
        CCLOG("isConnected_Platform:false");
    }
    
    return ret;
    
}

cocos2d::CCDictionary* CCDevice::getLoginedInfo_Platform(int platform){
    CCLOG("getLoginedInfo_Platform :%d", platform);
    
    cocos2d::CCDictionary* dict = cocos2d::CCDictionary::create();
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/CppCallJava", "getLoginedInfo_Platform", "(I)Ljava/lang/String;")) {
        CCLOGFUNC("jni: no method");
        return dict;
    }
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID,platform);
    minfo.env->DeleteLocalRef(minfo.classID);
    string jsonStr=JniHelper::jstring2string(retFromJava);
    CCLOG("getLoginedInfo_Platform jsonStr :%s", jsonStr.c_str());
    minfo.env->DeleteLocalRef(retFromJava);
    
    Json *jsonObj = Json_create(jsonStr.c_str());
    string _is_connect=Json_getString(jsonObj, "is_connect", "");
    string _userName=Json_getString(jsonObj, "userName", "");
    string _userId=Json_getString(jsonObj, "userId", "");
    Json_dispose(jsonObj);
//    Json::Value jsonObj;
//    Json::Reader reader;
//    reader.parse(jsonStr, jsonObj);
    if (_userName != ""){//有数据
//        string  userName=jsonObj["userName"].asCString();
        dict->setObject(CCString::create(_userName),"userName");
    }
    if (_userId != ""){//有数据
//        string  userId=jsonObj["userId"].asCString();
        dict->setObject(CCString::create(_userId),"userId");
    }
    if (_is_connect != ""){//有数据
//        bool is_connect  =jsonObj["is_connect"].asBool();
        dict->setObject(CCString::create(_is_connect),"is_connect");
    }
    
    return dict;
    
}
bool CCDevice::hasEnoughSpace(int needSize)
{
    CCLOG("device need space:%d",needSize);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Device", "hasEnoughSpace", "(I)Z") ) {
        CCLOGFUNC("jni: no method");
        return false;
    }
    
    jboolean ret = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID,needSize);
    minfo.env->DeleteLocalRef(minfo.classID);
    return ret;
#endif
    return false;
}

void CCDevice::onUploadPhoto(int uid, int srcCode, int idx)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/CppCallJava", "onUploadPhoto", "(III)V") ) {
        CCLOGFUNC("jni: no method");
        return;
    }
    jint juid = (jint)uid;
    jint jsrcCode = (jint)srcCode;
    jint jidx = (jint)idx;
    
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, juid, jsrcCode,jidx);
    minfo.env->DeleteLocalRef(minfo.classID);
    
#endif
}

std::string CCDevice::getSDcardPath()
{
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/CppCallJava", "getSDcardPath", "()Ljava/lang/String;")) {
        CCLOGFUNC("jni: no method");
        return "";
    }
    
    jstring retFromJava = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    string ret=JniHelper::jstring2string(retFromJava);
    minfo.env->DeleteLocalRef(retFromJava);
    
//    CCLOG("<<<<<<<<<<< publish Region = [%s] >>>>>>>>>>>", ret.c_str());
    
    return ret;
}

NS_CC_EXT_END
