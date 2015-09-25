//
//  Comment.h
//  myTalk
//
//  Created by LYoung on 15/1/4.
//  Copyright (c) 2015年 LYoung. All rights reserved.
//  图片拉伸

#import <UIKit/UIKit.h>

@interface UIImage (resized)


/**
 *  返回能够自由拉伸的不变形的图片
 *
 *  @param name 文件名
 *
 *  @return 图片
 */
+ (UIImage *)resizedImage:(NSString *)name;
+ (UIImage *)resizedImage:(NSString *)name leftScale:(CGFloat)leftScale topScale:(CGFloat)topScale;


+ (UIImage *)resizableImage:(NSString *)name;
@end
