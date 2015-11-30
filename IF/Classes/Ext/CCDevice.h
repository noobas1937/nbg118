//
//  GOEUtilies.h
//  ccgoe
//
//  Created by Limin on 12-11-8.
//
//

#ifndef _GOEUtilies_H_
#define _GOEUtilies_H_

#include "cocos2d.h"
#include "cocos-ext.h"

NS_CC_EXT_BEGIN

namespace CCDevice{
#pragma mark -
#pragma mark Platform dependent
    void rateApp();
    
    std::string getDeviceModel();
    std::string getLanguage();
    std::string getChannel();
    std::string getDeviceUid();
    std::string base64Encode(unsigned char* bytes_to_encode, unsigned int in_len);

    /* added by zou_c */
    std::string getOSVersion();
    void sendMail(const std::string &address,const std::string &title,const std::string &content);
//    float getUsedMemoryInMB();
//    float getAvailableMemoryInMB();
    /* added end */
    
    std::string getVersionName();
    // 动态更新资源时使用的版本号
    // android 的版本号因为历史原因为 1.1.8 + 'x.x.x' <==> getVersionName = 1.1.8 + getDLCVersionName
    std::string getDLCVersionName();
    std::string getHandSetInfo();
    std::string getVersionCode();
    void updateVersion(const std::string &msg);
    void openURL(const std::string &sURL);
    std::string getAccountInfo();
    std::string getGaid();
    void pushNotice(int type,int time,std::string body="",std::string soundKey="",std::string pushType="");
    void cancelNotice(int type);
    std::string getPushRecordData();
    std::string getPushClickData();
    void clearPushCache();
    void finishGame();
    void doPlatformLoading();
    void doPlatformLogin();
    bool needPlatformQuit();
    void doPlatformQuit();
    void doPlatformCollectUserInfo(const std::string &userId,const std::string &userName,const std::string &userLevel);
    //Google Play Login
    bool GooglePlusIsLogin();
    void GooglePlusLogout();
    void GooglePlusLogin();
    void setTestinUser(std::string userInfo);
    //发布地区
    std::string getPublishRegion();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //第三方登录：Google+,GameCenter相关
    void login_Platform(int platform);
    void logout_Platform(int platform);
    bool isAvailable_Platform(int platform);
    bool isConnected_Platform(int platform);
    cocos2d::CCDictionary* getLoginedInfo_Platform(int platform);
    std::string getSDcardPath();
//#else
//    void login_Platform(int platform){};
//    void logout_Platform(int platform){};
//    bool isAvailable_Platform(int platform){return false;};
//    bool isConnected_Platform(int platform){return false;};
//    cocos2d::CCDictionary* getLoginedInfo_Platform(int platform){};
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    std::string getOpenUDID(bool needSave = true);
    bool saveOpenUDIDToKeyChain(const std::string &openudid);
    std::string getOpenUDIDFromKeyChain();
    // 获取Parse唯一标识
    const char* getParseInstallationId();
    std::string recordPush(const std::string &push_msg,const std::string &push_type);
    void setPushClickData(const std::string &push_click_data);
    bool addSkipBackupAttributeToItemAtURL(const std::string filePath);
#endif
    
#pragma mark * clipboard
    std::string clipboardGetText();
    void clipboardSetText(const std::string &text);
    bool hasEnoughSpace(int needSize);
    
    //上传头像
    void onUploadPhoto(int uid, int srcCode, int idx);
};

NS_CC_EXT_END

#endif // _GOEUtilies_H_
