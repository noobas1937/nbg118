//
//  ExploreMailContents.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "ExploreMailContents.h"
#import "MJExtension.h"
@implementation ExploreMailContents
-(instancetype)init
{
    self = [super init];
    if (self) {
        [ExploreMailContents setupObjectClassInArray:^NSDictionary *{
            return @{
                     @"reward" : @"ExporeRewardParams"
                     };
        }];
    }
    return self;
}
@end
