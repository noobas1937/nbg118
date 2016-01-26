//
//  ChattingVC.h
//  IF
//
//  Created by 张彦涛 on 15/6/30.
//
//

#import <UIKit/UIKit.h>
#import "UIImage+resized.h"
#import "ChatChannel.h"
typedef enum : NSUInteger {
    ChatVCShowTableType_left= 0,
    ChatVCShowTableType_right=1
} ChatVCShowTableType;
@interface ChattingVC : UIViewController
 
@property (nonatomic,strong) ChatChannel * countryChannel;
@property (nonatomic,strong) ChatChannel * allianceChannel;
@property (nonatomic,assign) ChatVCShowTableType chatVCShowTableType;
-(void)deallocAny;
@end
