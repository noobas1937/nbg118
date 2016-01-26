//
//  ArmsParams.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "ArmsParams.h"
#import "MJExtension.h"

@implementation ArmsParams
-(instancetype)init
{
    self = [super init];
    if (self) {
        [ArmsParams setupReplacedKeyFromPropertyName:^NSDictionary *{
            return @{
                     @"ID" : @"id"
                     };
        }];
    }
    return self;
}
@end
