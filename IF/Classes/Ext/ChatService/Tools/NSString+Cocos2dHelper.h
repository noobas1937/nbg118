//
//  NSString+Cocos2dHelper.h
//  IF
//
//  Created by 张彦涛 on 15/8/21.
//
//

#import <Foundation/Foundation.h>

@interface NSString (Cocos2dHelper)
//计算时间字符串
+(NSString *)stringWithTimeFormatWithCreateTime:(long)vCreateTimeData;

//返回多语言字符串
+(NSString *)stringWithMultilingualWithKey:(NSString *)vKeyString ;
//返回带插入字符的多语言字符串
+(NSString *)stringWithMultilingualWithKey:(NSString *)vKeyString andWithPaddingString1:(NSString *)vPaddingString;

//获取自定义头像URL
+(NSString *)gettingCustomHeadPic:(NSString *)uid int:(int)picVer;


+(NSString *)gettingCurrentAccountUserUid;
@end
