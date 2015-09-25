//
//  MailInfo.h
//  IF
//
//  Created by mzc on 15/4/23.
//
//

#import <Foundation/Foundation.h>

@interface MailInfoIOS : NSObject
@property (nonatomic,strong) NSString *opponentUid;
@property (nonatomic,strong) NSString *myUid;
@property (nonatomic,strong) NSString *opponentName;
@property (nonatomic,assign) int      type;
@property (nonatomic,assign) BOOL     isCurChannelFirstVisit;
@end
