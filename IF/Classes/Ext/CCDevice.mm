#include <string>
#import <sys/utsname.h>
#import <sys/sysctl.h>
#include <sys/param.h>
#import <mach/mach.h>
#include <sys/mount.h>
#include "CCDevice.h"
#import <Parse/Parse.h>
#import "OpenUDID.h"
#import "SFHFKeychainUtils.h"
#import <TestinAgent/TestinAgent.h>
#import "UIDevice+IdentifierAddition.h"
#import "NSString+MD5Addition.h"
#include "FBUtilies.h"
#include "WorldController.h"
#import "GTMBase64.h"
#import <AdSupport/ASIdentifierManager.h>

using namespace std;

NS_CC_EXT_BEGIN

void CCDevice::rateApp()
{
    const string sURL=
    "itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id="
#if GOE_PLATFORM == GOE_PLATFORM_TENCENT
    "526362860";
#elif GOE_IAP_KINGSWARD > 0
    "515797492";
#else
    "595308614";
#endif   // GOE_PLATFORM
    openURL(sURL);
}

string CCDevice::getDeviceModel()
{
    struct utsname systemInfo;
    uname(&systemInfo);
    
    NSString *deviceModel = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    return [deviceModel UTF8String];
}

string CCDevice::getLanguage()
{
    static string sLanguage;
    if (sLanguage.empty()) {
        // get the current language and country config
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
        NSString *currentLanguage = [languages objectAtIndex:0];
        sLanguage = [currentLanguage UTF8String];
    }
    return sLanguage;
}

string CCDevice::getChannel()
{
	return "iOS";
}

void CCDevice::setTestinUser(std::string userInfo)
{
    [TestinAgent setUserInfo:[NSString stringWithUTF8String:userInfo.c_str()]];
}

string CCDevice::getDeviceUid()
{
    static string uid;
    
    uid = "test00000000000000007";

    if (uid.empty()) {
        uid = CCUserDefault::sharedUserDefault()->getStringForKey("ACCOUNT_OPENUDID","");
        CCLOG("ccdevice UserDefault %s",uid.c_str());
        if (uid == "") {
            uid = getOpenUDIDFromKeyChain();
            CCLOG("ccdevice KeyChain %s",uid.c_str());
            if (uid == "")
            {
                uid = getOpenUDID();
                CCLOG("ccdevice create %s",uid.c_str());
            }
            else
                CCUserDefault::sharedUserDefault()->setStringForKey("ACCOUNT_OPENUDID", uid);
        }
    }
    return uid;
}

string CCDevice::base64Encode(unsigned char* bytes_to_encode, unsigned int in_len){
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];
        
        while((i++ < 3))
            ret += '=';
        
    }
    
    return ret;
}

string CCDevice::getHandSetInfo()
{
    return getOSVersion();
}

string CCDevice::getVersionName()
{
    return [[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"] UTF8String];
}

string CCDevice::getVersionCode()
{
    return [[NSString stringWithFormat: @"%d",[[[[NSBundle mainBundle] infoDictionary] objectForKey:@"Version Code"] intValue]] UTF8String];
}

void CCDevice::openURL(const string &url)
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
}

string CCDevice::clipboardGetText()
{
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    string ret;
    ret=[pasteboard.string UTF8String];
    return ret;
}

void CCDevice::clipboardSetText(const string &text)
{
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    [pasteboard setPersistent:YES];
    [pasteboard setString:[NSString stringWithUTF8String:text.c_str()]];
}

string CCDevice::getOSVersion()
{
    string ret = "";
    NSString *name  = [[UIDevice currentDevice] systemName];
    NSString *number = [[UIDevice currentDevice] systemVersion];
    
    ret.append([name UTF8String]);
    ret.append([number UTF8String]);
    return ret;
}

void CCDevice::sendMail(const string &address,const string &title,const string &content)
{
    NSString *_address = [NSString stringWithUTF8String:address.c_str()];
    NSString *_title = [NSString stringWithUTF8String:title.c_str()];
    NSString *_content = [NSString stringWithUTF8String:content.c_str()];
    NSString *url = [NSString stringWithFormat:@"mailto:%@?subject=%@&body=%@",_address,_title,_content];
    NSString *escaped = [url stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:escaped]];
}

