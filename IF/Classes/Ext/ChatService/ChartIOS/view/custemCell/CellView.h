//
//  CellView.h
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import <UIKit/UIKit.h>

 

@interface InsetsLabel : UILabel

@property(nonatomic) UIEdgeInsets insets;

- (id)initWithFrame:(CGRect)frame andInsets:(UIEdgeInsets) insets;
- (id)initWithInsets:(UIEdgeInsets) insets;

@end


@interface CellView : UIView

@property (strong,nonatomic) InsetsLabel *timeLabel;

@property (nonatomic,strong)   UIView *bodyView;
/** 名字 */
@property (strong, nonatomic) UILabel *nameLabel;
/** 头像 */
@property (strong, nonatomic) UIImageView *avatarView;
/** 头像背景图片 */
@property (strong,nonatomic) UIImageView *avatarbackView;
/** 重发按钮 */
@property (strong, nonatomic) UIButton *reSendButton;
/** 发送菊花 */
@property (nonatomic, strong) UIActivityIndicatorView *sendStatusActivityView;


@end
