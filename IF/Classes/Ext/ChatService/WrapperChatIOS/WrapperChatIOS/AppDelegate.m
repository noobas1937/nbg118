//
//  AppDelegate.m
//  WrapperChatIOS
//
//  Created by mzc on 15/5/22.
//  Copyright (c) 2015年 mzc. All rights reserved.
//

#import "AppDelegate.h"
#import "AppDelegate.h"
#import "ServiceInterface.h"
#import "ChatServiceController.h"
#import "ChatViewController.h"
#import "MailViewController.h"
#import "ServiceInterface.h"
#import "GameHostInterface.h"
#import "GameHostDemo.h"

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    //设置用户信息
    //NSString *ns_customHeadImageUrl = [NSString stringWithUTF8String:customHeadImageUrl];
    NSString *ns_name = @"mzc";
    NSString *ns_uid = @"22222";
    NSString *ns_headPic = @"";
    NSString * ns_vipInfo = @"15";
    
    [[ServiceInterface serviceInterfaceSharedManager] screenAdaptation];
    
    [[ServiceInterface serviceInterfaceSharedManager] setPlayerInfo:2 headPicVer:0 customHeadImageUrl:@"" name:ns_name uidStr:ns_uid picStr:ns_headPic vipStr:ns_vipInfo];
    //设置联盟信息
    
    NSString *ns_asn = @"(asn)";
    NSString *ns_allianceId = @"111";
    
    [[ServiceInterface serviceInterfaceSharedManager] setPlayerAllianceInfo:ns_asn :ns_allianceId :11 :true ];
    
    [ChatServiceController chatServiceControllerSharedManager].gameHost = [[GameHostDemo alloc]init];
    
    //ChatViewController*cc =  [[ChatViewController alloc]init];
    MailViewController*cc =  [[MailViewController alloc]init];
    
    CGRect rx = [ UIScreen mainScreen ].bounds;
    
    //[ServiceInterface serviceInterfaceSharedManager].chatViewController = cc;
    [ServiceInterface serviceInterfaceSharedManager].mailViewController = cc;
    self.window=[[UIWindow alloc]initWithFrame:[UIScreen mainScreen].bounds];
    UINavigationController *nav=[[[UINavigationController alloc]initWithRootViewController:cc]init];
    self.window.rootViewController=nav;
    [self.window makeKeyAndVisible];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    NSLog(@"1");
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
