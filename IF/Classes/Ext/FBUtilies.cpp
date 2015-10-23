
#include "FBUtilies.h"
#include <regex.h>
#include <curl/curl.h>
#include "GameController.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

// 取得FACEBOOK奖励的key
const char* FBUtilies::getFacebookRewardKey() {
    // FacebookLoginReward
    return CCString::createWithFormat("%s", "FacebookLoginReward")->getCString();
}

static size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t real_size = nmemb * size;
    memcpy(userp, contents, real_size);
    ((char*)userp)[real_size] = '\0';
    CCLog("userp=%s",userp);
    return real_size;
}

void FBUtilies::getFriendsData(){
//    string appId = "713457855362204";
//    string red = "https://www.facebook.com/connect/login_success.html";
//    char url2[1024] = {0};
//    sprintf(url2, "https://www.facebook.com/dialog/oauth?client_id=%s&redirect_uri=%s&response_type=token&scope=email,publish_actions",appId.c_str(),red.c_str());
//    cocos2d::extension::CCDevice::updateVersion(url2);
    //return;
    //https://graph.facebook.com/me?access_token=CAAKI4toH9JwBAHid1YZBD0CmUw0A2NHliAkQe9FXJsYQGWOA5gkEWDlbKzBHBsnOC5VjiVh4thrtYAesnpAAol5JuGMKZBuJ4F0k7qUTGje0Q25vZAZAoDxTfO9hJUcwKgj63UCkNIdocyQZBMZBAQZArF3whQntdz2Jid5Yz5zp9dy0Wp0TMCXLEogolFQPwm4fRS1u6GZAPRkNxj9ezx9r&fields=id,name,email,permissions,friends.limit(500).fields(devices,name,picture,installed),picture.type(small)
    string access_token = "CAAKI4toH9JwBAPgSSpyBNeK2NarhWbzymBm8BuP8TZA7rjJGQHQnKS9t4I4yXtGKlfZAx8ZB8UTGLZAx7swoLt4V2apweeVtW4Wbcg03glqnrrEVsDnrdb0hZBLOlQZB4EmRThJyq5lNMnDO9G48DwEHOgmGZA5wWl4mbw811zv53R9acqzPR2eZALqZC8vgDjX1GOOThbmf58erxS8QqZBeuE";
    
    if (access_token == "")
    {
        return;
    }
    char url[1024] = {0};
    //sprintf(url,"https://graph.facebook.com/me?access_token=%s&fields=id,name,email,permissions,friends.limit(500).fields(devices,name,picture,installed),picture.type(small)",access_token.c_str());
    sprintf(url,"https://graph.facebook.com/445208542288191/picture?type=small");
    //第一步：初始化CURL，取得初始化成功后的CURL指针。
    CURL *curl = curl_easy_init();
    string strHtml;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strHtml);
        //      curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);
        //      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        
        ///第三步，执行上面设定的动作处理。返回结果放在res中。
        //这里定义一个CURL库中API的返回值，用于取得API调用的结果。
        CURLcode res = curl_easy_perform(curl);
        
        //最后一步，清除CURL指针，结束对CURL库的使用。
        //curl_easy_cleanup(curl);
        
        if (res == CURLE_OK)
        {//成功
            
        }
    }
