//
//  NBG.mm
//  nbg
//
//  Created by gj on 9/7/15.
//
//

#include "NBG.h"
#import "SFHFKeychainUtils.h"
#import "OpenUDID.h"

using namespace std;
USING_NS_CC;

// -------------------------------------------------------------------
#pragma mark - iOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

static bool saveOpenUDIDToKeyChain(const std::string &openudid);
static std::string getOpenUDIDFromKeyChain();

bool saveOpenUDIDToKeyChain(const std::string &openudid)
{
    NSString* _key_servicename = [NSString stringWithUTF8String:KEYCHAIN_ACCOUNT_SERVICENAME];
    NSString* _key_openudid = [NSString stringWithUTF8String:KEYCHAIN_ACCOUNT_OPENUDID];
    NSString* _openudid = [NSString stringWithUTF8String:openudid.c_str()];
    bool isStored = [SFHFKeychainUtils storeUsername:_key_openudid andPassword:_openudid forServiceName:_key_servicename updateExisting:TRUE error:nil];
    
    return isStored;
}

string getOpenUDIDFromKeyChain()
{
    NSString* _key_servicename = [NSString stringWithUTF8String:KEYCHAIN_ACCOUNT_SERVICENAME];
    NSString* _key_openudid = [NSString stringWithUTF8String:KEYCHAIN_ACCOUNT_OPENUDID];
    NSString* openudid = [SFHFKeychainUtils getPasswordForUsername:_key_openudid andServiceName:_key_servicename error:nil];
    return openudid ? [openudid UTF8String] : "";
}

// --

string NBG::getUDID()
{
    static string uid;
    if (uid.empty())
    {
        uid = UserDefault::getInstance()->getStringForKey(ACCOUNT_OPENUDID, "");
        CCLOG("NBG::getUDID UserDefault %s", uid.c_str());
        if (uid == "")
        {
            uid = getOpenUDIDFromKeyChain();
            CCLOG("NBG::getUDID KeyChain %s", uid.c_str());
            if (uid == "")
            {
                uid = getOpenUDID();
                CCLOG("NBG::getUDID create %s", uid.c_str());
            }
            else
            {
                UserDefault::getInstance()->setStringForKey(ACCOUNT_OPENUDID, uid);
            }
        }
    }
    return uid;
}

string NBG::getOpenUDID(bool autosave)
{
    NSString* openudid = [OpenUDID value];
    string openUDID = [openudid UTF8String];
    if (autosave)
    {
        UserDefault::getInstance()->setStringForKey(ACCOUNT_OPENUDID, openUDID);
        saveOpenUDIDToKeyChain(openUDID);
    }
    return openUDID;
}

string NBG::getCountryCode()
{
    NSLocale *locale = NSLocale.currentLocale;
    if (!locale) return "";
    NSString *countryCode = [locale objectForKey:NSLocaleCountryCode];
    if (!countryCode) return "";
    return [countryCode UTF8String];
}

string NBG::getPublishRegion()
{
    return "AppStore";
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
    string ret = "";
    NSString *name  = [[UIDevice currentDevice] systemName];
    NSString *number = [[UIDevice currentDevice] systemVersion];
    
    ret.append([name UTF8String]);
    ret.append([number UTF8String]);
    return ret;
}

string NBG::getVersionName()
{
    return [[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"] UTF8String];
}

string NBG::getVersionCode()
{
    return [[NSString stringWithFormat: @"%d",[[[[NSBundle mainBundle] infoDictionary] objectForKey:@"Version Code"] intValue]] UTF8String];
}

string NBG::getLanguage()
{
    static string sLanguage;
    if (sLanguage.empty())
    {
        // get the current language and country config
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
        NSString *currentLanguage = [languages objectAtIndex:0];
        sLanguage = [currentLanguage UTF8String];
    }
    return sLanguage;
}

#endif
