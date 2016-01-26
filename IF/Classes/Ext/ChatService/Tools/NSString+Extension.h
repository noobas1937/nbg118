//
//  NSString+Extension.h
//
//
//  Created by apple on 14-4-2.
//  Copyright (c) 2014年 itcast. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
@interface NSString (Extension)
/**
 *  返回字符串所占用的尺寸
 *
 *  @param font    字体
 *  @param maxSize 最大尺寸  ,不定的用 MAXFLOAT
 */
- (CGSize)sizeWithFont:(UIFont *)font maxSize:(CGSize)maxSize;

/**
 *  MD5
 *
 *  @return 返回md5后的字符串
 */
-(NSString *)md5;

/**
 *  判断当前字符串在不在数组中
 *
 *  @param vArray 数组字符串
 *
 *  @return 存在返回yes，不存在返回no
 */
-(BOOL)stringRepeatInArray:(NSArray *)vArray ;
@end