void CCDevice::updateVersion(const std::string &msg)
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:msg.c_str()]]];
}
//float CCDevice::getUsedMemoryInMB(){
//    task_basic_info_data_t taskInfo;
//    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
//    kern_return_t kernReturn = task_info(mach_task_self(),
//                                         TASK_BASIC_INFO,
//                                         (task_info_t)&taskInfo,
//                                         &infoCount);
//    if (kernReturn != KERN_SUCCESS) {
//        return NSNotFound;
//    }
//    return taskInfo.resident_size / 1024.0 / 1024.0;
//}
//
//float CCDevice::getAvailableMemoryInMB(){
//    vm_statistics_data_t vmStats;
//    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
//    kern_return_t kernReturn = host_statistics(mach_host_self(),
//                                               HOST_VM_INFO,
//                                               (host_info_t)&vmStats,
//                                               &infoCount);
//    if (kernReturn != KERN_SUCCESS) {
//        return NSNotFound;
//    }
//    return ((vm_page_size *vmStats.free_count) / 1024.0) / 1024.0;
//}
string CCDevice::getAccountInfo()
{
    static string sAccount;
    //    ACCOUNTS_CLASS_AVAILABLE(<#_desktopIntro#>, <#_iphoneIntro#>)
    return sAccount;
}

string CCDevice::getGaid()
{
    NSString *IDFA = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    return [IDFA UTF8String];
}

void CCDevice::pushNotice(int type, int time, std::string body, std::string soundKey, std::string pushType)
{
    if(body == "")
        return;
    UILocalNotification *notification=[[[UILocalNotification alloc] init] autorelease];
    if (notification!=nil) {
        UIApplication *app = [UIApplication sharedApplication];
        //获取本地推送数组
        NSArray *localArr = [app scheduledLocalNotifications];
        if (localArr) {
            for (UILocalNotification *noti in localArr) {
                NSDictionary *dict = noti.userInfo;
                if (dict) {
                    NSNumber *id = [dict objectForKey:@"id"];
                    if ([id isEqualToNumber:[NSNumber numberWithInt:type]]) {
                        [[UIApplication sharedApplication] cancelLocalNotification:noti];
                        break;
                    }
                }
            }
        }
        
        NSDate *now=[NSDate new];
        notification.fireDate=[now dateByAddingTimeInterval:time];
        notification.timeZone=[NSTimeZone defaultTimeZone];
        notification.alertBody=[NSString stringWithUTF8String:body.c_str()];
        NSDictionary *infoDic = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:type] forKey: @"id"];
        notification.userInfo = infoDic;
        notification.applicationIconBadgeNumber = 1;
        [[UIApplication sharedApplication] scheduleLocalNotification:notification];
        
        [now release];
    }
}

void CCDevice::cancelNotice(int type)
{
    UIApplication *app = [UIApplication sharedApplication];
    //获取本地推送数组
    NSArray *localArr = [app scheduledLocalNotifications];
    if (localArr) {
        for (UILocalNotification *noti in localArr) {
            NSDictionary *dict = noti.userInfo;
            if (dict) {
                NSNumber *id = [dict objectForKey:@"id"];
                if ([id isEqualToNumber:[NSNumber numberWithInt:type]] || type == -1) {
                    [[UIApplication sharedApplication] cancelLocalNotification:noti];
                    //break;
                }
            }
        }
    }
    return;
}

string CCDevice::recordPush(const std::string &push_msg,const std::string &push_type){
    double push_time = WorldController::getInstance()->getTime();
    CCString* pushRecordString = CCString::createWithFormat("%.0f[,]%s[,]%s", push_time, push_msg.c_str(), push_type.c_str());
	string cacheRecord = getPushRecordData();
    if(cacheRecord == "")
        cacheRecord = pushRecordString->getCString();
    else
        cacheRecord = cacheRecord + "[;]" + pushRecordString->getCString();
    CCUserDefault::sharedUserDefault()->setStringForKey("CATCH_PUSH_RECORD", cacheRecord);
    return pushRecordString->getCString();
}

string CCDevice::getPushRecordData()
{
    return CCUserDefault::sharedUserDefault()->getStringForKey("CATCH_PUSH_RECORD", "");
}

void CCDevice::setPushClickData(const std::string &push_click_data){
    CCUserDefault::sharedUserDefault()->setStringForKey("CATCH_PUSH_RECORD", push_click_data.c_str());
}

