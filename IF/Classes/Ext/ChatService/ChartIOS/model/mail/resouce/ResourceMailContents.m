//
//  JsonToMailResourceData.m
//  IF
//
//  Created by 马纵驰 on 15/9/11.
//
//

#import "ResourceMailContents.h"
#import "RewardParams.h"
#import "ChatServiceController.h"
#import "DropParams.h"

@implementation ResourceMailContents

-(instancetype)init
{
    self = [super init];
    if (self) {
        [ResourceMailContents setupObjectClassInArray:^NSDictionary *{
            return @{
                     @"reward" : @"RewardParams",
                     @"drop" : @"DropParams"
                     };
        }];
        
    }
    return self;
}

@end
