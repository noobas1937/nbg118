//
//  Cellconfig.m
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/13.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "Cellconfig.h"
#import "UIColor+MakeColor.h"
@implementation Cellconfig


+ (Cellconfig *)sharedCellConfig
{
    static Cellconfig *sharedCellConfig = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedCellConfig = [[Cellconfig alloc] _init];
    });
    return sharedCellConfig;
}

-(instancetype)_init
{
    self = [super init];
    if (self) {
        [self _settingConfig];
    }
    return self;
}

-(void)_settingConfig{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
        self.resendButtonSize = 20;
        self.activitySize = 20;
        self.bubbleMaxWidth = kMainScreenWidth * 0.59;
        self.leftBubbleMargin = UIEdgeInsetsMake(10, 20, 10, 15);
        self.rightBubbleMargin = UIEdgeInsetsMake(10, 15, 10, 20);
        
//       self.leftBubbleMargin = UIEdgeInsetsMake(8, 20, 8, 10);
//        self.rightBubbleMargin = UIEdgeInsetsMake(8, 10, 8, 20);
        self.avatarMargin = 10;
        
        self.timeLabelFont = [UIFont systemFontOfSize:12.0f];
        self.timeLabelHeight = 30;
        self.timeLabelTextColor = [UIColor whiteColor];
        
        self.timeLabelbackColor =[UIColor colorWith256Red:146 green:126 blue:102 alpha:0.5];
 
        self.avatarSize = 50;
        self.avatarCornerRadius = 5;
        
        self.messageNameColor =[UIColor colorWithRed:72/255.0 green:44/255.0 blue:19/255.0 alpha:1];
        self.messageNameFont = [UIFont boldSystemFontOfSize:13];
        self.messageNameHeight = 15;

        self.contentsTextSize = 14;
        self.contentsTextFont =[UIFont systemFontOfSize:14];
        self.contentsTextColor =[UIColor blackColor];
        
        self.translatExplainFont =[UIFont systemFontOfSize:11];
        self.translatExplainTextColor =[UIColor grayColor];
        
        
        self.chatRoomSystemTextSize = [UIFont systemFontOfSize:12];
        
         self.redPacketBubbleWith = kMainScreenWidth *0.5;
        self.redPacketBubbleHeight = 50;
        self.redSubTextFont =[UIFont systemFontOfSize:12];
        self.redTimeTextFont =[UIFont systemFontOfSize:12];
        self.redPacketLeftBubbleMargin = UIEdgeInsetsMake(8, 25, 8, 10);
        self.redPacketRightBubbleMargin = UIEdgeInsetsMake(8, 10, 8, 25);
        
    }else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad){
        self.resendButtonSize = 20;
        self.activitySize = 20;
        self.bubbleMaxWidth = kMainScreenWidth * 0.55;
        self.leftBubbleMargin = UIEdgeInsetsMake(10, 20, 10, 15);
        self.rightBubbleMargin = UIEdgeInsetsMake(10, 15, 10, 20);
        self.avatarMargin = 10;
        
        self.timeLabelFont = [UIFont systemFontOfSize:17.0];
        self.timeLabelHeight = 50;
        self.timeLabelTextColor = [UIColor whiteColor];
        
        self.timeLabelbackColor =[UIColor colorWith256Red:146 green:126 blue:102 alpha:0.5];
        
        self.avatarSize = 80;
        self.avatarCornerRadius = 10;
        
        self.messageNameColor =[UIColor colorWithRed:72/255.0 green:44/255.0 blue:19/255.0 alpha:1];
        self.messageNameFont =[UIFont boldSystemFontOfSize:20];
        self.messageNameHeight = 30;
        
        self.contentsTextSize = 25;
        self.contentsTextFont =[UIFont systemFontOfSize:25];
        self.contentsTextColor =[UIColor blackColor];
        
        self.translatExplainFont =[UIFont systemFontOfSize:17];
        self.translatExplainTextColor =[UIColor grayColor];
        
        
        self.chatRoomSystemTextSize = [UIFont systemFontOfSize:17];
        
        self.redPacketBubbleWith = 350;
        self.redPacketBubbleHeight = 80;
        self.redSubTextFont =[UIFont systemFontOfSize:15];
        self.redTimeTextFont =[UIFont systemFontOfSize:15];
        self.redPacketLeftBubbleMargin = UIEdgeInsetsMake(8, 25, 8, 10);
        self.redPacketRightBubbleMargin = UIEdgeInsetsMake(8, 10, 8, 25);
    }
    
    
}



@end
