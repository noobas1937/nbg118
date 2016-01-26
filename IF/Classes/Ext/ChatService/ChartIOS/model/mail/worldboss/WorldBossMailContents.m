//
//  WorldBossMailContents.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "WorldBossMailContents.h"
#import "MJExtension.h"

@implementation WorldBossMailContents
-(instancetype)init
{
    self = [super init];
    if (self) {
        [WorldBossMailContents setupObjectClassInArray:^NSDictionary *{
            return @{
                     @"attList" : @"AttListParams"
                     };
        }];
    }
    return self;
}
@end
