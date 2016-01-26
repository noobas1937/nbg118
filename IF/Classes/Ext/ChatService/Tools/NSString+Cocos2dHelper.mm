//
//  NSString+Cocos2dHelper.m
//  IF
//
//  Created by 张彦涛 on 15/8/21.
//
//

#import "NSString+Cocos2dHelper.h"
#import "GlobalData.h"
#import "CCCommonUtils.h"
#import "LocalController.h"
#import <string.h>
#import "md5.h"

#if DEBUG
#define MAIL_UID        			@"a8b21b1f7cb94ec5b55be02d81e186af"
#endif
@implementation NSString (Cocos2dHelper)
+(NSString *)stringWithTimeFormatWithCreateTime:(long)vCreateTimeData{
 
    auto dt = GlobalData::shared()->getWorldTime()-GlobalData::shared()->changeTime(vCreateTimeData);
    string timestr = "";
    int timedt = 0;
    
    if (dt>=24*3600*2) {
        time_t timeT = vCreateTimeData;
        timestr = CCCommonUtils::timeStampToMD(timeT);
    }
    else {
        if(dt>=24*60*60){
            timedt =dt/(24*60*60);
            timestr = CC_ITOA(timedt);
            timestr.append(_lang("105592"));
        }else if(dt>=60*60){
            timedt =dt/(60*60);
            timestr = CC_ITOA(timedt);
            timestr.append(_lang("105591"));
        }else if(dt>=60){
            timedt =dt/60;
            timestr = CC_ITOA(timedt);
            timestr.append(_lang("105590"));
        }else{
            timestr ="1";
            timestr.append(_lang("105590"));
        }
        timestr.append(" ");
        timestr.append(_lang("105593"));
    }
    
 return    [NSString stringWithUTF8String:timestr.c_str()];

}

+(NSString *)stringWithMultilingualWithKey:(NSString *)vKeyString {
    string keyString = [vKeyString UTF8String];
    string resultString = _lang(keyString);
    return [NSString stringWithUTF8String:resultString.c_str()];
}
+(NSString *)stringWithMultilingualWithKey:(NSString *)vKeyString andWithPaddingString1:(NSString *)vPaddingString{
    string keyString = [vKeyString UTF8String];
    string paddingString1 = [vPaddingString UTF8String];
    string resultString = _lang_1(keyString,paddingString1.c_str());
    return [NSString stringWithUTF8String:resultString.c_str()];
}

+(NSString *)stringWithMultilingualWithKey:(NSString *)vKeyString andWithPaddingString1:(NSString *)vPaddingString1 andWithPaddingString2:(NSString *)vPaddingString2
{
    string keyString = [vKeyString UTF8String];
    string paddingString1 = [vPaddingString1 UTF8String];
    string paddingString2 = [vPaddingString2 UTF8String];
    string resultString = _lang_2(keyString,paddingString1.c_str(),paddingString2.c_str());
    return [NSString stringWithUTF8String:resultString.c_str()];
}

+(NSString *)stringWithMultilingualWithKey:(NSString *)vKeyString andWithPaddingString1:(NSString *)vPaddingString1 andWithPaddingString2:(NSString *)vPaddingString2 andWithPaddingString3:(NSString*)vPaddingString3
{
    string keyString = [vKeyString UTF8String];
    string paddingString1 = [vPaddingString1 UTF8String];
    string paddingString2 = [vPaddingString2 UTF8String];
    string paddingString3 = [vPaddingString3 UTF8String];
    
    string resultString = _lang_3(keyString,paddingString1.c_str(),paddingString2.c_str(),paddingString3.c_str());
    return [NSString stringWithUTF8String:resultString.c_str()];
}

+(NSString *)gettingCustomHeadPic:(NSString *)uid intValue:(int)picVer
{
    if (uid.length== 0){
        return @"";
    }
    NSString *md5Stirng = [NSString stringWithFormat:@"%@_%d",uid,picVer];
    
    string md5Str = [md5Stirng UTF8String];
    MD5 md5;
    md5.update(md5Str.c_str(), md5Str.length());
    
    string tempStr = [uid UTF8String];
     string url = "http://cok.eleximg.com/cok/img/";
    url.append(tempStr.substr(tempStr.length()-6,tempStr.length()));
    url.append("/");
    url.append(md5.toString().c_str()).append(".jpg");
    
    
//     const char * cc_uid = [uid UTF8String];
//     string cc_result = CCCommonUtils::getCustomPicUrl1(cc_uid,picVer);
    return [NSString stringWithUTF8String:url.c_str()];
}

