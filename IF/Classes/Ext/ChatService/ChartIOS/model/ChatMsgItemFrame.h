//
//  ChatMsgItemFrame.h
//  IF
//
//  Created by 张彦涛 on 15/7/10.
//
//

#import <Foundation/Foundation.h>
#import "NSMsgItem.h"
@interface ChatMsgItemFrame : NSObject
@property (strong,nonatomic) NSMsgItem * msgItem;

@property (assign,nonatomic) CGRect   msgLabelF;
@property (assign,nonatomic) CGRect   headImageViewF;
@property (assign,nonatomic) CGRect   nickNameLabelF;

//

@end
