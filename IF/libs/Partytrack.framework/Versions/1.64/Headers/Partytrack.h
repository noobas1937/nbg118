//
//  Partytrack.h
//  partytrack
//
//  Created by Jamie on 12/3/12.
//  Copyright (c) 2012 adways. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "PTSenderDelegate.h"

@interface Partytrack : NSObject<PTSenderDelegate>

+ (Partytrack*) sharedInstance;
+ (void) openDebugInfo;

- (void) setConfigureWithName:(NSString *)name AndValue: (NSString *)value;
- (void) setCustomEventParameterWithName:(NSString *)name AndValue: (NSString *)value;
- (void) disableApplicationTracking;
- (void) startWithAppID:(int)app_id AndKey:(NSString *)key;
- (void) startWithAppID:(int)app_id AndKey:(NSString *)key AndOptions:(NSDictionary *)launchOptions;
- (void) sendEventWithID:(int)event_id;
- (void) sendEventWithName:(NSString *)event_name;
- (void) sendPaymentWithParameters: (NSDictionary *)params;

// Optional: you can only the methods below as you import "storekit" framework
- (void) addProducts:(NSArray *)products;
- (void) sendPaymentWithTransactions:(NSArray *)transactions;

@end