+(NSString *)gettingCurrentAccountUserUid{
    string uidString = GlobalData::shared()->playerInfo.uid;
    return [NSString stringWithUTF8String:uidString.c_str()];
}

+(NSString*) gettingRoundFormatNumber:(int) num
{
    NSString *numStr = [NSString stringWithFormat:@"%d",num];
    
    NSString *result = @"";
    if(num<0)
    {
        num = -num;
        result = @"-";
    }
    
    if(num>=0 && num<1000)
    {
        return [NSString stringWithFormat:@"%@%@",result,numStr];
    }
    else if(num >= 1000 && num < 1000000)
    {
        float number = roundf(num/1000.0*100) / 100;
        NSString *numberStr = [NSString stringWithFormat:@"%.2f",number];
        if(number>0)
            return [NSString stringWithFormat:@"%@%@%@",result,numberStr,@"k"];
    }
    else if(num >=1000000 && num<1000000000)
    {
        
        float number = roundf(num/1000000.0*100) / 100;
        NSString *numberStr = [NSString stringWithFormat:@"%.2f",number];
        if(number>0)
            return [NSString stringWithFormat:@"%@%@%@",result,numberStr,@"M"];
    }
    else if(num >=1000000000)
    {
        int number = roundf(num/1000000000.0*100) / 100;
        NSString *numberStr = [NSString stringWithFormat:@"%.2f",number];
        if(number>0)
            return [NSString stringWithFormat:@"%@%@%@",result,numberStr,@"G"];
            }
    return [NSString stringWithFormat:@"%@%@",result,numStr];
}

+(NSString*)formatStrByStr:(NSString*)str ByLength:(int)length;
{
    NSString *newStr = [str substringWithRange:NSMakeRange(0, length)];
    newStr = [newStr stringByAppendingString:@"..."];
    return newStr;
}

-(void) isSamePrintStrByStr:(NSString*) str
{
    if ([self isEqualToString:str]) {
        DVLog(@"mzcMail测试str相等");
    }
}

-(NSString*) replaceResourceName
{
    if ([self isEqualToString:@"ui_food.png"]) {
        return [NSString stringWithMultilingualWithKey:@"107514"];
    }else if ([self isEqualToString:@"ui_silver.png"]) {
        return [NSString stringWithMultilingualWithKey:@"107513"];
    }else if ([self isEqualToString:@"ui_iron.png"]) {
        return [NSString stringWithMultilingualWithKey:@"107512"];
    }else if ([self isEqualToString:@"ui_wood.png"]) {
        return [NSString stringWithMultilingualWithKey:@"107511"];
    }else{
        return @"";
    }
}

-(NSString*) cutBit
{
    if (![self isPureInteger]) {
        return self;
    }
    NSNumberFormatter* numberFormatter = [[NSNumberFormatter alloc] init];
    [numberFormatter setFormatterBehavior: NSNumberFormatterBehavior10_4];
    [numberFormatter setNumberStyle: NSNumberFormatterDecimalStyle];
    NSString *numberString = [numberFormatter stringFromNumber: [NSNumber numberWithInteger: [self longLongValue]]];
    return numberString;
}

- (BOOL)isPureInteger
{
    NSScanner *scan = [NSScanner scannerWithString:self];
    NSInteger val;
    return [scan scanInteger:&val] && [scan isAtEnd];
}

+ (NSDictionary *)dictionaryWithJsonString:(NSString *)jsonString {
    if (jsonString == nil) {
        return nil;
    }
    
    NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
    NSError *err;
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                        options:NSJSONReadingMutableContainers
                                                          error:&err];
    if(err) {
        NSLog(@"json解析失败：%@",err);
        return nil;
    }
    return dic;
}

+(BOOL)isSelfUserLeverLimitToServerLeverSetter:(int )vServerLever {
    int curentLever = GlobalData::shared()->playerInfo.level;
    if  (vServerLever<=curentLever){
         return YES;
    }else{
         return NO;
    }
   
}
+(NSString *)currentLanguageType{
    return  [NSString stringWithUTF8String: LocalController::shared()->getLanguageFileName().c_str()] ;
    
}
@end