//    return;
//    string access_token = "dfdsfsf";
//
//    if (access_token == "")
//    {
//        return;
//    }
//    char url[1024] = {0};
//    sprintf(url,"https://graph.facebook.com/me?access_token=%s&fields=id,name,email,permissions,friends.limit(500).fields(devices,name,picture,installed),picture.type(small)",access_token.c_str());
//    //第一步：初始化CURL，取得初始化成功后的CURL指针。
//    CURL *curl = curl_easy_init();
//    string strHtml;
//    if (curl)
//    {
//        //第二步，设定我们用此CURL指针来完成的动作。
//        //参数一为CURL指针，
//        //参数二为相应的动作类型枚举，这个枚举值在curl.h中定义，比如本例中的CURLOPT_URL，定义为CINIT(URL,  OBJECTPOINT, 2)，即联接一个网站的HTTP服务。
//        //参数三为动作对应的数据参数，这里是网站的URL地址。
//        curl_easy_setopt(curl, CURLOPT_URL, url);
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//        //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpWriteString);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strHtml);
//        //      curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);
//        //      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
//        
//        ///第三步，执行上面设定的动作处理。返回结果放在res中。
//        //这里定义一个CURL库中API的返回值，用于取得API调用的结果。
//        CURLcode res = curl_easy_perform(curl);
//        
//        //最后一步，清除CURL指针，结束对CURL库的使用。
//        curl_easy_cleanup(curl);
//        
//        if (res == CURLE_OK)
//        {//成功
//            
//        }
//    }
}



//-----end-----

#pragma mark -
#pragma mark Platform dependent code for Android

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#   include <jni.h>
#   include "platform/android/jni/JniHelper.h"

bool FBUtilies::fbLogin(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "Login", "()Z")) {
        CCLOGFUNC("jni: no method");
        return false;
    }
    jboolean isLogin = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
    
    if(isLogin){
        CCLOG("facebook login");
        return true;
    }
    CCLOG("facebook login fail!");
    return false;
#endif
}

void FBUtilies::fbLogout()
{
    CCUserDefault::sharedUserDefault()->setStringForKey("inviteFriends", "");
    CCUserDefault::sharedUserDefault()->setStringForKey("installFriendsInfo", "");
    CCUserDefault::sharedUserDefault()->flush();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "callFacebookLogout", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

bool FBUtilies::fbIsLogin(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "isLogin", "()Z")) {
        CCLOGFUNC("jni: no method");
        return false;
    }
    jboolean isLogin = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
    
    if(isLogin){
        CCLOG("facebook login sucess!");
        return true;
    }
    CCLOG("facebook login fail!");
    return false;
#endif
}

void FBUtilies::getFBFriendList(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG("FBUtilies::getFBFriendList");
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "getFriendsList", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
        
#endif
}

void FBUtilies::getAppRequestList(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG("FBUtilies::getAppRequestList");
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "getAppRequestList", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    
#endif
}

void FBUtilies::fbShowFansWall(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "showFansWall", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::postFBSelectedFriendList(cocos2d::CCArray* arr,const std::string &msg,const std::string &ftype){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG("FBUtilies::postFBSelectedFriendList");
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "sendFriendRequest", "(Ljava/util/ArrayList;Ljava/lang/String;Ljava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    
    jclass cls_ArrayList = minfo.env->FindClass("java/util/ArrayList");
    jmethodID construct = minfo.env->GetMethodID(cls_ArrayList,"<init>","()V");
    jobject obj_ArrayList = minfo.env->NewObject(cls_ArrayList,construct,"");
    jmethodID arrayList_add = minfo.env->GetMethodID(cls_ArrayList,"add","(Ljava/lang/Object;)Z");
    //User Object
    jclass cls_friend = minfo.env->FindClass("org/nbg/stac/empire/sns/FBFriendPoj");
    //none argument construct function
    jmethodID construct_friend = minfo.env->GetMethodID(cls_friend,"<init>","()V");
    
    CCLOG("arr in");
    for(int i=0;i<arr->count();i++){
        //new a object
        jobject obj_friend = minfo.env->NewObject(cls_friend,construct_friend,"");
        CCLOG("facebook selected friends--------------1-----------------");
        jmethodID friend_setId = minfo.env->GetMethodID(cls_friend,"setUserid","(Ljava/lang/String;)V");
        CCLOG("facebook selected friends--------------2-----------------");
        CCString* str=dynamic_cast<CCString*>(arr->objectAtIndex(i));
        CCLOG("facebook selected friends--------------3-----------------");
        CCLOG(str->getCString());
        jstring id=minfo.env->NewStringUTF(str->getCString());
        CCLOG("facebook selected friends--------------3.5-----------------");
        minfo.env->CallVoidMethod(obj_friend,friend_setId,id);
        CCLOG("facebook selected friends--------------4-----------------");
        minfo.env->CallBooleanMethod(obj_ArrayList,arrayList_add,obj_friend);        
        CCLOG("facebook selected friends--------------5-----------------");
    }
    CCLOG("facebook selected friends--------------6-----------------");
    jstring jtype = minfo.env->NewStringUTF(ftype.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,obj_ArrayList,minfo.env->NewStringUTF(msg.c_str()),jtype);
    CCLOG("facebook selected friends--------------7-----------------");
	minfo.env->DeleteLocalRef(minfo.classID);
    CCLOG("facebook selected friends--------------8-----------------");
    
#endif
    
}


void FBUtilies::checkIsGameFans_FB()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "checkIsGameFans_FB", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
#endif
    
}

