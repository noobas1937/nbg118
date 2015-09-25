//
//  PlatformUtil.m
//  IF
//
//  Created by chenliang on 14/12/1.
//
//

#include <string>
#include "PlatformUtil.h"
#import "NSString+MD5Addition.h"
#import <MessageUI/MessageUI.h>
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#include "AppController.h"
#include "CCSafeNotificationCenter.h"
#include "AppDelegate.h"
#include <sys/sysctl.h>
#include "CommonInclude.h"

#import <sys/sysctl.h>
#import <mach/mach.h>

#import <GameKit/GameKit.h>
#import <AudioToolbox/AudioToolbox.h>

#include "SNSUtil.h"


@interface COKPlatformUtilies : NSObject{
}

+(COKPlatformUtilies*)shared;

-(BOOL) isGameCenterAvailable;
- (void) authenticateLocalPlayer:(int)type;
-(BOOL) isConnected;
-(void) gotoIOSCenter;

@end

static bool isRequestLogin_gamecenter=false;
void PlatformUtilies::login_Platform(int loginType){
    isRequestLogin_gamecenter=true;
    [[COKPlatformUtilies shared] authenticateLocalPlayer:loginType];
}
void PlatformUtilies::goto_IOSGameCenter(){
    [[COKPlatformUtilies shared] gotoIOSCenter];
}
void PlatformUtilies::logout_Platform(int platform){
    string jsonStr="{\"userId\":\"\",\"userName\":\"\",\"is_connect\":\"false\",\"msgId\":\"logout_sucess_gamecenter\"}";
    cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->
    postNotification("onResponsed3rdPlatform",CCString::create(jsonStr.c_str()));
}
bool PlatformUtilies::isAvailable_Platform(int platform){
    //    http://www.himigame.com/iphone-cocos2dx/743.html
    return [[COKPlatformUtilies shared] isGameCenterAvailable];
}
bool PlatformUtilies::isConnected_Platform(int platform){
    return [[COKPlatformUtilies shared] isConnected];
}
cocos2d::CCDictionary* PlatformUtilies::getLoginedInfo_Platform(int platform){
    if(isAvailable_Platform(platform)&&[GKLocalPlayer localPlayer].isAuthenticated){
        
        NSLog(@"成功");
        NSLog(@"1--alias--.%@",[GKLocalPlayer localPlayer].alias);
        NSLog(@"2--authenticated--.%d",[GKLocalPlayer localPlayer].authenticated);
        NSLog(@"3--isFriend--.%d",[GKLocalPlayer localPlayer].isFriend);
        NSLog(@"4--playerID--.%@",[GKLocalPlayer localPlayer].playerID);
        NSLog(@"5--underage--.%d",[GKLocalPlayer localPlayer].underage);
        NSLog(@"6--displayName--.%@",[GKLocalPlayer localPlayer].displayName);
        NSLog(@"7--description--.%@",[GKLocalPlayer localPlayer].description);
        NSString* playerID=[GKLocalPlayer localPlayer].playerID;
        std::string str_playerID=[playerID UTF8String];
        CCDictionary* loginedDict=  CCDictionary::create();
        loginedDict->setObject(CCString::create(str_playerID.c_str()), "userName");
        loginedDict->setObject(CCBoolean::create(true), "is_connect");
        return loginedDict;
    }
    return CCDictionary::create();
}

void sendLoginSucess_Notification_GameCenter(){
    if (isRequestLogin_gamecenter) {//只响应一次
        isRequestLogin_gamecenter=false;
        NSString* playerID=[GKLocalPlayer localPlayer].playerID;
        NSString* playerName=[GKLocalPlayer localPlayer].alias;
        std::string str_playerID=[playerID UTF8String];
//        //发送登录成功通知：
        string jsonStr=[[NSString stringWithFormat:@"{\"userId\":\"%@\",\"userName\":\"%@\",\"is_connect\":\"true\",\"msgId\":\"login_sucess_gamecenter\"}",playerID,playerName] UTF8String];
        cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("onResponsed3rdPlatform",CCString::create(jsonStr.c_str()));
    }
}

void PlatformUtilies::loginPlatform(std::string pf){
    SNSUtilies::doLogin(pf);
}

void PlatformUtilies::logoutPlatform(std::string pf){
    if(pf=="weibo"){
    }
}


@implementation COKPlatformUtilies

static COKPlatformUtilies *_sharedUtilies;
+(COKPlatformUtilies*)shared {
    if(_sharedUtilies == nil) {
        _sharedUtilies = [[COKPlatformUtilies alloc] init];
        [_sharedUtilies registerForAuthenticationNotification];
        
    }
    return _sharedUtilies;
}
- (id)init {
    return self;
}


- (BOOL) isGameCenterAvailable
{
    //Check for presence of GKLocalPlayer API.
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
    //The device must be running running iOS 4.1 or later.
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
    
    return (gcClass && osVersionSupported);
}

- (BOOL) isConnected{
    return [GKLocalPlayer localPlayer].isAuthenticated;
}