string CCDevice::getPushClickData()
{
    return CCUserDefault::sharedUserDefault()->getStringForKey("CATCH_PUSH_CLICK_DATA", "");
}

void CCDevice::clearPushCache(){
    CCUserDefault::sharedUserDefault()->setStringForKey("CATCH_PUSH_RECORD", "");
    CCUserDefault::sharedUserDefault()->setStringForKey("CATCH_PUSH_CLICK_DATA", "");
}

void CCDevice::finishGame()
{

}

bool CCDevice::GooglePlusIsLogin()
{
    return true;
}
void CCDevice::GooglePlusLogout()
{}
void CCDevice::GooglePlusLogin()
{}
string CCDevice::getPublishRegion(){
#ifdef Channel_91
    return "91Store";
#else
    return "AppStore";
#endif
    return "";
}
void CCDevice::doPlatformLoading(){}
void CCDevice::doPlatformLogin(){}
bool CCDevice::needPlatformQuit(){
    return false;
}
void CCDevice::doPlatformQuit(){}
void CCDevice::doPlatformCollectUserInfo(const std::string &userId,const std::string &userName,const std::string &userLevel){}

string CCDevice::getOpenUDID(bool needSave)
{
    //重新获取openUDID
    NSString* openudid = [OpenUDID value];
    string openUDID = [openudid UTF8String];
    if(needSave){
        CCUserDefault::sharedUserDefault()->setStringForKey("ACCOUNT_OPENUDID", openUDID);
        saveOpenUDIDToKeyChain(openUDID);
    }
    return openUDID;
}

// 获取Parse唯一标识
const char* CCDevice::getParseInstallationId() {
    PFInstallation *currentInstallation = [PFInstallation currentInstallation];
    if ([currentInstallation deviceToken]) {
        return [[currentInstallation deviceToken] UTF8String];
    }
    else
        return "";
}

#define KEYCHAIN_ACCOUNT_OPENUDID "cok_keychain_account_openudid"
#define KEYCHAIN_ACCOUNT_SERVICENAME "cok_keychain_account_servicename"

bool CCDevice::saveOpenUDIDToKeyChain(const std::string &openudid)
{
    NSString* _key_servicename = [NSString stringWithUTF8String:KEYCHAIN_ACCOUNT_SERVICENAME];
    
    NSString* _key_openudid = [NSString stringWithUTF8String:KEYCHAIN_ACCOUNT_OPENUDID];
    NSString* _openudid = [NSString stringWithUTF8String:openudid.c_str()];
    bool isOk3 = [SFHFKeychainUtils storeUsername:_key_openudid andPassword:_openudid forServiceName:_key_servicename updateExisting:TRUE error:nil];
    
    return isOk3;
}

string CCDevice::getOpenUDIDFromKeyChain()
{
    NSString* _key_servicename = [NSString stringWithUTF8String:KEYCHAIN_ACCOUNT_SERVICENAME];
    NSString* _key_openudid = [NSString stringWithUTF8String:KEYCHAIN_ACCOUNT_OPENUDID];
    NSString* openudid = [SFHFKeychainUtils getPasswordForUsername:_key_openudid andServiceName:_key_servicename error:nil];
    if (openudid)
        return [openudid UTF8String];
    else
        return "";
}

bool CCDevice::hasEnoughSpace(int needSize)
{
    struct statfs buf;
    long long freespace = -1;
    if(statfs("/var", &buf) >= 0){
        freespace = (long long)(buf.f_bsize * buf.f_bavail);
        freespace = freespace/1024;
    }
    return freespace<needSize?false:true;
}

void CCDevice::onUploadPhoto(int uid, int srcCode, int idx)
{
    SysUtilies::onUploadPhoto(uid, srcCode, idx);
}
bool CCDevice::addSkipBackupAttributeToItemAtURL(const std::string filePath){
    NSURL *URL= [NSURL fileURLWithPath: [NSString stringWithUTF8String:filePath.c_str()]];
    assert([[NSFileManager defaultManager] fileExistsAtPath: [URL path]]);
    
    NSError *error = nil;
    BOOL success = [URL setResourceValue: [NSNumber numberWithBool: YES]
                                  forKey: NSURLIsExcludedFromBackupKey error: &error];
    if(!success){
        NSLog(@"Error excluding %@ from backup %@", [URL lastPathComponent], error);
    }
    return success;
}
NS_CC_EXT_END