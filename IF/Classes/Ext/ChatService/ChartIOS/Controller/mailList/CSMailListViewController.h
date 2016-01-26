//
//  CSMailListViewController.h
//  IF
//
//  Created by 张彦涛 on 15/8/19.
//
//

#import <UIKit/UIKit.h>
#import "SlideCellProtocol.h"
@class ChatChannel ;

typedef enum : NSUInteger {
    MailListVCChatType_Normol = 0,
    MailListVCChatType_Mod = 1
} MailListVCChatType;


@interface CSMailListViewController : UIViewController
@property (nonatomic,assign) MailListVCChatType  mailListVCChatType;
-(instancetype)initWithChatMailType:(MailListVCChatType)vChatTpye;

@end