static BOOL isRequestAuthenticateFlag=false;
static BOOL isRequestFirst=true;
static BOOL isSetAuthenticateHandler=false;

- (void) gotoIOSCenter
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"gamecenter:"]];
}

// type: 0-手动 1-自动
- (void) authenticateLocalPlayer:(int)type
{
    NSLog(@"Game Center code:%d @", type);
    if ([GKLocalPlayer localPlayer].isAuthenticated) {
        sendLoginSucess_Notification_GameCenter();
        return;
    }
    
    if (type == 0) {
        //打开gamecenter
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"gamecenter:"]];
        if (isSetAuthenticateHandler)
            return;
    }
    
    isSetAuthenticateHandler = true;
    isRequestAuthenticateFlag=true;
    NSString *reqSysVer = @"6.0";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
    {
        // Gamekit login for ios 6
        [[GKLocalPlayer localPlayer] setAuthenticateHandler:(^(UIViewController* viewcontroller, NSError *error) {
            if (viewcontroller != nil) {
                // 新手期间屏蔽
//                if (AOEGameBoardController::sharedGameBoardController()->isBetweenNewer()) {
//                    return;
//                }
                // 提示绑定
                AppController* appDelegate = [UIApplication sharedApplication].delegate;
                [appDelegate.viewController presentViewController:viewcontroller animated:YES completion:nil];
            }else if ([GKLocalPlayer localPlayer].authenticated)
            {
                //do some stuff
                //成功处理
                //切换用户时看是否要重新复制Plist文件
                sendLoginSucess_Notification_GameCenter();
            }else if (error && error.code == GKErrorCancelled) {
                //value	__NSCFString *	@"所请求的操作已被用户取消或停用。"	0x1cd37b00
                NSLog(@"Game Center code:%ld %@", (long)error.code, [error debugDescription]);
                string jsonStr="{\"userId\":\"\",\"userName\":\"\",\"is_connect\":\"false\",\"msgId\":\"login_failed_gamecenter\"}";
                cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->
                    postNotification("onResponsed3rdPlatform",CCString::create(jsonStr.c_str()));
                if (isRequestAuthenticateFlag){
                    isRequestAuthenticateFlag=false;
                    if (isRequestFirst&&GKErrorCancelled==[error code]) {
                        isRequestFirst=false;
                        return;
                    }
                    //打开gamecenter
                    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"gamecenter:"]];
                }
            }
            return ;
        })];
    } else {
        // Gamekit login for ios 5
        //[[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:nil];
        [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error){
            if (error == nil) {
                //成功处理
                //切换用户时看是否要重新复制Plist文件
                sendLoginSucess_Notification_GameCenter();
                
            }else {
                //错误处理
                NSLog(@"失败  %@",error);
                //Error Domain=GKErrorDomain Code=2 "The requested operation has been cancelled or disabled by the user." UserInfo=0x1025edd0 {NSLocalizedDescription=The requested operation has been cancelled or disabled by the user.}
                if ([GKLocalPlayer localPlayer].isAuthenticated) {
                    sendLoginSucess_Notification_GameCenter();
                }else{
                    string jsonStr="{\"userId\":\"\",\"userName\":\"\",\"is_connect\":\"false\",\"msgId\":\"login_failed_gamecenter\"}";
                    cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->
                    postNotification("onResponsed3rdPlatform",CCString::create(jsonStr.c_str()));
                    if (isRequestAuthenticateFlag){
                        isRequestAuthenticateFlag=false;
                        if (isRequestFirst&&GKErrorCancelled==[error code]) {
                            isRequestFirst=false;
                            return;
                        }
                        //打开gamecenter
                        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"gamecenter:"]];
                    }
                }
               
            }
        }];
        
        
    }
}
- (void) registerForAuthenticationNotification
{
    NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
    [nc addObserver:self
           selector:@selector(authenticationChanged)
               name:GKPlayerAuthenticationDidChangeNotificationName
             object:nil];
}

- (void) authenticationChanged
{
    if ([GKLocalPlayer localPlayer].isAuthenticated)
    {
        // Insert code here to handle a successful authentication.
        NSLog(@"isAuthenticated:true");
        NSLog(@"1--alias--.%@",[GKLocalPlayer localPlayer].alias);
        NSLog(@"2--authenticated--.%d",[GKLocalPlayer localPlayer].authenticated);
        NSLog(@"3--isFriend--.%d",[GKLocalPlayer localPlayer].isFriend);
        NSLog(@"4--playerID--.%@",[GKLocalPlayer localPlayer].playerID);
        NSLog(@"5--underage--.%d",[GKLocalPlayer localPlayer].underage);
        NSLog(@"6--displayName--.%@",[GKLocalPlayer localPlayer].displayName);
        NSLog(@"7--description--.%@",[GKLocalPlayer localPlayer].description);
        if ([GKLocalPlayer localPlayer].isAuthenticated) {
            sendLoginSucess_Notification_GameCenter();
        }
    }
    else
    {
        // Insert code here to clean up any outstanding Game Center-related classes.
        
        NSLog(@"isAuthenticated:false");
    }
}
@end