void FBUtilies::postFeed_FB(const std::string &msg)
{
    //1. 获取activity静态对象
    JniMethodInfo minfo;
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
    jobject activityObj;
    if(JniHelper::getStaticMethodInfo(minfo,"org/nbg/IF/IF","getSNSHelper","()Lcom/elextech/aoe/sns/SNSHelper;"))
    {
        activityObj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    //2. 查找updateURL接口，并用jobj调用
    if(!JniHelper::getMethodInfo(minfo,"org/nbg/IF/sns/SNSHelper","postFeed_facebook", "(Ljava/lang/String;)V"))
    {
        return;
    }
    else
    {
        //调用此函数
        jstring jmsg = minfo.env->NewStringUTF(msg.c_str());
        minfo.env->CallVoidMethod(activityObj, minfo.methodID, jmsg);
    }
}

void FBUtilies::postFeed_sina(const std::string &msg)
{
    //1. 获取activity静态对象
    JniMethodInfo minfo;
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
    jobject activityObj;
    if(JniHelper::getStaticMethodInfo(minfo,"org/nbg/IF/IF","getSNSHelper","()Lcom/elextech/aoe/sns/SNSHelper;"))
    {
        activityObj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    //2. 查找updateURL接口，并用jobj调用
    if(!JniHelper::getMethodInfo(minfo,"org/nbg/IF/sns/SNSHelper","postFeed_sina", "(Ljava/lang/String;)V"))
    {
        return;
    }
    else
    {
        //调用此函数
        jstring jmsg = minfo.env->NewStringUTF(msg.c_str());
        minfo.env->CallVoidMethod(activityObj, minfo.methodID, jmsg);
    }
}

void FBUtilies::postFeed_email(const std::string &msg, const std::string &subject)
{
    //1. 获取activity静态对象
    JniMethodInfo minfo;
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
    jobject activityObj;
    if(JniHelper::getStaticMethodInfo(minfo,"org/nbg/IF/IF","getSNSHelper","()Lcom/elextech/aoe/sns/SNSHelper;"))
    {
        activityObj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    //2. 查找updateURL接口，并用jobj调用
    if(!JniHelper::getMethodInfo(minfo,"org/nbg/IF/sns/SNSHelper","postFeed_email", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    else
    {
        //调用此函数
        jstring jmsg = minfo.env->NewStringUTF(msg.c_str());
        jstring jsubject = minfo.env->NewStringUTF(subject.c_str());
        minfo.env->CallVoidMethod(activityObj, minfo.methodID, jmsg, jsubject);
    }
}

void FBUtilies::postFeed_market()
{
    //1. 获取activity静态对象
    JniMethodInfo minfo;
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
    jobject activityObj;
    if(JniHelper::getStaticMethodInfo(minfo,"org/nbg/IF/IF","getSNSHelper","()Lcom/elextech/aoe/sns/SNSHelper;"))
    {
        activityObj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    //2. 查找updateURL接口，并用jobj调用
    if(!JniHelper::getMethodInfo(minfo,"org/nbg/IF/sns/SNSHelper","postFeed_playStore", "()V"))
    {
        return;
    }
    else
    {
        //调用此函数
        minfo.env->CallVoidMethod(activityObj, minfo.methodID);
    }
}

void FBUtilies::fbLike(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //1. 获取activity静态对象
    JniMethodInfo minfo;
    if(!JniHelper::getStaticMethodInfo(minfo,"org/nbg/stac/empire/sns/FBUtil","showLike", "()V"))
    {
        return;
    }
    else
    {
        //调用此函数
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
#endif
}

void FBUtilies::removeLike(){
    JniMethodInfo minfo;
    if(!JniHelper::getStaticMethodInfo(minfo,"org/nbg/stac/empire/sns/FBUtil","hideLike", "()V"))
    {
        return;
    }
    else
    {
        //调用此函数
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void FBUtilies::fbPublishFeedDialog(const std::string& name,const std::string& caption, const std::string& linkDescription,const std::string& link,const std::string& pictureUrl,int feedType,const std::string& ref)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //1. 获取activity静态对象
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "publishFeedDialog", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        return ;
    }
    
    //调用此函数
    jstring jCaption = minfo.env->NewStringUTF(caption.c_str());
    jstring jlink = minfo.env->NewStringUTF(link.c_str());
    jstring jlinkName = minfo.env->NewStringUTF(name.c_str());
    jstring jlinkDescription = minfo.env->NewStringUTF(linkDescription.c_str());
    jstring jpic = minfo.env->NewStringUTF(pictureUrl.c_str());
    jstring jref = minfo.env->NewStringUTF(ref.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jlinkName,jCaption,jlink,jlinkDescription,jpic,feedType,jref);
    minfo.env->DeleteLocalRef(minfo.classID);
    std::string data = "action=publish_feed";
    if (ref!="") {
        data.append(".");
        data.append(ref);
    }
    GameController::getInstance()->callXCApi(data);
#endif
}

void FBUtilies::fbSharePhotosByUrl(const std::string& url,const std::string& dialog)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //1. 获取activity静态对象
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "SharePhotos", "(Ljava/lang/String;Ljava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        return ;
    }
    //调用此函数
    jstring jUrl = minfo.env->NewStringUTF(url.c_str());
    jstring jDialog = minfo.env->NewStringUTF(dialog.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jUrl, jDialog);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::inviteFriends(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG("FBUtilies::inviteFriends");
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "inviteFriends", "()V")) {
        CCLOGFUNC("jni: no method inviteFriends");
        return;
    }
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::requestForMeInfo()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "requestForMeInfo", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::appInvite(const std::string &appLink){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "appInvite", "(Ljava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    jstring jAppLink = minfo.env->NewStringUTF(appLink.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jAppLink);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::appEventPurchaseItem(int type,int user_level, int castle_level,int item_id,const std::string& item_name,int item_price,int item_count,int spend,const std::string entrance){
    CCLog("FBUtilies::appEventPurchaseItem");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "appPurchaseItem", "(IIIILjava/lang/String;IIILjava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    jstring jname = minfo.env->NewStringUTF(item_name.c_str());
    jstring jentrance= minfo.env->NewStringUTF(entrance.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,type,user_level,castle_level,item_id,jname,item_price,item_count,spend,jentrance);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

bool FBUtilies::fbHasGranted(const std::string &permission){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "facebookHasGranted", "(Ljava/lang/String;)Z")) {
        CCLOGFUNC("jni: no method");
        return false;
    }
    
    jstring jpermission = minfo.env->NewStringUTF(permission.c_str());
    jboolean pflag = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID,jpermission);
    minfo.env->DeleteLocalRef(minfo.classID);
    
    if(pflag){
        CCLog(" fb has Granted %s",permission.c_str());
        return true;
    }
    CCLog("fb has not Granted %s",permission.c_str());
    return false;
#endif
}

void FBUtilies::fbPostAction(const std::string &action,const std::string &actionT,const std::string &actionObj){
    CCLog("FBUtilies::fbPostAction");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "facebookPostAction", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    jstring jaction = minfo.env->NewStringUTF(action.c_str());
    jstring jactionT= minfo.env->NewStringUTF(actionT.c_str());
     jstring jactionObj= minfo.env->NewStringUTF(actionObj.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jaction,jactionT,jactionObj);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::appEventShareLog(const std::string &realm,int user_level,int castle_level,const std::string &share_content){
    CCLog("FBUtilies::fb appEventShareLog");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "appEventShareLog", "(Ljava/lang/String;IILjava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    jstring jrealm = minfo.env->NewStringUTF(realm.c_str());
    jstring jshare_content= minfo.env->NewStringUTF(share_content.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jrealm,user_level,castle_level,jshare_content);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::appEventFBEntrance(const std::string &entrance){
    CCLog("FBUtilies::fb appEventFBEntrance");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo minfo;
        if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "appEventFBEntrance", "(Ljava/lang/String;)V")) {
            CCLOGFUNC("jni: no method");
            return;
        }
        jstring jentrance = minfo.env->NewStringUTF(entrance.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jentrance);
        minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::appEventSpeedUp(int user_castle,int user_level,int type,int spend){
    CCLog("FBUtilies::fb appEventSpeedUp");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "appEventSpeedUp", "(IIII)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,user_castle,user_level,type,spend);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::appEventGiftPackage(const std::string &entracnce,const std::string &name,int id,int user_castle,int user_level){
    CCLog("FBUtilies::fb appEventGiftPackage");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "appEventGiftPackage", "(Ljava/lang/String;Ljava/lang/String;III)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    jstring jentracnce = minfo.env->NewStringUTF(entracnce.c_str());
    jstring jname = minfo.env->NewStringUTF(name.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jentracnce,jname,id,user_castle,user_level);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::appEventHireWorker(int user_castle,int use_level){
    CCLog("FBUtilies::fb appEventHireWorker");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "appEventHireWorker", "(II)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,user_castle,use_level);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::appEventLevelUp(int user_level_up){
    CCLog("FBUtilies::fb appEventLevelUp");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "appEventLevelUp", "(I)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,user_level_up);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::appEventAllianceHonorExchange(const std::string &name,int id,int user_castle,int user_level,int rank){
    CCLog("FBUtilies::fb appEventAllianceHonorExchange");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "appEventAllianceHonorExchange", "(Ljava/lang/String;IIII)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    jstring jname = minfo.env->NewStringUTF(name.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jname,id,user_castle,user_level,rank);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::appEventAllianceScoreUsage(const std::string &name,int id,int rank){
    CCLog("FBUtilies::fb appEventAllianceScoreUsage");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "appEventAllianceScoreUsage", "(Ljava/lang/String;II)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    jstring jname = minfo.env->NewStringUTF(name.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jname,id,rank);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::appEventCityEffectLog(int cityLV,int playerLv){
    CCLog("FBUtilies::fb appEventCityEffectLog");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "appEventCityEffect", "(II)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,cityLV,playerLv);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

void FBUtilies::fbPostMethod(const std::string &method,const std::string &objectType,const std::string &objectValue,const std::string &actions,const std::string &actionsValue,const std::string &ref){
    CCLog("FBUtilies::fb fbPostMethod");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/stac/empire/sns/FBUtil", "facebookPostMethod", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    jstring jmethod = minfo.env->NewStringUTF(method.c_str());
    jstring jobjectType = minfo.env->NewStringUTF(objectType.c_str());
    jstring jobjectValue = minfo.env->NewStringUTF(objectValue.c_str());
    jstring jactions = minfo.env->NewStringUTF(actions.c_str());
    jstring jactionsValue = minfo.env->NewStringUTF(actionsValue.c_str());
    jstring jref = minfo.env->NewStringUTF(ref.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jmethod,jobjectType,jobjectValue,jactions,jactionsValue,jref);
    minfo.env->DeleteLocalRef(minfo.classID);
#endif
}

#endif