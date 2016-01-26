//
//  CSMailChattingVC.h
//  IF
//
//  Created by 张彦涛 on 15/8/31.
//
//

#import <UIKit/UIKit.h>
@class ChatChannel ;
@protocol MailChattingVCDelegate <NSObject>
@optional
-(void)mailChattingVCHasNewMsgCallBackWithChatChannel:(ChatChannel *)opendChannel;

@end
typedef void(^ReadStatusBlock)(void) ;
@interface CSMailChattingVC : UIViewController
@property (nonatomic,strong) UITableView * mailChatTableView;
@property (strong,nonatomic) ChatChannel * currentChatChannel;
@property (nonatomic,assign) id<MailChattingVCDelegate>mailChattingVCDelegate;
@property (nonatomic,assign) ReadStatusBlock readStatusBlock;

-(void)deallocAnyWithMailChattingVC;
@end
