//
//  SNSUtil.m
//  IF
//
//  Created by 王智圆 on 15/7/29.
//
//

#include <string>
#include "SNSUtil.h"
#include "AppController.h"
#include "CCSafeNotificationCenter.h"
#include "TypeDefinition.h"



void SNSUtilies::doLogin(std::string pf){
    if(pf == "weibo")
        [[SNSUtiliesIOS shared] loginWeibo];
    if(pf == "vk")
        [[SNSUtiliesIOS shared] loginVK];
}



USING_NS_CC;

@interface SNSUtiliesIOS() <VKSdkDelegate,UIAlertViewDelegate,WeiboSDKDelegate>{
//    UIPopoverController * popoverController;
}
@property (strong, nonatomic) UIWindow *window;
+(SNSUtiliesIOS*)shared;
@end

@implementation SNSUtiliesIOS

static SNSUtiliesIOS *_sharedUtilies;
+(SNSUtiliesIOS*)shared {
    if(_sharedUtilies == nil) {
        _sharedUtilies = [[SNSUtiliesIOS alloc] init];
    }
    return _sharedUtilies;
}
- (id)init {
    //weibo
    [WeiboSDK enableDebugMode:true];
    [WeiboSDK registerApp:@"2857911180"];
    //vk
    [VKSdk initializeWithDelegate:self andAppId:@"5024256"];
    return self;
}



-(void)loginWeibo
{
    WBAuthorizeRequest *authRequest = [WBAuthorizeRequest request];
    authRequest.redirectURI = @"https://api.weibo.com/oauth2/default.html";
    authRequest.scope = @"all";
    [WeiboSDK sendRequest:authRequest];    
}

- (void)didReceiveWeiboResponse:(WBBaseResponse *)response
{
    if ([response isKindOfClass:WBAuthorizeResponse.class])
    {
        NSString *playerID = [(WBAuthorizeResponse *)response userID];
        NSString *playerName = [(WBAuthorizeResponse *)response userID];//ios版本无法取得账号昵称
        NSDictionary *userInfo = response.userInfo;
        if(playerID != nil){
            std::string jsonStr=[[NSString stringWithFormat:@"{\"userId\":\"%@\",\"userName\":\"%@\",\"is_connect\":\"true\",\"msgId\":\"login_sucess_weibo\",\"platform\":\"weibo\"}",playerID,playerName] UTF8String];
            cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("onResponsed3rdPlatform",CCString::create(jsonStr.c_str()));
        }
    }
}



-(void)loginVK
{
    NSArray *SCOPE = @[VK_PER_FRIENDS, VK_PER_WALL, VK_PER_AUDIO, VK_PER_PHOTOS, VK_PER_NOHTTPS, VK_PER_EMAIL, VK_PER_MESSAGES];
    [VKSdk authorize:SCOPE revokeAccess:YES];
}

-(void)vkSdkNeedCaptchaEnter:(VKError*) captchaError
{
    VKCaptchaViewController *vc = [VKCaptchaViewController captchaControllerWithError:captchaError];
    [vc presentIn:self];
}

- (void)vkSdkTokenHasExpired:(VKAccessToken *)expiredToken {
    [self authorize:nil];
}

- (void)vkSdkUserDeniedAccess:(VKError *)authorizationError {
//    [[[UIAlertView alloc] initWithTitle:nil message:@"Access denied" delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil] show];
}

- (void)vkSdkShouldPresentViewController:(UIViewController *)controller {
    [self.navigationController.topViewController presentViewController:controller animated:NO completion:nil];
}

-(void)vkSdkReceivedNewToken:(VKAccessToken*) newToken
{
    NSString *playerID = [[VKSdk getAccessToken] userId];
    NSString *playerName = [[VKSdk getAccessToken] userId];
    if(playerID != nil){
        std::string jsonStr=[[NSString stringWithFormat:@"{\"userId\":\"%@\",\"userName\":\"%@\",\"is_connect\":\"true\",\"msgId\":\"login_sucess_other\",\"platform\":\"vk\"}",playerID,playerName] UTF8String];
        cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("onResponsed3rdPlatform",CCString::create(jsonStr.c_str()));
    }
}

-(BOOL)vkSdkAuthorizationAllowFallbackToSafari {
    return YES;
}

@end