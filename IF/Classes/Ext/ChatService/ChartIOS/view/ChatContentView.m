//
//  ChatContentView.m
//  气泡
//
//  Created by zzy on 14-5-13.
//  Copyright (c) 2014年 zzy. All rights reserved.
//
#define kContentStartMargin 5
#import "ChatContentView.h"
#import "NSMsgItem.h"
#import "ServiceInterface.h"
@implementation ChatContentView

- (id)init
{
    self = [super init];
    if (self) {
    
        self.backImageView=[[UIImageView alloc]init];
        [self addSubview:self.backImageView];
        
        self.contentLabel=[[KZLinkLabel alloc]initWithFrame:CGRectMake(15, 5, kMainScreenWidth *0.6, 0)];
        self.contentLabel.automaticLinkDetectionEnabled = YES;
//        [self.contentLabel setMaxWidth:kMainScreenWidth *0.6];
       
        self.contentLabel.backgroundColor = [UIColor clearColor];
        self.contentLabel.numberOfLines = 0;
        self.contentLabel.linkColor = [UIColor blueColor];
        self.contentLabel.linkHighlightColor = [UIColor orangeColor];
        self.contentLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].chatFontSize];
        [self addSubview:self.contentLabel];
        
        //添加文字结尾信息
        self.endStrLabel = [[UILabel alloc] init];
        self.endStrLabel.font = [UIFont boldSystemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].endtimeFontSize];
        self.endStrLabel.textAlignment = UITextAlignmentRight;
        self.endStrLabel.textColor = [UIColor grayColor];
        self.endStrLabel.backgroundColor =[UIColor clearColor];
        [self addSubview:self.endStrLabel];
        
        UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tapPress:)];
        [tap setCancelsTouchesInView:NO];
        [self addGestureRecognizer:tap];
        
        
        
    }
    return self;
}
-(void)setFrame:(CGRect)frame
{
    [super setFrame:frame];
    
}

-(void)tapPress:(UITapGestureRecognizer *)tapPress{


    if([self.chatContentViewdelegate respondsToSelector:@selector(chatContentViewTapPress:content:)]){
        
        [self.chatContentViewdelegate chatContentViewTapPress:self content:self.contentLabel.text];
    }
    
    [self changeBackgroundImage];
}

-(void) changeBackgroundImage
{
    //定义数组，存放所有图片对象
    NSArray *images=[NSArray arrayWithObjects:self.chatImageUp,self.chatImageDown,self.chatImageUp, nil];
    self.backImageView.animationImages = images;
    //切换动作的时间3秒，来控制图像显示的速度有多快，
    self.backImageView.animationDuration = 0.4;
    //动画的重复次数，想让它无限循环就赋成0
    self.backImageView.animationRepeatCount = 1;
    //开始动画
    [self.backImageView startAnimating];
}
//-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
////    self.backImageView.image = self.chatImageDown;
//    
//    self.backImageView.image = self.chatImageDown;
//    CATransition *transition = [CATransition animation];
//    transition.duration = 0.1f;
//    transition.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
//    transition.type = kCATransitionFade;
//    [self.backImageView.layer addAnimation:transition forKey:nil];
//}
//
//-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
//    self.backImageView.image = self.chatImageUp;
//}
//
//-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event{
//    self.backImageView.image = self.chatImageUp;
//}
//
//-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
////    UIImage *image = self.chatImageUp;
////    self.backImageView.image = self.chatImageUp;
//    
//    self.backImageView.image = self.chatImageUp;
//    CATransition *transition = [CATransition animation];
//    transition.duration = 0.1f;
//    transition.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
//    transition.type = kCATransitionFade;
//    [self.backImageView.layer addAnimation:transition forKey:nil];
//    
//}

@end
