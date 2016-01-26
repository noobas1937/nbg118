//
//  DefenceParams.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "DefenceParams.h"
#import "MJExtension.h"
@implementation DefenceParams
-(instancetype)init
{
    self = [super init];
    if (self) {
        [DefenceParams setupObjectClassInArray:^NSDictionary *{
            return @{
                     @"arms" : @"ArmsParams"
                     };
        }];
    }
    return self;
}
@end
