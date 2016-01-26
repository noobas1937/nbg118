//
//  UIColor+MakeColor.h
//  myTalk
//
//  Created by ZhangYantao on 15/1/9.
//  Copyright (c) 2015年 ZhangYantao. All rights reserved.
//



@interface UIColor (MakeColor)
/**
 *  输入10进制色值，返回UIcolor
 *

 *
 *  @return UIcolor
 */
+ (UIColor *) colorWith256Red:(NSUInteger)red green:(NSUInteger)green blue:(NSUInteger)blue alpha:(CGFloat)alpha;
/**
 *  输入16进制数据
 *
 *  @param hex 0xffffff
 *
 *  @return UIColor
 */
+ (UIColor *) colorWithHex:(long)hex;
@end
