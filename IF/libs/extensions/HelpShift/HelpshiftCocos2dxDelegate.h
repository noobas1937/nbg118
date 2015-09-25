//
//  HelpshiftDelegate.h
//  Beta

#import <Foundation/Foundation.h>
#import "Helpshift.h"

@interface HelpshiftCocos2dxDelegate : NSObject <HelpshiftDelegate>

+ (HelpshiftCocos2dxDelegate *) sharedInstance;

@property (nonatomic) void (*notificationCountHandler) (int count);
@property (nonatomic) void (*inAppNotificationCountHandler) (int count);
@property (nonatomic) void (*newConversationHandler) (const char *message);
@property (nonatomic) void (*newConversationMessageHandler) (const char *message);
@property (nonatomic) void (*csatCompletedHandler) (int rating, const char *message);
@property (nonatomic) void (*sessionBeganHandler) (void);
@property (nonatomic) void (*sessionEndedHandler) (void);

@end
