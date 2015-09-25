//
//  FRAppAgent.h
//
//  Copyright (c) 2014 Testin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TestinAgent : NSObject

+ (void)init:(NSString *)appId;
+ (void)init:(NSString *)appId channel:(NSString *)channel;

+ (void)setUserInfo:(NSString *)userInfo;

+ (void)reportCustomizedException:(NSException *)exception message:(NSString *)message;
+ (void)reportCustomizedException:(NSNumber *)type message:(NSString *)message stackTrace:(NSString *)stackTrace;

@end
