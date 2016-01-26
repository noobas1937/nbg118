//
//  ResourceHelpMailContents.m
//  IF
//
//  Created by 马纵驰 on 15/9/26.
//
//

#import "ResourceHelpMailContents.h"
#import "MJExtension.h"

@implementation ResourceHelpMailContents
-(instancetype)init
{
    self = [super init];
    if (self) {
        [ResourceHelpMailContents setupObjectClassInArray:^NSDictionary *{
            return @{
                     @"reward" : @"RewardParams"
                     };
        }];
        
    }
    return self;
}
@end
