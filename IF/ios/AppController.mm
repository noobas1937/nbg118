//
//  IFAppController.mm
//  IF
//
//  Created by 吴 凌江 on 13-8-28.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "CCEAGLView-ios.h"
#import "AppDelegate.h"
#import "ShopIAPHelper.h"
#import <Parse/Parse.h>
//#import <TestinAgent/TestinAgent.h>
#import "RootViewController.h"
#import <Fabric/Fabric.h>
#import <Crashlytics/Crashlytics.h>
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import <NanigansSDK/NanigansSDK.h>
#import "Helpshift.h"
#include "../Classes/Ext/CCDevice.h"
#include "GameController.h"
#import <Branch/Branch.h>
#import "WeiboSDK.h"
#import "ServiceInterface.h"
#include "BranchController.h"
#import <VKSdk/VKSdk.h>
#include "SNSUtil.h"

@implementation AppController

@synthesize window;
@synthesize viewController;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
   CCEAGLView *__glView = [CCEAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH_COMPONENT16
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples:0 ];

    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    [__glView setMultipleTouchEnabled: YES];
    viewController.view = __glView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
//branch
    Branch *branch = [Branch getInstance];
    [branch initSessionWithLaunchOptions:launchOptions andRegisterDeepLinkHandler:^(NSDictionary *params, NSError *error) {
        if (!error) {
            // params are the deep linked params associated with the link that the user clicked -> was re-directed to this app
            // params will be empty if no data found
            // ... insert custom logic here ...
            NSArray *keys = [params allKeys];// 所有key
            for(int i = 0; i < [keys count]; i++){
                NSString *key = [keys objectAtIndex:i];
                NSObject *value = [params objectForKey:key];
                
                std::string keyStr = [key cStringUsingEncoding: NSUTF8StringEncoding];
                std::string valueStr = [value.description cStringUsingEncoding: NSUTF8StringEncoding];

                BranchController::getInstance()->addToBranchData(keyStr, valueStr);
            }
            NSLog(@"params: %@", params.description);
        }
    }];
    
//    [TestinAgent init:@"f1d4256db620d787f78a1e28c415c5fd" channel:@"AppStore"];
    
    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:YES];
    
    [[SKPaymentQueue defaultQueue] addTransactionObserver:[ShopIAPHelper sharedHelper]];
    
    [Fabric with:@[CrashlyticsKit]];
    
    [Parse setApplicationId:@"mOzkRMxp0CwRal3O1rOXH9VJK4pIgJApzcY3Vmsj"
                  clientKey:@"vyXy2kQHrwmCv0sLCxIRFDwpZSZk2qyTirjKgmwh"];
    cocos2d::GLViewImpl *glview = cocos2d::GLViewImpl::createWithEAGLView(__glView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    cocos2d::CCApplication::sharedApplication()->run();
    
    //注册消息推送
#if __IPHONE_OS_VERSION_MAX_ALLOWED <= __IPHONE_7_1
    [[UIApplication sharedApplication] registerForRemoteNotificationTypes: (UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert)];
#elif __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_8_0
    if ([application respondsToSelector:@selector(registerUserNotificationSettings:)]) {
        UIUserNotificationSettings* notificationSettings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound categories:nil];
        [[UIApplication sharedApplication] registerUserNotificationSettings:notificationSettings];
        [[UIApplication sharedApplication] registerForRemoteNotifications];
    } else {
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes: (UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert)];
    }
#else
    [[UIApplication sharedApplication] registerForRemoteNotificationTypes: (UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert)];
#endif
    
    if (launchOptions != nil) //Handle PushNotification when app is opened
    {
        NSDictionary* userInfo = [launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey];
        if (userInfo != nil)
        {
            NSLog(@"launchOptions : %@",userInfo);
            //helpshift回复推送以后自动打开
            if([[userInfo objectForKey:@"origin"] isEqualToString:@"helpshift"])
                [[Helpshift sharedInstance] handleRemoteNotification:userInfo withController:self.viewController];
            //记录打开应用时候推送的数据，需要推送加参数，暂时去掉
//            if([userInfo objectForKey:@"cok_push_type"]){
//                std::string cok_push_type = [[userInfo objectForKey:@"cok_push_type"] UTF8String];
//                cocos2d::extension::CCDevice::setPushClickData(cok_push_type);
//            }
        }
    }
    
    [FBSDKProfile enableUpdatesOnAccessTokenChange:YES];
    return [[FBSDKApplicationDelegate sharedInstance] application:application didFinishLaunchingWithOptions:launchOptions];
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
//    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
//    cocos2d::CCDirector::sharedDirector()->resume();

//TODO:: 
//    cocos2d::Application::getInstance()->applicationDidBecomeActive();
    // 进入游戏，右上角计数清0
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
//    [FBSDKAppEvents activateApp];
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();

}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    [NANTracking trackAppLaunch:nil];
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

//- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
//    // Store the deviceToken in the current installation and save it to Parse.
//    PFInstallation *currentInstallation = [PFInstallation currentInstallation];
//    [currentInstallation setDeviceTokenFromData:deviceToken];
//    [currentInstallation saveInBackground];
//}
//
//- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
//    [PFPush handlePush:userInfo];
//}

//获取DeviceToken成功
- (void)application:(UIApplication *)application
didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    NSString *token = [NSString
                       stringWithFormat:@"%@",deviceToken];
    token = [token stringByReplacingOccurrencesOfString:@"<" withString:@""];
    token = [token stringByReplacingOccurrencesOfString:@">" withString:@""];
    token = [token stringByReplacingOccurrencesOfString:@" " withString:@""];
    NSLog(@"deviceToken is:%@", token);
    
    // Store the deviceToken in the current installation and save it to Parse.
    PFInstallation *currentInstallation = [PFInstallation currentInstallation];
    [currentInstallation setDeviceTokenFromData:deviceToken];
    [currentInstallation saveInBackground];
    
    [[Helpshift sharedInstance] registerDeviceToken:deviceToken];
    
}

//注册消息推送失败
- (void)application:(UIApplication *)application
didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    NSLog(@"Register Remote Notifications error:{%@}",[error localizedDescription]);
}

//处理收到的消息推送
- (void)application:(UIApplication *)application
didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    NSLog(@"Receive remote notification : %@",userInfo);
    // 收到来自服务器的推送时，图标右上角显示计数为1
    //    int count = CCUserDefault::sharedUserDefault()->getIntegerForKey("badge_number");
    //    CCUserDefault::sharedUserDefault()->setIntegerForKey("badge_number", count + 1);
    [UIApplication sharedApplication].applicationIconBadgeNumber = 1;

    if (application.applicationState == UIApplicationStateInactive) {
        [PFAnalytics trackAppOpenedWithRemoteNotificationPayload:userInfo];
    }
    //本地推送记录
    NSMutableDictionary *newPushInfo = [NSMutableDictionary dictionaryWithDictionary:userInfo];
    if (application.applicationState == UIApplicationStateInactive || application.applicationState == UIApplicationStateBackground) {
        if([userInfo objectForKey:@"aps"] && [[userInfo objectForKey:@"aps"] objectForKey:@"alert"]){
            std::string push_type = "0";
            std::string push_msg = [[[userInfo objectForKey:@"aps"] objectForKey:@"alert"] UTF8String];
            if([userInfo objectForKey:@"cok_push_type"]){
                push_type = [[userInfo objectForKey:@"cok_push_type"] UTF8String];
            }
            std::string recordPush = cocos2d::extension::CCDevice::recordPush(push_msg,push_type);
            NSString *nsRecordPush = [NSString stringWithUTF8String : recordPush.c_str()];
            [newPushInfo setValue:nsRecordPush forKey:@"cok_push_click"];
            //sendDataToXingcloud
            string gameUID = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID,"");
            if(gameUID != ""){
                string url = XINGCLOUD_URl+gameUID+"?action=pushreceive&type="+push_type+"&device=ios";
                GameController::getInstance()->sendURLRequest(url);
            }
        }
        [PFPush handlePush:newPushInfo];
    }
    if ([[userInfo objectForKey:@"origin"] isEqualToString:@"helpshift"]) {
        [[Helpshift sharedInstance] handleRemoteNotification:userInfo withController:self.viewController];
    }
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    if (url != nil) {
        NSString *temp = [url absoluteString];
        NSLog(@"appLink url=%@",temp);
        temp = [temp stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        std::string tempLink = [temp cStringUsingEncoding:NSUTF8StringEncoding];
        cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("iso_fbAPPLink", tempLink);
    }
    [NANTracking trackAppLaunch:url];
    [WeiboSDK handleOpenURL:url delegate:[SNSUtiliesIOS shared]];
    [VKSdk processOpenURL:url fromApplication:sourceApplication];
    return  [[FBSDKApplicationDelegate sharedInstance] application:application
                                                           openURL:url
                                                 sourceApplication:sourceApplication
                                                        annotation:annotation];
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
    return [WeiboSDK handleOpenURL:url delegate:[SNSUtiliesIOS shared]];
}

- (NSDictionary *)parametersWithSeparator:(NSString *)separator delimiter:(NSString *)delimiter url:(NSString *)str{
    NSArray *parameterPairs =[str componentsSeparatedByString:delimiter];
    NSMutableDictionary *parameters = [NSMutableDictionary dictionaryWithCapacity:[parameterPairs count]];
    for (NSString *currentPair in parameterPairs) {
        NSRange range = [currentPair rangeOfString:separator];
        if(range.location == NSNotFound)
            continue;
        NSString *key = [currentPair substringToIndex:range.location];
        NSString *value =[currentPair substringFromIndex:range.location + 1];
        [parameters setObject:value forKey:key];
    }
    return parameters;
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
//    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
//    cocos2d::CCDirector::sharedDirector()->purgeCachedData();
}


- (void)dealloc {
    [super dealloc];
}


@end

