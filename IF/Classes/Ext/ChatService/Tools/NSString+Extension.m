//
//  NSString+Extension.m
//   
//
//  Created by apple on 14-4-2.
//  Copyright (c) 2014年 itcast. All rights reserved.
//

#import "NSString+Extension.h"
#import <CommonCrypto/CommonDigest.h>
#import <CommonCrypto/CommonCryptor.h>

@implementation NSString (Extension)
- (CGSize)sizeWithFont:(UIFont *)font maxSize:(CGSize)maxSize
{
    NSDictionary *attrs = @{NSFontAttributeName : font};
    return [self boundingRectWithSize:maxSize options:NSStringDrawingUsesLineFragmentOrigin attributes:attrs context:nil].size;
}

-(NSString *)md5

{
    
    const char *cStr = [self UTF8String];
    
    unsigned char result[CC_MD5_DIGEST_LENGTH];
    
    CC_MD5( cStr, strlen(cStr), result );
    
    return [NSString
            
            stringWithFormat: @"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
            
            result[0], result[1],
            
            result[2], result[3],
            
            result[4], result[5],
            
            result[6], result[7],
            
            result[8], result[9],
            
            result[10], result[11],
            
            result[12], result[13],
            
            result[14], result[15]
            
            ];
    
}


-(BOOL)stringRepeatInArray:(NSArray *)vArray {
    NSString *tempString = nil;
    for (NSString *uid  in vArray) {
        if  ([uid isEqualToString:self]){
            tempString = self;
            break;
        }
    }
    if (tempString ) {
        return YES;
    }else{
        return NO;
    }
}


@end
