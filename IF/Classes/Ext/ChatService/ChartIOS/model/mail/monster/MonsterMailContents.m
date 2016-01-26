//
//  MonsterMailContents.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "MonsterMailContents.h"
#import "MJExtension.h"

@implementation MonsterMailContents
-(instancetype)init
{
    self = [super init];
    if (self) {
        [MonsterMailContents setupObjectClassInArray:^NSDictionary *{
            return @{
                     @"rateReward" : @"RateRewardParams"
                     };
        }];
    }
    return self;
}
@end
