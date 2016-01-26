//
//  CellView.m
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "CellView.h"
#import "YYLabel.h"


@implementation InsetsLabel

@synthesize insets = _insets;

- (id)initWithFrame:(CGRect)frame andInsets:(UIEdgeInsets)insets {
    self = [super initWithFrame:frame];
    if(self) {
        self.insets = insets;
    }
    return self;
}

- (id)initWithInsets:(UIEdgeInsets)insets {
    self = [super init];
    if(self) {
        self.insets = insets;
    }
    return self;
}

- (void)drawTextInRect:(CGRect)rect {
    return [super drawTextInRect:UIEdgeInsetsInsetRect(rect, self.insets)];
}

@end


@implementation CellView

-(instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        [self _initView];
    }
    return self;
}

-(void)_initView{
    
    
    self.backgroundColor =[UIColor clearColor];
    _timeLabel =[[InsetsLabel alloc]initWithInsets:UIEdgeInsetsMake(-3, 5, 3, 5)];
    
    _timeLabel.backgroundColor =[UIColor clearColor];
    
    _timeLabel.numberOfLines = 1;
    _timeLabel.textAlignment = NSTextAlignmentCenter;
    _timeLabel.lineBreakMode = NSLineBreakByWordWrapping;
    _timeLabel.layer.masksToBounds = YES;
    _timeLabel.layer.cornerRadius = 5;
//    [_timeLabel sizeToFit];
    [self addSubview:_timeLabel];
    
    _bodyView =[[UIView alloc]initWithFrame:CGRectZero];
    _bodyView.backgroundColor =[UIColor clearColor];
    [self addSubview:_bodyView];
    _nameLabel= [[UILabel alloc]initWithFrame:CGRectZero];
    [_bodyView addSubview:_nameLabel];
    _avatarbackView =[[UIImageView alloc]initWithFrame:CGRectZero];
    _avatarbackView.backgroundColor =[UIColor clearColor];
    _avatarbackView.image =[UIImage imageNamed:@"icon_kuang"];
    [_bodyView addSubview:_avatarbackView];
    _avatarView = [[UIImageView alloc]initWithFrame:CGRectZero];
    _avatarView.userInteractionEnabled = YES;
    [_bodyView insertSubview:_avatarView aboveSubview:_avatarbackView];
//    [self insertSubview:_avatarView aboveSubview:_avatarbackView];
    _reSendButton =[UIButton buttonWithType:UIButtonTypeCustom];
    [_reSendButton setImage:[UIImage imageNamed:@"sendfailed"] forState:UIControlStateNormal];
    [_bodyView addSubview:_reSendButton];
    _sendStatusActivityView =[[UIActivityIndicatorView alloc]initWithFrame:CGRectZero];
    _sendStatusActivityView.activityIndicatorViewStyle = UIActivityIndicatorViewStyleGray;
    [_bodyView addSubview:_sendStatusActivityView];
    

}
@end
