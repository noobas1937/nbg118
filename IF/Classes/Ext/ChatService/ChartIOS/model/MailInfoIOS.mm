//
//  MailInfo.m
//  IF
//
//  Created by mzc on 15/4/23.
//
//

#import "MailInfoIOS.h"

@implementation MailInfoIOS

-(MailInfoIOS*)init
{
    self = [super init];
    if (self) {
        self.opponentUid = @"";
        self.myUid = @"";
        self.opponentName = @"";
        self.type = -1;
        self.isCurChannelFirstVisit = FALSE;
    }
    return self;
}
@end
