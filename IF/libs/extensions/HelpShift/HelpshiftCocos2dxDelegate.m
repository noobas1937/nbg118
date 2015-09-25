//
//  HelpshiftDelegate.m
//  Beta

#import "HelpshiftCocos2dxDelegate.h"
#import "Helpshift.h"

@implementation HelpshiftCocos2dxDelegate

@synthesize notificationCountHandler, inAppNotificationCountHandler;

static HelpshiftCocos2dxDelegate *instance;

+ (HelpshiftCocos2dxDelegate *) sharedInstance {
    if(instance) {
        return instance;
    }
    instance = [[HelpshiftCocos2dxDelegate alloc] init];
    return instance;
}

- (void) didReceiveNotificationCount:(NSInteger)count {
    if ([HelpshiftCocos2dxDelegate sharedInstance].notificationCountHandler) {
        [HelpshiftCocos2dxDelegate sharedInstance].notificationCountHandler(count);
    }
}

- (void) didReceiveInAppNotificationWithMessageCount:(NSInteger)count {
    if ([HelpshiftCocos2dxDelegate sharedInstance].inAppNotificationCountHandler) {
        [HelpshiftCocos2dxDelegate sharedInstance].inAppNotificationCountHandler(count);
    }
}

- (void) newConversationStartedWithMessage:(NSString *)newConversationMessage {
    if ([HelpshiftCocos2dxDelegate sharedInstance].newConversationHandler) {
        [HelpshiftCocos2dxDelegate sharedInstance].newConversationHandler([newConversationMessage UTF8String]);
    }
}

- (void) userRepliedToConversationWithMessage:(NSString *)newMessage {
    if ([HelpshiftCocos2dxDelegate sharedInstance].newConversationMessageHandler) {
        [HelpshiftCocos2dxDelegate sharedInstance].newConversationMessageHandler([newMessage UTF8String]);
    }
}

- (void) userCompletedCustomerSatisfactionSurvey:(NSInteger)rating withFeedback:(NSString *)feedback {
    if ([HelpshiftCocos2dxDelegate sharedInstance].csatCompletedHandler) {
        [HelpshiftCocos2dxDelegate sharedInstance].csatCompletedHandler(rating, [feedback UTF8String]);
    }
}

- (void) helpshiftSessionHasEnded {
    if ([HelpshiftCocos2dxDelegate sharedInstance].sessionEndedHandler) {
        [HelpshiftCocos2dxDelegate sharedInstance].sessionEndedHandler();
    }
}

- (void) helpshiftSessionHasBegun {
    if ([HelpshiftCocos2dxDelegate sharedInstance].sessionBeganHandler) {
        [HelpshiftCocos2dxDelegate sharedInstance].sessionBeganHandler();
    }
}
@end
