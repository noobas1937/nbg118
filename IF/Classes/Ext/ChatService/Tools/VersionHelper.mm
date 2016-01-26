//
//  VersionHelper.m
//  IF
//
//  Created by 张彦涛 on 15/9/2.
//
//

#import "VersionHelper.h"

@implementation VersionHelper


+(CGFloat)versonStringToversonFloat:(NSString *)versionStr{
    NSArray *array=[versionStr componentsSeparatedByString:@"."];
    CGFloat floatVersion= 0.0;
    NSString *stringVersion = @"";
    for (int xx = 0; xx<array.count; xx++) {
        if (xx == 0) {
            stringVersion = [stringVersion stringByAppendingString:[array objectAtIndex:xx]];
            stringVersion = [stringVersion stringByAppendingString:@"."];
        }else{
            NSString *tempString = [array objectAtIndex:xx];
            if (tempString.length==1) {
                tempString = [@"0" stringByAppendingString:tempString];
            }
            stringVersion = [stringVersion stringByAppendingString:tempString];
        }
    }
    floatVersion = [stringVersion floatValue];
    DVLog(@"%@转换的字符串 ：%f",versionStr,floatVersion);
    return floatVersion;
}


+(NSString *)currentVersion{
    NSDictionary *infoDic = [[NSBundle mainBundle] infoDictionary];
    NSString *version = [infoDic objectForKey:@"CFBundleShortVersionString"];
    return version;
}


+(BOOL)compareVersionWithKeyVersionStr:(NSString *)vKeyString {
    CGFloat keyVersion = [self versonStringToversonFloat:vKeyString];
    CGFloat currentVersion = [self versonStringToversonFloat:[self currentVersion]];
    if (currentVersion >= keyVersion) {
        return YES;
    }else{
        return NO;
    }
}
@end
