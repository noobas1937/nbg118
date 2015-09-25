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

+(NSString *)gettingCustomHeadPic:(NSString *)uid int:(int)picVer
{
    std::string cc_uid = [uid UTF8String];
    std::string cc_result = CCCommonUtils::getCustomPicUrl(cc_uid,picVer);
    return [NSString stringWithUTF8String:cc_result.c_str()];
}

+(NSString *)gettingCurrentAccountUserUid{
    string uidString = GlobalData::shared()->playerInfo.uid;
    return [NSString stringWithUTF8String:uidString.c_str()];
}
@end
