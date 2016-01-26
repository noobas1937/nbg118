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

//返回带插入字符的多语言字符串
+(NSString *)stringWithMultilingualWithKey:(NSString *)vKeyString andWithPaddingString1:(NSString *)vPaddingString1 andWithPaddingString2:(NSString *)vPaddingString2;

+(NSString *)stringWithMultilingualWithKey:(NSString *)vKeyString andWithPaddingString1:(NSString *)vPaddingString1 andWithPaddingString2:(NSString *)vPaddingString2 andWithPaddingString3:(NSString*)vPaddingString3;

//获取自定义头像URL
+(NSString *)gettingCustomHeadPic:(NSString *)uid intValue:(int)picVer;


+(NSString *)gettingCurrentAccountUserUid;

+(NSString*) gettingRoundFormatNumber:(int) num;
/**
 *  根据长度格式化字符串
 *
 *  @param length 长度
 *
 *  @return 截取后的字符串 + 。。。
 */
+(NSString*)formatStrByStr:(NSString*)str ByLength:(int)length;
/**
 *  判断字符串是否相同 相同打印true
 *
 */
-(void) isSamePrintStrByStr:(NSString*) str;

/**
 *  替换资源名称  原逻辑 资源图片 + 数量  现逻辑 资源名称 + 数量
 *
 *   107511=木头
 *   107512=秘银
 *   107513=铁
 *   107514=粮食
 *
 *  @param name
 */
-(NSString*) replaceResourceName;
/**
 *  对数字字符串进行千位分割
 */
-(NSString*) cutBit;
/**
 *  判断字符串是否是Integer类型
 */
- (BOOL)isPureInteger;
/**
 *  字符串转字典
 *
 */
+ (NSDictionary *)dictionaryWithJsonString:(NSString *)jsonString;
/** 判断当前等级是否大于服务器开关配置等级 */
+(BOOL)isSelfUserLeverLimitToServerLeverSetter:(int )vServerLever ;
/** 返回当前语言类型 */
+(NSString *)currentLanguageType;
@end
