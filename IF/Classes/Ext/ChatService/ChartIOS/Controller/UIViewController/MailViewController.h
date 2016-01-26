//
//  MailViewController.h
//  IF
//
//  Created by mzc on 15/4/23.
//
//

#import <UIKit/UIKit.h>
#import "ChatMailTableTableViewController.h"
#import "KeyBordVIew.h"
#import "TopUIView.h"
@interface MailViewController : UIViewController
@property (nonatomic,strong) ChatMailTableTableViewController *mailTableTableViewController;
@property (nonatomic,strong) KeyBordVIew *keyBordView;
@property (nonatomic,strong) TopUIView *topUIView;
/**
 *  当前邮件控制器为邮件，还是聊天
 */
@property (nonatomic,assign)ChatMailTableVCType chatMailTableViewType;

-(void)endEditWithTextField;
-(void)loadingChannelAndMsg;

@end
